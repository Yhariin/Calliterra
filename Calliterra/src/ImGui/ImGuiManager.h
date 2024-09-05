#pragma once
#include "Core/DeltaTime.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Renderer/Camera.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void Begin();
	void SettingsGui();
	void DebugGui(DeltaTime dt, const Camera& camera);
	void DemoWindow();
	void End();

	void OnEvent(Event& e);
	bool OnWindowResize(WindowResizeEvent& e);

	void SetWindowSize(float width, float height);

	void EnableSettingsGui();
	void DisableSettingsGui();

	void EnableDebugGui();
	void DisableDebugGui();

	bool IsSettingsGuiEnabled();
	bool IsDebugGuiEnabled();

private:
	bool m_IsDebugGuiEnabled = false;
	bool m_IsSettingsGuiEnabled = false;
	float m_WindowWidth = 0;
	float m_WindowHeight = 0;
};

