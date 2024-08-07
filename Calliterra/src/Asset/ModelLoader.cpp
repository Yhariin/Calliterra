#include "pch.h"
#include "ModelLoader.h"
#include "fastgltf/tools.hpp"

std::shared_ptr<rapidobj::Result> ModelLoader::LoadModelObj(const std::filesystem::path& filepath)
{
	std::shared_ptr<rapidobj::Result> model = std::make_shared<rapidobj::Result>(rapidobj::ParseFile(filepath));
	ASSERT(!model->error, model->error.code.message());
	ASSERT_VERIFY(Triangulate(*model.get()), model->error.code.message());

	return model;
}

std::shared_ptr<fastgltf::Asset> ModelLoader::LoadModelGltf(const std::filesystem::path& filepath)
{
	auto gltfFile = fastgltf::MappedGltfFile::FromPath(filepath);
	ASSERT(bool(gltfFile), fastgltf::getErrorMessage(gltfFile.error()));

	auto asset = (m_GltfParser.loadGltf(gltfFile.get(), filepath.parent_path(), m_GltfOptions));
	ASSERT(asset.error() == fastgltf::Error::None, fastgltf::getErrorMessage(asset.error()));

	return std::make_shared<fastgltf::Asset>(std::move(asset.get()));
}

std::shared_ptr<UfbxScene> ModelLoader::LoadModelFbx(const std::filesystem::path& filepath)
{
	ufbx_load_opts opts = { }; // Optional, pass NULL for defaults
	ufbx_error error; // Optional, pass NULL if you don't care about errors

	std::shared_ptr<UfbxScene> scene = std::make_shared<UfbxScene>(ufbx_load_file(ufbx_string_view(filepath.generic_string().c_str(), filepath.generic_string().length()), &opts, &error));
	//ufbx_scene* scene1 = ufbx_load_file("assets/models/HumanHeart_FBX.fbx", &opts, &error);
	ASSERT(scene.get(), error.description.data);

	return scene;
}

std::vector<ModelVertex> ModelLoader::GetModelVertexVector(const rapidobj::Result& objModel)
{
	std::vector<ModelVertex> vertices;
	for (int i = 0; i < objModel.attributes.positions.size(); i += 3)
	{
		DX::XMFLOAT3 pos;
		DX::XMFLOAT3 norm;

		pos.x = objModel.attributes.positions[i];
		pos.y = objModel.attributes.positions[i+1];
		pos.z = objModel.attributes.positions[i+2];

		norm.x = objModel.attributes.normals[i];
		norm.y = objModel.attributes.normals[i+1];
		norm.z = objModel.attributes.normals[i+2];

		vertices.push_back({ pos, norm });
	}

	return vertices;
}

std::vector<uint32_t> ModelLoader::GetModelIndexVector(const rapidobj::Result& objModel)
{
	std::vector<uint32_t> indices;
	for (int i = 0; i < objModel.shapes[0].mesh.indices.size(); i++)
	{
		indices.push_back(objModel.shapes[0].mesh.indices[i].position_index);
	}

	return indices;
}

std::vector<ModelVertex> ModelLoader::GetModelVertexVector(const fastgltf::Asset& objModel)
{
	std::vector<ModelVertex> vertices;

	for (auto it = objModel.meshes[0].primitives.begin(); it < objModel.meshes[0].primitives.end(); it++)
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

std::vector<uint32_t> ModelLoader::GetModelIndexVector(const fastgltf::Asset& objModel)
{
	std::vector<uint32_t> indices;
	for (auto it = objModel.meshes[0].primitives.begin(); it < objModel.meshes[0].primitives.end(); it++)
	{
		auto& indexAccessor = objModel.accessors[it->indicesAccessor.value()];
		ASSERT(indexAccessor.bufferViewIndex.has_value());

		indices.resize(indexAccessor.count);

		fastgltf::copyFromAccessor<std::uint32_t>(objModel, indexAccessor, &indices[0]);
	}

	return indices;
}

std::vector<ModelVertex> ModelLoader::GetModelVertexVector(const UfbxScene& objModel)
{
	std::vector<ModelVertex> vertices;
	auto& mesh = objModel.Meshes()[0];
	//vertices.reserve(objModel.Meshes()[0]->num_vertices);

	//int index = 0;
	//for (auto position : objModel.Meshes()[0]->vertex_position.values)
	//{
	//	vertices.push_back({ {position.x, position.y, position.z}, {} });
	//	index++;
	//}

	//index = 0;
	//for (auto normal : objModel.Meshes()[0]->vertex_normal.values)
	//{
	//	vertices[index].Normal = { normal.x, normal.y, normal.z };
	//	index++;
	//}

	//for (ufbx_face face : mesh->faces)
	//{
	//	for (uint32_t corner = 0; corner < face.num_indices; corner++)
	//	{
	//		vertices.push_back({});
	//	}

	//}

	std::vector<uint32_t> tri_indices;
	tri_indices.resize(mesh->max_face_triangles * 3);

	// TODO: triangulate the faces
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
			//vertices[index] = { {position.x, position.y, position.z}, {normal.x, normal.y, normal.z} };
		}

		/*
        // Loop through the corners of the polygon.
        for (uint32_t corner = 0; corner < face.num_indices; corner++) 
		{
            // Faces are defined by consecutive indices, one for each corner.
            uint32_t index = face.index_begin + corner;

            // Retrieve the position, normal and uv for the vertex.
            ufbx_vec3 position = mesh->vertex_position[index];
            ufbx_vec3 normal = mesh->vertex_normal[index];

			vertices[index] = { {position.x, position.y, position.z}, {normal.x, normal.y, normal.z} };
			//vertices.push_back({ {position.x, position.y, position.z}, {normal.x, normal.y, normal.z} });
        }
		*/
    }
	ASSERT(vertices.size() == mesh->num_triangles * 3);

	return vertices;
}

std::vector<uint32_t> ModelLoader::GetModelIndexVector(const UfbxScene& objModel, std::vector<ModelVertex>& vertices)
{
	std::vector<uint32_t> indices;
	auto& mesh = objModel.Meshes()[0];
	indices.resize(mesh->num_triangles * 3);

	//std::vector<ModelVertex> vertices = ModelLoader::GetModelVertexVector(objModel);
	ufbx_vertex_stream stream[1] = { {vertices.data(), vertices.size(), sizeof(ModelVertex)} };

	uint32_t num_vertices = ufbx_generate_indices(stream, 1, indices.data(), indices.size(), nullptr, nullptr);

	vertices.resize(num_vertices);
	return indices;
	//indices.reserve(objModel.Meshes()[0]->max_face_triangles * 3);

	//for (auto face : objModel.Meshes()[0]->faces)
	//{
	//	for (uint32_t corner = 0; corner < face.num_indices; corner++)
	//	{
	//		indices.push_back(face.index_begin + corner);
	//	}
	//}

}
