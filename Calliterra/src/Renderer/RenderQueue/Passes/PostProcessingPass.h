#pragma once
#include "Base/FullScreenPass.h"

class PostProcessingPass : public FullScreenPass
{
public:
	PostProcessingPass(const GraphicsContext& context, std::shared_ptr<RenderTarget> backBuffer, std::shared_ptr<RenderTarget> renderTarget)
		: m_Context(context), m_BackBuffer(backBuffer), m_RenderTarget(renderTarget)
	{
		
	}

	void Execute() const override
	{
		m_BackBuffer->BindAsBuffer();
		m_RenderTarget->Bind();
		Shader::Resolve("assets/shaders/DefaultFullScreenPS.hlsl", Shader::PIXEL_SHADER)->Bind();
		Rasterizer::Resolve(FillMode::Solid, CullMode::Back)->Bind();
		FullScreenPass::Execute();
	}

private:
	const GraphicsContext& m_Context;
	std::shared_ptr<RenderTarget> m_BackBuffer;
	std::shared_ptr<RenderTarget> m_RenderTarget;
};
