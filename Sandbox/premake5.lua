project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/Build/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Build/Int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Dix/Source",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}",
	}

	links
	{
		"Dix"
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