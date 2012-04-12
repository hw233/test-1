#ifndef _ARENA_H_
#define _ARENA_H_

#include "Common/Stream.h"
#include "Common/BinaryReader.h"

namespace GObject
{

class Player;
class ItemEquip;

struct PriliminaryBattle
{
	UInt8 won;
	UInt8 otherColor;
	std::string otherName;
	UInt32 battleTime;
	UInt32 battleId;
};

struct BetInfo
{
	UInt8 round;
	UInt8 group;
	UInt8 pos;
	UInt8 tael;
};

struct ArenaPlayer
{
	ArenaPlayer(): group(0xFF) {}
	UInt8 group;
	std::string realName;
	std::vector<PriliminaryBattle> battles;
	std::vector<BetInfo> betList;
};

struct EliminationPlayer
{
	UInt64 id;
	UInt8 color;
	UInt8 level;
	UInt8 lastRank;
	std::string name;
	std::map<Player *, UInt8> betMap;
	EliminationPlayer(): id(0), color(0), level(0), lastRank(0)
	{}
	void calcBet(bool, const char *);
	void resetBet();
};

struct EliminationBattle
{
	UInt8 wonFlag;
	UInt32 battleId[3];
	UInt8 winner();
	UInt8 winCount();
	EliminationBattle(): wonFlag(0)
	{
		battleId[0] = battleId[1] = battleId[2] = 0;
	}
	inline void reset()
	{
		wonFlag = 0;
		battleId[0] = battleId[1] = battleId[2] = 0;
	}
};

class Arena
{
public:
	Arena();

	inline bool active() { return _loaded && (_session != 0x8001 || _progress != 0); }
	inline void getPlayerCount(UInt32 * pc) { pc[0] = _playerCount[0]; pc[1] = _playerCount[1]; pc[2] = _playerCount[2]; }
	inline UInt32 getBetCount(UInt8 group, UInt8 pos) { if(group > 2 || pos > 31) return 0; return _finals[group][pos].betMap.size(); }

	static void enterArena(Player * player);
	static void commitLineup(Player * player, int);
	UInt8 bet(Player * player, UInt8, UInt8, UInt8);
	void readFrom(BinaryReader&);
	void sendInfo(Player * player);
	void sendElimination(Player * player, UInt8);
	void push(Player * player, UInt8, const std::string&);
	void pushPriliminary(Player * player, UInt8, UInt8, const std::string&, UInt32, UInt32, float);
	void pushBetFromDB(Player * player, UInt8, UInt8, UInt8, UInt8);
	void pushPriliminaryCount(UInt32 *);
	void check();

private:
	static void appendLineup( Stream& st, Player * player);
	static void appendEquipInfo( Stream& st, ItemEquip * equip );

	static void appendLineup2( Stream& st, Player * player);
private:
	struct RecordInfo
	{
		UInt8 lastRank;
		UInt16 session;
		UInt8 rank;
		RecordInfo(): lastRank(0), session(0), rank(0) {}
	};

private:
	bool _loaded;
	UInt8 _notified;
	UInt16 _session;
	UInt8 _winnerColor[3];
	std::string _winner[3];
	UInt8 _progress;
	UInt8 _status;
	UInt8 _round;
	UInt32 _nextTime;
	UInt32 _playerCount[3];
	std::map<UInt64, RecordInfo> _records;
	std::map<Player *, ArenaPlayer> _players;
	EliminationPlayer _finals[3][32];
	UInt8 _finalIdx[3][6][32];
	EliminationBattle _finalBattles[3][31];
};

extern Arena arena;

}

#endif // _ARENA_H_
