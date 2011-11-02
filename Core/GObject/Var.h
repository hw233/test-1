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
        VAR_INVALID = 0,    //��Ч����
        
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
        }
        
        UInt32 GetVar(UInt32 id);
        void SetVar(UInt32 id, UInt32 data);
        void AddVar(UInt32 id, UInt32 data);

    private:
        UInt32 GetType(UInt32 id) const;
        bool CheckReset(UInt32 id);
        void UpdateDB(UInt32 id);

    private:
        //���id
        UInt64 m_PlayerID;
        UInt32 m_Vars[VAR_MAX];
        UInt32 m_OverTime[VAR_MAX];

        static UInt32 m_VarTypes[VAR_MAX];
    };
}

#endif


