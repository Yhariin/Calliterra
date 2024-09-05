#pragma once
#include "Renderer/Rasterizer.h"
#include "DX11Context.h"

class DX11Rasterizer : public Rasterizer
{
public:
	DX11Rasterizer(const DX11Context& context, FillMode fillMode, CullMode cullMode);

	void Bind() const override;

private:
	D3D11_FILL_MODE GetDX11FillMode(FillMode fillMode);
	D3D11_CULL_MODE GetDX11CullMode(CullMode cullMode);
private:
	const DX11Context& m_Context;
	ComPtr<ID3D11RasterizerState> m_RasterizerState;
};

