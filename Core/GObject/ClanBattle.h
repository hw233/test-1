#ifndef CLANBATTLE_INC
#define CLANBATTLE_INC

#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"

namespace GData
{
	class NpcGroup;
	struct ClanAssistant;
}

namespace GObject
{
 
class Clan;
class Player;
class ClanDynamicMsg;
struct ClanBattlePlayer
{
	ClanBattlePlayer(Player * player_ = NULL, UInt16 hold_ = 0, UInt8 status_ = 1, UInt8 side_ = 0)
		: player(player_), hold(hold_), status(status_), side(side_)
	{
		id = 0;
		reliveNum = 0;
		wins = 0;
		serailWins = maxSerailWins = 0;
		southEdurance = northEdurance = 0;
		grabAchieve = 0;
		offTime = 0;
		hasEnter = 0;
		hasAttack = 0;
	}
	Player * player;
	UInt32 id;
	UInt16 hold;
	UInt8  status;	
	UInt8  side;
	UInt16 reliveNum;
	UInt16 wins;
	UInt16 serailWins;	
	UInt16 maxSerailWins;
	UInt8  southEdurance;
	UInt8  northEdurance;	
	UInt32 grabAchieve;		//对于夺城战，采用积分制
	UInt32 offTime;		
	UInt8  hasEnter;
	UInt8  hasAttack;
};

struct ClanHoldMonster
{
	ClanHoldMonster(std::string n = "", UInt8 l = 0, UInt8 k = 0, UInt16 h = 0, UInt32 b = 0) 
		: name(n), level(l), klass(k), hold(h), buff(b), dirty(true)
	{
		hp.resize(25);
		maxHp.resize(25);
	}
	virtual ~ClanHoldMonster() {};
	inline void resetHp() { hp = maxHp; }
	inline UInt32 getHP(UInt8 i) { return i >= hp.size() ? 0 : hp[i]; }
	inline void setHp(UInt8 i, UInt32 h) { if (i < hp.size()) hp[i] = h; }
	inline void setMaxHp(UInt8 i, UInt32 h) { if (i < maxHp.size()) maxHp[i] = h; }
	
	std::string name;
	UInt8 level;
	UInt8 klass;
	UInt16 hold;
	UInt32 buff;
	bool dirty;
	std::vector<UInt32> hp;
	std::vector<UInt32> maxHp;
};

struct ClanCityHoldMonster : ClanHoldMonster
{
	ClanCityHoldMonster(std::string n = "", UInt8 l = 0, UInt8 k = 0, UInt16 h = 0, UInt32 b = 0, const GData::ClanAssistant * a = NULL)
		: ClanHoldMonster(n, l, k, h, b), assist(a) {  }
	~ClanCityHoldMonster() {}

	const GData::ClanAssistant * assist;
};

struct ClanRobHoldMonster : ClanHoldMonster
{
	ClanRobHoldMonster(std::string n = "", UInt8 l = 0, UInt8 k = 0, UInt16 h = 0, UInt32 b = 0, GData::NpcGroup * a = NULL)
		: ClanHoldMonster(n, l, k, h, b), assist(a)	{  }
	~ClanRobHoldMonster() {}

	GData::NpcGroup * assist;
};


struct ClanBattleHold
{
	typedef std::set<ClanBattlePlayer *>::iterator cbp_iter;

	UInt8 endurance, totalendurance;
	UInt16 hold;
	UInt32 buff;
	UInt16 holdCount[4];
	std::set<ClanBattlePlayer *> battlers;
	ClanHoldMonster * guarder;
	std::map<std::string, ClanHoldMonster *> assists;

	ClanBattleHold() : endurance(0), totalendurance(0), hold(0), buff(0), guarder(NULL) {}
	void addBattler(ClanBattlePlayer *);
	bool delBattler(ClanBattlePlayer *);
	bool delBattler(Player *);
	bool existBattler(Player *);
	bool existBattler(ClanBattlePlayer *);
	ClanBattlePlayer * getBattler(Player *);
	ClanBattlePlayer * getBattler(const std::string&);
	void changeBattlerStatus(ClanBattlePlayer *, UInt8, bool = true);	//在同一据点改变状态
	void clear();

