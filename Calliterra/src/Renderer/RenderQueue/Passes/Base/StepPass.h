#pragma once
#include "RenderPass.h"
#include "Renderer/RenderQueue/Step.h"

class StepPass : public RenderPass
{
public:
	StepPass() = default;

	virtual void Accept(const Step& step)
	{
		m_Steps.push_back(step);
	}

	void Execute() const override
	{
		for (auto& step : m_Steps)
		{
			step.Execute();
		}
	}

	void Reset() override
	{
		m_Steps.clear();
	}

private:
	std::vector<Step> m_Steps;
};

