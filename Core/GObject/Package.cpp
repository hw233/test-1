#include "Config.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "Fighter.h"
#include "Package.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "GObjectDBExecHelper.h"
#include "GData/GDataManager.h"
#include "GObject/LuckyDraw.h"
#include "GObject/SpecialAward.h"
#include "Server/SysMsg.h"
#include "Script/GameActionLua.h"
#include "Server/OidGenerator.h"
#include "Common/StringTokenizer.h"
#include "Server/Cfg.h"

#define ITEM_FORGE_L1 500      // 洗炼符
#define ITEM_SOCKET_L1 510
#define ITEM_SOCKET_L2 511
#define ITEM_SOCKET_L3 512
#define ITEM_GEM_PROTECT 513    // 宝石保护符
#define ITEM_SPLIT_PROTECT 8925
#define ITEM_DETACH_PROTECT 505 // 精致拆卸石
#define ITEM_ENCHANT_PROTECT 8927
#define ITEM_DETACH_RUNE 504    // 粗制拆卸石
#define ITEM_FORGE_PROTECT 501  // 洗炼保护符
#define ITEM_ACTIVATE_ATTR 9215

namespace GObject
{
	static UInt32 enchant_cost[] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000};
	static UInt32 merge_cost[] = {50, 100, 150, 200, 250, 300, 350, 400, 500};
	static UInt32 detach_cost = 500;
	static UInt32 exchange_cost[2][10] = {{2000, 2000, 5000, 20000, 30000}, {3000, 3000, 10000, 25000, 35000, 40000, 50000, 80000, 100000}};
	static UInt32 exchange_set_cost = 100000;
	static UInt32 split_cost[4][11] = {{1000, 1000, 2000, 3000, 4000, 5000, 6000, 8000, 10000},
		{2000, 2000, 3000, 4000, 5000, 6000, 8000, 10000, 15000},
		{0, 0, 0, 0, 6000, 8000, 10000, 15000, 20000, 25000, 40000},
		{0, 0, 0, 0, 8000, 10000, 15000, 20000, 25000, 40000, 60000}};
	static UInt32 forge_cost = 50000;
	static UInt32 split_chance[4][11][2] = {
		{{0, 15}, {0, 15}, {0, 20}, {0, 25}, {0, 30}, {0, 30}, {0, 35}, {0, 40}, {0, 40}},
		{{15, 3}, {15, 3}, {20, 5}, {20, 8}, {15, 15}, {15, 15}, {20, 15}, {20, 20}, {20, 20}},
		{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {25, 5}, {20, 10}, {20, 15}, {25, 15}, {25, 15}, {25, 15}, {25, 15}},
		{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {10, 20}, {10, 20}, {10, 25}, {10, 30}, {10, 40}, {10, 40}, {10, 40}}
	};
	static UInt32 split_count[11][2] = {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 2}, {1, 2}, {1, 2}, {1, 3}, {1, 3}, {1, 3}, {1, 3}};
	static UInt32 merge_chance[9] = {100, 85, 85, 75, 75, 65, 65, 55, 55};
	static UInt32 activate_cost[11][3] = {
		{2000, 5000, 10000},
		{2000, 5000, 10000},
		{2000, 5000, 10000},
		{2000, 5000, 10000},
		{2000, 5000, 10000},
		{5000, 10000, 20000},
		{5000, 10000, 20000},
		{5000, 10000, 20000},
		{10000, 20000, 50000},
		{10000, 20000, 50000},
		{10000, 20000, 50000}};
#if 0
	static void getRandomAttr2(UInt8 lv, UInt8 q, int c, UInt8 mask, UInt8 * t, Int16 * v)
	{
		static UInt8 attrChances[11][3][8] =
		{
			{{15, 50, 35}},
			{{15, 50, 35}},
			{{5, 60, 35}},
			{{25, 50, 25}},
			{{5, 65, 30}, {15, 50, 35}, {15, 44, 20, 10, 5, 3, 2, 1}},
			{{5, 65, 30}, {15, 50, 35}, {15, 44, 20, 10, 5, 3, 2, 1}},
			{{5, 65, 30}, {15, 50, 35}, {15, 70, 10, 5}},
			{{5, 65, 30}, {15, 50, 35}, {15, 44, 20, 10, 5, 3, 2, 1}},
			{{5, 65, 30}, {15, 50, 35}, {15, 44, 20, 10, 5, 3, 2, 1}},
			{{0}, {15, 50, 35}, {15, 44, 20, 10, 5, 3, 2, 1}},
			{{0}, {15, 50, 35}, {15, 44, 20, 10, 5, 3, 2, 1}},
		};

		static UInt16 attrInfo[11][7][3][8] =
		{
			{{{10, 15, 20}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{50, 80, 100}},
			{{50, 80, 100}},
			{{40, 50, 60}}},

			{{{10, 15, 20}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{50, 80, 100}},
			{{50, 80, 100}},
			{{40, 50, 60}}},

			{{{20, 25, 30}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{50, 80, 100}},
			{{50, 80, 100}},
			{{40, 50, 60}}},

			{{{40, 45, 50}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{40, 50, 60}},
			{{50, 80, 100}},
			{{50, 80, 100}},
			{{40, 50, 60}}},

			{{{50, 55, 60}, {60, 70, 80}, {80, 85, 90, 95, 100, 105, 110, 120}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{40, 50, 60}, {60, 70, 80}, {90, 100, 110, 120, 130, 140, 150, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{50, 80, 100}, {100, 120, 150}, {180, 190, 200, 220, 240, 260, 280, 300}},
			{{50, 80, 100}, {100, 120, 150}, {180, 190, 200, 210, 230, 250, 260, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}}},

			{{{60, 70, 80}, {80, 90, 100}, {120, 125, 130, 135, 140, 150, 160, 180}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{40, 50, 60}, {60, 70, 80}, {90, 100, 110, 120, 130, 140, 150, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{50, 80, 100}, {100, 120, 150}, {180, 190, 200, 220, 240, 260, 280, 300}},
			{{50, 80, 100}, {100, 120, 150}, {180, 190, 200, 210, 230, 250, 260, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}}},

			{{{80, 90, 100}, {100, 110, 120}, {120, 140, 160, 180}},
			{{40, 50, 60}, {60, 70, 80}, {80, 100, 120, 150}},
			{{50, 60, 70}, {70, 80, 90}, {90, 110, 130, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 100, 120, 150}},
			{{60, 90, 120}, {120, 150, 180}, {180, 220, 260, 300}},
			{{60, 90, 120}, {120, 150, 180}, {180, 220, 250, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 110, 130}}},

			{{{100, 110, 120}, {120, 130, 140}, {140, 145, 150, 160, 170, 180, 200, 220}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{50, 60, 70}, {70, 80, 90}, {90, 100, 110, 120, 130, 140, 150, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 220, 240, 260, 280, 300}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 210, 230, 250, 260, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}}},

			{{{120, 130, 140}, {140, 155, 170}, {170, 180, 190, 200, 210, 220, 240, 260}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{50, 60, 70}, {70, 80, 90}, {90, 100, 110, 120, 130, 140, 150, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 220, 240, 260, 280, 300}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 210, 230, 250, 260, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}}},

			{{{120, 130, 140}, {175, 200, 225}, {200, 220, 240, 260, 280, 300, 320, 350}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{50, 60, 70}, {70, 80, 90}, {90, 100, 110, 120, 130, 140, 150, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 220, 240, 260, 280, 300}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 210, 230, 250, 260, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}}},

			{{{120, 130, 140}, {175, 200, 225}, {250, 275, 300, 325, 350, 375, 400, 450}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{50, 60, 70}, {70, 80, 90}, {90, 100, 110, 120, 130, 140, 150, 160}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 220, 240, 260, 280, 300}},
			{{60, 90, 120}, {120, 150, 180}, {180, 190, 200, 210, 230, 250, 260, 280}},
			{{40, 50, 60}, {60, 70, 80}, {80, 90, 100, 110, 120, 130, 140, 150}}},
		};
		if(c == 0)
		{
			if(q > 2)
				c = 3;
			else if(q == 2)
				c = 2;
			else
				c = 1;
		}

		UInt8 types[8] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		bool retain[3] = {false, false, false};
		int tcount = 8;
		for(int i = 0; i < c; ++ i)
		{
			retain[i] = (mask & (1 << i)) > 0;
			if(retain[i])
			{
				int oidx = t[i] - 1;
				if(types[oidx])
				{
					types[oidx] = 0;
				}
			}
		}
		for(int i = tcount - 1; i >= 0; -- i)
		{
			if(types[i] == 0)
			{
				-- tcount;
				if(i < tcount)
					types[i] = types[tcount];
			}
		}
		for(int i = 0; i < c; ++ i)
		{
			if(retain[i])
				continue;
			UInt8 oldt = t[i];
			UInt16 oldv = v[i];
			do
			{
				int nidx = uRand(tcount);
				UInt32 dice = uRand(100);
				t[i] = 0;
				v[i] = 0;
				for(UInt8 j = 0; j < 8; ++ j)
				{
					if(dice < GObjectManager::getAttrChance(lv, q, j)  /*attrChances[lv][q][j]*/)
					{
						t[i] = types[nidx];
						v[i] = GObjectManager::getAttrInfo(lv, t[i] - 1, q, j);         //attrInfo[lv][t[i] - 1][q][j];
						if(nidx + 1 < tcount)
							types[nidx] = types[tcount - 1];
						-- tcount;
						break;
					}
					dice -= GObjectManager::getAttrChance(lv, q, j);   //attrChances[lv][q][j];
				}
			}
			while(oldt != 0 && t[i] == oldt && v[i] == oldv);
		}
	}
