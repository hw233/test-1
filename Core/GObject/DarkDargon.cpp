#include "Config.h"
#include "Common/Stream.h"
#include "DarkDargon.h"
#include "Country.h"
#include "Server/SysMsg.h"
#include "MsgID.h"
#include "Script/BattleFormula.h"
#include "Common/TimeUtil.h"
#include "Battle/BattleSimulator.h"
#include <algorithm>
#include "WBossMgr.h"

//#define _TEST_VECSION

const UInt16 MONSTER = 13200;
const UInt16 MAPMONSTER = 13201;
const UInt16 BOSSMONSTER = 13202;

namespace GObject
{
DarkDargon::DarkDargon()
    : beginTime(0), _status(DARKDARGON_NOINIT), globalBuffer(0x0F)
{
    for (size_t i = 0; i < 4; i++)
    {
        roundTowers[i] = new RoundTower(i);
    }
    for (size_t m = 0; m < 3; m++)
    {
        starMaps[m] = new StarMap(m);
    }
    UInt32 day6 = TimeUtil::SharpWeek(6) + 14 * 3600;
    UInt32 now = TimeUtil::Now();
    now = now - (now % 10);
    if(day6 < now)
        beginTime = day6 + 7 * 86400;
    else
        beginTime = day6;

#ifdef _TEST_VECSION
    beginTime = now + 15 * 60;
#endif
    std::cout << "DarkDargon Current beginTime= " << beginTime << std::endl;

    GData::NpcGroups::iterator it = GData::npcGroups.find(MONSTER);
    if(it == GData::npcGroups.end())
        return;
    _ng = it->second;
    
    GData::NpcGroups::iterator it1 = GData::npcGroups.find(MAPMONSTER);
    if(it1 == GData::npcGroups.end())
        return;
    _ng1 = it1->second;
    
    GData::NpcGroups::iterator it2 = GData::npcGroups.find(BOSSMONSTER);
    if(it2 == GData::npcGroups.end())
        return;
    _ng2 = it2->second;

}

DarkDargon::~DarkDargon()
{
}

void DarkDargon::process(UInt32 now)
{
    if (0 == now)
        return; 
    switch(_status)
    {
        case DARKDARGON_NOINIT:
        {
            if ( now < beginTime - 10 * 60) 
                return;
            if(!CheckStartAct())
                return; 
            Stream st;
            if (now == beginTime - 10 * 60)
                SYSMSG_BROADCASTV(5191,10);
            if (now >= beginTime - 5 * 60)
            {
                SYSMSG_BROADCASTV(5191,5);
                _status = DARKDARGON_AWAIT;
                RetAwaitInfo();
            }
        }
            break;
        case DARKDARGON_AWAIT:
            if (now == beginTime)
            {
                _status = DARKDARGON_ONESTEP;
                RefreshTowerMonster(3);
                rebuildNpc();
                InitStarMap();
            }
            break;
        case DARKDARGON_ONESTEP:
            {
                if(now == beginTime + 60 * 2)
                    InitStarMap(1);
                if(now == beginTime + 60 * 4)
                    InitStarMap(2);
                if(now % 60 == 0)
                {
                    if(now < beginTime + 60 * 5)
                        RefreshTowerMonster(3);
                    else if(now < beginTime + 60 * 10)
                        RefreshTowerMonster(5);
                    else
                        RefreshTowerMonster();
                }
                AttackTowerMonster();
                CheckDargonArrive(0);
                broadcast(&DarkDargon::RetFirstStepInfo);
                if (now == beginTime + 15 * 60)
                {
                    _status = DARKDARGON_TWOSTEP;
                    broadcast(&DarkDargon::RetSecStepInfo);
                    AccountFirstStepTask();
                    DestroyStarMap();
                }
            }
            break;
        case DARKDARGON_TWOSTEP:
            {
                if(now >= beginTime + 15 * 60 + 10 * 60)
                {
                    if(now % 60 == 0)
                        AutoDamageBoss();
                }
                broadcast(&DarkDargon::RefreshBossInfo);
            }
            break;
        case DARKDARGON_OVER:
            {
                beginTime += 7 * 86400;
#ifdef _TEST_VECSION
                beginTime = now + 5 * 60;
#endif              
                std::cout << "DarkDargon Next beginTime= " << beginTime << std::endl;
                _status = DARKDARGON_NOINIT; 
                ReserAllFunc();
            }
            break;
        default:
            break;
    }
}

void DarkDargon::RetAwaitInfo(Player* pl)
{
    if(_status != DARKDARGON_AWAIT && _status != DARKDARGON_ONESTEP && _status != DARKDARGON_TWOSTEP)
        return;
    UInt32 now = TimeUtil::Now();
    UInt32 lastTime = (beginTime > now)? (beginTime - now):0; 

    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x00);
    
    st << lastTime; 
    st <<Stream::eos;
   
    if(pl == NULL)
	    NETWORK()->Broadcast(st);
    else
        pl->send(st);
}

void DarkDargon::RetFirstStepInfo(Player* pl)
{
    UInt32 now = TimeUtil::Now();
    UInt32 lastTime = (beginTime + 15 * 60 > now)? (beginTime + 15 * 60 - now):0; 
    UInt8 arriveSum = 0;
    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;

    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01);
    st << lastTime;
    
    for (size_t i = 0; i < 4; i++)
        st << static_cast<UInt8>(roundTowers[i]->durability) << roundTowers[i]->defenceSum;
    for (size_t i = 0; i < 3; i++)
    {
        st << starMaps[i]->mapHp  << static_cast<UInt32>(_lastHP * 1) << static_cast<UInt8>((starMaps[i]->arriveTS > now)?(starMaps[i]->arriveTS - now):0xFF);
        arriveSum += starMaps[i]->arriveNum;
    }
    
    st << static_cast<UInt8>((s_pss.size() > 3)?3:(s_pss.size()));
    UInt8 size = 0;
    UInt16 pl_rank = 0;
    for (DDPlayerScoreSort::iterator it = s_pss.begin(); it != s_pss.end(); it++)
    {
        if(it->player == NULL)
            continue;
        if(it->player == pl)
            pl_rank = size + 1;
        if(size < 3)
            st << it->player->getName() << it->score;
        size ++;
    }
    
    st << static_cast<UInt8>(pl_rank > 0xFF?0xFF:pl_rank) << ddpl->score << static_cast<UInt8>((ddpl->driveCD > now)?(ddpl->driveCD - now):0);
    
    st << arriveSum; 
    st <<Stream::eos;
    
    pl->send(st);
}

