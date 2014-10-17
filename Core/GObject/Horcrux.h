
#ifndef HORCRUX_H_
#define HORCRUX_H_
#include "Config.h"
#include "Common/Stream.h" 
#include "GData/AttrExtra.h"
#include "GData/SkillTable.h"
namespace GObject
{
#define HORCRUX_EQUIPMENT_MAX 4
    class Player ;
    class Fighter;
    class ItemEquip;
    class Package;
    class Horcrux
    {
        public:
            Horcrux(Fighter *fgt)
            {
                _fighter = fgt ;
                //memset(&_horcrux,0,sizeof(_horcrux)) ;
                for(UInt8 i = 0; i < HORCRUX_EQUIPMENT_MAX ; ++i)
                    _horcrux[i] = NULL;
            }
            ItemEquip * GetEquip(UInt8 cnt){if(cnt >= HORCRUX_EQUIPMENT_MAX)return NULL ; return _horcrux[cnt]; }
            ItemEquip* SetHorcruxEquip(UInt8 pos ,ItemEquip * equip,UInt8 flag = 0);
        private:
            Fighter * _fighter;
            ItemEquip * _horcrux[HORCRUX_EQUIPMENT_MAX];
    };
}
#endif // HORCRUX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

