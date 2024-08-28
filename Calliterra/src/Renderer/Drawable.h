#pragma once
#include "Renderer.h"
#include "RenderQueue/Technique.h"

class Drawable
{
public:
	Drawable() = default;
	virtual ~Drawable() = default;

	Drawable(const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
		: m_Transform(transform), m_Color(color)
	{
	}

	virtual void Submit() const { SubmitTechniques(); }
	virtual void Draw() = 0;
	virtual void DrawOutline() {} // Temporary, used to test stencil buffer, remove later.

	virtual void Update(float dt) = 0;

	virtual void SetTransform(const DX::XMMATRIX& transform) { m_Transform = transform; }
	virtual void SetViewMatrix(const DX::XMMATRIX& transform) { m_ViewMatrix = transform; }
	virtual void SetProjectionMatrix(const DX::XMMATRIX& transform) { m_ProjectionMatrix = transform; }

	void AddTechnique(Technique technique)
	{
		technique.InitializeParentReferences(*this);
		m_Techniques.push_back(std::move(technique));
	}

	void SubmitTechniques() const
	{
		for (const auto& technique : m_Techniques)
		{
			technique.Submit();
		}
	}

	const DX::XMMATRIX& GetTransform() const { return m_Transform; }
	const DX::XMMATRIX& GetViewTransform() const { return m_ViewMatrix; }
	const DX::XMMATRIX& GetProjectionTransform() const { return m_ProjectionMatrix; }

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

	inline static const FaceColorsBuffer s_ColorsBuffer =
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

	std::vector<Bindable> m_Bindables;
	std::shared_ptr<TransformConstantBuffer> m_TransformConstantBuffer;

private:
	std::vector<Technique> m_Techniques;
};