void DarkDargon::RetSecStepInfo(Player* pl)
{
    UInt32 now = TimeUtil::Now();
    UInt8 arriveSum = 0;
    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    
    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02);

    for (size_t i = 0; i < 4; i++)
        st << static_cast<UInt8>(roundTowers[i]->durability) << roundTowers[i]->defenceSum;
    for (size_t i = 0; i < 3; i++)
    {
        st << starMaps[i]->mapHp  << static_cast<UInt32>(_lastHP * 1) << static_cast<UInt8>((starMaps[i]->arriveTS > now)?(starMaps[i]->arriveTS - now):0xFF);
        arriveSum += starMaps[i]->arriveNum;
    }

    st << static_cast<UInt8>((s_pss.size() > 3)?3:(s_pss.size()));
    UInt8 size = 0;
    UInt16 pl_rank = 0;
    for (DDPlayerScoreSort::iterator it = s_pss.begin(); it != s_pss.end(); it++)
    {
        if(it->player == NULL)
            continue;
        if(it->player == pl)
            pl_rank = size + 1;
        if(size < 3)
            st << it->player->getName() << it->score;
        size ++;
    }
    
    st << static_cast<UInt8>(pl_rank > 0xFF?0xFF:pl_rank) << ddpl->score << static_cast<UInt8>((ddpl->driveCD > now)?(ddpl->driveCD - now):0);

    st << _hp << _lastHP << static_cast<UInt8>(GET_BIT_4(ddpl->singleBuffer,1)) << arriveSum; 

    st <<Stream::eos;
    
    pl->send(st);
}

void DarkDargon::RefreshBossInfo(Player* pl)
{
    UInt32 now = TimeUtil::Now();
    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    
    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x04);

    st << static_cast<UInt8>((s_pss.size() > 3)?3:(s_pss.size()));
    UInt8 size = 0;
    UInt16 pl_rank = 0;
    for (DDPlayerScoreSort::iterator it = s_pss.begin(); it != s_pss.end(); it++)
    {
        if(it->player == NULL)
            continue;
        if(it->player == pl)
            pl_rank = size + 1;
        if(size < 3)
            st << it->player->getName() << it->score;
        size ++;
    }
    
    st << static_cast<UInt8>(pl_rank > 0xFF?0xFF:pl_rank) << ddpl->score << static_cast<UInt8>((ddpl->driveCD > now)?(ddpl->driveCD - now):0);

    st << _hp << _lastHP; 

    st <<Stream::eos;
    pl->send(st);

}

void DarkDargon::RetDargonArrive(Player* pl)
{
    UInt32 now = TimeUtil::Now();
    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    UInt8 arriveSum = 0;
    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x03);

    for (size_t i = 0; i < 4; i++)
        st << static_cast<UInt8>(roundTowers[i]->durability) << roundTowers[i]->defenceSum;
    for (size_t i = 0; i < 3; i++)
    {
        st << starMaps[i]->mapHp  << static_cast<UInt32>(_lastHP * 1) << static_cast<UInt8>((starMaps[i]->arriveTS > now)?(starMaps[i]->arriveTS - now):0xFF);
        arriveSum += starMaps[i]->arriveNum;
    }

    st << arriveSum; 
    st <<Stream::eos;
    pl->send(st);
}

void DarkDargon::EnterDarkDargon(Player* pl)
{
    if(m_ddMap.find(pl) == m_ddMap.end())
    {
        DDPlayer* ddpl = new DDPlayer(); 
        ddpl->player = pl;
        ddpl->ePlStatus = PLAYER_FREE;
        m_ddMap.insert(std::make_pair(pl,ddpl));
        //AddDDScore(ddpl,0);
    }
    else
    {
        DDPlayer* ddpl = NULL;
        ddpl = getDDPlayer(pl);
        if(ddpl == NULL)
            return;
        ddpl->ePlStatus = PLAYER_FREE;  
    }
    
    if(m_pMap.find(pl->getId()) == m_pMap.end())
        m_pMap.insert(std::make_pair(pl->getId(),pl));
    else
        m_pMap[pl->getId()] = pl;

    switch(_status)
    {
        case DARKDARGON_ONESTEP:
            RetFirstStepInfo(pl);
            break;
        case DARKDARGON_TWOSTEP:
            RetSecStepInfo(pl);
            break;
        default:
            break;
    }

}

void DarkDargon::QuitDarkDargon(Player* pl)
{
    if(_status == DARKDARGON_NOINIT || _status == DARKDARGON_OVER)
        return;
    if(m_ddMap.find(pl) != m_ddMap.end())
    {
        DDPlayer* ddpl = NULL;
        ddpl = getDDPlayer(pl);
        if(ddpl == NULL)
            return;
        if(_status == DARKDARGON_ONESTEP)
            QuitRoundTower(pl);
        ddpl->ePlStatus = PLAYER_LEAVE;  
    }

}

void DarkDargon::ReturnRoundTowerInfo(Player* pl,UInt8 idx, UInt8 pos,UInt8 opt,bool res,Player* objpl)
{
    if (pl == NULL)
        return;
    if (idx == 0 || pos == 0 || idx > 4 || pos > 6)
        return;
    UInt8 durability = static_cast<UInt8>(roundTowers[idx - 1]->durability);
    UInt8 defNum = roundTowers[idx - 1]->defPlayer[pos - 1].size();

    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02);
    st <<static_cast<UInt8>(0x02);
    st << opt;
    st << idx << pos << durability;
    for(size_t i = 0;i < 6;i++)
        st << roundTowers[idx - 1]->monsterNum[i];
    st << defNum;     
    for (size_t i = 0;i < defNum; i++)
    {
        Player* pltmp = NULL;    
        pltmp = roundTowers[idx - 1]->defPlayer[pos - 1][i];
        if(pltmp == NULL)
            continue;
        Fighter* fgt = pltmp->getMainFighter();
        st << pltmp->getId() << pltmp->getName() << fgt->getLevel() << fgt->getColor() << fgt->getClass() << fgt->getSex() << pltmp->getClanName(); 
    }
    if(opt == 5 || opt == 6)
    {
        if(pl == NULL)
            return;
        st << res;
        if(opt == 5)
        {
            if(pl == NULL)
                return;
            st << objpl->getName();
        }
        if(opt == 6)
        {
            if(objpl == NULL)
                return;
            st << objpl->getName(); 
        }
    }
    st <<Stream::eos;
    pl->send(st);
}

