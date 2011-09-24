#!/bin/bash

H=localhost
U=kingxin
P=kingxin
PT=3306
DBL=log_8888

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

