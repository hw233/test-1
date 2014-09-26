
#ifndef _GVAR_H_
#define _GVAR_H_

#include "Common/TimeUtil.h"
#include "Common/Singleton.h"
#include "Common/Mutex.h"

namespace GObject
{
    //变量周期类型
    enum GCycleType
    {
        GCYCLE_NONE =    0,  //无周期
        GCYCLE_MIN =     1,  //分
        GCYCLE_HOUR =    2,  //时
        GCYCLE_DAY =     3,  //日
        GCYCLE_WEEK =    4,  //周
        GCYCLE_MONTH =   5,  //月
        GCYCLE_YEAR =    6,  //年

        GCYCLE_MAX,
    };

    enum GVarID
    {
        GVAR_INVALID = 0,
        GVAR_TCPASS = 1, // 组队副本最高通过等级
        GVAR_CITTASPLIT = 2, // 心法拆分成两个处理
        GVAR_CITTASMERGE = 3, // 心法残页合并
        GVAR_CLANCOPYPASS = 4, // 帮派副本最高通过等级
        GVAR_1530BUCHANG = 5,
        GVAR_OLDRC7DAYBUCHANG = 6,
        GVAR_DT_SESSIONTIME = 7,
        GVAR_DT_STAGE = 8,
        GVAR_DT_RESPOINT_RED = 9,
        GVAR_DT_RESPOINT_BLUE = 10,
        GVAR_DT_TOTALRES_RED = 11,
        GVAR_DT_TOTALRES_BLUE = 12,
        GVAR_ARENA_EXT_PROCESS = 13,
        GVAR_TJ_TOWN_999_BUG = 14,  //999级天劫关闭时,导致未过天劫但过了锁妖塔等级,领取法宝的BUG
        GVAR_JOB_MO_PEXP = 15,  // 墨修为补偿

        GVAR_DISCOUNT_TYPE1 = 16, // 当前限购栏类型(0 普通 1 消费 2 充值)
        GVAR_DISCOUNT_TYPE2 = 17, // 当前限购栏类型(0 普通 1 消费 2 充值)
        GVAR_DISCOUNT_TYPE3 = 18, // 当前限购栏类型(0 普通 1 消费 2 充值)

        GVAR_DISCOUNT_BEGIN1 = 19, // 当前限购栏一开始时间
        GVAR_DISCOUNT_BEGIN2 = 20, // 当前限购栏二开始时间
        GVAR_DISCOUNT_BEGIN3 = 21, // 当前限购栏三开始时间

        GVAR_DISCOUNT_END1 = 22, // 当前限购栏一结束时间
        GVAR_DISCOUNT_END2 = 23, // 当前限购栏二结束时间
        GVAR_DISCOUNT_END3 = 24, // 当前限购栏三结束时间

        GVAR_HEROMEM_CUT = 25,  // 剑侠秘籍去除标志（用于更新时发送老玩家补偿的）
        GVAR_DRAGONKING_ACTION = 26, //大闹龙宫活动标志
        GVAR_DRAGONKING_BEGIN = 27, //大闹龙宫活动开始时间
        GVAR_DRAGONKING_END = 28, //大闹龙宫活动结束时间

        GVAR_EXP_HOOK_NEW = 29, //挂机经验从旧方案向新方案转变

        GVAR_TREASURE_ACTION = 30, //聚宝盆活动标志
        GVAR_TREASURE_BEGIN = 31, //聚宝盆活动开始时间
        GVAR_TREASURE_END = 32, //聚宝盆活动结束时间

        GVAR_HAS_CLAC_LB_BP = 33, // 是否计算过灵宝的战斗力

        GVAR_SURNAMELEGEND_BEGIN = 34,
        GVAR_SURNAMELEGEND_END = 35 ,
        GVAR_LUCKYSTAR_BEGIN = 36, //幸运星活动开始时间
        GVAR_LUCKYSTAR_END   = 37, //幸运星活动结束时间

        GVAR_TOTALRECHARGEACT_S = 38, //充值活动开始时间（IDIP）
        GVAR_TOTALRECHARGEACT_E = 39, //充值活动结束时间（IDIP）

