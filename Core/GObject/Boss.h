#ifndef _BOSS_H_
#define _BOSS_H_

#include "Common/Stream.h"

namespace GData
{
	class NpcGroup;
}

namespace GObject
{

class Player;

class Boss
{
	struct DamageInfo
	{
		DamageInfo(): damage(0), exp(0), count(0) {}
		UInt32 damage;
		UInt32 exp;
		UInt32 count;
	};
public:
	Boss(UInt32);
	bool attackBy(Player *);
	void firstBloodReward(Player *);
	void onReward();
	void reset();
	void setHP(UInt8, UInt32);
	void setDamage(Player *, UInt32, UInt32);
	UInt16 getId();
	const std::string& getName();
	inline bool isAlive() { return !_hp.empty() && _hp[0] != 0; }
	UInt8 getHPPercent();
	void appear();
	inline UInt8 getLevel() { return _level; }
	void setLevel(UInt8, bool = true);
public:
	inline void pushAttackerCountBS(bool isDead, UInt32 spawnTime);
protected:
	void BroadCastTV(UInt32 damage);
protected:
	void getRandomItemId(Player *player, UInt16& itemId1,UInt16& count1, UInt16& itemId2, UInt16& count2, UInt8& extraAwardNum);
private:
	bool _checkFBReward;
	UInt8 _hppercent;
	UInt8 _level;
	std::vector<UInt32> _hp;
	GData::NpcGroup * _ng;
	std::map<Player *, DamageInfo> _damage;
};

class BossManager
{
public:
	struct BossStruct
	{
		Boss * boss;
		UInt16 location;
		UInt32 prepareTime;
		UInt32 spawnTime;
		UInt32 fleeTime;
		UInt8 appearLevel;
	};
	BossManager();
	~BossManager();
	void addBoss(UInt32, UInt16, UInt32, UInt32, UInt8);
	void process(UInt32);
	Boss * findBoss(UInt32);
	bool attack(Player *, UInt32);
	void sendInfo(Player *);
	void buildInfo(Stream&);
	void buildCurrBossInfo(Stream&, UInt8);
	void getNextBoss();
private:
	std::map<UInt32, BossStruct> _bossOff, _bossOn;
	UInt32 _nextBroadcast;
	UInt16 _prevBoss, _currBoss, _nextBoss;
	UInt8 _prevBossLevel, _currBossLevel, _nextBossLevel;
	UInt8 _prevBossPHP, _currBossPHP;
};

extern BossManager bossManager;

}

#endif // _BOSS_H_
