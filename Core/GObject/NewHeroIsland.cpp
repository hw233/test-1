
#include "NewHeroIsland.h"
#include "Server/SysMsg.h"
#include "Common/Random.h"
#include "GObject/Package.h"
#include "Battle/BattleSimulator.h"
#include "GData/Money.h"
#include "MsgID.h"
#include "Country.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "HeroMemo.h"

namespace GObject
{

#define NHEROISLAND_BATTLE_TIME 30
#define NHEROISLAND_STAGE_TIME (cfg.GMCheck ? 21*60 : 3*60)

#define CHECK_MONEY() \
{\
    UInt32 goldUse = GData::moneyNeed[GData::HISKILL].gold; \
    if (pd->player->getGold() < goldUse) \
    { \
        pd->player->sendMsgCode(0, 1104); \
        return; \
    } \
    ConsumeInfo ci(HeroIslandSkill,0,0); \
    pd->player->useGold(goldUse, &ci); \
}

NewHeroIsland_Skill NewHeroIsland::_skills[5];
static float factor[5] = {1.0, 1.5, 2.0, 2.5, 3.0};
//static UInt32 maxScore[5] = {5000, 20000, 40000, 80000, 0xFFFFFFFF};
static UInt32 maxScore[5] = {100, 400, 800, 1600, 160000};

inline UInt32 calcExp(UInt8 lvl)
{
    if (lvl < 10)
        lvl = 0;
    return ((lvl-10)*(lvl/10)*5+25);
}

void NewHeroIsland::Init()
{
    //获取技能buff配置
    Table buffs = GameAction()->GetNewHeroIslandBuffs();
    size_t buffNum = buffs.size();
    for(UInt32 i = 0; i < buffNum && i < 5; ++ i)
    {
        Table buff = buffs.get<Table>(i+1);
        if(buff.size() < 14) continue;

        NewHeroIsland::_skills[i].id = i + 1;
        NewHeroIsland::_skills[i].cd = buff.get<UInt32>(1);
        NewHeroIsland::_skills[i].last = buff.get<UInt32>(14);
        NewHeroIsland::_skills[i].attrs.attackP = NewHeroIsland::_skills[i].attrs.magatkP = buff.get<float>(2) / 100;
        NewHeroIsland::_skills[i].attrs.defendP = NewHeroIsland::_skills[i].attrs.magdefP = buff.get<float>(3) / 100;
        NewHeroIsland::_skills[i].attrs.hpP = buff.get<float>(4) / 100;
        NewHeroIsland::_skills[i].attrs.actionP = buff.get<float>(5) / 100;
        NewHeroIsland::_skills[i].attrs.hitrateP = buff.get<float>(6) / 100;
        NewHeroIsland::_skills[i].attrs.evadeP = buff.get<float>(7) / 100;
        NewHeroIsland::_skills[i].attrs.criticalP = buff.get<float>(8) / 100;
        NewHeroIsland::_skills[i].attrs.criticaldmg = buff.get<float>(9) / 100;
        NewHeroIsland::_skills[i].attrs.pierceP = buff.get<float>(10) / 100;
        NewHeroIsland::_skills[i].attrs.counterP = buff.get<float>(11) / 100;
        NewHeroIsland::_skills[i].attrs.magresP = buff.get<float>(12) / 100;
        NewHeroIsland::_skills[i].attrs.aura = buff.get<float>(13);
    }
}

void NewHeroIsland::useSkill(Player * player, UInt8 skillid, UInt8 useGold)
{
    if (!player || !skillid || skillid > 5)
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    NHIPlayerData* pd = findPlayer(player);
    if (!pd) return;

    UInt32 now = TimeUtil::Now();
    if (now >= _endTime)
        return;
    if (!useGold && pd->skills[skillid-1].cd > now)
        return;
    if (useGold && pd->skills[skillid-1].cd > now)
        CHECK_MONEY();

    clearBuff(pd, false);
    switch (skillid)
    {
        case 1:
            pd->skills[skillid-1].bufid = PLAYER_BUFF_HIMS;
            SYSMSG_SEND(2134, pd->player);
            break;
        case 2:
            pd->skills[skillid-1].bufid = PLAYER_BUFF_HIPG;
            SYSMSG_SEND(2130, pd->player);
            break;
        case 3:
            pd->skills[skillid-1].bufid = PLAYER_BUFF_HIBT;
            SYSMSG_SEND(2131, pd->player);
            break;
        case 4:
            pd->skills[skillid-1].bufid = PLAYER_BUFF_HILN;
            SYSMSG_SEND(2132, pd->player);
            break;
        case 5:
            pd->skills[skillid-1].bufid = PLAYER_BUFF_HIJZ;
            SYSMSG_SEND(2133, pd->player);
            break;
    }
    if (!pd->skills[skillid-1].lastcd)
        pd->skills[skillid-1].lastcd = now + _skills[skillid-1].last;
    else
        pd->skills[skillid-1].lastcd += _skills[skillid-1].last;
    pd->skills[skillid-1].cd = now + _skills[skillid-1].cd;
    pd->skills[skillid-1].attr = &(_skills[skillid-1].attrs);
    pd->player->setBuffData(pd->skills[skillid-1].bufid, pd->skills[skillid-1].lastcd, false);
    player->addHIAttr(_skills[skillid-1].attrs);

    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(8);
    st << skillid << _skills[skillid-1].cd;
    st << Stream::eos;
    player->send(st);

}

void NewHeroIsland::clearBuff(NHIPlayerData* pd, bool isAdd)
{
    if (!pd || !pd->player)
        return;

    pd->player->clearHIAttr();
    UInt32 now = TimeUtil::Now();
    for (UInt8 i = 0; i < 5; ++ i)
    {
        if (now < pd->skills[i].lastcd)
        {
            if (pd->skills[i].attr && isAdd)
                pd->player->addHIAttr(*pd->skills[i].attr);
        }
        else
        {
            pd->skills[i].lastcd = 0;
        }
    }
}

void NewHeroIsland::playerEnter(Player* player)
{
    if (!player || player->GetLev() < 40)
    {
        player->sendMsgCode(0, 1010);
        return;
    }
    UInt32 now = TimeUtil::Now();
    if (now < _prepareTime || now >= _endTime)
        return;

    UInt32 left = player->getBuffData(PLAYER_BUFF_HIESCAPE);
    if (left > now)
        left -= now;
    else
        left = 0;
    if (left)
    {
        player->sendMsgCode(0, 2007, left);
        return;
    }

    UInt8 spot = 0;
    NHIPlayerData* pd = findPlayer(player);
    if (pd)
    {
        if (!player->getHIType() || player->getHIType() > 3)
            return;
        if (pd->status != NEWHERO_ISLAND_ESCAPE)
            return;
        std::map<UInt8, UInt8>::iterator it = _wasOwned.find(pd->spot);
        if (it != _wasOwned.end())
        {
            UInt8 size = _noOwned.size();
            if (size > 0)
                spot = _noOwned[uRand(size)];
            else
                spot = NEWHERO_ISLAND_SPOTS - 1;
            pd->spot = spot;
        }
        pd->player->setBuffData(PLAYER_BUFF_HIMOVE, 0, false);
        pd->type = player->getHIType();
        pd->status = NEWHERO_ISLAND_JOIN;
        pd->stage = _stage;
        pd->rndEvent = 0;
        spot = pd->spot;
        if (!enter(pd, spot, false))
        {
            pd->status = NEWHERO_ISLAND_ESCAPE;
            return;
        }
    }
    else
    {
        pd = new(std::nothrow) NHIPlayerData;
        if (!pd) return;
        getIdentity(player);
        if (!player->getHIType() || player->getHIType() > 3)
            return;
        UInt8 size = _noOwned.size();
        if (size == 0)
            spot = NEWHERO_ISLAND_SPOTS - 1;
        else
            spot = _noOwned[uRand(size)];
        pd->player = player;
        pd->type = player->getHIType();
        pd->status = NEWHERO_ISLAND_JOIN;
        pd->stage = _stage;
        if (!enter(pd, spot, false))
        {
            delete pd;
            return;
        }
        player->heroIslandUdpLog(1165, 1);
        _allPlayer.insert(std::make_pair(player, pd));
    }
    player->addFlag(Player::InHeroIsland);
    insertNHIPlayerData(pd);

    playerInfo(player);
    broadcastRank(player);
    if(now <= _startTime ||
            (now >= _stage * NHEROISLAND_STAGE_TIME + _startTime - 60 && now < _stage * NHEROISLAND_STAGE_TIME + _startTime))
    {
        _players[spot].insert(pd);
        sendPairPlayerInfo(pd, NULL);
    }
    else
        enterPairPlayer(pd);

    ++ _nplayers[spot][pd->type-1];
    updateSpotPlayers(spot);
    sendSkillInfo(pd);
    player->getSurnameLegendAward(e_sla_hi);
    player->OnHeroMemo(MC_ATHLETICS, MD_MASTER, 0, 0);
}

void NewHeroIsland::enterPairPlayer(NHIPlayerData * pd)
{
    //加入新的配对
    if (_pairPlayer[pd->spot].size() == 0)
    {
        pairsNHIPlayerData pnhipd0;
        pnhipd0.nhipd1 = pd;
        pnhipd0.nhipd2 = NULL;
        _pairPlayer[pd->spot].push_back(pnhipd0);
        sendPairPlayerInfo(pd, NULL);
    }
    else
    {
        pairsNHIPlayerData& pnhipd = _pairPlayer[pd->spot].back();
        if (pnhipd.nhipd1 == NULL && pnhipd.nhipd2 != pd)
        {
            pnhipd.nhipd1 = pd;
            sendPairPlayerInfo(pnhipd.nhipd1, pnhipd.nhipd2);
        }
        else if (pnhipd.nhipd2 == NULL && pnhipd.nhipd1 != pd)
        {
            pnhipd.nhipd2 = pd;
            sendPairPlayerInfo(pnhipd.nhipd1, pnhipd.nhipd2);
        }
        else
        {
            pairsNHIPlayerData pnhipd0;
            pnhipd0.nhipd1 = pd;
            pnhipd0.nhipd2 = NULL;
            _pairPlayer[pd->spot].push_back(pnhipd0);
            sendPairPlayerInfo(pd, NULL);
        }
    }
}

bool NewHeroIsland::enter(NHIPlayerData* pd, UInt8 spot, bool movecd)
{
    if (!pd || (spot != 0xFF && spot > NEWHERO_ISLAND_SPOTS))
        return false;

    if (spot == 0xFF)
        spot = 0;

    pd->spot = spot;
    //_players[spot].insert(pd);  //must be note!!

    if (movecd)
    {
        if (cfg.GMCheck)
            pd->movecd = TimeUtil::Now() + 120;
        else
            pd->movecd = TimeUtil::Now() + 10;
        pd->player->setBuffData(PLAYER_BUFF_HIMOVE, pd->movecd, false);
    }

    pd->player->setHISpot(spot);
    return true;
}

void NewHeroIsland::playerLeave(Player* player)
{
    if (!player)
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    NHIPlayerData* pd = findPlayer(player);
    if (!pd) return;

    pd->status = NEWHERO_ISLAND_ESCAPE;
    pd->reset(true);

    player->clearHIAttr();
    player->delFlag(Player::InHeroIsland);
    player->regenAll(true);

    UInt32 now = TimeUtil::Now();
    if (now >= _startTime && now <= _endTime)
    {
        if (cfg.GMCheck)
            player->setBuffData(PLAYER_BUFF_HIESCAPE, now+5*60, false);
        else
            player->setBuffData(PLAYER_BUFF_HIESCAPE, now+60, false);
    }
    else if (now < _startTime)
    {
        NHISort::iterator iter = _players[pd->spot].find(pd);
        if (iter != _players[pd->spot].end())
            _players[pd->spot].erase(iter);
    }
    if (_nplayers[pd->spot][pd->type-1])
    {
        --_nplayers[pd->spot][pd->type-1];
        updateSpotPlayers(pd->spot);
    }

    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(2);
    st << static_cast<UInt8>(1);
    st << Stream::eos;
    player->send(st);
}

bool NewHeroIsland::moveTo(Player* player, UInt8 to, bool movecd, bool force)
{
    if (!player || to >= NEWHERO_ISLAND_SPOTS)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    NHIPlayerData* pd = findPlayer(player);
    if (!pd || pd->status == NEWHERO_ISLAND_ESCAPE)
        return false;

    if (pd->spot == to)
        return false;
    if (to == NEWHERO_ISLAND_SPOTS - 1 && _wasOwned.size() < NEWHERO_ISLAND_SPOTS - 1)
        return false;
    std::map<UInt8, UInt8>::iterator it = _wasOwned.find(to);
    if (it != _wasOwned.end())
        return false;

    UInt32 now = TimeUtil::Now();
    if (!force)
    {
        if (movecd && pd->movecd > now)
        {
            pd->player->sendMsgCode(0, 2001);
            return false;
        }
    }

    pd->status = NEWHERO_ISLAND_MOVE;  //must be here
    //移动后清除数据
    NHISort::iterator iter = _players[pd->spot].find(pd);
    if (iter != _players[pd->spot].end())
        _players[pd->spot].erase(iter);
    else
    {
        for(std::vector<pairsNHIPlayerData>::iterator it = _pairPlayer[pd->spot].begin(); it != _pairPlayer[pd->spot].end(); ++ it)
        {
            if ((*it).nhipd1 == pd)
            {
                (*it).nhipd1 = NULL;
                sendPairPlayerInfo((*it).nhipd2, NULL);
                break;
            }
            else if ((*it).nhipd2 == pd)
            {
                (*it).nhipd2 = NULL;
                sendPairPlayerInfo((*it).nhipd1, NULL);
                break;
            }
        }
    }

    if (_nplayers[pd->spot][pd->type-1])
    {
        --_nplayers[pd->spot][pd->type-1];
        updateSpotPlayers(pd->spot);
    }
    bool cd = true;
    if (!movecd)
        cd = false;
    if (enter(pd, to, cd))
    {
        if(now <= _startTime ||
                (now >= _stage * NHEROISLAND_STAGE_TIME + _startTime - 60 && now < _stage * NHEROISLAND_STAGE_TIME + _startTime))
        {
            _players[to].insert(pd);
            sendPairPlayerInfo(pd, NULL);
        }
        else
            enterPairPlayer(pd);

        ++ _nplayers[to][pd->type-1];
        updateSpotPlayers(to);
        Stream st(REP::NEWHERO_ISLAND);
        st << static_cast<UInt8>(6);
        st << to;
        st << Stream::eos;
        player->send(st);
        return true;
    }
    return false;
}

UInt8 NewHeroIsland::getIdentity(Player* player, bool rand)
{
    if (!player)
        return 0;
    if (player->GetLev() < 40)
    {
        player->sendMsgCode(0, 1010);
        return 0;
    }

    if (player->getThreadId() != COUNTRY_NEUTRAL)
        return 0;

    UInt8 type = 0;
    if (rand)
    {
        type = player->getHIType();
        if (type && _types[type-1])
            --_types[type-1];

        type = (uRand(100) % 3) + 1;
        ++_types[type];
    }
    else
    {
        if (_types[0] < _types[1])
        {
            if (_types[2] < _types[0])
            {
                ++_types[2];
                type = 3;
            }
            else
            {
                ++_types[0];
                type = 1;
            }
        }
        else if (_types[1] < _types[2])
        {
            ++_types[1];
            type = 2;
        }
        else
        {
            ++_types[2];
            type = 3;
        }
    }

    player->setHIType(type);
    return type;
}

NHIPlayerData* NewHeroIsland::findPlayer(Player* player)
{
    std::map<Player *, NHIPlayerData *>::iterator iter = _allPlayer.find(player);
    if (iter == _allPlayer.end())
        return NULL;
    return iter->second;
}

void NewHeroIsland::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);

    broadcastTV(now);

    if (_startTime && now >= _startTime)
    {
        if (_expTime)
            checkAddExp(now);
        UInt8 corr = (now - _startTime) / NHEROISLAND_BATTLE_TIME;
        if (!_expTime)
            _expTime = _startTime + corr * NHEROISLAND_BATTLE_TIME + 60;
        if (!_tickTime)
        {   
            makePairPlayers();
            sendAllPlayerInfo();
            _tickTime = _startTime + (corr + 1) * NHEROISLAND_BATTLE_TIME;
        }
        bool isSettle = checkSettleAccounts(now);
        updatePlayerInfo(now);
        if (now < _tickTime)
            return;
        _tickTime += NHEROISLAND_BATTLE_TIME;
        if (!isSettle)
        {
            handleBattle();
            checkOccupySpot();
            broadcastRank();
            if(now < _stage * NHEROISLAND_STAGE_TIME + _startTime - 60
                    || (now >= _stage * NHEROISLAND_STAGE_TIME + _startTime && _stage != 3))
            {
                makePairPlayers();
                randomEvent();
                sendAllPlayerInfo();
            }
        }
        if (now >= _endTime)
            end(now);
        checkSkillTime();
    }

}

