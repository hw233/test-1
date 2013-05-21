#include "FairySpar.h"
#include "Player.h"
#include "MsgID.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "Clan.h"
#include "Package.h"

namespace GObject
{

#define ATK_MAX       1
#define MAG_ATK_MAX   2
#define PHY_MAX       3

    const Int32 statusLevel[7] = {0,5,10,15,20,25,30};
    const Int32 atkMax[7] = {100,160,240,340,460,600,800};
    const Int32 magAtkMax[7] = {100,160,240,340,460,600,800};
    const Int32 phyMax[7] = {1000,1600,2400,3400,4600,6000,8000};

    FairySpar::FairySpar(Player* player) : m_owner(player), m_atk(0), m_magAtk(0), m_phy(0), m_complexPercent(0), m_curMark(0), m_breakoutCnt(0)
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
    }

    //血之晶石
    void FairySpar::sendAtkPhyInfo()
    {
        if(m_breakoutCnt > 6)
            return;

        Stream st(REP::FARIY_SPAR);
        UInt8 type = 1;
        st << type;

        st << m_atk;
        st << m_atk * m_complexPercent / 100;
        //st << GameAction()->GetFairySparParaMax(ATK_MAX, m_breakoutCnt);
        st << atkMax[m_breakoutCnt];

        st << m_magAtk;
        st << m_magAtk * m_complexPercent / 100;
        //st << GameAction()->GetFairySparParaMax(MAG_ATK_MAX, m_breakoutCnt);
        st << magAtkMax[m_breakoutCnt];

        st << m_phy;
        st << m_phy * m_complexPercent / 100;
        //st << GameAction()->GetFairySparParaMax(PHY_MAX, m_breakoutCnt);
        st << phyMax[m_breakoutCnt];

        st << m_breakoutCnt;
        st << getFusePercent();
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
        st << Stream::eos;
        m_owner->send(st);
    }

    void FairySpar::sendAllInfo()
    {
        if(m_element[0] == 0 && m_element[1] == 0 && m_element[2] == 0 && m_element[3] == 0 && m_element[4] == 0)
        {
            getElement();
            DB3().PushUpdateData("INSERT INTO `fairy_spar`(`playerId`, `atk`, `magAtk`, `phy`,`element1`,`element2`,`element3`,`element4`,`element5`,`complexPercent`,`curMark`,`breakoutCnt`) VALUES (%"I64_FMT"u, 0, 0, 0, %u, %u, %u, %u, %u, 0, 0, 0)", m_owner->getId(), m_element[0], m_element[1], m_element[2], m_element[3], m_element[4]);
        }
        sendAtkPhyInfo();
        sendElementInfo();
        sendMarkInfo();
    }

    void FairySpar::getElement()
    {
        UInt8 flag;
        if(getFusePercent() == 100)
            flag = 2;
        else
            flag = 1;
        lua_tinker::table elements = GameAction()->GetFairySparElement(flag);
        if(elements.size() < 5)
        {
            ERROR_LOG("GetFairySparElement error!");
            return;
        }
        for(UInt8 i = 0; i < 5; i++)
            m_element[i] = elements.get<UInt8>(i + 1);
    }

    void FairySpar::freshElement(bool isCharge)
    {
        if(isCharge)
        {
            if(m_owner->getClanProffer() < 100)
            {
                m_owner->sendMsgCode(0, 1360);
                return;
            }
            ConsumeInfo ci(FairySparFreshElement, 0, 0);
            m_owner->useClanProffer(100, &ci);
        }
        getElement();
        sendElementInfo();
        DB3().PushUpdateData("UPDATE `fairy_spar` SET `element1` = %u, `element2` = %u, `element3` = %u, `element4` = %u, `element5` = %u WHERE `playerId` = %"I64_FMT"u", m_element[0], m_element[1], m_element[2], m_element[3], m_element[4], m_owner->getId());
    }

    void FairySpar::fuseBreakout()
    {
        if(m_breakoutCnt >= 6)
            return;
        UInt16 level = m_owner->getClan()->getStatueLevel();
        if(level <= statusLevel[m_breakoutCnt])
        {
            m_owner->sendMsgCode(0, 1359);
            return;
        }
        UInt32 proffer = (getFusePercent() / 10 + 1) * 50 * (m_breakoutCnt + 1);
        if(m_owner->getClanProffer() < proffer)
        {
            m_owner->sendMsgCode(0, 1360);
            return;
        }
        ConsumeInfo ci(FairySparBreakoutElement, 0, 0);
        m_owner->useClanProffer(proffer, &ci);

        UInt16 prob = uRand(10000);
        UInt16 successProb = 0;
        UInt32 atkTmp = 0;
        UInt32 magAtkTmp;
        UInt32 phyTmp;

        for(UInt8 i = 0; i < 5; i++)
        {
            switch(m_element[i])
            {
                case 21:
                    successProb += 1000;
                break;
                case 22:
                    successProb += 2000;
                break;
                case 23:
                    successProb += 3000;
                break;
                case 24:
                    successProb += 4000;
                break;
                case 25:
                    atkTmp += 25;
                break;
                case 26:
                    magAtkTmp += 25;
                break;
                case 27:
                    phyTmp += 250;
                break;
                default:
                break;
            }
        }
        bool isDirty = false;
        if(prob < successProb)
        {
            ++m_breakoutCnt;
            if(atkTmp > 0)
            {
                m_atk += atkTmp;
                isDirty = true;
            }
            if(magAtkTmp > 0)
            {
                m_magAtk += magAtkTmp;
                isDirty = true;
            }
            if(phyTmp > 0)
            {
                m_phy += phyTmp;
                isDirty = true;
            }
            if(isDirty)
                m_owner->setFightersDirty(true);
            sendAtkPhyInfo();
            DB3().PushUpdateData("UPDATE `fairy_spar` SET `atk` = %u, `magAtk` = %u, `phy` = %u, `breakoutCnt` = %u  WHERE `playerId` = %"I64_FMT"u", m_atk, m_magAtk, m_phy, m_breakoutCnt, m_owner->getId());
        }
        freshElement(false);
    }

    void FairySpar::fuseElement()
    {
        if(getFusePercent() >= 100)
            return fuseBreakout();

        if(m_breakoutCnt > 6)
            return;
        if(m_breakoutCnt == 6 && m_atk >= atkMax[m_breakoutCnt] && m_magAtk >= magAtkMax[m_breakoutCnt] && m_phy >= phyMax[m_breakoutCnt])
            return;
        UInt32 proffer = (getFusePercent() / 10 + 1) * 50 * (m_breakoutCnt + 1);
        if(m_owner->getClanProffer() < proffer)
        {
            m_owner->sendMsgCode(0, 1360);
            return;
        }
        ConsumeInfo ci(FairySparFuseElement, 0, 0);
        m_owner->useClanProffer(proffer, &ci);

        UInt16 prob = uRand(10000);
        UInt32 atkTmpSum = 0;
        UInt32 magAtkTmpSum = 0;
        UInt32 phyTmpSum = 0;
        UInt32 atkTmp;
        UInt32 magAtkTmp;
        UInt32 phyTmp;
        UInt8 i;
        UInt8 doublePercentCnt = 0;
        UInt8 addPercentCnt = 0;
        bool addPercent100 = false;
        bool doublePercent100 = false;

        for(i = 0; i < 5; i++)
        {
            switch(m_element[i])
            {
                case 10:
                    ++doublePercentCnt;
                break;
                case 11:
                    ++addPercentCnt;
                break;
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
                        phyTmp += 100;
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
                        phyTmp += 200;
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
                        phyTmp += 300;
                break;
                default:
                break;
            }

            Int32 curtmp;
            Int32 curdoublePercentCnt;
            Int32 curaddPercentCnt;
            for(UInt8 j = 0; j < 3; j++)
            {
                if(j == 0)
                    curtmp = atkTmp;
                else if(j == 1)
                    curtmp = magAtkTmp;
                else
                    curtmp = phyTmp;

                curdoublePercentCnt = doublePercentCnt;
                curaddPercentCnt = addPercentCnt;

                if(curtmp > 0)
                {
                    while(curdoublePercentCnt > 0)
                    {
                        --curdoublePercentCnt;
                        if(uRand(10000) < 3000)
                        {
                            curtmp *= 2;
                            break;
                        }
                    }
                    while(curaddPercentCnt > 0)
                    {
                        --curaddPercentCnt;
                        if(uRand(10000) < 3000)
                        {
                            curtmp += 10;
                            break;
                        }
                    }
                }

                if(j == 0)
                    atkTmp = curtmp;
                else if(j == 1)
                    magAtkTmp = curtmp;
                else
                    phyTmp = curtmp;
            }

            atkTmpSum += atkTmp;
            magAtkTmpSum += magAtkTmp;
            phyTmpSum += phyTmp;
        }

        bool isDirty = false;
        if(atkTmpSum > 0 && m_atk < atkMax[m_breakoutCnt])
        {
            m_atk += atkTmpSum;
            if(m_atk > atkMax[m_breakoutCnt])
                m_atk = atkMax[m_breakoutCnt];
            isDirty = true;
        }
        if(magAtkTmpSum > 0 && m_magAtk < magAtkMax[m_breakoutCnt])
        {
            m_magAtk += magAtkTmpSum;
            if(m_magAtk > magAtkMax[m_breakoutCnt])
                m_magAtk = magAtkMax[m_breakoutCnt];
            isDirty = true;
        }
        if(atkTmpSum > 0 && m_phy < phyMax[m_breakoutCnt])
        {
            m_phy += phyTmpSum;
            if(m_phy > phyMax[m_breakoutCnt])
                m_phy = phyMax[m_breakoutCnt];
            isDirty = true;
        }
        if(isDirty)
        {
            sendAtkPhyInfo();
            m_owner->setFightersDirty(true);
            DB3().PushUpdateData("UPDATE `fairy_spar` SET `atk` = %u, `magAtk` = %u, `phy` = %u  WHERE `playerId` = %"I64_FMT"u", m_atk, m_magAtk, m_phy, m_owner->getId());
        }
        freshElement(false);
    }

    void FairySpar::countermark()
    {
#define MARK_ITEM_ID  9389
        if(m_owner->GetPackage()->GetItemAnyNum(MARK_ITEM_ID) < 1)
            return;
        if(m_complexPercent >= 100)
            return;
        m_owner->GetPackage()->DelItemAny(MARK_ITEM_ID, 1);
        Int32 startPoint;
        Int32 endPoint;
        if(m_complexPercent == 0)
            startPoint = 50;
        else if(m_complexPercent == 1)
            startPoint = 45;
        else if(m_complexPercent == 2)
            startPoint = 40;
        else if(m_complexPercent == 3)
            startPoint = 35;
        else if(m_complexPercent == 4)
            startPoint = 30;
        else if(m_complexPercent == 5)
            startPoint = 25;
        else if(m_complexPercent == 6)
            startPoint = 20;
        else if(m_complexPercent == 7)
            startPoint = 15;
        else if(m_complexPercent == 8)
            startPoint = 10;
        else if(m_complexPercent == 9)
            startPoint = 5;
        else if(m_complexPercent == 10)
            startPoint = 0;
        else
            startPoint = -5;

        endPoint = startPoint + 15;
        if(m_curMark == 0)
        {
            startPoint += 10;
            endPoint += 5;
        }
        else if(m_curMark == 20)
        {
            startPoint += 8;
            endPoint += 3;
        }
        else if(m_curMark == 40)
        {
            startPoint += 6;
            endPoint += 1;
        }
        else if(m_curMark == 60)
        {
            startPoint += 4;
            endPoint += -1;
        }
        else if(m_curMark == 80)
        {
            startPoint += 2;
            endPoint += -3;
        }
        else if(m_curMark == 90)
        {
            startPoint += 0;
            endPoint += -5;
        }

        Int32 range = endPoint - startPoint + 1;
        Int32 tmp = startPoint + uRand(range);
        if(tmp != 0)
        {
            tmp += m_curMark;
            if(tmp < 0)
                tmp = 0;
            m_curMark = tmp;
            if(m_curMark >= 100)
            {
                ++m_complexPercent;
                m_curMark -= 100;
                DB3().PushUpdateData("UPDATE `fairy_spar` SET `complexPercent` = %u, `curMark` = %u WHERE `playerId` = %"I64_FMT"u", m_complexPercent, m_curMark, m_owner->getId());
                sendAtkPhyInfo();
                sendMarkInfo();
            }
            else
            {
                DB3().PushUpdateData("UPDATE `fairy_spar` SET `curMark` = %u WHERE `playerId` = %"I64_FMT"u", m_curMark, m_owner->getId());
                sendMarkInfo();
            }

        }
    }

    UInt8 FairySpar::getFusePercent()
    {
        if(m_breakoutCnt > 6)
            return 0;
        Int32 phyAdd;
        Int32 atkAdd;
        Int32 magAtkAdd;
        if(m_phy > phyMax[m_breakoutCnt])
            phyAdd = m_phy - phyMax[m_breakoutCnt];
        else
            phyAdd = m_phy;
        if(m_atk > atkMax[m_breakoutCnt])
            atkAdd = m_atk - atkMax[m_breakoutCnt];
        else
            atkAdd = m_atk;
        if(m_magAtk > magAtkMax[m_breakoutCnt])
            magAtkAdd = m_magAtk - magAtkMax[m_breakoutCnt];
        else
            magAtkAdd = m_magAtk;
        printf("---%u, %u, %u, ", phyAdd, atkAdd, magAtkAdd);
        printf("%u, %u, %u---\n", phyMax[m_breakoutCnt], atkMax[m_breakoutCnt], magAtkMax[m_breakoutCnt]);
        return (phyAdd+atkAdd*10+magAtkAdd*10)*100/(phyMax[m_breakoutCnt]+atkMax[m_breakoutCnt]*10+magAtkMax[m_breakoutCnt]*10);
    }

    float FairySpar::getFairySparPH()
    {
        return static_cast<float>(m_phy * ( 100 + m_complexPercent) / 100);
    }

    float FairySpar::getFairySparAtk()
    {
        return static_cast<float>(m_atk * ( 100 + m_complexPercent) / 100);
    }

    float FairySpar::getFairySparMagAtk()
    {
        return static_cast<float>(m_magAtk * ( 100 + m_complexPercent) / 100);
    }

}

