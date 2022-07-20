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
IncludeDir["glew"]      = "%{wks.location}/sculpto/external/glew/include"
IncludeDir["stb_image"] = "%{wks.location}/sculpto/external/stb_image"
IncludeDir["imgui"]     = "%{wks.location}/sculpto/external/imgui"
IncludeDir["entt"]      = "%{wks.location}/sculpto/external/entt"
IncludeDir["json"]      = "%{wks.location}/sculpto/external/json"
IncludeDir["assimp"]    = "%{wks.location}/sculpto/external/assimp/include"

group "Dependencies"
    include "sculpto/external/glew"
    include "sculpto/external/imgui"
    include "sculpto/external/assimp"
group ""

include "sculpto"
include "samples/sandbox"
include "samples/editor"
include "samples/raytracing"