project "STB"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	
	includedirs {
		"STB/"
	}
	
	files {
		"STB/**.h"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		postbuildcommands { 
			"{COPY} " .. _RA("STB") .. " " .. _RA("include/STB")
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		postbuildcommands {
			"{COPY} " .. _RA("STB") .. " " .. _RA("include/STB")
		}