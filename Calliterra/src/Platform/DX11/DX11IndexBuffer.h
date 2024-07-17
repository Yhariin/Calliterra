#pragma once
#include "Renderer/IndexBuffer.h"
#include "Platform/DX11/DX11Context.h"

class DX11IndexBuffer : public IndexBuffer
{
public:
	DX11IndexBuffer(DX11Context& context, uint32_t* indices, uint32_t count);

	void Bind() override;
	void Unbind() override;

	uint32_t GetCount() { return m_Count; }

private:
	DX11Context& m_DX11Context;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	uint32_t m_Count;
};

