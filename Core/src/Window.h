#pragma once

#include "pch.h"
#include "Events/Event.h"

enum class Cursor
{
	ARROW, HAND
};

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;

	WindowProps(const std::string& title="Project Invasion",
				unsigned int width=1200,
				unsigned int height=720)
		:Title(title),Width(width),Height(height){}
};


class Window
{
public:
	using EventCallbakcFn = std::function<void(Event&)>;

	virtual ~Window(){}

	virtual void OnUpdate() = 0;


	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual void SetCursor(Cursor cursor) = 0;

	virtual void* GetNativeWindow() = 0;

	virtual void SetEventCallBack(const EventCallbakcFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	static Window* Create(const WindowProps& props = WindowProps());
};
