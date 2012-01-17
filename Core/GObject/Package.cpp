#include "Config.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "MsgID.h"
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
#include "GData/SkillTable.h"
#include "GData/Money.h"
#include "GData/Store.h"
#include "GData/EUpgradeTable.h"
#include "AttainMgr.h"
#include "HoneyFall.h"

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
#define TRUMP_LORDER_ITEM       517      // 玲珑冰晶
#define MAX_TRUMP_LORDER_ITEM   9        // 最高阶

namespace GObject
{
#define EQUIPTYPE_EQUIP 0
#define EQUIPTYPE_TRUMP 1

    UInt16 getRandOEquip(UInt8 lvl)
    {
        static const UInt16* equips[] = {OEquip50, OEquip60, OEquip70, OEquip80};

        UInt16 equipid = 0;

        if (lvl < 50)
            lvl = 50;
        if (lvl > 80)
            lvl = 80;

        lvl -= 50;
        lvl /= 10;

        URandom rnd(time(NULL));
        UInt8 idx = rnd(24);
        equipid = equips[lvl][idx];
        return equipid;
    }

    UInt16 getRandPEquip(UInt8 lvl)
    {
        static const UInt16* equips[] = {PEquip40, };

        UInt16 equipid = 0;

        if (lvl < 40)
            lvl = 40;
        if (lvl > 40)
            lvl = 40;

        lvl -= 40;
        lvl /= 10;

        URandom rnd(time(NULL));
        UInt8 idx = rnd(24);
        equipid = equips[lvl][idx];
        return equipid;
    }


    static UInt32  gem[] = { 5001, 5011, 5021, 5031, 5041, 5051, 5061, 5071, 5081, 5091, 5101, 5111, 5121, 5131, 5141 };
    UInt32 getRandGem(UInt8 lev)
    {
        URandom rnd(time(NULL));
        UInt8 idx = rnd(15);

        if(lev > 1 && lev <= 9)
            return gem[idx] + lev;

        return gem[idx];
    }

#if 0
	static UInt32 enchant_cost[] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000};
	static UInt32 merge_cost[] = {50, 100, 150, 200, 250, 300, 350, 400, 500};
	static UInt32 detach_cost = 500;
	static UInt32 split_cost[4][11] = {{1000, 1000, 2000, 3000, 4000, 5000, 6000, 8000, 10000},
		{2000, 2000, 3000, 4000, 5000, 6000, 8000, 10000, 15000},
		{0, 0, 0, 0, 6000, 8000, 10000, 15000, 20000, 25000, 40000},
		{0, 0, 0, 0, 8000, 10000, 15000, 20000, 25000, 40000, 60000}};
	static UInt32 forge_cost = 50000;
	static UInt32 split_count[11][2] = {{1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 2}, {1, 2}, {1, 2}, {1, 3}, {1, 3}, {1, 3}, {1, 3}};
	static UInt32 split_chance[4][11][2] = {
		{{0, 15}, {0, 15}, {0, 20}, {0, 25}, {0, 30}, {0, 30}, {0, 35}, {0, 40}, {0, 40}},
		{{15, 3}, {15, 3}, {20, 5}, {20, 8}, {15, 15}, {15, 15}, {20, 15}, {20, 20}, {20, 20}},
		{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {25, 5}, {20, 10}, {20, 15}, {25, 15}, {25, 15}, {25, 15}, {25, 15}},
		{{0, 0}, {0, 0}, {0, 0}, {0, 0}, {10, 20}, {10, 20}, {10, 25}, {10, 30}, {10, 40}, {10, 40}, {10, 40}}
	};
