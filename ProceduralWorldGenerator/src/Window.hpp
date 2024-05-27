#pragma once

#include <Windows.h>
#include <string>

struct WindowProps
{
	std::string Title;
	uint16_t Width;
	uint16_t Height;

	WindowProps(const std::string& title = "Procedual World Generator",
		uint16_t width = 1600,
		uint16_t height = 900)
		: Title(title), Width(width), Height(height)
	{
	}
};

class Window
{
public:
	Window(const WindowProps& windowProps = WindowProps());
	Window(const Window&) = delete; // delete the copy constructor
	Window& operator =(const Window&) = delete; // delete the equals operator
	~Window();

public:
	static LRESULT CALLBACK MessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	static LRESULT WINAPI MessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	LRESULT MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	bool ProcessMessages();

private:
	const wchar_t* m_CLASSNAME;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	WindowProps m_WindowProps;

private:
};

