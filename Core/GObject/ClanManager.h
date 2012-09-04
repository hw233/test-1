#ifndef CLANMANAGER_INC
#define CLANMANAGER_INC

#include "Common/Stream.h"

namespace GObject
{

class Player;
class Clan;
class ClanBattle;
struct ClanBttleStatistics
{
	UInt16 *count;
	std::multimap<UInt32, Clan *, std::greater<UInt32> > *maxAchieveTable;
};


class ClanManager
{
public:
	ClanManager();
	~ClanManager();

public:
	void resumeRobClanBattleData();
	void resumeRobClanBattleScore();

public:
	static std::vector<UInt8>& getClanBattleTimeTable();
	static bool validClanBattleTime(UInt8);

public:
	void process(UInt32, UInt32);
	void reConfigClanBattle();
	//void allocRepo();
	void checkCBBroadcast(UInt32);
	//void updateAllocated();

public:
	void listClans(Player *, UInt16, UInt8);
	bool addOnBattleClan(ClanBattle *);
	bool addOffBattleClan(ClanBattle *);
	bool delBattleClan(ClanBattle *);
	ClanBattle * getRobBattleClan();
	ClanBattle * getBattleClan(Player *, std::string&);

private:
	bool getClanBattleTimePoints(UInt32);
	void makeBattleClanInfo(Player *, Stream&, ClanBattle *);
	UInt16 getOffBattleClanSize();
	void getOffBattleClanCheckPoint(UInt8 &, UInt8 *);

private:
	struct OnBCTKey
	{
		UInt8 _cbt;
		UInt8 _lev;

		OnBCTKey(UInt8 c = 0, UInt8 l = 0) : _cbt(c), _lev(l) {}
		bool operator < (const OnBCTKey& other) const
		{
			if (_cbt == other._cbt)
				return _lev > other._lev;
			return _cbt < other._cbt;
		}
	};
	typedef std::multimap<OnBCTKey, ClanBattle *> OnBattleClanT;
	typedef std::list<ClanBattle *> OffBattleClanT;

	OnBattleClanT _onBattleClans;
	OffBattleClanT _offBattleClans;

	//bool _hasAllocRepo;
	bool _firstBroadcast;
	UInt8 _hasBroadcast;
	ClanBattle * _clanBattle;
	ClanBattle * _clanRobBattle;
};

extern ClanManager clanManager;

}


#endif