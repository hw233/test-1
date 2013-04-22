#include "ClanBoss.h"
#include "Map.h"

#include <arpa/inet.h>
#include "Common/TimeUtil.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "Server/SysMsg.h"
#include "GData/GDataDBExecHelper.h"
#include "GObject/Package.h"
#include "GData/NpcGroup.h"
#include "MsgID.h"
#include "GObject/WBossMgr.h"
#include "Country.h"
#include "MapCollection.h" 

namespace GObject
{
extern URandom GRND;
#define CLANBOSS_START_TIME_HOUR 14 
#define CLANBOSS_START_TIME_MIN  0
#define TIME_60 60
#define ONE_DAY_SECOND (24*3600)
const UInt8 g_rankSize = 3;
static const int CLANBOSS_CHANGE_TIME = 10*60;    //boss状态改变时间
static const int CLANBOSS_PROCESS_TIME = 60*60;   //boss持续时间
const UInt8 g_buffTime = 30;
const UInt16 g_pickMaxPos = 50;
const UInt8 g_pickNum    = 60;
const UInt8 g_pickGongxian = 20;
const UInt16 g_emMaxPos = 50;
const UInt16 g_bossNpcId = 5515;
const UInt16 g_bossSpot = 12806;
const UInt32 g_bossGongxian= 500000;
const UInt32 g_powerGongxian = 100000;
const UInt32 g_empowerFull = 4000; //充满是1000点  
const UInt32 g_empowerNum  = 10;   //每次充10点能量
const UInt32 g_usedXianyun = 10;
const UInt32 g_gongxian   = 20;
const UInt32 g_gongxian2   = 60;

ClanBoss::ClanBoss()
{
    clear();
}
void ClanBoss::clear()
{
    m_openTime = 0;
    m_notifyRate = 0;
    m_bossStatus = 0;
    m_isOpening = false;
    _totalPercent = 0;
    _minutes = 0;
    _seconds = 0;
    memset(_empower, 0, sizeof(_empower));
    memset(_emPlayers, 0, sizeof(_emPlayers));
    _pickPlayer.clear();
    _pickPlayers = 0;
    _statusChanged = false;

    for (UInt8 i = 0; i < 6; ++i)
        _emClan[i].clear();
    for (UInt8 i = 0; i < 4; ++i)
        _membersClan[i].clear();
    _playerStatus.clear();
    _playerEmPos.clear();
    _leaderClan.clear();
    _skillsClan.clear();
    _crazyPlayer.clear();
}
void ClanBoss::init()
{
    UInt32 day7 = TimeUtil::SharpWeek(7);
    UInt32 now = TimeUtil::Now();
    UInt32 startTime = day7+ CLANBOSS_START_TIME_HOUR*3600+CLANBOSS_START_TIME_MIN*60;
    if (day7 >= now || now < startTime)
    {
        m_openTime = startTime;
    }
    else if (now-startTime < CLANBOSS_PROCESS_TIME) //14-15点之间down机了
    {
        start();
    }
    else //下个礼拜再开
    {
        m_openTime = startTime+7*86400;
    }
}
void ClanBoss::sendStatus(Player* pl, UInt8 t)
{
    if (t != 2)
        t = m_isOpening;
    UInt32 tm = 0;
    if (t == 0)
        tm = m_openTime - TimeUtil::Now();
    else if (t == 1)
        tm = TimeUtil::Now() - m_openTime;
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(20);
    st << t;
    st << tm;
    st << Stream::eos;
    if (pl)
        pl->send(st);
    else
        NETWORK()->Broadcast(st);
}

void ClanBoss::process(UInt32 now)
{
    int tm = m_openTime - now;
    if (0 == m_notifyRate && tm >= 0 && tm <= 15*TIME_60)
    {
        m_notifyRate = 1;
    }
    switch (m_notifyRate)
    {
        case 1:
            SYSMSG_BROADCASTV(4210, 15);
            m_notifyRate = 2;
            break;
        case 2:
            if (tm > 10*TIME_60)
                return;
            SYSMSG_BROADCASTV(4210, 10);
            m_notifyRate = 3;
            break;
        case 3:
            if (tm > 5*TIME_60)
                return;
            SYSMSG_BROADCASTV(4210, 5);
            m_notifyRate = 4;
            break;
        case 4:
            if (tm > 5)
                return;
            m_notifyRate = 5;
            start();
            break;
        case 5:
            if (tm > (-1*CLANBOSS_PROCESS_TIME))
                return;
            close();
            m_notifyRate = 0;
            break;
        default:
            break;
    }
}

void ClanBoss::start()
{
    if (m_isOpening)
        return;
    m_notifyRate = 5;
    m_openTime = TimeUtil::Now();
    m_isOpening = true;
    _bossPercent = 100;
    _statusChanged = true;
    addBoss();
    Change2HPStatus();
    sendStatus(NULL, 1);
    notify();
    SYSMSG_BROADCASTV(4211);
}
void ClanBoss::close()
{
    UInt32 day7 = TimeUtil::SharpWeek(7);
    UInt32 startTime = day7+ CLANBOSS_START_TIME_HOUR*3600+CLANBOSS_START_TIME_MIN*60;
    m_openTime = startTime+7*86400;

    sendStatus(NULL, 2);
    reward();

    clear();

    Map * p_map = Map::FromSpot(g_bossSpot);
    if (!p_map) return;
    p_map->Hide(g_bossNpcId, true);
	p_map->DelObject(g_bossNpcId);
}


void ClanBoss::refresh()
{
    if (!m_isOpening)
        return;
    _seconds++;
    if (_seconds % 60 == 0)
    {
        _minutes++;
        if (m_powerType == 6)
            _minutes++;
        if (_minutes > 60)
            _minutes = 60;
        UInt8 oldPercent = _totalPercent;
        _totalPercent = _minutes*100/60;
        if (_totalPercent/20 > oldPercent/20)
        {
            SYSMSGV(str, 4204, _totalPercent);
            sendClanBossMsg(str);
        }
        Stream st(REP::CLANBOSS);
        st << static_cast<UInt8>(25);
        st << _totalPercent;
        st << Stream::eos;
        NETWORK()->Broadcast(st);
        if (_totalPercent >= 100)
            close();
    }
    UInt32 now = TimeUtil::Now();
    set<Player*>::iterator it;
    for (it = _pickPlayer.begin(); it != _pickPlayer.end(); )
    {
        Player* pl = *it;
        UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
        if (buffLeft <= now)
        {
            ++it;
            pickComplete(pl, NULL, true);

            //printf("-------------refresh:pick:%d\n", pl->getId());
        }
        else
            ++it;
    }
    for (UInt8 i = 0; i < 6; ++i)
    {
        map<Clan*, set<Player*> >::iterator it;
        for (it = _emClan[i].begin(); it != _emClan[i].end();)
        {
            set<Player*>::iterator iter;
            for (iter = it->second.begin(); iter != it->second.end();)
            {
                Player* pl = *iter; 
                UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);  
                if (buffLeft <= now)
                {
                    ++iter;
                    EmpowerComplete(pl, i, NULL, true);
                    //printf("-------------refresh:Empower:%d\n", pl->getId());
                }
                else
                    ++iter;
                if (it->second.empty())
                    break;
            }
            if (it->second.empty())
                _emClan[i].erase(it++);
            else
                ++it;
        }
    }
    for (map<Player*, UInt8>::iterator it=_playerStatus.begin(); it != _playerStatus.end(); ++ it)
    {
        Player* pl = it->first;
        UInt8 status = it->second;
        if (3 == status)
        {
            UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
            if (isCrazy(pl))
                buffLeft = _crazyPlayer[pl];
            if (buffLeft <= now)
            {
                it->second = 0;
                membersAction(pl->getClan(), pl, false, 3);
                membersAction(pl->getClan(), pl, true, 0);
                sendMyStatus(pl);
            }
        }
    }
}
void ClanBoss::onClanBossReq(GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(pl);
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    br >> type;
    if (!m_isOpening)
    {
        pl->sendMsgCode(0, 1126); 
        return;
    }
    if (pl->getClan() == NULL)
    {
        pl->sendMsgCode(0, 1127);
        return;
    }
 
    switch (type)
    {
        case 0:
            sendStatus(pl, m_isOpening);
            makePlayerInfo(pl);
            sendClanSkillsInfo(pl);
            break;
        case 1:
            notify(pl);
            break;
        case 2:
            {
                UInt64 other = 0;
                br >> other;
                pickXianyun(pl, other);
            }
            break;
        case 3:
            {
                UInt8 t = 0;
                br >> t;
                pickComplete(pl, NULL);
            }
            break;
        case 4:
            sendPickStream(pl);
            break;
        case 5:
            {
                UInt64 clanId = 0;
                br >> clanId;
                sendClanPickerStream(pl, clanId);
            }
            break;
        case 6:
            {
                UInt8 t = 0;
                UInt64 other = 0;
                br >> t;
                br >> other;
                if (t >= 1 && t <=6)
                    Empowerment(pl, t-1, other);
            }
            break;
        case 7: //放弃充能
            {
                UInt8 t = 0;
                br >> t;
                if (t >= 1 && t <=6)
                    EmpowerComplete(pl, t-1, NULL,false, true);
            }
            break;
        case 8:
            {
                UInt8 t = 0;
                br >> t;
                if (t >= 1 && t <= 6)
                    sendEmpowerStream(pl, t-1);
            }
            break;
        case 9:
            {
                UInt8 t = 0;
                UInt64 other = 0;
                br >> t;
                br >> other;
                if (t >= 1 && t <= 6)
                    sendClanEmpowerStream(pl, t-1, other); 
            }
            break;
        case 10:
            {
                sendEmpowerPercentStream(pl);
            }
            break;
        case 11:
            {
                sendMyClanInfo(pl);
            }
            break;
        case 12:
            {
                UInt8 t = 0;
                br >> t;
                Urge(pl, t);
            }
            break;
        case 13:
            {
                UInt8 t = 0; 
                br >> t;
                UseClanSkill(pl, t);
            }
            break;
    }
}
void ClanBoss::makePlayerInfo(Player* pl)
{
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(0);
    Clan* cl = pl->getClan();
    if (NULL != cl)
    {
        UInt8 status = 0;
        UInt8 buffTime = 0;
        UInt8 pos = 0;
        status = getMyStatus(pl,buffTime);
        if (status == 1)
            pos = _playerEmPos[pl];
        st << static_cast<UInt8>(0);
        st << status;
        st << buffTime;
        st << pos;
        st << _totalPercent;
        st << getLeader(cl)->getName().c_str();
        st << cl->getXianyun();
        st << getClanGongxian(cl);
        st << getPlayerGongxian(pl);
        st << getMembers(cl, 0);
        st << getMembers(cl, 1);
        st << getMembers(cl, 2);
        st << getMembers(cl, 3);
        st << cl->getUrge(0) << cl->getUrge(1) << cl->getUrge(2);
        for (int i = 0; i < 6; ++i)
            st << _empower[i];
        //排行榜
        UInt8 rankSize = _gxSort.size() > g_rankSize ? g_rankSize : _gxSort.size();
        st << static_cast<UInt8>(rankSize);
        UInt8 count = 0;
        for (TSortMap::iterator it = _gxSort.begin(); it != _gxSort.end() && count < rankSize; ++it)
        {
            ++count;
            st << it->second->getName();
            st << it->first;
        }
    }
    else
        st << static_cast<UInt8>(1);
    st << Stream::eos;
    pl->send(st);
    notify(pl);
}
void ClanBoss::sendPickStream(Player* pl)
{
    UInt16 clanCount = _membersClan[2].size();
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(4);
    st << _pickPlayers;
    st << g_pickMaxPos;
    st << clanCount;
    for (map<Clan*, set<Player*> >::iterator it=_membersClan[2].begin(); it != _membersClan[2].end(); ++it)
    {
        Clan* cl = it->first;
        if (NULL != cl)
            st << static_cast<UInt64>(cl->getId()) << cl->getName().c_str() << static_cast<UInt8>(it->second.size());
    }
    st << Stream::eos;
    if (NULL != pl)
        pl->send(st);
}
void ClanBoss::sendClanPickerStream(Player* pl, UInt64 clanId)
{
    Clan* cl = globalClans[clanId];
    if (NULL != cl)
    {
        UInt32 now = TimeUtil::Now();
        map<Clan*, set<Player*> >::iterator it = _membersClan[2].find(cl);
        if (it != _membersClan[2].end())
        {
 
            Stream st(REP::CLANBOSS);
            st << static_cast<UInt8>(5);
            
            UInt8 count = it->second.size();
            st << count;
            for (set<Player*>::iterator iter=it->second.begin(); iter!=it->second.end(); ++iter)
            {
                Player* p = *iter;
                st << static_cast<UInt64>(p->getId());
                st << p->getName().c_str();
                st << static_cast<UInt8>(p->GetLev());
                
                UInt32 buffLeft = p->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
                if (buffLeft >= now)
                    st << static_cast<UInt8>(buffLeft-now);
                else
                    st << static_cast<UInt8>(0);
            }
            st << Stream::eos;
            pl->send(st);
        }
    }
}
void ClanBoss::sendEmpowerStream(Player* pl, UInt8 t)
{
    if (t >= 6)
        return;
    UInt16 clanCount = _emClan[t].size();
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(8);
    st << _emPlayers[t];
    st << g_emMaxPos;
    st << clanCount;
    for (map<Clan*, set<Player*> >::iterator it=_emClan[t].begin(); it != _emClan[t].end(); ++it)
    {
        Clan* cl = it->first;
        if (NULL != cl)
            st << static_cast<UInt64>(cl->getId()) << cl->getName().c_str() << static_cast<UInt8>(it->second.size());
    }
    st << Stream::eos;
    if (NULL != pl)
        pl->send(st);
    //printf("-----------------------t:%d, emPlayers:%d, clanCount:%d\n", t, _emPlayers[t], clanCount);
}
void ClanBoss::sendClanEmpowerStream(Player* pl, UInt8 t, UInt64 clanId)
{
    Clan* cl = globalClans[clanId];
    if (NULL != cl && t < 6)
    {
        UInt32 now = TimeUtil::Now();
        map<Clan*, set<Player*> >::iterator it = _emClan[t].find(cl);
        if (it != _emClan[t].end())
        {
            Stream st(REP::CLANBOSS);
            st << static_cast<UInt8>(9);
            
            UInt8 count = it->second.size();
            st << count;
            for (set<Player*>::iterator iter=it->second.begin(); iter!=it->second.end(); ++iter)
            {
                Player* p = *iter;
                st << static_cast<UInt64>(p->getId());
                st << p->getName().c_str();
                st << static_cast<UInt8>(p->GetLev());
                
                UInt32 buffLeft = p->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
                if (buffLeft >= now)
                    st << static_cast<UInt8>(buffLeft-now);
                else
                    st << static_cast<UInt8>(0);
            }
            st << Stream::eos;
            pl->send(st);
        }
        else
        {
            Stream st(REP::CLANBOSS);
            st << static_cast<UInt8>(9); 
            st << static_cast<UInt8>(0);
            st << Stream::eos;
            pl->send(st);
        }
    }
}
void ClanBoss::sendEmpowerPercentStream(Player* pl)
{
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(10);
    for (int i = 0; i < 6; ++i)
        st << _empower[i];
    st << Stream::eos;
    if (NULL != pl)
        pl->send(st); 
    else
    {
        for (map<Player*, UInt8>::iterator it = _playerStatus.begin(); it != _playerStatus.end(); ++it)
        {
            it->first->send(st);
        }
    }
}

