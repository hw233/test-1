#ifndef CLANSKILL_INC
#define CLANSKILL_INC

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

struct ClanSkillData
{
	ClanSkillData(UInt8 s = 0, UInt16 t = 0 , UInt8 l = 0, UInt16 e = 0) 
		: skillId(s), type(t), level(l), extra(e)
	{
	}

	UInt8  skillId;
	UInt16 type;
	UInt8  level;	//如果 = 0， 表示此技能尚未被激活
	UInt16 extra;
};

class ClanSkill
{
	typedef std::map<UInt8, ClanSkillData> Skills;

public:
	ClanSkill(Clan * = NULL);
	~ClanSkill();

public:
	void addSkillFromDB(UInt8, UInt8, UInt16);
	void buildSkill();

public:
	bool donate(Player *, UInt8, UInt16, UInt16);
	void makeSkillInfo(Stream&);
	void makeSkillInfo(Stream&, ClanSkillData&);
	UInt8 getSize() { return _skills.size(); }
	void addSkill(UInt8, UInt16, UInt8, UInt16);
	UInt8 getLev(UInt8);
	Int32 getExtra(UInt8);
	inline UInt8 getClanLev() { return getLev(1); }
	bool isSkillFull(UInt8);

	bool addAchieve(UInt16);
	bool delAchieve(UInt16);

	UInt32 getAtuobattleSpeed();
	UInt16 getSouthEdurance();
	UInt16 getNorthEdurance();
	UInt16 getBattleAchieve();
	UInt16 getClanAchieve();
	bool   getHoldAssist(UInt16&, UInt32&);
	UInt32 getHoldTotemGuarder();
	UInt32 getHoldCityGuarder();

private:
	bool skillLevelUp(UInt8, UInt8&, UInt16&, UInt16);
	bool skillLevelDown(UInt8, UInt8&, UInt16&, UInt16);

private:
	UInt8 _clanLev;
	Clan * _clan;
	Skills _skills;
	Mutex _mutex;
};

}

#endif
