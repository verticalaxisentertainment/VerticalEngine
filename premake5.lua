workspace "VerticalEngine"
	architecture "x64"
	startproject "SandBox"
	configurations{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir =	"%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir={}
IncludeDir["GLFW"]="Core/vendor/glfw/include"
IncludeDir["Glad"]="Core/vendor/Glad/include"
IncludeDir["imgui"]="Core/vendor/imgui"
IncludeDir["glm"]="Core/vendor/glm"
IncludeDir["stb"]="Core/vendor/stb"
IncludeDir["freetype"]="Core/vendor/freetype/include"
IncludeDir["box2d"]="Core/vendor/box2d/include"
IncludeDir["entt"]="Core/vendor/entt/include"
IncludeDir["optick"]="Core/vendor/optick/include"
IncludeDir["spdlog"]="Core/vendor/spdlog/include"
IncludeDir["yaml"]="Core/vendor/yaml-cpp/include"

group "Dependencies"
	include "Core/vendor/glfw"
	include "Core/vendor/Glad"
	include "Core/vendor/imgui"
	include "Core/vendor/box2d"
	include "Core/vendor/freetype"
	-- include "Core/vendor/optick"
	include "Core/vendor/yaml-cpp"

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
		"%{IncludeDir.entt}",
		-- "%{IncludeDir.optick}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.yaml}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32",
		"box2d",
		"freetype",
		-- "optick",
		"yaml-cpp"
	}

	
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir .."/VerticalEngine/\""),
		("{COPY} %{cfg.buildtarget.relpath} \"../bin/" ..outputdir .."/SandBox/\"")
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


project "VerticalEngine"
	location "VerticalEngine"
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.spdlog}"
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



project "SandBox"
location "SandBox"
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
	"%{IncludeDir.entt}",
	"%{IncludeDir.spdlog}"
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