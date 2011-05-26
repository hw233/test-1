#!/bin/bash
source conf.sh
mysql -h$H -u$U -p$P < Data.sql
mysql -h$H -u$U -p$P < DataInsert.sql
mysql -h$H -u$U -p$P < Object.sql

