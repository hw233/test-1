
#!/bin/bash

F=npc.txt
if [ "$1" != "" ]
then
    F=$1
fi

function npc()
{
# 11,"302,2,3|303,1",0,40,"3,4"
    f=$1
    d=npc
    sed -i /id/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `special_fighter_template` VALUES";
        } {
            printf("(%d,\x27%s\x27,%d,%d,%d,%f,%f,\x27%s\x27,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,\x27%s\x27)",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21,$22,$23,$24,$25,$26,$27,$28,$29,$30,$31,$32,$33);
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
    npc $F
else
    echo "File $F is not exists"
fi