//每阶段结束时结算
bool NewHeroIsland::checkSettleAccounts(UInt32 now)
{
    _stage = (now - _startTime) / NHEROISLAND_STAGE_TIME + 1;
    _stage = _stage > 3 ? 3 : _stage;
    if(now > _stage * NHEROISLAND_STAGE_TIME + _startTime - 60 && now < _stage * NHEROISLAND_STAGE_TIME + _startTime)
    {
        if(_isAward[_stage-1])
            return true;
        rankReward1();
        _isAward[_stage-1] = true;
        memset(_scores, 0, sizeof(_scores));
        memset(_types, 0, sizeof(_types));
        _wasOwned.clear();
        _noOwned.clear();
        for (UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS-1; ++ i)
            _noOwned.push_back(i);
        broadcastRank();
        for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
        {
            for (NHISort::iterator iter = _players[i].begin(); iter != _players[i].end(); ++ iter)
            {
                if ((*iter) == NULL || (*iter)->player == NULL)
                    continue;
                getIdentity((*iter)->player);
                (*iter)->type = (*iter)->player->getHIType();
                (*iter)->stage = _stage;
                if ((*iter)->spot == NEWHERO_ISLAND_SPOTS - 1)
                {
                    UInt8 spot = _noOwned[uRand(_noOwned.size())];
                    enter(*iter, spot, false);
                    _players[spot].insert(*iter);
                }
                playerInfo((*iter)->player);
                sendPairPlayerInfo(*iter, NULL);
            }
        }
        _players[NEWHERO_ISLAND_SPOTS-1].clear();
        return true;
    }
    return false;
}

