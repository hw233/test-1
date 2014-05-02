u
#ifndef DRINKATTR_H_
#define DRINKATTR_H_

class DrinkAttr
{
public:
        struct stDrinkAttr
        {
           UInt32 hp; 
        }
private:
        std::map<UInt32 , stDrinkAttr>  _drinkAttr;
public:
        void setDrinkAttrTable(stXingchen&);
        stDrinkAttr * getDrinkAttrTable(UInt32);

};

#endif // DRINKATTR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

