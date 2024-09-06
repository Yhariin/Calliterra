#pragma once
#include "Renderer/Drawable.h"

class DirectionalLight : public Drawable
{
public:
	DirectionalLight(const DX::XMFLOAT3& direction, const DX::XMFLOAT3& color, const DX::XMFLOAT3& ambient, float intensity);

	void Update(float dt) override;
	void Submit() const override;

protected:
	struct DirectionalLightProperties
	{
		alignas(16) DX::XMFLOAT3 Direction;
		alignas(16) DX::XMFLOAT3 Color;
		alignas(16) DX::XMFLOAT3 Ambient;
		float Intensity;

		float padding[3];
	};

	float m_Angle = 0.f;
	std::shared_ptr<ConstantBuffer> m_LightConstantBuffer;
	DirectionalLightProperties m_LightProperties;
};
