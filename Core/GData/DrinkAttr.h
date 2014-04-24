#ifndef DRINKATTR_H_
#define DRINKATTR_H_
#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"
namespace GData
{
class DrinkAttr
{
public:
        struct stDrinkAttr
        {
           UInt32 hp; 
           stDrinkAttr():hp(0){}
        };
private:
        std::map<UInt32 , stDrinkAttr>  _drinkAttr;
public:
        DrinkAttr(){}
        void setDrinkAttrTable(UInt32 val , stDrinkAttr&);
        stDrinkAttr * getDrinkAttrTable(UInt32);

};
extern DrinkAttr drinkAttrData;
}

#endif // DRINKATTR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

