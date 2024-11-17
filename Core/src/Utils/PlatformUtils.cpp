#include "pch.h"
#include "PlatformUtils.h"

#include "Application.h"
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <CommCtrl.h>
#include <string>

// Implementation of FileDialogs::OpenFile
std::string FileDialogs::OpenFile(const char* filter) {
    OPENFILENAMEW ofn;       // common dialog box structure
    wchar_t szFile[260] = { 0 };    // buffer for file name
    wchar_t currentDir[256] = { 0 }; // buffer for current directory
    ZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
    if (GetCurrentDirectoryW(256, currentDir)) {
        ofn.lpstrInitialDir = currentDir;
    }

    // Convert the filter from char* to wchar_t*
    std::wstring wideFilter = std::wstring(filter, filter + strlen(filter));
    ofn.lpstrFilter = wideFilter.c_str();
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameW(&ofn) == TRUE) {
        // Convert the selected file path from wchar_t* to std::string
        std::wstring widePath(ofn.lpstrFile);
        return std::string(widePath.begin(), widePath.end());
    }

    return std::string();
}

// Implementation of FileDialogs::SaveFile
std::string FileDialogs::SaveFile(const char* filter) {
    // Similar implementation as OpenFile with SaveFile logic
    return std::string();
}

void* Win32::CreateProgressBar() {
    Window* windowNew = Window::Create({ "Loading", 450, 30, Application::Get().GetWindow().GetNativeWindow() });

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
    auto a = ::SendMessage(progress, PBM_GETPOS, 0, 0);
    while (a != 100) {
        ::SendMessage(progress, PBM_SETMARQUEE, true, 0);
        ::SendMessage(progress, PBM_STEPIT, 0, 0);
        a = ::SendMessage(progress, PBM_GETPOS, 0, 0);
        Sleep(50);
    }
    delete windowNew;
    return windowNew;
}

void Win32::Assert(const std::string& msg) {
    Application& app = Application::Get();
    std::wstring temp = std::wstring(msg.begin(), msg.end());
    MessageBoxW(glfwGetWin32Window((GLFWwindow*)app.GetWindow().GetNativeWindow()), temp.c_str(), L"Assert", MB_OK | MB_ICONERROR);
}

void Win32::OpenMenu() {
    HMENU hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, NULL, L"File");
    AppendMenuW(hMenu, MF_STRING, NULL, L"Edit");

    SetMenu(glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow()), hMenu);
}
