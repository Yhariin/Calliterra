#pragma once
#include "Base/RenderQueuePass.h"
#include "Renderer/RenderGraph/Sink.h"
#include "Renderer/RenderGraph/Source.h"

class LambertianPass : public RenderQueuePass
{
public:
	LambertianPass(std::string name)
		: RenderQueuePass(std::move(name))
	{
		RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_RenderTarget));
		RegisterSink(DirectBufferSink<DepthStencilBuffer>::Make("depthStencil", m_DepthStencilBuffer));
		RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_RenderTarget));
		RegisterSource(DirectBufferSource<DepthStencilBuffer>::Make("depthStencil", m_DepthStencilBuffer));
		AddBind(DepthStencilMask::Resolve(DepthStencilMask::Mode::Off));
	}

};

