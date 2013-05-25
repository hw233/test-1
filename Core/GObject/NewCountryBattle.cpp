#include "Config.h"
#include "CountryBattle.h"
#include "NewCountryBattle.h"
#include "GObject/Country.h"
#include "Common/Random.h" 
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Map.h"
#include "Mail.h"
#include "World.h"
#include "Common/Serialize.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "SpecialAward.h"
#include "MsgID.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "Package.h"

namespace GObject
{

//新阵营战的每场战斗时间
const static UInt32 NCBATTLE_TIME = 45;
//新阵营战每个玩家霸气怒气上限值
const static UInt32 MAX_BANUQI = 200;

#define COUNT_LOSEACHIEVE(a,b) (4 + (a + b) / 2)
#define COUNT_WINACHIEVE(a,b,c,d) (4 + a + b + (c + d) * 2)

NewCountryBattleSkill NewCountryBattle::m_skills[NEWCOUNTRYBATTLE_SKILL_NUM + 1];

void NewCBPlayerData::setAllEffortInfo()
{
    Table infos = GameAction()->GetNCBEffortIdValue();
    for(UInt8 i = 1; i <= infos.size(); ++ i)
    {
        Table info = infos.get<Table>(i);
        NCB_EffortStruct effort;
        effort.effortId = info.get<UInt8>(1);
        effort.award = info.get<UInt8>(2);
        effort.complete = 0;
        effortInfo.insert(std::make_pair(effort.effortId, effort));
        player->countryBattleUdpLog(1217, 4, Itoa(effort.effortId));
    }
}

UInt8 NewCBPlayerData::updateEffortInfo(UInt8 effortId)
{
    if(effortId == 0)
        return 0;
    std::map<UInt8, NCB_EffortStruct>::iterator iter = effortInfo.find(effortId);
    if(iter != effortInfo.end())
    {
        NCB_EffortStruct& effort = iter->second;
        if(effort.complete)
            return 0;
        effort.complete = 1;
        player->countryBattleUdpLog(1217, 5, Itoa(effortId));
        return effort.award;
    }
    return 0;
}

bool NewCBPlayerData::canAddSkillFlag(UInt8 skillId)
{
    if(skillId == 0 || skillId > 10)
        return false;
    if((skillFlags & (1 << skillId)) != 0)
        return false;
    /*
    if(skillId <= 5)
    {
        for(UInt8 i = 1; i <= 5; ++i)
        {
            if((skillFlags & (1 << i)) != 0)
                return false;
        }
    }
    else
    {
        for(UInt8 i = 6; i <= 10; ++i)
        {
            if((skillFlags & (1 << i)) != 0)
                return false;
        }
    }
    */
    return true;
}

bool NewCBPlayerData::addAngerDomineer(UInt8 flag, UInt8 value)
{
    if(flag)
    {
        if(anger >= MAX_BANUQI)
            return false;
        anger += value;
        if(anger > MAX_BANUQI)
            anger = MAX_BANUQI;
    }
    else
    {
        if(domineer >= MAX_BANUQI)
            return false;
        domineer += value;
        if(domineer > MAX_BANUQI)
            domineer = MAX_BANUQI;
    }
    return true;
}

void NewCBPlayerData::setAchievementLevel(UInt8 achieve)
{
    totalAchievement += achieve;
    if(totalAchievement < 14)
        achLevel = 1;
    else if(totalAchievement < 44)
        achLevel = 2;
    else if(totalAchievement < 102)
        achLevel = 3;
    else if(totalAchievement < 170)
        achLevel = 4;
    else if(totalAchievement < 261)
        achLevel = 5;
    else if(totalAchievement < 375)
        achLevel = 6;
    else if(totalAchievement < 775)
        achLevel = 7;
    else if(totalAchievement < 1195)
        achLevel = 8;
    else
        achLevel = MAX_ACHLEVEL;
}

NewCountryBattle::NewCountryBattle(UInt16 spot)
{
    m_spot = spot;
    m_map = Map::FromSpot(m_spot);
    m_expTime = 0;
    m_tickTime = 0;
    m_topKiller = NULL;
    m_topStreak = 0;
    m_achieveKing = NULL;
    m_maxAchievement = 0;
    
}

void NewCountryBattle::Init()
{
    //获取技能buff配置
    Table buffs = GameAction()->GetNewCountryBattleBuffs();
    size_t buffNum = buffs.size();
    for(UInt32 i = 1; i <= buffNum && i <= NEWCOUNTRYBATTLE_SKILL_NUM; ++i)
    {
        Table buff = buffs.get<Table>(i);
        if(buff.size() < 14) continue;

        NewCountryBattle::m_skills[i].id = i;
        NewCountryBattle::m_skills[i].price = buff.get<UInt32>(1);
        NewCountryBattle::m_skills[i].effortId = buff.get<UInt8>(14);
        NewCountryBattle::m_skills[i].attrs.attackP = NewCountryBattle::m_skills[i].attrs.magatkP = buff.get<float>(2) / 100;
        NewCountryBattle::m_skills[i].attrs.defendP = NewCountryBattle::m_skills[i].attrs.magdefP = buff.get<float>(3) / 100;
        NewCountryBattle::m_skills[i].attrs.hpP = buff.get<float>(4) / 100;
        NewCountryBattle::m_skills[i].attrs.actionP = buff.get<float>(5) / 100;
        NewCountryBattle::m_skills[i].attrs.hitrateP = buff.get<float>(6) / 100;
        NewCountryBattle::m_skills[i].attrs.evadeP = buff.get<float>(7) / 100;
        NewCountryBattle::m_skills[i].attrs.criticalP = buff.get<float>(8) / 100;
        NewCountryBattle::m_skills[i].attrs.criticaldmg = buff.get<float>(9) / 100;
        NewCountryBattle::m_skills[i].attrs.pierceP = buff.get<float>(10) / 100;
        NewCountryBattle::m_skills[i].attrs.counterP = buff.get<float>(11) / 100;
        NewCountryBattle::m_skills[i].attrs.magresP = buff.get<float>(12) / 100;
        NewCountryBattle::m_skills[i].attrs.aura = buff.get<float>(13);
    }
}


bool NewCountryBattle::playerEnter( Player * player )
{
    if (player && player->getCountry() >= COUNTRY_NEUTRAL)
        return false;
	if(!globalCountryBattle.isRunning())
		return false;
	if(player->GetLev() < 40)
		return false;
	UInt32 curtime = TimeUtil::Now();
	if(player->getBuffData(PLAYER_BUFF_NEW_CBATTLE, curtime))
	{
		player->sendMsgCode(0, 1402);
		return false;
	}
    NewCBPlayerData * ncbpData = NULL;
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    if(iter == m_ncbpData.end())
    {
        ncbpData = new(std::nothrow) NewCBPlayerData;
        ncbpData->player = player;
        ncbpData->setAllEffortInfo();
    }
    else
        ncbpData = iter->second;
    ncbpData->type = 0;
    ncbpData->setAchievementLevel();
    m_ncbpData[player] = ncbpData;
    
    GameAction()->doStrong(player, SthCountryWar, 0, 0);
    player->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 0);
	player->addFlag(Player::CountryBattle);
    playerEnterInSpecialTime(curtime, player);
    sendSelfInfo(player);
    
