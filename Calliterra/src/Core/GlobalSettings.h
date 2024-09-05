#pragma once

enum class SettingsType
{
	None = 0,
	IsWireFrame,
	CullMode,
	Fov,
	PointLightPosition

};

enum class SettingsCategory
{
	None = 0,
	Rendering,
	Camera
};

class SettingsSubscriber
{
public:
	virtual ~SettingsSubscriber() = default;
	virtual void OnSettingsUpdate(SettingsType) = 0;
};

class GlobalSettings
{
//============================= Define settings here ============================================
public:
	struct Rendering
	{
		friend class GlobalSettings;
		friend class ImGuiManager;
		friend class Application;
		enum CullMode
		{
			CullNone = 0,
			CullFront = 1,
			CullBack = 2
		};

		static bool IsWireFrame() { return m_IsWireFrame; }
		static int CullType() { return m_CullType; }
		static DX::XMFLOAT3 LightPos() { return DX::XMFLOAT3(m_LightPos[0], m_LightPos[1], m_LightPos[2]); }
	private:
		inline static bool m_IsWireFrame = false;
		inline static int m_CullType = CullBack;
		inline static float m_LightPos[3] = {10.f, 9.f, 2.5f};
	};

	struct Camera
	{
		friend class GlobalSettings;
		friend class ImGuiManager;

		static int Fov() { return m_Fov; }
	private:
		inline static int m_Fov = 90;
	};

//============================= Static methods here ============================================
public:
	static void Notify(bool hasChanged, SettingsType setting)
	{
		if (hasChanged)
		{
			if (auto search = m_SubscriberMap.find(setting); search != m_SubscriberMap.end())
			{
				for (const auto& subscriber : search->second)
				{
					subscriber->OnSettingsUpdate(setting);
				}
			}
			else
			{
				LOG_DEBUG("No subscribers for this setting");
			}
		}
	}

	static void Register(SettingsType setting, SettingsSubscriber* subscriber)
	{
		m_SubscriberMap[setting].push_back(subscriber);
	}

	static void Register(std::vector<SettingsType> settings, SettingsSubscriber* subscriber)
	{
		for (const auto& setting : settings)
		{
			m_SubscriberMap[setting].push_back(subscriber);
		}
	}

private:
	inline static std::unordered_map<SettingsType, std::list<SettingsSubscriber*>> m_SubscriberMap;
};
