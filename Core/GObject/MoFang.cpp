#include "MoFang.h"
#include "Player.h"
#include "Item.h"
#include "MsgID.h"
#include "Package.h"
#include "GData/AttrExtra.h"
#include "GData/JiguanData.h"
#include "GObjectDBExecHelper.h"

namespace GObject
{

MoFang::MoFang(Player* pl)
{
    m_owner = pl;

    memset(m_grids, -1, sizeof(m_grids));

    UInt8 mofangInit[9] = {17, 18, 19, 24, 25, 26, 31, 32, 33};
    for(UInt8 i=0; i<9; i++)
    {
        UInt8 index = mofangInit[i];
        if(index > 0 && index <= 49)
            m_grids[index-1] = 0;
    }
}

void MoFang::Init()
{
}

void MoFang::AddJGSFromDB(DBJiguanshu & dbData)
{
    m_jiguanshu.curLvl = dbData.curLvl;
    m_jiguanshu.curExp = dbData.curExp;
    
    UInt8 temp[40] = {10, 11, 12, 13, 20, 27, 34, 41, 40, 39, 
                    38, 37, 30, 23, 16, 9, 2, 3, 4, 5,
                    6, 7, 14, 21, 28, 35, 42, 49, 48, 47,
                    46, 45, 44, 43, 36, 29, 22, 15, 8, 1};

    UInt8 count = m_jiguanshu.curLvl / 2;
   
    if(count > 40)
        count = 40;

    for(UInt8 i=0; i<count; i++)
    {
        UInt8 index = temp[i];
        if(index > 0 && index <= 49)
            m_grids[index-1] = 0;
    }
}

void MoFang::AddJGYFromDB(DBJiguanyu & dbData)
{
    if(dbData.pos > 0 && dbData.pos <= 49)
    {
        m_equipJG.insert(std::make_pair(dbData.pos, dbData.jiguanId));
        UInt8 pos = dbData.pos;
        GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(dbData.jiguanId);
        if(!jgyInfo)
            return;

        UInt8 k = 0;
        UInt8 i = 0;
        UInt8 mark = false;
        for(; i<3; i++)
        {
            if(mark)
                i = 0;

            if(k >= 9 || (pos+i) < 0 || (pos+i) >= 49)
                return;

            if(0 != jgyInfo->molding[k])
            {
                if(-1 == m_grids[pos+i-1])
                {
                    return;
                }
                else if(0 == m_grids[pos+i-1])
                {
                    m_grids[pos+i-1] = 1;

                    if(8 == k)
                        break;

                    if((i%3) == 2)
                    {
                        mark = true;
                        pos = pos + 7;
                    }

                    k++;
                }
                else
                    return; 
            }
            else
            {
                if(8 == k)
                    break;

                if((i%3) == 2)
                {
                    mark = true;
                    pos = pos + 7;
                }

                k++;
            }
        }
    }
    else if(0 == dbData.pos)
    {
       m_jg.push_back(dbData.jiguanId); 
    }
}

void MoFang::AddTuzhiFromDB(DBTuzhi & dbData)
{
    m_tuzhi.insert(std::make_pair(dbData.tuzhiId, dbData.curProficient));
}

UInt32 MoFang::addTuzhi(UInt32 tuzhiId, bool mark)
{
    if(!m_owner)
        return 0;

    GData::JiguanData::tuzhiInfo * tzInfo = GData::jiguanData.getTuzhiInfo(tuzhiId);
    if(!tzInfo)
        return 0;

    if(tuzhiId > NORMAL_TUZHI && tuzhiId < SPECIAL_TUZHI)
    {
        std::map<UInt8, UInt32>::iterator iter = m_equipJG.begin();
        for(; iter != m_equipJG.end(); iter++)
        {
            if(iter->second == (tuzhiId - NORMAL_TUZHI))
                tuzhiId = SPECIAL_TUZHI;
        }

        bool findRes = findNoEquipJG(tuzhiId - NORMAL_TUZHI);
        if(findRes)
            tuzhiId = SPECIAL_TUZHI;

        std::map<UInt32, UInt8>::iterator iterA = m_tuzhi.find(tuzhiId);
        if(iterA != m_tuzhi.end())
            tuzhiId = SPECIAL_TUZHI;
    }

    if(tuzhiId >= SPECIAL_TUZHI)
    {
        UInt32 tempId= 0;
        std::map<UInt32, UInt8>::iterator iterB = m_tuzhi.begin();
        for(; iterB != m_tuzhi.end(); iterB++)
        {
            if(iterB->first > tempId && iterB->first >= SPECIAL_TUZHI)
            {
                tempId = iterB->first;
            }
        }

        if(tempId >= SPECIAL_TUZHI) //特殊图纸
        {
            tuzhiId = tempId + 1;
        }
    }

    m_tuzhi.insert(std::make_pair(tuzhiId, 0));

    DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), tuzhiId, 0);

    if(!mark)
    {
        Stream st(REP::MOFANG_INFO);
        st << static_cast<UInt8>(4);
        st << tuzhiId;
        st << Stream::eos;
        m_owner->send(st);
    }

    return tuzhiId;
}

