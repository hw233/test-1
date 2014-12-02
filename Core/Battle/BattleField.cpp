#include "Config.h"
//#include "GObject/Country.h"
#include "BattleField.h"
//#include "GObject/FairyPet.h"

namespace Battle
{

    BattleField::BattleField()
    {
        memset(_objs, 0, sizeof(_objs));
    }

    BattleField::~BattleField()
    {
        clear();
    }
    void BattleField::clear()
    {

    }
    BattleObject * BattleField::GetTarget(UInt8 side, UInt8 posX ,UInt8 posY)
    { 
        //TODO
        return NULL;
    } 
    UInt8 BattleField::getDistance(UInt8 x1, UInt8 y1, UInt8 x2, UInt8 y2)
    { 
        //TODO
        return 0;
    } 

    void BattleField::setObject(UInt8 x , UInt8 y , BattleObject * bo ,UInt8 isBody)
    { 
        if(_objs[x][y] != NULL)
        {   
            if(!_objs[x][y])
                delete _objs[x][y];
            _objs[x][y] = NULL;
        }   
        _objs[x][y] = bo;
    } 
}
