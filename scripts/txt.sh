cat actmsg.lua | awk -F\" '{print $2}' > actmsg.txt 
cat task_msg.lua | awk -F\" '{print $2}' > taskmsg.txt 
sz actmsg.txt taskmsg.txt
