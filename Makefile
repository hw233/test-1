
VERSION=`data +%y%m%d%H%M`
JSON=json-1.4

all:debug_jit

debug_jit:
	rm -rf build && cd tools/udplog_cpp/ && make 
	cd tools/$(JSON) && ./make.sh
	cd luajit && make && mkdir -p ../bin/Debug/ && cp src/libluajit.a ../bin/Debug/libServer.lua.jit.a
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4jit.lua gmake; fi\
		&& cd build && config=debug $(MAKE) -j2 -f Makefile

release_jit:
	rm -rf build && cd tools/udplog_cpp/ && make 
	cd tools/$(JSON) && ./make.sh
	cd luajit && make && mkdir -p ../bin/Release/ && cp src/libluajit.a ../bin/Release/libServer.lua.jit.a
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4jit.lua gmake; fi\
		&& cd build && config=release $(MAKE) -j2 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

debug:
	rm -rf build && cd tools/udplog_cpp/ && make 
	cd tools/$(JSON) && ./make.sh
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=debug $(MAKE) -j2 -f Makefile

release:
	rm -rf build && cd tools/udplog_cpp/ && make 
	cd tools/$(JSON) && ./make.sh
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=release $(MAKE) -j2 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

debug_fb:
	rm -rf build && cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4fb.lua gmake; fi\
		&& cd build && config=debug $(MAKE) -j2 -f Makefile

release_fb:
	rm -rf build && cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4fb.lua gmake; fi\
		&& cd build && config=release $(MAKE) -j2 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

debug_vt:
	rm -rf build && cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4vt.lua gmake; fi\
		&& cd build && config=debug $(MAKE) -j2 -f Makefile

release_vt:
	rm -rf build && cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4vt.lua gmake; fi\
		&& cd build && config=release $(MAKE) -j2 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

clean:
	find . -name *.o | xargs rm -rf
	cd tools/$(JSON) && make distclean
	cd tools/udplog_cpp/ && make clean
	cd build && make -f Makefile clean

touch:
	touch .now
	find . -cnewer .now -exec touch {} \;
	rm -f .now


