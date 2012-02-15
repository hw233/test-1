#ifndef _DUNGEON_H_
#define _DUNGEON_H_

#include "GObject/GGlobalObjectManager.h"
#include "Common/Stream.h"
#include "GData/DungeonData.h"
#include "EventBase.h"

namespace GObject
{

class Player;
class Dungeon;

class EventDungeonAuto : public EventBase
{
public:
	EventDungeonAuto(Player * player, UInt32 interval, Dungeon * dg, bool won, UInt32 exp);

public:
	virtual UInt32 GetID() const { return EVENT_DUNGEONAUTO; }
	void Process(UInt32);
	void NotifyCancel() const;
	void Complete() const;
	inline Dungeon * GetDungeon() { return m_Dungeon; }

protected:
	Dungeon * m_Dungeon;
	bool m_Won;
	UInt32 m_TotalExp;
};

struct DungeonLevel
{
	std::set<Player *> singles;
};

class Dungeon: public GObjectBaseT<Dungeon, UInt8>
{
public:
	struct DungeonItemInfo
	{
		DungeonItemInfo(UInt16 id_, UInt8 count_): id(id_), count(count_) {}
		UInt16 id;
		UInt8 count;
	};
	struct DungeonPlayerInfo
	{
		DungeonPlayerInfo(): difficulty(0), level(0), count(0), totalCount(0), firstPass(0), counterEnd(0), justice(0), justice_roar(0)
		{}
		UInt8 difficulty;
		UInt8 level;
		UInt8 count;
		UInt16 totalCount;
		UInt32 firstPass;
		UInt32 counterEnd;
        UInt8 justice;
        UInt8 justice_roar;
		std::list<DungeonItemInfo> lootToday;
	};
	struct DungeonReportInfo
	{
		Player * player;
		UInt32 id;
	};
public:
	Dungeon(UInt8 id, const GData::DungeonData *);
	~Dungeon();
	void pushPlayer(Player *, UInt8, UInt8, UInt16, UInt32, UInt32, UInt8, UInt8);
	UInt8 playerEnter(Player *);
	UInt8 playerLeave(Player *);
	UInt8 playerBreak(Player *);
	UInt8 playerContinue(Player *);
	void playerJump(Player *, UInt8);
	void startChallenge(Player *);
	void sendAutoChallengeStart(Player *);
	void processAutoChallenge(Player *, UInt8, UInt32 *, UInt8 = 0);
	void cancelAutoChallengeNotify(Player *, UInt32);
	void completeAutoChallenge(Player *, UInt32, bool);
	void autoChallenge(Player *, UInt8 = 0);
	void pushChallenge(Player *, UInt32, bool);
	void sendDungeonInfo(Player *);
	void sendMyLootInfo(Player *);
	void buildInfo(Player *, Stream&);
	UInt32 getFirstPass(Player *);
	bool doAttack(Player *, UInt8);
	static void setMaxCount(UInt32 cnt);
	static void setPrice(UInt32 idx, UInt16 price);
	static UInt16 * getPrice(size_t& size);
	static void setExtraCount(UInt32 idx, UInt8 count);

    static UInt8 getMaxCount() { return _maxCount + (World::_wday==5?1:0); };
	static UInt8 getExtraCount(UInt32 viplvl) { return _extraCount[viplvl]; }

    void doJusticeRoar(Player*);
public:
	void pushEnterCountBS(UInt32 now);

private:
	void takeLoot(Player *, DungeonPlayerInfo&, UInt32&);
	bool doChallenge(Player *, DungeonPlayerInfo&, bool = true, UInt32 * = NULL);
	bool advanceLevel(Player *, DungeonPlayerInfo&, bool = false, UInt32 * = NULL, UInt32 = 0);
	void sendDungeonInfo(Player *, DungeonPlayerInfo&);
	void sendDungeonLevelData(Player *, DungeonPlayerInfo&);
	void enterLevel(Player *, UInt8);
	void leaveLevel(Player *, UInt8);
	void updateToDB(Player *, DungeonPlayerInfo&);
	void checkForTimeout(Player *, DungeonPlayerInfo&, bool);
	void broadcast(Stream&, Player * = NULL);
	inline static UInt8 getEnterCount() { return getMaxCount(); }
private:
	inline UInt8 getGemLeve3Chance(UInt8 id, UInt8 bossIndex)
	{
		const UInt8 chance[5][8] = {
			{2, 2, 5},
			{2, 2, 2, 5},
			{2, 2, 2, 2, 5, 5, 10, 10},
			{5, 5, 10, 15},
			{5, 5, 10, 15}
		};
		return chance[id - 1][bossIndex - 1];
	}
	inline UInt8 getLevelBossIndex(UInt8 id, UInt8 level)//id 的副本 level层是第n个boss层
	{
		const UInt8 bossIndex[5][44] = {
			{0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3},
			{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4},
			{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 7, 8},
			{0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4},
			{0, 0, 0, 0, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4}
		};
		return bossIndex[id - 1][level];
	}

private:
	const GData::DungeonData * _dungeon;
	std::map<Player *, DungeonPlayerInfo> _players;
	std::vector<DungeonLevel> _levels;
	struct DungeonLootInfo
	{
		DungeonLootInfo(Player * player_, UInt16 id_, UInt8 count_): player(player_), id(id_), count(count_) {}
		Player * player;
		UInt16 id;
		UInt8 count;
	};
	std::list<DungeonLootInfo> _recentLoots;
	std::vector<std::list<DungeonReportInfo> > _recentReports;
	static UInt8 _maxCount;
	static UInt8 _extraCount[16];
	static UInt16 _price[5];
	static size_t _priceCount;
};

extern GGlobalObjectManagerT<Dungeon, UInt8> dungeonManager;

}

#endif // _DUNGEON_H_
