#include "pch.h"
#include "ModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "fastgltf/tools.hpp"
#include "fastgltf/math.hpp"

#include "Material.h"
#include "ModelAPI.h"
#include "Model.h"


void ModelLoader::Init()
{
	s_AssimpImporter = std::make_unique<Assimp::Importer>();
}

void ModelLoader::Shutdown()
{
	s_AssimpImporter.release();
}
// Loads the modelAPI from a filepath and stores it into an unordered_map,
// if it already exists inside the map then just return it.
ModelAPI ModelLoader::LoadModel(const std::filesystem::path& filepath)
{
	std::string extension = filepath.extension().string();

	std::transform(extension.begin(), extension.end(), extension.begin(),
    [](unsigned char c){ return std::tolower(c); });

	std::string key = filepath.string();
	const auto i = s_Models.find(key);

	if (i == s_Models.end())
	{
		ModelAPI model;
#if FAST_MODEL_LOADING
		if (extension == ".obj")
		{
			model = ModelAPI(LoadModelObj(filepath));
		}
		else if (extension == ".gltf" || extension == ".glb")
		{
			model = ModelAPI(LoadModelGltf(filepath));
		}
		else if (extension == ".fbx")
		{
			model = ModelAPI(LoadModelFbx(filepath));
		}
		else
		{
			ASSERT(false, "Unsupported model type!");
		}
#else
		model = ModelAPI(LoadModelAssimp(filepath));
#endif

		s_Models[key] = model;
		return model;
	}
	else
	{
		return s_Models[key];
	}

}

// Returns a model that is ready to be used given the parameters. Internally calls LoadModel() so
// this function is all you need if you want a model.
Model ModelLoader::GetModel(const std::filesystem::path& filepath, const DX::XMMATRIX& transform, DX::XMFLOAT3 color)
{
	ModelAPI modelAPI = LoadModel(filepath);

	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;
	std::unique_ptr<Node> root;

#if FAST_MODEL_LOADING
	switch (modelAPI.GetAPI())
	{
	case ModelAPI::Obj:
	{
		meshes = GetModelMeshes(*modelAPI.GetObj(), transform, color, filepath.string());
		root = std::move(ParseNode(*modelAPI.GetObj(), meshes));
		break;
	}
	case ModelAPI::Gltf:
	{
		meshes = GetModelMeshes(*modelAPI.GetGltf(), transform, color, filepath.string());
		fastgltf::Node gltfRoot = ModelLoader::GetRootNode(*modelAPI.GetGltf());
		root = std::move(ParseNode(*modelAPI.GetGltf(), gltfRoot, meshes));
		break;
	}
	case ModelAPI::Fbx:
	{
		meshes = GetModelMeshes(*modelAPI.GetFbx(), transform, color, filepath.string());
		root = std::move(ParseNode(*modelAPI.GetFbx(), *modelAPI.GetFbx()->Root_node(), meshes));
		break;
	}
	}
#else
	meshes = GetModelMeshes(*modelAPI.GetAssimp(), transform, color, filepath.string());
	aiNode* aiRoot = modelAPI.GetAssimp()->mRootNode;
	int nextId = 0;
	root = std::move(ParseNode(*modelAPI.GetAssimp(), nextId, *aiRoot, meshes));
#endif

	return Model(std::move(root), std::move(meshes), transform, color);

}
//==============================Assimp==================================
#pragma region assimp 

std::shared_ptr<aiScene> ModelLoader::LoadModelAssimp(const std::filesystem::path& filepath)
{
	ASSERT(s_AssimpImporter, "ModelLoader must be initialized before trying to load model!");

	const auto flags = 
		aiProcess_Triangulate | // Make sure everything is triangles
		aiProcess_JoinIdenticalVertices | // Each mesh will contain unique vertices
		aiProcess_ConvertToLeftHanded | // Convert to Direct3D friendly data
		aiProcess_GenSmoothNormals | // Generate normals if we don't already have them
		aiProcess_GenUVCoords | // Generate UVs if we don't already have them
		aiProcess_CalcTangentSpace; // Calculate Tangents and Bitangents
	
	//Assimp::Importer importer;
	const aiScene* scene = s_AssimpImporter->ReadFile(filepath.string(), flags);
	ASSERT(scene, s_AssimpImporter->GetErrorString());

	return std::make_shared<aiScene>(*scene);
}

