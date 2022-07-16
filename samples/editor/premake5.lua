project "editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "%{wks.location}/sculpto/src",
        "%{wks.location}/sculpto/external",
        "%{IncludeDir.entt}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.imgui}",
    }

    links
    {
        "sculpto",
    }

    filter { "system:windows", "kind:WindowedApp or ConsoleApp" }
        entrypoint "mainCRTStartup"

    filter "system:windows"
        systemversion "latest"
        characterset ("MBCS")

    filter "configurations:Debug"
        defines { "SCL_DEBUG", "SCL_DEBUG_MEMORY_ENABLED", "SCL_ASSERTION_ENABLED" }
        symbols "On"

    filter "configurations:Release"
        defines "SCL_RELEASE"
        optimize "On"
        runtime "Release"

    filter "configurations:Dist"
        defines "SCL_DIST"
        optimize "On"
        runtime "Release"