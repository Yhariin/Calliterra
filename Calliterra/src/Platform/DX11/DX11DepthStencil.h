#pragma once
#include "Renderer/DepthStencil.h"
#include "DX11Context.h"

class DX11DepthStencil : public DepthStencil
{
public:
	DX11DepthStencil(const DX11Context& context, Mode mode);

	void Bind() const override;

private:
	const DX11Context& m_Context;
	Mode m_Mode;
	ComPtr<ID3D11DepthStencilState> m_Stencil;
};

