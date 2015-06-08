#ifndef __BATTLEAWARD_H
#define __BATTLEAWARD_H


#include "Config.h"
//战役奖励信息

#define AWARD_MAX 255
namespace GData
{
    struct ItemInfo
    {
        ItemInfo(UInt32 i, UInt32 n) : itemId(i), itemNum(n){}
        UInt32 itemId ;
        UInt32 itemNum ;
    };


    class  BattleAward
    {
        public:
            BattleAward(UInt8 i,UInt32 e,UInt32 m): mapId(i),exp(e),money(m) {}
            UInt8 GetMapId() const {   return mapId;  }
            UInt32 GetMoney() const {   return money;  }
            UInt32 GetExp()  const {   return exp; }
            void SetItems(const std::vector<ItemInfo>& items) { vecItem = items ; } 
            std::vector<ItemInfo> GetItems() const { return vecItem ; }

        private:
            UInt8 mapId;
            UInt32 exp;
            UInt32 money;
            std::vector<ItemInfo> vecItem;
    };

    class BattleAwardTable
    {
        public:
            void InsertBattleAward(BattleAward* ba)
            {
                _BattleAwardTable[ba->GetMapId()] = ba;
            }


            BattleAward* GetBattleAward(UInt8 mapId)
            {
                if( mapId <= 0 || mapId > AWARD_MAX )
                {
                    return NULL;
                }
                return _BattleAwardTable[mapId];
            }

        private:
            BattleAward* _BattleAwardTable[AWARD_MAX];
    };
    extern BattleAwardTable battleAwardTable;
}
#endif
