#pragma once
#include "pch.h"

#include "Events/Event.h"
#include "Input.h"
#include "Renderer/GraphicsContext.h"
#include "Timer.h"

struct WindowProps
{
	std::string Title;
	uint16_t Width;
	uint16_t Height;
	float AspectRatio;

	WindowProps(const std::string& title = "Calliterra",
		uint16_t width = 1600,
		uint16_t height = 900)
		: Title(title), Width(width), Height(height), AspectRatio((float)width/(float)height)
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

	void OnUpdate(float dt);
	void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }
	std::shared_ptr<GraphicsContext> GetGraphicsContext() { return m_GraphicsContext; }
	const WindowProps& GetWindowProps() { return m_WindowProps; }

	void EnableCursor();
	void DisableCursor();
	void ToggleCursor();

private:
	static LRESULT CALLBACK MessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	static LRESULT WINAPI MessageRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	LRESULT MessageHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lPARAM);
	bool ProcessMessages();
	HWND GetWindowHandle() const { return m_hWnd; }
	void ShowCursor();
	void HideCursor();
	void ConfineCursor();
	void FreeCursor();

private:
	const wchar_t* m_CLASSNAME;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	std::shared_ptr<GraphicsContext> m_GraphicsContext;
	WindowProps m_WindowProps;
	EventCallbackFn m_EventCallback;
	bool m_Resizing = false;
	bool m_Active = true;
	bool m_Minimized = false;
	bool m_CursorEnabled = false;
	CountdownTimer<float> m_Timer;
	std::vector<char> m_RawBuffer;
};

