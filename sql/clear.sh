#!/bin/bash

source conf.sh
mysql -h$H -u$U -p$P -e "drop database $DBD; drop database $DBO;"
mysql -h$H -u$U -p$P -e "create database $DBD;create database $DBO;"

