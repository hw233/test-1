#!/bin/bash

H=192.168.88.250
U=kingxin
P=kingxin
PT=3306

<<<<<<< HEAD
DBD=data_8833
DBO=asss_8833
=======
DBD=data_6666
DBO=asss_6666
>>>>>>> a752912150781a2c1034dae2979eefe163ae5678

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

