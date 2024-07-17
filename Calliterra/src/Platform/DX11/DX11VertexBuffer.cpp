#include "pch.h"
#include "DX11VertexBuffer.h"

DXGI_FORMAT ShaderDataTypeToD3D(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
	case ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
	case ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
	case ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ShaderDataType::Int:		return DXGI_FORMAT_R32_SINT;
	case ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
	case ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
	case ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
	}

	ASSERT(false, "ShaderType Conversion not supported yet.");
	return DXGI_FORMAT_UNKNOWN;
}

// Constructor for creating single buffer
DX11VertexBuffer::DX11VertexBuffer(DX11Context& context, float* vertices, uint32_t elementCount)
	: m_DX11Context(context), m_VertexArrayList(&vertices), m_ElementCountList(&elementCount), m_BufferCount(1)
{
	//m_D3DVertexBufferList.reserve(m_BufferCount);
	//m_D3DBufferLayoutList.reserve(m_BufferCount);
	//m_BufferLayoutList.reserve(m_BufferCount);

	//m_D3DVertexBufferList.push_back(nullptr);
	//m_D3DBufferLayoutList.push_back(nullptr);
	//m_BufferLayoutList.emplace_back(VertexBufferLayout());

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
			//&m_D3DVertexBufferList[0]
		)
	);

}

// Constructor for creating multiple vertex buffers. 
// float** vertices: An array of arrays to vertices
// size_t* count: An array of element counts corresponding to each vertex array
// size_t bufferCount: Number of vertex buffers
DX11VertexBuffer::DX11VertexBuffer(DX11Context& context, float** listOfVertexArrays, uint32_t* listOfElementCounts, uint32_t bufferCount)
	: m_DX11Context(context), m_VertexArrayList(listOfVertexArrays), m_ElementCountList(listOfElementCounts), m_BufferCount(bufferCount)
{
	ASSERT(bufferCount < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Too many buffers are being bound!");
	//m_D3DVertexBufferList.reserve(m_BufferCount);
	//m_D3DBufferLayoutList.reserve(m_BufferCount);


	for (uint32_t i = 0; i < bufferCount; i++)
	{
		//m_D3DVertexBufferList.push_back(nullptr);
		//m_D3DBufferLayoutList.push_back(nullptr);
		//m_BufferLayoutList.emplace_back(VertexBufferLayout());

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
				//&m_D3DVertexBufferList[i]
			)
		);
	}

}

void DX11VertexBuffer::Bind()
{
	std::vector<UINT> strideList;
	strideList.reserve(m_BufferCount);
	for (uint32_t i = 0; i < m_BufferCount; i++)
	{
		//strideList.push_back(static_cast<UINT>(m_BufferLayoutList[i].GetStride()));
		strideList.push_back(static_cast<UINT>(m_BufferLayoutArray[i].GetStride()));
	}

	UINT offset = 0;
	//m_DX11Context.GetDeviceContext().IASetVertexBuffers(0, static_cast<UINT>(m_BufferCount), m_D3DVertexBufferList[0].GetAddressOf(), &strideList[0], &offset);
	m_DX11Context.GetDeviceContext().IASetVertexBuffers(0, static_cast<UINT>(m_BufferCount), m_D3DVertexBufferArray[0].GetAddressOf(), &strideList[0], &offset);
	
}

void DX11VertexBuffer::Unbind()
{
	m_DX11Context.GetDeviceContext().IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
}

void DX11VertexBuffer::SetLayout(int index = 0) 
{
	ASSERT(index < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, "Layout Index out of bounds!");
	//m_DX11Context.GetDeviceContext().IASetInputLayout(m_D3DBufferLayoutList[index].Get());
	m_DX11Context.GetDeviceContext().IASetInputLayout(m_D3DBufferLayoutArray[index].Get());
}

void DX11VertexBuffer::CreateLayout(const VertexBufferLayout& layout)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
	std::vector<BufferElement> layoutElements = layout.GetElements();


	for (int i = 0; i < layoutElements.size(); i++)
	{
		D3D11_INPUT_ELEMENT_DESC tmp;
		tmp.SemanticName = layoutElements[i].Name.c_str();
		tmp.SemanticIndex = layoutElements[i].NameIndex;
		tmp.Format = ShaderDataTypeToD3D(layoutElements[i].Type);
		tmp.InputSlot = 0;
		tmp.AlignedByteOffset = static_cast<UINT>(layoutElements[i].Offset);
		tmp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		tmp.InstanceDataStepRate = 0;

		desc.push_back(tmp);
	}

	ASSERT_HR(
		m_DX11Context.GetDevice().CreateInputLayout(
			&desc[0],
			static_cast<UINT>(desc.size()),
			nullptr, //TODO: ShaderByteCode
			0, //TODO: ShaderByteCodeLength
			//&m_D3DBufferLayoutList[0]
			&m_D3DBufferLayoutArray[0]
		)
	);

}

void DX11VertexBuffer::CreateLayoutList(const std::vector<VertexBufferLayout>& layoutList)
{
	for (int i = 0; i < layoutList.size(); i++)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		std::vector<BufferElement> layoutElements = layoutList[i].GetElements();

		for (int j = 0; j < layoutElements.size(); j++)
		{
			D3D11_INPUT_ELEMENT_DESC tmp;
			tmp.SemanticName = layoutElements[j].Name.c_str();
			tmp.SemanticIndex = layoutElements[j].NameIndex;
			tmp.Format = ShaderDataTypeToD3D(layoutElements[j].Type);
			tmp.InputSlot = 0;
			tmp.AlignedByteOffset = static_cast<UINT>(layoutElements[j].Offset);
			tmp.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			tmp.InstanceDataStepRate = 0;

			desc.push_back(tmp);
		}

		ASSERT_HR(
			m_DX11Context.GetDevice().CreateInputLayout(
				&desc[0],
				static_cast<UINT>(desc.size()),
				nullptr, //TODO: ShaderByteCode
				0, //TODO: ShaderByteCodeLength
				//&m_D3DBufferLayoutList[i]
				&m_D3DBufferLayoutArray[i]
			)
		);

	}

}



