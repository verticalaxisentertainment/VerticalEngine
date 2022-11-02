#pragma once

#ifdef PLATFORM_WINDOWS
#ifdef DLL_BUILD
	#define _API __declspec(dllexport)
#else
	#define _API __declspec(dllimport)
#endif
#endif