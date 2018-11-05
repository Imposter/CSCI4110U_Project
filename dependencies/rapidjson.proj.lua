project "RapidJSON"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"RapidJSON/include/rapidjson"
	}
	
	files {
		"RapidJSON/include/rapidjson/**.h"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		defines { "NDEBUG" }