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
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
		"%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "opengl32.lib",
        "Glad"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }
