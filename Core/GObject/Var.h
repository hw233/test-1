#ifndef _VAR_H_
#define _VAR_H_

#include "Common/TimeUtil.h"

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
        VAR_INVALID = 0,    //??Ч??��
        VAR_SINGLE_CHOPSTICKS = 1, //???????Ϳ???
        VAR_SENGYI = 2,     // ɮ??
        VAR_APRON = 3,      // ?Ƕ?
        VAR_TGDT = 4,       // ?ж???????ʱ??
        VAR_TGDCNT = 5,     // ?ж???��??????3Сʱ????
        VAR_ATOFM = 6,     // ?Զ???ͼid
        VAR_TODAY_ONLINE = 9,     // time -- online today
        VAR_KEYPACK1 = 10,  // KEY????
        VAR_KEYPACK2,
        VAR_KEYPACK3,
        VAR_KEYPACK4,
        VAR_KEYPACK5,
        VAR_KEYPACK6,
        VAR_KEYPACK7,
        VAR_KEYPACK8,
       
        VAR_FAIL_ENCH = 20,  // fail to enchat equip
        VAR_SPLIT_EQUIP = 21 , //split equip times
        VAR_SPLIT_THRUMP = 22,//split thrump times
        VAR_BATTLE_MISS = 23, //???????
        VAR_BATTLE_CS  = 24,  //
        VAR_BATTLE_PR = 25,   //???????
        VAR_BATTLE_FJ = 26,  //???????
        VAR_BATTLE_SKILL_DMG = 27, //???????
        VAR_BATTLE_PEERLESS_DMG = 28,//????????
        
        VAR_COUNTRY_BATTLE_WIN =29,
        VAR_ATHLETICS_WIN = 30 ,

        vAR_TASK_SUBMITTED = 31,
        VAR_YAMEN_TASK_SUBMITTED = 32,
        VAR_SHIMEN_TASK_SUBMITTED = 33,
        VAR_CLAN_TASK_SUBMITTED  = 34,

        VAR_PEXP_GET = 35,
        VAR_PURPLE_EQUIP_NUM = 36,
        VAR_YELLOW_EQUIP_NUM = 37,
        VAR_YELLOW_THRUMP_NUM= 38,

        VAR_CLANBATTLE_HONOUR = 40, //帮派战今日贡献

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
            //注册变量
            REGISTER_VAR(VAR_SINGLE_CHOPSTICKS, CYCLE_YEAR)
            REGISTER_VAR(VAR_SENGYI, CYCLE_NONE);
            REGISTER_VAR(VAR_APRON, CYCLE_NONE);
            REGISTER_VAR(VAR_TGDT, CYCLE_MONTH);
            REGISTER_VAR(VAR_TGDCNT, CYCLE_MONTH);
            REGISTER_VAR(VAR_ATOFM, CYCLE_NONE);

            REGISTER_VAR(VAR_TODAY_ONLINE,  CYCLE_DAY);
            REGISTER_VAR(VAR_KEYPACK1, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK2, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK3, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK4, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK5, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK6, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK7, CYCLE_NONE);
            REGISTER_VAR(VAR_KEYPACK8, CYCLE_NONE);


            REGISTER_VAR(VAR_FAIL_ENCH, CYCLE_NONE);
            REGISTER_VAR(VAR_SPLIT_EQUIP, CYCLE_NONE);
            REGISTER_VAR(VAR_SPLIT_THRUMP, CYCLE_NONE);
            REGISTER_VAR( VAR_BATTLE_MISS, CYCLE_NONE);
            REGISTER_VAR( VAR_BATTLE_CS, CYCLE_NONE);
            REGISTER_VAR( VAR_BATTLE_PR, CYCLE_NONE);
            REGISTER_VAR( VAR_BATTLE_FJ, CYCLE_NONE);
            REGISTER_VAR( VAR_BATTLE_SKILL_DMG, CYCLE_NONE);
            REGISTER_VAR( VAR_BATTLE_PEERLESS_DMG, CYCLE_NONE);
            REGISTER_VAR( VAR_COUNTRY_BATTLE_WIN, CYCLE_NONE);
            REGISTER_VAR(  VAR_ATHLETICS_WIN,  CYCLE_NONE);
            REGISTER_VAR(  vAR_TASK_SUBMITTED,  CYCLE_NONE);
            REGISTER_VAR(  VAR_YAMEN_TASK_SUBMITTED,  CYCLE_NONE);
            REGISTER_VAR(  VAR_SHIMEN_TASK_SUBMITTED,  CYCLE_NONE);
            REGISTER_VAR(  VAR_CLAN_TASK_SUBMITTED,  CYCLE_NONE);
            REGISTER_VAR(  VAR_PEXP_GET,  CYCLE_NONE);
            REGISTER_VAR(  VAR_PURPLE_EQUIP_NUM,  CYCLE_NONE);
            REGISTER_VAR(  VAR_YELLOW_EQUIP_NUM,  CYCLE_NONE);
            REGISTER_VAR(  VAR_YELLOW_THRUMP_NUM,  CYCLE_NONE);

            REGISTER_VAR(VAR_CLANBATTLE_HONOUR, CYCLE_DAY);
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
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];
        UInt32 m_Offset;

        static UInt32 m_VarTypes[VAR_MAX];
    };
}




#endif
