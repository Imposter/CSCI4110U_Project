project "Project"
	kind "ConsoleApp"
	language "C++"
	characterset "MBCS"
	
	includedirs {
		"../dependencies/include",
		"../dependencies/include/GLEW",
		"../dependencies/include/FreeGLUT",
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