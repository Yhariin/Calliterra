#pragma once
#include "RenderPass.h"
#include "Sandbox/Components/FullScreenQuad.h"

class FullScreenPass : public RenderPass
{
public:
	FullScreenPass() = default;

	void Execute() const override
	{
		m_FullScreenQuad.Bind();
		m_FullScreenQuad.Draw();
	}

protected:
	FullScreenQuad m_FullScreenQuad;
};
