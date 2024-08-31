#include "pch.h"
#include "DX11RenderTarget.h"
#include "DX11DepthStencil.h"

DX11RenderTarget::DX11RenderTarget(DX11Context& context, uint32_t width, uint32_t height)
	: m_Context(context)
{

	// Create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texture;
	ASSERT_HR(m_Context.GetDevice().CreateTexture2D(&textureDesc, nullptr, &texture));

	// Create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	ASSERT_HR(m_Context.GetDevice().CreateShaderResourceView(texture.Get(), &srvDesc, &m_TextureView));

	// Create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

	ASSERT_HR(m_Context.GetDevice().CreateRenderTargetView(texture.Get(), &rtvDesc, &m_TargetView));
}

void DX11RenderTarget::Clear() const
{
	const auto color = m_Context.GetClearColor();
	const float colorBuff[4] = { color.x, color.y, color.z, color.w };
	m_Context.GetDeviceContext().ClearRenderTargetView(m_TargetView.Get(), colorBuff);
}

void DX11RenderTarget::BindAsTexture(uint32_t slot) const 
{
	m_Context.GetDeviceContext().PSSetShaderResources(slot, 1, m_TextureView.GetAddressOf());
}

void DX11RenderTarget::BindAsTarget() const
{
	// We need to unbind the texture from the shader before binding it as a render target
	ID3D11ShaderResourceView* nullView = nullptr;
	m_Context.GetDeviceContext().PSSetShaderResources(0, 1, &nullView);

	m_Context.GetDeviceContext().OMSetRenderTargets(1, m_TargetView.GetAddressOf(), nullptr);
}

void DX11RenderTarget::BindAsTargetDepth() const
{
	// We need to unbind the texture from the shader before binding it as a render target
	ID3D11ShaderResourceView* nullView = nullptr;
	m_Context.GetDeviceContext().PSSetShaderResources(0, 1, &nullView);

	m_Context.GetDeviceContext().OMSetRenderTargets(1, m_TargetView.GetAddressOf(), &m_Context.GetDepthStencilView());
}