void MoFang::addJGSExp(UInt32 exp)
{
    m_jiguanshu.curExp += exp;

    upgradeJGS();

    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(5);
    st << static_cast<UInt32>(m_jiguanshu.curExp);
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::randTuzhi(UInt8 num)
{
    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(6);
    st << num;

    for(UInt8 i=0; i<num; i++)
    {
        UInt8 lvl = 0;
        UInt8 markA = uRand(100);

        if(m_jiguanshu.curLvl == 0)
        {
            lvl = 0;
        }
        else if(m_jiguanshu.curLvl == 1)
        {
            lvl = 1;
        }
        else if(m_jiguanshu.curLvl > 19)
        {
            lvl = uRand(19);
        }
        else if(markA < 30)
        {
            lvl = m_jiguanshu.curLvl;
        }
        else if(markA < 60)
        {
            lvl = m_jiguanshu.curLvl - 1;
        }
        else if(markA < 100)
        {
            lvl = m_jiguanshu.curLvl - 2;
        }
        else
        {
            continue;
        }

        UInt32 id = GData::jiguanData.getTuzhiId(lvl);

        UInt32 tuzhiId = addTuzhi(id, true);

        st << tuzhiId;

        if(tuzhiId > SPECIAL_TUZHI) //特殊图纸，读取图纸ID为200000的配置 
            tuzhiId = SPECIAL_TUZHI;

        GData::JiguanData::tuzhiInfo * tzInfo = GData::jiguanData.getTuzhiInfo(tuzhiId);
        if(!tzInfo)
            return;

        UInt8 tzQuality = tzInfo->quality;

        switch(tzQuality)
        {
            case TUZHI_GREEN:
                m_owner->udpLog("feigong", "F_130817_2", "", "", "", "", "act");
                break;
            case TUZHI_BLUE:
                m_owner->udpLog("feigong", "F_130817_3", "", "", "", "", "act");
                break;
            case TUZHI_PURPLE:
                m_owner->udpLog("feigong", "F_130817_4", "", "", "", "", "act");
                break;
            case TUZHI_YELLOW:
                m_owner->udpLog("feigong", "F_130817_5", "", "", "", "", "act");
                break;
        }

        m_owner->udpLog("feigong", "F_130817_1", "", "", "", "", "act");
    }
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::makejiguan(UInt32 tuzhiId, UInt8 type, UInt8 mark)
{
    if(!m_owner)
        return;

    std::map<UInt32, UInt8>::iterator iter = m_tuzhi.find(tuzhiId);
    if(iter == m_tuzhi.end())
        return;

    UInt32 toolId = 0;
    if(1 == type)
        toolId = 9414;
    else
        toolId = 9413;

    ItemBase * item = m_owner->GetPackage()->FindItem(toolId);
    if(NULL == item)
        return;

    UInt32 id = 0;
    if(tuzhiId > SPECIAL_TUZHI) //特殊图纸，读取图纸ID为200000的配置 
        id = SPECIAL_TUZHI;
    else
        id = tuzhiId;

    GData::JiguanData::tuzhiInfo * tzInfo = GData::jiguanData.getTuzhiInfo(id);
    if(!tzInfo)
        return;

    UInt8 needJgsLvl = tzInfo->needjgsLvl;
    if(needJgsLvl > m_jiguanshu.curLvl)
        return;

    UInt8 tzQuality = tzInfo->quality; 
    UInt8 curProficient = iter->second;
    float successRate = 0;
    UInt32 addExp = 0;

    switch(tzQuality)
    {
        case TUZHI_GREEN:
            addExp = 80;
            successRate = 1;
            if(curProficient<=25)
                successRate += curProficient;
            else if(curProficient<=50)
            {
                successRate += curProficient;
                successRate += (curProficient - 25) * 3;
            }
            else if(curProficient<=75)
            {
                successRate += curProficient;
                successRate += (curProficient - 25) * 3;
                successRate += (curProficient - 50) * 3;
            }
            else if(curProficient<=100)
            {
                successRate += curProficient;
                successRate += (curProficient - 25) * 3;
                successRate += (curProficient - 50) * 3;
                successRate += (curProficient - 75) * 3;
            }
            m_owner->udpLog("feigong", "F_130817_7", "", "", "", "", "act");
            break;
        case TUZHI_BLUE:
            addExp = 150;
            successRate = 0.5;
            if(curProficient<=25)
                successRate += curProficient * 0.02;
            else if(curProficient<=50)
            {
                successRate += curProficient * 0.02;
                successRate += (curProficient - 25) * 2;
            }
            else if(curProficient<=75)
            {
                successRate += curProficient * 0.02;
                successRate += (curProficient - 25) * 2;
                successRate += (curProficient - 50) * 3;
            }
            else if(curProficient<=100)
            {
                successRate += curProficient * 0.02;
                successRate += (curProficient - 25) * 2;
                successRate += (curProficient - 50) * 3;
                successRate += (curProficient - 75) * 3;
            }
            m_owner->udpLog("feigong", "F_130817_8", "", "", "", "", "act");
            break;
        case TUZHI_PURPLE:
            addExp = 300;
            successRate = 0.2;
            if(curProficient<=25)
                successRate += curProficient * 0.01;
            else if(curProficient<=50)
            {
                successRate += curProficient * 0.01;
                successRate += (curProficient - 25) * 0.02;
            }
            else if(curProficient<=75)
            {
                successRate += curProficient * 0.01;
                successRate += (curProficient - 25) * 0.02;
                successRate += (curProficient - 50) * 2;
            }
            else if(curProficient<=100)
            {
                successRate += curProficient * 0.01;
                successRate += (curProficient - 25) * 0.02;
                successRate += (curProficient - 50) * 2;
                successRate += (curProficient - 75) * 3;
            }
            m_owner->udpLog("feigong", "F_130817_9", "", "", "", "", "act");
            break;
        case TUZHI_YELLOW:
            addExp = 750;
            successRate = 0.1;
            if(curProficient<=25)
                successRate += curProficient * 0.01;
            else if(curProficient<=50)
            {
                successRate += curProficient * 0.01;
                successRate += (curProficient - 25) * 0.02;
            }
            else if(curProficient<=75)
            {
                successRate += curProficient * 0.01;
                successRate += (curProficient - 25) * 0.02;
                successRate += (curProficient - 50) * 0.03;
            }
            else if(curProficient<=100)
            {
                successRate += curProficient * 0.01;
                successRate += (curProficient - 25) * 0.02;
                successRate += (curProficient - 50) * 0.03;
                successRate += (curProficient - 75) * 2.5;
            }
            m_owner->udpLog("feigong", "F_130817_10", "", "", "", "", "act");
            break;
    }

    if(!m_owner->GetPackage()->DelItem(toolId, 1, false, ToMakeJGY))
        return;

    UInt8 result = 0;
    if(curProficient >= 100 || uRand(10000) < successRate*100)
    {
        if(m_jiguanshu.curLvl < 100)
        {
            m_jiguanshu.curExp += addExp;
            if(m_jiguanshu.curExp > JGS_MAXEXP)
                m_jiguanshu.curExp = JGS_MAXEXP;
        }

        if(tuzhiId < SPECIAL_TUZHI) //训练图纸，制造后只增加机关术经验，不获得机关玉
        {
            m_jg.push_back(tzInfo->jiguanyuId);

            DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), tzInfo->jiguanyuId, 0);
        }

        DB4().PushUpdateData("DELETE FROM `player_tuzhi` WHERE `tuzhiId` = %u AND `playerId` = %" I64_FMT "u", tuzhiId, m_owner->getId());

        m_tuzhi.erase(iter);
        result = 1;

        switch(tzQuality)
        {
            case TUZHI_GREEN:
                m_owner->udpLog("feigong", "F_130817_12", "", "", "", "", "act");
                break;
            case TUZHI_BLUE:
                m_owner->udpLog("feigong", "F_130817_13", "", "", "", "", "act");
                break;
            case TUZHI_PURPLE:
                m_owner->udpLog("feigong", "F_130817_14", "", "", "", "", "act");
                break;
            case TUZHI_YELLOW:
                m_owner->udpLog("feigong", "F_130817_15", "", "", "", "", "act");
                break;
        }

        m_owner->udpLog("feigong", "F_130817_11", "", "", "", "", "act");
    }
    else
    {
        UInt8 randA = uRand(100);
        UInt8 randB = 0;
        switch(tzQuality)
        {
            case TUZHI_GREEN:
                randB = uRand(7);
                if(randB < 4)
                    curProficient += 5;
                else
                    curProficient += randB;
                break; 
            case TUZHI_BLUE:
                randB = uRand(6);
                if(randA >= 0 && randA < 80)
                {
                    if(randB < 3)
                        curProficient += randB + 3;
                    else
                        curProficient += randB;
                }
                else if(randA >= 80 && randA < 90 && 0 == type)
                {
                    if(randB < 3)
                        curProficient -= randB + 3;
                    else
                        curProficient -= randB;
                }
                break;
            case TUZHI_PURPLE:
                randB = uRand(5);
                if(randA >= 0 && randA < 60)
                {
                    if(randB < 2)
                        curProficient += randB + 2;
                    else
                        curProficient += randB;
                }
                else if(randA >=60 && randA < 80 && 0 == type)
                {
                    if(randB < 2)
                        curProficient -= randB + 2;
                    else
                        curProficient -= randB;
                }
                break;
            case TUZHI_YELLOW:
                randB = uRand(4);
                if(randA >= 0 && randA < 30)
                {
                    if(randB == 0)
                        curProficient += randB + 1;
                    else
                        curProficient += randB;
                }
                else if(randA >= 30 && randA < 60 && 0 == type)
                {
                    if(randB == 0)
                        curProficient -= randB + 1;
                    else
                        curProficient -= randB;
                }
                break;
        }

        iter->second = curProficient;

        if(m_jiguanshu.curLvl < 100)
        {
            m_jiguanshu.curExp += 10;
            if(m_jiguanshu.curExp > JGS_MAXEXP)
                m_jiguanshu.curExp = JGS_MAXEXP;
        }

        DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), tuzhiId,  curProficient);
    }

    upgradeJGS();

    m_owner->udpLog("feigong", "F_130817_6", "", "", "", "", "act");

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << tuzhiId << type << result << static_cast<UInt32>(m_jiguanshu.curExp);
    if(0 == result)
        st << static_cast<UInt8>(curProficient); 
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::upgradeJGS()
{
    if(!m_owner)
        return;

    if(m_jiguanshu.curLvl >= 100)
        return;

    GData::JiguanData::jiguanshuInfo * jgsInfo = GData::jiguanData.getUpgradeInfo(m_jiguanshu.curExp);
    if(!jgsInfo)
        return;

    m_jiguanshu.curLvl = jgsInfo->jgshuLvl + 1;

    DB4().PushUpdateData("REPLACE INTO `player_jiguanshu` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), m_jiguanshu.curLvl, m_jiguanshu.curExp);

    UInt8 temp[40] = {10, 11, 12, 13, 20, 27, 34, 41, 40, 39, 
                    38, 37, 30, 23, 16, 9, 2, 3, 4, 5,
                    6, 7, 14, 21, 28, 35, 42, 49, 48, 47,
                    46, 45, 44, 43, 36, 29, 22, 15, 8, 1};

    UInt8 lvlMark = m_jiguanshu.curLvl / 2;

    if(lvlMark > 40)
        lvlMark = 40;

    for(UInt8 i=0; i<lvlMark; i++)
    {
        UInt8 index = temp[i];
        if(index > 0 && index <= 49 && (m_grids[index-1] == -1))
            m_grids[index-1] = 0;
    }
}

void MoFang::equipJG(UInt32 jgId, UInt8 pos, UInt8 mark)
{
    if(!m_owner)
        return;

    if(EQUIP_JG == mark || DISMANT_JG == mark)
    {
        std::vector<UInt8> values;
        bool res = checkPoint(jgId, pos, mark, values);
        if(res)
        {
            Stream st(REP::MOFANG_INFO);
            st << mark;

            UInt8 posMark = 0;
            UInt8 occupyMark = 0;
            if(EQUIP_JG == mark)
            {
                m_equipJG.insert(std::make_pair(pos+1, jgId));

                std::vector<UInt32>::iterator iter = m_jg.begin();
                for(; iter != m_jg.end(); iter++)
                {
                    if(jgId == *iter)
                    {
                        m_jg.erase(iter);
                        break;
                    }
                }

                occupyMark = 1;
                posMark = pos + 1;

                st << jgId << pos;
            }
            else
            {
                std::map<UInt8, UInt32>::iterator iter = m_equipJG.find(pos+1);
                if (iter != m_equipJG.end())
                {
                    if(iter->second > 0)
                    {
                        m_jg.push_back(iter->second);
                        jgId = iter->second;
                    }

                    m_equipJG.erase(iter);
                }
                
                st << pos;
            }

            for(UInt8 i=0; i<values.size(); i++)
            {
                m_grids[values[i]-1] = occupyMark;
            }

            DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), jgId, posMark);

            st << Stream::eos;
            m_owner->send(st);

            std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
            for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++ it)
            {
                it->second->setDirty();
            }
        }
    }
}

