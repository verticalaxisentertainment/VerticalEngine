#pragma once

#include "Core.h"
#include <spdlog/spdlog.h>

#include "spdlog/sinks/stdout_color_sinks.h"


class _API Log
{
public:
	static void Init()
	{
		m_Logger = spdlog::stderr_color_st("stderr");
		m_Logger->set_pattern("[%T][%^%l%$] %v");
	}

	static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

private:
	inline static std::shared_ptr<spdlog::logger> m_Logger;
};

#ifdef PLATFORM_WINDOWS
	#define INFO(...) Log::GetLogger()->info(__VA_ARGS__)
	#define ERROR(...) Log::GetLogger()->error(__VA_ARGS__)

	//#define ERROR(x) SetConsoleTextAttribute(hcc,0x07);std::cout<<"[";SetConsoleTextAttribute(hcc,4);std::cout<<"error";SetConsoleTextAttribute(hcc,0x07);std::cout<<"] "<<x<<"\n"
	//#define INFO(x)SetConsoleTextAttribute(hcc,0x07);std::cout<<"[";SetConsoleTextAttribute(hcc,1);std::cout<<"info";SetConsoleTextAttribute(hcc,0x07);std::cout<<"] "<<x<<"\n"
	#define WARNING(x)SetConsoleTextAttribute(hcc,0x07);std::cout<<"[";SetConsoleTextAttribute(hcc,6);std::cout<<"warning";SetConsoleTextAttribute(hcc,0x07);std::cout<<"] "<<x<<"\n"

#endif
