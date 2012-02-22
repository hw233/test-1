#!/bin/bash
fl=filelist
find sql -type f > $fl
find scripts -type f >> $fl
awk '{system("./convert/doconvert.sh " $1)}' $fl
