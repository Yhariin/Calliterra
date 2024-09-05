#pragma once
#include "Renderer/RenderTarget.h"
#include "Platform/DX11/DX11Context.h"

class DX11DepthStencilBuffer;

class DX11ShaderInputRenderTarget : public ShaderInputRenderTarget
{
public:
	DX11ShaderInputRenderTarget(DX11Context& context, uint32_t width, uint32_t height, uint32_t slot);
	void Bind() const override; // Binds as a texture to be used in a shader
	void Clear() const override;
	void Clear(DX::XMFLOAT4) const override;

	void BindAsBuffer() const override; // Binds as a buffer to render to
	void BindAsBuffer(BufferResource* depthStencil) const override;
	void BindAsBuffer(const DX11DepthStencilBuffer* depthStencil) const;

	ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const ;

private:
	void BindAsBuffer(ID3D11DepthStencilView* depthStencilView) const;

private:
	DX11Context& m_Context;
	ComPtr<ID3D11RenderTargetView> m_TargetView;
	ComPtr<ID3D11ShaderResourceView> m_TextureView;
};


//=========================================================================================


class DX11OutputOnlyRenderTarget : public OutputOnlyRenderTarget
{
	friend class DX11Context;
public:
	void Bind() const override;
	void Clear() const override;
	void Clear(DX::XMFLOAT4) const override;

	void BindAsBuffer() const override;
	void BindAsBuffer(BufferResource* depthStencil) const override;
	void BindAsBuffer(const DX11DepthStencilBuffer* depthStencil) const;

	ComPtr<ID3D11RenderTargetView> GetRenderTargetView() const ;
private:
	void BindAsBuffer(ID3D11DepthStencilView* depthStencilView) const;
	DX11OutputOnlyRenderTarget(DX11Context& context, ID3D11Texture2D* texture);

private:
	DX11Context& m_Context;
	ComPtr<ID3D11RenderTargetView> m_TargetView;
};
