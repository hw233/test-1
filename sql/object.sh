#!/bin/bash
source conf.sh
function update_object()
{
    echo "update_object"
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/ClearQQTenpayAward.sql  # XXX: 清除QQ财付通奖励
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleargm.sql  # XXX: 清除所有GM权限，请一直保留
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/sysupdlg.sql # XXX: 更新公告弹窗，请一直保留
    #mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110918_1.sql
    #mysql -h$H -u$U -p$P -P$PT -D$DBO < updates/Object_20110913_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111010_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111021_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111024_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111031_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2011101101_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111109_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111114_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111115_1.sql
    
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111202_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111207_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111122.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111129_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111214.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111219.sql
    #
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111219_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111110_1.sql
    #
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111217_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20111227_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120111_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120116_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120129_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120214_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120214_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120227_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120301_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120307_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120314_1.sql # must be once
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120316_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120317_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120405_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120406_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120409_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120416_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120417_1.sql # must be once reset world boss time
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120423_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120427_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120503_1.sql # must be once -- fb
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120504_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120508_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120508_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120509_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120608_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120515_1.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/towndeamon_player_fix.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120613_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120619_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120622_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120720_1.sql 
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120703_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120706_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120709_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120629_2.sql  #must be once for update new_relation
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120713_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120720_2.sql   #must be once for soso (var 141)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120722_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120731_1.sql # must be once for consume (var 142)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120730_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120813_1.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120816_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120820_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120831_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120831_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120903_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120911_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120915_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121011_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20120928_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121015_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121015_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121017_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121017_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121012_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121022_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121025_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121029_1.sql # must be once for delete qixi total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121029_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121031_1.sql # must be once for delete var total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121108_1.sql # must be once for delete var total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121111_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121111_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearaward.sql # must be once for delete token
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121122_1.sql # must be once for record ganenjie active
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearbluediamond.sql # must be once for delete recharge total

    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartotalrecharge.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearconsume.sql # must be once for delete total consume
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartoken.sql # must be once for delete token
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clear3366totalrecharge.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartianjie.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearsingleparty.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121122_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121123_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121126_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121128_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121130_1.sql  #add table recharge_item
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121130_2.sql  # must be once for delete var total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121206_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121211_1.sql # must be once 场外活动
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121217_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121221_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121228_1.sql # must be once for delete var total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearconsume.sql # must be once for delete total consume
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartotalrecharge.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid50.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid49.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid514.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid133.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid511.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid1327.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid1326.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid507.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid509.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid400.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5001.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5011.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5021.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5031.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5041.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5051.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5061.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5071.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5081.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5091.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5101.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5111.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5121.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5131.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid5141.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130115_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130118_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid295.sql # must be once 登录礼包人人有
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar_20130118.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130114_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130116_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130116_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130124_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar_20130118.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130225_1.sql          #帮派QQ群
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130304_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130304_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130225_1.sql          #帮派QQ群
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130315_1.sql # must be once 有删除操作
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130319_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130326_1.sql # must be once 有删除操作
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearqqvipdiamond.sql     #QQ会员活动需要打开 
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130403_1.sql # must be once 有删除操作
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130408_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar199.sql        #清除强化装备的8次限制
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130422_1.sql   #末日之战帮派增加xianyun和gongxian
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130425_1.sql   #末日之战帮派增加urge
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130510_1.sql   #清除消费抽奖(var142/348)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_fairy_spar.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearaccrecharge.sql   #清除累积充值记录
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130524_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130530_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130607_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130607_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130608_1.sql  #must be once强制阵旗，阵眼在交易区下架
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130609_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130620_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearbluediamond.sql  #蓝钻活动需要打开
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130704_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearyellowdiamond.sql  #黄钻活动需要打开
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130712_1.sql #fighter表里面添加 innateTrump 字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130721_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_tempItem.sql #创建临时物品表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130805_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clear_qixi.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130802_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_set_gvar51_52.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_jiguan.sql #机关兽创建player_jiguanshu、player_jiguanyu和player_tuzhi
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_alter_friendkey.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_alter_friendkey.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_collect_card.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clean_mofang.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < player_tuzhi.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < player_jiguanshu.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < player_jiguanyu.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_pray_relation.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_correct_pray_relation.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130429_1.sql   #清除节日套装(var294)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2013_08_29_1.sql  #清除QQ秀合作记录
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar289.sql        #清除副本阵图额外掉落绑定情况
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2013_08_30_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2013_08_29_1.sql  #清除QQ秀合作记录
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130902_1.sql  #机关兽创建player_keyin
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130913_1.sql  #三魂创建fairyPet_sanhun
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2013_09_02_presentbox.sql  #创建礼品箱
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar182_187.sql        #清除仗剑江湖蜀山路
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130922_1.sql  #机关兽创建player_zhenwei
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130926_1.sql #创建天书奇缘记录表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar199.sql        #清除强化装备的8次限制
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131001_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131018_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131018_2.sql   #lingbaosmelt 表中添加 counts、 purpleAdjVal、orangeAdjVal 三个字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar544_545.sql        #清除累积登陆(开启万圣节登陆有礼)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130429_1.sql   #清除节日套装(var294)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearyellowdiamond.sql  #黄钻活动需要打开
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_player_named.sql  #改名卡 加一张表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_GG_1111.sql 
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar630.sql        #清除可前进次数
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131112_1.sql     #修复168区玩家仙宠根骨异常问题(id:33569705)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearguanggun.sql     #修复光棍bug
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_AriBook_1121.sql     #天书奇缘清空
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131101_1.sql    #七石斗法
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_add_acuGold_1125.sql     #武将添加本命金丹
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clan_battlescore.sql     #重置帮战积分
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131201_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131203_1.sql   #fighter_xingchen 表中添加 gem4、gem5、gem6、xctCurVal、xctMaxVal 五个字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clearvar592_1211.sql   #清空var592（QQ大厅登录）
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131212_clear636and637.sql   #清空var592（QQ大厅登录）
    #. clearshiyamen.sh
    #mysql -h$H -u$U -p$P -P$PT $DBO < clanscore.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clan_battlescore.sql     #重置帮战积分
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearxyx.sql #清除幸运星购买次数，开启幸运星活动时开启(Q点直购)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clear_var291.sql   #清除3366蓝砖礼包购买次数
}

#sh player_jiguan.sh
#sh clanscore.sh
update_object
#selectAction $DBO
#if [ $? -eq 1 ]; then
#    echo "Updating..."
#    update_object
#    echo "Updated!!!"
#else
#    echo "Cancelled!!!"
#fi
#
