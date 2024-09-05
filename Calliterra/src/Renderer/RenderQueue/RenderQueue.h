#pragma once
#include "Passes/Base/RenderPass.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/DepthStencilBuffer.h"
#include "Sandbox/Components/ScreenSpaceQuad.h"

class RenderQueue
{
public:
	RenderQueue(GraphicsContext&);

	void Accept(const Step& step, PassName targetPass);
	void Execute();
	void Reset();

private:
	std::array<std::unique_ptr<RenderPass>, (int)PassName::NumPasses> m_Passes;
	GraphicsContext& m_Context;
	std::shared_ptr<RenderTarget> m_RenderTarget;
	std::shared_ptr<DepthStencilBuffer> m_DepthStencilBuffer;
	ScreenSpaceQuad m_SSQuad;
};

