workspace "Yolo!"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "DebugOptOn",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}-%{cfg.system}"

IncludeDir = {}

-- IncludeDir["HeaderLibs"]    = "Ocean/Source/Thirdparty/Headers"

project "Yolo!"
    location "Yolo!"
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
        "LICENSE",
        "readme.md",

        -- Generation scripts

        "CMakeLists.txt",
        "premake5.lua",

        -- Documentation files

        "%{prj.name}/**.txt",

        -- Source files

        "%{prj.name}/**.hpp",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/Source",

        -- "%{IncludeDir.HeaderLibs}"
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
        defines "YOO_MODE_RELEASE"
        runtime "Release"
        symbols "Off"
        optimize "On"
