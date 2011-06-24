#ifndef CLANTECH_INC
#define CLANTECH_INC

namespace GObject
{

class Clan;
class Player;

/*
	1:ս��(�ۺ�) 2:����(����) 3:ϲ��Ʒ(����)
*/

/*
    1 : ����ȼ� 2: ���徭�� 3 : ϲ��Ʒ(����)
*/

struct ClanTechData
{
	ClanTechData(UInt8 s = 0, UInt16 t = 0 , UInt8 l = 0, UInt16 e = 0) 
		: techId(s), type(t), level(l), extra(e)
	{
	}

	UInt8  techId;
	UInt16 type;
	UInt8  level;	//��� = 0�� ��ʾ�˼�����δ������
	UInt16 extra;
};

class ClanTech
{
	typedef std::map<UInt8, ClanTechData> Techs;

public:
	ClanTech(Clan * = NULL);
	~ClanTech();

public:
	void addTechFromDB(UInt8, UInt8, UInt16);
	void buildTech();

public:
	bool donate(Player *, UInt8, UInt16, UInt16);
	void makeTechInfo(Stream&);
	void makeTechInfo(Stream&, ClanTechData&);
	UInt8 getSize() { return _techs.size(); }
	void addTech(UInt8, UInt16, UInt8, UInt16);
	UInt8 getLev(UInt8);
	Int32 getExtra(UInt8);
	inline UInt8 getClanLev() { return getLev(1); }
	bool isTechFull(UInt8);

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
	bool techLevelUp(UInt8, UInt8&, UInt16&, UInt16);
	bool techLevelDown(UInt8, UInt8&, UInt16&, UInt16);

private:
	UInt8 _clanLev;
	Clan * _clan;
	Techs _techs;
	Mutex _mutex;
};

}

#endif
