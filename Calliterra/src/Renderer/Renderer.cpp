#include "pch.h"
#include "Renderer.h"

std::unique_ptr<RendererAPI> Renderer::s_RendererAPI = RendererAPI::Create();

void Renderer::Init()
{
	s_RendererAPI->Init();
}

void Renderer::Shutdown()
{

}
