
all:
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && $(MAKE) -j8 -f Makefile

release:
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && $(MAKE) -j8 -f Makefile
	@objcopy --only-keep-debug bin/Debug/Server.ASSS bin/Debug/Server.ASSS.symbol
	@objcopy --strip-debug bin/Debug/Server.ASSS

clean:
	cd build && make -f Makefile clean

