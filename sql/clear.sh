#!/bin/bash

source conf.sh
mysql -h$H -u$U -p$P -e "drop database data; drop database asss;"

