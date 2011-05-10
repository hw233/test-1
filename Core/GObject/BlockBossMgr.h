#ifndef _BLOCKBOSS_H_
#define _BLOCKBOSS_H_

namespace GData
{
	class NpcGroup;
}
namespace GObject
{
class Player;

struct RankData
{
	Player *_player;
	UInt16 _bossLevel;
	UInt8 _playerLevel;
	RankData(Player *pl = NULL, UInt16 bossLevel = 21, UInt8 playerLevel = 0):_player(pl), _bossLevel(bossLevel), _playerLevel(playerLevel){}
};

struct RankCmp
{
	bool operator()(const RankData& rd1, const RankData&  rd2) const
	{
		if (rd1._bossLevel == rd2._bossLevel)
			return rd1._playerLevel > rd2._playerLevel;
		else
			return rd1._bossLevel > rd2._bossLevel;
			
	}

};

typedef std::multiset<RankData, RankCmp> Rank;
class BlockBossMgr
{
public:
	BlockBossMgr(){}
	~BlockBossMgr(){}

public:
	void resetPlayerRank(Player *pl, UInt16 bossLevel, UInt8 playerLevel, UInt16 couponCount, UInt16 itemId, UInt8 itemCount);
	void addPlayerRank(Player *pl, UInt16 bossLevel, UInt8 playerLevel);
	void reqBlockBossData(Player *pl, UInt16 bossLevel);
	
private:
	Rank::iterator find(Player *pl);

private:
	Rank _playerRank;
};

extern BlockBossMgr gBlockbossmgr;

}

#endif //_BLOCKBOSS_H_