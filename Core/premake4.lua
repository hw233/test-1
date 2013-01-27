project "Server.ASSS"
    kind "ConsoleApp"

    files { "**.cpp", "**.c", "**.h" }
    excludes { "lua/*", "JsonProxy/*", "Common/*_WIN32.*", "Common/*_UNIX.*", "Common/*_POSIX.*" }
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
        links { "event", "event_pthreads", "dl", "udplog", "pthread", "dcapi_cpp-64" }
    configuration { "windows", "not vs*" }
        links { "event" }
    configuration "not vs*"
    	buildoptions { "-std=c++0x" }
        links { "mysqlclient" }
    configuration { "Debug", "not vs*"}
    	buildoptions { "-pg" }
        linkoptions { "-pg" }
    configuration "not windows"
        links { "event", "event_pthreads", "dl", "udplog", "memcached", "dcapi_cpp-64", "curl" }
    configuration "linux"
        defines "LUA_USE_LINUX"
    configuration "macosx"
        defines "LUA_USE_MACOSX"
    configuration "luajit"
        links { "Server.lua.jit" }
    configuration "luaorg"
        links { "Server.lua" }

if (_OPTIONS["luaapi"] ~= "luajit") then
project "Server.lua"
    kind "StaticLib"

    files { "lua/*.c", "lua/*.h" }
    configuration "linux"
        defines "LUA_USE_LINUX"
    configuration { "Debug", "not vs*"}
    	buildoptions { "-pg" }
    configuration "macosx"
        defines "LUA_USE_MACOSX"
else
--project "Server.lua.jit"
--    kind "StaticLib"
--
--    configuration "linux"
--        defines "LUA_USE_LINUX"
--    configuration { "Debug", "not vs*"}
--    	buildoptions { "-pg" }
--    configuration "macosx"
--        defines "LUA_USE_MACOSX"
end

project "JsonProxy.ASSS"
    kind "ConsoleApp"

    files { "JsonProxy/*.cpp", "Common/*.cpp", "Log/*.cpp", "Server/Cfg.cpp", "Script/Script.cpp", "Script/lua_tinker.cpp", "Script/ConfigScript.cpp", "Script/DepartDBScript.cpp", "Common/*.h", "Log/*.h", "Script/ConfigScript.h", "Script/DepartDBScript.h", "Network/Network.h", "Server/Cfg.h" }
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
        links { "event", "event_pthreads", "dl", "pthread", "memcached" }
    configuration { "windows", "not vs*" }
        links { "event" }
    configuration "not vs*"
    	buildoptions { "-std=c++0x" }
        links { "mysqlclient" }
    configuration { "Debug", "not vs*"}
    	buildoptions { "-pg" }
        linkoptions { "-pg" }
    configuration "not windows"
        links { "event", "event_pthreads", "dl", "memcached" }
    configuration "linux"
        defines { "LUA_USE_LINUX", "_JSON_PROXY_" }
        defines "LUA_USE_LINUX"
    configuration "macosx"
        defines { "LUA_USE_MACOSX", "_JSON_PROXY_" }
    configuration "luajit"
        links { "Server.lua.jit" }
    configuration "luaorg"
        links { "Server.lua" }