#endif
	static UInt32 exchange_cost[2][10] = {{2000, 2000, 5000, 20000, 30000}, {3000, 3000, 10000, 25000, 35000, 40000, 50000, 80000, 100000}};
	static UInt32 exchange_set_cost = 100000;
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
	static void getRandomAttr2(UInt8 lv, UInt8 crr, UInt8 q, int c, UInt8 mask, UInt8 * t, Int16 * v, UInt8 equip_t = EQUIPTYPE_EQUIP)
    {
        switch(q)
        {
        case 0:
        case 1:
            if(c < 1)
                c = 1;
            break;
        case 2:
            if(c < 2)
                c = 2;
            break;
        default:
            if(c < 3)
                c = 3;
            break;
        }

        if(lv < 0)
            return;

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
                                UInt32 tmp = uRand(100) + 1;
                                UInt32 factor = GObjectManager::getAttrDics(q, k) + static_cast<float>(dics * tmp) / 100;
                                if(equip_t == EQUIPTYPE_EQUIP)
                                    v[i] = GObjectManager::getAttrMax(lv, t[i]-1, q, crr)*factor;
                                else if(equip_t == EQUIPTYPE_TRUMP)
                                    v[i] = GObjectManager::getAttrTrumpMax(lv, t[i]-1, q, crr)*factor;
                                break;
                            }
                        }
                        //attrInfo[lv][t[i] - 1][q][j];
                        types[j] = 0;
						break;
					}

                    if(types[j] != 0)
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
			item = new(std::nothrow) ItemBase(typeId, itemType);
			if (item == NULL) return NULL;
			item->SetBindStatus(bind);
			bool ret = TryAddItem(item, num);
			if (ret)
			{
				m_Items[ItemKey(typeId, bind)] = item;
			//	DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, num, m_Owner->getId(), bind ? 1 : 0);
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
			DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", item->Count(), typeId, bind ? 1 : 0, m_Owner->getId());
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

    //add log
    void  Package::AddItemCoursesLog(UInt32 typeId, UInt32 num, UInt8 fromWhere)
    {
         std::string tbn("item_courses");
         DBLOG().GetMultiDBName(tbn); 
         DBLOG().PushUpdateData("insert into `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", tbn.c_str(),cfg.serverLogId, m_Owner->getId(), typeId, num, fromWhere, TimeUtil::Now());
    }

    //add log
    void  Package::AddItemHistoriesLog(UInt32 itemId, UInt32 num)
    {
        std::string tbn("item_histories");
        DBLOG().GetMultiDBName(tbn);
        DBLOG().PushUpdateData("insert into %s (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)",tbn.c_str(), cfg.serverLogId, m_Owner->getId(), itemId, num, TimeUtil::Now());
        
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
				DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", item->Count(), typeId, bind, m_Owner->getId());
				SendItemData(item);
				if(notify)
					ItemNotify(item->GetItemType().getId(), num);
				if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
                {
                    AddItemCoursesLog(typeId, num, fromWhere);
                }
                if (fromWhere == FromNpcBuy)
                    udpLog(item->getClass(), typeId, num, GData::store.getPrice(typeId), "add");
				return item;
			}
			return NULL;
		}
		else
		{
			//Add New Item
			item = new(std::nothrow) ItemBase(typeId, itemType);
			if (item == NULL) return NULL;
			item->SetBindStatus(bind);
			bool ret = TryAddItem(item, num);
			if (ret)
			{
				m_Items[ItemKey(typeId, bind)] = item;
				DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, num, m_Owner->getId(), bind ? 1 : 0);
                //增加获取物品的荣誉
                GameAction()->doAttainment(m_Owner, Script::ON_ADD_ITEM, typeId);
				SendItemData(item);
				if(notify)
					ItemNotify(item->GetItemType().getId(), num);
				if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
                     AddItemCoursesLog(typeId, num, fromWhere);
                if (fromWhere == FromNpcBuy)
                    udpLog(item->getClass(), typeId, num, GData::store.getPrice(typeId), "add");
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
				DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", exist->Count(), typeId, bind, m_Owner->getId());
				SendItemData(exist);
				ItemNotify(item->GetItemType().getId(), count);
			}
            if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
            {
                AddItemCoursesLog(typeId, static_cast<UInt32>(count), fromWhere);
            }
			return item;
		}
		else
		{
			UInt32 newSize = m_Size + item->Size();
			if(newSize > m_Owner->getPacksize())
				return item;
			m_Size = newSize;
			m_Items[ItemKey(typeId, bind)] = item;
			DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, count, m_Owner->getId(), bind ? 1 : 0);
			SendItemData(item);
			ItemNotify(item->GetItemType().getId(), count);
            //获得物品
            GameAction()->doAttainment(m_Owner, Script::ON_ADD_ITEM, typeId);
            if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
            {
                 AddItemCoursesLog(typeId, static_cast<UInt32>(count), fromWhere);
            }
			return NULL;
		}
	}

	ItemBase* Package::AddItemFromDB(UInt32 id, UInt32 num, bool bind)
	{
		assert(!IsEquipId(id));
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
		if(itemType == NULL) return NULL;
		ItemBase * item = new(std::nothrow) ItemBase(id, itemType);
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
		if(itemType == NULL) return NULL;
		ItemBase * item = new(std::nothrow) ItemBase(id, itemType);
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
		if(itype == NULL) return NULL;
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

				UInt8 lv = itype->vLev;
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
					equip = new(std::nothrow) ItemWeapon(id, itype, edata);
					break;
				case Item_Armor1:
				case Item_Armor2:
				case Item_Armor3:
				case Item_Armor4:
				case Item_Armor5:
					equip = new ItemArmor(id, itype, edata);
					break;
                case Item_Trump:
                    {
                        UInt16 roll = uRand(1000);
                        edata.maxTRank = 1;
                        edata.trumpExp = 0;
                        for(int i = 0; i < 9; ++i)
                        {
                            UInt16 chance = GObjectManager::getTrumpMaxRankChance(i);
                            if(chance == 0)
                                break;
                            if(roll < chance)
                            {
                                edata.maxTRank = i + 1;
                                break;
                            }
                        }
                        equip = new ItemTrump(id, itype, edata);
                        if (equip && equip->getItemEquipData().enchant)
                            ((ItemTrump*)equip)->fixSkills();
                    }
                    break;
				default:
					equip = new ItemEquip(id, itype, edata);
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
				DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", id, m_Owner->getId(), bind ? 1 : 0);
				DB4().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `maxTRank`, `trumpExp`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`) VALUES(%u, %u, %u, %u, %u, %d, %u, %d, %u, %d)", id, typeId, edata.maxTRank, edata.trumpExp, edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3);
				SendSingleEquipData(equip);
				if(notify)
					ItemNotify(equip->GetItemType().getId());
				if(FromWhere != 0 && itype->quality >= 4)
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, id, FromWhere, TimeUtil::Now());
                
                OnAddEquipAndCheckAttainment(itype, FromWhere);
				return equip;
			}
		default:
			break;
		}
		return NULL;
	}

    ItemEquip* Package::AddUpgradeEquip(UInt32 typeId, bool notify, bool bind , ItemEquipData& ed, float * maxv)
    {
        const GData::ItemBaseType * itype = CheckBeforeEquipUpgrade(typeId);
        if(itype == NULL)
            return NULL;

        //ItemEquipData ed = oldEquip->getItemEquipData();
        ItemEquipData edata = ed;

        ItemEquip * equip = NULL;

        UInt8 lv =  itype->vLev ;
        UInt8 crr = itype->career;
        if(itype->quality > 2 )
        {
            UInt8 q = itype->quality - 3;
            UInt8 t[3] = {edata.extraAttr2.type1, edata.extraAttr2.type2, edata.extraAttr2.type3};
            Int16 v[3] = {  edata.extraAttr2.value1 ,  edata.extraAttr2.value2 ,  edata.extraAttr2.value3};
            for(int i =0 ; i < 3 ; i ++)
            {
                if(maxv[i] > 0.00001 && t[i] - 1 >= 0)
                {
                    float value = GObjectManager::getAttrMax(lv, t[i]-1, q, crr);
                    float tmp  = (value * static_cast<float>(v[i])) / maxv[i] + 0.9;
                    if(tmp > value * 100)
                       v[i] =  static_cast<Int16> (value * 100);
                    else
                       v[i] = static_cast<Int16> (tmp);
                }
            }
            edata.extraAttr2.type1 = t[0];
            edata.extraAttr2.value1 = v[0];
            edata.extraAttr2.type2 = t[1];
            edata.extraAttr2.value2 = v[1];
            edata.extraAttr2.type3 = t[2];
            edata.extraAttr2.value3 = v[2];
        }

        UInt32 id = IDGenerator::gItemOidGenerator.ID();
        switch(itype->subClass)
        {
            case Item_Weapon:
                equip = new(std::nothrow) ItemWeapon(id, itype, edata);
                break;
            case Item_Armor1:
            case Item_Armor2:
            case Item_Armor3:
            case Item_Armor4:
            case Item_Armor5:
                equip = new ItemArmor(id, itype, edata);
                break;
            default:
                equip = new ItemEquip(id, itype, edata);
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

        DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", id, m_Owner->getId(), bind ? 1 : 0);
        DB4().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `enchant`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`, `sockets`, `socket1`, `socket2`,`socket3`,`socket4`, `socket5`, `socket6`) VALUES(%u, %u, %u, %u, %d, %u, %d, %u, %d, %u, %u,%u,%u,%u,%u,%u)", id, typeId, edata.enchant,edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3, edata.sockets, edata.gems[0], edata.gems[1], edata.gems[2], edata.gems[3], edata.gems[4],edata.gems[5]);
        SendSingleEquipData(equip);
        if(notify)
            ItemNotify(equip->GetItemType().getId());
        if( itype->quality >= 4)
            DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, id, FromEquipUpgrade, TimeUtil::Now());
                                                                                        
        // OnAddEquipAndCheckAttainment(itype, FromWhere);
         return equip;
    }
    void  Package::OnAddEquipAndCheckAttainment(const GData::ItemBaseType * itype, UInt8 FromWhere)
    {
        if(FromWhere == FromDungeon || FromWhere  == FromNpc)
        {
                    //获取法宝成就
            if(itype->subClass == Item_Trump)
            {
                GameAction()->doAttainment(this->m_Owner, Script::ADD_TRUMP, itype->quality);
    //                if(itype->quality == 5)
               //获得累计法宝 
                GameAction()->doAttainment(this->m_Owner, Script::ADD_NTRUMP,1 );
                
                    
            }
            else
            {
                if(itype ->quality == 4 )
                {
                    GameAction()->doAttainment(this->m_Owner, Script::AddPEquip, 1 );
                }
                else if(itype ->quality == 5)
                {
                    GameAction()->doAttainment(this->m_Owner,  Script::AddYEquip, 1);
                }
            }
        }

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
		DB4().PushUpdateData("REPLACE INTO `item` VALUES(%u, %u, %"I64_FMT"u, %d)", equip->getId(), 1, m_Owner->getId(), equip->GetBindStatus() ? 1 : 0);
		SendSingleEquipData(equip);
		ItemNotify(equip->GetItemType().getId());
		if(FromWhere != 0 && equip->getQuality() >= 4)
			DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), equip->GetItemType().getId(), equip->getId(), FromWhere, TimeUtil::Now());

        OnAddEquipAndCheckAttainment(& (equip->GetItemType() ),  FromWhere);
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
		DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		SendDelEquipData(equip);
		return true;
	}

	bool Package::DelItem(UInt32 id, UInt16 num, bool bind, UInt8 toWhere)
	{
		if (num == 0 || IsEquipId(id))
			return false;
		ItemBase * item = FindItem(id, bind);
		if (item == NULL) return false;
		bool ret = TryDelItem(item, num);
		if (ret)
		{
			UInt16 cnt = item->Count();

            if((toWhere!= 0  && item->getQuality() >= 3) || (toWhere == ToGemMgerge && item->getQuality() >= 2))
            {
				std::string tbn("item_courses");
				DBLOG().GetMultiDBName(tbn); 
				DBLOG().PushUpdateData("insert into  `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str(), cfg.serverLogId, m_Owner->getId(), item->GetItemType().getId(), num, toWhere, TimeUtil::Now());
            }

			SendItemData(item);
			if (cnt == 0)
			{
				SAFE_DELETE(item);
				m_Items.erase(ItemKey(id, bind));
				DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", id, bind, m_Owner->getId());
			}
			else
				DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND`ownerId` = %"I64_FMT"u", cnt, id, bind, m_Owner->getId());
		}
		return ret;
	}

	bool Package::DelItem2(ItemBase* item, UInt16 num, UInt8 toWhere)
	{
		if (num == 0 || item == NULL || IsEquipId(item->getId()))
			return false;
		bool ret = TryDelItem(item, num);
		if (ret)
		{
			UInt16 cnt = item->Count();

            if((toWhere!= 0  && item->getQuality() >= 3) || (toWhere == ToGemMgerge && item->getQuality() >= 2))
            {
				std::string tbn("item_courses");
				DBLOG().GetMultiDBName(tbn); 
				DBLOG().PushUpdateData("insert into `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str() ,cfg.serverLogId, m_Owner->getId(), item->GetItemType().getId(), num, toWhere, TimeUtil::Now());
            }

			SendItemData(item);
			UInt32 id = item->getId();
			bool bind = item->GetBindStatus();
			if (cnt == 0)
			{
				SAFE_DELETE(item);
				m_Items.erase(ItemKey(id, bind));
				DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", id, bind, m_Owner->getId());
			}
			else
				DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND`ownerId` = %"I64_FMT"u", cnt, id, bind, m_Owner->getId());
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
		DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", id);
		DB4().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", id);
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
		DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		DB4().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", equip->getId());
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
		DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		SendDelEquipData(equip);
		return true;
	}

    /**
     *  @return  0表示有问题
     */
    UInt8 Package::GetPart( ItemEquip* eq)
    {
        if(!eq)
            return 0;
        UInt8 c  = eq->getClass();
        
        switch (c)
        {
            case Item_Weapon:
                return  0x21;
                
            case Item_Armor1:
                return  0x22;
               
            case Item_Armor2:
                return  0x23;
                break;
            case Item_Armor3:
                return  0x24;
                break;
            case Item_Armor4:
                return  0x25;
                
            case Item_Armor5:
                return  0x26;
               
            case Item_Amulet:
                return  0x27;
                
            case  Item_Ring:
                return 0x28;
        }
        return 0;
    }
    /*bool Package::EquipTo( ItemEquip * eq, Fighter * fgt,bool noCheckFull)
    {
        if(!eq || !fgt)
            return false;
        UInt8 c  = eq->getClass();
        UInt8 part = GetPart( eq);
        if(part == 0 )
            return false;

        ItemEquip* newItem = NULL;
        return EquipTo(eq->getId(), fgt, part, newItem, noCheckFull);

    }*/
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
            if (item->GetCareer() && fgt->getClass() != item->GetCareer())
            {
                m_Owner->sendMsgCode(0, 1056);
                return false;
            }
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
                if (fgt->canSetTrump(part-0x50, item->getId()))
                    old = fgt->setTrump(static_cast<GObject::ItemTrump*>(item), part-0x50);
                else
                    return false;
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
                old = fgt->setTrump(static_cast<GObject::ItemTrump*>(NULL), part-0x50);
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
		if (DelItem2(item, num, ToSellNpc))
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
                    m_Owner->sendMsgCode(0, 1800);
                else
                    m_Owner->sendMsgCode(0, 1802);
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
                    m_Owner->sendMsgCode(0, 1800);
                else
                    m_Owner->sendMsgCode(0, 1802);
                return ret;
            }
        }
        else if (GetItemSubClass(id) == Item_Normal)
        {
            ItemBase* item = GetItem(id, bind > 0);
            if (item && (item->getClass() == Item_Normal29 ||
                         item->getClass() == Item_Normal28))
            {
                ret = TrumpMerge(id, bind > 0);
                if (ret)
                    m_Owner->sendMsgCode(0, 1800);
                else
                    m_Owner->sendMsgCode(0, 1802);
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
				else if (UInt16 n = GameAction()->RunItemNormalUse(m_Owner, id, param, num, bind > 0))
				{
                    UInt8 rn = n<num?n:num;
                    udpLog(item->getClass(), id, rn, GData::store.getPrice(id), "sub");
					DelItem2(item, rn);
					//DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), id, rn, TimeUtil::Now());
					AddItemHistoriesLog(id, rn);
                    ret = true;
				}				
			}
			else
			{
				if (GetItemAnyNum(id) < num)
					ret = false;
				else if (UInt16 n = GameAction()->RunItemNormalUse(m_Owner, id, param, num, false))
				{
                    UInt8 rn = n<num?n:num;
                    ItemBase * item = FindItem(id, true);
                    if (!item)
                        item = FindItem(id, false);
                    udpLog(item->getClass(), id, rn, GData::store.getPrice(id), "sub");
					DelItemAny(id, rn);
					//DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), id, rn, TimeUtil::Now());
                    AddItemHistoriesLog(id, rn);
                    ret = true;
				}
			}
		}

		Stream st(REP::PACK_USE);
		st << id << static_cast<UInt8>(1) << static_cast<UInt8>(ret ? 1 : 0) << Stream::eos;
		m_Owner->send(st);

		return ret;
	}
	void Package::DelItemSendMsg(UInt32 itemid, Player *player)
	{
		SYSMSG_SENDV(104, player, itemid);
		SYSMSG_SENDV(1004, player, itemid);		
	}

    void Package::udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op)
    {
        if (!op || !price)
            return;
        char _price[32] = {0};
        char _type[32] = {0};
        char _id[32] = {0};
        snprintf(_price, 32, "%u", price);
        snprintf(_type, 32, "%u", type);
        snprintf(_id, 32, "%u", id);
        m_Owner->udpLog(op, _type, _id, _price, "", "", "props", num);
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

		Stream st(REP::PACK_USE);
		st << id << token << static_cast<UInt8>(ret ? 1 : 0) << Stream::eos;
		m_Owner->send(st);

		return ret;
	}

    bool Package::FCMerge(UInt32 id, bool bind)
    {
        /*
        static struct {
            UInt32 sid; // start id
            UInt32 eid; // end id
            const char* nums; // numbuers of every id
            UInt32 tid; // target id
        } config[] = {
            {1014,  1019,   "1,1,1,1,1,1",          1013},
            {1022,  1030,   "1,1,1,1,1,1,1,1,1",    1021},
            {1033,  1039,   "1,1,1,1,1,1,1",        1032},
            {1042,  1051,   "1,1,1,1,1,1,1,1,1,1",  1041},
            {1054,  1058,   "1,1,1,1,1",            1053},
            {1061,  1064,   "1,1,1,1",              1060},
            {1067,  1073,   "1,1,1,1,1,1,1",        1066},
            {1076,  1083,   "1,1,1,1,1,1,1,1",      1075},
            {1086,  1095,   "1,1,1,1,1,1,1,1,1,1",  1085},
            {1098,  1106,   "1,1,1,1,1,1,1,1,1",    1097},
            {1109,  1115,   "1,1,1,1,1,1,1",        1108},
            {1118,  1124,   "1,1,1,1,1,1,1",        1117},
            {1304,  1312,   "1,1,1,1,1,1,1,1,1",    1208},
            {1313,  1321,   "1,1,1,1,1,1,1,1,1",    1228},
            {1322,  1330,   "1,1,1,1,1,1,1,1,1",    1206},
            {1331,  1339,   "1,1,1,1,1,1,1,1,1",    1302},
            {1340,  1348,   "1,1,1,1,1,1,1,1,1",    1299},
            {1349,  1357,   "1,1,1,1,1,1,1,1,1",    1303},
            {1358,  1366,   "1,1,1,1,1,1,1,1,1",    1248},
            {1367,  1375,   "1,1,1,1,1,1,1,1,1",    1298},
            {82,    85,     "1,1,1,1 ",             1510},
            {86,    89,     "1,1,1,1 ",             1512},
            {200,   202,    "1,1,1",                1613},
            {203,   205,    "1,1,1",                1617},
            {206,   208,    "1,1,1",                1618},
            {1413,  1415,   "1,1,1",                1252},
            {209,   216,    "1,1,1,1,1,1,1,1",      1621},
            {217,   224,    "1,1,1,1,1,1,1,1",      1620},
            {225,   225,    "10",                   1628},    
            {90,    90,     "10",                   1524},
            {226,   226,    "10",                   1635},
            {227,   227,    "10",                   1619},
            {1416,  1420,   "1,1,1,1,1",            1232},
            {1421,  1426,   "1,1,1,1,1,1",          1239},
            {70,    71,     "1,1",                  1751},
            {0, 0, NULL, 0},
        };*/

        bool b = false; //绑定
        if (IsFull())
        {
            m_Owner->sendMsgCode(0, 1011);
            return false;
        }

        std::vector<stMergeStf> stfs = GObjectManager::getMergeStfs(id);

        if(stfs.size()  == 0 )
            return false;

        for(UInt32 i = 0 ; i < stfs[0].m_stfs.size() ; i ++)
        {
            UInt32 id = stfs[0].m_stfs[i].id;
            UInt32 num = stfs[0].m_stfs[i].num;

            if( GetItemAnyNum(id) < num)
                return false;
        }

         
        for(UInt32 i = 0 ; i < stfs[0].m_stfs.size() ; i ++)
        {
            UInt32 id = stfs[0].m_stfs[i].id;
            UInt32 num = stfs[0].m_stfs[i].num;
            
            DelItemAny(id, num, &bind);
            if( bind )
                b = true;
        }
        Add( stfs[0].m_to, 1, b, false, FromFCMerge);
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
		Stream st(REP::PACK_INFO);
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

        if(equip->getClass() == Item_Trump)
        {
            st << ied.maxTRank << ied.trumpExp;
        }
	}

	void Package::AppendItemData( Stream& st, ItemBase * item )
	{
		st << item->getId() << static_cast<UInt8>(item->GetBindStatus() ? 1 : 0) << item->Count();
	}

	void Package::SendSingleEquipData(ItemEquip * equip)
	{
		Stream st(REP::PACK_INFO);
		st << static_cast<UInt16>(1);
		AppendEquipData(st, equip);
		st << Stream::eos;
		m_Owner->send(st);
	}

	void Package::SendDelEquipData(ItemEquip * equip)
	{
		Stream st(REP::PACK_INFO);
		st << static_cast<UInt16>(1) << equip->getId() << static_cast<UInt8>(equip->GetBindStatus() ? 1 : 0) << static_cast<UInt16>(0) << Stream::eos;
		m_Owner->send(st);
	}

	void Package::SendItemData(ItemBase * item)
	{
		Stream st(REP::PACK_INFO);
		st << static_cast<UInt16>(1);
		AppendItemData(st, item);
		st << Stream::eos;
		m_Owner->send(st);
	}

	UInt8 Package::Enchant( UInt16 fighterId, UInt32 itemId, UInt8 type, UInt16 count, UInt8 level, UInt16& success, UInt16& failed/*, bool protect*/ )
	{
		if (type > 1) return 2;
		Fighter * fgt = NULL;
		UInt8 pos = 0;
        UInt32 failThisTime = 0; 
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL || equip->getClass() == Item_Ring || equip->getClass() == Item_Amulet)
			return 2;

        const GData::ItemBaseType& itemType =  equip-> GetItemType();
        if(itemType.getId() == 1525 || itemType.getId() == 1526)
            return 2;

        UInt32 item_enchant_l = ITEM_ENCHANT_L1;
        UInt8 quality = 0;
        UInt8 maxEnchantLevel = ENCHANT_LEVEL_MAX;
        if(equip->getClass() == Item_Weapon)
        {
            quality = 1;
        }
        else if(equip->getClass() == Item_Trump)
        {
            maxEnchantLevel = TRUMP_ENCHANT_LEVEL_MAX;
            item_enchant_l = TRUMP_ENCHANT_L1;
            quality = equip->getQuality();
        }

		ItemEquipData& ied = equip->getItemEquipData();
        if(ied.enchant >= maxEnchantLevel || level > maxEnchantLevel)
            return 2;

        if(level != 0 && ied.enchant >= level)
            return 2;

        // count不为0则type必须为1，表示自动强化时必须消耗精金
        if(count !=0 && type != 1)
            return 2;

		if(GetItemAnyNum(item_enchant_l + type) < (count > 0 ? count : 1))
            return 2;

        HoneyFall* hf = m_Owner->getHoneyFall();

        HoneyFallType hft;
        if(equip->getClass() == Item_Trump)
            hft = e_HFT_Trump_Enchant;
        else
            hft = e_HFT_Equip_Enchant;

        UInt32 enchant = 0;
        if(type == 0)
        {
            enchant = GObjectManager::getEnchantChance(type, quality, ied.enchant);
            if(enchant == 0)
                return 2;
        }
        else
        {
            enchant = hf->getChanceFromHft(quality, ied.enchant, hft);
        }

		// UInt32 viplvl = this->m_Owner->getVipLevel();
		// const UInt8 enchant_max[] = {7, 7, 7, 7, 8, 10, 10, 10, 10, 11, 12};
        // if(ied.enchant >= enchant_max[viplvl])
        //     return 2;
        //GObjectManager::getEnchantCost();  // enchant_cost[ied.enchant];
		UInt32 amount = GData::moneyNeed[GData::ENCHANT].tael;
		if(m_Owner->getTael() < (amount * (count > 0 ? count : 1)))
		{
			m_Owner->sendMsgCode(0, 1100);
			return 2;
		}
		bool isBound = equip->GetBindStatus();
		// static UInt32 enchant_chance[] = {100, 90, 80, 60, 50, 40, 20, 10, 5, 2, 2, 2};
        bool flag_suc = false;
        bool updateHft = false;
        UInt32 enc_times = 1;
	    UInt8 oldEnchant = ied.enchant;
        UInt8 oldHfValue = hf->getHftValue(hft);
        if(0 == count)
        {
            if(uRand(100000) < enchant)
            {
                if(type != 0 && ied.enchant > 3)
                {
                    updateHft = true;
                    hf->setHftValue(hft, 0);
                }

                flag_suc = true;
                ++ ied.enchant;
                if (equip->getClass() == Item_Trump && ied.enchant == 1)
                {
                    ((ItemTrump*)equip)->fixSkills();
                    if (fgt)
                    {
                        GData::AttrExtra* attr = const_cast<GData::AttrExtra*>(equip->getAttrExtra());
                        if (attr)
                            fgt->addSkillsFromCT(attr->skills, true);
                    }
                }
            }
            else if(type != 0 && ied.enchant > 3)
            {
                updateHft = true;
                hf->incHftValue(hft);
            }
        }
        else if( 0 != count )
        {
            UInt32 i = 0;
            enc_times = 0;
            for(; i < count; ++i)
            {
                if(uRand(100000) < enchant)
                {
                    ++success;
                    flag_suc = true;
                    ++ ied.enchant;

                    if(ied.enchant > 3)
                    {
                        updateHft = true;
                        hf->setHftValue(hft, 0);
                    }
                    if (equip->getClass() == Item_Trump && ied.enchant == 1)
                    {
                        ((ItemTrump*)equip)->fixSkills();
                    }

                    ++enc_times;
                    if(ied.enchant >= level)
                        break;
                }
                else
                {
                    if(ied.enchant > 3)
                    {
                        updateHft = true;
                        hf->incHftValue(hft);
                    }

                    ++failed;
                    ++failThisTime;
                    ++enc_times;
                }

                enchant = hf->getChanceFromHft(quality, ied.enchant, hft);
            }
        }

        if(!DelItemAny(item_enchant_l + type, enc_times, &isBound))
        {
            success = 0;
            failed = 0;
            ied.enchant = oldEnchant;
            hf->setHftValue(hft, oldHfValue);
            return 2;
        }

        if(updateHft)
            hf->updateHftValueToDB(hft);

        GameAction()->doAty(this->m_Owner,  AtyEnchant, 0, 0);
		AddItemHistoriesLog(item_enchant_l + type, enc_times);
        //DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), item_enchant_l + type, enc_times, TimeUtil::Now());
        ConsumeInfo ci(EnchantEquipment,0,0);
        m_Owner->useTael(amount * enc_times, &ci);

		if(flag_suc/*enchant_chance[ied.enchant]*/)
		{
            if(failThisTime)
                OnFailEnchAttainment(failThisTime);
			DB4().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", ied.enchant, equip->getId());
			if(ied.enchant >= 5)
				DBLOG().PushUpdateData("insert into enchant_histories (server_id, player_id, equip_id, template_id, enchant_level, enchant_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), equip->getId(), equip->GetItemType().getId(), ied.enchant, TimeUtil::Now());
			if(!equip->GetBindStatus() && isBound)
			{
				equip->DoEquipBind();
			}

            if(equip->getClass() == Item_Trump)
            {
                GData::AttrExtra* attr = const_cast<GData::AttrExtra*>(equip->getAttrExtra());
                if(ied.enchant != 1)
                {
                    ((ItemTrump*)equip)->enchant(ied.enchant, attr);
                    if (fgt && attr)
                        fgt->addSkillsFromCT(attr->skills, true);
                }
                //法宝强化
                GameAction()->doAttainment(this->m_Owner, 10204, ied.enchant);

                if(ied.enchant == 9)
                {
                    GameAction()->doAttainment(this->m_Owner, 10207,itemType.quality);
                }
            }
            else
            {
                //装备强化
                 GameAction()->doAttainment(this->m_Owner, 10164, ied.enchant);

                 if(fgt)
                     fgt->CheckEquipEnchantAttainment(ied.enchant);
            }

			if(fgt != NULL)
			{
				fgt->setDirty();

                if(equip->getClass() == Item_Trump)
                    fgt->sendModification(0x50 + pos, equip, false);
                else
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
            case 9:
			case 10:
			case 11:
			case 12:
                {
                    if (World::_halloween && ied.enchant == 8 && equip->GetItemType().subClass != Item_Trump)
                    {
                        if (!m_Owner->enchanted8(equip->getId()))
                            m_Owner->sendEnchanted8Box();
                    }
                    SYSMSG_BROADCASTV(275, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId(), ied.enchant);
                }
				break;
			}
			return 0;
		}

        //if fail
        if(0 == failThisTime)
            failThisTime  =1;
    
        OnFailEnchAttainment(failThisTime);
		if(type == 0 && ied.enchant >= 4)
		{
			ied.enchant --;
			DB4().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", ied.enchant, equip->getId());
			DBLOG().PushUpdateData("insert into enchant_histories (server_id, player_id, equip_id, template_id, enchant_level, enchant_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), equip->getId(), equip->GetItemType().getId(), ied.enchant, TimeUtil::Now());

            if(equip->getClass() == Item_Trump)
            {
                GData::AttrExtra* attr = const_cast<GData::AttrExtra*>(equip->getAttrExtra());
                ((ItemTrump*)equip)->enchant(ied.enchant, attr);
                if (fgt && attr)
                    fgt->addSkillsFromCT(attr->skills, true);
            }

			if(fgt != NULL)
			{
				fgt->setDirty();
                if(equip->getClass() == Item_Trump)
                    fgt->sendModification(0x50 + pos, equip, false);
                else
                    fgt->sendModification(0x21 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
		}

		return 1;
	}

    void  Package::OnFailEnchAttainment( UInt32 failThisTime)
    {
    
        UInt32  ft = m_Owner->GetVar(GObject::VAR_FAIL_ENCH);
        ft += failThisTime;

        if(ft >= 999)
        {
            GameAction()->doAttainment(this->m_Owner, Script::FAIL_ENCH, ft);
        }
        else
        {
            m_Owner->SetVar( GObject::VAR_FAIL_ENCH ,ft);
        }
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
            AddItemHistoriesLog(ITEM_SOCKET_L3, 1);
			//DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_SOCKET_L3, 1, TimeUtil::Now());
		}
		else if(ied.sockets >= 2)
		{
			if(!DelItemAny(ITEM_SOCKET_L2, 1, &isBound))
				return 2;
             AddItemHistoriesLog(ITEM_SOCKET_L2, 1);
			//DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_SOCKET_L2, 1, TimeUtil::Now());
		}
		else
		{
			if(!DelItemAny(ITEM_SOCKET_L1, 1, &isBound))
				return 2;
             AddItemHistoriesLog(ITEM_SOCKET_L1, 1);
			//DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_SOCKET_L1, 1, TimeUtil::Now());
		}
		//if(World::_wday == 6)
		//{
		//	if(chance[ied.sockets] > 25)
		//		chance[ied.sockets] -= 25;
		//}
		if(uRand(100) < GObjectManager::getSocketChance(ied.sockets) /*chance[ied.sockets]*/)
			return 1;
		++ied.sockets;
		DB4().PushUpdateData("UPDATE `equipment` SET `sockets` = %u WHERE `id` = %u", ied.sockets, equip->getId());
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
		UInt32 amount = GData::moneyNeed[GData::GEMMERGE].tael;//GObjectManager::getMergeCost(); // merge_cost[lvl];
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 1100);
			return 2;
		}

		if(GetRestPackageSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1011);
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
		if(uRand(1000) < GObjectManager::getMergeChance(lvl) /*merge_chance[lvl]*/)
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
#if 0
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
#endif

        /***力量宝石, 敏捷宝石,  智力宝石,  耐力宝石， 意志宝石, 生命宝石， 攻击宝石， 防御宝石， 命中宝石
         * 反击宝石， 闪避宝石， 暴击宝石， 破击宝石,身法宝石， 坚韧宝石， 法抗宝石
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Weapon
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Armor1 头盔
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Armor2 胸甲 
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Armor3 肩甲 
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Armor4 腰带 
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Armor5 腿甲 
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Amulet 项链
         * {Item_Gem, Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4, ........., Item_Gem19}   ---- Item_Ring
         */
        static UInt8 equip_gem[8][19] = {
            {1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0}
        };

        GData::ItemGemType * igt = GData::gemTypes[gemId - LGEM_ID];
        if(!igt)
            return 1;

        if(!equip_gem[equip->getClass() - Item_Weapon][igt->subClass - Item_Gem])
            return 1;

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
		if(!DelItem(gemId, 1, bind, ToGemAttach))
			return 1;
		ied.gems[fempty] = gemId;
		DB4().PushUpdateData("UPDATE `equipment` SET `socket%u` = %u WHERE `id` = %u", fempty + 1, ied.gems[fempty], equip->getId());
        //镶嵌成就
        UInt8 gemLev = getGemLev(gemId);
        GameAction()->doAttainment(m_Owner, 10170, gemLev);

        if((fempty  + 1)== SOCKETS_MAX)
        {
            //镶嵌满脸
            GameAction()->doAttainment(m_Owner, 10173, 0);
            for(UInt8 i = 0 ; i< SOCKETS_MAX ; i ++ )
            {
                if(getGemLev(ied.gems[i]) <10)
                    break;
                if( i == (SOCKETS_MAX - 1))
                     GameAction()->doAttainment(m_Owner, 10174, 0);//镶嵌满等级的宝石
            }
        }
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
		UInt32 amount = GData::moneyNeed[GData::GEMDETACH].tael;//GObjectManager::getDetachCost();
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 1100);
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

        if(protect == 0 && getGemLev(ied.gems[pos]) > 3)
        {
            return 2;
        }

		if(GetRestPackageSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1011);
			return 2;
		}
		bool bind = false;
        if(!protect)
        {
            if(DelItem(ITEM_DETACH_RUNE, 1, true))
                bind = true;
            else if(!DelItem(ITEM_DETACH_RUNE, 1, false))
                return 2;
             AddItemHistoriesLog(ITEM_DETACH_RUNE, 1);
            //DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_DETACH_RUNE, 1, TimeUtil::Now());
        }
        else
        {
            if(DelItem(ITEM_DETACH_PROTECT, 1, true))
                bind = true;
            else if(!DelItem(ITEM_DETACH_PROTECT, 1, false))
                return 2;
             AddItemHistoriesLog(ITEM_DETACH_RUNE, 1);
            //DBLOG().PushUpdateData("insert into item_histories (server_id,player_id,item_id,item_num,use_time) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_DETACH_PROTECT, 1, TimeUtil::Now());
        }
		if(!AddItem(ied.gems[pos], 1, bind | equip->GetBindStatus(), false, FromDetachGem))
			return 2;
        ConsumeInfo ci(DetachGems,0,0);
		m_Owner->useTael(amount,&ci);
