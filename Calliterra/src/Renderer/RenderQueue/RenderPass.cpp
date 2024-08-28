#include "pch.h"
#include "RenderPass.h"

void RenderPass::Accept(const Step& step)
{
	m_Steps.push_back(step);
}

void RenderPass::Execute()
{
	for (const auto& step : m_Steps)
	{
		step.Execute();
	}
}

void RenderPass::Reset()
{
	m_Steps.clear();
}
