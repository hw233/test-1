date=`date "+%Y, %m, %d"`
sed -i -e  '/setOpening/d' ../conf/config.lua
sed -i -e "$ a cfg:setOpening($date);" ../conf/config.lua
