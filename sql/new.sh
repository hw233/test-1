#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P -P$PT -D$DBD < Data.sql
mysql -h$H -u$U -p$P -P$PT -D$DBO < Object.sql
mysql -h$H -u$U -p$P -P$PT -D$DBD < DataInsert.sql
mysql -h$H -u$U -p$P -P$PT -D$DBO < ObjectInsert.sql

