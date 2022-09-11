#pragma once
#include "Core.h"

class _API FileDialogs
{
public:

	static std::string OpenFile(const char* filter);
	static std::string SaveFile(const char* filter);
};

class Win32Menu
{
public:
	static void OpenMenu();
};
