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
IncludeDir["assimp"] = PROJECT_NAME .. "/vendor/assimp/include"

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
        "_CONSOLE",
        "UFBX_REAL_IS_FLOAT"
    }

    files
    {
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.h",
        "%{prj.name}/vendor/ufbx/ufbx.c",
        "%{prj.name}/vendor/stb/stb_image.c"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/rapidobj/include",
        "%{prj.name}/vendor/ufbx/",
        "%{prj.name}/vendor/stb/",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.fastgltf}",
        "%{IncludeDir.assimp}"
    }

    libdirs
    {
        "%{wks.location}/Calliterra/vendor/assimp/bin/%{cfg.buildcfg}/"    
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
        links { "assimp-vc143-mtd"}
        postbuildcommands { "{COPYDIR} %[%{wks.location}Calliterra/vendor/assimp/bin/%{cfg.buildcfg}/assimp-vc143-mtd.dll] %[bin/%{outputdir}/%{PROJECT_NAME}]"}


    filter "configurations:Release"
        defines { "RELEASE" }
        kind "ConsoleApp"
        runtime "Debug"
        symbols "On"
        optimize "On"
        links { "assimp-vc143-mt"}
        postbuildcommands { "{COPYDIR} %[%{wks.location}Calliterra/vendor/assimp/bin/%{cfg.buildcfg}/assimp-vc143-mt.dll] %[bin/%{outputdir}/%{PROJECT_NAME}]"}

    filter "configurations:Distribution"
        defines { "NDEBUG", "DISTRIBUTION" }
        kind "WindowedApp"
        entrypoint "mainCRTStartup"
        runtime "Release"
        symbols "Off"
        optimize "Full"
        links { "assimp-vc143-mt"}
        postbuildcommands { "{COPYDIR} %[%{wks.location}Calliterra/vendor/assimp/bin/%{cfg.buildcfg}/assimp-vc143-mt.dll] %[bin/%{outputdir}/%{PROJECT_NAME}]"}
        postbuildcommands { "{COPYDIR} %[%{prj.location}/assets] %[bin/%{outputdir}/%{PROJECT_NAME}/assets]" }