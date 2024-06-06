#pragma once

#include "Event.h"

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_KeyCode; }

	int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }

protected:
	KeyEvent(KeyCode keycode)
		: m_KeyCode(keycode)
	{
	}

	KeyCode m_KeyCode;

};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(KeyCode keycode, int repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) 
	{
	}

	inline int GetRepeatCount() const { return m_RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << KeyCodeToString(m_KeyCode) << " (" << m_RepeatCount << " repeats)";
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::KeyPressed; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "KeyPressed"; }

private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(KeyCode keycode)
		: KeyEvent(keycode)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << KeyCodeToString(m_KeyCode);
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::KeyReleased; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "KeyReleased"; }

};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(KeyCode keycode)
		: KeyEvent(keycode)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::KeyTyped; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "KeyTyped"; }
};
