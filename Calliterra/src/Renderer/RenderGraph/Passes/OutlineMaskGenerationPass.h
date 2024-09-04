#pragma once
#include "Base/RenderQueuePass.h"
#include "Renderer/Shader.h"
#include "Renderer/DepthStencilMask.h"

class OutlineMaskGenerationPass : public RenderQueuePass
{
public:
	OutlineMaskGenerationPass(std::string name)
		: RenderQueuePass(std::move(name))
	{
		RegisterSink(DirectBufferSink<DepthStencilBuffer>::Make("depthStencil", m_DepthStencilBuffer));
		RegisterSource(DirectBufferSource<DepthStencilBuffer>::Make("depthStencil", m_DepthStencilBuffer));

		AddBind(Shader::Resolve("assets/shaders/FlatColorVS.hlsl", Shader::VERTEX_SHADER));
		AddBind(Shader::Resolve("", Shader::PIXEL_SHADER));

		AddBind(DepthStencilMask::Resolve(DepthStencilMask::Mode::Write));
	}
};
