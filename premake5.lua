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
IncludeDir["glad"]      = "%{wks.location}/sculpto/external/glad/include"
IncludeDir["stb_image"] = "%{wks.location}/sculpto/external/stb_image"
IncludeDir["imgui"]     = "%{wks.location}/sculpto/external/imgui"
IncludeDir["entt"]      = "%{wks.location}/sculpto/external/entt"

group "Dependencies"
	include "sculpto/external/glad"
	include "sculpto/external/imgui"
group ""

include "sculpto"
include "samples/sandbox"