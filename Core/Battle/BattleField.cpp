#include "Config.h"
//#include "GObject/Country.h"
#include "BattleField.h"
//#include "GObject/FairyPet.h"
#include "BattleFighter.h"
#include "math.h"

namespace Battle
{

    BattleField::BattleField(UInt16 fieldDistance, UInt16 timeActionLimit):_fieldDistance(fieldDistance),_timeActionLimit(timeActionLimit)
    {
        //memset(_objs, 0, sizeof(_objs));
    }

    BattleField::~BattleField()
    {
        clear();
    }
    void BattleField::clear()
    {

    }
    /*
       BattleObject * BattleField::GetTarget(UInt8 side, UInt16 posX ,UInt16 posY,UInt16 skillScopeId)
       { 
       BattleObject* bo = _objs[posX][posY];
       if(!bo)
       return NULL;
       if(bo->getClass() > 100)
       return NULL;
       BattleFighter* bf = static_cast<BattleFighter*>(bo);

       if(posY >= FIELD_HIGH || posX >= FIELD_WIDTH)
       return NULL;
       for(UInt16 j = 0; j < FIELD_HIGH ; ++j )
       {
       for(UInt16 i = 0 ; i < FIELD_WIDTH ; i++)
       { 
       UInt16 y = posY +i;
       if( y > FIELD_HIGH)
       break;
       if(!_objs[i][y])
       continue;
       if(_objs[i][y]->getClass() > eFighter)
       continue;
       BattleFighter* bf = static_cast<BattleFighter*>(_objs[j][i]);
       if(bf->GetSide() == side )
       continue;
       return _objs[i][y];
       } 

       for(UInt16 i = 0 ; i < FIELD_WIDTH ; i++)
       { 
       UInt16 y = posY +i;
       if( y > FIELD_HIGH)
       break;
       if(!_objs[j][i])
       continue;
       if(_objs[j][i]->getClass() > eFighter)
       continue;
       BattleFighter* bf = static_cast<BattleFighter*>(_objs[j][i]);
       if(bf->GetSide() == side )
       continue;
       return _objs[j][i];
       } 
       }
       return bf;
       } 
       */
    BattleObject * BattleField::GetTarget(UInt8 side, UInt16 posX ,UInt16 posY,UInt16 skillScopeId)
    { 
        if(side > 1)
            return NULL;
        UInt16 maxy = -1;
        UInt16 maxx = 0;
        UInt8 res = -1;
        for(UInt8 i = 0; i < _fighters[!side].size(); ++i)
        { 
            UInt16 x = _fighters[side][i]->getPosX();
            UInt16 y = _fighters[side][i]->getPosY();
            if( y <= maxy)
            {
                if(y == maxy && x >= maxx)
                    continue ;
                { 
                    maxy = y;
                    maxx = x;
                    res = i;
                } 
            }
        } 

        if(res != static_cast<UInt8>(-1))
        { 
            return _fighters[side][res];
        } 
        return NULL;
    } 

    UInt16 BattleField::getDistance(UInt16 x1, UInt16 y1, UInt16 x2, UInt16 y2)
    { 
        UInt16 adx = x1>x2?(x1-x2):(x2-x1);
        UInt16 ady = y1>y2?(y1-y2):(y2-y1);
        if(ady > 0 )
            --ady;
        return adx + ady;
    } 

    void BattleField::setObject(UInt16 x , UInt16 y , BattleObject * bo ,UInt16 isBody)
    { 
        //if(_objs[x][y] != NULL)
        //{   
        //    if(!_objs[x][y])
        //        delete _objs[x][y];
        //    _objs[x][y] = NULL;
        //}   
        //_objs[x][y] = bo;
    } 

    UInt16 BattleField::getDistance(BattleObject * bf , BattleObject* bo)
    { 
        UInt16 x1 = bf->getPosX();
        UInt16 y1 = bf->getPosY();
        UInt16 x2 = bo->getPosX();
        UInt16 y2 = bo->getPosY();
        UInt16 adx = x1>x2?(x1-x2):(x2-x1);
        UInt16 ady = y1>y2?(y1-y2):(y2-y1);
        if(static_cast<UInt16>(sqrt(adx*adx + ady*ady)) <= (bf->GetRad()+bo->GetRad()))
            return 0;
        return  (static_cast<UInt16>(sqrt(adx*adx + ady*ady)) - bf->GetRad() - bo->GetRad());

    } 

    bool BattleField::setObjectXY(UInt16 x, UInt16 y, BattleObject * bo, bool flag, UInt16 Body, UInt16 write)
    { 
        /*
           if(_objs[x][y] != NULL)
           {   
           if(!_objs[x][y])
           delete _objs[x][y];
           _objs[x][y] = NULL;
           }   
           _objs[x][y] = bo;
           */
        for(UInt8 j = 0; j < 2; ++j)
        {
            for(UInt8 i = 0; i < _fighters[j].size() ; ++i)
            { 
                if(bo == _fighters[j][i])
                    continue ;
                if(getDistance(bo,_fighters[j][i]) == 0)
                    return false;  
            } 
        }
        bo->setPos(x,y);
        return true;
    } 

    UInt16 BattleField::getShootDistance(BattleObject * begin , BattleObject * end , BattleObject* target)
    { 
        int x1 =static_cast<int>(begin->getPosX());
        int y1 =static_cast<int>(begin->getPosY());
        int x2 =static_cast<int>(end->getPosX());
        int y2 =static_cast<int>(end->getPosY());
        int x3 =static_cast<int>(target->getPosX());
        int y3 =static_cast<int>(target->getPosY());
        return static_cast<UInt16>(abs((y1-y2)*x3+(x2-x1)*y3-y1*x2+x1*y2)/sqrt(((y1-y2)*(y1-y2) + (x1-x2)*(x1-x2))));
    } 
    void BattleField::GetBSEnterInfo(Stream& st)
    { 
        st << static_cast<UInt8>(1);
        for(UInt8 i= 0; i < 2; ++i)
        { 
            st << static_cast<UInt8>(_fighters[i].size());
            for(UInt8 j = 0; j < _fighters[i].size(); ++ j)
            { 
               st << static_cast<UInt8>( _fighters[i][j]->GetBSNumber()); 
               st << static_cast<UInt16>(_fighters[i][j]->GetId());
               st << static_cast<UInt16>(_fighters[i][j]->getPosX());
               st << static_cast<UInt16>(_fighters[i][j]->getPosY());
               st << static_cast<UInt16>(_fighters[i][j]->getHP());
            } 
        } 
    } 
}
