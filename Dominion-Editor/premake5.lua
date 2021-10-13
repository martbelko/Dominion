project "Dominion-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

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
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.PhysX}/physx/include",
		"%{IncludeDir.PhysX}/physx/source/physxextensions/src",
		"%{IncludeDir.PhysX}/pxshared/include",
		"%{IncludeDir.PhysX}/physx/source/foundation/include"
	}

	links
	{
		"Dominion"
	}