	inline UInt16 size() { return static_cast<UInt16>(battlers.size()); }
	UInt16 atkerSize();/* { return holdCount[0]+holdCount[1]; }*/
	UInt16 deferSize();/* { return holdCount[2]+holdCount[3]; }*/
	UInt16 aliveDeferSize();
};

class ClanBattle
{
public:
	ClanBattle();
	virtual ~ClanBattle();

public:
	virtual void initClanBattle(bool = true) = 0;
	void disbandClanBattle();

public:
	inline void setBattleThisDay(UInt32 t) { _battleThisDay = t; }
	inline UInt32 getBattleThisDay() { return _battleThisDay; }
	inline void setBattleStatus(UInt8 s) { _isInbattling = s; }
	inline UInt8 getBattleStatus() { return _isInbattling; }
	inline void setOldBattleStatus(UInt8 s) { _isOldInBattling = s; }
	inline UInt8 getOldBattleStatus() { return _isOldInBattling; }
	inline void setSouthEdurance(UInt8 e) { _southEdurance = e; }
	inline UInt8 getSouthEdurance() { return _southEdurance; }
	inline void setNorthEdurance(UInt8 e) { _northEdurance = e; }
	inline UInt8 getNorthEdurance() { return _northEdurance; }
	inline void setHallEdurance(UInt8 e) { _hallEdurance = e; }
	inline UInt8 getHallEdurance() { return _hallEdurance; }
	inline UInt8 getEdurance(UInt16 h) { return h >= 3 ? 0 :_holds[h].endurance; }
	inline UInt8 getTotalEdurance(UInt16 h) { return h >= 3 ? 0 : _holds[h].totalendurance; }
	void setHasBattle(UInt8, bool = false);	//FIXME
	inline UInt8 getHasBattle() { return _hasBattle; }
	void setBattleTime(UInt8 r) { _battleTime = r; }
	void setNextBattleTime(UInt8 r) { _nextBattleTime = r; }
	virtual UInt8 getBattleTime() { return 0; }
	virtual bool setNextBattleTime(Player *, UInt8) { return true; }
	virtual UInt8 getNextBattleTime() { return 0; }
	virtual bool canClanBattleOpen() { return true; }
	void addOfflineBattler(Player *, UInt32 = TimeUtil::Now());
	inline UInt32 getGrabAchieve() { return _grabAchieve; }
	inline void setGrabAcheive(UInt32 ga) { _grabAchieve = ga; }
	virtual void incGrabAcheive(UInt16 ga) {}

public:
	virtual int testClanBattleStatus(UInt32, UInt32) { return 0; }
	void startClanBattle(UInt32 = TimeUtil::SharpDay(), bool = true);
	void overClanBattle(UInt8 = 1);
	inline bool isInBattling()  { return false; /*_isInbattling == 1;*/ }
	inline bool isInAttacking() { return _isInAttacking; }
	virtual void configClanBattleCheck() {}		
	virtual void configClanBattleData(bool = true);
	virtual bool resumeClanBattleData(UInt32) { return true; }
	bool resumePlayerToClanBattle(ClanBattlePlayer *, UInt32 = TimeUtil::Now());

public:
	virtual void update(UInt32) = 0;

public:
	virtual UInt8 getEnterBattleClanCount(Player *) { return 0; }
	virtual UInt8 getClanRelation(Clan *) { return 0; }
	UInt8 getClanRelation(Player *, Player *);
	UInt8 getClanMonsterRelation(Player *);
	
