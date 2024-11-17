#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>

class WindowsWindow :public Window
{
public:
	WindowsWindow(const WindowProps& props);
	virtual ~WindowsWindow();

	void OnUpdate() override;
	void OnEvent(Event& e);

	inline unsigned int GetWidth() const override { return m_Data.Width; }
	inline unsigned int GetHeight() const override { return m_Data.Height; }
	inline unsigned int GetAspectRatio() const override { return m_Data.Width / m_Data.Height; }

	inline void* GetNativeWindow() override { return m_Window; }
	virtual std::string& GetTitle() override { return m_Title; }

	virtual void SetCursor(Cursor cursor);
	virtual void SetPos(float x, float y);

	virtual void SetCurrentContext(void* window) override;
	inline void SetEventCallBack(const EventCallbakcFn& callback) override { m_Data.EventCallback = callback; }
	inline virtual EventCallbakcFn GetEventCallBack() override { return m_Data.EventCallback; }

	void SetVSync(bool enabled) override;
	bool IsVSync() const override;
private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();
private:
	GLFWwindow* m_Window;
	std::string m_Title;

	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbakcFn EventCallback;
	};

	GLFWcursor* m_Cursor;
	WindowData m_Data;
};