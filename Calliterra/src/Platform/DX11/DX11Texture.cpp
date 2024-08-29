#include "pch.h"
#include "DX11Texture.h"
#include "stb_image.h"

DX11Texture::DX11Texture(DX11Context& context, const std::string& filepath, uint32_t slot, Filter filter)
	: m_DX11Context(context), m_Filepath(filepath), m_Slot(slot)
{
	LOG_DEBUG("Loading {}", filepath);
	m_TextureData = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, m_DesiredChannels);
ASSERT(m_TextureData);

	if (m_NumChannels == 1 || m_NumChannels == 4)
	{
		m_HasAlpha = true;
	}

	// Note: It might be worthwhile to first generate the mips on a staging texture then create another
	// immutable texture with only BIND_SHADER_RESOURCE to allow the driver to optimize. We then copy
	// over the data from the first texture onto the new one.
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = m_Width;
	textureDesc.Height = m_Height;
	textureDesc.MipLevels = 0; // 0 will generate all mip levels
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
 
	ASSERT_HR(
		m_DX11Context.GetDevice().CreateTexture2D(&textureDesc, nullptr, &m_Texture)
	);

	m_DX11Context.GetDeviceContext().UpdateSubresource(m_Texture.Get(), 0, nullptr, m_TextureData, m_Width * m_DesiredChannels, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1; // -1 will use all mip levels

ASSERT_HR(
		m_DX11Context.GetDevice().CreateShaderResourceView(m_Texture.Get(), &srvDesc, &m_TextureView)
	);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = FilterToD3D(filter);
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MinLOD = 0.f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ASSERT_HR(
		m_DX11Context.GetDevice().CreateSamplerState(&samplerDesc, &m_SamplerState)
	);

	m_DX11Context.GetDeviceContext().GenerateMips(m_TextureView.Get());
}

void DX11Texture::Bind() const
{
	m_DX11Context.GetDeviceContext().PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
	m_DX11Context.GetDeviceContext().PSSetShaderResources(m_Slot, 1, m_TextureView.GetAddressOf());
}

D3D11_FILTER DX11Texture::FilterToD3D(Filter filter)
{
	switch (filter)
	{
	case Filter::Point:
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	case Filter::Linear:
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	case Filter::Anisotropic:
		return D3D11_FILTER_ANISOTROPIC;
	}

	ASSERT(false, "Filter not supported");
}
