
if [ ! -f .libs/libjson.a ]; then
    ./configure && make && cp .libs/libjson.a libjson.a
fi


