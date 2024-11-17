VULKAN_SDK = os.getenv("VULKAN_SDK")

project "optick"
    kind "StaticLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    IncludeDir["VulkanSdk"]="%{VULKAN_SDK}/Include"

    files
    {
        "src/**.cpp",
        "src/**.h"
    }

    includedirs
    {
        "include",
        "%{IncludeDir.VulkanSdk}"
    }

    filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

        filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
