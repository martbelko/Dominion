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
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "Glad",
		"ImGui"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }
