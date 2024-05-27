workspace "ProceduralWorldGenerator"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "ProceduralWorldGenerator"
    location "ProceduralWorldGenerator"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    defines
    {
        "_CONSOLE"
    }

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp"
    }


    filter "system:windows"
		cppdialect "C++20"
		staticruntime "On"
		systemversion "latest"
        flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "_DEBUG" }
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        runtime "Release"
