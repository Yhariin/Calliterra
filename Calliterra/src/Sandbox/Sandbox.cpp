#include "pch.h"
#include "Sandbox.h"
#include "Sandbox/BasicShapes/Cube.h"
#include "Sandbox/BasicShapes/RadialSphere.h"
#include "Sandbox/BasicShapes/IcoSphere.h"
#include "Sandbox/BasicShapes/Plane.h"
#include "Sandbox/Components/PointLight.h"
#include "Asset/Model.h"
#include "Asset/ModelLoader.h"

Sandbox::Sandbox(float aspectRatio)
	: m_Camera(aspectRatio, 90.f)
{
	Cube::InitBuffers();
		
}

void Sandbox::OnUpdate(float dt)
{
	m_Camera.OnUpdate(dt);
	for (auto& drawable : m_Drawables)
	{
		drawable->SetViewMatrix(m_Camera.GetViewMatrix());
		drawable->SetProjectionMatrix(m_Camera.GetProjectionMatrix());
		drawable->Update(dt);
		drawable->Draw();
	}

}

void Sandbox::OnEvent(Event& e)
{
	m_Camera.OnEvent(e);
}

void Sandbox::LoadSandboxPreset()
{
	CreatePlane();
	for(int i = 0; i < 300; i++)
	{ 
		CreateCube();
	}
	CreatePointLight();

	std::shared_ptr<rapidobj::Result> dragonModel = ModelLoader::LoadModelObj("assets/models/dragon.obj");
	//std::shared_ptr<fastgltf::Asset> vaseClayModel = ModelLoader::LoadModelGltf("assets/models/Vase_Clay.gltf");
	//std::shared_ptr<fastgltf::Asset> nanoModel = ModelLoader::LoadModelGltf("assets/models/nano.gltf");
	std::shared_ptr<rapidobj::Result> nanoModelObj = ModelLoader::LoadModelObj("assets/models/nanosuit.obj");
	//std::shared_ptr<UfbxScene> heartModel = ModelLoader::LoadModelFbx("assets/models/HumanHeart_FBX.fbx");

	DX::XMMATRIX transform = DX::XMMatrixRotationX(DX::XMConvertToRadians(90)) * DX::XMMatrixTranslation(10.f, 10.f, -10.f);
	DX::XMMATRIX transform2 = DX::XMMatrixIdentity();
	//m_Drawables.emplace_back(std::make_unique<Model>(nanoModel, transform, DX::XMFLOAT3(0.8f, 0.3f, 0.8f )));
	m_Drawables.emplace_back(std::make_unique<Model>(nanoModelObj, transform2, DX::XMFLOAT3(0.2f, 0.4f, 0.9f )));
	m_Drawables.emplace_back(std::make_unique<Model>(dragonModel, DX::XMMatrixTranslation(10.f, 0.f, -10.f), DX::XMFLOAT3(0.8f, 0.3f, 0.8f )));
	//m_Drawables.emplace_back(std::make_unique<Model>(vaseClayModel, DX::XMMatrixTranslation(0.f, 0.f, -10.f), DX::XMFLOAT3(0.2f, 0.3f, 0.78f )));
	//m_Drawables.emplace_back(std::make_unique<Model>(heartModel, DX::XMMatrixTranslation(5.f, 10.f, -10.f), DX::XMFLOAT3(0.2f, 0.3f, 0.78f )));

}

void Sandbox::CreateCube()
{
	m_Drawables.emplace_back(std::make_unique<Cube>());
}

void Sandbox::CreateRadialSphere()
{
	m_Drawables.emplace_back( std::make_unique<RadialSphere>() );
}

void Sandbox::CreateIcoSphere()
{
	m_Drawables.emplace_back(std::make_unique<IcoSphere>(4, DX::XMMatrixTranslation(0.f, 20.f, 10.f)));
}

void Sandbox::CreatePlane()
{
	m_Drawables.emplace_back(std::make_unique<Plane>(1));

	m_Drawables.back()->SetTransform(
		DX::XMMatrixTranslation(-0.5f, 0.0f, -0.5f) *
		DX::XMMatrixScaling(100.f, 1.f, 100.f)
	);
}

void Sandbox::CreatePointLight()
{
	m_Drawables.emplace_back(std::make_unique<PointLight>(DX::XMMatrixTranslation(0.f, 20.f, 10.f), DX::XMFLOAT3(1.f, 1.f, 1.f) ));
}