    player->countryBattleUdpLog(1090, player->getCountry());
    player->countryBattleUdpLog(1090, 2);
    player->countryBattleUdpLog(1217, 0);
	return true;
}

void NewCountryBattle::playerEnterInSpecialTime(UInt32 curtime, Player * player)
{
    UInt32 startTime = globalCountryBattle.getStartTime();
    bool pair = false;
    if(curtime > startTime && curtime < m_tickTime)
    {   //处理在一个轮回内新加入的玩家
        if(!findPairPlayer(player))
            pair = true;
    }
    if(pair)
    {   //配对or轮空
        if(m_joinByePlayer.size() != 1)
        {
            m_joinByePlayer.clear();
            m_joinByePlayer.push_back(player);
        }
        else
        {
            if(player != m_joinByePlayer[0])
            {
                m_pairPlayer.insert(std::make_pair(player, m_joinByePlayer[0]));
                m_joinByePlayer.clear();
                sendSelfInfo(m_joinByePlayer[0]);
            }
        }
    }
}

void NewCountryBattle::playerLeave( Player * player )
{
    if (player && player->getCountry() >= COUNTRY_NEUTRAL)
        return;
	if(!globalCountryBattle.isRunning())
		return;
    UInt32 curtime = TimeUtil::Now();
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    if(iter == m_ncbpData.end())
        return;
    NewCBPlayerData * ncbpData = iter->second;
    if(ncbpData->type == 1 || ncbpData->type == 0xFF)
        return;
    ncbpData->type = 1;
    ncbpData->skillFlags = 0;
    ncbpData->setAchievementLevel();
    //m_ncbpData[player] = ncbpData;
	player->delFlag(Player::CountryBattle);
    player->clearHIAttr();
    player->autoRegenAll();
    sendSelfInfo(player);
    if(curtime >= globalCountryBattle.getStartTime() && curtime < globalCountryBattle.getEndTime())
        player->setBuffData(PLAYER_BUFF_NEW_CBATTLE, curtime + 5 * 60);

    if(ncbpData->totalWin)
    {
        UInt32 w = player->GetVar(VAR_COUNTRY_BATTLE_WIN);
        GameAction()->doAttainment(player, Script::COUNTRY_BATTLE_WIN, ncbpData->totalWin + w);
    }
    if(ncbpData->maxKillStreak)
        GameAction()->doAttainment(player, Script::COUNTRY_BATTLE_KILL_STREAK, ncbpData->maxKillStreak);
}

void NewCountryBattle::useSkill(Player * player, UInt8 skillId)
{
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    if(iter != m_ncbpData.end())
    {
        NewCBPlayerData * ncbpData = iter->second;
        if(!ncbpData->canAddSkillFlag(skillId))
            return;
        UInt8 price = m_skills[skillId].price;
        if(skillId <= 5)
        {
            if(ncbpData->domineer < price)
                return;
            ncbpData->domineer -= price;
        }
        else
        {
            if(ncbpData->anger < price)
                return;
            ncbpData->anger -= price;
        }
        ncbpData->setSkillFlag(skillId);
        player->addHIAttr(m_skills[skillId].attrs);
        skillTriggerEffort(ncbpData, m_skills[skillId].effortId);
        sendSelfInfo(player, ncbpData);
        player->countryBattleUdpLog(1217, 6, Itoa(skillId));
    }
}

void NewCountryBattle::skillTriggerEffort(NewCBPlayerData * ncbpData, UInt8 effortId)
{
    UInt8 award = ncbpData->updateEffortInfo(effortId);
    if(award)
    {
        addAchievement(ncbpData->player, award);
        ncbpData->setAchievementLevel(award);
    }
}

