#include "pch.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "Drawable.h"

TransformConstantBuffer::TransformConstantBuffer()
{
	if (!s_ConstantBuffer)
	{
		s_ConstantBuffer = ConstantBuffer::Resolve(Shader::VERTEX_SHADER, Transforms(), 0, "TransformCBuff");
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
	Renderer::UpdateConstantBuffer(s_ConstantBuffer, GetTransforms());
	s_ConstantBuffer->Bind();
}

SkyBoxTransformConstantBuffer::SkyBoxTransformConstantBuffer()
{	
	if (!s_ConstantBuffer)
	{
		s_ConstantBuffer = ConstantBuffer::Resolve<DX::XMMATRIX>(Shader::VERTEX_SHADER, {}, 0, "SkyBoxTransformCBuff");
	}

}

void SkyBoxTransformConstantBuffer::InitializeParentReference(const Drawable& parent)
{
	m_Parent = &parent;
}

DX::XMMATRIX SkyBoxTransformConstantBuffer::GetTransform() const
{
	ASSERT(m_Parent != nullptr);
	return DX::XMMatrixTranspose(m_Parent->GetViewTransform() * m_Parent->GetProjectionTransform());
}

void SkyBoxTransformConstantBuffer::Bind() const
{
	Renderer::UpdateConstantBuffer(s_ConstantBuffer, GetTransform());
	s_ConstantBuffer->Bind();
}
