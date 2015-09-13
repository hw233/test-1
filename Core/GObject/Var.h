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

        CYCLE_DAY_4 =   7 ,  //日
        

        CYCLE_MAX,
    };


    enum VarID
    {
        VAR_INVALID = 0,            //无效变量
        VAR_TEAL    = 1,            //铜钱
        VAR_COUPON  = 2,            //银两
        VAR_GOLD    = 3,            //金子
        VAR_EXP    = 4,             //经验
        VAR_TOTAL_GOLD = 5,         //总充值量
        VAR_ENCHANT_CD = 6,         //装备强化cd
        VAR_ENCHANT_CD2 = 6,         //士兵强化cd
        VAR_CLAN_CONT = 7,          //帮派贡献
        VAR_CLAN_ENTER = 8,         //进入帮派的时间
        VAR_PACKAGE_SIZE = 9,
        VAR_OFF_LINE = 10 ,         //最近一次在线时间
        VAR_GOVERN_SPEEDUP_CNT = 11, //治理加速次数
        VAR_BATTLE_POINT = 12, //战斗力
        VAR_GOVERN_FIGHTERID = 13, //治理的将领id
        VAR_SEARCH_FIGHTER = 14 ,  //探索次数
        VAR_CLANBATTLE_COMMENT_TIME = 15,//军团战留言时间
        VAR_SIGN_MONTH = 16,  //按月签到
        VAR_EXPLOIT_WOOD = 17, //采集 木材
        VAR_EXPLOIT_LEATHER = 18, //皮革
        VAR_EXPLOIT_IRON = 19,    //铁
        VAR_EXPLOIT_COPPER = 20,  //铜
        VAR_SIGN_MONTH_ADD = 21,  //按月签到
        VAR_DAY_CHANGE = 22,  //按月签到
        VAR_EXPLOIT_TIME   = 23,  //上一次采集的时间
        VAR_ARENA_POS = 24, //竞技场排名
        VAR_ARENA_RAND = 25, //竞技场随机选择
        VAR_ARENA_TIME = 26, //竞技场时间
        VAR_ARENA_COUNT = 27, //竞技场次数
        VAR_ARENA_MONEY = 27, //竞技场货币

        //武装
        VAR_0_WALK_ENCHANT = 30,    //步兵强化值
        VAR_0_RIDE_ENCHANT = 31,    //骑兵强化值
        VAR_0_SHOOT_ENCHANT = 32,    //弓兵强化值
        VAR_0_ADVICE_ENCHANT = 33,    //谋士强化值
        VAR_0_LANCE_ENCHANT = 34,    //枪兵强化值
        VAR_0_WALK_ROAD = 35,    //步兵强化之路
        VAR_0_RIDE_ROAD = 36,    //骑兵强化之路
        VAR_0_SHOOt_ROAD = 37,    //弓兵强化之路
        VAR_0_ADVICE_ROAD = 38,   //谋士强化之路
        VAR_0_LANCE_ROAD = 39,   //枪兵强化之路

        //士气
        VAR_1_WALK_ENCHANT   =  40,    //步兵强化值
        VAR_1_RIDE_ENCHANT   =  41,    //骑兵强化值
        VAR_1_SHOOT_ENCHANT  =  42,    //弓兵强化值
        VAR_1_ADVICE_ENCHANT =  43,    //谋士强化值
        VAR_1_LANCE_ENCHANT  =  44,    //枪兵强化值
        VAR_1_WALK_ROAD      =  45,    //步兵强化之路
        VAR_1_RIDE_ROAD      =  46,    //骑兵强化之路
        VAR_1_SHOOt_ROAD     =  47,    //弓兵强化之路
        VAR_1_ADVICE_ROAD    =  48,   //谋士强化之路
        VAR_1_LANCE_ROAD     =  49,   //枪兵强化之路

        //体能
        VAR_2_WALK_ENCHANT   =  50,    //步兵强化值
        VAR_2_RIDE_ENCHANT   =  51,    //骑兵强化值
        VAR_2_SHOOT_ENCHANT  =  52,    //弓兵强化值
        VAR_2_ADVICE_ENCHANT =  53,    //谋士强化值
        VAR_2_LANCE_ENCHANT  =  54,    //枪兵强化值
        VAR_2_WALK_ROAD      =  55,    //步兵强化之路
        VAR_2_RIDE_ROAD      =  56,    //骑兵强化之路
        VAR_2_SHOOt_ROAD     =  57,    //弓兵强化之路
        VAR_2_ADVICE_ROAD    =  58,   //谋士强化之路
        VAR_2_LANCE_ROAD     =  59,   //枪兵强化之路

        //身手
        VAR_3_WALK_ENCHANT   =  60,    //步兵强化值
        VAR_3_RIDE_ENCHANT   =  61,    //骑兵强化值
        VAR_3_SHOOT_ENCHANT  =  62,    //弓兵强化值
        VAR_3_ADVICE_ENCHANT =  63,    //谋士强化值
        VAR_3_LANCE_ENCHANT  =  64,    //枪兵强化值
        VAR_3_WALK_ROAD      =  65,    //步兵强化之路
        VAR_3_RIDE_ROAD      =  66,    //骑兵强化之路
        VAR_3_SHOOt_ROAD     =  67,    //弓兵强化之路
        VAR_3_ADVICE_ROAD    =  68,   //谋士强化之路
        VAR_3_LANCE_ROAD     =  69,   //枪兵强化之路

        //纪律
        VAR_4_WALK_ENCHANT   =  70,    //步兵强化值
        VAR_4_RIDE_ENCHANT   =  71,    //骑兵强化值
        VAR_4_SHOOT_ENCHANT  =  72,    //弓兵强化值
        VAR_4_ADVICE_ENCHANT =  73,    //谋士强化值
        VAR_4_LANCE_ENCHANT  =  74,    //枪兵强化值
        VAR_4_WALK_ROAD      =  75,    //步兵强化之路
        VAR_4_RIDE_ROAD      =  76,    //骑兵强化之路
        VAR_4_SHOOt_ROAD     =  77,    //弓兵强化之路
        VAR_4_ADVICE_ROAD    =  78,   //谋士强化之路
        VAR_4_LANCE_ROAD     =  79,   //枪兵强化之路

        //操练
        VAR_5_WALK_ENCHANT   =  80,    //步兵强化值
        VAR_5_RIDE_ENCHANT   =  81,    //骑兵强化值
        VAR_5_SHOOT_ENCHANT  =  82,    //弓兵强化值
        VAR_5_ADVICE_ENCHANT =  83,    //谋士强化值
        VAR_5_LANCE_ENCHANT  =  84,    //枪兵强化值
        VAR_5_WALK_ROAD      =  85,    //步兵强化之路
        VAR_5_RIDE_ROAD      =  86,    //骑兵强化之路
        VAR_5_SHOOt_ROAD     =  87,    //弓兵强化之路
        VAR_5_ADVICE_ROAD    =  88,   //谋士强化之路
        VAR_5_LANCE_ROAD     =  89,   //枪兵强化之路

        /*公会战战场放将的数量*/
        VAR_CLANBATTLE_FIGHTERNUM = 90, 
        VAR_MAX_CONSTANTLYKILL  = 91,   //公会战最大连杀数
        VAR_MAX_ENDCONSTANTLYKILL = 92, //公会战最大终结连杀数量
        VAR_TOTAL_RECHARGE        = 93, //总充值数量

        VAR_MAX
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


#ifndef REGISTER_VAR
#define REGISTER_VAR(Var, Type) \
    m_VarTypes[Var] = Type;
#endif


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
                REGISTER_VAR(VAR_TEAL , CYCLE_NONE);
                REGISTER_VAR(VAR_COUPON , CYCLE_NONE);
                REGISTER_VAR(VAR_GOLD , CYCLE_NONE);
                REGISTER_VAR(VAR_OFF_LINE,CYCLE_NONE);
                REGISTER_VAR(VAR_GOVERN_SPEEDUP_CNT,CYCLE_DAY);
                REGISTER_VAR(VAR_GOVERN_FIGHTERID,CYCLE_NONE);
                REGISTER_VAR(VAR_SEARCH_FIGHTER,CYCLE_DAY_4);
                REGISTER_VAR(VAR_CLANBATTLE_COMMENT_TIME,CYCLE_NONE);
                REGISTER_VAR(VAR_SIGN_MONTH,CYCLE_MONTH);
                REGISTER_VAR(VAR_EXPLOIT_WOOD,CYCLE_NONE);
                REGISTER_VAR(VAR_EXPLOIT_LEATHER,CYCLE_NONE);
                REGISTER_VAR(VAR_EXPLOIT_IRON,CYCLE_NONE);
                REGISTER_VAR(VAR_EXPLOIT_COPPER,CYCLE_NONE);
                REGISTER_VAR(VAR_SIGN_MONTH_ADD,CYCLE_MONTH);
                REGISTER_VAR(VAR_DAY_CHANGE,CYCLE_DAY);
                REGISTER_VAR(VAR_EXPLOIT_TIME,CYCLE_NONE);
                REGISTER_VAR(VAR_CLANBATTLE_FIGHTERNUM,CYCLE_WEEK);
                REGISTER_VAR(VAR_ARENA_POS,CYCLE_NONE);
                REGISTER_VAR(VAR_ARENA_RAND,CYCLE_NONE);
                REGISTER_VAR(VAR_ARENA_MONEY,CYCLE_NONE);
                REGISTER_VAR(VAR_TOTAL_RECHARGE,CYCLE_NONE);
            }

            UInt32 GetVar(UInt32 id, UInt32 now = 0);
            void SetVar(UInt32 id, UInt32 data, UInt8 flag = 0, UInt32 now = 0);
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

