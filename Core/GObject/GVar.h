
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
        GVAR_INVALID = 0,    //??Ч??��
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
            //GREGISTER_VAR(VAR_2NDSOUL, CYCLE_NONE);
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
}

#endif

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

