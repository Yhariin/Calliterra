#pragma once
#include "DX11Context.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"

class DX11VertexBuffer : public VertexBuffer
{
public:
	DX11VertexBuffer(DX11Context& context, float* vertices, uint32_t elementCount);
	DX11VertexBuffer(DX11Context& context, float** listOfVertexArrays, uint32_t* listOfElementCounts, uint32_t bufferCount);

	void Bind() override;
	void Unbind() override;

	//void SetLayout(const VertexBufferLayout& layout) override { m_D3DBufferLayoutList[0] = layout; }
	void SetLayout(int index) override;
	void CreateLayout(const VertexBufferLayout& layout) override;
	void CreateLayoutList(const std::vector<VertexBufferLayout>& layoutList) override;

private:
	DX11Context& m_DX11Context;

	//std::vector<ComPtr<ID3D11Buffer>> m_D3DVertexBufferList;
	//std::vector<ComPtr<ID3D11InputLayout>> m_D3DBufferLayoutList;
	//std::vector<VertexBufferLayout> m_BufferLayoutList;
	float** m_VertexArrayList;
	uint32_t * m_ElementCountList;
	uint32_t m_BufferCount;

	ComPtr<ID3D11Buffer> m_D3DVertexBufferArray[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
	ComPtr<ID3D11InputLayout> m_D3DBufferLayoutArray[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
	VertexBufferLayout m_BufferLayoutArray[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = {};
};

