#!/bin/bash

F=goods.csv
if [ "$1" != "" ]
then
    F=$1
fi

#the format of line
#id,name,type,level,color,imageId,useable,pile,price,energy,binding,propId,tips
#1,金创药,20,1,1,eq0001,0,99,1,0,0,0,使用：恢复2000点生命

function item_template()
{
    f=$1
    d=item_template.txt
    export lines=`wc -l $f | awk '{print $1}'`
    sed -i /id,name/d $f
    echo "Generating file $d, total lines $l"
    awk -F, '
        BEGIN {
            print "INSERT INTO `item_template` VALUES";
        } {
            printf("(%d,\x27%s\x27,%d,%d,%d,%d,%d,%d,%d,%d)",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10);
            if (NR <= ENVIRON["lines"]-1)
                printf(",");
            else if (NR >= ENVIRON["lines"])
                printf(";");
            printf("\n");
        }
        END {
        }
    ' $f > $d
    if [ $? -eq 0 ]
    then
        echo "OK"
    else
        echo "ERROR"
    fi
}

if [ -f $F  ]
then
    item_template $F
else
    echo "File $F is not exists"
fi