void MoFang::addJGYAttr(GData::AttrExtra& ae)
{
    std::map<UInt8, UInt32>::iterator iter = m_equipJG.begin();
    for(; iter != m_equipJG.end(); iter++)
    {
        GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(iter->second);
        if(!jgyInfo)
            continue;

        if(JIGUANQI == jgyInfo->mark)
            continue;

        switch(jgyInfo->attrType)
        {
            case 1:
                ae.attack += jgyInfo->attrValue;
                break;
            case 2:
                ae.magatk += jgyInfo->attrValue;
                break;
            case 3:
                ae.defend += jgyInfo->attrValue;
                break;
            case 4:
                ae.magdef += jgyInfo->attrValue;
                break;
            case 5:
                ae.hp += jgyInfo->attrValue;
                break;
            case 6:
                ae.tough += jgyInfo->attrValue;
                break;
            case 7:
                ae.action += jgyInfo->attrValue;
                break;
            case 8:
                ae.hitrlvl += jgyInfo->attrValue;
                break;
            case 9:
                ae.evdlvl += jgyInfo->attrValue;
                break;
            case 10:
                ae.critical += jgyInfo->attrValue;
                break;
            case 11:
                ae.pierce += jgyInfo->attrValue;
                break;
            case 12:
                ae.counter += jgyInfo->attrValue;
                break;
        }
    }
}

