#!/bin/bash
source conf.sh
d=`date +%Y%m%d`
#mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from database_version; insert into database_version values ($d)";
mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from database_version; insert into database_version values ('20110822')";
