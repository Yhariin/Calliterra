#include "pch.h"
#include "RadialSphere.h"

RadialSphere::RadialSphere(const int latDiv, const int longDiv)
{
	CalculateSphere(latDiv, longDiv);
	InitBuffers();
}

RadialSphere::RadialSphere(const int latDiv, const int longDiv, DX::XMMATRIX transform)
	: Drawable(transform)
{
	CalculateSphere(latDiv, longDiv);
	InitBuffers();
}

void RadialSphere::Draw()
{
	Renderer::UpdateConstantBuffer(m_ConstantBuffer, m_Transform);
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer);
	Renderer::Draw();
}

void RadialSphere::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;
}

void RadialSphere::Update(float dt)
{
	m_Delta += 2.f * dt;
	m_Transform = DX::XMMatrixTranspose(
		DX::XMMatrixRotationRollPitchYaw(m_Delta, m_Delta, m_Delta) *
		DX::XMMatrixTranslation(0.f, 0.f, 2.f) *
		DX::XMMatrixPerspectiveFovLH(90.f, 16.f/9.f, 0.1f, 10.f)
	);
}

void RadialSphere::CalculateSphere(const int longDiv, const int latDiv)
{
	ASSERT(longDiv >= 3);
	ASSERT(latDiv >= 3);
	constexpr float radius = 1.f;
	constexpr DX::XMVECTORF32 base = { 0.f, 0.f, radius, 0.f };
	const float lattitudeAngle = static_cast<float>(2.0f * std::numbers::pi) / latDiv;
	const float longitudeAngle = static_cast<float>(std::numbers::pi) / longDiv;

	for (int iLong = 1; iLong < longDiv; iLong++)
	{
		const auto longBase = DX::XMVector3Transform(
			base,
			DX::XMMatrixRotationX(longitudeAngle * iLong)
		);

		for (int iLat = 0; iLat < latDiv; iLat++)
		{
			m_SphereVertices.emplace_back(DX::XMVector3Transform(longBase, DX::XMMatrixRotationZ(lattitudeAngle * iLat)));
		}
	}

	// Add the cap vertices
	const uint32_t iNorthPole = static_cast<uint32_t>(m_SphereVertices.size());
	m_SphereVertices.emplace_back(base);
	const uint32_t iSouthPole = static_cast<uint32_t>(m_SphereVertices.size());
	m_SphereVertices.emplace_back(DX::XMVectorNegate(base));

	const auto calculateIndex = [latDiv, longDiv](auto iLong, auto iLat)
		{ return iLong * latDiv + iLat; };

	for (int iLong = 0; iLong < longDiv - 2; iLong++)
	{
		for (int iLat = 0; iLat < latDiv - 1; iLat++)
		{
			m_SphereIndices.push_back(calculateIndex(iLong, iLat));
			m_SphereIndices.push_back(calculateIndex(iLong, iLat+1));
			m_SphereIndices.push_back(calculateIndex(iLong+1, iLat));
			m_SphereIndices.push_back(calculateIndex(iLong, iLat+1));
			m_SphereIndices.push_back(calculateIndex(iLong+1, iLat+1));
			m_SphereIndices.push_back(calculateIndex(iLong+1, iLat));
		}
		// Wrap band
		m_SphereIndices.push_back(calculateIndex(iLong, latDiv-1));
		m_SphereIndices.push_back(calculateIndex(iLong, 0));
		m_SphereIndices.push_back(calculateIndex(iLong+1, latDiv-1));
		m_SphereIndices.push_back(calculateIndex(iLong, 0));
		m_SphereIndices.push_back(calculateIndex(iLong+1, 0));
		m_SphereIndices.push_back(calculateIndex(iLong+1, latDiv-1));
	}

	// Cap fans
	for (int iLat = 0; iLat < latDiv - 1; iLat++)
	{
		// North
		m_SphereIndices.push_back(iNorthPole);
		m_SphereIndices.push_back(calculateIndex(0, iLat+1));
		m_SphereIndices.push_back(calculateIndex(0, iLat));

		// South
		m_SphereIndices.push_back(calculateIndex(longDiv - 2, iLat));
		m_SphereIndices.push_back(calculateIndex(longDiv - 2, iLat+1));
		m_SphereIndices.push_back(iSouthPole);
	}

	m_SphereIndices.push_back(iNorthPole);
	m_SphereIndices.push_back(calculateIndex(0, latDiv-1));
	m_SphereIndices.push_back(calculateIndex(0, 0));

	m_SphereIndices.push_back(calculateIndex(longDiv -2, latDiv-1));
	m_SphereIndices.push_back(calculateIndex(longDiv -2, 0));
	m_SphereIndices.push_back(iSouthPole);

}

void RadialSphere::InitBuffers()
{
	m_VertexShader = Renderer::CreateShader("src/assets/shaders/ColorIndexVS.hlsl", Shader::VERTEX_SHADER);
	m_PixelShader = Renderer::CreateShader("src/assets/shaders/ColorIndexPS.hlsl", Shader::PIXEL_SHADER);
	m_VertexBuffer = Renderer::CreateVertexBuffer(m_SphereVertices, static_cast<uint32_t>(m_SphereVertices.size()), m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(&m_SphereIndices[0], static_cast<uint32_t>(m_SphereIndices.size()));

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float4},
		});
	m_VertexBuffer->SetLayout();

	m_ConstantBuffer = Renderer::CreateConstantBuffer<DX::XMMATRIX>(Shader::VERTEX_SHADER);
	m_ConstantBuffer2 = Renderer::CreateConstantBuffer<FaceColorsBuffer>(Shader::PIXEL_SHADER, m_ColorsBuffer);
	m_ConstantBuffer->Bind();
	m_ConstantBuffer2->Bind();
}

