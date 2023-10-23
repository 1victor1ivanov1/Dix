IncludeDirs = {}
IncludeDirs["Glad"]				= "%{wks.location}/Dix/Vendor/Glad/include"
IncludeDirs["GLFW"]				= "%{wks.location}/Dix/Vendor/GLFW/include"
IncludeDirs["spdlog"]			= "%{wks.location}/Dix/Vendor/spdlog/include"
IncludeDirs["Assimp"]			= "%{wks.location}/Dix/Vendor/Assimp/include"
IncludeDirs["ImGui"]			= "%{wks.location}/Dix/Vendor/ImGui"
IncludeDirs["glm"] 				= "%{wks.location}/Dix/Vendor/glm"
IncludeDirs["stb_image"] 		= "%{wks.location}/Dix/Vendor/stb_image/include"

LibraryDirs = {}
LibraryDirs["Assimp"] 			= "%{wks.location}/Dix/Vendor/Assimp/lib/x64"

Library = {}
Library["Assimp"] 				= "%{LibraryDirs.Assimp}/assimp-vc143-mt.lib"