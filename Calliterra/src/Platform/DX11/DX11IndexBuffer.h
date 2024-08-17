#pragma once
#include "Renderer/IndexBuffer.h"
#include "Platform/DX11/DX11Context.h"

class DX11IndexBuffer : public IndexBuffer
{
public:
	DX11IndexBuffer(const DX11Context& context, const std::vector<uint32_t>& indices);

	void Bind() override;

	uint32_t GetCount() const { return m_Count; }

private:
	void CreateBuffer(const std::vector<uint32_t>& indices);
private:
	const DX11Context& m_DX11Context;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	uint32_t m_Count;
};

