
#ifndef _GVAR_H_
#define _GVAR_H_

#include "Common/TimeUtil.h"
#include "Common/Singleton.h"

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

        GVAR_QZONEQQGAME_BEGIN = 55,
        GVAR_QZONEQQGAME_END = 56,

        GVAR_QISHIBANGAME_BEGIN = 57,  //七石斗法开始时间
        GVAR_QISHIBANGAME_END = 58,    //七石斗法结束时间

        GVAR_QZONEQQGAMEY_BEGIN = 59,
        GVAR_QZONEQQGAMEY_END = 60,

        GVAR_QZONE_RECHARGE_BEGIN = 61,
        GVAR_QZONE_RECHARGE_END = 62,
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

            GREGISTER_VAR(GVAR_QZONE_RECHARGE_BEGIN, GCYCLE_NONE);
            GREGISTER_VAR(GVAR_QZONE_RECHARGE_END, GCYCLE_NONE);
        }

        UInt32 GetVar(UInt32 id, UInt32 now = 0);
        void SetVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void AddVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void LoadVar(UInt32 id, UInt32 data, UInt32 overTime);
        void SetOffset(UInt32 offset){ m_Offset = offset; }
        bool SetOverTime(UInt32 id, UInt32 overTime, bool force = false);

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id, UInt32 now);
        void UpdateDB(UInt32 id);

    private:
        //玩家
        UInt32 m_Vars[GVAR_MAX];
        UInt32 m_OverTime[GVAR_MAX];
        UInt32 m_Offset;

        static UInt32 m_VarTypes[GVAR_MAX];
    };

#define GVAR GVarSystem::Instance()
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

