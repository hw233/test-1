f=../log/DB/TRACE20131202.gz
echo $f
ff=`dirname $f`
ff=`dirname $ff`
ff=`dirname $ff`
n=`basename $ff`
zcat $f | grep dailyBattleScore | awk -F[][] '{print $6}'|sed 's/$/;/g' > clanscore.sql
