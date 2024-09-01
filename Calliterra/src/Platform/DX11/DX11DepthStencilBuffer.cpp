#include "pch.h"
#include "DX11DepthStencilBuffer.h"
#include "DX11RenderTarget.h"

DX11DepthStencilBuffer::DX11DepthStencilBuffer(DX11Context& context, uint32_t width, uint32_t height, bool canBindShaderInput)
	: m_Context(context)
{
	// Create depth stencil texture
	ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | (canBindShaderInput ? D3D11_BIND_SHADER_RESOURCE : 0);

	ASSERT_HR(m_Context.GetDevice().CreateTexture2D(&descDepth, nullptr, &depthStencil));

	ASSERT_HR(m_Context.GetDevice().CreateDepthStencilView(depthStencil.Get(), nullptr, &m_DepthStencilView));
}

void DX11DepthStencilBuffer::BindAsBuffer() const
{
	m_Context.GetDeviceContext().OMSetRenderTargets(0, nullptr, m_DepthStencilView.Get());
}

void DX11DepthStencilBuffer::BindAsBuffer(BufferResource* renderTarget) const
{
	ASSERT(dynamic_cast<DX11RenderTarget*>(renderTarget) != nullptr);
	BindAsBuffer(static_cast<DX11RenderTarget*>(renderTarget));
}

void DX11DepthStencilBuffer::BindAsBuffer(DX11RenderTarget* renderTarget) const
{
	renderTarget->BindAsBuffer(this);
}

void DX11DepthStencilBuffer::Clear() const
{
	m_Context.GetDeviceContext().ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

ComPtr<ID3D11DepthStencilView> DX11DepthStencilBuffer::GetView() const
{
	return m_DepthStencilView;
}
