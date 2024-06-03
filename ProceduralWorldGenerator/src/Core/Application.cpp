#include "pch.h"
#include "Application.h"

#include "Window.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	if(!s_Instance)
		s_Instance = this;

	// Create Window here
	m_Window = std::make_unique<Window>();
	// Set Window Callback here

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
		if (!m_Window->ProcessMessages())
		{
			m_Running = false;
		}

	}
}

void Application::Close()
{
	m_Running = false;
}

