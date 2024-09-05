#pragma once
#include "Passes/Base/RenderPass.h"
#include "Renderer/RenderTarget.h"
#include "Renderer/DepthStencilBuffer.h"
#include "Core/GlobalSettings.h"
#include "Renderer/Rasterizer.h"

class RenderQueue : public SettingsSubscriber
{
public:
	RenderQueue(GraphicsContext&);

	void Accept(const Step& step, PassName targetPass);
	void Execute();
	void Reset();

	void OnSettingsUpdate(SettingsType type);

private:
	std::array<std::unique_ptr<RenderPass>, (int)PassName::NumPasses> m_Passes;
	GraphicsContext& m_Context;
	std::shared_ptr<RenderTarget> m_BackBuffer;
	std::shared_ptr<DepthStencilBuffer> m_MasterDepthStencilBuffer;
	std::shared_ptr<RenderTarget> m_RenderTarget;

	FillMode m_FillMode = FillMode::Solid;
	CullMode m_CullMode = CullMode::Back;
};

