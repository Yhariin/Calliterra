#include "pch.h"
#include "DX11RenderTarget.h"
#include "DX11DepthStencilBuffer.h"

DX11ShaderInputRenderTarget::DX11ShaderInputRenderTarget(DX11Context& context, uint32_t width, uint32_t height, uint32_t slot)
	: m_Context(context), ShaderInputRenderTarget(slot)
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

	// Create the target view on the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

	ASSERT_HR(m_Context.GetDevice().CreateRenderTargetView(texture.Get(), &rtvDesc, &m_TargetView));

	ComPtr<ID3D11Resource> resource;
	m_TargetView->GetResource(&resource);

	// Create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	ASSERT_HR(m_Context.GetDevice().CreateShaderResourceView(resource.Get(), &srvDesc, &m_TextureView));
}

void DX11ShaderInputRenderTarget::Bind() const
{
	m_Context.GetDeviceContext().PSSetShaderResources(m_Slot, 1, m_TextureView.GetAddressOf());
}

void DX11ShaderInputRenderTarget::Clear() const
{
	Clear({0.f, 0.f, 0.f, 0.f});
}

void DX11ShaderInputRenderTarget::Clear(DX::XMFLOAT4 color) const
{
	const float colorBuff[4] = { color.x, color.y, color.z, color.w };
	m_Context.GetDeviceContext().ClearRenderTargetView(m_TargetView.Get(), colorBuff);
}

void DX11ShaderInputRenderTarget::BindAsBuffer() const
{
	ID3D11DepthStencilView* const null = nullptr;
	BindAsBuffer(null);
}

void DX11ShaderInputRenderTarget::BindAsBuffer(BufferResource* depthStencil) const
{
	ASSERT(dynamic_cast<DX11DepthStencilBuffer*>(depthStencil) != nullptr);
	BindAsBuffer(static_cast<DX11DepthStencilBuffer*>(depthStencil));
}

void DX11ShaderInputRenderTarget::BindAsBuffer(const DX11DepthStencilBuffer* depthStencil) const
{
	BindAsBuffer(depthStencil ? depthStencil->GetView().Get() : nullptr);
}

ComPtr<ID3D11RenderTargetView> DX11ShaderInputRenderTarget::GetRenderTargetView() const
{
	return m_TargetView;
}

void DX11ShaderInputRenderTarget::BindAsBuffer(ID3D11DepthStencilView* depthStencilView) const
{
	// We need to unbind the texture from the shader before binding it as a render target
	ID3D11ShaderResourceView* nullView = nullptr;
	m_Context.GetDeviceContext().PSSetShaderResources(0, 1, &nullView);

	m_Context.GetDeviceContext().OMSetRenderTargets(1, m_TargetView.GetAddressOf(), depthStencilView);
}


//=========================================================================================


void DX11OutputOnlyRenderTarget::Bind() const
{
	ASSERT(false, "Cannot bind OutputOnlyRenderTarget as shader input");
}

void DX11OutputOnlyRenderTarget::Clear() const
{
	Clear({0.f, 0.f, 0.f, 0.f});
}

void DX11OutputOnlyRenderTarget::Clear(DX::XMFLOAT4 color) const
{
	const float colorBuff[4] = { color.x, color.y, color.z, color.w };
	m_Context.GetDeviceContext().ClearRenderTargetView(m_TargetView.Get(), colorBuff);
}

void DX11OutputOnlyRenderTarget::BindAsBuffer() const
{
	ID3D11DepthStencilView* const null = nullptr;
	BindAsBuffer(null);
}

void DX11OutputOnlyRenderTarget::BindAsBuffer(BufferResource* depthStencil) const
{
	ASSERT(dynamic_cast<DX11DepthStencilBuffer*>(depthStencil) != nullptr);
	BindAsBuffer(static_cast<DX11DepthStencilBuffer*>(depthStencil));
}

void DX11OutputOnlyRenderTarget::BindAsBuffer(const DX11DepthStencilBuffer* depthStencil) const
{
	BindAsBuffer(depthStencil ? depthStencil->GetView().Get() : nullptr);
}

ComPtr<ID3D11RenderTargetView> DX11OutputOnlyRenderTarget::GetRenderTargetView() const
{
	return m_TargetView;
}

void DX11OutputOnlyRenderTarget::BindAsBuffer(ID3D11DepthStencilView* depthStencilView) const
{
	// We need to unbind the texture from the shader before binding it as a render target
	ID3D11ShaderResourceView* nullView = nullptr;
	m_Context.GetDeviceContext().PSSetShaderResources(0, 1, &nullView);

	m_Context.GetDeviceContext().OMSetRenderTargets(1, m_TargetView.GetAddressOf(), depthStencilView);
}

DX11OutputOnlyRenderTarget::DX11OutputOnlyRenderTarget(DX11Context& context, ID3D11Texture2D* texture)
	: m_Context(context)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };

	ASSERT_HR(m_Context.GetDevice().CreateRenderTargetView(texture, &rtvDesc, &m_TargetView));

}
