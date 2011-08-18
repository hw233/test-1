
all:debug release

debug:
	cd tools/udplog_cpp/ && make Makefile
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=debug $(MAKE) -j8 -f Makefile

release:
	cd tools/udplog_cpp/ && make Makefile
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=release $(MAKE) -j8 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

clean:
	make -f tools/udplog_cpp/Makefile clean
	cd build && make -f Makefile clean

