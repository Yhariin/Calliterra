#pragma once
#include "Renderer/IndexBuffer.h"
#include "Platform/DX11/DX11Context.h"

class DX11IndexBuffer : public IndexBuffer
{
public:
	DX11IndexBuffer(const DX11Context& context, const uint32_t* indices, uint32_t count);

	void Bind() override;
	void Unbind() override;

	uint32_t GetCount() const { return m_Count; }

private:
	const DX11Context& m_DX11Context;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	uint32_t m_Count;
};

