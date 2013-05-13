#include "FairySpar.h"
#include "Player.h"
#include "MsgID.h"
#include "Script/GameActionLua.h"
#include "Country.h"

namespace GObject
{

#define ATK_MAX       1
#define MAG_ATK_MAX   2
#define PHY_MAX       3

    FairySpar::FairySpar(Player* player) : m_owner(player), m_atk(0), m_magAtk(0), m_phy(0), m_complexPercent(0), m_curMark(0), m_breakoutCnt(0), alreadyExistTable(false)
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

    void FairySpar::loadFairySparFromDB(UInt32 atk, UInt32 magAtk, UInt32 phy, UInt8 element1, UInt8 element2, UInt8 element3, UInt8 element4, UInt8 element5, UInt8 complexPercent, UInt8 curMark, UInt8 breakoutCnt)
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
        m_breakoutCnt = breakoutCnt;
        alreadyExistTable = true;
    }

    //血之晶石
    void FairySpar::sendAtkPhyInfo()
    {
        Stream st(REP::FARIY_SPAR);
        UInt8 type = 1;
        st << type;

        st << m_atk;
        st << m_atk * m_complexPercent / 100;
        st << GameAction()->GetFairySparParaMax(ATK_MAX, m_breakoutCnt + 1);

        st << m_magAtk;
        st << m_magAtk * m_complexPercent / 100;
        st << GameAction()->GetFairySparParaMax(MAG_ATK_MAX, m_breakoutCnt + 1);

        st << m_phy;
        st << m_phy * m_complexPercent / 100;
        st << GameAction()->GetFairySparParaMax(PHY_MAX, m_breakoutCnt + 1);

        st << m_breakoutCnt;
        st << Stream::eos;
        m_owner->send(st);
    }

    //晶石融合
    void FairySpar::sendElementInfo()
    {
        Stream st(REP::FARIY_SPAR);
        UInt8 type = 2;
        st << type;
        for(UInt8 i = 0; i < 5; i++)
            st << m_element[i];
        st << Stream::eos;
        m_owner->send(st);
    }

    //血印密纹
    void FairySpar::sendMarkInfo()
    {
        Stream st(REP::FARIY_SPAR);
        UInt8 type = 3;
        st << type;
        st << m_complexPercent;
        st << m_curMark;
        m_owner->send(st);
    }

    void FairySpar::sendAllInfo()
    {
        if(!alreadyExistTable)
        {
            getElement();
            DB3().PushUpdateData("INSERT INTO `fairy_spar`(`playerId`, `atk`, `magAtk`, `phy`,`element1`,`element2`,`element3`,`element4`,`element5`,`complexPercent`,`curMark`,`breakoutCnt`) VALUES (%"I64_FMT"u, 0, 0, 0, %u, %u, %u, %u, %u, 0, 0, 0)", m_owner->getId(), m_element[0], m_element[1], m_element[2], m_element[3], m_element[4]);
            alreadyExistTable = true;
        }
        sendAtkPhyInfo();
        sendElementInfo();
        sendMarkInfo();
    }

    void FairySpar::getElement()
    {
        lua_tinker::table elements = GameAction()->GetFairySparElement();
        if(elements.size() < 3)
        {
            ERROR_LOG("GetFairySparElement error!");
            return;
        }
        for(UInt8 i = 0; i < 5; i++)
            m_element[i] = elements.get<UInt8>(i + 1);
    }

    void FairySpar::freshElement()
    {
        getElement();
        sendElementInfo();
        DB3().PushUpdateData("UPDATE `fairy_spar` SET `element1` = %u, `element2` = %u, `element3` = %u, `element4` = %u, `element5` = %u WHERE `playerId` = %"I64_FMT"u", m_element[0], m_element[1], m_element[2], m_element[3], m_element[4], m_owner->getId());
    }
}

