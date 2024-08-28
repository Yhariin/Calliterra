#pragma once
#include "Step.h"

class RenderPass
{
public:
	RenderPass() = default;

	void Accept(const Step& step);
	void Execute();
	void Reset();

private:
	std::vector<Step> m_Steps;

};

