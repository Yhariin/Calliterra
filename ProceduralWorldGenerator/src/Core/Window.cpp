#include "pch.h"
#include "Window.h"
#include "Input.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

Window::Window(const WindowProps& windowProps)
	: m_hInstance(GetModuleHandle(nullptr)), // Gets the instance handle of the current module
	  m_CLASSNAME(L"Procedural World Generator")
{
	WNDCLASS wndClass = {};
	wndClass.lpszClassName = m_CLASSNAME;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.lpfnWndProc = MessageSetup; 

	RegisterClass(&wndClass);

	DWORD style = WS_CAPTION | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	// Calculate center of screen to place window
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowProps.Width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowProps.Height / 2;

	// Calculate the required size of the window rectangle based on desired client area size
	RECT windowRegion = { centerScreenX, centerScreenY, centerScreenX + windowProps.Width, centerScreenY + windowProps.Height };
	AdjustWindowRectEx(&windowRegion, style, FALSE, 0);

	m_hWnd = CreateWindowEx(
		0,
		m_CLASSNAME,
		m_CLASSNAME,
		style,
		windowRegion.left,
		windowRegion.top,
		windowRegion.right - windowRegion.left, // Adjusted width
		windowRegion.bottom - windowRegion.top, // Adjusted height
		NULL,
		NULL,
		m_hInstance,
		this	
	);

	ShowWindow(m_hWnd, SW_SHOW);
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
	static int sizeCount = 0;

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
	case WM_SIZE:
	{
		// Upon window creation there is an initial WM_SIZE message.
		// To prevent calling m_EventCallback before setup is complete
		// We will break early if it's the first WM_SIZE message.
		sizeCount++;
		if (sizeCount == 1)
			break;

		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		m_WindowProps.Width = width;
		m_WindowProps.Height = height;

		WindowResizeEvent event(width, height);
		m_EventCallback(event);

		break;
	}

	// --------------- Keyboard Messages -------------- //
	case WM_KEYDOWN:
	{
		KeyCode keycode = static_cast<KeyCode>(wParam);

		KeyPressedEvent	event(keycode, m_KeyRepeatCount[keycode]);
		m_EventCallback(event);

		Input::OnKeyPressed(keycode);

		m_KeyRepeatCount[keycode]++;

		break;
	}
	case WM_KEYUP:
	{
		KeyCode keycode = static_cast<KeyCode>(wParam);
		KeyReleasedEvent event(keycode);
		m_EventCallback(event);

		Input::OnKeyReleased(keycode);

		m_KeyRepeatCount[keycode] = 0;
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
		POINTS pt = MAKEPOINTS(lParam);

		MouseMovedEvent event(pt.x, pt.y);
		m_EventCallback(event);

		Input::OnMouseMove(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		MouseButtonPressedEvent event(VK_LBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonPressed(VK_LBUTTON);
		break;
	}
	case WM_LBUTTONUP:
	{
		MouseButtonReleased event(VK_LBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonReleased(VK_LBUTTON);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		MouseButtonPressedEvent event(VK_RBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonPressed(VK_RBUTTON);
		break;
	}
	case WM_RBUTTONUP:
	{
		MouseButtonReleased event(VK_RBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonReleased(VK_RBUTTON);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		MouseButtonPressedEvent event(VK_MBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonPressed(VK_MBUTTON);
		break;
	}
	case WM_MBUTTONUP:
	{
		MouseButtonReleased event(VK_MBUTTON);
		m_EventCallback(event);

		Input::OnMouseButtonReleased(VK_MBUTTON);
		break;
	}

	case WM_XBUTTONDOWN:
	{
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

