
#!/bin/bash

F=mapspot.txt
if [ "$1" != "" ]
then
    F=$1
fi

function mapspot()
{
# 11,"302,2,3|303,1",0,40,"3,4"
    f=$1
    d=mapspot
    sed -i /countryBattle/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `map_spot` VALUES";
        } {
            printf("(%d,\x27%s\x27,%d,%d,%d,%d)",$1,$2,$3,$4,$5,$6);
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
        echo "OK"
    else
        echo "ERROR"
    fi
}

if [ -f $F  ]
then
    mapspot $F
else
    echo "File $F is not exists"
fi

