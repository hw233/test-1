#include "Config.h"
#include "MsgID.h"
#include "Common/URandom.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "DB/DBExecHelper.h"
#include "Common/StringTokenizer.h"

#include "SingleHeroStage.h"
#include "Player.h"
#include "Mail.h"
#include "Server/Cfg.h"
#include "Server/SysMsg.h"
#include "Common/Itoa.h"

#define HOUR (60*60)
// XXX neice
//#define MIN 6
#define MIN 60

#if 1
#define DEBUG(x, ...) fprintf(stderr, x, ##__VA_ARGS__);
#else
#define DEBUG(x, ...)
#endif



namespace GObject
{

struct DBSHGlobal
{
    UInt32 session;
    UInt32 nextTime;
    UInt8 progress;
    UInt8 open;
    UInt32 timeBegin;
    UInt32 timeEnd;
    UInt16 cls1TowerLevel;
    UInt16 cls2TowerLevel;
    UInt16 cls3TowerLevel;
};

struct DBSHStageProgress
{
    UInt8 round;
    UInt8 progress;
};

struct DBSHFinalPlayer
{
	UInt8 pos;
	UInt8 final3pos;
	UInt8 stageIdx;
	UInt64 playerId;
};

struct DBSHQualifyBattle
{
	UInt8 stageIdx;
	UInt8 id;
    std::string won;
    std::string battleId;
};

struct DBSHPriliminary
{
	UInt8 won;
	UInt64 player1;
	UInt64 player2;
	UInt32 battleTime;
	UInt32 battleId;
};

struct DBSHTowerBattle
{
    UInt64 playerId;
    UInt8  won;
    UInt16 level;
    UInt32 turns;
    UInt32 battleId;
    UInt32 npcId;
};


struct DBSHPlayer
{
	UInt64 playerId;
    UInt8 entered;
    UInt32 teamId;
    UInt16 tLevel;
    UInt32 tTurns;
    UInt16 tLastTurns;
    UInt32 score;
};

struct DBSHFighter
{
    UInt32 id;
	UInt64 playerId;
    UInt8 type;
    UInt8 level;
    Int32 soulMax;
    float potential;
    float capacity;
    UInt16 peerless;
    std::string citta;
    std::string trump;
    std::string skillstrengthen;
};

struct DBSHFighterAttrExtra 
{
    UInt32 fighterId;
    UInt64 playerId;
    UInt8 type;
	float strength;
	float physique;
	float agility;
	float intelligence;
	float will;
	float soul;
	float aura;
	float auraMax;
	float attack;
	float magatk;
	float defend;
	float magdef;
	float hp;
	float tough;
	float action;
	float hitrate;
	float evade;
	float critical;
	float criticaldmg;
	float pierce;
	float counter;
	float magres;

	float strengthP;
	float physiqueP;
	float agilityP;
	float intelligenceP;
	float willP;
	float soulP;
	float auraP;
	float auraMaxP;
	float attackP;
	float magatkP;
	float defendP;
	float magdefP;
	float hpP;
    float toughP;
	float actionP;
	float hitrateP;
	float evadeP;
	float criticalP;
	float criticaldmgP;
	float pierceP;
	float counterP;
	float magresP;

    float hitrlvl;
    float evdlvl;
    float crilvl;
    float pirlvl;
    float counterlvl;
    float mreslvl;
    float toughlvl;
};

struct DBSHAttr2
{
    UInt32 fighterId;
    UInt64 playerId;
    UInt8 type;
    UInt8 soulExtraAura;
    UInt8 soulAuraLeft;
    Int32 elixir_strength;
    Int32 elixir_physique;
    Int32 elixir_agility;
    Int32 elixir_intelligence;
    Int32 elixir_will;
    Int32 elixir_soul; // max soul
    Int32 elixir_attack; // atk,magatk
    Int32 elixir_defend; // def,magdef
    Int32 elixir_critical;
    Int32 elixir_pierce;
    Int32 elixir_evade;
    Int32 elixir_counter;
    Int32 elixir_tough;
    Int32 elixir_action;
};

struct DBSHCandidate
{
    UInt64 playerId;
    UInt8  jobClass;
    UInt32 supportedCount;
    UInt8  pos;
};

struct DBSupportPlayer
{
    UInt64 playerId;
    UInt8 flag;
    UInt64 class1PlayerId;
    UInt64 class2PlayerId;
    UInt64 class3PlayerId;
};

}

namespace DB
{

SPECIALBEGIN(GObject::DBSHGlobal)
SPECIALDEF(9)
(
    UInt32, session,
    UInt32, nextTime,
    UInt8, progress,
    UInt8, open,
    UInt32, timeBegin,
    UInt32, timeEnd,
    UInt16, cls1TowerLevel,
    UInt16, cls2TowerLevel,
    UInt16, cls3TowerLevel
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHStageProgress)
SPECIALDEF(2)
(
    UInt8, round,
    UInt8, progress
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHFinalPlayer)
SPECIALDEF(4)
(
	UInt8, pos,
	UInt8, final3pos,
	UInt8, stageIdx,
	UInt64, playerId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHQualifyBattle)
SPECIALDEF(4)
(
	UInt8, stageIdx,
	UInt8, id,
    std::string, won,
    std::string, battleId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHPriliminary)
SPECIALDEF(5)
(
	UInt8, won,
	UInt64, player1,
	UInt64, player2,
	UInt32, battleTime,
	UInt32, battleId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHTowerBattle)
SPECIALDEF(6)
(
    UInt64, playerId,
    UInt8,  won,
    UInt16, level,
    UInt32, turns,
    UInt32, battleId,
    UInt32, npcId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHPlayer)
SPECIALDEF(7)
(
	UInt64, playerId,
    UInt8, entered,
    UInt32, teamId,
    UInt16, tLevel,
    UInt32, tTurns,
    UInt16, tLastTurns,
    UInt32, score
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHFighter)
SPECIALDEF(10)
(
    UInt32, id,
	UInt64, playerId,
    UInt8, type,
    UInt8, level,
    Int32, soulMax,
    float, potential,
    float, capacity,
    std::string, citta,
    std::string, trump,
    std::string, skillstrengthen
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHFighterAttrExtra)
SPECIALDEF(54)
(
    UInt32, fighterId,
    UInt64, playerId,
    UInt8, type,
	float, strength,
	float, physique,
	float, agility,
	float, intelligence,
	float, will,
	float, soul,
	float, aura,
	float, auraMax,
	float, attack,
	float, magatk,
	float, defend,
	float, magdef,
	float, hp,
	float, tough,
	float, action,
	float, hitrate,
	float, evade,
	float, critical,
	float, criticaldmg,
	float, pierce,
	float, counter,
	float, magres,
	float, strengthP,
	float, physiqueP,
	float, agilityP,
	float, intelligenceP,
	float, willP,
	float, soulP,
	float, auraP,
	float, auraMaxP,
	float, attackP,
	float, magatkP,
	float, defendP,
	float, magdefP,
	float, hpP,
    float, toughP,
	float, actionP,
	float, hitrateP,
	float, evadeP,
	float, criticalP,
	float, criticaldmgP,
	float, pierceP,
	float, counterP,
	float, magresP,
    float, hitrlvl,
    float, evdlvl,
    float, crilvl,
    float, pirlvl,
    float, counterlvl,
    float, mreslvl,
    float, toughlvl
    )
SPECIALEND()


SPECIALBEGIN(GObject::DBSHAttr2)
SPECIALDEF(19)
(
    UInt32, fighterId,
    UInt64, playerId,
    UInt8, type,
    UInt8, soulExtraAura,
    UInt8, soulAuraLeft,
    Int32, elixir_strength,
    Int32, elixir_physique,
    Int32, elixir_agility,
    Int32, elixir_intelligence,
    Int32, elixir_will,
    Int32, elixir_soul, // max soul
    Int32, elixir_attack, // atk,magatk
    Int32, elixir_defend, // def,magdef
    Int32, elixir_critical,
    Int32, elixir_pierce,
    Int32, elixir_evade,
    Int32, elixir_counter,
    Int32, elixir_tough,
    Int32, elixir_action
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBSupportPlayer)
SPECIALDEF(5)
(
	UInt64, playerId,
    UInt8, flag,
    UInt64, class1PlayerId,
    UInt64, class2PlayerId,
    UInt64, class3PlayerId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSHCandidate)
SPECIALDEF(4)
(
	UInt64, playerId,
    UInt8,  jobClass,
    UInt32, supportedCount,
    UInt8,  pos
)
SPECIALEND()


}


namespace GObject
{

    static UInt8 chelun[5][3][2] = {
        {{0, 5}, {1, 4}, {2, 3}},
        {{0, 4}, {1, 2}, {3, 5}},
        {{0, 3}, {1, 5}, {2, 4}},
        {{0, 2}, {1, 3}, {4, 5}},
        {{0, 1}, {2, 5}, {3, 4}}
    };

    static UInt16 stageQualifyBattles[4] = { 8, 4, 2, 1 };
    static UInt16 stateQualifyIndex[4] = {0, 8, 12, 14};

    // 1-周日12点, 2-周一12点， 3-周一13点， 4-周二10点， 5-周二13点， 6-周三10点，
    // 7-周三13点, 8-周四10点，9-周四13点， 10-周五10点，11-周五13点，12-周六10点，13-周日0点
    // 1-报名,2-报名截止,3-单职业预选赛,4-单职业淘汰赛,5-策略预选赛,6-单职业决赛,
    // 7-明星预选赛,8-策略淘汰赛/人气,9-明星淘汰赛,10-策略决赛,11-明星决赛,12-逆转乾坤,13-结果
    static UInt32 stageStartTime[14] = 
        {12*HOUR,  12*HOUR,  13*HOUR,     10*HOUR,     13*HOUR,     10*HOUR,
         13*HOUR,  10*HOUR,  13*HOUR,     10*HOUR,     13*HOUR,     10*HOUR,     0,          0};

    static UInt32 stageTimeOffset[14] = 
        {0,        0,        0,           5*MIN,       5*MIN,      5*MIN,
         5*MIN,    5*MIN,    5*MIN,       5*MIN,       5*MIN,      5*MIN,      5*MIN,      0};


#define GET_STAGE_NAME(n, g) char n[1024]; { SysMsgItem * mi = globalSysMsg[880 + g]; if(mi != NULL) mi->get(n); else n[0] = 0; }
#define GET_STAGE_TYPE(n, g, t) char n[1024]; { SysMsgItem * mi = globalSysMsg[870 + 2*(g-1)+t]; if(mi != NULL) mi->get(n); else n[0] = 0; }
#define GET_STAGE_WIN(n, w) char n[1024]; { SysMsgItem * mi = globalSysMsg[878 + w]; if(mi != NULL) mi->get(n); else n[0] = 0; }

    SHBattleStageMgr shStageMgr;

    void QualifyBattle::reset()
    {
        _won.clear();
        _battleId.clear();
    }

    UInt8 QualifyBattle::push( UInt8 r, UInt32 bid )
    {
        _won.push_back(r);
        _battleId.push_back(bid);
        return winner();
    }

    UInt8 QualifyBattle::winner()
    {
        if(_won.size() < 3)
            return 0xFF;

        int cnt = 0;
        for(UInt8 i = 0; i < _won.size(); ++ i)
        {
            if(_won[i] == 0)
                ++ cnt;
        }

        if(cnt > 1)
            return 0;
        else
            return 1;
    }


    void SingleHeroPlayer::PutFighters( Battle::BattleSimulator& bsim, int side )
    {
        Battle::BattleFighter * bf = bsim.newFighter(side, 12, _fgt->getFighter());
        bf->setHP(0);
    }

    bool SingleHeroPlayer::challenge( SingleHeroPlayer* other, UInt32 * rid )
    {
        Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, _player, other->_player);
        PutFighters( bsim, 0 );
        other->PutFighters( bsim, 1 );

        bsim.start(0xFF, false);
        bool res = bsim.getWinner() == 1;
        *rid = bsim.getId();

