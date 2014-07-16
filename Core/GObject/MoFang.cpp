#include "MoFang.h"
#include "Player.h"
#include "Item.h"
#include "MsgID.h"
#include "Package.h"
#include "GData/AttrExtra.h"
#include "GData/JiguanData.h"
#include "GObjectDBExecHelper.h"
#include "Server/SysMsg.h"

namespace GObject
{

MoFang::MoFang(Player* pl)
{
    m_owner = pl;
 
    memset(m_kyQuality, 0, sizeof(m_kyQuality));
    for(UInt8 n=0; n<EQUIP_SCHEME_MAX; n++)
    {
        for(UInt8 m=0; m<EQUIP_GRID_MAX; m++)
        {
            m_grids[n][m] = -1;
        }
    }

    UInt8 mofangInit[9] = {17, 18, 19, 24, 25, 26, 31, 32, 33};
    for(UInt8 i=0; i<9; i++)
    {
        UInt8 index = mofangInit[i];
        if(index > 0 && index <= EQUIP_GRID_MAX)
        {
            for(UInt8 j=0; j<EQUIP_SCHEME_MAX; j++)
            {
                m_grids[j][index-1] = 0;
            }
        }
    }
}

void MoFang::Init()
{
}

void MoFang::AddJGSFromDB(DBJiguanshu & dbData)
{
    m_jiguanshu.curExp = dbData.curExp;

    GData::JiguanData::jiguanshuInfo * jgsInfo = GData::jiguanData.getUpgradeInfo(dbData.curExp);
    if(!jgsInfo)
        return;
    
    if(m_jiguanshu.curExp == jgsInfo->totalExp)
    {
        m_jiguanshu.curLvl = jgsInfo->jgshuLvl + 1;
    }
    else
        m_jiguanshu.curLvl = jgsInfo->jgshuLvl;

    //m_jiguanshu.curLvl = dbData.curLvl;
    //m_jiguanshu.curExp = dbData.curExp;

    UInt8 temp[20][2] = {{10, 11}, {12, 13}, {20, 27}, {34, 41}, {40, 39}, 
                {38, 37}, {30, 23}, {16, 9}, {2, 3}, {4, 5},
                {6, 7}, {14, 21}, {28, 35}, {42, 49}, {48, 47},
                {46, 45}, {44, 43}, {36, 29}, {22, 15}, {8, 1}};

    UInt8 lvlMark = m_jiguanshu.curLvl;
    UInt8 index = 0;

    for(UInt8 i=0; i<lvlMark; i++)
    {
        for(UInt8 k=0; k<2; k++)
        {
           index = temp[i][k]; 
           if(index > 0 && index <= EQUIP_GRID_MAX)
           {
               for(UInt8 j=0; j<EQUIP_SCHEME_MAX; j++)
               {
                   if(m_grids[j][index-1] == -1)
                       m_grids[j][index-1] = 0;
               }
           }
        }
    }
}

void MoFang::AddJGYFromDB(DBJiguanyu & dbData)
{
    UInt8 pos = dbData.pos;
    UInt8 scheme = dbData.scheme;
    if(scheme > EQUIP_SCHEME_MAX || pos > EQUIP_GRID_MAX)
        return;

    if(pos>0 && scheme==0) //老数据，默认放在第一方案
    {
        scheme = 1;
        m_owner->SetVar(VAR_JIGUAN_SCHEME, scheme);
        m_equipJGA.insert(std::make_pair(dbData.jiguanId, dbData.pos));
        DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), dbData.jiguanId, pos, scheme);
    }

    if(0!=pos && 0!=scheme)
    {
        GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(dbData.jiguanId);
        if(!jgyInfo)
            return;

        int i = 0;
        UInt8 k = 0;
        UInt8 mark = false;
        for(; i<3; i++)
        {
            if(k >= 9)
                return;

            if(((pos+i-1) < 0 || (pos+i-1) >= 49) && (0 != jgyInfo->molding[k]))
                return;

            if(0 != jgyInfo->molding[k])
            {
                if(-1 == m_grids[scheme-1][pos+i-1])
                {
                    return;
                }
                else if(0 == m_grids[scheme-1][pos+i-1])
                {
                    m_grids[scheme-1][pos+i-1] = 1;

                    if(8 == k)
                        break;

                    if(i == 2)
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

                if(i == 2)
                {
                    mark = true;
                    pos = pos + 7;
                }

                k++;
            }

            if(mark)
            {
                i = -1;
                mark = false;
            }
        }

        switch(scheme)
        {
            case 1:
                m_equipJGA.insert(std::make_pair(dbData.jiguanId, dbData.pos));
                break;
            case 2:
                m_equipJGB.insert(std::make_pair(dbData.jiguanId, dbData.pos));
                break;
            case 3:
                m_equipJGC.insert(std::make_pair(dbData.jiguanId, dbData.pos));
                break;
        }
    }
    else if(0==dbData.pos && 0==dbData.scheme)
    {
       m_jg.push_back(dbData.jiguanId); 
    }
}

void MoFang::AddKeyinFromDB(DBKeyin & dbData)
{
    m_kyLvl.insert(std::make_pair(dbData.keyinId, dbData.curLvl));
    m_kyCurExp.insert(std::make_pair(dbData.keyinId, dbData.curExp));

    if(dbData.keyinId >=1 && dbData.keyinId <=4)
    {
        GData::JiguanData::keyinInfo * kyInfo = GData::jiguanData.getKeyinInfo(dbData.keyinId, dbData.curLvl);
        if(!kyInfo)
            return;

        m_kyQuality[dbData.keyinId - 1] = kyInfo->quality;
    }
}

void MoFang::AddTuzhiFromDB(DBTuzhi & dbData)
{
    m_tuzhi.insert(std::make_pair(dbData.tuzhiId, dbData.curProficient));
}

void MoFang::AddZhenweiFromDB(DBZhenwei &dbData)
{
    m_zhenwei.insert(std::make_pair(dbData.keyId, dbData.mark));
}

void MoFang::AddGearFromDB(UInt16 gearId, UInt8 mark)
{
    if(GEAR_COMMON != mark && GEAR_SPECIAL != mark)
        return;

    if(GEAR_COMMON == mark)
        m_commonGear.push_back(gearId);
    else
        m_specialGear.push_back(gearId);
}

UInt32 MoFang::addTuzhi(UInt32 tuzhiId, bool mark)
{
    if(!m_owner)
        return 0;

    GData::JiguanData::tuzhiInfo * tzInfo = GData::jiguanData.getTuzhiInfo(tuzhiId);
    if(!tzInfo)
        return 0;

    if(tuzhiId > NORMAL_TUZHI && tuzhiId <= SPECIAL_TUZHI)
    {
        bool findRes = false;

        findRes = findEquipJG(tuzhiId - NORMAL_TUZHI);
        if(findRes)
        {
            m_jiguanshu.curExp += 30;
            return 0;
        }

        findRes = findNoEquipJG(tuzhiId - NORMAL_TUZHI);
        if(findRes)
        {
            m_jiguanshu.curExp += 30;
            return 0;
        }

        std::map<UInt32, UInt8>::iterator iterA = m_tuzhi.find(tuzhiId);
        if(iterA != m_tuzhi.end())
        {
            m_jiguanshu.curExp += 30;
            return 0;
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

    UInt32 sendExp = 0;
    if(m_jiguanshu.curExp > JGS_MAXEXP)
        sendExp = JGS_MAXEXP;
    else
        sendExp = m_jiguanshu.curExp;

    SYSMSG_SENDV(4935, m_owner, 9421, exp);
    SYSMSG_SENDV(4936, m_owner, 9421, exp);

    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(5);
    st << static_cast<UInt32>(sendExp);
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::randTuzhi(UInt16 num)
{
    UInt16 count = 0;
    //UInt8 jgsLvl = 0;

    /*if(m_jiguanshu.curLvl > 19) //机关术等级大于19，按照19等级的规则获取相关图纸
        jgsLvl = 19;
    else
        jgsLvl = m_jiguanshu.curLvl;*/

    UInt8 jgsLvl = m_jiguanshu.curLvl;
    UInt32 exp = m_jiguanshu.curExp;

    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(6);
    UInt16 offsetA = 0;
    offsetA = st.size();
    st << count;

    for(UInt16 i=0; i<num; i++)
    {
        UInt8 lvl = 0;
        UInt8 markA = uRand(100);

        if(jgsLvl == 0)
        {
            lvl = 0;
        }
        else if(jgsLvl == 1)
        {
            lvl = 1;
        }
        else if(jgsLvl > 19)
        {
            lvl = uRand(20);
        }
        else if(markA < 30)
        {
            lvl = jgsLvl;
        }
        else if(markA < 60)
        {
            lvl = jgsLvl - 1;
        }
        else if(markA < 100)
        {
            lvl = uRand(jgsLvl - 1);
        }
        else
        {
            continue;
        }

        UInt32 id = GData::jiguanData.getTuzhiId(lvl);

        UInt32 tuzhiId = addTuzhi(id, true);
        if(tuzhiId > 0)
        {
            //std::cout << "tuzhiId : " << tuzhiId << std::endl;
            st << tuzhiId;
            count ++;

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
    }

    upgradeJGS();

    exp = m_jiguanshu.curExp - exp;
    if(exp > 0)
    {
        SYSMSG_SENDV(4935, m_owner, 9418, exp);
        SYSMSG_SENDV(4936, m_owner, 9418, exp);
    }

    UInt32 sendExp = 0;
    if(m_jiguanshu.curExp > JGS_MAXEXP)
        sendExp = JGS_MAXEXP;
    else
        sendExp = m_jiguanshu.curExp;

    st.data<UInt16>(offsetA) = count;
    st << static_cast<UInt32>(sendExp); 
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

    UInt16 toolCount = m_owner->GetPackage()->GetItemAnyNum(toolId);
    if(0 == toolCount)
        return;

    GData::JiguanData::tuzhiInfo * tzInfo = GData::jiguanData.getTuzhiInfo(tuzhiId);
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
            addExp = 20;
            successRate = 1;
            if(curProficient<=25)
                successRate += curProficient;
            else if(curProficient<=50)
            {
                successRate += 25;
                successRate += (curProficient - 25) * 3;
            }
            else if(curProficient<=75)
            {
                successRate += 25 * 4;
                successRate += (curProficient - 50) * 3;
            }
            else if(curProficient<=100)
            {
                successRate += 25 * 7;
                successRate += (curProficient - 75) * 3;
            }
            m_owner->udpLog("feigong", "F_130817_7", "", "", "", "", "act");
            break;
        case TUZHI_BLUE:
            addExp = 50;
            successRate = 0.5;
            if(curProficient<=25)
                successRate += curProficient * 0.02;
            else if(curProficient<=50)
            {
                successRate += 25 * 0.02;
                successRate += (curProficient - 25) * 2;
            }
            else if(curProficient<=75)
            {
                successRate += 25 * 2.02;
                successRate += (curProficient - 50) * 3;
            }
            else if(curProficient<=100)
            {
                successRate += 25 * 5.02;
                successRate += (curProficient - 75) * 3;
            }
            m_owner->udpLog("feigong", "F_130817_8", "", "", "", "", "act");
            break;
        case TUZHI_PURPLE:
            addExp = 400;
            successRate = 0.2;
            if(curProficient<=25)
                successRate += curProficient * 0.01;
            else if(curProficient<=50)
            {
                successRate += 25 * 0.01;
                successRate += (curProficient - 25) * 0.02;
            }
            else if(curProficient<=75)
            {
                successRate += 25 * 0.03;
                successRate += (curProficient - 50) * 2;
            }
            else if(curProficient<=100)
            {
                successRate += 25 * 2.03;
                successRate += (curProficient - 75) * 3;
            }
            m_owner->udpLog("feigong", "F_130817_9", "", "", "", "", "act");
            break;
        case TUZHI_YELLOW:
            addExp = 2250;
            successRate = 0.1;
            if(curProficient<=25)
                successRate += curProficient * 0.001;
            else if(curProficient<=50)
            {
                successRate += 25 * 0.001;
                successRate += (curProficient - 25) * 0.005;
            }
            else if(curProficient<=75)
            {
                successRate += 25 * 0.006;
                successRate += (curProficient - 50) * 0.01;
            }
            else if(curProficient<=100)
            {
                successRate += 25 * 0.016;
                successRate += (curProficient - 75) * 2.5;
            }
            m_owner->udpLog("feigong", "F_130817_10", "", "", "", "", "act");
            break;
    }

    if(!m_owner->GetPackage()->DelItemAny(toolId, 1, NULL, ToMakeJGY))
        return;

    UInt8 result = 0;
    UInt32 randValue = uRand(100000);
    if(curProficient >= 100 || randValue < successRate*1000)
    {
        m_jiguanshu.curExp += addExp;

        m_jg.push_back(tzInfo->jiguanyuId);

        DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), tzInfo->jiguanyuId, 0, 0);
        DBLOG1().PushUpdateData("insert into make_jiguanyu (server_id, player_id, jiguanyu_id, jiguanyu_name, jiguanyu_quality, make_time) values(%u,%" I64_FMT "u,%u,'%s',%u,%u)",
                cfg.serverLogId, m_owner->getId(), tzInfo->jiguanyuId, tzInfo->name.c_str(), tzInfo->quality, TimeUtil::Now());

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
                
                GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(tzInfo->jiguanyuId);
                if(!jgyInfo)
                    return;

                SYSMSG_BROADCASTV(4937, m_owner->getCountry(), m_owner->getName().c_str(), jgyInfo->quality, (jgyInfo->name).c_str());
                break;
        }

        m_owner->udpLog("feigong", "F_130817_11", "", "", "", "", "act");
    }
    else
    {
        UInt8 randA = uRand(100);
        UInt8 randB = uRand(3);
        switch(tzQuality)
        {
            case TUZHI_GREEN:
                    curProficient += randB + 4;
                break; 
            case TUZHI_BLUE:
                randB += 3;
                if(randA < 85 || 1 == type)
                {
                    curProficient += randB;
                }
                else if(randA >= 85 && randA < 100 && 0 == type)
                {
                    if(randB < curProficient)
                        curProficient -= randB;
                    else
                        curProficient = 0;
                }
                break;
            case TUZHI_PURPLE:
                randB += 2;
                if(randA < 70 || 1 == type)
                {
                    curProficient += randB;
                }
                else if(randA >=70 && randA < 100 && 0 == type)
                {
                    if(randB < curProficient)
                        curProficient -= randB;
                    else
                        curProficient = 0;
                }
                break;
            case TUZHI_YELLOW:
                randB += 1;
                if(randA < 50 || 1 == type)
                {
                    curProficient += randB;
                }
                else if(randA >= 50 && randA < 100 && 0 == type)
                {
                    if(randB < curProficient)
                        curProficient -= randB;
                    else
                        curProficient = 0;
                }
                break;
        }

        if(curProficient > 100)
            curProficient = 100;

        iter->second = curProficient;

        m_jiguanshu.curExp += 5;

        DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), tuzhiId,  curProficient);
    }

    upgradeJGS();

    m_owner->udpLog("feigong", "F_130817_6", "", "", "", "", "act");

    UInt32 sendExp = 0;
    if(m_jiguanshu.curExp > JGS_MAXEXP)
        sendExp = JGS_MAXEXP;
    else
        sendExp = m_jiguanshu.curExp;

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << tuzhiId << type << result << static_cast<UInt32>(sendExp);
    if(0 == result)
        st << static_cast<UInt8>(curProficient); 
    st << Stream::eos;
    m_owner->send(st);
    m_owner->GuangGunCompleteTask(0,21);

}

