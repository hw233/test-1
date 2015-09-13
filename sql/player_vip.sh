
#!/bin/bash

F=player_vip.txt
if [ "$1" != "" ]
then
    F=$1
fi

function player_vip()
{
# 11,"302,2,3|303,1",0,40,"3,4"
    f=$1
    d=player_vip
    sed -i /LVL/d $f
    sed -i /lvl/d $f
    sed -i /REF/d $f
    sed -i /^$/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `player_vip` VALUES";
        } {
        if( NR > 1)
            {
                printf("(%u,%u,\x27%s\x27, \x27%s\x27 )",$1,$2,$3,$4);
                if (NR <= ENVIRON["lines"]-1)
                    printf(",");
                else if (NR >= ENVIRON["lines"])
                    printf(";");
                    printf("\n");
                }
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
    player_vip $F
else
    echo "File $F is not exists"
fi

