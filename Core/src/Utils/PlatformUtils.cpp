#include "pch.h"
#include "PlatformUtils.h"

#include "Application.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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

void Win32Menu::OpenMenu()
{
	HMENU hMenu = CreateMenu();

	AppendMenu(hMenu, MF_STRING, NULL, L"File");
	AppendMenu(hMenu, MF_STRING, NULL, L"Edit");

	SetMenu(glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()), hMenu);
}
