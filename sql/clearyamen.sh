source conf.sh
mysql -h$H -u$U -p$P -P$PT $DBO -e "update player set yamen='0,0|';";
