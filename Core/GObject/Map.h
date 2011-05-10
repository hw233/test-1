#ifndef MAP_INC
#define MAP_INC

#include "Common/Stream.h"
#include "MapObject.h"
#include "GObjectBase.h"
#include "Server/WorldServer.h"
#include "Player.h"


namespace Network
{
	class TcpConduit;
}

namespace GObject
{


class CountryBattle;
//��Ϊ�����������ݳ���
struct MapData
{
	MapData(): m_ID(0), m_Level(0), m_Acquirable(0), m_Country(0) { }
	UInt8		m_ID;
	std::string	m_Name;
	UInt8		m_Level;			//�����ͼ����ȼ�
	UInt8		m_Acquirable;
	UInt8		m_Country;
};
struct PlayerLevelCmp
{
	bool operator()(const Player * pl1, const Player * pl2) const
	{
		if (pl1->GetLev() > pl2->GetLev())
			return true;
		else
			return false;
	}
};
typedef std::multiset<Player *, PlayerLevelCmp> MapPlayer;
const UInt32 MAX_NUM = 20;
struct SpotData
{
	UInt16				m_ID;
	std::string			m_Name;
	UInt8				m_Type;
	UInt8				m_IsCountryBattle;
	UInt32				m_PosX;
	UInt32				m_PosY;
	std::set<Player *>	m_Players;

	CountryBattle *		m_CountryBattle;

	SpotData(): m_ID(0), m_Type(0), m_IsCountryBattle(0), m_PosX(0), m_PosY(0), m_CountryBattle(NULL) {}
	~SpotData();
	inline CountryBattle* GetCountryBattle() { return m_CountryBattle; }
	void InitCountryBattle();
};

class Map:
	public GObjectBaseT<Map, UInt8>
{
public:
	Map(UInt8 id, std::string name);
	~Map();

public:
	bool operator<(const Map& other) const { return getId() < other.getId(); }
	bool operator==(const Map& other) const { return getId() == other.getId(); }

protected:
	inline UInt32 getIndexbyPK(Player *pl);
public:
	void changebyStatus(Player *);

public:
	void InitCountryBattle();
	void PlayerEnter(Player *, bool = true);
	void PlayerLeave(Player *, bool = false, bool = true);

	inline const std::string& GetName() { return m_Name; }

	inline void SetMapData(MapData& md) { m_MapData = md; }
	inline const MapData& GetMapData() { return m_MapData; }

	bool AddObject(MOData& mo);
	void DelObject(UInt32 id);
	void DelObjects();	//all objects
	MapObject* GetObject(UInt32 id); 

	static void AddSpot(UInt16 id, const std::string&, UInt8 type, UInt8 countryBattle, UInt32 x, UInt32 y);
	static Map * FromSpot(UInt16 spotID);
	static Map * FromID(UInt8 mapID);
	static SpotData * Spot(UInt16 spotID);
	static void NotifyPlayerEnter( Player * );
	static void NotifyPlayerLeave( Player * );

	void SendCityNPCs( Player * );
	void SendOnMap( Player * );
	void SendOnSpot( Player * );
	void SendAtCity( Player *pl, bool inCity = true, bool notify = true);
	UInt32 getCityPlayerNum();
	void OnPlayerLevUp(Player *pl);
	MapPlayer::iterator find(UInt8 country, UInt8 status, Player *player);

	void Hide(UInt32, bool = true);
	void SendHide( UInt32 id, Player * player);
	void Show(UInt32, bool = true);

	inline void Broadcast( UInt16 loc, Stream& st, Player * pl = NULL )
	{
		Broadcast(GetSpot(loc), st, pl);
	}
	void Broadcast( SpotData *, Stream&, Player * = NULL );
	void Broadcast( Stream &, Player * = NULL );
	void Broadcast( const void *, int, Player * = NULL );

	SpotData* GetSpot(UInt16);

private:
	std::string m_Name;

	MapData		m_MapData;		//��ʼ��ʱ

	MOMap		m_MOMap;		//��ͼ�ݵ����ж���ļ���

	std::map<UInt16, SpotData>	m_Spots;
	MapPlayer _playerList[COUNTRY_MAX][2];//country pk_status
};

typedef std::vector<Map *> MapList;
extern MapList mapList;

}

#endif
