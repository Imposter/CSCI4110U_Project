project "GLM"
	kind "StaticLib"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"GLM/glm"
	}
	
	files {
		"GLM/glm/**.hpp",
		"GLM/glm/**.inl"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }

	filter "configurations:Release"
		defines { "NDEBUG" }