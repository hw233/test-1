
#!/bin/bash

F=rideUpdateCfg.txt
if [ "$1" != "" ]
then
    F=$1
fi

function rideUpdateCfg()
{
    f=$1
    d=ride_upgrade
    sed -i /ID/d $f
    sed -i /id/d $f
    sed -i /^$/d $f
    sed -i /REF/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `ride_upgrade` VALUES";
        } {
            printf("(%d,\x27%s\x27,%d,%d,%d,%f)",$1,$2,$3,$4,$5,$6);
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
    rideUpdateCfg $F
else
    echo "File $F is not exists"
fi
