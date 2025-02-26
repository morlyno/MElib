project "Tests"
    kind "ConsoleApp"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    links { "MElib" }

    files {
        "Source/**.h",
        "Source/**.cpp"
    }

    includedirs {
        "Source/",

        "%{wks.location}/MElib/Source/"
    }
