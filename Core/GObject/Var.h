
#ifndef _VAR_H_
#define _VAR_H_

#include "Common/TimeUtil.h"

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
        VAR_INVALID = 0,    //??Ч??��
        VAR_SINGLE_CHOPSTICKS = 1, //???????Ϳ???
        VAR_SENGYI = 2,     // ɮ??
        VAR_APRON = 3,      // ?Ƕ?
        VAR_TGDT = 4,       // ?ж???????ʱ??
        VAR_TGDCNT = 5,     // ?ж???��??????3Сʱ????
        VAR_ATOFM = 6,     // ?Զ???ͼid
        VAR_ICEMAN = 7,     // ѩ??
        VAR_BOOT = 8,     // ʥ??ѥ
        VAR_TODAY_ONLINE = 9,     // time -- online today
        VAR_KEYPACK1 = 10,  // KEY????
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
        VAR_BATTLE_MISS = 23, //???????
        VAR_BATTLE_CS  = 24,  //
        VAR_BATTLE_PR = 25,   //???????
        VAR_BATTLE_FJ = 26,  //???????
        VAR_BATTLE_SKILL_DMG = 27, //???????
        VAR_BATTLE_PEERLESS_DMG = 28,//????????
        
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
        VAR_RECHARGE_TOTAL = 46, //新年充值活动

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
        VAR_FOOLS_DAY = 58, //愚人节礼包是否领取
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
        VAR_RC7DAYWILL = 88, // 所选择的愿望索引
        VAR_INVITES = 89, // 邀请好友的次数
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
        VAR_INVITED = 109, // 今天领取过邀请好友奖励次数

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
        VAR_DUANWU = 135, // 端午上线邮件
        VAR_MAX,
    };


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
            REGISTER_VAR(VAR_TGDT, CYCLE_MONTH);
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

            REGISTER_VAR(VAR_CLAWARD2, CYCLE_DAY);

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
            REGISTER_VAR(VAR_DUANWU, CYCLE_NONE);
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

        static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

