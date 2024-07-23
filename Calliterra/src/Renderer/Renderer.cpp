#include "pch.h"
#include "Renderer.h"
#include "Platform/DX11/DX11VertexBuffer.h"
#include "Platform/DX11/DX11IndexBuffer.h"
#include "Platform/DX11/DX11Shader.h"

void Renderer::Init(std::shared_ptr<GraphicsContext> graphicsContext)
{
	s_GraphicsContext = graphicsContext;

	s_RendererAPI->Init(s_GraphicsContext);
}

void Renderer::Shutdown()
{

}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	s_GraphicsContext->SetClearColor(r, g, b, a);
}

void Renderer::Clear()
{
	s_GraphicsContext->Clear();
}

void Renderer::Bind(const std::vector<std::shared_ptr<Shader>>& shaderList, const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	for (auto& shader : shaderList)
	{
		shader->Bind();
	}

	if (vertexBuffer != nullptr) vertexBuffer->Bind();

	if (indexBuffer != nullptr)
	{
		indexBuffer->Bind();
		s_IndexCount = indexBuffer->GetCount();
	}
}

void Renderer::Draw()
{
	s_RendererAPI->DrawIndexed(s_IndexCount);
}

/*
std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(const float* vertices, uint32_t elementCount, Shader* shader)
{
	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		ASSERT(shader != nullptr, "Attempting to create DX11VertexBuffer with a Null shader paramater!");
		return std::make_shared<DX11VertexBuffer>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), vertices, elementCount, dynamic_cast<DX11Shader*>(shader)->GetCompiledShaderByteCode());
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}

std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(const float** listOfVertexArrays, const uint32_t* listOfElementCounts, uint32_t bufferCount, Shader* shader)
{
	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		ASSERT(shader != nullptr, "Attempting to create DX11VertexBuffer with a Null shader paramater!");
		return std::make_shared<DX11VertexBuffer>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), listOfVertexArrays, listOfElementCounts, bufferCount, dynamic_cast<DX11Shader*>(shader)->GetCompiledShaderByteCode());
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;
	
}
*/

std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(const uint32_t* indices, uint32_t count)
{
	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		return std::make_shared<DX11IndexBuffer>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), indices, count);
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}

std::shared_ptr<Shader> Renderer::CreateShader(const std::string& filepath, Shader::ShaderType type)
{

	switch(GetAPI())
	{
	case RendererAPI::None: 
		ASSERT(false, "RendererAPI is set to None!");
		return nullptr;
	case RendererAPI::DX11:
		return std::make_shared<DX11Shader>(*dynamic_cast<DX11Context*>(s_GraphicsContext.get()), filepath, type);
	}

	LOG_ERROR("Unknown RendererAPI");
	return nullptr;

}

