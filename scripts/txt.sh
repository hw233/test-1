cat actmsg.lua | awk -F\" '{print $2}' > actmsg.txt 
cat taskmsg.lua | awk -F\" '{print $2}' > taskmsg.txt 
cp actmsg.txt taskmsg.txt /opt/publish
sz actmsg.txt taskmsg.txt
