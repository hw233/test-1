#include "Config.h"
#include "Common/Stream.h"
#include "ClanCityBattle.h"
#include "CountryBattle.h"
#include "Clan.h"
#include "Player.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "Common/URandom.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "DB/DBExecHelper.h"
#include "GObject/WBossMgr.h"
#include "Common/URandom.h"
#include "Common/TimeUtil.h"
#include "MsgID.h"
#include "ClanRankBattle.h"
#include "Server/SysMsg.h"

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

#define CCB_CITY_TYPE_DEF            1 // 防守战
#define CCB_CITY_TYPE_ATK            2 // 攻城战

#define CCB_CITY_MOVE_CD             15

//#define  NEICE_VESION

namespace GObject
{

struct DBClanCity 
{
    UInt8 type;
    UInt8 round;
    UInt32 openTime;
    UInt32 startTime;
    UInt32 endTime;
    UInt32 defClanId;
};

struct DBCCBSpot
{
    UInt8 id;
    UInt16 hp;
    UInt8 canAtk;
    UInt8 skill_dmg0;
    UInt8 skill_action0;
    UInt8 skill_bomb0;
    UInt8 skill_dmg1;
    UInt8 skill_action1;
    UInt8 skill_bomb1;
};

struct DBCCBClan
{
    UInt32 clanId;
    UInt32 score;
    UInt8 skill_hp;
    UInt8 skill_atk;
    UInt8 skill_action;
};

struct DBCCBPlayer
{
    UInt64 playerId;
    UInt8 side;
    UInt8 entered;
    UInt8 win;
    UInt8 dead;
    UInt8 realive;
    UInt32 score;
    UInt32 realivecd;
    UInt32 weary;
};

}

namespace DB
{

SPECIALBEGIN(GObject::DBClanCity)
SPECIALDEF(6)
(
    UInt8, type,
    UInt8, round,
    UInt32, openTime,
    UInt32, startTime,
    UInt32, endTime,
    UInt32, defClanId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCCBSpot)
SPECIALDEF(9)
(
    UInt8, id,
    UInt16, hp,
    UInt8, canAtk,
    UInt8, skill_dmg0,
    UInt8, skill_action0,
    UInt8, skill_bomb0,
    UInt8, skill_dmg1,
    UInt8, skill_action1,
    UInt8, skill_bomb1
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCCBClan)
SPECIALDEF(5)
(
    UInt32, clanId,
    UInt32, score,
    UInt8, skill_hp,
    UInt8, skill_atk,
    UInt8, skill_action
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCCBPlayer)
SPECIALDEF(9)
(
    UInt64, playerId,
    UInt8, side,
    UInt8, entered,
    UInt8, win,
    UInt8, dead,
    UInt8, realive,
    UInt32, score,
    UInt32, realivecd,
    UInt32, weary
)
SPECIALEND()

}

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

static int deadcd[11] = {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60};

static UInt32 npc_num[20][7] = {
    { 10,   0,   0,   0,   0,   0,  10},
    { 20,   5,   0,   0,   0,   0,  25},
    { 30,  10,   2,   0,   0,   0,  42},
    { 40,  15,   4,   0,   0,   0,  59},
    { 50,  20,   6,   1,   0,   0,  76},
    { 60,  25,   8,   0,   0,   0,  93},
    { 70,  30,  10,   0,   0,   0, 110},
    { 80,  35,  12,   0,   0,   0, 127},
    { 90,  40,  14,   0,   0,   0, 144},
    {100,  45,  16,   0,   1,   0, 161},
    { 80,  50,  18,   0,   0,   0, 148},
    { 60,  60,  20,   0,   0,   0, 140},
    { 40,  70,  30,   0,   0,   0, 140},
    { 20,  80,  40,   0,   0,   0, 140},
    {  0,  90,  50,   0,   0,   1, 140},
    {  0, 100,  60,   2,   0,   0, 160},
    {  0,  80,  80,   0,   2,   0, 160},
    {  0,  60, 100,   0,   0,   2, 160},
    {  0,  40, 120,   1,   1,   1, 160},
    {  0,  20, 140,   2,   2,   2, 160},
};

static UInt32 npc_id[6] = {9005, 9006, 9007, 9008, 9009, 9010};
static float npc_atk_factor[6] = {0.01, 0.03, 0.05, 0.1, 0.5, 1.0};
static float npc_hp_factor[6] = {0.001, 0.003, 0.005, 0.005, 0.01, 0.02};

static UInt32 spot_score[2][6] = {
    {  20,   10,   10,  5, 5,   5},
    {300, 200, 200, 100, 100, 100}
};

static bool isBoss(UInt32 npcid)
{
    for(int i = 3; i < 6; ++ i)
    {
        if(npcid == npc_id[i])
            return true;
    }

    return false;
}

ClanCity* gClanCity = NULL;




bool bpGreater::operator() (const CCBClan* first, const CCBClan* second)
{
    if(first->score == second->score)
        return first->clan->getId() < second->clan->getId();
    return first->score > second->score;
}

void CCBPlayer::writeToDB()
{
    if(type != e_player)
        return;
    DB1().PushUpdateData("REPLACE INTO `clancity_player` (`playerId`, `side`, `entered`, `win`, `dead`, `realive`, `score`, `realivecd`, `weary`) VALUES (%" I64_FMT "u, %u, %u, %u, %u, %u, %u, %u, %u)", fgt.player->getId(), side, entered, win, dead, realive, score, realivecd, weary);
}

void CCBPlayer::sendInfo()
{
    if(type != e_player)
        return;

    Stream st(REP::CCB);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(1);
    UInt8 cdtime = 0;
    UInt32 now = TimeUtil::Now();
    if(now < realivecd)
        cdtime = realivecd - now;
    st << realive << static_cast<UInt8>(cdtime);
    st << side << static_cast<UInt8>(pos + 1);
    st << Stream::eos;

    fgt.player->send(st);
}

bool CCBPlayer::challenge(CCBPlayer* other, UInt32& bid)
{
    if(!other)
        return true;

    bid = 0;
    bool ret = false;

    if(other->type == e_player && other->entered == 0)
    {
        ret = true;
    }
    else if(type == e_player && entered == 0)
    {
        ret = false;
    }
    else if(type == e_skill)
    {
        ret = fgt.skill->doSkillEffect(other);
    }
    else if(other->type == e_skill)
    {
        ret = !(other->fgt.skill->doSkillEffect(this));
    }
    else if(type == e_player)
    {
        if(other->type == e_player)
            ret = attackPlayer(other, bid);
        else
            ret = attackNpc(other, bid);
    }
    else if(other->type == e_player)
    {
        ret = !(other->attackNpc(this, bid));
    }

    if(ret)
    {
        if(type == e_player)
            ++ win;
        other->weary = 0;
        if(other->type == e_player)
        {
            win = 0;
            ++ other->dead;
            if(other->entered != 0)
                other->fgt.player->autoRegenAll();
        }
    }
    else
    {
        if(other->type == e_player)
            ++ other->win;
        if(type == e_player)
        {
            win = 0;
            ++ dead;
            if(entered != 0)
                fgt.player->autoRegenAll();
        }
        weary = 0;
    }


    if(type == e_player)
    {
        if(entered != 0)
            fgt.player->clearHIAttr();
        writeToDB();
    }
    if(other->type == e_player)
    {
        if(other->entered != 0)
            other->fgt.player->clearHIAttr();
        other->writeToDB();
    }

    return ret;
}

bool CCBPlayer::attackNpc(CCBPlayer* npc, UInt32& bid)
{
    if(!npc)
        return true;
    GData::NpcGroups::iterator it = GData::npcGroups.find(npc->fgt.npcId);
    if(it == GData::npcGroups.end())
		return true;

    Player* player = fgt.player;
    float factor = 1.0f - static_cast<float>(weary)*0.009f;
    if(factor < 0.001f)
        factor = 0;
    player->setSpiritFactor(factor);
    player->setHiAttrFlag(true);

	GData::NpcGroup * ng = it->second;
    Battle::BattleSimulator bsim(Battle::BS_NEWCOUNTRYBATTLE, player, ng->getName(), ng->getLevel(), true);
    fgt.player->PutFighters( bsim, 0 );
    ng->putFighters( bsim );

    std::vector<GData::NpcFData>& nflist = ng->getList();
    GData::NpcFData& nfdata = nflist[0];
    Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);

    nflist[0].fighter->setWBoss(true);
    int i = 0;
    for(; i < 6; ++ i)
    {
        if(npc->fgt.npcId == npc_id[i])
            break;
    }

    nflist[0].fighter->setBaseHP(worldBoss.getLastHP(0) * npc_hp_factor[i]);
    nflist[0].fighter->setExtraAttack(worldBoss.getLastAtk(0) * npc_atk_factor[i]);
    nflist[0].fighter->setExtraMagAttack(worldBoss.getLastMAtk(0) * npc_atk_factor[i]);
    if(npc->weary != 0)
    {
        bf->setHP(npc->weary);
    }

    bsim.start();
    player->setHiAttrFlag(false);
    player->setSpiritFactor(1.0f);
    bid = bsim.getId();

    bool res = bsim.getWinner() == 1;
    if(res)
    {
        bsim.applyFighterHP(0, player, false, false);
        weary += 2 + 50 * bsim.getLostHPPercent(0, player);
        if(weary > 100)
            weary = 100;
    }
    else
    {
        npc->weary = bf->getHP();
    }

    return res;
}

