
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
        VAR_FOOLS_DAY = 58, //愚人节礼包是否领取  愚公移山(byte0失败题目id,byte1领取奖励的题目数,byte2购买复活卡次数,byte3玩家离线标志)
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

        VAR_RECHARGE_TODAY = 172, // 今日充值额度
        VAR_RECHARGE_TODAY_COUNTED = 173, // 今日充值额度已达目标额度
        VAR_RECHARGE_CONDCNT = 174, // 达到充值额度的次数
        VAR_RECHARGE_CONDCNT_LAST = 175, // 上次达到充值额度的次数，用作结果查询用

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

        VAR_TOTALRECHARGEACT = 217, // 活动期间充值总额
        VAR_DIRECTPUROPEN = 218, // 幸运星活动直购是否开启
        VAR_DIRECTPURCNT = 219, // 幸运星活动直购次数(低价专区)
        VAR_LEFTTIMES = 220, // 剩余挂机时间

        /*
         * 【注:此段var(221--239) for dtc】
         */
        VAR_GAMEBOX_DAILY = 221,
        VAR_GAMEBOX_NEW = 222,
        VAR_MARRY_STATUS = 223,//结婚状态
        VAR_CANCEL_APPOINTMENT = 224,//取消婚礼预约
        VAR_CANCEL_REPLYMARRIAGE = 225,//反悔回复征婚
        VAR_CANCEL_MARRIAGE = 226,//取消提出的征婚
        VAR_CANCEL_GIVEUP_JIEYUAN = 227,//放弃结缘
        VAR_COUPLE_ONLINE_START_TIME = 228,//夫妻双方同时在线开始时间
        VAR_COUPLE_ONLINE_AWARD = 229,//夫妻在线奖励
        VAR_COUPLE_ONLINE_TIME = 230,//夫妻当日在线总时间
        VAR_COUPLE_ONLINE_FISH = 231,//夫妻家园的钓鱼次数
        VAR_COUPLE_ONLINE_RENAME = 232,//夫妻家园修改名字
        VAR_COUPLE_COPY_STATUS = 233,//夫妻家园副本状态< 总共32位 从低到高8bit 副本类型，玩家在队伍状态，跳过战斗flag，自动开始flag > 
        VAR_COUPLE_COPY_DAY_COUNT = 234,//夫妻家园每日刷新免费次数or当日购买次数 8bit
        VAR_COUPLE_COPY_COUNT = 235,//夫妻家园总次数
        VAR_COUPLE_COPY_BATTLE = 236,//夫妻家园进行次数
        VAR_FRIEND_SECURITY = 237,//好友列表隐私设置标记
        VAR_COUPLE_LEVELEXP = 238,//夫妻家园离婚保留下来的成长值
        VAR_COUPLE_FRIENDLINESS = 239,//夫妻家园保留下来的亲密度

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
        VAR_TUIGUAN_AWARD_GOT = 274, // 推广平台用户奖励当天是否领取
        VAR_PET_CUILIAN_DAILY_CNT_DATE = 275, // 领取每日免费淬炼次数日期
        VAR_PET_CUILIAN_LEFT_CNT = 276,  // 剩余淬炼次数
        VAR_PET_CUILIAN_GOLD_DAILY_CNT = 277,  // 每日仙石购买淬炼次数
        VAR_SURNAME_LEGEND_STATUS = 278,  // 每日幸运礼包领取情况
        VAR_QQEXPLORER_AWARD = 279, // QQ浏览器奖励是否领取
        VAR_SYS_UPDATE_DLG = 280, // 更新公告弹窗

        // 281-290 占用 fro qiwy
        VAR_ARENA_SUPPORT = 281, //场外活动，支持的序列id
        //VAR_ARENA_SUFFERED = 282, //场外活动，被陷害的次数
        //VAR_ARENA_LASTTIME = 283, //场外活动，最后被陷害的时间
        VAR_ARENA_SUPPORT_TUE = 284, //场外活动，星期二支持的玩家
        VAR_ARENA_SUPPORT_WED = 285, //场外活动，星期三支持的玩家
        // 首充礼包
        VAR_FIRST_RECHARGE_VALUE = 286, //充值数
        VAR_FIRST_RECHARGE_STEP = 287, //已领阶段
        VAR_CF_FLAG = 288, //是副本(1)，还是阵图(2)，还是决战之地(3)
        VAR_CF_UNBIND = 289, //是否未绑定,1-未绑定,0-绑定
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

        VAR_XINGCHENZHEN_VALUE = 360,   //镇旗镇眼兑换的星辰值
        // 361-370 占用 for suntao
        VAR_DRAGONKING_STEP = 361,      //大闹龙宫当前移动到的格子
        VAR_DRAGONKING_STEP4_COUNT = 362, //大闹龙宫从第4格移动到第5失败的次数
        VAR_SAVEGOLD_COUNT = 363,   //金蛇献瑞,聚福兆祥活动中玩家存仙石数量
        VAR_SAVEGOLD_SET_TIME = 364,    //金蛇献瑞,聚福兆祥活动中玩家设置的结算时间
        VAR_SAVEGOLD_GET_STATUS = 365,    //金蛇献瑞,聚福兆祥活动中玩家领取自己仙石的状态,低10位
        VAR_SAVEGOLD_ISGET = 366,   //金蛇献瑞,聚福兆祥活动中玩家当日是否领取

        VAR_LUCKYSTAR_IS_CONSUME     = 367, //幸运星:是否消费过
        VAR_LUCKYSTAR_GET_STATUS     = 368, //幸运星:玩家领取和购买的状态(低12bit,0-3蓝 4-7紫 8-11橙)
        VAR_LUCKYSTAR_RECHARGE_TOTAL = 369, //幸运星:玩家充值仙石总数
        VAR_LUCKYSTAR_LOGIN_TIME     = 370, //幸运星:活动期间玩家第一次登录时间
        
        VAR_CALLSNAKEEGG = 371,         //拜金蛇
        VAR_SNAKEEGG_AWARD=372,         //领蛇蛋
        VAR_9344_USED=373,
        VAR_9343_USED=374,
        VAR_SNAKE_SPRING_EQUIP_GOT=375,
        VAR_WEIBO_AWARD_GOT=376,
        VAR_FISHUSER_AWARD=377,            //捕鱼大亨用户领奖标志 

        VAR_NUWA_SIGNET = 378,   //女娲石盘解封的印记(9个印记,一个印记3bit)
        VAR_NUWA_OPENTIME = 379, //女娲石盘解封时间

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
        VAR_QZONE_QQGAME_ACT = 410, //空间黄钻(bit1表示普通，bit2表示会员)、大厅蓝钻礼包(bit3表示普通，bit4表示会员)、3366蓝钻礼包(bit5表示普通，bit6表示会员)
        VAR_LAST_HOOK_TYPE = 411, //最后挂机类型
        VAR_SPREAD_FLAG = 413, //低第1bit表示是否使用落英秘典,低第2bit表示是否已领取
        VAR_SKILL_UP_FLAG = 414, //FB占用
        VAR_POOL_CNT = 415, //卡片碎片池数
        VAR_CARD_FROM_CLAN = 416, //是否领取来自帮派战的卡片
        VAR_MAP_INDEX = 417, //地图索引
        VAR_FEAST_LOGIN_AWARD_PER_DAY = 418, //中秋节登陆送月饼
        VAR_TJ_AUTO_FRONTMAP_END_TIME = 419,

        VAR_MOUNT_LEVEL = 420,  //铸剑等级
        //421-430 for suntao
        VAR_FOOLS_DAY_INFO  = 421,  //愚公移山答题信息(0位表示失败,1-30位表示题目id及是否答对,31位表示等级限制[0:70级以上 1:70级以下])
        VAR_FOOLS_DAY_TIME  = 422,  //愚公移山答题开始时间
        VAR_INVITEDSUCCESS = 423, //邀请成功的好友人数
        VAR_CFRIENDTICKETS = 424, //好友邀请的抽奖券

        VAR_TEAMARENA_CREATE  = 425, //组队跨服战创建战队限制次数
        VAR_TEAMARENA_WORSHIP = 426, //组队跨服战每日崇拜冠军
        VAR_TEAMARENA_INSPIRE = 427, //组队跨服战鼓舞等级失败保护次数

        VAR_SERVERWAR_JIJIANTAI = 428,  //跨服服战祭剑台 1byte步数,2byte内外圈,3和4byte道具id
        VAR_SERVERWAR_JIJIANTAI1 = 429,  //跨服服战祭剑台 1byte每人祭剑的次数,2byte成功完成的祭剑次数 3byte宝箱开启情况 4byte购买祭剑的次数

         VAR_MOUNT_EXP = 430,   //铸剑经验总值

        //431-440 for yijian
        VAR_LONGYUAN_GOT = 431,  //龙元风髓领取标志
        VAR_RP7_TREASURE = 432,  //回流服务器聚宝盆, 1-8位:是否购买的标志为 9-16,17-24,25-32位第1-3个聚宝盆的已领取次数
        VAR_RP7_TREASURE_TODAY_GOT=433, //第1-3位:各个聚宝盆今天是否已领取
        VAR_RP7_TREASURE1_GETTIME=434, //第1个聚宝盆的领取时间
        VAR_RP7_TREASURE2_GETTIME=435, //第2个聚宝盆的领取时间
        VAR_RP7_TREASURE3_GETTIME=436, //第3个聚宝盆的领取时间
        VAR_RP7_SIGN=437,              //注册签到
        VAR_RP7_SIGN_PACKAGE=438,      //连续签到礼包
        VAR_RP7_RECHARGE=439,      //开服7天充值金额
        VAR_QIXI_DROP_STATUS = 440, //七夕掉落活动状态

        // 441-442 for JLT
        VAR_HAS_VOTE = 441,                 // 本日是否投票过
        VAR_POPULARITY = 442,               // 本周人气

        VAR_PET_CUILIAN_EXTRA_LOW_SCORE = 443, // 额外淬炼基础品质下限
        VAR_PET_CUILIAN_EXTRA_UP_SCORE = 444,  // 额外淬炼基础品质下限
        VAR_PET_CUILIAN_SCORE_EQUIP = 445,     // 内丹淬炼值
        VAR_PET_CUILIAN_SCORE_GEM = 446,       // 精魄淬炼值
        VAR_PET_CUILIAN_SCORE_EQUIP_TIMES = 447,     // 内丹淬炼次数
        VAR_PET_CUILIAN_SCORE_GEM_TIMES = 448,       // 精魄淬炼次数

        VAR_FOOLBAO_USED = 450,  //愚人宝箱使用数
        VAR_HHBAWARD_GOT = 451 , //豪华蓝钻领取
        VAR_SURNAMELEGEND_USED = 452, //蜀山传奇幸运礼包使用数
        VAR_CARD_1 = 453,       //卡片活动  集齐五张获大奖
        VAR_CARD_2 = 454,
        VAR_CARD_3 = 455,
        VAR_CARD_4 = 456,
        VAR_CARD_5 = 457,
        VAR_LEVEL_AWARD = 458, 
        VAR_ZCJB_RECHARGE_GOLD = 459, //招财进宝总共充值仙石
        VAR_CLANBOSS_GONGXIAN= 460, //末日之战贡献

        // 441-442 for JLT
        VAR_GUANKA_ACTION_UDPLOG = 461, //关卡活动npc击杀情况udplog日志

        //繁体版占用470-499

        //500-520 for lijr
        VAR_QQNAVIGATION_DAY_AWARD = 500,                   // QQ导航每天奖励是否领取
        VAR_QQNAVIGATION_WEEK_AWARD = 501,                  // QQ导航每周奖励是否领取
        VAR_QQNAVIGATION_FIRST_LOGIN_AWARD = 502,           // QQ导航首次登录奖励是否领取

        VAR_QQMUSIC_DAY_AWARD = 503,                        // QQ音乐每天奖励是否领取
        VAR_QQTENPAY_AWARD = 504,                           // QQ财付通奖励是否领取
        VAR_QQIM_QUICK_LOGIN_AWARD = 505,                   // QQIM快捷登录奖励是否领取
        VAR_EQUIP_MOVE_AWARD = 506,                         // 装备继承奖励
        VAR_COPY_AUTO_FIGHT_USE_MONEY_MARK = 507,           // 副本自动战斗使用金钱情况
        VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK = 508,       // 阵图自动战斗使用金钱情况
        VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK = 509,        // 决战之地自动战斗使用金钱情况
        VAR_JOBHUNT_AUTO_FIGHT_USE_MONEY_MARK = 510,        // 寻墨自动战斗使用金钱情况
        VAR_VIPLEVEL_AWARD = 511,                           // 御剑等级回馈
        VAR_DROP_OUT_ITEM_MARK = 512,                       // 反工作室限制方案
        VAR_QQXIU_AWARD = 513,                              // QQ秀合作
        VAR_QQTENPAY_LOTTERY = 514,                         // QQ财付通抽奖 
        VAR_KEYIN_MONEY_A = 515,                            // 刻印钱币A 
        VAR_KEYIN_MONEY_B = 516,                            // 刻印钱币B 
        VAR_FAIRYPET_SHOUHUN = 517,                         // 仙宠兽魂
        VAR_GIVE_CARDAWARD_COUTS = 518,                     // 被赠送卡牌奖品次数
        VAR_QISHIDOUFA_REST_NUM = 519,                      // 七石斗法每天3次免费重置机会
        VAR_QISHIDOUFA_LASTFAIL_HIGHTERSCORE = 520,         // 七石斗法上次失败历史最高分

        // 521-540 for zhgc
        VAR_ZCJB_TIMES = 521,      // 招财进宝
        VAR_ZCJB_GOLD_GOT = 522,   // 招财进宝总共获得仙石
        VAR_ZRYJ_COUNT = 523,      // 逐日印记
        VAR_HYYJ_COUNT = 524,      // 皓月印记
        VAR_RYHB_ITEM_CNT_1 = 525,      // 日月合璧购买物品1个数
        VAR_RYHB_ITEM_CNT_2 = 526,      // 日月合璧购买物品2个数
        VAR_RYHB_ITEM_CNT_3 = 527,      // 日月合璧购买物品3个数
        VAR_RYHB_ITEM_CNT_4 = 528,      // 日月合璧购买物品4个数
        VAR_RYHB_ITEM_CNT_5 = 529,      // 日月合璧购买物品5个数
        VAR_RYHB_ITEM_CNT_6 = 530,      // 日月合璧购买物品6个数
        VAR_RYHB_ITEM_CNT_7 = 531,      // 日月合璧购买物品7个数
        VAR_RYHB_ITEM_CNT_8 = 532,      // 日月合璧购买物品8个数
        VAR_RYHB_ITEM_CNT_9 = 533,      // 日月合璧购买物品9个数
        VAR_RYHB_ITEM_CNT_10 = 534,      // 日月合璧购买物品10个数
        VAR_RYHB_ITEM_CNT_11 = 535,      // 日月合璧购买物品11个数
        VAR_RYHB_ITEM_CNT_12 = 536,      // 日月合璧购买物品12个数
        VAR_RYHB_ITEM_CNT_13 = 537,      // 日月合璧购买物品13个数
        VAR_RYHB_ITEM_CNT_14 = 538,      // 日月合璧购买物品14个数
        VAR_RYHB_ITEM_CNT_15 = 539,      // 日月合璧购买物品15个数
       
        VAR_VIP_PRIVILEGE_DATA_TYPE = 540 ,     //新手御剑奖励类型(0-未开通，1-七大豪享特权一阶段，2-十大至尊特权一阶段，3-七大豪享特权二阶段，4-十大至尊特权二阶段，5-七大梦幻特权，6-十大梦幻特权)
        VAR_VIP_PRIVILEGE_BREAK_LAST = 541 ,
        VAR_SUMMERFLOW_TYPE = 542 ,
        VAR_SUMMERFLOW_AWARD = 543,   //是否领取暑期回流礼包
        VAR_QQBOARD = 544 ,           //QQ面板
        VAR_QQBOARD_AWARD = 545,     //QQ面板奖励领取情况
        VAR_LUCKYMEET = 546,        //暑假奇遇
        VAR_LUCKYMEET_AWARD = 547,  //暑假奇遇登陆奖励
        VAR_LUCKYMEET_RECHARGE = 548,  //暑假奇遇充值数
        VAR_LUCKYMEET_RECHARGE_AWARD = 549 ,  //暑假奇遇充值奖励
        VAR_LUCKYMEET_VIP = 550 ,      //暑假奇遇期间御剑等级
        VAR_LUCKYMEET_STRENTH_AWARD = 551,  //暑假奇遇变强
      
        VAR_SUMMER_MEET_TYPE = 552 ,    //蜀山奇遇类型
        VAR_SUMMER_MEET_TYPE_AWARD = 553,  //蜀山奇遇类型奖励（登陆弹框）
        VAR_SUMMER_MEET_LOGIN = 554,    //蜀山奇遇登陆
        VAR_SUMMER_MEET_LOGIN_AWARD = 555, //蜀山奇遇登陆奖励领取情况
        VAR_SUMMER_MEET_RECHARGE = 556,   //蜀山奇遇充值
        VAR_SUMMER_MEET_RECHARGE_AWARD = 557 , //蜀山奇遇充值奖励领取
        VAR_SUMMER_MEET_STRENTH_AWARD = 558, //蜀山奇遇变强奖励领取

        VAR_SUMMER_FLOW_LOGIN_AWARD = 559,   //暑假回流登陆奖励
        // 560-580 for zhgc
        VAR_CLAN_SPTR_WATER = 560,       // 每日神魔之树浇水数
        VAR_CLAN_SPTR_AWARD = 561,       // 每日神魔之树奖励领取标识
        VAR_CLAN_ACTPT_MONTH = 562,      // 每月帮派活跃度
        VAR_CLAN_ACTPT_FLAG = 563,      // 活跃度领取标识
        VAR_QZONGPYGIFT = 564, //Qzong,PY合作礼包

        //581-600 for lb 
        VAR_SUMMERFLOW3_TYPE = 581 ,    //付费回流类型
        VAR_SUMMERFLOW3_TYPE_AWARD = 582 ,  //付费回流类型奖励
        VAR_SUMMERFLOW3_TIME_AWARD = 583, //付费回流第三期在线时间奖励

        VAR_PRAY_TYPE = 584 ,    //许愿类型
        VAR_PRAY_VALUE = 585,   //许愿值
        VAR_PRAY_COUNT = 586,   //一周许愿次数
        VAR_PRAY_SUCTIME = 587,  //愿望积满的时间
        VAR_PRAY_TYPE_TODAY = 588 ,   //今天是否许愿
        VAR_PRAY_TIME = 589 ,   //许愿时间
        VAR_PRAY_LOGIN = 590,   //一周连续登陆记录
        
        VAR_QQBOARD_ONLINE = 591,   //无效
        VAR_QQBOARD_LOGIN_AWARD = 592, //登陆奖励
        VAR_QQBOARD_ONLINE_AWARD = 593, //在线奖励（无效）
        VAR_RANDfRIEND = 594 ,   //是否发送推荐好友
        VAR_SENDPRESENT_VALUE = 595,  //赠送礼物数量（开服7天活动）
        VAR_GETPRESENT_VALUE = 596, //接受礼物数量（开服7天活动）
        VAR_PRESENT_LOGIN = 597 ,  //连续登陆记录
        VAR_STRENGTH_LEVEL = 598,      //
        VAR_11AIRBOOK_GRADE = 599,  //10· 1 活动积分
        VAR_11AIRBOOK_GRADE_DAY = 600,  //10· 1 活动积分
        //601-620 for qiwy  ^_^
        VAR_TOTAL_BATTLE_POINT = 601,  //（排行）战斗力
        VAR_VIA_ACC_DAYS = 602,  //（导入的玩家）累计登录天数
        VAR_SEVEN_SOUL_NUM = 603,  //玩家七魂仙魄数
        VAR_SEX_CHANGE = 604,  //使用变形卡的次数
        VAR_SKILL_GRADE_MONEY = 605,  //技能升阶货币
        VAR_DROP_ACT = 606,  //掉落活动
        VAR_DIRECTPURCNT2 = 607, // 幸运星活动直购次数(超值专区)
        VAR_FEAST_GIFT_LOGIN = 608, //周年庆套装——生辰宝甲
        VAR_PRIVATE_RECHARGE = 609, //私人定制对应的每日充值
        VAR_PACKAGE_SIZE_GEM = 610, //宝石背包空间
        VAR_PACKAGE_SIZE_FORMULA = 611, //阵法、阵元背包空间
        VAR_PACKAGE_SIZE_SL = 612, //元神背包空间

        //621-660 for lib 
        VAR_AIRBOOK_LOGIN = 621,    //天书奇缘登陆
        VAR_AIRBOOK_LOGIN_AWARD = 622,  //天书奇缘登陆奖励
        VAR_AIRBOOK_ONLINE_AWARD = 623,  //天书奇缘在线奖励
        VAR_AIRBOOK_RECHARGE = 624,    //天书奇缘充值
        VAR_AIRBOOK_CONSUME = 625,
        VAR_GUANGGUN_TIMES = 626,        //光棍节倍数（每天清零）
        VAR_GUANGGUN_GETROLL = 627,     //每天积分获得的抽奖次数（最大为3，每天清零）
        VAR_GUANGGUN_RECHARGE = 628,
        VAR_GUANGGUN_CONSUME = 629,
        VAR_GUANGGUN_ADVANCE_NUM = 630 ,    //可前进次数（每天初始化10）
        VAR_GUANGGUN_ADVANCE_OTHER = 631,    //其他方式获得的前进次数
        VAR_GUANGGUN_TODAY_SCORE = 632,
        VAR_GUANGGUN_ADVANCE_BUY = 633,
        VAR_GROWUPFUND_TYPE = 634,
        VAR_GROWUPFUND_AWARD = 635,
        VAR_NOV_LOGIN = 636,
        VAR_NOV_LOGIN_AWARD = 637, 
        VAR_RECHARGE_TIME = 638, 
        VAR_GUANGGUN_TODAY_TASK = 639,   //今日完成任务数
        VAR_REAL_SPIRIT = 640,           //真元
        VAR_MODIFY_NAME_CD = 641,//改名CD
        VAR_REAL_SPIRIT_GET = 642,       //当日是否领取真元 
        VAR_QZONE_RECHARGE = 643,       //空间充值 
        VAR_QZONE_RECHARGE_AWARD = 644,       //空间充值奖励领取
        
        VAR_OLDMAN_DAYSCORE = 645,       //捉迷藏活动每日积分
        VAR_OLDMAN_SCORE = 646,       //捉迷藏活动累计积分
        VAR_OLDMAN_PRESENT = 647,      //捉迷藏活动接受礼包数
        VAR_OLDMAN_DAYSCORE_AWARD = 648,       //捉迷藏活动每日积分奖励
        VAR_OLDMAN_SCORE_AWARD = 649,       //捉迷藏活动累计积分奖励

        VAR_YEARHAPPY_DAYVALUE = 650,       //每日累计欢乐值 
        VAR_YEARHAPPY_VALUE = 651,       //累计欢乐值
        VAR_YEARHAPPY_DAYVALUE_AWARD = 652,       //每日欢乐值奖励  ==>更改为NONE
        VAR_YEARHAPPY_LEFTVALUE = 653,       //每日剩余欢乐值   ==> 
        //婚礼进行系统
        VAR_MARRYBOARD_LIVELY  = 654,       //个人热闹值
        VAR_MARRYBOARD3  = 655,             //吃宴席的盘数 0表示未入席 1-10表示吃了 0-9盘
        VAR_MARRYBOARD2_NUM  = 656,             // 表示答题号
        VAR_MARRYBOARD2_ANS  = 657,             // 表示当前答案 
        VAR_MARRYBOARD4_TIME = 658,             //表示婚礼第三阶段吃喜筵第四阶段驱赶时间
        VAR_MARRYBOARD3_KEY  = 659,

        //660-680 for lijr
        VAR_CHRISTMAS_PRESENT = 660,                //圣诞礼物——麋鹿之角
        VAR_QT_AWARD_MARK = 661,                    //QT奖励领取
        VAR_QISHIDOUFA_CYCLE_HIGHESTSCORE = 662,    //七石板每期最高积分
        VAR_MOBAO_REFRESH_AWARD_MARK = 663,         //墨宝刷新奖励标记
        VAR_QT_REGIST_NUM = 664,                    //QT月签到次数
        VAR_QT_SPECIAL_MARK = 665,                  //QT记录12月份默认加6天签到次数
        VAR_QT_REGIST_MARK = 666,                   //QT月签到标志
        VAR_BUY_FUND_NuM = 667,                     //购买基金标志
        VAR_BUY_FUND_AWARD = 668,                   //购买基金回馈标志
        VAR_BUY_FUND_TRUMP_AWARD = 669,             //购买基金法宝奖励标志
        VAR_ORDINARY_KILL_NUM = 670,                //普通击杀次数
        VAR_DIFFICULTY_KILL_NUM = 671,              //困难击杀次数
        VAR_ORDINARY_REFRESH_NUM = 672,             //普通刷新次数
        VAR_DIFFICULTY_REFRESH_NUM = 673,           //困难 刷新次数
        VAR_HELP_NUM = 674,                         //协助次数
        VAR_PURPLE_NUM = 675,                       //紫色次数
        VAR_ORANGE_NUM = 676,                       //橙色次数
        VAR_PETTEAMCOPY_BATTLE_RES = 677,           //宠物副本战斗结果
        VAR_REFRESH_AWARD_NUM = 678,                //墨宝刷新奖品次数
        VAR_OPENCARD_NUM = 679,                     //墨宝翻牌次数
        VAR_BUY_OPENCARD_NUM = 680,                 //当天购买翻牌次数

        //681 - 700 for lb
        VAR_MARRYBOARD_YANHUA = 681,          //烟花积分
        VAR_3366_RECHARGE = 682,       //空间充值 
        VAR_3366_RECHARGE_AWARD = 683,       //空间充值奖励领取
        VAR_MARRYBOARD_BAIHE = 684,          //百合使用
        VAR_NEWYEARSPEED_COUNT = 685 ,
        VAR_ONCE_ONDAY = 686 ,    //记录一天只有一次
        VAR_SUMMERMEET_SCORE1 = 687 ,    //决战之地，个人副本，组队副本，阵图  计数
        VAR_SUMMERMEET_SCORE2 = 688 ,    //斗剑历练，斗剑天梯，挂机加速，师门任务
        VAR_SUMMERMEET_SCORE3 = 689 ,    //衙门任务，帮派任务任务
        VAR_SUMMERMEET_SCORE4 = 690 ,    //充值数
        VAR_SUMMERMEET_SCORE5 = 691 ,    //消费数
        VAR_COUPLE_NAME= 692 ,    //夫妻称号佩戴
        VAR_NEWYEAR_PRATICE_COUNT = 693 ,    //记录一天只有一次
        VAR_LEFTADDR_POWER = 694 ,   //仙界征战体力
        VAR_LEFTADDR_CREATE = 695 ,   //创建队伍的时间
        VAR_HEART_SWORD = 696 ,    //心剑值

        VAR_LEFTADDR_POWER_ADD  = 697,  //体力增加次数
        VAR_FRIEND_TASK1 = 698 ,    //友好度任务，主动私聊，帮助浇灌许愿树，发起与好友的切磋
        VAR_FRIEND_TASK2 = 699 ,    //与好友组队副本，与好友宠物副本，赠送黄色鸢尾
        VAR_FRIEND_VALUE = 700 ,    //友情值

        //701-710 for suntao
        VAR_MicroCloud_AWARD = 701,     //是否领取微云合作奖励
        VAR_JIQIREN_COPY = 702,     //机器人之副本未完成次数(1byte免费，2byte付费1，3byte付费2，4byte付费3)
        VAR_JIQIREN_FRONTMAP = 703, //机器人之阵图未完成次数(1byte免费，2byte付费1，3byte付费2，4byte付费3)
        VAR_JIQIREN_DUNGEON = 704,  //机器人之决战之地未完成次数(1byte普通免费，2byte普通付费，3byte困难免费，4byte困难付费)
        VAR_JIQIREN_SYBS = 705,     //机器人之帮派、师门、衙门、锁妖塔未完成次数
        VAR_MOUNT_CANGJIANYA_LEFT_CNT = 706,     //藏剑崖每天免费次数
        VAR_MOUNT_CANGJIANYA_DATE = 707,     //取藏剑崖每天免费次数时间
        VAR_GUANKA_ACTION_NPC = 708,     //关卡活动npc击杀情况,6类npc低18位表示，高8位表示奖励领取情况
        VAR_GUANKA_ACTION_SCORE = 709,   //关卡活动个人镇魔值
        VAR_GUANKA_ACTION_TIME  = 710,   //关卡活动个人镇魔值变化时间

        //711-720 for JLT
        VAR_QISHI_FRIEND_SEND_COUNT = 711,  // 七石斗法玩家赠送次数
        VAR_QISHI_FRIEND_USE_COUNT = 712,   // 七石斗法玩家赠送已使用次数
        VAR_CLAN_DUOBAO_SCORE = 713,        // 夺宝奇兵点数
        VAR_CLAN_DUOBAO_SUCCESS_NUM = 714,  // 夺宝奇兵成功次数
        VAR_CLAN_DUOBAO_STATUS = 715,       // 夺宝奇兵状态

        VAR_ZHENYUAN_TIQU_DATE = 716,    //阵元提取神符免费次数时间
        VAR_ZHENYUAN_TIQU_CNT = 717,     //阵元提取神符 低16位每天次数 高16位每天仙石购买次数
        VAR_KJTM_STATUS = 718,              // 抗击天魔状态
        VAR_KJTM_KILL_NPC_STATUS = 719,     // 抗击天魔击杀怪物状态
        VAR_KJTM_LOGIN_STATUS = 720,     // 抗击天魔回归玩家连续登录两天状态

        //721 - 730 for dtc 
        VAR_TYSS_RECHARGE = 721,//天元神兽期间玩家兑换灵果剩余充值数(充值数满足条件自动兑换灵果)
        VAR_TYSS_CONTRIBUTE_PLAYER = 722,//天元神兽期间玩家个人总贡献 
        VAR_TYSS_CONTRIBUTE_PLAYER_DAY = 723,//天元神兽期间玩家个人当日贡献 
        VAR_TYSS_CONTRIBUTE_CLAN = 724,//天元神兽期间玩家当前帮派贡献 
        VAR_TYSS_DISCOUNT_CONSUME1 = 725,//天元神兽期间限购栏1(购买次数)(8bit 一个礼包位置)
        VAR_TYSS_DISCOUNT_CONSUME2 = 726,//天元神兽期间限购栏2(购买次数)
        VAR_TYSS_DISCOUNT_CONSUME3 = 727,//天元神兽期间限购栏3(购买次数)
        VAR_TYSS_CONTRIBUTE_CLAN_SUM = 728,//天元神兽期间玩家帮派总贡献(帮主身上) 
        
        VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK = 729,//璇玑阵图自动战斗使用金钱标志
        VAR_ATOXJFM = 730,     //自动璇玑阵图id
        // 731 - 750 for lib

        VAR_FRIEND_ACHIEVEMENT  = 731,  //友好度成就奖励领取情况
        VAR_CLAN_FRIEND = 732 ,    //帮派玩家是否增加过友好度(8位) , 玩家购买的剩余饮酒次数 
        VAR_DRINK_COUNT = 733 ,    //饮酒次数 主动发起(8位) ,被动接受(8位) ,玩家总购买的次数 
        VAR_DRINK_VALUE = 734 ,    //豪饮值
        VAR_FRIEND_VALUE_DAY = 735 ,    //每日友情值

        VAR_LINGSHI_PEIYANG_CNT = 736, //灵侍仙石培养次数
        VAR_LINGSHI_PEIYANG_LUCKY = 737, //灵侍培养出现暴击记录
        VAR_TREE_VALUE = 738 ,    //木片数
        VAR_TREE_TIME = 739,      //砍树时间
        VAR_TREE_TOOL = 740,      //伐木工具
        VAR_CUTTREE_BUY = 741 ,   //玩家购买的剩余伐木次数 
        VAR_CUTTREE_COUNT = 742 ,    //伐木次数 主动发起(8位) ,玩家总购买的次数 
        VAR_CUBE_COUNT = 743,      //当前积木消耗数  (每一层构造好的时候清零)
        VAR_TREE_VALUE_DAY = 744 ,    //今日被邀请获得的木片数
        VAR_TOOL_CNT = 745 ,    //当前欧耶精粹数量
        VAR_WORLDCUP_RES = 746 ,    //世界杯积分
        VAR_ZIYUN_KUANG = 747 ,  //紫云矿
        VAR_ZIYUN_MU = 748 ,     //紫云木
        VAR_ZIYUN_PAI = 749 ,    //紫云牌
        VAR_ZIYUN_LIANFU = 750 , //紫云炼符



        //752 - 770 for dtc
        VAR_LIMIT_APPOINTMENT = 752,//取消婚礼预约限制
        VAR_CLANBOSS_LUCKY_LIMIT = 754,//帮派BOSS幸运奖励个数
        VAR_CLANBOSS_SECURE_LIMIT = 755,//帮派BOSS秘密奖励个数
        VAR_CLANBOSS_CLANBIGBOSS_LIMIT = 756,//帮派BOSS换帮限制
        VAR_WB_INSPIRE = 757,//世界BOSS鼓舞次数 银币(8位) 仙石(8位)
        VAR_WB_SKIPBATTLE = 758,//世界BOSS跳过战斗标记
        VAR_WB_EXPSUM = 759,//世界BOSS总经验
        VAR_WB_RELIVENUM = 760,//世界BOSS复活次数
        VAR_GUANGGUN_TODAY_STAR = 761,     //每日财富之星 清除
        VAR_GUANGGUN_TEAMSTAR = 762,     //团队财富之星 不清除
        VAR_GUANGGUN_TODAY_TEAMSCORE = 763,     //每日小队积分
        VAR_GUANGGUN_TENTIMES = 764,     //每日小队积分
        VAR_FLYROAD_SW = 765,     //飞升-圣王好感度
        VAR_FLYROAD_FT = 766,     //飞升-佛陀好感度
        VAR_FLYROAD_ZZ = 767,     //飞升-尊者好感度
        VAR_FLYROAD_DJ = 768,     //飞升-帝君好感度
        VAR_FLYROAD_FLAG = 769,     //飞升-仙格标记
        VAR_FLYROAD_DROP_STATUS = 770,     //飞升之路掉落标记
        //771 - 780 for qimj
        VAR_CUR_CLAN_TITLE = 771, //帮派当前称号
        VAR_FIRE_SACRIFICE_TIMES = 772, //拜火祭天：0-7帮众添柴次数，8-15帮主诏令次数
        VAR_FIRE_SACRIFICE_ADD_WOOD_TIME = 773, //拜火祭天添柴时间
        VAR_FIRE_SACRIFICE_CALLING_TIME = 774, //拜火祭天召唤帮众时间
        VAR_DUOBAO_ACTIVE_POINT = 775, //夺宝奇兵活跃值+1(一天一点) 
        VAR_CLAN_SHOP_CURRENT_LVL = 776, //帮贡商店当前等级
        VAR_CLAN_SHOP_FLUSH_TIMES = 777, //帮贡商店当天刷新次数
        VAR_COOL_SUMMER_ACTIVE_POINT = 778, //酷爽一夏当天活跃值
        VAR_COOL_SUMMER_STATUS = 779, //0-7位:酷爽一夏活跃领奖状态(0双倍，1单倍),8-15位：当前已领礼包编号1-5
        VAR_COOL_SUMMER_RAND_TYPE = 780, //酷爽一夏抽奖类型

        //781 - 810 for lijr
        VAR_ERLKING_STATUSA = 781,               //妖王再临状态(1-16)
        VAR_ERLKING_STATUSB = 782,               //妖王再临状态(17-32)
        VAR_ERLKING_STATUSC = 783,               //妖王再临状态(33-48)
        VAR_ERLKING_BUY_PASS_NUM_DAY = 784,      //妖王再临购买通关次数
        VAR_ERLKING_USE_FREE_NUM_DAY = 785,      //妖王再临使用免费通关次数
        VAR_ERLKING_BUY_PASS_TOTAL_NUM = 786,    //妖王再临购买通关总次数

        VAR_KJTM_AWARD_MARK = 787,               //抗击天魔奖励标志
        VAR_KJTM_LOGIN_NUM = 788,                //抗击天魔登录次数
        VAR_ANSWER_QUESTIONS_STATUS = 789,       //一战成名回答问题状态（0位代表当前这道题回答是否正确, 1-30位每一位代表是否参与该道题）
        VAR_ANSWER_LITERARY_VALUE = 790,         //一战成名文采值
        VAR_ANSWER_SUCCESSION_NUM = 791,         //一战成名连击数量
        VAR_ANSWER_ATTACK_NUM = 792,             //一战成名天眼通数量
        VAR_ANSWER_SKILL_STATUS = 793,           //一战成名技能状态 (每5位表示一种技能状态)
        VAR_ANSWER_SKILL_MARK = 794,             //一战成名本题所用技能标志 (每1位表示一种技能在本题中是否使用)
        VAR_ANSWER_SKILL_USE_NUM = 795,          //一战成名技能使用数量 (每2位表示一种技能已使用数量)
        VAR_ANSWER_QUESTIONS_SUCCORFAIL = 796,   //一战成名回答问题成功失败（1-30位每一位代表是否回答成功该道题）
        VAR_ANSWER_QUESTIONS_OPTION = 797,       //一战成名题号和当前选项(一个5位表示题号，第二个题号表示选项)
        VAR_AUTO_TEAMCOPY_INDEX = 798,           //组队副本本次需要自动的索引
        VAR_AUTO_TEAMCOPY_CUR_INDEX = 799,       //自动组队副本当前所进行的副本索引
        VAR_AUTO_TEAMCOPY_CNT = 800,             //自动组队副本总共需要战斗怪物的数量
        VAR_AUTO_TEAMCOPY_CUR_CNT = 801,         //自动组队副本当前战斗到第几个怪物
        VAR_AUTO_TEAMCOPY_TEMP_CNT = 802,        //自动组队副本已经打过的副本怪物数量
        VAR_GEAR_BUFF = 803,                     //机关库挂机经验buff
        VAR_JIGUAN_SCHEME = 804,                 //机关当前选择方案

        //811-830 for libo
        VAR_HAPPY_XXL_DAYCOUNT = 811 , //快乐消消乐 每日赠送(最大20)
        VAR_HAPPY_XXL_BUYCOUNT = 812 , //快乐消消乐 购买获得
        VAR_HAPPY_XXL_BUYNUM = 813 , //快乐消消乐 当日购买次数
        VAR_HAPPY_XXL_AWARD = 814 , //快乐消消乐 每日奖励领取情况
        VAR_HAPPY_XXL_SCORE = 815 , //快乐消消乐 每日积分
        VAR_HAPPY_XXL_PAGE = 816 , //快乐消消乐 编号

        VAR_11AIRBOOK_AWARDSCORE = 817,  
        VAR_KETTLE_TIME = 818,  //炼妖壶刷新
        VAR_KETTLE_COUNT = 819,  //炼妖值
        VAR_KETTLE_4CNT = 820,  //4星刷新次数
        VAR_KETTLE_5CNT = 821, //5星刷新次数
        VAR_KETTLE_LOCK = 822,  //是否锁定

        VAR_FAIRYCOPY_FREE = 823, //仙界副本免费次数
        VAR_FAIRYCOPY_GOLD = 824, //仙界副本付费次数
            
        VAR_BAIFU_BAG_STATUE = 825 , //百服大礼包状态  0-未满足 1-未领取 2-已领取
        VAR_XCTJ_COUNT = 826,       //喜从天降活跃度
        VAR_XCTJ_AWARD = 827,       //喜从天降活跃度奖励领取
        VAR_XCTJ_REPEAT = 828,      //喜从天降奖励重复度
        VAR_XCTJ_FULI = 829,      //喜从天降福利发放次数
        VAR_XCTJ_RECHARGE = 830,      //喜从天降充值
        //831-850 for qimj
        VAR_COOL_SUMMER_AWARD_TYPE = 831,        //酷爽一夏奖励类型
        VAR_COOL_SUMMER_ACTIVE_POINT_TOTAL = 832, //酷爽一夏累积活跃值
        VAR_SHU_SHAN_WEI_WEI_XDPB = 833, //蜀山微微"喜大普奔"奖励状态(1-6位)
        VAR_SHU_SHAN_WEI_WEI_MSYJ = 834, //蜀山微微"码上有奖"是否领取抽奖券(0：未领，1：已领,一天一次)
        VAR_SHU_SHAN_WEI_WEI_WXSC = 835, //蜀山微微"微信商城"奖励状态(1-7位,第一位为免费礼券)
        VAR_SHU_SHAN_WEI_WEI_WXSC_COUPON = 836, //蜀山微微"微信商城"免费礼券是否领取(0未领，1已领)
        VAR_SHU_SHAN_WEI_WEI_MSYJ_AWARD = 837, //蜀山微微"码上有奖",抽到奖励和数量(0-15为物品ID，16-31为物品数量)
        VAR_SEEKING_HER_BEAN_TOTAL = 838, //众里寻她收到豆的数量
        VAR_SEEKING_HER_CHARM_POINT = 839,  //众里寻她魅力值
        VAR_SEEKING_HER_ANNOUNCE_TIME = 840,  //众里寻她宣言时间
        VAR_FIRST_POT_GOLD_STATUS = 841, //第一桶金充值状态
        VAR_HIDE_VIP_LEVEL_FLAG = 842, //隐藏御剑等级标识(0未隐藏， 1隐藏)
        VAR_SEEKING_HER_CHARM_AWARD = 843, //众里寻她魅力值奖励领取情况(每位代表一个分数等级，0未领，1领取)
        VAR_SEEKING_HER_LOOT_STATUS = 844,  //众里寻他掉落状态
        VAR_CARNIVAL_CONSUME_TOTAL = 845,  //狂欢购物总消费仙石数
        VAR_CARNIVAL_CONSUME_TODAY_TOTAL = 846,  //狂欢购物当天消费仙石数
        VAR_CARNIVAL_CONSUME_TOTAL_REBATE = 847,  //狂欢购物总返利
        VAR_CARNIVAL_CONSUME_SHAKE_TIMES = 848,  //狂欢购物总返利领取次数(总共10次)
        VAR_GRATITUDE_GIVING_AWARD_STATUS = 849, //三周年感恩大回馈领奖状态(0未领，1领取，从低逐位代表:银币，礼券，修为)
        VAR_GRATITUDE_GIVING_LEVEL = 850, //感恩大回馈结束时玩家等级

        //851-870 for qimj
        VAR_GRATITUDE_GIVING_RECHARGE = 851, //感恩大回馈结束时玩家充值总数
        VAR_CARNIVAL_CONSUME_REBATE_FLAG = 852, //狂欢购物返利标志(1返仙石，0返礼券)
        VAR_CARNIVAL_CONSUME_SHAKE_STATUS = 853, //狂欢购物总返利当天领取状态(1领取，0未领)
        VAR_FLY_ROAD_GIFT_TODAY_STATUS = 854, //飞升之路礼包当天是否领取(0未领取，1领取)
        VAR_FLY_ROAD_GIFT_GOT_NUM = 855, //飞升之路礼包当前已领取次数(0-10)
        VAR_MEMOIR_TODAY_WEIXIN_STATUS = 856, //三年回忆录当天微信领取状态
        VAR_MEMOIR_TODAY_WEIBO_STATUS = 857, //三年回忆录当天微博领取状态
        VAR_MEMOIR_WEIXIN_COUNT = 858, //三年回忆录微信领取次数
        VAR_MEMOIR_WEIBO_COUNT = 859, //三年回忆录微博领取次数

        //871 -890 for dtc
        VAR_TREASURE_CONSUME = 871, //藏宝阁个人消费累计数
        VAR_TREASURE_CONSUME_EXT = 872, //藏宝阁个人消费累计积分剩余数
        VAR_TREASURE_SCORE = 873, //藏宝阁个人积分数
        VAR_JIQIREN_XJFRONTMAP = 874, //机器人之璇玑阵图未完成次数(1byte免费，2byte付费1，3byte付费2，4byte付费3)
        VAR_JIQIREN_FAIRYCOPY = 875,  //机器人之仙界副本未完成次数(1byte免费，2byte付费1，3byte付费2，4byte付费3)
        VAR_JIQIREN_ERLKING= 876,  //机器人之妖王未完成次数

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
#define TARGET_SENDRANDFRIEND     0x0800
#define TARGET_SELECTPRAY         0x1000


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
            REGISTER_VAR(VAR_RECHARGE_TIME, CYCLE_NONE);

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
            REGISTER_VAR(VAR_TJ_TASK3_COPYID, CYCLE_NONE);
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
            REGISTER_VAR(VAR_RECHARGE_TODAY, CYCLE_DAY);
            REGISTER_VAR(VAR_RECHARGE_TODAY_COUNTED, CYCLE_DAY);
            REGISTER_VAR(VAR_RECHARGE_CONDCNT, CYCLE_NONE);
            REGISTER_VAR(VAR_RECHARGE_CONDCNT_LAST, CYCLE_NONE);

            REGISTER_VAR(VAR_CTSLANDING_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_CTSLANDING_AWARD2, CYCLE_NONE);
            REGISTER_VAR(VAR_CTS_TARGET_COUNT, CYCLE_NONE);

            REGISTER_VAR(VAR_ITEM_9178_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9279_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9280_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9281_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9282_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_TOTALRECHARGEACT, CYCLE_NONE);
            REGISTER_VAR(VAR_DIRECTPUROPEN, CYCLE_NONE);
            REGISTER_VAR(VAR_DIRECTPURCNT, CYCLE_NONE);
            REGISTER_VAR(VAR_LEFTTIMES, CYCLE_NONE);
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
            REGISTER_VAR(VAR_VIP_PRIVILEGE_DATA_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_VIP_PRIVILEGE_BREAK_LAST, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERFLOW_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERFLOW_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_QQBOARD, CYCLE_NONE);
            REGISTER_VAR(VAR_QQBOARD_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYMEET, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYMEET_VIP, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYMEET_STRENTH_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_LUCKYMEET_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYMEET_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYMEET_RECHARGE_AWARD, CYCLE_NONE);
            
            REGISTER_VAR(VAR_SUMMER_MEET_TYPE, CYCLE_MONTH);
            REGISTER_VAR(VAR_SUMMER_MEET_TYPE_AWARD, CYCLE_MONTH);
            REGISTER_VAR(VAR_SUMMER_MEET_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMER_MEET_LOGIN_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMER_MEET_STRENTH_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMER_MEET_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMER_MEET_RECHARGE_AWARD, CYCLE_NONE);

            REGISTER_VAR(VAR_TUIGUAN_AWARD_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_PET_CUILIAN_DAILY_CNT_DATE, CYCLE_NONE);
            REGISTER_VAR(VAR_PET_CUILIAN_LEFT_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_PET_CUILIAN_GOLD_DAILY_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_QQEXPLORER_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_SURNAME_LEGEND_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_SYS_UPDATE_DLG, CYCLE_NONE);

            REGISTER_VAR(VAR_QQNAVIGATION_DAY_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_QQNAVIGATION_WEEK_AWARD, CYCLE_WEEK);
            REGISTER_VAR(VAR_QQNAVIGATION_FIRST_LOGIN_AWARD, CYCLE_NONE);

            REGISTER_VAR(VAR_QQMUSIC_DAY_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_QQTENPAY_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_QQTENPAY_LOTTERY, CYCLE_NONE);
            REGISTER_VAR(VAR_QQIM_QUICK_LOGIN_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYIN_MONEY_A, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYIN_MONEY_B, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYPET_SHOUHUN, CYCLE_NONE);
            REGISTER_VAR(VAR_EQUIP_MOVE_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_QISHIDOUFA_REST_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_QISHIDOUFA_LASTFAIL_HIGHTERSCORE, CYCLE_NONE);
            REGISTER_VAR(VAR_CHRISTMAS_PRESENT, CYCLE_NONE);
            
            REGISTER_VAR(VAR_COPY_AUTO_FIGHT_USE_MONEY_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_JOBHUNT_AUTO_FIGHT_USE_MONEY_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_VIPLEVEL_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_DROP_OUT_ITEM_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_QQXIU_AWARD, CYCLE_NONE);

            //REGISTER_VAR(VAR_ARENA_SUFFERED, CYCLE_DAY);
            //REGISTER_VAR(VAR_ARENA_LASTTIME, CYCLE_DAY);
            REGISTER_VAR(VAR_ARENA_SUPPORT_TUE, CYCLE_WEEK);
            REGISTER_VAR(VAR_ARENA_SUPPORT_WED, CYCLE_WEEK);
            REGISTER_VAR(VAR_FIRST_RECHARGE_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_FIRST_RECHARGE_STEP, CYCLE_NONE);
            REGISTER_VAR(VAR_CF_FLAG, CYCLE_NONE);
            REGISTER_VAR(VAR_CF_UNBIND, CYCLE_NONE);
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
            REGISTER_VAR(VAR_TEAMARENA_CREATE, CYCLE_DAY);
            REGISTER_VAR(VAR_TEAMARENA_WORSHIP, CYCLE_DAY);
            REGISTER_VAR(VAR_TEAMARENA_INSPIRE, CYCLE_NONE);
            REGISTER_VAR(VAR_SERVERWAR_JIJIANTAI, CYCLE_WEEK);
            REGISTER_VAR(VAR_SERVERWAR_JIJIANTAI1, CYCLE_WEEK);
            REGISTER_VAR(VAR_MOUNT_EXP, CYCLE_NONE);

            REGISTER_VAR(VAR_TOWER_LOGIN, CYCLE_DAY);
            REGISTER_VAR(VAR_TOWER_LEVEL, CYCLE_NONE);

            REGISTER_VAR(VAR_HAS_MO_BATTLER, CYCLE_NONE);
            REGISTER_VAR(VAR_ITEM_9316_FAILED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_CONSUME_AWARD_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_NEW_YEAR_GIVE_GIFT, CYCLE_NONE);

            REGISTER_VAR(VAR_DISCOUNT_CONSUME1, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_CONSUME2, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_CONSUME3, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_RECHARGE1, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_RECHARGE2, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_RECHARGE3, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_EX1_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_EX2_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_DISCOUNT_EX3_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_XINGCHENZHEN_VALUE, CYCLE_NONE);

            REGISTER_VAR(VAR_DRAGONKING_STEP, CYCLE_NONE);
            REGISTER_VAR(VAR_DRAGONKING_STEP4_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_SET_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_GET_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_SAVEGOLD_ISGET, CYCLE_DAY);

            REGISTER_VAR(VAR_LUCKYSTAR_IS_CONSUME, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYSTAR_GET_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYSTAR_RECHARGE_TOTAL, CYCLE_NONE);
            REGISTER_VAR(VAR_LUCKYSTAR_LOGIN_TIME, CYCLE_NONE);

            REGISTER_VAR(VAR_LONGYUAN_GOT, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE_TODAY_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_RP7_TREASURE1_GETTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE2_GETTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_TREASURE3_GETTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_SIGN, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_SIGN_PACKAGE, CYCLE_NONE);
            REGISTER_VAR(VAR_RP7_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_QIXI_DROP_STATUS, CYCLE_DAY);

            REGISTER_VAR(VAR_CALLSNAKEEGG, CYCLE_NONE);
            REGISTER_VAR(VAR_SNAKEEGG_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_9344_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_9343_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_SNAKE_SPRING_EQUIP_GOT, CYCLE_NONE);
            REGISTER_VAR(VAR_WEIBO_AWARD_GOT, CYCLE_NONE);
            REGISTER_VAR(VAR_FISHUSER_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_NUWA_SIGNET, CYCLE_NONE);
            REGISTER_VAR(VAR_NUWA_OPENTIME, CYCLE_NONE);
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
            REGISTER_VAR(VAR_LAST_HOOK_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_SPREAD_FLAG, CYCLE_DAY);
            REGISTER_VAR(VAR_SKILL_UP_FLAG, CYCLE_NONE);
            REGISTER_VAR(VAR_POOL_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_CARD_FROM_CLAN, CYCLE_DAY);
            REGISTER_VAR(VAR_MAP_INDEX, CYCLE_NONE);
            REGISTER_VAR(VAR_FEAST_LOGIN_AWARD_PER_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_TJ_AUTO_FRONTMAP_END_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_MOUNT_LEVEL, CYCLE_NONE);

            REGISTER_VAR(VAR_FOOLS_DAY_INFO, CYCLE_DAY);
            REGISTER_VAR(VAR_FOOLS_DAY_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_HAS_VOTE, CYCLE_DAY);
            REGISTER_VAR(VAR_POPULARITY, CYCLE_WEEK);
            REGISTER_VAR(VAR_FOOLBAO_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_HHBAWARD_GOT, CYCLE_DAY);
            REGISTER_VAR(VAR_SURNAMELEGEND_USED, CYCLE_NONE);
            REGISTER_VAR(VAR_CARD_1, CYCLE_NONE);
            REGISTER_VAR(VAR_CARD_2, CYCLE_NONE);
            REGISTER_VAR(VAR_CARD_3, CYCLE_NONE);
            REGISTER_VAR(VAR_CARD_4, CYCLE_NONE);
            REGISTER_VAR(VAR_CARD_5, CYCLE_NONE);
            REGISTER_VAR(VAR_LEVEL_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_ZCJB_RECHARGE_GOLD, CYCLE_NONE);
            REGISTER_VAR(VAR_CLANBOSS_GONGXIAN, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANKA_ACTION_UDPLOG, CYCLE_NONE);

            REGISTER_VAR(VAR_PET_CUILIAN_EXTRA_LOW_SCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_PET_CUILIAN_EXTRA_UP_SCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_PET_CUILIAN_SCORE_EQUIP, CYCLE_NONE);
            REGISTER_VAR(VAR_PET_CUILIAN_SCORE_GEM, CYCLE_NONE);
            REGISTER_VAR(VAR_PET_CUILIAN_SCORE_EQUIP_TIMES, CYCLE_NONE);
            REGISTER_VAR(VAR_PET_CUILIAN_SCORE_GEM_TIMES, CYCLE_NONE);

            REGISTER_VAR(VAR_ZCJB_TIMES, CYCLE_NONE);
            REGISTER_VAR(VAR_ZRYJ_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_HYYJ_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_1, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_2, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_3, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_4, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_5, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_6, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_7, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_8, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_9, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_10, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_11, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_12, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_13, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_14, CYCLE_NONE);
            REGISTER_VAR(VAR_RYHB_ITEM_CNT_15, CYCLE_NONE);

            REGISTER_VAR(VAR_CLAN_SPTR_WATER, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAN_SPTR_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAN_ACTPT_MONTH, CYCLE_MONTH);
            REGISTER_VAR(VAR_CLAN_ACTPT_FLAG, CYCLE_DAY);

            REGISTER_VAR(VAR_SUMMERFLOW3_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERFLOW3_TYPE_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERFLOW3_TIME_AWARD, CYCLE_DAY);

            REGISTER_VAR(VAR_PRAY_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_PRAY_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_PRAY_COUNT, CYCLE_WEEK);
            REGISTER_VAR(VAR_PRAY_LOGIN, CYCLE_WEEK);
            REGISTER_VAR(VAR_PRAY_SUCTIME, CYCLE_NONE);
            REGISTER_VAR(VAR_PRAY_TYPE_TODAY, CYCLE_DAY);
            REGISTER_VAR(VAR_PRAY_TIME, CYCLE_NONE);

            REGISTER_VAR(VAR_QQBOARD_ONLINE, CYCLE_DAY);
            REGISTER_VAR(VAR_QQBOARD_LOGIN_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_QQBOARD_ONLINE_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_RANDfRIEND, CYCLE_DAY);
            REGISTER_VAR(VAR_SENDPRESENT_VALUE, CYCLE_DAY);
            REGISTER_VAR(VAR_GETPRESENT_VALUE, CYCLE_DAY);
            REGISTER_VAR(VAR_PRESENT_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_11AIRBOOK_GRADE, CYCLE_NONE);
            REGISTER_VAR(VAR_11AIRBOOK_GRADE_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_TOTAL_BATTLE_POINT, CYCLE_NONE);
            REGISTER_VAR(VAR_VIA_ACC_DAYS, CYCLE_NONE);
            REGISTER_VAR(VAR_SEVEN_SOUL_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_SEX_CHANGE, CYCLE_NONE);
            REGISTER_VAR(VAR_SKILL_GRADE_MONEY, CYCLE_NONE);
            REGISTER_VAR(VAR_DROP_ACT, CYCLE_DAY);
            REGISTER_VAR(VAR_DIRECTPURCNT2, CYCLE_NONE);
            REGISTER_VAR(VAR_FEAST_GIFT_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_PRIVATE_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_PACKAGE_SIZE_GEM, CYCLE_NONE);
            REGISTER_VAR(VAR_PACKAGE_SIZE_FORMULA, CYCLE_NONE);
            REGISTER_VAR(VAR_PACKAGE_SIZE_SL, CYCLE_NONE);
            REGISTER_VAR(VAR_STRENGTH_LEVEL, CYCLE_NONE);
            REGISTER_VAR(VAR_AIRBOOK_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_AIRBOOK_LOGIN_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_AIRBOOK_ONLINE_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_AIRBOOK_RECHARGE, CYCLE_DAY);
            REGISTER_VAR(VAR_AIRBOOK_CONSUME, CYCLE_DAY);
            REGISTER_VAR(VAR_MODIFY_NAME_CD, CYCLE_NONE);

            REGISTER_VAR(VAR_GUANGGUN_TIMES, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_GETROLL, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_RECHARGE, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_CONSUME, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_ADVANCE_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_ADVANCE_OTHER, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_TODAY_SCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_ADVANCE_BUY, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_TODAY_TASK, CYCLE_NONE);
            REGISTER_VAR(VAR_NOV_LOGIN, CYCLE_NONE);
            REGISTER_VAR(VAR_NOV_LOGIN_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_GAMEBOX_DAILY, CYCLE_DAY);
            REGISTER_VAR(VAR_GAMEBOX_NEW, CYCLE_NONE);
            REGISTER_VAR(VAR_MARRY_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_CANCEL_APPOINTMENT, CYCLE_NONE);
            REGISTER_VAR(VAR_CANCEL_REPLYMARRIAGE, CYCLE_NONE);
            REGISTER_VAR(VAR_CANCEL_MARRIAGE, CYCLE_NONE);
            REGISTER_VAR(VAR_CANCEL_GIVEUP_JIEYUAN, CYCLE_NONE);
            REGISTER_VAR(VAR_COUPLE_ONLINE_START_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_COUPLE_ONLINE_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_COUPLE_ONLINE_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_COUPLE_ONLINE_FISH, CYCLE_DAY);
            REGISTER_VAR(VAR_COUPLE_ONLINE_RENAME, CYCLE_NONE);
            REGISTER_VAR(VAR_COUPLE_COPY_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_COUPLE_COPY_DAY_COUNT, CYCLE_DAY);
            REGISTER_VAR(VAR_COUPLE_COPY_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_COUPLE_COPY_BATTLE, CYCLE_DAY);
            REGISTER_VAR(VAR_FRIEND_SECURITY, CYCLE_NONE);
            REGISTER_VAR(VAR_REAL_SPIRIT, CYCLE_NONE);
            REGISTER_VAR(VAR_REAL_SPIRIT_GET, CYCLE_DAY);
            REGISTER_VAR(VAR_QZONE_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_QZONE_RECHARGE_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_QT_AWARD_MARK, CYCLE_MONTH);
            REGISTER_VAR(VAR_QT_REGIST_NUM, CYCLE_MONTH);
            REGISTER_VAR(VAR_QT_SPECIAL_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_QT_REGIST_MARK, CYCLE_MONTH);
            REGISTER_VAR(VAR_BUY_FUND_NuM, CYCLE_NONE);
            REGISTER_VAR(VAR_BUY_FUND_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_BUY_FUND_TRUMP_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_ORDINARY_KILL_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_DIFFICULTY_KILL_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_ORDINARY_REFRESH_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_DIFFICULTY_REFRESH_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_HELP_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_PURPLE_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_ORANGE_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_PETTEAMCOPY_BATTLE_RES, CYCLE_NONE);
            REGISTER_VAR(VAR_REFRESH_AWARD_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_OPENCARD_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_BUY_OPENCARD_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_MOBAO_REFRESH_AWARD_MARK, CYCLE_DAY);

            REGISTER_VAR(VAR_OLDMAN_DAYSCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_OLDMAN_SCORE, CYCLE_NONE);
            REGISTER_VAR(VAR_OLDMAN_PRESENT, CYCLE_DAY);
            REGISTER_VAR(VAR_OLDMAN_DAYSCORE_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_OLDMAN_SCORE_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_YEARHAPPY_DAYVALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_YEARHAPPY_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_YEARHAPPY_DAYVALUE_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_YEARHAPPY_LEFTVALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_MARRYBOARD_LIVELY, CYCLE_NONE);
            REGISTER_VAR(VAR_MARRYBOARD3, CYCLE_DAY);
            REGISTER_VAR(VAR_MARRYBOARD2_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_MARRYBOARD2_ANS, CYCLE_DAY);
            REGISTER_VAR(VAR_MARRYBOARD4_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_MARRYBOARD_YANHUA, CYCLE_NONE);
            REGISTER_VAR(VAR_3366_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_3366_RECHARGE_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_MARRYBOARD_BAIHE, CYCLE_DAY);

            REGISTER_VAR(VAR_NEWYEARSPEED_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_ONCE_ONDAY, CYCLE_DAY);
            REGISTER_VAR(VAR_NEWYEAR_PRATICE_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERMEET_SCORE1, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERMEET_SCORE2, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERMEET_SCORE3, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERMEET_SCORE4, CYCLE_NONE);
            REGISTER_VAR(VAR_SUMMERMEET_SCORE5, CYCLE_NONE);
            REGISTER_VAR(VAR_COUPLE_NAME, CYCLE_NONE);
            REGISTER_VAR(VAR_MicroCloud_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_COPY, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_FRONTMAP, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_DUNGEON, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_SYBS, CYCLE_NONE);
            REGISTER_VAR(VAR_LEFTADDR_POWER, CYCLE_NONE);
            REGISTER_VAR(VAR_LEFTADDR_CREATE, CYCLE_NONE);
            REGISTER_VAR(VAR_HEART_SWORD, CYCLE_NONE);
            REGISTER_VAR(VAR_FRIEND_TASK1, CYCLE_DAY);
            REGISTER_VAR(VAR_FRIEND_TASK2, CYCLE_DAY);
            REGISTER_VAR(VAR_FRIEND_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_CLAN_FRIEND, CYCLE_NONE);
            REGISTER_VAR(VAR_LEFTADDR_POWER_ADD, CYCLE_DAY);

            REGISTER_VAR(VAR_QISHI_FRIEND_SEND_COUNT, CYCLE_DAY);
            REGISTER_VAR(VAR_QISHI_FRIEND_USE_COUNT, CYCLE_DAY);
            REGISTER_VAR(VAR_MOUNT_CANGJIANYA_LEFT_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_MOUNT_CANGJIANYA_DATE, CYCLE_NONE);
            REGISTER_VAR(VAR_CLAN_DUOBAO_SCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAN_DUOBAO_SUCCESS_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAN_DUOBAO_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_ZHENYUAN_TIQU_DATE, CYCLE_NONE);
            REGISTER_VAR(VAR_ZHENYUAN_TIQU_CNT, CYCLE_NONE);

            REGISTER_VAR(VAR_GUANKA_ACTION_NPC, CYCLE_NONE);
            REGISTER_VAR(VAR_GUANKA_ACTION_SCORE, CYCLE_NONE);
            REGISTER_VAR(VAR_GUANKA_ACTION_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_KJTM_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_KJTM_KILL_NPC_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_KJTM_LOGIN_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_KJTM_LOGIN_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_KJTM_AWARD_MARK, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_QUESTIONS_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_LITERARY_VALUE, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_SUCCESSION_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_ATTACK_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_SKILL_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_SKILL_MARK, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_SKILL_USE_NUM, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_QUESTIONS_SUCCORFAIL, CYCLE_DAY);
            REGISTER_VAR(VAR_ANSWER_QUESTIONS_OPTION, CYCLE_DAY);
            REGISTER_VAR(VAR_AUTO_TEAMCOPY_INDEX, CYCLE_NONE);
            REGISTER_VAR(VAR_AUTO_TEAMCOPY_CUR_INDEX, CYCLE_NONE);
            REGISTER_VAR(VAR_AUTO_TEAMCOPY_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_AUTO_TEAMCOPY_CUR_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_AUTO_TEAMCOPY_TEMP_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_GEAR_BUFF, CYCLE_NONE);
            REGISTER_VAR(VAR_JIGUAN_SCHEME, CYCLE_NONE);

            REGISTER_VAR(VAR_TYSS_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_TYSS_CONTRIBUTE_PLAYER, CYCLE_NONE);
            REGISTER_VAR(VAR_TYSS_CONTRIBUTE_PLAYER_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_TYSS_CONTRIBUTE_CLAN, CYCLE_NONE);
            REGISTER_VAR(VAR_TYSS_DISCOUNT_CONSUME1, CYCLE_NONE);
            REGISTER_VAR(VAR_TYSS_DISCOUNT_CONSUME2, CYCLE_NONE);
            REGISTER_VAR(VAR_TYSS_DISCOUNT_CONSUME3, CYCLE_NONE);
            REGISTER_VAR(VAR_TYSS_CONTRIBUTE_CLAN_SUM, CYCLE_NONE);

            REGISTER_VAR(VAR_FRIEND_ACHIEVEMENT, CYCLE_NONE);
            REGISTER_VAR(VAR_DRINK_COUNT, CYCLE_DAY);
            REGISTER_VAR(VAR_CLAN_FRIEND, CYCLE_NONE);
            REGISTER_VAR(VAR_DRINK_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_FRIEND_VALUE_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_CUTTREE_BUY, CYCLE_NONE);
            REGISTER_VAR(VAR_CUTTREE_COUNT, CYCLE_DAY);
            REGISTER_VAR(VAR_TREE_VALUE, CYCLE_NONE);
            REGISTER_VAR(VAR_TREE_VALUE_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_TOOL_CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_CUR_CLAN_TITLE, CYCLE_NONE);
            REGISTER_VAR(VAR_FIRE_SACRIFICE_TIMES, CYCLE_DAY);
            REGISTER_VAR(VAR_FIRE_SACRIFICE_ADD_WOOD_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_FIRE_SACRIFICE_CALLING_TIME, CYCLE_DAY);
            REGISTER_VAR(VAR_DUOBAO_ACTIVE_POINT, CYCLE_DAY);
            REGISTER_VAR(VAR_CUBE_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_COOL_SUMMER_ACTIVE_POINT, CYCLE_DAY);
            REGISTER_VAR(VAR_COOL_SUMMER_ACTIVE_POINT_TOTAL, CYCLE_NONE);
            REGISTER_VAR(VAR_SHU_SHAN_WEI_WEI_XDPB, CYCLE_NONE);
            REGISTER_VAR(VAR_SHU_SHAN_WEI_WEI_MSYJ, CYCLE_DAY);
            REGISTER_VAR(VAR_SHU_SHAN_WEI_WEI_WXSC, CYCLE_NONE);
            REGISTER_VAR(VAR_SHU_SHAN_WEI_WEI_WXSC_COUPON, CYCLE_NONE);
            REGISTER_VAR(VAR_SHU_SHAN_WEI_WEI_MSYJ_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_COOL_SUMMER_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_COOL_SUMMER_RAND_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_COOL_SUMMER_AWARD_TYPE, CYCLE_NONE);
            REGISTER_VAR(VAR_CLAN_SHOP_CURRENT_LVL, CYCLE_NONE);
            REGISTER_VAR(VAR_CLAN_SHOP_FLUSH_TIMES, CYCLE_DAY);
            REGISTER_VAR(VAR_SEEKING_HER_BEAN_TOTAL, CYCLE_NONE);
            REGISTER_VAR(VAR_SEEKING_HER_CHARM_POINT, CYCLE_NONE);
            REGISTER_VAR(VAR_SEEKING_HER_ANNOUNCE_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_SEEKING_HER_CHARM_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_SEEKING_HER_LOOT_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_CARNIVAL_CONSUME_TOTAL, CYCLE_NONE);
            REGISTER_VAR(VAR_CARNIVAL_CONSUME_TOTAL_REBATE, CYCLE_NONE);
            REGISTER_VAR(VAR_CARNIVAL_CONSUME_SHAKE_TIMES, CYCLE_NONE);
            REGISTER_VAR(VAR_CARNIVAL_CONSUME_TODAY_TOTAL, CYCLE_DAY);
            REGISTER_VAR(VAR_GRATITUDE_GIVING_AWARD_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_GRATITUDE_GIVING_LEVEL, CYCLE_NONE);
            REGISTER_VAR(VAR_GRATITUDE_GIVING_RECHARGE, CYCLE_NONE);
            REGISTER_VAR(VAR_CARNIVAL_CONSUME_REBATE_FLAG, CYCLE_NONE);
            REGISTER_VAR(VAR_CARNIVAL_CONSUME_SHAKE_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_FLY_ROAD_GIFT_GOT_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_MEMOIR_TODAY_WEIBO_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_MEMOIR_WEIBO_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_MEMOIR_WEIXIN_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_MEMOIR_TODAY_WEIXIN_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_FLY_ROAD_GIFT_TODAY_STATUS, CYCLE_DAY);
            REGISTER_VAR(VAR_FIRST_POT_GOLD_STATUS, CYCLE_NONE);
            REGISTER_VAR(VAR_HIDE_VIP_LEVEL_FLAG, CYCLE_NONE);

            REGISTER_VAR(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, CYCLE_NONE);
            REGISTER_VAR(VAR_ATOXJFM, CYCLE_NONE);
            REGISTER_VAR(VAR_LINGSHI_PEIYANG_CNT, CYCLE_DAY);
            REGISTER_VAR(VAR_LINGSHI_PEIYANG_LUCKY, CYCLE_NONE);
            REGISTER_VAR(VAR_LIMIT_APPOINTMENT, CYCLE_NONE);
            REGISTER_VAR(VAR_CLANBOSS_LUCKY_LIMIT, CYCLE_DAY);
            REGISTER_VAR(VAR_CLANBOSS_SECURE_LIMIT, CYCLE_DAY);
            REGISTER_VAR(VAR_CLANBOSS_CLANBIGBOSS_LIMIT, CYCLE_DAY);
            REGISTER_VAR(VAR_WB_INSPIRE, CYCLE_HOUR);
            REGISTER_VAR(VAR_WB_SKIPBATTLE, CYCLE_HOUR);
            REGISTER_VAR(VAR_WB_EXPSUM, CYCLE_HOUR);
            REGISTER_VAR(VAR_WB_RELIVENUM, CYCLE_HOUR);
            REGISTER_VAR(VAR_GUANGGUN_TODAY_STAR, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_TEAMSTAR, CYCLE_NONE);
            REGISTER_VAR(VAR_GUANGGUN_TODAY_TEAMSCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_GUANGGUN_TENTIMES, CYCLE_NONE);
            REGISTER_VAR(VAR_FLYROAD_SW, CYCLE_NONE);
            REGISTER_VAR(VAR_FLYROAD_FT, CYCLE_NONE);
            REGISTER_VAR(VAR_FLYROAD_ZZ, CYCLE_NONE);
            REGISTER_VAR(VAR_FLYROAD_DJ, CYCLE_NONE);
            REGISTER_VAR(VAR_FLYROAD_FLAG, CYCLE_NONE);
            REGISTER_VAR(VAR_FLYROAD_DROP_STATUS, CYCLE_DAY);

            REGISTER_VAR(VAR_ERLKING_STATUSA, CYCLE_NONE);
            REGISTER_VAR(VAR_ERLKING_STATUSB, CYCLE_NONE);
            REGISTER_VAR(VAR_ERLKING_STATUSC, CYCLE_NONE);
            REGISTER_VAR(VAR_ERLKING_BUY_PASS_NUM_DAY, CYCLE_DAY);
            REGISTER_VAR(VAR_ERLKING_BUY_PASS_TOTAL_NUM, CYCLE_NONE);
            REGISTER_VAR(VAR_ERLKING_USE_FREE_NUM_DAY, CYCLE_DAY);

            REGISTER_VAR(VAR_HAPPY_XXL_DAYCOUNT, CYCLE_DAY);
            REGISTER_VAR(VAR_HAPPY_XXL_BUYCOUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_HAPPY_XXL_BUYNUM, CYCLE_DAY);
            REGISTER_VAR(VAR_HAPPY_XXL_AWARD, CYCLE_DAY);
            REGISTER_VAR(VAR_HAPPY_XXL_SCORE, CYCLE_DAY);
            REGISTER_VAR(VAR_HAPPY_XXL_PAGE, CYCLE_NONE);
            REGISTER_VAR(VAR_11AIRBOOK_AWARDSCORE, CYCLE_NONE);
            REGISTER_VAR(VAR_KETTLE_TIME, CYCLE_NONE);
            REGISTER_VAR(VAR_KETTLE_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_KETTLE_4CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_KETTLE_5CNT, CYCLE_NONE);
            REGISTER_VAR(VAR_KETTLE_LOCK, CYCLE_NONE);
            REGISTER_VAR(VAR_FAIRYCOPY_FREE, CYCLE_DAY);
            REGISTER_VAR(VAR_FAIRYCOPY_GOLD, CYCLE_DAY);
            REGISTER_VAR(VAR_BAIFU_BAG_STATUE, CYCLE_NONE);
            REGISTER_VAR(VAR_XCTJ_COUNT, CYCLE_NONE);
            REGISTER_VAR(VAR_XCTJ_AWARD, CYCLE_NONE);
            REGISTER_VAR(VAR_XCTJ_REPEAT, CYCLE_NONE);
            REGISTER_VAR(VAR_XCTJ_FULI, CYCLE_NONE);
            REGISTER_VAR(VAR_XCTJ_RECHARGE, CYCLE_NONE);
            
            REGISTER_VAR(VAR_TREASURE_CONSUME, CYCLE_NONE);
            REGISTER_VAR(VAR_TREASURE_CONSUME_EXT, CYCLE_NONE);
            REGISTER_VAR(VAR_TREASURE_SCORE, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_XJFRONTMAP, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_FAIRYCOPY, CYCLE_NONE);
            REGISTER_VAR(VAR_JIQIREN_ERLKING, CYCLE_NONE);
        }

        UInt32 GetVar(UInt32 id, UInt32 now = 0);
        void SetVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void DelVar(UInt32 id);
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

