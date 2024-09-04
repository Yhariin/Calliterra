#pragma once
#include "Base/RenderQueuePass.h"

class OutlineDrawingPass : public RenderQueuePass
{
public:
	OutlineDrawingPass(std::string name)
		: RenderQueuePass(std::move(name))
	{
		RegisterSink(DirectBufferSink<RenderTarget>::Make("renderTarget", m_RenderTarget));
		RegisterSink(DirectBufferSink<DepthStencilBuffer>::Make("depthStencil", m_DepthStencilBuffer));
		RegisterSource(DirectBufferSource<RenderTarget>::Make("renderTarget", m_RenderTarget));
		RegisterSource(DirectBufferSource<DepthStencilBuffer>::Make("depthStencil", m_DepthStencilBuffer));

		AddBind(Shader::Resolve("assets/shaders/FlatColorVS.hlsl", Shader::VERTEX_SHADER));
		AddBind(Shader::Resolve("assets/shaders/FlatColorPS.hlsl", Shader::PIXEL_SHADER));
		AddBind(DepthStencilMask::Resolve(DepthStencilMask::Mode::Mask));
	}
};
