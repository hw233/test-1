#!/bin/bash

H=192.168.88.250
U=kingxin
P=kingxin
PT=3306

<<<<<<< HEAD
DBD=data_jlt;
DBO=asss_jlt;
=======
DBD=data_jlt
<<<<<<< HEAD
DBO=asss_jlt
>>>>>>> 9a2af98... clanCopy (Wait for testing.)
=======
DBO=asss_s110
>>>>>>> 3c77e0c... clanCopy set tick=5.

OPTIONS="Yes/No/Exit"
IFS=/

function selectAction()
{
echo -e "!!!!!!!!!!>>>>>> $1 <<<<<<!!!!!!!!!!"
select selected in $OPTIONS;do
    case $selected in
        Yes)
        return 1
        break
        ;;
        *)
        return 0;
        break;;
    esac
done
}

IFS=' '