void NewHeroIsland::randomEvent()
{
    UInt8 size = _noOwned.size();
    UInt8 rndSpot = 0;
    if (size == 0)
        rndSpot = NEWHERO_ISLAND_SPOTS - 1;
    else
        rndSpot = _noOwned[uRand(size)];
    UInt32 scores[3] = {0};
    for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
    {
        for(UInt8 j = 0; j < 3; ++ j)
            scores[j] += _scores[i][j];
    }
    UInt8 sort[3] = {0};
    UInt8 i,j,k;
    for(i = k = 0; i < sizeof(scores)/sizeof(scores[0]); ++ i)
    {
        for(j = 0; j < sizeof(scores)/sizeof(scores[0]); ++ j)
        {
            if(scores[j] < scores[k])
                k = j;
        }
        scores[k] += 1000000;
        sort[i] = k;
    }
    static UInt32 events[] = {2, 4, 8, 10, 10, 10};
    UInt8 idx = uRand(sizeof(events)/sizeof(events[0]));
    std::vector<pairsNHIPlayerData>::iterator iter = _pairPlayer[rndSpot].begin();
    for(; iter != _pairPlayer[rndSpot].end(); ++ iter)
    {
        NHIPlayerData * pd1 = (*iter).nhipd1;
        NHIPlayerData * pd2 = (*iter).nhipd2;
        if (pd1 && pd1->status != NEWHERO_ISLAND_MOVE && pd1->status != NEWHERO_ISLAND_ESCAPE)
        {
            if (pd1->type == sort[0] + 1)
            {   //取积分最低的玩家
                pd1->rndEvent = idx + 1;
                GData::AttrExtra attrs;
                if (idx == 3)   //生命
                    attrs.hpP = static_cast<float>(events[idx] / 100);
                else if (idx == 4)  //攻防
                {
                    attrs.attackP = static_cast<float>(events[idx] / 100);
                    attrs.defendP = static_cast<float>(events[idx] / 100);
                }
                else if (idx == 5)   //身法
                    attrs.actionP = static_cast<float>(events[idx] / 100);
                else
                {
                    pd1->score[_stage-1] += events[idx];
                    addSpotScore(pd1->spot, pd1->type, events[idx], pd1->player);
                }
                if (idx >= 3)
                    pd1->player->addHIAttr(attrs);
            }
        }
        if (pd2 && pd2->status != NEWHERO_ISLAND_MOVE && pd2->status != NEWHERO_ISLAND_ESCAPE)
        {
            if (pd2->type == sort[0] + 1)
            {   //取积分最低的玩家
                pd2->rndEvent = idx + 1;
                GData::AttrExtra attrs;
                if (idx == 3)   //生命
                    attrs.hpP = static_cast<float>(events[idx] / 100);
                else if (idx == 4)  //攻防
                {
                    attrs.attackP = static_cast<float>(events[idx] / 100);
                    attrs.defendP = static_cast<float>(events[idx] / 100);
                }
                else if (idx == 5)   //身法
                    attrs.actionP = static_cast<float>(events[idx] / 100);
                else
                {
                    pd2->score[_stage-1] += events[idx];
                    addSpotScore(pd2->spot, pd2->type, events[idx], pd2->player);
                }
                if (idx >= 3)
                    pd2->player->addHIAttr(attrs);
            }
        }
    }
}

