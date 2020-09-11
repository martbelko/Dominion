workspace "Dominion"
    platforms { "x32", "x64" }

    startproject "Sandbox"

    filter "platforms:x32"
        architecture "x86"
        defines
        {
            "DM_X86"
        }
    filter "platforms:x64"
        architecture "x86_64"
        defines
        {
            "DM_X64"
        }
    
    filter "system:windows"
    systemversion "latest"
    defines
    {
        "DM_WINDOWS"
    }

    filter "configurations:Debug"
        defines "DM_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "DM_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "DM_DIST"
        runtime "Release"
        optimize "on"

    filter {}

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dominion"
include "Sandbox"
