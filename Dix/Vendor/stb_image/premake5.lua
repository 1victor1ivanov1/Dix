project "stb_image"
    kind "StaticLib"
    language "C"
    staticruntime "off"
    
    targetdir ("Build/Bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("Build/Int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/stb_image.h",
        "src/stb_image.cpp"
    }

    includedirs
    {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"