sources = {
   "loader_example.cc",
   }

-- premake5.lua
project "TinyObjLoader"
   kind "StaticLib"
   language "C++"
   cppdialect "C++17"
   staticruntime "off"

   targetdir ("Build/Bin/" .. outputdir .. "%{prj.name}")
   objdir    ("Build/Int/" .. outputdir .. "%{prj.name}")

   files 
   { 
      "include/TinyObjLoader/tiny_obj_loader.h",
      "src/tiny_obj_loader.cc"
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
