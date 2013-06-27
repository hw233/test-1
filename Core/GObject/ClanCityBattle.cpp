#include "Config.h"
#include "CountryBattle.h"
#include "ClanCityBattle.h"
#include "Clan.h"
#include "Player.h"
#include "Common/URandom.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "DB/DBExecHelper.h"

#define BATTLE_TIME 2
#define FIRST_PREPARE_TIME 90
#define PREPARE_TIME 45

#define CCB_SKILL_HP              1     // 生命鼓舞
#define CCB_SKILL_ATK             2     // 攻击鼓舞
#define CCB_SKILL_ACTION_1        3     // 身法鼓舞
#define CCB_SKILL_SPOT_DEC_DMG    4     // 巨熊
#define CCB_SKILL_ACTION_2        5     // 猎豹
#define CCB_SKILL_SPOT_BOMB_1     6     // 炼蛇
#define CCB_SKILL_SPOT_EXTRA_DMG  7     // 蛮牛
#define CCB_SKILL_ACTION_3        8     // 飞鹰
#define CCB_SKILL_SPOT_BOMB_2     9     // 狂狮

#define CLAN_CITY_TYPE_DEF            1 // 防守战
#define CLAN_CITY_TYPE_ATK            2 // 攻城战

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

namespace DB
{
}

void CCBPlayer::sendInfo()
{
}

bool CCBPlayer::challenge(CCBPlayer* other)
{
    if(!other)
        return true;

    bool ret = false;
    if(type == e_skill)
        ret = fgt.skill->doSkillEffect(other);
    else if(other->type == e_skill)
        ret = !(other->fgt.skill->doSkillEffect(this));
    else if(type == e_player)
    {
        if(other->type == e_player)
        {
            ret = attackPlayer(other);
        }
        else
            ret = attackNpc(other);
    }
    else if(other->type == e_player)
    {
        ret = !(other->attackNpc(this));
    }

    if(type == e_player)
        fgt.player->clearHIAttr();
    if(other->type == e_player)
        other->fgt.player->clearHIAttr();

    return ret;
}

bool CCBPlayer::attackNpc(CCBPlayer* npc)
{
    if(!npc)
        return true;

    GData::NpcGroups::iterator it = GData::npcGroups.find(npc->fgt.npcId);
    if(it == GData::npcGroups.end())
		return true;

    Player* player = fgt.player;
    float factor = 1.0f - static_cast<float>(weary)*0.5f;
    if(factor < 0.001f)
        factor = 0;
    player->setSpiritFactor(factor);

	GData::NpcGroup * ng = it->second;
    Battle::BattleSimulator bsim(location, player, ng->getName(), ng->getLevel(), false, turns);
    fgt.player->PutFighters( bsim, 0 );
    ng->putFighters( bsim );

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    GData::NpcFData& nfdata = nflist[0];
    Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
    if(npc->weary != 0)
    {
        bf->setHP(npc->weary);
    }

    bsim.start();
    player->setSpiritFactor(1.0f);

    bool res = bsim.getWinner() == 1;
    if(res)
    {
        npc->weary = 0;
        ++ win;
        bsim.applyFighterHP(0, player, false, false);
        weary = 1 + 0.5f * bsim.getLostHPPercent(0, player);
    }
    else
    {
        win = 0;
        npc->weary = bf->getHP();
        player->autoRegenAll();
    }

    return res;
}

bool CCBPlayer::attackPlayer(CCBPlayer* other)
{
    if(!other)
        return true;

    Player* player1 = fgt.player;
    Player* player2 = other->fgt.player;

    float factor = 1.0f - static_cast<float>(weary)*0.5f;
    if(factor < 0.001f)
        factor = 0;
    player1->setSpiritFactor(factor);

    factor = 1.0f - static_cast<float>(other->weary)*0.5f;
    if(factor < 0.001f)
        factor = 0;
    player2->setSpiritFactor(factor);

    Battle::BattleSimulator bsim(location, player1, player2);
    player1->PutFighters( bsim, 0);
    player2->PutFighters( bsim, 1);

    bsim.start();
    player1->setSpiritFactor(1.0f);
    player2->setSpiritFactor(1.0f);

    bool res = bsim.getWinner() == 1;
    if(res)
    {
        bsim.applyFighterHP(0, player1, false, false);
        weary = 1 + 0.5f * bsim.getLostHPPercent(0, player1);
        player2->autoRegenAll();
        other->weary = 0;
        ++ win;
        other->win = 0;
    }
    else
    {
        win = 0;
        ++ other->win;
        weary = 0;
        bsim.applyFighterHP(1, player2, false, false);
        other->weary = 1 + 0.5f * bsim.getLostHPPercent(1, player2);
        player1->autoRegenAll();
    }

    return res;
}

