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

// We use an unscoped enum here to allow for implicit integer conversion for bitmasking
enum EventCategory
{
	None = 0,
	EventCategoryApplication = 1 << 0,
	EventCategoryInput = 1 << 1,
	EventCategoryKeyboard = 1 << 2,
	EventCategoryMouse = 1 << 3,
	EventCategoryMouseButton = 1 << 4
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
			m_Event.Handled |= func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};
