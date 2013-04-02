#!/bin/bash
source conf.sh
function update_object()
{
    echo "update_object"
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleargm.sql  # XXX: 清除所有GM权限，请一直保留
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
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar199.sql        #清除强化装备的8次限制
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
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar199.sql        #清除强化装备的8次限制
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130114_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130116_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130116_2.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130124_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar_20130118.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar199.sql          #清除强化装备的8次限制
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130225_1.sql          #帮派QQ群
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130304_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130304_1.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130225_1.sql          #帮派QQ群
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130315_1.sql # must be once 有删除操作
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130319_1.sql 
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130326_1.sql # must be once 有删除操作
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearbluediamond.sql # must be once for delete recharge total

    #. clearshiyamen.sh
}

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
