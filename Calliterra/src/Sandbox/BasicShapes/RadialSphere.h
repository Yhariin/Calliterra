#pragma once
#include "Renderer/Drawable.h"

class RadialSphere : public Drawable
{
public:
	RadialSphere(const int latDiv = 12, const int longDiv = 24, DX::XMMATRIX transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});

	void LinkTechniques() override;

	void Update(float dt) override;

private:
	void CalculateSphere(const int latDiv, const int longDiv);
	void InitBuffers();

private:
	uint32_t m_LatDiv, m_LongDiv;
	std::vector<DX::XMVECTOR> m_Vertices;
	std::vector<uint32_t> m_Indices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<Blender> m_Blender = nullptr;
	std::shared_ptr<DepthStencilMask> m_DepthStencil = nullptr;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_ColorConstantBuffer = nullptr;

	float m_Delta = 0.f;
};

