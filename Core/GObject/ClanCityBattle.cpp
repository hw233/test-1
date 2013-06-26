#include "Config.h"
#include "CountryBattle.h"
#include "ClanCityBattle.h"
#include "Clan.h"
#include "Player.h"

#define BATTLE_TIME 2
#define FIRST_PREPARE_TIME 120
namespace GObject
{

static int atkRout[7][3] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {2, 0, 0},
    {2, 3, 0},
    {0, 3, 0},
    {4, 5, 6}
};

void CCBPlayer::sendInfo()
{
}

bool CCBPlayer::challenge(CCBPlayer* other)
{
}

void CCBSpot::playerEnter(CCBPlayer* pl)
{
    if(!pl || !canAtk || hp == 0)
        return;

    UInt8 side = pl->side;
    waiters[side].push_back(pl);
}

bool CCBSpot::erasePl(CCBPlayerList& list, CCBPlayer* pl)
{
    if(!pl)
        return false;
    std::vector it = std::find(list.begin(), list.end(), pl);
    if(it == list.end())
        return false;
    list.erase(it);
    return true;
}

void CCBSpot::moveAll(CCBPlayerList& list, UInt8 spot)
{
    size_t cnt = list.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        CCBPlayer* pl = list[i];
        clancity->move(pl, spot);
    }
}

void CCBSpot::fillTo100(CCBPlayerList& dst, CCBPlayerList& src)
{
    size_t cnt = dst.size();
    if(100 - cnt > src.size())
    {
        dst.append(src);
        src.clear();
    }
    else
    {
        for(size_t i = 0; i < 100 - cnt; ++ i)
        {
            dst.push_back(src[i]);
        }
        src.erase(src.begin(), src.begin()+i);
    }
}

bool CCBSpot::playerLeave(CCBPlayer* pl)
{
    if(!pl)
        return false;
    UInt8 side = pl->side;
    return erasePl(waiters[side], pl);
}

void CCBSpot::prepare()
{
    if(hp == 0)
        return;

    for(int i = 0; i < 3; ++ i)
    {
        skills[0][i].init();
        skills[1][i].init();
    }

    dead[0].clear()
    dead[1].clear()
    battler[0].clear()
    battler[1].clear()

    fillTo100(alive[0], waiters[0]);
    fillTo100(alive[1], waiters[1]);
}

void CCBSpot::handleBattle()
{
    if(!canAtk)
        return;
    if(hp == 0)
        return;

    size_t cnt0 = alive[0].size();
    size_t cnt1 = alive[1].size();
    battler[0].clear();
    battler[1].clear();
    for(size_t i = 0; i < 5; ++ i)
    {
        if(i < cnt0)
        {
            battler[0].push_back(alive[0][i]);
            if(i < cnt1)
            {
                battler[1].push_back(alive[1][i]);
                CCBPlayer* pl0 = battler[0][i];
                CCBPlayer* pl1 = battler[1][i];
                if(pl0.challenge(pl1))
                {
                    dead[1].push_back(pl1);
                    erasePl(alive[1], pl1);
                }
                else
                {
                    dead[0].push_back(pl0);
                    erasePl(alive[0], pl0);
                }
            }
            else
            {
                break;
            }
        }
        else if(i < cnt1)
        {
            if(hp == 0)
            {
                break;
            }
            hp -= 10;
        }
    }
}

void CCBSpot::end()
{
    if(!canAtk)
        return;

    battler[0].clear()
    battler[1].clear()

    if(hp == 0)
    {
        canAtk = false;
        if(id != 1)
        {
            clancity->openNextSpot(id);
            moveAll(alive[0], 1);
            moveAll(alive[1], 7);
            moveAll(waiters[0], 1);
            moveAll(waiters[1], 7);
        }
    }
    else
    {
        if(id != 1)
            moveAll(dead[0], 1);

        moveAll(dead[1], 7);
    }

    dead[0].clear()
    dead[1].clear()
}

void CCBSkill::doSkillEffect(CCBSpot& spot)
{
}