void ClanBoss::sendMyClanInfo(Player* pl)
{
    if (NULL == pl)
        return;
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    UInt8 status = 0;
    UInt8 buffTime;
    UInt8 pos = 0;
    status = getMyStatus(pl,buffTime);
    if (status == 1)
        pos = _playerEmPos[pl];

    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(11);  
    st << status;
    st << buffTime;
    st << pos;
    st <<  _totalPercent;
    st << getLeader(cl)->getName().c_str();
    st << cl->getXianyun();
    st << getClanGongxian(cl);
    st << getPlayerGongxian(pl);
    st << getMembers(cl, 0);
    st << getMembers(cl, 1);
    st << getMembers(cl, 2);
    st << getMembers(cl, 3);
    st << cl->getUrge(0) << cl->getUrge(1) << cl->getUrge(2);
    st << Stream::eos;
    pl->send(st);
}
void ClanBoss::sendMyStatus(Player* pl)
{
    if (NULL == pl)
        return;
    UInt8 status = 0;
    UInt8 buffTime = 0;
    UInt8 pos = 0;
    UInt32 crazyTm = _crazyPlayer[pl];
    if ( crazyTm > TimeUtil::Now())
    {
        status = 4;
        buffTime = crazyTm - TimeUtil::Now();
    }
    else
    {
        status = getMyStatus(pl,buffTime);
        if (status == 1)
            pos = _playerEmPos[pl];
    }
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(16);  
    st << status;
    st << buffTime;
    st << pos;
    st << Stream::eos;
    pl->send(st); 
}
void ClanBoss::broadClanMembersResume(Clan* cl)
{
    if (NULL == cl)
        return;
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(16);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(0);
    st << Stream::eos;
    broadClanStream(cl, &st);
}

