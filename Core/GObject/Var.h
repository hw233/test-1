
#ifndef _VAR_H_
#define _VAR_H_

#include "Common/TimeUtil.h"
#include "Common/Mutex.h"

namespace GObject
{
    //变量周期类型
    enum CycleType
    {
        CYCLE_NONE =    0,  //无周期
        CYCLE_MIN =     1,  //分
        CYCLE_HOUR =    2,  //时
        CYCLE_DAY =     3,  //日
        CYCLE_WEEK =    4,  //周
        CYCLE_MONTH =   5,  //月
        CYCLE_YEAR =    6,  //年

        CYCLE_MAX,
    };


    enum VarID
    {
        VAR_INVALID = 0,    //无效变量
        VAR_SINGLE_CHOPSTICKS = 1, //光棍节送筷子
        VAR_SENGYI = 2,     //僧衣
        VAR_APRON = 3,      //肚兜
        VAR_TGDT = 4,       //感恩节
        VAR_TGDCNT = 5,     //感恩节连续在线3小时次数
        VAR_ATOFM = 6,     //自动阵图id
        VAR_ICEMAN = 7,     //雪人
        VAR_BOOT = 8,     //圣诞靴
        VAR_TODAY_ONLINE = 9,     // time -- online today
        VAR_KEYPACK1 = 10,  // KEY礼包
        VAR_KEYPACK2 = 11,
        VAR_KEYPACK3 = 12,
        VAR_KEYPACK4 = 13,
        VAR_KEYPACK5 = 14,
        VAR_KEYPACK6 = 15,
        VAR_KEYPACK7 = 16,
        VAR_KEYPACK8 = 17,

        VAR_FAIL_ENCH = 20,  // fail to enchat equip
        VAR_SPLIT_EQUIP = 21 , //split equip times
        VAR_SPLIT_THRUMP = 22,//split thrump times
        VAR_BATTLE_MISS = 23, //靠靠靠
        VAR_BATTLE_CS  = 24,  //
        VAR_BATTLE_PR = 25,   //靠靠靠
        VAR_BATTLE_FJ = 26,  //靠靠靠
        VAR_BATTLE_SKILL_DMG = 27, //靠靠靠
        VAR_BATTLE_PEERLESS_DMG = 28,//靠靠靠靠

        VAR_COUNTRY_BATTLE_WIN =29,
        VAR_ATHLETICS_WIN = 30 ,

        VAR_TASK_SUBMITTED = 31,
        VAR_YAMEN_TASK_SUBMITTED = 32,
        VAR_SHIMEN_TASK_SUBMITTED = 33,
        VAR_CLAN_TASK_SUBMITTED  = 34,

        VAR_PEXP_GET = 35,
        VAR_PURPLE_EQUIP_NUM = 36,
        VAR_YELLOW_EQUIP_NUM = 37,
        VAR_YELLOW_THRUMP_NUM= 38,

        VAR_AWARD_3366 = 39,

        VAR_CLANBATTLE_HONOUR = 40, //帮派战今日贡献
        VAR_CLAN_WEAL = 41,         //今日领取的福利数

        VAR_NEWYEAR_PACK = 42, // 新年活动等级(30级)礼包

        VAR_DAILY_CLANBATTLE = 43,  //每日参加帮派战
        VAR_WEEKLY_CLANBATTLE = 44, //每周帮派战

        VAR_BLUE_ACTIVE_GET = 45, //蓝钻登陆1小时在线奖励
        VAR_RECHARGE_TOTAL = 46, //充值活动

        VAR_LOVER_RING = 47, //情人之戒
        VAR_SHUSAN_LOVE = 48, //蜀山之恋
        VAR_LOGIN_PF = 49, //每日平台登陆宝箱
        VAR_GIRL_PACK = 50, //快乐女生礼包使用个数
        VAR_OFFLINE = 51,//下线时时间
        VAR_OFFLINE_EXP = 52,//离线经验
        VAR_OFFLINE_PEXP = 53,//离线修为，分钟
        VAR_TOWNDEAMON = 54, //锁妖塔战斗一次
        VAR_GOLD_TOKEN = 55, //金代币 先天灵石
        VAR_TAEL_TOKEN = 56, //银代币 盘古灵石
        VAR_COIN_TOKEN = 57, //铜代币 轩辕灵石
        VAR_FOOLS_DAY = 58, //愚人节礼包是否领取  愚公移山(byte0失败题目id,byte1领取奖励的题目数,byte2购买复活卡次数)
        VAR_CBWHITEPACK = 59, //阵营战白色情人节礼包

        VAR_ATYITEM_1 = 60, //活跃度星期一奖励
        VAR_ATYITEM_2 = 61, //活跃度星期二奖励
        VAR_ATYITEM_3 = 62, //活跃度星期三奖励
        VAR_ATYITEM_4 = 63, //活跃度星期四奖励
        VAR_ATYITEM_5 = 64, //活跃度星期五奖励
        VAR_ATYITEM_6 = 65, //活跃度星期六奖励
        VAR_ATYITEM_7 = 66, //活跃度星期天奖励

        VAR_CLAN_LEAVE_TIME = 67, //退出帮派时间
        VAR_FIGHTER1_3_6_8 = 68, // 繁体版主将潜力值达到1.3，资质达到6.8活动是否达成

        VAR_CFRIEND_1 = 70, //密友等级相应个数 1级
        VAR_CFRIEND_45 = 71,
        VAR_CFRIEND_60 = 72,

        VAR_DISCOUNT_1 = 80, // 3折区购买次数
        VAR_DISCOUNT_2 = 81, // 5折区购买次数
        VAR_DISCOUNT_3 = 82, // 8折区购买次数

