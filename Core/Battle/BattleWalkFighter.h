#ifndef BATTLEWALKFIGHTER_H_
#define BATTLEWALKFIGHTER_H_
#include "BattleFighter.h"

class BattleWalkFighter:
    public BattleFighter
{
public:
    BattleWalkFighter(Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):
        BattleFighter(bf,f,pointX,pointY),_target(NULL)
    { 
        
    } 
    virtual void Action();
    virtual void PreGetObject(); 
private:
    BattleFighter _target; 
};

#endif // BATTLEWALKFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

