#include "pch.h"
#include "Model.h"

Model::Model(std::unique_ptr<Node> root, std::unordered_map<int, std::unique_ptr<Mesh>> meshes, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: m_Root(std::move(root)), m_Meshes(std::move(meshes)), Drawable(transform, color)
{
	m_Root->ApplyTransformations(m_Transform);
}

void Model::Submit() const
{
	m_Root->Submit();
}

void Model::LinkTechniques() 
{
	m_Root->LinkTechniques();
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