Player* ClanBoss::getLeader(Clan* cl)
{
    if (NULL == cl)
        return NULL;
    map<Clan*, Player*>::iterator iter = _leaderClan.find(cl);
    if (iter != _leaderClan.end())
        return iter->second;
    return cl->getLeader();
}
UInt32 ClanBoss::getClanGongxian(Clan* cl)
{
    if (NULL == cl)
        return 0;
    return cl->getGongxian();
}
UInt32 ClanBoss::getPlayerGongxian(Player* pl)
{
    if (NULL == pl)
        return 0;
    return pl->GetVar(VAR_CLANBOSS_GONGXIAN);
}
UInt8 ClanBoss::getMembers(Clan* cl, UInt8 t)
{
    if (NULL == cl)
        return 0;
    /*0:等待 1:充能 2:采集 3:疗伤*/
    UInt8 num = 0;
    if (t > 3)
        return 0;
    map<Clan*, set<Player*> >::iterator iter = _membersClan[t].find(cl);
    if (iter != _membersClan[t].end())
        num = iter->second.size();
    return num;
}
UInt8 ClanBoss::getMyStatus(Player* pl, UInt8& buffTime)
{
    UInt8 res = 0;
    buffTime = 0;
    UInt32 now = TimeUtil::Now();
    if (isCrazy(pl))
    {
        buffTime = _crazyPlayer[pl] - now;
        return 4;
    }
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
    if (buffLeft >= now)
        buffTime = buffLeft - now;

    return _playerStatus[pl];
}
void ClanBoss::notify(Player* pl)
{
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(1);
    st << m_bossStatus << m_powerType << m_bossMaxHp << _hp << m_maxPower << m_power;
    st << Stream::eos;
    if (pl != NULL)
        pl->send(st);
    else
        NETWORK()->Broadcast(st); 
}

