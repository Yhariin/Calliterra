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

void Mesh::Update(float dt)
{
}

void Mesh::InitBuffers()
{
	Technique drawTech;
	Step onlyStep(0);

	using namespace std::string_literals;
	auto meshTag = m_Filepath + "%" + std::to_string(m_MeshIndex);

	bool hasNormalMap = m_Material->HasMaterialMap(Material::Normal);
	bool hasSpecMap = m_Material->HasMaterialMap(Material::Specular);

	auto vShader = Shader::Resolve("assets/shaders/BPhongMapVS.hlsl", Shader::VERTEX_SHADER);
	onlyStep.AddBindable(vShader);
	onlyStep.AddBindable(Shader::Resolve("assets/shaders/BPhongMapPS.hlsl", Shader::PIXEL_SHADER));

	m_VerticesFull = ModelLoader::GetMeshVertexVectorFull(*m_AssimpModel, m_MeshIndex);
	auto vBuff = VertexBuffer::Resolve(meshTag, m_VerticesFull);

	vBuff->CreateLayout({
		{"POSITION", 0, ShaderDataType::Float3},
		{"NORMAL", 0, ShaderDataType::Float3},
		{"TANGENT", 0, ShaderDataType::Float3},
		{"BITANGENT", 0, ShaderDataType::Float3},
		{"TEXCOORD", 0, ShaderDataType::Float2},
		}, vShader.get());
	onlyStep.AddBindable(vBuff);

	m_Indices = ModelLoader::GetMeshIndexVector(*m_AssimpModel, m_MeshIndex);

	onlyStep.AddBindable(IndexBuffer::Resolve(meshTag, m_Indices));

	bool blending = false;
	for (int i = 0; i < m_Material->NumSupportedMaps; i++)
	{
		if (m_Material->HasMaterialMap(static_cast<Material::MapTypes>(i)))
		{
			if (i == 0)
			{
				auto tex = Texture::Resolve(m_Material->GetMaterialMaps()[i], i);
				if (tex->HasBlending())
				{
					blending = true;
				}
				onlyStep.AddBindable(tex);
			}
			else
			{
				onlyStep.AddBindable(Texture::Resolve(m_Material->GetMaterialMaps()[i], i));
			}
		}
		else
		{
			onlyStep.AddBindable(Texture::Resolve(m_Material->GetMaterialMaps()[i], i, Texture::Filter::Point));
		}
	}

	onlyStep.AddBindable(std::make_shared<TransformConstantBuffer>());

	PixelConstantBuffer pcb = {
		m_Material->GetShininess(),
		hasNormalMap,
		hasSpecMap
	};

	onlyStep.AddBindable(ConstantBuffer::Resolve<PixelConstantBuffer>(Shader::PIXEL_SHADER, pcb, 1, meshTag));

	if (blending)
	{
		onlyStep.AddBindable(Blender::Resolve(true, Blender::BlendFunc::BLEND_SRC_ALPHA, Blender::BlendFunc::BLEND_INV_SRC_ALPHA, Blender::BlendOp::ADD));
	}
	else
	{
		onlyStep.AddBindable(Blender::Resolve(false, Blender::BlendFunc::NONE, Blender::BlendFunc::NONE, Blender::BlendOp::NONE));
	}

	drawTech.AddStep(std::move(onlyStep));
	AddTechnique(std::move(drawTech));

}
