#!/bin/bash

H=192.168.88.250
U=kingxin
P=kingxin
PT=3306

#请勿改动该文件，请将个人配置存放到 conf.user.sh 文件中
#如果文件不存在，则使用8888的默认配置
# conf.user.sh 文件请勿提交入 git 仓库
DBD=data_8888
DBO=asss_8888

if [ -f conf.user.sh ] ; then
    source conf.user.sh
fi

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