void ClanBoss::addBoss()
{
    const float WBOSS_BASE_TIME = 300.f;
    const float WBOSS_ATK_FACTOR = 0.5f;
    const Int32 WBOSS_MIN_HP = 20000000;
    const Int32 WBOSS_MAX_HP = 350000000;
    const float WBOSS_MAX_ASC_HP_FACTOR = 1.40f;
    const float WBOSS_HP_FACTOR = 7.8;
    const float WBOSS_POWER_FACTOR = 3.0;

    Map * pmap = Map::FromSpot(g_bossSpot);
    MOData mo;
    mo.m_ID = g_bossNpcId;
    mo.m_Hide = false;
    mo.m_Spot = g_bossSpot;
    mo.m_Type = 6;
    mo.m_ActionType = 0;
    if (pmap->AddObject(mo))
    {
        pmap->Show(g_bossNpcId, true, mo.m_Type);
    }
 
    GData::NpcGroups::iterator it = GData::npcGroups.find(g_bossNpcId);
    if (it !=  GData::npcGroups.end())
    {
        m_bossStatus = 1;

        _ng = it->second;
        std::vector<GData::NpcFData>& nflist = _ng->getList();
     //   broadBossAppear(s_tjBoss[m_bossIndex][1], m_loc);
     //   SYSMSG_BROADCASTV(553, s_tjBoss[m_bossIndex][1]);

        Int32 extatk = 0;
        Int32 extmagatk = 0;
        Int32 atk = 0;
        Int32 matk = 0;
        Int32 ohp = 0;
        float lastTime = 0;

        atk = worldBoss.getLastAtk(0);
        matk = worldBoss.getLastMAtk(0);
        ohp = worldBoss.getLastHP(0);
        Int32 baseatk = nflist[0].fighter->getBaseAttack();
        Int32 basematk = nflist[0].fighter->getBaseMagAttack();
        lastTime = worldBoss.getLastTime(0);

        if (ohp < WBOSS_MIN_HP)
            ohp = WBOSS_MIN_HP;
        else if (ohp > WBOSS_MAX_HP)
            ohp = WBOSS_MAX_HP;

        if (lastTime > 0)
        {
            extatk = (WBOSS_BASE_TIME/(float)lastTime - 1.f) * WBOSS_ATK_FACTOR * (atk + baseatk);
            nflist[0].fighter->setExtraAttack(extatk + atk);
            extmagatk = (WBOSS_BASE_TIME/(float)lastTime - 1.f) * WBOSS_ATK_FACTOR * (matk + basematk);
            nflist[0].fighter->setExtraMagAttack(extmagatk + matk);
            nflist[0].fighter->setDirty();
        }
        m_bossMaxHp =  ohp * WBOSS_HP_FACTOR;
        _hp = m_bossMaxHp;
        m_maxPower = ohp * WBOSS_POWER_FACTOR;
        m_power = m_maxPower;
    }
}
void ClanBoss::caclPlayerBuff(Player* pl, bool isAttackBoss)
{
    if (NULL == pl)
        return;
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    UInt32 now = TimeUtil::Now();
    AttrFactor af;
    af.strength=af.physique=af.agility=af.intelligence=af.will=af.soul=af.aura=af.auraMax=af.attack=af.magatk=af.defend=af.magdef=af.hp=af.tough=af.action=af.hitrate=af.evade=af.critical=af.criticaldmg=af.pierce=af.counter=af.magres=1;
    //鼓舞buff
    af.hp = 1+0.01*cl->getUrge(0);
    af.attack = 1+0.01*cl->getUrge(1);
    af.magatk = 1+0.01*cl->getUrge(1);
    af.action = 1+0.01*cl->getUrge(2);
    //战斗指挥
    ClanSkills* skills = getClanSkills(cl);
    if (NULL != skills)
    {
        if (skills->buffEffect[2] > now)
        {
            af.attack *= 1.5;
            af.magatk *= 1.5;
        }
        if (skills->buffEffect[4] > now && isAttackBoss && 2 == m_bossStatus)
        {
            af.attack *= 3;
            af.magatk *= 3;
        }
    }
    //深渊之力
    if (1 == m_bossStatus)
    {
        if (m_powerType == 2 && isAttackBoss)
        {
            af.attack *= 0.5;
            af.magatk *= 0.5;
        }
        if (m_powerType == 3)
        {
            af.hp *= 0.1;
        }
        if (m_powerType == 1)
        {
            af.attack *= 2;
            af.magatk *= 2;
        }
    }
    //狂暴
    if (!isAttackBoss && isCrazy(pl))
    {
        af.attack *= 2;
        af.magatk *= 2;
    }
    //深渊劫阵
    af.attack *= (100-_minutes);
    af.magatk *= (100-_minutes);
    af.attack /= 100;
    af.magatk /= 100;
    
    pl->setHiAfFlag(true);
    pl->setHIAf(af);
    printf("-----------------------hp:%f, attack: %f, powerType:%d\n",af.hp, af.attack, m_powerType);
}
bool ClanBoss::attack(Player* pl)
{
    if (!m_isOpening)
    {
        pl->sendMsgCode(0, 1126); 
        return false;
    }
   static UInt32 sendflag = 7;
    ++sendflag;

    if (!pl ) return false;
    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
    if(buffLeft > now)
    {
        pl->sendMsgCode(0, 1120, buffLeft - now);
        return false;
    }
    if (isCrazy(pl))
    {
        pl->sendMsgCode(0, 1121); 
        return false;
    }
    //pl->checkLastBattled();

    int npcId=g_bossNpcId;
    GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
    if(it == GData::npcGroups.end())
        return false;

    _ng = it->second;
    Battle::BattleSimulator bsim(Battle::BS_WBOSS, pl, _ng->getName(), _ng->getLevel(), false);
    pl->PutFighters(bsim, 0);
    _ng->putFighters(bsim);

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();
    if (sz == 0) return false;

    GData::NpcFData& nfdata = nflist[0];
    UInt32 oldHP = 0;
    UInt32* thisHp;
    UInt8* thisPercent;
    UInt32 maxHp = 0;
    UInt8 oldPercent = 0;
    //GM命令设置最大上限
    if (_setBossHp > 0)
    {
        m_bossMaxHp = _setBossHp;
        nflist[0].fighter->setBaseHP(m_bossMaxHp);
        nflist[0].fighter->setDirty();

        _hp = m_bossMaxHp;
        _powerPercent = 100;
        _bossPercent = 100;
        m_power = m_bossMaxHp;
        m_maxPower = m_bossMaxHp;
        _setBossHp = 0;
    }
    if (m_bossStatus == 1)
    {
        oldHP = m_power;
        thisHp = &m_power;
        thisPercent = &_powerPercent;
        maxHp = m_maxPower;
    }
    else
    {
        oldHP = _hp;
        thisHp = &_hp;
        thisPercent = &_bossPercent;
        maxHp = m_bossMaxHp;
    }
    oldPercent = *thisPercent;
    if (_statusChanged)
    {
        _statusChanged = false;
        nflist[0].fighter->setBaseHP(maxHp); 
        nflist[0].fighter->setDirty(); 
    }
    Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
    bf->setHP(oldHP);
    caclPlayerBuff(pl, true);

    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;

    Battle::BattleObject * obj = bsim(1, nfdata.pos);
    if(obj != NULL && obj->isChar())
    {
        Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
        UInt32 nHP = bfgt->getHP();

//        if(nHP == 0)
//            nHP = 0xFFFFFFFF;
//        if(*thisHp != 0xFFFFFFFF && *thisHp != nHP)
        if (*thisHp != nHP)
            *thisHp = nHP;
        printf("---------------oldhp:%d, newhp:%d, hp:%d\n", oldHP, nHP, oldHP-nHP);
       // if(oldHP != 0xFFFFFFFF)
        {
        //    if(oldHP == 0)
        //        oldHP = maxHp;
        //    UInt32 newHP = (*thisHp == 0xFFFFFFFF) ? 0 : *thisHp;
            UInt32 newHP = *thisHp;
            if(oldHP > newHP)
            {
                UInt32 damage = oldHP - newHP;
                UInt32 exp = ((float)damage / maxHp) * _ng->getExp();
                if (exp < 1000)
                    exp = 1000;
                pl->pendExp(exp);
                UInt32 gx = 0;
                if (1 == m_bossStatus)
                    gx = ((float)damage / maxHp) * g_powerGongxian;
                else
                    gx = ((float)damage / maxHp) * g_bossGongxian;
                addGongXian(pl,pl->getClan(), gx, true);
               // printf("---------------attack, %d/%d, gx:%d\n", damage, maxHp, gx);

               /* AttackInfo info(pl, damage);
                AtkInfoType::iterator i = m_atkinfo.begin();
                while (i != m_atkinfo.end())
                {
                    if ((*i).player == pl)
                    {
                        info += *i;
                        m_atkinfo.erase(i);
                        break;
                    }
                    ++i;
                }
                bool ret = m_atkinfo.insert(info).second;
                */
                UInt32 bossMaxHP = nflist[0].fighter->getBaseHP();
                UInt8 newPercent = (double)newHP / bossMaxHP * 100;
                if (newPercent > 100)
                    newPercent = 100;
                if (*thisPercent > newPercent)
                    *thisPercent = newPercent;
                if (!newPercent)
                {
                    *thisPercent = 0;
                    *thisHp = 0;
                    if (m_bossStatus == 1)
                        Change2HPStatus();
                    else
                        BossDead(pl);
                }
                if (!cfg.GMCheck || !(sendflag % 8))
                    notify();
                //printf("-----------------thisPercent:%d, oldPercent:%d\n", *thisPercent, oldPercent);
                if (m_bossStatus == 2 && oldPercent<100 && *thisPercent/10 < oldPercent/10)
                {
                    SYSMSGV(str, 4205, *thisPercent);
                    sendClanBossMsg(str);
                }
                if (m_bossStatus == 2 && *thisPercent >0 && oldPercent<100 && *thisPercent/20 < oldPercent/20)
                {
                    Change2PowerStatus();
                }
            }
        }
    }
    if(res)
    {
        ret = 0x0101;
        pl->_lastNg = _ng;
        pl->pendExp(_ng->getExp());
        _ng->getLoots(pl, pl->_lastLoot, 1, NULL);
    }
    else
    {
        _playerStatus[pl] = 3;
        if (!toBeCrazy(pl))
            pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, now + g_buffTime);
 
        membersAction(pl->getClan(), pl, true, 3);
        membersAction(pl->getClan(), pl, false, 0);
       // sendMyClanInfo(pl);
        broadClanStatus(pl->getClan());
        sendMyStatus(pl);
        broadClanInfo(pl->getClan(), pl);
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(pl, lastExp) << static_cast<UInt8>(0);
    UInt8 size = pl->_lastLoot.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
    {
        st << pl->_lastLoot[i].id << pl->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    pl->send(st);
    bsim.applyFighterHP(0, pl);

    pl->setHiAfFlag(false);
    pl->clearHIAf();

    return res;

}

bool ClanBoss::toBeCrazy(Player* pl)
{
    if (m_powerType != 1)
        return false;
    _crazyPlayer[pl] = TimeUtil::Now() + 30;
    return true;
}
bool ClanBoss::isCrazy(Player* pl)
{
    return _crazyPlayer[pl] > TimeUtil::Now();
}
void ClanBoss::membersAction(Clan* cl, Player* pl, bool b, UInt8 t)
{
    if (NULL != cl && NULL != pl && t < 4)
    {
        map<Clan*, set<Player*> >::iterator iter = _membersClan[t].find(cl);
        if (iter != _membersClan[t].end())
        {
            set<Player*>& s = iter->second;
            if (b)
                s.insert(pl);
            else
            {
                s.erase(pl);
                if (s.empty())
                    _membersClan[t].erase(iter);
            }
        }
        else if (b)
        {
            set<Player*> temp;
            temp.insert(pl);
            _membersClan[t][cl] = temp;
        }
    }
}
void ClanBoss::pickXianyun(Player* pl, UInt64 other)
{
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
    if(buffLeft > now)
    {
        pl->sendMsgCode(0, 1120, buffLeft - now);
        return;
    }
    bool crazy =false;
    if (0 != other)
    {
        Player* plOther = globalPlayers[other];
        bool notFound = true;
        if (plOther != NULL)
        {
            if (cl == plOther->getClan())
            {
                pl->sendMsgCode(0, 1128);
                return;
            }
            set<Player*>::iterator iter = _pickPlayer.find(plOther);
            if (iter != _pickPlayer.end())
            {
                notFound = false;
                caclPlayerBuff(pl);
                caclPlayerBuff(plOther);
                bool res = pl->challenge(plOther);
                if (res)
                    pickComplete(plOther, pl, false, false);  
                else  //进入狂暴
                {
                    crazy = toBeCrazy(pl);
                    _playerStatus[pl] = 3; 
                    pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, now+g_buffTime);
                    membersAction(cl, pl, false, 0);
                    membersAction(cl, pl, true, 3);
                    broadClanStatus(cl);
                    sendMyStatus(pl);
                    return;
                }
           }
        }
        if (notFound)
        {
            pl->sendMsgCode(0, 1122);
            return;
        }
    }
    else
    {
        if (isCrazy(pl))
        {
            pl->sendMsgCode(0, 1121);
            return;
        }
        if ( _pickPlayer.size() >= g_pickMaxPos)
        {
            pl->sendMsgCode(0, 1123);
            return;
        }
    }
    pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, now+g_buffTime);
    _pickPlayer.insert(pl);
    _pickPlayers++;

    membersAction(cl, pl, false, 0);
    membersAction(cl, pl, true, 2);
    membersAction(cl, pl, false, 3);
    _playerStatus[pl] = 2;

    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(2) << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);

    sendPickStream(pl);
    broadClanStatus(cl);
    sendMyStatus(pl);

    //printf("-------------pickXianyun:%d\n", pl->getId());
}
void ClanBoss::pickComplete(Player* pl, Player* other, bool timeFinish, bool stopBySelf)
{
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    set<Player*>::iterator it = _pickPlayer.find(pl);
    if (it ==  _pickPlayer.end())
        return;
    _playerStatus[pl] = 0;
    membersAction(cl, pl, false, 2);
    if (other == NULL)
        membersAction(cl, pl, true, 0);

    UInt8 res = 0;
    UInt8 num = g_pickNum;
    UInt8 gx = 10;
    if (hasClanSkills(cl, 0))
    {
        num *= 2;
        gx *= 2;
    }
    if (m_bossStatus == 1 && m_powerType == 4)
    {
        num /= 2;
        gx /= 2;
    }
    if (stopBySelf)
    {
        res = 1;
        num /= 2;
        gx /= 2;
    }
    else if (other != NULL)
    {
        res = 2;
        num /= 2;
        gx /= 2;
        membersAction(cl, pl, true, 3);
        _playerStatus[pl] = 3;

        if (toBeCrazy(pl)) //清掉他的Buff,让他狂暴
            pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, TimeUtil::Now());
        else
            pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, TimeUtil::Now()+g_buffTime);
    }
    cl->addXianyun(num);
    SYSMSG_SENDV(175, pl, g_usedXianyun);
    SYSMSG_SENDV(176, pl, g_usedXianyun);
 
    addGongXian(pl, cl, gx);
    _pickPlayer.erase(it);
    _pickPlayers--;

    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(3) << num << res;
    if (2 == res)
        st << other->getClan()->getName().c_str() << other->getName().c_str();
    st << Stream::eos;
    pl->send(st);

    broadClanStatus(cl);
    broadClanInfo(cl, pl);
    sendMyStatus(pl);

