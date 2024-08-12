#include "pch.h"
#include "Node.h"
#include "Mesh.h"

Node::Node(int id, const std::string& name,  const DX::XMMATRIX& relativeTransform, std::vector<Mesh*> meshes)
	: m_Id(id), m_Name(name), m_RelativeTransform(relativeTransform), m_Meshes(std::move(meshes))
{
}

// Constructor for an empty node with no corresponding mesh
Node::Node()
	: m_Id(-1), m_Name(""), m_RelativeTransform(DX::XMMatrixIdentity())
{
}

void Node::Draw() 
{
	for (const auto& mesh : m_Meshes)
	{
		if (!m_Meshes.empty())
		{
			mesh->Draw();
		}
	}

	for (const auto& child : m_Children)
	{
		child->Draw();
	}
}

void Node::ApplyTransformations(const DX::XMMATRIX& transform)
{
	const DX::XMMATRIX newTransform = m_RelativeTransform * m_ModelTransform * transform;
	const DX::XMMATRIX accumulatedTransform = m_AppliedTransform * newTransform;
	
	for (const auto& mesh : m_Meshes)
	{
		if (!m_Meshes.empty())
		{
			mesh->SetTransform(accumulatedTransform);
		}
	}

	for (const auto& child : m_Children)
	{
		child->ApplyTransformations(accumulatedTransform);
	}
}

void Node::SetAppliedTransformation(const DX::XMMATRIX& transform)
{
	// We only want to set the applied transform for the current node, not it's children
	// or else the transform will be applied repeatedly for all it's children
	// (e.g. 90 degree rotation on this node would result in two 90 degree turns for children)
	m_AppliedTransform = transform;
}

void Node::AddChild(std::unique_ptr<Node> child)
{
	ASSERT(child);
	m_Children.push_back(std::move(child));
}

void Node::SetModelTransform(const DX::XMMATRIX& transform)
{
	m_ModelTransform = transform;
}

void Node::SetProjectionMatrix(const DX::XMMATRIX& projMatrix)
{
	for (const auto& mesh : m_Meshes)
	{
		if (!m_Meshes.empty())
		{
			mesh->SetProjectionMatrix(projMatrix);
		}
	}

	for (const auto& child : m_Children)
	{
		child->SetProjectionMatrix(projMatrix);
	}
}

void Node::SetViewMatrix(const DX::XMMATRIX& viewMatrix)
{
	for (const auto& mesh : m_Meshes)
	{
		if (!m_Meshes.empty())
		{
			mesh->SetViewMatrix(viewMatrix);
		}
	}

	for (const auto& child : m_Children)
	{
		child->SetViewMatrix(viewMatrix);
	}

}
