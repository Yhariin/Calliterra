#include "pch.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const DX::XMFLOAT3& direction, const DX::XMFLOAT3& color, const DX::XMFLOAT3& ambient, float intensity)
{
	const auto lightTag = "$DirectionalLight";

	Technique draw;
	Step only(PassName::Lambertian);
	
	m_LightProperties = { direction, color, ambient, intensity };

	m_LightConstantBuffer = ConstantBuffer::Resolve(Shader::PIXEL_SHADER, m_LightProperties, 3, lightTag);

	only.AddBindable(m_LightConstantBuffer);

	draw.AddStep(std::move(only));
	AddTechnique(std::move(draw));
}

void DirectionalLight::Update(float dt)
{

}

void DirectionalLight::Submit() const
{
	m_LightConstantBuffer->Bind();
}