	virtual UInt32 getClanBattleOverTime() { return 0; }

public:
	UInt16 getTotalAtkerNum();
	UInt16 getTotalDeferNum();
	UInt16 getAtkerNum(UInt16);
	UInt16 getDeferNum(UInt16);
	UInt16 getBattleClanPlayerLocation(Player *);
	UInt8 getBattleClanPlayerSide(Player *);
	UInt8 getClanEndurance(UInt16);
	bool  isClanHoldDestroy(UInt16);
	UInt8 getTotalAliveClanBattlerCount() { return 0; }	//
	UInt8 getTotalClanBattlerCount() { return 0; }
	void  broadcastHold(Stream&);
	void  broadcastHold(UInt16, Stream&);
	void  delClanBattlePlayer(UInt16, UInt8, UInt8, Player *);
	void  addClanBattlePlayer(ClanBattlePlayer *);
	bool  moveToHold(Player *, UInt16);
	virtual UInt32 getBattleOverTime(UInt32 = TimeUtil::Now()) { return 0; }
	virtual void  clearClanBattle();
	void  kickClanBattler(Player *);
	virtual void closingBattlerAward(UInt8) {}
	virtual UInt16 getWillGainBattlerAward(Clan *) { return 0; }
	void  setClanBattlerStartData(bool = true);
	void  cancelClanBattlerOffCheck(Player *);
	bool  isDeadBattlePlayer(Player *);
	void  recoveBattlePlayer(Player *, UInt8);
	bool incAwardClanBattleVictor(Player *);
	bool setAwardClanBattleVictor(ClanBattlePlayer *, bool = true);
	ClanBattlePlayer * getBattlePlayer(Player *);

	virtual UInt8 getClanBattleType() { return 0; }

public:
	virtual bool enterTotem(Player *) = 0;
	virtual bool enterClanCity(Player *) = 0;
	bool leaveClanCity(Player *);

public:
	virtual bool attackPlayer(Player *, std::string) { return true; }
	virtual bool attackPlayer2(ClanBattlePlayer *, ClanBattlePlayer *) { return true; }
	virtual bool attackNpc(Player *, std::string) { return true; }
	virtual bool  attackAssist(Player *, std::string&, UInt32&,  UInt32&, ClanHoldMonster *);
	virtual bool  attackGuarder(Player *, std::string&, UInt32&, UInt32&, ClanHoldMonster *);

public:
	virtual void listClanHoldPlayerInfo(Player *, UInt16);
	void notifyClanBattleEnterInfo(Player *, UInt8, UInt8, std::string = "");
	void notifyClanBattlePlayerMoveInfo(Player *, UInt8, UInt16);
	void notifyClanBattlePlayerInfo(Player *, Player *, UInt8);
	void notifyClanBattleOverTime(Player * = NULL);
	void notifyClanBattlePlayerCount(Player * = NULL);
	void notifyClanBattleWinData(Player *);
	void notifyClanHoldEndurance(Player * = NULL);
	void notifyClanHoldEndurance(ClanBattleHold&);
	void notifyClanHoldPlayerInfo(Player *, UInt16, UInt8, UInt8);
	void notifyClanHoldPlayerInfo(Player *, UInt8);
	void notifyClanHoldAssistEnterInfo(std::string, UInt16, UInt8);
	void notifyClanHoldGuarderEnterInfo(UInt16, UInt8);
	void notifyClanBattleReport(Player *, Player *, UInt8, UInt32);
	void notifyClanBattleReport(Player *, const std::string&, UInt8, UInt32);
	void notifyClanBattleRecoveData(Player *, UInt16, UInt32);
	void notifyClanBattleRecoveData(Player *);
	void notifyClanBattleOverReport(UInt8);
	virtual void notifyClanBattleTotemAttack() {}
	virtual void notifyBattleOverMailNotify(UInt8) {}
	virtual void notifyBattleScore(Player * = NULL) {}

public:
	virtual Clan * getOwnerClan() = 0;
	virtual UInt32 getOwnerClanId() = 0;
	virtual void setOwnerClanId(UInt32) = 0;
	virtual UInt8 getOwnerClanLev() = 0;
	virtual void setOwnerClanLev(UInt8, bool = false) = 0;
	virtual void setOwnerClanAtkCount(UInt16, bool = false) = 0;
	virtual void setOwnerClanRank(UInt8, bool = false) = 0;
	virtual UInt8 getOwnerClanRank() = 0;
	virtual std::string getOwnerClanName() = 0;
    virtual ClanDynamicMsg * getClanDynamicMsg() = 0;
	virtual Clan * getAllyClan() { return NULL; }
	virtual UInt16 getBattleAchieve() { return 0; }
	virtual UInt8 getLev() { return 0; }

public:
	void sendClanBattleReport(Player *, UInt32);

protected:
	inline void pushBattleCount(UInt8);
	virtual inline UInt8 getBattleReportType(bool leave =  false) { return leave ? 0x01 : 0x00; }

protected:
	UInt32	_grabAchieve;
	UInt8	_southEdurance;	//used for service restart and battler reLoad
	UInt8	_northEdurance;
	UInt8	_hallEdurance;
	UInt32	_battleThisDay;
	UInt8	_battleTime;
	UInt8	_nextBattleTime;

