#pragma once

class FileDialogs
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
