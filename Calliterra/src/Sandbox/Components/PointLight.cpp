#include "pch.h"
#include "PointLight.h"

PointLight::PointLight(DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: m_Sphere(3, transform, color), Drawable(transform, color)
{
	DX::XMStoreFloat4(&m_Position, DX::XMVector4Transform(DX::XMLoadFloat4(&m_Position), transform));
	InitBuffers();

	GlobalSettings::Register(SettingsType::PointLightPosition, this);
}

void PointLight::Draw()
{
	m_Sphere.Draw();
	m_PixelShader->Bind();
	DX::XMFLOAT3 posViewSpace;
	DX::XMStoreFloat3(&posViewSpace, DX::XMVector3Transform(DX::XMLoadFloat4(&m_Position), m_ViewMatrix));
	PointLightConstantBuffer cb = {
		posViewSpace,
		{ 0.15f, 0.15f, 0.15f },
		{ 1.f, 1.f, 1.f },
		1.f,
		1.f,
		0.015f,
		0.0015f
	};
	Renderer::UpdateConstantBuffer(m_PositionConstantBuffer, cb);

	m_PositionConstantBuffer->Bind({});
}

void PointLight::OnSettingsUpdate(SettingsType type)
{
	m_Position = DX::XMFLOAT4(GlobalSettings::Rendering::LightPos().x, GlobalSettings::Rendering::LightPos().y, GlobalSettings::Rendering::LightPos().z, 1.0f);
}

void PointLight::Update(float dt)
{

	m_Transform = DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_Sphere.SetTransform(m_Transform);
}

void PointLight::InitBuffers()
{
	m_PixelShader = Renderer::CreateShader("assets/shaders/PhongPS.hlsl", Shader::PIXEL_SHADER);
	m_PositionConstantBuffer = Renderer::CreateConstantBuffer<PointLightConstantBuffer>(Shader::PIXEL_SHADER, 2);
}
