#pragma once
#include "Sandbox/Drawable.h"

class Plane : public Drawable
{
public:
	Plane(uint32_t resolution, DX::XMMATRIX transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});

	void Draw() override;

	void Update(DX::XMMATRIX transform) override;
	void Update(float dt) override;

private:
	struct PlaneVertex
	{
		DX::XMFLOAT3 Position;
		DX::XMFLOAT3 Normal;
	};

	struct PlaneTransformConstantBuffer
	{
		DX::XMMATRIX Model;
		DX::XMMATRIX ModelView;
		DX::XMMATRIX ModelViewProj;
		DX::XMMATRIX NormalMatrix;
	};

	struct PlanePixelConstantBuffer
	{
		alignas(16) DX::XMFLOAT3 Color;
		float SpecularIntensity;
		float SpecularPower;
		float padding[2];
	};

	void CalculatePlane(uint32_t resolution);
	void InitBuffers();
	void CalculateNormals();

	std::vector<PlaneVertex> m_PlaneVertices;
	std::vector<uint32_t> m_PlaneIndices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_PlanePixelConstantBuffer = nullptr;
};

