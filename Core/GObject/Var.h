#ifndef _VAR_H_
#define _VAR_H_

#include "Common/TimeUtil.h"

namespace GObject
{
    //��������
    enum CycleType
    {
        CYCLE_NONE =    0,  //�����ڣ���������
        CYCLE_MIN =     1,  //�Է�Ϊ����
        CYCLE_HOUR =    2,  //��СʱΪ����
        CYCLE_DAY =     3,   //����Ϊ����
        CYCLE_WEEK =    4,  //����Ȼ��Ϊ���ڣ���һ��ʼ������Ϊһ��
        CYCLE_MONTH =   5, //����Ȼ��Ϊ����
        CYCLE_YEAR =    6,  //����Ϊ����
        
        CYCLE_MAX,
    };


    enum VarID{
        VAR_INVALID = 0,    // ��Ч����
        VAR_SENGYI = 2,     // ɮ��
        VAR_APRON = 3,      // �Ƕ�
        VAR_FAIL_ENCH = 4,  // fail to enchat equip
        VAR_SPLIT_EQUIP = 5 , //split equip times
        VAR_SPLIT_THRUMP = 6,//split thrump times
        VAR_BATTLE_MISS = 7, //�������
        VAR_BATTLE_CS  = 8,  //
        VAR_BATTLE_PR = 9,   //�������
        VAR_BATTLE_FJ = 10,  //�������
        VAR_BATTLE_SKILL_DMG = 11, //�������
        VAR_BATTLE_PEERLESS_DMG = 12,//��������
        
        VAR_COUNTRY_BATTLE_WIN =13,
        VAR_ATHLETICS_WIN = 14,

        vAR_TASK_SUBMITTED = 15,
        VAR_YAMEN_TASK_SUBMITTED = 16,
        VAR_SHIMEN_TASK_SUBMITTED = 17,
        VAR_CLAN_TASK_SUBMITTED  = 18,

        VAR_PEXP_GET = 19,
        VAR_PURPLE_EQUIP_NUM = 20,
        VAR_YELLOW_EQUIP_NUM = 21,
        VAR_YELLOW_THRUMP_NUM=22,
        VAR_MAX,
    };


#define REGISTER_VAR(Var, Type) \
    m_VarTypes[Var] = Type;


    /** 
     *@brief ����ϵͳ
     */
    class VarSystem
    {
    public:
        VarSystem(UInt64 playerid);
        ~VarSystem();

    public:

        static void Init()
        {
            //�ڴ�ʹ��REGISTER_VARע�����
            REGISTER_VAR(VAR_SENGYI, CYCLE_NONE);
            REGISTER_VAR(VAR_APRON, CYCLE_NONE);
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
        //���id
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];
        UInt32 m_Offset;

        static UInt32 m_VarTypes[VAR_MAX];
    };
}




#endif
