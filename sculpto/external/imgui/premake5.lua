project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imgui_tables.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",

        "backends/imgui_impl_opengl3.h",
        "backends/imgui_impl_opengl3.cpp",
    }

    includedirs
    {
        "%{prj.dir}"
    }

    filter "system:windows"
        systemversion "latest"

        files
        {
            "backends/imgui_impl_win32.h",
            "backends/imgui_impl_win32.cpp",
        }

    filter "system:linux"
        pic "On"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
