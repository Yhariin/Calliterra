#pragma once
#include "DX11Context.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/BufferLayout.h"
#include "Platform/DX11/DX11Shader.h"

class DX11VertexBuffer : public VertexBuffer
{
public:
	DX11VertexBuffer(DX11Context& context, float* vertices, uint32_t elementCount, ComPtr<ID3DBlob> shaderByteCode);
	DX11VertexBuffer(DX11Context& context, float** listOfVertexArrays, uint32_t* listOfElementCounts, uint32_t bufferCount, ComPtr<ID3DBlob> vertexShader);

	void Bind() override;
	void Unbind() override;

	void SetLayout(int index) override;
	void CreateLayout(const VertexBufferLayout& layout) override;
	void CreateLayoutList(const std::vector<VertexBufferLayout>& layoutList) override;

private:
	DX11Context& m_DX11Context;

	float** m_VertexArrayList;
	uint32_t * m_ElementCountList;
	uint32_t m_BufferCount;

	ComPtr<ID3D11Buffer> m_D3DVertexBufferArray[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	ComPtr<ID3D11InputLayout> m_D3DBufferLayoutArray[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
	VertexBufferLayout m_BufferLayoutArray[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];

	ComPtr<ID3DBlob> m_ShaderByteCode;
};

