#pragma once
#include "pch.h"

#include "Events/Event.h"
#include "Input.h"
#include "Renderer/GraphicsContext.h"

struct WindowProps
{
	std::string Title;
	uint16_t Width;
	uint16_t Height;

	WindowProps(const std::string& title = "Calliterra",
		uint16_t width = 1600,
		uint16_t height = 900)
		: Title(title), Width(width), Height(height)
	{
	}
};

class Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	Window(const WindowProps& windowProps = WindowProps());
	Window(const Window&) = delete;
	Window& operator =(const Window&) = delete;
	~Window();

	static LRESULT CALLBACK MessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	static LRESULT WINAPI MessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	LRESULT MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	bool ProcessMessages();
	void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }
	HWND GetWindowHandle() const { return m_hWnd; }
	void OnUpdate();
	GraphicsContext& GetGraphicsContext() { return *m_GraphicsContext; }

private:
	const wchar_t* m_CLASSNAME;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	std::unique_ptr<GraphicsContext> m_GraphicsContext;
	WindowProps m_WindowProps;
	EventCallbackFn m_EventCallback;
	bool m_Resizing = false;
	bool m_Active = true;
	bool m_Minimized = false;
};