        VAR_CTSLANDING = 83, // 创建角色前七天连续登陆记录
        VAR_CTSAWARD = 84, // 七天登陆奖励
        VAR_CLAWARD = 85, // 七天连续登陆奖励
        VAR_CL3DAY = 86, // 七天连接三天登陆奖励
        VAR_RC7DAYRECHARGE = 87, // 创建角色后前七天充值总额
        VAR_RC7DAYWILL = 88, // 创建角色后前七天充值奖励领取（许愿类型 << 8 | 点亮龙珠数量）
        VAR_INVITES = 89, // 邀请好友的人数
        VAR_MDSOUL = 90, // 五一活动灵气值
        VAR_MDSOUL_CNT1 = 91, // 五一活动风雷宝珠使用次数 9000
        VAR_MDSOUL_CNT2 = 92, // 雷之石 497
        VAR_MDSOUL_CNT3 = 93, // 风之石 496
        VAR_MDES = 94, // 五月份活动节日套装
        VAR_MDPACKS = 95, // 五月份活动礼包是否发放
        VAR_RC7DAYTURNON = 96,
        VAR_DIAMOND_YELLOW = 97,//黄钻特权
        VAR_YBBUF = 98, // 蓝黄钻之力是否领取
        VAR_LDPOINT = 99, // 夺宝积分，整数类型，可正可负，非绑定
        VAR_LDCOUNT = 100, // 夺宝次数，触发后清空
        VAR_LDPOINTB = 101, // 夺宝积分，整数类型，可正可负，绑定

        VAR_AWARD_QPLUS = 102, // QPLUS等级奖励
        VAR_AWARD_SSDT_1 = 103, // 搜搜地图点亮旗帜每日奖励
        VAR_AWARD_SSDT_2 = 104, // 搜搜地图点亮旗帜达成奖励

        VAR_DIAMOND_BLUE = 105,  //蓝钻特权
        VAR_NETVDPACK = 106, // 网络情人节礼包是否发放
        VAR_LASTATHRANK = 107, // 昨天结算前的斗剑排名
        VAR_SHUOSHUO = 108, // 今天领取过说说奖励次数
        VAR_INVITED = 109, // 今天领取过邀请好友奖励次数    //停用[suntao:2013-1-15]

        VAR_JUNE = 110,      // 六月礼包是否发放
        VAR_JUNE_EQUIP = 111, // 六月节日套装是否发放
        VAR_JUNE_ITEM = 112, // 欢乐礼包是否领取
        VAR_JUNE_RECHARGE_TOTAL = 113, //六一充值
        VAR_JUNE_HAPPY = 114, //六一欢乐值
        VAR_JUNE_HAPPY_ITEM_CNT = 115, // 六一使用棒棒糖数
        VAR_LUCKYDRAW_CNT = 116, // 寻宝次数

        VAR_CLAWARD2 = 117, // 七天目标抽奖之今日目标

        VAR_OFFLINE_EQUIP = 118, //离线道具

        VAR_ENCHANTGT11 = 119, // 限时强化一次性返还是否返过
        VAR_INRF7DAY = 120, // 回流7日活动开始时间
        VAR_CTSLANDINGRF = 121, // 回流7日连续登陆7天记录
        VAR_RF7DAYRECHARGE = 122, // 回流后前七天充值总额
        VAR_RF7DAYWILL = 123, // 回流后所选择的愿望索引
        VAR_RF7DAYTURNON = 124, // 回流充值刻度点亮
        VAR_CTSAWARDRF = 125, // 回流后七天登陆抽奖
        VAR_CLAWARDRF = 126, // 回流后七天连续登陆奖励
        VAR_CL3DAYRF = 127, // 回流后七天连接三天登陆奖励

        VAR_MONEY_ARENA = 128, // 剑魂
        VAR_MONEY_ARENA2 = 129, // 入围赛获得剑魂
        VAR_MONEY_ARENA3 = 130, // 32强赛获得剑魂
        VAR_LX_CNT = 131, //冷血,鸡蛋小仙
        VAR_WQ_CNT = 132, //无情,油条师兄
        VAR_TS_CNT = 133, //铁手,包子智者
        VAR_ZM_CNT = 134, //追命,面馆BOSS

        VAR_DUANWU = 135, // 端午上线邮件
        VAR_JULY = 136, // 七月礼包
        VAR_2NDSOUL = 137, // 元神吞噬奖励是否发放
        VAR_QQVIP = 138, //QQ会员
        VAR_VIPFIRST  = 139, // 是否领取VIP首充礼包
        VAR_PEXPITEMS = 140, // 丹使用量
        VAR_SOSOMAPAWARD = 141, // SOSO地图登陆奖励记录
        VAR_CONSUME = 142, // 消耗累积

        VAR_PHYSICAL_BUY = 143, //购买体力限制
        VAR_QQVIP_BUF = 144,    //QQ会员之力是否领取
        VAR_QQVIP_CNT = 145,    //QQ会员特权

        VAR_DISCOUNT_SP_1 = 146, // 原3折区域，增加活动限购后的购买次数
        VAR_DISCOUNT_SP_2 = 147, // 原5折区域，增加活动限购后的购买次数
        VAR_DISCOUNT_SP_3 = 148, // 原8折区域，增加活动限购后的购买次数

        VAR_DISCOUNT_1_TIME = 149,
        VAR_DISCOUNT_2_TIME = 150,
        VAR_DISCOUNT_3_TIME = 151,

        VAR_DISCOUNT_SP_1_TIME = 152,
        VAR_DISCOUNT_SP_2_TIME = 153,
        VAR_DISCOUNT_SP_3_TIME = 154,

        VAR_AWARD_NEWREGISTER  = 155, //新用户注册抽奖