void NewHeroIsland::makePairPlayers()
{
    for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
    {
        _pairPlayer[i].clear();
        while(_players[i].size() > 0)
        {
            for (NHISort::iterator iter = _players[i].begin(); iter != _players[i].end(); ++ iter)
            {
                if((*iter)->status == NEWHERO_ISLAND_ESCAPE)
                {
                    _players[i].erase(iter);
                    break;
                }
                NHISort::iterator tmpit = iter;
                std::vector<NHISort::iterator> tmppl;
                while(tmppl.size() < 5)
                {
                    if (++ tmpit == _players[i].end())
                        break;
                    if ((*tmpit)->status != NEWHERO_ISLAND_ESCAPE)
                    {
                        tmppl.push_back(tmpit);
                    }
                }
                (*iter)->status = NEWHERO_ISLAND_PAIR;
                (*iter)->rndEvent = 0;
                (*iter)->stage = _stage;
                pairsNHIPlayerData pnhipd;
                pnhipd.nhipd1 = *iter;
                UInt8 size = tmppl.size();
                if (size > 0)
                {
                    NHISort::iterator it = tmppl[uRand(size)];
                    (*it)->status = NEWHERO_ISLAND_PAIR;
                    (*it)->rndEvent = 0;
                    (*it)->stage = _stage;
                    pnhipd.nhipd2 = *it;
                    _players[i].erase(it);
                }
                else
                    pnhipd.nhipd2 = NULL;
                _pairPlayer[i].push_back(pnhipd);
                _players[i].erase(iter);
                break;
            }
        }
    }
}

void NewHeroIsland::handleBattle()
{
    for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
    {
        size_t sz = _pairPlayer[i].size();
        UInt32 count = sz / 4;
        UInt32 rand = uRand(count);
        UInt32 index = 0, idx = 0;
        for(size_t j = 0; j < sz; ++ j, ++ idx)
        {
            NHIPlayerData * pd1 = _pairPlayer[i][j].nhipd1;
            NHIPlayerData * pd2 = _pairPlayer[i][j].nhipd2;
            if (pd1 == NULL && pd2 == NULL)
                continue;
            clearBuff(pd1);
            clearBuff(pd2);
            if (checkSpecialWin(pd1, pd2))
                continue;
            Player * player1 = pd1->player;
            Player * player2 = pd2->player;
            if (player1 == NULL || player2 == NULL)
                continue;
            player1->setHiAttrFlag(true);
            player2->setHiAttrFlag(true);
            bool res = player1->challenge(player2, NULL, NULL, false, 0, true, Battle::BS_NEWCOUNTRYBATTLE);
            player1->setHiAttrFlag(false);
            player2->setHiAttrFlag(false);
            if (res)
            {
                pd1->score[_stage-1] += 20 * factor[i];
                pd1->status = NEWHERO_ISLAND_FIGHT;
                pd1->stage = _stage;
                player1->clearHIAttr();
                _players[i].insert(pd1);
                insertNHIPlayerData(pd1);

                pd2->score[_stage-1] += 10 * factor[i];
                pd2->status = NEWHERO_ISLAND_FIGHT;
                pd2->stage = _stage;
                player2->clearHIAttr();
                player2->autoRegenAll();
                _players[i].insert(pd2);
                insertNHIPlayerData(pd2);

                addSpotScore(i, pd1->type, 20 * factor[i], player1);
                addSpotScore(i, pd2->type, 10 * factor[i], player2);

                if(sz <= 4)
                    sendWinerInfo(player1, player2, 20 * factor[i], i);
                else if(idx == index + rand)
                {
                    index += count;
                    sendWinerInfo(player1, player2, 20 * factor[i], i);
                }
            }
            else
            {
                pd2->score[_stage-1] += 20 * factor[i];
                pd2->status = NEWHERO_ISLAND_FIGHT;
                pd2->stage = _stage;
                player2->clearHIAttr();
                _players[i].insert(pd2);
                insertNHIPlayerData(pd2);

                pd1->score[_stage-1] += 10 * factor[i];
                pd1->status = NEWHERO_ISLAND_FIGHT;
                pd1->stage = _stage;
                player1->clearHIAttr();
                player1->autoRegenAll();
                _players[i].insert(pd1);
                insertNHIPlayerData(pd1);

                addSpotScore(i, pd2->type, 20 * factor[i], player2);
                addSpotScore(i, pd1->type, 10 * factor[i], player1);

                if(sz <= 4)
                    sendWinerInfo(player2, player1, 20 * factor[i], i);
                else if(idx == index + rand)
                {
                    index += count;
                    sendWinerInfo(player2, player1, 20 * factor[i], i);
                }
            }
        }
        _pairPlayer[i].clear();
    }
}

