#pragma once
#include "Base/ScreenSpacePass.h"

class ColorInvertPass : public ScreenSpacePass
{
public:
	ColorInvertPass(const GraphicsContext& context, std::shared_ptr<RenderTarget> backBuffer, std::shared_ptr<RenderTarget> renderTarget)
		: m_Context(context), m_BackBuffer(backBuffer), m_RenderTarget(renderTarget)
	{
		
	}

	void Execute() const override
	{
		//m_Context.BindSwapBuffer();
		m_BackBuffer->BindAsBuffer();
		m_RenderTarget->Bind();
		Shader::Resolve("assets/shaders/ColorInvertPS.hlsl", Shader::PIXEL_SHADER)->Bind();
		ScreenSpacePass::Execute();
	}

private:
	const GraphicsContext& m_Context;
	std::shared_ptr<RenderTarget> m_BackBuffer;
	std::shared_ptr<RenderTarget> m_RenderTarget;
};
