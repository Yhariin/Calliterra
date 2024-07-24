#pragma once
#include "Sandbox/Drawable.h"

class Plane : public Drawable
{
public:
	Plane(uint32_t resolution);
	Plane(uint32_t resolution, DX::XMMATRIX transform);

	void Draw() override;

	void Update(DX::XMMATRIX transform) override;
	void Update(float dt) override;

private:
	void CalculatePlane(uint32_t resolution);
	void InitBuffers();

	std::vector<DX::XMFLOAT3> m_PlaneVertices;
	std::vector<uint32_t> m_PlaneIndices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_ConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_ConstantBuffer2 = nullptr;
};

