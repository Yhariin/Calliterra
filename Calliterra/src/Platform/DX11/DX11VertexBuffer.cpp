#include "pch.h"
#include "DX11VertexBuffer.h"

DX11VertexBuffer::DX11VertexBuffer(DX11Context& context, float* vertices, size_t elementCount)
	: m_DX11Context(context), m_VertexArrayList(&vertices), m_ElementCountList(&elementCount), m_BufferCount(1)
{
	m_VertexBufferList.reserve(m_BufferCount);
	m_BufferLayoutList.reserve(m_BufferCount);

	for (int i = 0; i < m_BufferCount; i++)
	{
		m_VertexBufferList.push_back(nullptr);
		m_BufferLayoutList.emplace_back(VertexBufferLayout());
	}
	// TODO: Allow for construction of buffer with different flags
	D3D11_BUFFER_DESC vbDesc = {};
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = static_cast<UINT>(m_ElementCountList[0] * sizeof(float));
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexInitData = {};
	vertexInitData.pSysMem = vertices;

	ASSERT_HR(
		m_DX11Context.GetDevice().CreateBuffer(
			&vbDesc,
			&vertexInitData,
			&m_VertexBufferList[0]
		)
	);

}

// Constructor for creating multiple vertex buffers. 
// float** vertices: An array of arrays to vertices
// size_t* count: An array of element counts corresponding to each vertex array
// size_t bufferCount: Number of vertex buffers
DX11VertexBuffer::DX11VertexBuffer(DX11Context& context, float** listOfVertexArrays, size_t* listOfElementCounts, size_t bufferCount)
	: m_DX11Context(context), m_VertexArrayList(listOfVertexArrays), m_ElementCountList(listOfElementCounts), m_BufferCount(bufferCount)
{
	ASSERT(bufferCount < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Too many buffers are being bound!");
	m_VertexBufferList.reserve(bufferCount);
	m_BufferLayoutList.reserve(bufferCount);

	for (int i = 0; i < bufferCount; i++)
	{
		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbDesc.ByteWidth = static_cast<UINT>(m_ElementCountList[i] * sizeof(float));
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = 0;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData = {};
		vertexInitData.pSysMem = m_VertexArrayList[i];

		ASSERT_HR(
			m_DX11Context.GetDevice().CreateBuffer(
				&vbDesc,
				&vertexInitData,
				&m_VertexBufferList[i]
			)
		);
	}

}

void DX11VertexBuffer::Bind()
{
	std::vector<UINT> strideList;
	strideList.reserve(m_BufferCount);
	for (int i = 0; i < m_BufferCount; i++)
	{
		strideList.push_back(static_cast<UINT>(m_BufferLayoutList[i].GetStride()));
	}

	UINT offset = 0;
	m_DX11Context.GetDeviceContext().IASetVertexBuffers(0, static_cast<UINT>(m_BufferCount), m_VertexBufferList[0].GetAddressOf(), &strideList[0], &offset);
	
}

void DX11VertexBuffer::Unbind()
{
	m_DX11Context.GetDeviceContext().IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
}
