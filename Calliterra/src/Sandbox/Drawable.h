#pragma once
#include "Renderer/Renderer.h"

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	Drawable(DX::XMMATRIX transform)
		: m_Transform(transform)
	{
	}

	virtual void Draw() = 0;

	virtual void Update(DX::XMMATRIX transform) = 0;
	virtual void Update(float dt) = 0;

	void Bind()
	{

	}

protected:
	DX::XMMATRIX m_Transform;
};
