#pragma once
#include "Renderer/Drawable.h"
#include "Node.h"
#include "Renderer/Renderer.h"
#include "ModelLoader.h"

struct ModelVertex;
class UfbxScene;

class Mesh : public Drawable
{
public:
	Mesh(int meshIndex, const rapidobj::Result& ObjModel, const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});
	Mesh(int meshIndex, const fastgltf::Asset& gltfModel, const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});
	Mesh(int meshIndex, const UfbxScene& fbxModel, const DX::XMMATRIX& transform = DX::XMMatrixIdentity(), DX::XMFLOAT3 color = {-1.f, -1.f, -1.f});

	void Draw() override;

	void Update(float dt) override;

private:
	void InitBuffers();

	struct TransformConstantBuffer
	{
		DX::XMMATRIX Model;
		DX::XMMATRIX ModelView;
		DX::XMMATRIX ModelViewProj;
		DX::XMMATRIX NormalMatrix;
	};

	struct PixelConstantBuffer
	{
		alignas(16) DX::XMFLOAT3 Color;
		float SpecularIntensity;
		float SpecularPower;
		float padding[2];
	};

private:
	int m_MeshIndex;

	const rapidobj::Result* m_ObjModel;
	const fastgltf::Asset* m_GltfModel;
	const UfbxScene* m_FbxModel;

	std::vector<ModelVertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_PixelConstantBuffer = nullptr;
};

