
all:debug

debug:
	cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=debug $(MAKE) -j2 -f Makefile

release:
	cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=release $(MAKE) -j2 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

clean:
	cd tools/udplog_cpp/ && make clean
	cd build && make -f Makefile clean