	UInt8	_hasBattle;
	UInt8	_isInbattling; // 0:未开启 1:开启 2:结束 (针对一天而言) 0xFF:不开放
	UInt8	_isOldInBattling;
	bool	_isInAttacking;

	ClanBattleHold _holds[4];	// 0xF001:宗祠 0xF002:南门 0xF003:北门 0xF004:复活点
	bool _firstAttack[2];
	std::map<Player *, ClanBattlePlayer *> _clanBattlePlayerLocs;
	std::set<ClanBattlePlayer *> _recoverClanBattlers;	
	std::set<ClanBattlePlayer *> _offClanBattlers;
	std::map<std::string, ClanHoldMonster *> _recoverClanHoldAssistant;
	Stream _attackClanBattlerStream;
	Stream _attackClanTips;
};


class ClanCityBattle : public ClanBattle
{
public:
	ClanCityBattle(Clan *);
	virtual ~ClanCityBattle();

public:
	void initClanBattle(bool = true);

public:
	UInt8 getEnterBattleClanCount(Player *);
	UInt8 getBattleTime();
	bool setNextBattleTime(Player *, UInt8);
	UInt8 getNextBattleTime();
	UInt8 getClanRelation(Clan *);

public:
	UInt32 getClanBattleOverTime();
	bool canClanBattleOpen();
	void incGrabAcheive(UInt32);

	int testClanBattleStatus(UInt32, UInt32);
	void configClanBattleCheck();
	bool resumeClanBattleData(UInt32 = TimeUtil::Now());

	UInt32 getBattleOverTime(UInt32 = TimeUtil::Now());
	void closingBattlerAward(UInt8);

	bool incEnterClanCount(Player *);
	UInt16 getWillGainBattlerAward(Clan *);

public:
	void update(UInt32);

public:
	bool enterTotem(Player *);
	bool enterClanCity(Player *);

