project "Server.ASSS"
    kind "ConsoleApp"

    files { "**.cpp", "**.c", "**.h" }
    excludes { "lua/*", "Common/*_WIN32.*", "Common/*_UNIX.*", "Common/*_POSIX.*" }
    includedirs { "." }

    configuration { "vs* or codeblocks" }
        pchheader "Config.h"
	pchsource "Config.cpp"
    configuration { "not vs* or codeblocks" }
        excludes { "Config.cpp" }
    configuration "windows"
        links { "ws2_32", "mswsock", "kernel32", "user32" }
    configuration { "Debug", "vs*" }
        links { "eventd" }
    configuration { "Release", "vs*" }
        links { "event", "event_pthreads", "dl", "udplog", "pthread" }
    configuration { "windows", "not vs*" }
        links { "event" }
    configuration "not vs*"
    	buildoptions { "-std=c++0x", "-pg" }
        links { "mysqlclient" }
    configuration "not windows"
        links { "event", "event_pthreads", "dl", "udplog", "memcached" }
        --links { "event", "event_pthreads", "dl", "udplog", "pthread" }
    configuration "linux"
        defines "LUA_USE_LINUX"
    configuration "macosx"
        defines "LUA_USE_MACOSX"
    configuration "luajit"
        links { "luajit-5.1" }
    configuration "luaorg"
        links { "Server.lua" }

if (_OPTIONS["luaapi"] ~= "luajit") then
project "Server.lua"
    kind "StaticLib"

    files { "lua/*.c", "lua/*.h" }
    configuration "linux"
        defines "LUA_USE_LINUX"
    configuration "macosx"
        defines "LUA_USE_MACOSX"
else
project "Server.lua.jit"
    kind "StaticLib"

    files { "luajit/*.c", "luajit/*.h" }
    configuration "linux"
        defines "LUA_USE_LINUX"
    configuration "macosx"
        defines "LUA_USE_MACOSX"
end

