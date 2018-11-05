project "FreeGLUT"
	kind "SharedLib"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"FreeGLUT/include",
		"FreeGLUT/src",
	}
	
	files {
		"FreeGLUT/include/**.h",
		"FreeGLUT/src/**.h",
		"FreeGLUT/src/**.c",
		"FreeGLUT/src/freeglutdll.def.in"
	}
	
	-- Not supported by Windows
	removefiles {
		"FreeGLUT/src/android/**",
		"FreeGLUT/src/blackberry/**",
		"FreeGLUT/src/egl/**",
		"FreeGLUT/src/osx/**",
		"FreeGLUT/src/x11/**",
		"FreeGLUT/src/gles_stubs.c",
	}
	
	defines {
		"FREEGLUT_EXPORTS",
		"HAVE_SYS_TYPES_H",
		"HAVE_STDBOOL_H",
		"HAVE_FCNTL_H",
		"NEED_XPARSEGEOMETRY_IMPL", -- Required for fg_init.c
		"HAVE_STDINT_H",
		"HAVE_INTTYPES_H",
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"