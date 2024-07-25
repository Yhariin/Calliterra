#pragma once

enum class SettingsType
{
	None = 0,
	IsWireFrame,
	CullMode,
	Fov

};

class SettingsSubscriber
{
public:
	virtual ~SettingsSubscriber() = default;
	virtual void OnSettingsUpdate(SettingsType) = 0;
};


enum class SettingsCategory
{
	None = 0,
	Rendering,
	Camera
};


class GlobalSettings
{
public:
	struct Rendering
	{
		enum CullMode
		{
			CullNone = 0,
			CullFront = 1,
			CullBack = 2
		};

		inline static bool IsWireFrame;
		inline static int CullType;
	};

	struct Camera
	{
		inline static int Fov;
	};

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
