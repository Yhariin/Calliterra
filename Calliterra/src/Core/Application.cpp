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
	m_ImGui.SetWindowSize(static_cast<float>(m_Window->GetWindowProps().Width), static_cast<float>(m_Window->GetWindowProps().Height));
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
	m_ImGui.OnEvent(e);
	//LOG_INFO(e.ToString());
}

void Application::Run()
{
	m_Sandbox->LoadSandboxPreset();
	while (m_Running)
	{
		// Calculate deltaTime
		double time = DeltaTime::GetCurrentTimeMicroseconds();
		m_DeltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		// Clear previous frame
		Renderer::SetClearColor(0.7f, 0.7f, 1.0f);
		Renderer::Clear();


		// Draw new frame
		m_Sandbox->OnUpdate(static_cast<float>(m_DeltaTime.GetSeconds()));


		m_ImGui.Begin();
		//m_ImGui.DemoWindow();
		m_ImGui.SettingsGui();
		m_ImGui.DebugGui(m_DeltaTime);
		m_ImGui.End();


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
			m_ImGui.EnableImGui();
		}
		else
		{
			Input::EnableMouseRawInput();
			Input::DisableMousePositionInput();
			m_ImGui.DisableImGui();
		}
		
	}
	if (e.GetKeyCode() == VK_F1)
	{
		m_Window->GetGraphicsContext()->ToggleWireFrame();
	}
	return true;
}


