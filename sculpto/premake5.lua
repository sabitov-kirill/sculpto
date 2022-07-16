project "sculpto"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "%{prj.name}")

    pchheader "sclpch.h"
    pchsource "src/sclpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "external/stb_image/stb_image.h",
        "external/stb_image/stb_image.cpp",
    }

    includedirs
    {
        "src",
        "%{IncludeDir.glad}",
        "%{IncludeDir.glew}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.assimp}",
    }

    links
    {
        "glad",
        "glew",
        "imgui",
        "assimp",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"
        characterset ("MBCS")

    filter "configurations:Debug"
        defines { "SCL_DEBUG", "SCL_DEBUG_MEMORY_ENABLED", "SCL_ASSERTION_ENABLED" }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "SCL_RELEASE"
        optimize "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "SCL_DIST"
        optimize "On"
        runtime "Release"