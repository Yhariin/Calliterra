#pragma once
#include "RenderPass.h"
#include "Sandbox/Components/ScreenSpaceQuad.h"

class ScreenSpacePass : public RenderPass
{
public:
	ScreenSpacePass() = default;

	void Execute() const override
	{
		m_SSQuad.Bind();
		m_SSQuad.Draw();
	}

protected:
	ScreenSpaceQuad m_SSQuad;
};
