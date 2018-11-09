project "AssImp"
	kind "StaticLib" --kind "SharedLib"
	language "C++"
	characterset "MBCS"
	systemversion "latest"
	
	includedirs {
		"AssImp",
		"AssImp/include",
		"AssImp/code",
		"AssImp/code/include",
		"AssImp/contrib",
		"AssImp/contrib/rapidjson/include",
		"AssImp/contrib/irrXML",
		"AssImp/contrib/openddlparser/include",
		"config",
		"config/assimp",
	}
	
	files {
		"config/assimp/config.h",
		"config/assimp/revision.h",
		"AssImp/include/**.h",
		"AssImp/code/**.h",
		"AssImp/code/**.cpp"
	}
	
	defines {
		"assimp_EXPORTS",
		--"ASSIMP_BUILD_DLL_EXPORT",
		"ASSIMP_BUILD_NO_EXPORT",
		"ASSIMP_BUILD_NO_IFC_IMPORTER",
		"ASSIMP_BUILD_NO_3MF_IMPORTER",
		"ASSIMP_BUILD_NO_ASSBIN_IMPORTER",
		"ASSIMP_BUILD_NO_C4D_IMPORTER",
		"ASSIMP_BUILD_NO_BLEND_IMPORTER",
		"ASSIMP_BUILD_NO_FBX_IMPORTER",
		"ASSIMP_BUILD_NO_Q3BSP_IMPORTER",
		"ASSIMP_BUILD_NO_X_IMPORTER",
		"ASSIMP_BUILD_NO_XGL_IMPORTER",
		"ASSIMP_BUILD_NO_STEPFILE_IMPORTER",
		"ASSIMP_IMPORTER_GLTF_USE_OPEN3DGC=1",
		"_CRT_SECURE_NO_WARNINGS",
		"_SCL_SECURE_NO_WARNINGS"
	}
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"