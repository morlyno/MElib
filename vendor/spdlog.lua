
spdlog = {
    IncludeDir = "%{wks.location}/vendor/spdlog/include",
    Defines = { "SPDLOG_USE_STD_FORMAT" }
}

project "spdlog"
    kind "None"
    location "spdlog"

    files {
        "%{prj.location}/include/**.h"
    }
