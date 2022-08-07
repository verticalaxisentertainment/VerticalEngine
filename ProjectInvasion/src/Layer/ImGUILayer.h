#pragma once

#include "Layer.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGUILayer :public Layer
{
public:
	ImGUILayer();
	~ImGUILayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;

	void Begin();
	void End();
private:
	float m_Time;
};
