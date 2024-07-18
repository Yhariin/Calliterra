#pragma once

#include "RendererAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

class Renderer
{
public:
	static void Init(std::shared_ptr<GraphicsContext> graphicsContext);
	static void Shutdown();
	static void SetClearColor(float r, float g, float b, float a = 1.f);
	static void Clear();
	static void Bind(const std::vector<std::shared_ptr<Shader>>& shaderList = {}, const std::shared_ptr<VertexBuffer>& vertexBuffer = nullptr, const std::shared_ptr<IndexBuffer>& indexBuffer = nullptr);
	static void Draw();

	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, uint32_t elementCount, Shader* shader = nullptr);
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float** listOfVertexArrays, uint32_t * listOfElementCounts, uint32_t bufferCount, Shader* shader = nullptr);
	static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count);
	static std::shared_ptr<Shader> CreateShader(const std::string& filepath, Shader::ShaderType type);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	inline static std::shared_ptr<RendererAPI> s_RendererAPI = RendererAPI::Create();
	inline static std::shared_ptr<GraphicsContext> s_GraphicsContext = nullptr;
	inline static uint32_t s_IndexCount = 0;
};

