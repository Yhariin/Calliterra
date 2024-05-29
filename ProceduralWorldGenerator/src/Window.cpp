#include "Window.h"
#include <iostream>

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

	DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	// Calculate center of screen to place window
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowProps.Width / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowProps.Height / 2;

	// Calculate the required size of the window rectangle based on desired client area size
	RECT windowRegion = { centerScreenX, centerScreenY, windowProps.Width, windowProps.Height };
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
			std::cout << "ERROR: window ptr null" << std::endl;
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

LRESULT Window::MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lPARAM);

}

bool Window::ProcessMessages()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

