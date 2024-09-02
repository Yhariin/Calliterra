#include "pch.h"
#include "BufferClearPass.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/DepthStencilBuffer.h"
#include "Renderer/RenderGraph/Sink.h"
#include "Renderer/RenderGraph/Source.h"

BufferClearPass::BufferClearPass(std::string name)
	: Pass(std::move(name))
{
	RegisterSink(DirectBufferSink<BufferResource>::Make("buffer", m_Buffer));
	RegisterSource(DirectBufferSource<BufferResource>::Make("buffer", m_Buffer));
}

void BufferClearPass::Execute() const
{
	m_Buffer->Clear();
}
