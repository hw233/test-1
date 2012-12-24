#ifndef _NEWCOUNTRYBATTLE_H_
#define _NEWCOUNTRYBATTLE_H_

#include "Common/Stream.h"
#include "Map.h"
#include <map>
#include "Script/GameActionLua.h"

namespace GObject
{

class Player;

#define NEWCOUNTRYBATTLE_SKILL_NUM 10
#define MAX_ACHLEVEL 9

/**
 *@brief 新阵营战(蜀山论剑)技能
 */
struct NewCountryBattleSkill
{
    NewCountryBattleSkill()
        :id(0),price(0){}

    //id
    UInt8 id;
    //价格(霸气or怒气)
    UInt32 price;
    //成就id
    UInt8 effortId;
    //技能加成属性
    GData::AttrExtra  attrs;
};

/**
 *@brief 新阵营战(蜀山论剑)成就结构
 */
struct NCB_EffortStruct
{
    NCB_EffortStruct()
        :effortId(0), award(0), complete(0) {}
    //成就id
    UInt8 effortId;
    UInt8 award;
    UInt8 complete;
};

struct NewCBPlayerData
{
	NewCBPlayerData()
        :player(NULL), totalAchievement(0), achLevel(0), totalWin(0), totallose(0), maxKillStreak(0), currKillStreak(0), domineer(0), anger(0), skillFlags(0), type(0xFF) {}

    Player * player;
	UInt16 totalAchievement;
	UInt8 achLevel;
	UInt8 totalWin;
	UInt8 totallose;
	UInt8 maxKillStreak;
	UInt8 currKillStreak;
    UInt8 domineer; //霸气值
    UInt8 anger;   //怒气值
    UInt16 skillFlags; //技能域值
	UInt8 type; //0:进入 1:离开 2:轮空 3:战斗

    std::map<UInt8, NCB_EffortStruct> effortInfo;
    UInt8 updateEffortInfo(UInt8);
    void setAllEffortInfo();
    void setSkillFlag(UInt8 skillId) { skillFlags |= (1 << skillId); }
    bool canAddSkillFlag(UInt8);
    void addAngerDomineer(UInt8, UInt8);
    void setAchievementLevel(UInt8 achieve = 0);
};

/*
struct _rankAchievement
{
	bool operator() (const NewCBPlayerData * pd1, const NewCBPlayerData * pd2) const
	{
		if(pd1->totalAchievement == pd2->totalAchievement)
		{
			if(pd1->totalWin == pd2->totalWin)
			{
			    if(pd1->maxKillStreak == pd2->maxKillStreak)
                    return pd1->player != pd2->player;
			    return pd1->maxKillStreak > pd2->maxKillStreak;
			}
		    return pd1->totalWin > pd2->totalWin;
		}
		return pd1->totalAchievement > pd2->totalAchievement;
	}
};
*/

//typedef std::set<NewCBPlayerData *, _rankAchievement> NCBPlayerData;
typedef std::map<Player *, NewCBPlayerData *> NCBPlayerData;

class NewCountryBattle
{
public:
	//NewCountryBattle(UInt16 spot): m_map(NULL), m_spot(spot), m_expTime(0), m_tickTime(0), m_topKiller(NULL), m_topStreak(0), m_achieveKing(NULL), m_maxAchievement(0)
	//{ m_map = Map::FromSpot(m_spot); }
    NewCountryBattle(UInt16 spot);
    static void Init();
	inline UInt16 getLocation() { return m_spot; }
	void process(UInt32);
	void prepare(UInt16);
	void start(UInt16);
	void end();
	bool playerEnter(Player *);
	void playerLeave(Player *);
    void sendSelfInfo(Player *);
    void sendSelfInfo(Player *, NewCBPlayerData *);
    void allotPlayers();
    void makePairPlayers();
    void useSkill(Player *, UInt8);
    NewCBPlayerData * findNCBData(Player *);
private:
    void completeEffort(NewCBPlayerData *, NewCBPlayerData *);
    void completeEffort1(NewCBPlayerData *);
    void skillTriggerEffort(NewCBPlayerData *, UInt8);
    void playerEnterInSpecialTime(UInt32, Player *);
	void checkAddExp(UInt32);
    void addAchievement(Player *, UInt8);
    Player * findPairPlayer(Player *);
    void joinBye(Player *);
    void sendAllInfo();
    void sendEndAwardInfo(Player *, Table);
    void broadcast(Stream&);
    void updateFirst();
    void sendWinerInfo(Player *, Player *, UInt8, UInt8, UInt8);
    void handleBattle();
    bool isRunAway(NewCBPlayerData *, NewCBPlayerData *);
private:
    NCBPlayerData m_ncbpData;
    //新阵营战(蜀山论剑)技能
    static NewCountryBattleSkill m_skills[NEWCOUNTRYBATTLE_SKILL_NUM + 1];
    std::vector<Player *> m_players[MAX_ACHLEVEL];
    std::vector<Player *> m_joinByePlayer; //轮空队列,最多只会有一个对象
    std::map<Player *, Player *> m_pairPlayer;
    Map * m_map;
	UInt16 m_spot;
	UInt32 m_expTime;
	UInt32 m_tickTime;
	Player * m_topKiller;
	UInt8 m_topStreak;
	Player * m_achieveKing;
	UInt16 m_maxAchievement;
};

}

#endif // _NEWCOUNTRYBATTLE_H_
