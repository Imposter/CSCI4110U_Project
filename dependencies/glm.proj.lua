project "GLM"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	
	includedirs {
		"GLM/glm"
	}
	
	files {
		"GLM/glm/**.hpp",
		"GLM/glm/**.inl"
	}
	
	links {
		"opengl32.lib"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		postbuildcommands { 
			"{COPY} " .. _RA("GLM/glm") .. " " .. _RA("include/GLM")
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		postbuildcommands {
			"{COPY} " .. _RA("GLM/glm") .. " " .. _RA("include/GLM")
		}