#include "pch.h"
#include "DX11DepthStencilMask.h"

DX11DepthStencilMask::DX11DepthStencilMask(const DX11Context& context, Mode mode)
	: m_Context(context), m_Mode(mode)
{
	// Default parameters will enable depth buffer and disable stencil
	D3D11_DEPTH_STENCIL_DESC stencilDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	if (m_Mode == Mode::Write)
	{
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilWriteMask = 0xFF;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if (m_Mode == Mode::Mask)
	{
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		stencilDesc.StencilEnable = true;
		stencilDesc.StencilReadMask = 0xFF;
		stencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		stencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}
	else if (m_Mode == Mode::DepthOff)
	{
		stencilDesc.DepthEnable = false;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	else if (m_Mode == Mode::DepthReversed)
	{
		stencilDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	}
	else if (m_Mode == Mode::DepthFirst)
	{
		stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}

	ASSERT_HR(m_Context.GetDevice().CreateDepthStencilState(&stencilDesc, &m_Stencil));
}

void DX11DepthStencilMask::Bind() const
{
	m_Context.GetDeviceContext().OMSetDepthStencilState(m_Stencil.Get(), 0xFF);
}
