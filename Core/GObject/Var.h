#ifndef _VAR_H_
#define _VAR_H_

#include "Common/TimeUtil.h"

namespace GObject
{
    //周期类型
    enum CycleType
    {
        CYCLE_NONE =    0,  //无周期，即持续性
        CYCLE_MIN =     1,  //以分为周期
        CYCLE_HOUR =    2,  //以小时为周期
        CYCLE_DAY =     3,   //以天为周期
        CYCLE_WEEK =    4,  //以自然周为周期，周一开始到周日为一周
        CYCLE_MONTH =   5, //以自然月为周期
        CYCLE_YEAR =    6,  //以年为周期
        
        CYCLE_MAX,
    };


    enum VarID{
        VAR_INVALID = 0,    //无效变量
        
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
            //在此使用REGISTER_VAR注册变量
        }
        
        UInt32 GetVar(UInt32 id);
        void SetVar(UInt32 id, UInt32 data);
        void AddVar(UInt32 id, UInt32 data);

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id);
        void UpdateDB(UInt32 id);

    private:
        //玩家id
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];

        static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif


