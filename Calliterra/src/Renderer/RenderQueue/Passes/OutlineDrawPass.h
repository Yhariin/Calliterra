#pragma once
#include "Base/StepPass.h"

class OutlineDrawPass : public StepPass
{
public:
	OutlineDrawPass() = default;

	void Execute() const override
	{
		DepthStencilMask::Resolve(DepthStencilMask::Mode::Mask)->Bind();
		Rasterizer::Resolve(FillMode::Solid, CullMode::Back)->Bind();
		StepPass::Execute();
	}
};
