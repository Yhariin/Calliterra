#include "pch.h"
#include "ModelAPI.h"

ModelAPI::ModelAPI(std::shared_ptr<AssimpAPI> modelAPI)
	: m_API(Assimp), m_Assimp(modelAPI)
{
}

ModelAPI::ModelAPI(std::shared_ptr<ObjAPI> modelAPI)
	: m_API(Obj), m_Obj(modelAPI)
{
}

ModelAPI::ModelAPI(std::shared_ptr<GltfAPI> modelAPI)
	: m_API(Gltf), m_Gltf(modelAPI)
{
}

ModelAPI::ModelAPI(std::shared_ptr<FbxAPI> modelAPI)
	: m_API(Fbx), m_Fbx(modelAPI)
{
}

ModelAPI::API ModelAPI::GetAPI()
{
	return m_API;
}

std::shared_ptr<AssimpAPI> ModelAPI::GetAssimp()
{
	return m_Assimp;
}

std::shared_ptr<ObjAPI> ModelAPI::GetObj()
{
	return m_Obj;
}

std::shared_ptr<GltfAPI> ModelAPI::GetGltf()
{
	return m_Gltf;
}

std::shared_ptr<FbxAPI> ModelAPI::GetFbx()
{
	return m_Fbx;
}


