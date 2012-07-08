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
	UInt8 type;
	UInt8 otherHeroId;
	std::string otherName;
	UInt32 battleTime;
	UInt32 battleId;
};

struct BetInfo
{
	UInt8 state;
	UInt8 round;
	UInt8 group;
	UInt8 recieved;
	UInt16 pos;
	UInt8 type;
};

struct ArenaPlayer
{
	ArenaPlayer(): group(0xFF) {}
	UInt8 group;
	std::string realName;
	std::vector<PriliminaryBattle> battles[2];
	std::vector<BetInfo> betList[7][2];
};

struct EliminationPlayer
{
	UInt64 id;
	UInt8 level;
	UInt8 heroId;
    UInt32 battlePoint;
    UInt32 support;
	std::string name;
	std::map<Player *, UInt8> betMap;
	EliminationPlayer(): id(0), level(0), battlePoint(0), support(0)
	{}
    ~EliminationPlayer()
    {
        betMap.clear();
    }
	void calcBet(bool, const char *);
	void resetBet();
};

typedef EliminationPlayer PreliminaryPlayer;

struct EliminationBattle
{
    UInt8 wonFlag;
    std::vector<UInt32> battleId;
	UInt8 winner(UInt8 idx);
	UInt8 winCount(UInt8 idx);
	EliminationBattle() {}
	inline void reset()
	{
		battleId.clear();
	}
};

typedef std::list<PreliminaryPlayer> PreliminaryPlayerList;
typedef PreliminaryPlayerList::iterator PreliminaryPlayerListIterator;
typedef std::map<UInt64, PreliminaryPlayerListIterator> PreliminaryPlayerListMap;

class Arena
{
public:
	Arena();

	inline bool active() { return _loaded && (_session != 0x8001 || (_progress != 0)); }
	inline void getPlayerCount(UInt32 * pc) { pc[0] = _playerCount[0]; pc[1] = _playerCount[1]; pc[2] = _playerCount[2]; }

	static void enterArena(Player * player);
	void commitLineup(Player * player);
    UInt8 bet( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type );
	void readFrom(BinaryReader&);
	void sendInfo(Player * player);
	void sendElimination(Player * player, UInt8, UInt8);
	void push(Player * player, UInt8, const std::string&);
	void pushPriliminary(BinaryReader& br);
    void pushBetFromDB( Player * player, UInt8 round, UInt8 state, UInt8 group, UInt8 recieved, UInt16 pos, UInt8 type );
	void pushPriliminaryCount(UInt32 *);
	void check();

    void readPlayers(BinaryReader& brd);
    void readPrePlayers(BinaryReader& brd);
    void readHistories(BinaryReader& brd);
    void readElimination(BinaryReader& brd);
    void calcFinalBet(int i);
    void calcBet(PreliminaryPlayer& pp, UInt16 pos, UInt8 state, UInt8 group, bool won, const char * t);

    void sendActive(Player* pl);
    void sendStatus(Player* pl);
    void sendEnter(Player* pl);
    void sendPreliminary(Player* pl, UInt8 type, UInt8 flag);

    void updateSuport(UInt8 type, UInt8 flag, UInt16 pos);
    void updateBattlePoint(BinaryReader& brd);

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
    EliminationPlayer _finals[2][32];
	UInt64 _finalIdx[2][6][32];
	EliminationBattle _finalBattles[2][31];
    PreliminaryPlayerListMap _preliminaryPlayers[2];
    PreliminaryPlayerList _preliminaryPlayers_list[2];
};

extern Arena arena;

}

#endif // _ARENA_H_