UInt16 MoFang::useJGQskill()
{
    std::map<UInt8, UInt32>::iterator iter = m_equipJG.begin();
    for(; iter != m_equipJG.end(); iter++)
    {
        GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(iter->second);
        if(!jgyInfo)
            continue;

        if(JIGUANQI == jgyInfo->mark)
            return jgyInfo->skillId;
    }
    return 0;
}

void MoFang::sendMoFangInfo(UInt8 mark)
{
    if(!m_owner)
        return;

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << static_cast<UInt32>(m_jiguanshu.curExp);

    UInt8 countA = 0;
    UInt8 countB = 0;
    UInt16 offsetA = 0;
    UInt16 offsetB = 0;

    offsetA = st.size();
    st << countA;
    std::map<UInt8, UInt32>::iterator iter = m_equipJG.begin();
    for(; iter != m_equipJG.end(); iter++)
    {
        st << iter->second << static_cast<UInt8>(iter->first - 1);
        countA++;    
    }

    offsetB = st.size();
    st << countB;
    std::vector<UInt32>::iterator iterA = m_jg.begin();
    for(; iterA != m_jg.end(); iterA++)
    {
        st << static_cast<UInt32>(*iterA);
        countB++;
    }

    std::map<UInt32, UInt8>::iterator iterB = m_tuzhi.begin();
    for(; iterB != m_tuzhi.end(); iterB++)
    {
        st << iterB->first << iterB->second;
        countB++;    
    }

    st.data<UInt8>(offsetA) = countA;
    st.data<UInt8>(offsetB) = countB;

    st << Stream::eos;
    m_owner->send(st);
}

