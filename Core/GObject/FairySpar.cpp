#include "FairySpar.h"
#include "Player.h"
#include "MsgID.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "Clan.h"

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
        st << GameAction()->GetFairySparParaMax(ATK_MAX, m_breakoutCnt);

        st << m_magAtk;
        st << m_magAtk * m_complexPercent / 100;
        st << GameAction()->GetFairySparParaMax(MAG_ATK_MAX, m_breakoutCnt);

        st << m_phy;
        st << m_phy * m_complexPercent / 100;
        st << GameAction()->GetFairySparParaMax(PHY_MAX, m_breakoutCnt);

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

    void FairySpar::fuseElement()
    {
        UInt16 level = m_owner->getClan()->getStatueLevel();
        if(m_breakoutCnt <= level)
        {
            m_owner->sendMsgCode(0, 1359);
            return;
        }
        UInt16 prob = uRand(10000);
        UInt32 atkTmpSum = 0;
        UInt32 magAtkTmpSum = 0;
        UInt32 phyTmpSum = 0;
        UInt32 atkTmp;
        UInt32 magAtkTmp;
        UInt32 phyTmp;
        UInt8 i;
        bool addPercent100 = false;
        bool doublePercent100 = false;

        for(i = 0; i < 5; i++)
        {
            switch(m_element[i])
            {
                case 12:
                    addPercent100 = true;
                break;
                case 13:
                    doublePercent100 = true;
                break;
                default:
                break;
            }
        }

        for(i = 0; i < 5; i++)
        {
            atkTmp = 0;
            magAtkTmp = 0;
            phyTmp = 0;
            switch(m_element[i])
            {
                case 1:
                    if(addPercent100 || prob < 2000)
                        atkTmp += 10;
                break;
                case 2:
                    if(addPercent100 || prob < 2000)
                        magAtkTmp += 10;
                break;
                case 3:
                    if(addPercent100 || prob < 2000)
                        phyTmp += 10;
                break;
                case 4:
                    if(addPercent100 || prob < 2000)
                        atkTmp += 20;
                break;
                case 5:
                    if(addPercent100 || prob < 2000)
                        magAtkTmp += 20;
                break;
                case 6:
                    if(addPercent100 || prob < 2000)
                        phyTmp += 20;
                break;
                case 7:
                    if(addPercent100 || prob < 2000)
                        atkTmp += 30;
                break;
                case 8:
                    if(addPercent100 || prob < 2000)
                        magAtkTmp += 30;
                break;
                case 9:
                    if(addPercent100 || prob < 2000)
                        phyTmp += 30;
                break;
                case 10:
                    if(doublePercent100 || prob < 2000)
                    {
                        atkTmp *= 2;
                        magAtkTmp *= 2;
                        phyTmp *= 20;
                    }
                break;
                case 11:
                    if(doublePercent100 || prob < 3000)
                    {
                        atkTmp *= 2;
                        magAtkTmp *= 2;
                        phyTmp *= 20;
                    }
                break;
                case 14:
                break;
                default:
                break;
            }
            atkTmpSum += atkTmp;
            magAtkTmpSum += magAtkTmp;
            phyTmpSum += phyTmp;
        }

        bool isDirty = false;
        if(atkTmpSum > 0)
        {
            m_atk += atkTmpSum;
            isDirty = true;
        }
        if(magAtkTmpSum > 0)
        {
            m_magAtk += magAtkTmpSum;
            isDirty = true;
        }
        if(atkTmpSum > 0)
        {
            m_phy += phyTmpSum;
            isDirty = true;
        }
        if(isDirty)
        {
            sendAtkPhyInfo();
            m_owner->setFightersDirty(true);
            DB3().PushUpdateData("UPDATE `fairy_spar` SET `atk` = %u, `magAtk` = %u, `phy` = %u  WHERE `playerId` = %"I64_FMT"u", m_atk, m_magAtk, m_phy, m_owner->getId());
        }
    }

    void FairySpar::countermark()
    {
    }

}

