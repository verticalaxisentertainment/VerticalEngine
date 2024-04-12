#include "pch.h"
#include "PlatformUtils.h"

#include "Application.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <CommCtrl.h>

std::string FileDialogs::OpenFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

std::string FileDialogs::SaveFile(const char* filter)
{
	return std::string();
}

void* Win32::CreateProgressBar()
{
	Window* windowNew = Window::Create({ "Loading",450,30,Application::Get().GetWindow().GetNativeWindow() });
	
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	windowNew->SetPos(mode->width / 2 - 225, mode->height / 2 - 15);

	HWND progress;
	progress = ::CreateWindowEx(
		0,
		PROGRESS_CLASS,
		TEXT("Loading"),
		WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_MARQUEE,
		0,
		0,
		450,
		30,
		glfwGetWin32Window((GLFWwindow*)windowNew->GetNativeWindow()),
		(HMENU)402,
		NULL,
		NULL);

		::SendMessage(progress, PBM_SETSTEP, (WPARAM)(INT)2, 0);
		::SendMessage(progress, PBM_SETBARCOLOR, 0, RGB(0, 180, 0));
		auto a= ::SendMessage(progress, PBM_GETPOS, 0, 0);
	while (a!=100)
	{
		::SendMessage(progress, PBM_SETMARQUEE, true, 0);
		::SendMessage(progress, PBM_STEPIT, 0, 0);
		a = ::SendMessage(progress, PBM_GETPOS, 0, 0);
		Sleep(50);
	}
	delete windowNew;
	return windowNew;
}

void Win32::Assert(const std::string& msg)
{
	Application& app = Application::Get();
	std::wstring temp = std::wstring(msg.begin(), msg.end());
	MessageBoxW(glfwGetWin32Window((GLFWwindow*)app.GetWindow().GetNativeWindow()), temp.c_str(), L"Assert", MB_OK | MB_ICONERROR);
}

void Win32::OpenMenu()
{
	HMENU hMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, NULL, L"File");
	AppendMenu(hMenu, MF_STRING, NULL, L"Edit");

	SetMenu(glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()), hMenu);
}