//    if (!timeFinish)
//        printf("-------------pickComplete:%d, num:%d\n", pl->getId(), num);
}

void ClanBoss::Empowerment(Player* pl, UInt8 t, UInt64 other)
{
    if (t >= 6)
        return;
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    if (cl->getXianyun() < g_usedXianyun)
    {
        pl->sendMsgCode(0, 1129);
        return;
    }
    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
    if(buffLeft > now)
    {
        pl->sendMsgCode(0, 1120, buffLeft - now);
        return;
    }
    map<Clan*, set<Player*> >::iterator it = _emClan[t].find(cl);
    if (it != _emClan[t].end())
    {
        set<Player*>::iterator iter = it->second.find(pl);
        if (iter != it->second.end())
        {
            pl->sendMsgCode(0, 1125);
            return;
        }
    }
    else
    {
        set<Player*> temp;
        _emClan[t][cl] = temp;
        it = _emClan[t].find(cl);
    }
    if (_empower[t] >= g_empowerFull)
    {
        pl->sendMsgCode(0, 1124);
        return;
    }
    cl->addXianyun(-1*g_usedXianyun);
    SYSMSG_SENDV(177, pl, g_usedXianyun);
    SYSMSG_SENDV(178, pl, g_usedXianyun);
 
    bool crazy = false;
    if (0 != other)
    {
        Player* plOther = globalPlayers[other];
        bool notFount = true;
        if (plOther != NULL)
        {
            if (cl == plOther->getClan())
            {
                pl->sendMsgCode(0, 1128);
                return;
            }
 
            map<Clan*, set<Player*> >::iterator it = _emClan[t].find(plOther->getClan());
            if (it != _emClan[t].end())
            {
                set<Player*>::iterator iter = it->second.find(plOther);
                if (iter != it->second.end())
                {
                    notFount = false;
                    caclPlayerBuff(pl);
                    caclPlayerBuff(plOther);
                    bool res = pl->challenge(plOther);
                    if (res)
                        EmpowerComplete(plOther, t, pl, false, false);  
                    else  //进入狂暴
                    {
                        crazy = toBeCrazy(pl);
                        _playerStatus[pl] = 3;
                        pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, now+g_buffTime);
                        membersAction(cl, pl, false, 0);
                        membersAction(cl, pl, true, 3);
                        broadClanStatus(cl);
                        sendMyStatus(pl);
                        return;
                    }
                }
           }
        }
        if (notFount)
        {
            pl->sendMsgCode(0, 1122);
            return;
        }
    }
    else
    {
        if (isCrazy(pl))
        {
            pl->sendMsgCode(0, 1121);
            return;
        }
    }
    pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, now+g_buffTime);
    
    it->second.insert(pl);
    _emPlayers[t]++;
    _playerEmPos[pl] = t+1;
    
    membersAction(cl, pl, false, 0);
    membersAction(cl, pl, true, 1);
    membersAction(cl, pl, false, 3);
    _playerStatus[pl] = 1;

    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(6) << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);

    sendEmpowerStream(pl, t);
    broadClanStatus(cl);
    broadClanInfo(cl);
    sendMyStatus(pl);
    return;
}
void ClanBoss::EmpowerComplete(Player* pl, UInt8 t, Player* other, bool timeFinish, bool stopBySelf)
{
    if (t >= 6)
        return;
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    map<Clan*, set<Player*> >::iterator it = _emClan[t].find(cl);
    set<Player*>::iterator iter;
    if (it != _emClan[t].end())
    {
        iter = it->second.find(pl);
        if (iter == it->second.end())
            return;
    }
    else
        return;
    _playerStatus[pl] = 0;
    membersAction(cl, pl, false, 1);
    if (NULL == other)
        membersAction(cl, pl, true, 0);
 
    UInt8 res = 0;
    UInt8 num = g_empowerNum;
    UInt32 gongxian = g_gongxian;
    if (t+1 == m_powerType)
        gongxian = g_gongxian2;
    if (hasClanSkills(cl, 1))
    {
        num *= 2;
        gongxian *= 2;
    }
    if (m_bossStatus == 1 && m_powerType == 5)
    {
        num /= 2;
        gongxian /= 2;
    }
    if (stopBySelf)
    {
        res = 1;
        num /= 2;
        gongxian /= 2;
    }
    else if (other != NULL)
    {
        res = 2;
        num /= 2;
        gongxian /= 2;
        membersAction(cl, pl, true, 3);
        _playerStatus[pl] = 3;
        if (toBeCrazy(pl)) //清掉他的Buff,让他狂暴
            pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, TimeUtil::Now());
        else
        {
            pl->setBuffData(PLAYER_BUFF_CLANBOSS_CD, TimeUtil::Now()+g_buffTime);
        }
    }
 
    addGongXian(pl, cl, gongxian);
    _empower[t] += num;

    it->second.erase(iter);
   _emPlayers[t]--;

    membersAction(cl, pl, false, 1);
    
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(7) << num << res;
    if (2 == res)
        st << other->getClan()->getName().c_str() << other->getName().c_str();
    st << Stream::eos;
    pl->send(st);

    broadClanStatus(cl);
    broadClanInfo(cl, pl);
    sendMyStatus(pl);

    if (_empower[t]-num < g_empowerFull/2 && _empower[t] >= g_empowerFull/2)
    {
        SYSMSGV(str, 4200,  t+1, 50);
        sendClanBossMsg(str);
    }
    if (_empower[t] >= g_empowerFull)
    {
        _empower[t] = 0;
        EmpowerRelease(pl, (t+1));
    }
    sendEmpowerPercentStream(NULL);
    return;
}
void ClanBoss::addGongXian(Player* pl, Clan* cl, UInt32 num, bool pend)
{
    if (NULL == pl || NULL == cl || 0 == num)
        return;
    pl->AddVar(VAR_CLANBOSS_GONGXIAN, num);
    UInt32 gx = cl->addGongxian(num);
    if (gx-num < 5000 && gx >= 5000)
    {
        SYSMSGV(str, 4201,cl->getName().c_str(), gx);
        sendClanBossMsg(str);
    }
    UInt32 oldGx = gx-num;
    insertToGxSort(cl, oldGx, gx, true);
    pl->pendLastGongxian(num);
    if (!pend)
        pl->addLastGongxian();
}
void ClanBoss::insertToGxSort(Clan* cl, UInt32 oldGx, UInt32 newGx, bool needBroad)
{
    TSortMap::iterator iter = _gxSort.find(oldGx);
    while (iter != _gxSort.end())
    {
        if (iter->first != oldGx)
            break;
        if (iter->second == cl)
        {
            _gxSort.erase(iter);
            break;
        }
        ++iter;
    }
   _gxSort.insert(make_pair(newGx, cl));
   if (needBroad)
   {
       UInt8 count = 0;
       for (iter=_gxSort.begin(); count < g_rankSize && iter !=_gxSort.end(); ++iter)
       {
           if (iter->second == cl)
           {
               broadClanRank();
               break;
           }
       }
   }
}
void ClanBoss::broadEmpower(UInt8 releaseType, UInt8 powerType)
{
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(15);
    st << releaseType;
    st << powerType;
    st << Stream::eos;
    NETWORK()->Broadcast(st); 
}
const UInt8 g_powerEmPercent = 10;       //对深渊之力造成的伤害 
const UInt8 g_powerEmExtraPercent = 20;  //法器相克额外造成的伤害
const UInt8 g_hpEmPercent = 1;         //对BOSS真身造成的伤害
void ClanBoss::EmpowerRelease(Player* pl,UInt8 t)
{
    broadEmpower(1, t);
    UInt8 percent = 0;
    if (1 == m_bossStatus) //深渊之力
    {
        percent = g_powerEmPercent;
        if (t == m_powerType) 
            percent += g_powerEmExtraPercent;
        if (percent >= _powerPercent)
        {
           Change2HPStatus();
        }
        else
        {
            _powerPercent -= percent;
            m_power -= m_maxPower/100*percent;
            notify();
        }
    }
    else if (2 == m_bossStatus) //真身
    {
        percent = g_hpEmPercent;
        if (percent >= _bossPercent)
        {
            BossDead(pl);
        }
        else if (_bossPercent < 100 && _bossPercent/20 > (_bossPercent-percent)/20)
        {
            _bossPercent -= percent;
            _hp -= m_bossMaxHp/100*percent;
            Change2PowerStatus();
        }
        else
        {
            _bossPercent -= percent;
            _hp -= m_bossMaxHp/100*percent;
            notify();
        }
    }
}
void ClanBoss::Change2HPStatus()
{
    _statusChanged = true;
    if (m_powerType > 0)
    {
        SYSMSGV(str, 4203, m_powerType);
        sendClanBossMsg(str);
    }
    m_power = 0;
    m_powerType = 0;
    _powerPercent = 0;
    m_bossStatus = 2;

    for (UInt8 i = 0; i < 6;++i)
    {
        _empower[i] /= 2;
    }
    sendEmpowerPercentStream(NULL);
    notify();
}
void ClanBoss::Change2PowerStatus()
{
    _statusChanged = true;
    m_power = m_maxPower;
    m_powerType = uRand(6);
    _powerPercent = 100;
    m_bossStatus = 1;

    broadEmpower(2, m_powerType);
    notify();
}
void ClanBoss::BossDead(Player* pl)
{
    close();
}
void ClanBoss::Urge(Player* pl, UInt8 t)
{
    if (t >= 3)
        return;
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(12);
    UInt8 res = 0;
    if (cl->getUrge(t) >= 100)
        res = 1;
    else
    {
        if (pl->getGold() < 30)
        {
            pl->sendMsgCode(0, 1104);
            return;
        }
        ConsumeInfo ci(ClanBossAct,0,0);
        pl->useGold(30, &ci);
        cl->addUrge(t, 1);

        broadClanSkillsInfo(cl);
    }
    st << res;
    st << Stream::eos;
    pl->send(st);
//    sendMyClanInfo(pl);
    broadClanUrge(cl);
}

