#pragma once

#include "ImGUILayer.h"
#include "Layer.h"

class DebugLayer:public Layer
{
public:
	DebugLayer()
		:Layer("Debug Layer") {}
	void OnImGuiRender() override
	{
		Application& app = Application::Get();
		static bool show = true;
		ImGui::Begin("Debug window", &show);
		ImGui::Checkbox("Post Process", &app.showPostProcessing);
		ImGui::End();
	}
};
