#include "pch.h"
#include "BindingPass.h"
#include "Renderer/Bindable.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/DepthStencilBuffer.h"

BindingPass::BindingPass(std::string name, std::vector<std::shared_ptr<Bindable>> binds)
	: Pass(std::move(name)), m_Binds(binds)
{
}

void BindingPass::AddBind(std::shared_ptr<Bindable> bind)
{
	m_Binds.push_back(std::move(bind));
}

void BindingPass::BindAll() const
{
	BindBufferResources();
	for (auto& bind : m_Binds)
	{
		bind->Bind();
	}
}

void BindingPass::Finalize()
{
	Pass::Finalize();
	ASSERT(m_RenderTarget || m_DepthStencilBuffer, "Binding pass needs at least a render target or depth stencil");
}

void BindingPass::BindBufferResources() const
{
	if (m_RenderTarget)
	{
		m_RenderTarget->BindAsBuffer(m_DepthStencilBuffer.get());
	}
	else
	{
		m_DepthStencilBuffer->BindAsBuffer();
	}
}
