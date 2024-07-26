#pragma once

enum class SettingsType
{
	None = 0,
	IsWireFrame,
	CullMode,
	Fov

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
		enum CullMode
		{
			CullNone = 0,
			CullFront = 1,
			CullBack = 2
		};

		static bool IsWireFrame() { return m_IsWireFrame; }
		static int CullType() { return m_CullType; }
	private:
		inline static bool m_IsWireFrame = false;
		inline static int m_CullType = CullBack;
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

	//static bool IsWireFrame() { return Rendering::m_IsWireFrame; }
	//static bool CullType() { return Rendering::m_CullType; }
	//static int Fov() { return Camera::m_Fov; }

private:
	inline static std::unordered_map<SettingsType, std::list<SettingsSubscriber*>> m_SubscriberMap;
};
