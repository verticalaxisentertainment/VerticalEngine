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
IncludeDir["GLFW"]="Core/vendor/GLFW/include"
IncludeDir["Glad"]="Core/vendor/Glad/include"
IncludeDir["imgui"]="Core/vendor/imgui"
IncludeDir["glm"]="Core/vendor/glm"
IncludeDir["stb"]="Core/vendor/stb"
IncludeDir["freetype"]="Core/vendor/freetype/include"
IncludeDir["box2d"]="Core/vendor/box2d/include"
IncludeDir["entt"]="Core/vendor/entt/include"

group "Dependencies"
	include "Core/vendor/GLFW"
	include "Core/vendor/Glad"
	include "Core/vendor/imgui"
	include "Core/vendor/box2d"
	include "Core/vendor/freetype"

group ""

project "Core"
	location "Core"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .."/%{prj.name}")
	objdir("bin-int/" .. outputdir .."/%{prj.name}")

	pchheader "pch.h"
	pchsource "Core/src/pch.cpp"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{IncludeDir.stb}/**.h",
		"%{IncludeDir.stb}/**.cpp",
	}

	includedirs{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{prj.name}/src",
		"%{IncludeDir.imgui}/backends",
		"%{IncludeDir.box2d}",
		"%{IncludeDir.freetype}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32",
		"box2d",
		"freetype",
	}

	
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir .."/ProjectInvasion/\"")
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"GLFW_INCLUDE_NONE",
			"PLATFORM_WINDOWS",
			"DLL_BUILD",
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


project "ProjectInvasion"
	location "ProjectInvasion"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir("bin/" .. outputdir .."/%{prj.name}")
	objdir("bin-int/" .. outputdir .."/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs{
		"Core/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Core"
	}


	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines{
			"GLFW_INCLUDE_NONE",
			"PLATFORM_WINDOWS",
			"USE_IMGUI_API",
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