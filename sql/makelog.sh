#!/bin/bash

source log_conf.sh
mysql -h$H -u$U -p$P -P$PT -e "set names utf8; create database if not exists $DBL;"
mysql -h$H -u$U -p$P -P$PT -D$DBL < DataLog.sql