void DarkDargon::DefRoundTower(Player* pl,UInt8 idx, UInt8 pos)
{
    if (idx == 0 || pos == 0 || idx > 4 || pos > 6 || roundTowers[idx - 1]->status != 1)
        return;
    DDPlayer* ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;

    if(_status != DARKDARGON_ONESTEP || ddpl->ePlStatus != PLAYER_FREE)
        return;
    switch(pos)
    {
        case 1:
            if(roundTowers[idx - 1]->defPlayer[pos - 1].size() >= 7)
                return;//满了
            roundTowers[idx - 1]->defPlayer[pos - 1].push_back(pl);
            break;
        case 2:
            if(roundTowers[idx - 1]->defPlayer[pos - 1].size() >= 8)
                return;//满了
            roundTowers[idx - 1]->defPlayer[pos - 1].push_back(pl);
            break;
        case 3:
            if(roundTowers[idx - 1]->defPlayer[pos - 1].size() >= 8)
                return;//满了
            roundTowers[idx - 1]->defPlayer[pos - 1].push_back(pl);
            break;
        case 4:
            if(roundTowers[idx - 1]->defPlayer[pos - 1].size() >= 9)
                return;//满了
            roundTowers[idx - 1]->defPlayer[pos - 1].push_back(pl);
            break;
        case 5:
            if(roundTowers[idx - 1]->defPlayer[pos - 1].size() >= 9)
                return;//满了
            roundTowers[idx - 1]->defPlayer[pos - 1].push_back(pl);
            break;
        case 6:
            if(roundTowers[idx - 1]->defPlayer[pos - 1].size() >= 9)
                return;//满了
            roundTowers[idx - 1]->defPlayer[pos - 1].push_back(pl);
            break;
        default:
            break;
    }
    ddpl->ePlStatus = PLAYER_DEFENCE; 
    ddpl->toweridx = idx;
    ddpl->towerpos = pos;
    roundTowers[idx - 1]->defenceSum += 1;

    ReturnRoundTowerInfo(pl,idx,pos,2);
}

void DarkDargon::QuitRoundTower(Player* pl)
{
    DDPlayer* ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    if (_status != DARKDARGON_ONESTEP || ddpl->ePlStatus != PLAYER_DEFENCE || ddpl->toweridx == 0 || ddpl->towerpos == 0 || ddpl->toweridx > 4 || ddpl->towerpos > 6)
        return;

    std::vector<Player*>::iterator it1 = std::find(roundTowers[ddpl->toweridx - 1]->defPlayer[ddpl->towerpos - 1].begin(),roundTowers[ddpl->toweridx - 1]->defPlayer[ddpl->towerpos - 1].end(),pl);
    if(it1 != roundTowers[ddpl->toweridx - 1]->defPlayer[ddpl->towerpos - 1].end())
        roundTowers[ddpl->toweridx - 1]->defPlayer[ddpl->towerpos - 1].erase(it1);    

    ddpl->ePlStatus = PLAYER_FREE; 
    roundTowers[ddpl->toweridx - 1]->defenceSum -= 1;
}

DDPlayer* DarkDargon::getDDPlayer(Player* pl)
{
    if(pl == NULL)
        return NULL;
    DDMap::iterator it = m_ddMap.find(pl);
    if(it == m_ddMap.end())
        return NULL;
    DDPlayer* ddpl = it->second;          
    return ddpl;
}

Player* DarkDargon::getPlayer(UInt64 playerId)
{
    if(m_pMap.find(playerId) == m_pMap.end())
        return NULL;
    PlayerMap::iterator it = m_pMap.find(playerId);
    if(it->second == NULL)
    {
        m_pMap.erase(it);
        return NULL;
    }
    Player* pl = it->second;          
    return pl;
}

void DarkDargon::PKRoundTower(Player* pl,UInt8 idx, UInt8 pos,UInt64 playerId)
{
    if (idx > 4 || pos > 6)
        return;
    Player* objpl = NULL;
    DDPlayer* ddpl = NULL;
    DDPlayer* ddobjpl = NULL;
    objpl = getPlayer(playerId);
    if(objpl == NULL)
        return;
    ddpl = getDDPlayer(pl);
    ddobjpl = getDDPlayer(objpl);
    if(ddpl == NULL || ddobjpl == NULL)
        return;
    if(ddpl->ePlStatus != PLAYER_FREE || ddobjpl->ePlStatus != PLAYER_DEFENCE)
        return;    
    UInt32 now = TimeUtil::Now();
    if(ddpl->driveCD > now)
    {
        pl->sendMsgCode(0, 1407, ddpl->driveCD - now);
        return;
    }
    bool res = attackPlayer(ddpl,ddobjpl); 
    if(res)
    {
        DefRoundTower(pl,idx,pos);
        QuitRoundTower(objpl);
    }

    ReturnRoundTowerInfo(pl,idx,pos,5,res,objpl);
    ReturnRoundTowerInfo(objpl,idx,pos,6,res,pl);
}

void DarkDargon::RefreshTowerMonster(UInt8 num)//60秒刷怪
{
    AccountTowerMonster();
    for (size_t i = 0; i < 4; i++ )
    {
        if(roundTowers[i]->status == 2)//已被攻破
            continue;
        roundTowers[i]->resetMonster();
        if(roundTowers[i]->status == 0)
            roundTowers[i]->status = 1; 
    }
}

