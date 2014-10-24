
#ifndef HORCRUX_H_
#define HORCRUX_H_
#include "Config.h"
#include "Common/Stream.h" 
#include "GData/AttrExtra.h"
#include "GData/SkillTable.h"
namespace GObject
{
#define HORCRUX_EQUIPMENT_MAX 4
#define HORCRUX_HOLD_MAX 5
    class Fighter;
    class ItemHorcrux;
    class Horcrux
    {
        public:
            Horcrux(Fighter *fgt)
            {
                _fighter = fgt ;
                memset(&_horcrux,0,sizeof(_horcrux)) ;
                memset(&_exp,0,sizeof(_exp)) ;
            }
            ItemHorcrux * GetEquip(UInt8 cnt){if(cnt >= HORCRUX_EQUIPMENT_MAX)return NULL ; return _horcrux[cnt]; }
            ItemHorcrux* SetHorcruxEquip(UInt8 pos ,ItemHorcrux * equip,UInt8 flag = 0);
            void SetHorcruxHoldExp(UInt8 index ,UInt32 exp);
            void AddHorcruxHoldExp(UInt8 index ,UInt32 exp);
            UInt32 GetHorcruxHoldExp(UInt8 index);
            void UpdateHorcurxHoldToDB();
            void UpdateHorcruxEquipmentToDB();
            void EatHorcrux(ItemHorcrux* item);
            void sendHorcruxInfo();
            UInt32 getHorcruxEquipmentTotalAttr(UInt8 index);
        private:
            Fighter * _fighter;
            ItemHorcrux * _horcrux[HORCRUX_EQUIPMENT_MAX];
            UInt32 _exp[HORCRUX_HOLD_MAX];
            static float limit[3];
    };
}
#endif // HORCRUX_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

