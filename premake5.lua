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
		defines
		{
			"DM_DEBUG"
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines
		{
			"DM_RELEASE"
		}
		runtime "Release"
		optimize "speed"

	filter "configurations:Dist"
		defines
		{
			"DM_DIST"
		}
		runtime "Release"
		optimize "speed"

	filter "configurations:PGO"
		defines
		{
			"DM_PGO"
		}
		runtime "Release"
		optimize "speed"

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