std::unordered_map<int, std::unique_ptr<Mesh>> ModelLoader::GetModelMeshes(const aiScene& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::filesystem::path& filepath)
{
	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;
	std::string parentPath = filepath.parent_path().string() + "/";

	for (unsigned int i = 0; i < model.mNumMeshes; i++)
	{
		aiString aiTexturePath;
		int materialIndex = model.mMeshes[i]->mMaterialIndex;
		std::unique_ptr<Material> material = std::make_unique<Material>();
		const aiMaterial& aiMaterial = *model.mMaterials[materialIndex];

		// Get the albedo/diffuse map
		bool hasAlbedoMap = aiMaterial.GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &aiTexturePath) == AI_SUCCESS;
		if (!hasAlbedoMap)
		{
			hasAlbedoMap = aiMaterial.GetTexture(aiTextureType_DIFFUSE, 0, &aiTexturePath) == AI_SUCCESS;
			if (hasAlbedoMap)
			{
				material->AddMaterialMap(Material::Albedo, parentPath + aiTexturePath.C_Str());
			}
		}

		// Get the specular map, otherwise default the shininess
		bool hasSpecularMap = aiMaterial.GetTexture(aiTextureType_SPECULAR, 0, &aiTexturePath) == AI_SUCCESS;
		if (hasSpecularMap)
		{
			material->AddMaterialMap(Material::Specular, parentPath + aiTexturePath.C_Str());
		}
		else
		{
			float shininess;
			if (aiMaterial.Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
			{
				material->SetShininess(shininess);
			}
		}

		meshes[i] = std::make_unique<Mesh>(i, model, transform, color, std::move(material), filepath.string());
	}

	return meshes;
}

std::vector<ModelVertex> ModelLoader::GetMeshVertexVector(const aiScene& objModel, int meshIndex)
{
	const auto mesh = objModel.mMeshes[meshIndex];
	std::vector<ModelVertex> vertices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.emplace_back(
			*reinterpret_cast<DX::XMFLOAT3*>(&mesh->mVertices[i]),
			*reinterpret_cast<DX::XMFLOAT3*>(&mesh->mNormals[i]),
			//*reinterpret_cast<DX::XMFLOAT3*>(&mesh->mTangents[i]),
			//*reinterpret_cast<DX::XMFLOAT3*>(&mesh->mBitangents[i]),
			*reinterpret_cast<DX::XMFLOAT2*>(&mesh->mTextureCoords[0][i])
		);
	}

	return vertices;
}

std::vector<uint32_t> ModelLoader::GetMeshIndexVector(const aiScene& objModel, int meshIndex)
{
	const auto mesh = objModel.mMeshes[meshIndex];
	std::vector<uint32_t> indices;

	indices.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		ASSERT(face.mNumIndices == 3);

		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	return indices;
}

std::unique_ptr<Node> ModelLoader::ParseNode(const aiScene& model, int nextId, const aiNode& node, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes)
{
	std::vector<Mesh*> currentMeshes;
	const auto transform = DX::XMMatrixTranspose(DX::XMLoadFloat4x4(
		reinterpret_cast<const DX::XMFLOAT4X4*>(&node.mTransformation)
	));

	for (unsigned int i = 0; i < node.mNumMeshes; i++)
	{
		currentMeshes.push_back(modelMeshes.at(node.mMeshes[i]).get());
	}

	std::unique_ptr<Node> nextNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), transform, std::move(currentMeshes));
	for (unsigned int i = 0; i < node.mNumChildren; i++)
	{
		nextNode->AddChild(ParseNode(model, nextId, *node.mChildren[i], modelMeshes));
	}

	return std::move(nextNode);

}

//==============================Obj====================================
#pragma region obj
std::shared_ptr<rapidobj::Result> ModelLoader::LoadModelObj(const std::filesystem::path& filepath)
{
	std::shared_ptr<rapidobj::Result> model = std::make_shared<rapidobj::Result>(rapidobj::ParseFile(filepath));
	ASSERT(!model->error, model->error.code.message());
	ASSERT_VERIFY(Triangulate(*model.get()), model->error.code.message());

	return model;
}

std::unordered_map<int, std::unique_ptr<Mesh>> ModelLoader::GetModelMeshes(const rapidobj::Result& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::filesystem::path& filepath)
{
	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;

	for (int i = 0; i < model.shapes.size(); i++)
	{
		int materialIndex = model.shapes[i].mesh.material_ids[0];
		std::unique_ptr<Material> material = std::make_unique<Material>();

		if (model.materials[materialIndex].diffuse_texname != "")
		{
			material->AddMaterialMap(Material::Albedo, filepath.parent_path().string() + "/" + model.materials[materialIndex].diffuse_texname);
		}
		if (model.materials[materialIndex].specular_texname != "")
		{
			material->AddMaterialMap(Material::Specular, filepath.parent_path().string() + "/" + model.materials[materialIndex].specular_texname);
		}
		else
		{
			material->SetShininess(model.materials[materialIndex].shininess);
		}

		meshes[i] = std::make_unique<Mesh>(i, model, transform, color, std::move(material), filepath.string());
	}

	return meshes;
}

