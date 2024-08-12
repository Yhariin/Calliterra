#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(int meshIndex, const rapidobj::Result& ObjModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_ObjModel(&ObjModel)
{
	m_Vertices = ModelLoader::GetMeshVertexVector(*m_ObjModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_ObjModel, m_MeshIndex);

	InitBuffers();
}

Mesh::Mesh(int meshIndex, const fastgltf::Asset& gltfModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_GltfModel(&gltfModel)
{
	m_Vertices = ModelLoader::GetMeshVertexVector(*m_GltfModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_GltfModel, m_MeshIndex);

	InitBuffers();
}

Mesh::Mesh(int meshIndex, const UfbxScene& fbxModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_FbxModel(&fbxModel)
{
	m_Vertices = ModelLoader::GetMeshVertexVector(*m_FbxModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_FbxModel, m_Vertices, m_MeshIndex);

	InitBuffers();
}

void Mesh::Draw()
{
	TransformConstantBuffer cb = { 
		DX::XMMatrixTranspose(m_Transform), 
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix),
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix * m_ProjectionMatrix),
		DX::XMMatrixInverse(nullptr, m_Transform * m_ViewMatrix)
	};

	Renderer::UpdateConstantBuffer(m_TransformConstantBuffer, cb);
	Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, { m_TransformConstantBuffer, m_PixelConstantBuffer });
	Renderer::Draw();
}

void Mesh::Update(float dt)
{

}

void Mesh::InitBuffers()
{
	m_VertexShader = Renderer::CreateShader("assets/shaders/PhongVS.hlsl", Shader::VERTEX_SHADER);
	m_PixelShader = Renderer::CreateShader("assets/shaders/PhongPS.hlsl", Shader::PIXEL_SHADER);
	m_VertexBuffer = Renderer::CreateVertexBuffer(m_Vertices, static_cast<uint32_t>(m_Vertices.size()), m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(&m_Indices[0], static_cast<uint32_t>(m_Indices.size()));

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		});
	m_VertexBuffer->SetLayout();

	m_TransformConstantBuffer = Renderer::CreateConstantBuffer<TransformConstantBuffer>(Shader::VERTEX_SHADER);

	PixelConstantBuffer pcb = {
		m_Color,
		.3f,
		32.f
	};
	m_PixelConstantBuffer = Renderer::CreateConstantBuffer<PixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);
}
