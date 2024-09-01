#pragma once
#include "Renderer/DepthStencilMask.h"
#include "DX11Context.h"

class DX11DepthStencilMask : public DepthStencilMask
{
public:
	DX11DepthStencilMask(const DX11Context& context, Mode mode);

	void Bind() const override;

private:
	const DX11Context& m_Context;
	Mode m_Mode;
	ComPtr<ID3D11DepthStencilState> m_Stencil;
};