        GVAR_SPREAD_KEEPER1 = 40, //持有者ID高32位
        GVAR_SPREAD_KEEPER2 = 41, //持有者ID低32位
        GVAR_SPREAD_CONDITION = 42, //0~7bits代表阶段(1代表刚开始(11:00);2代表刚结束(22:00);8-31bits代表使用秘典的人的个数)
        GVAR_RYHB_ACTIVITY_BEGIN = 43, // 日月合璧活动开始时间
        GVAR_RYHB_ACTIVITY_END = 44, // 日月合璧活动结束时间
        GVAR_ZCJB_ACTIVITY_BEGIN = 45, // 招财进宝活动开始时间
        GVAR_ZCJB_ACTIVITY_END = 46, // 招财进宝活动结束时间

        GVAR_NewUser_Cur = 47, 
        GVAR_NewUser_Max = 48, 


        GVAR_LUCKYMEET_BEGIN = 49,
        GVAR_LUCKYMEET_END  = 50,

        GVAR_SUMMER_MEET_BEGIN = 51,
        GVAR_SUMMER_MEET_END = 52,

        GVAR_SUMMER_FLOW_BEGIN = 53,
        GVAR_SUMMER_FLOW_END = 54,

        GVAR_QZONEQQGAME_BEGIN = 55,   //蓝钻空间
        GVAR_QZONEQQGAME_END = 56,

        GVAR_QISHIBANGAME_BEGIN = 57,  //七石斗法开始时间
        GVAR_QISHIBANGAME_END = 58,    //七石斗法结束时间

        GVAR_QZONEQQGAMEY_BEGIN = 59,  //黄钻空间
        GVAR_QZONEQQGAMEY_END = 60,

        GVAR_QZONE_RECHARGE_BEGIN = 61,
        GVAR_QZONE_RECHARGE_END = 62,

        GVAR_SERVERWAR_ISENTER = 63,    //跨服服战是否报名
        GVAR_SERVERWAR_JIJIANTAI = 64, //跨服服战祭剑台 所有人成功的祭剑总数
        GVAR_SERVERWAR_XIUWEI = 65, //服战名次带来的修为加成 20表明服战第一名

        GVAR_CHRISTMAS_PILESNOW_BEGIN = 66,    
        GVAR_CHRISTMAS_PILESNOW_END = 67,
       
        GVAR_OLDMAN_BEGIN = 68,   //捉迷藏
        GVAR_OLDMAN_END = 69,

        GVAR_YEARHAPPY_RANK_BEGIN = 70, //跨年大转盘
        GVAR_YEARHAPPY_RANK_END = 71, //跨年大转盘


        GVAR_3366_RECHARGE_BEGIN = 72,
        GVAR_3366_RECHARGE_END = 73,
        
        GVAR_MARRY_TIME1 = 74,//婚礼发放烟花
        GVAR_MARRY_TIME2 = 75,
        GVAR_MARRY_TIME3 = 76,
        GVAR_CREATMARRY_TIMES = 77,//婚礼当天创建
        
        GVAR_REPAIRMARRYBUG = 78,//婚礼bug修复
        GVAR_COUPLE_NUM = 79,//夫妻对数

        GVAR_QISHIBAN_INVITE_ENABLE = 80, // 七石斗法好友赠送开启（用于设置老玩家的MemCached的值）
        GVAR_3366_BUY_BEGIN = 81,
        GVAR_3366_BUY_END = 82,
        GVAR_SEVENSOUL_FIX = 83,

        GVAR_GUANKAACT_BEGIN = 84,  //关卡活动开始时间
        GVAR_GUANKAACT_END   = 85,  //关卡活动结束时间
        GVAR_DUOBAO_ENDTIME = 86,       // 夺宝每轮结束时间
        GVAR_CLAN_DUOBAO_BEGIN = 87,    // 夺宝每天开始时间
        GVAR_CLAN_DUOBAO_END = 88,      // 夺宝每天结束时间
                