void MoFang::upgradeJGS()
{
    if(!m_owner)
        return;

    if(m_jiguanshu.curLvl >= JGS_MAXLVL)
    {        
        DB4().PushUpdateData("REPLACE INTO `player_jiguanshu` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), m_jiguanshu.curLvl, m_jiguanshu.curExp);
    }
    else
    {
        //UInt32 exp = 0;
        GData::JiguanData::jiguanshuInfo * jgsInfo = NULL;
        if(m_jiguanshu.curExp > JGS_MAXEXP)
        {
            //exp = JGS_MAXEXP;
            jgsInfo = GData::jiguanData.getJiguanshuInfo(JGS_MAXLVL);
        }
        else
        {
            //exp = m_jiguanshu.curExp;
            jgsInfo = GData::jiguanData.getUpgradeInfo(m_jiguanshu.curExp);
        }

        if(NULL==jgsInfo)
            return;
        
        if(m_jiguanshu.curExp == jgsInfo->totalExp)
        {
            m_jiguanshu.curLvl = jgsInfo->jgshuLvl + 1;
        }
        else
            m_jiguanshu.curLvl = jgsInfo->jgshuLvl;

        DB4().PushUpdateData("REPLACE INTO `player_jiguanshu` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), m_jiguanshu.curLvl, m_jiguanshu.curExp);

        UInt8 temp[20][2] = {{10, 11}, {12, 13}, {20, 27}, {34, 41}, {40, 39}, 
                    {38, 37}, {30, 23}, {16, 9}, {2, 3}, {4, 5},
                    {6, 7}, {14, 21}, {28, 35}, {42, 49}, {48, 47},
                    {46, 45}, {44, 43}, {36, 29}, {22, 15}, {8, 1}};

        UInt8 lvlMark = m_jiguanshu.curLvl;
        UInt8 index = 0;

        for(UInt8 i=0; i<lvlMark; i++)
        {
            for(UInt8 k=0; k<2; k++)
            {
               index = temp[i][k]; 
               if(index > 0 && index <= EQUIP_GRID_MAX)
               {
                   for(UInt8 j=0; j<EQUIP_SCHEME_MAX; j++)
                   {
                       if(m_grids[j][index-1] == -1)
                           m_grids[j][index-1] = 0;
                   }
               }
            }
        }

        std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
        for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
        {
            it->second->setDirty();
        }
    }
}

void MoFang::equipJG(UInt32 jgId, UInt8 pos, UInt8 mark)
{
    if(!m_owner)
        return;

    if((EQUIP_JG == mark) && m_owner->GetLev() < 70)
        return;

    UInt8 scheme = m_owner->GetVar(VAR_JIGUAN_SCHEME);

    if(EQUIP_JG == mark || DISMANT_JG == mark)
    {
        std::vector<UInt8> values;
        bool res = checkPoint(jgId, pos, mark, scheme, values);
        if(res)
        {
            Stream st(REP::MOFANG_INFO);
            st << mark;

            UInt8 posMark = 0;
            UInt8 occupyMark = 0;
            if(EQUIP_JG == mark)
            {
                switch(scheme)
                {
                    case 1:
                        m_equipJGA.insert(std::make_pair(jgId, pos+1));
                        break;
                    case 2:
                        m_equipJGB.insert(std::make_pair(jgId, pos+1));
                        break;
                    case 3:
                        m_equipJGC.insert(std::make_pair(jgId, pos+1));
                        break;
                }

                std::vector<UInt32>::iterator iter = m_jg.begin();
                for(; iter != m_jg.end(); iter++)
                {
                    if(jgId == *iter)
                    {
                        m_jg.erase(iter);
                        DB4().PushUpdateData("DELETE FROM `player_jiguanyu` WHERE `jiguanId` = %u AND `scheme` = %u AND `playerId` = %" I64_FMT "u", jgId, 0, m_owner->getId());
                        break;
                    }
                }

                occupyMark = 1;
                posMark = pos + 1;
                DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), jgId, posMark, scheme);
            }
            else
            {
                switch(scheme)
                {
                    case 1:
                        {
                            std::map<UInt32, UInt8>::iterator iter = m_equipJGA.find(jgId);
                            if (iter != m_equipJGA.end())
                            {
                                //m_jg.push_back(jgId);
                                m_equipJGA.erase(iter);
                            }
                        }
                        break;
                    case 2:
                        {
                            std::map<UInt32, UInt8>::iterator iter = m_equipJGB.find(jgId);
                            if (iter != m_equipJGB.end())
                            {
                                //m_jg.push_back(jgId);
                                m_equipJGB.erase(iter);
                            }
                        }
                        break;
                    case 3:
                        {
                            std::map<UInt32, UInt8>::iterator iter = m_equipJGC.find(jgId);
                            if (iter != m_equipJGC.end())
                            {
                                //m_jg.push_back(jgId);
                                m_equipJGC.erase(iter);
                            }
                        }
                        break;
                }
            }

            for(UInt8 i=0; i<values.size(); i++)
            {
                m_grids[scheme-1][values[i]] = occupyMark;
            }

            if(DISMANT_JG == mark)
            {
                DB4().PushUpdateData("DELETE FROM `player_jiguanyu` WHERE `jiguanId` = %u AND `scheme` = %u AND `playerId` = %" I64_FMT "u", jgId, scheme, m_owner->getId());
                if(!findTotalJG(jgId))
                {
                    m_jg.push_back(jgId);
                    DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), jgId, 0, 0);
                }
            }

            st << jgId << pos;
            st << Stream::eos;
            m_owner->send(st);

            std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
            for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
            {
                it->second->setDirty();
            }
        }
    }
}

void MoFang::selectSchmem(UInt8 scheme)
{
    if(!m_owner)
        return;

    if(0==scheme || scheme>EQUIP_SCHEME_MAX)
        return;

    m_owner->SetVar(VAR_JIGUAN_SCHEME, scheme);

    std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
    for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
    {
        it->second->setDirty();
    }
}

void MoFang::addJGYAttrFromSchmem(GData::AttrExtra& ae)
{
    UInt8 scheme = m_owner->GetVar(VAR_JIGUAN_SCHEME);
    switch(scheme)
    {
        case 1:
            {
                std::map<UInt32, UInt8>::iterator iterA = m_equipJGA.begin();
                for(; iterA != m_equipJGA.end(); iterA++)
                {
                    addJGYAttr(ae, iterA->first);
                }
            }
            break;
        case 2:
            {
                std::map<UInt32, UInt8>::iterator iterB = m_equipJGB.begin();
                for(; iterB != m_equipJGB.end(); iterB++)
                {
                    addJGYAttr(ae, iterB->first);
                }
            }
            break;
        case 3:
            {
                std::map<UInt32, UInt8>::iterator iterC = m_equipJGC.begin();
                for(; iterC != m_equipJGC.end(); iterC++)
                {
                    addJGYAttr(ae, iterC->first);
                }
            }
            break;
    }
}

void MoFang::addJGYAttr(GData::AttrExtra& ae, UInt32 jgyId)
{

    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(jgyId);
    if(!jgyInfo)
        return;

    if(JIGUANQI == jgyInfo->mark)
        return;

    float addAttr = 0.00;
    if((m_jiguanshu.curLvl>0) && (m_jiguanshu.curLvl<=JGS_MAXLVL))
    {
        GData::JiguanData::jiguanshuInfo* jgsInfo = GData::jiguanData.getJiguanshuInfo(m_jiguanshu.curLvl);
        if(jgsInfo)
        {
            switch(jgyInfo->element)
            {
                case WIND_ELEMENT:
                    addAttr = jgsInfo->attrValue1;
                    break;
                case FOREST_ELEMENT:
                    addAttr = jgsInfo->attrValue2;
                    break;
                case FIRE_ELEMENT:
                    addAttr = jgsInfo->attrValue3;
                    break;
                case MOUNTAIN_ELEMENT:
                    addAttr = jgsInfo->attrValue4;
                    break;
            }
        }
    }

    switch(jgyInfo->attrType)
    {
        case 1:
            ae.attack += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 2:
            ae.magatk += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 3:
            ae.defend += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 4:
            ae.magdef += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 5:
            ae.hp += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 6:
            ae.toughlvl += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 7:
            ae.action += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 8:
            ae.hitrlvl += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 9:
            ae.evdlvl += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 10:
            ae.crilvl += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 11:
            ae.pirlvl += jgyInfo->attrValue * (1 + addAttr);
            break;
        case 12:
            ae.counterlvl += jgyInfo->attrValue * (1 + addAttr);
            break;
    }
}

/*void MoFang::useJGQskill()
{
    switch(scheme)
    {
        case 1:
            {
                std::map<UInt32, UInt8>::iterator iterA = m_equipJGA.begin();
                for(; iterA != m_equipJGA.end(); iterA++)
                {
                    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(iterA->first);
                    if(!jgyInfo)
                        continue;
                }
            }
            break;
        case 2:
            {
                std::map<UInt32, UInt8>::iterator iterB = m_equipJGB.begin();
                for(; iterB != m_equipJGB.end(); iterB++)
                {
                    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(iterB->first);
                    if(!jgyInfo)
                        continue;
                }
            }
            break;
        case 3:
            {
                std::map<UInt32, UInt8>::iterator iterC = m_equipJGC.begin();
                for(; iterC != m_equipJGC.end(); iterC++)
                {
                    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(iterC->first);
                    if(!jgyInfo)
                        continue;
                }
            }
            break;
    }
    return;
}*/

