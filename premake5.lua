project "MElib"
    kind "StaticLib"
    language "C++"
	cppdialect "C++20"
	staticruntime "Off"
	conformancemode "On"

    targetdir ("%{wks.location}/bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")
    objdir ("%{wks.location}/bin-int/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}")

    files {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs {
        "Source/"
    }

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX"
	}

	filter "configurations:Debug or configurations:Debug-AS"
		optimize "Off"
		symbols "On"
        defines { "ME_CONFIG_DEBUG", "_DEBUG" }

	filter "configurations:Release"
		optimize "On"
		symbols "Default"
        defines { "ME_CONFIG_RELEASE", "NDEBUG" }

	filter "system:windows"
		buildoptions { "/Zc:preprocessor", "/Zc:__cplusplus" }
        systemversion "latest"
        defines { "ME_PLATFORM_WINDOWS" }
