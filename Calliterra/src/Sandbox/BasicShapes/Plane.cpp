#include "pch.h"
#include "Plane.h"

Plane::Plane(uint32_t resolution, DX::XMMATRIX transform, DX::XMFLOAT3 color )
	: Drawable(transform, color)
{
	CalculatePlane(resolution);
	InitBuffers();
}

void Plane::Draw()
{
	PlaneTransformConstantBuffer cb = { 
		DX::XMMatrixTranspose(m_Transform), 
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix),
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix * m_ProjectionMatrix),
		DX::XMMatrixInverse(nullptr, m_Transform * m_ViewMatrix)
	};

	Renderer::UpdateConstantBuffer(m_TransformConstantBuffer, cb);
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, {}, { m_TransformConstantBuffer, m_PlanePixelConstantBuffer });
	Renderer::Draw();
}

void Plane::Update(float dt)
{

}

void Plane::CalculatePlane(uint32_t resolution)
{
	float div = 1.f / resolution;

	for (uint32_t x = 0; x <= resolution; x++)
	{
		for (uint32_t z = 0; z <= resolution; z++)
		{
			m_PlaneVertices.emplace_back(PlaneVertex(DX::XMFLOAT3(x * div, 0.f, z * div), DX::XMFLOAT3(0.f, 0.f, 0.f)));
		}
	}
	
	const auto calculateIndex = [resolution](auto x, auto z)
		{ return x * (resolution+1) + z; };

	for (uint32_t x = 0; x < resolution; x++)
	{
		for (uint32_t z = 0; z < resolution; z++)
		{
			m_PlaneIndices.push_back(calculateIndex(x, z));
			m_PlaneIndices.push_back(calculateIndex(x, z+1));
			m_PlaneIndices.push_back(calculateIndex(x+1, z));

			m_PlaneIndices.push_back(calculateIndex(x, z+1));
			m_PlaneIndices.push_back(calculateIndex(x+1, z+1));
			m_PlaneIndices.push_back(calculateIndex(x+1, z));
		}
	}

	CalculateNormals();

}

void Plane::InitBuffers()
{
	m_VertexShader = Renderer::CreateShader("assets/shaders/PhongVS.hlsl", Shader::VERTEX_SHADER);
	m_PixelShader = Renderer::CreateShader("assets/shaders/PhongPS.hlsl", Shader::PIXEL_SHADER);
	m_VertexBuffer = Renderer::CreateVertexBuffer(m_PlaneVertices, m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(m_PlaneIndices);

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		});
	m_VertexBuffer->SetLayout();

	m_TransformConstantBuffer = Renderer::CreateConstantBuffer<PlaneTransformConstantBuffer>(Shader::VERTEX_SHADER);

	PlanePixelConstantBuffer pcb = {
		DX::XMFLOAT3(0.3f, 0.3f, 0.3f),
		.3f,
		32.f
	};
	m_PlanePixelConstantBuffer = Renderer::CreateConstantBuffer<PlanePixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);

}

void Plane::CalculateNormals()
{
	using namespace DirectX; // For some reason we need to include this line in order to use the XMMath overloaded operators...
	for (int i = 0; i < m_PlaneVertices.size(); i += 3)
	{
		PlaneVertex& v0 = m_PlaneVertices[m_PlaneIndices[i]];
		PlaneVertex& v1 = m_PlaneVertices[m_PlaneIndices[i + 1]];
		PlaneVertex& v2 = m_PlaneVertices[m_PlaneIndices[i + 2]];
		const XMVECTOR p0 = XMLoadFloat3(&v0.Position);
		const XMVECTOR p1 = XMLoadFloat3(&v1.Position);
		const XMVECTOR p2 = XMLoadFloat3(&v2.Position);

		const XMVECTOR normal = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

		XMStoreFloat3(&v0.Normal, normal);
		XMStoreFloat3(&v1.Normal, normal);
		XMStoreFloat3(&v2.Normal, normal);
	}
}
