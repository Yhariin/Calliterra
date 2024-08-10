#pragma once
#include "Renderer/Drawable.h"
#include "ModelLoader.h"
#include "Node.h"
#include "Mesh.h"

class Model : public Drawable
{
public:
	Model(std::shared_ptr<rapidobj::Result> model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color);
	Model(std::shared_ptr<fastgltf::Asset> model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color);
	Model(std::shared_ptr<UfbxScene> model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color);

	void Draw() override;

	void Update(float dt) override;

	void SetTransform(const DX::XMMATRIX& transform) override;
	void SetViewMatrix(const DX::XMMATRIX& transform) override;
	void SetProjectionMatrix(const DX::XMMATRIX& transform) override;

private:
	
private:
	std::shared_ptr<rapidobj::Result> m_ObjModel;
	std::shared_ptr<fastgltf::Asset> m_GltfModel;
	std::shared_ptr<UfbxScene> m_FbxModel;

	std::unique_ptr<Node> m_Root;
	std::vector<std::unique_ptr<Mesh>> m_Meshes;
};

