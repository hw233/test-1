#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P -D$DBD < Data.sql
mysql -h$H -u$U -p$P -D$DBO < Object.sql
mysql -h$H -u$U -p$P -D$DBD < DataInsert.sql
mysql -h$H -u$U -p$P -D$DBO < ObjectInsert.sql

