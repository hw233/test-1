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
    UInt32 ranking;
    UInt64 country;
};

class Leaderboard
{
public:
	Leaderboard(): _id(0), _maxLevel(100), m_sorting(false) {}
	void update();
	bool hasUpdate(UInt32);
	bool getPacket(UInt8, Stream*&);
	void sendOwnRank(Player *, UInt32);
    void newDrawingGame(UInt32 nextday); //新人冲级赛
	inline UInt8 getMaxLevel() { return _maxLevel; }

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
private:
	void doUpdate();

	Stream _levelStream;
	Stream _moneyStream;
	Stream _achievementStream;
	Stream _clanStream;
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

    FastMutex _tmutex;
    std::vector<LeaderboardTowndown> _towndown;
    FastMutex _cmutex;
    std::vector<LeaderboardClanCopy> _clancopy;
    FastMutex _lmutex;
    std::vector<RankingInfoList> _level;

    AtomicVal<bool> m_sorting;
};

extern Leaderboard leaderboard;

}

#endif // _LEADERBOARD_H_
