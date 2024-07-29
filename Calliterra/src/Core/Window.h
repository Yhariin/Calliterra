#pragma once
#include "pch.h"

#include "Events/Event.h"
#include "Input.h"
#include "Renderer/GraphicsContext.h"
#include "Timer.h"
#include "DeltaTime.h"

enum class WindowState
{
	None = 0,
	Fullscreen, // Exclusive fullscreen
	Borderless, // Non-movable window with no border
	Windowed // Moveable window with border
};

constexpr DWORD WINDOWED_STYLE = WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE;
constexpr DWORD BORDERLESS_STYLE = WS_POPUP | WS_VISIBLE;

struct WindowProps
{
	std::string Title;
	uint16_t Width;
	uint16_t Height;
	int PosX; 
	int PosY; 
	WindowState WindowState;
	DWORD WindowStyle;
	float AspectRatio;

	WindowProps(const std::string& title = "Calliterra",
		uint16_t width = 1600,
		uint16_t height = 900)
		: Title(title), Width(width), Height(height), PosX(0), PosY(0), WindowState(WindowState::Windowed), WindowStyle(WINDOWED_STYLE), AspectRatio((float)width / (float)height)
	{
	}
};
 
// Gets the information of the user's monitors on construction.
// Specifically we want the HMONITOR handle and the dimensions as a RECT.
// https://stackoverflow.com/questions/18112616/how-do-i-get-the-dimensions-rect-of-all-the-screens-in-win32-api
struct MonitorRects
{
	struct MonitorInfo
	{
		HMONITOR hMonitor;
		RECT MonitorRect;
	};

	std::vector<MonitorInfo> Monitors;
    RECT                rcCombined;

    static BOOL CALLBACK MonitorEnum(HMONITOR hMon,HDC hdc,LPRECT lprcMonitor,LPARAM pData)
    {
        MonitorRects* pThis = reinterpret_cast<MonitorRects*>(pData);

		pThis->Monitors.push_back({hMon, *lprcMonitor});
        UnionRect(&pThis->rcCombined, &pThis->rcCombined, lprcMonitor);

        return TRUE;
    }

    MonitorRects()
    {
        SetRectEmpty(&rcCombined);
        EnumDisplayMonitors(0, 0, MonitorEnum, (LPARAM)this);
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

	void OnUpdate(DeltaTime dt);
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
	void EnableImGuiMouse();
	void DisableImGuiMouse();
	void ConfineCursor();
	void FreeCursor();
	void ToggleFullscreen();
	void SetWindowedState();
	void SetBorderlessState();
	void GetMonitorRealResolution(HMONITOR monitor, int* pixelsWidth, int* pixelsHeight);

private:
	const wchar_t* m_CLASSNAME;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	std::shared_ptr<GraphicsContext> m_GraphicsContext;
	WindowProps m_WindowProps;
	WindowProps m_OldWindowProps;
	EventCallbackFn m_EventCallback;
	bool m_Resizing = false;
	bool m_Active = true;
	bool m_Minimized = false;
	bool m_CursorEnabled = false;
	CountdownTimer<DeltaTime> m_Timer;
	std::vector<char> m_RawBuffer;
	std::pair<int, int> m_LastCursorPos;
	MonitorRects m_MonitorRects;

};

