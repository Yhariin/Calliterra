#include "pch.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "Drawable.h"

TransformConstantBuffer::TransformConstantBuffer()
{
	if (!m_ConstantBuffer)
	{
		m_ConstantBuffer = ConstantBuffer::Resolve(Shader::VERTEX_SHADER, Transforms(), 0, "TransformCBuff");
	}
}

void TransformConstantBuffer::InitializeParentReference(const Drawable& parent)
{
	m_Parent = &parent;
}

TransformConstantBuffer::Transforms TransformConstantBuffer::GetTransforms() const
{
	ASSERT(m_Parent != nullptr);
	auto& m = m_Parent->GetTransform();
	auto& v = m_Parent->GetViewTransform();
	auto& p = m_Parent->GetProjectionTransform();

	return {
		DX::XMMatrixTranspose(m * v),
		DX::XMMatrixTranspose(m * v * p),
		DX::XMMatrixInverse(nullptr, m * v)
	};
}

void TransformConstantBuffer::Bind() const
{
	Renderer::UpdateConstantBuffer(m_ConstantBuffer, GetTransforms());
	m_ConstantBuffer->Bind();
}
