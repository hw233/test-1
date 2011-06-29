#ifndef _GOBJECTMANAGER_H_
#define _GOBJECTMANAGER_H_

#include "GLocalObjectManager.h"
#include "GGlobalObjectManager.h"
#include "ItemData.h"

namespace GObject
{
	class ItemEquip;
	class ItemWeapon;
	class ItemArmor;
#ifndef CHANCECOND
#define CHANCECOND(a) ((0xFFFF0000 & a) >> 16)
#endif
#ifndef CHANCEVALUE
#define CHANCEVALUE(b) (0x0000FFFF & b)
#endif
#ifndef MAKECHANCE
#define MAKECHANCE(a,b) (((0x0000FFFF & a) << 16) | (0x0000FFFF & b))
#endif
	class GObjectManager
	{
	public:
		static void loadAllData();

	public:
		static bool InitIDGen();
		static bool loadMapData();
		static bool loadFighters();
		static bool loadClanAssist();
		static bool loadClanRobMonster();
		static bool loadAllPlayers();
		static bool loadAllAthletics();
		static bool delayLoad();
		static bool loadAllFriends();
		static bool loadAllClans();
		static bool loadEquipments();
		static bool LoadDungeon();
		static bool unloadEquipments();				//从数据库内卸载所有的装备
		static bool LoadSpecialAward();
		static bool LoadLuckyDraw();
		static bool LoadFighterTrain();
		static bool LoadArena();
        static bool LoadPracticePlace();
        static bool LoadPracticeData();
		static ItemEquip * fetchEquipment(UInt32, bool = true);  //获取装备
		static ItemWeapon * fetchWeapon(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemArmor * fetchArmor(UInt32);      //获取盔甲,基于fetchEquipment函数实现

		static bool InitGlobalObject();
		static bool resetClanData(UInt32);

        static bool loadEquipForge();
        static bool loadFightersPCChance();

        static UInt32 getEnchantCost() { return _enchant_cost; }
        static UInt32 getMergeCost() { return _merge_cost; }
        static UInt32 getDetachCost() { return _detach_cost; }
        static UInt32 getSplitCost() { return _split_cost; }
        static UInt32 getForgeCost() { return _forge_cost; }
        static UInt32 getSplitChance( UInt8 q, UInt8 l_h ) { return _split_chance[q][l_h]; }
        static UInt32 getMergeChance( UInt8 lvl ) { return _merge_chance[lvl]; }
        static UInt32 getEnchantChance( UInt8 lvl ) { return _enchant_chance[lvl]; }
        static UInt8 getEnchantMax( UInt8 vip ) { return _enchant_max[vip]; }
        static UInt8 getAttrChance( UInt8 lvl, UInt8 q, UInt8 idx ) { return _attrChances[lvl][q][idx]; }
        static UInt16 getAttrInfo( UInt8 lvl, UInt8 t, UInt8 q, UInt8 idx ) { return _attrInfo[lvl][t][q][idx]; }
        static UInt32 getSocketChance( UInt8 s ) { return _socket_chance[s]; }
        static UInt32 getMinPotential() { return _min_potential; }
        static UInt32 getMaxPotential() { return _max_potential; }
        static UInt32 getMinCapacity() { return _min_capacity; }
        static UInt32 getMaxCapacity() { return _max_capacity; }
        static std::vector<UInt32>& getPotentialChance() { return _potential_chance; }
        static std::vector<UInt32>& getCapacityChance() { return _capacity_chance; }
        static UInt32 getTavernFighterStart() { return _tavernFighterStart; }
        static UInt32 getTavernFighterEnd() { return _tavernFighterEnd; }
	private:
		static std::map<UInt32, ItemEquip *> equips;
        static UInt32 _enchant_cost;
        static UInt32 _merge_cost;
        static UInt32 _detach_cost;
        static UInt32 _split_cost;
        static UInt32 _forge_cost;
        static UInt32 _split_chance[4][2];
        static UInt32 _merge_chance[9];
		static UInt32 _enchant_chance[12];
		static UInt8  _enchant_max[11];
		static UInt8  _attrChances[11][3][8];
		static UInt16 _attrInfo[11][8][3][8];
        static UInt32 _socket_chance[6];
        static UInt32 _min_potential;
        static UInt32 _max_potential;
        static UInt32 _min_capacity;
        static UInt32 _max_capacity;
        static std::vector<UInt32> _potential_chance;
        static std::vector<UInt32> _capacity_chance;
        static UInt32 _tavernFighterStart;
        static UInt32 _tavernFighterEnd;
	};
}

#endif // _GOBJECTMANAGER_H_
