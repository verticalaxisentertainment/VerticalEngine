#pragma once

#include "Event.h"
#include <sstream>

class WindowResizeEvent :public Event
{
public:
	WindowResizeEvent(unsigned int width,unsigned int height)
		:m_Width(width),m_Height(height){}

	inline unsigned int GetWidth() const { return m_Width; }
	inline unsigned int GetHeight() const { return m_Height; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
		return ss.str();
	}

	//EVENT_CLASS_TYPE(WindowResize);
	static EventType GetStaticType() { return EventType::WindowResize; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "WindowResize"; }
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
private:
	unsigned int m_Width, m_Height;
};

class WindowCloseEvent :public Event
{
public:
	WindowCloseEvent(){}

	//EVENT_CLASS_TYPE(WindowClose);
	static EventType GetStaticType() { return EventType::WindowClose; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "WindowClose"; }
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppTickEvent:public Event
{
public:
	AppTickEvent(){}

	//EVENT_CLASS_TYPE(AppTick);
	static EventType GetStaticType() { return EventType::AppTick; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "AppTick"; }
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppUpdateEvent:public Event
{
public:
	AppUpdateEvent(){}

	//EVENT_CLASS_TYPE(AppUpdate);
	static EventType GetStaticType() { return EventType::AppUpdate; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "AppUpdate"; }
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};

class AppRenderEvent :public Event
{
public:
	AppRenderEvent() {}

	//EVENT_CLASS_TYPE(AppRender);
	static EventType GetStaticType() { return EventType::AppRender; }
	virtual EventType GetEventType() const override { return GetStaticType(); }
	virtual const char* GetName() const override { return "AppRender"; }
	EVENT_CLASS_CATEGORY(EventCategoryApplication);
};
