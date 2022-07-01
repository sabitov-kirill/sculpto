workspace "sculpto"
  architecture "x64"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}-"

IncludeDir = {}
IncludeDir["glad"] = "sculpto/external/glad/include"
IncludeDir["stb_image"] = "sculpto/external/stb_image"
IncludeDir["imgui"] = "sculpto/external/imgui"

include "sculpto/external/glad"
include "sculpto/external/imgui"

project "sculpto"
  location "sculpto"
  kind "StaticLib"
  staticruntime "on"
  cppdialect "C++20"
  language "C++"

  targetdir ("bin/" .. outputdir .. "%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "%{prj.name}")

  pchheader "sclpch.h"
  pchsource "sculpto/src/sclpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/external/stb_image/stb_image.h",
    "%{prj.name}/external/stb_image/stb_image.cpp",
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{IncludeDir.glad}",
    "%{IncludeDir.stb_image}",
    "%{IncludeDir.imgui}",
  }

  links
  {
    "glad",
    "imgui",
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

project "sandbox"
  location "samples/sandbox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  targetdir ("bin/" .. outputdir .. "%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "%{prj.name}")

  files
  {
    "samples/%{prj.name}/src/**.h",
    "samples/%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "sculpto/src",
    "sculpto/external",
    "%{IncludeDir.stb_image}",
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
    runtime "Debug"

  filter "configurations:Release"
    defines "SCL_RELEASE"
    optimize "On"
    runtime "Release"

  filter "configurations:Dist"
    defines "SCL_DIST"
    optimize "On"
    runtime "Release"
