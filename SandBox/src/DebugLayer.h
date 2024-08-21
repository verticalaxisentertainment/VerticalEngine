#pragma once

#include "GameLayer.h"
#include "DLL.h"

#include <imgui.h>
#include "ImGuizmo/ImGuizmo.h"
#define BIND_EVENT_FN(x) std::bind(&x,this,std::placeholders::_1)


class DebugLayer :public Layer
{
public:
	DebugLayer()
		:Layer("Debug Layer")
	{
	}
	
	void OnAttach()
	{
		scenes.push_back(std::make_shared<Scene>("deneme"));
		scenes.push_back(std::make_shared<Scene>("test"));
		scenes.push_back(std::make_shared<Scene>("Sandbox"));
		scenes.push_back(std::make_shared<Scene>("zaa"));
		scenes.push_back(std::make_shared<Scene>("efe"));

		m_TitleTemp = Application::Get().GetWindow().GetTitle();

	}

	void OnImGuiRender()
	{
		static bool show = true;
		static bool vsync = true;
		Application& app = Application::Get();
		
		ImGui::GetIO().ConfigFlags = 0;

		auto& camPosition = GameLayer::Instance->m_CameraController->GetCamera().GetPosition();
		auto testPos = Math::WorldToScreenPoint({ 0.0f, 5.0f, -0.1f },GameLayer::Instance->m_CameraController->GetCamera().GetViewProjectionMatrix());
		ImGui::SetNextWindowPos({ ((testPos.x + 1) / 2) * app.GetWindow().GetWidth(),((1 - testPos.y) / 2) * app.GetWindow().GetHeight() }, ImGuiCond_Always, ImVec2{ 0.5f,0.5f });
		//ImGui::SetNextWindowSize({ 200.0f,changeSize.y });
		
		ImGui::Begin("Pos Test", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground| ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize);
		size = { ImGui::GetWindowSize().x,ImGui::GetWindowSize().y };
		//ImGui::PushItemWidth(-FLT_MIN);
		ImGui::PushItemWidth(150);
		ImGui::InputText("Text", GameLayer::Instance->m_Text, IM_ARRAYSIZE(GameLayer::Instance->m_Text));
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));
		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize("Click").x;

		//ImGui::SetCursorPosY(0);
		ImGui::SetCursorPosX((windowWidth - 150) * 0.5f);
		if (ImGui::Button("Click",{150,80}))Win32::Assert("UI Test!");
		ImGui::PopStyleColor(3);
		ImGui::PushItemWidth(150);
		ImGui::SliderFloat(" ", &GameLayer::Instance->progress,0.0f,100.0f);
		ImGui::End();


		//ImGui::SetNextWindowPos({ 0,720 }, ImGuiCond_Appearing, { 0,1 });
		ImGui::Begin("Debug", &show);
		ImGui::SetWindowSize({ 338,466 }, ImGuiCond_Appearing);
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
					app.GetWindow().GetTitle() = m_TitleTemp + " | Scene: " + scenes[i]->m_SceneName;

					if (scenes[i]->m_SceneName == "Sandbox")
					{
						m_Quad = scenes[i]->GetEntityWithUUID(3009908);
						m_Picker = scenes[i]->GetEntityWithUUID(8053989);
						m_Circle = scenes[i]->GetEntityWithUUID(6836113);
					}
				}
			ImGui::EndPopup();
		}

		ImGui::Checkbox("ShowDemoWindow", &ImGUILayer::show);
		ImGui::Checkbox("Show Stats", &showStats);
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
		if (ImGui::Button("Assert Test"))
		{
			Win32::Assert("An error has been occured!");
		}
		if (ImGui::Button("Start the SandBox"))
		{
			system("START Sandbox.exe exit");
		}
		if (ImGui::Button("ReCompile Shaders"))
		{
			Shader::ReCompileShaders();
		}
		//ImGui::DragInt2("Tiles", GameLayer::tiles, 0.5f, 0, 100);
		ImGui::DragFloat("Progress", &GameLayer::Instance->progress, 0.05f, 0.0f, 500.0f);
		ImGui::InputText("Text", GameLayer::Instance->m_Text, IM_ARRAYSIZE(GameLayer::Instance->m_Text));
		if (ImGui::IsItemHovered())
		{
			app.GetWindow().SetCursor(Cursor::IBEAM);
		}

		ImGuiIO& io = ImGui::GetIO();

		ImGuizmo::SetOrthographic(true);

		ImGuizmo::SetDrawlist();
		
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		//ImGuizmo::Manipulate(glm::value_ptr(GameLayer::m_CameraController->GetCamera().GetViewMatrix()), glm::value_ptr(GameLayer::m_CameraController->GetCamera().GetProjectionMatrix()), ImGuizmo::TRANSLATE, ImGuizmo::WORLD,const_cast<float*>(glm::value_ptr(glm::mat4(1.0f))), NULL, NULL, NULL, NULL);


		ImGui::Image((void*)GameLayer::Instance->m_FrameBuffer->GetColorAttachmentRendererID(), ImVec2(GameLayer::Instance->m_FrameBuffer->GetSpecification().Width/4, GameLayer::Instance->m_FrameBuffer->GetSpecification().Height/4), ImVec2{ 0,1 }, ImVec2{ 1,0 });
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
				//ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "UUID test: %d", uuid.id());


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
		GameLayer::Instance->m_Move = false;
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(DebugLayer::onMouseScrolled));
	}
	glm::vec2 size = { 0.0f,0.0f };
	glm::vec2 changeSize;

	private:
		bool onMouseScrolled(MouseScrolledEvent& e)
		{
			auto& io=ImGui::GetIO();
			changeSize = { size.x +e.GetYOffset(),size.y };
			return false;
		}

private:
	ImVec2 windowPos;
    inline static bool showStats = true;
	inline static Entity m_Quad, m_Circle, m_Picker;
	inline static std::vector<std::shared_ptr<Scene>> scenes;
	inline static int selectedScene = 0;
	std::string m_TitleTemp;

	friend class SceneInit;
};

