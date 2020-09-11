workspace "Dominion"
    platforms { "x32", "x64" }

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
