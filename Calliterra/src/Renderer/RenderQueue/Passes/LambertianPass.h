#pragma once
#include "Base/StepPass.h"

class LambertianPass : public StepPass
{
public:
	LambertianPass(std::shared_ptr<RenderTarget> renderTarget, std::shared_ptr<DepthStencilBuffer> depthStencil, FillMode fillMode, CullMode cullMode)
		: m_RenderTarget(renderTarget), m_DepthStencilBuffer(depthStencil), m_FillMode(fillMode), m_CullMode(cullMode)
	{
	}

	void Execute() const override
	{
		m_RenderTarget->BindAsBuffer(m_DepthStencilBuffer.get());
		DepthStencilMask::Resolve(DepthStencilMask::Mode::Off)->Bind();
		Topology::Resolve(PrimitiveTopology::Triangles)->Bind();
		Rasterizer::Resolve(m_FillMode, m_CullMode)->Bind();
		StepPass::Execute();
	}

	void SetFillMode(FillMode fillMode) { m_FillMode = fillMode; }
	void SetCullMode(CullMode cullMode) { m_CullMode = cullMode; }
private:
	std::shared_ptr<RenderTarget> m_RenderTarget;
	std::shared_ptr<DepthStencilBuffer> m_DepthStencilBuffer;
	FillMode m_FillMode;
	CullMode m_CullMode;
};
