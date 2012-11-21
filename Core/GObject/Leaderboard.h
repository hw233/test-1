#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_

#include "Common/Stream.h"
#include "Common/AtomicVal.h"
#include "Common/Mutex.h"

namespace GObject
{

class Player;

struct LeaderboardTowndown
{
    UInt64 id;
	std::string name;
    UInt16 level;
    UInt32 time;
};

struct LeaderboardClanCopy
{
    UInt64 id;
	std::string name;
    UInt32 level;
    UInt64 time;
};

struct RankingInfoList
{
    UInt64 id;
	std::string name;
    UInt32 country;
    std::string clanName;
    UInt32 roleLevel;
    UInt64 value;
};
struct TownRankingInfoList
{
    UInt64 id;
	std::string name;
    UInt32 country;
    std::string clanName;
    UInt32 roleLevel;
    UInt32 value; //锁妖塔层数
    UInt64 reachTime;
};
struct ClanCopyRankingInfoList
{
	std::string name;
    std::string leaderName;
    UInt32 level;
    UInt32 memberCount;
    UInt32 country;
    UInt32 value; //帮派副本层数 
    UInt64 reachTime;
};
struct ClanBattleRankingInfoList
{
	std::string name;
    std::string leaderName;
    UInt32 level;
    UInt32 memberCount;
    UInt32 country;
    UInt32 value; //帮派积分 
};


class Leaderboard
{
public:
	Leaderboard(): _id(0), _maxLevel(100), m_sorting(false) {}
	void update();
	bool hasUpdate(UInt32);
	bool getPacket(UInt8, Stream*&, Player* pl=NULL);
	void sendOwnRank(Player *, UInt32);
    void newDrawingGame(UInt32 nextday); //新人冲级赛
	inline UInt8 getMaxLevel() { return _maxLevel; }

    std::vector<RankingInfoList>* getLevelList() {return &_level;};
    std::vector<RankingInfoList>* getAthleticsList() {return &_athletics;};
    std::vector<RankingInfoList>* getAchievementList() {return &_achievement;};
    std::vector<TownRankingInfoList>& getTownList() {return _town;};
    std::vector<ClanCopyRankingInfoList>& getClanCopyList() {return _clanCopyInfo;};
    std::vector<ClanBattleRankingInfoList>& getClanBattleList() {return _clanBattleInfo;};

    const std::vector<LeaderboardTowndown>& getTowndown()
    {
        FastMutex::ScopedLock lk(_tmutex);
        return _towndown;
    };

    const std::vector<LeaderboardClanCopy>& getClanCopy()
    {
        FastMutex::ScopedLock lk(_cmutex);
        return _clancopy;
    };

    const std::vector<RankingInfoList>& getLevel()
    {
        FastMutex::ScopedLock lk(_lmutex);
        return _level;
    };

    void begin() { m_sorting = true; }
    void end() { m_sorting = false; }
    bool isSorting() const { return m_sorting; }
    void buildBattlePacket();
    int getMyRank(Player* pl, UInt8 type);
private:
	void doUpdate();
    void makeRankStream(Stream*& st, UInt8 type, Player* pl);

	Stream _levelStream;
	Stream _moneyStream;
	Stream _achievementStream;
	Stream _clanStream;
    Stream _battleStream;
	UInt32 _id;
	UInt8 _maxLevel;

	std::map<UInt64, UInt16> _levelRankWorld;
	std::map<UInt64, UInt16> _levelRankCountry[2];
	std::map<UInt64, UInt16> _moneyRankWorld;
	std::map<UInt64, UInt16> _moneyRankCountry[2];
	std::map<UInt64, UInt16> _achievementRankWorld;
	std::map<UInt64, UInt16> _achievementRankCountry[2];
	std::map<UInt32, UInt16> _clanRankWorld;
	std::map<UInt32, UInt16> _clanRankCountry[2];
    std::vector<UInt64> _levelRankWorld10;  //世界等级前十名
    std::multimap<int, Player*, std::greater<int> > _battleRankWorld;
    std::multimap<UInt64, Player*, std::greater<UInt64> > _expRankWorld;
    FastMutex _tmutex;
    std::vector<LeaderboardTowndown> _towndown;
    FastMutex _cmutex;
    std::vector<LeaderboardClanCopy> _clancopy;
    FastMutex _lmutex;
    std::vector<RankingInfoList> _level;
    std::vector<RankingInfoList> _athletics;
    std::vector<RankingInfoList> _achievement;
    std::vector<TownRankingInfoList> _town;
    std::vector<ClanCopyRankingInfoList> _clanCopyInfo;
    std::vector<ClanBattleRankingInfoList> _clanBattleInfo;

    AtomicVal<bool> m_sorting;

    std::map<UInt64, int> _playerLevelRank;
    std::map<UInt64, int> _playerAthleticsRank;
    std::map<UInt64, int> _playerClanRank;
    std::map<UInt64, int> _playerBattleRank;
    FastMutex _opMutex;
};

extern Leaderboard leaderboard;

}

#endif // _LEADERBOARD_H_
