f=../log/DB/INFO20130821.gz
echo $f
ff=`dirname $f`
ff=`dirname $ff`
ff=`dirname $ff`
n=`basename $ff`
zcat $f | grep player_tuzhi|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] > $ff/sql/player_tuzhi.sql
zcat $f | grep player_jiguanshu|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] > $ff/sql/player_jiguanshu.sql
zcat $f | grep player_jiguanyu|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] > $ff/sql/player_jiguanyu.sql

f=../log/DB/INFO20130822.gz
echo $f
ff=`dirname $f`
ff=`dirname $ff`
ff=`dirname $ff`
n=`basename $ff`
zcat $f | grep player_tuzhi|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] >> $ff/sql/player_tuzhi.sql
zcat $f | grep player_jiguanshu|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] >> $ff/sql/player_jiguanshu.sql
zcat $f | grep player_jiguanyu|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] >> $ff/sql/player_jiguanyu.sql

f=../log/DB/INFO20130823
echo $f
ff=`dirname $f`
ff=`dirname $ff`
ff=`dirname $ff`
n=`basename $ff`
cat $f | grep player_tuzhi|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] >> $ff/sql/player_tuzhi.sql
cat $f | grep player_jiguanshu|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] >> $ff/sql/player_jiguanshu.sql
cat $f | grep player_jiguanyu|awk -F[][] '{printf "%s;\n", $6}' | grep ^[RD] >> $ff/sql/player_jiguanyu.sql

