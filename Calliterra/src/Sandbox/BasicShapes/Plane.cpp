#include "pch.h"
#include "Plane.h"

Plane::Plane(uint32_t resolution)
{
	CalculatePlane(resolution);
	InitBuffers();
}

Plane::Plane(uint32_t resolution, DX::XMMATRIX transform)
	: Drawable(transform)
{
	CalculatePlane(resolution);
	InitBuffers();
}

void Plane::Draw()
{
	Renderer::UpdateConstantBuffer(m_ConstantBuffer, DX::XMMatrixTranspose(m_Transform * m_ViewProjectionMatrix));
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer);
	Renderer::Draw();
}

void Plane::Update(float dt)
{

}

void Plane::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;
}

void Plane::CalculatePlane(uint32_t resolution)
{
	float div = 1.f / resolution;

	for (uint32_t x = 0; x <= resolution; x++)
	{
		for (uint32_t z = 0; z <= resolution; z++)
		{
			m_PlaneVertices.emplace_back(DX::XMFLOAT3(x * div, 0.f, z * div));
		}
	}

	m_PlaneIndices.push_back(0);
	m_PlaneIndices.push_back(1);
	m_PlaneIndices.push_back(3);

	m_PlaneIndices.push_back(3);
	m_PlaneIndices.push_back(2);
	m_PlaneIndices.push_back(0);

	for (uint32_t x = 0; x < resolution; x++)
	{
		for (uint32_t z = 0; z < resolution; z++)
		{

		}
	}
}

void Plane::InitBuffers()
{
	m_VertexShader = Renderer::CreateShader("src/assets/shaders/ColorIndexVS.hlsl", Shader::VERTEX_SHADER);
	m_PixelShader = Renderer::CreateShader("src/assets/shaders/PlaneFlatPS.hlsl", Shader::PIXEL_SHADER);
	m_VertexBuffer = Renderer::CreateVertexBuffer(m_PlaneVertices, static_cast<uint32_t>(m_PlaneVertices.size()), m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(&m_PlaneIndices[0], static_cast<uint32_t>(m_PlaneIndices.size()));

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		});
	m_VertexBuffer->SetLayout();

	m_ConstantBuffer = Renderer::CreateConstantBuffer<DX::XMMATRIX>(Shader::VERTEX_SHADER);
	m_ConstantBuffer2 = Renderer::CreateConstantBuffer<DX::XMVECTOR>(Shader::PIXEL_SHADER);
	m_ConstantBuffer->Bind();
	m_ConstantBuffer2->Bind();
	Renderer::UpdateConstantBuffer(m_ConstantBuffer2, DX::XMVECTOR({ 0.3f, 0.3f, 0.3f, 1.f }));

}