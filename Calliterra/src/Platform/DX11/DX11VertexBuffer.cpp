#include "pch.h"
#include "DX11VertexBuffer.h"


/*
// Constructor for creating single buffer
DX11VertexBuffer::DX11VertexBuffer(const DX11Context& context, const float* vertices, uint32_t elementCount, ComPtr<ID3DBlob> shaderByteCode)
	: m_DX11Context(context), m_VertexArrayList(&vertices), m_ElementCountList(&elementCount), m_BufferCount(1), m_ShaderByteCode(shaderByteCode)
{
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
			&m_D3DVertexBufferArray[0]
		)
	);

	LOG_INFO("Vertex Buffer Created");
}

// Constructor for creating multiple vertex buffers. 
// float** vertices: An array of arrays to vertices
// size_t* count: An array of element counts corresponding to each vertex array
// size_t bufferCount: Number of vertex buffers
DX11VertexBuffer::DX11VertexBuffer(const DX11Context& context, const float** listOfVertexArrays, const uint32_t* listOfElementCounts, uint32_t bufferCount, ComPtr<ID3DBlob> shaderByteCode)
	: m_DX11Context(context), m_VertexArrayList(listOfVertexArrays), m_ElementCountList(listOfElementCounts), m_BufferCount(bufferCount), m_ShaderByteCode(shaderByteCode)
{
	ASSERT(bufferCount < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Too many buffers are being bound!");

	for (uint32_t i = 0; i < bufferCount; i++)
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
				&m_D3DVertexBufferArray[i]
			)
		);
	}

	LOG_INFO("Vertex Buffer(s) Created");
}
*/



