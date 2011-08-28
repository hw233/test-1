#!/bin/bash

H=localhost
U=asss
P=asss
PT=3306
DBD=data
DBO=asss

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

