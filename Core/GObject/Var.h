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


    enum VarID
    {
        VAR_INVALID = 0,    //无效变量
        VAR_SINGLE_CHOPSTICKS = 1, //光棍节送筷子
        VAR_SENGYI = 2,     // 僧衣
        VAR_APRON = 3,      // 肚兜
        VAR_TGDT = 4,       // 感恩节在线时间
        VAR_TGDCNT = 5,     // 感恩节连续在线3小时次数

        VAR_KEYPACK1 = 10,  // KEY礼包
        VAR_KEYPACK2,
        VAR_KEYPACK3,
        VAR_KEYPACK4,
        VAR_KEYPACK5,
        VAR_KEYPACK6,
        VAR_KEYPACK7,
        VAR_KEYPACK8,
       
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
            REGISTER_VAR(VAR_SINGLE_CHOPSTICKS, CYCLE_YEAR)
            REGISTER_VAR(VAR_SENGYI, CYCLE_NONE);
            REGISTER_VAR(VAR_APRON, CYCLE_NONE);
            REGISTER_VAR(VAR_TGDT, CYCLE_MONTH);
            REGISTER_VAR(VAR_TGDCNT, CYCLE_MONTH);
            REGISTER_VAR(VAR_KEYPACK1, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK2, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK3, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK4, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK5, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK6, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK7, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK8, CYCLE_NONE);
        }
        
        UInt32 GetVar(UInt32 id);
        void SetVar(UInt32 id, UInt32 data);
        void AddVar(UInt32 id, UInt32 data);
        void LoadVar(UInt32 id, UInt32 data, UInt32 overTime);
        void SetOffset(UInt32 offset){ m_Offset = offset; }

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id);
        void UpdateDB(UInt32 id);

    private:
        //玩家id
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];
        UInt32 m_Offset;

        static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif


