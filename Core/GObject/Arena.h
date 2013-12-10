#ifndef _ARENA_H_
#define _ARENA_H_

#include "Common/Stream.h"
#include "Common/BinaryReader.h"
#include "GObject/World.h"

namespace GObject
{

class Player;
class ItemEquip;
struct DBArenaBet;

enum STAGE_PROGRESS
{
    e_progress_sign = 0,
    e_progress_ruwei = 1,
    e_progress_32 = 2,
    e_progress_16 = 3,
    e_progress_8 = 4,
    e_progress_4 = 5,
    e_progress_2 = 6,
    e_progress_1 = 7,
    e_progress_sign_end = 8,
    e_progress_ruwei_end = 9,
    e_progress_32_end = 10,
    e_progress_end = 11,
    e_progress_nextbegin = 12,
};

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

struct PlayerLess
{
     bool operator()(const PreliminaryPlayerListIterator p1, const PreliminaryPlayerListIterator p2) const
     {
         return p1->battlePoint > p2->battlePoint;
     }
};
typedef std::multiset<PreliminaryPlayerListIterator, PlayerLess> PreliminaryPlayersSet;

struct LeaderPlayer
{
    LeaderPlayer() : level(0), battlePoint(0), heroId(0) {}
    std::string name;
    UInt8 level;
    UInt32 battlePoint;
    UInt8 heroId;
};


class Arena
{
public:
	Arena();

	inline bool active() { return _loaded && (_session != 0x8001 || (_progress != 0)) && World::getArenaState() == ARENA_XIANJIE_DIYI; }
    inline bool isOpen() { return World::getArenaState() == ARENA_XIANJIE_DIYI; }
    inline void setSession(UInt16 s) { _session = s; }
	inline void getPlayerCount(UInt32 * pc) { pc[0] = _playerCount[0]; pc[1] = _playerCount[1]; pc[2] = _playerCount[2]; }

	void enterArena(Player * player);
	void commitLineup(Player * player);
    //UInt8 bet( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type );
    UInt8 bet1( Player * player, UInt8 state, UInt8 group, UInt64 pid, UInt8 type );
    UInt8 bet2( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type );
	void readFrom(BinaryReader&);
	void sendInfo(Player * player);
	void sendElimination(Player * player, UInt8, UInt8);
	void push(Player * player, UInt8, const std::string&);
	void pushPreliminary(BinaryReader& br);
    void pushBetFromDB(Player * player, DBArenaBet& dbab);
	void pushPreliminaryCount(UInt32 *);
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
    void sendPreliminary(Player* player, UInt8 type, UInt8 flag, UInt16 start, UInt8 len);

    void updateSuport(UInt8 type, UInt8 flag, UInt16 pos);
    void updateBattlePoint(BinaryReader& brd);
    void commitArenaForceOnce();

    void updateLeaderBoard(BinaryReader& brd);
    void sendLeaderBoard(Player* pl);
    void sendLastLeaderBoard(Player* pl);
    bool hasLastLeaderBoard();
    void setArenaInfo(UInt8 type);
    //void setArenaTotalCnt(UInt8 type);
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
    PreliminaryPlayersSet _preliminaryPlayers_list_set[2];
    std::map<UInt16, std::vector<LeaderPlayer>> _leaderBoard;

public:
    Stream _readbuf;
};

extern Arena arena;

}

#endif // _ARENA_H_
