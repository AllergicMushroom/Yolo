workspace "Yolo"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "DebugOptOn",
        "Release"
    }

    startproject "Yolo"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"
libdir = "Libraries"

includedir = {}
includedir["spdlog"] = "Libraries/spdlog-1.8.2/include"
includedir["googletest"] = "Libraries/googletest-1.10.0/include"

sourcedir = "Yolo/Source"

group "Dependencies"
    include "Libraries/spdlog-1.8.2"
    include "Libraries/googletest-1.10.0"

group ""

project "Yolo"
    location "Yolo"
    kind "ConsoleApp"
    staticruntime "On"

    language "C++"
    cppdialect "C++17"

    floatingpoint "Fast"

    warnings "Extra"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    debugdir ("%{cfg.targetdir}")

    files
    {
        -- General files
        
        ".gitignore",
        ".clang-format",
        "LICENSE",
        "readme.md",

        -- Generation scripts

        "CMakeLists.txt",
        "premake5.lua",

        -- Source files

        "%{sourcedir}/Core/**.hpp",
        "%{sourcedir}/Core/**.cpp",
        "%{sourcedir}/Domain/**.hpp",
        "%{sourcedir}/Domain/**.cpp",
        "%{sourcedir}/Technical/**.hpp",
        "%{sourcedir}/Technical/**.cpp",

        "%{sourcedir}/UserInterface/Sandbox.cpp",
    }

    includedirs
    {
        "%{sourcedir}",

        "%{includedir.spdlog}",
    }

    links
    {
        "spdlog",
    }

    postbuildcommands
    {
        -- postbuildcommands starts in bin directory so we have to cd ..
        "{copy} ../Instances %{cfg.targetdir}/Instances"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "YOLO_MODE_DEBUG"
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:DebugOptOn"
        defines "YOLO_MODE_DEBUGOPTON"
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Release"
        defines "YOLO_MODE_RELEASE"
        runtime "Release"
        symbols "Off"
        optimize "On"

project "YoloUnitTests"
    location "Yolo"
    kind "ConsoleApp"
    staticruntime "On"

    language "C++"
    cppdialect "C++17"

    floatingpoint "Fast"

    warnings "Extra"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    debugdir ("%{cfg.targetdir}")

    files
    {
        -- Source files

        "%{sourcedir}/Core/**.hpp",
        "%{sourcedir}/Core/**.cpp",
        "%{sourcedir}/Domain/**.hpp",
        "%{sourcedir}/Domain/**.cpp",
        "%{sourcedir}/Technical/**.hpp",
        "%{sourcedir}/Technical/**.cpp",

        "%{sourcedir}/UnitTests/**.hpp",
        "%{sourcedir}/UnitTests/**.cpp",
    }

    includedirs
    {
        "%{sourcedir}",

        "%{includedir.spdlog}",
        "%{includedir.googletest}",
    }

    links
    {
        "spdlog",
        "googletest",
    }

    postbuildcommands
    {
        -- postbuildcommands starts in bin directory so we have to cd ..
        "{copy} ../Instances %{cfg.targetdir}/Instances"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "YOLO_MODE_DEBUG"
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:DebugOptOn"
        defines "YOLO_MODE_DEBUGOPTON"
        runtime "Release"
        symbols "On"
        optimize "On"

    filter "configurations:Release"
        defines "YOLO_MODE_RELEASE"
        runtime "Release"
        symbols "Off"
        optimize "On"

