#include "pch.h"
#include "Window.h"
#include <backends/imgui_impl_win32.cpp>
#include "Input.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include <comdef.h>

Window::Window(const WindowProps& windowProps)
	: m_hInstance(GetModuleHandle(nullptr)), // Gets the instance handle of the current module
	  m_CLASSNAME(L"Calliterra"),
	  m_Timer(50),
	  m_WindowProps(windowProps)
{
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = m_CLASSNAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = MessageSetup; 

	RegisterClass(&wndClass);

	DWORD style = WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;

	// Calculate center of screen to place window
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowProps.Width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowProps.Height / 2;

	m_WindowProps.PosX = centerScreenX;
	m_WindowProps.PosY = centerScreenY;

	// Calculate the required size of the window rectangle based on desired client area size
	RECT windowRegion = { centerScreenX, centerScreenY, centerScreenX + windowProps.Width, centerScreenY + windowProps.Height };
	AdjustWindowRectEx(&windowRegion, WINDOWED_STYLE, FALSE, 0);

	m_hWnd = CreateWindowEx(
		0,
		m_CLASSNAME,
		m_CLASSNAME,
		WINDOWED_STYLE,
		windowRegion.left,
		windowRegion.top,
		windowRegion.right - windowRegion.left, // Adjusted width
		windowRegion.bottom - windowRegion.top, // Adjusted height
		NULL,
		NULL,
		m_hInstance,
		this	
	);

	m_LastCursorPos = { centerScreenX + windowProps.Width / 2, centerScreenY + windowProps.Height / 2};

	// Register mouse raw input device
	RAWINPUTDEVICE rawInputDevice;
	rawInputDevice.usUsagePage = 0x01; // Mouse page
	rawInputDevice.usUsage = 0x02; // Mouse usage
	rawInputDevice.dwFlags = 0;
	rawInputDevice.hwndTarget = m_hWnd;
	ASSERT_VERIFY(RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)));

	auto lambda = [this](DeltaTime dt)
		{
			std::string title = std::format("Calliterra  FPS: {0:4}  Frame Time: {1:.2f}ms", static_cast<int>(1 / dt.GetSeconds()), dt.GetMilliseconds());
			SetWindowTextA(m_hWnd, title.c_str());
		};

	m_Timer.SetCallback(lambda);

	ImGui_ImplWin32_Init(m_hWnd);

	DisableCursor();
	m_GraphicsContext = GraphicsContext::Create(&m_hWnd, m_WindowProps);
	m_GraphicsContext->Init();

}

Window::~Window()
{
	UnregisterClass(m_CLASSNAME, m_hInstance);
}

LRESULT CALLBACK Window::MessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM)
{
	if (uMsg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lPARAM);
		Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			// Error Logging here
			DWORD error = GetLastError();
			LOG_ERROR("{0} CreateWindowEx failed! pWindow is null.", error);
			std::exit(error);
		}

		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

		// Set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(*Window::MessageRedirect));

		// forward message to window class handler
		return pWindow->MessageHandler(hwnd, uMsg, wParam, lPARAM);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hwnd, uMsg, wParam, lPARAM);

}

LRESULT WINAPI Window::MessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM)
{

	// Retrieve ptr to window class
	Window* const pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	// Foward message to window class message handler
	return pWindow->MessageHandler(hwnd, uMsg, wParam, lPARAM);
}

