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
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		postbuildcommands { 
			"{COPY} " .. _RA("GLM/glm") .. " " .. _RA("include/glm")
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		postbuildcommands {
			"{COPY} " .. _RA("GLM/glm") .. " " .. _RA("include/glm")
		}