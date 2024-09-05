#pragma once
#include "Renderer/Drawable.h"
#include "Node.h"
#include "Renderer/Renderer.h"
#include "ModelLoader.h"
#include "Material.h"

struct ModelVertexFull;
struct ModelVertexSemi;
class UfbxScene;

class Mesh : public Drawable
{
public:
	Mesh(int meshIndex, 
		 const aiScene& ObjModel, 
		 const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), 
		 DX::XMFLOAT3 color = {-1.f, -1.f, -1.f}, 
		 std::unique_ptr<Material> material = nullptr, 
		 const std::string& filepath = "");

	Mesh(int meshIndex, 
		 const rapidobj::Result& ObjModel, 
		 const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), 
		 DX::XMFLOAT3 color = {-1.f, -1.f, -1.f}, 
		 std::unique_ptr<Material> material = nullptr, 
		 const std::string& filepath = "");

	Mesh(int meshIndex, 
		 const fastgltf::Asset& gltfModel, 
		 const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), 
		 DX::XMFLOAT3 color = {-1.f, -1.f, -1.f}, 
		 std::unique_ptr<Material> material = nullptr,
		 const std::string& filepath = "");

	Mesh(int meshIndex, 
		 const UfbxScene& fbxModel, 
		 const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), 
		 DX::XMFLOAT3 color = {-1.f, -1.f, -1.f}, 
		 std::unique_ptr<Material> material = nullptr,
		 const std::string& filepath = "");

	void Submit() const override { Drawable::Submit(); }

	void Update(float dt) override;

private:
	void InitBuffers();

	struct PixelConstantBuffer
	{
		float SpecularPower;
		BOOL hasNormalMap;
		BOOL hasSpecMap;
		float padding[1];
	};

private:
	int m_MeshIndex;
	std::string m_Filepath;

	const aiScene* m_AssimpModel;
	const rapidobj::Result* m_ObjModel;
	const fastgltf::Asset* m_GltfModel;
	const UfbxScene* m_FbxModel;

	std::vector<ModelVertexFull> m_VerticesFull;
	std::vector<ModelVertexSemi> m_VerticesSemi;
	std::vector<uint32_t> m_Indices;
	std::unique_ptr<Material> m_Material;
};

