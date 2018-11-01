project "TinyOBJLoader"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	
	includedirs {
		"TinyOBJLoader/"
	}
	
	files {
		"TinyOBJLoader/*.h"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		postbuildcommands { 
			"{COPY} " .. _RA("TinyOBJLoader/tiny_obj_loader.h") .. " " .. _RA("include/TinyOBJLoader/tiny_obj_loader.h")
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		postbuildcommands {
			"{COPY} " .. _RA("TinyOBJLoader/tiny_obj_loader.h") .. " " .. _RA("include/TinyOBJLoader/tiny_obj_loader.h")
		}