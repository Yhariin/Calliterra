#pragma once
#include "Sandbox/Drawable.h"
#include "Asset/ModelLoader.h"

class Model : public Drawable
{
public:
	Model(std::shared_ptr<rapidobj::Result> model, DX::XMMATRIX transform, DX::XMFLOAT3 color);
	Model(std::shared_ptr<fastgltf::Asset> model, DX::XMMATRIX transform, DX::XMFLOAT3 color);
	Model(std::shared_ptr<UfbxScene> model, DX::XMMATRIX transform, DX::XMFLOAT3 color);

	void Draw() override;

	void Update(DX::XMMATRIX transform) override;
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
	std::shared_ptr<rapidobj::Result> m_ObjModel;
	std::shared_ptr<fastgltf::Asset> m_GltfModel;
	std::shared_ptr<UfbxScene> m_FbxModel;

	std::vector<ModelVertex> m_Vertices;
	std::vector<uint32_t> m_Indices;

	std::shared_ptr<Shader> m_VertexShader = nullptr;
	std::shared_ptr<Shader> m_PixelShader = nullptr;
	std::shared_ptr<VertexBuffer> m_VertexBuffer = nullptr;
	std::shared_ptr<IndexBuffer> m_IndexBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_TransformConstantBuffer = nullptr;
	std::shared_ptr<ConstantBuffer> m_PixelConstantBuffer = nullptr;
};

