#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(int meshIndex, const aiScene& ObjModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material, const std::string& filepath)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_AssimpModel(&ObjModel), m_Material(std::move(material)), m_Filepath(filepath)
{
	InitBuffers();
}

Mesh::Mesh(int meshIndex, const rapidobj::Result& ObjModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material, const std::string& filepath)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_ObjModel(&ObjModel), m_Material(std::move(material)), m_Filepath(filepath)
{
	m_VerticesSemi = ModelLoader::GetMeshVertexVector(*m_ObjModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_ObjModel, m_MeshIndex);

	InitBuffers();
}

Mesh::Mesh(int meshIndex, const fastgltf::Asset& gltfModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material, const std::string& filepath)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_GltfModel(&gltfModel), m_Material(std::move(material)), m_Filepath(filepath)
{
	m_VerticesSemi = ModelLoader::GetMeshVertexVector(*m_GltfModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_GltfModel, m_MeshIndex);

	InitBuffers();
}

Mesh::Mesh(int meshIndex, const UfbxScene& fbxModel, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, std::unique_ptr<Material> material, const std::string& filepath)
	: Drawable(transform, color), m_MeshIndex(meshIndex), m_FbxModel(&fbxModel), m_Material(std::move(material)), m_Filepath(filepath)
{
	m_VerticesSemi = ModelLoader::GetMeshVertexVector(*m_FbxModel, m_MeshIndex);
	m_Indices = ModelLoader::GetMeshIndexVector(*m_FbxModel, m_VerticesSemi, m_MeshIndex);

	InitBuffers();
}

void Mesh::Draw()
{
	TransformConstantBuffer cb = {
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix),
		DX::XMMatrixTranspose(m_Transform * m_ViewMatrix * m_ProjectionMatrix),
		DX::XMMatrixInverse(nullptr, m_Transform * m_ViewMatrix)
	};

	Renderer::UpdateConstantBuffer(m_TransformConstantBuffer, cb);
	if (m_Material->HasMaterialMap(Material::Specular))
	{
		Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, m_Textures, { m_TransformConstantBuffer }, m_Blender);
	}
	else 
	{
		Renderer::Bind({ m_VertexShader, m_PixelShader }, m_VertexBuffer, m_IndexBuffer, m_Textures, { m_TransformConstantBuffer, m_PixelConstantBuffer }, m_Blender);
	}
	Renderer::Draw();
}

void Mesh::Update(float dt)
{

}

void Mesh::InitBuffers()
{
	using namespace std::string_literals;

	auto meshTag = m_Filepath + "%" + std::to_string(m_MeshIndex);

	bool hasSpecular = m_Material->HasMaterialMap(Material::Specular);
	bool hasNormal = m_Material->HasMaterialMap(Material::Normal);


	std::string vertexShaderPath;
	std::string pixelShaderPath;


	if (hasSpecular && hasNormal)
	{
		vertexShaderPath = "assets/shaders/NormalMapVS.hlsl";
		pixelShaderPath = "assets/shaders/BPhongNormMapSpecMap.hlsl";
	}
	else if (hasSpecular)
	{
		vertexShaderPath = "assets/shaders/BPhongTexVS.hlsl";
		pixelShaderPath = "assets/shaders/BPhongSpecMapPS.hlsl";
	}
	else if (hasNormal)
	{
		vertexShaderPath = "assets/shaders/NormalMapVS.hlsl";
		pixelShaderPath = "assets/shaders/BPhongNormalMapPS.hlsl";
	}
	else
	{
		vertexShaderPath = "assets/shaders/BPhongTexVS.hlsl";
		pixelShaderPath = "assets/shaders/BPhongTexPS.hlsl";
	}
	m_VertexShader = Shader::Resolve(vertexShaderPath, Shader::VERTEX_SHADER);
	m_PixelShader = Shader::Resolve(pixelShaderPath, Shader::PIXEL_SHADER);

	if (hasNormal)
	{
		m_VerticesFull = ModelLoader::GetMeshVertexVectorFull(*m_AssimpModel, m_MeshIndex);
		m_VertexBuffer = VertexBuffer::Resolve(meshTag, m_VerticesFull, m_VertexShader.get());
		m_VertexBuffer->CreateLayout({
			{"POSITION", 0, ShaderDataType::Float3},
			{"NORMAL", 0, ShaderDataType::Float3},
			{"TANGENT", 0, ShaderDataType::Float3},
			{"BITANGENT", 0, ShaderDataType::Float3},
			{"TEXCOORD", 0, ShaderDataType::Float2},
			});
	}
	else
	{
		m_VerticesSemi = ModelLoader::GetMeshVertexVectorSemi(*m_AssimpModel, m_MeshIndex);
		m_VertexBuffer = VertexBuffer::Resolve(meshTag, m_VerticesSemi, m_VertexShader.get());
		m_VertexBuffer->CreateLayout({
			{"POSITION", 0, ShaderDataType::Float3},
			{"NORMAL", 0, ShaderDataType::Float3},
			{"TEXCOORD", 0, ShaderDataType::Float2},
			});
	}
	m_VertexBuffer->SetLayout();
	m_Indices = ModelLoader::GetMeshIndexVector(*m_AssimpModel, m_MeshIndex);

	m_IndexBuffer = IndexBuffer::Resolve(meshTag, m_Indices);

	m_VertexShader->Bind();
	m_PixelShader->Bind();

	bool blending = false;
	for (int i = 0; i < m_Material->NumSupportedMaps; i++)
	{
		if (m_Material->HasMaterialMap(static_cast<Material::MapTypes>(i)))
		{
			m_Textures.emplace_back(Texture::Resolve(m_Material->GetMaterialMaps()[i], i));
			if (m_Textures.back()->HasBlending())
			{
				blending = true;
			}
		}
	}

	m_TransformConstantBuffer = ConstantBuffer::Resolve<TransformConstantBuffer>(Shader::VERTEX_SHADER, {});

	if (!hasSpecular)
	{
		PixelConstantBuffer pcb = {
			0.3f,
			m_Material->GetShininess()
		};

		m_PixelConstantBuffer = ConstantBuffer::Resolve<PixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1);
	}

	if (blending)
	{
		m_Blender = Blender::Resolve(true, Blender::BlendFunc::BLEND_SRC_ALPHA, Blender::BlendFunc::BLEND_INV_SRC_ALPHA, Blender::BlendOp::ADD);
	}
	else
	{
		m_Blender = Blender::Resolve(false, Blender::BlendFunc::NONE, Blender::BlendFunc::NONE, Blender::BlendOp::NONE);
	}
}
