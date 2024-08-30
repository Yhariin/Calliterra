#include "pch.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>
#include "Core/GlobalSettings.h"

static std::string YawToDirection(float yawInDegrees)
{
	if (yawInDegrees > -22.5f && yawInDegrees < 22.5f)
	{
		return "North";
	}
	else if (yawInDegrees > 22.5f && yawInDegrees < 67.5f)
	{
		return "North East";
	}
	else if (yawInDegrees > 67.5f && yawInDegrees < 112.5f)
	{
		return "East";
	}
	else if (yawInDegrees > 112.5f && yawInDegrees < 157.5f)
	{
		return "South East";
	}
	else if (yawInDegrees > 157.5f && yawInDegrees < 180.f || yawInDegrees > -180.f && yawInDegrees < -157.5f)
	{
		return "South";
	}
	else if (yawInDegrees > -157.5f && yawInDegrees < -112.5f)
	{
		return "South West";
	}
	else if (yawInDegrees > -112.5f && yawInDegrees < -67.5f)
	{
		return "West";
	}
	else if (yawInDegrees > -67.5f && yawInDegrees < -22.5f)
	{
		return "North West";
	}

	ASSERT(false, "Invalid direction!");
	return "Null";
}

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
	if (m_IsSettingsGuiEnabled || m_IsDebugGuiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

	}
}

void ImGuiManager::SettingsGui()
{
	if (m_IsSettingsGuiEnabled)
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
			GlobalSettings::Notify(ImGui::SliderFloat3("LightPos", GlobalSettings::Rendering::m_LightPos, -60.f, 60.f), SettingsType::PointLightPosition);

		}
		ImGui::End();
	}
}

void ImGuiManager::DebugGui(DeltaTime dt, const Camera& camera)
{
	if (m_IsDebugGuiEnabled)
	{
		// TODO: Implement .ttf font file for high quality font for higher font scaling
		// https://github.com/ocornut/imgui/issues/1018#issuecomment-1891041578 

		ImVec2 guiSize = { 175.f, 150.f };

		ImGui::SetNextWindowPos({ m_WindowWidth - guiSize.x, 0 });
		ImGui::SetNextWindowSize(guiSize);
		ImGui::SetNextWindowBgAlpha(0.35f);
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
		ImGui::Begin("Debug Info", nullptr, flags);

		ImGui::Text("FPS: %d", static_cast<int>(1 / dt.GetSeconds()));
		ImGui::Text("Frame Time: %.3lf ms", dt.GetMilliseconds());

		ImGui::Text("Position:");
		DX::XMFLOAT3 cameraPos = camera.GetPosition();
		ImGui::Text("(%.2f, %.2f, %.2f)", cameraPos.x, cameraPos.y, cameraPos.z);
		ImGui::Text(YawToDirection(DX::XMConvertToDegrees(camera.GetYaw())).c_str());

		ImGui::End();
	}
}

void ImGuiManager::DemoWindow()
{
	if (m_IsSettingsGuiEnabled || m_IsDebugGuiEnabled)
	{
		ImGui::ShowDemoWindow();
	}
}

void ImGuiManager::End()
{
	if (m_IsSettingsGuiEnabled || m_IsDebugGuiEnabled)
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

void ImGuiManager::EnableSettingsGui()
{
	m_IsSettingsGuiEnabled = true;
}

void ImGuiManager::DisableSettingsGui()
{
	m_IsSettingsGuiEnabled = false;
}

void ImGuiManager::EnableDebugGui()
{
	m_IsDebugGuiEnabled = true;
}

void ImGuiManager::DisableDebugGui()
{
	m_IsDebugGuiEnabled = false;
}

bool ImGuiManager::IsSettingsGuiEnabled()
{
	return m_IsSettingsGuiEnabled;
}

bool ImGuiManager::IsDebugGuiEnabled()
{
	return m_IsDebugGuiEnabled;
}
