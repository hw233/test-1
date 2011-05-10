#ifndef	MAPOBJECT_INC
#define MAPOBJECT_INC

namespace GObject
{

struct MOData
{
	UInt32			m_ID;	//Key
	std::string		m_Name;
	UInt16			m_Spot;
	UInt8			m_Type;
	UInt32			m_ActionType;
	bool			m_Hide;


	bool operator<(const MOData& cmp) const { return m_ID < cmp.m_ID;  }
	bool operator==(const MOData& cmp) const { return m_ID == cmp.m_ID; }
};

class Player;
class MOAction;
class MapObject
{
public:
	MapObject(const MOData& moData);
	virtual ~MapObject();

public:
	inline UInt32 GetID() const			{ return m_MOData.m_ID;		}
	inline std::string GetName() const	{ return m_MOData.m_Name;	}
	inline UInt16 GetSpot() const		{ return m_MOData.m_Spot;	}
	inline UInt8 GetType() const		{ return m_MOData.m_Type;	}
	inline UInt32 GetActionType() const	{ return m_MOData.m_ActionType; }
	inline bool IsHide() const			{ return m_MOData.m_Hide; }
	
	inline MOData& GetMOData()	{ return m_MOData;	}

public:
	void Action(Player* player);
	
private:
	MOData m_MOData;
};

typedef std::map<UInt32, MapObject*> MOMap;

}

#endif
