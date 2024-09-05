#include "pch.h"
#include "RenderQueue.h"
#include "Renderer/DepthStencilMask.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Passes/ClearBufferPass.h"
#include "Passes/LambertianPass.h"
#include "Passes/OutlineMaskPass.h"
#include "Passes/OutlineDrawPass.h"
#include "Passes/ColorInvertPass.h"

RenderQueue::RenderQueue(GraphicsContext& context)
	: m_Context(context), m_BackBuffer(context.GetBackBufferTarget())
{
	m_RenderTarget = Renderer::CreateRenderTarget();
	m_MasterDepthStencilBuffer = Renderer::CreateDepthStencilBuffer(0, 0, false);

	m_Passes[(int)PassName::ClearRenderTarget] = std::make_unique<ClearBufferPass>(m_RenderTarget);
	m_Passes[(int)PassName::ClearDepthStencilBuffer] = std::make_unique<ClearBufferPass>(m_MasterDepthStencilBuffer);
	m_Passes[(int)PassName::Lambertian] = std::make_unique<LambertianPass>(m_RenderTarget, m_MasterDepthStencilBuffer);
	m_Passes[(int)PassName::OutlineMask] = std::make_unique<OutlineMaskPass>();
	m_Passes[(int)PassName::OutlineDraw] = std::make_unique<OutlineDrawPass>();
	m_Passes[(int)PassName::ColorInvert] = std::make_unique<ColorInvertPass>(m_Context, m_BackBuffer, m_RenderTarget);

}

void RenderQueue::Accept(const Step& step, PassName targetPass)
{
	// Only passes that inherit from StepPass should be allowed to accept a step
	switch (targetPass)
	{
	case PassName::Lambertian:
	{
		auto p = dynamic_cast<LambertianPass*>(m_Passes[(int)targetPass].get());
		ASSERT(p, "Cannot add Steps to this pass type");
		p->Accept(step);
		break;
	}
	case PassName::OutlineMask:
	{
		auto p = dynamic_cast<OutlineMaskPass*>(m_Passes[(int)targetPass].get());
		ASSERT(p, "Cannot add Steps to this pass type");
		p->Accept(step);
		break;
	}
	case PassName::OutlineDraw:
	{
		auto p = dynamic_cast<OutlineDrawPass*>(m_Passes[(int)targetPass].get());
		ASSERT(p, "Cannot add Steps to this pass type");
		p->Accept(step);
		break;
	}
	default:
		ASSERT(false, "Cannot add Steps to this pass type");
	}

}

void RenderQueue::Execute()
{
	for (const auto& pass : m_Passes)
	{
		pass->Execute();
	}
}

void RenderQueue::Reset()
{
	for(auto& pass : m_Passes)
	{
		pass->Reset();
	}
}