void NewCountryBattle::buySkill(Player * player, UInt8 type)
{
    if(type != 0 && type != 1)
        return;
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    if(iter == m_ncbpData.end())
        return;
    if(player->getGold() < 5)
    {
        player->sendMsgCode(0, 1104);
        return;
    }
    NewCBPlayerData * ncbpData = iter->second;
    if(!ncbpData) return;
    if(!ncbpData->addAngerDomineer(type, 50))
        return;
    ConsumeInfo ci(NEWCountryBattleSkill, 0, 0);
    player->useGold(5, &ci);
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x08);
    st << type;
    if(type)
        st << ncbpData->anger;
    else
        st << ncbpData->domineer;
    st << Stream::eos;
    player->send(st);
    player->countryBattleUdpLog(1217, 2, Itoa(type));
}

void NewCountryBattle::checkAddExp(UInt32 curtime)
{
    if(m_expTime > curtime)
        return;
    NCBPlayerData::iterator iter = m_ncbpData.begin();
    while(iter != m_ncbpData.end())
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

void NewCountryBattle::addAchievement(Player * player, UInt8 achieve)
{
    if(!player || !achieve) return;
    if(player->isOnline())
    {
        player->pendAchievement(achieve);
        player->checkLastBattled();
    }
    else
        player->getAchievement(achieve);
}

void NewCountryBattle::allotPlayers()
{
    for(UInt8 lvl = MAX_ACHLEVEL; lvl > 0; --lvl)
    {
        m_players[lvl-1].clear();
    }
    NCBPlayerData::iterator iter = m_ncbpData.begin();
    while(iter != m_ncbpData.end())
    {
        for(UInt8 lvl = MAX_ACHLEVEL; lvl > 0; --lvl)
        {
            NewCBPlayerData * ncbpData = iter->second;
            if(ncbpData->achLevel == lvl && ncbpData->type != 1)
            {
                m_players[lvl-1].push_back(iter->first);
                break;
            }
        }
        ++ iter;
    }
}

void NewCountryBattle::makePairPlayers()
{
    m_pairPlayer.clear();
    m_joinByePlayer.clear();
    for(UInt8 lvl = MAX_ACHLEVEL; lvl > 0; --lvl)
    {
        Player * pl1 = NULL;
        Player * pl2 = NULL;
        while(m_players[lvl-1].size())
        {
            UInt32 count = m_players[lvl-1].size();
            if(count <= 0) break;
            if(count == 1)
            {
                pl1 = m_players[lvl-1][0];
                m_players[lvl-1].clear();
                if(lvl > 1)
                    m_players[lvl-2].push_back(pl1);
                else
                    m_joinByePlayer.push_back(pl1);
                break;
            }
            UInt32 rand = uRand(count);
            pl1 = m_players[lvl-1][rand];
            m_players[lvl-1].erase(m_players[lvl-1].begin() + rand);
            
            count = m_players[lvl-1].size();
            rand = uRand(count);
            pl2 = m_players[lvl-1][rand];
            m_players[lvl-1].erase(m_players[lvl-1].begin() + rand);
            m_pairPlayer.insert(std::make_pair(pl1, pl2));
        }
    }
}

Player * NewCountryBattle::findPairPlayer(Player * player)
{
    if(!player)
        return NULL;
    std::map<Player *, Player *>::iterator iter = m_pairPlayer.find(player);
    if(iter != m_pairPlayer.end())
        return iter->second;
    else
    {
        std::map<Player *, Player *>::iterator iter = m_pairPlayer.begin();
        while(iter != m_pairPlayer.end())
        {
            if(iter->second == player) 
                return iter->first;
            ++iter;
        }
    }
    return NULL;
}

void NewCountryBattle::joinBye(Player * player)
{   //轮空
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    if(iter == m_ncbpData.end())
        return;
    NewCBPlayerData * ncbpData = iter->second;
    ncbpData->type = 2;
    ncbpData->setAchievementLevel(8);
    //m_ncbpData[player] = ncbpData;
    sendWinerInfo(player, NULL, 0, 8);
    sendSelfInfo1(player, NULL, 0, 8);
    addAchievement(player, 8);
    completeEffort1(ncbpData);
}

void NewCountryBattle::process(UInt32 curtime)
{
    UInt64 tick = TimeUtil::GetTick();
    UInt32 startTime = globalCountryBattle.getStartTime();
    if(startTime == 0)
        return;
    if(m_expTime)
        checkAddExp(curtime);
    UInt8 corr = (curtime - startTime) / NCBATTLE_TIME;
    if(!m_expTime)
        m_expTime = startTime + corr * NCBATTLE_TIME + 60;
    if(!m_tickTime)
    {   
        m_tickTime = startTime + (corr + 1) * NCBATTLE_TIME;
        //开始时配对
        allotPlayers();
        makePairPlayers();
        sendAllInfo();
    }
    if(curtime < m_tickTime)
        return;
    m_tickTime += NCBATTLE_TIME;

    handleBattle();     //战斗
    TRACE_LOG("NewCountryBattle=>>所有战斗开始结束时间差:[%"I64_FMT"u]", TimeUtil::GetTick() - tick);
    if(curtime >= globalCountryBattle.getEndTime())
    {   //策划要求这里调结束
        m_pairPlayer.clear();
        updateFirst();
        sendAllInfo();
        end();
        return;
    }
    tick = TimeUtil::GetTick();
    allotPlayers();     //重新分配玩家
    makePairPlayers();  //配对玩家
    updateFirst();      //广播荣誉王连胜王
    sendAllInfo();      //发送所有玩家的个人数据
    TRACE_LOG("NewCountryBattle=>>配对所有玩家开始结束时间差:[%"I64_FMT"u]", TimeUtil::GetTick() - tick);
}

void NewCountryBattle::prepare(UInt16 rt)
{
    m_expTime = 0;
    m_tickTime = 0;
    m_topKiller = NULL;
    m_topStreak = 0;
    m_achieveKing = NULL;
    m_maxAchievement = 0;
    m_ncbpData.clear();
    m_pairPlayer.clear();
    m_joinByePlayer.clear();
    for(UInt8 lvl = MAX_ACHLEVEL; lvl > 0; --lvl)
    {
        m_players[lvl-1].clear();
    }
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x05);
    st << static_cast<UInt8>(0) << rt << Stream::eos;
    NETWORK()->Broadcast(st);
}

