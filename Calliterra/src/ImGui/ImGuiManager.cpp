#include "pch.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include "Core/GlobalSettings.h"

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

void ImGuiManager::SettingsGui()
{
	if (m_IsImGuiEnabled)
	{
		ImVec2 guiSize = { 250.f, 250.f };
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::SetNextWindowSize(guiSize);

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		ImGui::Begin("Settings", nullptr, flags);
		if (ImGui::CollapsingHeader("Camera"))
		{
			GlobalSettings::Notify(ImGui::SliderInt("Fov", &GlobalSettings::Camera::m_Fov, 45, 120), SettingsType::Fov);
		}
		if (ImGui::CollapsingHeader("Renderer"))
		{
			GlobalSettings::Notify(ImGui::Checkbox("Enable Wireframe", &GlobalSettings::Rendering::m_IsWireFrame), SettingsType::IsWireFrame);
			ImGui::SeparatorText("Cull Mode");
			GlobalSettings::Notify(ImGui::RadioButton("None", &GlobalSettings::Rendering::m_CullType, 0), SettingsType::CullMode); ImGui::SameLine();
			GlobalSettings::Notify(ImGui::RadioButton("Front", &GlobalSettings::Rendering::m_CullType, 1), SettingsType::CullMode); ImGui::SameLine();
			GlobalSettings::Notify(ImGui::RadioButton("Back", &GlobalSettings::Rendering::m_CullType, 2), SettingsType::CullMode);

		}
		ImGui::End();
	}
}

void ImGuiManager::DebugGui(DeltaTime dt)
{
	if (m_IsImGuiEnabled)
	{
		ImVec2 guiSize = { 175.f, 100.f };

		ImGui::SetNextWindowPos({ m_WindowWidth - guiSize.x, 0 });
		ImGui::SetNextWindowSize(guiSize);
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		ImGui::Begin("Debug Info", nullptr, flags);

		ImGui::Text("FPS: %d", static_cast<int>(1 / dt.GetSeconds()));
		ImGui::Text("Frame Time: %.3lf ms", dt.GetMilliseconds());

		ImGui::End();
	}
}

void ImGuiManager::DemoWindow()
{
	if (m_IsImGuiEnabled)
	{
		ImGui::ShowDemoWindow();

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

void ImGuiManager::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>([this](WindowResizeEvent& e) { return this->OnWindowResize(e); });
}

bool ImGuiManager::OnWindowResize(WindowResizeEvent& e)
{
	m_WindowWidth = e.GetWidth();
	m_WindowHeight = e.GetHeight();
	return true;
}

void ImGuiManager::SetWindowSize(float width, float height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;
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
