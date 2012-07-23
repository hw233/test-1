
cat actmsg.txt| awk '{printf("msg_%d = \"%s\"\n", NR, $0);}' > actmsg_vt.lua
cat taskmsg.txt | awk '{printf("task_msg_%d = \"%s\"\n", NR, $0);}' > task_msg_vt.lua
#awk '{}' taskmsg.txt 
