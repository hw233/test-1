
#!/bin/bash

F=geartree.txt
if [ "$1" != "" ]
then
    F=$1
fi

function geartree()
{
    f=$1
    d=geartree
    sed -i /ID/d $f
    sed -i /id/d $f
    sed -i /^$/d $f
    sed -i /REF/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `geartree` VALUES";
        } {
            printf("(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",$1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11);
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
        #iconv2utf8 $d
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
    geartree $F
else
    echo "File $F is not exists"
fi
