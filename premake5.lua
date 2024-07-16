workspace "Calliterra"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Calliterra"
    location "Calliterra"
    kind "ConsoleApp"
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
    }

    links
    {
    }

    filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
        flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "_DEBUG", "DEBUG" }
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        runtime "Release"
