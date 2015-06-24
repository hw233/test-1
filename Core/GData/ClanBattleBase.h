#pragma once
#ifndef __CLANBATTLEBASE_H__
#define __CLANBATTLEBASE_H__

#include"Config.h"
#define BASE_MAX 100
namespace GData
{
    class ClanBattleBase
    {
        public:
            ClanBattleBase(UInt8 id,UInt32 l,UInt8 n,UInt8 min,UInt8 max) : battleId(id),explimit(l),forcenum(n),playermin(min),playermax(max) {}
            UInt8 GetBattleId() const { return battleId;}
            UInt32 GetExpLimit() const { return explimit;}
            UInt8 GetForceNum() const { return forcenum;}
            UInt8 GetPlayerMin() const { return playermin;}
            UInt8 GetPlayerMax() const { return playermax;}
        private:
            UInt8 battleId;
            UInt32 explimit;
            UInt8 forcenum;
            UInt8 playermin;
            UInt8 playermax;
    };

    class ClanBattleBaseTable
    {
        public:
            void InsertBase(ClanBattleBase* base);
            ClanBattleBase* GetClanBattleBase(UInt32 exp);
            ClanBattleBase* GetClanBattleBase(UInt8 battleId);
            
        private:
            ClanBattleBase* clanBatteBaseTable[BASE_MAX];
    };

    extern ClanBattleBaseTable clanBattleBaseTable;
}

#endif
