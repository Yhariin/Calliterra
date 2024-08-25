#pragma once
#include "Renderer/Drawable.h"
#include "Renderer/Camera.h"
#include "Events/Event.h"

class Sandbox
{
public:
	Sandbox() = default;
	Sandbox(float aspectRatio);

	void OnUpdate(float dt);
	void OnEvent(Event& e);

	void LoadSandboxPreset();

	void CreateCube(const DX::XMMATRIX& transform = DX::XMMatrixIdentity());
	void CreateRadialSphere();
	void CreateIcoSphere();
	void CreatePlane();
	void CreatePointLight();

private:
	std::vector<std::unique_ptr<Drawable>> m_Drawables;
	Camera m_Camera;
};
