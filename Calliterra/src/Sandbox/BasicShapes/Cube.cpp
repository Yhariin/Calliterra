#include "pch.h"
#include "Cube.h"

Cube::Cube()
{
	InitRNG();
}

Cube::Cube(DX::XMMATRIX transform)
	: Drawable(transform)
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
	s_VertexShader = Renderer::CreateShader("src/assets/shaders/ColorIndexVS.hlsl", Shader::VERTEX_SHADER);
	s_PixelShader = Renderer::CreateShader("src/assets/shaders/ColorIndexPS.hlsl", Shader::PIXEL_SHADER);
	s_VertexBuffer = Renderer::CreateVertexBuffer(m_CubeVertices, m_VERTEXCOUNT, s_VertexShader.get());
	s_IndexBuffer = Renderer::CreateIndexBuffer(m_CubeIndices, m_INDEXCOUNT);

	s_VertexShader->Bind();
	s_PixelShader->Bind();

	s_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		});
	s_VertexBuffer->SetLayout();
		
	s_TransformConstantBuffer = Renderer::CreateConstantBuffer<DX::XMMATRIX>(Shader::VERTEX_SHADER);
	s_ColorConstantBuffer = Renderer::CreateConstantBuffer<FaceColorsBuffer>(Shader::PIXEL_SHADER, m_ColorsBuffer);

}

void Cube::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;
}

void Cube::Update(float dt)
{
	m_Roll += m_dRoll * dt;
	m_Pitch += m_dPitch * dt;
	m_Yaw += m_dYaw * dt;
	m_Theta += m_dTheta * dt;
	m_Phi += m_dPhi * dt;
	m_Chi += m_dChi * dt;

	m_Transform =
		//DX::XMMatrixTranslation(0.f, 20.f, 0.f) *
		DX::XMMatrixRotationRollPitchYaw(m_Pitch, m_Yaw, m_Roll) *
		DX::XMMatrixTranslation(m_R, 0.f, 0.f) *
		DX::XMMatrixRotationRollPitchYaw(m_Theta, m_Phi, m_Chi) * 
		DX::XMMatrixTranslation(0.f, 20.f, 10.f);
}

void Cube::Draw()
{
	Renderer::UpdateConstantBuffer(s_TransformConstantBuffer, DX::XMMatrixTranspose(m_Transform * m_ViewProjectionMatrix));
	Renderer::Bind({ s_VertexShader, s_PixelShader }, s_VertexBuffer, s_IndexBuffer, { s_TransformConstantBuffer, s_ColorConstantBuffer });
	Renderer::Draw();

}
