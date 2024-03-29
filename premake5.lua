include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Dominion"
	architecture "x86_64"
	startproject "DominionEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist",
		"PGO"
	}

	flags
	{
		"MultiProcessorCompile",
		"ShadowedVariables"
	}

	solution_items
	{
		".editorconfig"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		defines
		{
			"DM_DEBUG"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "speed"
		defines
		{
			"DM_RELEASE"
		}
		flags
		{
			"LinkTimeOptimization"
		}

	filter "configurations:Dist"
		runtime "Release"
		optimize "speed"
		defines
		{
			"DM_DIST"
		}
		flags
		{
			"LinkTimeOptimization"
		}

	filter "configurations:PGO"
		runtime "Release"
		optimize "speed"
		defines
		{
			"DM_PGO"
		}
		flags
		{
			"LinkTimeOptimization"
		}

	filter {}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	group "Dependencies"
		include "vendor/premake"
		include "Dominion/vendor/GLFW"
		include "Dominion/vendor/Glad"
		include "Dominion/vendor/ImGui"
		include "Dominion/vendor/yaml-cpp"
	group ""

	include "Dominion"
	include "Sandbox"
	include "DominionEditor"
