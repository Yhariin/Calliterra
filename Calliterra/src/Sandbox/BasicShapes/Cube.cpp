#include "pch.h"
#include "Cube.h"

Cube::Cube(DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{ 
	InitRNG();
}

void Cube::InitRNG()
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> aDist(0.0f, std::numbers::pi_v<float> * 2.f);
	std::uniform_real_distribution<float> dDist(0.0f, std::numbers::pi_v<float> * 2.f);
	std::uniform_real_distribution<float> oDist(0.0f, std::numbers::pi_v<float> * 0.3f);
	std::uniform_real_distribution<float> rDist(6.0f, 19.f);

	m_R = rDist(rng);
	m_Roll = 0.f;
	m_Pitch = 0.f; 
	m_Yaw = 0.f;
	m_Theta = aDist(rng);
	m_Phi = aDist(rng);
	m_Chi = aDist(rng);
	// Speed
	m_dRoll = dDist(rng);
	m_dPitch = dDist(rng);
	m_dYaw = dDist(rng);
	m_dTheta = oDist(rng);
	m_dPhi = oDist(rng);
	m_dChi = oDist(rng);

}

void Cube::InitBuffers()
{
	CalculateNormals();
	s_VertexShader = Renderer::CreateShader("assets/shaders/PhongVS.hlsl", Shader::VERTEX_SHADER);
	s_PixelShader = Renderer::CreateShader("assets/shaders/PhongPS.hlsl", Shader::PIXEL_SHADER);
	s_VertexBuffer = Renderer::CreateVertexBuffer(m_IndependentCubeVertices, static_cast<uint32_t>(m_IndependentCubeVertices.size()), s_VertexShader.get());
	s_IndexBuffer = Renderer::CreateIndexBuffer(m_IndependentCubeIndices, m_INDEXCOUNT);

	s_VertexShader->Bind();
	s_PixelShader->Bind();

	s_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		});
	s_VertexBuffer->SetLayout();
		
	s_TransformConstantBuffer = Renderer::CreateConstantBuffer<CubeTransformConstantBuffer>(Shader::VERTEX_SHADER);
	s_ColorConstantBuffer = Renderer::CreateConstantBuffer<DX::XMFLOAT4>(Shader::PIXEL_SHADER, {0.7f, 0.7f, 0.9f, 0.0f}, 1);

}

void Cube::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;
}

void Cube::Update(float dt)
{
	dt *= 0.5;

	m_Roll += m_dRoll * dt;
	m_Pitch += m_dPitch * dt;
	m_Yaw += m_dYaw * dt;
	m_Theta += m_dTheta * dt;
	m_Phi += m_dPhi * dt;
	m_Chi += m_dChi * dt;

	m_Transform =
		DX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll) *
		DX::XMMatrixTranslation(m_R, 0.f, 0.f) *
		DX::XMMatrixRotationRollPitchYaw(m_Theta, m_Phi, m_Chi) * 
		DX::XMMatrixTranslation(0.f, 20.f, 10.f);
}

void Cube::Draw()
{
	CubeTransformConstantBuffer cb = { DX::XMMatrixTranspose(m_Transform), DX::XMMatrixTranspose(m_Transform * m_ViewProjectionMatrix) };

	Renderer::UpdateConstantBuffer(s_TransformConstantBuffer, cb);
	Renderer::Bind({ s_VertexShader, s_PixelShader }, s_VertexBuffer, s_IndexBuffer, { s_TransformConstantBuffer, s_ColorConstantBuffer });
	Renderer::Draw();

}

void Cube::CalculateNormals()
{
	using namespace DirectX; // For some reason we need to include this line in order to use the XMMath overloaded operators...
	for (int i = 0; i < m_IndependentCubeVertices.size(); i += 3)
	{
		CubeVertex& v0 = m_IndependentCubeVertices[m_IndependentCubeIndices[i]];
		CubeVertex& v1 = m_IndependentCubeVertices[m_IndependentCubeIndices[i+1]];
		CubeVertex& v2 = m_IndependentCubeVertices[m_IndependentCubeIndices[i+2]];
		const XMVECTOR p0 = XMLoadFloat3(&v0.Position);
		const XMVECTOR p1 = XMLoadFloat3(&v1.Position);
		const XMVECTOR p2 = XMLoadFloat3(&v2.Position);

		const XMVECTOR normal = XMVector3Normalize(XMVector3Cross( (p1 - p0), (p2 - p0)) );

		XMStoreFloat3(&v0.Normal, normal);
		XMStoreFloat3(&v1.Normal, normal);
		XMStoreFloat3(&v2.Normal, normal);
	}
}
