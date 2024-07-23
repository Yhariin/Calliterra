#include "pch.h"
#include "Application.h"

#include "Window.h"
#include "Renderer/Renderer.h"
#include "Sandbox/Sandbox.h"

#include "Platform/DX11/DX11Shader.h"

Application::Application()
{
	ASSERT(!s_Instance, "Application already exists!");
	s_Instance = this;

	Timer::SetApplicationTimer(&m_ApplicationTimer);
	m_ApplicationTimer.Reset();

	// Create Window here
	m_Window = std::make_unique<Window>();

	// Set Window Callback here
	// We want to set m_Window's event callback to Application's OnEvent function
	// however setEventCallback expects a function with the signature void(*fn)(Event&).
	// This is a problem because our OnEvent function has the signature void(*fn)(this, Event&).
	// We can use a lambda capture with this to then return OnEvent. Alternatively you can use std::bind,
	// m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	m_Window->SetEventCallback([this](Event& e) { return this->OnEvent(e); });

	Renderer::Init(m_Window->GetGraphicsContext());

}

Application::~Application()
{
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
	Sandbox sandbox = Sandbox();
	for(int i = 0; i < 1; i++)
	{
		//sandbox.CreateCube();
		sandbox.CreateRadialSphere();
	}

	while (m_Running)
	{
		// Calculate deltaTime
		double time = DeltaTime::GetCurrentTimeMicroseconds();
		m_DeltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		// Clear previous frame
		float angle = static_cast<float>(m_ApplicationTimer.GetElapsedInSeconds());
		const float c = static_cast<float>(sin(m_ApplicationTimer.GetElapsedInSeconds()) / 2.0 + 0.5);
		Renderer::SetClearColor(c * .5f, 0.5f, c);
		Renderer::Clear();


		// Draw new frame
		sandbox.OnUpdate(static_cast<float>(m_DeltaTime.GetSeconds()));
		//LOG_INFO("{0}ms : {1:.2f} FPS", m_DeltaTime.GetMilliseconds(), 1.f / m_DeltaTime.GetSeconds());

		m_Window->OnUpdate(1.f / static_cast<float>(m_DeltaTime.GetSeconds()));

	}
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