void NewCountryBattle::start(UInt16 rt)
{
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x05);
    st << static_cast<UInt8>(1) << rt << Stream::eos;
    NETWORK()->Broadcast(st);
}

void NewCountryBattle::end()
{
    SYSMSG(title, 268);
    for(NCBPlayerData::iterator iter = m_ncbpData.begin(); iter != m_ncbpData.end(); ++ iter)
    {
        Player * player = iter->first;
        if(!player) continue;
        NewCBPlayerData * ncbpData = iter->second;
        UInt8 achLvl = ncbpData->achLevel;
        if(World::_activityStage > 0)
            GameAction()->onCountryBattleAttend(player);
        UInt16 count = 10 * achLvl;
        UInt8 cnt = 0;
        UInt8 award = 0;
        std::map<UInt8, NCB_EffortStruct>::iterator it = ncbpData->effortInfo.begin();
        while(it != ncbpData->effortInfo.end())
        {
            if(it->second.complete)
            {
                award += it->second.award;
                ++ cnt;
            }
            ++ it;
        }
        Table items = GameAction()->GetNewCountryBattleAward(achLvl);
        UInt8 size = items.size();
        MailPackage::MailItem * mitem = new MailPackage::MailItem[size];
        for(UInt8 i = 0; i < size; ++i)
        {
            Table item = items.get<Table>(i+1);
            if(item.size() < 2) continue;
            mitem[i].id = item.get<UInt16>(1);
            mitem[i].count = item.get<UInt32>(2);
        }
        SYSMSGV(content, 269, ncbpData->totalWin, ncbpData->totallose, ncbpData->maxKillStreak, cnt, award, ncbpData->totalAchievement, achLvl, count);
        MailItemsInfo itemsInfo(mitem, NewCountryBattleAward, size);
        Mail * pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        if(pmail != NULL)
            mailPackageManager.push(pmail->id, mitem, size, true);
        delete [] mitem;
        mitem = NULL;
        addAchievement(player, count);
        playerLeave(player);
        sendEndAwardInfo(player, items);

        player->countryBattleUdpLog(1091, player->getCountry());
        player->countryBattleUdpLog(1217, 1, Itoa(ncbpData->totalWin)+","+Itoa(ncbpData->totallose)+","+Itoa(ncbpData->totalAchievement + count));
    }
	
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x05);
    st << static_cast<UInt8>(2) << static_cast<UInt16>(0) << Stream::eos;
    NETWORK()->Broadcast(st);
}

void NewCountryBattle::sendEndAwardInfo(Player * player, Table items)
{
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x06);
    size_t pos = st.size();
    st << static_cast<UInt8>(0);
    UInt8 size = items.size();
    for(UInt8 i = 1; i <= size; ++i)
    {
        Table item = items.get<Table>(i);
        if(item.size() < 2) continue;
        st << item.get<UInt16>(1);
        st << item.get<UInt8>(2);
    }
    if(m_topKiller == player)
    {
        size += 1;
        st << static_cast<UInt16>(9311) << static_cast<UInt8>(1);
        SYSMSG_BROADCASTV(294, m_topKiller->getCountry(), m_topKiller->getName().c_str(), m_topStreak);
        MailPackage::MailItem mitem = {9311, 1};
        m_topKiller->sendMailItem(291, 292, &mitem, 1, true);
    }
    if(m_achieveKing == player)
    {
        size += 1;
        st << static_cast<UInt16>(9312) << static_cast<UInt8>(1);
        SYSMSG_BROADCASTV(293, m_achieveKing->getCountry(), m_achieveKing->getName().c_str(), m_maxAchievement);
        MailPackage::MailItem mitem = {9312, 1};
        m_achieveKing->sendMailItem(289, 290, &mitem, 1, true);
    }
    st.data<UInt8>(pos) = size;
    st << Stream::eos;
    player->send(st);
}

void NewCountryBattle::sendAllInfo()
{
    NCBPlayerData::iterator iter = m_ncbpData.begin();
    while(iter != m_ncbpData.end())
    {
        sendSelfInfo(iter->first, iter->second);
        ++ iter;
    }
}

