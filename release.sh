#!/bin/bash

if [ $# -lt 1 ]; then
    echo "usage: $0 release_name [fb|vt]"
    exit -1
fi

R=../release/
file=$1

if [ $# -ge 2 -a "$2" == "fb" ]; then
    make release_fb
    if [ $# -ge 3 -a "$3" == "s" ]; then
        tar cvfz $R$file -X exclude bin/Release/Server.ASSS scripts/ sql/
    else
        cp -rf sql sql.bak
        cp -rf scripts scripts.bak
        ./toft.sh
        tar cvfz $R"FB."$file -X exclude bin/Release/Server.ASSS scripts/ sql/
        rm -rf sql scripts
        mv sql.bak sql
        mv scripts.bak scripts
    fi
elif [ $# -ge 2 -a "$2" == "vt" ]; then
    make release_vt
    tar cvfz $R"VT."$file -X exclude bin/Release/JsonProxy.ASSS bin/Release/Server.ASSS scripts/ sql/
elif [ $# -ge 2 -a "$2" == "jit" ]; then
    make release_jit
    tar cvfz $R$file -X exclude bin/Release/JsonProxy.ASSS bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol scripts/ sql/
else
    make release
    tar cvfz $R$file -X exclude bin/Release/JsonProxy.ASSS bin/Release/Server.ASSS bin/Release/Server.ASSS.symbol scripts/ sql/
fi
