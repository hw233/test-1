
#ifndef KETTLENPC_H_
#define KETTLENPC_H_

#define KETTLE_COUNT 12    //壶中界数
#define MONSTER_TYPE 7   //每层的怪物数量
#define MONSTER_KING 5   //每层的怪物星数
#include "Common/URandom.h"
#include "AttrExtra.h"
namespace GData
{
    struct KettleAttr
    {
        float _attack;
        float _magatk;
        float _hp;
        float _action;
        KettleAttr(float attack , float magatk , float hp,float action):_attack(attack),_magatk(magatk),_hp(hp),_action(action){}
        KettleAttr():_attack(0),_magatk(0),_hp(0),_action(0){}
    };
    class KettleNpc
    {
        public:
            void LoadKettleMonsterAttr(UInt8 kettleNum /*壶中界*/, UInt8 num /*怪物编号*/, KettleAttr npcAttr/*1星属性*/)
            {
                if(kettleNum >= KETTLE_COUNT || num >= MONSTER_TYPE )
                    return ;
                _kettleMonsterAttr[kettleNum][num] = npcAttr;
            }
            void GetMonsterAttr(AttrExtra& ae ,UInt8 kettleNum , UInt8 num ,UInt8 king , UInt8 count )
            {
                if(kettleNum >= KETTLE_COUNT || num >= MONSTER_TYPE || count > 5)
                    return ;
                ae.attack += (_kettleMonsterAttr[kettleNum][num]._attack * king * count);
                ae.magatk += (_kettleMonsterAttr[kettleNum][num]._magatk * king * count);
                ae.hp += (_kettleMonsterAttr[kettleNum][num]._hp * king * count);
                ae.action += (_kettleMonsterAttr[kettleNum][num]._action * king * count);
                return ;
            }
        private:
           KettleAttr _kettleMonsterAttr[KETTLE_COUNT][MONSTER_TYPE];
    };
    static KettleNpc kettleAttrData;
}
#endif // KETTLENPC_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

