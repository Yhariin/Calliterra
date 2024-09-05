#pragma once
#include "Renderer/DepthStencilBuffer.h"
#include "DX11Context.h"

class DX11ShaderInputRenderTarget;

class DX11DepthStencilBuffer : public DepthStencilBuffer
{
public:
	DX11DepthStencilBuffer(DX11Context& context, uint32_t width, uint32_t height, bool canBindShaderResource);
	void BindAsBuffer() const override;
	void BindAsBuffer(BufferResource* renderTarget) const override;
	void Clear() const override;

	ComPtr<ID3D11DepthStencilView> GetView() const;
	
private:
	void BindAsBuffer(DX11ShaderInputRenderTarget* renderTarget) const;

private:
	DX11Context& m_Context;
	ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
};

