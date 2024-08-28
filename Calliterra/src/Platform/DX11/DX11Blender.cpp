#include "pch.h"
#include "DX11Blender.h"

DX11Blender::DX11Blender(const DX11Context& context, bool enableBlending, BlendFunc srcBlend, BlendFunc destBlend, BlendOp blendOp)
	: m_Context(context), m_Enabled(enableBlending), m_Src(srcBlend), m_Dest(destBlend), m_Op(blendOp)
{
	D3D11_BLEND_DESC blendDesc = CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} };
	D3D11_RENDER_TARGET_BLEND_DESC& brt = blendDesc.RenderTarget[0];

	if (m_Enabled)
	{
		brt.BlendEnable = true;
		brt.SrcBlend = GetDX11BlendFunc(srcBlend);
		brt.DestBlend = GetDX11BlendFunc(destBlend);
		brt.BlendOp = GetDX11BlendOp(blendOp);
	}

	ASSERT_HR(m_Context.GetDevice().CreateBlendState(&blendDesc, &m_BlendState));
}

void DX11Blender::Bind() const
{
	m_Context.GetDeviceContext().OMSetBlendState(m_BlendState.Get(), nullptr, 0xFFFFFFFF);
}

#pragma warning(disable:4715) // Disable warning about no return for all control paths
D3D11_BLEND DX11Blender::GetDX11BlendFunc(BlendFunc blendFunc)
{
	switch (blendFunc)
	{
	case BlendFunc::BLEND_ZERO:
		return D3D11_BLEND_ZERO;

	case BlendFunc::BLEND_ONE:
		return D3D11_BLEND_ONE;

	case BlendFunc::BLEND_SRC_COLOR:
		return D3D11_BLEND_SRC_COLOR;
		
	case BlendFunc::BLEND_INV_SRC_COLOR:
		return D3D11_BLEND_INV_SRC_COLOR;
		
	case BlendFunc::BLEND_SRC_ALPHA:
		return D3D11_BLEND_SRC_ALPHA;

	case BlendFunc::BLEND_INV_SRC_ALPHA:
		return D3D11_BLEND_INV_SRC_ALPHA;

	case BlendFunc::BLEND_DEST_ALPHA:
		return D3D11_BLEND_DEST_ALPHA;

	case BlendFunc::BLEND_INV_DEST_ALPHA:
		return D3D11_BLEND_INV_DEST_ALPHA;

	case BlendFunc::BLEND_DEST_COLOR:
		return D3D11_BLEND_DEST_COLOR;
		
	case BlendFunc::BLEND_INV_DEST_COLOR:
		return D3D11_BLEND_INV_DEST_COLOR;

	case BlendFunc::BLEND_SRC_ALPHA_SAT:
		return D3D11_BLEND_SRC_ALPHA_SAT;
	}

	ASSERT(false, "Invalid Blending Function!");
	#pragma warning(default:4715)
}

#pragma warning(disable:4715) // Disable warning about no return for all control paths
D3D11_BLEND_OP DX11Blender::GetDX11BlendOp(BlendOp blendOp)
{
	switch(blendOp)
	{
	case BlendOp::ADD:
		return D3D11_BLEND_OP_ADD;

	case BlendOp::SUBTRACT:
		return D3D11_BLEND_OP_SUBTRACT;

	case BlendOp::REV_SUBSTRACT:
		return D3D11_BLEND_OP_REV_SUBTRACT;
		
	case BlendOp::MIN:
		return D3D11_BLEND_OP_MIN;

	case BlendOp::MAX:
		return D3D11_BLEND_OP_MAX;
	}

	ASSERT(false, "Invalid Blending Operation!");
	#pragma warning(default:4715)
}
