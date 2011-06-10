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
		static ItemEquip * fetchEquipment(UInt32, bool = true);  //获取装备
		static ItemWeapon * fetchWeapon(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemArmor * fetchArmor(UInt32);      //获取盔甲,基于fetchEquipment函数实现

		static bool InitGlobalObject();
		static bool resetClanData(UInt32);

	private:
		static std::map<UInt32, ItemEquip *> equips;
	};
}

#endif // _GOBJECTMANAGER_H_
