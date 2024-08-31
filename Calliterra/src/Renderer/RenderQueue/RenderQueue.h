#pragma once
#include "RenderPass.h"
#include "Renderer/RenderTarget.h"
#include "Sandbox/Components/ScreenSpaceQuad.h"

class RenderQueue
{
public:
	RenderQueue(GraphicsContext&);

	void Accept(const Step& step, uint32_t targetPass);
	void Execute();
	void Reset();

private:
	std::array<RenderPass, 3> m_Passes;
	GraphicsContext& m_Context;
	std::unique_ptr<RenderTarget> m_RenderTarget;
	ScreenSpaceQuad m_SSQuad;
};

