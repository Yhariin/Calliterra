#include "pch.h"
#include "RenderQueue.h"
#include "Renderer/DepthStencilMask.h"
#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"

RenderQueue::RenderQueue(GraphicsContext& context)
	: m_Context(context)
{
	m_RenderTarget = Renderer::CreateRenderTarget();
	m_DepthStencilBuffer = Renderer::CreateDepthStencilBuffer(0, 0, false);
}

void RenderQueue::Accept(const Step& step, uint32_t targetPass)
{
	ASSERT(targetPass < m_Passes.max_size());
	m_Passes[targetPass].Accept(step);
}

void RenderQueue::Execute()
{
	// Setup render target used for all passes
	//m_DepthStencilBuffer->Clear();
	//m_RenderTarget->Clear();
	//m_RenderTarget->BindAsBuffer(m_DepthStencilBuffer.get());
	//m_DepthStencilBuffer->BindAsBuffer(m_RenderTarget.get());
	m_Context.BindSwapBufferDepth();

	// Phong lighting pass
	DepthStencilMask::Resolve(DepthStencilMask::Mode::Off)->Bind();
	m_Passes[0].Execute();

	// Outline masking pass
	DepthStencilMask::Resolve(DepthStencilMask::Mode::Write)->Bind();
	Shader::Resolve("", Shader::PIXEL_SHADER)->Bind();
	m_Passes[1].Execute();

	// Outline drawing pass
	DepthStencilMask::Resolve(DepthStencilMask::Mode::Mask)->Bind();
	m_Passes[2].Execute();

	//m_Context.BindSwapBuffer();
	//m_RenderTarget->BindAsTexture(0);
	//m_SSQuad.Bind();
	//m_SSQuad.Draw();
}

void RenderQueue::Reset()
{
	for(auto& pass : m_Passes)
	{
		pass.Reset();
	}
}