void CCBSkill::doSkillEffect(CCBClan& clan)
{
}

void CCBSkill::doSkillEffect(CCBPlayer* pl)
{
}

ClanCity::ClanCity(UInt16 loc) : m_loc(loc)
{
}

void ClanCity::Init()
{
}

void ClanCity::process(UInt32 curtime)
{
    UInt32 startTime = globalCountryBattle.getStartTime();
    if(startTime == 0)
        return;
    if(curtime < startTime + FIRST_PREPARE_TIME)
        return;

    UInt8 corr = (curtime - startTime - FIRST_PREPARE_TIME) / BATTLE_TIME;
    if(!m_nextTime)
    {   
        m_nextTime = startTime + FIRST_PREPARE_TIME + (corr + 1) * BATTLE_TIME;
    }
    if(curtime < m_nextTime)
        return;
    m_nextTime += BATTLE_TIME;

    handleBattle();
    if(curtime >= globalCountryBattle.getEndTime())
    {
        end();
        return;
    }
}

void ClanCity::prepare(UInt16)
{
    UInt32 startTime = globalCountryBattle.getStartTime();
    if(startTime == 0)
        return;
    m_nextTime = startTime + FIRST_PREPARE_TIME;
    openNextSpot(7);
}

void ClanCity::start(UInt16 rt)
{
}

void ClanCity::end()
{
}

bool ClanCity::playerEnter(Player * player)
{
    if(!player)
        return false;
    Clan* cl = player->getClan();
    if(!cl)
        return false;

    if (player->getCountry() >= COUNTRY_NEUTRAL)
        return false;
	if(!globalCountryBattle.isRunning())
		return false;
	UInt32 curtime = TimeUtil::Now();
	if(player->getBuffData(PLAYER_BUFF_NEW_CBATTLE, curtime))
	{
		player->sendMsgCode(0, 1402);
		return false;
	}

	player->addFlag(Player::CountryBattle);
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
    {
        CCBPlayer* pl = new CCBPlayer(e_player);
        pl->fgt.player = player;
        m_players[player] = pl;
        if(cl->isClanCityDefender())
        {
            pl->side = 0;
            move(pl, 1);
        }
        else
        {
            pl->side = 1;
            move(pl, 7);
        }
    }
}

void ClanCity::playerLeave(Player * player)
{
    if(!player)
        return;
    if (player->getCountry() >= COUNTRY_NEUTRAL)
        return;

    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;

    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    if(m_spots[pos].playerLeave(pl))
    {
        UInt32 curtime = TimeUtil::Now();
        m_players.erase(player);
        if(curtime >= globalCountryBattle.getStartTime() && curtime < globalCountryBattle.getEndTime())
            player->setBuffData(PLAYER_BUFF_NEW_CBATTLE, curtime + 5 * 60);

        player->delFlag(Player::CountryBattle);
        player->clearHIAttr();
        player->autoRegenAll();
    }
}

void ClanCity::move(Player* player, UInt8 spot)
{
    if(!player)
        return;
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;
    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    if(spot == pos + 1)
        return;
    if(m_spots[pos].playerLeave(pl))
        m_spots[spot-1].playerEnter(pl);
}

void ClanCity::move(CCBPlayer* pl, UInt8 spot)
{
    if(!pl)
        return;
    UInt8 pos = pl->pos;
    if(spot == pos + 1)
        return;
    if(m_spots[pos].playerLeave(pl))
        m_spots[spot-1].playerEnter(pl);
}

void ClanCity::openNextSpot(UInt8 id)
{
    UInt8 pos = id - 1;
    for(UInt8 i = 0; i < 3; ++ i)
    {
        UInt8 nextSpot = atkRout[pos][i];
        if(nextSpot == 0)
            continue;
        UInt8 nextPos = nextSpot - 1;
        m_spots[nextPos].canAtk = true;
    }
}

void ClanCity::handleBattle()
{
    for(int i = 0; i < 6; ++ i)
    {
        m_spots[i].handleBattle();
    }
}

}
