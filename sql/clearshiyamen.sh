source conf.sh
mysql -h$H -u$U -p$P -P$PT $DBO -e "update player set shimen='0,0|',yamen='0,0|';";