bool CCBPlayer::attackPlayer(CCBPlayer* other, UInt32& bid)
{
    if(!other)
        return true;

    Player* player1 = fgt.player;
    Player* player2 = other->fgt.player;

    float factor = 1.0f - static_cast<float>(weary)*0.009f;
    if(factor < 0.001f)
        factor = 0;
    player1->setSpiritFactor(factor);

    factor = 1.0f - static_cast<float>(other->weary)*0.009f;
    if(factor < 0.001f)
        factor = 0;
    player2->setSpiritFactor(factor);
    player1->setHiAttrFlag(true);
    player2->setHiAttrFlag(true);

    Battle::BattleSimulator bsim(Battle::BS_NEWCOUNTRYBATTLE, player1, player2);
    player1->PutFighters( bsim, 0);
    player2->PutFighters( bsim, 1);

    bsim.start();
    bid = bsim.getId();
    player1->setSpiritFactor(1.0f);
    player2->setSpiritFactor(1.0f);
    player1->setHiAttrFlag(false);
    player2->setHiAttrFlag(false);

    bool res = bsim.getWinner() == 1;
    if(res)
    {
        bsim.applyFighterHP(0, player1, false, false);
        weary += 2 + 50 * bsim.getLostHPPercent(0, player1);
        if(weary > 100)
            weary = 100;
    }
    else
    {
        bsim.applyFighterHP(1, player2, false, false);
        other->weary += 2 + 50 * bsim.getLostHPPercent(1, player2);
        if(other->weary > 100)
            other->weary = 100;
    }

    return res;
}

void CCBPlayer::addReAliveCD()
{
    UInt8 idx = 0;

    if(dead  > 10)
        idx = 10;
    else
        idx = dead - 1;
    realivecd = TimeUtil::Now() + deadcd[idx];
    writeToDB();
}

bool CCBPlayer::inReAliveCD()
{
    return TimeUtil::Now() < realivecd;
}

CCBClan::CCBClan(Clan* cl) : clan(cl), score(0)
{
    skill_hp.id = CCB_SKILL_HP;
    skill_atk.id = CCB_SKILL_ATK;
    skill_action.id = CCB_SKILL_ACTION_1;
}

void CCBClan::writeToDB()
{
    DB1().PushUpdateData("REPLACE INTO `clancity_clan` (`clanId`, `score`, `skill_hp`, `skill_atk`, `skill_action`) VALUES (%u, %u, %u, %u, %u)", clan->getId(), score, skill_hp.lvl, skill_atk.lvl, skill_action.lvl);
}

void CCBClan::doSkill(CCBPlayer* pl)
{
    if(!pl)
        return;
    skill_hp.doSkillEffect(pl);
    skill_atk.doSkillEffect(pl);
    skill_action.doSkillEffect(pl);
}

void CCBClan::sendInfo(CCBPlayer* pl)
{
    if(!pl)
        return;

    Stream st(REP::CCB);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(2);
    Player* leader = clan->getLeader();
    if(leader)
        st << leader->getName();
    else
        st << "";
    st << score << pl->score;
    st << skill_hp.lvl;
    st << skill_atk.lvl;
    st << skill_action.lvl;
    st << Stream::eos;

    pl->fgt.player->send(st);
}

CCBSpot::CCBSpot() : id(0), canAtk(false), hp(0), clancity(NULL)
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

void CCBSpot::writeToDB()
{
    DB1().PushUpdateData("REPLACE INTO `clancity_spot` (`id`, `hp`, `canAtk`, `skill_dmg0`, `skill_action0`, `skill_bomb0`, `skill_dmg1`, `skill_action1`, `skill_bomb1`) VALUES (%u, %u, %u, %u, %u, %u, %u, %u, %u)", id, hp, (canAtk ? 1 : 0), skill_dmg[0].lvl, skill_action[0].lvl, skill_bomb[0].lvl, skill_dmg[1].lvl, skill_action[1].lvl, skill_bomb[1].lvl);
}

void CCBSpot::Init()
{
    hp = 1000;
    writeToDB();
}

void CCBSpot::clear()
{
    canAtk = false;
    for(int i = 0; i < 2; ++ i)
    {
        waiters[i].clear();
        alive[i].clear();
        dead[i].clear();
        battler[i].clear();
        skill_dmg[i].clear();
        skill_action[i].clear();
        skill_bomb[i].clear();
    }

    for(CCBPlayerList::iterator it = npcs.begin(); it != npcs.end(); ++ it)
    {
        CCBPlayer* pl = *it;
        delete pl;
    }
    npcs.clear();
    writeToDB();
}

bool CCBSpot::canMoveTo()
{
    return ((canAtk && hp != 0) || id == 1 || id == 7);
}

void CCBSpot::reAlive(CCBPlayer* pl, bool force)
{
    if(!pl)
        return;
    UInt8 side = pl->side;
    if(id == 1)
    {
        if(erasePl(dead[side], pl))
            waiters[side].push_back(pl);
    }

    if(force)
        ++ pl->realive;
    pl->realivecd = 0;
    pl->writeToDB();
    pl->sendInfo();
}

void CCBSpot::playerEnter(CCBPlayer* pl)
{
    if(!pl)
        return;

    UInt8 side = pl->side;
    pl->pos = id - 1;
    waiters[side].push_back(pl);

    if(canAtk && clancity->getStatus() == 1 && pl->type == e_player)
    {
        Stream st(REP::CCB);
        makeAliveInfo(st);
        pl->fgt.player->send(st);
    }
}

void CCBSpot::sendAliveInfo(CCBPlayer* pl)
{
    if(canAtk && clancity->getStatus() == 1 && pl->type == e_player)
    {
        Stream st(REP::CCB);
        makeAliveInfo(st);
        pl->fgt.player->send(st);
    }
}

void CCBSpot::send(CCBPlayerList& list, Stream& st)
{
    size_t cnt = list.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        CCBPlayer* pl = list[i];
        if(pl->type == e_player)
            pl->fgt.player->send(st);
    }
}

void CCBSpot::broadcast(Stream& st)
{
    for(int i = 0; i < 2; ++ i)
    {
        send(waiters[i], st);
        send(alive[i], st);
        send(dead[i], st);
        send(battler[i], st);
    }
}

bool CCBSpot::erasePl(CCBPlayerList& list, CCBPlayer* pl)
{
    if(!pl)
        return false;
    CCBPlayerList::iterator it = std::find(list.begin(), list.end(), pl);
    if(it == list.end())
        return false;
    list.erase(it);
    return true;
}

void CCBSpot::handleAllDead()
{
    for(int i = 0; i < 2; ++ i)
    {
        size_t cnt = dead[i].size();
        for(size_t j = 0; j < cnt; ++ j)
        {
            CCBPlayer* pl = dead[i][j];
            if(pl->type == e_player)
            {
                pl->addReAliveCD();
                if(id == 1 && i == 0)
                    pl->sendInfo();
            }
        }
    }
    if(id != 1)
        moveAllToHome(dead[0], 1);
    moveAllToHome(dead[1], 7);
}

void CCBSpot::moveAllToHome(CCBPlayerList& list, UInt8 spot)
{
    if(spot != 1 && spot != 7)
        return;

    size_t cnt = list.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        CCBPlayer* pl = list[i];
        if(pl->type == e_player)
        {
            clancity->forceEnter(pl, spot);
        }
        else if(pl->type == e_npc)
        {
            pl->fgt.npcId = 0;
            npcs.push_back(pl);
        }
    }
    list.clear();
}

void CCBSpot::fillBoss(CCBPlayerList& dst, CCBPlayerList& src)
{
    size_t dstcnt = dst.size();
    size_t srccnt = src.size();
    size_t i = 0;
    for(; i < dstcnt; ++ i)
    {
        CCBPlayer* pl = dst[i];
        if(pl->type != e_npc || !isBoss(pl->fgt.npcId))
            break;
    }
    while(srccnt > 0)
    {
        CCBPlayer* pl = src[0];
        if(pl->type != e_npc || !isBoss(pl->fgt.npcId))
            break;
        dst.insert(dst.begin() + i, pl);
        src.erase(src.begin());
        -- srccnt;
    }

    dstcnt = dst.size();
    if(dstcnt > 100)
    {
        for(size_t i = dstcnt - 1; i > 99; -- i)
        {
            CCBPlayer* pl = dst[i];
            pl->fgt.npcId = 0;
            npcs.push_back(pl);
            dst.erase(dst.begin() + i);
        }
    }
}

