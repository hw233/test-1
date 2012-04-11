
VERSION=`data +%y%m%d%H%M`

all:debug

debug:
	rm -rf build && cd tools/udplog_cpp/ && make 
	cd tools/json-c-0.9 && ./make.sh
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=debug $(MAKE) -j8 -f Makefile

release:
	rm -rf build && cd tools/udplog_cpp/ && make 
	cd tools/json-c-0.9 && ./make.sh
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && config=release $(MAKE) -j8 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

debug_fb:
	rm -rf build && cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4fb.lua gmake; fi\
		&& cd build && config=debug $(MAKE) -j8 -f Makefile

release_fb:
	rm -rf build && cd tools/udplog_cpp/ && make
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 --file=premake4fb.lua gmake; fi\
		&& cd build && config=release $(MAKE) -j8 -f Makefile
	@objcopy --only-keep-debug bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol
	@objcopy --strip-debug bin/Release/Server.ASSS

clean:
	find . -name *.o | xargs rm -rf
	cd tools/json-c-0.9 && make distclean
	cd tools/udplog_cpp/ && make clean
	cd build && make -f Makefile clean

