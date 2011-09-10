#!/bin/bash
source conf.sh
function update()
{
    mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110905_1.sql
}

selectAction $DBO
if [ $? -eq 1 ]; then
    echo "Updating..."
    update
    echo "Updated!!!"
else
    echo "Cancelled!!!"
fi

