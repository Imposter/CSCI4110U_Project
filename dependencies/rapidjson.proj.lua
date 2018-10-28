project "RapidJSON"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	
	includedirs {
		"RapidJSON/include/rapidjson"
	}
	
	files {
		"RapidJSON/include/rapidjson/**.h"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		postbuildcommands { 
			"{COPY} " .. _RA("RapidJSON/include/rapidjson") .. " " .. _RA("include/rapidjson")
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		postbuildcommands {
			"{COPY} " .. _RA("RapidJSON/include/rapidjson") .. " " .. _RA("include/rapidjson")
		}