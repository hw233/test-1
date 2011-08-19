#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P -P$PT $DBO -e "delete from database_version; insert into database_version values (`date +%Y%m%d`)";
