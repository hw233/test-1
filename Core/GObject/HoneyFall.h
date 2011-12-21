#ifndef _HONEYFALL_H_
#define _HONEYFALL_H_

#include "Config.h"

namespace GObject
{

    class Player;

    enum HoneyFallType
    {
        e_HFT_None = 0,
        e_HFT_Equip_Enchant = 1,
        e_HFT_Trump_Enchant = 2,
        e_HFT_Max,
    };

    class HoneyFall
    {
        private:
            UInt32  m_hft_value[e_HFT_Max];
            Player* m_Owner;

        public:
            HoneyFall(Player* pl);
            ~HoneyFall();

            void updateHftValueToDB(HoneyFallType hft);
            UInt32 getHftValue(HoneyFallType hft);
            void setHftValue(UInt8 hft, UInt32 value);
            UInt32 incHftValue(HoneyFallType hft, UInt32 inc = 1);
            bool checkHft(UInt8 hft);
            UInt32 getChanceFromHft(UInt8 q, UInt8 lv, HoneyFallType hft);
    };

}


#endif

