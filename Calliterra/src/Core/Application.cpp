#include "pch.h"
#include "Application.h"

#include "Window.h"
#include "Renderer/Renderer.h"

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
	float vertices[] = {
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	uint32_t indices[] = {
		0, 1, 2
	};

	std::vector<std::shared_ptr<Shader>> shaderList;
	std::shared_ptr<Shader> vertexShader = Renderer::CreateShader("src/assets/shaders/VertexShader.hlsl", Shader::VERTEX_SHADER);
	std::shared_ptr<Shader> pixelShader = Renderer::CreateShader("src/assets/shaders/PixelShader.hlsl", Shader::PIXEL_SHADER);
	shaderList.push_back(vertexShader);
	shaderList.push_back(pixelShader);

	std::shared_ptr<VertexBuffer> vertexBuffer = Renderer::CreateVertexBuffer(vertices, sizeof(vertices) / sizeof(float), vertexShader.get());
	vertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"COLOR", 0, ShaderDataType::Float3 }
		});
	vertexBuffer->SetLayout();

	std::shared_ptr<IndexBuffer> indexBuffer = Renderer::CreateIndexBuffer(indices, sizeof(indices) / sizeof(int));

	Renderer::Bind(shaderList, vertexBuffer, indexBuffer);
	while (m_Running)
	{
		const float c = static_cast<float>(sin(m_ApplicationTimer.GetElapsedInSeconds()) / 2.0 + 0.5);
		Renderer::SetClearColor(c * .5f, 0.5f, c);
		Renderer::Clear();
		// Calculate deltaTime
		double time = DeltaTime::GetCurrentTimeMicroseconds();
		m_DeltaTime = time - m_LastFrameTime;
		m_LastFrameTime = time;

		//LOG_INFO("{0}ms : {1:.2f} FPS", m_DeltaTime.GetMilliseconds(), 1.f / m_DeltaTime.GetSeconds());
		Renderer::Draw();
		m_Window->OnUpdate();

	}
}

bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

