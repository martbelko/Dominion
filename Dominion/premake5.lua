project "Dominion"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	pchheader "dmpch.h"
	pchsource "src/dmpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"vendor/stb_image",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.PhysX}/physx/include",
		"%{IncludeDir.PhysX}/physx/source/physxextensions/src",
		"%{IncludeDir.PhysX}/pxshared/include",
		"%{IncludeDir.PhysX}/physx/source/foundation/include"
	}

	links
	{
		"GLFW",
		"opengl32.lib",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"assimp",
		"PhysX",
		"PhysXCharacterKinematic",
		"PhysXCommon",
		-- "PhysXCooking",
		"PhysXExtensions",
		"PhysXFoundation",
		"PhysXPvdSDK",
		"PhysXVehicle",
		"LowLevelDynamics"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"PX_PHYSX_STATIC_LIB"
	}
