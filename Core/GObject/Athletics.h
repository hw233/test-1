#ifndef ATHLETICS_INC_
#define ATHLETICS_INC_

#include "Common/TimeUtil.h"

namespace GObject
{

struct AthleticsData
{
	UInt32 id;
	UInt8 side;	// 0: atker 1: defer
	Player * target;
	UInt8 win;	//0 : winner 1: failure
	UInt8  awardType;
	UInt32 awardCount;
	UInt32 reptid;
	UInt32 time;

	AthleticsData() { memset(this, 0x00, sizeof(*this));}
	AthleticsData(UInt32 id_, UInt8 side_, Player * target_, UInt8 win_, UInt8 awardType_ = 0, UInt32 awardCount_ = 0, UInt32 reptid_ = 0, UInt32 time_ = TimeUtil::Now())
		: id(id_), side(side_), target(target_), win(win_), awardType(awardType_), awardCount(awardCount_), reptid(reptid_),time(time_) {}
};

struct Lineup;
class Player;
class Athletics
{
public:
	Athletics(Player *);
	~Athletics();

public:
	bool addAthleticsDataFromDB(UInt32, UInt8, Player *, UInt8, UInt8, UInt32, UInt32, UInt32);
	void setEnterAthletics(bool = true);
	bool hasEnterAthletics();
	void enterAthleticsNotify(UInt8);

public:
	UInt32 addAthleticsData(UInt8, Player *, UInt8, UInt32, UInt32 = TimeUtil::Now());
	void addAthleticsDataFromTarget(UInt8, Player *, UInt32, UInt8, UInt32, UInt32);
	void attack(Player *);
	void beAttack(Player *, UInt8, UInt16, Lineup*);
    void notifyAttackRes(Player *, bool);


public:
	void notifyAthleticsData(UInt16 = 1);
	void notifyAthleticsData2(AthleticsData *);
	void notifyAthleticsData2(UInt32);
	void notifyAthleticsDeferData(UInt8, Player *, UInt32, UInt8, UInt32, UInt32);
	void notifyDropAthleticsData(UInt32);
	void updateAthleticsAwardData(UInt32, UInt8, UInt32);

public:
	void defendergainsource(Player *, UInt32, UInt8, UInt32, UInt8);
	void attackergainsource(UInt32, UInt8, UInt32, UInt8);
	inline void AddUserSource(Player *player, UInt8 type, UInt32 count, bool delay = false);

	void GetBoxAddSourceNotify(UInt8 type, UInt32 count);

public:
	bool addAthleticsExtraAward(UInt32 EquipId, UInt8 rank);

private:
	Player * _owner;
	bool _hasEnterAthletics;
	std::deque<AthleticsData *> _athleticses;
};

}


#endif