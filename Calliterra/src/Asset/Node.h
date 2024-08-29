#pragma once

class Mesh;

class Node
{
public:
	Node(int id, const std::string& name,  const DX::XMMATRIX& relativeTransform, std::vector<Mesh*> meshes);
	Node();

	void Submit() const;

	void ApplyTransformations(const DX::XMMATRIX& transform = DX::XMMatrixIdentity());
	void SetModelTransform(const DX::XMMATRIX& transform);
	void SetAppliedTransformation(const DX::XMMATRIX& transform);

	void AddChild(std::unique_ptr<Node> child); 

	void SetProjectionMatrix(const DX::XMMATRIX& projMatrix);
	void SetViewMatrix(const DX::XMMATRIX& viewMatrix);

	const DX::XMMATRIX& GetRelativeTransform() const { return m_RelativeTransform; }
	const std::vector<Mesh*>& GetMeshes() const { return m_Meshes; }
	int GetNumChildren() const { return static_cast<int>(m_Children.size()); }
	const std::vector<std::unique_ptr<Node>>& GetChildren() const { return m_Children; }
	const std::string& GetName() const { return m_Name; }
	int GetId() const { return m_Id; }

private:
	int m_Id;
	std::string m_Name;
	std::vector<std::unique_ptr<Node>> m_Children;
	std::vector<Mesh*> m_Meshes;

	DX::XMMATRIX m_RelativeTransform = DX::XMMatrixIdentity();
	DX::XMMATRIX m_ModelTransform = DX::XMMatrixIdentity();
	DX::XMMATRIX m_AppliedTransform = DX::XMMatrixIdentity();
};

