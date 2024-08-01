#pragma once
#include "Renderer/Renderer.h"

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	Drawable(DX::XMMATRIX transform, DX::XMFLOAT3 color)
		: m_Transform(transform), m_FlatColor(color)
	{
	}

	virtual void Draw() = 0;

	virtual void Update(DX::XMMATRIX transform) = 0;
	virtual void Update(float dt) = 0;

	virtual void SetViewProjectionMatrix(DX::XMMATRIX transform) { m_ViewProjectionMatrix = transform; }

	DX::XMMATRIX GetTransform() { return m_Transform; }

	void Bind()
	{

	}

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
	DX::XMMATRIX m_ViewProjectionMatrix;
	DX::XMFLOAT3 m_FlatColor;
};
