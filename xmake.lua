-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsf")

-- set project
set_project("sfse-console-clear")
set_version("0.2.1")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")
set_defaultmode("releasedbg")

-- add rules
add_rules("mode.releasedbg", "mode.debug")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)

-- setup targets
target("sfse-console-clear")
    -- add dependencies to target
    add_deps("commonlibsf")

    -- add commonlibsf plugin
    add_rules("commonlibsf.plugin", {
        name = "console-clear",
        author = "qudix",
        description = "Fixes the stubbed console clear command"
    })

    -- add source files
    add_files("src/*.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- copy build to MODS or GAME paths
    after_build(function(target)
        local copy = function(env, ext)
            for _, env in pairs(env:split(";")) do
                if os.exists(env) then
                    local plugins = path.join(env, ext, "SFSE/Plugins")
                    os.mkdir(plugins)
                    os.trycp(target:targetfile(), plugins)
                    os.trycp(target:symbolfile(), plugins)
                end
            end
        end
        if os.getenv("XSE_SF_MODS_PATH") then
            copy(os.getenv("XSE_SF_MODS_PATH"), target:name())
        elseif os.getenv("XSE_SF_GAME_PATH") then
            copy(os.getenv("XSE_SF_GAME_PATH"), "Data")
        end
    end)
