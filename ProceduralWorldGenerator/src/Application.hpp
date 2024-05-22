#pragma once

class Application
{
public:

	Application();

	~Application();

	void Run();

	void GetWindow();

	void Close();

	static Application& Get() { return *s_Instance; }

private:

private:
	static Application* s_Instance;
	bool m_Running = true;
	bool m_Minimized = false;
	float m_LastFrameTime = 0.0f;
};


