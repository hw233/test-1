#include "DrinkAttr.h"
namespace GData
{
    DrinkAttr drinkAttrData;
    void DrinkAttr::setDrinkAttrTable(UInt32 val,stDrinkAttr& da)
    {
         _drinkAttr.insert( std::make_pair(val, da) );
    }

    DrinkAttr::stDrinkAttr*  DrinkAttr::getDrinkAttrTable(UInt32 val)
    {
        std::map<UInt32 , stDrinkAttr>::iterator it =  _drinkAttr.begin();
        stDrinkAttr* da = NULL;
        for(;it != _drinkAttr.end();++it)
        {
            da = &(it->second);
            if(val < it->first )
                break;
        }
        return da;
    }
}

