#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P $DBO -e "replace into database_version values (`date +%Y%m%d`)";