	bool attackPlayer(Player *, std::string);
	bool attackPlayer2(ClanBattlePlayer *, ClanBattlePlayer *);
	bool attackNpc(Player *, std::string);

public:
	void notifyClanBattleOverReport(UInt8);
	void notifyClanBattleTotemAttack();
	void notifyBattleOverMailNotify(UInt8);


public: 
	static void setMaxEnterCount(UInt8 count){_maxEnterCount = count;}
	static UInt8 getMaxEnterCount(){return _maxEnterCount;}
	static UInt8 getClanBattleStatus(UInt8, UInt32 = TimeUtil::Now());

public:
	Clan * getOwnerClan() { return _clan; }
	UInt32 getOwnerClanId();
	void setOwnerClanId(UInt32) {} 
	void setOwnerClanRank(UInt8, bool = false) {}
	UInt8 getOwnerClanRank() { return 0; }
	UInt8 getOwnerClanLev();
	void setOwnerClanLev(UInt8, bool = false) {}
	void setOwnerClanAtkCount(UInt16, bool = false) {}
	std::string getOwnerClanName();
	ClanDynamicMsg * getClanDynamicMsg();
	Clan * getAllyClan();
	UInt16 getBattleAchieve();
	UInt8 getLev();

private:
	static UInt8 _maxEnterCount;
	UInt16 _achieve;
	Clan * _clan;
};

class ClanRobBattle : public ClanBattle
{
public:
	ClanRobBattle(UInt8 = 0xFF, Clan * = NULL);
	virtual ~ClanRobBattle();

public:
	void initClanBattle(bool = true);

public:
	Clan * getOwnerClan() { return _status == 0 ? NULL : _clan; }
	UInt32 getOwnerClanId() { return 0; }
	void setOwnerClanId(UInt32 id) { _heroClanId = id; }
	UInt8 getOwnerClanLev();
	void setOwnerClanLev(UInt8, bool = false);
	void setOwnerClanAtkCount(UInt16, bool = false);
	void setOwnerClanRank(UInt8, bool = false);
	UInt8 getOwnerClanRank() { return _heroClanRank; }
	std::string getOwnerClanName();
	ClanDynamicMsg * getClanDynamicMsg() { return _clanDynamicMsg; }
	UInt8 getClanBattleType() { return _clan != NULL ? 2 : 1; }

public:
	void update(UInt32);

public:
	int testClanBattleStatus(UInt32, UInt32);
	void configClanBattleCheck();
	void configClanBattleData(bool = true);
	UInt8 getBattleTime() { return 39; }	//FIXME
	UInt8 getNextBattleTime() { return 39; }
	bool resumeClanBattleData(UInt32 = TimeUtil::Now());
	void resumeBattleScore();
	UInt32 getClanBattleOverTime();
	UInt8 getClanRelation(Clan *);

public:
	bool enterTotem(Player *);
	bool enterClanCity(Player *);

	bool attackPlayer(Player *, std::string);
	bool attackPlayer2(ClanBattlePlayer *, ClanBattlePlayer *);
	bool attackNpc(Player *, std::string);
	virtual bool  attackAssist(Player *, std::string&, UInt32&, UInt32&, ClanHoldMonster *);
	virtual bool  attackGuarder(Player *, std::string&, UInt32&, UInt32&, ClanHoldMonster *);

	void closingBattlerAward(UInt8);
	void notifyBattleOverMailNotify(UInt8);
	void notifyBattleScore(Player * = NULL);
	void notifySelfBattleScore(Clan *, UInt32);
	void notifySelfBattleScore(Player *, UInt32 = 0, bool = false);

	void  clearClanBattle();

public:
	static UInt8 getClanBattleStatus(UInt8, UInt32 = TimeUtil::Now());

protected:
	virtual inline UInt8 getBattleReportType(bool leave = false) { return leave ? 0x11 : 0x10; }

private:
	void switchBattleClanOwner();
	bool attackMonster(Player *, std::string&, UInt32&, UInt32&, ClanHoldMonster *, UInt8);
	void initDisplayMonsterStatus();
	void displayHiddenMonster(UInt16, bool = true);
	UInt8 getBattlerLev();
	void calcBattleScore(Player *, UInt32);
	void calcBattleExp();

private:
	//static const UInt8 _battleTime = 39;
	UInt8  _status;	//状态 0: 抢夺战 1: 守城战  0xFF: 未开放状态(关闭状态)
	std::string _heroClanName;
	UInt32 _heroClanId;
	UInt8 _heroClanLev;
	UInt16 _heroClanAtkCount;
	UInt8 _heroClanRank;
	bool _display[2][2];
	Clan * _clan;
	ClanDynamicMsg * _clanDynamicMsg;
	std::map<Clan *, UInt32> _sst;
	std::multimap<UInt32, Clan *, std::greater<UInt32> > _rowSst;
	std::map<Clan *, std::set<ClanBattlePlayer *> > _cCbp;
};

}


#endif