CCBClan::CCBClan() : clan(NULL), score(0)
{
    skill_hp.id = CCB_SKILL_HP;
    skill_atk.id = CCB_SKILL_ATK;
    skill_action.id = CCB_SKILL_ACTION_1;
}

void CCBClan::doSkill(CCBPlayer* pl)
{
    if(!pl)
        return;
    skill_hp.doSkillEffect(pl);
    skill_atk.doSkillEffect(pl);
    skill_action.doSkillEffect(pl);
}

CCBSpot::CCBSpot(ClanCity* cc) : id(0), canAtk(false), hp(0), clancity(cc)
{
    for(int i = 0; i < 2; ++ i)
    {
        bomb[i].type = e_skill;
        bomb[i].side = i;
        bomb[i].pos = id - 1;
        bomb[i].fgt.skill = &(skill_bomb[i]);
    }

    skill_dmg[0].id = CCB_SKILL_SPOT_DEC_DMG;
    skill_action[0].id = CCB_SKILL_ACTION_2;
    skill_bomb[0].id = CCB_SKILL_SPOT_BOMB_1;

    skill_dmg[1].id = CCB_SKILL_SPOT_EXTRA_DMG;
    skill_action[1].id = CCB_SKILL_ACTION_3;
    skill_bomb[1].id = CCB_SKILL_SPOT_BOMB_2;
}

void CCBSpot::playerEnter(CCBPlayer* pl)
{
    if(!pl || (!canAtk && id != 1 && id != 7) || hp == 0)
        return;

    UInt8 side = pl->side;
    pl->pos = id - 1;
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
        if(pl->type == e_player)
            clancity->move(pl, spot);
    }
}

void CCBSpot::fillTo100(CCBPlayerList& dst, CCBPlayerList& src, CCBPlayer* bomb)
{
    size_t cnt = dst.size();
    if(100 - cnt > src.size())
    {
        dst.append(src);
        src.clear();
    }
    else
    {
        size_t cnt2 = 100 - cnt;
        if(bomb != NULL)
            -- cnt;
        for(size_t i = 0; i < cnt2; ++ i)
        {
            dst.push_back(src[i]);
        }
        src.erase(src.begin(), src.begin()+i);
    }
    if(bomb != NULL)
    {
        size_t cnt = dst.size();
        UInt32 = uRand(cnt);
        dst.insert(dst.begin() + rnd, bomb);
    }
}

bool CCBSpot::playerLeave(CCBPlayer* pl)
{
    if(!pl)
        return false;
    if(canAtk)
        return false;
    UInt8 side = pl->side;
    return erasePl(waiters[side], pl);
}

void CCBSpot::prepare()
{
    if(hp == 0)
        return;

    CCBPlayer* bomb0= NULL;
    CCBPlayer* bomb1= NULL;
    skill_bomb[0].init();
    skill_bomb[1].init();
    if(skill_bomb[0].id == CCB_SKILL_SPOT_BOMB_1 && skill_bomb[0].lvl > 0)
    {
        bomb0 = &(bomb[0]);
    }
    if(skill_bomb[1].id == CCB_SKILL_SPOT_BOMB_2 && skill_bomb[1].lvl > 0)
    {
        bomb1 = &(bomb[1]);
    }

    dead[0].clear()
    dead[1].clear()
    battler[0].clear()
    battler[1].clear()

    fillTo100(alive[0], waiters[0], bomb0);
    fillTo100(alive[1], waiters[1], bomb1);
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
                skill_action[0].doSkillEffect(pl0);
                skill_action[1].doSkillEffect(pl1);
                clancity->doClanSkill(pl0);
                clancity->doClanSkill(pl1);
                if(pl0->challenge(pl1))
                {
                    dead[1].push_back(pl1);
                    erasePl(alive[1], pl1);
                }
                else
                {
                    skill_dmg[1].doSkillEffect(this);
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
            makeDamage(10);
        }
    }
}

void CCBSpot::makeDamage(UInt16 dmg)
{
    if(hp == 0)
        return;

    UInt16 dmg2 = skill_dmg[0].doSkillEffect(dmg);
    if(dmg2 > hp)
        hp = 0;
    else
        hp -= dmg2;
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

    erasePl(alive[0], &(bomb[0]));
    erasePl(alive[1], &(bomb[1]));

    dead[0].clear()
    dead[1].clear()
}

