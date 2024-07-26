#include "pch.h"
#include "DX11IndexBuffer.h"

DX11IndexBuffer::DX11IndexBuffer(const DX11Context& context, const uint32_t* indices, uint32_t count)
	: m_DX11Context(context), m_Count(count)
{
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(uint32_t) * m_Count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexInitData = {};
	indexInitData.pSysMem = indices;

	ASSERT_HR(
		m_DX11Context.GetDevice().CreateBuffer(
			&indexBufferDesc,
			&indexInitData,
			&m_IndexBuffer
		)
	);

	LOG_INFO("Index Buffer Created");

}

void DX11IndexBuffer::Bind()
{
	m_DX11Context.GetDeviceContext().IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}