//检查占领据点
void NewHeroIsland::checkOccupySpot()
{
    if (_wasOwned.size() < NEWHERO_ISLAND_SPOTS-1)
    {
        UInt8 sort[NEWHERO_ISLAND_SPOTS][3] = {{0},};
        for(UInt8 idx = 0; idx < NEWHERO_ISLAND_SPOTS; ++ idx)
        {
            std::vector<UInt8>::iterator it = std::find(_noOwned.begin(), _noOwned.end(), idx);
            if (it == _noOwned.end())
                continue;
            UInt8 i,j,k;
            for(i = k = 0; i < 3; ++ i)
            {
                for(j = 0; j < 3; ++ j)
                {
                    if(_scores[idx][j] < _scores[idx][k])
                        k = j;
                }
                _scores[idx][k] += 1000000;
                sort[idx][i] = k;
            }
            for(k = 0; k < 3; ++ k)
                _scores[idx][k] %= 1000000;
        }
        for(UInt8 idx = 0; idx < NEWHERO_ISLAND_SPOTS-1; ++ idx)
        {
            std::vector<UInt8>::iterator it = std::find(_noOwned.begin(), _noOwned.end(), idx);
            if (it == _noOwned.end())
                continue;
            UInt8 type = sort[idx][2];
            if (_scores[idx][type] < maxScore[idx])
                continue;
            _wasOwned.insert(std::make_pair(idx, type+1));
            _noOwned.erase(it);
            UInt8 size = _noOwned.size();
            UInt8 spot = 0;
            if (size > 0)
                spot = _noOwned[uRand(size)];
            else
                spot = NEWHERO_ISLAND_SPOTS - 1;
            bool hasBroad = false;
            for (NHISort::iterator iter = _players[idx].begin(); iter != _players[idx].end(); ++ iter)
            {
                (*iter)->spot = spot;
                if((*iter)->type == type + 1)
                {
                    (*iter)->score[_stage-1] += 100;
                    addSpotScore(idx, type+1, 100, (*iter)->player);
                    insertNHIPlayerData(*iter);
                    sendOccupySpotAward(*iter, idx, hasBroad);
                }
                (*iter)->player->setHISpot(spot);
                _players[spot].insert(*iter);
                ++_nplayers[spot][(*iter)->type-1];

                Stream st(REP::NEWHERO_ISLAND);
                st << static_cast<UInt8>(6);
                st << spot;
                st << Stream::eos;
                (*iter)->player->send(st);
            }
            _players[idx].clear();
            updateSpotPlayers(spot);
            memset(_nplayers[idx], 0, sizeof(_nplayers[idx]));
            updateSpotPlayers(idx);
        }
    }
}

void NewHeroIsland::sendOccupySpotAward(NHIPlayerData * pd, UInt8 spot, bool& hasBroad)
{
    if (!pd || !(pd->player) || spot >= NEWHERO_ISLAND_SPOTS)
        return;
    std::string spotName = "";
    std::string typeName = "";
    if (spot == 0)
        spotName = "星罗滩";
    else if(spot == 1)
        spotName = "摩天崖";
    else if(spot == 2)
        spotName = "英灵之船";
    else if(spot == 3)
        spotName = "陷空岛";
    else
        spotName = "戮仙台";
    if (pd->type == 1)
        typeName = "天";
    else if (pd->type == 2)
        typeName = "地";
    else
        typeName = "人";
    MailPackage::MailItem item[] = { {9411, 1}, };
    SYSMSG(title, 2328);
    SYSMSGV(content, 2329, _stage, spotName.c_str(), item[0].count);
    MailItemsInfo itemsInfo(item, NewHeroIslandAward, 1);
    Mail * mail = pd->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
    if(mail)
        mailPackageManager.push(mail->id, item, 1, true);
    if (!hasBroad)
    {
        SYSMSG_BROADCASTV(2327, spotName.c_str(), typeName.c_str(), typeName.c_str(), item[0].count);
        hasBroad = true;
    }
}

void NewHeroIsland::insertNHIPlayerData(NHIPlayerData * pd)
{
    if (pd == NULL)
        return;
    for (NHISortAll::iterator i = _sorts.begin(); i != _sorts.end(); ++i)
    {
        if (*i && (*i)->player == pd->player)
        {
            _sorts.erase(i);
            break;
        }
    }
    _sorts.insert(pd);
}

void NewHeroIsland::addSpotScore(UInt8 spot, UInt8 type, UInt16 score, Player * player)
{
    if (spot > NEWHERO_ISLAND_SPOTS - 1 || type > 3 || type <= 0)
        return;
    _scores[spot][type-1] += score;
    if (player)
    {
        SYSMSG_SENDV(130, player, score);
        SYSMSG_SENDV(1030, player, score);
    }
}

bool NewHeroIsland::checkSpecialWin(NHIPlayerData * pd1, NHIPlayerData * pd2)
{
    if (pd1 == NULL && pd2 == NULL)
        return true;
    if (pd1 && pd2)
    {
        if (pd1->status == NEWHERO_ISLAND_ESCAPE && pd2->status == NEWHERO_ISLAND_ESCAPE)
        {
            return true;
        }
        else if (pd1->status == NEWHERO_ISLAND_ESCAPE)
        {   //player2轮空
            pd2->score[_stage-1] += 20 * factor[pd2->spot];
            pd2->stage = _stage;
            if (pd2->player)
                pd2->player->clearHIAttr();
            _players[pd2->spot].insert(pd2);
            insertNHIPlayerData(pd2);
            addSpotScore(pd2->spot, pd2->type, 20 * factor[pd2->spot], pd2->player);
            return true;
        }
        else if (pd2->status == NEWHERO_ISLAND_ESCAPE)
        {   //player1轮空
            pd1->score[_stage-1] += 20 * factor[pd1->spot];
            pd1->stage = _stage;
            if (pd1->player)
                pd1->player->clearHIAttr();
            _players[pd1->spot].insert(pd1);
            insertNHIPlayerData(pd1);
            addSpotScore(pd1->spot, pd1->type, 20 * factor[pd1->spot], pd1->player);
            return true;
        }
        else
            return false;
    }
    else if (pd1)
    {
        if (pd1->status != NEWHERO_ISLAND_ESCAPE)
        {   //player1轮空
            pd1->score[_stage-1] += 20 * factor[pd1->spot];
            pd1->stage = _stage;
            if (pd1->player)
                pd1->player->clearHIAttr();
            _players[pd1->spot].insert(pd1);
            insertNHIPlayerData(pd1);
            addSpotScore(pd1->spot, pd1->type, 20 * factor[pd1->spot], pd1->player);
        }
        return true;
    }
    else
    {
        if (pd2->status != NEWHERO_ISLAND_ESCAPE)
        {   //player2轮空
            pd2->score[_stage-1] += 20 * factor[pd2->spot];
            pd2->stage = _stage;
            if (pd2->player)
                pd2->player->clearHIAttr();
            _players[pd2->spot].insert(pd2);
            insertNHIPlayerData(pd2);
            addSpotScore(pd2->spot, pd2->type, 20 * factor[pd2->spot], pd2->player);
        }
        return true;
    }
    return false;
}