void CCBSpot::fillNpc(CCBPlayerList& dst, CCBPlayerList& src)
{
    size_t dstcnt = dst.size();
    size_t srccnt = src.size();
    while(srccnt > 0)
    {
        CCBPlayer* pl = src[0];
        if(pl->type != e_npc || pl->fgt.npcId == 0)
            break;
        dst.push_back(pl);
        src.erase(src.begin());
        -- srccnt;
    }

    dstcnt = dst.size();
    if(dstcnt > 100)
    {
        for(size_t i = dstcnt - 1; i > 99; -- i)
        {
            CCBPlayer* pl = dst[i];
            npcs.push_back(pl);
            dst.erase(dst.begin() + i);
        }
    }
}

void CCBSpot::fillTo100(CCBPlayerList& dst, CCBPlayerList& src, CCBPlayer* bomb)
{
    size_t cnt = dst.size();
    if(100 - cnt > src.size())
    {
        size_t cnt = src.size();
        for(size_t i = 0; i < cnt; ++ i)
        {
            dst.push_back(src[i]);
        }
        src.clear();
    }
    else if(100 - cnt > 0)
    {
        size_t cnt2 = 100 - cnt;
        if(bomb != NULL)
            -- cnt;
        size_t i = 0;
        for(; i < cnt2; ++ i)
        {
            dst.push_back(src[i]);
        }
        src.erase(src.begin(), src.begin()+i);
    }
    if(bomb != NULL)
    {
        size_t cnt = dst.size();
        UInt32 rnd = uRand(cnt);
        dst.insert(dst.begin() + rnd, bomb);
    }
}

void CCBSpot::flushNpc()
{
    if(!canAtk || hp == 0)
        return;

    skill_dmg[1].lvl = 100;
    UInt8 round = clancity->getRound();
    if(round > 19)
        round = 19;

    UInt32 npcs_size = npcs.size();
    UInt32 npcs_idx = 0;

    // 找boss
    for(UInt32 idx = 3; idx < 6; ++ idx)
    {
        for(UInt32 j = 0; j < npc_num[round][idx]; ++ j)
        {
            if(npcs_size > 0)
            {
                npcs[npcs_idx]->fgt.npcId = npc_id[idx];
                npcs[npcs_idx]->weary = 0;
                -- npcs_size;
                ++ npcs_idx;
            }
            else
            {
                CCBPlayer* pl = new CCBPlayer(e_npc);
                pl->fgt.npcId = npc_id[idx];
                pl->side = 1;
                pl->pos = id - 1;
                npcs.push_back(pl);
            }
        }
    }
    fillBoss(waiters[1], npcs);

    npcs_size = npcs.size();
    npcs_idx = 0;
    UInt32 total = npc_num[round][6]/2;
    UInt32 num = uRand(total) + total;
    UInt32 max_flush_num = 100 - alive[1].size();
    if(num > max_flush_num)
        num = max_flush_num;

    for(UInt32 i = 0; i < num; ++ i)
    {
        // 找兵
        UInt32 roll = uRand(num);
        for(UInt32 idx = 0; idx < 3; ++ idx)
        {
            if(roll < npc_num[round][idx])
            {
                if(npcs_size > 0)
                {
                    npcs[npcs_idx]->fgt.npcId = npc_id[idx];
                    npcs[npcs_idx]->weary = 0;
                    -- npcs_size;
                    ++ npcs_idx;
                    break;
                }
                else
                {
                    CCBPlayer* pl = new CCBPlayer(e_npc);
                    pl->fgt.npcId = npc_id[idx];
                    pl->side = 1;
                    pl->pos = id - 1;
                    npcs.push_back(pl);
                    break;
                }
            }
            else
            {
                roll -= npc_num[round][idx];
            }
        }
    }

    fillNpc(waiters[1], npcs);
}

bool CCBSpot::playerLeave(CCBPlayer* pl)
{
    if(!pl)
        return false;
    UInt8 side = pl->side;
    bool res = erasePl(waiters[side], pl);
    if(pl->type == e_player && res)
    {
        Stream st(REP::CCB);
        st << static_cast<UInt8>(0) << static_cast<UInt8>(10);
        st << id << static_cast<UInt8>(0) << side;
        st << pl->fgt.player->getId();
        st << pl->fgt.player->getName();
        st << pl->weary;
        st << Stream::eos;
        broadcast(st);
    }

    return res;
}

void CCBSpot::playerEscape(CCBPlayer* pl)
{
    if(!pl)
        return;

    UInt8 side = pl->side;
    pl->entered = 0;
    erasePl(dead[side], pl);
    bool res = erasePl(waiters[side], pl);
    if(pl->type == e_player && res)
    {
        Stream st(REP::CCB);
        st << static_cast<UInt8>(0) << static_cast<UInt8>(10);
        st << id << static_cast<UInt8>(0) << side;
        st << pl->fgt.player->getId();
        st << pl->fgt.player->getName();
        st << pl->weary;
        st << Stream::eos;
        broadcast(st);
    }
}

void CCBSpot::prepare()
{
    checkPlayers();

    if(hp == 0 || !canAtk)
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

    if(id != 1)
    {
        dead[0].clear();
    }
    else
    {
        size_t cnt = dead[0].size();
        for(size_t j = 0; j < cnt; ++ j)
        {
            CCBPlayer* pl = dead[0][j];
            if(pl->type == e_player)
            {
                if(!pl->inReAliveCD())
                {
                    waiters[0].push_back(pl);
                    dead[0].erase(dead[0].begin() + j);
                    -- cnt;
                    -- j;
                    if(cnt == 0)
                        break;
                }
            }
        }
    }
    dead[1].clear();

    fillTo100(alive[0], waiters[0], bomb0);
    if(clancity->getType() == CCB_CITY_TYPE_DEF)
        fillTo100(alive[1], waiters[1], NULL);
    else
        fillTo100(alive[1], waiters[1], bomb1);

    Stream st(REP::CCB);
    makeAliveInfo(st);
    broadcast(st);
}

void CCBSpot::makeAliveInfo(Stream& st)
{
    st << static_cast<UInt8>(0) << static_cast<UInt8>(6) << static_cast<UInt8>(0);
    for(int i = 0; i < 2; ++ i)
    {
        UInt16 cnt = alive[i].size();
        size_t offset = st.size();
        st << cnt;
        for(size_t j = 0; j < cnt; ++ j)
        {
            CCBPlayer* pl = alive[i][j];
            if(pl->type == e_player)
            {
                Player* player = pl->fgt.player;
                st << static_cast<UInt8>(1);
                st << player->getName();
                st << pl->weary;
                st << player->getCountry();
                st << static_cast<UInt8>(player->getMainFighter()->getId());
            }
            else if(pl->type == e_npc)
            {
                st << static_cast<UInt8>(3);
                st << pl->fgt.npcId;
            }
            else if(pl->type == e_skill)
            {
                st << static_cast<UInt8>(0);
                st << pl->fgt.skill->id;
            }
            else
            {
                -- cnt;
            }
        }
        st.data<UInt16>(offset) = cnt;
    }
    UInt8 round = clancity->getRecycleRound();
    UInt16 time = (20 - round) * BATTLE_TIME;
    st << hp << time;
    st << Stream::eos;
}

CCBPlayer* CCBSpot::popOnePlayer(CCBPlayerList& list)
{
    if(list.size() == 0)
        return NULL;
    CCBPlayer* pl = list[0];
    list.erase(list.begin());

    return pl;
}

