#pragma once
#include "Renderer/RenderTarget.h"
#include "Platform/DX11/DX11Context.h"

class DX11RenderTarget : public RenderTarget
{
public:
	DX11RenderTarget(DX11Context& context, uint32_t width, uint32_t height);

	void Clear() const override;

	void BindAsTexture(uint32_t slot) const override;
	void BindAsTarget() const override;
	void BindAsTargetDepth() const override;

private:
	DX11Context& m_Context;
	ComPtr<ID3D11ShaderResourceView> m_TextureView;
	ComPtr<ID3D11RenderTargetView> m_TargetView;
};