ClanSkills* ClanBoss::getClanSkills(Clan* cl)
{
    map<Clan*, ClanSkills>::iterator iter = _skillsClan.find(cl);
    if (iter != _skillsClan.end())
    {
        return &(iter->second);
    }
    return NULL;
}
bool ClanBoss::hasClanSkills(Clan* cl, UInt8 t)
{
    if (t >= 5)
        return false;
    ClanSkills* p = getClanSkills(cl);
    if (NULL != p)
    {
        UInt32 now = TimeUtil::Now();
        return p->buffEffect[t] > now;
    }
    return false;
}

void ClanBoss::UseClanSkill(Player* pl, UInt8 t)
{
    static const UInt32 s_EffectSecond[] = {120,120,60,0,120};
    static const UInt32 s_CDSecond[] = {300,600,1800,900,1200};
//    static const UInt32 s_EffectSecond[] = {10,20,30,40,50};
//    static const UInt32 s_CDSecond[] = {20,30,40,50,60};
    Clan* cl = pl->getClan();
    if (NULL == cl || t >= 5)
        return;
    UInt8 res = 0;
    if (pl != getLeader(cl))
        res = 1;
    else
    {
        if (!IsSkillInCD(cl, t))
        {
            UInt32 now = TimeUtil::Now();
            _skillsClan[cl].buffEffect[t] = now+s_EffectSecond[t];
            _skillsClan[cl].buffCD[t] = now + s_CDSecond[t];

            broadClanSkillsInfo(cl);
            if (3 == t) //全体恢复
            {
                ClanMembersResume(cl);
            }
            SYSMSGV(str, 4202, t+1);
            sendClanBossMsg(str,NULL, cl);
        }
        else
            res = 2;
    }
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(13);
    st << static_cast<UInt8>(res);
    st << Stream::eos;
    pl->send(st);
}
bool ClanBoss::IsSkillInCD(Clan* cl , UInt8 t)
{
    map<Clan*, ClanSkills>::iterator iter = _skillsClan.find(cl);
    if (iter == _skillsClan.end())
    {
        ClanSkills tmp;
        _skillsClan[cl] = tmp;
        return false;
    }
    else
    {
        UInt32 now = TimeUtil::Now();
        return now < (iter->second).buffEffect[t];
    }
}
void ClanBoss::ClanMembersResume(Clan* cl)
{
    if (NULL == cl)
        return;

    UInt32 now = TimeUtil::Now();
    map<Clan*, set<Player*> >::iterator iter3 = _membersClan[3].find(cl);
    set<Player*>::iterator it3;
    if (iter3 != _membersClan[3].end())
    {
        map<Clan*, set<Player*> >::iterator iter0 = _membersClan[0].find(cl);
        if (iter0 == _membersClan[0].end())
        {
            set<Player*> tmp;
            _membersClan[0][cl] = tmp;
            iter0 = _membersClan[0].find(cl);
        }
        for (it3 = iter3->second.begin(); it3 != iter3->second.end(); ++it3)
        {
            (*it3)->setBuffData(PLAYER_BUFF_CLANBOSS_CD, now);
            iter0->second.insert(*it3);
            _playerStatus[*it3] = 0;
        }
        iter3->second.clear();
        broadClanMembersResume(cl);
    }
}

