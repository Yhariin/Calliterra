#pragma once
#include "Renderer/Drawable.h"

class IcoSphere : public Drawable
{
public:
	IcoSphere(const uint32_t resolution, DX::XMMATRIX transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = DX::XMFLOAT3(-1.f, -1.f, -1.f));
	
	void Draw() override;

	void Update(float dt) override;

private:
	struct Indices
	{
		int v1;
		int v2;
		int v3;

		Indices(int v1, int v2, int v3)
			: v1(v1), v2(v2), v3(v3)
		{
		}
	};

	void CalculateSphere(const int resolution);
	int GetMidPoint(uint32_t p1, uint32_t p2);
	int AddVertex(DX::XMFLOAT3 point);
	void InitBuffers();

private:
	uint32_t m_Resolution;
	std::vector<DX::XMFLOAT3> m_Vertices;
	std::vector<uint32_t> m_Indices;
	std::vector<Indices> m_TriangleIndices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<Blender> m_Blender = nullptr;
	std::shared_ptr<DepthStencil> m_DepthStencil = nullptr;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_ColorConstantBuffer = nullptr;

	std::unordered_map<long long, uint32_t> m_MiddlePointCache;
	float m_Delta = 0.f;
};