#if 0
		if(protect == 0 && uRand(100) < 75)
		{
			if(fgt != NULL)
				fgt->removeEquip(pos2, equip, ToDetachGemDesdroy);
			else
				DelEquip2(equip, ToDetachGemDesdroy);
			return 1;
		}
#endif
		ied.gems[pos] = 0;
		DB4().PushUpdateData("UPDATE `equipment` SET `socket%u` = 0 WHERE `id` = %u", pos + 1, equip->getId());
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

    static void pushBackSplitItem( std::vector<SplitItemOut>& splitOut, UInt16 itemId, UInt16 count )
    {
        UInt32 cnt = splitOut.size();
        bool find = false;
        for( UInt32 i = 0; i < cnt; ++i )
        {
            if(splitOut[i].itemId == itemId)
            {
                find = true;
                splitOut[i].count += count;
            }
        }

        if(!find)
        {
            SplitItemOut sio = {itemId, count};
            splitOut.push_back(sio);
        }
    }

	UInt8 Package::Split( UInt32 itemId, std::vector<SplitItemOut>& splitOut, /*bool protect,*/ bool silence )
	{
        UInt8 res = 1;
		if (!IsEquipId(itemId)) return 2;
		ItemBase * item = FindItem(itemId);
		if(item == NULL || item->getQuality() < 2 || item->getReqLev() < 1)
			return 2;

        if(GetRestPackageSize() < 2)
        {
            m_Owner->sendMsgCode(0, 1011);
            return 2;
        }

		UInt8 q = item->getQuality() - 2;
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
        const GData::ItemBaseType& t = item->GetItemType();
        UInt32 itemOutId = 0;
        UInt32 count = 0;
        GameAction()->doAty(this->m_Owner, AtySplit, 0,0);    
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

			r = uRand(100);
            if(q < 3)
            {
                UInt32 chance_team_matieral = GObjectManager::getTeamMatieralChance(q);
                if(r < chance_team_matieral)
                {
                    itemOutId = GObjectManager::getTeamMatieralItemFromSplit(q);
                    count = 1;
                }
            }
            else
            {
                UInt32 typeId = t.getId();
                if(r < GObjectManager::getOrangeTeamMatieralChance(typeId, 0))
                    count = 1;
                else if(r < GObjectManager::getOrangeTeamMatieralChance(typeId, 1))
                    count = 2;
                else if(r < GObjectManager::getOrangeTeamMatieralChance(typeId, 2))
                    count = 3;

                if(count)
                    itemOutId = GObjectManager::getOrangeTeamMatieralItemFromSplit(typeId);
            }
		}

        if(IsEquip(t.subClass))
        {
            if(t.subClass == Item_Trump)
            {
                UInt32 n = m_Owner->GetVar(VAR_SPLIT_THRUMP);
                n ++ ;
                m_Owner->SetVar(VAR_SPLIT_THRUMP, n);//分解法宝的次数

                GameAction()->doAttainment(this->m_Owner, Script:: SPLIT_THRUMP , n);

                GameAction()->doAttainment(this->m_Owner, Script::SPLIT_THRUMP_COLOR, t.quality );
            }
            else
            {
                UInt32 n = m_Owner->GetVar(VAR_SPLIT_EQUIP);
                n ++ ;
                m_Owner->SetVar(VAR_SPLIT_EQUIP, n);

                GameAction()->doAttainment(this->m_Owner, Script:: SPLIT_EQUIP , n);
                GameAction()->doAttainment(this->m_Owner, Script:: SPLIT_EQUIP_COLOR, t.quality );
            }
        }

        if(itemOutId != 0 && count != 0)
        {
            if(item != NULL)
                DelEquip2(static_cast<ItemEquip *>(item), ToSplit);
            AddItem(itemOutId, count, isBound, silence, FromSplit);

            item = NULL;
            pushBackSplitItem( splitOut, itemOutId, count );
            res = 0;
        }

		if(got)
		{
			UInt32 itemOutId = ITEM_ENCHANT_L1 + got - 1;
            UInt32 count = 1;
            if(item != NULL)
                DelEquip2(static_cast<ItemEquip *>(item), ToSplit);

            item = NULL;
			AddItem(itemOutId, count, isBound, silence, FromSplit);

            pushBackSplitItem( splitOut, itemOutId, count );
            res = 0;
		}

        if(item != NULL)
            DelEquip2(static_cast<ItemEquip *>(item), ToSplit);
        
		return res;
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
			m_Owner->sendMsgCode(0, 1100);
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
			m_Owner->sendMsgCode(0, 1100);
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

    UInt8 Package::BatchMergeGem(UInt16 gemId, UInt16 unbindCount, UInt16 bindCount, UInt8 protect, UInt16& gemIdOut, UInt16& unbindGemsOut, UInt16& bindGemsOut, UInt16& succTimes, UInt16& failedTimes)
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
        if(lvl == 9)
            return 3;

        if(bindCount > 0 && GetItemNum(gemId, true) < bindCount)
            return 3;
        if(unbindCount > 0 && GetItemNum(gemId, false) < unbindCount)
            return 3;

        UInt32 bindUsed = 0, unbindUsed = 0;
        unbindGemsOut = 0;
        bindGemsOut = 0;
        gemIdOut = gemId + 1;
        succTimes = 0;
        failedTimes = 0;

        while(result == 0 && bindCount >= 3)
        {
            UInt32 amount = GData::moneyNeed[GData::GEMMERGE].tael;//GObjectManager::getMergeCost();    // merge_cost[lvl];
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

            if(rnd(1000) < GObjectManager::getMergeChance(lvl))
            {
                bindUsed += 3;
                bindCount -= 3;
                ++ bindGemsOut;
                ++ succTimes;
            }
            else
            {
                ++ failedTimes;
                if(!protect)
                {
                    bindUsed += 3;
                    bindCount -= 3;
                }
            }
        }

        while(result == 0 && unbindCount >= 3)
        {
            UInt32 amount = GData::moneyNeed[GData::GEMMERGE].tael;//GObjectManager::getMergeCost();        // merge_cost[lvl];
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
            if(rnd(1000) < GObjectManager::getMergeChance(lvl))
            {
                unbindUsed += 3;
                unbindCount -= 3;
                ++ unbindGemsOut;
                ++ succTimes;
            }
            else
            {
                ++ failedTimes;
                if(!protect)
                {
                    unbindUsed += 3;
                    unbindCount -= 3;
                }
            }
        }

        while(bindCount + unbindCount >= 3 && result == 0)
        {
            UInt32 amount = GData::moneyNeed[GData::GEMMERGE].tael;//GObjectManager::getMergeCost();      // merge_cost[lvl];
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
            if(rnd(1000) < GObjectManager::getMergeChance(lvl))
            {
                bindUsed += bindCount;
                unbindUsed += 3 - bindCount;

                unbindCount -= 3 - bindCount;
                bindCount = 0;

                ++ bindGemsOut;
                ++ succTimes;
                break;
            }
            else
            {
                ++ failedTimes;
                if(!protect)
                {
                    bindUsed += bindCount;
                    unbindUsed += 3 - bindCount;

                    unbindCount -= 3 - bindCount;
                    bindCount = 0;
                }
            }
        }

        if(bindUsed > 0)
            DelItem(gemId, bindUsed, true, ToGemMgerge);
        if(unbindUsed > 0)
            DelItem(gemId, unbindUsed, false, ToGemMgerge);

        if(bindGemsOut > 0)
        {
            AddItem(gemIdOut, bindGemsOut, true, false, FromMerge);
            if(World::_activityStage > 0)
                GameAction()->onMergeGem(m_Owner, lvl + 2, bindGemsOut);
        }
        if(unbindGemsOut > 0)
        {
            AddItem(gemIdOut, unbindGemsOut, false, false, FromMerge);
            if(World::_activityStage > 0)
                GameAction()->onMergeGem(m_Owner, lvl + 2, unbindGemsOut);
        }

		ConsumeInfo ci(MergeGems,0,0);
		m_Owner->useTael(coinAmount, &ci);
		if(protectBindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectBindUsed, true, ToGemMgerge);
		if(protectUnbindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectUnbindUsed, false, ToGemMgerge);

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
		if(ied.extraAttr2.type3 > 0 || (equip->GetItemType().subClass == Item_Trump && ied.tRank < 1)) return 1;
		ItemEquip * equip2 = GetEquip(itemId2);
		if(equip2 == NULL) return 1;

		UInt8 lv = (equip->getReqLev() + 5) / 10 - 1;
		if(equip->getQuality() != equip2->getQuality() || lv != (equip2->getReqLev() + 5) / 10) return 1;

		q -= 3;
		if(m_Owner->getTael() < activate_cost[lv][q])
		{
			m_Owner->sendMsgCode(0, 1100);
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
            UInt8 crr = equip->GetCareer();

            UInt8 equip_t = EQUIPTYPE_EQUIP;
            lv = equip->getValueLev();
            if(equip->GetItemType().subClass == Item_Trump)
            {
                equip_t = EQUIPTYPE_TRUMP;
                lv = ied.tRank;
            }

			getRandomAttr2(lv, crr, q, c, protect, types, values, equip_t);
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

    const GData::ItemBaseType*  Package::CheckBeforeEquipUpgrade(UInt32 typeId)
    {
        const GData::ItemBaseType * itype = GData::itemBaseTypeManager[typeId];
        if(itype == NULL)
            return NULL;
        if(itype->subClass < Item_Weapon || itype->subClass > Item_Ring )
            return NULL;
        if(m_Size >= m_Owner->getPacksize())
            return NULL;
        return itype;

    }

    /***
     *  @param  pNewId 新生成的物品ID
     */
    UInt8 Package::EquipUpgrade( UInt16 fighterId, UInt32 itemId, UInt32 * pNewId , UInt16* pFgtId)
    {
        Fighter * fgt = NULL;
        UInt8 pos = 0;
        
        ItemEquip * oldEquip = FindEquip(fgt, pos, fighterId, itemId);
        if (oldEquip == NULL) return 2;
        
        if(this->IsFull()){
             m_Owner->sendMsgCode(0, 1011);
             return 2;
        }

        if(this->m_Owner->getTael() < 10)
        {
            this->m_Owner->sendMsgCode(0, 1100);
            return 2;
        }

        UInt8 part = GetPart(oldEquip);
        if( part == 0 )
            return  2;
        const GData::ItemBaseType& itype =  oldEquip-> GetItemType();
        //UInt32 newId  = GObjectManager::getEUpgradeId(itype.getId());

        //if(newId == 0 )
            //return 2;

        GData::EUpgradeTable::iterator it =  GData::eUpgradeTable.find(itype.getId());
        if(it == GData::eUpgradeTable.end())
            return 2;

        UInt32 newId = it->second.toId;

        const GData::ItemBaseType* newType =  CheckBeforeEquipUpgrade(newId);
        if(newType == NULL)
            return 2;

        for(UInt32 i = 0 ;  i < it->second.stfs.size(); i++)
        {
            GData::stUseItem&  itm = it->second.stfs[i]; 
            if(  GetItemAnyNum(itm.id) < itm.num)
                return 2;
        }
        bool bind = oldEquip->GetBindStatus();

        for(UInt32 i = 0 ;  i < it->second.stfs.size(); i++)
        {
            bool b = false;
            GData::stUseItem&  itm = it->second.stfs[i];
            DelItemAny(itm.id, itm.num, &b);
            if(b)
                bind = true;
        }

        ItemEquipData ed = oldEquip->getItemEquipData();

        UInt8 lv = itype.vLev ;
        UInt8 crr = itype.career;
        float maxv[3] = {0};
        if(itype.quality > 2 )
        {
             UInt8 q = itype.quality - 3;
             UInt8 t[3] = {ed.extraAttr2.type1, ed.extraAttr2.type2, ed.extraAttr2.type3};

             for(UInt8 i = 0 ; i < 3; i ++ )
             {
                if(t[i] - 1 >= 0)
                  maxv[i] = GObjectManager::getAttrMax(lv, t[i]-1, q, crr);
             }             
        }

        //删除原有物品
        if(fgt)
        {
            ItemEquip* ptmp = NULL;
            EquipTo(0, fgt, part, ptmp, true);
        }

        if( false == DelEquip2(oldEquip, ToEquipUpgrade))
            return 2;
        

        ItemEquip*  pEquip = AddUpgradeEquip(newId, true, bind, ed , maxv);
        if( pEquip == NULL)
            return 2;


        if(pNewId)
            *pNewId =  pEquip->getId();
        ConsumeInfo ci(ForEquipUpgrade,0,0);
        m_Owner->useTael(10,&ci);

        if(fgt)
        {
            UInt8 newPart = GetPart(pEquip);
            ItemEquip* ptmp = NULL;
            if(EquipTo(pEquip->getId(), fgt, newPart,  ptmp, true) == false)
            {
                if(pFgtId)
                    *pFgtId = 0;
            }
        }
        return 0;
    }

	UInt8 Package::Forge( UInt16 fighterId, UInt32 itemId, /*UInt8 t,*/ UInt8 * types, Int16 * values, UInt8 protect )
	{
		// if (t > 2) return 2;
		UInt32 amount = GObjectManager::getForgeCost();  // forge_cost;
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 1100);
			return 1;
		}

		Fighter * fgt = NULL;
		UInt8 pos = 0;

		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if (equip == NULL) return 2;
		ItemEquipData& ied = equip->getItemEquipData();
        if(equip->GetItemType().subClass == Item_Trump && ied.tRank < 1)
            return 2;
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
        AddItemHistoriesLog(ITEM_FORGE_L1,  1);
        //DBLOG().PushUpdateData("insert into `item_histories` (`server_id`, `player_id`, `item_id`, `item_num`, `use_time`) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_FORGE_L1, 1, TimeUtil::Now());
        //
        //装备洗练成就
        GameAction()->doAttainment(this->m_Owner, 10175, 0);
        GameAction()->doAty(this->m_Owner, AtyForge, 0, 0);
		UInt8 lv = equip->getValueLev();
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
            {
				protect = 0;
                return 2;
            }
            else
                AddItemHistoriesLog(ITEM_FORGE_PROTECT,  c);
                //DBLOG().PushUpdateData("insert into `item_histories` (`server_id`, `player_id`, `item_id`, `item_num`, `use_time`) values(%u,%"I64_FMT"u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), ITEM_FORGE_PROTECT, c, TimeUtil::Now());
		}
		types[0] = ied.extraAttr2.type1;
		values[0] = ied.extraAttr2.value1;
		types[1] = ied.extraAttr2.type2;
		values[1] = ied.extraAttr2.value2;
		types[2] = ied.extraAttr2.type3;
		values[2] = ied.extraAttr2.value3;
		ConsumeInfo ci(ForgeEquipment,0,0);
		m_Owner->useTael(amount,&ci);
        UInt8 crr = equip->GetCareer();

        UInt8 equip_t = EQUIPTYPE_EQUIP;
        if(equip->GetItemType().subClass == Item_Trump)
        {
            equip_t = EQUIPTYPE_TRUMP;
            lv = ied.tRank;
        }

		getRandomAttr2(lv, crr, q, ied.extraAttr2.getCount(), protect, types, values, equip_t);

        float v0 = 0;
        if(equip_t == EQUIPTYPE_EQUIP)
            v0 = GObjectManager::getAttrMax(lv, types[0]-1, q, crr)*90;
        else if(equip_t == EQUIPTYPE_TRUMP)
            v0 = GObjectManager::getAttrTrumpMax(lv, types[0]-1, q, crr)*90;
        if ((float)values[0] > v0 && !(protect & 1))
        {
            SYSMSG_BROADCASTV(2203, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId());
        }

        float v1 = 0;
        if(equip_t == EQUIPTYPE_EQUIP)
            v1 = GObjectManager::getAttrMax(lv, types[1]-1, q, crr)*90;
        else if(equip_t == EQUIPTYPE_TRUMP)
            v1 = GObjectManager::getAttrTrumpMax(lv, types[1]-1, q, crr)*90;
        if ((float)values[1] > v1 && !(protect & 2))
        {
            SYSMSG_BROADCASTV(2203, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId());
        }

		ApplyAttr2(equip, types, values);
		if(!equip->GetBindStatus() && isBound)
			equip->DoEquipBind();
		if(fgt != NULL)
		{
			fgt->setDirty();
            if(equip->GetItemType().subClass == Item_Trump)
                fgt->sendModification(0x50 + pos, equip, false);
            else
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
		DB4().PushUpdateData("UPDATE `equipment` SET `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u", types[0], values[0], types[1], values[1], types[2], values[2], equip->getId());
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
			//SYSMSG_SENDV(190, m_Owner);
			return;
		}
		GameAction()->RunAutoRegen(m_Owner, fighter);
	}

	bool Package::DelItemAny( UInt32 id, UInt16 num, bool * hasBind, UInt8 toWhere )
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
				DelItem2(item, num, toWhere);
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
					DelItem2(item, bcount, toWhere);
				DelItem2(item2, num - bcount, toWhere);
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

    UInt8 Package::TrumpUpgrade(UInt16 fighterId, UInt32 trumpId, UInt32 itemId)
    {
		Fighter * fgt = NULL;
		UInt8 pos = 0;
        if(trumpId == itemId)
            return 2;
		ItemEquip * trump = FindEquip(fgt, pos, fighterId, trumpId);
		ItemEquip * item = FindEquip(fgt, pos, 0, itemId);
		if(trump == NULL || trump->getClass() != Item_Trump || item == NULL || item->getClass() != Item_Trump)
			return 2;

		bool isBound = item->GetBindStatus();
		ItemEquipData& ied_trump = trump->getItemEquipData();
		ItemEquipData& ied_item = item->getItemEquipData();

        UInt8 q = trump->getQuality();
        if(q < 2)
            return 2;
        UInt8 l = ied_trump.tRank;
        UInt32 rankUpExp = GObjectManager::getTrumpExpRank(q-2, l);
        if(l >= ied_trump.maxTRank || rankUpExp == 0)
            return 2;

        UInt8 item_q = item->getQuality();
        UInt8 item_enchant = ied_item.enchant;
		const GData::ItemBaseType& ibt = item->GetItemType();
        UInt32 exp = (ibt.trumpExp * GObjectManager::getTrumpSmelt(item_q - 2, item_enchant)) + ied_item.trumpExp * 0.5;
        if(exp == 0)
            return 2;

        if(!DelEquip(itemId, ToTrumpUpgrade))
            return 2;

        UInt8 q1 = q - 3;
        UInt8 oldTRank = ied_trump.tRank;
        UInt8 crr = trump->GetCareer();
        UInt8 types[3] = {0};
        UInt16 values[3] = {0};
        float maxv[3] = {0};

		types[0] = ied_trump.extraAttr2.type1;
		values[0] = ied_trump.extraAttr2.value1;
		types[1] = ied_trump.extraAttr2.type2;
		values[1] = ied_trump.extraAttr2.value2;
		types[2] = ied_trump.extraAttr2.type3;
		values[2] = ied_trump.extraAttr2.value3;

        if(ied_trump.tRank > 0 && q > 2)
        {
            UInt8 lv = ied_trump.tRank;
            maxv[0] = GObjectManager::getAttrTrumpMax(lv, types[0]-1, q1, crr);
            maxv[1] = GObjectManager::getAttrTrumpMax(lv, types[1]-1, q1, crr);
            maxv[2] = GObjectManager::getAttrTrumpMax(lv, types[2]-1, q1, crr);
        }

        ied_trump.trumpExp += exp;
        bool isRankUp = false;
        bool newAttr2 = false;
        while(ied_trump.trumpExp >= rankUpExp)
        {
            ++ied_trump.tRank;
            if(ied_trump.tRank == 1)
            {
                newAttr2 = true;
            }
            isRankUp = true;
            rankUpExp = GObjectManager::getTrumpExpRank(q-2, ied_trump.tRank);
            if(ied_trump.tRank >= ied_trump.maxTRank || rankUpExp == 0)
            {
                ied_trump.trumpExp = GObjectManager::getTrumpExpRank(q-2, ied_trump.maxTRank-1);
                break;
            }
        }

        if(newAttr2)
        {
            UInt8 lv = ied_trump.tRank;
            if(q > 2)
            {
                UInt8 t[3] = {0, 0, 0};
                Int16 v[3] = {0, 0, 0};
                getRandomAttr2(lv, crr, q1, 0, 0, t, v, EQUIPTYPE_TRUMP);
                ApplyAttr2(trump, t, v);
            }
        }

        if(oldTRank != ied_trump.tRank && q > 2 && oldTRank > 0)
        {
            UInt8 lv = ied_trump.tRank;
            Int16 values2[3] = {0};
            if(maxv[0] > 0.00001)
            {
                float v = GObjectManager::getAttrTrumpMax(lv, types[0]-1, q1, crr);
                float tmp = (v * static_cast<float>(values[0])) / maxv[0] + 0.9;
                if(tmp > v * 100)
                    values2[0] = v * 100;
                else
                    values2[0] = tmp;

            }
            if(maxv[1] > 0.00001)
            {
                float v = GObjectManager::getAttrTrumpMax(lv, types[1]-1, q1, crr);
                float tmp = (v * static_cast<float>(values[1])) / maxv[1] + 0.9;
                if(tmp > v * 100)
                    values2[1] = v * 100;
                else
                    values2[1] = tmp;
            }
            if(maxv[2] > 0.00001)
            {
                float v = GObjectManager::getAttrTrumpMax(lv, types[2]-1, q1, crr);
                float tmp = (v * static_cast<float>(values[2])) / maxv[2] + 0.9;
                if(tmp > v * 100)
                    values2[2] = v * 100;
                else
                    values2[2] = tmp;
            }
            ApplyAttr2(trump, types, values2);
        }

        if(isRankUp)
        {
			DB4().PushUpdateData("UPDATE `equipment` SET `tRank` = %u, `trumpExp` = %u WHERE `id` = %u", ied_trump.tRank, ied_trump.trumpExp, trump->getId());
            DBLOG().PushUpdateData("insert into upgrade_histories (server_id, player_id, equip_id, template_id, equip_rank, upgrade_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), trump->getId(), trump->GetItemType().getId(), ied_trump.tRank, TimeUtil::Now());
        }
        else
        {
			DB4().PushUpdateData("UPDATE `equipment` SET `trumpExp` = %u WHERE `id` = %u", ied_trump.trumpExp, trump->getId());
        }

		if(!trump->GetBindStatus() && isBound)
			trump->DoEquipBind();

		if(fgt != NULL)
        {
            if(l != ied_trump.tRank)
                fgt->setDirty();
			fgt->sendModification(0x50 + pos, trump, false);
        }
		else
			SendSingleEquipData(trump);

        return 0;
    }

    UInt8 Package::TrumpLOrder(UInt16 fighterId, UInt32 trumpId)
    {
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * trump = FindEquip(fgt, pos, fighterId, trumpId);
		if(trump == NULL || trump->getClass() != Item_Trump)
			return 2;

        UInt8 q = trump->getQuality();
        if(q < 2)
            return 2;

		ItemEquipData& ied_trump = trump->getItemEquipData();
        UInt8 l = ied_trump.maxTRank;
        if(l >= MAX_TRUMP_LORDER_ITEM || l < 1)
            return 2;

		bool isBound = trump->GetBindStatus();

        if(!DelItemAny(TRUMP_LORDER_ITEM, 1, &isBound))
            return 2;
        AddItemHistoriesLog(TRUMP_LORDER_ITEM, 1);

        UInt32 chance = GObjectManager::getTrumpLOrderChance(q-2, l-1);
        if(uRand(1000) >= chance)
            return 1;

        ++ ied_trump.maxTRank;
        DB4().PushUpdateData("UPDATE `equipment` SET `maxTRank` = %u WHERE `id` = %u", ied_trump.maxTRank, trump->getId());
        DBLOG().PushUpdateData("insert into lorder_histories (server_id, player_id, equip_id, template_id, equip_maxrank, upgrade_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u)", cfg.serverLogId, m_Owner->getId(), trump->getId(), trump->GetItemType().getId(), ied_trump.maxTRank, TimeUtil::Now());

		if(!trump->GetBindStatus() && isBound)
			trump->DoEquipBind();
		if(fgt != NULL)
			fgt->sendModification(0x50 + pos, trump, false);
		else
			SendSingleEquipData(trump);

        return 0;
    }

}