void CCBSpot::handleBattle()
{
    if(hp == 0 || !canAtk)
        return;

    for(size_t i = 0; i < 2; ++ i)
    {
        size_t bnum = battler[i].size();
        for(size_t j = bnum; j < 5; ++ j)
        {
            CCBPlayer* pl = popOnePlayer(alive[i]);
            if(!pl)
                break;
            battler[i].push_back(pl);
        }
    }
    if(battler[1].size() == 0) 
        return;

    UInt8 round = clancity->getRecycleRound();
    Stream st(REP::CCB);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(6) << static_cast<UInt8>(1) << round;
    UInt8 cnt = 0;
    size_t offset = st.size();
    st << cnt;

    CCBPlayerList b0;
    CCBPlayerList b1;
    for(size_t i = 0; i < 5; ++ i)
    {
        CCBPlayer* pl0 = popOnePlayer(battler[0]);
        CCBPlayer* pl1 = popOnePlayer(battler[1]);
        if(pl0 != NULL)
        {
            if(pl1 != NULL)
            {
                if(pl0->entered != 0)
                {
                    skill_action[0].doSkillEffect(pl0);
                    clancity->doClanSkill(pl0);
                }
                if(pl1->entered != 0)
                {
                    skill_action[1].doSkillEffect(pl1);
                    clancity->doClanSkill(pl1);
                }
                UInt32 bid = 0;

                if(pl0->challenge(pl1, bid))
                {
                    UInt32 weary = 0;
                    if(pl0->type == e_player)
                        weary = pl0->weary;
                    st << bid << static_cast<UInt8>(0) << static_cast<UInt32>(hp) << weary;
                    if(pl1->entered != 0)
                        dead[1].push_back(pl1);
                    if(pl0->entered != 0)
                        b0.push_back(pl0);
                    clancity->giveScore(pl0, pl0->getWinScore());
                    clancity->giveScore(pl1, pl1->getLostScore());
                }
                else
                {
                    UInt32 weary = 0;
                    if(pl1->type == e_player)
                        weary = pl1->weary;
                    st << bid << static_cast<UInt8>(1) << static_cast<UInt32>(hp) << weary;
                    if(pl0->entered != 0)
                        dead[0].push_back(pl0);
                    if(pl1->entered != 0)
                    {
                        skill_dmg[1].doSkillEffect(*this);
                        b1.push_back(pl1);
                    }
                    clancity->giveScore(pl1, pl1->getWinScore());
                    clancity->giveScore(pl0, pl0->getLostScore());
                }
            }
            else
            {
                UInt32 weary = 0;
                if(pl0->type == e_player)
                    weary = pl0->weary;
                if(pl0->entered != 0)
                    b0.push_back(pl0);
                st << static_cast<UInt32>(0) << static_cast<UInt8>(0);
                st << static_cast<UInt32>(hp) << weary;
                break;
            }
        }
        else if(pl1 != NULL)
        {
            makeDamage(10);
            if(pl1->entered != 0)
            {
                dead[1].push_back(pl1);
                clancity->giveScore(pl1, pl1->getWinScore());
            }
            if(pl1->type == e_player)
            {
                pl1->weary = 0;
                pl1->win = 0;
                ++ pl1->dead;
                pl1->fgt.player->autoRegenAll();
            }
            pl1->writeToDB();
            st << static_cast<UInt32>(0) << static_cast<UInt8>(0);
            st << static_cast<UInt32>(hp) << static_cast<UInt32>(0);
        }
        else
        {
            break;
        }

        ++ cnt;
        if(hp == 0)
            break;
    }

    {
        int i = 0;
        CCBPlayer* pl = popOnePlayer(b0);
        while(pl)
        {
            battler[0].insert(battler[0].begin() + i, pl);
            pl = popOnePlayer(b0);
            ++ i;
        }
    }
    {
        int i = 0;
        CCBPlayer* pl = popOnePlayer(b1);
        while(pl)
        {
            battler[1].insert(battler[1].begin() + i, pl);
            pl = popOnePlayer(b1);
            ++ i;
        }
    }

    st.data<UInt8>(offset) = cnt;
    st << Stream::eos;
    broadcast(st);
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
    writeToDB();
}

void CCBSpot::end()
{
    checkPlayers();

    if(!canAtk)
    {
        if(hp > 0)
            clancity->giveAllScore(0, spot_score[0][id-1]);
        return;
    }

    for(int i = 0; i < 2; ++ i)
    {
        CCBPlayer* pl = popOnePlayer(battler[i]);
        int pos = 0;
        while(pl)
        {
            if(hp == 0 && pl->type == e_player)
                pl->fgt.player->autoRegenAll();

            waiters[i].insert(waiters[i].begin()+pos, pl);
            pl = popOnePlayer(battler[i]);
            ++ pos;
        }
        battler[i].clear();
        pl = popOnePlayer(alive[i]);
        while(pl)
        {
            waiters[i].insert(waiters[i].begin()+pos, pl);
            pl = popOnePlayer(alive[i]);
            ++ pos;
        }
    }

    if(hp == 0)
    {
        clancity->giveAllScore(1, spot_score[1][id-1]);
        canAtk = false;
        writeToDB();
        handleAllDead();
        if(id != 1)
        {
            moveAllToHome(alive[0], 1);
            moveAllToHome(alive[1], 7);
            moveAllToHome(waiters[0], 1);
            moveAllToHome(waiters[1], 7);
            clancity->openNextSpot(id);
        }
    }
    else
    {
        handleAllDead();
        clancity->giveAllScore(0, spot_score[0][id-1]);
    }

    erasePl(waiters[0], &(bomb[0]));
    erasePl(waiters[1], &(bomb[1]));
}

void CCBSpot::checkLeavePlayer(CCBPlayerList& list)
{
    size_t cnt = list.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        CCBPlayer* pl = list[i];
        if(pl->type == e_player && pl->entered == 0)
        {
            list.erase(list.begin() + i);
            -- i;
            -- cnt;
            if(cnt == 0)
                break;
        }
    }
}

void CCBSpot::checkPlayers()
{
    for(int i = 0; i < 2; ++ i)
    {
        checkLeavePlayer(waiters[i]);
        checkLeavePlayer(alive[i]);
        checkLeavePlayer(battler[i]);
        checkLeavePlayer(dead[i]);
    }
}

UInt32 CCBSpot::getDefCount()
{
    return waiters[0].size() + alive[0].size() + battler[0].size();
}

UInt32 CCBSpot::getAtkCount()
{
    return waiters[1].size() + alive[1].size() + battler[1].size();
}

