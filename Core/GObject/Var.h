
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
        VAR_GOLD = 1,       //仙石
        VAR_COUPON = 2,     //礼券
        VAR_TAEL = 3,      //硬币
        VAR_TOTAL_RECHARGE = 4, //总充值
        VAR_LASTONLINE_TIME = 5, //上次登录的时间戳
        VAR_EQUIPMENT_1,    //装备1
        VAR_EQUIPMENT_2,    //装备2
        VAR_EQUIPMENT_3,    //装备3
        VAR_EQUIPMENT_4,    //装备4
        VAR_EQUIPMENT_5,    //装备5
        VAR_EQUIPMENT_6,    //装备6

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


#define REGISTER_VAR(Var, Type) \
    m_VarTypes[Var] = Type;


    /**
     *@brief 变量系统
     */
    class Player;
    class VarSystem
    {
        public:
            VarSystem(Player * pl);
            ~VarSystem();

        public:

            static void Init()
            {
                //注册变量
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
            Player * m_owner;
            UInt32 m_Vars[VAR_MAX];
            UInt32 m_OverTime[VAR_MAX];
            UInt32 m_Offset;
            FastMutex _mutex;

            static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

