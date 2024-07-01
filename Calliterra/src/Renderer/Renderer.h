#pragma once

#include "RendererAPI.h"
#include "Renderer/GraphicsContext.h"
#include "Renderer/VertexBuffer.h"

class Renderer
{
public:
	static void Init(GraphicsContext& graphicsContext);
	static void Shutdown();
	static void Submit(const std::shared_ptr<VertexBuffer>& vertexBuffer);

	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float* vertices, size_t elementCount);
	static std::shared_ptr<VertexBuffer> CreateVertexBuffer(float** listOfVertexArrays, size_t* listOfElementCounts, size_t bufferCount);

	static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
private:
	inline static std::unique_ptr<RendererAPI> s_RendererAPI = RendererAPI::Create();
	inline static GraphicsContext* s_GraphicsContext = nullptr;
};