void NewCountryBattle::sendSelfInfo(Player * player)
{
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    if(iter != m_ncbpData.end() && iter->second->type != 1)
        sendSelfInfo(player, iter->second);
    else
    {   //与客户端约定的
        Stream st(REP::NEW_CAMPS_WAR_JOIN);
        st << static_cast<UInt8>(0x01);
        st << Stream::eos;
        player->send(st);
    }
}

void NewCountryBattle::sendSelfInfo(Player * player, NewCBPlayerData * ncbpData)
{   //更新个人数据(下一轮)
    if(ncbpData->type == 1) //已退出玩家
        return;
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x01);
    st << ncbpData->currKillStreak << ncbpData->maxKillStreak;
    st << ncbpData->totalWin << ncbpData->totallose;
    st << ncbpData->totalAchievement;
    st << ncbpData->domineer << ncbpData->anger;
    st << ncbpData->skillFlags;
    st << static_cast<UInt8>(ncbpData->effortInfo.size()); //成就位数量
    std::map<UInt8, NCB_EffortStruct>::iterator iter = ncbpData->effortInfo.begin();
    while(iter != ncbpData->effortInfo.end())
    {
        NCB_EffortStruct info = iter->second;
        st << info.effortId << info.award << info.complete;
        ++ iter;
    }
    UInt8 round = 0;
    UInt16 timeLeft = 0;
    UInt32 curtime = TimeUtil::Now();
    UInt32 startTime = globalCountryBattle.getStartTime();
    if(m_tickTime)
    {
        round = (m_tickTime - startTime) / NCBATTLE_TIME;
        timeLeft = m_tickTime - curtime;
    }
    else
    {
        if(curtime > startTime)  //预防宕机,纠正时间
        {
            round = (curtime - startTime) / NCBATTLE_TIME;
            timeLeft = (round + 1) * NCBATTLE_TIME + startTime - curtime; 
        }
        else
            timeLeft = startTime - curtime;
    }
    //st << static_cast<UInt8>((m_tickTime - globalCountryBattle.getStartTime()) / NCBATTLE_TIME);
    //st << static_cast<UInt16>((m_tickTime > 0 ? m_tickTime : globalCountryBattle.getStartTime()) - TimeUtil::Now());
    st << round << timeLeft;
    st << player->getBattleMaxHp();
    st << player->getBattleCurrentHp();
    st << player->getBattlePoint();
    Player * pl = findPairPlayer(player);
    bool hasPair = true;
    if(pl)
    {
        NCBPlayerData::iterator iter = m_ncbpData.find(pl);
        if(iter != m_ncbpData.end() && iter->second->type != 1)
        {
            st << iter->second->totalAchievement << iter->second->currKillStreak;
            st << static_cast<UInt8>(pl->getMainFighter() ? pl->getMainFighter()->getId() : 0);
            st << pl->getName();
            st << pl->getBattleMaxHp();
            st << pl->getBattleCurrentHp();
            st << pl->getBattlePoint();
        }
        else
            hasPair = false;
    }
    if(!pl || !hasPair) //轮空
    {
        st << static_cast<UInt16>(0) << static_cast<UInt8>(0);
        st << static_cast<UInt8>(0) << "" << static_cast<UInt32>(0);
        st << static_cast<UInt32>(0) << static_cast<UInt32>(0);
    }
    st << Stream::eos;
    player->send(st);
}

void NewCountryBattle::broadcast( Stream& st )
{
	if(m_map != NULL)
		m_map->Broadcast(m_spot, st);
}

void NewCountryBattle::updateFirst()
{   //广播连胜王与荣誉王
    NCBPlayerData::iterator iter = m_ncbpData.begin();
    for(; iter != m_ncbpData.end(); ++ iter)
    {
        if(iter->second->totalAchievement > m_maxAchievement)
        {
            m_achieveKing = iter->first;
            m_maxAchievement = iter->second->totalAchievement;
        }
        if(iter->second->currKillStreak > m_topStreak)
        {
            m_topKiller = iter->first;
            m_topStreak = iter->second->currKillStreak;
        }
    }
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x03);
    if(m_topKiller)
        st << m_topKiller->getName() << m_topStreak;
    else
        st << "" << static_cast<UInt8>(0);
    if(m_achieveKing)
        st << m_achieveKing->getName() << m_maxAchievement;
    else
        st << "" << static_cast<UInt16>(0);
    st << Stream::eos;
    broadcast(st);
}

void NewCountryBattle::sendWinerInfo(Player * player1, Player * player2, UInt8 killStreak, UInt8 achieve)
{   //广播战报
    if(!player1) return;
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x02);
    st << player1->getName() << player1->getCountry();
    if(player2)
        st << player2->getName() << player2->getCountry();
    else
        st << "" << static_cast<UInt8>(2);
    st << killStreak << achieve;
    st << Stream::eos;
    broadcast(st);
}

void NewCountryBattle::sendSelfInfo1(Player * player1, Player * player2, UInt8 killStreak, UInt8 achieve)
{   //单播给个人
    if(!player1) return;
    Stream st(REP::NEW_CAMPS_WAR_JOIN);
    st << static_cast<UInt8>(0x07);
    st << player1->getName() << player1->getCountry();
    if(player2)
        st << player2->getName() << player2->getCountry();
    else
        st << "" << static_cast<UInt8>(2);
    st << killStreak << achieve;
    st << Stream::eos;
    if(killStreak)
        player1->send(st);
    else if(!player2)
        player1->send(st);
    else
        player2->send(st);
}