bool ClanBoss::makeClanSkillsStream(Clan* cl, Stream& st)
{
    if (NULL == cl)
        return false;
    bool res = false;
    UInt32 now = TimeUtil::Now();
    UInt32 urgeSecond = CLANBOSS_PROCESS_TIME-_seconds;
    st << static_cast<UInt8>(14);
    st << static_cast<UInt8>(8);
    for (UInt8 i = 0; i < 3; ++i)
    {
        st << static_cast<UInt8>(i+1);
        if (cl->getUrge(i) > 0)
        {
            st << urgeSecond;
            res = true;
        }
        else
            st << static_cast<UInt32>(0);
        st << static_cast<UInt32>(0);
    }
    ClanSkills tmp = _skillsClan[cl];
    for (UInt8 i = 0; i < g_clanSkillsNax; ++i)
    {
        st << static_cast<UInt8>(i+11);
        if (tmp.buffEffect[i] > now)
        {
            res = true;
            st << static_cast<UInt32>(tmp.buffEffect[i]-now);
        }
        else
            st << static_cast<UInt32>(0);
        if (tmp.buffCD[i] > now)
        {
            res = true;
            st << static_cast<UInt32>(tmp.buffCD[i]-now);
        }
        else
            st << static_cast<UInt32>(0);
    }
    st << Stream::eos;
    return res;
}
void ClanBoss::sendClanSkillsInfo(Player* pl)
{
    if (NULL == pl || !m_isOpening)
        return;
    Clan* cl = pl->getClan();
    if (NULL == cl)
        return;
    Stream st(REP::CLANBOSS);
    if (makeClanSkillsStream(cl, st))
        pl->send(st);
}
void ClanBoss::broadClanSkillsInfo(Clan* cl)
{
    if (NULL == cl)
        return;
    Stream st(REP::CLANBOSS);
    if (makeClanSkillsStream(cl, st))
    {
        broadClanStream(cl, &st);
    }
}
void ClanBoss::broadClanStream(Clan* cl, Stream* st, Player* pl)
{
    if (NULL == cl || NULL == st)
        return;
    class StreamVisitor : public Visitor<ClanMember>
    {
        public:
            StreamVisitor(Stream* st, Player* pl=NULL)
            {
                _st = st;
                except = pl;

            }

            bool operator()(ClanMember* member)
            {
                if (member->player != except)
                    member->player->send(*_st);
                return true;
            }
        private:
            Stream* _st;
            Player* except;
    };
    StreamVisitor visitor(st);
    cl->VisitMembers(visitor);
}
void ClanBoss::sendClanBossMsg(char* str, Player* pl, Clan* cl)
{
    if (NULL == str)
        return;
    Stream st(REP::CLANBOSS);
    st << static_cast<UInt8>(17);
    st << str;
    st << Stream::eos;
    if (NULL != pl)
        pl->send(st);
    else if (NULL != cl)
        broadClanStream(cl, &st);
    else
        NETWORK()->Broadcast(st); 
}

