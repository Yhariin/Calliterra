#include "pch.h"
#include "Application.h"

#include "Window.h"
#include "DeltaTime.h"
#include "Renderer/Renderer.h"

Application* Application::s_Instance = nullptr;

Application::Application()
{
	ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	// Create Window here
	m_Window = std::make_unique<Window>();

	// Set Window Callback here
	// We want to set m_Window's event callback to Application's OnEvent function
	// however setEventCallback expects a function with the signature void(*fn)(Event&).
	// This is a problem because our OnEvent function has the signature void(*fn)(this, Event&).
	// We can use a lambda capture with this to then return OnEvent. Alternatively you can use std::bind,
	// m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_Window->SetEventCallback([this](Event& e) { return this->OnEvent(e); });

	// Initialize Renderer here
	Renderer::Init();

}

Application::~Application()
{
	// Shutdown Renderer here
	Renderer::Shutdown();
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->OnWindowClose(e); });

	//LOG_INFO(e.ToString());
}

void Application::Run()
{
	while (m_Running)
	{
		// Calculate deltaTime
		double time = DeltaTime::GetCurrentTimeMicroseconds();
		DeltaTime deltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		//LOG_INFO("{0}ms : {1:.2f} FPS", deltaTime.GetMilliseconds(), 1.f / deltaTime.GetSeconds());

		// Update window here
		m_Window->ProcessMessages();

	}
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