void NewHeroIsland::end(UInt32 now)
{
    checkSettleAccounts(now+5);
    rankReward();
    _prepareStep = 0;
    SYSMSG_BROADCASTV(2116);
    setStatus(2);
    reset();
    calcNext(now);
}

void NewHeroIsland::reset()
{
    for (std::map<Player *, NHIPlayerData *>::iterator iter = _allPlayer.begin(); iter != _allPlayer.end(); ++ iter)
    {
        delete iter->second;
    }
    for (UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++i)
    {
        _players[i].clear();
        _pairPlayer[i].clear();

    }
    _allPlayer.clear();
    _sorts.clear();

    _expTime = 0;
    _tickTime = 0;
    _stage = 1;
    _wasOwned.clear();
    _noOwned.clear();
    memset(_types, 0, sizeof(_types));
    memset(_nplayers, 0, sizeof(_nplayers));
    memset(_isAward, false, sizeof(_isAward));
    memset(_scores, 0, sizeof(_scores));
    for (UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS-1; ++ i)
        _noOwned.push_back(i);
}

void NewHeroIsland::calcNext(UInt32 now)
{
    if (cfg.GMCheck)
    {
        /*
        _prepareTime = TimeUtil::SharpDayT(0,now) + 11 * 60 * 60 + 50 * 60;

        if(_prepareTime + 75 * 60 < now)
            _prepareTime += 24 * 60 * 60;
        */
        _prepareTime = now + 300;
        _startTime = _prepareTime + 10 * 60;
        _endTime = _startTime + NHEROISLAND_STAGE_TIME * 3 - 60;
    }
    else
    {
        _prepareTime = now + 30;
        _startTime = _prepareTime + 60;
        _endTime = _startTime + NHEROISLAND_STAGE_TIME * 3 - 60;
    }
}

void NewHeroIsland::rankReward()
{
    if (!_endTime)
        return;

    SYSMSG_BROADCASTV(2312);

    UInt16 n = 0;
    for (NHISortAll::iterator i = _sorts.begin(); i != _sorts.end(); ++i, ++n)
    {
        if (!(*i) || !(*i)->player)
            continue;

        UInt16 prestige = 0;
        if (n < 5)
        {
            UInt16 count = (World::_wday == 6 || World::_wday == 7)?2:1;
            MailPackage::MailItem item[2] = {};
            if (n == 0)
            {
                prestige = 500*count;
                item = {{10, count}, {MailPackage::Coupon, 250*count},};
            }
            else if (n == 1)
            {
                prestige = 400*count;
                item = {{10, count}, {MailPackage::Coupon, 200*count},};
            }
            else if (n == 2)
            {
                prestige = 300*count;
                item = {{10, count}, {MailPackage::Coupon, 150*count},};
            }
            else if (n == 3)
            {
                prestige = 200*count;
                item = {{10, count}, {MailPackage::Coupon, 100*count},};
            }
            else if (n == 4)
            {
                prestige = 100*count;
                item = {{10, count}, {MailPackage::Coupon, 50*count},};
            }
            UInt32 allScore = (*i)->score[0] + (*i)->score[1] + (*i)->score[2];
            SYSMSG(title, 2316);
            SYSMSGV(content, 2317, allScore, n+1, count, prestige, item[1].count);
            SYSMSG_BROADCASTV(2326, n+1, (*i)->player->getCountry(), (*i)->player->getName().c_str(), allScore, count, prestige, item[1].count);
            MailItemsInfo itemsInfo(item, NewHeroIslandAward, 2);
            Mail * mail = (*i)->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
            if(mail)
                mailPackageManager.push(mail->id, item, 2, true);
            if (prestige)
            {
                if ((*i)->player->getThreadId() != WORKER_THREAD_NEUTRAL)
                {
                    GameMsgHdr hdr(0x236, (*i)->player->getThreadId(), (*i)->player, sizeof(prestige));
                    GLOBAL().PushMsg(hdr, &prestige);
                }
                else
                    (*i)->player->getPrestige(prestige);
            }
        }

        (*i)->player->autoRegenAll();
        if((*i)->player->getThreadId() != WORKER_THREAD_NEUTRAL)
        {
            stActivityMsg msg;
            msg.id = SthHeroIsland;
            GameMsgHdr hdr2(0x245, (*i)->player->getThreadId(), (*i)->player, sizeof(stActivityMsg));
            GLOBAL().PushMsg(hdr2, &msg);
        }
        else
        {
            GameAction()->doStrong((*i)->player, SthHeroIsland, 0 ,0);
        }
        Stream st(REP::NEWHERO_ISLAND);
        st << static_cast<UInt8>(0x0A);
        st << Stream::eos;
        if ((*i)->status != NEWHERO_ISLAND_ESCAPE)
            (*i)->player->send(st);
        //udpLog
        for(UInt8 id = 0; id < 3; ++ id)
            prestige += (*i)->score[id] / 8;
        (*i)->player->heroIslandUdpLog(1165, 3, prestige);
    }
}