        return res;
    }

    void SingleHeroPlayer::setEnter(Player* pl, SingleHeroFighter* fgt, UInt8 type, bool writeDB)
    {
        _type = type;
        _player = pl;
        _fgt = fgt;
        if(writeDB)
            DB1().PushUpdateData("UPDATE `sh_player` SET `entered`=`entered`|%u WHERE `playerId`=%"I64_FMT"u", type, _player->getId());
    }

    void SingleHeroPlayer::setTeam(UInt32 teamId)
    {
        _teamId = teamId;
        DB1().PushUpdateData("UPDATE `sh_player` SET `teamId`=%u WHERE `playerId`=%"I64_FMT"u", teamId, _player->getId());
    }


    void SHTowerPlayer::PutFighters( Battle::BattleSimulator& bsim )
    {
        Battle::BattleFighter * bf = bsim.newFighter(0, 12, _fgt->getFighter());
        bf->setHP(_fgt->getCurrentHP());
    }

    bool SHTowerPlayer::challenge(std::vector<std::vector<UInt32>>& monsters)
    {
        std::vector<UInt32>& npc = monsters[_tLevel];
        if(npc.size() == 0)
            return false;

        URandom& rnd = URandom::current();
        UInt32 npcId = npc[rnd(npc.size())];
        GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
        if(it == GData::npcGroups.end())
            return false;
        GData::NpcGroup * ng = it->second;
        if (!ng)
            return false;

        UInt32 turns = 0;
        Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, _player, ng->getName(), ng->getLevel(), false, turns);
        PutFighters( bsim );
        ng->putFighters( bsim );

        bsim.start(0xFF, false);
        bool res = bsim.getWinner() == 1;

        _battles[_tLevel+1] = bsim.getId();
        if(res)
        {
            _tTurns += turns;
            ++ _tLevel;
        }

        return res;
    }

    void SHTowerPlayer::setEnter(Player* pl, SingleHeroFighter* fgt, UInt8 type, bool writeDB)
    {
        _type = type;
        _player = pl;
        _fgt = fgt;
        if(writeDB)
            DB1().PushUpdateData("UPDATE `sh_player` SET `entered`=`entered`|%u WHERE `playerId`=%"I64_FMT"u", type, _player->getId());
    }


    void StarTeam::PutFighters( Battle::BattleSimulator& bsim, int side )
    {
        URandom& rnd = URandom::current();
        static UInt16 formation[12] = {101, 201, 301, 401, 501, 601, 701, 801, 901, 1001, 1101, 1201};
        UInt16 formId = formation[rnd(12)];
        std::vector<UInt8> posVec;
        for(int i = 0; i < 5; ++ i)
            posVec.push_back(i);

        size_t cnt = _playerList.size();
        for(UInt32 j = 0; j < cnt; ++ j)
        {
            UInt32 posCnt = posVec.size();
            UInt8 posIdx = rnd(posCnt);
            UInt8 idx = posVec[posIdx];
            posVec.erase(posVec.begin() + posIdx);
            posVec.resize(posCnt-1);
            UInt8 pos = (*GData::formationManager[formId])[idx].pos;
            Battle::BattleFighter * bf = bsim.newFighter(side, pos, _playerList[j]->_fgt->getFighter());
            bf->setHP(0);
        }
    }

    bool StarTeam::challenge( StarTeam * other, UInt32 * rid )
    {
        URandom& rnd = URandom::current();
        size_t cnt = _playerList.size();
        Player* pl = _playerList[rnd(cnt)]->_player;
        size_t othercnt = other->_playerList.size();
        Player* otherpl = other->_playerList[rnd(othercnt)]->_player;

        Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, pl, otherpl);
        PutFighters( bsim, 0 );
        other->PutFighters( bsim, 1 );

        bsim.start(0xFF, false);
        bool res = bsim.getWinner() == 1;
        *rid = bsim.getId();

        return res;
    }



    SHBattleStage::SHBattleStage(StageType type, GroupType gpId, UInt8 cls) : m_currIdx(0), m_swapIdx(1), m_gpId(gpId), m_cls(cls), m_type(type), m_round(0), m_progress(e_sh_pre)
    {
        memset(m_finals, 0, sizeof(m_finals));
        memset(m_3finals, 0, sizeof(m_3finals));
        memset(m_finalBattles, 0, sizeof(m_finalBattles));
        memset(m_3finalBattles, 0, sizeof(m_3finalBattles));
    }

    SHBattleStage::~SHBattleStage()
    {
    }

    bool SHBattleStage::enter(Player* player, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return false;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return true;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->setEnter(player, fgt, m_type);
        m_players[player] = sp;
        m_preliminary[m_currIdx][0].push_back(sp);

        return true;
    }

    void SHBattleStage::pushPlayer(Player* player, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->setEnter(player, fgt, m_type, false);
        m_players[player] = sp;
    }

    bool SHBattleStage::processPreliminary()
    {
        if(m_progress > e_sh_pre)
            return true;
        UInt32 pCnt = getPlayerCount();
        if(pCnt == 0)
        {
            ++ m_progress;
            return true;
        }
        else if(pCnt <= STAGE_FINAL_PLAYER_CNT)
        {
            std::swap(m_currIdx, m_swapIdx);
            initQualifyStage();
            ++ m_progress;
            DB1().PushUpdateData("UPDATE `sh_stage_progress` SET `round`=%u, `progress`=%u WHERE `gpId`=%u AND `type`=%u AND `cls`=%u", m_round, m_progress, m_gpId, m_type, m_cls);
            return true;
        }

        const size_t minLeft[5] = {2, 2, 4, 8, 16};
        UInt32 totalLeft = STAGE_FINAL_PLAYER_CNT;
        ++ m_round;
        UInt32 totalSize = 0;

        for(int i = 0; i < 5; ++ i)
            totalSize += m_preliminary[m_currIdx][i].size();

        URandom& rnd = URandom::current();
        bool ret = true;
        for(int i = 0; i < 5; ++ i)
        {
            std::vector<SingleHeroPlayer*>& spl = m_preliminary[m_currIdx][i];
            UInt32 size = static_cast<UInt32>(spl.size());
            UInt32 size2 = size;
            UInt8 mod = 0;
            if(size%2)
            {
                UInt32 pos = rnd(size);
                std::vector<SingleHeroPlayer*>::iterator it = spl.begin();
                std::advance(it, pos);
                m_preliminary[m_swapIdx][i].push_back(*it);
                spl.erase(it);
                ++ mod;
            }

            int round = size/2;
            bool flag = false;
            size += m_preliminary[m_swapIdx][i].size() - mod;
            for(int j = 0; j < round; ++ j)
            {
                if(size <= minLeft[i] || totalSize <= totalLeft)
                {
                    flag = true;
                    for(std::vector<SingleHeroPlayer*>::iterator it = spl.begin(); it != spl.end(); ++ it)
                    {
                        m_preliminary[m_swapIdx][i].push_back(*it);
                    }
                    spl.clear();
                    break;;
                }

                UInt32 pos1 = rnd(size2 - mod);
                std::vector<SingleHeroPlayer*>::iterator it1 = spl.begin();
                std::advance(it1, pos1);
                SingleHeroPlayer* pl1 = *it1;
                spl.erase(it1);
                -- size2;

                UInt32 pos2 = rnd(size2 - mod);
                std::vector<SingleHeroPlayer*>::iterator it2 = spl.begin();
                std::advance(it2, pos2);
                SingleHeroPlayer* pl2 = *it2;
                spl.erase(it2);
                -- size2;

                UInt32 rid = 0;
                bool result = pl1->challenge(pl2, &rid);
                StageBattleReport sbr;
                StageBattleReport sbr2;
                sbr._type = m_type;
                sbr._won = !result;
                sbr._other = pl2->_player;
                sbr._battleTime = TimeUtil::Now();
                sbr._battleId = rid;
                pl1->_battles.push_back(sbr);
                sbr2._type = m_type;
                sbr2._won = result;
                sbr2._other = pl1->_player;
                sbr2._battleTime = TimeUtil::Now();
                sbr2._battleId = rid;
                pl2->_battles.push_back(sbr2);
                Stream st1(REP::SINGLE_HERO);
                Stream st2(REP::SINGLE_HERO);
                st1 << static_cast<UInt8>(0x06) << static_cast<UInt8>(m_gpId) << static_cast<UInt8>(0x01);
                st2 << static_cast<UInt8>(0x06) << static_cast<UInt8>(m_gpId) << static_cast<UInt8>(0x01);
                st1 << static_cast<UInt8>((m_type & e_sh_slsy) ? 0 : 1);
                st2 << static_cast<UInt8>((m_type & e_sh_slsy) ? 0 : 1);
                if(result)
                {
                    st1 << static_cast<UInt8>(0x00);
                    st2 << static_cast<UInt8>(0x01);
                    ++ pl1->_won;
                    m_preliminary[m_swapIdx][i].push_back(pl1);
                    if(i < 4)
                    {
                        m_preliminary[m_swapIdx][i + 1].push_back(pl2);
                    }
                    else
                    {
                        -- totalSize;
                        if(m_type == e_sh_slsy)
                        {
                            SingleHeroFighter* fgt = pl2->_fgt;
                            Player* pl = pl2->_player;
                            if(m_gpId == e_sh_gp_sl)
                                shStageMgr.singleStage2(m_cls)->enter(pl, fgt);
                            else
                                shStageMgr.strategyStage2(m_cls)->enter(pl, fgt);
                        }
                        else
                        {
                            sendStageMail(pl2, false);
                        }
                        pl2->_battles.back()._won = 2;
                    }
                }
                else
                {
                    st1 << static_cast<UInt8>(0x01);
                    st2 << static_cast<UInt8>(0x00);
                    ++ pl2->_won;
                    m_preliminary[m_swapIdx][i].push_back(pl2);
                    if(i < 4)
                    {
                        m_preliminary[m_swapIdx][i + 1].push_back(pl1);
                    }
                    else
                    {
                        -- totalSize;
                        if(m_type == e_sh_slsy)
                        {
                            SingleHeroFighter* fgt = pl1->_fgt;
                            Player* pl = pl1->_player;
                            if(m_gpId == e_sh_gp_sl)
                                shStageMgr.singleStage2(m_cls)->enter(pl, fgt);
                            else
                                shStageMgr.strategyStage2(m_cls)->enter(pl, fgt);
                        }
                        else
                        {
                            sendStageMail(pl1, false);
                        }
                        pl1->_battles.back()._won = 2;
                    }
                }
                -- size;

                st1 << static_cast<UInt16>(pl2->_fgt->getId()) << static_cast<UInt32>(rid) << pl2->_player->getName() << Stream::eos;
                st2 << static_cast<UInt16>(pl1->_fgt->getId()) << static_cast<UInt32>(rid) << pl1->_player->getName() << Stream::eos;

                pl1->_player->send(st1);
                pl2->_player->send(st2);

                DB1().PushUpdateData("REPLACE INTO `sh_preliminary`(`gpId`, `cls`, `type`, `won`, `player1`, `player2`, `battleTime`, `battleId`) VALUES(%u, %u, %u, %u, %"I64_FMT"u, %"I64_FMT"u, %u, %u)", m_gpId, m_cls, m_type, result ? 1 : 0, pl1->_player->getId(), pl2->_player->getId(), sbr._battleTime, sbr._battleId);
            }
            if(!flag)
            {
                if(size <= minLeft[i] || totalSize < totalLeft)
                {
                    for(std::vector<SingleHeroPlayer*>::iterator it = spl.begin(); it != spl.end(); ++ it)
                    {
                        m_preliminary[m_swapIdx][i].push_back(*it);
                    }
                    spl.clear();
                }
                else
                    ret = false;
            }
        }

        if(ret)
        {
            ++ m_progress;
            initQualifyStage();
        }

        write2DB();
        for(int k = 0; k < 5; ++ k)
            m_preliminary[m_currIdx][k].clear();

        std::swap(m_currIdx, m_swapIdx);
        return ret;
    }

    bool SHBattleStage::processFinals()
    {
        if(m_progress < e_sh_16)
            return false;
        if(m_progress > e_sh_2)
            return true;

        int idx = m_progress - 2;
        for(int j = 0; j < 2; ++ j)
        {
            for(int i = 0; i < stageQualifyBattles[idx]; ++ i)
            {
                SingleHeroPlayer* bspl1 = m_finals[j][idx][i * 2];
                SingleHeroPlayer* bspl2 = m_finals[j][idx][i * 2 + 1];

                int curridx = stateQualifyIndex[idx] + i;
                QualifyBattle& elimBattle = m_finalBattles[j][curridx];
                elimBattle._id = curridx;
                UInt32 battleId;
                UInt8 r = 0xFF;
                if(bspl1 == NULL)
                    r = elimBattle.push(1, 0);
                else if(bspl2 == NULL)
                    r = elimBattle.push(0, 0);
                else
                {
                    if(bspl1->challenge(bspl2, &battleId))
                        r = elimBattle.push(0, battleId);
                    else
                        r = elimBattle.push(1, battleId);
                }
                if(r == 0)
                {
                    m_finals[j][idx + 1][i] = bspl1;
                    if(idx == 3)
                    {
                        if(bspl1 != NULL)
                        {
                            m_3finals[0][j] = bspl1;
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%"I64_FMT"u", j+1, m_gpId, m_type, bspl1->_player->getId());
                        }
                        if(bspl2 != NULL)
                        {
                            m_3finals[0][j+2] = bspl2;
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%"I64_FMT"u", j+3, m_gpId, m_type, bspl2->_player->getId());
                        }
                    }
                }
                else if(r == 1)
                {
                    m_finals[j][idx + 1][i] = bspl2;
                    if(idx == 3)
                    {
                        if(bspl2 != NULL)
                        {
                            m_3finals[0][j] = bspl2;
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%"I64_FMT"u", j+1, m_gpId, m_type, bspl2->_player->getId());
                        }
                        if(bspl1 != NULL)
                        {
                            m_3finals[0][j+2] = bspl1;
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%"I64_FMT"u", j+3, m_gpId, m_type, bspl1->_player->getId());
                        }
                    }
                }

                std::string won = "";
                std::string battleIds = "";
                int cnt = elimBattle._won.size();

                char tmp[256] = {0};
                sprintf(tmp, "%u", elimBattle._won[0]);
                won += tmp;
                sprintf(tmp, "%u", elimBattle._battleId[0]);
                battleIds += tmp;

                for(int k = 1; k < cnt; ++ k)
                {
                    char tmp[256] = {0};
                    sprintf(tmp, ", %u", elimBattle._won[k]);
                    won += tmp;
                    sprintf(tmp, ", %u", elimBattle._battleId[k]);
                    battleIds += tmp;
                }

                DB1().PushUpdateData("REPLACE INTO `sh_elimination`(`gpId`, `type`, `cls`, `stage_idx`, `id`, `won`, `battleId`) VALUES(%u, %u, %u, %u, %u, '%s', '%s')", m_gpId, m_type, m_cls, j, curridx, won.c_str(), battleIds.c_str());
            }
        }

        ++ m_round;
        if(m_round == 3)
        {
            ++ m_progress;
            m_round = 0;
        }

        write2DB();

        return false;
    }

    bool SHBattleStage::process3Finals()
    {
        if(m_progress > e_sh_1)
            return true;

        bool ret = false;

        for(int i = 0; i < 2; ++ i)
        {
            SingleHeroPlayer* bspl1 = m_3finals[0][2*i];
            SingleHeroPlayer* bspl2 = m_3finals[0][2*i+1];
            if(bspl1 == NULL && bspl2 == NULL)
                continue;

            QualifyBattle& elimBattle = m_3finalBattles[i];
            elimBattle._id = i;
            UInt32 battleId;
            UInt8 r = 0xFF;
            if(bspl1 == NULL)
                r = elimBattle.push(1, 0);
            else if(bspl2 == NULL)
                r = elimBattle.push(0, 0);
            else
            {
                if(bspl1->challenge(bspl2, &battleId))
                    r = elimBattle.push(0, battleId);
                else
                    r = elimBattle.push(1, battleId);
            }
            if(r == 0)
            {
                m_3finals[1][2*i] = bspl1;
                m_3finals[1][2*i+1] = bspl2;
            }
            else if(r == 1)
            {
                m_3finals[1][2*i] = bspl2;
                m_3finals[1][2*i+1] = bspl1;
            }

            std::string won = "";
            std::string battleIds = "";
            int cnt = elimBattle._won.size();

            char tmp[256] = {0};
            sprintf(tmp, "%u", elimBattle._won[0]);
            won += tmp;
            sprintf(tmp, "%u", elimBattle._battleId[0]);
            battleIds += tmp;

            for(int j = 1; j < cnt; ++ j)
            {
                char tmp[256] = {0};
                sprintf(tmp, ", %u", elimBattle._won[j]);
                won += tmp;
                sprintf(tmp, ", %u", elimBattle._battleId[j]);
                battleIds += tmp;
            }
            DB1().PushUpdateData("REPLACE INTO `sh_elimination`(`gpId`, `type`, `cls`, `stage_idx`, `id`, `won`, `battleId`) VALUES(%u, %u, %u, 2, %u, '%s', '%s')", m_gpId, m_type, m_cls, i, won.c_str(), battleIds.c_str());
        }

        ++ m_round;

        if(m_round == 3)
        {
            m_round = 0;
            ret = true;
            ++ m_progress;
            for(int i = 0; i < 3; ++ i)
            {
                if(!m_3finals[1][i])
                    continue;
                SingleHeroFighter* fgt = m_3finals[1][i]->_fgt;
                Player* pl = m_3finals[1][i]->_player;
                if(m_gpId == e_sh_gp_sl)
                    shStageMgr.singleFinalStage(m_cls)->enter(pl, fgt);
                else
                    shStageMgr.strategyFinalStage(m_cls)->enter(pl, fgt);
            }
        }

        write2DB();

        return ret;
    }

    void SHBattleStage::initQualifyStage()
    {
        bool odd = false;
        int j = 0;
        URandom& rnd = URandom::current();

        for(int k = 0; k < 2; ++ k)
            for(int i = 0; i < 15; ++ i)
                m_finalBattles[k][i].reset();
        for(int i = 0; i < 5; ++ i)
        {
            std::vector<SingleHeroPlayer*>& spl = m_preliminary[m_swapIdx][i];
            for(std::vector<SingleHeroPlayer*>::iterator it = spl.begin(); it != spl.end(); ++ it)
            {
                int base;
                if(odd)
                    base = 8;
                else
                    base = 0;
                int n = rnd(8);
                if(m_finals[j][0][base + n] != NULL)
                {
                    int endn = n;
                    do {
                        n = (n + 1) % 8;
                    } while (n != endn && m_finals[j][0][base + n] != NULL);
                    if(n == endn)
                    {
                        continue;
                    }
                }
                (*it)->_won = 0;
                m_finals[j][0][base + n] = *it;
                sendStageMail(*it, true);
                DB1().PushUpdateData("REPLACE INTO `sh_final_player`(`gpId`, `type`, `cls`, `stage_idx`, `pos`, `playerId`, `final3pos`) VALUES(%u, %u, %u, %u, %u, %"I64_FMT"u, 0)", m_gpId, m_type, m_cls, j, base+n, (*it)->_player->getId());
                if(m_gpId == e_sh_gp_sl)
                {
                    Player* pl = (*it)->_player;
                    shStageMgr.newCandidate(pl, (*it)->_fgt->getClass());
                    SingleHeroFighter* fgt = (*it)->_fgt;
                    shStageMgr.starStage()->enter(pl, fgt);
                }
                if(j != 0)
                {
                    odd = !odd;
                    j = 0;
                }
                else
                    j = 1;
            }
        }
        m_round = 0;
        write2DB();
    }

    void SHBattleStage::sendStageMail(SingleHeroPlayer* spl, bool win)
    {
        UInt8 type = 2;
        if(m_type == e_sh_slsy)
            type = 1;
        GET_STAGE_NAME(sn, m_gpId)
        GET_STAGE_TYPE(st, m_gpId, type)

        SYSMSGV(title, 880, sn);

        std::string content;
        if(win)
        {
            SYSMSGV(content1, 886, sn, st, sn);
            content = content1;
        }
        else
        {
            SYSMSGV(content1, 887, sn);
            content = content1;
        }
        std::string vs = "\n";
        std::vector<StageBattleReport>& battles = spl->_battles;
        size_t cnt = battles.size();
        for(size_t i = 0; i < cnt; ++ i)
        {
            GET_STAGE_WIN(sw, battles[i]._won)
            SYSMSGV(content1, 889, spl->_player->getName().c_str(), battles[i]._other->getName().c_str(), sw);
            vs += content1;
            vs += "\n";
        }
        {
            SYSMSGV(content1, 888, vs.c_str());
            content += content1;
        }

        spl->_player->GetMailBox()->newMail(NULL, 0x01, title, content.c_str());
    }

    void SHBattleStage::loadFromDB()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        char querystr[1024] = {0};

        sprintf(querystr, "SELECT `round`, `progress` FROM `sh_stage_progress` WHERE `type`=%u AND `gpId`=%u AND `cls`=%u", m_type, m_gpId, m_cls);
        DBSHStageProgress stageProgress = {0};
        if(execu->Extract(querystr, stageProgress) == DB::DB_OK)
        {
            m_round = stageProgress.round;
            m_progress = stageProgress.progress;
        }

        {
            DBSHFinalPlayer fplayer;
            bool flag = false;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `pos`, `final3pos`, `stage_idx`, `playerId` FROM `sh_final_player` WHERE `type`=%u AND `gpId`=%u AND `cls`=%u", m_type, m_gpId, m_cls);
            if(execu->Prepare(querystr, fplayer) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    if(fplayer.pos > 15)
                        continue;
                    Player * pl = globalPlayers[fplayer.playerId];
                    if(pl == NULL)
                        continue;
                    std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(pl);
                    if(it == m_players.end())
                        continue;
                    m_finals[fplayer.stageIdx][0][fplayer.pos] = it->second;
                    if(!flag)
                        flag = true;
                    if(fplayer.final3pos > 0)
                        m_3finals[0][fplayer.final3pos-1] = it->second;
                }
            }

            if(flag)
            {
                DBSHQualifyBattle dbqb;
                memset(querystr, 0, sizeof(querystr));
                sprintf(querystr, "SELECT `stage_idx`, `id`, `won`, `battleId` FROM `sh_elimination` WHERE `gpId`=%u AND `type`=%u AND `cls`=%u", m_gpId, m_type, m_cls);
                if(execu->Prepare(querystr, dbqb) == DB::DB_OK)
                {
                    while(execu->Next() == DB::DB_OK)
                    {
                        if(dbqb.id > 14)
                            continue;
                        if(dbqb.stageIdx > 1)
                        {
                            QualifyBattle& qb = m_3finalBattles[dbqb.id];
                            qb._id = dbqb.id;
                            {
                                StringTokenizer tokenizer(dbqb.won, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._won.push_back(atoi(tokenizer[j].c_str()));
                            }
                            {
                                StringTokenizer tokenizer(dbqb.battleId, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._battleId.push_back(atoi(tokenizer[j].c_str()));
                            }
                            UInt8 winner = qb.winner();
                            if(winner == 0xFF)
                                continue;

                            if(winner == 0)
                            {
                                m_3finals[1][qb._id*2] = m_3finals[0][qb._id*2];
                                m_3finals[1][qb._id*2+1] = m_3finals[0][qb._id*2+1];
                            }
                            else
                            {
                                m_3finals[1][qb._id*2] = m_3finals[0][qb._id*2+1];
                                m_3finals[1][qb._id*2+1] = m_3finals[0][qb._id*2];
                            }
                        }
                        else
                        {
                            QualifyBattle& qb = m_finalBattles[dbqb.stageIdx][dbqb.id];
                            qb._id = dbqb.id;
                            {
                                StringTokenizer tokenizer(dbqb.won, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._won.push_back(atoi(tokenizer[j].c_str()));
                            }
                            {
                                StringTokenizer tokenizer(dbqb.battleId, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._battleId.push_back(atoi(tokenizer[j].c_str()));
                            }
                            UInt8 winner = qb.winner();
                            if(winner == 0xFF)
                                continue;

                            {
                                int j = 1;
                                const int aIndex[5] = {0, 8, 12, 14, 15};
                                while(dbqb.id >= aIndex[j]) ++ j;
                                int idx = static_cast<int>(dbqb.id) - aIndex[j - 1];
                                if(winner == 0)
                                    m_finals[dbqb.stageIdx][j][idx] = m_finals[dbqb.stageIdx][j - 1][idx * 2];
                                else
                                    m_finals[dbqb.stageIdx][j][idx] = m_finals[dbqb.stageIdx][j - 1][idx * 2 + 1];
                            }
                        }

                    }
                }
            }
        }
        {
            DBSHPriliminary pril;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `won`, `player1`, `player2`, `battleTime`, `battleId` FROM `sh_preliminary` WHERE `type`=%u AND `gpId` = %u AND `cls`=%u ORDER BY `battleTime`", m_type, m_gpId, m_cls);
            if(execu->Prepare(querystr, pril) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    Player* player1 = globalPlayers[pril.player1];
                    Player* player2 = globalPlayers[pril.player2];
                    SingleHeroPlayer* spl1 = m_players[player1];
                    if(spl1 == NULL)
                    {
                        m_players.erase(player1);
                        continue;
                    }
                    SingleHeroPlayer* spl2 = m_players[player2];
                    if(spl2 == NULL)
                    {
                        m_players.erase(player2);
                        continue;
                    }
                    if(pril.won == 1)
                    {
                        ++ spl1->_won;
                    }
                    else
                    {
                        ++ spl2->_won;
                    }
                    StageBattleReport sbr;
                    StageBattleReport sbr2;
                    sbr._type = m_type;
                    sbr._won = !pril.won;
                    sbr._other = spl2->_player;
                    sbr._battleTime = pril.battleTime;
                    sbr._battleId = pril.battleId;
                    spl1->_battles.push_back(sbr);

                    sbr2._type = m_type;
                    sbr2._won = pril.won;
                    sbr2._other = spl1->_player;
                    sbr2._battleTime = pril.battleTime;
                    sbr2._battleId = pril.battleId;
                    spl2->_battles.push_back(sbr2);
                }
            }
            UInt16 prePlayerNum = 0;
            for(std::map<Player*, SingleHeroPlayer*>::iterator iter = m_players.begin(); iter != m_players.end(); ++ iter)
            {
                if(iter->second == NULL)
                    continue;
                UInt16 total = static_cast<UInt16>(iter->second->_battles.size());
                UInt16 lost = total - iter->second->_won;
                if(lost < 5)
                {
                    ++ prePlayerNum;
                    m_preliminary[m_currIdx][lost].push_back(iter->second);
                }
                else
                {
                    if(total > 0)
                        iter->second->_battles.back()._won = 2;
                }
            }
            fprintf(stdout, "gpId:[%d] type[%d] class[%d] ----preliminary player count %d\n", m_gpId, m_type, m_cls, prePlayerNum);
            fflush(stdout);
        }
    }

    void SHBattleStage::clear()
    {
        m_currIdx = 0;
        m_swapIdx = 1;
        m_round = 0;
        m_progress = e_sh_pre;
        for(int i = 0; i < 5; ++ i)
        {
            m_preliminary[0][i].clear();
            m_preliminary[1][i].clear();
        }
        for(int j = 0; j < 2; ++ j)
            for(int i = 0; i < 15; ++ i)
                m_finalBattles[j][i].reset();
        memset(m_finals, 0, sizeof(m_finals));

        m_3finalBattles[0].reset();
        m_3finalBattles[1].reset();
        memset(m_3finals, 0, sizeof(m_3finals));
        for(std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.begin(); it != m_players.end(); ++ it)
        {
            delete it->second;
        }
        m_players.clear();

        DB1().PushUpdateData("DELETE FROM `sh_preliminary` WHERE `gpId` = %u AND `type`=%u AND `cls`=%u", m_gpId, m_type, m_cls);
        DB1().PushUpdateData("DELETE FROM `sh_final_player` WHERE `gpId` = %u AND `type`=%u AND `cls`=%u", m_gpId, m_type, m_cls);
        DB1().PushUpdateData("DELETE FROM `sh_elimination` WHERE `gpId` = %u AND `type`=%u AND `cls`=%u", m_gpId, m_type, m_cls);
        DB1().PushUpdateData("DELETE FROM `sh_stage_progress` WHERE `gpId`=%u AND `type` = %u AND `cls`=%u", m_gpId, m_type, m_cls);
    }

    void SHBattleStage::write2DB()
    {
        DB1().PushUpdateData("REPLACE INTO `sh_stage_progress` (`gpId`, `type`, `cls`, `round`, `progress`) VALUES(%u, %u, %u, %u, %u)", m_gpId, m_type, m_cls, m_round, m_progress);
    }

    bool SHBattleStage::sendEliminary(Stream& st, UInt8 type)
    {
        static UInt8 aIndex[5] = {0, 8, 12, 14, 15};
        UInt8 round = 0;

        if(m_progress < e_sh_16)
            return false;

        if(type < 2)
        {
            for(int i = 0; i < 16; ++ i)
            {
                if(m_finals[type][0][i] != NULL)
                {
                    st << static_cast<UInt16>(m_finals[type][0][i]->_fgt->getId());
                    st << static_cast<UInt8>(m_finals[type][0][i]->_fgt->getLevel());
                    st << m_finals[type][0][i]->_player->getName();
                }
                else
                {
                    st << static_cast<UInt16>(0);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
            }

            round = m_progress - 2;
            if(m_round > 0)
                ++ round;
            if(round > 4)
                round = 4;
            st << round;
            for(int r = 0; r < aIndex[round]; ++ r)
            {
                UInt8 cnt = m_finalBattles[type][r]._battleId.size();
                UInt8 wonFlag = 0;
                for(int i = 0; i < cnt; ++ i)
                    wonFlag |= (m_finalBattles[type][r]._won[i] << i);
                st << cnt << wonFlag;
                for(int j = 0; j < cnt; ++ j)
                    st << m_finalBattles[type][r]._battleId[j];
            }
        }
        else
        {
            for(int i = 0; i < 4; ++ i)
            {
                if(m_3finals[0][i] != NULL)
                {
                    st << static_cast<UInt16>(m_3finals[0][i]->_fgt->getId());
                    st << static_cast<UInt8>(m_3finals[0][i]->_fgt->getLevel());
                    st << m_3finals[0][i]->_player->getName();
                }
                else
                {
                    st << static_cast<UInt16>(0);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
            }
            if(m_progress > 6)
                round = 5;
            st << round;
            for(int r = 0; r < 2; ++ r)
            {
                UInt8 cnt = m_3finalBattles[r]._battleId.size();
                UInt8 wonFlag = 0;
                for(int i = 0; i < cnt; ++ i)
                    wonFlag |= (m_3finalBattles[r]._won[i] << i);
                st << cnt << wonFlag;
                for(int j = 0; j < cnt; ++ j)
                    st << m_3finalBattles[r]._battleId[j];
            }
        }

        return true;
    }

    bool SHBattleStage::isInFinal(Player* pl)
    {
        bool find = false;
        for(int i = 0; i < 2; ++ i)
        {
            for(int j = 0; j < 16; ++ j)
            {
                if(!m_finals[i][0][j])
                    continue;
                if(m_finals[i][0][j]->_player == pl)
                {
                    find = true;
                    break;
                }
            }
            if(find)
                break;
        }

        return find;
    }


    SHFinalBattleStage::SHFinalBattleStage(StageType type, GroupType gpId, UInt8 cls) : m_gpId(gpId), m_cls(cls), m_progress(e_sh_0), m_round(0), m_type(type)
    {
    }

    SHFinalBattleStage::~SHFinalBattleStage()
    {
    }

    void SHFinalBattleStage::clear()
    {
        m_round = 0;
        m_progress = e_sh_0;

        for(std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.begin(); it != m_players.end(); ++ it)
        {
            delete it->second;
        }
        m_players.clear();
        m_finals.clear();
        m_finals.resize(0);
        DB1().PushUpdateData("DELETE FROM `sh_preliminary` WHERE `gpId` = %u AND `type`=%u AND `cls`=%u", m_gpId, m_type, m_cls);
        DB1().PushUpdateData("DELETE FROM `sh_final_player` WHERE `gpId` = %u AND `type`=%u AND `cls`=%u", m_gpId, m_type, m_cls);
        DB1().PushUpdateData("DELETE FROM `sh_stage_progress` WHERE `gpId`=%u AND `type` = %u AND `cls`=%u", m_gpId, m_type, m_cls);
    }

    bool SHFinalBattleStage::enter(Player* player, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return false;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return true;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->setEnter(player, fgt, m_type);
        m_players[player] = sp;
        m_finals.push_back(sp);

        int pos = m_finals.size() - 1;
        DB1().PushUpdateData("REPLACE INTO `sh_final_player`(`gpId`, `type`, `cls`, `stage_idx`, `pos`, `playerId`, `final3pos`) VALUES(%u, %u, %u, 2, %u, %"I64_FMT"u, 0)", m_gpId, m_type, m_cls, pos, player->getId());
        return true;
    }

    void SHFinalBattleStage::pushPlayer(Player* player, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->setEnter(player, fgt, m_type, false);
        m_players[player] = sp;
    }

    void SHFinalBattleStage::write2DB()
    {
        DB1().PushUpdateData("REPLACE INTO `sh_stage_progress` (`gpId`, `type`, `cls`, `round`, `progress`) VALUES(%u, %u, %u, %u, %u)", m_gpId, m_type, m_cls, m_round, m_progress);
    }

    bool SHFinalBattleStage::process()
    {
        if(m_progress > e_sh_0)
            return true;
        bool ret = false;

        for(UInt8 i = 0; i < 3; ++ i)
        {
            UInt32 rid = 0;
            UInt8 idx1 = chelun[m_round][i][0];
            UInt8 idx2 = chelun[m_round][i][1];

            SingleHeroPlayer* sp1 = NULL;
            SingleHeroPlayer* sp2 = NULL;
            if(idx1 < m_finals.size())
                sp1 = m_finals[idx1];

            if(idx2 < m_finals.size())
                sp2 = m_finals[idx2];

            if(sp1 == NULL || sp2 == NULL)
            {
                continue;
            }

            bool result = sp1->challenge(sp2, &rid);
            StageBattleReport sbr;
            StageBattleReport sbr2;
            sbr._type = m_type;
            sbr._won = !result;
            sbr._other = sp2->_player;
            sbr._battleTime = TimeUtil::Now();
            sbr._battleId = rid;
            sp1->_battles.push_back(sbr);
            sbr2._type = m_type;
            sbr2._won = result;
            sbr2._other = sp1->_player;
            sbr2._battleTime = TimeUtil::Now();
            sbr2._battleId = rid;
            sp2->_battles.push_back(sbr2);
            if(result)
                ++ sp1->_won;
            else
                ++ sp2->_won;

            DB1().PushUpdateData("REPLACE INTO `sh_preliminary`(`gpId`, `cls`, `type`, `won`, `player1`, `player2`, `battleTime`, `battleId`) VALUES(%u, %u, %u, %u, %"I64_FMT"u, %"I64_FMT"u, %u, %u)", m_gpId, m_cls, m_type, result ? 1 : 0, sp1->_player->getId(), sp2->_player->getId(), sbr._battleTime, sbr._battleId);
        }

        ++ m_round;
        if(m_round == 5)
        {
            ret = true;
            ++ m_progress;

            class SHGiveFinalAward : public SHPlayerVisitor
            {
            public:
                virtual bool operator()(SingleHeroPlayer* sp, UInt8 cls, UInt8 idx)
                {
                    static UInt32 score1[3] = {20, 10, 5};
                    static UInt32 score2[3] = {3, 2, 1};
                    if(!sp)
                        return false;

                    if(sp->_type == e_sh_sl_final)
                    {
                        shStageMgr.addScore(sp->_player, cls, score1[idx], true);
                        shStageMgr.setTopCandidate(sp->_player, cls, idx+1);
                    }
                    else
                    {
                        shStageMgr.addScore(sp->_player, cls, score2[idx], true);
                    }

                    m_sbs->sendStageMail(sp, idx);

                    return true;
                }

                SHGiveFinalAward(SHFinalBattleStage* sbs) { m_sbs = sbs; }
                SHFinalBattleStage* m_sbs;
            };

            SHGiveFinalAward awardvisitor(this);
            visitFinals(awardvisitor);
        }

        write2DB();
        return ret;
    }

    void SHFinalBattleStage::sendStageMail(SingleHeroPlayer* spl, UInt8 idx)
    {
        static MailPackage::MailItem award[3][2] = 
        {
            {{509, 3}, {9076, 3}},
            {{509, 2}, {9076, 2}},
            {{509, 1}, {9076, 1}},
        };

        GET_STAGE_NAME(sn, m_gpId)
        SYSMSGV(title, 880, sn);
        SYSMSGV(content, 890, sn, sn, idx + 1);

        Mail * mail = spl->_player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail != NULL)
        {
            Player* pl = spl->_player;
            MailPackage::MailItem* mitem = &award[idx][0];
            UInt32 size = 2;
            std::string strItems;
            for (UInt32 i = 0; i < size; ++i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            mailPackageManager.push(mail->id, mitem, size, true);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    void SHFinalBattleStage::loadFromDB()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        char querystr[1024] = {0};

        sprintf(querystr, "SELECT `round`, `progress` FROM `sh_stage_progress` WHERE `type`=%u AND `gpId`=%u AND `cls`=%u", m_type, m_gpId, m_cls);
        DBSHStageProgress stageProgress = {0};
        if(execu->Extract(querystr, stageProgress) == DB::DB_OK)
        {
            m_round = stageProgress.round;
            m_progress = stageProgress.progress;
        }

        {
            DBSHPriliminary pril;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `won`, `player1`, `player2`, `battleTime`, `battleId` FROM `sh_preliminary` WHERE `type`=%u AND `gpId` = %u AND `cls`=%u ORDER BY `battleTime`", m_type, m_gpId, m_cls);
            if(execu->Prepare(querystr, pril) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    Player* player1 = globalPlayers[pril.player1];
                    Player* player2 = globalPlayers[pril.player2];
                    SingleHeroPlayer* spl1 = m_players[player1];
                    if(spl1 == NULL)
                    {
                        m_players.erase(player1);
                        continue;
                    }
                    SingleHeroPlayer* spl2 = m_players[player2];
                    if(spl2 == NULL)
                    {
                        m_players.erase(player2);
                        continue;
                    }
                    if(pril.won == 1)
                    {
                        ++ spl1->_won;
                    }
                    else
                    {
                        ++ spl2->_won;
                    }
                    StageBattleReport sbr;
                    StageBattleReport sbr2;
                    sbr._type = m_type;
                    sbr._won = !pril.won;
                    sbr._other = spl2->_player;
                    sbr._battleTime = pril.battleTime;
                    sbr._battleId = pril.battleId;
                    spl1->_battles.push_back(sbr);

                    sbr2._type = m_type;
                    sbr2._won = pril.won;
                    sbr2._other = spl1->_player;
                    sbr2._battleTime = pril.battleTime;
                    sbr2._battleId = pril.battleId;
                    spl2->_battles.push_back(sbr2);
                }
            }
        }

        {
            DBSHFinalPlayer fplayer;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `pos`, `final3pos`, `stage_idx`, `playerId` FROM `sh_final_player` WHERE `type`=%u AND `gpId`=%u AND `cls`=%u ORDER by `pos` ASC", m_type, m_gpId, m_cls);
            if(execu->Prepare(querystr, fplayer) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    Player* player = globalPlayers[fplayer.playerId];
                    SingleHeroPlayer* spl = m_players[player];
                    if(spl == NULL)
                    {
                        m_players.erase(player);
                        continue;
                    }
                    m_finals.push_back(spl);
                }
            }
        }

        if(m_round == 5)
        {
            class SHGiveFinalAward : public SHPlayerVisitor
            {
            public:
                virtual bool operator()(SingleHeroPlayer* sp, UInt8 cls, UInt8 idx)
                {
                    if(!sp)
                        return false;

                    if(sp->_type == e_sh_sl_final)
                        shStageMgr.setTopCandidate(sp->_player, cls, idx+1);

                    return true;
                }
            };

            SHGiveFinalAward awardvisitor;
            visitFinals(awardvisitor);
        }
    }

    bool SHFinalBattleStage::sendEliminary(Stream& st)
    {
        UInt8 round = m_round;
        if(m_progress > e_sh_0)
            round = 5;
        else if(m_progress < e_sh_0)
            return false;

        size_t cnt = m_finals.size();
        st << static_cast<UInt8>(cnt);
        for(UInt32 i = 0; i < cnt; ++ i)
        {
            SingleHeroPlayer* sp = m_finals[i];
            st << static_cast<UInt16>(sp->_fgt->getId());
            st << static_cast<UInt8>(sp->_fgt->getLevel());
            st << sp->_player->getName();
        }

        st << round;
        for(int r = 0; r < round; ++ r)
        {
            UInt8 cnt = 3;
            UInt8 wonFlag = 0;
            st << cnt;

            size_t offset = st.size();
            st << wonFlag;
            for(int j = 0; j < cnt; ++ j)
            {
                UInt8 idx1 = chelun[r][j][0];
                UInt8 idx2 = chelun[r][j][1];
                SingleHeroPlayer* sp1 = NULL;
                SingleHeroPlayer* sp2 = NULL;
                if(idx1 < m_finals.size())
                    sp1 = m_finals[idx1];

                if(idx2 < m_finals.size())
                    sp2 = m_finals[idx2];

                if(sp1 == NULL || sp2 == NULL)
                    st << static_cast<UInt32>(0);
                else
                {
                    std::vector<StageBattleReport>& battles = sp1->_battles;
                    size_t cnt = battles.size();
                    for(size_t i = 0; i < cnt; ++ i)
                    {
                        if(battles[i]._other == sp2->_player)
                        {
                            wonFlag |= (battles[i]._won << j);
                            st << battles[i]._battleId;
                            break;
                        }
                    }
                }
            }
            st.data<UInt8>(offset) = wonFlag;
        }

        return true;
    }


    void SHFinalBattleStage::visitFinals(SHPlayerVisitor& visitor)
    {
        SingleHeroPlayer* pls[3] = {0};
        int end = m_finals.size();
        for(int i = 0; i < end; ++ i)
        {
            if(m_finals[i] == NULL)
                break;
            for(int j = 0; j < 3; ++ j)
            {
                if(pls[j] == NULL)
                {
                    pls[j] = m_finals[i];
                    break;
                }
                else if(pls[j]->_won < m_finals[i]->_won)
                {
                    SingleHeroPlayer* final = m_finals[i];
                    for(int k = j; k < 3; ++ k)
                    {
                        SingleHeroPlayer* tmp = pls[k];
                        pls[k] = final;
                        final = tmp;
                    }
                    break;
                }
            }
        }
        for(int k = 0; k < 3; ++ k)
        {
            visitor(pls[k], m_cls, k);
        }
    }

    void SHFinalBattleStage::sendLeaderBoard(Stream& st)
    {
        class SHLeaderBoardVisitor : public SHPlayerVisitor
        {
        public:
            SHLeaderBoardVisitor(Stream& st) : m_st(st) {}
            virtual bool operator()(SingleHeroPlayer* sp, UInt8 cls, UInt8 idx)
            {
                static UInt32 score1[3] = {20, 10, 5};
                static UInt32 score2[3] = {3, 2, 1};
                if(!sp)
                {
                    m_st << "" << static_cast<UInt16>(0);
                }
                else
                {
                    m_st << sp->_player->getName();
                    if(sp->_type == e_sh_sl_final)
                        m_st << static_cast<UInt16>(score1[idx]);
                    else
                        m_st << static_cast<UInt16>(score2[idx]);
                }

                return true;
            }

            Stream& m_st;
        };

        SHLeaderBoardVisitor slbVisitor(st);
        visitFinals(slbVisitor);
    }


    SHTowerStage::SHTowerStage(StageType type, GroupType gpId, UInt8 cls) : m_gpId(gpId), m_round(0), m_type(type), m_cls(cls)
    {
        memset (m_topTowerFighter, 0, sizeof(m_topTowerFighter));
    }

    SHTowerStage::~SHTowerStage()
    {
    }

    void SHTowerStage::loadFromDB()
    {
        // 从数据库读取爬塔数据
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        char querystr[1024] = {0};

        sprintf(querystr, "SELECT `round`, `progress` FROM `sh_stage_progress` WHERE `type`=%u AND `gpId`=%u AND `cls`=%u", m_type, m_gpId, m_cls);
        DBSHStageProgress stageProgress = {0};
        if(execu->Extract(querystr, stageProgress) == DB::DB_OK)
        {
            m_round = stageProgress.round;
        }

        DBSHTowerBattle shtb;
        memset(querystr, 0, sizeof(querystr));
        sprintf(querystr, "SELECT `player`, `won`, `level`, `turns`, `battleId`, `npcId`  FROM `sh_towerbattle` WHERE `cls`=%u ORDER BY `player` ASC, `level` ASC", 
                m_cls);
        if(execu->Prepare(querystr, shtb) == DB::DB_OK)
        {
            while(execu->Next() == DB::DB_OK)
            {
                TowerBattleReport tbr;
                tbr._won = shtb.won;
                tbr._level = shtb.level;
                tbr._turns = shtb.turns;
                tbr._battleId = shtb.battleId;
                tbr._npcId = shtb.npcId;
                Player *player = GObject::globalPlayers[shtb.playerId];
                if (player == NULL)
                    continue;
                _battleMap[player].push_back(tbr);
            }
        }
    }

    void SHTowerStage::setLevel(UInt16 level)
    {
        m_round = level;
    }

    Player *SHTowerStage::getTopPlayer(UInt8 pos)
    {
        if (pos < 1 || pos > 3)
            return NULL;
        if (m_topTowerFighter[pos - 1])
            return m_topTowerFighter[pos -1]->getPlayer();
        else
            return NULL;
    }

    UInt16 SHTowerStage::getTopPlayerLevel(UInt8 pos)
    {
        if (pos < 1 || pos > 3)
            return 0;
        if (m_topTowerFighter[pos - 1])
            return m_topTowerFighter[pos -1]->getTLevel();
        else
        return 0;
    }

    UInt32 SHTowerStage::getTopPlayerTurns(UInt8 pos)
    {
        if (pos < 1 || pos > 3)
            return 0;
        if (m_topTowerFighter[pos - 1])
            return m_topTowerFighter[pos -1]->getTTurns();
        else
        return 0;
    }



    bool SHTowerStage::process(UInt8 process)
    {
        // 无尽爬塔过程
        bool stillAlive = false;
        for (TowerFighterIt it = m_towerFighters.begin(); it != m_towerFighters.end(); ++ it)
        {
            if ((*it)->getTLevel() < m_round)
                continue; 
            if ((*it)->getTLevel() > m_round)
            {
                stillAlive = true;
                //DEBUG("Better Fighter.");
                continue;
            }
            if (challenge(*it))
                stillAlive = true;
        }
        for (TowerFighterIt it = m_towerFighters.begin(); it != m_towerFighters.end(); ++ it)
        {
            sendTowerInfo((*it)->getPlayer(), (*it)->getTLevel(), (*it)->getTTurns());
        }
        ++ m_round;
        write2DB();
        if (stillAlive)
            return false; // 还有人在坚持爬塔。
        else
            return true; // 全死光了，没人可以爬塔了，结束了。
    }

    void SHTowerStage::write2DB()
    {
        DB1().PushUpdateData("REPLACE INTO `sh_stage_progress` (`gpId`, `type`, `cls`, `round`, `progress`) VALUES(%u, %u, %u, %u, 0)", m_gpId, m_type, m_cls, m_round);
    }

    bool SHTowerStage::enter(Player* player, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return false;
        std::map<Player*, TowerFighterIt>::iterator it = m_towerFightersIt.find(player);
        if (it != m_towerFightersIt.end())
            return true;
        std::pair<TowerFighterIt, bool> res = m_towerFighters.insert(fgt);
        m_towerFightersIt.insert(std::make_pair(player, res.first));
        
        return true;
    }

    void SHTowerStage::pushPlayer(Player* player, SingleHeroFighter* fgt, UInt16 lvl, UInt32 turns, UInt16 lastTurns)
    {
        if(player == NULL)
            return;
        std::map<Player*, TowerFighterIt>::iterator it = m_towerFightersIt.find(player);
        if(it != m_towerFightersIt.end())
            return;

        fgt->setTLevel(lvl);
        fgt->setTTurns(turns);
        fgt->setTLastTurns(lastTurns);
        enter(player, fgt);

        UInt8 pos = 3;
        for (UInt8 i = 0; i < 3; ++i)
        {
            if (m_topTowerFighter[i] == fgt)
            {
                pos = i + 1;
                break;
            }
        }
        for (UInt8 i = 0; i < pos - 1; ++i)
        {
            if (!m_topTowerFighter[i])
            {
                m_topTowerFighter[i] = fgt;
                break;
            }
            if (TowerPlayerBetter(fgt, m_topTowerFighter[i]))
            {
                // 需要更新排名
                for (UInt8 j = pos - 1; j > i; --j)
                {
                    if (m_topTowerFighter[j - 1])
                        m_topTowerFighter[j] = m_topTowerFighter[j - 1];
                }
                m_topTowerFighter[i] = fgt;
                break;
            }
        }
    }

    void SHTowerStage::clear()
    {
        m_towerFighters.clear();
        m_towerFightersIt.clear();
        memset (m_topTowerFighter, 0, sizeof(m_topTowerFighter));
        _battleMap.clear();
        m_round = 0;
        DB1().PushUpdateData("DELETE FROM `sh_towerbattle` WHERE `cls`=%u", m_cls);
        DB1().PushUpdateData("DELETE FROM `sh_stage_progress` WHERE `gpId`=%u AND `type` = %u AND `cls`=%u", m_gpId, m_type, m_cls);
    }

    bool SHTowerStage::challenge(SingleHeroFighter* fighter)
    {
        // 对于该玩家的具体战斗，返回bool为是否胜利

        const UInt32 defaultNpcIdBegin = 12052;
        const UInt32 defaultNpcIdEnd = 12061;

        UInt32 npcId = defaultNpcIdBegin + m_round / 10;
        if (npcId > defaultNpcIdEnd)
            npcId = defaultNpcIdEnd;
        GData::NpcGroups::iterator it = GData::npcGroups.find(12052);
        GData::NpcGroup * ng = it->second;


        UInt32 turns = 0;
        Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, fighter->getPlayer(), ng->getName(), m_round, true);
        Battle::BattleFighter * bf = bsim.newFighter(0, 12, fighter->getFighter());
        bf->setHP(fighter->getCurrentHP());
        ng->putFighters(bsim);
        // 基础值 * (1 + 层数 * 系数 * 随机值)
        //物攻  法攻  物防  法防  生命  身法  命中   闪避   反击   法术抵抗
        static const float  s_rate3NpcBaseModulus[6] = {0.01f, 0.01f, 0.1f, 0.1f, 0.1f, 0.01f};
        static const float  s_rate3NpcAdvanceModulus[4] = {0.01f, 0.01f, 0.01f, 0.01f};

        static const float  s_rate3NpcAdvanceModMax[] =  {200, 100, 100, 200};

        UInt16 formation = ng->getFormation();
        bsim.setFormation(1, formation);
        std::vector<GData::NpcFData>& nl = ng->getList();
        UInt32 npcCount = nl.size();
        Fighter ** fighterList = new Fighter *[npcCount];
        UInt32 i = 0;
        for (std::vector<GData::NpcFData>::iterator it = nl.begin(); it != nl.end(); ++ it)
        {
            fighterList[i] = it->fighter->clone(NULL);

            fighterList[i]->maxhp = fighterList[i]->getBaseHP();
            fighterList[i]->setCurrentHP(fighterList[i]->maxhp, false);
            fighterList[i]->attack  = fighterList[i]->getBaseAttack()    * (1 + s_rate3NpcBaseModulus[0]*m_round*(_rnd(10) + 1));
            fighterList[i]->magatk  = fighterList[i]->getBaseMagAttack() * (1 + s_rate3NpcBaseModulus[1]*m_round*(_rnd(10) + 1));
            fighterList[i]->defend  = fighterList[i]->getBaseDefend()    * (1 + s_rate3NpcBaseModulus[2]*m_round*(_rnd(10) + 1));
            fighterList[i]->magdef  = fighterList[i]->getBaseMagDefend() * (1 + s_rate3NpcBaseModulus[3]*m_round*(_rnd(10) + 1));
            fighterList[i]->maxhp   = fighterList[i]->getBaseHP()        * (1 + s_rate3NpcBaseModulus[4]*m_round*(_rnd(10) + 1));
            fighterList[i]->action  = fighterList[i]->getBaseAction()    * (1 + s_rate3NpcBaseModulus[5]*m_round*(_rnd(10) + 1));
            fighterList[i]->hitrate = fighterList[i]->getBaseHitrate()   * (1 + s_rate3NpcAdvanceModulus[0]*m_round*(_rnd(10) + 1));
            fighterList[i]->evade   = fighterList[i]->getBaseEvade()     * (1 + s_rate3NpcAdvanceModulus[1]*m_round*(_rnd(10) + 1));
            fighterList[i]->counter = fighterList[i]->getBaseCounter()   * (1 + s_rate3NpcAdvanceModulus[2]*m_round*(_rnd(10) + 1));
            fighterList[i]->magres  = fighterList[i]->getBaseMagRes()    * (1 + s_rate3NpcAdvanceModulus[3]*m_round*(_rnd(10) + 1));

            fighterList[i]->hitrate = fighterList[i]->hitrate < s_rate3NpcAdvanceModMax[0] ? fighterList[i]->hitrate : s_rate3NpcAdvanceModMax[0];
            fighterList[i]->evade = fighterList[i]->evade < s_rate3NpcAdvanceModMax[1] ? fighterList[i]->evade : s_rate3NpcAdvanceModMax[1];
            fighterList[i]->counter = fighterList[i]->counter < s_rate3NpcAdvanceModMax[2] ? fighterList[i]->counter : s_rate3NpcAdvanceModMax[2];
            fighterList[i]->magres = fighterList[i]->magres < s_rate3NpcAdvanceModMax[3] ? fighterList[i]->magres : s_rate3NpcAdvanceModMax[3];
            fighterList[i]->setDirty(false);

            if(fighterList[i]->getId() <= GREAT_FIGHTER_MAX)
                bsim.setPortrait(1, fighterList[i]->getId());
            else
                bsim.setPortrait(1, fighterList[i]->favor);
            bsim.newFighter(1, it->pos, fighterList[i]);
            ++ i;
        }

        bsim.start(0xFF, false);

        for (i = 0; i < npcCount; ++ i)
        {
            delete fighterList[i];
        }
        delete [] fighterList;


        bool res = bsim.getWinner() == 1;
        turns = bsim.getTurns();
        if (res)
        {
            // 战斗胜利，增加玩家身上的战斗数据
            fighter->stepTLevel();
            fighter->addTTurns (turns);
            fighter->setTLastTurns (turns);
            fighter->saveTowerProgress();

            UInt8 pos = 4;
            for (UInt8 i = 0; i < 3; ++i)
            {
                if (m_topTowerFighter[i] == fighter)
                {
                    pos = i + 1;
                    break;
                }
            }
            for (UInt8 i = 0; i < pos - 1; ++i)
            {
                if (!m_topTowerFighter[i])
                {
                    m_topTowerFighter[i] = fighter;
                    break;
                }
                if(m_topTowerFighter[i] == fighter)
                    continue;
                if (TowerPlayerBetter(fighter, m_topTowerFighter[i]))
                {
                    // 需要更新排名
                    SingleHeroFighter* fgt = fighter;
                    for (UInt8 j = i; j < pos - 1; ++j)
                    {
                        SingleHeroFighter* tmpfgt = m_topTowerFighter[j];
                        if (m_topTowerFighter[j])
                        {
                            m_topTowerFighter[j] = fgt;
                            if (tmpfgt == fighter)
                                break;
                            fgt = tmpfgt;
                        }
                    }
                    if (pos - 1 < 3)
                        m_topTowerFighter[pos - 1]=fgt;
                    break;
                }
            }

            Battle::BattleObject * obj = bsim(0, 12);
            if(obj == NULL || !obj->isChar())
            {
                res = false;
                return res;
            }
            Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
            UInt32 nHP = bfgt->getHP();
            UInt32 maxHP = fighter->getMaxHP();
            nHP += maxHP / 4;
            fighter->setCurrentHP(nHP > maxHP ? maxHP : nHP);
        }
        else
        {
            // 战斗失败
        }

        Stream& packet = bsim.getPacket();
        if (packet.size() <= 8)
        {
            return false;
        }

        Stream st(REP::ATTACK_NPC);
        st << static_cast<UInt8>(res) << static_cast<UInt8>(1) << static_cast<UInt32> (0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
        st.append(&packet[8], packet.size() - 8);
        st << Stream::eos;
        //fighter->getPlayer()->send(st);

        TowerBattleReport tbr;
        tbr._won = res ? 0 : 1;
        UInt16 tLevel = fighter->getTLevel() + (res ? 0 : 1);
        tbr._level = tLevel;
        tbr._battleId = bsim.getId();
        tbr._turns = turns;
        tbr._npcId = npcId;
        _battleMap[fighter->getPlayer()].push_back(tbr);
        DB1().PushUpdateData("INSERT INTO `sh_towerbattle`(`cls`, `player`, `won`, `level`, `turns`, `battleId`, `npcId` ) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u)", 
                m_cls, fighter->getPlayer()->getId(), tbr._won, tbr._level, tbr._turns, tbr._battleId, tbr._npcId );
        return res;
    }

    void SHTowerStage::sendTowerInfo(Player * player, UInt16 tLevel, UInt32 tTurns)
    {
        // 发送玩家爬塔信息

        Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01);
        st << static_cast<UInt8>(6);

        st << static_cast<UInt8>(m_cls);
        st << static_cast<UInt16>(tLevel);
        st << static_cast<UInt32>(tTurns * 2);


        for (UInt8 i = 0; i < 3; ++ i)
        {
            if (m_topTowerFighter[i])
            {
                st << m_topTowerFighter[i]->getPlayer()->getName();
                st << static_cast<UInt16> (m_topTowerFighter[i]->getTLevel());
                st << static_cast<UInt32> (m_topTowerFighter[i]->getTTurns() * 2);
            }
            else
            {
                std::string str("");
                st << str;
                st << static_cast<UInt16>(0);
                st << static_cast<UInt32>(0);
            }
        }
        if (_battleMap.find(player) != _battleMap.end())
        {
            UInt16 battleCount = _battleMap[player].size();
            st << static_cast<UInt16>(battleCount);
            for (TowerBattles::iterator it = _battleMap[player].begin(); it != _battleMap[player].end(); ++ it)
            {
                st << static_cast<UInt16>(it->_level);    // 层数
                st << static_cast<UInt32>(it->_turns * 2);// 耗时
                st << static_cast<UInt8> (it->_won);      // 战斗结果
                st << static_cast<UInt32>(it->_battleId); // 战报Id
                st << static_cast<UInt32>(it->_npcId);    // 怪物Id

            }
        }
        else
        {
            st << static_cast<UInt16>(0);
        }
        st << Stream::eos;
        player->send(st);
    }


    SHStarStage::SHStarStage(StageType type, GroupType gpId) : m_currIdx(0), m_swapIdx(1), m_gpId(gpId), m_type(type), m_round(0), m_progress(e_sh_pre)
    {
        memset(m_finals, 0, sizeof(m_finals));
        memset(m_3finals, 0, sizeof(m_3finals));
        memset(m_finalBattles, 0, sizeof(m_finalBattles));
        memset(m_3finalBattles, 0, sizeof(m_3finalBattles));
    }

    SHStarStage::~SHStarStage()
    {
    }

    bool SHStarStage::enter(Player* player, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return false;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return true;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->setEnter(player, fgt, m_type);
        m_players[player] = sp;

        return true;
    }

    bool SHStarStage::enter(StarTeam* starTeam)
    {
        std::vector<SingleHeroPlayer*>& plList = starTeam->_playerList;
        StarTeam* starTeam2 = new StarTeam();
        starTeam2->_teamId = starTeam->_teamId;
        m_starTeam[starTeam->_teamId] = starTeam2;
        m_preliminary[m_currIdx][0].push_back(starTeam2);
        for(std::vector<SingleHeroPlayer*>::iterator it = plList.begin(); it != plList.end(); ++ it)
        {
            SingleHeroPlayer* sp = *it;
            SingleHeroPlayer* spNew = new SingleHeroPlayer();
            m_players[sp->_player] = spNew;
            spNew->_teamId = sp->_teamId;
            starTeam2->_playerList.push_back(spNew);
            spNew->setEnter((*it)->_player, (*it)->_fgt, m_type);
        }

        return true;
    }

    void SHStarStage::pushPlayer(Player* player, UInt32 teamId, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->_teamId = teamId;
        sp->setEnter(player, fgt, m_type, false);
        m_players[player] = sp;
        if(teamId > 0 && teamId != (UInt32)(-1))
        {
            StarTeam* starTeam2 = m_starTeam[teamId];
            if(starTeam2 == NULL)
            {
                starTeam2 = new StarTeam();
                m_starTeam[teamId] = starTeam2;
            }
            starTeam2->_teamId = teamId;
            starTeam2->_playerList.push_back(sp);
        }
    }

    void SHStarStage::createTeams()
    {
        URandom& rnd = URandom::current();
        std::map<Player*, SingleHeroPlayer*> tmpMap = m_players;

        UInt32 teamId = 0;
        UInt32 mapSize = tmpMap.size();
        while(mapSize != 0)
        {
            ++ teamId;

            std::map<Player*, SingleHeroPlayer*>::iterator it = tmpMap.begin();
            std::advance(it, rnd(mapSize));

            StarTeam* starTeam = new StarTeam();
            m_starTeam[teamId] = starTeam;

            starTeam->_playerList.push_back(it->second);
            starTeam->_teamId = teamId;

            it->second->setTeam(teamId);
            tmpMap.erase(it);
            -- mapSize;
            m_preliminary[m_currIdx][0].push_back(starTeam);
            for(int i = 0; i < 2 && mapSize != 0; ++ i)
            {
                it = tmpMap.begin();
                std::advance(it, rnd(mapSize));
                starTeam->_playerList.push_back(it->second);

                it->second->setTeam(teamId);
                tmpMap.erase(it);
                -- mapSize;
            }
        }
    }

    bool SHStarStage::processPreliminary()
    {
        if(m_progress > e_sh_pre)
            return true;
        UInt32 pCnt = getTeamCount();
        if(pCnt == 0)
        {
            ++ m_progress;
            return true;
        }
        else if(pCnt <= STAGE_FINAL_PLAYER_CNT)
        {
            std::swap(m_currIdx, m_swapIdx);
            initQualifyStage();
            ++ m_progress;
            DB1().PushUpdateData("UPDATE `sh_stage_progress` SET `round`=%u, `progress`=%u WHERE `gpId`=%u AND `type`=%u AND `cls`=0", m_round, m_progress, m_gpId, m_type);
            return true;
        }

        const size_t minLeft[5] = {2, 2, 4, 8, 16};
        UInt32 totalLeft = STAGE_FINAL_PLAYER_CNT;
        ++ m_round;
        UInt32 totalSize = 0;

        for(int i = 0; i < 5; ++ i)
            totalSize += m_preliminary[m_currIdx][i].size();

        URandom& rnd = URandom::current();
        bool ret = true;
        for(int i = 0; i < 5; ++ i)
        {
            std::vector<StarTeam*>& spl = m_preliminary[m_currIdx][i];
            UInt32 size = static_cast<UInt32>(spl.size());
            UInt32 size2 = size;
            UInt8 mod = 0;
            if(size%2)
            {
                UInt32 pos = rnd(size);
                std::vector<StarTeam*>::iterator it = spl.begin();
                std::advance(it, pos);
                m_preliminary[m_swapIdx][i].push_back(*it);
                spl.erase(it);
                ++ mod;
            }

            int round = size/2;
            bool flag = false;
            size += m_preliminary[m_swapIdx][i].size() - mod;
            for(int j = 0; j < round; ++ j)
            {
                if(size <= minLeft[i] || totalSize <= totalLeft)
                {
                    flag = true;
                    for(std::vector<StarTeam*>::iterator it = spl.begin(); it != spl.end(); ++ it)
                    {
                        m_preliminary[m_swapIdx][i].push_back(*it);
                    }
                    spl.clear();
                    break;;
                }

                UInt32 pos1 = rnd(size2 - mod);
                std::vector<StarTeam*>::iterator it1 = spl.begin();
                std::advance(it1, pos1);
                StarTeam* pl1 = *it1;
                spl.erase(it1);
                -- size2;

                UInt32 pos2 = rnd(size2 - mod);
                std::vector<StarTeam*>::iterator it2 = spl.begin();
                std::advance(it2, pos2);
                StarTeam* pl2 = *it2;
                spl.erase(it2);
                -- size2;

                UInt32 rid = 0;
                bool result = pl1->challenge(pl2, &rid);
                TeamStageBattleReport sbr;
                TeamStageBattleReport sbr2;
                sbr._type = m_type;
                sbr._won = !result;
                sbr._other = pl2;
                sbr._battleTime = TimeUtil::Now();
                sbr._battleId = rid;
                pl1->_battles.push_back(sbr);
                sbr2._type = m_type;
                sbr2._won = result;
                sbr2._other = pl1;
                sbr2._battleTime = TimeUtil::Now();
                sbr2._battleId = rid;
                pl2->_battles.push_back(sbr2);

                Stream st1(REP::SINGLE_HERO);
                Stream st2(REP::SINGLE_HERO);
                st1 << static_cast<UInt8>(0x06) << static_cast<UInt8>(m_gpId) << static_cast<UInt8>(0x01);
                st2 << static_cast<UInt8>(0x06) << static_cast<UInt8>(m_gpId) << static_cast<UInt8>(0x01);
                st1 << static_cast<UInt8>((m_type & e_sh_sr) ? 0 : 1);
                st2 << static_cast<UInt8>((m_type & e_sh_sr) ? 0 : 1);
                if(result)
                {
                    st1 << static_cast<UInt8>(0x00);
                    st2 << static_cast<UInt8>(0x01);
                    ++ pl1->_won;
                    m_preliminary[m_swapIdx][i].push_back(pl1);
                    if(i < 4)
                    {
                        m_preliminary[m_swapIdx][i + 1].push_back(pl2);
                    }
                    else
                    {
                        -- totalSize;
                        shStageMgr.starStage2()->enter(pl2);
                        pl2->_battles.back()._won = 2;
                        sendStageMail(pl2, false);
                    }
                }
                else
                {
                    st1 << static_cast<UInt8>(0x01);
                    st2 << static_cast<UInt8>(0x00);
                    ++ pl2->_won;
                    m_preliminary[m_swapIdx][i].push_back(pl2);
                    if(i < 4)
                    {
                        m_preliminary[m_swapIdx][i + 1].push_back(pl1);
                    }
                    else
                    {
                        -- totalSize;
                        shStageMgr.starStage2()->enter(pl1);
                        pl1->_battles.back()._won = 2;
                        sendStageMail(pl1, false);
                    }
                }
                -- size;

                st1 << static_cast<UInt16>(pl2->_teamId) << static_cast<UInt32>(rid) << "" << Stream::eos;
                st2 << static_cast<UInt16>(pl1->_teamId) << static_cast<UInt32>(rid) << "" << Stream::eos;

                {
                    std::vector<SingleHeroPlayer*>& playerList = pl1->_playerList;
                    size_t cnt = playerList.size();
                    for(size_t i = 0; i < cnt; ++ i)
                    {
                        playerList[i]->_player->send(st1);
                    }
                }
                {
                    std::vector<SingleHeroPlayer*>& playerList = pl2->_playerList;
                    size_t cnt = playerList.size();
                    for(size_t i = 0; i < cnt; ++ i)
                    {
                        playerList[i]->_player->send(st2);
                    }
                }

                DB1().PushUpdateData("REPLACE INTO `sh_preliminary`(`gpId`, `cls`, `type`, `won`, `player1`, `player2`, `battleTime`, `battleId`) VALUES(%u, 0, %u, %u, %"I64_FMT"u, %"I64_FMT"u, %u, %u)", m_gpId, m_type, result ? 1 : 0, pl1->_teamId, pl2->_teamId, sbr._battleTime, sbr._battleId);
            }
            if(!flag)
            {
                if(size <= minLeft[i] || totalSize < totalLeft)
                {
                    for(std::vector<StarTeam*>::iterator it = spl.begin(); it != spl.end(); ++ it)
                    {
                        m_preliminary[m_swapIdx][i].push_back(*it);
                    }
                    spl.clear();
                }
                else
                    ret = false;
            }
        }

        if(ret)
        {
            ++ m_progress;
            initQualifyStage();
        }

        for(int k = 0; k < 5; ++ k)
            m_preliminary[m_currIdx][k].clear();

        write2DB();
        std::swap(m_currIdx, m_swapIdx);
        return ret;

    }

    bool SHStarStage::processFinals()
    {
        if(m_progress < e_sh_16)
            return false;
        if(m_progress > e_sh_2)
            return true;

        int idx = m_progress - 2;
        for(int j = 0; j < 2; ++ j)
        {
            for(int i = 0; i < stageQualifyBattles[idx]; ++ i)
            {
                StarTeam* bspl1 = m_finals[j][idx][i * 2];
                StarTeam* bspl2 = m_finals[j][idx][i * 2 + 1];
                if(bspl1 == NULL && bspl2 == NULL)
                    continue;

                int curridx = stateQualifyIndex[idx] + i;
                QualifyBattle& elimBattle = m_finalBattles[j][curridx];
                elimBattle._id = curridx;
                UInt32 battleId;
                UInt8 r = 0xFF;
                if(bspl1 == NULL)
                    r = elimBattle.push(1, 0);
                else if(bspl2 == NULL)
                    r = elimBattle.push(0, 0);
                else
                {
                    if(bspl1->challenge(bspl2, &battleId))
                        r = elimBattle.push(0, battleId);
                    else
                        r = elimBattle.push(1, battleId);
                }
                if(r == 0)
                {
                    m_finals[j][idx + 1][i] = bspl1;
                    if(idx == 3)
                    {
                        m_3finals[0][j] = bspl1;
                        m_3finals[0][j+2] = bspl2;

                        if(bspl1)
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%u", j+1, m_gpId, m_type, bspl1->_teamId);
                        if(bspl2)
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%u", j+3, m_gpId, m_type, bspl2->_teamId);
                    }
                }
                else if(r == 1)
                {
                    m_finals[j][idx + 1][i] = bspl2;
                    if(idx == 3)
                    {
                        if(bspl2 != NULL)
                        {
                            m_3finals[0][j] = bspl2;
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%u", j+1, m_gpId, m_type, bspl2->_teamId);
                        }
                        if(bspl1 != NULL)
                        {
                            m_3finals[0][j+2] = bspl1;
                            DB1().PushUpdateData("UPDATE `sh_final_player` SET `final3pos`=%u WHERE `gpId`=%u AND `type`=%u AND `playerId`=%u", j+3, m_gpId, m_type, bspl1->_teamId);
                        }
                    }
                }

                std::string won = "";
                std::string battleIds = "";
                int cnt = elimBattle._won.size();

                char tmp[256] = {0};
                sprintf(tmp, "%u", elimBattle._won[0]);
                won += tmp;
                sprintf(tmp, "%u", elimBattle._battleId[0]);
                battleIds += tmp;

                for(int k = 1; k < cnt; ++ k)
                {
                    char tmp[256] = {0};
                    sprintf(tmp, ", %u", elimBattle._won[k]);
                    won += tmp;
                    sprintf(tmp, ", %u", elimBattle._battleId[k]);
                    battleIds += tmp;
                }

                DB1().PushUpdateData("REPLACE INTO `sh_elimination`(`gpId`, `type`, `cls`, `stage_idx`, `id`, `won`, `battleId`) VALUES(%u, %u, 0, %u, %u, '%s', '%s')", m_gpId, m_type, j, curridx, won.c_str(), battleIds.c_str());
            }
        }

        ++ m_round;

        if(m_round == 3)
        {
            m_round = 0;
            ++ m_progress;
        }

        write2DB();

        return false;
    }

    bool SHStarStage::process3Finals()
    {
        if(m_progress > e_sh_1)
            return true;

        bool ret = false;

        for(int i = 0; i < 2; ++ i)
        {
            StarTeam* bspl1 = m_3finals[0][2*i];
            StarTeam* bspl2 = m_3finals[0][2*i+1];
            if(bspl1 == NULL && bspl2 == NULL)
                continue;

            QualifyBattle& elimBattle = m_3finalBattles[i];
            elimBattle._id = i;
            UInt32 battleId;
            UInt8 r = 0xFF;
            if(bspl1 == NULL)
                r = elimBattle.push(1, 0);
            else if(bspl2 == NULL)
                r = elimBattle.push(0, 0);
            else
            {
                if(bspl1->challenge(bspl2, &battleId))
                    r = elimBattle.push(0, battleId);
                else
                    r = elimBattle.push(1, battleId);
            }
            if(r == 0)
            {
                m_3finals[1][2*i] = bspl1;
                m_3finals[1][2*i+1] = bspl2;
            }
            else if(r == 1)
            {
                m_3finals[1][2*i] = bspl2;
                m_3finals[1][2*i+1] = bspl1;
            }

            std::string won = "";
            std::string battleIds = "";
            int cnt = elimBattle._won.size();

            char tmp[256] = {0};
            sprintf(tmp, "%u", elimBattle._won[0]);
            won += tmp;
            sprintf(tmp, "%u", elimBattle._battleId[0]);
            battleIds += tmp;

            for(int j = 1; j < cnt; ++ j)
            {
                char tmp[256] = {0};
                sprintf(tmp, ", %u", elimBattle._won[j]);
                won += tmp;
                sprintf(tmp, ", %u", elimBattle._battleId[j]);
                battleIds += tmp;
            }
            DB1().PushUpdateData("REPLACE INTO `sh_elimination`(`gpId`, `type`, `cls`, `stage_idx`, `id`, `won`, `battleId`) VALUES(%u, %u, 0, 2, %u, '%s', '%s')", m_gpId, m_type, i, won.c_str(), battleIds.c_str());
        }

        ++ m_round;

        if(m_round == 3)
        {
            m_round = 0;
            ret = true;
            ++ m_progress;
            for(int i = 0; i < 3; ++ i)
            {
                if(!m_3finals[1][i])
                    continue;
                shStageMgr.starFinalStage()->enter(m_3finals[1][i]);
            }
        }

        write2DB();

        return ret;
    }

    void SHStarStage::initQualifyStage()
    {
        bool odd = false;
        int j = 0;
        URandom& rnd = URandom::current();
        for(int k = 0; k < 2; ++ k)
            for(int i = 0; i < 15; ++ i)
                m_finalBattles[k][i].reset();
        for(int i = 0; i < 5; ++ i)
        {
            std::vector<StarTeam*>& spl = m_preliminary[m_swapIdx][i];
            for(std::vector<StarTeam*>::iterator it = spl.begin(); it != spl.end(); ++ it)
            {
                int base;
                if(odd)
                    base = 8;
                else
                    base = 0;
                int n = rnd(8);
                if(m_finals[j][0][base + n] != NULL)
                {
                    int endn = n;
                    do {
                        n = (n + 1) % 8;
                    } while (n != endn && m_finals[j][0][base + n] != NULL);
                    if(n == endn)
                    {
                        continue;
                    }
                }
                (*it)->_won = 0;
                m_finals[j][0][base + n] = *it;
                sendStageMail(*it, true);

                DB1().PushUpdateData("REPLACE INTO `sh_final_player`(`gpId`, `type`, `cls`, `stage_idx`, `pos`, `playerId`, `final3pos`) VALUES(%u, %u, 0, %u, %u, %u, 0)", m_gpId, m_type, j, base + n, (*it)->_teamId);
                if(j != 0)
                {
                    odd = !odd;
                    j = 0;
                }
                else
                    j = 1;
            }
        }
        m_round = 0;
        write2DB();
    }

    void SHStarStage::clear()
    {
        m_currIdx = 0;
        m_swapIdx = 1;
        m_round = 0;
        m_progress = e_sh_pre;
        for(int i = 0; i < 5; ++ i)
        {
            m_preliminary[0][i].clear();
            m_preliminary[1][i].clear();
        }
        for(int j = 0; j < 2; ++ j)
            for(int i = 0; i < 15; ++ i)
                m_finalBattles[j][i].reset();
        memset(m_finals, 0, sizeof(m_finals));

        m_3finalBattles[0].reset();
        m_3finalBattles[1].reset();
        memset(m_3finals, 0, sizeof(m_3finals));

        for(std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.begin(); it != m_players.end(); ++ it)
        {
            delete it->second;
        }
        m_players.clear();
        for(std::map<UInt32, StarTeam*>::iterator it = m_starTeam.begin(); it != m_starTeam.end(); ++ it)
        {
            delete it->second;
        }
        m_starTeam.clear();

        DB1().PushUpdateData("DELETE FROM `sh_preliminary` WHERE `gpId` = %u AND `type`=%u AND `cls`=0", m_gpId, m_type);
        DB1().PushUpdateData("DELETE FROM `sh_final_player` WHERE `gpId` = %u AND `type`=%u AND `cls`=0", m_gpId, m_type);
        DB1().PushUpdateData("DELETE FROM `sh_elimination` WHERE `gpId` = %u AND `type`=%u AND `cls`=0", m_gpId, m_type);
        DB1().PushUpdateData("DELETE FROM `sh_stage_progress` WHERE `gpId`=%u AND `type` = %u AND `cls`=0", m_gpId, m_type);
    }

    void SHStarStage::write2DB()
    {
        DB1().PushUpdateData("REPLACE INTO `sh_stage_progress` (`gpId`, `type`, `cls`, `round`, `progress`) VALUES(%u, %u, 0, %u, %u)", m_gpId, m_type, m_round, m_progress);
    }

    void SHStarStage::sendStageMail(StarTeam* spl, bool win)
    {
        UInt8 type = 2;
        if(m_type == e_sh_sr)
            type = 1;
        GET_STAGE_NAME(sn, m_gpId)
        GET_STAGE_TYPE(st, m_gpId, type)

        SYSMSGV(title, 880, sn);

        std::string content;
        if(win)
        {
            SYSMSGV(content1, 886, sn, st, sn);
            content = content1;
        }
        else
        {
            SYSMSGV(content1, 887, sn);
            content = content1;
        }
        std::string vs = "\n";
        std::vector<TeamStageBattleReport>& battles = spl->_battles;
        size_t cnt = battles.size();
        size_t i = 0;
        for(i = 0; i < cnt; ++ i)
        {
            GET_STAGE_WIN(sw, battles[i]._won)
            SYSMSGV(content1, 893, spl->_teamId, battles[i]._other->_teamId, sw);
            vs += content1;
            vs += "\n";
        }
        {
            SYSMSGV(content1, 888, vs.c_str());
            content += content1;
        }

        std::vector<SingleHeroPlayer*> playerList = spl->_playerList;
        cnt = playerList.size();
        for(i = 0; i < cnt; ++ i)
        {
            SingleHeroPlayer* pl = playerList[i];
            pl->_player->GetMailBox()->newMail(NULL, 0x01, title, content.c_str());
        }
    }

    void SHStarStage::loadFromDB()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        char querystr[1024] = {0};

        sprintf(querystr, "SELECT `round`, `progress` FROM `sh_stage_progress` WHERE `type`=%u AND `gpId`=%u AND `cls`=0", m_type, m_gpId);
        DBSHStageProgress stageProgress = {0};
        if(execu->Extract(querystr, stageProgress) == DB::DB_OK)
        {
            m_round = stageProgress.round;
            m_progress = stageProgress.progress;
        }

        {
            DBSHFinalPlayer fplayer;
            bool flag = false;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `pos`, `final3pos`, `stage_idx`, `playerId` FROM `sh_final_player` WHERE `type`=%u AND `gpId`=%u AND `cls`=0", m_type, m_gpId);
            if(execu->Prepare(querystr, fplayer) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    if(fplayer.pos > 15)
                        continue;
                    if(fplayer.playerId == 0)
                        continue;
                    std::map<UInt32, StarTeam*>::iterator it = m_starTeam.find(fplayer.playerId);
                    if(it == m_starTeam.end())
                        continue;
                    m_finals[fplayer.stageIdx][0][fplayer.pos] = it->second;
                    if(!flag)
                        flag = true;
                    if(fplayer.final3pos > 0)
                        m_3finals[0][fplayer.final3pos-1] = it->second;
                }
            }

            if(flag)
            {
                DBSHQualifyBattle dbqb;
                memset(querystr, 0, sizeof(querystr));
                sprintf(querystr, "SELECT `stage_idx`, `id`, `won`, `battleId` FROM `sh_elimination` WHERE `gpId`=%u AND `type`=%u AND `cls`=0", m_gpId, m_type);
                if(execu->Prepare(querystr, dbqb) == DB::DB_OK)
                {
                    while(execu->Next() == DB::DB_OK)
                    {
                        if(dbqb.id > 14)
                            continue;
                        if(dbqb.stageIdx > 1)
                        {
                            QualifyBattle& qb = m_3finalBattles[dbqb.id];
                            qb._id = dbqb.id;
                            {
                                StringTokenizer tokenizer(dbqb.won, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._won.push_back(atoi(tokenizer[j].c_str()));
                            }
                            {
                                StringTokenizer tokenizer(dbqb.battleId, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._battleId.push_back(atoi(tokenizer[j].c_str()));
                            }
                            UInt8 winner = qb.winner();
                            if(winner == 0xFF)
                                continue;

                            if(winner == 0)
                            {
                                m_3finals[1][qb._id*2] = m_3finals[0][qb._id*2];
                                m_3finals[1][qb._id*2+1] = m_3finals[0][qb._id*2+1];
                            }
                            else
                            {
                                m_3finals[1][qb._id*2] = m_3finals[0][qb._id*2+1];
                                m_3finals[1][qb._id*2+1] = m_3finals[0][qb._id*2];
                            }
                        }
                        else
                        {
                            QualifyBattle& qb = m_finalBattles[dbqb.stageIdx][dbqb.id];
                            qb._id = dbqb.id;
                            {
                                StringTokenizer tokenizer(dbqb.won, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._won.push_back(atoi(tokenizer[j].c_str()));
                            }
                            {
                                StringTokenizer tokenizer(dbqb.battleId, ",");
                                for(size_t j = 0; j < tokenizer.count(); ++ j)
                                    qb._battleId.push_back(atoi(tokenizer[j].c_str()));
                            }
                            UInt8 winner = qb.winner();
                            if(winner == 0xFF)
                                continue;

                            {
                                int j = 1;
                                const int aIndex[5] = {0, 8, 12, 14, 15};
                                while(dbqb.id >= aIndex[j]) ++ j;
                                int idx = static_cast<int>(dbqb.id) - aIndex[j - 1];
                                if(winner == 0)
                                    m_finals[dbqb.stageIdx][j][idx] = m_finals[dbqb.stageIdx][j - 1][idx * 2];
                                else
                                    m_finals[dbqb.stageIdx][j][idx] = m_finals[dbqb.stageIdx][j - 1][idx * 2 + 1];
                            }
                        }

                    }
                }
            }
        }
        {
            DBSHPriliminary pril;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `won`, `player1`, `player2`, `battleTime`, `battleId` FROM `sh_preliminary` WHERE `type`=%u AND `gpId` = %u AND `cls`=0 ORDER BY `battleTime`", m_type, m_gpId);
            if(execu->Prepare(querystr, pril) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    StarTeam* spl1 = m_starTeam[pril.player1];
                    if(spl1 == NULL)
                    {
                        m_starTeam.erase(pril.player1);
                        continue;
                    }
                    StarTeam* spl2 = m_starTeam[pril.player2];
                    if(spl2 == NULL)
                    {
                        m_starTeam.erase(pril.player2);
                        continue;
                    }
                    if(pril.won == 1)
                    {
                        ++ spl1->_won;
                    }
                    else
                    {
                        ++ spl2->_won;
                    }
                    TeamStageBattleReport sbr;
                    TeamStageBattleReport sbr2;
                    sbr._type = m_type;
                    sbr._won = !pril.won;
                    sbr._other = spl2;
                    sbr._battleTime = pril.battleTime;
                    sbr._battleId = pril.battleId;
                    spl1->_battles.push_back(sbr);

                    sbr2._type = m_type;
                    sbr2._won = pril.won;
                    sbr2._other = spl1;
                    sbr2._battleTime = pril.battleTime;
                    sbr2._battleId = pril.battleId;
                    spl2->_battles.push_back(sbr2);
                }
            }
            UInt16 prePlayerNum = 0;
            for(std::map<UInt32, StarTeam*>::iterator iter = m_starTeam.begin(); iter != m_starTeam.end(); ++ iter)
            {
                if(iter->second == NULL)
                    continue;
                UInt16 total = static_cast<UInt16>(iter->second->_battles.size());
                UInt16 lost = total - iter->second->_won;
                if(lost < 5)
                {
                    ++ prePlayerNum;
                    m_preliminary[m_currIdx][lost].push_back(iter->second);
                }
                else
                {
                    if(total > 0)
                        iter->second->_battles.back()._won = 2;
                }
            }
            fprintf(stdout, "gpId:[%d] type[%d] ----preliminary player count %d\n", m_gpId, m_type, prePlayerNum);
            fflush(stdout);
        }
    }

    bool SHStarStage::sendEliminary(Stream& st, UInt8 type)
    {
        static UInt8 aIndex[5] = {0, 8, 12, 14, 15};
        UInt8 round = 0;

        if(m_progress < e_sh_16)
            return false;

        if(type < 2)
        {
            for(int i = 0; i < 16; ++ i)
            {
                if(m_finals[type][0][i] != NULL)
                {
                    st << static_cast<UInt16>(m_finals[type][0][i]->_teamId);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
                else
                {
                    st << static_cast<UInt16>(0);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
            }

            round = m_progress - 2;
            if(m_round > 0)
                ++ round;
            if(round > 4)
                round = 4;
            st << round;
            for(int r = 0; r < aIndex[round]; ++ r)
            {
                UInt8 cnt = m_finalBattles[type][r]._battleId.size();
                UInt8 wonFlag = 0;
                for(int i = 0; i < cnt; ++ i)
                    wonFlag |= (m_finalBattles[type][r]._won[i] << i);
                st << cnt << wonFlag;
                for(int j = 0; j < cnt; ++ j)
                    st << m_finalBattles[type][r]._battleId[j];
            }
        }
        else
        {
            for(int i = 0; i < 4; ++ i)
            {
                if(m_3finals[0][i] != NULL)
                {
                    st << static_cast<UInt16>(m_3finals[0][i]->_teamId);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
                else
                {
                    st << static_cast<UInt16>(0);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
            }
            if(m_progress > 6)
                round = 5;
            st << round;
            for(int r = 0; r < 2; ++ r)
            {
                UInt8 cnt = m_3finalBattles[r]._battleId.size();
                UInt8 wonFlag = 0;
                for(int i = 0; i < cnt; ++ i)
                    wonFlag |= (m_3finalBattles[r]._won[i] << i);
                st << cnt << wonFlag;
                for(int j = 0; j < cnt; ++ j)
                    st << m_3finalBattles[r]._battleId[j];
            }
        }

        return true;
    }

    bool SHStarStage::isInFinal(Player* pl)
    {
        bool find = false;
        for(int i = 0; i < 2; ++ i)
        {
            for(int j = 0; j < 16; ++ j)
            {
                if(!m_finals[i][0][j])
                    continue;

                std::vector<SingleHeroPlayer*>& playerList = m_finals[i][0][j]->_playerList;
                size_t cnt = playerList.size();
                for(size_t k = 0; k < cnt; ++ k)
                {
                    if(playerList[k]->_player == pl)
                    {
                        find = true;
                        break;
                    }
                }
                if(find)
                    break;
            }
            if(find)
                break;
        }

        return find;
    }


    SHFinalStarStage::SHFinalStarStage(StageType type, GroupType gpId) : m_gpId(gpId), m_round(0), m_type(type), m_progress(e_sh_0)
    {
    }

    SHFinalStarStage::~SHFinalStarStage()
    {
    }

    void SHFinalStarStage::clear()
    {
        m_round = 0;
        m_progress = e_sh_0;

        for(std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.begin(); it != m_players.end(); ++ it)
        {
            delete it->second;
        }
        m_players.clear();

        for(std::map<UInt32, StarTeam*>::iterator it = m_starTeam.begin(); it != m_starTeam.end(); ++ it)
        {
            delete it->second;
        }
        m_starTeam.clear();
        m_finals.clear();
        m_finals.resize(0);
        DB1().PushUpdateData("DELETE FROM `sh_preliminary` WHERE `gpId` = %u AND `type`=%u AND `cls`=0", m_gpId, m_type);
        DB1().PushUpdateData("DELETE FROM `sh_final_player` WHERE `gpId` = %u AND `type`=%u AND `cls`=0", m_gpId, m_type);
        DB1().PushUpdateData("DELETE FROM `sh_stage_progress` WHERE `gpId`=%u AND `type` = %u AND `cls`=0", m_gpId, m_type);
    }

    bool SHFinalStarStage::enter(StarTeam* starTeam)
    {
        std::vector<SingleHeroPlayer*>& plList = starTeam->_playerList;
        StarTeam* starTeam2 = new StarTeam();
        starTeam2->_teamId = starTeam->_teamId;
        m_starTeam[starTeam->_teamId] = starTeam2;
        for(std::vector<SingleHeroPlayer*>::iterator it = plList.begin(); it != plList.end(); ++ it)
        {
            SingleHeroPlayer* sp = *it;
            SingleHeroPlayer* spNew = new SingleHeroPlayer();
            m_players[sp->_player] = spNew;
            spNew->_teamId = sp->_teamId;
            starTeam2->_playerList.push_back(spNew);
            spNew->setEnter((*it)->_player, (*it)->_fgt, m_type);
        }

        m_finals.push_back(starTeam2);
        int pos = m_finals.size() - 1;
        DB1().PushUpdateData("REPLACE INTO `sh_final_player`(`gpId`, `type`, `cls`, `stage_idx`, `pos`, `playerId`, `final3pos`) VALUES(%u, %u, 0, 2, %u, %u, 0)", m_gpId, m_type, pos, starTeam2->_teamId);
        return true;
    }

    void SHFinalStarStage::pushPlayer(Player* player, UInt32 teamId, SingleHeroFighter* fgt)
    {
        if(player == NULL)
            return;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            return;

        SingleHeroPlayer* sp = new SingleHeroPlayer();
        sp->setEnter(player, fgt, m_type, false);
        m_players[player] = sp;
        if(teamId > 0 && teamId != (UInt32)(-1))
        {
            StarTeam* starTeam2 = m_starTeam[teamId];
            if(starTeam2 == NULL)
            {
                starTeam2 = new StarTeam();
                m_starTeam[teamId] = starTeam2;
            }
            starTeam2->_teamId = teamId;
            starTeam2->_playerList.push_back(sp);
        }
    }

    void SHFinalStarStage::write2DB()
    {
        DB1().PushUpdateData("REPLACE INTO `sh_stage_progress` (`gpId`, `type`, `cls`, `round`, `progress`) VALUES(%u, %u, 0, %u, %u)", m_gpId, m_type, m_round, m_progress);
    }

    void SHFinalStarStage::visitFinals(SHTeamVisitor& visitor)
    {
        StarTeam* pls[3] = {0};
        int end = m_finals.size();
        for(int i = 0; i < end; ++ i)
        {
            if(m_finals[i] == NULL)
                break;
            for(int j = 0; j < 3; ++ j)
            {
                if(pls[j] == NULL)
                {
                    pls[j] = m_finals[i];
                    break;
                }
                else if(pls[j]->_won < m_finals[i]->_won)
                {
                    StarTeam* final = m_finals[i];
                    for(int k = j; k < 3; ++ k)
                    {
                        StarTeam* tmp = pls[k];
                        pls[k] = final;
                        final = tmp;
                    }
                    break;
                }
            }
        }
        for(int k = 0; k < 3; ++ k)
        {
            visitor(pls[k], k);
        }
    }

    bool SHFinalStarStage::sendEliminary(Stream& st)
    {
        UInt8 round = m_round;
        if(m_progress > e_sh_0)
            round = 5;
        else if(m_progress < e_sh_0)
            return false;

        size_t cnt = m_finals.size();
        st << static_cast<UInt8>(cnt);
        for(UInt32 i = 0; i < cnt; ++ i)
        {
            StarTeam* sp = m_finals[i];
            st << static_cast<UInt16>(sp->_teamId);
            st << static_cast<UInt8>(0);
            st << "";
        }

        st << round;
        for(int r = 0; r < round; ++ r)
        {
            UInt8 cnt = 3;
            UInt8 wonFlag = 0;
            st << cnt;
            size_t offset = st.size();
            st << wonFlag;
            for(int j = 0; j < cnt; ++ j)
            {
                UInt8 idx1 = chelun[r][j][0];
                UInt8 idx2 = chelun[r][j][1];
                StarTeam* sp1 = NULL;
                StarTeam* sp2 = NULL;
                if(idx1 < m_finals.size())
                    sp1 = m_finals[idx1];

                if(idx2 < m_finals.size())
                    sp2 = m_finals[idx2];

                if(sp1 == NULL || sp2 == NULL)
                    st << static_cast<UInt32>(0);
                else
                {
                    std::vector<TeamStageBattleReport>& battles = sp1->_battles;
                    size_t cnt = battles.size();
                    for(size_t i = 0; i < cnt; ++ i)
                    {
                        if(battles[i]._other == sp2)
                        {
                            wonFlag |= (battles[i]._won << j);
                            st << battles[i]._battleId;
                            break;
                        }
                    }
                }
            }
            st.data<UInt8>(offset) = wonFlag;
        }

        return true;
    }

    void SHFinalStarStage::sendLeaderBoard(Stream& st)
    {
        class SHLeaderBoardVisitor : public SHTeamVisitor
        {
        public:
            SHLeaderBoardVisitor(Stream& st) : m_st(st) {}
            virtual bool operator()(StarTeam* sp, UInt8 idx)
            {
                static UInt32 score[3] = {3, 2, 1};
                if(!sp)
                {
                    m_st << static_cast<UInt16>(0);
                    m_st << "" << static_cast<UInt16>(0);
                    m_st << "" << static_cast<UInt16>(0);
                    m_st << "" << static_cast<UInt16>(0);
                }
                else
                {
                    m_st << static_cast<UInt16>(sp->_teamId);
                    std::vector<SingleHeroPlayer*>& playerList = sp->_playerList;
                    UInt8 cnt = playerList.size();
                    UInt8 i = 0;
                    for(i = 0; i < cnt; ++ i)
                    {
                        SingleHeroPlayer* spl = playerList[i];
                        m_st << spl->_player->getName() << static_cast<UInt16>(score[idx]);
                    }
                    for(; i < 3; ++ i)
                    {
                        m_st << "" << static_cast<UInt16>(0);
                    }
                }

                return true;
            }

            Stream& m_st;
        };

        SHLeaderBoardVisitor slbVisitor(st);
        visitFinals(slbVisitor);
    }


    bool SHFinalStarStage::process()
    {
        if(m_progress > e_sh_0)
            return true;

        bool ret = false;

        for(UInt8 i = 0; i < 3; ++ i)
        {
            UInt32 rid = 0;
            UInt8 idx1 = chelun[m_round][i][0];
            UInt8 idx2 = chelun[m_round][i][1];

            StarTeam* sp1 = NULL;
            StarTeam* sp2 = NULL;
            if(idx1 < m_finals.size())
                sp1 = m_finals[idx1];

            if(idx2 < m_finals.size())
                sp2 = m_finals[idx2];

            if(sp1 == NULL || sp2 == NULL)
            {
                continue;
            }
            bool result = sp1->challenge(sp2, &rid);
            TeamStageBattleReport sbr;
            TeamStageBattleReport sbr2;
            sbr._type = m_type;
            sbr._won = !result;
            sbr._other = sp2;
            sbr._battleTime = TimeUtil::Now();
            sbr._battleId = rid;
            sp1->_battles.push_back(sbr);
            sbr2._type = m_type;
            sbr2._won = result;
            sbr2._other = sp1;
            sbr2._battleTime = TimeUtil::Now();
            sbr2._battleId = rid;
            sp2->_battles.push_back(sbr2);
            if(result)
                ++ sp1->_won;
            else
                ++ sp2->_won;

            DB1().PushUpdateData("REPLACE INTO `sh_preliminary`(`gpId`, `cls`, `type`, `won`, `player1`, `player2`, `battleTime`, `battleId`) VALUES(%u, 0, %u, %u, %u, %u, %u, %u)", m_gpId, m_type, result ? 1 : 0, sp1->_teamId, sp2->_teamId, sbr._battleTime, sbr._battleId);
        }

        ++ m_round;
        if(m_round == 5)
        {
            ret = true;
            ++ m_progress;

            class SHGiveFinalAward : public SHTeamVisitor
            {
            public:
                virtual bool operator()(StarTeam* sTeam, UInt8 idx)
                {
                    static UInt32 score[3] = {3, 2, 1};
                    if(!sTeam)
                        return false;

                    std::vector<SingleHeroPlayer*>& plList = sTeam->_playerList;
                    for(std::vector<SingleHeroPlayer*>::iterator it = plList.begin(); it != plList.end(); ++ it)
                    {
                        SingleHeroPlayer* sp = *it;
                        UInt8 cls = sp->_fgt->getClass();
                        shStageMgr.addScore(sp->_player, cls, score[idx], true);
                        m_sbs->sendStageMail(sp, idx);
                    }

                    return true;
                }

                SHGiveFinalAward(SHFinalStarStage * sbs) { m_sbs = sbs; }
                SHFinalStarStage * m_sbs;
            };

            SHGiveFinalAward awardvisitor(this);
            visitFinals(awardvisitor);

        }

        write2DB();
        return ret;
    }

    void SHFinalStarStage::sendStageMail(SingleHeroPlayer* spl, UInt8 idx)
    {
        static MailPackage::MailItem award[3][2] = 
        {
            {{509, 3}, {9076, 3}},
            {{509, 2}, {9076, 2}},
            {{509, 1}, {9076, 1}},
        };

        GET_STAGE_NAME(sn, m_gpId)
        SYSMSGV(title, 880, sn);
        SYSMSGV(content, 890, sn, sn, idx + 1);

        Mail * mail = spl->_player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail != NULL)
        {
            Player* pl = spl->_player;
            MailPackage::MailItem* mitem = &award[idx][0];
            UInt32 size = 2;
            std::string strItems;
            for (UInt32 i = 0; i < size; ++i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            mailPackageManager.push(mail->id, mitem, size, true);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    void SHFinalStarStage::loadFromDB()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        char querystr[1024] = {0};

        sprintf(querystr, "SELECT `round`, `progress` FROM `sh_stage_progress` WHERE `type`=%u AND `gpId`=%u AND `cls`=0", m_type, m_gpId);
        DBSHStageProgress stageProgress = {0};
        if(execu->Extract(querystr, stageProgress) == DB::DB_OK)
        {
            m_round = stageProgress.round;
            m_progress = stageProgress.progress;
        }

        {
            DBSHPriliminary pril;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `won`, `player1`, `player2`, `battleTime`, `battleId` FROM `sh_preliminary` WHERE `type`=%u AND `gpId` = %u AND `cls`=0 ORDER BY `battleTime`", m_type, m_gpId);
            if(execu->Prepare(querystr, pril) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    StarTeam* spl1 = m_starTeam[pril.player1];
                    if(spl1 == NULL)
                    {
                        m_starTeam.erase(pril.player1);
                        continue;
                    }
                    StarTeam* spl2 = m_starTeam[pril.player2];
                    if(spl2 == NULL)
                    {
                        m_starTeam.erase(pril.player2);
                        continue;
                    }
                    if(pril.won == 1)
                    {
                        ++ spl1->_won;
                    }
                    else
                    {
                        ++ spl2->_won;
                    }
                    TeamStageBattleReport sbr;
                    TeamStageBattleReport sbr2;
                    sbr._type = m_type;
                    sbr._won = !pril.won;
                    sbr._other = spl2;
                    sbr._battleTime = pril.battleTime;
                    sbr._battleId = pril.battleId;
                    spl1->_battles.push_back(sbr);

                    sbr2._type = m_type;
                    sbr2._won = pril.won;
                    sbr2._other = spl1;
                    sbr2._battleTime = pril.battleTime;
                    sbr2._battleId = pril.battleId;
                    spl2->_battles.push_back(sbr2);
                }
            }
        }

        {
            DBSHFinalPlayer fplayer;
            memset(querystr, 0, sizeof(querystr));
            sprintf(querystr, "SELECT `pos`, `final3pos`, `stage_idx`, `playerId` FROM `sh_final_player` WHERE `type`=%u AND `gpId`=%u AND `cls`=0 ORDER by `pos` ASC", m_type, m_gpId);
            if(execu->Prepare(querystr, fplayer) == DB::DB_OK)
            {
                while(execu->Next() == DB::DB_OK)
                {
                    StarTeam* team = m_starTeam[fplayer.playerId];
                    if(team == NULL)
                    {
                        m_starTeam.erase(fplayer.playerId);
                        continue;
                    }
                    m_finals.push_back(team);
                }
            }
        }
    }


    SHBattleStageMgr::SHBattleStageMgr() : m_progress(0), m_nextTime(0), m_session(0), m_lvlCnt70(0), m_towerEndTime(0), m_fOpen(false)
    {
        m_dstprogress = 0;

        m_starStage = NULL;
        m_starStage2 = NULL;
        m_starFinalStage = NULL;

        memset(m_singleStage, 0, sizeof(m_singleStage));
        memset(m_singleStage2, 0, sizeof(m_singleStage2));
        memset(m_singleFinalStage, 0, sizeof(m_singleFinalStage));

        memset(m_strategyStage, 0, sizeof(m_strategyStage));
        memset(m_strategyStage2, 0, sizeof(m_strategyStage2));
        memset(m_strategyFinalStage, 0, sizeof(m_strategyFinalStage));

        memset(m_towerStage, 0, sizeof(m_towerStage));
        memset(m_topPopular, 0, sizeof(m_topPopular));
        memset(m_topCandidate, 0, sizeof(m_topCandidate));
    }

    SHBattleStageMgr::~SHBattleStageMgr()
    {
    }

    bool SHBattleStageMgr::loadFromDB()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());

        DBSHGlobal global;
        if(execu->Extract("SELECT `session`, `nextTime`, `progress`, `open`, `timeBegin`, `timeEnd`, `cls1TowerLevel`, `cls2TowerLevel`, `cls3TowerLevel` FROM `sh_global`", global) != DB::DB_OK)
            return false;

        m_progress = global.progress;
        m_nextTime = global.nextTime;
        m_session = global.session;
        m_fOpen = global.open;
        m_onOffTime._timeBegin = global.timeBegin;
        m_onOffTime._timeEnd = global.timeEnd;
        if(m_progress == e_sh_tower)
            // XXX neice
            //m_towerEndTime = TimeUtil::Now() + 600;
            m_towerEndTime = TimeUtil::SharpDay(1, m_nextTime) + stageStartTime[m_progress];


        DBSHFighter shFighter;
		if(execu->Prepare("SELECT `id`, `playerId`, `type`, `level`, `soulMax`, `potential`, `capacity`, `citta`, `trump`, `skillstrengthen`  FROM `sh_fighter` ORDER BY `playerId`", shFighter) != DB::DB_OK)
			return true;
		while(execu->Next() == DB::DB_OK)
		{
            Player* pl = globalPlayers[shFighter.playerId];
            if(pl == NULL)
                continue;
            if(shFighter.id > GREAT_FIGHTER_MAX)
                continue;
            Fighter * fgt = globalFighters[shFighter.id];
            if(fgt == NULL)
                continue;
            Fighter* fgt2 = fgt->clone(NULL);
            fgt2->setUpCittasMax();
            SingleHeroFighter* sfgt = new SingleHeroFighter(pl, shFighter.type);
            sfgt->setFighter(fgt2);
            if(shFighter.id < 10)
                sfgt->setName(pl->getName());
            sfgt->setSoulMax(shFighter.soulMax);
            sfgt->setPotential(shFighter.potential);
            sfgt->setCapacity(shFighter.capacity);
            sfgt->setLevel(shFighter.level);
            sfgt->setUpCittas(shFighter.citta);
            sfgt->setUpTrumps(shFighter.trump);
            sfgt->setUpSS(shFighter.skillstrengthen);
            if(shFighter.type == e_sh_gp_sl)
                m_singlFgt[pl] = sfgt;
            if(shFighter.type == e_sh_gp_sy)
                m_strategyFgt[pl] = sfgt;
		}

        if(!loadAttr2())
            return true;

        if(!loadAttrExtraEquip())
            return true;

        DBSHPlayer shPlayer;
        if(execu->Prepare("SELECT `playerId`, `entered`, `teamId`, `tLevel`, `tTurns`, `tLastTurns`, `score` FROM `sh_player` ORDER BY `teamId`", shPlayer) == DB::DB_OK)
        {
            while(execu->Next() == DB::DB_OK)
            {
                Player* pl = globalPlayers[shPlayer.playerId];
                if(!pl)
                    continue;
                SingleHeroPlayer* sp = new SingleHeroPlayer();
                sp->_player = pl;
                sp->_type = shPlayer.entered;
                sp->_teamId = shPlayer.teamId;
                pushPlayer(sp, shPlayer.tLevel, shPlayer.tTurns, shPlayer.tLastTurns);

                SingleHeroFighter* fgt = m_singlFgt[pl];
                if(!fgt)
                    m_singlFgt.erase(pl);
                else
                {
                    UInt8 cls = fgt->getClass();
                    if(shPlayer.score != 0)
                        addScore(pl, cls, shPlayer.score, false);
                }
            }
        }
        else
            return true;


        for(int i = 0; i < 4; ++ i)
        {
            if(m_singleStage[i])
                m_singleStage[i]->loadFromDB();
            if(m_singleStage2[i])
                m_singleStage2[i]->loadFromDB();
            if(m_singleFinalStage[i])
                m_singleFinalStage[i]->loadFromDB();

            if(m_strategyStage[i])
                m_strategyStage[i]->loadFromDB();
            if(m_strategyStage2[i])
                m_strategyStage2[i]->loadFromDB();
            if(m_strategyFinalStage[i])
                m_strategyFinalStage[i]->loadFromDB();

            if(m_towerStage[i])
                m_towerStage[i]->loadFromDB();
        }
        if(m_starStage)
            m_starStage->loadFromDB();
        if(m_starStage2)
            m_starStage2->loadFromDB();
        if(m_starFinalStage)
            m_starFinalStage->loadFromDB();

        DBSHCandidate shCandidate;
        if (execu->Prepare("SELECT `playerId`, `jobClass`, `supportedCount`, `pos` FROM `sh_candidate`", shCandidate) == DB::DB_OK)
        {
            while(execu->Next() == DB::DB_OK)
            {
                Player* pl = globalPlayers[shCandidate.playerId];
                if(!pl)
                    continue;
                SHCandidate *shc = new SHCandidate(pl, shCandidate.supportedCount, shCandidate.pos);
                //SHCandidateIt it = 
                m_stars[shCandidate.jobClass].insert(shc);
                m_candidateMap.insert(std::make_pair(pl, m_stars[shCandidate.jobClass].find(shc)));
                if (shCandidate.pos < 4 && shCandidate.pos > 0)
                    m_topPopular[shCandidate.jobClass][shCandidate.pos - 1] = pl;
            }
        }
        else
            return true;

        DBSupportPlayer shSupportPlayer;
        if (execu->Prepare("SELECT `playerId`, `flag`, `class1PlayerId`, `class2PlayerId`, `class3PlayerId` FROM `sh_supporter`", shSupportPlayer) == DB::DB_OK)
        {
            while (execu->Next() == DB::DB_OK)
            {
                Player* pl = globalPlayers[shSupportPlayer.playerId];
                if(!pl)
                    continue;
                SHSupportPlayer *shsPlayer = new SHSupportPlayer(pl);
                shsPlayer->_flag = shSupportPlayer.flag;
                shsPlayer->_mySupport.resize(4);

                Player* pl1 = globalPlayers[shSupportPlayer.class1PlayerId];
                if(!pl)
                    continue;
                (shsPlayer->_mySupport)[1] = pl1;

                Player* pl2 = globalPlayers[shSupportPlayer.class2PlayerId];
                if(!pl)
                    continue;
                (shsPlayer->_mySupport)[2] = pl2;

                Player* pl3 = globalPlayers[shSupportPlayer.class3PlayerId];
                if(!pl)
                    continue;
                (shsPlayer->_mySupport)[3] = pl3;

                m_support.insert(std::make_pair(pl, shsPlayer));
            }
        }
        else 
            return true;



        return true;
    }

    bool enum_progress(void * ptr, void* param)
    {
        Player * pl = static_cast<Player *>(ptr);
        if(pl == NULL)
            return true;

        shStageMgr.sendProgress(pl);

        return true;
    }

    bool enum_stream(GObject::Player *player, void *v)
    {
        if(player == NULL)
            return true;

        Stream *st = static_cast<Stream *>(v);
        player->send(*st);
        return true;
    }

    void SHBattleStageMgr::process()
    {
        if(!isOpen())
            return;
        UInt32 now = TimeUtil::Now();
        if (now < m_nextTime && getProgress() >= m_dstprogress)
        {
            return;
        }

        UInt8 nextDay = 0;
        UInt32 oldSession = m_session;
        UInt8 oldProgress = m_progress;
        switch (m_progress)
        {
        case e_sh_single_fin_end:
            createTeams();
        case e_sh_single_pre_end:
        case e_sh_single_eli_end:
        case e_sh_strategy_pre_end:
        case e_sh_star_pre_end:
        case e_sh_strategy_eli_s_end:
        case e_sh_star_eli_end:
        case e_sh_strategy_fin_end:
        case e_sh_tower_end:
            m_progress = m_progress - SH_GP_OFFSET_B_E + 1;
            break;
        case e_sh_star_fin_end:
            m_progress = m_progress - SH_GP_OFFSET_B_E + 1;
            // XXX neice
            //m_towerEndTime = now + 600;
            m_towerEndTime = TimeUtil::SharpDay(1, m_nextTime) + stageStartTime[m_progress];
            break;
        case e_sh_result_end:
            m_progress = 0;
            break;
        case 0:    // 开始报名
            if(!openStage(m_nextTime))
            {
                m_progress = e_sh_result_end;
                break;
            }
            clear();
            ++ m_session;
            ++ m_progress;
            // nextTime报名结束 周一12点
            // XXX neice
            //m_nextTime = now + 900; 
            m_nextTime = TimeUtil::SharpDay(1, m_nextTime) + stageStartTime[m_progress]; 
            break;
        case e_sh_signIn: // 报名结束
            ++ m_progress;
            // nextTime单职业预赛 周一13点
            // XXX neice
            m_nextTime = TimeUtil::SharpDay(0, m_nextTime) + stageStartTime[m_progress]; 
            break;
        case e_sh_signIn_end:  // 开赛
            ++ m_progress;
        case e_sh_single_pre:  // 单职业预赛
            nextDay = 1;
        case e_sh_single_eli:  // 单职业淘汰赛
        case e_sh_single_fin:  // 单职业决赛
            if(processSingleStage())
            {
                // XXX neice
                //m_nextTime = now + 300; 
                m_nextTime = TimeUtil::SharpDay(nextDay, m_nextTime) + stageStartTime[m_progress]; 
                m_progress += SH_GP_OFFSET_B_E;
            }
            else
            {
                m_nextTime += stageTimeOffset[m_progress]; 
            }
            break;
        case e_sh_strategy_pre:   // 策略预赛
            nextDay = 1;
        case e_sh_strategy_eli_s: // 策略淘汰赛
        case e_sh_strategy_fin:   // 策略决赛
            if(processStrategyStage())
            {
                // XXX neice
                //m_nextTime = now + 300; 
                m_nextTime = TimeUtil::SharpDay(nextDay, m_nextTime) + stageStartTime[m_progress]; 
                m_progress += SH_GP_OFFSET_B_E;
            }
            else
            {
                m_nextTime += stageTimeOffset[m_progress]; 
            }
            break;
        case e_sh_star_pre:  // 明星预赛
        case e_sh_star_eli:  // 明星淘汰赛
        case e_sh_star_fin:  // 明星决赛
            if(processStarStage())
            {
                // XXX neice
                //m_nextTime = now + 300; 
                m_nextTime = TimeUtil::SharpDay(1, m_nextTime) + stageStartTime[m_progress]; 
                m_progress += SH_GP_OFFSET_B_E;
            }
            else
            {
                m_nextTime += stageTimeOffset[m_progress]; 
            }
            break;
        case e_sh_tower:    // 塔
            if(processTowerStage())
            {
                // XXX neice
                //m_nextTime = now + 300; 
                // 为防止爬塔爬到24点还没结束，这里sharpDay的时间参数－1小时，从而避免将nextTime时间延迟
                m_nextTime = TimeUtil::SharpDay(1, m_nextTime - 3600) + stageStartTime[m_progress]; 
                m_progress += SH_GP_OFFSET_B_E;
            }
            else
            {
                m_nextTime += stageTimeOffset[m_progress]; 
            }
            break;
        case e_sh_result:   // 结果
            voteEnd();
            towerEnd();
            m_progress += SH_GP_OFFSET_B_E;
            processStageOver();
            // nextTime 下次报名 周日12点
            // XXX neice
            //m_nextTime = now + 600;
            m_nextTime = TimeUtil::SharpDay(1, m_nextTime - 3600) + stageStartTime[0]; 
            break;
        }

        if(oldProgress != m_progress)
        {
            globalPlayers.enumerate(enum_progress, static_cast<void *>(this));
        }

        DB1().PushUpdateData("UPDATE `sh_global` SET `progress`=%u, `nextTime`=%u, `session`=%u where `session`=%u", m_progress, m_nextTime, m_session, oldSession);
        return;
    }

    bool SHBattleStageMgr::processStageOver()
    {
        UInt8 progress = getProgress();
        if(progress != e_sh_result)
            return false;

        static MailPackage::MailItem award[3][1] = 
        {
            {{8555, 9}},
            {{8555, 6}},
            {{8555, 3}}
        };

        UInt32 titles[4] = {0, 40, 39, 38};
#define TITLE_TIME_LEN 7*86400
        for(int cls = 1; cls < 4; ++ cls)
        {
            int idx = 0;
            for(SHScoreSetIt it = m_scoreSet[cls].begin(); it != m_scoreSet[cls].end() && idx < 3; ++ it, ++ idx)
            {
                Player* pl = (*it)->_player;

                UInt32 cType = 892;
                if(idx == 0)
                {
                    UInt32 contents[4] = {0, 894, 895, 896};
                    cType = contents[cls];
                    pl->setTitle(titles[cls], TITLE_TIME_LEN);
                }
                SYSMSG(title, 891);
                SYSMSGV(content, cType, idx + 1);

                Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail)
                {
                    MailPackage::MailItem* mitem = &award[idx][0];
                    UInt32 size = 1;
                    std::string strItems;
                    for (UInt32 i = 0; i < size; ++i)
                    {
                        strItems += Itoa(mitem[i].id);
                        strItems += ",";
                        strItems += Itoa(mitem[i].count);
                        strItems += "|";
                    }
                    mailPackageManager.push(mail->id, mitem, size, true);
                    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
                }
            }
        }

        return true;
    }

    bool SHBattleStageMgr::processSingleStage()
    {
        bool ret = true;

        UInt8 process = 0;
        for(int i = 0; i < 4; ++ i)
        {
            if(!m_singleStage[i])
                continue;
            bool ret2 = true;
            switch(m_progress)
            {
            case e_sh_single_pre:
                ret2 = m_singleStage[i]->processPreliminary();
                if(ret2 && m_singleStage2[i])
                    ret2 = m_singleStage2[i]->processPreliminary();
                break;
            case e_sh_single_eli:
                ret2 = m_singleStage[i]->processFinals();
                if(m_singleStage2[i])
                    ret2 = m_singleStage2[i]->processFinals();
                process = 2;
                if(ret2)
                {
                    ret2 = m_singleStage[i]->process3Finals();
                    if(m_singleStage2[i])
                        ret2 = m_singleStage2[i]->process3Finals();
                    process = 3;
                }
                break;
            case e_sh_single_fin:
                {
                    if(m_singleFinalStage[i])
                        ret2 = m_singleFinalStage[i]->process();
                    process = 4;
                }
                break;
            }
            if(!ret2)
                ret = ret2;
        }

        if(process != 0)
        {
            Stream st(REP::SINGLE_HERO);
            st << static_cast<UInt8>(0x06) << static_cast<UInt8>(e_sh_gp_sl) << process << Stream::eos;
            globalPlayers.enumerate(enum_stream, (void *)(&st));
        }
        return ret;
    }

    bool SHBattleStageMgr::processStrategyStage()
    {
        bool ret = true;

        UInt8 process = 0;
        for(int i = 0; i < 4; ++ i)
        {
            if(!m_strategyStage[i])
                continue;
            bool ret2 = true;
            switch(m_progress)
            {
            case e_sh_strategy_pre:
                ret2 = m_strategyStage[i]->processPreliminary();
                if(ret2 && m_strategyStage2[i])
                    ret2 = m_strategyStage2[i]->processPreliminary();
                break;
            case e_sh_strategy_eli_s:
                ret2 = m_strategyStage[i]->processFinals();
                if(m_strategyStage2[i])
                    ret2 = m_strategyStage2[i]->processFinals();
                process = 2;
                if(ret2)
                {
                    ret2 = m_strategyStage[i]->process3Finals();
                    if(m_strategyStage2[i])
                        ret2 = m_strategyStage2[i]->process3Finals();
                    process = 3;
                }
                break;
            case e_sh_strategy_fin:
                {
                    if(m_strategyFinalStage[i])
                        ret2 = m_strategyFinalStage[i]->process();
                    process = 4;
                }
                break;
            }
            if(!ret2)
                ret = ret2;
        }

        if(process != 0)
        {
            Stream st(REP::SINGLE_HERO);
            st << static_cast<UInt8>(0x06) << static_cast<UInt8>(e_sh_gp_sy) << process << Stream::eos;
            globalPlayers.enumerate(enum_stream, (void *)(&st));
        }
        return ret;
    }

    void SHBattleStageMgr::createTeams()
    {
        if(m_starStage)
            m_starStage->createTeams();
    }

    bool SHBattleStageMgr::processStarStage()
    {
        bool ret = true;

        if(!m_starStage)
            return ret;

        bool ret2 = true;
        UInt8 process = 0;
        switch(m_progress)
        {
        case e_sh_star_pre:
            ret2 = m_starStage->processPreliminary();
            if(ret2 && m_starStage2)
                ret2 = m_starStage2->processPreliminary();
            break;
        case e_sh_star_eli:
            ret2 = m_starStage->processFinals();
            if(m_starStage2)
                ret2 = m_starStage2->processFinals();
            process = 2;
            if(ret2)
            {
                ret2 = m_starStage->process3Finals();
                if(m_starStage2)
                    ret2 = m_starStage2->process3Finals();
                process = 3;
            }
            break;
        case e_sh_star_fin:
            {
                if(m_starFinalStage)
                    ret2 = m_starFinalStage->process();
                process = 4;
            }
            break;
        }
        if(!ret2)
            ret = ret2;

        if(process != 0)
        {
            Stream st(REP::SINGLE_HERO);
            st << static_cast<UInt8>(0x06) << static_cast<UInt8>(e_sh_gp_sr) << process << Stream::eos;
            globalPlayers.enumerate(enum_stream, (void *)(&st));
        }
        return ret;
    }

    bool SHBattleStageMgr::processTowerStage()
    {
        bool ret = true;
        UInt32 now = TimeUtil::Now();
        if(now >= m_towerEndTime)
            return ret;

        for(int i = 0; i < 4; ++ i)
        {
            if(!m_towerStage[i])
                continue;
            bool ret2 = m_towerStage[i]->process(m_progress);
            if(!ret2)
                ret = ret2;
        }

        return ret;
    }

    void SHBattleStageMgr::init(UInt32 now, UInt8 weekday)
    {
        if(!getActive())
            return;
        m_nextTime = TimeUtil::SharpDay(0, now)  + (7 - weekday) * 86400 + stageStartTime[0];

        m_progress = 0;
        m_session = 0;
        m_fOpen = false;
        // XXX neice
        //m_nextTime = now;

        DB1().PushUpdateData("INSERT INTO `sh_global`(`session`, `progress`, `nextTime`, `open`, `timeBegin`, `timeEnd`) VALUES(%u, %u, %u, 0, 0, 0)", m_session, m_progress, m_nextTime);
    }

    bool SHBattleStageMgr::enter(Player* player, Fighter* fgt)
    {
        if(m_progress != e_sh_signIn)
            return false;
        if(!player || !fgt)
            return false;
        UInt8 cls = fgt->getClass();
        if(cls > 3)
            return false;

        UInt8 res = 0;
        if(fgt->getLevel() < 70)
            res = 1;
        else
        {
            std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
            if(it != m_players.end())
                res = 2;
            else
            {
                GameMsgHdr hdr(0x263, player->getThreadId(), player, sizeof(Fighter**));
                GLOBAL().PushMsg(hdr, &fgt);
            }
        }

		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x00) <<  static_cast<UInt8>(0x01) << res;
        st << Stream::eos;

        player->send(st);

        return true;
    }


    void SHBattleStageMgr::onFighterClone(Player* player, Fighter* fgt)
    {
        UInt8 cls = fgt->getClass();

        DB1().PushUpdateData("INSERT INTO `sh_player` (`playerId`, `entered`, `teamId`, `tLevel`, `tTurns`, `tLastTurns`, `score`) VALUES (%"I64_FMT"u, 0, 0, 0, 0, 0, 0)", player->getId());

        SingleHeroFighter* slFgt = new SingleHeroFighter(player, fgt, e_sh_gp_sl);
        SingleHeroFighter* syFgt = new SingleHeroFighter(player, cls, e_sh_gp_sy);
        m_singlFgt[player] = slFgt;
        m_strategyFgt[player] = syFgt;
        singleStage(cls)->enter(player, slFgt);
        strategyStage(cls)->enter(player, syFgt);
        towerStage(cls)->enter(player, slFgt);
        SingleHeroPlayer* sp = new SingleHeroPlayer();
        m_players[player] = sp;
        sp->setEnter(player, slFgt, 0, false);
    }


    void SHBattleStageMgr::clear()
    {
        for(int i = 0; i < 4; ++ i)
        {
            if(m_singleStage[i])
                m_singleStage[i]->clear();
            if(m_singleStage2[i])
                m_singleStage2[i]->clear();
            if(m_singleFinalStage[i])
                m_singleFinalStage[i]->clear();

            if(m_strategyStage[i])
                m_strategyStage[i]->clear();
            if(m_strategyStage2[i])
                m_strategyStage2[i]->clear();
            if(m_strategyFinalStage[i])
                m_strategyFinalStage[i]->clear();

            if(m_towerStage[i])
                m_towerStage[i]->clear();
        }
        if(m_starStage)
            m_starStage->clear();
        if(m_starStage2)
            m_starStage2->clear();
        if(m_starFinalStage)
            m_starFinalStage->clear();

        for(int cls = 0; cls < 4; ++ cls)
        {
            for(SHScoreSetIt it = m_scoreSet[cls].begin(); it != m_scoreSet[cls].end(); ++ it)
            {
                delete (*it);
            }
            m_scoreSet[cls].clear();
            m_scoreMap[cls].clear();
        }

        for(std::map<Player*, SingleHeroPlayer*>::iterator spIt = m_players.begin(); spIt != m_players.end(); ++ spIt)
        {
            delete spIt->second;
        }
        m_players.clear();
        for(std::map<Player*, SingleHeroFighter*>::iterator slfIt = m_singlFgt.begin(); slfIt != m_singlFgt.end(); ++ slfIt)
        {
            delete slfIt->second;
        }
        m_singlFgt.clear();
        for(std::map<Player*, SingleHeroFighter*>::iterator syfIt = m_strategyFgt.begin(); syfIt != m_strategyFgt.end(); ++ syfIt)
        {
            delete syfIt->second;
        }
        m_strategyFgt.clear();

        clearVoteData();
        DB1().PushUpdateData("DELETE FROM `sh_candidate`;");
        DB1().PushUpdateData("DELETE FROM `sh_supporter`;");
        DB1().PushUpdateData("DELETE FROM `sh_player`;");
        DB1().PushUpdateData("DELETE FROM `sh_fighter`;");
        DB1().PushUpdateData("DELETE FROM `sh_fighter_attr_extra`;");
        DB1().PushUpdateData("DELETE FROM `sh_fighter_attr2`;");
        m_progress = 0;
        m_dstprogress = 0;
    }


    void SHBattleStageMgr::sendActive(Player* pl)
    {
		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt16>(0x0000) << static_cast<UInt8>(isOpen());
        st << Stream::eos;

        pl->send(st);
    }

    void SHBattleStageMgr::sendProgress(Player* player)
    {
        UInt8 entered = 0;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            entered = 1;

        UInt8 progress = getProgress();
		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x00) << static_cast<UInt8>(progress) << entered;
        st << Stream::eos;

        player->send(st);
    }

    void SHBattleStageMgr::sendStatus(Player* player)
    {
        UInt32 timeLeft = m_nextTime - TimeUtil::Now();
        UInt32 cnt = m_players.size();
        UInt8 entered = 0;
        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
            entered = 1;

		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01) << entered << timeLeft << cnt;
        st << Stream::eos;

        player->send(st);
    }

    void SHBattleStageMgr::sendSHStageInfo(Player* player, UInt8 cls, UInt8 type)
    {
		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02) << cls << type;

        bool fsend = false;
        switch(type)
        {
        case 0x00:
            {
                fsend = true;
                SingleHeroPlayer* sp = NULL;
                SingleHeroPlayer* sp2 = NULL;
                size_t rptCnt = 0;
                size_t rptCnt2 = 0;

                if(m_singleStage[cls])
                {
                    sp = m_singleStage[cls]->getStagePlayer(player);
                    if(sp)
                        rptCnt =  sp->_battles.size();
                }
                if(m_singleStage2[cls])
                {
                    sp2 = m_singleStage2[cls]->getStagePlayer(player);
                    if(sp2)
                        rptCnt2 = sp2->_battles.size();
                }
                UInt8 cnt = static_cast<UInt8>(rptCnt + rptCnt2);
                st << cnt;
                size_t i = 0;
                for(i = 0; i < rptCnt; ++ i)
                {
                    UInt8 type = 0;
                    std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(sp->_battles[i]._other);
                    SingleHeroPlayer* spOther = it->second;
                    st << type << sp->_battles[i]._won << static_cast<UInt16>(spOther->_fgt->getId());
                    st << sp->_battles[i]._battleId << spOther->_player->getName();
                }
                for(i = 0; i < rptCnt2; ++ i)
                {
                    UInt8 type = 1;
                    std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(sp2->_battles[i]._other);
                    SingleHeroPlayer* spOther = it->second;
                    st << type << sp2->_battles[i]._won << static_cast<UInt16>(spOther->_fgt->getId());
                    st << sp2->_battles[i]._battleId << spOther->_player->getName();
                }
            }
            break;
        case 0x01:
            if(m_singleStage[cls])
                fsend = m_singleStage[cls]->sendEliminary(st, 0);
            break;
        case 0x02:
            if(m_singleStage[cls])
                fsend = m_singleStage[cls]->sendEliminary(st, 1);
            break;
        case 0x05:
            if(m_singleStage[cls])
                fsend = m_singleStage[cls]->sendEliminary(st, 2);
            break;
        case 0x03:
            if(m_singleStage2[cls])
                fsend = m_singleStage2[cls]->sendEliminary(st, 0);
            break;
        case 0x04:
            if(m_singleStage2[cls])
                fsend = m_singleStage2[cls]->sendEliminary(st, 1);
            break;
        case 0x06:
            if(m_singleStage2[cls])
                fsend = m_singleStage2[cls]->sendEliminary(st, 2);
            break;
        case 0x07:
            if(m_singleFinalStage[cls])
                fsend = m_singleFinalStage[cls]->sendEliminary(st);
            break;
        }


        st << Stream::eos;
        if(fsend)
            player->send(st);
    }

    void SHBattleStageMgr::sendSYStageInfo(Player* player, UInt8 cls, UInt8 type)
    {
		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x03) << cls << type;

        bool fsend = false;
        switch(type)
        {
        case 0x00:
            {
                fsend = true;
                SingleHeroPlayer* sp = NULL;
                SingleHeroPlayer* sp2 = NULL;
                size_t rptCnt = 0;
                size_t rptCnt2 = 0;

                if(m_strategyStage[cls])
                {
                    sp = m_strategyStage[cls]->getStagePlayer(player);
                    if(sp)
                        rptCnt =  sp->_battles.size();
                }
                if(m_strategyStage2[cls])
                {
                    sp2 = m_strategyStage2[cls]->getStagePlayer(player);
                    if(sp2)
                        rptCnt2 = sp2->_battles.size();
                }
                UInt8 cnt = static_cast<UInt8>(rptCnt + rptCnt2);
                st << cnt;
                size_t i = 0;
                for(i = 0; i < rptCnt; ++ i)
                {
                    UInt8 type = 0;
                    std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(sp->_battles[i]._other);
                    SingleHeroPlayer* spOther = it->second;
                    st << type << sp->_battles[i]._won << static_cast<UInt16>(spOther->_fgt->getId());
                    st << sp->_battles[i]._battleId << spOther->_player->getName();
                }
                for(i = 0; i < rptCnt2; ++ i)
                {
                    UInt8 type = 1;
                    std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(sp2->_battles[i]._other);
                    SingleHeroPlayer* spOther = it->second;
                    st << type << sp2->_battles[i]._won << static_cast<UInt16>(spOther->_fgt->getId());
                    st << sp2->_battles[i]._battleId << spOther->_player->getName();
                }
            }
            break;
        case 0x01:
            if(m_strategyStage[cls])
                fsend = m_strategyStage[cls]->sendEliminary(st, 0);
            break;
        case 0x02:
            if(m_strategyStage[cls])
                fsend = m_strategyStage[cls]->sendEliminary(st, 1);
            break;
        case 0x05:
            if(m_strategyStage[cls])
                fsend = m_strategyStage[cls]->sendEliminary(st, 2);
            break;
        case 0x03:
            if(m_strategyStage2[cls])
                fsend = m_strategyStage2[cls]->sendEliminary(st, 0);
            break;
        case 0x04:
            if(m_strategyStage2[cls])
                fsend = m_strategyStage2[cls]->sendEliminary(st, 1);
            break;
        case 0x06:
            if(m_strategyStage2[cls])
                fsend = m_strategyStage2[cls]->sendEliminary(st, 2);
            break;
        case 0x07:
            if(m_strategyFinalStage[cls])
                fsend = m_strategyFinalStage[cls]->sendEliminary(st);
            break;
        }


        st << Stream::eos;
        if(fsend)
            player->send(st);
    }

    void SHBattleStageMgr::sendSRStageInfo(Player* player, UInt8 cls, UInt8 type)
    {
        sendTeamId(player);
		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x04) << cls << type;

        bool fsend = false;
        switch(type)
        {
        case 0x00:
            {
                fsend = true;
                StarTeam* sp = NULL;
                StarTeam* sp2 = NULL;
                size_t rptCnt = 0;
                size_t rptCnt2 = 0;

                if(m_starStage)
                {
                    sp = m_starStage->getStageTeam(player);
                    if(sp)
                        rptCnt =  sp->_battles.size();
                }
                if(m_starStage2)
                {
                    sp2 = m_starStage2->getStageTeam(player);
                    if(sp2)
                        rptCnt2 = sp2->_battles.size();
                }
                UInt8 cnt = static_cast<UInt8>(rptCnt + rptCnt2);
                st << cnt;
                size_t i = 0;
                for(i = 0; i < rptCnt; ++ i)
                {
                    UInt8 type = 0;
                    StarTeam* spOther = sp->_battles[i]._other;
                    st << type << sp->_battles[i]._won << static_cast<UInt16>(spOther->_teamId);
                    st << sp->_battles[i]._battleId << "";
                }
                for(i = 0; i < rptCnt2; ++ i)
                {
                    UInt8 type = 1;
                    StarTeam* spOther = sp2->_battles[i]._other;
                    st << type << sp2->_battles[i]._won << static_cast<UInt16>(spOther->_teamId);
                    st << sp2->_battles[i]._battleId << "";
                }
            }
            break;
        case 0x01:
            if(m_starStage)
                fsend = m_starStage->sendEliminary(st, 0);
            break;
        case 0x02:
            if(m_starStage)
                fsend = m_starStage->sendEliminary(st, 1);
            break;
        case 0x05:
            if(m_starStage)
                fsend = m_starStage->sendEliminary(st, 2);
            break;
        case 0x03:
            if(m_starStage2)
                fsend = m_starStage2->sendEliminary(st, 0);
            break;
        case 0x04:
            if(m_starStage2)
                fsend = m_starStage2->sendEliminary(st, 1);
            break;
        case 0x06:
            if(m_starStage2)
                fsend = m_starStage2->sendEliminary(st, 2);
            break;
        case 0x07:
            if(m_starFinalStage)
                fsend = m_starFinalStage->sendEliminary(st);
            break;
        }


        st << Stream::eos;
        if(fsend)
            player->send(st);
    }

    void SHBattleStageMgr::sendSupportInfo(Player* player)
    {
        UInt8 progress = getProgress();
        if (progress < e_sh_strategy_eli_s)
        {
            player->sendMsgCode(0, 2210);
            return;
        }
        Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x01);
        st << static_cast<UInt8>(5);
        st << static_cast<UInt8>(progress < e_sh_result? 1 : 2 ); 
        st << static_cast<UInt8>(3); // FIXME: 现在只有三个职业
        for (UInt8 i = 1; i < 4; ++i)
        {
            st << static_cast<UInt8>(m_stars[i].size());
            for (SHCandidateIt it = m_stars[i].begin(); it != m_stars[i].end(); ++it)
            {
                st << static_cast<UInt64> ((*it)->_player->getId());
                st << (*it)->_player->getName();
                st << static_cast<UInt32> ((*it)->_supportedCount);
            }
            for (UInt32 j = 0; j < 3; ++j)
            {
                if (m_topCandidate[i][j])
                    st << static_cast<UInt64> (m_topCandidate[i][j]->getId());
                else
                    st << static_cast<UInt64> (0);
            }
            for (UInt32 j = 0; j < 3; ++j)
            {
                // 鲜花榜前三名的玩家
                if (m_topPopular[i][j])
                    st << static_cast<UInt64> (m_topPopular[i][j]->getId());
                else
                    st << static_cast<UInt64> (0);
            }
        }
        if (m_support.find(player) == m_support.end())
        {
            st << static_cast<UInt8>(0);
        }
        else
        {
            st << static_cast<UInt8>(m_support[player]->getSupportCount());
            for (UInt8 i = 1; i < 4; ++i)
            {
                if (Player *supportPlayer = m_support[player]->getPlayerByClass(i))
                {
                    if (!supportPlayer)
                        continue;
                    st << static_cast<UInt64>(supportPlayer->getId());
                }
            }
        }
        st << Stream::eos;
        player->send(st);
    }

    void SHBattleStageMgr::sendTowerInfo(Player* player, UInt8 cls)
    {
        // 发送给客户端有关爬塔的详细信息
        UInt8 progress = getProgress();
        if (progress < e_sh_tower)
        {
            player->sendMsgCode(0, 2299);// FIXME: 随便用一个msg
            return;
        }

        UInt16 tLevel = 0;
        UInt32 tTurns = 0;

        std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(player);
        if(it != m_players.end())
        {
            tLevel = it->second->_fgt->getTLevel();
            tTurns = it->second->_fgt->getTTurns();
        }

        if (cls > 0 && cls < 4 && m_towerStage[cls])
            m_towerStage[cls]->sendTowerInfo(player, tLevel, tTurns);
    }


    void SHBattleStageMgr::setNextTime()
    {
        m_nextTime = TimeUtil::Now();
    }

    bool SHBattleStageMgr::loadAttrExtraEquip()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		DBSHFighterAttrExtra dbfae;
        if(execu->Prepare("SELECT `fighterId`, `playerId`, `type`,"
                        "`strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`,"
                        "`auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`,"
                        "`hitrate`, `evade`, `critical`, `criticaldmg`, `pierce`, `counter`, `magres`,"
                        "`strengthP`, `physiqueP`, `agilityP`, `intelligenceP`, `willP`, `soulP`, `auraP`,"
                        "`auraMaxP`, `attackP`, `magatkP`, `defendP`, `magdefP`, `hpP`, `toughP`, `actionP`,"
                        "`hitrateP`, `evadeP`, `criticalP`, `criticaldmgP`, `pierceP`, `counterP`, `magresP`,"
                        "`hitrlvl`, `evdlvl`, `crilvl`, `pirlvl`, `counterlvl`, `mreslvl`, `toughlvl`"
                        " FROM `sh_fighter_attr_extra` ORDER BY `playerId`, `fighterId`", dbfae) != DB::DB_OK)
            return false;
		while(execu->Next() == DB::DB_OK)
		{
            Player* pl = globalPlayers[dbfae.playerId];
			if(pl == NULL)
				continue;
            SingleHeroFighter* fgt = NULL;
            if(dbfae.type == e_sh_gp_sl)
            {
                fgt = m_singlFgt[pl];
                if(fgt == NULL)
                {
                    m_singlFgt.erase(pl);
                    continue;
                }
            }
            else if(dbfae.type == e_sh_gp_sy)
            {
                fgt = m_strategyFgt[pl];
                if(fgt == NULL)
                {
                    m_strategyFgt.erase(pl);
                    continue;
                }
            }

            GData::AttrExtra attr;
            attr.strength = dbfae.strength;
            attr.physique = dbfae.physique;
            attr.agility = dbfae.agility;
            attr.intelligence = dbfae.intelligence;
            attr.will = dbfae.will;
            attr.soul = dbfae.soul;
            attr.aura = dbfae.aura;
            attr.auraMax = dbfae.auraMax;
            attr.attack = dbfae.attack;
            attr.magatk = dbfae.magatk;
            attr.defend = dbfae.defend;
            attr.magdef = dbfae.magdef;
            attr.hp = dbfae.hp;
            attr.tough = dbfae.tough;
            attr.action = dbfae.action;
            attr.hitrate = dbfae.hitrate;
            attr.evade = dbfae.evade;
            attr.critical = dbfae.critical;
            attr.criticaldmg = dbfae.criticaldmg;
            attr.pierce = dbfae.pierce;
            attr.counter = dbfae.counter;
            attr.magres = dbfae.magres;

            attr.strengthP = dbfae.strengthP;
            attr.physiqueP = dbfae.physiqueP;
            attr.agilityP = dbfae.agilityP;
            attr.intelligenceP = dbfae.intelligenceP;
            attr.willP = dbfae.willP;
            attr.soulP = dbfae.soulP;
            attr.auraP = dbfae.auraP;
            attr.auraMaxP = dbfae.auraMaxP;
            attr.attackP = dbfae.attackP;
            attr.magatkP = dbfae.magatkP;
            attr.defendP = dbfae.defendP;
            attr.magdefP = dbfae.magdefP;
            attr.hpP = dbfae.hpP;
            attr.toughP = dbfae.toughP;
            attr.actionP = dbfae.actionP;
            attr.hitrateP = dbfae.hitrateP;
            attr.evadeP = dbfae.evadeP;
            attr.criticalP = dbfae.criticalP;
            attr.criticaldmgP = dbfae.criticaldmgP;
            attr.pierceP = dbfae.pierceP;
            attr.counterP = dbfae.counterP;
            attr.magresP = dbfae.magresP;

            attr.hitrlvl = dbfae.hitrlvl;
            attr.evdlvl = dbfae.evdlvl;
            attr.crilvl = dbfae.crilvl;
            attr.pirlvl = dbfae.pirlvl;
            attr.counterlvl = dbfae.counterlvl;
            attr.mreslvl = dbfae.mreslvl;
            attr.toughlvl = dbfae.toughlvl;

            fgt->setAttrExtraEquip(attr);
		}

		return true;

    }

    bool SHBattleStageMgr::loadAttr2()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBSHAttr2 dba2;

		if(execu->Prepare("SELECT `fighterId`, `playerId`, `type`, `soulExtraAura`, `soulAuraLeft`, `elixir_strength`, `elixir_physique`, `elixir_agility`,"
                " `elixir_intelligence`, `elixir_will`, `elixir_soul`, `elixir_attack`, `elixir_defend`, `elixir_critical`, `elixir_pierce`, `elixir_evade`, `elixir_counter`,"
                " `elixir_tough`, `elixir_action` FROM `sh_fighter_attr2` ORDER BY `playerId`, `fighterId`", dba2) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
            Player* pl = globalPlayers[dba2.playerId];
			if(pl == NULL)
				continue;

            SingleHeroFighter* fgt = NULL;
            if(dba2.type == e_sh_gp_sl)
            {
                fgt = m_singlFgt[pl];
                if(fgt == NULL)
                {
                    m_singlFgt.erase(pl);
                    continue;
                }
            }
            else if(dba2.type == e_sh_gp_sy)
            {
                fgt = m_strategyFgt[pl];
                if(fgt == NULL)
                {
                    m_strategyFgt.erase(pl);
                    continue;
                }
            }

            if(fgt->getId() != dba2.fighterId)
                continue;

            ElixirAttr attr;
            attr.strength = dba2.elixir_strength;
            attr.physique = dba2.elixir_physique;
            attr.agility = dba2.elixir_agility;
            attr.intelligence = dba2.elixir_intelligence;
            attr.will = dba2.elixir_will;
            attr.soul = dba2.elixir_soul; // max soul
            attr.attack = dba2.elixir_attack; // atk,magatk
            attr.defend = dba2.elixir_defend; // def,magdef
            attr.critical = dba2.elixir_critical;
            attr.pierce = dba2.elixir_pierce;
            attr.evade = dba2.elixir_evade;
            attr.counter = dba2.elixir_counter;
            attr.tough = dba2.elixir_tough;
            attr.action = dba2.elixir_action;

            fgt->setAttr2(dba2.soulExtraAura, dba2.soulAuraLeft, attr);
        }

        return true;
    }

    void SHBattleStageMgr::pushPlayer(SingleHeroPlayer* sp, UInt16 lvl, UInt32 turns, UInt16 lastTurns)
    {
        Player* pl = sp->_player;
        UInt8 type = sp->_type;

        SingleHeroFighter* fgt = m_singlFgt[pl];
        if(fgt == NULL)
        {
            m_singlFgt.erase(pl);
            return;
        }
        SingleHeroFighter* sfgt = m_strategyFgt[pl];
        if(sfgt == NULL)
            m_strategyFgt.erase(pl);

        sp->_fgt = fgt;

        UInt8 cls = fgt->getClass();
        m_players[pl] = sp;
        if(type & e_sh_slsy)
        {
            singleStage(cls)->pushPlayer(pl, fgt);
            if(sfgt != NULL)
                strategyStage(cls)->pushPlayer(pl, sfgt);
            towerStage(cls)->pushPlayer(pl, fgt, lvl, turns, lastTurns);
        }
        if(type & e_sh_sl_earth)
        {
            singleStage2(cls)->pushPlayer(pl, fgt);
        }
        if(type & e_sh_sl_final)
        {
            singleFinalStage(cls)->pushPlayer(pl, fgt);
        }
        if(type & e_sh_sy_earth)
        {
            if(sfgt != NULL)
                strategyStage2(cls)->pushPlayer(pl, sfgt);
        }
        if(type & e_sh_sy_final)
        {
            if(sfgt != NULL)
                strategyFinalStage(cls)->pushPlayer(pl, sfgt);
        }
        if(type & e_sh_sr)
        {
            starStage()->pushPlayer(pl, sp->_teamId, fgt);
        }
        if(type & e_sh_sr_earth)
        {
            starStage2()->pushPlayer(pl, sp->_teamId, fgt);
        }
        if(type & e_sh_sr_final)
        {
            starFinalStage()->pushPlayer(pl, sp->_teamId, fgt);
        }
    }

    void SHBattleStageMgr::reset()
    {
        m_progress = 0;
        clear();
        DB1().PushUpdateData("UPDATE `sh_global` SET `progress`=%u, `nextTime`=%u where `session`=%u", m_progress, m_nextTime, m_session);
    }

    void SHBattleStageMgr::addScore(Player* pl, UInt8 cls, UInt32 score, bool writeDB)
    {
        if(score == 0)
            return;

        SHScoreMap::iterator it = m_scoreMap[cls].find(pl);
        SHPlayerScore* sp = NULL;
        if(it == m_scoreMap[cls].end())
        {
            sp = new SHPlayerScore();
            sp->_score = score;
            sp->_player = pl;
            SHScoreSetIt scoreIt = m_scoreSet[cls].insert(sp);
            m_scoreMap[cls][pl] = scoreIt;
        }
        else
        {
            sp = *(it->second);
            m_scoreSet[cls].erase(it->second);
            sp->_score += score;
            SHScoreSetIt scoreIt = m_scoreSet[cls].insert(sp);
            m_scoreMap[cls][pl] = scoreIt;
        }

        if(writeDB)
            DB1().PushUpdateData("UPDATE `sh_player` SET `score`=%u WHERE `playerId`=%"I64_FMT"u", sp->_score, pl->getId());
    }

    void SHBattleStageMgr::initSupportData()
    {
        // 初始化人气达人的数据
        /*
        if(type < 2)
        {
            for(int i = 0; i < 16; ++ i)
            {
                if(m_finals[type][0][i] != NULL)
                {
                    st << static_cast<UInt16>(m_finals[type][0][i]->_fgt->_fgt->getId());
                    st << static_cast<UInt8>(m_finals[type][0][i]->_fgt->_fgt->getLevel());
                    st << m_finals[type][0][i]->_player->getName();
                }
                else
                {
                    st << static_cast<UInt8>(0);
                    st << static_cast<UInt8>(0);
                    st << "";
                }
            }

            round = m_progress - 2;
            if(m_round > 0)
                ++ round;
            if(round > 4)
                round = 4;
            st << round;
            for(int r = 0; r < aIndex[round]; ++ r)
            {
                UInt8 cnt = m_finalBattles[type][r]._battleId.size();
                UInt8 wonFlag = 0;
                for(int i = 0; i < cnt; ++ i)
                    wonFlag |= (m_finalBattles[type][r]._won[i] << i);
                st << cnt << wonFlag;
                for(int j = 0; j < cnt; ++ j)
                    st << m_finalBattles[type][r]._battleId[j];
            }
        }
        */
    }

    void SHBattleStageMgr::voteForHero(Player *player, UInt8 cls, Player *votePlayer)
    {
        UInt8 progress = getProgress();
        // 玩家投票给看中的玩家
        if (player->GetLev() < MIN_VOTE_LEVEL)
        {
            // 等级不够，无法投票
            player->sendMsgCode(0, 2214);
            return;
        }
        if (progress < e_sh_strategy_eli_s || progress >= e_sh_result)
        {
            // 还未进入投票开始阶段
            player->sendMsgCode(0, 2210);
            return;
        }
        if (player == votePlayer)
        {
            // 不能投给自己
            player->sendMsgCode(0, 2211);
            return;
        }

        if (m_candidateMap.find(votePlayer) == m_candidateMap.end())
        {
            // 被投票人不在榜单中
            player->sendMsgCode(0, 2212);
            return;
        }

        SingleHeroFighter* sfg = m_singlFgt[votePlayer];
        if(!sfg)
        {
            m_singlFgt.erase(votePlayer);
            return;
        }

        if (!cls || cls > 3 || sfg->getClass() != cls)
            return;
        
        SHSupportPlayer * shsPlayer = NULL;
        if (m_support.find(player) == m_support.end())
        {
            // 没有投票过的玩家，创建一个新投票数据结构
            shsPlayer = new SHSupportPlayer(player);
            if (!shsPlayer)
                return;
            m_support.insert(std::make_pair(player, shsPlayer));
        }
        else
        {
            // 已经投票过的，检查是否重复投票
            shsPlayer = &(*(m_support[player]));
            if (shsPlayer->_flag & (0x01 << (cls - 1)))
            {
                // 已经给该职业投票过
                player->sendMsgCode(0, 2213);
                return;
            }
        }
        shsPlayer->_flag |= 0x01 << (cls - 1);
        (shsPlayer->_mySupport)[cls] = votePlayer;
        
        ++((*m_candidateMap[votePlayer])->_supportedCount);
        UInt8 pos = (*m_candidateMap[votePlayer])->_pos;
        if (!pos)
            pos = 4;

        UInt64 plId1 = 0;
        UInt64 plId2 = 0;
        UInt64 plId3 = 0;
        if ((shsPlayer->_mySupport)[1] != NULL)
            plId1 = (shsPlayer->_mySupport)[1]->getId();
        if ((shsPlayer->_mySupport)[2] != NULL)
            plId2 = (shsPlayer->_mySupport)[2]->getId();
        if ((shsPlayer->_mySupport)[3] != NULL)
            plId3 = (shsPlayer->_mySupport)[3]->getId();

        DB1().PushUpdateData("REPLACE INTO `sh_candidate` (`playerId`, `jobClass`, `supportedCount`, `pos`) VALUES (%"I64_FMT"u, %u, %u, %u)", 
                votePlayer->getId(), (UInt32)cls, ((*m_candidateMap[votePlayer])->_supportedCount),(UInt32)((*m_candidateMap[votePlayer])->_pos));
        DB1().PushUpdateData("REPLACE INTO `sh_supporter` (`playerId`, `flag`, `class1PlayerId`, `class2PlayerId`, `class3PlayerId`) \
                VALUES (%"I64_FMT"u, %u, %"I64_FMT"u, %"I64_FMT"u, %"I64_FMT"u)", player->getId(), shsPlayer->_flag, plId1, plId2, plId3);


        Stream st2(REP::SINGLE_HERO);
        st2 << static_cast<UInt8> (0x00);
        st2 << static_cast<UInt8> (0x04);
        st2 << static_cast<UInt8> (0x00);
        st2 << static_cast<UInt64> (votePlayer->getId());
        st2 << Stream::eos;
        player->send(st2);

        Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x10);
        st << static_cast<UInt8>(cls);
        st << static_cast<UInt64>(votePlayer->getId());
        st << static_cast<UInt32>((*m_candidateMap[votePlayer])->_supportedCount);
        for (UInt8 i = 0; i < pos - 1; ++i)
        {
            if (!m_topPopular[cls][i])
            {
                m_topPopular[cls][i] = votePlayer;
                (*m_candidateMap[votePlayer])->_pos = i + 1;
                DB1().PushUpdateData("REPLACE INTO `sh_candidate` (`playerId`, `jobClass`, `supportedCount`, `pos`) VALUES (%"I64_FMT"u, %u, %u, %u)", 
                        votePlayer->getId(), (UInt32)cls, ((*m_candidateMap[votePlayer])->_supportedCount),(UInt32)((*m_candidateMap[votePlayer])->_pos));
                st << static_cast<UInt64>(m_topPopular[cls][0] ? m_topPopular[cls][0]->getId() : 0);
                st << static_cast<UInt64>(m_topPopular[cls][1] ? m_topPopular[cls][1]->getId() : 0);
                st << static_cast<UInt64>(m_topPopular[cls][2] ? m_topPopular[cls][2]->getId() : 0);
                break;
            }
            if(m_topPopular[cls][i] == votePlayer)
                continue;
            if ((*m_candidateMap[votePlayer])->_supportedCount 
                    > (*m_candidateMap[m_topPopular[cls][i]])->_supportedCount)
            {
                // 需要更新排名
                Player* pl = votePlayer;
                for (UInt8 j = i; j < pos - 1; ++j)
                {
                    Player* tmppl = m_topPopular[cls][j];
                    if (m_topPopular[cls][j])
                    {
                        ++((*m_candidateMap[m_topPopular[cls][j]])->_pos);
                        DB1().PushUpdateData("REPLACE INTO `sh_candidate` (`playerId`, `jobClass`, `supportedCount`, `pos`) VALUES (%"I64_FMT"u, %u, %u, %u)", 
                                m_topPopular[cls][j]->getId(), (UInt32)cls, ((*m_candidateMap[m_topPopular[cls][j]])->_supportedCount),(UInt32)((*m_candidateMap[m_topPopular[cls][j]])->_pos));
                        m_topPopular[cls][j] = pl;
                        (*m_candidateMap[pl])->_pos = j + 1;
                        DB1().PushUpdateData("REPLACE INTO `sh_candidate` (`playerId`, `jobClass`, `supportedCount`, `pos`) VALUES (%"I64_FMT"u, %u, %u, %u)", 
                                m_topPopular[cls][j]->getId(), (UInt32)cls, ((*m_candidateMap[m_topPopular[cls][j]])->_supportedCount),(UInt32)((*m_candidateMap[m_topPopular[cls][j]])->_pos));
                        if(tmppl == votePlayer)
                            break;
                        pl = tmppl;
                    }
                }
                if (pos - 1 < 3)
                    m_topPopular[cls][pos - 1] = pl;
                st << static_cast<UInt64>(m_topPopular[cls][0] ? m_topPopular[cls][0]->getId() : 0);
                st << static_cast<UInt64>(m_topPopular[cls][1] ? m_topPopular[cls][1]->getId() : 0);
                st << static_cast<UInt64>(m_topPopular[cls][2] ? m_topPopular[cls][2]->getId() : 0);
                break;
            }
        }

        // 广播通知所有玩家更新某一人的票数

        st << Stream::eos;
        globalPlayers.enumerate(enum_stream, (void *)(&st));
    }

    void SHBattleStageMgr::clearVoteData()
    {
        //清除人气的所有数据
        for (UInt8 i = 1; i < 4; ++ i)
        {
            for(SHCandidateIt scIt = m_stars[i].begin(); scIt != m_stars[i].end(); ++ scIt)
            {
                delete *scIt;
            }
            m_stars[i].clear();
        }
        m_candidateMap.clear();
        for(std::map<Player*, SHSupportPlayer*>::iterator sspIt = m_support.begin(); sspIt != m_support.end(); ++ sspIt)
        {
            delete sspIt->second;
        }
        m_support.clear();
        memset(m_topPopular, 0, sizeof(m_topPopular));
        memset(m_topCandidate, 0, sizeof(m_topCandidate));
        return;
    }

    void SHBattleStageMgr::newCandidate(Player * player, UInt8 cls)
    {
        // 新增投票候选人
        SHCandidate *shCandidate = new SHCandidate(player);
        if (!shCandidate)
            return;
        m_stars[cls].insert(shCandidate);
        m_candidateMap.insert(std::make_pair(player, m_stars[cls].find(shCandidate)));
        DB1().PushUpdateData("REPLACE INTO `sh_candidate` (`playerId`, `jobClass`, `supportedCount`, `pos`) VALUES (%"I64_FMT"u, %u, %u, %u)", 
                player->getId(), (UInt32)cls, (shCandidate->_supportedCount), (UInt32)(shCandidate->_pos));

    }

    void SHBattleStageMgr::setTopCandidate(Player * player, UInt8 cls, UInt8 pos)
    {
        // 设置置顶的三位候选人
        if (cls < 4 && pos > 0 && pos < 4)
            m_topCandidate[cls][pos - 1] = player;
    }

    void SHBattleStageMgr::voteEnd()
    {
        static int score[3] = {3, 2, 1};
        for(int cls = 1; cls < 4; ++ cls)
        {
            for(int i = 0; i < 3; ++ i)
            {
                if(m_topPopular[cls][i])
                    addScore(m_topPopular[cls][i], cls, score[i], true);
            }
        }
    }

    void SHBattleStageMgr::towerEnd()
    {
        static int score[3] = {3, 2, 1};
        for(int cls = 1; cls < 4; ++ cls)
        {
            for(int i = 0; i < 3; ++ i)
            {
                if(!m_towerStage[cls])
                    continue;
                Player *player = m_towerStage[cls]->getTopPlayer(i + 1);
                if (player)
                {
                    addScore(player, cls, score[i], true);
                }
            }
        }
    }


    void SHBattleStageMgr::sendLeaderBoard(Player* player)
    {
        UInt8 progress = getProgress();
        if(progress != e_sh_result)
            return;
		Stream st(REP::SINGLE_HERO);
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02);
        for(int cls = 1; cls < 4; ++ cls)
        {
            int i = 0;
            for(SHScoreSetIt it = m_scoreSet[cls].begin(); it != m_scoreSet[cls].end() && i < 3; ++ it, ++ i)
            {
                st << (*it)->_player->getName() << static_cast<UInt16>((*it)->_score);
            }
            for(; i < 3; ++ i)
            {
                st << "" << static_cast<UInt16>(0);
            }
        }

        st << Stream::eos;
        player->send(st);
    }

    void SHBattleStageMgr::sendSHLeaderBoard(Player* player)
    {
		Stream st(REP::SINGLE_HERO);

        UInt8 progress = getProgress();
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x02);
        if(progress < e_sh_single_pre)
        {
             st << static_cast<UInt8>(0x00);
        }
        else
        {
            if(progress <= e_sh_single_fin && m_progress != e_sh_single_fin_end)
            {
                UInt8 cls = 0;
                std::map<Player*, SingleHeroFighter*>::iterator it = m_singlFgt.find(player);
                if(it != m_singlFgt.end())
                    cls = it->second->getClass();

                st << static_cast<UInt8>(0x01);
                UInt32 res = 0;
                if(m_singleFinalStage[cls])
                    res = m_singleFinalStage[cls]->getStagePlayer(player) == NULL ? 0 : 3;

                if(res == 0)
                {
                    if(m_singleStage2[cls])
                        res = m_singleStage2[cls]->isInFinal(player) ? 2 : 0;
                    if(res == 0 && m_singleStage[cls])
                        res = m_singleStage[cls]->isInFinal(player) ? 1 : 0;
                }
                st << res;
            }
            else
            {
                st << static_cast<UInt8>(0x02);
                for(int cls = 1; cls < 4; ++ cls)
                {
                    if(m_singleFinalStage[cls])
                    {
                        m_singleFinalStage[cls]->sendLeaderBoard(st);
                    }
                    else
                    {
                        st << "" << static_cast<UInt16>(0);
                    }
                }
            }
        }

        st << Stream::eos;
        player->send(st);
    }

    void SHBattleStageMgr::sendSYLeaderBoard(Player* player)
    {
		Stream st(REP::SINGLE_HERO);

        UInt8 progress = getProgress();
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x03);
        if(progress < e_sh_strategy_pre)
        {
             st << static_cast<UInt8>(0x00);
        }
        else
        {
            if(progress <= e_sh_strategy_fin && m_progress != e_sh_strategy_fin_end)
            {
                UInt8 cls = 0;
                std::map<Player*, SingleHeroFighter*>::iterator it = m_strategyFgt.find(player);
                if(it != m_strategyFgt.end())
                    cls = it->second->getClass();

                st << static_cast<UInt8>(0x01);
                UInt32 res = 0;
                if(m_strategyFinalStage[cls])
                    res = m_strategyFinalStage[cls]->getStagePlayer(player) == NULL ? 0 : 3;

                if(res == 0)
                {
                    if(m_strategyStage2[cls])
                        res = m_strategyStage2[cls]->isInFinal(player) ? 2 : 0;
                    if(res == 0 && m_strategyStage[cls])
                        res = m_strategyStage[cls]->isInFinal(player) ? 1 : 0;
                }
                st << res;
            }
            else
            {
                st << static_cast<UInt8>(0x02);
                for(int cls = 1; cls < 4; ++ cls)
                {
                    if(m_strategyFinalStage[cls])
                    {
                        m_strategyFinalStage[cls]->sendLeaderBoard(st);
                    }
                    else
                    {
                        st << "" << static_cast<UInt16>(0);
                    }
                }
            }
        }

        st << Stream::eos;
        player->send(st);
    }

    void SHBattleStageMgr::sendSRLeaderBoard(Player* player)
    {
        sendTeamId(player);
		Stream st(REP::SINGLE_HERO);

        bool fSend = true;
        UInt8 progress = getProgress();
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x04);
        if(progress < e_sh_star_pre)
        {
             st << static_cast<UInt8>(0x00);
        }
        else
        {
            if(progress <= e_sh_star_fin && m_progress != e_sh_star_fin_end)
            {
                st << static_cast<UInt8>(0x01);
                UInt32 res = 0;
                if(m_starFinalStage)
                    res = m_starFinalStage->getStageTeam(player) == NULL ? 0 : 3;

                if(res == 0)
                {
                    if(m_starStage2)
                        res = m_starStage2->isInFinal(player) ? 2 : 0;
                    if(res == 0 && m_starStage)
                        res = m_starStage->isInFinal(player) ? 1 : 0;
                }
                st << res;
            }
            else
            {
                st << static_cast<UInt8>(0x02);
                if(m_starFinalStage)
                    m_starFinalStage->sendLeaderBoard(st);
                else
                    fSend = false;
            }
        }

        st << Stream::eos;
        if(fSend)
            player->send(st);
    }

    void SHBattleStageMgr::sendSupportLeaderBoard(Player* player)
    {
		Stream st(REP::SINGLE_HERO);

        UInt8 progress = getProgress();
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x05);
        if(progress < e_sh_strategy_eli_s)
        {
            st << static_cast<UInt8>(0x00);
        }
        else
        {
            if(progress < e_sh_result)
            {
                st << static_cast<UInt8>(0x01);
                std::map<Player*, SHCandidateIt>::iterator it = m_candidateMap.find(player);
                if(it == m_candidateMap.end())
                    st << static_cast<UInt32>(0);
                else
                {
                    SHCandidateIt candIt = it->second;
                    SHCandidate* cand = *candIt;
                    st << static_cast<UInt32>(cand->_supportedCount);
                }
            }
            else
            {
                st << static_cast<UInt8>(0x02);
                for(int cls = 1; cls < 4; ++ cls)
                {
                    for(int i = 0; i < 3; ++ i)
                    {
                        Player* pl = m_topPopular[cls][i];
                        if(!pl)
                            st << "" << static_cast<UInt16>(0);
                        else
                        {
                            st << pl->getName();
                            std::map<Player*, SHCandidateIt>::iterator it = m_candidateMap.find(pl);
                            if(it == m_candidateMap.end())
                                st << static_cast<UInt16>(0);
                            else
                            {
                                SHCandidateIt candIt = it->second;
                                SHCandidate* cand = *candIt;
                                st << static_cast<UInt16>(cand->_supportedCount);
                            }
                        }
                    }
                }
            }
        }
    }

    void SHBattleStageMgr::sendTowerLeaderBoard(Player* player)
    {
        // 发送有关爬塔排行榜的信息
		Stream st(REP::SINGLE_HERO);

        UInt8 progress = getProgress();
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x06);
        if(progress < e_sh_tower)
        {
            st << static_cast<UInt8>(0x00);
        }
        else
        {
            if(progress == e_sh_tower)
            {
                st << static_cast<UInt8>(0x01);
            }
            else
            {
                static int score[3] = {3, 2, 1};
                st << static_cast<UInt8>(0x02);
                for(int cls = 1; cls < 4; ++ cls)
                {
                    for(int i = 0; i < 3; ++ i)
                    {
                        Player* pl = m_towerStage[cls] ? m_towerStage[cls]->getTopPlayer(i + 1) : NULL;
                        if(!pl)
                            st << "" << static_cast<UInt16>(0);
                        else
                        {
                            st << pl->getName();
                            st << static_cast<UInt16>(score[i]);
                        }
                    }
                }
            }
        }
        st << Stream::eos;
        player->send(st);
    }


    void SHBattleStageMgr::saveStrategyFighter(Player* pl, std::vector<UInt16>& trumps, std::vector<UInt16>& cittas)
    {
        if(!pl)
            return;
        UInt8 progress = getProgress();
        if(progress >= e_sh_strategy_pre)
            return;

        std::map<Player*, SingleHeroFighter*>::iterator it = m_strategyFgt.find(pl);
        if(it == m_strategyFgt.end())
            return;

        SingleHeroFighter* sfgt = it->second;
        if(!sfgt)
            return;

        sfgt->saveStrategy(trumps, cittas);
    }

    void SHBattleStageMgr::showStrategyFighter(Player* pl)
    {
        if(!pl)
            return;

        std::map<Player*, SingleHeroFighter*>::iterator it = m_strategyFgt.find(pl);
        if(it == m_strategyFgt.end())
            return;

        SingleHeroFighter* sfgt = it->second;
        if(!sfgt)
            return;

        sfgt->showStrategy();
    }

    void SHBattleStageMgr::sendTeamId(Player* player)
    {
        if(!player)
            return;
        if(!m_starStage)
            return;

        SingleHeroPlayer* sp = m_starStage->getStagePlayer(player);
        if(sp)
        {
            Stream st(REP::SINGLE_HERO);
            st << static_cast<UInt8>(0x05);
            st << static_cast<UInt16>(sp->_teamId);
            st << Stream::eos;
            player->send(st);
        }
    }

    bool SHBattleStageMgr::openStage(UInt32 openTime)
    {
        if(!m_nextTime)
            return false;
        bool oldOpenFlag = m_fOpen;
        if(openTime < m_onOffTime._timeBegin || openTime > m_onOffTime._timeEnd)
            m_fOpen = false;
        else
            m_fOpen = true;

        if(oldOpenFlag != m_fOpen)
            DB1().PushUpdateData("UPDATE `sh_global` SET `open`=%u WHERE `session`=%u", m_fOpen, m_session);

        return m_fOpen;
    }

    bool SHBattleStageMgr::setOnOffTime(UInt32 timeBegin, UInt32 timeEnd)
    {
        if(!getActive())
            return false;

        bool ret = true;
        m_onOffTime._timeBegin = timeBegin;
        m_onOffTime._timeEnd = timeEnd;
        DB1().PushUpdateData("UPDATE `sh_global` SET `timeBegin`=%u, `timeEnd`=%u WHERE `session`=%u", timeBegin, timeEnd, m_session);
        if(!isOpen())
        {
            time_t curtime = time(NULL);
            UInt32 now = TimeUtil::Now();
            struct tm *local = localtime(&curtime);
            UInt8 wday = static_cast<UInt8>(local->tm_wday);
            if(wday == 0)
                wday = static_cast<UInt8>(7);

            m_nextTime = TimeUtil::SharpDay(0, now)  + (7 - wday) * 86400 + stageStartTime[0];

            ret = openStage(m_nextTime);
            reset();
        }

        return ret;
    }
}