        GVAR_TYSS_BEGIN= 89,        // 天元神兽开启时间
        GVAR_TYSS_END= 90,          // 天元神兽结束时间
        GVAR_REPAIRTYSSBUG = 91,    //天元神兽bug修复

        GVAR_KANGJITIANMO_BEGIN = 92,   // 抗击天魔开始时间
        GVAR_KANGJITIANMO_END = 93,     // 抗击天魔结束时间
        GVAR_MAX_TEAM_NUM = 94,         // 抗击天魔最大队伍数量

        /*GVAR_ANSWER_ENDTIME = 95,       // 答题每轮结束时间
        GVAR_ANSWER_AWARDTIME = 96,     // 答题每轮结算时间
        GVAR_ANSWER_PREPARE_DAY = 97,   // 答题每天准备时间
        GVAR_ANSWER_BEGIN_DAY = 98,     // 答题每天开始时间
        GVAR_ANSWER_END_DAY = 99,       // 答题每天结束时间*/
        GVAR_ANSWER_BEGIN = 98,         // 一战成名活动开启时间
        GVAR_ANSWER_END = 99,           // 一战成名活动结束时间
        GVAR_RAND_QUESTIONS_MARK = 100, // 选题标记
        
        GVAR_CLAN_BIGBOSS_ENABLE = 101,         //帮派BOSS能否预约
        GVAR_MAX_LEVEL = 102 ,          //玩家最大等级 , 用于开启等级天劫判断
        GVAR_11TIME_BEGIN = 103,  //天书奇缘开始时间
        GVAR_11TIME_END  = 104,  //天书奇缘结束时间
        
        GVAR_GG_BEGIN = 105,          //情义罗盘开启时间
        GVAR_GG_END = 106,           //情义罗盘关闭时间

        GVAR_MOSTER_PET_BEGIN = 107,          //妖王，珍兽收费减半
        GVAR_MOSTER_PET_END = 108,          

        GVAR_CLAN_LOCAL_RANK = 109,

        GVAR_MEMOIR_BEGIN = 110,     //三年纪念录开启时间
        GVAR_MEMOIR_END = 111,       //三年纪念录结束时间

        GVAR_MAX,
    };


#define GREGISTER_VAR(Var, Type) \
    m_VarTypes[Var] = Type;

    /**
     *@brief 变量系统
     */
    class GVarSystem : public Singleton<GVarSystem>
    {
    public:
        GVarSystem();
         ~GVarSystem();

    public:

