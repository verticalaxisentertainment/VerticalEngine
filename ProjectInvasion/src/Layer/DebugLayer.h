#pragma once

#include "ImGUILayer.h"
#include "Layer.h"
#include "Utils/PlatformUtils.h"

class DebugLayer:public Layer
{
public:
	DebugLayer()
		:Layer("Debug Layer") {}
	void OnImGuiRender() override
	{
		static bool show = true;
		static bool vsync = true;
		Application& app = Application::Get();
		ImGui::Begin("Debug", &show);
		ImGui::Checkbox("Post Process", &app.showPostProcessing);
		ImGui::Checkbox("VSYNC", &vsync);
		Application::Get().GetWindow().SetVSync(vsync);
		if(ImGui::Button("Texture"))
		{
			std::string path=FileDialogs::OpenFile(".jpg");
			if(!path.empty())
			{
				GameLayer::m_Texture->UpdateTexture(path);
			}
		}
		ImGui::End();
	}
};
