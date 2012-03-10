#!/bin/bash
source conf.sh

source data.sh
source object.sh

mysql -h$H -u$U -p$P -P$PT $DBO -e "update player set packsize=packsize+50;"

