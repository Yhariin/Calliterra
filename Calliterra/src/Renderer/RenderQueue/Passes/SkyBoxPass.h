#pragma once
#include "Base/StepPass.h"
#include "Renderer/DepthStencilMask.h"

class SkyBoxPass : public StepPass
{
public:
	SkyBoxPass() = default;

	void Execute() const override
	{
		DepthStencilMask::Resolve(DepthStencilMask::Mode::DepthFirst)->Bind();
		StepPass::Execute();
	}
};
