project "TinyOBJLoader"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"TinyOBJLoader/"
	}
	
	files {
		"TinyOBJLoader/*.h"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		defines { "NDEBUG" }