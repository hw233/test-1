
#ifndef _FVAR_H_
#define _FVAR_H_

#include "Common/TimeUtil.h"
#include "Common/Mutex.h"

namespace GObject
{
    //变量周期类型
    enum FCycleType
    {
        FCYCLE_NONE =    0,  //无周期
        FCYCLE_MIN =     1,  //分
        FCYCLE_HOUR =    2,  //时
        FCYCLE_DAY =     3,  //日
        FCYCLE_WEEK =    4,  //周
        FCYCLE_MONTH =   5,  //月
        FCYCLE_YEAR =    6,  //年

        FCYCLE_MAX,
    };


    enum FVarID
    {
        FVAR_INVALID = 0,    //无效变量
        FVAR_WEAPON_ENCHANT = 1,    //武器强化值
        FVAR_ARMOR1_ENCHANT = 2,    //盾牌强化值
        FVAR_ARMOR2_ENCHANT = 3,    //帽子强化值
        FVAR_ARMOR3_ENCHANT = 4,    //胸甲强化值
        FVAR_ARMOR4_ENCHANT = 5,    //鞋子强化值
        FVAR_ARMOR5_ENCHANT = 6,    //项链强化值

        FVAR_WEAPON_ROAD = 7,    //武器强化之路
        FVAR_ARMOR1_ROAD = 8,    //胸甲强化之路
        FVAR_ARMOR2_ROAD = 9,    //护头强化之路
        FVAR_ARMOR3_ROAD = 10,   //绑腿强化之路
        FVAR_ARMOR4_ROAD = 11,   //戒指强化之路
        FVAR_ARMOR5_ROAD = 12,   //项链强化之路

        FVAR_SKILL0_LEVEL = 13,    //技能等级 
        FVAR_SKILL1_LEVEL = 14,    //技能等级
        FVAR_SKILL2_LEVEL = 15,    //技能等级
        FVAR_SKILL3_LEVEL = 16,    
        FVAR_SKILL4_LEVEL = 17,    //
        FVAR_SKILL5_LEVEL = 18,    //

        FVAR_HP           = 19,    //最大血量
        FVAR_ATTACK       = 20,    //物理攻击力
        FVAR_DEFEND       = 21,    //物理防御力
        FVAR_MGCATK       = 22,    //魔法攻击力
        FVAR_MGCDEF       = 23,    //魔法防御力
        FVAR_CRITICAL     = 24,    //暴击
        FVAR_EVADE        = 25,    //闪避
        FVAR_AFFAIRS_STATUS = 26,  //内政的状态  0 空闲  1 治理 2 开垦 3 
        FVAR_QUALITY = 27,

        FVAR_MAX
    };



#define REGISTER_FVAR(FVar, Type) \
    m_FVarTypes[FVar] = Type;

#define IDTYPE UInt64

    /**
     *@brief 变量系统
     */
    //class Fighter; 
    class FVarSystem
    {
    public:
        FVarSystem(IDTYPE playerId,UInt32 fighterId);
         ~FVarSystem();

    public:

        static void Init()
        {
            REGISTER_FVAR(FVAR_INVALID, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_WEAPON_ENCHANT, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_ARMOR1_ENCHANT, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_ARMOR2_ENCHANT, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_ARMOR3_ENCHANT, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_ARMOR4_ENCHANT, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_ARMOR5_ENCHANT, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_SKILL0_LEVEL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_SKILL1_LEVEL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_SKILL2_LEVEL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_SKILL3_LEVEL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_SKILL4_LEVEL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_SKILL5_LEVEL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_HP, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_ATTACK, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_DEFEND, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_MGCATK, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_MGCDEF, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_CRITICAL, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_EVADE, FCYCLE_NONE);
            REGISTER_FVAR(FVAR_AFFAIRS_STATUS, FCYCLE_NONE);
        }

        UInt32 GetFVar(UInt32 id, UInt32 now = 0);
        void SetFVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void DelFVar(UInt32 id);
        void AddFVar(UInt32 id, UInt32 data, UInt32 now = 0);
        void LoadFVar(UInt32 id, UInt32 data, UInt32 overTime);
        void SetOffset(UInt32 offset){ m_Offset = offset; }

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id, UInt32 now);
        void UpdateDB(UInt32 id);
        void ReplaceDB(UInt32 id);

    private:
        //玩家
        IDTYPE playerId;
        UInt32 fighterId;
        UInt32 m_FVars[FVAR_MAX];
        UInt32 m_OverTime[FVAR_MAX];
        UInt32 m_Offset;
        FastMutex _mutex;

        static UInt32 m_FVarTypes[FVAR_MAX];
    };
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

