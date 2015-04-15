#include"GObjectBase.h"
#include "Config.h"

#ifndef MONSTER_H_
#define MONSTER_H_
#define MONSTER_MAX 100
namespace GObject
{
    class Monster
    {
        public:
            Monster(UInt32 id,std::string name,UInt8 lev):_monsterId(id),_monsterName(name),_lev(lev) {}
            ~Monster();
            UInt32 GetMonsterId() const { return _monsterId;}
            void SetMonsterId(UInt32 id) { _monsterId = id;};
            std::string GetMonsterName() const { return _monsterName;}
            UInt8 GetLev() const { return _lev;}

        private:
            UInt32 _monsterId;
            std::string _monsterName;
            UInt8 _lev;    //
    };

    class MonsterTable
    {
        public:
            void InsertMonster(Monster *monster)
            {
                if(monster == NULL )
                    return;
                monsterTable[monster->GetMonsterId()] = monster;
            }

            Monster*  GetMonster(UInt32 id)
            {
                return monsterTable[id];
            }

        private:
            Monster* monsterTable[MONSTER_MAX];
    };
    extern MonsterTable monsterTable;
}

#endif