void DarkDargon::AccountTowerMonster()//结算四防之塔怪
{
    for (size_t i = 0; i < 4; i++ )
    {
        if(roundTowers[i]->status == 0 || roundTowers[i]->status == 2)
            continue;
        if(roundTowers[i]->durability == 0.0f)
            DestroyTower(i); 
        if(roundTowers[i]->status == 0 || roundTowers[i]->status == 2)
            continue;
        float dmg = roundTowers[i]->calcDmg(); 
        if(dmg)
        {
            roundTowers[i]->durability = (roundTowers[i]->durability > dmg)?(roundTowers[i]->durability - dmg):(0.0f); 
            Stream st(REP::EXTEND_PROTOCAOL);
            st << static_cast<UInt8>(0x02);
            st << static_cast<UInt8>(0x02) << static_cast<UInt8>(8);
            UInt8 monsterSum = 0; 
            for (size_t m = 0; m < 6; m++)//遍历每一层
                monsterSum += roundTowers[i]->monsterNum[m];
            st << monsterSum << static_cast<UInt8>(dmg);  
            st << Stream::eos;
            SendDefAccountInfo(i,st); 
            if(roundTowers[i]->durability == 0.0f)
               DestroyTower(i); 
        }
    }

}

void DarkDargon::SendDefAccountInfo(UInt8 idx,Stream& st)//结算驻守信息
{
    for (size_t m = 0; m < 6; m++)//遍历每一层
    {
        for (std::vector<Player*>::iterator it = roundTowers[idx]->defPlayer[m].begin();it != roundTowers[idx]->defPlayer[m].end();it ++ )
        {
            if((*it) == NULL)
                continue;
            (*it)->send(st);
        }
    }
}

void DarkDargon::DestroyTower(UInt8 i)
{
    AddAllDDScore(-100);//塔被摧毁减100功德值
    for (size_t m = 0; m < 6; m++)//遍历每一层
    {
        std::vector<Player*> deftmp = roundTowers[i]->defPlayer[m];
        for (std::vector<Player*>::iterator it = deftmp.begin();it != deftmp.end();it ++ )
        {
            if((*it) == NULL)
                continue;
            QuitRoundTower((*it)); 
        }
    }
    roundTowers[i]->status = 2;
    globalBuffer = CLR_BIT(globalBuffer,i);
    Stream st(REP::EXTEND_PROTOCAOL);
    st << static_cast<UInt8>(0x02);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(10) << static_cast<UInt8>(i+1);
    st << Stream::eos;
    broadcast(st);
}


void DarkDargon::AttackTowerMonster()
{
        
    for (size_t i = 0; i < 4; i++ )
    {
        if(roundTowers[i]->status == 2)
            continue;
        for (size_t m = 0; m < 6; m++)//遍历每一层
        {
            if(roundTowers[i]->monsterNum[m] == 0 || roundTowers[i]->defPlayer[m].size() == 0)
                continue;
            //UInt8 defer_num = roundTowers[i]->defPlayer[m].size();
            vector<Player*> defTmp = roundTowers[i]->defPlayer[m];
            std::random_shuffle(roundTowers[i]->defPlayer[m].begin(), roundTowers[i]->defPlayer[m].end());
            UInt8 tmp_num = roundTowers[i]->monsterNum[m];
            for (size_t t = 0; t < tmp_num; t++)
            {
                if(defTmp.size() == 0)
                    break;
                Player* pl = *(defTmp.begin());
                DDPlayer* atker = getDDPlayer(pl);
                if(atker == NULL)
                {
                    vector<Player*>::iterator it = std::find(roundTowers[i]->defPlayer[m].begin(),roundTowers[i]->defPlayer[m].end(),pl);
                    roundTowers[i]->defPlayer[m].erase(it);
                }
                else
                {
                    bool res = attackNpc(atker,_ng);
                    if(res)
                    {
                        roundTowers[i]->monsterNum[m] --;
                        AddDDScore(atker,10);
                    }
                    else
                        QuitRoundTower(pl);
                        Stream st(REP::EXTEND_PROTOCAOL);
                        st << static_cast<UInt8>(0x02);
                        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(7);
                        st << res;
                        st << Stream::eos;
                        pl->send(st);
                }
                defTmp.erase(defTmp.begin());
            }

        }

    }

}

void DarkDargon::ReturnStarMapInfo(Player* pl,UInt8 opt,UInt8 idx, UInt32 ext1, UInt8 ext2)
{
    if(idx == 0 || idx > 3)
        return;
    
    DDPlayer* ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    UInt32 now = TimeUtil::Now();
    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02);
    st <<static_cast<UInt8>(0x03);
    st << opt;
    switch(opt)
    {
        case 1:
            st << GetBufferFlag(ddpl,true); 
            break;
        case 2:
            st << GetBufferFlag(ddpl,true); 
            break;
        case 3:
            st << idx ;
            st << starMaps[idx - 1]->mapHp << static_cast<UInt32>(_lastHP * 1) << static_cast<UInt8>((starMaps[idx - 1]->arriveTS > now)?(starMaps[idx - 1]->arriveTS - now):0xFF) << ext1 << ext2 ;
            break;
        case 4:
            st << idx ;
            st << starMaps[idx - 1]->mapHp << static_cast<UInt32>(_lastHP * 1) << static_cast<UInt8>((starMaps[idx - 1]->arriveTS > now)?(starMaps[idx - 1]->arriveTS - now):0xFF) << static_cast<UInt8>(ext1) << ext2;
            break;
        default:
            break;
    }

    st << Stream::eos;
    pl->send(st);
}

UInt8 DarkDargon::GetBufferFlag(DDPlayer* ddpl,bool isStarMap)
{
    if(isStarMap)
    {
        UInt8 bit_num = 0;        
        while(bit_num < 4)
        {
            if(GET_BIT(ddpl->singleBuffer, bit_num))
                return (bit_num + 1);
            bit_num ++;
        }
        return 0;
    }
    else
    {
        if(GET_BIT_4(ddpl->singleBuffer, 1))
            return GET_BIT_4(ddpl->singleBuffer, 1);
        else
            return 0;
    }

    return 0;
}