std::vector<ModelVertex> ModelLoader::GetMeshVertexVector(const rapidobj::Result& objModel, int meshIndex)
{
	const rapidobj::Mesh& mesh = objModel.shapes[meshIndex].mesh;
	std::vector<ModelVertex> vertices;

	for (int i = 0; i < objModel.shapes[meshIndex].mesh.num_face_vertices.size(); i++)
	{
		ASSERT(objModel.shapes[meshIndex].mesh.num_face_vertices[i] == 3);

		for (int j = 0; j < 3; j++)
		{
			int posIndex = mesh.indices[i * 3 + j].position_index;
			int normIndex = mesh.indices[i * 3 + j].normal_index;
			int texIndex = mesh.indices[i * 3 + j].texcoord_index;

			DX::XMFLOAT3 pos;
			DX::XMFLOAT3 norm;
			DX::XMFLOAT2 tex;

			pos.x = objModel.attributes.positions[posIndex * 3];
			pos.y = objModel.attributes.positions[posIndex * 3 + 1];
			pos.z = objModel.attributes.positions[posIndex * 3 + 2];

			norm.x = objModel.attributes.normals[normIndex * 3];
			norm.y = objModel.attributes.normals[normIndex * 3 + 1];
			norm.z = objModel.attributes.normals[normIndex * 3 + 2];

			tex.x = objModel.attributes.texcoords[texIndex * 2];
			tex.y = objModel.attributes.texcoords[texIndex * 2 + 1];

			if (s_FlipUVs)
			{
				DX::XMStoreFloat2(&tex, DX::XMVector2Transform(DX::XMLoadFloat2(&tex), DX::XMMatrixRotationX(DX::XMConvertToRadians(180)) * DX::XMMatrixTranslation(0.f, 1.f, 0.f)));
			}
			vertices.push_back({ pos, norm, tex });
		}

	}

	ASSERT(vertices.size() == mesh.num_face_vertices.size() * 3);

	return vertices;
}

std::vector<uint32_t> ModelLoader::GetMeshIndexVector(const rapidobj::Result& objModel, int meshIndex)
{
	std::vector<uint32_t> indices(objModel.shapes[meshIndex].mesh.num_face_vertices.size() * 3);

	std::generate(indices.begin(), indices.end(), [index = 0]() mutable { return index++; });

	return indices;
}

std::unique_ptr<Node> ModelLoader::ParseNode(const rapidobj::Result& model, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes)
{
	std::vector<Mesh*> currentMeshes;
	for (auto& mesh : modelMeshes)
	{
		currentMeshes.push_back(mesh.second.get());
	}
	
	std::unique_ptr<Node> root = std::make_unique<Node>(0, "Root", DX::XMMatrixIdentity(), std::move(currentMeshes));

	return std::move(root);
}
#pragma endregion



//==============================Gltf===================================
#pragma region gltf
std::shared_ptr<fastgltf::Asset> ModelLoader::LoadModelGltf(const std::filesystem::path& filepath)
{
	auto gltfFile = fastgltf::MappedGltfFile::FromPath(filepath);
	ASSERT(bool(gltfFile), fastgltf::getErrorMessage(gltfFile.error()));

	auto asset = (s_GltfParser.loadGltf(gltfFile.get(), filepath.parent_path(), s_GltfOptions));
	ASSERT(asset.error() == fastgltf::Error::None, fastgltf::getErrorMessage(asset.error()));

	return std::make_shared<fastgltf::Asset>(std::move(asset.get()));
}

std::unordered_map<int, std::unique_ptr<Mesh>> ModelLoader::GetModelMeshes(const fastgltf::Asset& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::string& filepath)
{
	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;

	for (int i = 0; i < model.meshes.size(); i++)
	{
		std::unique_ptr<Material> material = std::make_unique<Material>();
		meshes[i] = std::make_unique<Mesh>(i, model, transform, color, std::move(material), filepath);
	}

	return meshes;
}