void CCBSkill::doSkillEffect(CCBSpot& spot)
{
    if(lvl == 0)
        return;
    if(id == CCB_SKILL_SPOT_EXTRA_DMG)
    {
        if(uRand(1000) < 5*lvl)
        {
            spot.makeDamage(6);
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
        attr.attackP = 0.1f * lvl;
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

ClanCity::ClanCity(UInt16 loc) : m_loc(loc), m_openTime(0xFFFFFFFF), m_nextTime(0), m_expTime(0), m_type(0), m_recycle_round(0), m_round(0), m_status(0), m_defClanId(0), m_openFlag(false)
{
    for(int i = 0; i < 7; ++ i)
    {
        m_spots[i].clancity = this;
        m_spots[i].id = i+1;
        if(i > 2 && i < 6)
            m_spots[i].canAtk = true;
        else
            m_spots[i].canAtk = false;
    }
}

void ClanCity::writeToDB()
{
    DB1().PushUpdateData("REPLACE INTO `clancity` (`id`, `type`, `round`, `openTime`, `startTime`, `endTime`, `defClanId`) VALUES (1, %u, %u, %u, %u, %u, %u)", m_type, m_round, m_openTime, m_startTime, m_endTime, m_defClanId);
}

bool ClanCity::isOpen()
{
    /*
    //XXX
    UInt32 now = TimeUtil::Now();
    UInt32 today = TimeUtil::SharpDayT(0);
    if(today + 30*60 <= now && now < today + 3600+30*60)
        return true;
    else
        return false;
    */
#ifdef NEICE_VESION
    return true;
#endif

    return (m_openFlag &&(TimeUtil::Now() > m_openTime) && (World::_wday > 5));
}

void ClanCity::process(UInt32 curtime)
{
    if(!isOpen())
        return;

    if(m_startTime == 0)
    {
#ifndef NEICE_VESION
        if(cfg.GMCheck)
            m_startTime = TimeUtil::SharpDay(0) + 20 * 60 * 60;
        else
#endif
            m_startTime = curtime + 30;
        m_endTime = m_startTime + 30 * 60;
        /*XXX
        if(m_type == 0 || m_type == CCB_CITY_TYPE_DEF)
            m_startTime = TimeUtil::SharpDay(0) + 30 * 60;
        else
            m_startTime = TimeUtil::SharpDay(0) + 3600;

        if(curtime > m_endTime)
            return;
            */
    }

    if(curtime < m_startTime)
        return;

    if(m_nextTime == 0)
        prepare();

    if(m_spots[0].hp == 0)
        return;
    if(m_expTime)
        checkAddExp(curtime);

    if(!m_expTime)
        m_expTime = m_startTime + 60;

    UInt8 oldstatus = m_status;
    if(curtime < m_nextTime)
        return;

    if(curtime <= m_endTime || m_recycle_round != 0)
    {
        if(m_recycle_round == 0)
        {
            ++ m_round;
            prepareOneRound();
        }
        ++ m_recycle_round;
        m_status = 1;
        handleBattle();
        m_nextTime += BATTLE_TIME;

        if(0 == (m_recycle_round % 20))
        {
            m_status = 0;
            m_recycle_round = 0;
            endOneRound();
            prepareNpc();
            m_nextTime += PREPARE_TIME;
        }

        if(oldstatus != m_status || m_spots[0].hp == 0)
        {
            Stream st(REP::CCB);
            makeAllSpotInfo(st);
            broadcast(st);
        }
    }

    if(curtime >= m_endTime || m_spots[0].hp == 0 || (m_round == 20 && m_recycle_round == 0) || m_round > 20)
    {
        end();
        return;
    }
    writeToDB();
}

void ClanCity::prepare()
{
    if(!isOpen())
        return;

	if(globalCountryBattle.getPrepareTime() != 0)
        globalCountryBattle.end();

    UInt32 now = TimeUtil::Now();
    m_expTime = 0;
    //XXX
    if(m_type == 0)
        m_type = CCB_CITY_TYPE_DEF;
    if(World::_wday == 6)
        m_type = CCB_CITY_TYPE_DEF;
    else if(World::_wday == 7)
        m_type = CCB_CITY_TYPE_ATK;

    if(m_type == CCB_CITY_TYPE_ATK && m_defClanId == 0)
    {
        const std::vector<Clan*>& clanRanking = ClanRankBattleMgr::Instance().getClanRanking();
        if(clanRanking.size() > 0)
        {
            GObject::Clan * clan = clanRanking[0];
            if(clan)
                m_defClanId = clan->getId();
        }
        if(m_defClanId == 0)
            m_openFlag = false;
    }


    if(m_round == 0)
    {
        for(int i = 0; i < 7; ++ i)
        {
            m_spots[i].Init();
        }
        openNextSpot(7);
    }

    if(cfg.GMCheck)
        m_nextTime = m_startTime + FIRST_PREPARE_TIME + (m_round-1) * ((20 * BATTLE_TIME) + PREPARE_TIME);
    else
        m_nextTime = m_startTime + PREPARE_TIME;
    prepareNpc();

    Stream st(REP::CCB);
    makeOpenStatusInfo(st);
    st << static_cast<UInt8>(0) << Stream::eos;
    NETWORK()->Broadcast(st);

    if(cfg.GMCheck)
    {
        for(; m_round < 20; ++ m_round)
        {
            if(now > m_nextTime)
                m_nextTime += 20 * BATTLE_TIME;
            else
                break;

            if(now > m_nextTime)
                m_nextTime += PREPARE_TIME;
            else
                break;
        }
    }
    writeToDB();
}

void ClanCity::start()
{
}

void ClanCity::end()
{
	SYSMSG(title, 4920);
    UInt32 leavescore[2] = {0, 0};
    if(m_spots[0].hp == 0)
    {
        leavescore[0] = 50;
        leavescore[1] = 100;
        giveAllScore(0, 50);
        giveAllScore(1, 100);
    }
    else
    {
        leavescore[0] = 100;
        leavescore[1] = 50;
        giveAllScore(0, 100);
        giveAllScore(1, 50);
    }

    Player* udpLogPlayer = NULL;
    for(CCBClanMap::iterator itc = m_clans.begin(); itc != m_clans.end(); ++ itc)
    {
        CCBClan* ccl = itc->second;
        Clan* cl = ccl->clan;
        cl->addStatueExp(ccl->score*10);
    }

    Clan* firstClan = NULL;
    CCBClanSort::iterator it = m_clanSort.begin();
    if(it != m_clanSort.end())
    {
        CCBClan* ccl = *it;
        firstClan = ccl->clan;
    }
    for(CCBPlayerMap::iterator itp = m_players.begin(); itp != m_players.end(); ++ itp)
    {
        CCBPlayer* pl = itp->second;
        Player* player = pl->fgt.player;
        Clan* cl = player->getClan();
        if(!udpLogPlayer)
            udpLogPlayer = player;

        player->setInClanCity(false);
        player->clearHIAttr();
        player->autoRegenAll();
        player->getAchievement(pl->score*2);

        UInt32 clscore = 0;
        CCBClanMap::iterator itc = m_clans.find(cl);
        if(itc != m_clans.end())
        {
            CCBClan* ccl = itc->second;
            clscore = ccl->score;
        }

        UInt32 count = 0;
        if(pl->score >= 500)
            count = 15;
        else if(pl->score >= 200)
            count = 10;
        else if(pl->score >= 100)
            count = 5;
        else
            count = 1;

        MailPackage::MailItem mitem[1] = {{1125, count}};
        MailItemsInfo itemsInfo(mitem, CountryBattleAward, 1);

        SYSMSG(defer, 4923);
        SYSMSG(atker, 4924+(m_type == CCB_CITY_TYPE_DEF ? 1 : 0));
        SYSMSG(atk, 4926);
        SYSMSG(def, 4927);

        char* winner = defer;
        char* jointo = defer;
        char* act = def;
        if(m_spots[0].hp == 0)
            winner = atker;
        const char* clanname = "";
        if(firstClan)
            clanname = firstClan->getName().c_str();
        if(m_type == CCB_CITY_TYPE_DEF)
        {
            if(cl == firstClan)
            {
                jointo = defer;
                act = def;
            }
            else
            {
                jointo = atker;
                act = atk;
            }
        }

        SYSMSGV(content, 4920+m_type, winner, clanname, pl->score, pl->score*2, count, clscore, clscore*10, jointo, act);
        Mail * pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        if(pmail != NULL)
            mailPackageManager.push(pmail->id, mitem, 1, true);
    }
    for(CCBPlayerMap::iterator itp = m_players_leave.begin(); itp != m_players_leave.end(); ++ itp)
    {
        CCBPlayer* pl = itp->second;
        Player* player = pl->fgt.player;
        Clan* cl = player->getClan();
        player->setInClanCity(false);
        pl->score += leavescore[pl->side];
        if(!udpLogPlayer)
            udpLogPlayer = player;

        player->getAchievement(pl->score*2);

        UInt32 count = 0;
        if(pl->score >= 500)
            count = 15;
        else if(pl->score >= 200)
            count = 10;
        else if(pl->score >= 100)
            count = 5;
        else
            count = 1;

        UInt32 clscore = 0;
        CCBClanMap::iterator itc = m_clans.find(cl);
        if(itc != m_clans.end())
        {
            CCBClan* ccl = itc->second;
            clscore = ccl->score;
        }
        MailPackage::MailItem mitem[1] = {{1125, count}};
        MailItemsInfo itemsInfo(mitem, CountryBattleAward, 1);

        SYSMSG(defer, 4923);
        SYSMSG(atker, 4924+(m_type == CCB_CITY_TYPE_DEF ? 1 : 0));
        SYSMSG(atk, 4926);
        SYSMSG(def, 4927);

        char* winner = defer;
        char* jointo = defer;
        char* act = def;
        if(m_spots[0].hp == 0)
            winner = atker;
        const char* clanname = "";
        if(firstClan)
            clanname = firstClan->getName().c_str();
        if(m_type == CCB_CITY_TYPE_DEF)
        {
            if(cl == firstClan)
            {
                jointo = defer;
                act = def;
            }
            else
            {
                jointo = atker;
                act = atk;
            }
        }

        SYSMSGV(content, 4920+m_type, winner, clanname, pl->score, pl->score*2, count, clscore, clscore*10, jointo, act);
        Mail * pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        if(pmail != NULL)
            mailPackageManager.push(pmail->id, mitem, 1, true);
    }

    if(udpLogPlayer != NULL)
    {
        if(m_spots[0].hp == 0)
        {
            if(m_type == CCB_CITY_TYPE_DEF)
            {
                udpLogPlayer->udpLog("moshouchenggui", "F_130723_2", "", "", "", "", "act");
            }
            else
            {
                udpLogPlayer->udpLog("moshouchenggui", "F_130723_4", "", "", "", "", "act");
            }
        }
        else
        {
            if(m_type == CCB_CITY_TYPE_DEF)
            {
                udpLogPlayer->udpLog("moshouchenggui", "F_130723_3", "", "", "", "", "act");
            }
            else
            {
                udpLogPlayer->udpLog("moshouchenggui", "F_130723_5", "", "", "", "", "act");
            }
        }
    }

    Stream st(REP::CCB);
    makeOpenStatusInfo(st);
    st << static_cast<UInt8>(0) << Stream::eos;
    NETWORK()->Broadcast(st);

    if(m_type == CCB_CITY_TYPE_DEF)
    {
        CCBClanSort::iterator it = m_clanSort.begin();
        if(it != m_clanSort.end())
        {
            CCBClan* ccl = *it;
            m_defClanId = ccl->clan->getId();
        }
        m_type = CCB_CITY_TYPE_ATK;
#ifndef NEICE_VESION
        //XXX
        if(cfg.GMCheck)
            m_startTime = TimeUtil::SharpDay(1) + 20 * 60 * 60;
        else
            m_startTime = TimeUtil::Now() + 30;
        m_endTime = m_startTime + 30 * 60;
#endif
    }
    else
    {
        m_type = CCB_CITY_TYPE_DEF;
        m_defClanId = 0;
#ifndef NEICE_VESION
        //XXX
        if(cfg.GMCheck)
            m_startTime = TimeUtil::SharpWeek(1) + 5*86400 + 20 * 60 * 60;
        else
            m_startTime = TimeUtil::Now() + 30;
        m_endTime = m_startTime + 30 * 60;
#endif
    }

    for(CCBPlayerMap::iterator itp = m_players.begin(); itp != m_players.end(); ++ itp)
    {
        CCBPlayer* pl = itp->second;
        delete pl;
    }
    for(CCBPlayerMap::iterator itp = m_players_leave.begin(); itp != m_players_leave.end(); ++ itp)
    {
        CCBPlayer* pl = itp->second;
        delete pl;
    }
    for(CCBClanMap::iterator itc = m_clans.begin(); itc != m_clans.end(); ++ itc)
    {
        CCBClan* ccl = itc->second;
        delete ccl;
    }
    for(int idx = 0; idx < 2; ++ idx)
    {
        for(CCBClanPlayerMap::iterator itcp = m_clanPlayers[idx].begin(); itcp != m_clanPlayers[idx].end(); ++ itcp)
        {
            CCBPlayerList& list = itcp->second;
            list.clear();
        }
        m_clanPlayers[idx].clear();
    }

    m_players.clear();
    m_players_leave.clear();
    m_clans.clear();
    m_clanSort.clear();
    m_recycle_round = 0;
    m_round = 0;
    m_status = 0;
    for(int i = 0; i < 7; ++ i)
    {
        m_spots[i].clear();
    }

    writeToDB();

#ifdef NEICE_VESION
    //XXX
    m_startTime = 0;
    m_endTime = 0;
#endif
    m_nextTime = 0;

    DB1().PushUpdateData("DELETE FROM `clancity_player`");
    DB1().PushUpdateData("DELETE FROM `clancity_clan`");
}

bool ClanCity::playerEnter(Player * player)
{
    if(!player || !isOpen())
        return false;
    Clan* cl = player->getClan();
    if(!cl)
        return false;

    if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
        return false;

	UInt32 curtime = TimeUtil::Now();
    if(curtime < m_startTime || curtime > m_endTime || m_spots[0].hp == 0)
        return false;
	if(player->getBuffData(PLAYER_BUFF_CLAN_CITY, curtime))
	{
		player->sendMsgCode(0, 2300);
		return false;
	}
    player->getSurnameLegendAward(e_sla_cb);
    cl->addMemberActivePoint(player, 15, e_clan_actpt_clan_city);
	player->setInClanCity(true);
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
    {
        it = m_players_leave.find(player);
        UInt8 side = 0;
        if(isClanCityDefender(cl))
            side = 0;
        else
            side = 1;
        CCBPlayer* pl = NULL;
        if(it == m_players_leave.end())
        {
            player->udpLog("moshouchenggui", "F_130723_1", "", "", "", "", "act");
            pl = new CCBPlayer(e_player);
            pl->fgt.player = player;
            pl->side = side;
            pl->entered = 1;
            m_players[player] = pl;
            CCBPlayerList& list = m_clanPlayers[side][cl];
            list.push_back(pl);
            CCBClanMap::iterator itc = m_clans.find(cl);
            if(itc == m_clans.end())
            {
                CCBClan* ccl = new CCBClan(cl);
                m_clans[cl] = ccl;
                m_clanSort.insert(ccl);
                ccl->writeToDB();
            }
        }
        else
        {
            pl = it->second;
            pl->entered = 1;
            m_players[player] = pl;
            CCBPlayerList& list = m_clanPlayers[side][cl];
            list.push_back(pl);
            m_players_leave.erase(player);
        }
        pl->writeToDB();

        if(side == 0)
            m_spots[0].playerEnter(pl);
        else
            m_spots[6].playerEnter(pl);
    }
    else
    {
        CCBPlayer* pl = it->second;
        UInt8 pos = pl->pos;
        m_spots[pos].sendAliveInfo(pl);
    }
    GameAction()->doStrong(player, SthMSCG, 0, 0);  
    return true;
}

bool ClanCity::isClanCityDefender(Clan* cl)
{
    if(!cl)
        return false;
    if(m_type == CCB_CITY_TYPE_ATK)
    {
        if(m_defClanId == cl->getId())
            return true;
        else
            return false;
    }

    return true;
}

bool ClanCity::playerLeave(Player * player)
{
    if(!player)
        return false;
    if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
        return false;

    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
    {
        player->setInClanCity(false);
        return true;
    }

    Clan* cl = player->getClan();
    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    m_spots[pos].playerEscape(pl);
    //if(m_spots[pos].playerLeave(pl))
    {
        UInt32 curtime = TimeUtil::Now();
        UInt8 side = pl->side;
        m_players_leave[player] = pl;
        m_players.erase(player);
        CCBPlayerList& list = m_clanPlayers[side][cl];
        CCBPlayerList::iterator itp = std::find(list.begin(), list.end(), pl);
        if(itp != list.end())
            list.erase(itp);
        pl->writeToDB();

        if(curtime >= m_startTime && curtime < m_endTime)
            player->setBuffData(PLAYER_BUFF_CLAN_CITY, curtime + 5 * 60);

        player->setInClanCity(false);
        player->clearHIAttr();
        player->autoRegenAll();
        return true;
    }

    return false;
}

UInt8 ClanCity::move(Player* player, UInt8 spot)
{
    if(!player)
        return 5;
    if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
        return 5;

    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return 5;
    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    if(spot == pos + 1 || spot > 7 || spot == 0)
        return 5;
    UInt32 now = TimeUtil::Now();
    if((m_nextTime < now + 5) && m_spots[pos].canAtk)
        return 4;
    if(pl->realivecd > now)
        return 3;
    if(pl->movecd > now)
        return 1;
    if(!m_spots[spot-1].canMoveTo())
        return 5;

    if(m_spots[pos].playerLeave(pl))
    {
        m_spots[spot-1].playerEnter(pl);

        UInt8 pos = pl->pos;
        if(pl->type == e_player)
        {
            Stream st(REP::CCB);
            st << static_cast<UInt8>(0);
            st << static_cast<UInt8>(10);
            st << m_spots[pos].id << static_cast<UInt8>(1) << pl->side;
            st << pl->fgt.player->getId();
            st << pl->fgt.player->getName();
            st << pl->weary;
            st << Stream::eos;
            broadcast(st);

            pl->movecd = now + CCB_CITY_MOVE_CD;
        }

        return 0;
    }

    return 2;
}

void ClanCity::move(CCBPlayer* pl, UInt8 spot)
{
    if(!pl)
        return;
    UInt8 pos = pl->pos;
    if(spot == pos + 1 || spot == 0 || spot > 7)
        return;

    UInt32 now = TimeUtil::Now();
    if(pl->realivecd > now)
        return;
    if(pl->movecd > now)
        return;
    if(!m_spots[spot-1].canMoveTo())
        return;

    if(m_spots[pos].playerLeave(pl))
    {
        m_spots[spot-1].playerEnter(pl);

        UInt8 pos = pl->pos;
        if(pl->type == e_player)
        {
            Stream st(REP::CCB);
            st << static_cast<UInt8>(0);
            st << static_cast<UInt8>(10);
            st << m_spots[pos].id << static_cast<UInt8>(1) << pl->side;
            st << pl->fgt.player->getId();
            st << pl->fgt.player->getName();
            st << pl->weary;
            st << Stream::eos;
            broadcast(st);

            pl->movecd = now + CCB_CITY_MOVE_CD;
        }

    }
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
        m_spots[nextPos].writeToDB();
    }
}

void ClanCity::handleBattle()
{
    for(int i = 0; i < 6; ++ i)
    {
        m_spots[i].handleBattle();
    }
}

void ClanCity::prepareNpc()
{
    if(m_type != CCB_CITY_TYPE_DEF)
        return;

    for(int i = 0; i < 6; ++ i)
    {
        m_spots[i].flushNpc();
    }
}

void ClanCity::prepareOneRound()
{
    for(int i = 0; i < 6; ++ i)
    {
        m_spots[i].prepare();
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

    if(pl->type != e_player)
        return;

    Clan* cl = pl->fgt.player->getClan();
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

void ClanCity::sendPlayerInfo(Player* player)
{
    if(!player)
        return;
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;
    CCBPlayer* pl = it->second;
    pl->sendInfo();
}

void ClanCity::sendClanInfo(Player* player)
{
    if(!player)
        return;
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;

    CCBPlayer* pl = it->second;
    Clan* cl = player->getClan();
    if(!cl)
        return;

    CCBClanMap::iterator it2 = m_clans.find(cl);
    if(it2 == m_clans.end())
        return;

    CCBClan* ccl = it2->second;
    ccl->sendInfo(pl);
}

void ClanCity::makeAllSpotInfo(Stream& st)
{
    UInt32 now = TimeUtil::Now();
    st << static_cast<UInt8>(0) << static_cast<UInt8>(3);
    st << m_status << m_round;
    if(m_status == 0)
        st << static_cast<UInt32>(m_nextTime - now);
    else
        st << static_cast<UInt32>(0);
    for(int i = 0; i < 6; ++ i)
    {
        st << m_spots[i].hp;
        st << m_spots[i].getDefCount();
        st << m_spots[i].getAtkCount();
        st << m_spots[i].canAtk;
    }
    st << Stream::eos;
}

void ClanCity::sendAllSpotInfo(Player* player)
{
    if(!player)
        return;

    Stream st(REP::CCB);
    makeAllSpotInfo(st);
    player->send(st);
}

void ClanCity::makeLeaderBoardInfo(Stream& st)
{
    st << static_cast<UInt8>(0) << static_cast<UInt8>(4);
    size_t cnt = m_clanSort.size();
    if(cnt > 3)
        cnt = 3;
    st << static_cast<UInt8>(cnt);
    CCBClanSort::iterator it = m_clanSort.begin();
    for(size_t i = 0; i < cnt; ++ i, ++ it)
    {
        st << (*it)->clan->getName();
        st << (*it)->score;
    }

    st << Stream::eos;
}

void ClanCity::sendLeaderBoard(Player* player)
{
    if(!player)
        return;

    Stream st(REP::CCB);
    makeLeaderBoardInfo(st);
    player->send(st);
}

void ClanCity::sendSelfSpotInfo(Player* player)
{
    if(!player)
        return;
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;

    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    if(pos == 6)
        return;
    if(pos == 0 && !m_spots[0].canAtk)
        return;

    UInt32 prepareTime = 0;
    UInt32 now = TimeUtil::Now();
    if(now < m_nextTime)
        prepareTime = m_nextTime - now;

    Stream st(REP::CCB);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(5);
    st << m_spots[pos].id << m_spots[pos].hp << prepareTime;

    for(int i = 0; i < 2; ++ i)
    {
        UInt32 cnt1 = m_spots[pos].waiters[i].size();
        UInt32 cnt0 = m_spots[pos].alive[i].size();
        UInt32 cnt = 0;
        size_t offset = st.size();
        st << cnt;
        size_t j = 0;
        for(j = 0; j < cnt0; ++ j)
        {
            CCBPlayer* pl = m_spots[pos].alive[i][j];
            if(pl->type == e_player)
            {
                st << static_cast<UInt8>(1);
                st << pl->fgt.player->getId();
                st << pl->fgt.player->getName();
                st << pl->weary;
                ++ cnt;
            }
            else if(pl->type == e_npc)
            {
                st << static_cast<UInt8>(3);
                st << pl->fgt.npcId;
                ++ cnt;
            }
            else if(pl->type == e_skill)
            {
                st << static_cast<UInt8>(0);
                st << pl->fgt.skill->id;
                ++ cnt;
            }
        }
        for(j = 0; j < cnt1; ++ j)
        {
            CCBPlayer* pl = m_spots[pos].waiters[i][j];
            if(pl->type == e_player)
            {
                st << static_cast<UInt8>(1);
                st << pl->fgt.player->getId();
                st << pl->fgt.player->getName();
                st << pl->weary;
                ++ cnt;
            }
            else if(pl->type == e_npc)
            {
                st << static_cast<UInt8>(3);
                st << pl->fgt.npcId;
                ++ cnt;
            }
            else if(pl->type == e_skill)
            {
                st << static_cast<UInt8>(0);
                st << pl->fgt.skill->id;
                ++ cnt;
            }
        }
        st.data<UInt32>(offset) = cnt;
    }

    st << m_spots[pos].skill_dmg[0].lvl;
    st << m_spots[pos].skill_action[0].lvl;
    st << m_spots[pos].skill_bomb[0].lvl;
    st << m_spots[pos].skill_dmg[1].lvl;
    st << m_spots[pos].skill_action[1].lvl;
    st << m_spots[pos].skill_bomb[1].lvl;

    st << Stream::eos;
    player->send(st);
}

void ClanCity::upClanSkill(Player* player, UInt8 sidx)
{
    if(!player || sidx > 2)
        return;
	if(!player->hasChecked())
		return;

    if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
        return;

    if(player->getGold() < 5)
    {
        player->sendMsgCode(0, 1104);
        return;
    }
    Clan* cl = player->getClan();
    if(!cl)
        return;
    CCBPlayerMap::iterator itp = m_players.find(player);
    if(itp == m_players.end())
        return;
    CCBPlayer* pl = itp->second;

    CCBClanMap::iterator it = m_clans.find(cl);
    if(it == m_clans.end())
        return;
    CCBClan* ccl = it->second;

    CCBSkill* skill = NULL;
    if(sidx == 0 && ccl->skill_hp.lvl < 100)
    {
        skill = &(ccl->skill_hp);
    }
    else if(sidx == 1 && ccl->skill_atk.lvl < 100)
    {
        skill = &(ccl->skill_atk);
    }
    else if(sidx == 2 && ccl->skill_action.lvl < 100)
    {
        skill = &(ccl->skill_action);
    }

    if(!skill)
        return;

    ++ skill->lvl;
    ConsumeInfo ci(CCBSkillUP, 0, 0);
    player->useGold(5, &ci);
    ccl->writeToDB();

    Stream st(REP::CCB);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(8);
    st << ccl->skill_hp.lvl;
    st << ccl->skill_atk.lvl;
    st << ccl->skill_action.lvl;
    st << Stream::eos;
    player->send(st);

    UInt8 side = pl->side;
    CCBClanPlayerMap::iterator it2 = m_clanPlayers[side].find(cl);
    if(it2 != m_clanPlayers[side].end())
    {
        CCBPlayerList& players = it2->second;
        for(CCBPlayerList::iterator itp = players.begin(); itp != players.end(); ++ itp)
        {
            CCBPlayer* pl = *itp;
            pl->fgt.player->send(st);
        }
    }
}

static UInt32 realive_gold[6] = {10, 20, 35, 50, 75, 100};

void ClanCity::reAlive(Player* player, UInt8 force)
{
    if(!player)
        return;
    if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
        return;
    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;

    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    if(pos != 0 && pos != 6)
        return;

    UInt32 now = TimeUtil::Now();
    if(pl->realivecd < now)
    {
        m_spots[pos].reAlive(pl, false);
        return;
    }
    if(force == 0)
        return;

    UInt8 idx = pl->realive;
    if(idx > 5)
        idx = 5;
    UInt32 needgold = realive_gold[idx];
    if(player->getGold() < needgold)
    {
        player->sendMsgCode(0, 1104);
        return;
    }

    m_spots[pos].reAlive(pl, true);
    ConsumeInfo ci(CCBReAlive, 0, 0);
    player->useGold(needgold, &ci);
}

void ClanCity::upSpotSkill(Player* player, UInt8 spot, UInt8 sidx)
{
    if(!player || m_status == 1)
        return;
    if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
        return;
    if(player->getGold() < 5)
    {
        player->sendMsgCode(0, 1104);
        return;
    }

    CCBPlayerMap::iterator it = m_players.find(player);
    if(it == m_players.end())
        return;

    CCBPlayer* pl = it->second;
    UInt8 pos = pl->pos;
    if(!m_spots[pos].canAtk || pl->pos != spot - 1)
        return;

    CCBSkill* skill = NULL;
    if(sidx == 0 && m_spots[pos].skill_dmg[pl->side].lvl < 100)
    {
        skill = &(m_spots[pos].skill_dmg[pl->side]);
    }
    else if(sidx == 1 && m_spots[pos].skill_action[pl->side].lvl < 100)
    {
        skill = &(m_spots[pos].skill_action[pl->side]);
    }
    else if(sidx == 2 && m_spots[pos].skill_bomb[pl->side].lvl < 100)
    {
        skill = &(m_spots[pos].skill_bomb[pl->side]);
    }

    if(!skill)
        return;

    ++ skill->lvl;
    ConsumeInfo ci(CCBSkillUP, 0, 0);
    player->useGold(5, &ci);
    m_spots[pos].writeToDB();

    Stream st(REP::CCB);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(9);
    st << m_spots[pos].id;
    st << m_spots[pos].skill_dmg[0].lvl;
    st << m_spots[pos].skill_action[0].lvl;
    st << m_spots[pos].skill_bomb[0].lvl;
    st << m_spots[pos].skill_dmg[1].lvl;
    st << m_spots[pos].skill_action[1].lvl;
    st << m_spots[pos].skill_bomb[1].lvl;
    st << Stream::eos;

    m_spots[pos].broadcast(st);
}

void ClanCity::setOpenTime()
{
    UInt32 now = TimeUtil::Now();
    if(now < (TimeUtil::SharpDayT(0) + 20 * 3600))
        m_openTime = TimeUtil::SharpDayT(0);
    else
        m_openTime = TimeUtil::SharpDayT(1);
    writeToDB();
}

void ClanCity::makeOpenStatusInfo(Stream& st)
{
    st << static_cast<UInt8>(0) << static_cast<UInt8>(7);
    if(m_openTime == 0xFFFFFFFF)
    {
        st << static_cast<UInt8>(0);
    }
    else if(!isOpen())
    {
        st << static_cast<UInt8>(1);
    }
    else
    {
        UInt32 curtime = TimeUtil::Now();
        if(curtime < m_startTime)
        {
            if(m_type == CCB_CITY_TYPE_DEF)
                st << static_cast<UInt8>(1);
            else if(World::_wday == 6)
                st << static_cast<UInt8>(3);
            else
                st << static_cast<UInt8>(2);
        }
        else if(curtime >= m_startTime && curtime < m_endTime)
        {
            if(m_spots[0].hp == 0 || (m_round == 20 && m_recycle_round == 0))
                st << static_cast<UInt8>(3);
            else
                st << static_cast<UInt8>(2);
        }
        else
        {
            st << static_cast<UInt8>(3);
        }
    }
    st << m_type;
}

void ClanCity::sendOpenStatus(Player* pl)
{
    UInt8 entered = 0;
    CCBPlayerMap::iterator it = m_players.find(pl);
    if(it != m_players.end())
        entered = 1;

    Stream st(REP::CCB);
    makeOpenStatusInfo(st);
    st << entered << Stream::eos;
    pl->send(st);
}

void ClanCity::broadcast(Stream& st)
{
    for(CCBPlayerMap::iterator it = m_players.begin(); it != m_players.end(); ++ it)
    {
        CCBPlayer* pl = it->second;
        pl->fgt.player->send(st);
    }
}

void ClanCity::giveScore(CCBPlayer*pl, UInt32 score)
{
    if(!pl || pl->type != e_player)
        return;

    Clan* cl = pl->fgt.player->getClan();
    if(!cl)
        return;

    CCBClanMap::iterator it = m_clans.find(cl);
    if(it == m_clans.end())
        return;

    CCBClan* ccl = it->second;
    pl->score += score;
    pl->writeToDB();
    UInt8 side = pl->side;

    m_clanSort.erase(ccl);
    ccl->score += score;
    ccl->writeToDB();
    CCBClanSort::iterator itres = m_clanSort.insert(ccl).first;
    if(std::distance(m_clanSort.begin(), itres) < 3)
    {
        Stream st(REP::CCB);
        makeLeaderBoardInfo(st);
        broadcast(st);
    }

    CCBClanPlayerMap::iterator it2 = m_clanPlayers[side].find(cl);
    if(it2 != m_clanPlayers[side].end())
    {
        CCBPlayerList& players = it2->second;
        for(CCBPlayerList::iterator itp = players.begin(); itp != players.end(); ++ itp)
        {
            CCBPlayer* pl = *itp;
            ccl->sendInfo(pl);
        }
    }
}

void ClanCity::giveAllScore(UInt8 side, UInt32 score)
{
    for(CCBClanPlayerMap::iterator itcp = m_clanPlayers[side].begin(); itcp != m_clanPlayers[side].end(); ++ itcp)
    {
        Clan* cl = itcp->first;
        CCBClan* ccl = NULL;
        CCBClanMap::iterator itc = m_clans.find(cl);
        if(itc != m_clans.end())
        {
            ccl = itc->second;
            m_clanSort.erase(ccl);
            ccl->score += score;
            ccl->writeToDB();
            m_clanSort.insert(ccl);
        }

        CCBPlayerList& list = itcp->second;
        for(CCBPlayerList::iterator itp = list.begin(); itp != list.end(); ++ itp)
        {
            CCBPlayer* pl = *itp;
            pl->score += score;
            pl->writeToDB();
            if(ccl)
                ccl->sendInfo(pl);
        }
    }
    Stream st(REP::CCB);
    makeLeaderBoardInfo(st);
    broadcast(st);
}

void ClanCity::forceEnter(CCBPlayer* pl, UInt8 spot)
{
    UInt8 pos = spot - 1;
    if(!m_spots[pos].canMoveTo())
        return;
    m_spots[pos].playerEnter(pl);
    pl->sendInfo();
}

bool ClanCity::isRunning()
{
    UInt32 now = TimeUtil::Now();
    return (now >= m_startTime && now <= m_endTime);
}

void ClanCity::loadFromDB()
{
    std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
    char querystr[1024] = {0};

    sprintf(querystr, "SELECT `type`, `round`, `openTime`, `startTime`, `endTime`, `defClanId` FROM `clancity`");
    DBClanCity dbcc = {0};
    if(execu->Extract(querystr, dbcc) == DB::DB_OK)
    {
        m_round = dbcc.round;
        m_type = dbcc.type;
        m_openTime = dbcc.openTime;
        m_startTime = dbcc.startTime;
        m_endTime = dbcc.endTime;
        m_defClanId = dbcc.defClanId;
    }
    if(m_openFlag && m_openTime == 0xFFFFFFFF)
    {
        setOpenTime();
    }
    if(m_openTime != 0xFFFFFFFF)
        m_openFlag = true;

    DBCCBSpot dbccbspot;
    memset(querystr, 0, sizeof(querystr));
    sprintf(querystr, "SELECT `id`, `hp`, `canAtk`, `skill_dmg0`, `skill_action0`, `skill_bomb0`, `skill_dmg1`, `skill_action1`, `skill_bomb1` FROM `clancity_spot`");
    if(execu->Prepare(querystr, dbccbspot) == DB::DB_OK)
    {
        while(execu->Next() == DB::DB_OK)
        {
            UInt8 pos = dbccbspot.id - 1;
            if(pos > 6)
                continue;
            m_spots[pos].hp = dbccbspot.hp;
            m_spots[pos].canAtk = dbccbspot.canAtk;
            m_spots[pos].skill_dmg[0].lvl = dbccbspot.skill_dmg0;
            m_spots[pos].skill_action[0].lvl = dbccbspot.skill_action0;
            m_spots[pos].skill_bomb[0].lvl = dbccbspot.skill_bomb0;
            m_spots[pos].skill_dmg[1].lvl = dbccbspot.skill_dmg1;
            m_spots[pos].skill_action[1].lvl = dbccbspot.skill_action1;
            m_spots[pos].skill_bomb[1].lvl = dbccbspot.skill_bomb1;
        }
    }

    DBCCBClan dbccbclan;
    memset(querystr, 0, sizeof(querystr));
    sprintf(querystr, "SELECT `clanId`, `score`, `skill_hp`, `skill_atk`, `skill_action` FROM `clancity_clan`");
    if(execu->Prepare(querystr, dbccbclan) == DB::DB_OK)
    {
        while(execu->Next() == DB::DB_OK)
        {
            Clan* cl = globalClans[dbccbclan.clanId];
            if(!cl)
                continue;
            CCBClan* ccl = new CCBClan(cl);
            ccl->score = dbccbclan.score;
            ccl->skill_hp.lvl = dbccbclan.skill_hp;
            ccl->skill_atk.lvl = dbccbclan.skill_atk;
            ccl->skill_action.lvl = dbccbclan.skill_action;
            m_clans[cl] = ccl;
            m_clanSort.insert(ccl);
        }
    }

    DBCCBPlayer dbccbp;
    memset(querystr, 0, sizeof(querystr));
    sprintf(querystr, "SELECT `playerId`, `side`, `entered`, `win`, `dead`, `realive`, `score`, `realivecd`, `weary` FROM `clancity_player`");
    if(execu->Prepare(querystr, dbccbp) == DB::DB_OK)
    {
        while(execu->Next() == DB::DB_OK)
        {
			Player * player = globalPlayers[dbccbp.playerId];
			if(!player)
				continue;
            Clan* cl = player->getClan();
            if(!cl)
                continue;

            CCBPlayer* pl = new CCBPlayer(e_player);
            pl->fgt.player = player;
            pl->side = dbccbp.side;
            pl->pos = 0;
            pl->win = dbccbp.win;
            pl->dead = dbccbp.dead;
            pl->realive = dbccbp.realive;
            pl->score = dbccbp.score;
            pl->realivecd = dbccbp.realivecd;
            pl->weary = dbccbp.weary;
            pl->entered = dbccbp.entered;
            if(dbccbp.entered != 0)
            {
                m_players[player] = pl;
                CCBPlayerList& list = m_clanPlayers[dbccbp.side][cl];
                list.push_back(pl);
                player->setInClanCity(true);
                if(pl->side == 0)
                    m_spots[0].playerEnter(pl);
                else
                    m_spots[6].playerEnter(pl);
            }
            else
            {
                m_players_leave[player] = pl;
            }
        }
    }
}

}
