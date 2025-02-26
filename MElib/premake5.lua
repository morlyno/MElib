project "MElib"
    kind "StaticLib"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs {
        "Source/",

        "%{spdlog.IncludeDir}"
    }

    defines {
        spdlog.Defines
    }