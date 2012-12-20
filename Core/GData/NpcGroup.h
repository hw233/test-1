#ifndef _NPCGROUP_H_
#define _NPCGROUP_H_

namespace GObject
{
	class Fighter;
	class Player;
}

namespace Battle
{
	class BattleSimulator;
}

namespace Script
{
	class BattleFormula;
}

namespace GData
{

class Formation;
struct LootItem;
struct LootResult;

struct NpcFData
{
	GObject::Fighter * fighter;
	UInt8 pos;
};

class NpcGroup
{
public:
	NpcGroup(UInt32 id): _id(id), _formation(0), _exp(0), _bp(0.0f), _type(0) {}
	inline UInt32 getId() { return _id; }
	void addFighter(UInt32 id, UInt8 pos);
	inline void setExp(UInt32 e) { _exp = e; }
	inline UInt32 getExp() { return _exp; }
	inline void setFormation(UInt16 f) { _formation = f;}
	const std::string& getName() const;
	inline UInt16 getFormation() { return _formation; }
	void putFighters(Battle::BattleSimulator&);
	void monsterKilled(GObject::Player *, UInt32 = 1);
	void addLoots(std::vector<const LootItem *>&);
	void getLoots(GObject::Player *, UInt8 lootlvl = 0);
	void getLoots(GObject::Player *, std::vector<LootResult>&, UInt8 lootlvl = 0, UInt8* atoCnt = NULL);
    void getLoots( GObject::Player * player, std::vector<LootResult>& il, UInt8 lootlvl, UInt8* atoCnt , bool bind);
	void calcBattlePoints(Script::BattleFormula *);
	inline float getBattlePoints() { return _bp; }
	UInt8 getLevel();
	UInt8 getClass();
    UInt16 getPortrait();
	std::vector<NpcFData>& getList() { return _npcList; }
    inline UInt8 getType() { return _type; }
    inline void setType(UInt8 type) { _type = type; }
private:
	UInt32 _id;
	std::vector<NpcFData> _npcList;
	std::map<UInt32, UInt16> _npcCount;
	UInt16 _formation;
	UInt32 _exp;
    std::vector<std::vector<const LootItem *> > _loots;
	float _bp;
    UInt8 _type;
};

typedef std::map<UInt32, NpcGroup *> NpcGroups;
extern NpcGroups npcGroups;

}

#endif // _NPCGROUP_H_
