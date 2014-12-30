#include "FVar.h"
#include "Config.h"
#include "Server/WorldServer.h"

namespace GObject
{

    UInt32 FVarSystem::m_FVarTypes[FVAR_MAX];


    FVarSystem::FVarSystem(IDTYPE pid , UInt32 fid):playerId(pid),fighterId(fid)
    {
        memset(m_FVars, 0, sizeof(m_FVars));
        memset(m_OverTime, 0, sizeof(m_OverTime));
        m_Offset = 0;
    }

    FVarSystem::~FVarSystem()
    {
    }



    UInt32 FVarSystem::GetType(UInt32 id) const
    {
        if(id >= FVAR_MAX) return 0;
        return m_FVarTypes[id];
    }


    UInt32 FVarSystem::GetFVar(UInt32 id, UInt32 now)
    {
        if(id >= FVAR_MAX || m_FVars[id] == 0) return 0;

        if(CheckReset(id , now)) DelFVar(id);

        return m_FVars[id];
    }

    void FVarSystem::SetFVar(UInt32 id, UInt32 data, UInt32 now)
    {
        if(id >= FVAR_MAX) return;
        if(data == 0)
        {
            DelFVar(id);
            return ;
        }
        UInt32 oldVal = m_FVars[id];
        // m_FVars[id] = data;
        bool bUpdateDB = CheckReset(id , now);
        m_FVars[id] = data;
        bUpdateDB = bUpdateDB || (oldVal != m_FVars[id]);
        if(bUpdateDB)
        {
            if(oldVal == 0)
                ReplaceDB(id);
            else
                UpdateDB(id);
        }
    }
    void FVarSystem::DelFVar(UInt32 id)
    {
        if(id >= FVAR_MAX) return;
        UInt32 oldVal = m_FVars[id];
        if(oldVal != 0 )
            m_FVars[id] = 0;
        DB7().PushUpdateData("delete from fvar where `playerId` = %u and fighterId = %u and `id` = %u ",playerId,fighterId, id);

    }

    void FVarSystem::AddFVar(UInt32 id, UInt32 data, UInt32 now)
    {
        if(id >= FVAR_MAX || data == 0) return;

        CheckReset(id , now);
        m_FVars[id] += data;
        UpdateDB(id);
    }

    void FVarSystem::LoadFVar(UInt32 id, UInt32 data, UInt32 overTime)
    {
        if(id >= FVAR_MAX) return;

#if 0
        if (TimeUtil::Now() > overTime)
        {
            DB7().PushUpdateData("DELETE FROM `var` WHERE `playerId` = %" I64_FMT "u AND `id` = %u", m_PlayerID, id);
            return;
        }
#endif

        m_FVars[id] = data;
        m_OverTime[id] = overTime;
    }

    bool FVarSystem::CheckReset(UInt32 id, UInt32  now)
    {
        if(now == 0 )
             now = TimeUtil::Now() + m_Offset;
        else
            now += m_Offset;

        FastMutex::ScopedLock lk(_mutex);
        if(now < m_OverTime[id]) return false;

        UInt32 oldtime = m_OverTime[id];

        switch(m_FVarTypes[id])
        {
            case FCYCLE_NONE:
                {
                    m_OverTime[id] = 0xFFFFFFFF;
                }
                break;
            case FCYCLE_MIN:
                {
                    //分
                    m_OverTime[id] = TimeUtil::SharpMinute(1, now);
                }
                break;
            case FCYCLE_HOUR:
                {
                    //时
                    m_OverTime[id] = TimeUtil::SharpHour(1, now);
                }
                break;
            case FCYCLE_DAY:
                {
                    //天
                    m_OverTime[id] = TimeUtil::SharpDayT(1, now);
                }
                break;
            case FCYCLE_WEEK:
                {
                    //周
                    m_OverTime[id] = TimeUtil::SharpWeek(1, now);
                }
                break;
            case FCYCLE_MONTH:
                {
                    //月
                    m_OverTime[id] = TimeUtil::SharpMonth(1, now);
                }
                break;
            case FCYCLE_YEAR:
                {
                    //年
                    m_OverTime[id] = TimeUtil::SharpYear(1, now);
                }
                break;
            default:
                break;
        }

        UInt32 oldFVar = m_FVars[id];
        m_FVars[id] = 0;

        return oldtime != m_OverTime[id] || oldFVar != 0;
    }

    void FVarSystem::ReplaceDB(UInt32 id)
    {
        DB7().PushUpdateData("REPLACE INTO `fvar` (`playerId`,`fighterId` ,`id`, `data`, `over`) VALUES (%u,%u, %u, %u, %u)" ,playerId,fighterId ,id, m_FVars[id], m_OverTime[id]);
    }

    void FVarSystem::UpdateDB(UInt32 id)
    {
        DB7().PushUpdateData("update `fvar` set data = %u where playerId = %u and fighterId = %u and id = %u" , m_FVars[id],playerId,fighterId ,id);
        DB7().PushUpdateData("update `fvar` set over = %u where playerId = %u and fighterId = %u and id = %u" , m_OverTime[id],playerId,fighterId ,id);
    }
}