std::vector<ModelVertex> ModelLoader::GetMeshVertexVector(const fastgltf::Asset& objModel, int meshIndex)
{
	std::vector<ModelVertex> vertices;

	for (auto it = objModel.meshes[meshIndex].primitives.begin(); it < objModel.meshes[meshIndex].primitives.end(); it++)
	{
		DX::XMFLOAT3 position;
		DX::XMFLOAT3 normal;

		auto* positionIt = it->findAttribute("POSITION");
		ASSERT(positionIt != it->attributes.end()); // A mesh primitive is required to hold the POSITION attribute
		ASSERT(it->indicesAccessor.has_value()); // We specify GenerateMeshIndices, so we should always have indices

		auto& positionAccessor = objModel.accessors[positionIt->accessorIndex];
		if (!positionAccessor.bufferViewIndex.has_value())
		{
			continue;
		}

		fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(objModel, positionAccessor, [&](fastgltf::math::fvec3 pos, std::size_t idx) {
			position.x = pos.x();
			position.y = pos.y();
			position.z = pos.z();
			
			vertices.push_back({ position, {} });
		});

		auto* normalIt = it->findAttribute("NORMAL");
		ASSERT(normalIt != it->attributes.end()); // A mesh primitive is required to hold the POSITION attribute
		ASSERT(it->indicesAccessor.has_value()); // We specify GenerateMeshIndices, so we should always have indices

		auto& normalAccessor = objModel.accessors[normalIt->accessorIndex];
		if (!normalAccessor.bufferViewIndex.has_value())
		{
			continue;
		}

		fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(objModel, normalAccessor, [&](fastgltf::math::fvec3 norm, std::size_t idx) {
			normal.x = norm.x();
			normal.y = norm.y();
			normal.z = norm.z();
			
			vertices[idx].Normal = normal;
		});

		vertices.push_back({ position, normal });
	}
	
	return vertices;
}

std::vector<uint32_t> ModelLoader::GetMeshIndexVector(const fastgltf::Asset& objModel, int meshIndex)
{
	std::vector<uint32_t> indices;
	for (auto it = objModel.meshes[meshIndex].primitives.begin(); it < objModel.meshes[meshIndex].primitives.end(); it++)
	{
		auto& indexAccessor = objModel.accessors[it->indicesAccessor.value()];
		ASSERT(indexAccessor.bufferViewIndex.has_value());

		indices.resize(indexAccessor.count);

		fastgltf::copyFromAccessor<std::uint32_t>(objModel, indexAccessor, indices.data());
	}

	return indices;
}

std::unique_ptr<Node> ModelLoader::ParseNode(const fastgltf::Asset& model, const fastgltf::Node& node, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes)
{
	DX::XMMATRIX transform = GetMeshTransform(model, node);

	std::vector<Mesh*> currentMesh;
	currentMesh.push_back(modelMeshes.at(static_cast<int>(node.meshIndex.value())).get());

	std::unique_ptr<Node> nextNode = std::make_unique<Node>(static_cast<int>(node.meshIndex.value()), node.name.c_str(), transform, std::move(currentMesh));
	for (int i = 0; i < node.children.size(); i++)
	{
		nextNode->AddChild(ParseNode(model, model.nodes[node.children[i]], modelMeshes));
	}

	return std::move(nextNode);

}

#pragma warning(disable:4715) // Disable warning about no return for all control paths since we are always guarenteed to return
const fastgltf::Node& ModelLoader::GetRootNode(const fastgltf::Asset& model)
{
	std::map<size_t, size_t> nodeMap;
	for (size_t i = 0; i < model.nodes.size(); i++)
	{
		nodeMap[i] = -1;
		for (int j = 0; j < model.nodes[i].children.size(); j++)
		{
			nodeMap[model.nodes[i].children[j]] = i;
		}
	}

	for (auto it = nodeMap.begin(); it != nodeMap.end(); it++)
	{
		if (it->second == -1) return model.nodes[it->first];
	}

	ASSERT(false, "No Root node found!");
	#pragma warning(default:4715)
}

DX::XMMATRIX ModelLoader::GetMeshTransform(const fastgltf::Asset& model, const fastgltf::Node& node)
{
	DX::XMMATRIX transform;
	if (auto value = std::get_if<fastgltf::math::fmat4x4>(&node.transform))
	{
		transform = DX::XMMatrixTranspose(DX::XMLoadFloat4x4(reinterpret_cast<const DX::XMFLOAT4X4*>(&node.transform)));
	}
	else
	{
		auto trs = std::get<fastgltf::TRS>(node.transform);
		transform =
			DX::XMMatrixScaling(trs.scale.x(), trs.scale.y(), trs.scale.z()) *
			DX::XMMatrixRotationRollPitchYaw(trs.rotation.x(), trs.rotation.y(), trs.rotation.z()) *
			DX::XMMatrixTranslation(trs.translation.x(), trs.translation.y(), trs.translation.z());
	}

	return transform;
}
#pragma endregion



