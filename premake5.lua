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

	filter "configurations:Debug or configurations:Debug-AS"
		optimize "Off"
		symbols "On"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "system:windows"
		buildoptions { "/Zc:preprocessor", "/Zc:__cplusplus" }
        systemversion "latest"
