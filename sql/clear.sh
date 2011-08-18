#!/bin/bash

source conf.sh
mysql -h$H -u$U -p$P -P$PT -e "drop database $DBD; drop database $DBO;"
mysql -h$H -u$U -p$P -P$PT -e "set names utf8; create database $DBD;create database $DBO;"

