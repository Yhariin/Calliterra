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

	void Draw() override;
	void Submit() const override {}

	void Update(float dt) override;

private:
	void InitBuffers();

	struct TransformConstantBuffer
	{
		DX::XMMATRIX ModelView;
		DX::XMMATRIX ModelViewProj;
		DX::XMMATRIX NormalMatrix;
	};

	struct PixelConstantBuffer
	{
		float SpecularIntensity;
		float SpecularPower;
		float padding[2];
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

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::vector<std::shared_ptr<Texture>> m_Textures;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_PixelConstantBuffer = nullptr;
	std::shared_ptr<Blender> m_Blender = nullptr;
	std::shared_ptr<DepthStencil> m_DepthStencil = nullptr;
};

