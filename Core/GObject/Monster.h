#include"GObjectBase.h"
#include "Config.h"

#ifndef MONSTER_H_
#define MONSTER_H_
#define MONSTER_GROUPMAX 10
namespace GObject
{
    class Monster
    {
        public:
            Monster(UInt32 id,UInt8 groupId,std::string name,UInt16 power,UInt16 money,UInt16 p,UInt32 itemId,UInt16 itemNum):_id(id),_groupId(groupId),_name(name),_power(power),_money(money),_pro(p),_itemId(itemId),_itemNum(itemNum) {}
            UInt32 GetMonsterId() const { return _id;}
            void SetMonsterId(UInt32 id) { _id = id;};
            std::string GetMonsterName() const { return _name;}
            UInt8  GetGroupId() const { return _groupId;}
            UInt16 GetMoney() const { return _money;}
            UInt16 GetItemId() const { return _itemId;}
            UInt16 GetPower() const { return _power;}
            UInt8  GetItemNum() const { return _itemNum;}
            UInt16 GetProb() const { return _pro;}

        private:
            UInt32 _id;
            UInt8  _groupId;
            std::string _name;
            UInt16 _power;
            UInt16 _money;
            UInt16 _pro; //出现的概率
            UInt32 _itemId;
            UInt8  _itemNum;
    };

    class MonsterTable
    {
        public:
            void InsertMonster(Monster *monster)
            {
                if(monster == NULL )
                    return;
                _monsterTable[monster->GetGroupId()].insert(monster);
            }

            Monster* GetMonster(UInt8 groupId,UInt32 id)
            {

                std::set<Monster*> monSet = _monsterTable[groupId];
                if( monSet.empty() )
                    return NULL;
                for(auto it= monSet.begin(); it != monSet.end(); ++it)
                {
                    if( (*it)->GetMonsterId() == id )
                    {
                        return (*it);
                    }
                }
                return NULL;
            }

            Monster* GetMonster(UInt32 id)
            {
                for(UInt8 i = 0 ; i < MONSTER_GROUPMAX ; ++i)
                {
                    for( auto it = _monsterTable[i].begin(); it != _monsterTable[i].end(); ++it)
                    {
                        if( (*it)->GetMonsterId() == id )
                        {
                            return (*it);
                        }
                    }
                }
                return NULL;

            }

            std::set<Monster*> &GetMonsterSet(UInt8 groupId)
            {
                return _monsterTable[groupId];
            }

        private:
            std::set<Monster*> _monsterTable[MONSTER_GROUPMAX];
    };
    extern MonsterTable monsterTable;
}

#endif
