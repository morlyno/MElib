workspace "MElib"
	configurations { "Debug", "Release" }
	startproject "Tests"
	conformancemode "On"

	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	flags { "MultiProcessorCompile" }

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX"
	}

	filter "language:C++ or Language:C"
		architecture "x86_64"
		
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

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "Dependencies"
	include "vendor/spdlog.lua"
group ""

include "melib"
include "Tests"
