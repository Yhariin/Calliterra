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
IncludeDir["fastgltf"] = PROJECT_NAME .. "/vendor/fastgltf/include"

include (PROJECT_NAME .. "/vendor/imgui")
include (PROJECT_NAME .. "/vendor/fastgltf")

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
        "%{prj.name}/src/**.h",
        "%{prj.name}/vendor/ufbx/ufbx.c"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/rapidobj/include",
        "%{prj.name}/vendor/ufbx/",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.fastgltf}"
    }

    links
    {
        "ImGui",
        "fastgltf"
    }

    filter "files:**.c"
        flags {"NoPCH"}

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
        defines { "RELEASE" }
        kind "ConsoleApp"
        runtime "Debug"
        symbols "On"
        optimize "On"

    filter "configurations:Distribution"
        defines { "NDEBUG", "DISTRIBUTION" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        runtime "Release"
        symbols "Off"
        optimize "Full"
        postbuildcommands { "{COPYDIR} %[%{prj.location}/assets] %[bin/%{outputdir}/%{PROJECT_NAME}/assets]" }