LRESULT Window::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int wmSizeMessageCount = 0;
	static int wmMoveMessageCount = 0;
	static uint16_t oldWindowWidth;
	static uint16_t oldWindowHeight;

	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
	{
		return true;
	}
	const auto& imIO = ImGui::GetIO();

	switch (uMsg)
	{
	// --------------- Window Closing ---------------- //
	case WM_CLOSE:
	{
		WindowCloseEvent event;
		m_EventCallback(event);

		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// --------------- Window Events ---------------- //
	case WM_ACTIVATE:
	{

		if (LOWORD(wParam) == WA_ACTIVE)
		{
			m_Active = true;
		}
		else
		{
			m_Active = false;
			FreeCursor();
			ShowCursor();
		}
		break;
	}
	case WM_SIZE:
	{
		m_WindowProps.Width = LOWORD(lParam);
		m_WindowProps.Height = HIWORD(lParam);
		m_WindowProps.AspectRatio = (float)m_WindowProps.Width / (float)m_WindowProps.Height;

		// Upon window creation there is an initial WM_SIZE message.
		// To prevent calling m_EventCallback before setup is complete
		// We will break early if it's the first WM_SIZE message.
		wmSizeMessageCount++;
		if (wmSizeMessageCount == 1)
			break;

		if (m_Resizing)
			break;

		if (wParam == SIZE_MINIMIZED)
		{
			m_Minimized = true;
		}
		else
		{
			RECT windowRect;
			GetWindowRect(m_hWnd, &windowRect);

			m_WindowProps.PosX = windowRect.left;
			m_WindowProps.PosY = windowRect.top;

			WindowResizeEvent event(m_WindowProps.Width, m_WindowProps.Height);
			m_EventCallback(event);

			m_GraphicsContext->OnWindowResize();
		}
		break;
	}
	// Sent when the user grabs the resize bars
	case WM_ENTERSIZEMOVE:
	{
		m_Resizing = true;

		oldWindowWidth = m_WindowProps.Width;
		oldWindowHeight = m_WindowProps.Height;
		break;
	}
	// Sent when the user releases the resize bars or releases the title bar
	case WM_EXITSIZEMOVE:
	{
		m_Resizing = false;

		RECT windowRect;
		GetWindowRect(m_hWnd, &windowRect);

		m_WindowProps.PosX = windowRect.left;
		m_WindowProps.PosY = windowRect.top;

		// Only create a window resize event when the window size is different from when
		// the user first grabbed the resize bars
		if (oldWindowWidth != m_WindowProps.Width || oldWindowHeight != m_WindowProps.Height)
		{
			WindowResizeEvent event(m_WindowProps.Width, m_WindowProps.Height);
			m_EventCallback(event);

			m_GraphicsContext->OnWindowResize();

		}

		break;
	}
	// Catch this message to prevent the window from becoming too small
	case WM_GETMINMAXINFO: 
	{
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 640;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 360;
		break;
	}
	case WM_INPUT:
	{
		if (!Input::IsMouseRawInputEnabled())
		{
			break;
		}
		UINT size = 0;

		// Get size of input data
		ASSERT_VERIFY(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) != -1);
		m_RawBuffer.resize(size);
		ASSERT_VERIFY(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, m_RawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size);

		auto& rawInput = reinterpret_cast<const RAWINPUT&>(*m_RawBuffer.data());
		if (rawInput.header.dwType == RIM_TYPEMOUSE) 
		{
			Input::OnRawMouseDelta(rawInput.data.mouse.lLastX, rawInput.data.mouse.lLastY);
		}
		break;
	}
	// --------------- Keyboard Messages -------------- //
	case WM_SYSCOMMAND:
	{
		// Disable the windows menu when pressing the alt key
		if ((wParam & 0xFFF0) == SC_KEYMENU)
		{
			return S_OK;
		}
		
		break;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if (imIO.WantCaptureKeyboard)
		{
			break;
		}

		KeyCode keycode = static_cast<KeyCode>(wParam);

		KeyPressedEvent	event(keycode, Input::m_KeyRepeatCount[keycode]);
		m_EventCallback(event);

		Input::OnKeyPressed(keycode);

		Input::m_KeyRepeatCount[keycode]++;

		if (Input::IsKeyPressed(VK_MENU) && Input::IsKeyPressed(VK_RETURN))
		{
			ToggleFullscreen();
		}

		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		if (imIO.WantCaptureKeyboard)
		{
			break;
		}

		KeyCode keycode = static_cast<KeyCode>(wParam);
		KeyReleasedEvent event(keycode);
		m_EventCallback(event);

		Input::OnKeyReleased(keycode);

		Input::m_KeyRepeatCount[keycode] = 0;
		break;
	}
	case WM_CHAR:
	{
		KeyCode keycode = static_cast<KeyCode>(wParam);

		KeyTypedEvent event(keycode);
		m_EventCallback(event);

		break;
	}
	// ---------------- Mouse Messages ---------------- //
	case WM_MOUSEMOVE:
	{
		if (!Input::IsMousePositionInputEnabled())
		{
			break;
		}
		
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		POINT lpt = POINT(pt.x, pt.y);

		MouseMovedEvent event(pt.x, pt.y);
		m_EventCallback(event);

		Input::OnMouseMove(pt.x, pt.y);
		ClientToScreen(m_hWnd, &lpt);
		m_LastCursorPos = { lpt.x, lpt.y };
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(m_hWnd);
		if (!m_CursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}

		if (imIO.WantCaptureMouse)
		{
			break;
		}

		MouseButtonPressedEvent event(VK_LBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonPressed(VK_LBUTTON);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		MouseButtonReleased event(VK_LBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonReleased(VK_LBUTTON);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		MouseButtonPressedEvent event(VK_RBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonPressed(VK_RBUTTON);
		break;
	}
	case WM_RBUTTONUP:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		MouseButtonReleased event(VK_RBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonReleased(VK_RBUTTON);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		MouseButtonPressedEvent event(VK_MBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonPressed(VK_MBUTTON);
		break;
	}
	case WM_MBUTTONUP:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		MouseButtonReleased event(VK_MBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonReleased(VK_MBUTTON);
		break;
	}

	case WM_XBUTTONDOWN:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		WORD xButton = GET_XBUTTON_WPARAM(wParam);
		if (xButton == 1)
		{
			MouseButtonPressedEvent event(VK_XBUTTON1);
			m_EventCallback(event);

			Input::OnMouseButtonPressed(VK_XBUTTON1);
		}
		else if (xButton == 2)
		{
			MouseButtonPressedEvent event(VK_XBUTTON2);
			m_EventCallback(event);

			Input::OnMouseButtonPressed(VK_XBUTTON2);
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		WORD xButton = GET_XBUTTON_WPARAM(wParam);
		if (xButton == 1)
		{
			MouseButtonReleased event(VK_XBUTTON1);
			m_EventCallback(event);

			Input::OnMouseButtonReleased(VK_XBUTTON1);
		}
		else if (xButton == 2)
		{
			MouseButtonReleased event(VK_XBUTTON2);
			m_EventCallback(event);

			Input::OnMouseButtonReleased(VK_XBUTTON2);
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (imIO.WantCaptureMouse)
		{
			break;
		}

		POINTS pt = MAKEPOINTS(lParam);
		int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		Input::m_WheelDeltaCarry += delta;

		// Calculate when we should generate a mouse scroll event given delta
		while (Input::m_WheelDeltaCarry >= WHEEL_DELTA) // Positive scrolling
		{
			Input::m_WheelDeltaCarry -= WHEEL_DELTA;
			MouseScrolledEvent event(0, WHEEL_DELTA);
			m_EventCallback(event);
		}
		while (Input::m_WheelDeltaCarry <= -WHEEL_DELTA) // Negative scrolling
		{
			Input::m_WheelDeltaCarry += WHEEL_DELTA;
			MouseScrolledEvent event(0, -WHEEL_DELTA);
			m_EventCallback(event);
		}
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

void Window::EnableCursor()
{
	m_CursorEnabled = true;
	ShowCursor();
	EnableImGuiMouse();
	FreeCursor();
	SetCursorPos(m_LastCursorPos.first, m_LastCursorPos.second);
	Input::OnCursorVisibility(m_CursorEnabled);
}

void Window::DisableCursor()
{
	m_CursorEnabled = false;
	HideCursor();
	DisableImGuiMouse();
	ConfineCursor();
	Input::OnCursorVisibility(m_CursorEnabled);
}

void Window::ToggleCursor()
{
	if (m_CursorEnabled)
	{
		DisableCursor();
	}
	else
	{
		EnableCursor();
	}
}

void Window::ShowCursor()
{
	while (::ShowCursor(true) < 0);
}

void Window::HideCursor()
{
	while (::ShowCursor(false) >= 0);
}

void Window::EnableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Window::DisableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

void Window::ConfineCursor()
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}

void Window::FreeCursor()
{
	ClipCursor(nullptr);
}

void Window::ToggleFullscreen()
{
	WindowState& state = m_WindowProps.WindowState;

	if (state == WindowState::Fullscreen || state == WindowState::Borderless)
	{
		// In fullscreen mode, switch to windowed
		if (state == WindowState::Fullscreen)
		{
			m_GraphicsContext->ToggleFullscreen();
		}

		// In Borderless mode, switch to windowed
		SetWindowedState();

	}
	else // In windowed mode, switch to borderless
	{

			state = WindowState::Fullscreen;
			m_OldWindowProps = m_WindowProps;
			//m_GraphicsContext->ToggleFullscreen(); // Exclusive fullscreen
			SetBorderlessState(); // Borderless fullscreen
	}
}

void Window::SetWindowedState()
{
	m_WindowProps.WindowState = WindowState::Windowed;
	m_WindowProps.WindowStyle = WINDOWED_STYLE;

	SetLastError(0);

	SetWindowPos(m_hWnd, HWND_TOP, m_OldWindowProps.PosX, m_OldWindowProps.PosY, m_OldWindowProps.Width, m_OldWindowProps.Height, SWP_FRAMECHANGED);
	SetWindowLongPtr(m_hWnd, GWL_STYLE, m_WindowProps.WindowStyle);
}

void Window::SetBorderlessState()
{
	m_WindowProps.WindowState = WindowState::Borderless;
	m_WindowProps.WindowStyle = BORDERLESS_STYLE;

	int width, height;
	RECT monitorRect = {};
	HMONITOR monitor = MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST);
	for (int i = 0; i < m_MonitorRects.Monitors.size(); i++)
	{
		if (monitor == m_MonitorRects.Monitors[i].hMonitor)
		{
			monitorRect = m_MonitorRects.Monitors[i].MonitorRect;
		}
	}
	GetMonitorRealResolution(monitor, &width, &height);

	SetLastError(0);
	SetWindowLongPtr(m_hWnd, GWL_STYLE, BORDERLESS_STYLE);
	SetWindowPos(m_hWnd, HWND_TOP, monitorRect.left, monitorRect.top, width, height, SWP_FRAMECHANGED);
}

void Window::OnUpdate(DeltaTime dt)
{
	m_Timer.Update(dt);
	ProcessMessages();
	m_GraphicsContext->SwapBuffers();
}

// https://stackoverflow.com/a/74035135 
void Window::GetMonitorRealResolution(HMONITOR monitor, int* pixelsWidth, int* pixelsHeight)
{
    MONITORINFOEX info = { sizeof(MONITORINFOEX) };
    GetMonitorInfo(monitor, &info);
    DEVMODE devmode = {};
    devmode.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(info.szDevice, ENUM_CURRENT_SETTINGS, &devmode);
    *pixelsWidth = devmode.dmPelsWidth;
    *pixelsHeight = devmode.dmPelsHeight;
}
