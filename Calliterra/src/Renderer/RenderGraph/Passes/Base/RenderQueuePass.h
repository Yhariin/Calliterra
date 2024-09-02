#pragma once
#include "BindingPass.h"
#include "Renderer/RenderQueue/Step.h"

class RenderQueuePass : public BindingPass
{
public:
	using BindingPass::BindingPass;

	void Accept(const Step& step);
	void Execute() const override;
	void Reset();

private:
	std::vector<Step> m_Steps;
};

