-- premake5.lua

include "Dependencies.lua"

workspace "Dix"
    architecture "x86_64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release",
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Dix/Vendor/GLFW"
    include "Dix/Vendor/Glad"
    include "Dix/Vendor/ImGui"
    include "Dix/Vendor/stb_image"
group ""

group "Core"
    include "Dix"
group ""

group "Misc"
    include "Sandbox"
group ""