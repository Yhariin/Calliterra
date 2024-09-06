#include "pch.h"
#include "GraphicsContext.h"

#include "Renderer/Renderer.h"
#include "Platform/DX11/DX11Context.h"
#include "Renderer/RenderQueue/RenderQueue.h"

std::shared_ptr<GraphicsContext> GraphicsContext::Create(void* window, WindowProps& windowProps)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::API::DX11:
		return std::make_shared<DX11Context>(static_cast<HWND*>(window), windowProps);
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}

void GraphicsContext::FreeRenderQueueBuffers()
{
	m_RenderQueue->FreeBuffers();
}

void GraphicsContext::InitRenderQueueBuffers()
{
	m_RenderQueue->InitPasses();
}

