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

    memset(m_grids, 0, sizeof(m_grids));
}

void MoFang::Init()
{
}

void MoFang::AddJGSFromDB(DBJiguanshu & dbData)
{
    m_jiguanshu.curLvl = dbData.curLvl;
    m_jiguanshu.curExp = dbData.curExp;
}

void MoFang::AddJGYFromDB(DBJiguanyu & dbData)
{
    if(dbData.pos > 0 && dbData.pos <= 49)
    {
        m_equipJG.insert(std::make_pair(dbData.pos, dbData.jiguanId));
    }

    if(0 == dbData.pos)
       m_jg.push_back(dbData.jiguanId); 

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
        if(iter->first > tempId && iter->first >= 10000)
        {
            tempId = iter->first;
        }
    }

    if(tempId >= 10000) //特殊图纸
    {
        tuzhiId = tempId + 1;
    }

    m_tuzhi.insert(std::make_pair(tuzhiId, 0));

    DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), tuzhiId, 0);
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
        toolId = 9314;
    else
        toolId = 9313;

    ItemBase * item = m_owner->GetPackage()->FindItem(toolId);
    if(NULL == item)
        return;

    UInt32 id = 0;
    if(tuzhiId > 10000) //特殊图纸，读取图纸ID为10000的配置 
        id = 10000;
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
            addExp = 10;
            successRate = 30;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 2;
            else if(curProficient>=51 && curProficient<=75)
                successRate += curProficient * 3;
            else if(curProficient>=76 && curProficient<=100)
                successRate += curProficient * 4;
            break;
        case TUZHI_BLUE:
            addExp = 20;
            successRate = 20;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient * 0.5;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 1;
            else if(curProficient>=51 && curProficient<=75)
                successRate += curProficient * 2;
            else if(curProficient>=76 && curProficient<=100)
                successRate += curProficient * 3;
            break;
        case TUZHI_PURPLE:
            addExp = 30;
            successRate = 10;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient * 0.1;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 0.5;
            else if(curProficient>=51 && curProficient<=75)
                successRate += curProficient;
            else if(curProficient>=76 && curProficient<=100)
                successRate += curProficient * 2;
            break;
        case TUZHI_YELLOW:
            addExp = 40;
            successRate = 5;
            if(curProficient>=1 && curProficient<=25)
                successRate += curProficient * 0.01;
            else if(curProficient>=26 && curProficient<=50)
                successRate += curProficient * 0.1;
            else if(curProficient>=51 && curProficient<=75)
                successRate += curProficient * 0.5;
            else if(curProficient>=76 && curProficient<=100)
                successRate += curProficient;
            break;
    }

    if(!m_owner->GetPackage()->DelItem(toolId, 1, false, ToMakeJGY))
        return;

    UInt8 result = 0;
    if(successRate >= 100)
    {
        m_jiguanshu.curExp += addExp;
        m_tuzhi.erase(iter);
        result = 1;

        if(tuzhiId < 10000) //训练图纸，制造后只增加机关术经验，不获得机关玉
        {
            m_jg.push_back(tzInfo->jiguanyuId); 
            DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), tzInfo->jiguanyuId, 0);
        }

        DB4().PushUpdateData("DELETE FROM `player_tuzhi` WHERE `tuzhiId` = %u AND `playerId` = %" I64_FMT "u", tuzhiId, m_owner->getId());
    }
    else
    {
        m_jiguanshu.curExp += 10;

        UInt8 randA = uRand(100);
        UInt8 randB = 0;
        switch(tzQuality)
        {
            case TUZHI_GREEN:
                randB = uRand(20);
                if(randB < 10)
                    curProficient += randB + 10;
                else
                    curProficient += randB;
                break; 
            case TUZHI_BLUE:
                randB = uRand(10);
                if(randA >= 0 && randA < 80)
                {
                    if(randB < 5)
                        curProficient += randB + 5;
                    else
                        curProficient += randB;
                }
                else if(randA >= 80 && randA < 90 && 0 == type)
                {
                    if(randB < 5)
                        curProficient -= randB + 5;
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
                randB = uRand(2);
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

        DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), tuzhiId,  curProficient);
    }

    if(10 == m_jiguanshu.curLvl)
    {

        DB4().PushUpdateData("UPDATE `player_jiguanshu` SET `curExp` = %u WHERE `playerId` = %" I64_FMT "u", m_jiguanshu.curExp, m_owner->getId());
    }
    else
        upgradeJGS();

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << tuzhiId << type << result << m_jiguanshu.curExp;
    if(0 == result)
        st << curProficient; 
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::upgradeJGS()
{
    if(!m_owner)
        return;

    if(m_jiguanshu.curLvl >= 10)
        return;

    GData::JiguanData::jiguanshuInfo * jgsInfo = GData::jiguanData.getJiguanshuInfo(m_jiguanshu.curLvl);
    if(!jgsInfo)
        return;

    if(m_jiguanshu.curExp >= jgsInfo->needExp)
    {
        ++m_jiguanshu.curLvl;        
    }

    DB4().PushUpdateData("REPLACE INTO `player_jiguanshu` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), m_jiguanshu.curLvl, m_jiguanshu.curExp);
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
                m_equipJG.insert(std::make_pair(pos, jgId));

                std::vector<UInt32>::iterator iter = m_jg.begin();
                for(; iter != m_jg.end(); iter++)
                {
                    if(jgId == *iter)
                        m_jg.erase(iter);
                }

                occupyMark = 1;
                posMark = pos;

                st << jgId << pos;
            }
            else
            {
                std::map<UInt8, UInt32>::iterator iter = m_equipJG.find(pos);
                if (iter != m_equipJG.end())
                {
                    if(iter->second > 0)
                        m_jg.push_back(iter->second);
                    m_equipJG.erase(iter);
                }
                
                st << pos;
            }

            for(UInt8 i=0; i<values.size(); i++)
            {
                m_grids[values[i]] = occupyMark;
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
            case 0:
                ae.attack += jgyInfo->attrValue;
                break;
            case 1:
                ae.defend += jgyInfo->attrValue;
                break;
            case 2:
                ae.magdef += jgyInfo->attrValue;
                break;
            case 3:
                ae.hitrlvl += jgyInfo->attrValue;
                break;
            case 4:
                ae.evdlvl += jgyInfo->attrValue;
                break;
            case 5:
                ae.pierce += jgyInfo->attrValue;
                break;
            case 6:
                ae.critical += jgyInfo->attrValue;
                break;
            case 7:
                ae.action += jgyInfo->attrValue;
                break;
            case 8:
                ae.magres += jgyInfo->attrValue;
                break;
            case 9:
                ae.tough += jgyInfo->attrValue;
                break;
            case 10:
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
    st << m_jiguanshu.curExp;

    UInt8 countA = 0;
    UInt8 countB = 0;

    std::map<UInt8, UInt32>::iterator iter = m_equipJG.begin();
    for(; iter != m_equipJG.end(); iter++)
    {
        st << iter->second << iter->first;
        countA++;    
    }

    std::vector<UInt32>::iterator iterA = m_jg.begin();
    for(; iterA != m_jg.end(); iterA++)
    {
        st << *iterA;
        countB++;
    }

    std::map<UInt32, UInt8>::iterator iterB = m_tuzhi.begin();
    for(; iterB != m_tuzhi.end(); iterB++)
    {
        st << iterB->first << iterB->second;
        countB++;    
    }

    st << Stream::eos;
    m_owner->send(st);
}

bool MoFang::checkPoint(UInt32 jgId, UInt8 pos, UInt8 mark, std::vector<UInt8> & values)
{
    UInt8 i = 0;
    UInt8 k = 0;
    UInt8 markA = 0;
    UInt8 markB = 0;
    UInt32 jiguanId = findEquipJG(pos);

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
        if(k >= 9 || (pos+i) < 0 || (pos+i) >= 49 || (pos+i) >= (m_jiguanshu.curLvl * 7))
            return false;

        if(0 != jgyInfo->molding[k])
        {
            if(markA == m_grids[pos+i])
            {
                return false;
            }
            else if(markB == m_grids[pos+i])
            {
                values.push_back(pos+i);

                if(8 == k)
                    break;

                if((i%3) == 2)
                {
                    i = 0;
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
                i = 0;
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
