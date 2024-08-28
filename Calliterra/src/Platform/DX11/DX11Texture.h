#pragma once
#include "Renderer/Texture.h"
#include "DX11Context.h"

class DX11Texture : public Texture
{
public:
	DX11Texture(DX11Context& context, const std::string& filepath, uint32_t slot = 0, Filter filter = Filter::Anisotropic);
	
	void Bind() const override;

	bool HasBlending() override { return m_HasAlpha; }

private:
	D3D11_FILTER FilterToD3D(Filter filter); 
private:
	DX11Context& m_DX11Context;
	uint32_t m_Slot;

	std::string m_Filepath;
	int m_Width;
	int m_Height;
	int m_NumChannels;
	const int m_DesiredChannels = 4;
	unsigned char* m_TextureData;
	bool m_HasAlpha = false;

	ComPtr<ID3D11Texture2D> m_Texture;
	ComPtr<ID3D11ShaderResourceView> m_TextureView;
	ComPtr<ID3D11SamplerState> m_SamplerState;
};

