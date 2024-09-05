#pragma once
#include "Renderer/Topology.h"
#include "DX11Context.h"

class DX11Topology : public Topology
{
public:
	DX11Topology(const DX11Context& context, PrimitiveTopology primitiveTopology);
	
	void Bind() const override;
private:
	D3D11_PRIMITIVE_TOPOLOGY GetDX11PrimitveTopology(PrimitiveTopology primitiveTopology) const;
private:
	const DX11Context& m_Context;
	PrimitiveTopology m_PrimitveTopology;
};

