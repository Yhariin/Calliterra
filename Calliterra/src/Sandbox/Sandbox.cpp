#include "pch.h"
#include "Sandbox.h"
#include "Sandbox/BasicShapes/Cube.h"
#include "Sandbox/BasicShapes/RadialSphere.h"
#include "Sandbox/BasicShapes/IcoSphere.h"
#include "Sandbox/BasicShapes/Plane.h"

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
		//DX::XMMATRIX transform = DX::XMMatrixTranspose(
		//	DX::XMMatrixRotationZ(dt) *
		//	DX::XMMatrixRotationY(dt) *
		//	DX::XMMatrixRotationX(dt) *
		//	DX::XMMatrixScaling(0.5f, 0.5f, 0.5f) *
		//	DX::XMMatrixTranslation(i, 0.0f, 2.f)*
		//	DX::XMMatrixPerspectiveFovLH(90.f, 16.f/9.f, 0.05f, 10.f)
		//);

		//drawable->Update(transform);
		DX::XMMATRIX objTransform = drawable->GetTransform();
		drawable->Update(dt);
		drawable->SetViewProjectionMatrix(m_Camera.GetViewProjectionMatrix());
		drawable->Draw();
	}


}

void Sandbox::OnEvent(Event& e)
{
	m_Camera.OnEvent(e);
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
	m_Drawables.emplace_back(std::make_unique<IcoSphere>(7));
}

void Sandbox::CreatePlane()
{
	m_Drawables.emplace_back(std::make_unique<Plane>(1));

	m_Drawables.back()->Update(
		DX::XMMatrixScaling(40.f, 1.f, 40.f) * 
		DX::XMMatrixTranslation(0.f, 0.0f, 0.f) 
	);
}
