#pragma once
#include "Base/StepPass.h"

class OutlineDrawPass : public StepPass
{
public:
	OutlineDrawPass() = default;

	void Execute() const override
	{
		DepthStencilMask::Resolve(DepthStencilMask::Mode::Mask)->Bind();
		StepPass::Execute();
	}
};
