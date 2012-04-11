
if [ ! -f .libs/libjson.a ]; then
    ./configure --disable-shared && make && cp .libs/libjson.a libjson.a
fi


