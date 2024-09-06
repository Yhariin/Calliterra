#include "pch.h"
#include "Sun.h"
#include "Core/Utils.h"

Sun::Sun()
	: DirectionalLight({1.f, 0.f, 0.f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.5f, 0.5f}, 1.0f)
{
	m_Angle = 270.f;
}

void Sun::Update(float dt)
{
	m_Angle += 1 * dt; // Update the angle by one degree every second;
	m_Angle = Utils::wrapAngleDegrees(m_Angle);

	DX::XMFLOAT4 sunDir(1.f, 0.f, 0.f, 0.f);
	DX::XMMATRIX transform = DX::XMMatrixRotationZ(DX::XMConvertToRadians(m_Angle)) * m_ViewMatrix;
	DX::XMStoreFloat4(&sunDir, DX::XMVector4Normalize(DX::XMVector4Transform(DX::XMLoadFloat4(&sunDir), transform)));
	m_LightProperties.Direction = { sunDir.x, sunDir.y, sunDir.z };
	Renderer::UpdateConstantBuffer(m_LightConstantBuffer, m_LightProperties);

}
