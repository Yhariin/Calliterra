#include "pch.h"
#include "DX11Topology.h"

DX11Topology::DX11Topology(const DX11Context& context, PrimitiveTopology primitiveTopology)
	: m_Context(context), m_PrimitveTopology(primitiveTopology)
{
}

void DX11Topology::Bind() const
{
	m_Context.GetDeviceContext().IASetPrimitiveTopology(GetDX11PrimitveTopology(m_PrimitveTopology));
}

#pragma warning(disable:4715) // Disable warning about no return for all control paths
D3D11_PRIMITIVE_TOPOLOGY DX11Topology::GetDX11PrimitveTopology(PrimitiveTopology primitiveTopology) const
{
	switch (primitiveTopology)
	{
	case PrimitiveTopology::Points:
		return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	case PrimitiveTopology::Lines:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	case PrimitiveTopology::Triangles:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveTopology::LineStrip:
		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
	case PrimitiveTopology::TriangleStrip:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	}
#pragma warning(default:4715)
}
