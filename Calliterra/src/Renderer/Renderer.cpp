#include "pch.h"
#include "Renderer.h"
#include "Platform/DX11/DX11VertexBuffer.h"
#include "Platform/DX11/DX11IndexBuffer.h"

void Renderer::Init(GraphicsContext& graphicsContext)
{
	s_GraphicsContext = &graphicsContext;
	s_RendererAPI->Init();
}

void Renderer::Shutdown()
{

}

void Renderer::Submit(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	vertexBuffer->Bind();
	//s_RendererAPI->Draw();
}

std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(float* vertices, uint32_t elementCount)
{
	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		return std::make_shared<DX11VertexBuffer>(*dynamic_cast<DX11Context*>(s_GraphicsContext), vertices, elementCount);
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}

std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(float** listOfVertexArrays, uint32_t* listOfElementCounts, uint32_t bufferCount)
{
	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		return std::make_shared<DX11VertexBuffer>(*dynamic_cast<DX11Context*>(s_GraphicsContext), listOfVertexArrays, listOfElementCounts, bufferCount);
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;
	
}

std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(uint32_t* indices, uint32_t count)
{
	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		return std::make_shared<DX11IndexBuffer>(*dynamic_cast<DX11Context*>(s_GraphicsContext), indices, count);
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}