void NewHeroIsland::rankReward1()
{
    UInt32 scores[3] = {0};
    for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
    {
        for(UInt8 j = 0; j < 3; ++ j)
            scores[j] += _scores[i][j];
    }
    UInt8 sort[3] = {0};
    UInt8 i,j,k;
    for(i = k = 0; i < sizeof(scores)/sizeof(scores[0]); ++ i)
    {
        for(j = 0; j < sizeof(scores)/sizeof(scores[0]); ++ j)
        {
            if(scores[j] < scores[k])
                k = j;
        }
        scores[k] += 1000000;
        sort[i] = k;
    }
    SYSMSGV(title, 2318, _stage);
    for (NHISortAll::iterator it = _sorts.begin(); it != _sorts.end(); ++it)
    {
        if (!(*it) || !(*it)->player)
            continue;
        UInt16 prestige = (*it)->score[_stage-1] / 8;
        UInt16 cnt1 = 0, cnt2 = 0, cnt3 = 0;
        if (sort[2] + 1 == (*it)->type)
        {
            cnt1 = 1;
            cnt2 = 1;
            cnt3 = 3;
        }
        else if (sort[1] + 1 == (*it)->type)
        {
            cnt1 = 1;
            cnt2 = 3;
            cnt3 = 1;
        }
        else
        {
            cnt1 = 3;
            cnt2 = 1;
            cnt3 = 1;
        }
        SYSMSGV(content, 2319, _stage, (*it)->score[_stage-1], prestige, cnt1, cnt2, cnt3);
        UInt16 count = (World::_wday == 6 || World::_wday == 7)?2:1;
        //MailPackage::MailItem item[] = {{MailPackage::Prestige, prestige*count}, {9408, cnt1*count}, {9409, cnt2*count}, {9410, cnt3*count},};
        MailPackage::MailItem item[] = {{9408, cnt1*count}, {9409, cnt2*count}, {9410, cnt3*count},};
        MailItemsInfo itemsInfo(item, NewHeroIslandAward, 3);
        Mail * mail = (*it)->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        if(mail)
            mailPackageManager.push(mail->id, item, 3, true);

        if (prestige)
        {
            if ((*it)->player->getThreadId() != WORKER_THREAD_NEUTRAL)
            {
                GameMsgHdr hdr(0x236, (*it)->player->getThreadId(), (*it)->player, sizeof(prestige));
                GLOBAL().PushMsg(hdr, &prestige);
            }
            else
                (*it)->player->getPrestige(prestige);
        }

        Stream st(REP::NEWHERO_ISLAND);
        st << static_cast<UInt8>(5);
        st << _stage;
        UInt8 size = sizeof(item)/sizeof(item[0]) + 1;
        st << size;
        st << static_cast<UInt16>(MailPackage::Prestige) << prestige;
        for(UInt8 j = 0; j < size; ++ j)
            st << item[j].id << static_cast<UInt16>(item[j].count);
        st << Stream::eos;
        (*it)->player->send(st);
    }
}

void NewHeroIsland::checkAddExp(UInt32 now)
{
    if(_expTime > now)
        return;
    UInt8 factor = (World::_wday == 6 || World::_wday == 7)?2:1;
    for (NHISortAll::iterator it = _sorts.begin(); it != _sorts.end(); ++it)
    {
        if (*it && (*it)->player && (*it)->status != NEWHERO_ISLAND_ESCAPE)
        {
            //计算经验
            NHIPlayerData* pd = *it;
            UInt8 lvl = pd->player->GetLev();
            UInt32 exp = 0;
            if (cfg.rpServer && lvl < 70)
                exp = calcExp(lvl) *3*factor*2;
            else
                exp = calcExp(lvl) *3*factor;
            pd->player->AddExp(exp);
        }
    }
    _expTime += 60;
}

void NewHeroIsland::checkSkillTime()
{
    for (NHISortAll::iterator it = _sorts.begin(); it != _sorts.end(); ++it)
    {
        if (*it && (*it)->player && (*it)->status != NEWHERO_ISLAND_ESCAPE)
        {
            for(UInt8 i = 0; i < 5; ++ i)
            {
                if (TimeUtil::Now() >= (*it)->skills[i].cd)
                {
                    sendSkillInfo(*it);
                    break;
                }
            }
        }
    }
}

void NewHeroIsland::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime && !_prepareStep)
        _prepareStep = 1;

    switch (_prepareStep)
    {
        case 1:
            //SYSMSG_BROADCASTV(2117, 15);
            _prepareStep = 2;
            break;

        case 2:
            if (now < _startTime - 10 * 60)
                return;
            SYSMSG_BROADCASTV(2117, 10);
            _prepareStep = 3;
            setStatus(1);
            break;

        case 3:
            if (now < _startTime - 5 * 60)
                return;
            SYSMSG_BROADCASTV(2117, 5);
            _prepareStep = 4;
            setStatus(1);
            break;

        case 4:
            if (now < _startTime)
                return;
            SYSMSG_BROADCASTV(2118);
            _prepareStep = 5;
            setStatus(1);
            break;

        default:
            break;
    }
}

