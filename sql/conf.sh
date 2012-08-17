#!/bin/bash

H=192.168.88.250
U=kingxin
P=kingxin
PT=3306

DBD=data_3322
DBO=asss_3322

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
