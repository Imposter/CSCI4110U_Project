project "STB"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"STB/"
	}
	
	files {
		"STB/**.h"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		defines { "NDEBUG" }