        VAR_TJ_TASK_PRESTIGE=156,   //天劫活动中的声望
        VAR_TJ_TASK1_NUMBER=157,    //天劫每日任务1的已完成次数
        VAR_TJ_TASK2_TAEL=158,      //天劫任务2捐献的银币积分
        VAR_TJ_TASK2_GOLD=159,      //天劫任务2捐献的仙石积分
        VAR_TJ_TASK2_COUPON=160,    //天劫任务2捐献的礼券积分
        VAR_TJ_TASK2_TJYJ=161,      //天劫任务2捐献的天劫印记
        VAR_TJ_TASK2_SCORE=162,     //天劫任务2的积分
        VAR_TJ_TASK3_COPYID=163,    //天劫任务3当前破阵层数
        VAR_TJ_EVENT_PRESTIGE=164,  //天劫事件中的声望

        VAR_RP_VALUE=165,           //周年庆回流用户礼包
        VAR_RP_REWARD_GOT=166,      //回流用户奖励,是否已领取
        VAR_CONSUME_918 = 167, // 9/18-9/24活动 消耗累积
        VAR_TJ_TOWN_ITEM_GOT=168,   //锁妖塔的天劫物品领取标志

        /** 结束 **/
        VAR_RECHARGE_TOTAL3366 = 169, //3366合作充值活动
        VAR_BDIAMOND_CMD = 170, //蓝钻用户每日可领取当前等级寻宝令

        VAR_DONTIAN_KILLPOINT = 171, //洞天福地击杀点数

        VAR_YEAR_SWORDSMAN = 180, //周岁剑侠红包
        VAR_YEAR_NOBLE = 181, //周岁贵族红包
        /** 斩妖除魔剑侠路 **/
        VAR_ZYCM_POS = 182, //偏移
        VAR_XIAGU_CNT = 183, //侠骨个数
        VAR_ROUQING_CNT = 184, //柔情个数
        VAR_CAIFU_CNT = 185, //财富个数
        VAR_CHUANQI_CNT = 186, //传奇个数
        VAR_ZYCM_TIPS = 187, //随机数1～3，为TIPS

        VAR_QGAME_GIFT = 188, //Qgame大厅礼包
        VAR_AWARD_LOGIN = 189, //2012.10.14登录抽奖合作
        VAR_AWARD_BIRTHDAY     = 190, //周年庆活动(生日罗盘许愿星)
        
        VAR_THIEF_CAMP     = 191, //贼兵营地(新手任务中使用)
        VAR_FOR_OLDUSER    = 192, //兼容老区老用户的新手任务
        
        VAR_YUEBING_USED = 193, //中秋国庆使用月饼数量
        VAR_MAX_CLAN_COPY_LEVEL = 194, // 本周已通关的最大层数
        VAR_TOWN_REWARD = 195, //镇妖封神 修仙夺宝 10-15--10-18
        VAR_BLUE_AWARD_COUNT = 196 ,//蓝钻抽奖次数
        VAR_YELLOW_AWARD_COUNT = 197, //黄钻抽奖次数

        VAR_WANSHENG_ITEM = 198, // 万圣节是否领取
        VAR_EQUIP_MOVE_COUNT = 199,

        // TGC活动
        VAR_TGC_1_LOGIN = 200,
        VAR_TGC_1_COPY = 201,
        VAR_TGC_1_FRONTMAP = 202,
        VAR_TGC_1_DUNGEON = 203,

        VAR_CTSLANDING_AWARD = 204, // 创建角色前七天连续登陆奖励领取记录
        VAR_CTSLANDING_AWARD2 = 205, // 创建角色前七天累计登录奖励领取记录
        VAR_CTS_TARGET_COUNT  = 206, // 七天目标抽奖之剩余抽奖次数

        VAR_ITEM_9178_FAILED_COUNT = 207, // 都率宝伞礼包连续开启失败次数
                                          // （ItemNormal.lua里面值已经写死）
        VAR_USETAEL_CNT = 208, //玩家每日使用银币的总数
        VAR_USECOUPON_CNT = 209, //玩家每日使用礼券的总数
        VAR_USEGOLD_CNT = 210,  //玩家每日使用仙石的总数
        VAR_GETACHIEVEMENT_CNT = 211, //玩家每日获得的荣誉总数
        VAR_GETPRESTIGE_CNT = 212, //玩家每日获得的声望总数

        VAR_ITEM_9279_FAILED_COUNT = 213, // 锦云兜·壁礼包连续开启失败次数
        VAR_ITEM_9280_FAILED_COUNT = 214, // 锦云兜·破礼包连续开启失败次数
        VAR_ITEM_9281_FAILED_COUNT = 215, // 锦云兜·龙礼包连续开启失败次数
        VAR_ITEM_9282_FAILED_COUNT = 216, // 锦云兜·墨礼包连续开启失败次数

        VAR_LOCAL_RANK = 240, //本服斗剑，当前排名
        VAR_LOCAL_MAXRANK = 241, //本服斗剑，最好排名
        VAR_LOCAL_PRESTIGE = 242, //本服斗剑，声望数
        VAR_LOCAL_PAGE = 243, //本服斗剑，翻页数

        VAR_9215_USED = 244,  //逍遥礼包使用数
        VAR_QQVIP_AWARD_COUNT = 245 ,//qq会员抽奖卷
        VAR_AWARD_SSTOOLBAR = 246,   //搜搜工具栏奖励是否已领取
        
        VAR_EX_JOB_ENABLE = 247,   // 墨家面板是否开启
        VAR_JOB_HUNTER_SPOT_ID = 248,   // 寻墨据点据点id
        VAR_EX_JOB_HAS_HAD     = 249,   // 是否招募过墨家散仙

        //250-269 越南版已占用

