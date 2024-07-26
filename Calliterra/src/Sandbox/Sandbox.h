#pragma once
#include "Drawable.h"
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

	void CreateCube();
	void CreateRadialSphere();
	void CreateIcoSphere();
	void CreatePlane();

private:
	std::vector<std::unique_ptr<Drawable>> m_Drawables;
	Camera m_Camera;
};