//===============================Fbx===================================
#pragma region fbx
std::shared_ptr<UfbxScene> ModelLoader::LoadModelFbx(const std::filesystem::path& filepath)
{
	ufbx_load_opts opts = { }; // Optional, pass NULL for defaults
	ufbx_error error; // Optional, pass NULL if you don't care about errors

	std::shared_ptr<UfbxScene> scene = std::make_shared<UfbxScene>(ufbx_load_file(ufbx_string_view(filepath.generic_string().c_str(), filepath.generic_string().length()), &opts, &error));
	ASSERT(scene.get(), error.description.data);

	return scene;
}

std::unordered_map<int, std::unique_ptr<Mesh>> ModelLoader::GetModelMeshes(const UfbxScene& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::string& filepath)
{
	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;

	for (const auto& mesh : model.Meshes())
	{
		int meshIndex = mesh->element_id;
		meshes[meshIndex] = std::make_unique<Mesh>(meshIndex, model, transform, color, nullptr, filepath);
	}

	return meshes;
}

std::vector<ModelVertex> ModelLoader::GetMeshVertexVector(const UfbxScene& objModel, int meshIndex)
{
	std::vector<ModelVertex> vertices;
	const auto& mesh = reinterpret_cast<ufbx_mesh*>(objModel.Elements()[meshIndex]);

	std::vector<uint32_t> tri_indices;
	tri_indices.resize(mesh->max_face_triangles * 3);

	for (ufbx_face face : mesh->faces)
	{

		// Triangulate the face into tri_indices[]
		uint32_t num_tris = ufbx_triangulate_face(tri_indices.data(), tri_indices.size(), mesh, face);

		for (uint32_t i = 0; i < num_tris * 3; i++)
		{
			uint32_t index = tri_indices[i];

            ufbx_vec3 position = mesh->vertex_position[index];
            ufbx_vec3 normal = mesh->vertex_normal[index];

			vertices.push_back({ {position.x, position.y, position.z}, {normal.x, normal.y, normal.z} });
		}

    }
	ASSERT(vertices.size() == mesh->num_triangles * 3);

	return vertices;
}

std::vector<uint32_t> ModelLoader::GetMeshIndexVector(const UfbxScene& objModel, std::vector<ModelVertex>& vertices, int meshIndex)
{
	std::vector<uint32_t> indices;
	const auto& mesh = reinterpret_cast<ufbx_mesh*>(objModel.Elements()[meshIndex]);
	indices.resize(mesh->num_triangles * 3);

	ufbx_vertex_stream stream[1] = { {vertices.data(), vertices.size(), sizeof(ModelVertex)} };

	uint32_t num_vertices = static_cast<uint32_t>(ufbx_generate_indices(stream, 1, indices.data(), indices.size(), nullptr, nullptr));

	vertices.resize(num_vertices);
	return indices;

}

std::unique_ptr<Node> ModelLoader::ParseNode(const UfbxScene& model, const ufbx_node& node, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes)
{
	DX::XMMATRIX transform = GetNodeTransform(node);

	std::vector<Mesh*> currentMesh;
	std::unique_ptr<Node> nextNode;
	if (node.mesh != nullptr)
	{
		currentMesh.push_back(modelMeshes.at(node.mesh->element_id).get());
		nextNode = std::make_unique<Node>(node.mesh->element_id, node.name.data, transform, std::move(currentMesh));
	}
	else
	{
		ASSERT(DX::XMMatrixIsIdentity(transform));
		nextNode = std::make_unique<Node>();
	}

	for (int i = 0; i < node.children.count; i++)
	{
		nextNode->AddChild(ParseNode(model, *node.children[i], modelMeshes));
	}
	return std::move(nextNode);
}


DX::XMMATRIX ModelLoader::GetNodeTransform(const ufbx_node& node)
{
	DX::XMMATRIX transform;
	auto trs = node.local_transform;
	transform =
		DX::XMMatrixScaling(trs.scale.x, trs.scale.y, trs.scale.z) *
		DX::XMMatrixRotationQuaternion({ trs.rotation.x, trs.rotation.y, trs.rotation.z }) *
		DX::XMMatrixTranslation(trs.translation.x, trs.translation.y, trs.translation.z);

	return transform;
}
#pragma endregion