bool NewCountryBattle::isRunAway(NewCBPlayerData * ncbpData1, NewCBPlayerData * ncbpData2)
{
    if(ncbpData1->type == 1 && ncbpData2->type == 1)
        return true;
    UInt8 achLvl1 = ncbpData1->achLevel;
    UInt8 kills1 = ncbpData1->currKillStreak;
    UInt8 achLvl2 = ncbpData2->achLevel;
    UInt8 kills2 = ncbpData2->currKillStreak;
    UInt16 achieve = 0;
    Player * player1 = ncbpData1->player;
    Player * player2 = ncbpData2->player;
    if(ncbpData1->type == 1)
    {   //玩家1逃跑,玩家2胜利
        Stream st(REP::NEW_CAMPS_WAR_JOIN);
        st << static_cast<UInt8>(0x04);
        st << player1->getCountry();
        st << Stream::eos;
        player2->send(st);
       
        ncbpData2->currKillStreak ++;
        ncbpData2->totalWin ++;
        if(ncbpData2->currKillStreak > ncbpData2->maxKillStreak)
            ncbpData2->maxKillStreak = ncbpData2->currKillStreak;
        kills2 = ncbpData2->currKillStreak > 10 ? 10 : ncbpData2->currKillStreak;
        achieve = COUNT_WINACHIEVE(kills1, kills2, achLvl1, achLvl2);
        ncbpData2->setAchievementLevel(achieve);
        ncbpData2->addAngerDomineer(0, 5);
        ncbpData2->skillFlags = 0;
        player2->clearHIAttr();
        addAchievement(player2, achieve);
        completeEffort(ncbpData2, ncbpData1);
        return true;
    }
    if(ncbpData2->type == 1)
    {   //玩家2逃跑,玩家1胜利
        Stream st(REP::NEW_CAMPS_WAR_JOIN);
        st << static_cast<UInt8>(0x04);
        st << player2->getCountry();
        st << Stream::eos;
        player1->send(st);
        
        ncbpData1->currKillStreak ++;
        ncbpData1->totalWin ++;
        if(ncbpData1->currKillStreak > ncbpData1->maxKillStreak)
            ncbpData1->maxKillStreak = ncbpData1->currKillStreak;
        kills1 = ncbpData1->currKillStreak > 10 ? 10 : ncbpData1->currKillStreak;
        achieve = COUNT_WINACHIEVE(kills1, kills2, achLvl1, achLvl2);
        ncbpData1->setAchievementLevel(achieve);
        ncbpData1->addAngerDomineer(0, 5);
        ncbpData1->skillFlags = 0;
        player1->clearHIAttr();
        addAchievement(player1, achieve);
        completeEffort(ncbpData1, ncbpData2);
        return true;
    }
    return false;
}

