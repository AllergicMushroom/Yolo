project "googletest"
    kind "StaticLib"
    language "C++"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/gtest.cc",

        "src/gtest-death-test.cc",
        "src/gtest-filepath.cc",
        "src/gtest-internal-inl.h",
        "src/gtest-matchers.cc",
        "src/gtest-port.cc",
        "src/gtest-printers.cc",
        "src/gtest-test-part.cc",
        "src/gtest-typed-test.cc",

        "src/gtest_main.cc",
    }

    includedirs
    {
        "include",
        ".",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:DebugOptOn"
        runtime "Release"
        symbols "On"
        optimize "On"
        
    filter "configurations:Release"
        runtime "Release"
        symbols "Off"
        optimize "On"
