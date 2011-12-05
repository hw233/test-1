#include "Var.h"
#include "Config.h"
#include "Server/WorldServer.h"

namespace GObject
{

    UInt32 VarSystem::m_VarTypes[VAR_MAX];


    VarSystem::VarSystem(UInt64 playerid)
    {
        m_PlayerID = playerid;
        memset(m_Vars, 0, sizeof(m_Vars));
        memset(m_OverTime, 0, sizeof(m_OverTime));
        m_Offset = 0;
    }

    VarSystem::~VarSystem()
    {
    }



    UInt32 VarSystem::GetType(UInt32 id) const
    {
        if(id >= VAR_MAX) return 0;
        return m_VarTypes[id];
    }


    UInt32 VarSystem::GetVar(UInt32 id, UInt32 now)
    {
        if(id >= VAR_MAX || m_Vars[id] == 0) return 0;

        if(CheckReset(id , now)) UpdateDB(id);
        
        return m_Vars[id];
    }

    void VarSystem::SetVar(UInt32 id, UInt32 data, UInt32 now)
    {
        if(id >= VAR_MAX) return;
#ifdef NO_ATTAINMENT
        if (id >=  VAR_FAIL_ENCH &&  id <= VAR_YELLOW_THRUMP_NUM)
            return;
#endif
        UInt32 oldVal = m_Vars[id];
       // m_Vars[id] = data;
        bool bUpdateDB = CheckReset(id , now);
        m_Vars[id] = data;
        bUpdateDB = bUpdateDB || (oldVal != m_Vars[id]);
        if(bUpdateDB) UpdateDB(id);
    }

    void VarSystem::AddVar(UInt32 id, UInt32 data, UInt32 now)
    {
        if(id >= VAR_MAX || data == 0) return;

#ifdef NO_ATTAINMENT 
        if (id >=  VAR_FAIL_ENCH &&  id <= VAR_YELLOW_THRUMP_NUM)
            return;
#endif
        CheckReset(id , now);
        m_Vars[id] += data;
        UpdateDB(id);
    }

    void VarSystem::LoadVar(UInt32 id, UInt32 data, UInt32 overTime)
    {
        if(id >= VAR_MAX) return;

        m_Vars[id] = data;
        m_OverTime[id] = overTime;
    }

    bool VarSystem::CheckReset(UInt32 id, UInt32  now)
    {
        if(now == 0 )
             now = TimeUtil::Now() + m_Offset;
        else
            now += m_Offset;

        if(now < m_OverTime[id]) return false;
        
        UInt32 oldtime = m_OverTime[id];

        switch(m_VarTypes[id])
        {
            case CYCLE_NONE:
                {
                    m_OverTime[id] = 0xFFFFFFFF;
                }
                break;
            case CYCLE_MIN:
                {
                    //下个分钟
                    m_OverTime[id] = TimeUtil::SharpMinute(1, now);
                }
                break;
            case CYCLE_HOUR:
                {
                    //下个小时
                    m_OverTime[id] = TimeUtil::SharpHour(1, now);
                }
                break;
            case CYCLE_DAY:
                {
                    //下一自然天
                    m_OverTime[id] = TimeUtil::SharpDayT(1, now);
                }
                break;
            case CYCLE_WEEK:
                {
                    //下一个自然周
                    m_OverTime[id] = TimeUtil::SharpWeek(1, now);
                }
                break;
            case CYCLE_MONTH:
                {
                    //下一个自然月
                    m_OverTime[id] = TimeUtil::SharpMonth(1, now);
                }
                break;
            case CYCLE_YEAR:
                {
                    //下一个自然年
                    m_OverTime[id] = TimeUtil::SharpYear(1, now);
                }
                break;
            default:
                break;
        }

        UInt32 oldVar = m_Vars[id];
        m_Vars[id] = 0;

        //返回是否发生变化
        return oldtime != m_OverTime[id] || oldVar != 0;
    }

    void VarSystem::UpdateDB(UInt32 id)
    {
       DB7().PushUpdateData("REPLACE INTO `var` (`playerId`, `id`, `data`, `over`) VALUES (%"I64_FMT"u, %u, %u, %u)"
               ,m_PlayerID, id, m_Vars[id], m_OverTime[id]);
    }
}


