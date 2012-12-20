
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
        UInt32 m_Vars[GVAR_MAX];
        UInt32 m_OverTime[GVAR_MAX];
        UInt32 m_Offset;

        static UInt32 m_VarTypes[GVAR_MAX];
    };

#define GVAR GVarSystem::Instance()
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

