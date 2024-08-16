#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(int meshIndex, const rapidobj::Result& ObjModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_ObjModel(&ObjModel), m_Material(std::move(material))
{
	m_Vertices = ModelLoader::GetMeshVertexVector(*m_ObjModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_ObjModel, m_MeshIndex);

	InitBuffers();
}

Mesh::Mesh(int meshIndex, const fastgltf::Asset& gltfModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_GltfModel(&gltfModel), m_Material(std::move(material))
{
	m_Vertices = ModelLoader::GetMeshVertexVector(*m_GltfModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_GltfModel, m_MeshIndex);

	InitBuffers();
}

Mesh::Mesh(int meshIndex, const UfbxScene& fbxModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_FbxModel(&fbxModel), m_Material(std::move(material))
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
	if (m_Material->HasMaterialMap(Material::Specular))
	{
		Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, m_Textures, { m_TransformConstantBuffer });
	}
	else 
	{
		Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, m_Textures, { m_TransformConstantBuffer, m_PixelConstantBuffer });
	}
	Renderer::Draw();
}

void Mesh::Update(float dt)
{

}

void Mesh::InitBuffers()
{
	m_VertexShader = Renderer::CreateShader("assets/shaders/PhongTexVS.hlsl", Shader::VERTEX_SHADER);
	if (m_Material->HasMaterialMap(Material::Specular))
	{
		m_PixelShader = Renderer::CreateShader("assets/shaders/PhongTexSpecMapPS.hlsl", Shader::PIXEL_SHADER);
	}
	else
	{
		m_PixelShader = Renderer::CreateShader("assets/shaders/PhongTexPS.hlsl", Shader::PIXEL_SHADER);
	}
	m_VertexBuffer = Renderer::CreateVertexBuffer(m_Vertices, static_cast<uint32_t>(m_Vertices.size()), m_VertexShader.get());
	m_IndexBuffer = Renderer::CreateIndexBuffer(&m_Indices[0], static_cast<uint32_t>(m_Indices.size()));

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	m_VertexBuffer->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		{"TEXCOORD", 0, ShaderDataType::Float2},
		});
	m_VertexBuffer->SetLayout();

	for (int i = 0; i < m_Material->NumSupportedMaps; i++)
	{
		if (m_Material->HasMaterialMap(static_cast<Material::MapTypes>(i)))
		{
			m_Textures.emplace_back(Renderer::CreateTexture(m_Material->GetMaterialMaps()[i], i));
		}
	}

	m_TransformConstantBuffer = Renderer::CreateConstantBuffer<TransformConstantBuffer>(Shader::VERTEX_SHADER);

	PixelConstantBuffer pcb = {
		1.f,
		m_Material->GetShininess()
	};

	if (!m_Material->HasMaterialMap(Material::Specular))
	{
		m_PixelConstantBuffer = Renderer::CreateConstantBuffer<PixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);
	}
}