        // 270-280 占用 for zhgc
        VAR_ACT_LOGIN_AWARD     = 270, // 节日每日登陆奖励普通
        VAR_ACT_LOGIN_AWARD_VIP = 271, // 节日每日登陆奖励VIP
        VAR_VIP_PRIVILEGE_TIME  = 272, // VIP特权剩余时间
        VAR_VIP_PRIVILEGE_DATA  = 273, // VIP特权数据

        // 281-290 占用 fro qiwy
        VAR_ARENA_SUPPORT = 281, //场外活动，支持的序列id
        //VAR_ARENA_SUFFERED = 282, //场外活动，被陷害的次数
        //VAR_ARENA_LASTTIME = 283, //场外活动，最后被陷害的时间
        VAR_ARENA_SUPPORT_TUE = 284, //场外活动，星期二支持的玩家
        VAR_ARENA_SUPPORT_WED = 285, //场外活动，星期三支持的玩家
        // 首充礼包
        VAR_FIRST_RECHARGE_VALUE = 286, //充值数
        VAR_FIRST_RECHARGE_STEP = 287, //已领阶段
        VAR_CF_FLAG = 288, //是副本，还是阵图
        VAR_CF_BIND = 289, //是否绑定，低1位表示副本，低2位表示阵图，数字1表示非绑定，数字0表示绑定，低5，6位为1，2的备份
        VAR_GOOD_VOICE = 290, //是否领取蜀山好声音
        VAR_3366GIFT = 291, //3366合作礼包
        VAR_CF_INDEX = 292, //副本、阵图通关索引，从1开始
        VAR_CF_LOCATION = 293, //副本、阵图据点索引
        VAR_FEAST_LOGIN = 294, //节日套装人人拿
        VAR_FEAST_GIFT = 295, //登录礼包人人有

        VAR_FAIRYPET_LONGYUAN = 296,   //仙宠龙元数值
        VAR_FAIRYPET_FENGSUI = 297,  //仙宠凤髓数值
        VAR_FAIRYPET_XIANYUAN = 298, //仙宠空间仙缘数值
        VAR_FAIRYPET_STEP = 299, //仙宠空间当前寻宠步数
        VAR_FAIRYPET_LIKEABILITY = 300, //仙宠空间仙宠好感度数值(紫色)

        //繁体版占用301-330

        //340简体版
        VAR_RECHARGE_SCORE = 340, //累计充值的积分
        VAR_JOYVIP_BUFF_GOT     = 341, //心悦Buff
        VAR_QQGAME_GIFT_1218 = 342,    //QQGame登录奖励 1218-1220
        VAR_TOWER_LOGIN = 343,          // 楼一平台登录奖励
        VAR_TOWER_LEVEL = 344,          // 楼一平台登录等级奖励
        VAR_SNOW_AWARD     = 345,     //堆雪人奖励
        VAR_HAS_MO_BATTLER = 346,       // 出战散仙里是否有墨家
        VAR_ITEM_9316_FAILED_COUNT = 347, // 都率宝伞礼包连续开启失败次数
        VAR_CONSUME_AWARD_COUNT = 348,   //消费抽奖已抽次数
        VAR_NEW_YEAR_GIVE_GIFT = 349,   //迎新纳福，红包入袋(bit1~10依次代表对应的天数是否领取过奖励)
        VAR_FAIRYPET_ISGET_PET = 350,   //50级以上玩家是否免费领取过仙宠

        // 351~360 占用 for JLT
        VAR_DISCOUNT_CONSUME1  = 351, // 消费限购的金额
        VAR_DISCOUNT_CONSUME2  = 352, // 消费限购的金额
        VAR_DISCOUNT_CONSUME3  = 353, // 消费限购的金额
        VAR_DISCOUNT_RECHARGE1 = 354, // 充值限购的金额
        VAR_DISCOUNT_RECHARGE2 = 355, // 充值限购的金额
        VAR_DISCOUNT_RECHARGE3 = 356, // 充值限购的金额
        VAR_DISCOUNT_EX1_TIME  = 357, // 限购栏一过期时间
        VAR_DISCOUNT_EX2_TIME  = 358, // 限购栏二过期时间
        VAR_DISCOUNT_EX3_TIME  = 359, // 限购栏三过期时间

        // 361-370 占用 for suntao
        VAR_DRAGONKING_STEP = 361,      //大闹龙宫当前移动到的格子
        VAR_DRAGONKING_STEP4_COUNT = 362, //大闹龙宫从第4格移动到第5失败的次数
        VAR_SAVEGOLD_COUNT = 363,   //金蛇献瑞,聚福兆祥活动中玩家存仙石数量
        VAR_SAVEGOLD_SET_TIME = 364,    //金蛇献瑞,聚福兆祥活动中玩家设置的结算时间
        VAR_SAVEGOLD_GET_STATUS = 365,    //金蛇献瑞,聚福兆祥活动中玩家领取自己仙石的状态,低10位
        VAR_SAVEGOLD_ISGET = 366,   //金蛇献瑞,聚福兆祥活动中玩家当日是否领取
        
        VAR_CALLSNAKEEGG = 371,         //拜金蛇
        VAR_SNAKEEGG_AWARD=372,         //领蛇蛋
        VAR_9344_USED=373,
        VAR_9343_USED=374,
        VAR_SNAKE_SPRING_EQUIP_GOT=375,
        VAR_WEIBO_AWARD_GOT=376,
        VAR_FISHUSER_AWARD=377,            //捕鱼大亨用户领奖标志 
        //越南版380-399
        //400-420 for qiwy
        VAR_NEWYEAR_QQGAME_ACT = 401, //蓝钻新年回馈活动
        VAR_NEWYEAR_QZONECONTINUE_ACT = 402, //空间朋友连续登录奖励
        VAR_NEWYEAR_QZONECONTINUE_LASTTIME = 403, //空间朋友连续登录奖励
        VAR_EXP_HOOK_INDEX = 404, //1:TRAINP1(初级),2:TRAINP2(高级，包括TRAINP4、ADVANCED_HOOK(超值)),3:TRAINP3(齐天)
        VAR_TRAINP1 = 405,
        VAR_TRAINP2 = 406,
        VAR_TRAINP3 = 407,
        VAR_ONLINE_TOTAL_TIME = 408, //累计在线时间
        VAR_ONLINE_AWARD = 409, //累计在线奖励是否领取
        VAR_QZONE_QQGAME_ACT = 410, //空间黄钻(bit1表示普通，bit2表示会员)、大厅蓝钻礼包(bit3表示普通，bit4表示会员)