#endif
	static void getRandomAttr2(UInt8 lv, UInt8 crr, UInt8 q, int c, UInt8 mask, UInt8 * t, Int16 * v)
    {
		if(c == 0)
		{
			if(q > 2)
				c = 3;
			else if(q == 2)
				c = 2;
			else
				c = 1;
		}

		UInt8 types[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
		bool retain[3] = {false, false, false};
		for(int i = 0; i < c; ++ i)
		{
			retain[i] = (mask & (1 << i)) > 0;
			if(retain[i])
			{
				int oidx = t[i] - 1;
				if(types[oidx])
				{
					types[oidx] = 0;
				}
			}
		}
		for(int i = 0; i < c; ++ i)
		{
			if(retain[i])
				continue;
			UInt8 oldt = t[i];
			UInt16 oldv = v[i];
			do
			{
				UInt32 tidx = uRand(10000);
				t[i] = 0;
				v[i] = 0;
                int prev = -1;
				for(UInt8 j = 0; j < 9; ++ j)
				{
					if(tidx < GObjectManager::getAttrTypeChance(q, j)  /*attrChances[lv][q][j]*/)
					{
                        if(types[j] == 0)
                        {
                            if(prev != -1)
                                j = prev;
                            else
                                continue;
                        }

						t[i] = types[j];
                        UInt32 aidx = uRand(10000);
                        for(UInt8 k = 0; k < 8; ++ k)
                        {
                            if(aidx < GObjectManager::getAttrChance(q, k))
                            {
                                UInt32 dics = GObjectManager::getAttrDics(q, k+1) - GObjectManager::getAttrDics(q, k);
                                UInt32 factor = GObjectManager::getAttrDics(q, k) + uRand(dics);
                                v[i] = GObjectManager::getAttrMax(lv, t[i]-1, q, crr)*factor;
                                break;
                            }
                        }
                        //attrInfo[lv][t[i] - 1][q][j];
                        types[j] = 0;
						break;
					}
                    prev = j;
				}
			}
			while(oldt != 0 && t[i] == oldt && v[i] == oldv);
		}
    }

	Package::Package(Player* player) : m_Owner(player), m_Size(0), _lastActivateLv(0), _lastActivateQ(0), _lastActivateCount(0)
	{
	}

	Package::~Package()
	{
	}

	bool Package::Init()
	{
		return true;
	}

	void Package::UnInit()
	{
		item_elem_iter iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_Items.clear();
	}

	ItemEquip* Package::GetEquip(UInt32 id)
	{
		if (!IsEquipId(id)) return NULL;
		return static_cast<ItemEquip *>(FindItem(id));
	}

	ItemBase* Package::GetItem(UInt32 id, bool bind)
	{
		if (IsEquipId(id)) return NULL;
		return FindItem(id, bind);
	}

	ItemBase* Package::Add(UInt32 typeId, UInt32 num, bool bind, bool silence, UInt8 FromWhere)
	{
		if(IsEquipTypeId(typeId))
			return AddEquipN(typeId, num, bind, silence, FromWhere);
		return AddItem(typeId, num, bind, silence, FromWhere);
	}

	ItemBase* Package::SetItem(UInt32 typeId, UInt32 num, bool bind)
	{
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
		if(itemType == NULL) return NULL;
		ITEM_BIND_CHECK(itemType->bindType,bind);
		ItemBase * item = FindItem(typeId, bind);
		if (item == NULL)
		{
			//Add New Item
			item = new(std::nothrow) ItemBase(typeId, *itemType);
			if (item == NULL) return NULL;
			item->SetBindStatus(bind);
			bool ret = TryAddItem(item, num);
			if (ret)
			{
				m_Items[ItemKey(typeId, bind)] = item;
				DB().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, num, m_Owner->getId(), bind ? 1 : 0);
				SendItemData(item);
				return item;
			}

			SAFE_DELETE(item);
			return NULL;
		}
		else
		{
			UInt16 cur = m_Size;
			UInt16 oldq = item->Size(), newq = item->Size(num);
			if (oldq == newq) return item;
			cur = cur + newq - oldq;
			if(cur > m_Owner->getPacksize())
				return NULL;
			if(!item->SetItem(num))
				return NULL;
			m_Size = cur;
			DB().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", item->Count(), typeId, bind ? 1 : 0, m_Owner->getId());
			SendItemData(item);
			return item;
		}
	}

	bool Package::DelItemAll(UInt32 id, bool bind)
	{
		if (IsEquipTypeId(id))
			return false;
		ItemBase * item = FindItem(id, bind);
		if (item == NULL) return false;
		DelItem2(item, item->Count());
		return true;
	}

	ItemBase* Package::AddItem(UInt32 typeId, UInt32 num, bool bind, bool silence, UInt8 fromWhere)
	{
		return AddItem2(typeId, num, !silence, bind, fromWhere);
	}

	ItemBase* Package::AddItem2(UInt32 typeId, UInt32 num, bool notify, bool bind, UInt8 fromWhere)
	{
        if (!typeId || !num) return NULL;
		if (IsEquipTypeId(typeId)) return NULL;
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
		if(itemType == NULL) return NULL;
		ITEM_BIND_CHECK(itemType->bindType,bind);
		ItemBase * item = FindItem(typeId, bind);
		if (item != NULL)
		{
			bool ret = TryAddItem(item, num);
			if (ret)
			{
				DB().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", item->Count(), typeId, bind, m_Owner->getId());
				SendItemData(item);
				if(notify)
					ItemNotify(item->GetItemType().getId(), num);
				if(fromWhere != 0 && item->getQuality() >= 3)
					DBLOG().PushUpdateData("insert into `item_courses`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, num, fromWhere, TimeUtil::Now());
				return item;
			}
			return NULL;
		}
		else
		{
			//Add New Item
			item = new(std::nothrow) ItemBase(typeId, *itemType);
			if (item == NULL) return NULL;
			item->SetBindStatus(bind);
			bool ret = TryAddItem(item, num);
			if (ret)
			{
				m_Items[ItemKey(typeId, bind)] = item;
				DB().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, num, m_Owner->getId(), bind ? 1 : 0);
				SendItemData(item);
				if(notify)
					ItemNotify(item->GetItemType().getId(), num);
				if(fromWhere != 0 && item->getQuality() >= 3)
					DBLOG().PushUpdateData("insert into `item_courses`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, num, fromWhere, TimeUtil::Now());
				return item;
			}

			SAFE_DELETE(item);
			return NULL;
		}
	}

	ItemBase* Package::AddItem2(ItemBase* item, UInt8 fromWhere)
	{
		UInt32 typeId = item->GetItemType().getId();
		if (IsEquipTypeId(typeId)) return item;
		bool bind = item->GetBindStatus();
		UInt16 count = item->Count();
		ItemBase * exist = FindItem(typeId, bind);
		if (exist != NULL)
		{
			if (TryAddItem(exist, count))
			{
				DB().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", exist->Count(), typeId, bind, m_Owner->getId());
				SendItemData(exist);
				ItemNotify(item->GetItemType().getId(), count);
			}
			if(fromWhere != 0 && item->getQuality() >= 3)
				DBLOG().PushUpdateData("insert into `item_courses`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, count, fromWhere, TimeUtil::Now());
			return item;
		}
		else
		{
			UInt32 newSize = m_Size + item->Size();
			if(newSize > m_Owner->getPacksize())
				return item;
			m_Size = newSize;
			m_Items[ItemKey(typeId, bind)] = item;
			DB().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, count, m_Owner->getId(), bind ? 1 : 0);
			SendItemData(item);
			ItemNotify(item->GetItemType().getId(), count);
			if(fromWhere != 0 && item->getQuality() >= 3)
				DBLOG().PushUpdateData("insert into `item_courses`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, count, fromWhere, TimeUtil::Now());
			return NULL;
		}
	}

	ItemBase* Package::AddItemFromDB(UInt32 id, UInt32 num, bool bind)
	{
		assert(!IsEquipId(id));
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
        if (!itemType)
            return 0;
		ItemBase * item = new(std::nothrow) ItemBase(id, *itemType);
		if (item == NULL) return NULL;
		ITEM_BIND_CHECK(itemType->bindType,bind);
		item->SetBindStatus(bind);
		UInt16 oldq = item->Size(), newq = item->Size(item->Count() + num);
		m_Size = m_Size + newq - oldq;
		item->IncItem(num);
		m_Items[ItemKey(id, bind)] = item;
		return item;
	}

	ItemBase* Package::BuildItem(UInt32 id, UInt32 num, bool bind)
	{
		assert(!IsEquipId(id));
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
		ItemBase * item = new(std::nothrow) ItemBase(id, *itemType);
		if (item == NULL) return NULL;
		ITEM_BIND_CHECK(itemType->bindType,bind);
		item->SetBindStatus(bind);
		item->IncItem(num);
		return item;
	}

	ItemBase* Package::AddExistEquip( ItemEquip * equip )
	{
		ItemBase *& e = m_Items[ItemKey(equip->getId())];
		if(e == NULL)
			++ m_Size;
		e = equip;
		SendSingleEquipData(equip);
		return equip;
	}

	ItemBase* Package::AddEquip(UInt32 typeId, bool bind, bool silence, UInt8 FromWhere)
	{
		return AddEquip2(typeId, !silence, bind, FromWhere);
	}

	ItemBase* Package::AddEquip2(UInt32 typeId, bool notify, bool bind, UInt8 FromWhere)
	{
		if (!IsEquipTypeId(typeId)) return NULL;
		if(m_Size >= m_Owner->getPacksize())
			return NULL;
		//Add New Equip
		const GData::ItemBaseType * itype = GData::itemBaseTypeManager[typeId];
		if(itype == NULL)
			return NULL;
		switch(itype->subClass)
		{
		case Item_Weapon:
		case Item_Armor1:
		case Item_Armor2:
		case Item_Armor3:
		case Item_Armor4:
		case Item_Armor5:
		case Item_Ring:
		case Item_Amulet:
        case Item_Mounts:
        case Item_Trump:
			{
				ItemEquip * equip;
				ItemEquipData edata;

				UInt8 lv = (itype->reqLev + 5) / 10;
                UInt8 crr = itype->career;
				if(itype->quality > 2 && itype->subClass != Item_Trump)
				{
					UInt8 q = itype->quality - 3;
					UInt8 t[3] = {0, 0, 0};
					Int16 v[3] = {0, 0, 0};
					getRandomAttr2(lv, crr, q, 0, 0, t, v);
					edata.extraAttr2.type1 = t[0];
					edata.extraAttr2.value1 = v[0];
					edata.extraAttr2.type2 = t[1];
					edata.extraAttr2.value2 = v[1];
					edata.extraAttr2.type3 = t[2];
					edata.extraAttr2.value3 = v[2];
				}
				else
				{
					edata.extraAttr2.type1 = 0; edata.extraAttr2.value1 = 0;
					edata.extraAttr2.type2 = 0; edata.extraAttr2.value2 = 0;
					edata.extraAttr2.type3 = 0; edata.extraAttr2.value3 = 0;
				}

				UInt32 id = IDGenerator::gItemOidGenerator.ID();
				switch(itype->subClass)
				{
				case Item_Weapon:
					equip = new(std::nothrow) ItemWeapon(id, *static_cast<const GData::ItemWeaponType *>(itype), edata);
					break;
				case Item_Armor1:
				case Item_Armor2:
				case Item_Armor3:
				case Item_Armor4:
				case Item_Armor5:
					equip = new ItemArmor(id, *static_cast<const GData::ItemEquipType *>(itype), edata);
					break;
				default:
					equip = new ItemEquip(id, *static_cast<const GData::ItemEquipType *>(itype), edata);
					break;
				}
				if(equip == NULL)
					return NULL;
				ITEM_BIND_CHECK(itype->bindType,bind);
				equip->SetBindStatus(bind);

				ItemBase *& e = m_Items[id];
				if(e == NULL)
					++ m_Size;
				e = equip;
				DB().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", id, m_Owner->getId(), bind ? 1 : 0);
				DB().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`) VALUES(%u, %u, %u, %d, %u, %d, %u, %d)", id, typeId, edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3);
				SendSingleEquipData(equip);
				if(notify)
					ItemNotify(equip->GetItemType().getId());
				if(FromWhere != 0 && itype->quality >= 4)
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, id, FromWhere, TimeUtil::Now());
				return equip;
			}
		default:
			break;
		}
		return NULL;
	}

	ItemBase* Package::AddEquipN( UInt32 typeId, UInt32 num, bool bind, bool silence, UInt8 FromWhere )
	{
		if(GetRestPackageSize() < num)
			return NULL;
		ItemBase * item = NULL;
		for(UInt32 i = 0; i < num; ++ i)
		{
			item = AddEquip(typeId, bind, silence, FromWhere);
		}
		return item;
	}

	ItemBase* Package::AddEquipFromDB(UInt32 id, bool bind)
	{
		ItemEquip * equip = GObjectManager::fetchEquipment(id, false);
		if(equip == NULL) return NULL;
		ITEM_BIND_CHECK(equip->getBindType(),bind);
		equip->SetBindStatus(bind);
		ItemBase *& e = m_Items[ItemKey(id)];
		if(e == NULL)
			++ m_Size;
		e = equip;
		return equip;
	}

	ItemBase* Package::AddEquip2(ItemEquip * equip, UInt8 FromWhere)
	{
		ItemBase *& e = m_Items[ItemKey(equip->getId())];
		if(e == NULL)
			++ m_Size;
		e = equip;
		DB().PushUpdateData("REPLACE INTO `item` VALUES(%u, %u, %"I64_FMT"u, %d)", equip->getId(), 1, m_Owner->getId(), equip->GetBindStatus() ? 1 : 0);
		SendSingleEquipData(equip);
		ItemNotify(equip->GetItemType().getId());
		if(FromWhere != 0 && equip->getQuality() >= 4)
			DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), equip->GetItemType().getId(), equip->getId(), FromWhere, TimeUtil::Now());
		return equip;
	}



	ItemBase* Package::BuildEquipFromDB(UInt32 id, bool bind)
	{
		ItemEquip * equip = GObjectManager::fetchEquipment(id);
		if(equip == NULL) return NULL;
		ITEM_BIND_CHECK(equip->getBindType(),bind);
		equip->SetBindStatus(bind);
		return equip;
	}

	bool Package::MoveEquipFromPack(ItemEquip * equip)
	{
		item_elem_iter iter = m_Items.find(equip->getId());
		if(iter == m_Items.end())
			return false;
		m_Items.erase(iter);
		-- m_Size;
		DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		SendDelEquipData(equip);
		return true;
	}

	bool Package::DelItem(UInt32 id, UInt16 num, bool bind)
	{
		if (num == 0 || IsEquipId(id))
			return false;
		ItemBase * item = FindItem(id, bind);
		if (item == NULL) return false;
		bool ret = TryDelItem(item, num);
		if (ret)
		{
			UInt16 cnt = item->Count();
			SendItemData(item);
			if (cnt == 0)
			{
				SAFE_DELETE(item);
				m_Items.erase(ItemKey(id, bind));
				DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", id, bind, m_Owner->getId());
			}
			else
				DB().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND`ownerId` = %"I64_FMT"u", cnt, id, bind, m_Owner->getId());
		}
		return ret;
	}

	bool Package::DelItem2(ItemBase* item, UInt16 num)
	{
		if (num == 0 || item == NULL || IsEquipId(item->getId()))
			return false;
		bool ret = TryDelItem(item, num);
		if (ret)
		{
			UInt16 cnt = item->Count();
			SendItemData(item);
			UInt32 id = item->getId();
			bool bind = item->GetBindStatus();
			if (cnt == 0)
			{
				SAFE_DELETE(item);
				m_Items.erase(ItemKey(id, bind));
				DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", id, bind, m_Owner->getId());
			}
			else
				DB().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND`ownerId` = %"I64_FMT"u", cnt, id, bind, m_Owner->getId());
		}
		return ret;
	}

	bool Package::DelEquip(UInt32 id, UInt8 toWhere)
	{
		if(!IsEquipId(id)) return false;
		item_elem_iter iter = m_Items.find(ItemKey(id));
		if(iter == m_Items.end())
			return false;
		ItemBase * item = iter->second;
		m_Items.erase(iter);
		-- m_Size;
		DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", id);
		DB().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", id);
		if(toWhere != 0 && item->getQuality() >= 4)
		{
			DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), item->GetItemType().getId(), item->getId(), toWhere, TimeUtil::Now());
		}
		SendDelEquipData(static_cast<ItemEquip *>(item));
		SAFE_DELETE(item);
		return true;
	}

	bool Package::DelEquip2(ItemEquip * equip, UInt8 toWhere)
	{
		item_elem_iter iter = m_Items.find(equip->getId());
		if(iter == m_Items.end())
			return false;
		m_Items.erase(iter);
		-- m_Size;
		DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		DB().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", equip->getId());
		if(toWhere != 0 && equip->getQuality() >= 4)
		{
			DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), equip->GetItemType().getId(), equip->getId(), toWhere, TimeUtil::Now());
		}
		SendDelEquipData(equip);
		SAFE_DELETE(equip);
		return true;
	}

	bool Package::DelEquip3(ItemEquip * equip)
	{
		item_elem_iter iter = m_Items.find(equip->getId());
		if(iter == m_Items.end())
			return false;
		m_Items.erase(iter);
		-- m_Size;
		DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		SendDelEquipData(equip);
		return true;
	}

	bool Package::EquipTo( UInt32 id, Fighter * fgt, UInt8 part, ItemEquip *& newOne, bool noCheckFull )
	{
		newOne = NULL;
		if(id == 0 && !noCheckFull && IsFull())
			return false;
		ItemEquip * old = NULL;
		if(id != 0)
		{
			if (!IsEquipId(id)) return false;
			item_elem_iter iter = m_Items.find(ItemKey(id));
			if(iter == m_Items.end())
				return false;
			ItemBase * item = iter->second;
			if(fgt->getLevel() < item->getReqLev())
				return false;
			switch(part)
			{
			case 0x21:
				{
					if(item->getClass() != Item_Weapon)
						return false;
					GObject::ItemWeapon * wp = static_cast<GObject::ItemWeapon *>(item);
					old = fgt->setWeapon(wp);
				}
				break;
			case 0x22:
				if(item->getClass() != Item_Armor1)
					return false;
				old = fgt->setArmor(0, static_cast<GObject::ItemArmor *>(item));
				break;
			case 0x23:
				if(item->getClass() != Item_Armor2)
					return false;
				old = fgt->setArmor(1, static_cast<GObject::ItemArmor *>(item));
				break;
			case 0x24:
				if(item->getClass() != Item_Armor3)
					return false;
				old = fgt->setArmor(2, static_cast<GObject::ItemArmor *>(item));
				break;
			case 0x25:
				if(item->getClass() != Item_Armor4)
					return false;
				old = fgt->setArmor(3, static_cast<GObject::ItemArmor *>(item));
				break;
			case 0x26:
				if(item->getClass() != Item_Armor5)
					return false;
				old = fgt->setArmor(4, static_cast<GObject::ItemArmor *>(item));
				break;
			case 0x27:
				if(item->getClass() != Item_Amulet)
					return false;
				old = fgt->setAmulet(static_cast<GObject::ItemEquip *>(item));
				break;
			case 0x28:
				if(item->getClass() != Item_Ring)
					return false;
				old = fgt->setRing(static_cast<GObject::ItemEquip *>(item));
				break;
            case 0x50:
            case 0x51:
            case 0x52:
                if(item->getClass() != Item_Trump)
                    return false;
                old = fgt->setTrump(static_cast<GObject::ItemEquip *>(item), part-0x50);
                break;
            default:
                return false;
			}

			newOne = static_cast<GObject::ItemEquip *>(item);
			SendDelEquipData(static_cast<ItemEquip *>(item));
			m_Items.erase(iter);
			-- m_Size;
		}
		else
		{
			switch(part)
			{
			case 0x21:
				old = fgt->setWeapon(NULL);
				break;
			case 0x22:
				old = fgt->setArmor(0, NULL);
				break;
			case 0x23:
				old = fgt->setArmor(1, NULL);
				break;
			case 0x24:
				old = fgt->setArmor(2, NULL);
				break;
			case 0x25:
				old = fgt->setArmor(3, NULL);
				break;
			case 0x26:
				old = fgt->setArmor(4, NULL);
				break;
			case 0x27:
				old = fgt->setAmulet(NULL);
				break;
			case 0x28:
				old = fgt->setRing(NULL);
				break;
            case 0x50:
            case 0x51:
            case 0x52:
                old = fgt->setTrump(static_cast<GObject::ItemEquip *>(NULL), part-0x50);
                break;
            default:
                return false;
                break;
			}
		}
		if(old != NULL)
		{
			AddExistEquip(old);
		}
		return true;
	}

	UInt32 Package::SellItem(UInt32 id, UInt16 num, bool bind)
	{
		if (num == 0 || IsEquipId(id) )
			return 0;
		ItemBase* item = GetItem(id, bind);
		if (item == NULL) return 0;
		UInt32 price = item->getPrice();
		if (price == 0) return 0;
		if (DelItem2(item, num))
		{
			return price * num;
		}
		return 0;
	}

	UInt32 Package::SellEquip(UInt32 id)
	{
		if (!IsEquipId(id)) return 0;
		ItemBase* item = GetEquip(id);
		if (item == NULL) return 0;
		UInt32 price = item->getPrice();
		if (DelEquip2(static_cast<ItemEquip *>(item), ToSellNpc))
		{
			return price;
		}
		return 0;
	}

	bool Package::UseItem(UInt32 id, UInt16 num, UInt32 param, UInt8 bind)
	{
		if(!m_Owner->hasChecked())
			return false;
		bool ret = false;

        if (GetItemSubClass(id) == Item_Formula)
        {
            ItemBase* item = GetItem(id, bind > 0);
            if (item && item->getClass() == Item_Formula5)
            {
                ret = FormulaMerge(id, bind > 0);
                if (ret)
                    m_Owner->sendMsgCode(0, 5001);
                else
                    m_Owner->sendMsgCode(0, 5002);
                return ret;
            }
        }
        else if (GetItemSubClass(id) == Item_Citta)
        {
            ItemBase* item = GetItem(id, bind > 0);
            if (item && item->getClass() == Item_Citta5)
            {
                ret = CittaMerge(id, bind > 0);
                if (ret)
                    m_Owner->sendMsgCode(0, 5001);
                else
                    m_Owner->sendMsgCode(0, 5002);
                return ret;
            }
        }

        if (num == 0 || IsEquipId(id) ||
                (GetItemSubClass(id) != Item_Normal &&
                 (GetItemSubClass(id) != Item_Formula && 
                GetItemSubClass(id) != Item_Citta)))
			ret = false;
		else
		{
            if (GetItemSubClass(id) == Item_Formula || GetItemSubClass(id) == Item_Citta)
                num = 1;

			if (bind != 0xFF)
			{
				ItemBase* item = GetItem(id, bind > 0);
				if (item == NULL || item->Count() < num)
					ret = false;
				else if (GameAction()->RunItemNormalUse(m_Owner, id, param, num, bind > 0))
				{
					DelItem2(item, num);
					DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), id, num, TimeUtil::Now());
					ret = true;
				}				
			}
			else
			{
				if (GetItemAnyNum(id) < num)
					ret = false;
				else if (GameAction()->RunItemNormalUse(m_Owner, id, param, num, false))
				{
					DelItemAny(id, num);
					DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), id, num, TimeUtil::Now());
					ret = true;
				}
			}
		}

		Stream st(0x33);
		st << id << static_cast<UInt8>(1) << static_cast<UInt8>(ret ? 1 : 0) << Stream::eos;
		m_Owner->send(st);

		return ret;
	}
	void Package::DelItemSendMsg(UInt32 itemid, Player *player)
	{
		SYSMSG_SENDV(104, player, itemid);
		SYSMSG_SENDV(1004, player, itemid);		
	}

    UInt8 Package::GetItemCareer(UInt32 itemid, UInt8 bind)
    {
		ItemBase * item = FindItem(itemid, bind);
        if (item) {
            return item->GetCareer();
        }
        return 0;
    }

	bool Package::UseTaskItem(UInt32 id, UInt8 bind)
	{
		bool ret = false;
		UInt8 token = 0;
		if (IsEquipId(id) || GetItemSubClass(id) != Item_Task)
			ret = false;
		else if (GameAction()->IsTaskItemItem(id))
		{
			token = 2;
			ret = GameAction()->RunTaskItemUse(m_Owner, id);
		}
		else if (GameAction()->IsItemTaskItem(id))
		{
			token = 3;
			ret = GameAction()->RunItemTaskUse(m_Owner, id);
		}

		Stream st(0x33);
		st << id << token << static_cast<UInt8>(ret ? 1 : 0) << Stream::eos;
		m_Owner->send(st);

		return ret;
	}

    bool Package::FCMerge(UInt32 id, bool bind)
    {
        static struct {
            UInt32 sid; // start id
            UInt32 eid; // end id
            const char* nums; // numbuers of every id
            UInt32 tid; // target id
        } config[] = {
            {1014, 1019, "1,1,1,1,1,1",         1013},
            {1022, 1030, "1,1,1,1,1,1,1,1,1",   1021},
            {1033, 1039, "1,1,1,1,1,1,1",       1032},
            {1042, 1051, "1,1,1,1,1,1,1,1,1,1", 1041},
            {1054, 1058, "1,1,1,1,1",           1053},
            {1061, 1064, "1,1,1,1",             1060},
            {1067, 1073, "1,1,1,1,1,1,1",       1066},
            {1076, 1083, "1,1,1,1,1,1,1",       1075},
            {1086, 1095, "1,1,1,1,1,1,1,1,1,1", 1085},
            {1098, 1106, "1,1,1,1,1,1,1,1,1",   1097},
            {1109, 1115, "1,1,1,1,1,1,1",       1108},
            {1118, 1124, "1,1,1,1,1,1,1",       1117},
            {1304, 1312, "1,1,1,1,1,1,1,1,1",   1208},
            {1313, 1321, "1,1,1,1,1,1,1,1,1",   1228},
            {1322, 1330, "1,1,1,1,1,1,1,1,1",   1206},
            {1331, 1339, "1,1,1,1,1,1,1,1,1",   1302},
            {1340, 1348, "1,1,1,1,1,1,1,1,1",   1299},
            {1349, 1357, "1,1,1,1,1,1,1,1,1",   1303},
            {1358, 1366, "1,1,1,1,1,1,1,1,1",   1248},
            {1367, 1375, "1,1,1,1,1,1,1,1,1",   1298},
            {0, 0, NULL, 0},
        };

        int k = -1;
        int i = -1;
        while (true)
        {
            if (!config[i].sid)
                break;
            if (id >= config[i].sid && id <= config[i].eid) {
                k = i;
                break;
            }
            ++i;
        }

        if (k < 0)
            return false;

        StringTokenizer tk(config[k].nums, ",");
        if (!tk.count())
            return false;

        UInt8 j = 0;
        for (; j < tk.count(); ++j)
        {
            UInt32 id = config[i].sid+j;
            UInt16 num = atoi(tk[j].c_str());

            UInt16 rnum = GetItemNum(id, bind);
            if (rnum < num)
                return false;
        }

        if (config[i].eid != j + config[i].sid) {
            return false;
        }

        for (j = 0; j < tk.count(); ++j)
        {
            UInt32 id = config[i].sid+j;
            UInt16 num = atoi(tk[j].c_str());
            DelItem(id, num, bind);
        }
        AddItem(config[i].tid, 1, bind, false, FromFCMerge);
        return true;
    }

	UInt16 Package::GetItemNum(UInt32 id, bool bind)
	{
		if (IsEquipId(id))
			return 0;
		ItemBase * item = FindItem(id, bind);
		return item != NULL ? item->Count() : 0;
	}

	UInt16 Package::GetItemAnyNum(UInt32 id)
	{
		if (IsEquipId(id))
			return 0;
		ItemBase * bItem = FindItem(id, true);
		ItemBase * nBItem = FindItem(id, false);
		return (bItem != NULL ? bItem->Count() : 0) + (nBItem != NULL ? nBItem->Count() : 0);
	}

	UInt16 Package::GetNewUsedGrids(const GData::ItemBaseType& itemType, UInt16 num)
	{
		return (num + itemType.maxQuantity - 1) / itemType.maxQuantity;
	}

	UInt16 Package::GetNewUsedGrids(UInt32 id, UInt16 num)
	{
		if (IsEquipId(id))
			return num;
		else
		{
			const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
			if(itemType == NULL)
				return 0;
			return GetNewUsedGrids(*itemType, num);
		}
	}

	UInt16 Package::GetItemUsedGrids(UInt32 id, UInt16 num, bool bind)
	{
		if (num == 0 || IsEquipId(id)) return 0;
		ItemBase * item = FindItem(id, bind);
		if (item == NULL)
			return GetNewUsedGrids(id, num);
		UInt16 newTotal = item->Count() + num;
		UInt16 oNum = item->GetOverlayNum();
		UInt16 newGrids = (newTotal + oNum - 1) / oNum;
		return newGrids - item->Size();
	}

	bool Package::ExistItem(UInt32 id)
	{
		if (IsEquipId(id))
		{
			return GetEquip(id) != NULL;
		}
		else
		{
			if (GetItem(id, 0) == NULL)
				return GetItem(id, 1) != NULL;
			else
				return true;
		}
	}

	UInt32 Package::GetItemTypeId(UInt32 id)
	{
		if (!IsEquipId(id))
			return id;
		else
		{
			ItemBase* item = GetEquip(id);
			return item != NULL ? item->GetItemType().getId() : 0;
		}
	}

	void Package::SendPackageItemInfor()
	{
		ItemCont::iterator cit = m_Items.begin();
		Stream st(0x30);
		st << static_cast<UInt16>(0);
		UInt16 count = 0;
		for (; cit != m_Items.end(); ++cit)
		{
			ItemBase * item = cit->second;
			if(IsEquip(item->getClass()))
			{
				count ++;
				ItemEquip * equip = static_cast<ItemEquip *>(item);
				AppendEquipData(st, equip);
			}
			else
			{
				count++;
				AppendItemData(st, item);
			}
		}
		st.data<UInt16>(4) = count;
		st << Stream::eos;
		m_Owner->send(st);
	}

	void Package::AppendEquipData( Stream& st, ItemEquip * equip, bool hascount )
	{
		st << equip->getId() << static_cast<UInt8>(equip->GetBindStatus() ? 1 : 0);
		if(hascount)
			st << equip->Count();
		st << static_cast<UInt16>(equip->GetItemType().getId()) << equip->getItemEquipData().enchant;
		ItemEquipData& ied = equip->getItemEquipData();
		st << ied.sockets;
		for(UInt8 z = 0; z < ied.sockets; ++ z)
		{
			st << ied.gems[z];
		}
		ItemEquipAttr2& ea2 = equip->getEquipAttr2();
		ea2.appendAttrToStream(st);
	}

	void Package::AppendItemData( Stream& st, ItemBase * item )
	{
		st << item->getId() << static_cast<UInt8>(item->GetBindStatus() ? 1 : 0) << item->Count();
	}

	void Package::SendSingleEquipData(ItemEquip * equip)
	{
		Stream st(0x30);
		st << static_cast<UInt16>(1);
		AppendEquipData(st, equip);
		st << Stream::eos;
		m_Owner->send(st);
	}

	void Package::SendDelEquipData(ItemEquip * equip)
	{
		Stream st(0x30);
		st << static_cast<UInt16>(1) << equip->getId() << static_cast<UInt8>(equip->GetBindStatus() ? 1 : 0) << static_cast<UInt16>(0) << Stream::eos;
		m_Owner->send(st);
	}

	void Package::SendItemData(ItemBase * item)
	{
		Stream st(0x30);
		st << static_cast<UInt16>(1);
		AppendItemData(st, item);
		st << Stream::eos;
		m_Owner->send(st);
	}

	UInt8 Package::Enchant( UInt16 fighterId, UInt32 itemId, UInt8 type/*, bool protect*/ )
	{
		if (type > 3) return 2;
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL || equip->getClass() == Item_Ring || equip->getClass() == Item_Amulet)
			return 2;
        UInt32 item_enchant_l = ITEM_ENCHANT_L1;
        UInt8 quality = 0;
        if(equip->getClass() == Item_Trump)
        {
            item_enchant_l = TRUMP_ENCHANT_L1;
            quality = equip->getQuality() - 1;
        }

		ItemEquipData& ied = equip->getItemEquipData();
        if(ied.enchant >= ENCHANT_LEVEL_MAX)
            return 2;
		
		// UInt32 viplvl = this->m_Owner->getVipLevel();
		// const UInt8 enchant_max[] = {7, 7, 7, 7, 8, 10, 10, 10, 10, 11, 12};
        // if(ied.enchant >= enchant_max[viplvl])
        //     return 2;

		UInt32 amount = GObjectManager::getEnchantCost();  // enchant_cost[ied.enchant];
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 2;
		}
		bool isBound = equip->GetBindStatus();
		if(!DelItemAny(item_enchant_l + type, 1, &isBound))
		{
			return 2;
		}
		DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), item_enchant_l + type, 1, TimeUtil::Now());
		ConsumeInfo ci(EnchantEquipment,0,0);
		m_Owner->useTael(amount,&ci);
		// static UInt32 enchant_chance[] = {100, 90, 80, 60, 50, 40, 20, 10, 5, 2, 2, 2};
		if(uRand(1000) < GObjectManager::getEnchantChance(quality, ied.enchant)/*enchant_chance[ied.enchant]*/)
		{
			++ ied.enchant;
			DB().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", ied.enchant, equip->getId());
			if(ied.enchant >= 5)
				DBLOG().PushUpdateData("insert into enchant_histories (server_id, player_id, equip_id, template_id, enchant_level, enchant_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), equip->getId(), equip->GetItemType().getId(), ied.enchant, TimeUtil::Now());
			if(!equip->GetBindStatus() && isBound)
			{
				equip->DoEquipBind();
			}
			if(fgt != NULL)
			{
				fgt->setDirty();
				fgt->sendModification(0x21 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
			//if(World::_newYearStage > 0)
			//	GameAction()->onEnchant(m_Owner, ied.enchant);
			if(World::_actAvailable1 && ied.enchant >= 8 && gSpecialAward.getAwardSize(1) < 3)
			{
				UInt8 type1 = 1;
				GameMsgHdr hdr(0x1A3, WORKER_THREAD_WORLD, m_Owner, sizeof(type1));
				GLOBAL().PushMsg(hdr, &type1);
			}
			switch(ied.enchant)
			{
			case 8:
			case 10:
			case 11:
			case 12:
				SYSMSG_BROADCASTV(275, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId(), ied.enchant);
				break;
			}
			return 0;
		}
		if(type == 0 && ied.enchant >= 4)
		{
			ied.enchant --;
			DB().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", ied.enchant, equip->getId());
			DBLOG().PushUpdateData("insert into enchant_histories (server_id, player_id, equip_id, template_id, enchant_level, enchant_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), equip->getId(), equip->GetItemType().getId(), ied.enchant, TimeUtil::Now());
			if(fgt != NULL)
			{
				fgt->setDirty();
				fgt->sendModification(0x21 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
		}

		return 1;
	}

	UInt8 Package::OpenSocket( UInt16 fighterId, UInt32 itemId )
	{
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL)
			return 2;
		ItemEquipData& ied = equip->getItemEquipData();
		bool isBound = equip->GetBindStatus();
		if(ied.sockets >= 6)
			return 2;
		else if(ied.sockets >= 4)
		{
			if(!DelItemAny(ITEM_SOCKET_L3, 1, &isBound))
				return 2;
			DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_SOCKET_L3, 1, TimeUtil::Now());
		}
		else if(ied.sockets >= 2)
		{
			if(!DelItemAny(ITEM_SOCKET_L2, 1, &isBound))
				return 2;
			DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_SOCKET_L2, 1, TimeUtil::Now());
		}
		else
		{
			if(!DelItemAny(ITEM_SOCKET_L1, 1, &isBound))
				return 2;
			DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_SOCKET_L1, 1, TimeUtil::Now());
		}
		//if(World::_wday == 6)
		//{
		//	if(chance[ied.sockets] > 25)
		//		chance[ied.sockets] -= 25;
		//}
		if(uRand(100) < GObjectManager::getSocketChance(ied.sockets) /*chance[ied.sockets]*/)
			return 1;
		++ied.sockets;
		DB().PushUpdateData("UPDATE `equipment` SET `sockets` = %u WHERE `id` = %u", ied.sockets, equip->getId());
		if(!equip->GetBindStatus() && isBound)
			equip->DoEquipBind();
		if(fgt != NULL)
			fgt->sendModification(0x21 + pos, equip, false);
		else
			SendSingleEquipData(equip);
		return 0;
	}

	UInt8 Package::MergeGem( UInt32 gemId, UInt8 bindCount, bool protect, UInt32& ogid )
	{
		if (GetItemSubClass(gemId) != Item_Gem) return 2;
		UInt32 lvl = (gemId - 1) % 10;
		if(lvl >= 9) return 2;

		UInt8 unbindCount = 3 - bindCount;
		UInt32 amount = GObjectManager::getMergeCost(); // merge_cost[lvl];
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 2;
		}

		if(GetRestPackageSize() < 1)
		{
			m_Owner->sendMsgCode(0, 2016);
			return 2;
		}

		ItemBase * itemBind = NULL, * itemUnbind = NULL;
		if(bindCount > 0)
		{
			itemBind = FindItem(gemId, true);
			if(itemBind == NULL || itemBind->Count() < bindCount)
				return 2;
		}
		if(unbindCount > 0)
		{
			itemUnbind = FindItem(gemId, false);
			if(itemUnbind == NULL || itemUnbind->Count() < unbindCount)
				return 2;
		}
		ConsumeInfo ci(MergeGems,0,0);
		m_Owner->useTael(amount,&ci);
		bool isBound = bindCount > 0;
		if(protect && !DelItemAny(ITEM_GEM_PROTECT, 1, &isBound))
		{
			protect = false;
		}
		if(uRand(100) < GObjectManager::getMergeChance(lvl) /*merge_chance[lvl]*/)
		{
			if(bindCount > 0)
				DelItem(gemId, bindCount, true);
			if(unbindCount > 0)
				DelItem(gemId, unbindCount, false);
			ogid = gemId + 1;
			AddItem(ogid, 1, bindCount > 0 || isBound, false, FromMerge);
			if(World::_activityStage > 0)
				GameAction()->onMergeGem(m_Owner, lvl + 2, 1);
			return 0;
		}
		if(!protect)
		{
			if(bindCount > 0)
				DelItem(gemId, bindCount, true);
			if(unbindCount > 0)
				DelItem(gemId, unbindCount, false);
		}
		return 1;
	}

	UInt8 Package::AttachGem( UInt16 fighterId, UInt32 itemId, UInt32 gemId, bool bind )
	{
		if (GetItemSubClass(gemId) != Item_Gem) return 1;
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL)
			return 1;

        // 坚韧宝石可以镶嵌任何装备
        if(gemId < 5131)
        {
            switch(equip->getClass())
            {
            case Item_Weapon:
                if((gemId > 5060 && gemId <= 5070) || gemId > 5090)
                    return 1;
                break;
            case Item_Armor1:
            case Item_Armor2:
                if((gemId > 5050 && gemId <= 5060) || gemId > 5090)
                    return 1;
                break;
            case Item_Armor3:
                if((gemId > 5050 && gemId <= 5070) || gemId > 5090)
                    return 1;
                break;
            case Item_Armor4:
                if((gemId > 5050 && gemId <= 5070) || gemId > 5090)
                    return 1;
                break;
            case Item_Armor5:
                if((gemId > 5050 && gemId <= 5070) || (gemId > 5090 && gemId <= 5120))
                    return 1;
                break;
            case Item_Ring:
            case Item_Amulet:
                if((gemId > 5050 && gemId <= 5070) || gemId > 5120)
                    return 1;
                break;
            default:
                return 1;
            }
        }

		ItemEquipData& ied = equip->getItemEquipData();
		UInt8 fempty = 0xFF;
		UInt32 gt = (gemId - 1) / 10;
		for(UInt8 i = 0; i < ied.sockets; ++ i)
		{
			if(ied.gems[i] == 0)
			{
				if(fempty == 0xFF)
					fempty = i;
			}
			else if(static_cast<UInt32>((ied.gems[i] - 1) / 10) == gt)
				return 1;
		}
		if(fempty == 0xFF)
		{
			UInt8 r = OpenSocket(fighterId, itemId);
			if(r > 0)
				return 2;
			fempty = ied.sockets - 1;
		}
		if(!DelItem(gemId, 1, bind))
			return 1;
		ied.gems[fempty] = gemId;
		DB().PushUpdateData("UPDATE `equipment` SET `socket%u` = %u WHERE `id` = %u", fempty + 1, ied.gems[fempty], equip->getId());
		if(bind)
			equip->DoEquipBind();
		if(fgt != NULL)
		{
			fgt->setDirty();
			fgt->sendModification(0x21 + pos, equip, false);
		}
		else
			SendSingleEquipData(equip);
		return 0;
	}

	UInt8 Package::DetachGem( UInt16 fighterId, UInt32 itemId, UInt8 pos, UInt8 protect )
	{
		-- pos;
		if (pos > 5) return 2;
		UInt32 amount = GObjectManager::getDetachCost();
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 2;
		}
		Fighter * fgt = NULL;
		UInt8 pos2 = 0;
		ItemEquip * equip = FindEquip(fgt, pos2, fighterId, itemId);
		if(equip == NULL)
			return 2;
		ItemEquipData& ied = equip->getItemEquipData();
		if(pos >= ied.sockets || ied.gems[pos] == 0)
			return 2;
		if(GetRestPackageSize() < 1)
		{
			m_Owner->sendMsgCode(0, 2016);
			return 2;
		}
		bool bind = false;
        if(!protect)
        {
            if(DelItem(ITEM_DETACH_RUNE, 1, true))
                bind = true;
            else if(!DelItem(ITEM_DETACH_RUNE, 1, false))
                return 2;
            DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_DETACH_RUNE, 1, TimeUtil::Now());
        }
        else
        {
            if(DelItem(ITEM_DETACH_PROTECT, 1, true))
                bind = true;
            else if(!DelItem(ITEM_DETACH_PROTECT, 1, false))
                return 2;
            DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_DETACH_PROTECT, 1, TimeUtil::Now());
        }
		if(!AddItem(ied.gems[pos], 1, bind | equip->GetBindStatus(), false, FromDetachGem))
			return 2;
        ConsumeInfo ci(DetachGems,0,0);
		m_Owner->useTael(amount,&ci);
		if(protect == 0 && uRand(100) < 75)
		{
			if(fgt != NULL)
				fgt->removeEquip(pos2, equip, ToDetachGemDesdroy);
			else
				DelEquip2(equip, ToDetachGemDesdroy);
			return 1;
		}
		ied.gems[pos] = 0;
		DB().PushUpdateData("UPDATE `equipment` SET `socket%u` = 0 WHERE `id` = %u", pos + 1, equip->getId());
		if(!equip->GetBindStatus() && bind)
			equip->DoEquipBind();
		if(fgt != NULL)
		{
			fgt->setDirty();
			fgt->sendModification(0x21 + pos2, equip, false);
		}
		else
			SendSingleEquipData(equip);
		return 0;
	}

	UInt8 Package::Split( UInt32 itemId, UInt32& enchantId, UInt8& count, /*bool protect,*/ bool silence )
	{
		count = 0;
		if (!IsEquipId(itemId)) return 2;
		ItemBase * item = FindItem(itemId);
		if(item == NULL || item->getQuality() < 2 || item->getReqLev() < 1)
			return 2;
		UInt8 q = item->getQuality() - 2;
		UInt32 amount = GObjectManager::getSplitCost();   // split_cost[q][lv];
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 2;
		}
		ConsumeInfo ci(SplitEquipment,0,0);
		m_Owner->useTael(amount, &ci);
		bool isBound = item->GetBindStatus();
        UInt32 chance_low = GObjectManager::getSplitChance(q, 0);  // split_chance[q][lv][0];
		UInt32 chance_high = GObjectManager::getSplitChance(q, 1);  // split_chance[q][lv][1];
		UInt8 got = 0;
#if 0
		if(protect)
		{
			if(uRand(chance_low + chance_high) < chance_low)
				got = 1;
			else
				got = 2;
		}
		else
#endif
		{
			UInt32 r = uRand(100);
			if(r < chance_low)
				got = 1;
			else
			{
				r -= chance_low;
				if(r < chance_high)
					got = 2;
			}
		}
		if(got)
		{
			enchantId = ITEM_ENCHANT_L1 + got - 1;
			isBound |= item->GetBindStatus();
			DelEquip2(static_cast<ItemEquip *>(item), ToSplit);
		    count = 1;
			AddItem(enchantId, count, isBound, silence, FromSplit);
			return 0;
		}
		DelEquip2(static_cast<ItemEquip *>(item), ToSplit);
		return 1;
	}

	static UInt32 getRandomEquipment( UInt8 l, UInt8 q, UInt8 t = 0 )
	{
		static UInt32 EquipSets[11][5][8] = {
			/*  0*/ {{0, 0}, {1, 14}},
			/* 10*/ {{21, 14}, {41, 14}, {61, 14}},
			/* 20*/ {{81, 14}, {101, 14}, {121, 14}},
			/* 30*/ {{141, 14}, {161, 14}, {181, 14}},
			/* 40*/ {{201, 14}, {221, 14}, {241, 14}, {261, 14, 281, 14, 301, 14, 321, 14}},
			/* 50*/ {{0, 0}, {421, 14}, {441, 14}, {461, 14, 481, 14, 501, 14, 521, 14}, {1521, 14, 1541, 14}},
			/* 60*/ {{0, 0}, {621, 14}, {641, 14}, {661, 14, 681, 14, 701, 14, 721, 14}},
			/* 70*/ {{0, 0}, {821, 14}, {841, 14}, {861, 14, 881, 14, 901, 14, 921, 14}, {1561, 14, 1581, 14}},
			/* 80*/ {{0, 0}, {1021, 14}, {1041, 14}, {1061, 14, 1081, 14, 1101, 14, 1121, 14}, {1601, 14, 1621, 14}},
			/* 90*/ {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {1681, 14, 1701, 14}},
			/*100*/ {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {1721, 14, 1741, 14}},
		};

		static UInt32 specialSets[5][8] = {
			/* 55 p */ {1401, 14},
			/* 65 p */ {1501, 14},
			/* 70 p */ {1641, 14},
			/* 90 p */ {1661, 14},
			/* 100p */ {1761, 14},
		};

		if(l > 15 || q > 4)
			return 0;
		UInt32 * data;
		if(l > 10)
			data = specialSets[l - 11];
		else
			data = EquipSets[l][q];
		int c = 0;
		while(c < 4 && data[c * 2] != 0) ++ c;
		if(c == 0)
			return 0;
		UInt32 r = uRand(c);
		switch(t)
		{
		case 1:
			return data[r * 2] + 7 + uRand(data[r * 2 + 1] / 2);
		case 2:
			return data[r * 2] + uRand(data[r * 2 + 1] / 2);
		default:
			break;
		}
		return data[r * 2] + uRand(data[r * 2 + 1]);
	}

	UInt8 Package::Exchange( UInt32 * itemId, UInt32& resultId )
	{
		UInt8 quality = 0;
		UInt16 level = 0;
		ItemBase * item[5] = {NULL};
		bool bind = false;
		for(int i = 0; i < 5; ++ i)
		{
			if (!IsEquipId(itemId[i])) return 2;
			item[i] = FindItem(itemId[i]);
			if (item[i] == NULL) return 2;
			if (i == 0)
			{
				quality = item[i]->getQuality();
				if(quality > 2)
					return 1;
				level = item[i]->getReqLev();
				if(level < 10)
					return 1;
			}
			else if (quality != item[i]->getQuality() || level != item[i]->getReqLev())
			{
				return 1;
			}
			for(int j = 0; j < i; ++ j)
			{
				if(item[i] == item[j])
					return 1;
			}
			bind |= item[i]->GetBindStatus();
		}
		UInt8 lv = (level + 5) / 10;
		UInt32 amount = exchange_cost[quality - 1][lv];
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 1;
		}
		ConsumeInfo ci(ExchangeEquipment,0,0);
		m_Owner->useTael(amount,&ci);
		for(int i = 0; i < 5; ++ i)
		{
			DelEquip2(static_cast<ItemEquip *>(item[i]), ToExchange);
		}
		UInt32 targetId = getRandomEquipment(lv, quality);
		if(targetId == 0)
			return 1;
		ItemBase * itemTarget = AddEquip(targetId, bind, false, FromExchange);
		if(itemTarget == NULL)
			return 1;
		resultId = itemTarget->getId();
		return 0;
	}

	UInt8 Package::ExchangeSet( UInt32 * itemId, UInt8 type, UInt32& resultId )
	{
		UInt8 quality = 0;
		UInt16 level = 0;
		ItemBase * item[3] = {NULL};
		bool bind = false;
		for(int i = 0; i < 3; ++ i)
		{
			if (!IsEquipId(itemId[i])) return 2;
			item[i] = FindItem(itemId[i]);
			if (item[i] == NULL) return 2;
			UInt32 id = item[i]->GetItemType().getId();
			UInt8 _q;
			UInt16 _l;
			if(id > 1400 && id < 1420)
			{
				_q = 4;
				_l = 110;
			}
			else if(id > 1500 && id < 1520)
			{
				_q = 4;
				_l = 120;
			}
			else if(id > 1640 && id < 1660)
			{
				_q = 4;
				_l = 130;
			}
			else if(id > 1660 && id < 1680)
			{
				_q = 4;
				_l = 140;
			}
			else if(id > 1760 && id < 1780)
			{
				_q = 4;
				_l = 150;
			}
			else
			{
				_q = item[i]->getQuality();
				_l = item[i]->getReqLev();
			}
			if (i == 0)
			{
				quality = _q;
				level = _l;
			}
			else if (quality != _q || level != _l)
			{
				return 1;
			}
			for(int j = 0; j < i; ++ j)
			{
				if(item[i] == item[j])
					return 1;
			}
			bind |= item[i]->GetBindStatus();
		}
		UInt8 lv = (level + 5) / 10;
		UInt32 amount = exchange_set_cost;
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 1;
		}
		ConsumeInfo ci(ExchangeEquipment,0,0);
		m_Owner->useTael(amount,&ci);
		for(int i = 0; i < 3; ++ i)
		{
			DelEquip2(static_cast<ItemEquip *>(item[i]), ToExchangeSet);
		}
		UInt32 targetId = getRandomEquipment(lv, quality - 1, type + 1);
		if(targetId == 0)
			return 1;
		ItemBase * itemTarget = AddEquip(targetId, bind, false, FromExchangeSet);
		if(itemTarget == NULL)
			return 1;
		resultId = itemTarget->getId();
		return 0;
	}

    UInt8 Package::BatchMergeGem(UInt16 gemId, UInt16 unbindCount, UInt16 bindCount, UInt8 protect, UInt16& unbindGemsOut, UInt16& bindGemsOut)
    {
		UInt16 protectUnbindNum = GetItemNum(ITEM_GEM_PROTECT, false);
		UInt16 protectBindNum = GetItemNum(ITEM_GEM_PROTECT, true);
		if(protect && protectUnbindNum == 0 && protectBindNum == 0)
			return 1;

		UInt16 protectBindUsed = 0, protectUnbindUsed = 0;
		UInt32 coinAmount = 0;
		UInt32 myCoin = m_Owner->getTael();

		URandom& rnd = static_cast<BaseThread *>(Thread::current())->uRandom;
		UInt8 result = 0;

        if (GetItemSubClass(gemId) != Item_Gem)
            return 3;
        UInt32 lvl = (gemId - 1) % 10;

        if(bindCount > 0 && GetItemNum(gemId, true) < bindCount)
            return 3;
        if(unbindCount > 0 && GetItemNum(gemId, false) < unbindCount)
            return 3;

        UInt32 bindUsed = 0, unbindUsed = 0;
        unbindGemsOut = 0;
        bindGemsOut = 0;

        while(result == 0 && bindCount >= 3)
        {
            UInt32 amount = GObjectManager::getMergeCost();    // merge_cost[lvl];
            coinAmount += amount;
            if(coinAmount > myCoin)
            {
                coinAmount -= amount;
                result = 2;
                break;
            }
            if(protect)
            {
                if(protectBindUsed >= protectBindNum)
                {
                    if(protectUnbindUsed >= protectUnbindNum)
                    {
                        coinAmount -= amount;
                        result = 1;
                        break;
                    }
                    ++ protectUnbindUsed;
                }
                else
                    ++ protectBindUsed;
            }

            if(rnd(100) < merge_chance[lvl])
            {
                bindUsed += 3;
                bindCount -= 3;
                ++ bindGemsOut;
            }
            else if(!protect)
            {
                bindCount -= 3;
            }
        }

        while(result == 0 && unbindCount >= 3)
        {
            UInt32 amount = GObjectManager::getMergeCost();        // merge_cost[lvl];
            coinAmount += amount;
            if(coinAmount > myCoin)
            {
                coinAmount -= amount;
                result = 2;
                break;
            }
            if(protect)
            {
                if(protectUnbindUsed >= protectUnbindNum)
                {
                    if(protectBindUsed >= protectBindNum)
                    {
                        coinAmount -= amount;
                        result = 1;
                        break;
                    }
                    ++ protectBindUsed;
                }
                else
                    ++ protectUnbindUsed;
            }
            if(rnd(100) < merge_chance[lvl])
            {
                unbindUsed += 3;
                unbindCount -= 3;
                ++ unbindGemsOut;
            }
            else if(!protect)
            {
                unbindCount -= 3;
            }
        }

        while(bindCount + unbindCount >= 3 && result == 0)
        {
            UInt32 amount = GObjectManager::getMergeCost();      // merge_cost[lvl];
            coinAmount += amount;
            if(coinAmount > myCoin)
            {
                coinAmount -= amount;
                result = 2;
                break;
            }
            if(protect)
            {
                if(protectBindUsed >= protectBindNum)
                {
                    if(protectUnbindUsed >= protectUnbindNum)
                    {
                        coinAmount -= amount;
                        result = 1;
                        break;
                    }
                    ++ protectUnbindUsed;
                }
                else
                    ++ protectBindUsed;
            }
            if(rnd(100) < merge_chance[lvl])
            {
                bindUsed += bindCount;
                unbindUsed += 3 - bindCount;

                unbindCount -= 3 - bindCount;
                bindCount = 0;

                ++ bindGemsOut;
                break;
            }
            else if(!protect)
            {
                unbindCount -= 3 - bindCount;
                bindCount = 0;
            }
        }

        if(bindUsed > 0)
            DelItem(gemId, bindUsed, true);
        if(unbindUsed > 0)
            DelItem(gemId, unbindUsed, false);
        if(bindGemsOut > 0)
        {
            AddItem(gemId + 1, bindGemsOut, true);
            if(World::_activityStage > 0)
                GameAction()->onMergeGem(m_Owner, lvl + 2, bindGemsOut);
        }
        if(unbindGemsOut > 0)
        {
            AddItem(gemId + 1, unbindGemsOut, false);
            if(World::_activityStage > 0)
                GameAction()->onMergeGem(m_Owner, lvl + 2, unbindGemsOut);
        }

		ConsumeInfo ci(MergeGems,0,0);
		m_Owner->useTael(coinAmount, &ci);
		if(protectBindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectBindUsed, true);
		if(protectUnbindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectUnbindUsed, false);

		return result;
    }

#if 0
	UInt8 Package::BatchMergeGem( std::map<UInt16, UInt32, _GemCompare >& gems, std::vector<UInt32>& gemsOut )
	{
		if(gems.empty())
			return 0;
		UInt16 protectUnbindNum = GetItemNum(ITEM_GEM_PROTECT, false);
		UInt16 protectBindNum = GetItemNum(ITEM_GEM_PROTECT, true);
		if(protectUnbindNum == 0 && protectBindNum == 0)
			return 1;
		UInt16 protectBindUsed = 0, protectUnbindUsed = 0;
		UInt32 coinAmount = 0;
		UInt32 myCoin = m_Owner->getTael();

		URandom& rnd = static_cast<BaseThread *>(Thread::current())->uRandom;
		UInt8 result = 0;
		for(std::map<UInt16, UInt32, _GemCompare >::iterator it = gems.begin(); result == 0 && it != gems.end(); ++ it)
		{
			UInt32 gemId = it->first;
			if (GetItemSubClass(gemId) != Item_Gem) continue;
			UInt32 lvl = (gemId - 1) % 10;

			UInt32 bindCount = it->second >> 16;
			if(bindCount > 0 && GetItemNum(gemId, true) < bindCount)
				continue;
			UInt32 unbindCount = it->second & 0xFFFF;
			if(unbindCount > 0 && GetItemNum(gemId, false) < unbindCount)
				continue;

			UInt32 protectThis = 0, bindUsed = 0, unbindUsed = 0;
			UInt32 bindGot = 0, unbindGot = 0;

			while(result == 0 && bindCount >= 3)
			{
				UInt32 amount = GObjectManager::getMergeCost();    // merge_cost[lvl];
				coinAmount += amount;
				if(coinAmount > myCoin)
				{
					gemsOut.push_back(0x8000 | (gemId + 1) | protectThis);
					coinAmount -= amount;
					result = 2;
					break;
				}
				if(protectBindUsed >= protectBindNum)
				{
					if(protectUnbindUsed >= protectUnbindNum)
					{
						gemsOut.push_back(0x8000 | (gemId + 1) | protectThis);
						coinAmount -= amount;
						result = 1;
						break;
					}
					++ protectUnbindUsed;
				}
				else
					++ protectBindUsed;
				protectThis += 0x10000;
				if(rnd(100) < merge_chance[lvl])
				{
					gemsOut.push_back(0x8000 | (gemId + 1) | protectThis);
					bindUsed += 3;
					bindCount -= 3;
					++ bindGot;
					protectThis = 0;
				}
			}

			while(result == 0 && unbindCount >= 3)
			{
				UInt32 amount = GObjectManager::getMergeCost();        // merge_cost[lvl];
				coinAmount += amount;
				if(coinAmount > myCoin)
				{
					gemsOut.push_back((gemId + 1) | protectThis);
					coinAmount -= amount;
					result = 2;
					break;
				}
				if(protectUnbindUsed >= protectUnbindNum)
				{
					if(protectBindUsed >= protectBindNum)
					{
						gemsOut.push_back((gemId + 1) | protectThis);
						coinAmount -= amount;
						result = 1;
						break;
					}
					++ protectBindUsed;
				}
				else
					++ protectUnbindUsed;
				protectThis += 0x10000;
				if(rnd(100) < merge_chance[lvl])
				{
					gemsOut.push_back((gemId + 1) | protectThis);
					unbindUsed += 3;
					unbindCount -= 3;
					++ unbindGot;
					protectThis = 0;
				}
			}

			if(bindCount + unbindCount >= 3) while(result == 0)
			{
				UInt32 amount = GObjectManager::getMergeCost();      // merge_cost[lvl];
				coinAmount += amount;
				if(coinAmount > myCoin)
				{
					gemsOut.push_back(0x8000 | (gemId + 1) | protectThis);
					coinAmount -= amount;
					result = 2;
					break;
				}
				if(protectBindUsed >= protectBindNum)
				{
					if(protectUnbindUsed >= protectUnbindNum)
					{
						gemsOut.push_back(0x8000 | (gemId + 1) | protectThis);
						coinAmount -= amount;
						result = 1;
						break;
					}
					++ protectUnbindUsed;
				}
				else
					++ protectBindUsed;
				protectThis += 0x10000;
				if(rnd(100) < merge_chance[lvl])
				{
					gemsOut.push_back(0x8000 | (gemId + 1) | protectThis);
					bindUsed += bindCount;
					unbindUsed += 3 - bindCount;
					bindCount -= bindUsed;
					unbindCount -= unbindUsed;
					++ bindGot;
					break;
				}
			}
			if(bindUsed > 0)
				DelItem(gemId, bindUsed, true);
			if(unbindUsed > 0)
				DelItem(gemId, unbindUsed, false);
			if(bindGot > 0)
			{
				AddItem(gemId + 1, bindGot, true);
				if(World::_activityStage > 0)
					GameAction()->onMergeGem(m_Owner, lvl + 2, bindGot);
			}
			if(unbindGot > 0)
			{
				AddItem(gemId + 1, unbindGot, false);
				if(World::_activityStage > 0)
					GameAction()->onMergeGem(m_Owner, lvl + 2, unbindGot);
			}
		}

		ConsumeInfo ci(MergeGems,0,0);
		m_Owner->useTael(coinAmount, &ci);
		if(protectBindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectBindUsed, true);
		if(protectUnbindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectUnbindUsed, false);

		return result;
	}
#endif

	UInt8 Package::ActivateAttr( UInt16 fighterId, UInt32 itemId, UInt32 itemId2 )
	{
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL) return 1;

		UInt8 q = equip->getQuality();
		if(q < 3) return 1;

		ItemEquipData& ied = equip->getItemEquipData();
		if(ied.extraAttr2.type3 > 0) return 1;

		ItemEquip * equip2 = GetEquip(itemId2);
		if(equip2 == NULL) return 1;

		UInt8 lv = (equip->getReqLev() + 5) / 10;
		if(equip->getQuality() != equip2->getQuality() || lv != (equip2->getReqLev() + 5) / 10) return 1;

		q -= 3;
		if(m_Owner->getTael() < activate_cost[lv][q])
		{
			m_Owner->sendMsgCode(0, 2009);
			return 1;
		}

		bool isBound = equip->GetBindStatus();
		if(!DelItemAny(ITEM_ACTIVATE_ATTR, 1, &isBound)) return 1;

		isBound |= equip2->GetBindStatus();
		DelEquip2(equip2, ToActivateAttr);

		m_Owner->useTael(activate_cost[lv][q]);
		if(lv != _lastActivateLv || q != _lastActivateQ)
		{
			_lastActivateLv = lv;
			_lastActivateQ = q;
			_lastActivateCount = 0;
		}
		if(GameAction()->getActivateAttrResult(_lastActivateCount, q))
		{
			_lastActivateCount = 0;
			ItemEquipData& ied = equip->getItemEquipData();
			UInt8 protect = 0;
			UInt8 c = 1;
			if(ied.extraAttr2.type1 > 0)
			{
				protect |= 1;
				c = 2;
				if(ied.extraAttr2.type2 > 0)
				{
					protect |= 2;
					c = 3;
				}
			}
			UInt8 types[3];
			Int16 values[3];
			types[0] = ied.extraAttr2.type1;
			values[0] = ied.extraAttr2.value1;
			types[1] = ied.extraAttr2.type2;
			values[1] = ied.extraAttr2.value2;
			types[2] = ied.extraAttr2.type3;
			values[2] = ied.extraAttr2.value3;
            UInt8 crr = 0;
            const GData::ItemBaseType * itype = GData::itemBaseTypeManager[itemId];
            if(itype)
                crr = itype->career;
			getRandomAttr2(lv, crr, q, c, protect, types, values);
			if(!equip->GetBindStatus() && isBound)
				equip->DoEquipBind();
			ApplyAttr2(equip, types, values);
			if(fgt != NULL)
			{
				fgt->setDirty();
				fgt->sendModification(0x21 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
			return 0;
		}
		++ _lastActivateCount;
		return 1;
	}

	UInt8 Package::Forge( UInt16 fighterId, UInt32 itemId, /*UInt8 t,*/ UInt8 * types, Int16 * values, UInt8 protect )
	{
		// if (t > 2) return 2;
		UInt32 amount = GObjectManager::getForgeCost();  // forge_cost;
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 2009);
			return 1;
		}

		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if (equip == NULL) return 2;
		bool isBound = equip->GetBindStatus();
		switch(equip->getQuality())
		{
		case 3:
		case 4:
		case 5:
			if(!DelItemAny(ITEM_FORGE_L1, 1, &isBound))
				return 2;
			break;
		default:
			return 2;
		}
        DBLOG().PushUpdateData("insert into `item_histories` (`server_id`, `player_id`, `item_id`, `item_num`, `use_time`) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_FORGE_L1, 1, TimeUtil::Now());
		UInt8 lv = (equip->getReqLev() + 5) / 10;
		UInt8 q = equip->getQuality() - 3;
		if(protect)
		{
			UInt16 c = 0;
			if(protect & 1)
				++ c;
			if(protect & 2)
				++ c;
			if(protect & 4)
				++ c;
			if(!DelItemAny(ITEM_FORGE_PROTECT, c))
				protect = 0;
		}
		ItemEquipData& ied = equip->getItemEquipData();
		types[0] = ied.extraAttr2.type1;
		values[0] = ied.extraAttr2.value1;
		types[1] = ied.extraAttr2.type2;
		values[1] = ied.extraAttr2.value2;
		types[2] = ied.extraAttr2.type3;
		values[2] = ied.extraAttr2.value3;
		ConsumeInfo ci(ForgeEquipment,0,0);
		m_Owner->useTael(amount,&ci);
        UInt8 crr = 0;
		const GData::ItemBaseType * itype = GData::itemBaseTypeManager[itemId];
		if(itype)
            crr = itype->career;
		getRandomAttr2(lv, crr, q, ied.extraAttr2.getCount(), protect, types, values);

		ApplyAttr2(equip, types, values);
		if(!equip->GetBindStatus() && isBound)
			equip->DoEquipBind();
		if(fgt != NULL)
		{
			fgt->setDirty();
			fgt->sendModification(0x21 + pos, equip, false);
		}
		else
			SendSingleEquipData(equip);
		return 0;
	}

	bool Package::TryAddItem( ItemBase * item, UInt16 num )
	{
		UInt16 cur = m_Size;
		UInt16 oldq = item->Size(), newq = item->Size(item->Count() + num);
		cur = cur - oldq + newq;
		if(cur > m_Owner->getPacksize())
			return false;
		if(!item->IncItem(num))
			return false;
		m_Size = cur;
		return true;
	}

	bool Package::TryDelItem( ItemBase * item, UInt16 num )
	{
		if(item->Count() < num)
			return false;
		UInt16 cur = m_Size;
		UInt16 oldq = item->Size(), newq = item->Size(item->Count() - num);
		cur = cur - oldq + newq;
		if(!item->DecItem(num))
			return false;
		m_Size = cur;

		return true;
	}

	void Package::ApplyAttr2( ItemEquip * equip, UInt8 * types, Int16 * values )
	{
		if(equip == NULL)
			return;
		ItemEquipData& ied = equip->getItemEquipData();
		ied.extraAttr2.type1 = types[0];
		ied.extraAttr2.type2 = types[1];
		ied.extraAttr2.type3 = types[2];
		ied.extraAttr2.value1 = values[0];
		ied.extraAttr2.value2 = values[1];
		ied.extraAttr2.value3 = values[2];
		DB().PushUpdateData("UPDATE `equipment` SET `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u", types[0], values[0], types[1], values[1], types[2], values[2], equip->getId());
	}

	UInt32 Package::GetItemIdByName(const std::string& name)
	{
		const GData::ItemBaseType * itemBaseType = GData::itemBaseTypeNameManager[name];
		return itemBaseType == NULL ? 0 : itemBaseType->getId();
	}

	const GData::ItemBaseType * Package::GetItemBaseType(UInt32 typeId)
	{
		return GData::itemBaseTypeManager[typeId];
	}

	const GData::ItemBaseType * Package::GetItemBaseType(const std::string& name)
	{
		return GData::itemBaseTypeNameManager[name];
	}

	void Package::AutoRegen( Fighter * fighter )
	{
		if(fighter->getCurrentHP() == 0)
		{
			SYSMSG_SENDV(190, m_Owner);
			return;
		}
		GameAction()->RunAutoRegen(m_Owner, fighter);
	}

	bool Package::DelItemAny( UInt32 id, UInt16 num, bool * hasBind )
	{
		if(num == 0)
			return true;
		bool bind;
		if(hasBind == NULL)
			bind = true;
		else
		{
			bind = *hasBind;
			*hasBind = false;
		}
		ItemBase * item = FindItem(id, bind);
		UInt16 bcount;
		if(item != NULL)
		{
			bcount = item->Count();
			if(bind && bcount > 0 && hasBind != NULL)
				*hasBind = true;
			if(bcount >= num)
			{
				DelItem2(item, num);
				return true;
			}
		}
		else
			bcount = 0;

		ItemBase * item2 = FindItem(id, !bind);
		if(item2 != NULL)
		{
			UInt16 ucount = item2->Count();
			if(bcount + ucount >= num)
			{
				if(item != NULL && bcount > 0)
					DelItem2(item, bcount);
				DelItem2(item2, num - bcount);
				if(!bind && hasBind != NULL)
					*hasBind = true;
				return true;
			}
		}

		return false;
	}

	ItemEquip * Package::FindEquip( Fighter *& fgt, UInt8& pos, UInt16 fgtId, UInt32 id )
	{
		if(!IsEquipId(id))
			 return NULL;
		if(fgtId == 0)
			return static_cast<ItemEquip *>(FindItem(id));
		fgt = m_Owner->findFighter(fgtId);
		if(fgt == NULL)
			return NULL;
		return fgt->findEquip(id, pos);
	}

	void Package::ItemNotify( UInt32 id, UInt16 num )
	{
		if(IsEquipId(id))
		{
			SYSMSG_SENDV(103, m_Owner, id);
			SYSMSG_SENDV(1003, m_Owner, id);
		}
		else
		{
			SYSMSG_SENDV(102, m_Owner, id, num);
			SYSMSG_SENDV(1002, m_Owner, id, num);
		}
	}
}
