#pragma once
#include "Base/StepPass.h"

class OutlineMaskPass : public StepPass
{
public:
	OutlineMaskPass() = default;

	void Execute() const override
	{
		DepthStencilMask::Resolve(DepthStencilMask::Mode::Write)->Bind();
		Shader::Resolve("", Shader::PIXEL_SHADER)->Bind();
		
		StepPass::Execute();
	}
};
