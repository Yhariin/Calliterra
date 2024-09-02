#include "pch.h"
#include "RenderQueuePass.h"

void RenderQueuePass::Accept(const Step& step)
{
	m_Steps.push_back(step);
}

void RenderQueuePass::Execute() const
{
	BindAll();

	for (const auto& step : m_Steps)
	{
		step.Execute();
	}
}

void RenderQueuePass::Reset()
{
	m_Steps.clear();
}
