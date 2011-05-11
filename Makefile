
all:
	if [ ! -d build ]; then\
		premake4 --os=linux --platform=x64 gmake; fi\
		&& cd build && make -j2 -f Makefile

clean:
	cd build && make -f Makefile clean

