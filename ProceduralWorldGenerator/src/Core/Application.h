#pragma once
#include "Window.h"
#include "Events/Event.h"

class Application
{
public:

	Application();
	~Application();

	void OnEvent(Event& e);

	void Run();

	Window& GetWindow() { return *m_Window; }

	void Close();

	static Application& Get() { return *s_Instance; }

private:

private:
	static Application* s_Instance;
	std::unique_ptr<Window> m_Window;

	bool m_Running = true;
	bool m_Minimized = false;
	float m_LastFrameTime = 0.0f;
};


