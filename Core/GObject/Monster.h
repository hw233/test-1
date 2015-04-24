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
            Monster(UInt32 id,UInt8 groupId,std::string name,UInt16 power,UInt16 money,UInt32 itemId,UInt16 itemNum):_id(id),_groupId(groupId),_name(name),_power(power),_money(money),_itemId(itemId),_itemNum(itemNum) {}
            ~Monster();
            UInt32 GetMonsterId() const { return _id;}
            void SetMonsterId(UInt32 id) { _id = id;};
            std::string GetMonsterName() const { return _name;}
            UInt8  GetGroupId() const { return _groupId;}
            UInt16 GetMoney() const { return _money;}
            UInt16 GetItemId() const { return _itemId;}
            UInt16 GetPower() const { return _power;}
            UInt8  GetItemNum() const { return _itemNum;}

        private:
            UInt32 _id;
            UInt8  _groupId;
            std::string _name;
            UInt16 _power;
            UInt16 _money;
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
                _monsterTable[monster->GetGroupId()-1].insert(monster);
            }

            Monster* GetMonster(UInt8 groupId,UInt32 id)
            {
               
                for(auto it= _monsterTable[groupId-1].begin(); it != _monsterTable[groupId-1].end(); ++it)
                {
                    if( (*it)->GetMonsterId() == id )
                    {
                        return (*it);
                    }
                }
                return NULL;
            }

        private:
            std::set<Monster*> _monsterTable[MONSTER_GROUPMAX];
    };
    extern MonsterTable monsterTable;
}

#endif
