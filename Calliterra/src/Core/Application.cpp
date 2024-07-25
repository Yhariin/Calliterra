#include "pch.h"
#include "Application.h"

#include "Window.h"
#include "Renderer/Renderer.h"
#include "Platform/DX11/DX11Shader.h"

#include "imgui.h"

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

	m_Sandbox = std::make_unique<Sandbox>(m_Window->GetWindowProps().AspectRatio);
}

Application::~Application()
{
	Renderer::Shutdown();
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return this->OnWindowClose(e); });

	dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { return this->OnKeyPressed(e); });

	m_Sandbox->OnEvent(e);
	//LOG_INFO(e.ToString());
}

void Application::Run()
{
	for(int i = 0; i < 55; i++)
	{
		m_Sandbox->CreateCube();
		//m_Sandbox->CreateRadialSphere();
		//m_Sandbox->CreateIcoSphere();
		//m_Sandbox->CreatePlane();
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
		m_Sandbox->OnUpdate(static_cast<float>(m_DeltaTime.GetSeconds()));


		ImGuiManager::Begin();
		//ImGuiManager::DemoWindow();
		ImGuiManager::SettingsGui();
		ImGuiManager::End();


		m_Window->OnUpdate(m_DeltaTime);

	}
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

bool Application::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == VK_ESCAPE)
	{
		m_Window->ToggleCursor();
		if (Input::IsMouseRawInputEnabled())
		{
			Input::DisableMouseRawInput();
			Input::EnableMousePositionInput();
			ImGuiManager::EnableImGui();
		}
		else
		{
			Input::EnableMouseRawInput();
			Input::DisableMousePositionInput();
			ImGuiManager::DisableImGui();
		}
		
	}
	if (e.GetKeyCode() == VK_F1)
	{
		m_Window->GetGraphicsContext()->ToggleWireFrame();
	}
	return true;
}


