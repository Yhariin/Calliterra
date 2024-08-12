#pragma once
#include "Renderer.h"

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	Drawable(const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
		: m_Transform(transform), m_Color(color)
	{
	}

	virtual void Draw() = 0;

	virtual void Update(float dt) = 0;

	virtual void SetTransform(const DX::XMMATRIX& transform) { m_Transform = transform; }
	virtual void SetViewMatrix(const DX::XMMATRIX& transform) { m_ViewMatrix = transform; }
	virtual void SetProjectionMatrix(const DX::XMMATRIX& transform) { m_ProjectionMatrix = transform; }

	DX::XMMATRIX GetTransform() { return m_Transform; }

protected:
	struct FaceColorsBuffer
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} faceColors[8];
	};

	inline static const FaceColorsBuffer m_ColorsBuffer =
	{
		{
			{ 1.f, 1.f, 1.f },
			{ 1.f, 0.f, 0.f },
			{ 0.f, 1.f, 0.f },
			{ 1.f, 1.f, 0.f },
			{ 0.f, 0.f, 1.f },
			{ 1.f, 0.f, 1.f },
			{ 0.f, 1.f, 1.f },
			{ 0.f, 0.f, 0.f }
		}
	};

	DX::XMMATRIX m_Transform;
	DX::XMMATRIX m_ViewMatrix = DX::XMMatrixIdentity();
	DX::XMMATRIX m_ProjectionMatrix = DX::XMMatrixIdentity();
	DX::XMFLOAT3 m_Color;
};
