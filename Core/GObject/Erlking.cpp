#include "Erlking.h"
#include "TownDeamon.h"
#include "Player.h"
#include "Item.h"
#include "MsgID.h"
#include "Package.h"
#include "GData/ErlkingTable.h"
#include "GObjectDBExecHelper.h"

namespace GObject
{

Erlking::Erlking(Player* pl)
{
    m_owner = pl;
}

void Erlking::ErlkingInfo()
{
    DeamonPlayerData* dpd = m_owner->getDeamonPlayerData();
    if(NULL == dpd)
        return;
    UInt16 level = dpd->maxLevel;

    for(UInt8 i=1; i<=ERLKING_MAX_COPY_NUM; i++)
    {
        GData::ErlkingData::erlkingInfo * info = GData::erlkingData.getErlkingInfo(i);
        if(NULL == info)
            break;

        UInt8 curMark = GetErlkingStatus(i);
        UInt8 lastMark = GetErlkingStatus(i-1);
        if(ERLKING_MARKA == curMark && (ERLKING_MARKC == lastMark || 1 == i) && level >= info->conditionB)
            SetErlkingStatus(i);
        else
            break;
    }
    SendErlkingInfo();
}

void Erlking::SendErlkingInfo()
{
    Stream st(REP::ERLKING_INFO);
    st << static_cast<UInt8>(0x00);  
    st << GetsurplusNum()  
        << static_cast<UInt8>(m_owner->GetVar(VAR_ERLKING_BUY_PASS_NUM_DAY))
        << m_owner->GetVar(VAR_ERLKING_STATUSA)
        << m_owner->GetVar(VAR_ERLKING_STATUSB)
        << m_owner->GetVar(VAR_ERLKING_STATUSC)
        << Stream::eos;
    m_owner->send(st);
}

UInt8 Erlking::GetErlkingStatus(UInt8 copyId)
{
    if(0 == copyId || copyId > ERLKING_MAX_COPY_NUM)
        return 0;

    UInt32 status = 0;
    if(copyId <=16 )
        status = m_owner->GetVar(VAR_ERLKING_STATUSA);
    else if(copyId <= 32)
        status = m_owner->GetVar(VAR_ERLKING_STATUSB);
    else
        status = m_owner->GetVar(VAR_ERLKING_STATUSC);

    UInt8 pos = 0;
    if(0 == copyId % 16)
        pos = 16;
    else
        pos = copyId % 16;

    UInt8 mark = GET_BIT_2(status, (pos-1));

    return mark;
}

void Erlking::SetErlkingStatus(UInt8 copyId, UInt8 type)
{
    if(0 == copyId || copyId > ERLKING_MAX_COPY_NUM)
        return;

    UInt32 status = 0;
    if(copyId <=16 )
        status = m_owner->GetVar(VAR_ERLKING_STATUSA);
    else if(copyId <= 32)
        status = m_owner->GetVar(VAR_ERLKING_STATUSB);
    else
        status = m_owner->GetVar(VAR_ERLKING_STATUSC);

    UInt8 pos = 0;
    if(0 == copyId % 16)
        pos = 16;
    else
        pos = copyId % 16;

    if(0 == type)
        status = SET_BIT(status, ((pos - 1) * 2));
    else
    {
        status = CLR_BIT(status, ((pos - 1) * 2));
        status = SET_BIT(status, ((pos - 1) * 2 + 1));
    }

    if(copyId <= 16)
        m_owner->SetVar(VAR_ERLKING_STATUSA, status);
    else if(copyId <= 32)
        m_owner->SetVar(VAR_ERLKING_STATUSB, status);
    else
        m_owner->SetVar(VAR_ERLKING_STATUSC, status);
}

UInt8 Erlking::GetsurplusNum()
{
    UInt8 buyTotalNum = m_owner->GetVar(VAR_ERLKING_BUY_PASS_TOTAL_NUM);
    UInt8 freeNum = m_owner->GetVar(VAR_ERLKING_USE_FREE_NUM_DAY);

    UInt8 surplusNum = 0;
    if(freeNum < 8)
        surplusNum = 8 - freeNum;

    surplusNum += buyTotalNum;

    return surplusNum;
}

void Erlking::SetsurplusNum(UInt8 num)
{
    UInt8 buyTotalNum = m_owner->GetVar(VAR_ERLKING_BUY_PASS_TOTAL_NUM);
    UInt8 freeNum = m_owner->GetVar(VAR_ERLKING_USE_FREE_NUM_DAY);
    
    if(num <= freeNum)
        m_owner->SetVar(VAR_ERLKING_USE_FREE_NUM_DAY, freeNum-num);
    else
    {
        num = num - freeNum;
        m_owner->SetVar(VAR_ERLKING_USE_FREE_NUM_DAY, 0);
        m_owner->SetVar(VAR_ERLKING_BUY_PASS_TOTAL_NUM, buyTotalNum-num);        
    }
}

void Erlking::BuyPassNum()
{
    UInt8 buyNumDay = m_owner->GetVar(VAR_ERLKING_BUY_PASS_NUM_DAY);
    //if(buyNumDay >= 5)
      //  return;

    UInt8 money = 0;
    if(buyNumDay < 2)
        money = 20;
    else if(buyNumDay < 4)
        money = 40;
    else
        money = 50;
    if(m_owner->getGold() < money)
    {
        m_owner->sendMsgCode(0, 1104);
        return;
    }
    ConsumeInfo ci(erlking, 0, 0);
    m_owner->useGold(money, &ci);

    m_owner->AddVar(VAR_ERLKING_BUY_PASS_NUM_DAY, 1);
    m_owner->AddVar(VAR_ERLKING_BUY_PASS_TOTAL_NUM, 1);

    Stream st(REP::ERLKING_INFO);
    st << static_cast<UInt8>(0x03);
    st << GetsurplusNum() << static_cast<UInt8>(m_owner->GetVar(VAR_ERLKING_BUY_PASS_NUM_DAY));
    st << Stream::eos;
    m_owner->send(st);
}

void Erlking::StartBattle(UInt8 copyId)
{
    if(0 == copyId || copyId > ERLKING_MAX_COPY_NUM)
        return;

    if(0 == GetsurplusNum())
        return;

    GData::ErlkingData::erlkingInfo * info = GData::erlkingData.getErlkingInfo(copyId);
    if(NULL == info)
        return;

    UInt8 mark = GetErlkingStatus(copyId);
    if(ERLKING_MARKA == mark)
        return;

    m_owner->checkLastBattled();

    GData::NpcGroups::iterator it = GData::npcGroups.find(info->npcgroup);
    if(it == GData::npcGroups.end())
        return;

    GData::NpcGroup * ng = it->second;
    if(NULL == ng)
        return;

    Battle::BattleSimulator bsim(Battle::BS_COPY5, m_owner, ng->getName(), ng->getLevel(), false);
    m_owner->PutFighters(bsim, 0);
    ng->putFighters(bsim);
    bsim.start();

    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return;

    bool isLast = (ng != NULL);
    UInt8 side = 0;
    UInt16 r = 0;
    UInt8 res = bsim.getWinner();
    if(1 == res)
    {
        side = 1;
        r = static_cast<UInt16>(0x0201);
    }
    else if(2 == res)
        r = static_cast<UInt16>(0x0200);

    UInt32 id = bsim.clearLastBattle(side, isLast);

    if(ng && 1==res)
    {
        if(ERLKING_MARKB == mark)
        {
            m_owner->GetPackage()->Add(info->itemId, 1, true, false, FromNpc);
            SetErlkingStatus(copyId, 1);
        }

        ng->getLoots(m_owner, m_owner->_lastLoot, 0, NULL);
        SetsurplusNum();
    }

    Stream st(REP::ATTACK_NPC);
    st << r << id << Stream::eos;
    m_owner->send(st);

    SendErlkingInfo();
}

void Erlking::AutoBattle(UInt8 copyId, UInt8 num)
{
    UInt8 surplusNum = GetsurplusNum();
    if(0 == num || num > surplusNum)
        return;

    UInt8 mark = GetErlkingStatus(copyId);
    if(mark != ERLKING_MARKC)
        return;

    GData::ErlkingData::erlkingInfo * info = GData::erlkingData.getErlkingInfo(copyId);
    if(NULL == info)
        return;

    GData::NpcGroups::iterator it = GData::npcGroups.find(info->npcgroup);
    if(it == GData::npcGroups.end())
        return;

    GData::NpcGroup * ng = it->second;
    if(NULL == ng)
        return;

    SetsurplusNum(num);

    for(UInt8 i=0; i<num; i++)
    {
        ng->getLoots(m_owner, m_owner->_lastLoot, 0, NULL);
    }

    Stream st(REP::ERLKING_INFO);
    st << static_cast<UInt8>(0x02);
    st << GetsurplusNum() << static_cast<UInt8>(m_owner->GetVar(VAR_ERLKING_BUY_PASS_NUM_DAY));
    UInt8 sz = m_owner->_lastLoot.size();
    st << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st << static_cast<UInt32>(m_owner->_lastLoot[i].id) << static_cast<UInt8>(m_owner->_lastLoot[i].count);
    }

    m_owner->checkLastBattled();

    st << Stream::eos;
    m_owner->send(st);
}

}

