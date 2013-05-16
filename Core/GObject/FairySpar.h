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
        void freshElement(bool isCharge = true);
        void fuseBreakout();
        void fuseElement();
        void countermark();
        UInt8 getFusePercent();
        float getFairySparPH();
        float getFairySparAtk();
        float getFairySparMagAtk();
        void gmSetComplexPercent(UInt8 complexP);
        void gmSetElement(UInt8 elem1, UInt8 elem2, UInt8 elem3, UInt8 elem4, UInt8 elem5);

    private:
        Player* m_owner;
        Int32 m_atk;
        Int32 m_magAtk;
        Int32 m_phy;
        UInt8 m_element[5];
        UInt8 m_complexPercent;
        UInt8 m_curMark;
        UInt8 m_breakoutCnt;
};

}
#endif //_FAIRY_SPAR_