        //421-430 for suntao
        VAR_FOOLS_DAY_INFO  = 421,  //愚公移山答题信息(0位表示失败,1-30位表示题目id及是否答对)
        VAR_FOOLS_DAY_TIME  = 422,  //愚公移山答题开始时间
        VAR_INVITEDSUCCESS = 423, //邀请成功的好友人数
        VAR_CFRIENDTICKETS = 424, //好友邀请的抽奖券

        //431-440 for yijian
        VAR_LONGYUAN_GOT = 431,  //龙元风髓领取标志

        // 441-442 for JLT
        VAR_HAS_VOTE = 441,                 // 本日是否投票过
        VAR_POPULARITY = 442,               // 本周人气

        VAR_RP7_TREASURE = 432,  //回流服务器聚宝盆, 1-8位:是否购买的标志为 9-16,17-24,25-32位第1-3个聚宝盆的已领取次数
        VAR_RP7_TREASURE_TODAY_GOT=433, //第1-3位:各个聚宝盆今天是否已领取
        VAR_RP7_TREASURE1_GETTIME=434, //第1个聚宝盆的领取时间
        VAR_RP7_TREASURE2_GETTIME=435, //第2个聚宝盆的领取时间
        VAR_RP7_TREASURE3_GETTIME=436, //第3个聚宝盆的领取时间
        VAR_RP7_SIGN=437,              //注册签到
        VAR_RP7_SIGN_PACKAGE=438,      //连续签到礼包
        VAR_RP7_RECHARGE=439,      //开服7天充值金额

        VAR_FOOLBAO_USED = 450,  //愚人宝箱使用数
        VAR_HHBAWARD_GOT = 451 , //豪华蓝钻领取
        VAR_MAX,
    };

#define TARGET_LEVEL              0x0001
#define TARGET_ATHLETICSRANK      0x0002
#define TARGET_BOSS               0x0004
#define TARGET_COUNTRY_BATTLE     0x0008
#define TARGET_CLAN_BATTLE        0x0010
#define TARGET_TEAM_COPY          0x0020
#define TARGET_TOWN_DEAMON        0x0040
#define TARGET_FRONT_MAP          0x0080
#define TARGET_ENHANCE            0x0100
#define TARGET_TRUMP_UPGRADE      0x0200
#define TARGET_RECHARGE_PACKGE    0x0400


#define REGISTER_VAR(Var, Type) \
    m_VarTypes[Var] = Type;


    /**
     *@brief 变量系统
     */
    class VarSystem
    {
    public:
        VarSystem(UInt64 playerid);
         ~VarSystem();

    public:

        static void Init()
        {
            //注册变量
            REGISTER_VAR(VAR_SINGLE_CHOPSTICKS, CYCLE_YEAR)
            REGISTER_VAR(VAR_SENGYI, CYCLE_NONE);
            REGISTER_VAR(VAR_APRON, CYCLE_NONE);
            REGISTER_VAR(VAR_TGDT, CYCLE_DAY);
            REGISTER_VAR(VAR_TGDCNT, CYCLE_MONTH);
            REGISTER_VAR(VAR_ATOFM, CYCLE_NONE);
            REGISTER_VAR(VAR_ICEMAN, CYCLE_YEAR);
            REGISTER_VAR(VAR_BOOT, CYCLE_YEAR);
            REGISTER_VAR(VAR_TODAY_ONLINE,  CYCLE_DAY);
            REGISTER_VAR(VAR_KEYPACK1, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK2, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK3, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK4, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK5, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK6, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK7, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK8, CYCLE_NONE);


            REGISTER_VAR(VAR_FAIL_ENCH, CYCLE_NONE);
            REGISTER_VAR(VAR_SPLIT_EQUIP, CYCLE_NONE);
            REGISTER_VAR(VAR_SPLIT_THRUMP, CYCLE_NONE);

            REGISTER_VAR(VAR_BATTLE_MISS, CYCLE_NONE);
            REGISTER_VAR(VAR_BATTLE_CS, CYCLE_NONE);
            REGISTER_VAR(VAR_BATTLE_PR, CYCLE_NONE);
            REGISTER_VAR(VAR_BATTLE_FJ, CYCLE_NONE);
            REGISTER_VAR(VAR_BATTLE_SKILL_DMG, CYCLE_NONE);
            REGISTER_VAR(VAR_BATTLE_PEERLESS_DMG, CYCLE_NONE);
            REGISTER_VAR(VAR_COUNTRY_BATTLE_WIN, CYCLE_NONE);
            REGISTER_VAR(VAR_ATHLETICS_WIN, CYCLE_NONE);
            REGISTER_VAR(VAR_TASK_SUBMITTED, CYCLE_NONE);
            REGISTER_VAR(VAR_YAMEN_TASK_SUBMITTED, CYCLE_NONE);
            REGISTER_VAR(VAR_SHIMEN_TASK_SUBMITTED, CYCLE_NONE);
            REGISTER_VAR(VAR_CLAN_TASK_SUBMITTED, CYCLE_NONE);
            REGISTER_VAR(VAR_PEXP_GET, CYCLE_NONE);
            REGISTER_VAR(VAR_PURPLE_EQUIP_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_YELLOW_EQUIP_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_YELLOW_THRUMP_NUM, CYCLE_NONE);

            REGISTER_VAR(VAR_AWARD_3366, CYCLE_DAY);
            REGISTER_VAR(VAR_AWARD_QPLUS, CYCLE_DAY);
            REGISTER_VAR(VAR_AWARD_SSDT_1, CYCLE_DAY);
            REGISTER_VAR(VAR_AWARD_SSDT_2, CYCLE_NONE);
            REGISTER_VAR(VAR_AWARD_NEWREGISTER, CYCLE_DAY);
            REGISTER_VAR(VAR_AWARD_BIRTHDAY, CYCLE_DAY);
            REGISTER_VAR(VAR_AWARD_LOGIN, CYCLE_DAY);

            REGISTER_VAR(VAR_YUEBING_USED, CYCLE_NONE);

            REGISTER_VAR(VAR_CLANBATTLE_HONOUR, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAN_WEAL, CYCLE_DAY);

            REGISTER_VAR(VAR_NEWYEAR_PACK, CYCLE_YEAR);

            REGISTER_VAR(VAR_DAILY_CLANBATTLE, CYCLE_DAY);
            REGISTER_VAR(VAR_WEEKLY_CLANBATTLE, CYCLE_WEEK);

            REGISTER_VAR(VAR_BLUE_ACTIVE_GET, CYCLE_DAY);
            REGISTER_VAR(VAR_RECHARGE_TOTAL, CYCLE_NONE);

            REGISTER_VAR(VAR_LOVER_RING, CYCLE_YEAR);
            REGISTER_VAR(VAR_SHUSAN_LOVE, CYCLE_YEAR);
            REGISTER_VAR(VAR_LOGIN_PF, CYCLE_DAY);
            REGISTER_VAR(VAR_GIRL_PACK, CYCLE_YEAR);
            REGISTER_VAR(VAR_OFFLINE, CYCLE_NONE);
            REGISTER_VAR(VAR_OFFLINE_EXP, CYCLE_NONE);
            REGISTER_VAR(VAR_OFFLINE_PEXP, CYCLE_NONE);
            REGISTER_VAR(VAR_TOWNDEAMON, CYCLE_DAY);
            REGISTER_VAR(VAR_GOLD_TOKEN, CYCLE_NONE);
            REGISTER_VAR(VAR_TAEL_TOKEN, CYCLE_NONE);
            REGISTER_VAR(VAR_COIN_TOKEN, CYCLE_NONE);
            REGISTER_VAR(VAR_FOOLS_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_CBWHITEPACK, CYCLE_DAY);
            REGISTER_VAR(VAR_ATYITEM_1, CYCLE_WEEK);
            REGISTER_VAR(VAR_ATYITEM_2, CYCLE_WEEK);
            REGISTER_VAR(VAR_ATYITEM_3, CYCLE_WEEK);
            REGISTER_VAR(VAR_ATYITEM_4, CYCLE_WEEK);
            REGISTER_VAR(VAR_ATYITEM_5, CYCLE_WEEK);
            REGISTER_VAR(VAR_ATYITEM_6, CYCLE_WEEK);
            REGISTER_VAR(VAR_ATYITEM_7, CYCLE_WEEK);
            REGISTER_VAR(VAR_CLAN_LEAVE_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_CFRIEND_1, CYCLE_NONE);
            REGISTER_VAR(VAR_CFRIEND_45, CYCLE_NONE);
            REGISTER_VAR(VAR_CFRIEND_60, CYCLE_NONE);

            REGISTER_VAR(VAR_DISCOUNT_1, CYCLE_WEEK);
            REGISTER_VAR(VAR_DISCOUNT_2, CYCLE_WEEK);
            REGISTER_VAR(VAR_DISCOUNT_3, CYCLE_WEEK);
            REGISTER_VAR(VAR_DISCOUNT_SP_1, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_SP_2, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_SP_3, CYCLE_NONE);

            REGISTER_VAR(VAR_DISCOUNT_1_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_2_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_3_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_SP_1_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_SP_2_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_SP_3_TIME, CYCLE_NONE);

            REGISTER_VAR(VAR_CTSLANDING, CYCLE_NONE);
            REGISTER_VAR(VAR_CTSAWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_CL3DAY, CYCLE_NONE);
            REGISTER_VAR(VAR_RC7DAYRECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_RC7DAYWILL, CYCLE_NONE);
            REGISTER_VAR(VAR_INVITES, CYCLE_DAY);
            REGISTER_VAR(VAR_FIGHTER1_3_6_8, CYCLE_NONE);
            REGISTER_VAR(VAR_MDSOUL, CYCLE_NONE);
            REGISTER_VAR(VAR_MDSOUL_CNT1, CYCLE_NONE);
            REGISTER_VAR(VAR_MDSOUL_CNT2, CYCLE_NONE);
            REGISTER_VAR(VAR_MDSOUL_CNT3, CYCLE_NONE);
            REGISTER_VAR(VAR_MDES, CYCLE_NONE);
            REGISTER_VAR(VAR_MDPACKS, CYCLE_NONE);
            REGISTER_VAR(VAR_RC7DAYTURNON, CYCLE_NONE);
            REGISTER_VAR(VAR_YBBUF, CYCLE_DAY);
            REGISTER_VAR(VAR_LDPOINT, CYCLE_NONE);
            REGISTER_VAR(VAR_LDCOUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_LDPOINTB, CYCLE_NONE);
            REGISTER_VAR(VAR_DIAMOND_BLUE, CYCLE_DAY);
            REGISTER_VAR(VAR_DIAMOND_YELLOW, CYCLE_DAY);
            REGISTER_VAR(VAR_NETVDPACK, CYCLE_NONE);
            REGISTER_VAR(VAR_LASTATHRANK, CYCLE_NONE);
            REGISTER_VAR(VAR_SHUOSHUO, CYCLE_DAY);
            REGISTER_VAR(VAR_INVITED, CYCLE_DAY);

            REGISTER_VAR(VAR_JUNE, CYCLE_NONE);
            REGISTER_VAR(VAR_JUNE_EQUIP, CYCLE_NONE);
            REGISTER_VAR(VAR_JUNE_ITEM, CYCLE_DAY);
            REGISTER_VAR(VAR_JUNE_RECHARGE_TOTAL, CYCLE_NONE);
            REGISTER_VAR(VAR_JUNE_HAPPY, CYCLE_DAY);
            REGISTER_VAR(VAR_JUNE_HAPPY_ITEM_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYDRAW_CNT, CYCLE_NONE);

            REGISTER_VAR(VAR_CLAWARD2, CYCLE_NONE);

            REGISTER_VAR(VAR_OFFLINE_EQUIP, CYCLE_NONE);

            REGISTER_VAR(VAR_ENCHANTGT11, CYCLE_NONE);
            REGISTER_VAR(VAR_INRF7DAY, CYCLE_NONE);
            REGISTER_VAR(VAR_CTSLANDINGRF, CYCLE_NONE);
            REGISTER_VAR(VAR_RF7DAYRECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_RF7DAYWILL, CYCLE_NONE);
            REGISTER_VAR(VAR_RF7DAYTURNON, CYCLE_NONE);
            REGISTER_VAR(VAR_CTSAWARDRF, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAWARDRF, CYCLE_DAY);
            REGISTER_VAR(VAR_CL3DAYRF, CYCLE_NONE);

            REGISTER_VAR(VAR_MONEY_ARENA, CYCLE_NONE);
            REGISTER_VAR(VAR_MONEY_ARENA2, CYCLE_WEEK);
            REGISTER_VAR(VAR_MONEY_ARENA3, CYCLE_WEEK);
            REGISTER_VAR(VAR_LX_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_WQ_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_TS_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ZM_CNT, CYCLE_NONE);

            REGISTER_VAR(VAR_DUANWU, CYCLE_NONE);
            REGISTER_VAR(VAR_JULY, CYCLE_NONE);
            REGISTER_VAR(VAR_2NDSOUL, CYCLE_NONE);
            REGISTER_VAR(VAR_QQVIP, CYCLE_NONE);
            REGISTER_VAR(VAR_VIPFIRST, CYCLE_NONE);
            REGISTER_VAR(VAR_PEXPITEMS, CYCLE_NONE);
            REGISTER_VAR(VAR_SOSOMAPAWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_CONSUME, CYCLE_NONE);
            REGISTER_VAR(VAR_PHYSICAL_BUY, CYCLE_DAY);

            REGISTER_VAR(VAR_TJ_TASK_PRESTIGE,  CYCLE_NONE);
            REGISTER_VAR(VAR_TJ_TASK1_NUMBER, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_TASK2_TAEL, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_TASK2_GOLD, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_TASK2_COUPON, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_TASK2_TJYJ, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_TASK2_SCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_TASK3_COPYID, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_EVENT_PRESTIGE, CYCLE_NONE);
           
            REGISTER_VAR(VAR_RP_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_RP_REWARD_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_CONSUME_918, CYCLE_NONE);
            REGISTER_VAR(VAR_TJ_TOWN_ITEM_GOT, CYCLE_NONE);

            REGISTER_VAR(VAR_QQVIP_BUF, CYCLE_DAY);
            REGISTER_VAR(VAR_QQVIP_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_BDIAMOND_CMD, CYCLE_DAY);
            REGISTER_VAR(VAR_YEAR_SWORDSMAN, CYCLE_DAY);
            REGISTER_VAR(VAR_YEAR_NOBLE, CYCLE_DAY);
            REGISTER_VAR(VAR_ZYCM_POS, CYCLE_NONE);
            REGISTER_VAR(VAR_XIAGU_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ROUQING_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_CAIFU_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_CHUANQI_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ZYCM_TIPS, CYCLE_NONE);
            REGISTER_VAR(VAR_QGAME_GIFT, CYCLE_DAY);
            REGISTER_VAR(VAR_MAX_CLAN_COPY_LEVEL, CYCLE_WEEK);
            REGISTER_VAR(VAR_TOWN_REWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_BLUE_AWARD_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_YELLOW_AWARD_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_TGC_1_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_TGC_1_COPY, CYCLE_NONE);
            REGISTER_VAR(VAR_TGC_1_FRONTMAP, CYCLE_NONE);
            REGISTER_VAR(VAR_TGC_1_DUNGEON, CYCLE_NONE);

            REGISTER_VAR(VAR_DONTIAN_KILLPOINT, CYCLE_NONE);

            REGISTER_VAR(VAR_CTSLANDING_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_CTSLANDING_AWARD2, CYCLE_NONE);
            REGISTER_VAR(VAR_CTS_TARGET_COUNT, CYCLE_NONE);

            REGISTER_VAR(VAR_ITEM_9178_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9279_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9280_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9281_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9282_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_USETAEL_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_USECOUPON_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_USEGOLD_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_GETACHIEVEMENT_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_GETPRESTIGE_CNT, CYCLE_DAY);

            REGISTER_VAR(VAR_LOCAL_RANK, CYCLE_NONE);
            REGISTER_VAR(VAR_LOCAL_MAXRANK, CYCLE_NONE);
            REGISTER_VAR(VAR_LOCAL_PRESTIGE, CYCLE_NONE);
            REGISTER_VAR(VAR_LOCAL_PAGE, CYCLE_NONE);
            REGISTER_VAR(VAR_9215_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_QQVIP_AWARD_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_RECHARGE_SCORE, CYCLE_NONE);
            REGISTER_VAR(VAR_JOYVIP_BUFF_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_QQGAME_GIFT_1218, CYCLE_DAY);
            REGISTER_VAR(VAR_SNOW_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_AWARD_SSTOOLBAR, CYCLE_NONE);
            REGISTER_VAR(VAR_EX_JOB_ENABLE, CYCLE_NONE);
            REGISTER_VAR(VAR_JOB_HUNTER_SPOT_ID, CYCLE_NONE);
            REGISTER_VAR(VAR_EX_JOB_HAS_HAD, CYCLE_NONE);

            REGISTER_VAR(VAR_EQUIP_MOVE_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ACT_LOGIN_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_ACT_LOGIN_AWARD_VIP, CYCLE_DAY);
            REGISTER_VAR(VAR_VIP_PRIVILEGE_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_VIP_PRIVILEGE_DATA, CYCLE_DAY);

            REGISTER_VAR(VAR_ARENA_SUPPORT, CYCLE_DAY);
            //REGISTER_VAR(VAR_ARENA_SUFFERED, CYCLE_DAY);
            //REGISTER_VAR(VAR_ARENA_LASTTIME, CYCLE_DAY);
            REGISTER_VAR(VAR_ARENA_SUPPORT_TUE, CYCLE_WEEK);
            REGISTER_VAR(VAR_ARENA_SUPPORT_WED, CYCLE_WEEK);
            REGISTER_VAR(VAR_FIRST_RECHARGE_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_FIRST_RECHARGE_STEP, CYCLE_NONE);
            REGISTER_VAR(VAR_CF_FLAG, CYCLE_NONE);
            REGISTER_VAR(VAR_CF_BIND, CYCLE_NONE);
            REGISTER_VAR(VAR_GOOD_VOICE, CYCLE_NONE);
            REGISTER_VAR(VAR_3366GIFT, CYCLE_NONE);
            REGISTER_VAR(VAR_CF_INDEX, CYCLE_NONE);
            REGISTER_VAR(VAR_CF_LOCATION, CYCLE_NONE);
            REGISTER_VAR(VAR_FEAST_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_FEAST_GIFT, CYCLE_DAY);

            REGISTER_VAR(VAR_FAIRYPET_LONGYUAN, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYPET_FENGSUI, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYPET_XIANYUAN, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYPET_STEP, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYPET_LIKEABILITY, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYPET_ISGET_PET, CYCLE_NONE);

            REGISTER_VAR(VAR_INVITEDSUCCESS, CYCLE_MONTH);
            REGISTER_VAR(VAR_CFRIENDTICKETS, CYCLE_NONE);

            REGISTER_VAR(VAR_TOWER_LOGIN, CYCLE_DAY);
            REGISTER_VAR(VAR_TOWER_LEVEL, CYCLE_NONE);

            REGISTER_VAR(VAR_HAS_MO_BATTLER, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9316_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_CONSUME_AWARD_COUNT, CYCLE_NONE);

            REGISTER_VAR(VAR_DRAGONKING_STEP, CYCLE_NONE);
            REGISTER_VAR(VAR_DRAGONKING_STEP4_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_SET_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_GET_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_ISGET, CYCLE_DAY);

            REGISTER_VAR(VAR_HHBAWARD_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_LONGYUAN_GOT, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE_TODAY_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_RP7_TREASURE1_GETTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE2_GETTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE3_GETTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_SIGN, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_SIGN_PACKAGE, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_RECHARGE, CYCLE_NONE);

            REGISTER_VAR(VAR_CALLSNAKEEGG, CYCLE_NONE);
            REGISTER_VAR(VAR_SNAKEEGG_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_9344_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_9343_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_SNAKE_SPRING_EQUIP_GOT, CYCLE_NONE);
            REGISTER_VAR(VAR_WEIBO_AWARD_GOT, CYCLE_NONE);
            REGISTER_VAR(VAR_FISHUSER_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_NEWYEAR_QQGAME_ACT, CYCLE_DAY);
            REGISTER_VAR(VAR_NEWYEAR_QZONECONTINUE_ACT, CYCLE_NONE);
            REGISTER_VAR(VAR_NEWYEAR_QZONECONTINUE_LASTTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_EXP_HOOK_INDEX, CYCLE_NONE);
            REGISTER_VAR(VAR_TRAINP1, CYCLE_NONE);
            REGISTER_VAR(VAR_TRAINP2, CYCLE_NONE);
            REGISTER_VAR(VAR_TRAINP3, CYCLE_NONE);
            REGISTER_VAR(VAR_ONLINE_TOTAL_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_ONLINE_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_QZONE_QQGAME_ACT, CYCLE_DAY);

            REGISTER_VAR(VAR_FOOLS_DAY_INFO, CYCLE_DAY);
            REGISTER_VAR(VAR_FOOLS_DAY_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_HAS_VOTE, CYCLE_DAY);
            REGISTER_VAR(VAR_POPULARITY, CYCLE_WEEK);

            REGISTER_VAR(VAR_FOOLBAO_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_HHBAWARD_GOT, CYCLE_DAY);
        }

        UInt32 GetVar(UInt32 id, UInt32 now = 0);
        void SetVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void AddVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void LoadVar(UInt32 id, UInt32 data, UInt32 overTime);
        void SetOffset(UInt32 offset){ m_Offset = offset; }

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id, UInt32 now);
        void UpdateDB(UInt32 id);

    private:
        //玩家
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];
        UInt32 m_Offset;
        FastMutex _mutex;

        static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

