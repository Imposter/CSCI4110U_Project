project "GLEW"
	kind "SharedLib"
	language "C++"
	characterset "MBCS"
	
	includedirs {
		"GLEW/include",
		"GLEW/src"
	}
	
	files {
		"GLEW/include/**.h",
		"GLEW/src/glew.c"
	}
	
	links {
		"opengl32.lib"
	}
	
	defines {
		"GLEW_BUILD"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		postbuildcommands { 
			"{COPY} " .. _RA("GLEW/include") .. " " .. _RA("include/GLEW")
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		postbuildcommands {
			"{COPY} " .. _RA("GLEW/include") .. " " .. _RA("include/GLEW")
		}