void MoFang::sendMoFangInfo(UInt8 mark)
{
    if(!m_owner)
        return;

    UInt32 sendExp = 0;
    if(m_jiguanshu.curExp > JGS_MAXEXP)
        sendExp = JGS_MAXEXP;
    else
        sendExp = m_jiguanshu.curExp;

    UInt32 moneyA = m_owner->GetVar(VAR_KEYIN_MONEY_A);
    UInt32 moneyB = m_owner->GetVar(VAR_KEYIN_MONEY_B);
    UInt8 scheme = m_owner->GetVar(VAR_JIGUAN_SCHEME);
    if(0 == scheme)
    {
        scheme = 1;
        m_owner->SetVar(VAR_JIGUAN_SCHEME, scheme);
    }

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << static_cast<UInt32>(sendExp) << moneyA << moneyB << static_cast<UInt8>(scheme);

    UInt16 countA = 0;
    UInt16 countB = 0;
    UInt16 countC = 0;
    UInt16 countD = 0;
    UInt8 countE = 0;
    UInt16 offsetA = 0;
    UInt16 offsetB = 0;
    UInt16 offsetC = 0;
    UInt16 offsetD = 0;
    UInt16 offsetE = 0;

    offsetA = st.size();
    st << countA;
    std::map<UInt32, UInt8>::iterator iterA = m_equipJGA.begin();
    for(; iterA != m_equipJGA.end(); iterA++)
    {
        st << iterA->first << static_cast<UInt8>(iterA->second - 1);
        countA++;    
    }

    offsetB = st.size();
    st << countB;
    std::map<UInt32, UInt8>::iterator iterB = m_equipJGB.begin();
    for(; iterB != m_equipJGB.end(); iterB++)
    {
        st << iterB->first << static_cast<UInt8>(iterB->second - 1);
        countB++;    
    }

    offsetC = st.size();
    st << countC;
    std::map<UInt32, UInt8>::iterator iterC = m_equipJGC.begin();
    for(; iterC != m_equipJGC.end(); iterC++)
    {
        st << iterC->first << static_cast<UInt8>(iterC->second - 1);
        countC++;    
    }

    offsetD = st.size();
    st << countD;
    std::vector<UInt32>::iterator iterD = m_jg.begin();
    for(; iterD != m_jg.end(); iterD++)
    {
        st << static_cast<UInt32>(*iterD);
        countD++;
    }

    std::map<UInt32, UInt8>::iterator iterDD = m_tuzhi.begin();
    for(; iterDD != m_tuzhi.end(); iterDD++)
    {
        st << iterDD->first << iterDD->second;
        countD++;    
    }

    offsetE = st.size();
    st << countE;
    std::map<UInt8, UInt32>::iterator iterE = m_kyCurExp.begin();
    for(; iterE != m_kyCurExp.end(); iterE++)
    {
        st << iterE->first << iterE->second;
        countE++;
    }

    zhenweiInfo(st);
    
    st.data<UInt16>(offsetA) = countA;
    st.data<UInt16>(offsetB) = countB;
    st.data<UInt16>(offsetC) = countC;
    st.data<UInt16>(offsetD) = countD;
    st.data<UInt8>(offsetE) = countE;

    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::zhenweiInfo(Stream& st)
{
    UInt16 keyCount = GData::jiguanData.getJGMYCount(); //震位拥有的机关密钥总个数
    UInt16 count = 0;
    if(0 == keyCount % 8)
        count = keyCount / 8;
    else
        count = keyCount / 8 + 1;

    UInt8 temp[count];
    memset(temp, 0, sizeof(temp));
    std::map<UInt16, UInt8>::iterator iter = m_zhenwei.begin(); //已经使用过的密钥
    for(; iter != m_zhenwei.end(); iter++)
    {
        UInt16 index = 0;
        UInt8 mark = 0;
        if(0 == (iter->first) % 8)
        {
            index = ((iter->first) / 8) - 1;
            mark = 7;
        }
        else
        {
            index = ((iter->first) / 8);
            mark = ((iter->first) % 8) - 1;
        }
        temp[index] = SET_BIT(temp[index], mark);
    }

    st << keyCount;
    for(UInt16 i=0; i<count; i++)
    {
        st << temp[i];
    }
}

bool MoFang::checkPoint(UInt32 jgId, UInt8 pos, UInt8 mark, UInt8 scheme, std::vector<UInt8> & values)
{
    int i = 0;
    UInt8 k = 0;
    UInt8 markA = 0;
    UInt8 markB = 0;
    bool markC = false;
    /*bool findResA = false;
    
    if(EQUIP_JG == mark)
        findResA = findEquipJG(jgId);
    else
        findResA = specialFindEquipJG(jgId);*/

    bool findResA = specialFindEquipJG(jgId);
    //bool findResB = findNoEquipJG(jgId);

    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(jgId);
    if(!jgyInfo)
        return false;

    if(EQUIP_JG == mark)
    {
        if(JIGUANQI == jgyInfo->mark)
            return false;

        if(findResA)
            return false;

        /*if(!findResB)
            return false;*/

        bool findResB = findTotalJG(jgId);
        if(!findResB)
            return false;

        markA = OCCUPY_MOFANG;
        markB = NOOCCUPY_MOFANG;
    }
    else
    {
        if(!findResA)
            return false;

        /*if(findResB)
            return false;*/

        markA = NOOCCUPY_MOFANG;
        markB = OCCUPY_MOFANG;
    }

    for(; i<3; i++)
    {
       if(k >= 9)
           return false;

       if(((pos+i) < 0 || (pos+i) >= 49) && (0 != jgyInfo->molding[k]))
            return false;

        if(0 != jgyInfo->molding[k])
        {
            if(markA == m_grids[scheme-1][pos+i] || -1 == m_grids[scheme-1][pos+i])
            {
                return false;
            }
            else if(markB == m_grids[scheme-1][pos+i])
            {
                values.push_back(pos+i);

                if(8 == k)
                    break;

                if(i == 2)
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

            if(i == 2)
            {
                markC = true;
                pos = pos + 7;
            }

            k++;
        }

        if(markC)
        {
            i = -1;
            markC = false;
        }
    }

    return true;
}

bool MoFang::findEquipJG(UInt32 jgId)
{
    bool markA = false;
    bool markB = false;
    bool markC = false;
    std::map<UInt32, UInt8>::iterator iterA = m_equipJGA.find(jgId);
    if(iterA != m_equipJGA.end())
        markA = true;

    std::map<UInt32, UInt8>::iterator iterB = m_equipJGB.find(jgId);
    if(iterB != m_equipJGB.end())
        markB = true;

    std::map<UInt32, UInt8>::iterator iterC = m_equipJGC.find(jgId);
    if(iterC != m_equipJGC.end())
        markC = true;

    if(!markA && !markB && !markC)
        return false;
    else
        return true;
}

bool MoFang::specialFindEquipJG(UInt32 jgId)
{
    UInt8 scheme = m_owner->GetVar(VAR_JIGUAN_SCHEME);
    if(0==scheme || scheme>EQUIP_SCHEME_MAX)
        return false;

    switch(scheme)
    {
        case 1:
            {
                std::map<UInt32, UInt8>::iterator iterA = m_equipJGA.find(jgId);
                if(iterA == m_equipJGA.end())
                    return false;
            }
            break;
        case 2:
            {
                std::map<UInt32, UInt8>::iterator iterB = m_equipJGB.find(jgId);
                if(iterB == m_equipJGB.end())
                    return false;
            }
            break;
        case 3:
            {
                std::map<UInt32, UInt8>::iterator iterC = m_equipJGC.find(jgId);
                if(iterC == m_equipJGC.end())
                    return false;
            }
            break;
    }

    return true;
}

bool MoFang::findTotalJG(UInt32 jgId)
{
    bool markA = false;
    bool markB = false;
    bool markC = false;
    bool markD = false;
    std::map<UInt32, UInt8>::iterator iterA = m_equipJGA.find(jgId);
    if(iterA != m_equipJGA.end())
        markA = true;

    std::map<UInt32, UInt8>::iterator iterB = m_equipJGB.find(jgId);
    if(iterB != m_equipJGB.end())
        markB = true;

    std::map<UInt32, UInt8>::iterator iterC = m_equipJGC.find(jgId);
    if(iterC != m_equipJGC.end())
        markC = true;

    if(findNoEquipJG(jgId))
        markD = true;

    if(!markA && !markB && !markC && !markD)
        return false;
    else
        return true;
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

bool MoFang::findKey(UInt16 keyId)
{
    std::map<UInt16, UInt8>::iterator iter = m_zhenwei.find(keyId);
    if(iter == m_zhenwei.end())
        return false;

    return true;
}

void MoFang::quickMakejiguan(UInt32 tuzhiId, UInt8 mark)
{
    if(!m_owner)
        return;

    std::map<UInt32, UInt8>::iterator iter = m_tuzhi.find(tuzhiId);
    if(iter == m_tuzhi.end())
        return;

    UInt32 toolId = 9414;

    UInt16 toolCount = m_owner->GetPackage()->GetItemAnyNum(toolId);
    if(0 == toolCount)
        return;

    GData::JiguanData::tuzhiInfo * tzInfo = GData::jiguanData.getTuzhiInfo(tuzhiId);
    if(!tzInfo)
        return;

    UInt8 needJgsLvl = tzInfo->needjgsLvl;
    if(needJgsLvl > m_jiguanshu.curLvl)
        return;

    UInt8 tzQuality = tzInfo->quality; 
    UInt8 curProficient = iter->second;
    UInt32 addExp = 0;
    UInt32 randValue = 0;
    UInt32 count = 0;
    float successRate = 0;

    for(UInt16 i=0; i<toolCount; i++)
    {
        successRate = 0;

        switch(tzQuality)
        {
            case TUZHI_GREEN:
                successRate = 1;
                if(curProficient<=25)
                    successRate += curProficient;
                else if(curProficient<=50)
                {
                    successRate += 25;
                    successRate += (curProficient - 25) * 3;
                }
                else if(curProficient<=75)
                {
                    successRate += 25 * 4;
                    successRate += (curProficient - 50) * 3;
                }
                else if(curProficient<=100)
                {
                    successRate += 25 * 7;
                    successRate += (curProficient - 75) * 3;
                }
                m_owner->udpLog("feigong", "F_130817_7", "", "", "", "", "act");
                break;
            case TUZHI_BLUE:
                successRate = 0.5;
                if(curProficient<=25)
                    successRate += curProficient * 0.02;
                else if(curProficient<=50)
                {
                    successRate += 25 * 0.02;
                    successRate += (curProficient - 25) * 2;
                }
                else if(curProficient<=75)
                {
                    successRate += 25 * 2.02;
                    successRate += (curProficient - 50) * 3;
                }
                else if(curProficient<=100)
                {
                    successRate += 25 * 5.02;
                    successRate += (curProficient - 75) * 3;
                }
                m_owner->udpLog("feigong", "F_130817_8", "", "", "", "", "act");
                break;
            case TUZHI_PURPLE:
                successRate = 0.2;
                if(curProficient<=25)
                    successRate += curProficient * 0.01;
                else if(curProficient<=50)
                {
                    successRate += 25 * 0.01;
                    successRate += (curProficient - 25) * 0.02;
                }
                else if(curProficient<=75)
                {
                    successRate += 25 * 0.03;
                    successRate += (curProficient - 50) * 2;
                }
                else if(curProficient<=100)
                {
                    successRate += 25 * 2.03;
                    successRate += (curProficient - 75) * 3;
                }
                m_owner->udpLog("feigong", "F_130817_9", "", "", "", "", "act");
                break;
            case TUZHI_YELLOW:
                successRate = 0.1;
                if(curProficient<=25)
                    successRate += curProficient * 0.001;
                else if(curProficient<=50)
                {
                    successRate += 25 * 0.001;
                    successRate += (curProficient - 25) * 0.005;
                }
                else if(curProficient<=75)
                {
                    successRate += 25 * 0.006;
                    successRate += (curProficient - 50) * 0.01;
                }
                else if(curProficient<=100)
                {
                    successRate += 25 * 0.016;
                    successRate += (curProficient - 75) * 2.5;
                }
                m_owner->udpLog("feigong", "F_130817_10", "", "", "", "", "act");
                break;
        }

        count++;
        randValue = uRand(100000);
        
        if(curProficient >= 100 || successRate*1000 >= randValue)
            break;
        else
        {
            addExp += 5;
            //UInt8 randA = uRand(100);
            UInt8 randB = uRand(3);
            switch(tzQuality)
            {
                case TUZHI_GREEN:
                        curProficient += randB + 4;
                    break; 
                case TUZHI_BLUE:
                    randB += 3;
                    //if(randA < 85)
                    {
                        curProficient += randB;
                    }
                    break;
                case TUZHI_PURPLE:
                    randB += 2;
                    //if(randA < 70)
                    {
                        curProficient += randB;
                    }
                    break;
                case TUZHI_YELLOW:
                    randB += 1;
                    //if(randA < 50)                 
                    {
                        curProficient += randB;
                    }
                    break;
            }

        }
    }

    if(!m_owner->GetPackage()->DelItemAny(toolId, count, NULL, ToMakeJGY))
        return;

    UInt8 result = 0;
    if(curProficient >= 100 || randValue < successRate*1000)
    {
        switch(tzQuality)
        {
            case TUZHI_GREEN:
                addExp += 20;
                m_owner->udpLog("feigong", "F_130817_12", "", "", "", "", "act");
                break;
            case TUZHI_BLUE:
                addExp += 50;
                m_owner->udpLog("feigong", "F_130817_13", "", "", "", "", "act");
                break;
            case TUZHI_PURPLE:
                addExp += 400;
                m_owner->udpLog("feigong", "F_130817_14", "", "", "", "", "act");
                break;
            case TUZHI_YELLOW:
                addExp += 2250;
                m_owner->udpLog("feigong", "F_130817_15", "", "", "", "", "act");
                
                GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(tzInfo->jiguanyuId);
                if(!jgyInfo)
                    return;

                SYSMSG_BROADCASTV(4937, m_owner->getCountry(), m_owner->getName().c_str(), jgyInfo->quality, (jgyInfo->name).c_str());
                break;
        }

        m_owner->udpLog("feigong", "F_130817_11", "", "", "", "", "act");

        m_jiguanshu.curExp += addExp;

        m_jg.push_back(tzInfo->jiguanyuId);

        DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), tzInfo->jiguanyuId, 0, 0);

        DB4().PushUpdateData("DELETE FROM `player_tuzhi` WHERE `tuzhiId` = %u AND `playerId` = %" I64_FMT "u", tuzhiId, m_owner->getId());

        DBLOG1().PushUpdateData("insert into make_jiguanyu (server_id, player_id, jiguanyu_id, jiguanyu_name, jiguanyu_quality, make_time) values(%u,%" I64_FMT "u,%u,'%s',%u,%u)",
                    cfg.serverLogId, m_owner->getId(), tzInfo->jiguanyuId, tzInfo->name.c_str(), tzInfo->quality, TimeUtil::Now());

        m_tuzhi.erase(iter);
        result = 1;
    }
    else
    {
        if(curProficient > 100)
            curProficient = 100;

        iter->second = curProficient;

        m_jiguanshu.curExp += addExp;

        DB4().PushUpdateData("REPLACE INTO `player_tuzhi` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), tuzhiId,  curProficient);
    }

    upgradeJGS();

    m_owner->udpLog("feigong", "F_130817_6", "", "", "", "", "act");

    UInt32 sendExp = 0;
    if(m_jiguanshu.curExp > JGS_MAXEXP)
        sendExp = JGS_MAXEXP;
    else
        sendExp = m_jiguanshu.curExp;

    Stream st(REP::MOFANG_INFO);
    st << mark;
    st << tuzhiId << result << static_cast<UInt32>(sendExp) << static_cast<UInt16>(count);
    if(0 == result)
        st << static_cast<UInt8>(curProficient); 
    st << Stream::eos;
    m_owner->send(st);
    m_owner->GuangGunCompleteTask(0,21);
}

void MoFang::upgradeKY(UInt8 keyinId, UInt8 opt)
{
    if(!m_owner)
        return;
     
    UInt8 keyinLvl = 0;
    UInt32 keyinCurExp = 0;
    bool mark = false;

    std::map<UInt8, UInt8>::iterator iter = m_kyLvl.find(keyinId);
    if(iter == m_kyLvl.end())
        mark = true;
    else
        keyinLvl = iter->second;

    if(keyinLvl >= 20)
        return;
    
    GData::JiguanData::keyinInfo * kyInfo = GData::jiguanData.getKeyinInfo(keyinId, keyinLvl+1);
    if(!kyInfo)
        return;

    if(5 == keyinId) //特殊的拂晓刻印升级，需要其他4个刻印的品质达到一定的条件
    {
        for(UInt8 i=0; i<4; i++)
        {
            if(kyInfo->quality > m_kyQuality[i])
                return;
        }
    }

    std::map<UInt8, UInt32>::iterator iterA = m_kyCurExp.find(keyinId);
    if(iterA != m_kyCurExp.end())
        keyinCurExp = iterA->second;
    
    UInt32 money = 0;
    UInt32 consumeValue = 0;

    if(kyInfo->materialA == 0)
    {
        money = m_owner->GetVar(VAR_KEYIN_MONEY_B);
        consumeValue = kyInfo->materialB;
        if(money < consumeValue)
            return;

        m_owner->SetVar(VAR_KEYIN_MONEY_B, money-consumeValue);
    }
    else
    {
        money = m_owner->GetVar(VAR_KEYIN_MONEY_A);
        consumeValue = kyInfo->materialA;
        if(money < consumeValue)
            return;

        m_owner->SetVar(VAR_KEYIN_MONEY_A, money-consumeValue);
    }

    UInt8 addKYExp = uRand(9) + 1;
    keyinCurExp += addKYExp;

    if(keyinCurExp >= kyInfo->maxValue)
    {
        keyinLvl++;
        /*if(mark)
            m_kyLvl.insert(std::make_pair(keyinId, keyinLvl));
        else
            iter->second = keyinLvl;*/

        if(keyinId >= 1 && keyinId <= 4)
            m_kyQuality[keyinId-1] = kyInfo->quality;

        std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
        for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
        {
            it->second->setDirty();
        }
    }

    if(mark)
    {
        m_kyLvl.insert(std::make_pair(keyinId, keyinLvl));
        m_kyCurExp.insert(std::make_pair(keyinId, keyinCurExp));
    }
    else
    {
        iter->second = keyinLvl;
        iterA->second = keyinCurExp;
    }

    DB4().PushUpdateData("REPLACE INTO `player_keyin` VALUES(%" I64_FMT "u, %u, %u, %u)", m_owner->getId(), keyinId, keyinLvl, keyinCurExp);

    Stream st(REP::MOFANG_INFO);
    st << opt;
    st << keyinId << addKYExp;
    st << Stream::eos;
    m_owner->send(st);
    m_owner->GuangGunCompleteTask(0,22);
}

void MoFang::quickUpgradeKY(UInt8 keyinId, UInt8 opt)
{
    if(!m_owner)
        return;

    UInt8 keyinLvl = 0;
    UInt32 keyinCurExp = 0;
    bool mark = false;

    std::map<UInt8, UInt8>::iterator iter = m_kyLvl.find(keyinId);
    if(iter == m_kyLvl.end())
        mark = true;
    else
        keyinLvl = iter->second;

    if(keyinLvl >= 20)
        return;
    
    GData::JiguanData::keyinInfo * kyInfo = GData::jiguanData.getKeyinInfo(keyinId, keyinLvl+1);
    if(!kyInfo)
        return;

    if(5 == keyinId) //特殊的拂晓刻印升级，需要其他4个刻印的品质达到一定的条件
    {
        for(UInt8 i=0; i<4; i++)
        {
            if(kyInfo->quality > m_kyQuality[i])
                return;
        }
    }

    std::map<UInt8, UInt32>::iterator iterA = m_kyCurExp.find(keyinId);
    if(iterA != m_kyCurExp.end())
        keyinCurExp = iterA->second;

    UInt32 money = 0;
    if(kyInfo->materialA == 0)
    {
        money = m_owner->GetVar(VAR_KEYIN_MONEY_B);
        if(money < kyInfo->materialB)
            return;
    }
    else
    {
        money = m_owner->GetVar(VAR_KEYIN_MONEY_A);
        if(money < kyInfo->materialA)
            return;
    }

    UInt32 consumeValue = 0;
    UInt32 addTotalExp = 0;
    UInt16 count = 0;
    for( ; keyinCurExp < kyInfo->maxValue; )
    {
        if(kyInfo->materialA == 0)
        {
            consumeValue += kyInfo->materialB;
            if(money < consumeValue)
            {
                consumeValue -= kyInfo->materialB;
                break;
            }
        }
        else
        {
            consumeValue += kyInfo->materialA;
            if(money < consumeValue)
            {
                consumeValue -= kyInfo->materialA;
                break;
            }
        }

        UInt8 addKYExp = uRand(9) + 1;
        keyinCurExp += addKYExp;
        addTotalExp += addKYExp;
        count++;
        m_owner->GuangGunCompleteTask(0,22);
    }
    
    if(money >= consumeValue)
    {
        if(kyInfo->materialA == 0)
            m_owner->SetVar(VAR_KEYIN_MONEY_B, money-consumeValue);
        else
            m_owner->SetVar(VAR_KEYIN_MONEY_A, money-consumeValue);
    }

    if(keyinCurExp >= kyInfo->maxValue)
    {
        keyinLvl++;
        
        /*if(mark)
            m_kyLvl.insert(std::make_pair(keyinId, keyinLvl));
        else
            iter->second = keyinLvl;*/

        if(keyinId >= 1 && keyinId <= 4)
            m_kyQuality[keyinId-1] = kyInfo->quality;

        std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
        for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
        {
            it->second->setDirty();
        }
    }

    if(mark)
    {
        m_kyLvl.insert(std::make_pair(keyinId, keyinLvl));
        m_kyCurExp.insert(std::make_pair(keyinId, keyinCurExp));
    }
    else
    {
        iter->second = keyinLvl;
        iterA->second = keyinCurExp;
    }

    DB4().PushUpdateData("REPLACE INTO `player_keyin` VALUES(%" I64_FMT "u, %u, %u, %u)", m_owner->getId(), keyinId, keyinLvl, keyinCurExp);

    Stream st(REP::MOFANG_INFO);
    st << opt;
    st << keyinId << count << addTotalExp;
    st << Stream::eos;
    m_owner->send(st);
       
}
      
void MoFang::changeMoney(UInt16 num, UInt8 mark)
{
    if(!m_owner)
        return;

    if(0 == num)
        return;

    UInt8 res = 0;
    UInt32 money = m_owner->GetVar(VAR_KEYIN_MONEY_A);
    if(money >= num*100)
    {
        m_owner->SetVar(VAR_KEYIN_MONEY_A, (money-num*100));
        m_owner->AddVar(VAR_KEYIN_MONEY_B, (10*num));

        res = 1;
    }

    Stream st(REP::MOFANG_INFO);
    st << mark << res << num;
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::addMoney(UInt32 toolId, UInt32 count)
{
    if(!m_owner)
        return;

    UInt32 money = 0;
    UInt8 mark = 0;
    if(9424 == toolId)
    {
        money = m_owner->GetVar(VAR_KEYIN_MONEY_A);
        m_owner->AddVar(VAR_KEYIN_MONEY_A, count*100);
        mark = 1;
		SYSMSG_SENDV(4941, m_owner, count*100);
		SYSMSG_SENDV(4942, m_owner, count*100);
    }
    else
    {
        money = m_owner->GetVar(VAR_KEYIN_MONEY_B);
        m_owner->AddVar(VAR_KEYIN_MONEY_B, count*100);
        mark = 2;
		SYSMSG_SENDV(4943, m_owner, count*100);
		SYSMSG_SENDV(4944, m_owner, count*100);
    }

    money += count*100;

    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(11) << mark << static_cast<UInt32>(money);
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::addKYAttr(GData::AttrExtra& ae)
{
    if(!m_owner)
        return;

    std::map<UInt8, UInt8>::iterator iter = m_kyLvl.begin();
    for(; iter != m_kyLvl.end(); iter++)
    {
        GData::JiguanData::keyinInfo * kyInfo = GData::jiguanData.getKeyinInfo(iter->first, iter->second);
        if(!kyInfo)
            continue;

        if(kyInfo->keyinLvl > 0 && kyInfo->keyinLvl <= 20)
        {
            switch(kyInfo->attrType)
            {
                case 1:
                    ae.hp += kyInfo->attrValue;
                    break;
                case 2:
                    ae.attack += kyInfo->attrValue;
                    break;
                case 3:
                    ae.magatk += kyInfo->attrValue;
                    break;
                case 4:
                    ae.action += kyInfo->attrValue;
                    break;
                case 5:
                    ae.criticaldmgimmune += (static_cast<float>(kyInfo->attrValue)) / 100.0f;
                    break;
            }
        }
    }
}

void MoFang::checkKey(UInt16 keyId, UInt8 opt)
{
    if(!m_owner)
        return;

    GData::JiguanData::zhenweiInfo * zwInfo = GData::jiguanData.getZhenweiInfo(keyId);
    if(!zwInfo)
        return;

    if(findKey(keyId))
        return;

    UInt32 jiguanyuId = 0; 
    switch(zwInfo->type)
    {
        case 1:
            //jiguanyuId = yizhiyue(keyId); // 暂不上
            break;
        case 2:
            jiguanyuId = renzhiyue(keyId);
            break;
    }

    if(jiguanyuId > 0)
    {
        GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(jiguanyuId);
        if(!jgyInfo)
            return;

        m_jg.push_back(jiguanyuId);
        m_zhenwei.insert(std::make_pair(keyId, 1));

        DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), jiguanyuId, 0, 0);
        DB4().PushUpdateData("REPLACE INTO `player_zhenwei` VALUES(%" I64_FMT "u, %u, %u)",m_owner->getId(), keyId, 1);
        DBLOG1().PushUpdateData("insert into make_jiguanyu (server_id, player_id, jiguanyu_id, jiguanyu_name, jiguanyu_quality, make_time) values(%u,%" I64_FMT "u,%u,'%s',%u,%u)",
                cfg.serverLogId, m_owner->getId(), jgyInfo->jiguanyuId, jgyInfo->name.c_str(), jgyInfo->quality, TimeUtil::Now());

        Stream st(REP::MOFANG_INFO);
        st << opt;
        st << keyId;
        st << Stream::eos;
        m_owner->send(st);
    }
}

UInt32 MoFang::yizhiyue(UInt16 keyId)
{
    GData::JiguanData::zhenweiInfo * zwInfo = GData::jiguanData.getZhenweiInfo(keyId);
    if(!zwInfo)
        return 0;

    std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
    std::map<UInt32, Fighter *>::iterator it1 = fighters.find(zwInfo->collect1);
    std::map<UInt32, Fighter *>::iterator it2 = fighters.find(zwInfo->collect2);
    std::map<UInt32, Fighter *>::iterator it3 = fighters.find(zwInfo->collect3);
    std::map<UInt32, Fighter *>::iterator it4 = fighters.find(zwInfo->collect4);
    std::map<UInt32, Fighter *>::iterator it5 = fighters.find(zwInfo->collect5);
    std::map<UInt32, Fighter *>::iterator it6 = fighters.find(zwInfo->collect6);
    std::map<UInt32, Fighter *>::iterator it7 = fighters.find(zwInfo->collect7);
    std::map<UInt32, Fighter *>::iterator it8 = fighters.find(zwInfo->collect8);

    if(it1 != fighters.end() 
            && it2 != fighters.end() 
            && it3 != fighters.end() 
            && it4 != fighters.end()
            && it5 != fighters.end() 
            && it6 != fighters.end() 
            && it7 != fighters.end()
            && it8 != fighters.end())
    {
        return zwInfo->award;    
    }
    else
    {
        return 0;
    }
}

UInt32 MoFang::renzhiyue(UInt16 keyId)
{
    GData::JiguanData::zhenweiInfo * zwInfo = GData::jiguanData.getZhenweiInfo(keyId);
    if(!zwInfo)
        return 0;
    
    if((findEquipJG(zwInfo->collect1) || findNoEquipJG(zwInfo->collect1))
                &&(findEquipJG(zwInfo->collect2) || findNoEquipJG(zwInfo->collect2))
                &&(findEquipJG(zwInfo->collect3) || findNoEquipJG(zwInfo->collect3))
                &&(findEquipJG(zwInfo->collect4) || findNoEquipJG(zwInfo->collect4))
                &&(findEquipJG(zwInfo->collect5) || findNoEquipJG(zwInfo->collect5))
                &&(findEquipJG(zwInfo->collect6) || findNoEquipJG(zwInfo->collect6))
                &&(findEquipJG(zwInfo->collect7) || findNoEquipJG(zwInfo->collect7))
                &&(findEquipJG(zwInfo->collect8) || findNoEquipJG(zwInfo->collect8)))
    {
        return zwInfo->award;    
    }
    else
    {
        return 0;
    }
}

void MoFang::addjiguanyu(UInt32 jiguanyuId)
{
    if(!m_owner)
        return;

    GData::JiguanData::jiguanyuInfo * jgyInfo = GData::jiguanData.getJiguanyuInfo(jiguanyuId);
    if(!jgyInfo)
        return;

    if(findEquipJG(jiguanyuId))
        return;

    if(findNoEquipJG(jiguanyuId))
        return;

    std::map<UInt32, UInt8>::iterator iter = m_tuzhi.find(jiguanyuId + NORMAL_TUZHI);
    if(iter != m_tuzhi.end())
        return;

    m_jg.push_back(jiguanyuId);

    DB4().PushUpdateData("REPLACE INTO `player_jiguanyu` VALUES(%" I64_FMT "u, %u, %u, %u)",m_owner->getId(), jiguanyuId, 0, 0);

    sendMoFangInfo();
}

void MoFang::sendCommonGearInfo()
{
    if(!m_owner)
        return;

    if(m_owner->GetLev() < 75)
        return;

    UInt16 sz = m_commonGear.size();
    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(13);
    sendZYValue(st);
    st << static_cast<UInt8>(1);
    st << sz;
    std::vector<UInt16>::iterator iter = m_commonGear.begin();
    for(; iter!=m_commonGear.end(); iter++)
    {
        st << *iter;
    }
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::sendSpecialGearInfo()
{
    if(!m_owner)
        return;

    if(m_owner->GetLev() < 75)
        return;

    UInt16 sz = m_specialGear.size();
    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(13);
    sendZYValue(st);
    st << static_cast<UInt8>(2);
    st << sz;
    std::vector<UInt16>::iterator iter = m_specialGear.begin();
    for(; iter!=m_specialGear.end(); iter++)
    {
        st << *iter;
    }
    st << Stream::eos;
    m_owner->send(st);
}

void MoFang::sendZYValue(Stream& st)
{
    if(!m_owner)
        return;

    st << m_owner->GetVar(VAR_ZIYUN_KUANG);
    st << m_owner->GetVar(VAR_ZIYUN_MU);
    st << m_owner->GetVar(VAR_ZIYUN_PAI);
    st << m_owner->GetVar(VAR_ZIYUN_LIANFU);
}

void MoFang::makeGear(UInt16 gearId, UInt8 mark)
{
    if(!m_owner)
        return;

    if(GEAR_COMMON != mark && GEAR_SPECIAL != mark)
        return;

    GData::JiguanData::gearInfo* gearInfo = GData::jiguanData.getGearInfo(gearId);
    if(!gearInfo)
        return;
   
    
    if(m_owner->getTael() < gearInfo->taelNum)
    {
        m_owner->sendMsgCode(0, 1100);
        return;
    }

    if(GEAR_COMMON == mark)
    {
        if(checkCommonGear(gearId))
            return;

        if(!makeCommonGear(gearId))
            return;
    }
    else
    {
        if(checkSpecialGear(gearId))
            return;

        if(!makeSpecialGear(gearId))
            return;
    }

    UInt32 zyk = m_owner->GetVar(VAR_ZIYUN_KUANG);
    UInt32 zym = m_owner->GetVar(VAR_ZIYUN_MU);
    UInt32 zys = m_owner->GetVar(VAR_ZIYUN_PAI);
    UInt32 zylf = m_owner->GetVar(VAR_ZIYUN_LIANFU);

    if(zyk<gearInfo->ZYKNum
            || zym<gearInfo->ZYMNum
            || zys<gearInfo->ZYSNum
            || zylf<gearInfo->ZYLFNum)
        return;

    m_owner->SetVar(VAR_ZIYUN_KUANG, (zyk - gearInfo->ZYKNum));
    m_owner->SetVar(VAR_ZIYUN_MU, (zym - gearInfo->ZYMNum));
    m_owner->SetVar(VAR_ZIYUN_PAI, (zys - gearInfo->ZYSNum));
    m_owner->SetVar(VAR_ZIYUN_LIANFU, (zylf - gearInfo->ZYLFNum));

    ConsumeInfo ci(MakeGear, 0, 0);
    m_owner->useTael(gearInfo->taelNum, &ci);

    if(GEAR_COMMON == mark)
        m_commonGear.push_back(gearId);
    else
        m_specialGear.push_back(gearId);
    DB4().PushUpdateData("REPLACE INTO `player_gear` VALUES(%" I64_FMT "u, %u, %u)", m_owner->getId(), gearId, mark);

    Stream st(REP::MOFANG_INFO);
    st << static_cast<UInt8>(14);
    sendZYValue(st);
    st << static_cast<UInt8>(mark);
    st << gearId;
    st << Stream::eos;
    m_owner->send(st);

    UInt32 addValue = gearInfo->attrValueG;
    //std::cout << "0000000 : " << gearId << std::endl;
    //std::cout << "AAAAAAA : " << addValue << std::endl;
    //std::cout << "BBBBBBB : " << m_owner->GetVar(VAR_GEAR_BUFF) << std::endl;
    m_owner->AddVar(VAR_GEAR_BUFF, addValue);
    //std::cout << "CCCCCCC : " << m_owner->GetVar(VAR_GEAR_BUFF) << std::endl;

    std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
    for(std::map<UInt32, Fighter *>::iterator it=fighters.begin(); it!=fighters.end(); ++it)
    {
        it->second->setDirty();
    }

    UInt8 gearType = GData::jiguanData.getGearType(gearId);
    if(1 == gearType)
        SYSMSG_BROADCASTV(5155, m_owner->getCountry(), m_owner->getName().c_str(), 5, (gearInfo->name).c_str());

    switch(gearId)
    {
        case JIGUANKU_A:
            m_owner->udpLog("jiguanku", "F_140627_21", "", "", "", "", "act");
            break;
        case JIGUANKU_B:
            m_owner->udpLog("jiguanku", "F_140627_22", "", "", "", "", "act");
            break;
        case JIGUANKU_C:
            m_owner->udpLog("jiguanku", "F_140627_23", "", "", "", "", "act");
            break;
        case JIGUANKU_D:
            m_owner->udpLog("jiguanku", "F_140627_24", "", "", "", "", "act");
            break;
        case JIGUANKU_E:
            m_owner->udpLog("jiguanku", "F_140627_25", "", "", "", "", "act");
            break;
        case JIGUANKU_F:
            m_owner->udpLog("jiguanku", "F_140627_26", "", "", "", "", "act");
            break;
        case JIGUANKU_G:
            m_owner->udpLog("jiguanku", "F_140627_27", "", "", "", "", "act");
            break;
        case JIGUANKU_H:
            m_owner->udpLog("jiguanku", "F_140627_28", "", "", "", "", "act");
            break;
        case JIGUANKU_I:
            m_owner->udpLog("jiguanku", "F_140627_29", "", "", "", "", "act");
            break;
        case JIGUANKU_J:
            m_owner->udpLog("jiguanku", "F_140627_30", "", "", "", "", "act");
            break;
        case JIGUANKU_K:
            m_owner->udpLog("jiguanku", "F_140627_31", "", "", "", "", "act");
            break;
    }
}

bool MoFang::makeCommonGear(UInt16 gearId)
{
    UInt8 gearType = GData::jiguanData.getGearType(gearId);
    if(0 == gearType)
        return false;

    switch(gearType)
    {
        case 1:
            {
                GData::JiguanData::needPart* needPartInfo = GData::jiguanData.getMakeSuiteInfo(gearId);
                if(!needPartInfo)
                    return false;

                if(needPartInfo->lastSuiteId > 0)
                    if(!checkCommonGear(needPartInfo->lastSuiteId))
                        return false;

                if(!checkCommonGear(needPartInfo->needPartAId)
                        || !checkCommonGear(needPartInfo->needPartBId)
                        || !checkCommonGear(needPartInfo->needPartCId))
                    return false;
            }
            break;
        case 2:
            {
                GData::JiguanData::needComponent* needComponentInfo = GData::jiguanData.getMakePartInfo(gearId);
                if(!needComponentInfo)
                    return false;

                if(needComponentInfo->lastSuiteId > 0)
                    if(!checkCommonGear(needComponentInfo->lastSuiteId))
                        return false;

                if(needComponentInfo->needComponentAId > 0)
                    if(!checkCommonGear(needComponentInfo->needComponentAId))
                        return false;

                if(needComponentInfo->needComponentBId > 0)
                    if(!checkCommonGear(needComponentInfo->needComponentBId))
                        return false;
            }
            break;
        case 3:
            {
                UInt16 condition = GData::jiguanData.getMakeComponentInfo(gearId);
                if(condition > 0)
                    if(!checkCommonGear(condition))
                        return false;
            }
            break;
    }

    return true;
}

bool MoFang::makeSpecialGear(UInt16 gearId)
{
    UInt8 gearType = GData::jiguanData.getGearType(gearId);
    if(0 == gearType)
        return false;

    switch(gearType)
    {
        case 1:
            {
                GData::JiguanData::needPart* needPartInfo = GData::jiguanData.getMakeSuiteInfo(gearId);
                if(!needPartInfo)
                    return false;

                if(!checkSpecialGear(needPartInfo->needPartAId)
                        || !checkSpecialGear(needPartInfo->needPartBId)
                        || !checkSpecialGear(needPartInfo->needPartCId))
                    return false;
            }
            break;
        case 2:
            {
                GData::JiguanData::needComponent* needComponentInfo = GData::jiguanData.getMakePartInfo(gearId);
                if(!needComponentInfo)
                    return false;

                if(needComponentInfo->needComponentAId > 0)
                    if(!checkSpecialGear(needComponentInfo->needComponentAId))
                        return false;

                if(needComponentInfo->needComponentBId > 0)
                    if(!checkSpecialGear(needComponentInfo->needComponentBId))
                        return false;
            }
            break;
    }

    return true;
}

bool MoFang::checkCommonGear(UInt16 gearId)
{
    std::vector<UInt16>::iterator iter = m_commonGear.begin();
    for(; iter!=m_commonGear.end(); iter++)
    {
        if(gearId == *iter)
            return true;
    }

    return false;
}

bool MoFang::checkSpecialGear(UInt16 gearId)
{
    std::vector<UInt16>::iterator iter = m_specialGear.begin();
    for(; iter!=m_specialGear.end(); iter++)
    {
        if(gearId == *iter)
            return true;
    }

    return false;
}

void MoFang::addGearAttr(GData::AttrExtra& ae)
{
    if(m_owner->GetLev() < 75)
        return;

    float addAttrA = 0.00;
    float addAttrB = 0.00;
    float addAttrC = 0.00;
    float addAttrD = 0.00;
    float addAttrE = 0.00;
    float addAttrF = 0.00;
    std::vector<UInt16>::iterator iter = m_commonGear.begin();
    for(; iter!=m_commonGear.end(); iter++)
    {
        GData::JiguanData::gearInfo* commonGearInfo = GData::jiguanData.getGearInfo(*iter);
        if(!commonGearInfo)
            continue;

        addAttrA += commonGearInfo->attrValueA;
        addAttrB += commonGearInfo->attrValueB;
        addAttrC += commonGearInfo->attrValueC;
        addAttrD += commonGearInfo->attrValueD;
        addAttrE += commonGearInfo->attrValueE;
        addAttrF += commonGearInfo->attrValueF;
    }

    std::vector<UInt16>::iterator iterA = m_specialGear.begin();
    for(; iterA!=m_specialGear.end(); iterA++)
    {
        GData::JiguanData::gearInfo* specialGearInfo = GData::jiguanData.getGearInfo(*iterA);
        if(!specialGearInfo)
            continue;

        addAttrA += specialGearInfo->attrValueA;
        addAttrB += specialGearInfo->attrValueB;
        addAttrC += specialGearInfo->attrValueC;
        addAttrD += specialGearInfo->attrValueD;
        addAttrE += specialGearInfo->attrValueE;
        addAttrF += specialGearInfo->attrValueF;
    }

    ae.attack += addAttrA;
    ae.magatk += addAttrB;
    ae.defend += addAttrC;
    ae.magdef += addAttrD;
    ae.hp += addAttrE;
    ae.action += addAttrF;
}

}
