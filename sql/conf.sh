#!/bin/bash

H=localhost
U=kingxin
P=kingxin
PT=3306
DBD=data_7777
DBO=asss_7777

OPTIONS="Yes/No/Exit"
IFS=/

function selectAction()
{
echo -e "Update !!!!!!!!!!>>>>>> $1 <<<<<<!!!!!!!!!!"
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

