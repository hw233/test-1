
all:debug release

debug:
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=debug64 $(MAKE) -j8 -f Makefile

release:
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=release64 $(MAKE) -j8 -f Makefile
	@objcopy --only-keep-debug bin/Debug/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

clean:
	cd build && make -f Makefile clean