bool MoFang::checkPoint(UInt32 jgId, UInt8 pos, UInt8 mark, std::vector<UInt8> & values)
{
    UInt8 i = 0;
    UInt8 k = 0;
    UInt8 markA = 0;
    UInt8 markB = 0;
    bool markC = false;
    UInt32 jiguanId = findEquipJG(pos+1);

    if(EQUIP_JG == mark)
    {
        if(jiguanId > 0)
            return false;

        bool findRes = findNoEquipJG(jgId);
        if(!findRes)
            return false;

        std::map<UInt8, UInt32>::iterator iter = m_equipJG.begin();
        for(; iter != m_equipJG.end(); iter++)
        {
            GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(iter->second);
            if(!jgyInfo)
                continue;

            if(JIGUANQI == jgyInfo->mark)
                return false;
        }

        markA = OCCUPY_MOFANG;
        markB = NOOCCUPY_MOFANG;
    }
    else
    {
        if(0 == jiguanId)
            return false;

        jgId = jiguanId;
        markA = NOOCCUPY_MOFANG;
        markB = OCCUPY_MOFANG;
    }

    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(jgId);
    if(!jgyInfo)
        return false;

    for(; i<3; i++)
    {
        if(markC)
            i = 0;

        if(k >= 9 || (pos+i) < 0 || (pos+i) >= 49)
            return false;

        if(0 != jgyInfo->molding[k])
        {
            if(markA == m_grids[pos+i] || -1 == m_grids[pos+i])
            {
                return false;
            }
            else if(markB == m_grids[pos+i])
            {
                values.push_back(pos+i+1);

                if(8 == k)
                    break;

                if((i%3) == 2)
                {
                    markC = true;
                    pos = pos + 7;
                }

                k++;
            }
            else
                return false; 
        }
        else
        {
            if(8 == k)
                break;

            if((i%3) == 2)
            {
                markC = true;
                pos = pos + 7;
            }

            k++;
        }
    }

    return true;
}

UInt32 MoFang::findEquipJG(UInt8 pos)
{
    std::map<UInt8, UInt32>::iterator iter = m_equipJG.find(pos);
    if(iter == m_equipJG.end())
        return 0;

    return iter->second;
}

bool MoFang::findNoEquipJG(UInt32 jgId)
{
    std::vector<UInt32>::iterator iter = m_jg.begin();
    for(; iter != m_jg.end(); iter++)
    {
        if(jgId == *iter)
            return true;
    }

    return false;
}

}
