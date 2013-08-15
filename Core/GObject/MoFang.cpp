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
    
    UInt8 count = m_jiguanshu.curLvl / 2;
    
    if(count > 0 && count <= 40)
    {
        for(UInt8 i=0; i<count; i++)
        {
            UInt8 temp[40] = {10, 11, 12, 13, 20, 27, 34, 41, 40, 39, 
                            38, 37, 30, 23, 16, 9, 2, 3, 4, 5,
                            6, 7, 14, 21, 28, 35, 42, 49, 48, 47,
                            46, 45, 44, 43, 36, 29, 22, 15, 8, 1};

            UInt8 index = temp[i];
            if(index > 0 && index <= 49)
                m_grids[index-1] = 0;
        }
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

void MoFang::addTuzhi(UInt32 tuzhiId)
{
    if(!m_owner)
        return;

    UInt32 tempId= 0;
    std::map<UInt32, UInt8>::iterator iter = m_tuzhi.begin();
    for(; iter != m_tuzhi.end(); iter++)
    {
        if(iter->first > tempId && iter->first >= 20000)
        {
            tempId = iter->first;
        }
    }

    if(tempId >= 20000) //特殊图纸
    {
        tuzhiId = tempId + 1;
    }

    m_tuzhi.insert(std::make_pair(tuzhiId, 0));

    DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), tuzhiId, 0);
    
    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(4);
    st << tuzhiId;
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
    if(tuzhiId > 20000) //特殊图纸，读取图纸ID为20000的配置 
        id = 20000;
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
    UInt8 successRate = 0;
    UInt32 addExp = 0;

    switch(tzQuality)
    {
        case TUZHI_GREEN:
            addExp = 80;
            successRate = 1;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient;
            else if(curProficient>=26 && curProficient<=100)
                successRate += curProficient * 3 / 100;
            break;
        case TUZHI_BLUE:
            addExp = 150;
            successRate = 0.5;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient * 0.02 / 100;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 2 / 100;
            else if(curProficient>=51 && curProficient<=100)
                successRate += curProficient * 3 / 100;
            break;
        case TUZHI_PURPLE:
            addExp = 300;
            successRate = 0.2;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient * 0.01 / 100;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 0.02 / 100;
            else if(curProficient>=51 && curProficient<=75)
                successRate += curProficient * 2 / 100;
            else if(curProficient>=76 && curProficient<=100)
                successRate += curProficient * 3 / 100;
            break;
        case TUZHI_YELLOW:
            addExp = 750;
            successRate = 0.1;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient * 0.01 / 100;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 0.02 /100;
            else if(curProficient>=51 && curProficient<=75)
                successRate += curProficient * 0.03 / 100;
            else if(curProficient>=76 && curProficient<=100)
                successRate += curProficient * 2.5 / 100;
            break;
    }

    if(!m_owner->GetPackage()->DelItem(toolId, 1, false, ToMakeJGY))
        return;

    UInt8 result = 0;
    if(successRate >= 100)
    {
        m_jiguanshu.curExp += addExp;
    }
    else
    {
        m_jiguanshu.curExp += 10;

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
    }

    if(curProficient >= 100 || successRate >= 100)
    {
        m_tuzhi.erase(iter);
        result = 1;

        if(tuzhiId < 20000) //训练图纸，制造后只增加机关术经验，不获得机关玉
        {
            m_jg.push_back(tzInfo->jiguanyuId); 
            DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), tzInfo->jiguanyuId, 0);
        }

        DB4().PushUpdateData("DELETE FROM `player_tuzhi` WHERE `tuzhiId` = %u AND `playerId` = %" I64_FMT "u", tuzhiId, m_owner->getId());
    }
    else
        DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), tuzhiId,  curProficient);

    if(100 == m_jiguanshu.curLvl)
    {

        DB4().PushUpdateData("UPDATE `player_jiguanshu` SET `curExp` = %u WHERE `playerId` = %" I64_FMT "u", m_jiguanshu.curExp, m_owner->getId());
    }
    else
        upgradeJGS();

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << tuzhiId << type << result << static_cast<UInt16>(m_jiguanshu.curExp);
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

    GData::JiguanData::jiguanshuInfo * jgsInfo = GData::jiguanData.getJiguanshuInfo(m_jiguanshu.curLvl);
    if(!jgsInfo)
        return;

    if(m_jiguanshu.curExp >= jgsInfo->needExp)
    {
        ++m_jiguanshu.curLvl;        
    }

    DB4().PushUpdateData("REPLACE INTO `player_jiguanshu` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), m_jiguanshu.curLvl, m_jiguanshu.curExp);

    if(0 == m_jiguanshu.curLvl % 2)
    {
        UInt8 temp[40] = {10, 11, 12, 13, 20, 27, 34, 41, 40, 39, 
                        38, 37, 30, 23, 16, 9, 2, 3, 4, 5,
                        6, 7, 14, 21, 28, 35, 42, 49, 48, 47,
                        46, 45, 44, 43, 36, 29, 22, 15, 8, 1};

        UInt8 lvlMark = m_jiguanshu.curLvl / 2;
        if(lvlMark > 0 && lvlMark <= 40)
        {
            UInt8 index = temp[lvlMark-1];
            if(index > 0 && index <= 49)
                m_grids[index-1] = 0;
        }
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
                ae.hitrlvl += jgyInfo->attrValue;
                break;
            case 6:
                ae.evdlvl += jgyInfo->attrValue;
                break;
            case 7:
                ae.pierce += jgyInfo->attrValue;
                break;
            case 8:
                ae.critical += jgyInfo->attrValue;
                break;
            case 9:
                ae.action += jgyInfo->attrValue;
                break;
            case 10:
                ae.magres += jgyInfo->attrValue;
                break;
            case 11:
                ae.tough += jgyInfo->attrValue;
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
    st << static_cast<UInt16>(m_jiguanshu.curExp);

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