void DarkDargon::SetBufferFlag(Player* pl, UInt8 bit_num)
{
    DDPlayer* ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;

    if(bit_num > 4)
        return;
    if(bit_num < 4)
        ddpl->singleBuffer = (ddpl->singleBuffer >> 4) << 4;
    ddpl->singleBuffer = SET_BIT(ddpl->singleBuffer, bit_num);
}

void DarkDargon::AttackStarMap(Player* pl,UInt8 idx,UInt8 opt/* 0 - 破坏阵眼 1 - 延滞降临 */)
{
    if(idx == 0 || idx > 4 || starMaps[idx - 1]->status != 1)
        return;
    
    if(_status != DARKDARGON_ONESTEP)
        return;

    DDPlayer* ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    UInt32 now = TimeUtil::Now();
    if(ddpl->driveCD > now)
    {
        pl->sendMsgCode(0, 1407, ddpl->driveCD - now);
        return;
    }
    
    CheckDargonArrive(idx);

    UInt32 oldHP = starMaps[idx - 1]->mapHp;

    if(opt == 0)
    {
        bool res = attackNpc(ddpl,_ng1,true,(idx - 1));
        if(res == true)
        {
            starMaps[idx - 1]->overCD();            
            starMaps[idx - 1]->status = 2;            
            globalBuffer = SET_BIT(globalBuffer,(idx + 3));
            AddAllDDScore(100);
        }
    }
    else
    {
        /*bool res = */attackNpc(ddpl,_ng1,true,(idx - 1));
    }
    UInt32 damage = oldHP - starMaps[idx - 1]->mapHp;
    float dmg_percent = damage * 100 / static_cast<UInt32>(_lastHP * 1);

    UInt8 add_time = 0;//延滞时间
    UInt8 add_score = 0;//加的功德
    if(dmg_percent >= 1)    
    {
        add_score = 20;
        if(opt != 0)
            add_time = 10; 
    }
    else if(dmg_percent >= 0.03)
    {
        add_score = 10;
        if(opt != 0)
            add_time = 5; 
    }
    else
    {
        add_score = 5;
        if(opt != 0)
            add_time = 3; 
    }
    AddDDScore(ddpl,add_score);
    if(opt != 0)
    {
        AddDargonArriveTime(ddpl,idx,add_time);
        starMaps[idx - 1]->mapHp = oldHP; 
        ReturnStarMapInfo(pl,opt + 3,idx,add_time,add_score);
    }
    else
        ReturnStarMapInfo(pl,opt + 3,idx,damage,add_score);


}

void DarkDargon::CheckDargonArrive(UInt8 idx)
{
    if(idx > 4)
        return;
        
    UInt32 now = TimeUtil::Now();
    if(idx == 0)
    {
        for(size_t m = 0;m < 3;m++)
        {
            if(now >= starMaps[m]->arriveTS && starMaps[m]->status == 1)    
            {
                starMaps[m]->resetCD(now + 100);
                starMaps[m]->arriveNum += 1;
                broadcast(&DarkDargon::RetDargonArrive);
                for(size_t i = 0;i < 4;i++)
                    roundTowers[i]->durability = (roundTowers[i]->durability > 5)?(roundTowers[i]->durability - 5):(0.0f); 
            }
        }

    }
    else
    {
        if(now >= starMaps[idx -1]->arriveTS && starMaps[idx - 1]->status == 1)    
        {
            starMaps[idx - 1]->resetCD(now + 100);
            starMaps[idx - 1]->arriveNum += 1;
            broadcast(&DarkDargon::RetDargonArrive);
            Stream st(REP::EXTEND_PROTOCAOL);
            st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x02) << static_cast<UInt8>(9);
            st << Stream::eos; 
            broadcast(st);
            for(size_t i = 0;i < 4;i++)
                roundTowers[i]->durability = (roundTowers[i]->durability > 5)?(roundTowers[i]->durability - 5):(0.0f); 

        }
    }
}

void DarkDargon::InitStarMap(UInt8 idx)
{
    UInt32 now = TimeUtil::Now();
    for (size_t i = 0; i < 3; i++)
    {
        if(i == idx)
        {
            starMaps[i]->status = 1;
            starMaps[i]->resetCD(now + 100);
        }
    }

}

void DarkDargon::OptBoss(Player* pl,UInt8 opt)
{
    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    UInt32 now = TimeUtil::Now();

    ConsumeInfo ci; 
    ci.purchaseType = WBInspire; 
    switch(opt) 
    {
        case 1://天雷
        {
            if(ddpl->thunderCD >= now)
                return;
            if(pl->getGold() < 10)
            {
                pl->sendMsgCode(0, 1101);
                return ;
            }
            pl->useGold(10,&ci);

            UInt32 dmg = _hp * 0.01;
            _hp = (_hp >= dmg) ? (_hp - dmg) : 0 ;
            if(_hp == 0)
                _status = DARKDARGON_OVER;
            DamageBoss(ddpl,dmg); 
            ReturnBossInfo(pl,opt,dmg);
            ddpl->thunderCD = now + 10;
            break;
        }
        case 2://恢复活力
        {
            if(ddpl->recoverCD >= now)
                return;
            if(pl->getTael() < 2000)
            {
                pl->sendMsgCode(0, 1100);
                return;
            }
            pl->useTael(2000,&ci);

            UInt32 now = TimeUtil::Now();
            ddpl->driveCD = now;
            ReturnBossInfo(pl,opt);
            ddpl->recoverCD = now + 10;
            break;
        }
        case 3://仙力加持
        {
            UInt8 buf_num = GET_BIT_4(ddpl->singleBuffer,1); 
            if(buf_num >= 5)
                return;
            if(pl->getGold() < 10)
            {
                pl->sendMsgCode(0, 1101);
                return ;
            }
            pl->useGold(10,&ci);

            ddpl->singleBuffer = SET_BIT_4(ddpl->singleBuffer,1,(buf_num+1));
            ReturnBossInfo(pl,opt);
            break;
        }
        default:
            break;
    }

}

