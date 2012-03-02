
#!/bin/bash

F=towndeamonmonster.txt
if [ "$1" != "" ]
then
    F=$1
fi

function towndeamonmonster()
{
    f=$1
    d=towndeamonmonster
    sed -i /level/d $f
    sed -i /npcId/d $f
    sed -i /itemId/d $f
    sed -i /^$/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `towndeamon_monster` VALUES";
        } {
            printf("(%d,%d,%d,%d)",$1,$2,$3,$4);
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
    towndeamonmonster $F
else
    echo "File $F is not exists"
fi

