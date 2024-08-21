#pragma once
#include "Renderer/Drawable.h"

class Plane : public Drawable
{
public:
	Plane(uint32_t resolution, DX::XMMATRIX transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});

	void Draw() override;

	void Update(float dt) override;

private:
	struct PlaneVertex
	{
		DX::XMFLOAT3 Position;
		DX::XMFLOAT3 Normal;
		DX::XMFLOAT3 Tangent;
		DX::XMFLOAT3 Bitangent;
		DX::XMFLOAT2 Texture;
	};

	struct TransformConstantBuffer
	{
		DX::XMMATRIX Model;
		DX::XMMATRIX ModelView;
		DX::XMMATRIX ModelViewProj;
		DX::XMMATRIX NormalMatrix;
	};

	struct PixelConstantBuffer
	{
		//alignas(16) DX::XMFLOAT3 Color;
		float SpecularIntensity;
		float SpecularPower;
		float padding[2];
	};

	void CalculatePlane(uint32_t resolution);
	void InitBuffers();
	void CalculateNormals();
	void CalculateTangentSpace();

private:
	uint32_t m_Resolution;
	std::vector<PlaneVertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::vector<std::shared_ptr<Texture>> m_Textures;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBufferVS = nullptr;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBufferPS = nullptr;
	std::shared_ptr<ConstantBuffer> m_PixelConstantBuffer = nullptr;
};

