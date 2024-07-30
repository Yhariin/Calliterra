#pragma once
#include "Core/DeltaTime.h"
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void Begin();
	void SettingsGui();
	void DebugGui(DeltaTime dt);
	void DemoWindow();
	void End();

	void OnEvent(Event& e);
	bool OnWindowResize(WindowResizeEvent& e);

	void SetWindowSize(float width, float height);
	void EnableImGui();
	void DisableImGui();
	bool IsImGuiEnabled();

private:
	bool m_IsImGuiEnabled = false;
	float m_WindowWidth;
	float m_WindowHeight;
};

