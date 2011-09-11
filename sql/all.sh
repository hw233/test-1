#!/bin/bash
a=`ls *.txt`
b=`echo $a|sed s/\.txt/\.sh/g`
for i in $b
do
    . `pwd`/$i
done
rm -f $a
