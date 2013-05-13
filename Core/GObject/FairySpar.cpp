#include "FairySpar.h"
#include "Player.h"
#include "MsgID.h"
#include "Script/GameActionLua.h"

namespace GObject
{
    FairySpar::FairySpar(Player* player) : m_owner(player), m_atk(0), m_magAtk(0), m_phy(0), m_complexPercent(0), m_curMark(0)
    {
        m_element[0] = 0;
        m_element[1] = 0;
        m_element[2] = 0;
        m_element[3] = 0;
        m_element[4] = 0;
    }

    FairySpar::~FairySpar()
    {
    }

    void FairySpar::loadFairySparFromDB(UInt32 atk, UInt32 magAtk, UInt32 phy, UInt8 element1, UInt8 element2, UInt8 element3, UInt8 element4, UInt8 element5, UInt8 complexPercent, UInt8 curMark)
    {
        m_atk = atk;
        m_magAtk = magAtk;
        m_phy = phy;
        m_element[0] = element1;
        m_element[1] = element2;
        m_element[2] = element3;
        m_element[3] = element4;
        m_element[4] = element5;
        m_complexPercent = complexPercent;
        m_curMark = curMark;
    }
}

