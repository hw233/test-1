
#!/bin/bash

F=eupgrade.txt
if [ "$1" != "" ]
then
    F=$1
fi

function eupgrade()
{
    f=$1
    d=eupgrade
    sed -i /stuff/d $f
    sed -i /isPack/d $f
    sed -i /ID/d $f
    sed -i /REF/d $f
    sed -i /^$/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `eupgrade` VALUES";
        } {
            printf("(%d,\x27%s\x27,%d)",$1,$2,$3);
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
    eupgrade $F
else
    echo "File $F is not exists"
fi

