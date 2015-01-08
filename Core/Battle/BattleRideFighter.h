#ifndef BATTLERIDEFIGHTER_H_
#define BATTLERIDEFIGHTER_H_
#include "BattleFighter.h"

class BattleRideFighter:
    public BattleFighter
{
public:
    BattleRideFighter(Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):
        BattleFighter(bf,f,pointX,pointY),_target(NULL),count(0)
    { 
        
    } 
    virtual void Action();
    virtual void PreGetObject(); 
private:
    BattleFighter * _target ; 
    UInt8 count; //控制来回次数
};

#endif // BATTLERIDEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

