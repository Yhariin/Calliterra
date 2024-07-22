#include "pch.h"
#include "Sandbox.h"
#include "Sandbox/BasicShapes/Cube.h"

Sandbox::Sandbox()
{
	Cube::InitBuffers();
}

void Sandbox::OnUpdate(float dt)
{

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
		drawable->Update(dt);
		drawable->Draw();
	}
}

void Sandbox::CreateCube()
{

	m_Drawables.emplace_back(std::make_unique<Cube>());

}

