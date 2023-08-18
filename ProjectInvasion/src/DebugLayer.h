#pragma once

#include "Layer/ImGUILayer.h"
#include "Layer/Layer.h"
#include "Utils/PlatformUtils.h"
#include "Math/Mathematics.h"
#include "UUID.h"
#include <imgui.h>
#include "GameLayer.h"
#include "Renderer/Shader.h"
#include "TestLayer.h"

class DebugLayer:public Layer
{
public:
	DebugLayer()
		:Layer("Debug Layer")
	{
		result = 0;
		values[0] = 0; values[1] = 0;
	}

	void OnImGuiRender()
	{
		static bool show = true;
		static bool vsync = true;
		Application& app = Application::Get();
		ImGui::Begin("Debug", &show);
		ImGui::Checkbox("ShowDemoWindow", &ImGUILayer::show);
		ImGui::Checkbox("Show Stats", &showStats);
		ImGui::Checkbox("Post Process", &Application::showPostProcessing);
		ImGui::Checkbox("VSYNC", &vsync);
		ImGui::Checkbox("Lock Camera", &GameLayer::m_LockCamera);
		Application::Get().GetWindow().SetVSync(vsync);
		if (ImGui::Button("Texture"))
		{
			std::string path = FileDialogs::OpenFile(".jpg");
			if (!path.empty())
			{
				GameLayer::m_Texture->UpdateTexture(path);
			}
		}
		if (ImGui::Button("Clear PhysicObjects"))
		{
			Physics::Clear();
		}
		/*{
			ImGui::DragInt2("", values, 1, -100, 100);
			ImGui::SameLine();
			if (ImGui::Button("Generate Number"))
			{
				result = Math::RandomInt(values[0], values[1]);
			}

			ImGui::Text("%d", result);
		}*/
		if (ImGui::Button("ReCompile Shaders"))
		{
			Shader::ReCompileShaders();
		}
		ImGui::DragInt2("Tiles", GameLayer::tiles, 0.5f, 0, 100);
		ImGui::InputText("Text", TestLayer::m_Text, IM_ARRAYSIZE(TestLayer::m_Text));
		
		//ImGui::Text("%f", GameLayer::m_CameraController->GetCamera().GetPosition().x);
		//ImGui::Text("%f", Physics::GetLastObjectsPos().x);


		ImGui::End();
		if (showStats)
		{
			static int corner = 3;
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			if (corner != -1)
			{
				const float PAD = 10.0f;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
				ImVec2 work_size = viewport->WorkSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				ImGui::SetNextWindowViewport(viewport->ID);
				window_flags |= ImGuiWindowFlags_NoMove;
			}
			ImGui::SetNextWindowBgAlpha(0.15f); // Transparent background
			if (ImGui::Begin("Example: Simple overlay", &showStats, window_flags))
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "STATS");
				ImGui::Separator();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Draw Calls: %d", Renderer::GetStats().DrawCalls);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Quad Counts: %d", Renderer::GetStats().QuadCount);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "FPS: %f", ImGui::GetIO().Framerate);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "UUID test: %d", uuid.id());


				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
					if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
					if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
					if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
					if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
					if (&showStats && ImGui::MenuItem("Close")) showStats = false;
					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}
	}

private:
	int values[2], result;
    inline static bool showStats = true;
    id::UUID uuid;
};
