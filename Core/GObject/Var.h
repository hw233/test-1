
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
        VAR_ENCHANT_CD = 6,         //强化cd
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
        VAR_EXPLOIT_TIME   = 21,  //上一次采集的时间
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
                REGISTER_VAR(VAR_EXPLOIT_TIME,CYCLE_NONE);
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