void NewCountryBattle::handleBattle()
{
    if(m_joinByePlayer.size())  //轮空
        joinBye(m_joinByePlayer[0]);
    UInt32 size = m_pairPlayer.size();
    UInt32 count = size / 10;
    UInt32 rand = uRand(count);
    UInt32 index = 0;
    UInt32 j = 0;
    TRACE_LOG("NewCountryBattle=>>size蜀山论剑该轮次的玩家总数量:[%u]", size * 2 + m_joinByePlayer.size());
    for(std::map<Player *, Player *>::iterator iter = m_pairPlayer.begin(); iter != m_pairPlayer.end(); ++iter, ++j)
    {
        Player * player1 = iter->first;
        Player * player2 = iter->second;
        NewCBPlayerData * ncbpData1 = NULL;
        NewCBPlayerData * ncbpData2 = NULL;
        NCBPlayerData::iterator iter1 = m_ncbpData.find(player1);
        if(iter1 != m_ncbpData.end())
            ncbpData1 = iter1->second;
        NCBPlayerData::iterator iter2 = m_ncbpData.find(player2);
        if(iter2 != m_ncbpData.end())
            ncbpData2 = iter2->second;
        if(!ncbpData1 || !ncbpData2)
            continue;
        if(isRunAway(ncbpData1, ncbpData2))
            continue;

        UInt32 currHp1 = player1->getBattleCurrentHp();
        UInt32 currHp2 = player2->getBattleCurrentHp();

        player1->setHiAttrFlag(true);
        player2->setHiAttrFlag(true);
        bool res = player1->challenge(player2, NULL, NULL, false, 0, true, Battle::BS_NEWCOUNTRYBATTLE);
        player1->setHiAttrFlag(false);
        player2->setHiAttrFlag(false);
        GameAction()->RunOperationTaskAction1(player1, 2, res);
        GameAction()->RunOperationTaskAction1(player2, 2, !res);

        UInt8 achLvl1 = ncbpData1->achLevel;
        UInt8 kills1 = ncbpData1->currKillStreak;
        UInt8 achLvl2 = ncbpData2->achLevel;
        UInt8 kills2 = ncbpData2->currKillStreak;
        UInt16 achieve = 0;
        UInt16 loserAchieve = COUNT_LOSEACHIEVE(achLvl1, achLvl2);

        UInt32 thisDay = TimeUtil::SharpDay();
        UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(player1, created));
        if(thisDay <= endDay)
        {
            UInt32 targetVal = player1->GetVar(VAR_CLAWARD2);
            if (!(targetVal & TARGET_COUNTRY_BATTLE))
            {
                targetVal |=TARGET_COUNTRY_BATTLE;
                player1->AddVar(VAR_CTS_TARGET_COUNT, 1);
                player1->SetVar(VAR_CLAWARD2, targetVal);
                player1->sendNewRC7DayTarget();
                player1->newRC7DayUdpLog(1152, 4);
            }
        }

        endDay = TimeUtil::SharpDay(6, PLAYER_DATA(player2, created));
        if(thisDay <= endDay)
        {
            UInt32 targetVal = player2->GetVar(VAR_CLAWARD2);
            if (!(targetVal & TARGET_COUNTRY_BATTLE))
            {
                targetVal |=TARGET_COUNTRY_BATTLE;
                player2->AddVar(VAR_CTS_TARGET_COUNT, 1);
                player2->SetVar(VAR_CLAWARD2, targetVal);
                player2->sendNewRC7DayTarget();
                player2->newRC7DayUdpLog(1152, 4);
            }
        }

        if(res)
        {
            ncbpData1->currKillStreak ++;
            ncbpData1->totalWin ++;
            kills1 = ncbpData1->currKillStreak > 10 ? 10 : ncbpData1->currKillStreak;
            achieve = COUNT_WINACHIEVE(kills1, kills2, achLvl1, achLvl2);
            ncbpData1->setAchievementLevel(achieve);
            if(ncbpData1->type != 1)
                ncbpData1->type = 3;
            ncbpData1->addAngerDomineer(0, 5);
            ncbpData1->skillFlags = 0;
            if(ncbpData1->currKillStreak > ncbpData1->maxKillStreak)
                ncbpData1->maxKillStreak = ncbpData1->currKillStreak;
            /*
            if(ncbpData1->currKillStreak > m_topStreak)
            {
                m_topKiller = player1;
                m_topStreak = ncbpData1->currKillStreak;
            }
            if(ncbpData1->totalAchievement > m_maxAchievement)
            {
                m_achieveKing = player1;
                m_maxAchievement = ncbpData1->totalAchievement;
            }
            */
            if(currHp1 < currHp2)
            {   //击败一位生命高于自己的对手
                if(UInt8 award = ncbpData1->updateEffortInfo(6))
                {
                    addAchievement(player1, award);
                    ncbpData1->setAchievementLevel(award);
                }
            }
            completeEffort(ncbpData1, ncbpData2);
            ncbpData2->currKillStreak = 0;
            ncbpData2->totallose ++;
            ncbpData2->setAchievementLevel(loserAchieve);
            if(ncbpData2->type != 1)
                ncbpData2->type = 3;
            ncbpData2->addAngerDomineer(1, 5);
            ncbpData2->skillFlags = 0;

            completeEffort1(ncbpData2);
            if(player1->isOnline())
                player1->pendAchievement(achieve);
            else
                player1->getAchievement(achieve);
            if(player2->isOnline())
                player2->pendAchievement(loserAchieve);
            else
                player2->getAchievement(loserAchieve);
            
            player1->clearHIAttr();
            player2->clearHIAttr();
            player2->autoRegenAll();
            if(ncbpData1->currKillStreak >= 10 && World::_actAvailable && gSpecialAward.getAwardSize(0) < 3)
                gSpecialAward.AddSpecialAwardList(0, player1);

            player1->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 1);
            player1->OnShuoShuo(SS_CTRYBATTLE);
            if (ncbpData1->currKillStreak == 2)
                player1->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 2);
            sendSelfInfo1(player1, player2, ncbpData1->currKillStreak, achieve);
            sendSelfInfo1(player1, player2, 0, loserAchieve);
            if(size < 20)
                sendWinerInfo(player1, player2, ncbpData1->currKillStreak, achieve);
            else if(j == index + rand)
            {
                index += count;
                sendWinerInfo(player1, player2, ncbpData1->currKillStreak, achieve);
            }
        }
        else
        {
            ncbpData2->currKillStreak ++;
            ncbpData2->totalWin ++;
            kills2 = ncbpData2->currKillStreak > 10 ? 10 : ncbpData2->currKillStreak;
            achieve = COUNT_WINACHIEVE(kills1, kills2, achLvl1, achLvl2);
            ncbpData2->setAchievementLevel(achieve);
            if(ncbpData2->type != 1)
                ncbpData2->type = 3;
            ncbpData2->addAngerDomineer(0, 5);
            ncbpData2->skillFlags = 0;
            if(ncbpData2->currKillStreak > ncbpData2->maxKillStreak)
                ncbpData2->maxKillStreak = ncbpData2->currKillStreak;
            /*
            if(ncbpData2->currKillStreak > m_topStreak)
            {
                m_topKiller = player2;
                m_topStreak = ncbpData2->currKillStreak;
            }
            if(ncbpData2->totalAchievement > m_maxAchievement)
            {
                m_achieveKing = player2;
                m_maxAchievement = ncbpData2->totalAchievement;
            }
            */

            if(currHp2 < currHp1)
            {   //击败一位生命高于自己的对手
                if(UInt8 award = ncbpData2->updateEffortInfo(6))
                {
                    addAchievement(player2, award);
                    ncbpData2->setAchievementLevel(award);
                }
            }
            completeEffort(ncbpData2, ncbpData1);
            ncbpData1->currKillStreak = 0;
            ncbpData1->totallose ++;
            ncbpData1->setAchievementLevel(loserAchieve);
            if(ncbpData1->type != 1)
                ncbpData1->type = 3;
            ncbpData1->addAngerDomineer(1, 5);
            ncbpData1->skillFlags = 0;
    
            completeEffort1(ncbpData1);
            if(player2->isOnline())
                player2->pendAchievement(achieve);
            else
                player2->getAchievement(achieve);
            if(player1->isOnline())
                player1->pendAchievement(loserAchieve);
            else
                player1->getAchievement(loserAchieve);

            player1->clearHIAttr();
            player2->clearHIAttr();
            player1->autoRegenAll();
            if(ncbpData2->currKillStreak >= 10 && World::_actAvailable && gSpecialAward.getAwardSize(0) < 3)
                gSpecialAward.AddSpecialAwardList(0, player2);

            player2->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 1);
            player2->OnShuoShuo(SS_CTRYBATTLE);
            if (ncbpData2->currKillStreak == 2)
                player2->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 2);
            sendSelfInfo1(player2, player1, ncbpData2->currKillStreak, achieve);
            sendSelfInfo1(player2, player1, 0, loserAchieve);
            if(size < 20)
                sendWinerInfo(player2, player1, ncbpData2->currKillStreak, achieve);
            else if(j == index + rand)
            {
                index += count;
                sendWinerInfo(player2, player1, ncbpData2->currKillStreak, achieve);
            }
        }
        //m_ncbpData[player1] = ncbpData1;
        //m_ncbpData[player2] = ncbpData2;
    }
}