void NewHeroIsland::sendDaily(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(8);
    st << static_cast<UInt8>(_status);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void NewHeroIsland::broadcastRank(Player * player)
{
    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(3);
    //五个据点天地人各自的人数,积分及占领情况
    for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
    {
        for(UInt8 j = 0; j < 3; ++ j)
            st << _nplayers[i][j] << _scores[i][j];
        if (i == NEWHERO_ISLAND_SPOTS - 1)
        {
            if (_wasOwned.size() >= i)
                st << static_cast<UInt8>(0);
            else
                st << static_cast<UInt8>(4);
        }
        else
        {
            std::map<UInt8, UInt8>::iterator it = _wasOwned.find(i);
            if (it != _wasOwned.end())
                st << it->second;
            else
                st << static_cast<UInt8>(0);
        }
    }
    //前五名玩家信息
    size_t pos = st.size();
    UInt8 cnt = 0;
    st << cnt;
    for (NHISortAll::iterator it = _sorts.begin(); it != _sorts.end(); ++it)
    {
        if (*it && (*it)->player)
        {
            st << (*it)->player->getName();
            st << (*it)->player->getCountry();
            UInt32 allSco = 0;
            for(UInt8 j = 0; j < 3; ++ j)
                allSco += (*it)->score[j];
            st << allSco;
            ++ cnt;
        }
        if (cnt >= 5)
            break;
    }
    st.data<UInt8>(pos) = cnt;
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        broadcast(st);
}

void NewHeroIsland::broadcast(Stream& st)
{
    for (NHISortAll::iterator it = _sorts.begin(); it != _sorts.end(); ++it)
    {
        if (*it && (*it)->player && (*it)->status != NEWHERO_ISLAND_ESCAPE)
            (*it)->player->send(st);
    }
}

void NewHeroIsland::sendAllPlayerInfo()
{
    for(UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS; ++ i)
    {
        std::vector<pairsNHIPlayerData>::iterator iter = _pairPlayer[i].begin();
        for(; iter != _pairPlayer[i].end(); ++ iter)
        {
            sendPairPlayerInfo((*iter).nhipd1, (*iter).nhipd2);
        }
    }
}

//配对信息
void NewHeroIsland::sendPairPlayerInfo(NHIPlayerData * pd1, NHIPlayerData * pd2)
{
    if (pd1 == NULL && pd2 == NULL)
        return;
    if (pd1 && pd1->player == NULL)
        return;
    if (pd2 && pd2->player == NULL)
        return;
    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(1);
    size_t pos = st.size();
    UInt32 allScore = 0;
    st << allScore;
    UInt16 timeLeft = 0;
    UInt32 curtime = TimeUtil::Now();
    if(_tickTime)
    {
        if(curtime >= _stage * NHEROISLAND_STAGE_TIME + _startTime - 60 && curtime < _stage * NHEROISLAND_STAGE_TIME + _startTime)
            timeLeft = _stage * NHEROISLAND_STAGE_TIME + _startTime - curtime;
        else
            timeLeft = _tickTime - curtime;
    }
    else
    {
        if(curtime >= _startTime)  //预防宕机,纠正时间
        {
            UInt8 round = (curtime - _startTime) / NHEROISLAND_BATTLE_TIME;
            timeLeft = (round + 1) * NHEROISLAND_BATTLE_TIME + _startTime - curtime;
        }
        else
            timeLeft = _startTime - curtime;
    }
    st << timeLeft;
    if (pd1)
    {
        st << pd1->player->getName() << pd1->player->getCountry();
        st << static_cast<UInt8>(pd1->player->getMainFighter() ? pd1->player->getMainFighter()->getId() : 0);
        st << pd1->player->GetLev() << pd1->player->getBattlePoint();
        st << pd1->player->getBattleMaxHp() << pd1->player->getBattleCurrentHp();
        st << pd1->score[_stage-1] << pd1->type << pd1->rndEvent;
    }
    else
    {
        st << "" << static_cast<UInt8>(2) << static_cast<UInt8>(0);
        st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
        st << static_cast<UInt32>(0) << static_cast<UInt32>(0);
        st << static_cast<UInt16>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
    }
    if (pd2)
    {
        st << pd2->player->getName() << pd2->player->getCountry();
        st << static_cast<UInt8>(pd2->player->getMainFighter() ? pd2->player->getMainFighter()->getId() : 0);
        st << pd2->player->GetLev() << pd2->player->getBattlePoint();
        st << pd2->player->getBattleMaxHp() << pd2->player->getBattleCurrentHp();
        st << pd2->score[_stage-1] << pd2->type << pd2->rndEvent;
    }
    else
    {
        st << "" << static_cast<UInt8>(2) << static_cast<UInt8>(0);
        st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
        st << static_cast<UInt32>(0) << static_cast<UInt32>(0);
        st << static_cast<UInt16>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
    }
    if (pd1 && pd1->status != NEWHERO_ISLAND_ESCAPE)
    {
        allScore = pd1->score[0] + pd1->score[1] + pd1->score[2];
        st.data<UInt8>(pos) = allScore;
        st << Stream::eos;
        pd1->player->send(st);
    }
    if (pd2 && pd2->status != NEWHERO_ISLAND_ESCAPE)
    {
        allScore = pd2->score[0] + pd2->score[1] + pd2->score[2];
        st.data<UInt8>(pos) = allScore;
        st << Stream::eos;
        pd2->player->send(st);
    }
}

//更新据点人数
void NewHeroIsland::updateSpotPlayers(UInt8 spot)
{
    if (spot >= NEWHERO_ISLAND_SPOTS)
        return;
    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(4);
    st << spot;
    for(UInt8 i = 0; i < 3; ++ i)
        st << _nplayers[spot][i];
    st << Stream::eos;
    broadcast(st);
}

void NewHeroIsland::playerInfo(Player * player)
{
    UInt32 now = TimeUtil::Now();
    if (!player || now < _prepareTime || now >= _endTime)
        return;
    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(0);
    NHIPlayerData * pd = findPlayer(player);
    if (pd && pd->status != NEWHERO_ISLAND_ESCAPE)
    {
        st << static_cast<UInt8>(1) << pd->type << pd->spot;
        st << static_cast<UInt32>(pd->score[0]+pd->score[1]+pd->score[2]);
    }
    else
    {
        st << static_cast<UInt8>(0) << static_cast<UInt8>(0xFF);
        st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
    }
    if (now >= _prepareTime && now < _startTime)
    {
        st << static_cast<UInt8>(0);
        st << static_cast<UInt16>(_startTime - now);
    }
    else if (now >= _startTime && now <= _stage * NHEROISLAND_STAGE_TIME + _startTime - 60)
    {
        st << static_cast<UInt8>(1);
        st << _stage;
        st << static_cast<UInt16>(_stage * NHEROISLAND_STAGE_TIME + _startTime - 60 - now);
    }
    else if (now > _stage * NHEROISLAND_STAGE_TIME + _startTime - 60)
    {
        st << static_cast<UInt8>(2);
        st << static_cast<UInt8>(_stage+1);
        st << static_cast<UInt16>(_stage * NHEROISLAND_STAGE_TIME + _startTime - now);
    }
    st << Stream::eos;
    player->send(st);
}

void NewHeroIsland::updatePlayerInfo(UInt32 now)
{   //每阶段一开始广播
    if(now >= (_stage-1) * NHEROISLAND_STAGE_TIME + _startTime && now < (_stage-1) * NHEROISLAND_STAGE_TIME + _startTime + 5)
    {
        for (NHISortAll::iterator it = _sorts.begin(); it != _sorts.end(); ++it)
        {
            if (*it && (*it)->player && (*it)->status != NEWHERO_ISLAND_ESCAPE)
                playerInfo((*it)->player);
        }
    }
}

void NewHeroIsland::sendSkillInfo(NHIPlayerData * pd)
{
    if (!pd || !pd->player)
        return;
    UInt32 now = TimeUtil::Now();
    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(7);
    UInt8 sz = sizeof(_skills)/sizeof(_skills[0]);
    st << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st << _skills[i].id;
        if (now >= pd->skills[i].cd)
            st << static_cast<UInt16>(0);
        else
            st << static_cast<UInt16>(pd->skills[i].cd - now);
    }
    st << Stream::eos;
    pd->player->send(st);
}


void NewHeroIsland::sendWinerInfo(Player * player1, Player * player2, UInt16 score, UInt8 spot)
{   //广播战报
    if(!player1 || spot > NEWHERO_ISLAND_SPOTS-1)
        return;
    Stream st(REP::NEWHERO_ISLAND);
    st << static_cast<UInt8>(0x09);
    st << player1->getName() << player1->getCountry();
    if(player2)
        st << player2->getName() << player2->getCountry();
    else
        st << "" << static_cast<UInt8>(2);
    st << score << spot << Stream::eos;
    for (std::map<Player *, NHIPlayerData *>::iterator iter = _allPlayer.begin(); iter != _allPlayer.end(); ++ iter)
    {
        if (iter->second == NULL || iter->second->status == NEWHERO_ISLAND_ESCAPE)
            continue;
        if (iter->second->spot == spot)
            iter->first->send(st);
    }
}
NewHeroIsland newHeroIsland;

}

