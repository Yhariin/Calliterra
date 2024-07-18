#pragma once
#include <d3d11.h>

#include "Renderer/RendererAPI.h"
#include "Platform/DX11/DX11Context.h"

class DX11RendererAPI : public RendererAPI
{
public:
	void Init(std::shared_ptr<GraphicsContext> context) override;
	void DrawIndexed(uint32_t indexCount) override;

private:
	std::shared_ptr<DX11Context> m_Context;
};

