#include"ClanBattleBase.h"

namespace GData
{

    ClanBattleBaseTable clanBattleBaseTable;

    void ClanBattleBaseTable::InsertBase(ClanBattleBase* base)
    {
        if( base == NULL )
        {
            return;
        }
        clanBatteBaseTable[base->GetBattleId()] = base;
    }

    ClanBattleBase* ClanBattleBaseTable::GetClanBattleBase(UInt32 exp)
    {
        for(UInt8 i = 1; i < BASE_MAX ; ++i )
        {
            if( exp <= clanBatteBaseTable[i]->GetExpLimit() )
            {
                return clanBatteBaseTable[i];
            }
        }
        return NULL;
    }

    ClanBattleBase* ClanBattleBaseTable::GetClanBattleBase(UInt8 battleId)
    {
        if( battleId < 1 || battleId >= BASE_MAX )
            return NULL;
        return clanBatteBaseTable[battleId];
    }
}
