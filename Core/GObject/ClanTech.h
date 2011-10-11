#ifndef CLANTECH_INC
#define CLANTECH_INC

namespace GObject
{

class Clan;
class Player;

/*
	1:战功(综合) 2:银两(其他) 3:喜好品(神兽)
*/

/*
    1 : 宗族等级 2: 宗族经验 3 : 喜好品(神兽)
*/

#define CLAN_TECH_PRACTICE_SPEED    1
#define CLAN_TECH_PRACTICE_SLOT     2
#define CLAN_TECH_MEMBER_COUNT      3
#define CLAN_TECH_SKILL_EXTEND      4

struct ClanTechData
{
	ClanTechData(UInt8 s = 0, UInt16 t = 0 , UInt8 l = 0, UInt32 e = 0) 
		: techId(s), type(t), level(l), extra(e)
	{
	}

	UInt8  techId;
	UInt16 type;
	UInt8  level;	//如果 = 0， 表示此技能尚未被激活
	UInt32 extra;
};

class ClanTech
{
	typedef std::map<UInt8, ClanTechData> Techs;

public:
	ClanTech(Clan * = NULL);
	~ClanTech();

public:
	void addTechFromDB(UInt8, UInt8, UInt32);
	void buildTech();

public:
	bool donate(Player *, UInt8, UInt16, UInt32);
	void makeTechInfo(Stream&);
	void makeTechInfo(Stream&, ClanTechData&);
	UInt8 getSize() { return _techs.size(); }
	void addTech(UInt8, UInt16, UInt8, UInt32);
	UInt8 getLev(UInt8);
	UInt32 getExtra(UInt8);
	inline UInt8 getClanLev();
	bool isTechFull(UInt8);

	bool addAchieve(UInt32);
	bool delAchieve(UInt32);

	UInt32 getAtuobattleSpeed();
	UInt16 getSouthEdurance();
	UInt16 getNorthEdurance();
	UInt16 getBattleAchieve();
	UInt16 getClanAchieve();
	bool   getHoldAssist(UInt16&, UInt32&);
	UInt32 getHoldTotemGuarder();
	UInt32 getHoldCityGuarder();

    UInt32 getPracticeSpeed();
    UInt32 getPracticeSlot();
    UInt32 getMemberCount();
    UInt32 getSkillExtend();

private:
	bool techLevelUp(UInt8, UInt8&, UInt32&, UInt32);
	bool techLevelDown(UInt8, UInt8&, UInt32&, UInt32);

private:
	UInt8 _clanLev;
	Clan * _clan;
	Techs _techs;
	Mutex _mutex;
};

}

#endif
