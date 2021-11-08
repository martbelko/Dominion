project "DominionEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{wks.location}/Dominion/src",
		"%{wks.location}/Dominion/vendor/spdlog/include",
		"%{wks.location}/Dominion/vendor",

		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Dominion"
	}

	filter "configurations:Debug"
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}

	filter "configurations:PGO"
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/shaderc_shared.lib\" \"%{cfg.targetdir}\"",
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/spirv-cross-c-shared.lib\" \"%{cfg.targetdir}\"",
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/SPIRV-Tools-shared.lib\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Release"
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/shaderc_shared.lib\" \"%{cfg.targetdir}\"",
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/spirv-cross-c-shared.lib\" \"%{cfg.targetdir}\"",
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/SPIRV-Tools-shared.lib\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Dist"
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/shaderc_shared.lib\" \"%{cfg.targetdir}\"",
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/spirv-cross-c-shared.lib\" \"%{cfg.targetdir}\"",
			"{COPYDIR} \"%{LibraryDir.VulkanSDK}/SPIRV-Tools-shared.lib\" \"%{cfg.targetdir}\""
		}
