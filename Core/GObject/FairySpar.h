#ifndef _FAIRY_SPAR_
#define _FAIRY_SPAR_
/*
    仙蕴晶石
*/
#include "Config.h"

namespace GObject
{

class Player;

class FairySpar
{
    public:
        FairySpar(Player* player);
        ~FairySpar();
        void loadFairySparFromDB(UInt32 atk, UInt32 magAtk, UInt32 phy, UInt8 element1, UInt8 element2, UInt8 element3, UInt8 element4, UInt8 element5, UInt8 complexPercent, UInt8 curMark, UInt8 breakoutCnt);
        void sendAtkPhyInfo();
        void sendElementInfo();
        void sendMarkInfo();
        void sendAllInfo();
        void getElement();
        void freshElement();
        void fuseElement();
        void countermark();
        float getFairySparPH();
        float getFairySparAtk();
        float getFairySparMagAtk();

    private:
        Player* m_owner;
        UInt32 m_atk;
        UInt32 m_magAtk;
        UInt32 m_phy;
        UInt8 m_element[5];
        UInt8 m_complexPercent;
        UInt8 m_curMark;
        UInt8 m_breakoutCnt;
        bool alreadyExistTable;
};

}
#endif //_FAIRY_SPAR_

