#include "pch.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

ImGuiManager::ImGuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	if (m_IsImGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
}

void ImGuiManager::End()
{
	if (m_IsImGuiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void ImGuiManager::EnableImGui()
{
	m_IsImGuiEnabled = true;
}

void ImGuiManager::DisableImGui()
{
	m_IsImGuiEnabled = false;
}

bool ImGuiManager::IsImGuiEnabled()
{
	return m_IsImGuiEnabled;
}