void DarkDargon::AttackBoss(Player* pl)
{
    if(_status != DARKDARGON_TWOSTEP)
        return;
    
    DDPlayer* ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    
    UInt32 now = TimeUtil::Now();
    if(ddpl->driveCD > now)
    {
        pl->sendMsgCode(0, 1407, ddpl->driveCD - now);
        return;
    }
      
    UInt32 oldHP = _hp;
    bool res = attackNpc(ddpl,_ng2,true,4);
    UInt32 damage = oldHP - _hp;
    DamageBoss(ddpl,damage); 
    if(res == true)
    {
        _status = DARKDARGON_OVER; 
        AccountFinalScore();
    }
}

void DarkDargon::DamageBoss(DDPlayer* ddpl,UInt32 dmg)
{
    float dmg_percent = dmg * 100 / _lastHP;
    UInt32 add_score = dmg_percent * 120; 
    AddDDScore(ddpl, (add_score > 20 ? add_score : 20));    
}

void DarkDargon::AutoDamageBoss()
{
    UInt32 damage = _lastHP / 5 + 1;
    _hp = (_hp > damage) ? (_hp - damage) : 0;
    if(_hp == 0)
    {
        _status = DARKDARGON_OVER; 
        AccountFinalScore();
    }
}

void DarkDargon::ReturnBossInfo(Player* pl,UInt8 type,UInt32 ext1)
{

    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(pl);
    if(ddpl == NULL)
        return;
    UInt32 now = TimeUtil::Now();
    Stream st(REP::EXTEND_PROTOCAOL);
    st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x04) << type;

    if(type == 1 || type == 4)
    {
        st << static_cast<UInt8>((s_pss.size() > 3)?3:(s_pss.size()));
        UInt8 size = 0;
        UInt16 pl_rank = 0;
        for (DDPlayerScoreSort::iterator it = s_pss.begin(); it != s_pss.end(); it++)
        {
            if(it->player == NULL)
                continue;
            if(it->player == pl)
                pl_rank = size + 1;
            if(size < 3)
                st << it->player->getName() << it->score;
            size ++;
        }
    
        st << static_cast<UInt8>(pl_rank > 0xFF?0xFF:pl_rank) << ddpl->score << static_cast<UInt8>((ddpl->driveCD > now)?(ddpl->driveCD - now):0);

        st << _hp << _lastHP; 
    }

    switch(type)
    {
        case 1:
            st << ext1; 
            break;
        case 2:
            st << (ddpl->driveCD > now ? ddpl - now : 0); 
            break;
        case 3:
        {
            UInt8 buf_num = GET_BIT_4(ddpl->singleBuffer,1);
            st << buf_num;
            break;
        }
        default:
            break;
    }
    st << Stream::eos;
    pl->send(st);

}


bool DarkDargon::attackPlayer(DDPlayer* atker,DDPlayer* defer)
{
    bool res;
    UInt32 reptid = 0;
    UInt32 now = 0;
    now= TimeUtil::Now();
    atker->driveCD = now + 20;
    Battle::BattleSimulator bsim(0xFD03, atker->player, defer->player);
    atker->player->PutFighters( bsim, 0, true );
    defer->player->PutFighters( bsim, 1, true );
    bsim.start();
    res = bsim.getWinner() == 1;
    reptid = bsim.getId();
    Stream st(REP::ATTACK_NPC);
    st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId()<<static_cast<UInt64>(0) << Stream::eos;
    atker->player->send(st); 
    defer->player->send(st); 
    
    return (res == 1);
}

