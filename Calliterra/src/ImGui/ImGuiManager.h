#pragma once
#include "Core/DeltaTime.h"

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	static void Begin();
	static void SettingsGui();
	static void DebugGui(DeltaTime dt);
	static void DemoWindow();
	static void End();

	static void EnableImGui();
	static void DisableImGui();
	static bool IsImGuiEnabled();

private:
	inline static bool m_IsImGuiEnabled = false;
};

