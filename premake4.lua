solution "HoHH"
    newoption {
        trigger     = "luaapi",
        value       = "API",
        description = "Select lua wrapper API to use",
        allowed = {
            { "luaorg",  "Original official normal lua api" },
            { "luajit",  "LuaJIT wrapper" }
        }
    }

    if not _OPTIONS["luaapi"] then
        _OPTIONS["luaapi"] = "luaorg"
    end

    location "build"
    language "C++"
    objdir "obj"

    configurations { "Debug", "Release" }

    configuration "windows"
        defines { "WIN32", "_WIN32" }

    configuration "bsd"
        includedirs { "/usr/local/include", "/usr/local/include/mysql" }
        libdirs { "/usr/local/lib" }

    configuration "macosx"
        includedirs { "/opt/local/include", "/opt/local/include/mysql" }
        libdirs { "/opt/local/lib" }

    configuration "linux"
    	includedirs { "/usr/include/mysql", "/usr/local/include/mysql", "/usr/local/include", "/usr/local/mysql", "/usr/local/mysql/include", "/usr/local/mysql/include/mysql", "/usr/local/services/mysql/include/mysql", "/usr/local/services/libevent/include", "tools/udplog_cpp", "/usr/local/services/libmemcached/include", "tools/DC_CPP_SDK_V1.0/inc", "/usr/local/services/curl/include", "/opt/app/mysql", "/opt/app/mysql/include", "/opt/app/mysql/include/mysql", "/opt/app/libevent/include", "/opt/app/libmemcached/include", "/opt/app/curl/include" }
        libdirs { "/usr/local/lib/mysql", "/usr/local/lib64", "/usr/local/lib", "/usr/local/mysql/lib", "/usr/local/mysql/lib/mysql/", "/usr/local/services/mysql/lib/mysql", "/usr/local/services/libevent/lib", "tools/udplog_cpp", "/usr/local/services/libmemcached/lib", "tools/DC_CPP_SDK_V1.0/lib", "/usr/local/services/curl/lib", "/opt/app/mysql/lib", "/opt/app/mysql/lib/mysql", "/opt/app/libevent/lib", "/opt/app/libmemcached/lib", "/opt/app/curl/lib" }
        defines { "_APPID=27036", "_VERSION="..os.date("%y%m%d%H%M", os.time()), "_NEED_OPENID", "_FB" }

    configuration "vs*"

		flags "EnableSSE2"
		defines { "_CRT_SECURE_NO_WARNINGS" }

    configuration "not vs*"
        buildoptions { "-Wall", "-fexceptions", "-fno-strict-aliasing", "-Wno-multichar" }
    configuration {"not vs*", "windows"}
        buildoptions { "-static-libgcc", "-static-libstdc++" }

    configuration "Debug"
        flags "Symbols"
        defines "_DEBUG"
        targetdir "bin/Debug"

    configuration { "Release", "not vs*" }
        flags "Symbols"
	-- buildoptions "-march=native" -- for suse

    configuration "Release"
        flags { "OptimizeSpeed", "FloatFast",  }
        targetdir "bin/Release"

    include "Core"