bool DarkDargon::attackNpc(DDPlayer* atker,GData::NpcGroup* n_ng , bool b_extend,UInt8 idx)
{
    atker->player->checkLastBattled();

    if(b_extend)
    {
        UInt32 now = TimeUtil::Now();
        atker->driveCD = now + 20;
    }

    rebuildFgtAttr(atker->player,true);
    UInt16 ret = 0x0100;
    std::vector<GData::NpcFData>& nflist = n_ng->getList();
    if(nflist.size() == 0)
        return false;
    GData::NpcFData& nfdata = nflist[0];
    GObject::Fighter* monster = nfdata.fighter;
    Battle::BattleSimulator bsim(0xFD03, atker->player, monster->getName(), monster->getLevel(), true);
    atker->player->PutFighters(bsim, 0);
    if(!b_extend)
        n_ng->putFighters(bsim);
    else
    {
        if(idx > 4)
            return false; 
        Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
        if(idx == 4)
            bf->setHP(_hp);
        else
            bf->setHP(starMaps[idx]->mapHp);
    }
    
    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;
    bool res = bsim.getWinner() == 1;

    if(b_extend)
    {
        UInt32 oldHP = 0;
        if(idx == 4)
            oldHP = _hp;
        else
            oldHP = starMaps[idx]->mapHp;
        Battle::BattleObject * obj = bsim(1, nfdata.pos);
        Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
        UInt32 damage = 0;
        if(idx == 4) 
        {
            _hp = bfgt->getHP();
            damage = oldHP - _hp;
        }
        else
        {
            starMaps[idx]->mapHp = bfgt->getHP();
            damage = oldHP - starMaps[idx]->mapHp;
        }
        TRACE_LOG("DarkDargon INSERT ret: %u (pid: %" I64_FMT "u, dmg: %u)", ret, atker->player->getId(), damage);

    }

    UInt64 exp = 0 ; 
    if(res)
    {
        ret = 0x0101;
        atker->player->_lastNg = n_ng;
        exp = n_ng->getExp(); 
        atker->player->pendExp(exp);
    }
        
    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(atker->player, lastExp) << static_cast<UInt8>(0);
    UInt8 size = atker->player->_lastLoot.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
    {
        st << atker->player->_lastLoot[i].id << atker->player->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << static_cast<UInt64>(exp);
    st << Stream::eos;
    atker->player->send(st);
    rebuildFgtAttr(atker->player,false);

    return res;
}

void DarkDargon::broadcast(Stream& st,bool isIn/* 是否在活动中 */)
{
    for (DDMap::iterator it = m_ddMap.begin(); it != m_ddMap.end(); it++)
    {
        if(it->second->player == NULL)
            continue;
        if(isIn)
        {
            if(it->second->ePlStatus == PLAYER_LEAVE)
                continue;
        }
        it->second->player->send(st);
    }
}

void DarkDargon::broadcast(Func1 func,bool isIn/* 是否在活动中 */)
{
    for (DDMap::iterator it = m_ddMap.begin(); it != m_ddMap.end(); it++)
    {
        if(it->second->player == NULL)
            continue;
        if(isIn)
        {
            if(it->second->ePlStatus == PLAYER_LEAVE)
                continue;
        }
        (this->*func)(it->second->player);
    }
}

void DarkDargon::AddDDScore(DDPlayer* ddpl,Int32 score)
{
    if(ddpl->player != NULL)
    {
        DDPlayerScore ps;    
        ps.score = 0;
        for(DDPlayerScoreSort::iterator i = s_pss.begin(); i != s_pss.end(); ++i)
        {
            if(i->player == NULL)
                continue;

            if(i->player == ddpl->player)    
            {
                //ps.player = i->player;  
                ps.score = i->score;  
                s_pss.erase(i);
                break;
            }
        }

        ps.player = ddpl->player;  
        Int32 score_sum = ps.score + score; 
        ps.score = (score_sum > 0)?(score_sum):0;  
        ps.time = TimeUtil::Now();  
        
        s_pss.insert(ps);
        ddpl->score = ps.score; 
        if(score > 0)
        {
            SYSMSG_SENDV(5194, ddpl->player, score);
            SYSMSG_SENDV(5199, ddpl->player, score);
        }
    }
    /*else
    {
        for(DDPlayerScoreSort::iterator i = s_pss.begin(); i != s_pss.end(); ++i)
        {
            if(i->player == NULL)
                continue;
            {
                i->score = i->score + score;
                DDPlayer* t_ddpl = getDDPlayer(i->player);
                if(t_ddpl == NULL)
                    continue;
                t_ddpl->score = i->score; 

            }
        }
    }*/

    return;   
}

void DarkDargon::AddAllDDScore(Int32 score)
{
    for (DDMap::iterator it = m_ddMap.begin(); it != m_ddMap.end(); it++)
    {
        if(it->second->player == NULL)
            continue;
        AddDDScore(it->second,score); 
    }
}



void DarkDargon::AddDargonArriveTime(DDPlayer* ddpl,UInt8 idx,UInt32 add_tm)
{
    UInt32 max_time = TimeUtil::Now() + 100;
    UInt32 tmp_time = starMaps[idx - 1]->arriveTS;
    starMaps[idx - 1]->arriveTS = (tmp_time + add_tm > max_time)?(max_time):(tmp_time + add_tm);

}

void DarkDargon::rebuildNpc()
{
    worldBoss.calInitDarkDargon(_lastHP,_lastAtk,_lastMAtk);

    std::vector<GData::NpcFData>& nflist1 = _ng1->getList();
    Int32 baseatk1 = Script::BattleFormula::getCurrent()->calcAttack(nflist1[0].fighter);
    Int32 basematk1 = Script::BattleFormula::getCurrent()->calcMagAttack(nflist1[0].fighter);
    nflist1[0].fighter->setWBoss(true);
    nflist1[0].fighter->setBaseHP(static_cast<UInt32>(_lastHP * 1));
    nflist1[0].fighter->setExtraAttack(_lastAtk * 3 - baseatk1);
    nflist1[0].fighter->setExtraMagAttack(_lastMAtk * 3 - basematk1);
    
    std::vector<GData::NpcFData>& nflist2 = _ng2->getList();
    Int32 baseatk2 = Script::BattleFormula::getCurrent()->calcAttack(nflist2[0].fighter);
    Int32 basematk2 = Script::BattleFormula::getCurrent()->calcMagAttack(nflist2[0].fighter);
    nflist2[0].fighter->setWBoss(true);
    nflist2[0].fighter->setBaseHP(_lastHP);
    nflist2[0].fighter->setExtraAttack(_lastAtk* 4 - baseatk2);
    nflist2[0].fighter->setExtraMagAttack(_lastMAtk * 4 - basematk2);

    for (size_t i = 0; i < 4; i++)
    {
        if(i < 3)
            starMaps[i]->mapHp = static_cast<UInt32>(_lastHP * 1);
        else
            _hp = _lastHP;
    }


}

bool DarkDargon::CheckStartAct()
{
    UInt32 tmp = GVAR.GetVar(GVAR_MAX_LEVEL);
    if(tmp >= 80)
        return true;        
    else
        return false;
}

void DarkDargon::rebuildFgtAttr(Player* player,bool isddbuf)
{
    DDPlayer* ddpl = NULL;
    ddpl = getDDPlayer(player);
    if(ddpl == NULL)
        return;

    std::map<UInt32, Fighter *>& fighters = player->getFighterMap();
    float atk_fct = 0.0f; 
    float def_fct = 0.0f; 
    float action_fct = 0.0f; 
    float critical_fct = 0.0f; 
    float hp_fct = 0.0f; 
    float ru_fct = 0.0f; 
    float shi_fct = 0.0f; 
    float dao_fct = 0.0f; 
    float mo_fct = 0.0f; 
    UInt8 num = 0;
    while(num < 8)
    {
        if(GET_BIT(globalBuffer,num))
        {
            switch(num)
            {
                case 0:
                    atk_fct += 0.05f;
                    break;
                case 1:
                    critical_fct += 0.05f;
                    break;
                case 2:
                    hp_fct += 0.05f;
                    break;
                case 3:
                    action_fct += 0.05f;
                    break;
                case 4:
                    atk_fct += 0.05f;
                    break;
                case 5:
                    def_fct += 0.05f;
                    break;
                case 6:
                    hp_fct += 0.05f;
                    break;
                default:
                    break;
            }
        }
        num ++;
    }
    UInt8 pl_buf1 = GetBufferFlag(ddpl,true);
    UInt8 pl_buf2 = GetBufferFlag(ddpl,false);
    switch(pl_buf1)
    {
        case 1:
            ru_fct += 1.0f;
            break;
        case 2:
            shi_fct += 1.0f;
            break;
        case 3:
            dao_fct += 1.0f;
            break;
        case 4:
            mo_fct += 1.0f;
            break;
        default:
            break;
    }
    
    if(pl_buf2 > 0)
        atk_fct += 0.2f * pl_buf2; 

    for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
    {
        switch(it->second->getClass())  
        {
            case 1:
                if(ru_fct)
                    atk_fct += ru_fct;
                break;
            case 2:
                if(shi_fct)
                    atk_fct += shi_fct;
                break;
            case 3:
                if(dao_fct)
                    atk_fct += dao_fct;
                break;
            case 4:
                if(mo_fct)
                    atk_fct += mo_fct;
                break;
            default:
                break;
        }
        Int32 addatk = Script::BattleFormula::getCurrent()->calcAttack(it->second) * atk_fct;
        Int32 addmatk = Script::BattleFormula::getCurrent()->calcMagAttack(it->second) * atk_fct;
        Int32 adddef = Script::BattleFormula::getCurrent()->calcDefend(it->second) * def_fct;
        Int32 addaction = Script::BattleFormula::getCurrent()->calcAction(it->second) * action_fct;
        Int32 addcritical = Script::BattleFormula::getCurrent()->calcCritical(it->second,NULL) * critical_fct;
        Int32 addhp = Script::BattleFormula::getCurrent()->calcHP(it->second) * hp_fct;
        
        it->second->setPlDDExtraAttack(addatk);
        it->second->setPlDDExtraMagAttack(addmatk);
        it->second->setPlDDExtraDef(adddef);
        it->second->setPlDDExtraAction(addaction);
        it->second->setPlDDExtraCritical(addcritical);
        it->second->setPlDDExtraHp(addhp);

        it->second->setDarkDargonBuf(isddbuf); 
        it->second->setDirty();
    }

}

void DarkDargon::GMDestroyStarMap()
{
    for (size_t i = 0; i < 3; i++)
    {
        if(starMaps[i]->status != 1)
            continue;
        starMaps[i]->mapHp = 1;
    }
}

void DarkDargon::AccountFirstStepTask()
{
    UInt8 star_num = 0;
    UInt8 tower_num = 0;
    UInt8 arrive_num = 0;

    for (size_t i = 0; i < 3; i++)
    {
        if(starMaps[i]->status == 2)
            star_num ++;
        arrive_num += starMaps[i]->arriveNum;
    }

    for (size_t i = 0; i < 4; i++)
    {
        if(roundTowers[i]->status == 1)
            tower_num ++;
    }

    if(star_num == 3)
        AddAllDDScore(50);
    else if(star_num > 0)
        AddAllDDScore(20);

    if(tower_num == 4)
        AddAllDDScore(50);
    else if(tower_num > 0)
        AddAllDDScore(20);
    
    if(arrive_num == 0)
        AddAllDDScore(50);
    else if(arrive_num <= 3)
        AddAllDDScore(20);

    AddAllDDScore(100);//任务四
}

void DarkDargon::AccountFinalScore()
{
    AddAllDDScore(200);//击退boss
    SYSMSG(title, 5192);
    
    UInt16 pos = 1;
    for(DDPlayerScoreSort::iterator i = s_pss.begin(); i != s_pss.end(); ++i)
    {
        if(i->player == NULL)
            continue;
        {
            DDPlayer* t_ddpl = getDDPlayer(i->player);
            if(t_ddpl == NULL)
                continue;
            UInt32 box_num = i->score / 150;
            UInt32 p_num = i->score / 5;
            if(pos == 1)
            {
                p_num += 1000;
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item1[2] = {{17022,6},{17025,box_num}};
                MailItemsInfo itemsInfo(item1, DarkDargonAward, 2);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item1, 2, true);
            }
            else if(pos == 2)
            {
                p_num += 700;
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item2[2] = {{17022,4},{17025,box_num}};
                MailItemsInfo itemsInfo(item2, DarkDargonAward, 2);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item2, 2, true);
            }
            else if(pos == 3)
            {
                p_num += 500;
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item3[2] = {{17023,6},{17025,box_num}};
                MailItemsInfo itemsInfo(item3, DarkDargonAward, 2);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item3, 2, true);
            }
            else if(pos >= 4 || pos <= 9)
            {
                p_num += 300;
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item4[2] = {{17023,4},{17025,box_num}};
                MailItemsInfo itemsInfo(item4, DarkDargonAward, 2);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item4, 2, true);
            }
            else if(pos >= 10 || pos <= 29)
            {
                p_num += 200;
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item5[2] = {{17024,4},{17025,box_num}};
                MailItemsInfo itemsInfo(item5, DarkDargonAward, 2);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item5, 2, true);
            }
            else if(pos >= 30 || pos <= 50)
            {
                p_num += 100;
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item6[2] = {{17024,2},{17025,box_num}};
                MailItemsInfo itemsInfo(item6, DarkDargonAward, 2);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item6, 2, true);
            }
            else
            {
                SYSMSGV(content, 5193, pos, p_num);
                MailPackage::MailItem item7[] = {{17025,box_num}};
                MailItemsInfo itemsInfo(item7, DarkDargonAward, 1);
                Mail * mail = i->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(mail)
                     mailPackageManager.push(mail->id, item7, 1, true);
            }
            
            if(p_num)
                i->player->getPrestige(p_num);
            std::cout << "score is " << i->score << "Prestige: " << p_num << "." << std::endl;
    
            Stream st(REP::EXTEND_PROTOCAOL);
            st <<static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x05) << static_cast<UInt8>(pos > 0xFF?0xFF:pos);
            st << Stream::eos; 
            i->player->send(st);
        }
        pos ++;
    }

}

void DarkDargon::DestroyStarMap()
{
    for (size_t i = 0; i < 3; i++)
    {
        starMaps[i]->status = 2;
        //starMaps[i]->mapHp = 0;
        starMaps[i]->overCD();
        globalBuffer = SET_BIT(globalBuffer,(i + 3));
    }
}

void DarkDargon::ReserAllFunc()
{
    globalBuffer = 0x0F;    
    m_pMap.clear();
    s_pss.clear();

    for (DDMap::iterator it = m_ddMap.begin(); it != m_ddMap.end(); it++)
        delete it->second;

    m_ddMap.clear();
    for (size_t i = 0; i < 4; i++)
    {
        roundTowers[i]->resetAll();
    }
    for (size_t m = 0; m < 3; m++)
    {
        starMaps[m]->resetAll();
    }

}


}







