#pragma once
#include "DX11Context.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"

class DX11VertexBuffer : public VertexBuffer
{
public:
	DX11VertexBuffer(DX11Context& context, float* vertices, size_t elementCount);
	DX11VertexBuffer(DX11Context& context, float** listOfVertexArrays, size_t* listOfElementCounts, size_t bufferCount);

	void Bind() override;
	void Unbind() override;

	void SetLayout(const VertexBufferLayout& layout) override { m_BufferLayoutList[0] = layout; }
	const VertexBufferLayout& GetLayout() override { return m_BufferLayoutList[0]; }
	void SetLayoutList(std::vector<VertexBufferLayout> layoutList) { m_BufferLayoutList = layoutList; }
	const std::vector<VertexBufferLayout>& GetLayoutList() { return m_BufferLayoutList; }

private:
	DX11Context& m_DX11Context;

	std::vector<ComPtr<ID3D11Buffer>> m_VertexBufferList;
	std::vector<VertexBufferLayout> m_BufferLayoutList;
	float** m_VertexArrayList;
	size_t* m_ElementCountList;
	size_t  m_BufferCount;
};

