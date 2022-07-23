project "rccpp_runtime_compiler"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "*.h",
        "*.cpp",
        "SimpleFileWatcher/FileWatcher.cpp",
        "SimpleFileWatcher/FileWatcher.h",
        "SimpleFileWatcher/FileWatcherImpl.h"
    }

    includedirs
    {
        "%{prj.dir}"
    }

    filter "system:windows"
        systemversion "latest"
        files
        {
            "SimpleFileWatcher/FileWatcherWin32.cpp",
            "SimpleFileWatcher/FileWatcherWin32.h",
            "SimpleFileWatcher/FileWatcherWin32_AltImpl.h"
        }

    filter "system:linux"
        pic "On"
        systemversion "latest"
        files
        {
            "SimpleFileWatcher/FileWatcherLinux.cpp",
            "SimpleFileWatcher/FileWatcherLinux.h"
        }

    filter "system:macos"
        systemversion "latest"
        files
        {
            "SimpleFileWatcher/FileWatcherOSX.cpp",
            "SimpleFileWatcher/FileWatcherOSX.h"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
