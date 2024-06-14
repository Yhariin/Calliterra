#pragma once
#include "Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "DeltaTime.h"
#include "Timer.h"

class Application
{
public:

	Application();
	~Application();

	void OnEvent(Event& e);

	void Run();

	Window& GetWindow() { return *m_Window; }

	bool OnWindowClose(WindowCloseEvent& e);

	static Application& Get() { return *s_Instance; }
	static DeltaTime GetDeltaTime() { return m_DeltaTime; }

private:
	static Application* s_Instance;
	std::unique_ptr<Window> m_Window;

	bool m_Running = true;
	bool m_Minimized = false;
	double m_LastFrameTime = 0.0;
	static DeltaTime m_DeltaTime;
	static Timer m_ApplicationTimer;
};


