
#!/bin/bash

F=soullvlexp.txt
if [ "$1" != "" ]
then
    F=$1
fi

function soullvlexp()
{
# 11,"302,2,3|303,1",0,40,"3,4"
    f=$1
    d=soullvlexp
    sed -i /LVL/d $f
    sed -i /lvl/d $f
    sed -i /exp/d $f
    sed -i /pexp/d $f
    sed -i /^$/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `soul_lvl_exp` VALUES";
        } {
            printf("(%d,%d,%d)",$1,$2,$3);
            if (NR <= ENVIRON["lines"]-1)
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
    soullvlexp $F
else
    echo "File $F is not exists"
fi

