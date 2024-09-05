#pragma once
#include "Renderer/Drawable.h"
#include "Node.h"
#include "Mesh.h"

class Model : public Drawable
{
public:
	Model(std::unique_ptr<Node> root, std::unordered_map<int, std::unique_ptr<Mesh>> meshes, const DX::XMMATRIX& transform, DX::XMFLOAT3 color);

	void Submit() const override;

	void Update(float dt) override;

	void SetTransform(const DX::XMMATRIX& transform) override;
	void SetViewMatrix(const DX::XMMATRIX& transform) override;
	void SetProjectionMatrix(const DX::XMMATRIX& transform) override;

private:
	std::unique_ptr<Node> m_Root;
	std::unordered_map<int, std::unique_ptr<Mesh>> m_Meshes;
};

