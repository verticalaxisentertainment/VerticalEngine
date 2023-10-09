#include "pch.h"
#include "ImGUILayer.h"

#include "Application.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "imgui_internal.h"

#include "ImGuizmo/ImGuizmo.h"

ImGUILayer::ImGUILayer()
	:Layer("ImGuiLayer")
{
}

ImGUILayer::~ImGUILayer()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGUILayer::OnAttach()
{
	IMGUI_CHECKVERSION();
	ImGuiContext* context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 5.0f;
		style.FrameRounding = 5.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	Application& app = Application::Get();
	GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGUILayer::OnDetach()
{
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void ImGUILayer::OnImGuiRender()
{
	if(show)
		ImGui::ShowDemoWindow(&show);
}

void ImGUILayer::OnEvent(Event& e)
{
	ImGuiIO& io = ImGui::GetIO();
	e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
	e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
}

void ImGUILayer::Begin()
{
	ImGui_ImplGlfw_NewFrame();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ImGUILayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::Get();
	io.DisplaySize = ImVec2(800,600);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
