#pragma once

#include <rapidobj/rapidobj.hpp>
#include <fastgltf/core.hpp>
#include <ufbx.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Mesh.h"
#include "ModelAPI.h"

// Currently we will turn this off and use Assimp for our model loading needs.
// At some point we will want to write our own data transformation functions to 
// utilize the faster model loading libraries to work with Lefthanded systems (and more!)
#define FAST_MODEL_LOADING 0

class Model;

struct ModelVertexFull
{
	DX::XMFLOAT3 Position;
	DX::XMFLOAT3 Normal;
	DX::XMFLOAT3 Tangent;
	DX::XMFLOAT3 Bitangent;
	DX::XMFLOAT2 Texture;
};

struct ModelVertexSemi
{
	DX::XMFLOAT3 Position;
	DX::XMFLOAT3 Normal;
	DX::XMFLOAT2 Texture;
};


class ModelLoader
{
public:
	using Obj = rapidobj::Result;
	using Gltf = fastgltf::Asset;
	using Fbx = UfbxScene;

	static void Init();
	static void Shutdown();

	static ModelAPI LoadModel(const std::filesystem::path& filepath);
	static Model GetModel(const std::filesystem::path& filepath, const DX::XMMATRIX& transform, DX::XMFLOAT3 color);

	static std::vector<ModelVertexFull> GetMeshVertexVectorFull(const aiScene& objModel, int meshIndex);
	static std::vector<ModelVertexSemi> GetMeshVertexVectorSemi(const aiScene& objModel, int meshIndex);
	static std::vector<ModelVertexSemi> GetMeshVertexVector(const rapidobj::Result& objModel, int meshIndex);
	static std::vector<ModelVertexSemi> GetMeshVertexVector(const fastgltf::Asset& objModel, int meshIndex);
	static std::vector<ModelVertexSemi> GetMeshVertexVector(const UfbxScene& objModel, int meshIndex);

	static std::vector<uint32_t> GetMeshIndexVector(const aiScene& objModel, int meshIndex);
	static std::vector<uint32_t> GetMeshIndexVector(const rapidobj::Result& objModel, int meshIndex);
	static std::vector<uint32_t> GetMeshIndexVector(const fastgltf::Asset& objModel, int meshIndex);
	static std::vector<uint32_t> GetMeshIndexVector(const UfbxScene& objModel, std::vector<ModelVertexSemi>& vertices, int meshIndex);

private:
	static std::shared_ptr<aiScene> LoadModelAssimp(const std::filesystem::path& filepath);
	static std::shared_ptr<rapidobj::Result> LoadModelObj(const std::filesystem::path& filepath);
	static std::shared_ptr<fastgltf::Asset> LoadModelGltf(const std::filesystem::path& filepath);
	static std::shared_ptr<UfbxScene> LoadModelFbx(const std::filesystem::path& filepath);

	static std::unordered_map<int, std::unique_ptr<Mesh>> GetModelMeshes(const aiScene& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::filesystem::path& filepath);
	static std::unordered_map<int, std::unique_ptr<Mesh>> GetModelMeshes(const rapidobj::Result& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::filesystem::path& filepath);
	static std::unordered_map<int, std::unique_ptr<Mesh>> GetModelMeshes(const fastgltf::Asset& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::string& filepath);
	static std::unordered_map<int, std::unique_ptr<Mesh>> GetModelMeshes(const UfbxScene& model, const DX::XMMATRIX& transform, DX::XMFLOAT3 color, const std::string& filepath);

	static std::unique_ptr<Node> ParseNode(const aiScene& model, int nextId, const aiNode& node, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes);
	static std::unique_ptr<Node> ParseNode(const rapidobj::Result& model, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes);
	static std::unique_ptr<Node> ParseNode(const fastgltf::Asset& model, const fastgltf::Node& node, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes);
	static std::unique_ptr<Node> ParseNode(const UfbxScene& model, const ufbx_node& node, const std::unordered_map<int, std::unique_ptr<Mesh>>& modelMeshes);

	static const fastgltf::Node& GetRootNode(const fastgltf::Asset& model);

	static DX::XMMATRIX GetMeshTransform(const fastgltf::Asset& model, const fastgltf::Node& node);
	static DX::XMMATRIX GetNodeTransform(const ufbx_node& node);

private:
	inline static constexpr auto s_GltfSupportedExtensions =
		fastgltf::Extensions::KHR_mesh_quantization |
		fastgltf::Extensions::KHR_texture_transform |
		fastgltf::Extensions::KHR_materials_variants;

	inline static constexpr auto s_GltfOptions =
		fastgltf::Options::DontRequireValidAssetMember |
		fastgltf::Options::AllowDouble |
		fastgltf::Options::LoadExternalBuffers |
		fastgltf::Options::LoadExternalImages |
		fastgltf::Options::GenerateMeshIndices;

	inline static fastgltf::Parser s_GltfParser = fastgltf::Parser(s_GltfSupportedExtensions);
	inline static constexpr bool s_FlipUVs = true;

	// Map used to cache models to prevent reloading ones we've previously loaded
	inline static std::unordered_map<std::string, ModelAPI> s_Models;
	inline static std::unique_ptr<Assimp::Importer> s_AssimpImporter = nullptr;
};

