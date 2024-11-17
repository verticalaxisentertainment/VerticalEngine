#pragma once
#include "Core.h"

class _API FileDialogs
{
public:

	static std::string OpenFile(const char* filter);
	static std::string SaveFile(const char* filter);
};

class _API Win32
{
public:
	static void OpenMenu();
	static void* CreateProgressBar();
	static void Assert(const std::string& msg);
};
