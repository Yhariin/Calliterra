#pragma once
#include "Renderer\GraphicsContext.h"

class RendererAPI
{
public:
	enum API
	{
		None = 0, DX11 = 1
	};

	virtual ~RendererAPI() = default;
	virtual void Init(std::shared_ptr<GraphicsContext> context) = 0;

	virtual void DrawIndexed(uint32_t indexCount) = 0;

	static API GetAPI() { return s_API; }
	static std::unique_ptr<RendererAPI> Create();
private:
	inline static API s_API = DX11;
};

