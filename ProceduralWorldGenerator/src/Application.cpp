#include "Application.hpp"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	if(!s_Instance)
		s_Instance = this;

	// Create Window here

	// Initialize Renderer here
}

Application::~Application()
{

	// Shutdown Renderer here
}

void Application::Run()
{
	while (m_Running)
	{
		// Calcualte m_LastFrameTime here

		// Update window here
	}
}

void Application::Close()
{
	m_Running = false;
}

