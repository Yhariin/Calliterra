#include "pch.h"
#include "RenderQueue.h"
#include "Renderer/DepthStencil.h"
#include "Renderer/Shader.h"

void RenderQueue::Accept(const Step& step, uint32_t targetPass)
{
	ASSERT(targetPass < m_Passes.max_size());
	m_Passes[targetPass].Accept(step);
}

void RenderQueue::Execute()
{
	// Phong lighting pass
	DepthStencil::Resolve(DepthStencil::Mode::Off)->Bind();
	m_Passes[0].Execute();

	// Outline masking pass
	DepthStencil::Resolve(DepthStencil::Mode::Write)->Bind();
	Shader::Resolve("", Shader::PIXEL_SHADER)->Bind();
	m_Passes[1].Execute();

	// Outline drawing pass
	DepthStencil::Resolve(DepthStencil::Mode::Mask)->Bind();
	m_Passes[2].Execute();

}

void RenderQueue::Reset()
{
	for(auto& pass : m_Passes)
	{
		pass.Reset();
	}
}
