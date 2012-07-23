#!/bin/bash

H=192.168.88.250
U=kingxin
P=kingxin
PT=3306

<<<<<<< HEAD
DBD=suntao_Data
DBO=suntao_Object
=======
DBD=data_8888
DBO=asss_8888
>>>>>>> d2ab1f1841964889069487955e22c7256c1ae556

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

