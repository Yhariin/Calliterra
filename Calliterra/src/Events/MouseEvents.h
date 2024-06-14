#pragma once

#include "Event.h"
#include "Core/KeyCodes.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(int x, int y)
		: m_MouseX(x), m_MouseY(y)
	{
	}

	int GetX() const { return m_MouseX; }
	int GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::MouseMoved; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "MouseMoved"; }
	int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

private:
	int m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float xOffset, float yOffset)
		: m_XOffset(xOffset), m_YOffset(yOffset)
	{
	}

	float GetXOffset() const { return m_XOffset; }
	float GetYOffset() const { return m_YOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::MouseScrolled; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "MouseScrolled"; }
	int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

private:
	float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
	// Might need to change to KeyCode type
	KeyCode GetMouseButton() const { return m_Button;  }

	int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
protected:
	MouseButtonEvent(KeyCode button)
		: m_Button(button)
	{
	}

	KeyCode m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(KeyCode button)
		: MouseButtonEvent(button)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << KeyCodeToString(m_Button);
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::MouseButtonPressed; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "MouseButtonPressed"; }

};

class MouseButtonReleased : public MouseButtonEvent
{
public:
	MouseButtonReleased(KeyCode button)
		: MouseButtonEvent(button)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << KeyCodeToString(m_Button);
		return ss.str();
	}

	static EventType GetStaticType() { return EventType::MouseButtonReleased; }
	EventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "MouseButtonReleased"; }
	
};
