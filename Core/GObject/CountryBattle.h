#ifndef _COUNTRYBATTLE_H_
#define _COUNTRYBATTLE_H_

#include "Common/Stream.h"
#include "Map.h"

namespace GObject
{

#define COUNTRY_BATTLE_BARRIER1 59
#define COUNTRY_BATTLE_BARRIER2 79

class Player;

struct CountryBattleData
{
	CountryBattleData(): resetEndTime(0), joinTime(0), restCountTime(0), killStreak(0) { }
	UInt32 getReward(UInt8, UInt32, UInt32);
	void sendAchieveUpdate(UInt16);
	Player * player;
	UInt32 resetEndTime;
	UInt32 joinTime;
	UInt32 restCountTime;
	UInt8 killStreak;
};

struct CBStatusData
{
	CBStatusData(): type(0xFF), lvlside(0), player(NULL), streak(0), score(0), playerOther(NULL), streakOther(0) {}
	UInt8 type;
	UInt8 lvlside;
	Player * player;
	UInt8 streak;
	UInt8 score;
	Player * playerOther;
	UInt8 streakOther;
	inline void setSideLevel(UInt8 side, UInt8 lvl) { lvlside = (side << 6) + lvl; }
};

typedef std::vector<CBStatusData> CBStatusDataList;

struct CBPlayerData
{
	UInt32 awardTime;
	UInt16 totalAchievement;
	UInt8 totalWin;
	UInt8 totallose;
	UInt8 maxKillStreak;
	UInt8 currKillStreak;
	UInt8 rank;
	bool firstEnter;

	CBPlayerData():awardTime(0), totalAchievement(0), totalWin(0), totallose(0), maxKillStreak(0), currKillStreak(0), rank(0), firstEnter(false) {}
};

class CountryBattle
{
public:
	CountryBattle(UInt16 spot): _firstBlooded(false), _owner(0), _map(NULL), _rewardTime(0), _spot(spot), _topKiller(NULL), _topStreak(0)
	{ _score[0] = 0; _score[1] = 0; _map = Map::FromSpot(_spot); }
	void process(UInt32);
	void prepare(UInt32);
	void start(UInt32);
	void end(UInt32);
	void getReward(UInt32);
	bool playerEnter(Player *);
	void playerLeave(Player *);
	inline UInt16 getLocation() { return _spot; }
	inline void setOwner(UInt8 o) { _owner = o; }
	inline UInt8 getOwner() { return _owner; }
	inline UInt16 getCount(UInt8 lvl, UInt8 side) { return static_cast<UInt16>(_players[lvl][side].size()); }
	inline UInt16 getCount(UInt8 side)
    {
        return static_cast<UInt16>(_players[0][side].size() + _players[1][side].size() + _players[2][side].size()); }
	inline UInt32 getScore(UInt8 side) { return _score[side]; }
	inline std::vector<CountryBattleData *>::iterator playerEnd(UInt8 lvl, UInt8 side) { return _players[lvl][side].end(); }
	void padPlayerData(Stream&, UInt8);
	void sendInfo(Player *);

private:
	int top(UInt32, UInt8 lvl, UInt8 side);
	int findPlayer(Player *, UInt8&, UInt8&);
	void doLeave(UInt32, UInt8, UInt8, int, UInt16 = 0, UInt8 = 0);
	void rotate(UInt32, UInt8, UInt8, int, UInt16 = 0, UInt8 = 0);
	void restCount(UInt32, UInt8, UInt8);
	void padPlayerData(Stream&, UInt32, UInt8, UInt8, bool);
	void padPlayerData(Stream&, CBStatusData&);
	void broadcast(Stream&);

    void LeaveGetAttainment(Player* p,CBPlayerData& data );
	static inline UInt8 getJoinLevel(UInt8 level)
    {
        if (level <= COUNTRY_BATTLE_BARRIER1)
            return 0;
        if (level <= COUNTRY_BATTLE_BARRIER2)
            return 1;
        return 2;
    }

private:
	std::vector<CountryBattleData *> _players[3][2];
	bool _firstBlooded;
	UInt8 _owner;
	Map * _map;
	UInt32 _rewardTime;
	UInt16 _spot;
	UInt32 _score[2];
	Player * _topKiller;
	UInt8 _topStreak;
	std::map<Player *, CBPlayerData> _battleDuration[COUNTRY_NEUTRAL];
	CBStatusDataList _cbsdlist;
	Stream _lastReport;
};

class GlobalCountryBattle
{
public:
	GlobalCountryBattle(): _running(false), _prepareTime(0), _startTime(0), _endTime(0), _countryBattle(NULL), _status(0) { }
	void prepare(UInt32);
	void prepare2(UInt32);
	bool process(UInt32);
	void start(UInt32);
	void end();
	inline bool isRunning() { return _running; }
	inline UInt32 getPrepareTime() { return _prepareTime; }
	inline UInt32 getStartTime() { return _startTime; }
	inline UInt32 getEndTime() { return _endTime; }
	inline void setCountryBattle(CountryBattle * cb) { _countryBattle = cb; }
	void addAutoCB(Player *);
	void delAutoCB(Player *);
    void sendDaily(Player*);
    inline void setStatus(UInt8 status) { _status = status; sendDaily(NULL); }

private:
	bool _running;
	UInt32 _prepareTime, _startTime, _endTime;
	CountryBattle * _countryBattle;
	std::set<Player *> _autoList;
    UInt8 _status;
};

extern GlobalCountryBattle globalCountryBattle;

}

#endif // _COUNTRYBATTLE_H_
