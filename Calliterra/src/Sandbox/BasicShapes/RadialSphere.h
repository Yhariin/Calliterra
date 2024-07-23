#pragma once
#include "Sandbox/Drawable.h"

class RadialSphere : public Drawable
{
public:
	RadialSphere(const int longDiv = 12, const int latDiv = 24);
	RadialSphere(const int latDiv, const int longDiv, DX::XMMATRIX transform);

	void Draw() override;

	virtual void Update(DX::XMMATRIX transform) override;
	virtual void Update(float dt) override;

private:

	void CalculateSphere(const int latDiv, const int longDiv);
	void InitBuffers();

	std::vector<DX::XMVECTOR> m_SphereVertices;
	std::vector<uint32_t> m_SphereIndices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_ConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_ConstantBuffer2 = nullptr;

	float m_Delta = 0.f;
};

