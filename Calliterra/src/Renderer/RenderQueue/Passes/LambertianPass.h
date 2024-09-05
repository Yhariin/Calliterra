#pragma once
#include "Base/StepPass.h"

class LambertianPass : public StepPass
{
public:
	LambertianPass(std::shared_ptr<RenderTarget> renderTarget, std::shared_ptr<DepthStencilBuffer> depthStencil)
		: m_RenderTarget(renderTarget), m_DepthStencilBuffer(depthStencil)
	{
	}

	void Execute() const override
	{
		m_RenderTarget->BindAsBuffer(m_DepthStencilBuffer.get());
		DepthStencilMask::Resolve(DepthStencilMask::Mode::Off)->Bind();
		StepPass::Execute();
	}
private:
	std::shared_ptr<RenderTarget> m_RenderTarget;
	std::shared_ptr<DepthStencilBuffer> m_DepthStencilBuffer;
};
