#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110817_1.sql
mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110822_1.sql
mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110825_1.sql
