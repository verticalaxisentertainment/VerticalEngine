#pragma once

#include "Layer/ImGUILayer.h"
#include "Layer/Layer.h"
#include "Utils/PlatformUtils.h"
#include "Math/Mathematics.h"
#include "UUID.h"
#include "GameLayer.h"
#include "Renderer/Shader.h"
#include "TestLayer.h"

#include <imgui.h>
#include "ImGuizmo/ImGuizmo.h"

class DebugLayer :public Layer
{
public:
	DebugLayer()
		:Layer("Debug Layer")
	{
		result = 0;
		values[0] = 0; values[1] = 0;
	}
	
	void OnAttach()
	{
		scenes.push_back(std::make_shared<Scene>("deneme"));
		scenes.push_back(std::make_shared<Scene>("test"));
		scenes.push_back(std::make_shared<Scene>("Sandbox"));
		scenes.push_back(std::make_shared<Scene>("zaa"));
		scenes.push_back(std::make_shared<Scene>("efe"));

	}

	void OnImGuiRender()
	{
		static bool show = true;
		static bool vsync = true;
		Application& app = Application::Get();

		ImGui::Begin("Debug", &show);

		if (ImGui::Button("Scene"))
			ImGui::OpenPopup("scene_popup");
		ImGui::SameLine();
		ImGui::TextUnformatted(selectedScene == -1 ? "<None>" : scenes[selectedScene]->m_SceneName.c_str());
		if (ImGui::BeginPopup("scene_popup"))
		{
			ImGui::Text("Game");
			ImGui::Separator();
			for (int i = 0; i < scenes.size(); i++)
				if (ImGui::Selectable(scenes[i]->m_SceneName.c_str()))
				{
					selectedScene = i;
					std::string temp = scenes[i]->m_SceneName;
					scenes[i].reset(new Scene(temp));
					SceneSerializer::Read(scenes[i]->m_SceneName, scenes[i]);
				}
			ImGui::EndPopup();
		}

		ImGui::Checkbox("ShowDemoWindow", &ImGUILayer::show);
		ImGui::Checkbox("Show Stats", &showStats);
		ImGui::Checkbox("Post Process", &Application::showPostProcessing);
		ImGui::Checkbox("VSYNC", &vsync);
		//ImGui::Checkbox("Lock Camera", &GameLayer::m_LockCamera);
		Application::Get().GetWindow().SetVSync(vsync);
		if (ImGui::Button("Texture"))
		{
			std::string path = FileDialogs::OpenFile(".jpg");
			if (!path.empty())
			{
				//GameLayer::m_Texture->UpdateTexture(path);
			}
		}
		if (ImGui::Button("Clear PhysicObjects"))
		{
		}
		if (ImGui::Button("ReCompile Shaders"))
		{
			Shader::ReCompileShaders();
		}
		//ImGui::DragInt2("Tiles", GameLayer::tiles, 0.5f, 0, 100);
		ImGui::InputText("Text", GameLayer::m_Text, IM_ARRAYSIZE(TestLayer::m_Text));
		if (ImGui::IsItemHovered())
		{
			app.GetWindow().SetCursor(Cursor::IBEAM);
		}

		ImGuiIO& io = ImGui::GetIO();

		ImGuizmo::SetOrthographic(true);

		ImGuizmo::SetDrawlist();
		
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		//ImGuizmo::Manipulate(glm::value_ptr(GameLayer::m_CameraController->GetCamera().GetViewMatrix()), glm::value_ptr(GameLayer::m_CameraController->GetCamera().GetProjectionMatrix()), ImGuizmo::TRANSLATE, ImGuizmo::WORLD,const_cast<float*>(glm::value_ptr(glm::mat4(1.0f))), NULL, NULL, NULL, NULL);


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

	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		GameLayer::m_Move = false;
	}


	inline static Layer* GetActiveScene() { return activeScene; }
private:
	int values[2], result;
    inline static bool showStats = true;
    id::UUID uuid;
	inline static std::vector<std::shared_ptr<Scene>> scenes;
	inline static int selectedScene = 0;
	static Layer* activeScene;

	friend class SceneInit;
};

Layer* DebugLayer::activeScene;