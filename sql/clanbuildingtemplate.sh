#!/bin/bash

F=clanbuildingtemplate.txt
if [ "$1" != "" ]
then
    F=$1
fi

function clanbuildingtemplate()
{
    f=$1
    d=clanbuildingtemplate
<<<<<<< HEAD
    sed -i /lev/d $f
=======
    sed -i /Lv/d $f
>>>>>>> 2328f2a874a05db7bb431f3c531ed3ae3f7b78bf
    sed -i /EXP/d $f
    sed -i /level/d $f
    sed -i /^$/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `clan_building_template` VALUES";
        } {
            printf("(%d,%d,%d,%d,%d,%d,%d)",$1,$2,$4,$5,$6,$7,$8);
            if (NR < ENVIRON["lines"])
                printf(",");
            else if (NR >= ENVIRON["lines"])
                printf(";");
            printf("\n");
        }
        END {
        }
    ' $f > $d
    sed -i s/\\r//g $d
    if [ $? -eq 0 ]
    then
        iconv2utf8 $d
        echo "OK"
    else
        echo "ERROR"
    fi
}

function iconv2utf8()
{
    iconv -f cp936 -t utf8 $1 -o $1.tmp
    rm $1
    mv $1.tmp $1
}

if [ -f $F  ]
then
    clanbuildingtemplate $F
else
    echo "File $F is not exists"
fi

