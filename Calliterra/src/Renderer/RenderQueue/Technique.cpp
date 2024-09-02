#include "pch.h"
#include "Technique.h"
#include "Step.h"

Technique::Technique(std::string name)
	: m_Name(name)
{
}

void Technique::AddStep(Step step)
{
	m_Steps.push_back(std::move(step));
}

void Technique::Submit() const
{
	for (const auto& step : m_Steps)
	{
		step.Submit();
	}
}

void Technique::InitializeParentReferences(const Drawable& parent)
{
	for (auto& step : m_Steps)
	{
		step.InitializeParentReferences(parent);
	}
}

void Technique::Link()
{
	for (auto& step : m_Steps)
	{
		step.Link();
	}
}
