#pragma once
#include "Renderer/RenderTarget.h"
#include "Platform/DX11/DX11Context.h"

class DX11DepthStencilBuffer;

class DX11RenderTarget : public RenderTarget
{
public:
	DX11RenderTarget(DX11Context& context, uint32_t width, uint32_t height);
	DX11RenderTarget(DX11Context& context, ID3D11Texture2D* texture);

	void Clear() const override;

	void BindAsTexture(uint32_t slot) const override;
	void BindAsBuffer() const override;
	void BindAsBuffer(BufferResource* depthStencil) const override;
	void BindAsBuffer(const DX11DepthStencilBuffer* depthStencil) const;

	ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const ;

private:
	void BindAsBuffer(ID3D11DepthStencilView* depthStencilView) const;

private:
	DX11Context& m_Context;
	ComPtr<ID3D11ShaderResourceView> m_TextureView;
	ComPtr<ID3D11RenderTargetView> m_TargetView;
};

