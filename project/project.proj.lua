project "Project"
	kind "ConsoleApp"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"../dependencies/FreeGLUT/include",
		"../dependencies/GLEW/include",
		"../dependencies/GLM",
		"../dependencies/RapidJSON/include",
		"../dependencies/STB",
		"../dependencies/TinyOBJLoader",
		"src",
	}
	
	files {
		"src/**.h",
		"src/**.c",
		"src/**.cpp",
		"data/**"
	}
	
	defines {
		"FREEGLUT_LIB_PRAGMAS=0"
	}
	
	links {
		"GLEW",
		"FreeGLUT",
		"opengl32.lib"
	}
	
	dependson {
		"GLM",
		"rapidjson",
		"STB",
		"TinyOBJLoader"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		postbuildcommands { 
			"{COPY} " .. _RA("data") .. " %{cfg.buildtarget.directory}/data"
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		postbuildcommands {
			"{COPY} " .. _RA("data") .. " %{cfg.buildtarget.directory}/data"
		}