void CCBSkill::doSkillEffect(CCBSpot& spot)
{
    if(lvl == 0)
        return;
    if(id == CCB_SKILL_SPOT_EXTRA_DMG)
    {
        if(uRand(1000) < 0.5f*lvl)
        {
            spot.makeDamage(5);
        }
    }
}

bool CCBSkill::doSkillEffect(CCBPlayer* pl)
{
    if(!pl || lvl == 0)
        return false;
    GData::AttrExtra attr;
    switch(id)
    {
    case CCB_SKILL_HP:
        attr.hpP = 0.1f * lvl;
        break;
    case CCB_SKILL_ATK:
        attr.atkP = 0.1f * lvl;
        attr.magatkP = 0.1f * lvl;
        break;
    case CCB_SKILL_ACTION_1:
    case CCB_SKILL_ACTION_2:
    case CCB_SKILL_ACTION_3:
        attr.actionP = 0.1f * lvl;
        break;
    case CCB_SKILL_SPOT_BOMB_1:
        if(times >= 10)
            return false;
        if(uRand(1000) < 10 * lvl)
        {
            ++ times;
            return true;
        }
        return false;
    case CCB_SKILL_SPOT_BOMB_2:
        if(times >= 3)
            return false;
        if(uRand(1000) < 5 * lvl)
        {
            ++ times;
            return true;
        }
        return false;
    }
    if(pl->type == e_player)
        pl->fgt.player->addHIAttr(attr);
    return false;
}

UInt16 CCBSkill::doSkillEffect(UInt16 dmg)
{
    if(lvl == 0)
        return dmg;

    if(id == CCB_SKILL_SPOT_DEC_DMG)
    {
        if(uRand(1000) < 5 * lvl)
        {
            return dmg/2;
        }
    }

    return dmg;
}

ClanCity::ClanCity(UInt16 loc) : m_loc(loc), m_nextTime(0), m_expTime(0), m_type(0), m_round(0)
{
    for(int i = 0; i < 7; ++ i)
    {
        m_spots[i].id = i+1;
        m_spots[i].hp = 1000;
        if(i > 2 && i < 6)
            m_spots[i].canAtk = true;
        else
            m_spots[i].canAtk = false;
    }
}

void ClanCity::process(UInt32 curtime)
{
    UInt32 startTime = globalCountryBattle.getStartTime();
    if(startTime == 0)
        return;
    if(curtime < startTime + FIRST_PREPARE_TIME)
        return;
    if(m_expTime)
        checkAddExp(curtime);

    if(!m_expTime)
        m_expTime = startTime + 60;

    if(!m_nextTime)
    {   
        UInt8 corr = (curtime - startTime - FIRST_PREPARE_TIME) / BATTLE_TIME;
        m_nextTime = startTime + FIRST_PREPARE_TIME + (corr + 1) * BATTLE_TIME;
    }
    if(curtime < m_nextTime)
        return;

    handleBattle();
    ++ m_round;
    if(0 == (m_round % 20))
    {
        m_round = 0;
        endOneRound();
        m_nextTime += PREPARE_TIME;
    }
    else
    {
        m_nextTime += BATTLE_TIME;
    }

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
    m_expTime = 0;
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

void ClanCity::endOneRound()
{
    for(int i = 0; i < 6; ++ i)
    {
        m_spots[i].end();
    }
}

void ClanCity::doClanSkill(CCBPlayer* pl)
{
    if(!pl)
        return;

    Clan* cl = pl->player->getClan();
    if(!cl)
        return;

    CCBClanMap::iterator it = m_clans.find(cl);
    if(it == m_clans.end())
        return;

    CCBClan* ccl = it->second;
    ccl->doSkill(pl);
}

void ClanCity::checkAddExp(UInt32 curtime)
{
    if(m_expTime > curtime)
        return;
    CCBPlayerMap::iterator iter = m_players.begin();
    while(iter != m_players.end())
    {
        //计算经验
        Player * player = iter->first;
        if(player && iter->second->type != 1)
        {
            UInt8 plvl = player->GetLev();
            UInt32 exp = 16 * ((plvl - 10) * ((plvl > 99 ? 99 : plvl) / 10) * 5 + 25);
            if (cfg.rpServer && player->GetLev() < 70)
                exp *= 2;
            player->AddExp(exp);
        }
        ++ iter;
    }
    m_expTime += 60;
}

void ClanCity::loadFromDB()
{
    std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
    char querystr[1024] = {0};
}

}
