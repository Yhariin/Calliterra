#include "pch.h"
#include "DX11RendererAPI.h"

void DX11RendererAPI::Init(std::shared_ptr<GraphicsContext> context)
{
	m_Context = std::dynamic_pointer_cast<DX11Context>(context);
	LOG_INFO("DX11 Renderer Initialized");
}

void DX11RendererAPI::DrawIndexed(uint32_t indexCount)
{
	m_Context->DrawIndexed(indexCount);
}
