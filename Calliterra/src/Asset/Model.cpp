#include "pch.h"
#include "Model.h"

Model::Model(std::shared_ptr<rapidobj::Result> model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	m_Meshes = std::move(ModelLoader::GetModelMeshes(*model, transform, color));

	m_Root = ModelLoader::ParseNode(*model, m_Meshes);

	m_Root->SetModelTransform(m_Transform);
	m_Root->ApplyTransformations();
	
}

Model::Model(std::shared_ptr<fastgltf::Asset> model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	m_Meshes = std::move(ModelLoader::GetModelMeshes(*model, transform, color));

	fastgltf::Node root = ModelLoader::GetRootNode(*model);
	m_Root = ModelLoader::ParseNode(*model, root, m_Meshes);

	m_Root->SetModelTransform(m_Transform);
	m_Root->ApplyTransformations();
}

Model::Model(std::shared_ptr<UfbxScene> model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: Drawable(transform, color)
{
	m_Meshes = std::move(ModelLoader::GetModelMeshes(*model, transform, color));

	m_Root = ModelLoader::ParseNode(*model, *model->Root_node(), m_Meshes);

	m_Root->SetModelTransform(m_Transform);
	m_Root->ApplyTransformations();

}

void Model::Draw()
{
	m_Root->Draw();
}

void Model::Update(float dt)
{
}

void Model::SetTransform(const DX::XMMATRIX& transform)
{
	m_Transform = transform;
	m_Root->SetModelTransform(m_Transform);
}

void Model::SetProjectionMatrix(const DX::XMMATRIX& transform)
{
	m_ProjectionMatrix = transform;

	m_Root->SetProjectionMatrix(transform);
}

void Model::SetViewMatrix(const DX::XMMATRIX& transform)
{
	m_ViewMatrix = transform;

	m_Root->SetViewMatrix(transform);
}
