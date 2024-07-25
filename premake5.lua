PROJECT_NAME = "Calliterra"

workspace (PROJECT_NAME)
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Distribution"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["ImGui"] = PROJECT_NAME .. "/vendor/imgui"

include (PROJECT_NAME .. "/vendor/imgui")

project (PROJECT_NAME)
    location "Calliterra"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "%{prj.name}/src/pch.cpp"

    defines
    {
        "_CONSOLE"
    }

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.ImGui}"
    }

    links
    {
        "ImGui"
    }

    filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
        flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "_DEBUG", "DEBUG" }
        kind "ConsoleApp"
        runtime "Debug"
        symbols "On"
        optimize "Debug"


    filter "configurations:Release"
        defines { "NDEBUG" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Distribution"
        defines { "NDEBUG" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        runtime "Release"
        symbols "Off"
        optimize "Full"