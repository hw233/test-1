#include "BattleSimulator.h"
#include "Script/BattleFormula.h"

namespace Battle
{
    BattleSimulator::BattleSimulator():BattleField()
    { 

    } 

    BattleFighter* BattleSimulator::newFighter(UInt8 x,UInt8 y ,GObject::Fighter * fgt)
    { 
        BattleFighter * bf = new(std::nothrow) Battle::BattleFighter(_formula,fgt,this, x, y);
        setObject(x, y, bf);
        return bf;
    } 
}
