#pragma once

#include "ImGUILayer.h"
#include "Layer.h"
#include "Utils/PlatformUtils.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Math/Mathematics.h"

class DebugLayer:public Layer
{
public:
	DebugLayer()
		:Layer("Debug Layer") {
		values[0] = 0; values[1] = 0; result = 0;
	}

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

		ImGui::DragInt2("", values, 1, -100,100);
		ImGui::SameLine();
		if(ImGui::Button("Generate Number"))
		{
			result = Math::RandomInt(values[0], values[1]);
		}

		ImGui::Text("%d", result);
		ImGui::End();
	}

private:
	int values[2], result;
};
