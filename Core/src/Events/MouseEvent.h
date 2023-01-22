#pragma once

#include "Event.h"
#include <sstream>

class MouseMovedEvent:public Event
{
public:
	MouseMovedEvent(float x,float y)
		:m_MouseX(x),m_MouseY(y){}

	inline float GetX() const { return m_MouseX; }
	inline float GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	//EVENT_CLASS_TYPE(MouseMoved);
	static EventType GetStaticType() { return EventType::MouseMoved; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "MouseMoved"; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
private:
	float m_MouseX, m_MouseY;
};

class MouseScrolledEvent :public Event
{
public:
	MouseScrolledEvent(float xOffset, float yOffset)
		:m_xOffset(xOffset), m_yOffset(yOffset) {}

	inline float GetXOffset() const { return m_xOffset; }
	inline float GetYOffset() const { return m_yOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	//EVENT_CLASS_TYPE(MouseScrolled);
	static EventType GetStaticType() { return EventType::MouseScrolled; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "MouseScrolled"; }
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
private:
	float m_xOffset, m_yOffset;
};

class MouseButtonEvent:public Event
{
public:
	inline int GetMouseButton() const { return m_Button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput);
protected:
	MouseButtonEvent(int button)
		:m_Button(button){}

	int m_Button;
};

class MouseButtonPressedEvent:public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button)
		:MouseButtonEvent(button){}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	//EVENT_CLASS_TYPE(MouseButtonPressed);

	static EventType GetStaticType() { return EventType::MouseButtonPressed; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "MouseButtonPressed"; }
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button)
		:MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	//EVENT_CLASS_TYPE(MouseButtonReleased);
	static EventType GetStaticType() { return EventType::MouseButtonReleased; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "MouseButtonReleased"; }
};
