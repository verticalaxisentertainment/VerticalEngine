workspace "ProjectInvasion"
	architecture "x64"
	startproject "ProjectInvasion"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir =	"%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir={}
IncludeDir["GLFW"]="ProjectInvasion/vendor/GLFW/include"
IncludeDir["Glad"]="ProjectInvasion/vendor/Glad/include"
IncludeDir["imgui"]="ProjectInvasion/vendor/imgui"
IncludeDir["glm"]="ProjectInvasion/vendor/glm"
IncludeDir["stb"]="ProjectInvasion/vendor/stb"

group "Dependencies"
	include "ProjectInvasion/vendor/GLFW"
	include "ProjectInvasion/vendor/Glad"
	include "ProjectInvasion/vendor/imgui"

group ""

project "ProjectInvasion"
	location "ProjectInvasion"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .."/%{prj.name}")
	objdir("bin-int/" .. outputdir .."/%{prj.name}")

	pchheader "pch.h"
	pchsource "ProjectInvasion/src/pch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.stb}/**.h",
		"%{IncludeDir.stb}/**.cpp"
	}

	includedirs{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{prj.name}/src",
		"%{IncludeDir.imgui}/backends"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib"
	}


	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"GLFW_INCLUDE_NONE",
			"PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "RELEASE"
			runtime "Release"
			optimize "On"

		filter "configurations:Dist"
			defines "DIST"
			runtime "Release"
			optimize "On"