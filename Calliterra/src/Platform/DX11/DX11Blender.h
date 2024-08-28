#pragma once
#include "Renderer/Blender.h"
#include "DX11Context.h"

class DX11Blender : public Blender
{
public:
	DX11Blender(const DX11Context& context, bool enableBlending, BlendFunc srcBlend, BlendFunc destBlend, BlendOp blendOp);

	void Bind() const override;

private:
	D3D11_BLEND GetDX11BlendFunc(BlendFunc blendFunc);
	D3D11_BLEND_OP GetDX11BlendOp(BlendOp blendOp);
private:
	const DX11Context& m_Context;
	bool m_Enabled;
	BlendFunc m_Src;
	BlendFunc m_Dest;
	BlendOp m_Op;

	ComPtr<ID3D11BlendState> m_BlendState;
};

