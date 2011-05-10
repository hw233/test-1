#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_

#include "Common/Stream.h"
#include "Common/Mutex.h"

namespace GObject
{

class Player;

class Leaderboard
{
public:
	Leaderboard(): _id(0), _maxLevel(100) {}
	void update();
	bool hasUpdate(UInt32);
	bool getPacket(UInt8, Stream*&);
	void sendOwnRank(Player *, UInt32);
	inline UInt8 getMaxLevel() { return _maxLevel; }
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
};

extern Leaderboard leaderboard;

}

#endif // _LEADERBOARD_H_
