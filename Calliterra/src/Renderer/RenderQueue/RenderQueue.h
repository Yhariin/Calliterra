#pragma once
#include "RenderPass.h"

class RenderQueue
{
public:
	RenderQueue() = default;

	void Accept(const Step& step, uint32_t targetPass);
	void Execute();
	void Reset();

private:
	std::array<RenderPass, 3> m_Passes;
};

