#include "pch.h"
#include "GraphicsContext.h"

#include "Renderer/Renderer.h"
#include "Platform/DX11/DX11Context.h"

std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
{
	switch (Renderer::GetAPI())
	{
	case RendererAPI::API::None: 
		//TODO Throw Assert here
		LOG_ERROR("RendererAPI::None");
		return nullptr;
	case RendererAPI::API::DX11:
		return std::make_unique<DX11Context>(static_cast<HWND*>(window));
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}
