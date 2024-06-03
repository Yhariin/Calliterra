#pragma once
#include "pch.h"

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication = 1 << 0,
	EventCategoryInput = 1 << 2,
	EventCategoryKeyboard = 1 << 3,
	EventCategoryMouse = 1 << 4,
	EventCategoryMouseButton = 1 << 5
};

class Event
{
public:
	bool Handled = false;
	
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(EventCategory category) const
	{
		return GetCategoryFlags() & category;
	}

	friend std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_Event(event)
	{
	}

	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled = func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};