const UInt8 g_rankRewardSize = 3;
void ClanBoss::reward()
{
    static MailPackage::MailItem s_rankItems[g_rankRewardSize] = {{134,150},{134,100},{134,50}};
    static MailPackage::MailItem s_items[4] = {{134,50},{134,30},{134,20},{134,10}};
    static UInt32  s_score[4] = {10000, 5000, 1000, 1};
    static MailPackage::MailItem s_items2[6] = {{1325,20},{1325,15},{1325,10},{1325,5},{1325,2},{1325,1}};
    static UInt32  s_score2[6] = {3000,2000,1000,500,100,1};
    class RewardVisitor : public Visitor<ClanMember>
    {
        public:
            RewardVisitor(UInt32 gx, UInt8 rank=0, string title="", string content="")
            {
                _gx = gx;
                _rank = rank;
                _title = title;
                _content= content;
            }

            bool operator()(ClanMember* member)
            {
                if (_rank > 0 && _rank <= g_rankRewardSize)
                {
                    member->player->GetMailBox()->newMail(NULL, 0x01, _title, _content);
                }
                UInt32 score =  member->player->GetVar(VAR_CLANBOSS_GONGXIAN);
                if (score == 0)
                    return true;
                SYSMSG(title2, 4222);
                SYSMSGV(content2, 4223, score);
                Mail * mail2 = member->player->GetMailBox()->newMail(NULL, 0x21, title2, content2, 0xFFFE0000);
                if(mail2)
                {
                    for (UInt8 i = 0; i < sizeof(s_score2)/sizeof(s_score2[0]); ++i)
                    {
                        if (score >= s_score2[i])
                        {
                            mailPackageManager.push(mail2->id, s_items2[i].id, s_items2[i].count, true);
                            break;
                        }
                    }
                }
                member->player->SetVar(VAR_CLANBOSS_GONGXIAN, 0);
                return true;
            }
        private:
            UInt32 _gx;
            UInt8 _rank;
            string _title;
            string _content;
    };
 
    TSortMap::iterator it = _gxSort.begin();
    UInt8 rankCount = 0;
    for (; it != _gxSort.end(); ++it)
    {
        if (rankCount < g_rankRewardSize)
        {
            it->second->AddItem(s_rankItems[rankCount].id, s_rankItems[rankCount].count);
        }
        UInt32 score = it->first;
        for (UInt8 i = 0; i < sizeof(s_score)/sizeof(s_score[0]); ++i)
        {
            if (score >= s_score[i])
            {
                it->second->AddItem(s_items[i].id, s_items[i].count);
                break;
            }
        }
        rankCount++;
        SYSMSG(title, 4220);
        SYSMSGV(content, 4221, rankCount, it->first);
        RewardVisitor visitor(it->first, rankCount, title, content);
        it->second->VisitMembers(visitor);
        
        it->second->setGongxian(0, true);
        it->second->clearUrge();
    }
    _gxSort.clear();
}

void ClanBoss::broadClanStatus(Clan* cl)
{
    if (cl)
    {
        Stream st(REP::CLANBOSS);
        st << static_cast<UInt8>(21);
        st << getMembers(cl, 0);
        st << getMembers(cl, 1);
        st << getMembers(cl, 2);
        st << getMembers(cl, 3);
        st << Stream::eos;
        broadClanStream(cl, &st);
    }
}

void ClanBoss::broadClanUrge(Clan* cl)
{
    if (cl)
    {
        Stream st(REP::CLANBOSS);
        st << static_cast<UInt8>(22);
        st << cl->getUrge(0) << cl->getUrge(1) << cl->getUrge(2);
        st << Stream::eos;
        broadClanStream(cl, &st);
    }
}
void ClanBoss::broadClanInfo(Clan* cl, Player* pl)
{
    if (NULL != cl)
    {
        Stream st(REP::CLANBOSS);
        st << static_cast<UInt8>(23);
        st << getLeader(cl)->getName().c_str();
        st << cl->getXianyun();
        st << getClanGongxian(cl);
        st << static_cast<UInt32>(0);
        st << Stream::eos;
        broadClanStream(cl, &st, pl); 
    }
    if (NULL != pl)
    {
        Stream st(REP::CLANBOSS);
        st << static_cast<UInt8>(23);
        st << getLeader(cl)->getName().c_str();
        st << cl->getXianyun();
        st << getClanGongxian(cl);
        st << getPlayerGongxian(pl);
        st << Stream::eos;
        pl->send(st);
    }
}
void ClanBoss::broadClanRank()
{
    Stream st(REP::CLANBOSS); 
    st << static_cast<UInt8>(24); 
    UInt8 rankSize = _gxSort.size() > g_rankSize ? g_rankSize : _gxSort.size();
    st << static_cast<UInt8>(rankSize);
    UInt8 count = 0;
    for (TSortMap::iterator it = _gxSort.begin(); it != _gxSort.end() && count < rankSize; ++it)
    {
        ++count;
        st << it->second->getName();
        st << it->first;
    }
    st << Stream::eos;
    NETWORK()->Broadcast(st);
}


}
