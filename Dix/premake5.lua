project "Dix"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/Build/Bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("%{wks.location}/Build/Int/" .. outputdir .. "/%{prj.name}")

    pchheader "dixpch.h"
    pchsource "Source/dixpch.cpp"

    files
    {
        "Source/**.h",
        "Source/**.cpp",

        "Vendor/glm/glm/**.hpp",
        "Vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "Source",
        "%{IncludeDirs.Glad}",
        "%{IncludeDirs.GLFW}",
        "%{IncludeDirs.spdlog}",
        "%{IncludeDirs.glm}",
        "%{IncludeDirs.stb_image}",
        "%{IncludeDirs.Assimp}",
        "%{IncludeDirs.ImGui}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "stb_image",

        "%{Library.Assimp}",

        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "DIX_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DIX_RELEASE"
        runtime "Release"
        optimize "on"