#include "GVar.h"
#include "Config.h"
#include "Server/WorldServer.h"

namespace GObject
{

    UInt32 GVarSystem::m_VarTypes[GVAR_MAX];


    GVarSystem::GVarSystem()
    {
        memset(m_Vars, 0, sizeof(m_Vars));
        memset(m_OverTime, 0, sizeof(m_OverTime));
        m_Offset = 0;
    }

    GVarSystem::~GVarSystem()
    {
    }



    UInt32 GVarSystem::GetType(UInt32 id) const
    {
        if(id >= GVAR_MAX) return 0;
        return m_VarTypes[id];
    }


    UInt32 GVarSystem::GetVar(UInt32 id, UInt32 now)
    {
        if(id >= GVAR_MAX || m_Vars[id] == 0) return 0;

        if(CheckReset(id , now)) UpdateDB(id);

        return m_Vars[id];
    }

    void GVarSystem::SetVar(UInt32 id, UInt32 data, UInt32 now)
    {
        if(id >= GVAR_MAX) return;
        UInt32 oldVal = m_Vars[id];
        bool bUpdateDB = CheckReset(id , now);
        m_Vars[id] = data;
        bUpdateDB = bUpdateDB || (oldVal != m_Vars[id]);
        if(bUpdateDB) UpdateDB(id);
    }

    void GVarSystem::AddVar(UInt32 id, UInt32 data, UInt32 now)
    {
        if(id >= GVAR_MAX || data == 0) return;
        CheckReset(id , now);
        m_Vars[id] += data;
        UpdateDB(id);
    }

    void GVarSystem::LoadVar(UInt32 id, UInt32 data, UInt32 overTime)
    {
        if(id >= GVAR_MAX) return;

#if 0
        if (TimeUtil::Now() > overTime)
        {
            DB7().PushUpdateData("DELETE FROM `var` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", m_PlayerID, id);
            return;
        }
#endif

        m_Vars[id] = data;
        m_OverTime[id] = overTime;
    }

    bool GVarSystem::CheckReset(UInt32 id, UInt32  now)
    {
        if(now == 0 )
             now = TimeUtil::Now() + m_Offset;
        else
            now += m_Offset;

        if(now < m_OverTime[id]) return false;

        UInt32 oldtime = m_OverTime[id];

        switch(m_VarTypes[id])
        {
            case GCYCLE_NONE:
                {
                    m_OverTime[id] = 0xFFFFFFFF;
                }
                break;
            case GCYCLE_MIN:
                {
                    //分
                    m_OverTime[id] = TimeUtil::SharpMinute(1, now);
                }
                break;
            case GCYCLE_HOUR:
                {
                    //时
                    m_OverTime[id] = TimeUtil::SharpHour(1, now);
                }
                break;
            case GCYCLE_DAY:
                {
                    //天
                    m_OverTime[id] = TimeUtil::SharpDayT(1, now);
                }
                break;
            case GCYCLE_WEEK:
                {
                    //周
                    m_OverTime[id] = TimeUtil::SharpWeek(1, now);
                }
                break;
            case GCYCLE_MONTH:
                {
                    //月
                    m_OverTime[id] = TimeUtil::SharpMonth(1, now);
                }
                break;
            case GCYCLE_YEAR:
                {
                    //年
                    m_OverTime[id] = TimeUtil::SharpYear(1, now);
                }
                break;
            default:
                break;
        }

        UInt32 oldVar = m_Vars[id];
        m_Vars[id] = 0;

        return oldtime != m_OverTime[id] || oldVar != 0;
    }

    void GVarSystem::UpdateDB(UInt32 id)
    {
       DB7().PushUpdateData("REPLACE INTO `gvar` (`id`, `data`, `over`) VALUES (%u, %u, %u)",
               id, m_Vars[id], m_OverTime[id]);
    }
}

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

