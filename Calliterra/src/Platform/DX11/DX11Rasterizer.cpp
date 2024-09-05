#include "pch.h"
#include "DX11Rasterizer.h"

DX11Rasterizer::DX11Rasterizer(const DX11Context& context, FillMode fillMode, CullMode cullMode)
	: m_Context(context)
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};

	rasterizerDesc.FillMode = GetDX11FillMode(fillMode);
	rasterizerDesc.CullMode = GetDX11CullMode(cullMode);

	ASSERT_HR(m_Context.GetDevice().CreateRasterizerState(&rasterizerDesc, &m_RasterizerState));
}

void DX11Rasterizer::Bind() const
{
	m_Context.GetDeviceContext().RSSetState(m_RasterizerState.Get());
}

#pragma warning(disable:4715) // Disable warning about no return for all control paths
D3D11_FILL_MODE DX11Rasterizer::GetDX11FillMode(FillMode fillMode)
{
	switch (fillMode)
	{
	case FillMode::Solid:
		return D3D11_FILL_SOLID;
	case FillMode::Wireframe:
		return D3D11_FILL_WIREFRAME;
	}
#pragma warning(default:4715)
}

#pragma warning(disable:4715) // Disable warning about no return for all control paths
D3D11_CULL_MODE DX11Rasterizer::GetDX11CullMode(CullMode cullMode)
{
	switch (cullMode)
	{
	case CullMode::None:
		return D3D11_CULL_NONE;
	case CullMode::Back:
		return D3D11_CULL_BACK;
	case CullMode::Front:
		return D3D11_CULL_FRONT;
	}
#pragma warning(default:4715)
}