void NewCountryBattle::completeEffort(NewCBPlayerData * ncbpData1, NewCBPlayerData * ncbpData2)
{
    Player * player = ncbpData1->player;
    UInt8 currKill = ncbpData1->currKillStreak;
    if(currKill >= 2)
    {
        UInt8 award = 0;
        award = ncbpData1->updateEffortInfo(1);
        if(currKill >= 3)
            award = ncbpData1->updateEffortInfo(17);
        if(currKill >= 4)
            award = ncbpData1->updateEffortInfo(21);
        if(currKill >= 6)
            award = ncbpData1->updateEffortInfo(25);
        if(currKill >= 10)
            award = ncbpData1->updateEffortInfo(28);
        if(award)
        {
            addAchievement(player, award);
            ncbpData1->setAchievementLevel(award);
        }
    }
    UInt8 totalWin = ncbpData1->totalWin;
    if(totalWin >= 3)
    {
        UInt8 award = 0;
        award = ncbpData1->updateEffortInfo(3);
        if(totalWin >= 10)
            award = ncbpData1->updateEffortInfo(19);
        if(totalWin >= 15)
            award = ncbpData1->updateEffortInfo(23);
        if(totalWin >= 20)
            award = ncbpData1->updateEffortInfo(26);
        if(totalWin >= 25)
            award = ncbpData1->updateEffortInfo(29);
        if(award)
        {
            addAchievement(player, award);
            ncbpData1->setAchievementLevel(award);
        }
            
    }
    UInt8 killOther = ncbpData2->currKillStreak;
    if(killOther >= 2)
    {
        UInt8 award = 0;
        award += ncbpData1->updateEffortInfo(2);   //终结2连杀
        if(killOther >= 3)
            award += ncbpData1->updateEffortInfo(18);  //终结3连杀
        if(killOther >= 4)
            award += ncbpData1->updateEffortInfo(22);  //终结4连杀
        if(award)
        {
            addAchievement(player, award);
            ncbpData1->setAchievementLevel(award);
        }
    }
    if(player->getBattlePoint() < ncbpData2->player->getBattlePoint())
    {
        if(UInt8 award = ncbpData1->updateEffortInfo(5))
        {   //击败一位战力高于自己的对手
            addAchievement(player, award);
            ncbpData1->setAchievementLevel(award);
        }
    }
    UInt8 achLevel1 = ncbpData1->achLevel;
    if(achLevel1 >= 2)
    {
        UInt8 award = 0;
        award += ncbpData1->updateEffortInfo(4);   //2级
        if(achLevel1 >= 4)
            award += ncbpData1->updateEffortInfo(20);  //4级
        if(achLevel1 >= 6)
            award += ncbpData1->updateEffortInfo(24);  //6级
        if(achLevel1 >= 8)
            award += ncbpData1->updateEffortInfo(27);  //8级
        if(achLevel1 >= 9)
            award += ncbpData1->updateEffortInfo(30);  //9级
        if(award)
        {
            addAchievement(player, award);
            ncbpData1->setAchievementLevel(award);
        }
    }
}

void NewCountryBattle::completeEffort1(NewCBPlayerData * ncbpData)
{
    //一直失败或者轮空的玩家
    UInt8 achLevel = ncbpData->achLevel;
    if(achLevel >= 2)
    {
        UInt8 award = 0;
        award += ncbpData->updateEffortInfo(4);   //2级
        if(achLevel >= 4)
            award += ncbpData->updateEffortInfo(20);  //4级
        if(achLevel >= 6)
            award += ncbpData->updateEffortInfo(24);  //6级
        if(achLevel >= 8)
            award += ncbpData->updateEffortInfo(27);  //8级
        if(achLevel >= 9)
            award += ncbpData->updateEffortInfo(30);  //9级
        if(award)
        {
            addAchievement(ncbpData->player, award);
            ncbpData->setAchievementLevel(award);
        }
    }
}

NewCBPlayerData * NewCountryBattle::findNCBData(Player * player)
{
    NCBPlayerData::iterator iter = m_ncbpData.find(player);
    while(iter != m_ncbpData.end())
    {
        return iter->second;
    }
    return NULL;
}

}