        static void Init()
        {
            //注册变量
            GREGISTER_VAR(GVAR_TCPASS, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CITTASPLIT, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CITTASMERGE, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CLANCOPYPASS, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_1530BUCHANG, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_OLDRC7DAYBUCHANG, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ARENA_EXT_PROCESS, GCYCLE_DAY);
            GREGISTER_VAR(GVAR_TJ_TOWN_999_BUG, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_JOB_MO_PEXP, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_DISCOUNT_TYPE1, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DISCOUNT_TYPE2, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DISCOUNT_TYPE3, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_DISCOUNT_BEGIN1, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DISCOUNT_BEGIN2, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DISCOUNT_BEGIN3, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_DISCOUNT_END1, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DISCOUNT_END2, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DISCOUNT_END3, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_HEROMEM_CUT, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DRAGONKING_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DRAGONKING_END, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_EXP_HOOK_NEW, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_TREASURE_ACTION, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_TREASURE_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_TREASURE_END, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_HAS_CLAC_LB_BP, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_SURNAMELEGEND_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_SURNAMELEGEND_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_LUCKYSTAR_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_LUCKYSTAR_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_TOTALRECHARGEACT_S, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_TOTALRECHARGEACT_E, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_SPREAD_KEEPER1, GCYCLE_DAY);
            GREGISTER_VAR(GVAR_SPREAD_KEEPER2, GCYCLE_DAY);
            GREGISTER_VAR(GVAR_SPREAD_CONDITION, GCYCLE_DAY);

            GREGISTER_VAR(GVAR_RYHB_ACTIVITY_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_RYHB_ACTIVITY_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ZCJB_ACTIVITY_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ZCJB_ACTIVITY_END, GCYCLE_NONE);
            
            GREGISTER_VAR(GVAR_NewUser_Max, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_NewUser_Cur, GCYCLE_HOUR);

            GREGISTER_VAR(GVAR_LUCKYMEET_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_LUCKYMEET_END, GCYCLE_NONE);
            
            GREGISTER_VAR(GVAR_SUMMER_MEET_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_SUMMER_MEET_END, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_SUMMER_FLOW_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_SUMMER_FLOW_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONEQQGAME_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONEQQGAME_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QISHIBANGAME_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QISHIBANGAME_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONEQQGAMEY_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONEQQGAMEY_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONE_RECHARGE_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONE_RECHARGE_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_3366_RECHARGE_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_3366_RECHARGE_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_3366_BUY_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_3366_BUY_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_SEVENSOUL_FIX, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_DUOBAO_ENDTIME, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CLAN_DUOBAO_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CLAN_DUOBAO_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_KANGJITIANMO_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_KANGJITIANMO_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_MAX_TEAM_NUM, GCYCLE_NONE);
            /*GREGISTER_VAR(GVAR_ANSWER_ENDTIME, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ANSWER_AWARDTIME, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ANSWER_PREPARE_DAY, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ANSWER_BEGIN_DAY, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ANSWER_END_DAY, GCYCLE_NONE);*/
            GREGISTER_VAR(GVAR_ANSWER_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_ANSWER_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_RAND_QUESTIONS_MARK, GCYCLE_DAY);

            GREGISTER_VAR(GVAR_SERVERWAR_ISENTER, GCYCLE_WEEK);
            GREGISTER_VAR(GVAR_SERVERWAR_JIJIANTAI, GCYCLE_WEEK);
            GREGISTER_VAR(GVAR_SERVERWAR_XIUWEI, GCYCLE_WEEK);


            GREGISTER_VAR(GVAR_CHRISTMAS_PILESNOW_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CHRISTMAS_PILESNOW_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_YEARHAPPY_RANK_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_YEARHAPPY_RANK_END, GCYCLE_NONE);
            
            GREGISTER_VAR(GVAR_MARRY_TIME1, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_MARRY_TIME2, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_MARRY_TIME3, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CREATMARRY_TIMES, GCYCLE_DAY);
            GREGISTER_VAR(GVAR_REPAIRMARRYBUG, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_COUPLE_NUM, GCYCLE_NONE);

            GREGISTER_VAR(GVAR_QISHIBAN_INVITE_ENABLE, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_GUANKAACT_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_GUANKAACT_END, GCYCLE_NONE);
            
            GREGISTER_VAR(GVAR_TYSS_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_TYSS_END, GCYCLE_NONE);
            
            GREGISTER_VAR(GVAR_REPAIRTYSSBUG, GCYCLE_NONE);
            
            GREGISTER_VAR(GVAR_CLAN_BIGBOSS_ENABLE, GCYCLE_DAY);
            GREGISTER_VAR(GVAR_MAX_LEVEL, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_11TIME_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_11TIME_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_MOSTER_PET_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_MOSTER_PET_END, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_CLAN_LOCAL_RANK, GCYCLE_NONE);
        }

        UInt32 GetVar(UInt32 id, UInt32 now = 0);
        void SetVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void AddVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void LoadVar(UInt32 id, UInt32 data, UInt32 overTime);
        void SetOffset(UInt32 offset){ m_Offset = offset; }
        bool SetOverTime(UInt32 id, UInt32 overTime, bool force = false);
        UInt32 GetOverTime(UInt32 id);

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id, UInt32 now);
        void UpdateDB(UInt32 id);

    private:
        //玩家
        UInt32 m_Vars[GVAR_MAX];
        UInt32 m_OverTime[GVAR_MAX];
        UInt32 m_Offset;
        FastMutex _mutex;

        static UInt32 m_VarTypes[GVAR_MAX];
    };

#define GVAR GVarSystem::Instance()
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

