#include "pch.h"
#include "PointLight.h"

PointLight::PointLight(DX::XMMATRIX transform, DX::XMFLOAT3 color)
	: m_Sphere(4, transform, color), Drawable(transform, color)
{
	DX::XMStoreFloat4(&m_Position, DX::XMVector4Transform(DX::XMLoadFloat4(&m_Position), transform));
	InitBuffers();

	GlobalSettings::Register(SettingsType::PointLightPosition, this);
}

void PointLight::Draw()
{
	m_Sphere.Draw();
	m_PixelShader->Bind();
	DX::XMFLOAT4 posViewSpace;
	DX::XMStoreFloat4(&posViewSpace, DX::XMVector3Transform(DX::XMLoadFloat4(&m_Position), m_ViewMatrix));
	Renderer::UpdateConstantBuffer(m_PositionConstantBuffer, posViewSpace);
	m_PositionConstantBuffer->Bind({});
}

void PointLight::OnSettingsUpdate(SettingsType type)
{
	m_Position = DX::XMFLOAT4(GlobalSettings::Rendering::LightPos().x, GlobalSettings::Rendering::LightPos().y, GlobalSettings::Rendering::LightPos().z, 1.0f);
}

void PointLight::Update(DX::XMMATRIX transform)
{
	m_Transform = transform;


	m_Transform = DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_Sphere.Update(m_Transform);
}

void PointLight::Update(float dt)
{

	m_Transform = DX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_Sphere.Update(m_Transform);
}

void PointLight::InitBuffers()
{
	m_PixelShader = Renderer::CreateShader("assets/shaders/PhongPS.hlsl", Shader::PIXEL_SHADER);
	m_PositionConstantBuffer = Renderer::CreateConstantBuffer<DX::XMFLOAT4>(Shader::PIXEL_SHADER, 2);
}
