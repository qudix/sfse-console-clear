includes("lib/commonlibsf")

set_project("sfse-console-clear")
set_version("0.2.3")
set_license("GPL-3.0")
set_languages("c++23")
set_warnings("allextra")

add_rules("mode.releasedbg", "mode.debug")
add_rules("plugin.vsxmake.autoupdate")

target("sfse-console-clear")
    add_rules("commonlibsf.plugin", {
        name = "console-clear",
        author = "qudix",
        description = "Fixes the stubbed console clear command"
    })

    add_files("src/*.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")
