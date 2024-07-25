#pragma once

class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	static void Begin();
	static void End();

	static void EnableImGui();
	static void DisableImGui();
	static bool IsImGuiEnabled();

private:
	inline static bool m_IsImGuiEnabled = false;
};

