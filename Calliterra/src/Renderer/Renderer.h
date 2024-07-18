#pragma once

#include "RendererAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

class Renderer
{
public:
	static void Init(GraphicsContext& graphicsContext);
	static void Shutdown();
	static void Submit(const std::shared_ptr<VertexBuffer>& vertexBuffer);

	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t elementCount, Shader* shader = nullptr);
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float** listOfVertexArrays, uint32_t * listOfElementCounts, uint32_t bufferCount, Shader* shader = nullptr);
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count);
	static std::shared_ptr<Shader> CreateShader(const std::string& filepath, Shader::ShaderType type);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	inline static std::unique_ptr<RendererAPI> s_RendererAPI = RendererAPI::Create();
	inline static GraphicsContext* s_GraphicsContext = nullptr;
};

