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


    enum VarID
    {
        VAR_INVALID = 0,    //��Ч����
        VAR_SINGLE_CHOPSTICKS = 1, //������Ϳ���
        VAR_SENGYI = 2,     // ɮ��
        VAR_APRON = 3,      // �Ƕ�
        VAR_TGDT = 4,       // �ж�������ʱ��
        VAR_TGDCNT = 5,     // �ж�����������3Сʱ����

        VAR_KEYPACK1 = 10,  // KEY���
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
        //���id
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];
        UInt32 m_Offset;

        static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif


