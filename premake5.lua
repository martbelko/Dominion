include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Dominion"
    architecture "x86_64"

    startproject "Sandbox"

    solution_items
	{
		".editorconfig"
	}

    defines
    {
        "DM_ARCH_X64",
		"_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
		systemversion "latest"
		defines
		{
			"DM_PLATFORM_WINDOWS"
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
    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    IncludeDir = {}
    IncludeDir["GLFW"] = "%{wks.location}/Dominion/vendor/GLFW/include"
    IncludeDir["Glad"] = "%{wks.location}/Dominion/vendor/Glad/include"
	IncludeDir["glm"] = "%{wks.location}/Dominion/vendor/glm"
	IncludeDir["ImGui"] = "%{wks.location}/Dominion/vendor/ImGui"

    group "Dependencies"
        include "vendor/premake"
	    include "Dominion/vendor/GLFW"
        include "Dominion/vendor/Glad"
		include "Dominion/vendor/ImGui"
    group ""

    include "Dominion"
    include "Sandbox"
