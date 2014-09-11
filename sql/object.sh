#!/bin/bash
source conf.sh
function update_object()
{
    echo "update_object"
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/ClearQQTenpayAward.sql  # XXX: 清除QQ财付通奖励
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleargm.sql  # XXX: 清除所有GM权限，请一直保留
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/sysupdlg.sql # XXX: 更新公告弹窗，请一直保留
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearaward.sql # must be once for delete token
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartoken.sql # must be once for delete token
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clear3366totalrecharge.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartianjie.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearsingleparty.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121130_1.sql  #add table recharge_item
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121130_2.sql  # must be once for delete var total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121211_1.sql # must be once 场外活动
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20121228_1.sql # must be once for delete var total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearconsume.sql # must be once for delete total consume
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/cleartotalrecharge.sql # must be once for delete recharge total
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid50.sql # must be once 指定玩家非绑定的九龙神火改成绑定的
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearid295.sql # must be once 登录礼包人人有
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130225_1.sql          #帮派QQ群
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130315_1.sql # must be once 有删除操作
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130326_1.sql # must be once 有删除操作
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearqqvipdiamond.sql     #QQ会员活动需要打开 
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130403_1.sql # must be once 有删除操作
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130422_1.sql   #末日之战帮派增加xianyun和gongxian
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130425_1.sql   #末日之战帮派增加urge
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130510_1.sql   #清除消费抽奖(var142/348)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_fairy_spar.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearaccrecharge.sql   #清除累积充值记录
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130608_1.sql  #must be once强制阵旗，阵眼在交易区下架
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearbluediamond.sql  #蓝钻活动需要打开
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130712_1.sql #fighter表里面添加 innateTrump 字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_tempItem.sql #创建临时物品表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_jiguan.sql #机关兽创建player_jiguanshu、player_jiguanyu和player_tuzhi
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_alter_friendkey.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_collect_card.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clean_mofang.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < player_tuzhi.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < player_jiguanshu.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < player_jiguanyu.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_pray_relation.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_correct_pray_relation.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar289.sql        #清除副本阵图额外掉落绑定情况
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2013_08_29_1.sql  #清除QQ秀合作记录
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130902_1.sql  #机关兽创建player_keyin
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130913_1.sql  #三魂创建fairyPet_sanhun
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_2013_09_02_presentbox.sql  #创建礼品箱
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130922_1.sql  #机关兽创建player_zhenwei
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130926_1.sql #创建天书奇缘记录表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131018_2.sql   #lingbaosmelt 表中添加 counts、 purpleAdjVal、orangeAdjVal 三个字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar544_545.sql        #清除累积登陆(开启万圣节登陆有礼)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearyellowdiamond.sql  #黄钻活动需要打开
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_player_named.sql  #改名卡 加一张表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_GG_1111.sql 
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar630.sql        #清除可前进次数
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131112_1.sql     #修复168区玩家仙宠根骨异常问题(id:33569705)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearguanggun.sql     #修复光棍bug
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131101_1.sql    #七石斗法
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_add_acuGold_1125.sql     #武将添加本命金丹
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131203_1.sql   #fighter_xingchen 表中添加 gem4、gem5、gem6、xctCurVal、xctMaxVal 五个字段
    #. clearshiyamen.sh
    #mysql -h$H -u$U -p$P -P$PT $DBO < clanscore.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clan_battlescore.sql     #重置帮战积分
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_marriage.sql     #创建征婚薄
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_reply_marriage.sql  #创建征婚回复表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_marry_log.sql     #创建征婚log表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_married_log.sql     #创建结婚log表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140109_1.sql     #修复仙界8强之前押注未结算的问题
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140114.sql   #去除夫妻家园表中的在线时长字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_setVar636.sql    #qq浏览器补签1月3 4 5 号
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_married_couple.sql     #创建结婚log表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20131212_clear636and637.sql   #清空VAR（QQ大厅登录） 2014年1月17号活动
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140101_1.sql     #仙宠组队副本
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140101_2.sql     #仙宠组队副本击杀橙色怪物日志
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_sevensoul.sql  #创建七魄
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_SetGVar79_Marry.sql  #设置夫妻对数
    
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar199.sql        #清除强化装备的8次限制
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clearGuangGun.sql        #清除光棍节情义罗盘相关数据(为开服活动做准备)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140214_1.sql     #墨宝
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140214_2.sql   #藏剑崖
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_sevensoul_bug.sql     #七魄bug
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140307_1.sql   #clan表中添加duobaoAward字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140307_2.sql   #夺宝日志
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140123_1.sql     # 创建帮派建筑表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/changevar174.sql        #bug(达到充值额度的次)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_AddFighterXinMo_0317.sql        #添加散仙心魔值
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140318_1.sql   #clan表中添加tyssAward字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140318_2.sql   #清除天元神兽var数据
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_skillgrade.sql     #创建技能升阶
    #增加的表中如果有fighterId，记得在useChangeSexCard函数中添加修改主将ID的函数
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140401_1.sql         #璇玑阵图
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140403_1.sql   #clan表中添加iclantitleAll字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140403_2.sql   #修改clanTitleAll
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140408_1.sql   #修改strengthenData字段flag
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140328_1.sql   #被活跃玩家邀请过的回流玩家
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140328_2.sql   #回流玩家申请加入队伍列表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140328_3.sql   #队伍数据
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140328_4.sql   #本次活动开启回流玩家数据
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_cardsystem.sql     #创建卡牌系统
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_CollectCnt_20140414.sql     #创建卡牌之魂
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_3_24_CreateFriendlyCount.sql     #创建友好度表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_alter_card.sql     #创建卡牌系统
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_lingshi.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearautobattle.sql #清除自动挂机
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140510_1.sql     #创建帮派篝火
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140515.sql     #卡牌优化
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_lingshi_bug.sql     #灵侍bug
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clearvar592_1211.sql   #清空var592（QQ大厅登录）
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_reset_newyeargift_var.sql  #清除春节礼包(var349)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140523_1.sql #一战成名每天题目
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140418_1.sql   #修改数据库世界boss物攻、法功
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_clanbigboss.sql   #增加帮派boss
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_Create_pictureAttr.sql     #惊羽楼
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140610_WorldCup.sql     #世界杯
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140613_1.sql     #清除散仙等级小于85级穿戴的灵侍
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clear_var291.sql   #清除3366礼包购买次数
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/GObject_GVar102.sql     #世界最高等级
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140617_1.sql     #帮贡商店
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140620_1.sql #修复帮贡商店var776
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_201400623_Cp599To817.sql     #天书奇缘拷贝积分599到817
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_AriBook_Bug.sql     #天书奇缘时间段积分还原
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140616_cXXL.sql     #快乐消消乐
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_ClanBigBossBug.sql     #帮派BossBug修复
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140627_1.sql #机关库
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clear_var402_403.sql   #这“夏”有礼咯
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_alter_jiguanyu.sql   #player_jiguanyu表中添加字段scheme
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140623.sql     #帮派加入优化
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_Snow_Bug.sql     
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clearcarditem.sql     #清除称号卡物品
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_alter_jiguanyu_1.sql   #修改player_jiguanyu表主索引
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_tyss_buff6b.sql     #天元神兽阳光普照buff
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140721_1.sql #蜀山微微码上有奖发布微博周期一周变一天
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140707_cKettle.sql     
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clear_qixi.sql
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clear651.sql     #清空VAR(大转盘)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clearOldMan.sql     #捉迷藏清空
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_seekingher.sql     #众里寻他送豆记录表,player表增加宣言字段
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140731_clear_var790.sql     #清除文采值
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_drop_packSizeLS.sql #drop column packSizeLS of player
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140804_aLingbao.sql #create fighter_lingbaoLevel
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140804_aLingbaoFall.sql #create fighter_lingbaoFall
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_modify_clan_founder.sql #modify clan founder
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_AriBook_1121.sql     #天书奇缘清空
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140811_1.sql     #跨服修炼地修改clan,practicce_place表
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar182_187.sql        #清除仗剑江湖蜀山路
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20130429_1.sql   #中秋节节日套装(var294)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140818_addInsensevim.sql     #添加心法增幅(燃香)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearxyx.sql #清除幸运星购买次数，开启幸运星活动时开启(Q点直购)
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140820_clearGvar95_99.sql     #清除一战成名时间控制
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_20140422_1.sql     #周年好礼送不停
    #mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_create_shakemoneybag.sql     #创建狂欢大购物摇钱袋记录表
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar606.sql        #清除掉落活动
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar844.sql        #众里寻他掉落状态
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/clearvar182_187.sql        #清除仗剑江湖蜀山路
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_clearCoolSummer.sql     #清除酷爽一夏
    mysql -h$H -u$U -p$P -P$PT $DBO < updates/Object_serverwar_clearstate.sql     #服战押注没结算
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
