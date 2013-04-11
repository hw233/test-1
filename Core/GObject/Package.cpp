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
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "GData/LBSkillTable.h"
#include "Common/Itoa.h"
#include "LBNameTmpl.h"

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
#define ITEM_ACTIVATE_ATTR 549
#define TRUMP_LORDER_ITEM       517      // 玲珑冰晶
#define MAX_TRUMP_LORDER_ITEM   9        // 最高阶
#define SPIRIT_TRANS_ITEM   548    // 转魂符
#define SPIRIT_LEVEL_MAX    100    // 最大注灵等级

namespace GObject
{
#define EQUIPTYPE_EQUIP 0
#define EQUIPTYPE_TRUMP 1

    extern URandom GRND;

    // 注灵类型 spiritType[属性条][装备类型]
    // 属性条:1, 2, 3, 4
    // 装备类洗:1-武器，2-护头，3-护胸，4-护肩，5-护腰，6-护腿，7-项链，8-戒指
    // spiritType:0-攻击，1-防御，2-暴击，3-破击，4-身法，5-坚韧，6-暴击伤害，7-生命
    static int spiritType[4][8] = {
        {0, 1, 1, 1, 1, 1, 0, 0},
        {3, 4, 4, 4, 4, 4, 7, 7},
        {2, 3, 5, 2, 5, 5, 3, 2},
        {4, 7, 7, 7, 7, 7, 6, 6}
    };
    static UInt32 baseSpiritItem = 7000;

    UInt16 getRandOEquip(UInt8 lvl)
    {
        static const UInt16* equips[] = {OEquip50, OEquip60, OEquip70, OEquip80, OEquip90, OEquip100};

        UInt16 equipid = 0;

        if (lvl < 50)
            lvl = 50;
        if (lvl > 100)
            lvl = 100;

        lvl -= 50;
        lvl /= 10;

        UInt8 idx = GRND(24);
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

        UInt8 idx = GRND(24);
        equipid = equips[lvl][idx];
        return equipid;
    }

    static UInt32  gem[] = { 5001, 5011, 5021, 5031, 5041, 5051, 5061, 5071, 5081, 5091, 5101, 5111, 5121, 5131, 5141 };
    UInt32 getRandGem(UInt8 lev)
    {
        UInt8 idx = GRND(15);

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
	//static UInt32 merge_chance[9] = {100, 85, 85, 75, 75, 65, 65, 55, 55};
	/*static UInt32 activate_cost[11][3] = {
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
        */
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

    static void getRandomLBAttr(UInt8 lv, UInt8 subClass, UInt8 color, ItemLingbaoAttr& lbattr)
    {
        if(subClass != Item_LBling && subClass != Item_LBwu && subClass != Item_LBxin)
            return;
		stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
        UInt8 attrNum = lbAttrConf.getAttrNum(uRand(100));
        if(color > 3)
            attrNum = 4;

        std::vector<UInt8> allAttrType = lbAttrConf.attrType;
        UInt8 itemTypeIdx = subClass - Item_LBling;
        for(int i = 0; i < attrNum; ++ i)
        {
            UInt8 size = allAttrType.size();
            UInt8 idx = uRand(size);
            lbattr.type[i] = allAttrType[idx];
            float fChance = ((float)(uRand(10000)))/10000;
            UInt8 tmpcolor = 2 + lbAttrConf.getColor(lv, itemTypeIdx, lbattr.type, lbattr.value, attrNum);
            if(tmpcolor >= color)
                color = 2;
            lbattr.value[i] = lbAttrConf.getAttrMax(lv, itemTypeIdx, lbattr.type[i]-1) * lbAttrConf.getDisFactor4(uRand(10000), fChance, color) + 0.9999f;
            allAttrType.erase(allAttrType.begin() + idx);
            if(i + 1 == attrNum)
            {
                if(tmpcolor  < color)
                {
                    ++ attrNum;
                    if(attrNum == 5)
                    {
                        -- attrNum;
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        lbattr.lbColor = 2 + lbAttrConf.getColor(lv, itemTypeIdx, lbattr.type, lbattr.value, attrNum);
        // 以防精确度问题，导致颜色略微偏低
        //if(lbattr.lbColor < color)
        //    lbattr.lbColor = color;
        if(lbattr.lbColor == 5)
        {
            UInt8 skillSwitch = lbAttrConf.getSkillSwitch(uRand(100));
            UInt8 startIdx = 0;
            UInt8 endIdx = 0;
            switch(skillSwitch)
            {
            case 0:
                endIdx = 1;
                break;
            case 1:
                startIdx = 1;
                endIdx = 2;
                break;
            case 2:
                endIdx = 2;
                break;
            }
            for(int i = startIdx; i < endIdx; ++ i)
            {
                UInt16 lbIdx = 0;
                if(i > 0)
                    lbIdx = subClass - Item_LBling + 1;
                UInt8 maxCnt = lbAttrConf.getSkillsMaxCnt(lbIdx, lv);
                UInt16 skillId = lbAttrConf.getSkill(lbIdx, lv, uRand(maxCnt));
                lbattr.skill[i-startIdx] = skillId;
                UInt16 factor = GData::lbSkillManager[skillId]->minFactor;
                lbattr.factor[i-startIdx] = factor + uRand(10000-factor);
            }
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
			if(cur > m_Owner->getPacksize() + 50)
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
				//if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
                //{
                    AddItemCoursesLog(typeId, num, fromWhere);
                //}
                if (fromWhere != FromNpcBuy && (GData::store.getPrice(typeId) || GData::GDataManager::isInUdpItem(typeId)))
                {
                    udpLog(item->getClass(), typeId, num, 0, "add");
                }
                cittaUdpLog(1, typeId, num);
                secondSoulItemUdpLog(1, typeId, num);
                if (fromWhere == FromQixi)
                {
                    qixiUdpLog(typeId, num);
                }

                // 太乙精金的特殊记录
                if (typeId == 503 && bind!= true)
                {
                    m_Owner->udpLog("item", "I_503_1_2", "", "", "", "", "act", num);
                }

                // 这是什么道具要统计？文档里没找到。。。
                if (typeId == 550)
                {
                    char strBuf[32] = "";
                    m_Owner->udpLog("item", "I_550_1", "", "", "", "", "act", num);
                    snprintf(strBuf, 32, "I_550_1_%d", fromWhere);
                    m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
                }

                if (typeId == 9193)
                {
                    char strBuf[32] = "";
                    m_Owner->udpLog("item", "I_9193_1", "", "", "", "", "act", num);
                    snprintf(strBuf, 32, "I_9193_1_%d", fromWhere);
                    m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
                }

                if (fromWhere == FromKillMonster)
                {
                    char strBuf[32] = "";
                    snprintf(strBuf, 32, "I_%u_1_%u", typeId, (UInt32)fromWhere);
                    m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
                }

                if (typeId >= 48 && typeId <= 51)//乾坤净水等4个物品
                {   
                    char udpStr[32] = {0};            
                    sprintf(udpStr, "F_1140%d%d_%d", bind, typeId, fromWhere);
                    m_Owner->udpLog("tripod", udpStr, "", "", "", "", "act", num);
                }
		
                if (typeId >= 5001 && typeId <= 5152)// 宝石
                {   
                    char udpStr[32] = {0};            
                    sprintf(udpStr, "I_%d_%d_%d", bind, typeId, fromWhere);
                    m_Owner->udpLog("item", udpStr, "", "", "", "", "act", num);
                }

                if (typeId == 1209)
                    m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 0);
                if (typeId == 1223)
                    m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 1);
                if (typeId == 1224)
                    m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 2);
                if (item->getClass() == Item_Citta)
                    m_Owner->OnShuoShuo(SS_CITTA);
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
                if (fromWhere != FromNpcBuy && (GData::store.getPrice(typeId) || GData::GDataManager::isInUdpItem(typeId)))
                {
                    udpLog(item->getClass(), typeId, num, 0, "add");
                }
                cittaUdpLog(1, typeId, num);
                secondSoulItemUdpLog(1, typeId, num);
               
                if (fromWhere == FromQixi)
                {
                    qixiUdpLog(typeId, num);
                }

                // 太乙精金的特殊记录
                if (typeId == 503 && bind!= true)
                {
                    m_Owner->udpLog("item", "I_503_1_2", "", "", "", "", "act", num);
                }

                // 这是什么道具要统计？文档里没找到。。。
                if (typeId == 550)
                {
                    char strBuf[32] = "";
                    m_Owner->udpLog("item", "I_550_1", "", "", "", "", "act", num);
                    snprintf(strBuf, 32, "I_550_1_%d", fromWhere);
                    m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
                }

                if (typeId == 9193)
                {
                    char strBuf[32] = "";
                    m_Owner->udpLog("item", "I_9193_1", "", "", "", "", "act", num);
                    snprintf(strBuf, 32, "I_9193_1_%d", fromWhere);
                    m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
                }

                if (fromWhere == FromKillMonster)
                {
                    char strBuf[32] = "";
                    snprintf(strBuf, 32, "I_%u_1_%u", typeId, (UInt32)fromWhere);
                    m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
                }
                if (typeId >= 48 && typeId <= 51)//乾坤净水等4个物品
                {   
                    char udpStr[32] = {0};            
                    sprintf(udpStr, "F_1140%d%d_%d", bind, typeId, fromWhere);
                    m_Owner->udpLog("tripod", udpStr, "", "", "", "", "act", num);
                 }
                if (typeId >= 5001 && typeId <= 5152)// 宝石
                {   
                    char udpStr[32] = {0};            
                    sprintf(udpStr, "I_%d_%d_%d", bind, typeId, fromWhere);
                    m_Owner->udpLog("item", udpStr, "", "", "", "", "act", num);
                }
	
                //增加获取物品的荣誉
                //GameAction()->doAttainment(m_Owner, Script::ON_ADD_ITEM, typeId);
                if (typeId == 1209)
                    m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 0);
                if (typeId == 1223)
                    m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 1);
                if (typeId == 1224)
                    m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 2);
                if (item->getClass() == Item_Citta)
                    m_Owner->OnShuoShuo(SS_CITTA);
				SendItemData(item);
				if(notify)
					ItemNotify(item->GetItemType().getId(), num);
				//if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
                     AddItemCoursesLog(typeId, num, fromWhere);
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
            //if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
            //{
                AddItemCoursesLog(typeId, static_cast<UInt32>(count), fromWhere);
            //}
            if (fromWhere != FromNpcBuy && (GData::store.getPrice(typeId) || GData::GDataManager::isInUdpItem(typeId)))
            {
                udpLog(item->getClass(), typeId, count, 0, "add");
            }
            cittaUdpLog(1, typeId, count);
            secondSoulItemUdpLog(1, typeId, count);
            if (fromWhere == FromQixi)
            {
                qixiUdpLog(typeId, count);
            }
            // 太乙精金的特殊记录
            if (typeId == 503 && bind!= true)
            {
                m_Owner->udpLog("item", "I_503_1_2", "", "", "", "", "act", count);
            }

            // 这是什么道具要统计？文档里没找到。。。
            if (typeId == 550)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_550_1", "", "", "", "", "act", count);
                snprintf(strBuf, 32, "I_550_1_%d", fromWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", count);
            }

            if (typeId == 9193)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_9193_1", "", "", "", "", "act", count);
                snprintf(strBuf, 32, "I_9193_1_%d", fromWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", count);
            }
            if (fromWhere == FromKillMonster)
            {
                char strBuf[32] = "";
                snprintf(strBuf, 32, "I_%u_1_%u", typeId, (UInt32)fromWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", count);
            }
            if (typeId >= 48 && typeId <= 51)//乾坤净水等4个物品
            {   
                char udpStr[32] = {0};            
                sprintf(udpStr, "F_1140%d%d_%d", bind, typeId, fromWhere);
                m_Owner->udpLog("tripod", udpStr, "", "", "", "", "act", count);
            }
                if (typeId >= 5001 && typeId <= 5152)// 宝石
                {   
                    char udpStr[32] = {0};            
                    sprintf(udpStr, "I_%d_%d_%d", bind, typeId, fromWhere);
                    m_Owner->udpLog("item", udpStr, "", "", "", "", "act", count);
                }
	
            if (typeId == 1209)
                m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 0);
            if (typeId == 1223)
                m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 1);
            if (typeId == 1224)
                m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 2);
            if (item->getClass() == Item_Citta)
                m_Owner->OnShuoShuo(SS_CITTA);
			return item;
		}
		else
		{
			UInt32 newSize = m_Size + item->Size();
			if(newSize > (UInt32)(m_Owner->getPacksize()) + 50)
				return item;
			m_Size = newSize;
			m_Items[ItemKey(typeId, bind)] = item;
			DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, count, m_Owner->getId(), bind ? 1 : 0);
            if (fromWhere != FromNpcBuy && (GData::store.getPrice(typeId) || GData::GDataManager::isInUdpItem(typeId)))
            {
                udpLog(item->getClass(), typeId, count, 0, "add");
            }
            cittaUdpLog(1, typeId, count);
            secondSoulItemUdpLog(1, typeId, count);
            if (fromWhere == FromQixi)
            {
                qixiUdpLog(typeId, count);
            }
            // 太乙精金的特殊记录
            if (typeId == 503 && bind!= true)
            {
                m_Owner->udpLog("item", "I_503_1_2", "", "", "", "", "act", count);
            }

            // 这是什么道具要统计？文档里没找到。。。
            if (typeId == 550)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_550_1", "", "", "", "", "act", count);
                snprintf(strBuf, 32, "I_550_1_%d", fromWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", count);
            }

            if (typeId == 9193)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_9193_1", "", "", "", "", "act", count);
                snprintf(strBuf, 32, "I_9193_1_%d", fromWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", count);
            }
            if (fromWhere == FromKillMonster)
            {
                char strBuf[32] = "";
                snprintf(strBuf, 32, "I_%u_1_%u", typeId, (UInt32)fromWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", count);
            }

            SendItemData(item);
            ItemNotify(item->GetItemType().getId(), count);
            //获得物品
            //GameAction()->doAttainment(m_Owner, Script::ON_ADD_ITEM, typeId);
            //if((fromWhere != 0  && item->getQuality() >= 3) || (fromWhere == FromMerge && item->getQuality() >= 2))
            //{
            AddItemCoursesLog(typeId, static_cast<UInt32>(count), fromWhere);
            //}
            if (typeId >= 48 && typeId <= 51)//乾坤净水等4个物品
            {   
                char udpStr[32] = {0};            
                sprintf(udpStr, "F_1140%d%d_%d", bind, typeId, fromWhere);
                m_Owner->udpLog("tripod", udpStr, "", "", "", "", "act", count);
            }
                if (typeId >= 5001 && typeId <= 5152)// 宝石
                {   
                    char udpStr[32] = {0};            
                    sprintf(udpStr, "I_%d_%d_%d", bind, typeId, fromWhere);
                    m_Owner->udpLog("item", udpStr, "", "", "", "", "act", count);
                }
	           
            if (typeId == 1209)
                m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 0);
            if (typeId == 1223)
                m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 1);
            if (typeId == 1224)
                m_Owner->OnHeroMemo(MC_CITTA, MD_LEGEND, 0, 2);
            if (item->getClass() == Item_Citta)
                m_Owner->OnShuoShuo(SS_CITTA);
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
		if(m_Size >= m_Owner->getPacksize() + 50)
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
        case Item_Halo:
        case Item_Fashion:
        case Item_Trump:
        case Item_LBling:
        case Item_LBwu:
        case Item_LBxin:
			{
				ItemEquip * equip;
				ItemEquipData edata;

				UInt8 lv = itype->vLev;
                UInt8 crr = itype->career;
                bool randomAttr = false;
				switch(itype->subClass)
                {
                case Item_Trump:
                case Item_Fashion:
                case Item_Halo:
                case Item_LBling:
                case Item_LBwu:
                case Item_LBxin:
                    break;
                default:
                    if(itype->quality > 2)
                        randomAttr = true;
                }
				if(randomAttr)
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

                bool lingbao = false;
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
                case Item_Halo:
                case Item_Fashion:
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
                        if (itype->subClass == Item_Halo)
                        {
                            equip = new ItemHalo(id, itype, edata);
                        }
                        else if (itype->subClass == Item_Fashion)
                        {
                            equip = new ItemFashion(id, itype, edata);
                        }
                        else
                        {
                            equip = new ItemTrump(id, itype, edata);
                            if (equip && equip->getItemEquipData().enchant)
                                ((ItemTrump*)equip)->fixSkills();
                        }
                    }
                    break;
                case Item_LBling:
                case Item_LBwu:
                case Item_LBxin:
                    {
                        ItemLingbaoAttr lbattr;
                        getRandomLBAttr(lv, itype->subClass, itype->quality, lbattr);
                        equip = new ItemLingbao(id, itype, edata, lbattr);
                        lingbao = true;
                        switch(lbattr.lbColor)
                        {
                        case 2:
                            m_Owner->udpLog("Tongling", "F_10000_1", "", "", "", "", "act");
                            break;
                        case 3:
                            m_Owner->udpLog("Tongling", "F_10000_2", "", "", "", "", "act");
                            break;
                        case 4:
                            m_Owner->udpLog("Tongling", "F_10000_3", "", "", "", "", "act");
                            break;
                        case 5:
                            m_Owner->udpLog("Tongling", "F_10000_4", "", "", "", "", "act");
                            break;
                        }

                        std::string strType;
                        std::string strValue;
                        std::string strSkill;
                        std::string strFactor;
                        for(int i = 0; i < 4; ++ i)
                        {
                            strType += Itoa(lbattr.type[i], 10);
                            strValue += Itoa(lbattr.value[i], 10);

                            if(i < 3)
                            {
                                strType += ',';
                                strValue += ',';
                            }
                            if(i < 2)
                            {
                                strSkill += Itoa(lbattr.skill[i], 10);
                                strFactor += Itoa(lbattr.factor[i], 10);

                                if(i < 1)
                                {
                                    strSkill += ',';
                                    strFactor += ',';
                                }
                            }
                        }
                        DB4().PushUpdateData("REPLACE INTO `lingbaoattr`(`id`, `tongling`, `lbcolor`, `types`, `values`, `skills`, `factors`, `battlepoint`) VALUES(%u, %d, %d, '%s', '%s', '%s', '%s', '%u')", id, lbattr.tongling, lbattr.lbColor, strType.c_str(), strValue.c_str(), strSkill.c_str(), strFactor.c_str(), lbattr.battlePoint);
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

                if (itype->subClass != Item_Trump && itype->subClass != Item_Fashion && itype->subClass != Item_Halo && itype->quality == 5)
                    m_Owner->OnShuoShuo(SS_OE);
                if (itype->subClass == Item_Trump || itype->subClass == Item_Halo || itype->subClass == Item_Fashion)
                    m_Owner->OnShuoShuo(SS_TRUMP);

				ItemBase *& e = m_Items[id];
				if(e == NULL)
					++ m_Size;
				e = equip;
				DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", id, m_Owner->getId(), bind ? 1 : 0);
				DB4().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `maxTRank`, `trumpExp`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`) VALUES(%u, %u, %u, %u, %u, %d, %u, %d, %u, %d)", id, typeId, edata.maxTRank, edata.trumpExp, edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3);
                GenSpirit(equip);

				SendSingleEquipData(equip);
				if(notify)
					ItemNotifyEquip(equip);
				if((FromWhere != 0 && itype->quality >= 4) || lingbao)
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, id, FromWhere, TimeUtil::Now());

                OnAddEquipAndCheckAttainment(itype, FromWhere);
				return equip;
			}
		default:
			break;
		}
		return NULL;
	}

    ItemEquip* Package::AddUpgradeEquip(UInt32 typeId, UInt32 oldEquipId, bool notify, bool bind , ItemEquipData& ed, float * maxv)
    {
        const GData::ItemBaseType * itype = CheckBeforeEquipUpgrade(typeId);
        if(itype == NULL)
            return NULL;

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
        DB4().PushUpdateData("UPDATE `equipment_spirit` SET `id` = %u WHERE `id` = %u", id, oldEquipId);
        GenSpirit2(equip);
        SendSingleEquipData(equip);
        if(notify)
            ItemNotifyEquip(equip);
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
            if(itype->subClass == Item_Trump || itype->subClass == Item_Halo || itype->subClass == Item_Fashion)
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
		if((UInt32)(GetRestPackageSize()) + 50 < num)
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

        if (equip->getClass() != Item_Trump && equip->getClass() != Item_Fashion && equip->getClass() != Item_Halo && equip->getQuality() == 5)
            m_Owner->OnShuoShuo(SS_OE);
        if (equip->getClass() == Item_Trump || equip->getClass() == Item_Halo || equip->getClass() == Item_Fashion)
            m_Owner->OnShuoShuo(SS_TRUMP);

		DB4().PushUpdateData("REPLACE INTO `item` VALUES(%u, %u, %"I64_FMT"u, %d)", equip->getId(), 1, m_Owner->getId(), equip->GetBindStatus() ? 1 : 0);
		SendSingleEquipData(equip);
		ItemNotifyEquip(equip);
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
        if (!toWhere)
            toWhere = ToDelete;
		ItemBase * item = FindItem(id, bind);
		if (item == NULL) return false;
		bool ret = TryDelItem(item, num);
		if (ret)
		{
			UInt16 cnt = item->Count();

            //if((toWhere!= 0  && item->getQuality() >= 3) || (toWhere == ToGemMgerge && item->getQuality() >= 2))
            //{
				std::string tbn("item_courses");
				DBLOG().GetMultiDBName(tbn);
				DBLOG().PushUpdateData("insert into  `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str(), cfg.serverLogId, m_Owner->getId(), item->GetItemType().getId(), num, toWhere, TimeUtil::Now());
            //}

            UInt32 price = GData::store.getPrice(id);
            if (price || GData::GDataManager::isInUdpItem(id))
            {
                udpLog(item->getClass(), id, num, price, "sub");
            }

            cittaUdpLog(2, id, num);
            if (toWhere == ToSkillStrengthenOpen || toWhere == ToSkillStrengthenUpgrade)
                cittaUdpLog(3, id, num);

            // 太乙精金的特殊记录
            if (id == 503 && bind!= true)
            {
                m_Owner->udpLog("item", "I_503_2_2", "", "", "", "", "act", num);
            }

            // 这是什么道具要统计？文档里没找到。。。
            if (id == 550)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_550_2", "", "", "", "", "act", num);
                snprintf(strBuf, 32, "I_550_2_%d", toWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
            }

            if (id == 9193)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_9193_2", "", "", "", "", "act", num);
                snprintf(strBuf, 32, "I_9193_2_%d", toWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
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
        if (!toWhere)
            toWhere = ToDelete;
		bool ret = TryDelItem(item, num);
		if (ret)
		{
			UInt16 cnt = item->Count();

            //if((toWhere!= 0  && item->getQuality() >= 3) || (toWhere == ToGemMgerge && item->getQuality() >= 2))
            //{
				std::string tbn("item_courses");
				DBLOG().GetMultiDBName(tbn);
				DBLOG().PushUpdateData("insert into `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str() ,cfg.serverLogId, m_Owner->getId(), item->GetItemType().getId(), num, toWhere, TimeUtil::Now());
            //}

            UInt32 price = GData::store.getPrice(item->getId());
            if (price || GData::GDataManager::isInUdpItem(item->getId()))
            {
                udpLog(item->getClass(), item->getId(), num, price, "sub");
            }
            cittaUdpLog(2, item->getId(), num);
            if (toWhere == ToSkillStrengthenOpen || toWhere == ToSkillStrengthenUpgrade)
                cittaUdpLog(3, item->getId(), num);

            // 太乙精金的特殊记录
            if (item->getId() == 503 && item->GetBindStatus()!= true)
            {
                m_Owner->udpLog("item", "I_503_2_2", "", "", "", "", "act", num);
            }

            // 这是什么道具要统计？文档里没找到。。。
            if (item->getId() == 550)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_550_2", "", "", "", "", "act", num);
                snprintf(strBuf, 32, "I_550_2_%d", toWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
            }

            if (item->getId() == 9193)
            {
                char strBuf[32] = "";
                m_Owner->udpLog("item", "I_9193_2", "", "", "", "", "act", num);
                snprintf(strBuf, 32, "I_9193_2_%d", toWhere);
                m_Owner->udpLog("item", strBuf, "", "", "", "", "act", num);
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
        if(Item_LBling <= item->GetItemType().subClass || Item_LBxin >= item->GetItemType().subClass)
        {
            DB4().PushUpdateData("DELETE FROM `lingbaoattr` WHERE `id`=%u", id);
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
        if(Item_LBling <= equip->GetItemType().subClass || Item_LBxin >= equip->GetItemType().subClass)
        {
            DB4().PushUpdateData("DELETE FROM `lingbaoattr` WHERE `id`=%u", equip->getId());
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
            case Item_Halo:
                return 0x1f;

            case Item_Fashion:
                return 0x20;

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
            case 0x1f:
				if(item->getClass() != Item_Halo)
					return false;
				old = fgt->setHalo(static_cast<GObject::ItemHalo*>(item));
                break;
            case 0x20:
				if(item->getClass() != Item_Fashion)
					return false;
				old = fgt->setFashion(static_cast<GObject::ItemFashion*>(item));
                break;
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
            case 0x0a:
            case 0x0b:
            case 0x0c:
                if(item->getClass() != Item_Trump)
                    return false;
                if (fgt->canSetTrump(part-0x0a, item->getId()) && !m_Owner->checkTrumpMutually(item->GetItemType().getId()))
                    old = fgt->setTrump(static_cast<GObject::ItemTrump*>(item), part-0x0a);
                else
                    return false;
                break;
            case 0x60:
            case 0x61:
            case 0x62:
                old = fgt->setLingbao(part-0x60, static_cast<GObject::ItemLingbao*>(item));
                fgt->eraseLingbaoInfo(old);
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
            case 0x1f:
				old = fgt->setHalo(NULL);
                break;
            case 0x20:
				old = fgt->setFashion(NULL);
                break;
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
            case 0x0a:
            case 0x0b:
            case 0x0c:
                old = fgt->setTrump(static_cast<GObject::ItemTrump*>(NULL), part-0x0a);
                break;
            case 0x60:
            case 0x61:
            case 0x62:
                old = fgt->setLingbao(part-0x60, static_cast<GObject::ItemLingbao*>(NULL));
                fgt->eraseLingbaoInfo(old);
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
        if (!World::canDestory(id)) return 0;
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

	bool Package::UseItem(UInt32 id, UInt16 num, UInt8 type, UInt32 param, UInt8 bind)
	{
        if(!m_Owner->hasChecked())
			return false;
		bool ret = false;
        UInt8 ret2 = 0;
        // XXX: 0-使用 1-合成 2-分解
        // XXX: 0,2同时走使用流程
        if (type == 1)
        {
            ItemBase* item = GetItem(id, bind > 0);
            if(2 == bind){ //2:优先使用绑定材料合成
                if( !item )
                    item = GetItem(id, false);
            }
            if( !item ){
                m_Owner->sendMsgCode(0, 1802);
                return false;
            }
            if (GetItemSubClass(id) == Item_Formula)
            {
                if (item && item->getClass() == Item_Formula5)
                {
                    ret2 = FormulaMerge(id, bind, num);
                    if (1 == ret2){
                        multiMergeUdpLog(num);
                        m_Owner->sendMsgCode(0, 1800);
                        return true;
                    }
                    else if(0 == ret2){
                        m_Owner->sendMsgCode(0, 1802);
                        return false;
                    }
                    else{
                        m_Owner->sendMsgCode(0, 1011);
                        return false;
                    }
                }
            }
            else if (GetItemSubClass(id) == Item_Citta)
            {
                if (item && item->getClass() == Item_Citta5)
                {
                    ret2 = CittaMerge(id, bind, num);
                    if (1 == ret2){
                        m_Owner->sendMsgCode(0, 1800);
                        multiMergeUdpLog(num);
                        return true;
                    }
                    else if(0 == ret2){
                        m_Owner->sendMsgCode(0, 1802);
                        return false;
                    }
                    else{
                        m_Owner->sendMsgCode(0, 1011);
                        return false;
                    }
                }
            }
            else if (GetItemSubClass(id) == Item_Normal ||
                    GetItemSubClass(id) == Item_Soul ||
                    GetItemSubClass(id) == Item_SL)
            {
                if (item && (item->getClass() == Item_Normal29 ||
                            item->getClass() == Item_Normal28 ||
                            (item->getClass() >= Item_Soul && item->getClass() <= Item_Soul9) ||
                            item->getClass() == Item_SL1 ||
                            item->getClass() == Item_SL2))
                {
                    ret2 = TrumpMerge(id, bind, num);
                    if (1 == ret2){
                        m_Owner->sendMsgCode(0, 1800);
                        multiMergeUdpLog(num);
                        return true;
                    }
                    else if(0 == ret2){
                        m_Owner->sendMsgCode(0, 1802);
                        return false;
                    }
                    else{
                        m_Owner->sendMsgCode(0, 1011);
                        return false;
                    }
                }
            }
            return false;
        }

        if (num == 0 || IsEquipId(id) ||
                (GetItemSubClass(id) != Item_Normal &&
                 GetItemSubClass(id) != Item_Formula &&
                 GetItemSubClass(id) != Item_Enhance &&
                GetItemSubClass(id) != Item_Citta &&
                GetItemSubClass(id) != Item_Soul)){
            ret = false;}
		else
		{
            //if (GetItemSubClass(id) == Item_Formula || GetItemSubClass(id) == Item_Citta) // XXX: confirm in lua
            //    num = 1;

			if (bind != 0xFF)
			{
				ItemBase* item = GetItem(id, bind > 0);
                if (item == NULL || item->Count() < num)
					ret = false;
				else if (UInt16 n = GameAction()->RunItemNormalUse(m_Owner, id, param, num, bind > 0))
				{
                    UInt16 rn = n<num?n:num;
					DelItem2(item, rn);
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
                    UInt16 rn = n<num?n:num;
                    ItemBase * item = FindItem(id, true);
                    if (!item)
                        item = FindItem(id, false);
					DelItemAny(id, rn);
                    AddItemHistoriesLog(id, rn);
                    ret = true;
				}
			}

            if(ret) //变强之路
            {
                UInt32 guaji[] = { 55, 56, 9100, 9092, 9126, 9141, 9142, 9143 };
                UInt32 xiuwei[] = { 57, 9093, 9133, 9144, 9145, 9146 };
                for(UInt32 i = 0; i < sizeof(guaji) / sizeof(guaji[0]); ++i)
                {
                    if(guaji[i] == id)
                        GameAction()->doStrong(m_Owner, SthHookUse, 0, 0);
                }
                for(UInt32 i = 0; i < sizeof(xiuwei) / sizeof(xiuwei[0]); ++i)
                {
                    if(xiuwei[i] == id)
                        GameAction()->doStrong(m_Owner, SthPUse, 0, 0);
                }
            }
            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(m_Owner, created));
            if(ret == true && id == 449 && thisDay <= endDay)
            {
                // 新注册七日内开启首充礼包，完成每日目标
                UInt32 targetVal = m_Owner->GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_RECHARGE_PACKGE))
                {
                    targetVal |=TARGET_RECHARGE_PACKGE;
                    m_Owner->AddVar(VAR_CTS_TARGET_COUNT, 1);
                    m_Owner->SetVar(VAR_CLAWARD2, targetVal);
                    m_Owner->sendNewRC7DayTarget();
                    m_Owner->newRC7DayUdpLog(1152, 11);
                }
            }

		}

		Stream st(REP::PACK_USE);
		st << id << static_cast<UInt8>(1) << static_cast<UInt8>(ret ? 1 : 0) << Stream::eos;
		m_Owner->send(st);
		return ret;
	}
    bool Package::UseItemOther(UInt32 id, UInt16 num, std::string& name, UInt8 bind)
    {
		if(!m_Owner->hasChecked())
			return false;
        if (!num)
            return false;
		bool ret = false;

        if (GetItemSubClass(id) != Item_Normal)
            return ret;

		Stream st(REP::PACK_USE_OTHER);
        Player* other = globalNamedPlayers[m_Owner->fixName(name)];
        if (other == m_Owner)
            return ret;

        if (other)
        {
            if (bind != 0xFF)
            {
                ItemBase* item = GetItem(id, bind > 0);
                if (item == NULL || item->Count() < num)
                    ret = false;
                else if (UInt16 n = GameAction()->RunItemNormalUseOther(m_Owner, id, other, num, bind > 0))
                {
                    UInt16 rn = n<num?n:num;
                    DelItem2(item, rn);
                    AddItemHistoriesLog(id, rn);
                    ret = true;
                }
            }
            else
            {
                if (GetItemAnyNum(id) < num)
                    ret = false;
                else if (UInt16 n = GameAction()->RunItemNormalUseOther(m_Owner, id, other, num, false))
                {
                    UInt16 rn = n<num?n:num;
                    ItemBase * item = FindItem(id, true);
                    if (!item)
                        item = FindItem(id, false);
                    DelItemAny(id, rn);
                    AddItemHistoriesLog(id, rn);
                    ret = true;
                }
            }
            st << id << static_cast<UInt8>(ret ? 1 : 0) << Stream::eos;
        }
        else
            st << id << static_cast<UInt8>(2) << Stream::eos;

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
        if (!op)
            return;
        char _price[32] = {0};
        char _type[32] = {0};
        char _id[32] = {0};
        snprintf(_price, 32, "%u", price);
        UInt8 t = GetItemLogType(id);
        snprintf(_type, 32, "%u", t);
        snprintf(_id, 32, "%u", id);
        m_Owner->udpLog(op, _type, _id, _price, "", "", "props", num);
    }

    void Package::cittaUdpLog(UInt8 type, UInt32 id, UInt32 num)
    {
        char itemAct[32] = "";

        // 是否是解封石
        if (id != 550 && id != 551)
        {
            // 是否属于心法书/残卷范围
            if (! ((id >= LCITTA_ID && id <=RCITTA_ID) || (id >= LCITTA1_ID && id <= RCITTA1_ID)))
                return;
        }

        if (type < 1 || type > 3)
            return;

        snprintf (itemAct, 32, "%d_%d", id, type);
        m_Owner->udpLog("citta", itemAct, "", "", "", "", "act", num);
    }

    void Package::secondSoulItemUdpLog(UInt8 type, UInt32 id, UInt32 num)
    {
        char itemAct[32] = "";

        if (id < LSL_ID || id > RSL_ID)
            return;

        if (type < 1 || type > 3)
            return;

        snprintf (itemAct, 32, "I_%d_%d", id, type);
        m_Owner->udpLog("secondSoul", itemAct, "", "", "", "", "act", num);
    }

    void Package::qixiUdpLog(UInt32 id, UInt32 num)
    {
        char itemAct[32] = "";

        snprintf (itemAct, 32, "F_1087_%d", id);
        m_Owner->udpLog("qixi", itemAct, "", "", "", "", "act", num);
    }

    void Package::gemMergeUdpLog(UInt32 num)
    {
        m_Owner->udpLog("gemMerge", "F_1078", "", "", "", "", "act", num);
    }

    void Package::multiMergeUdpLog(UInt32 num)
    {
        m_Owner->udpLog("multiMerge", "F_1080", "", "", "", "", "act", num);
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

    UInt8 Package::FCMerge(UInt32 id, UInt8 bind, UInt32 Mnum)
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
        if(Mnum <= 0){ //兼容老的合成协议
            Mnum = 1;
            bind = 2;
        }

        std::vector<stMergeStf> stfs = GObjectManager::getMergeStfs(id);
        if(stfs.size()  == 0 )
            return 0;

        UInt32 itemId = stfs[0].m_to;
        ItemBase * item = FindItem(itemId, true);
        if( !item )
            item = FindItem(itemId, false);
        if( !item ){
            const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[itemId];
            if(itemType == NULL) return 0;
            item = new(std::nothrow) ItemBase(itemId, itemType);
            if(item == NULL) return 0;
        }
        if( IsEquip(item->getClass()) ){ //合成的是法宝，叠加数为1
            if(Mnum > GetRestPackageSize())
                return 2;
        }
        else{
            if(item->Size(item->Count() + Mnum) - item->Size() + 1 > GetRestPackageSize())
                return 2;
        }

        for(UInt32 i = 0 ; i < stfs[0].m_stfs.size() ; i ++)
        {
            UInt32 id = stfs[0].m_stfs[i].id;
            UInt32 num = stfs[0].m_stfs[i].num;
            if(2 == bind){ //优先使用绑定材料
                if( GetItemAnyNum(id) < num * Mnum)
                    return 0;
            }
            else if(1 == bind){ //只使用绑定材料
                if( GetItemNum(id, true) < num * Mnum)
                   return 0;
            }
            else if(0 == bind){ //只使用不绑定材料
                if( GetItemNum(id, false) < num * Mnum)
                   return 0;
            }
            else
                return 0;
        }
        for(UInt32 j = 0; j < Mnum; j ++){
            bool b = false;
            for(UInt32 i = 0 ; i < stfs[0].m_stfs.size() ; i ++)
            {
                UInt32 id = stfs[0].m_stfs[i].id;
                UInt32 num = stfs[0].m_stfs[i].num;
                if(2 == bind){ //优先使用绑定材料
                    bool hasBind = true;
                    DelItemAny(id, num, &hasBind);
                    if (hasBind)
                        b = true;
                }
                else if(1 == bind){ //只使用绑定材料
                    DelItem(id, num, true);
                    b = true;
                }
                else if(0 == bind){ //只使用不绑定材料
                    DelItem(id, num, false);
                    b = false;
                }
            }
            Add(itemId, 1, b, true, FromFCMerge);
	    }
        Stream st(REP::PACK_USE);  //合成成功，供客户端更新数据
		st << id << static_cast<UInt8>(4) << static_cast<UInt8>(1) << Stream::eos;
		m_Owner->send(st);
        ItemNotify(itemId, Mnum);
        return 1;
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

        UInt8 itemClass = equip->getClass();
        UInt8 q = equip->getQuality();
		if(itemClass >= Item_Weapon && itemClass <= Item_Ring && q == 5)
        {
            ItemEquipSpiritAttr& esa = equip->getEquipSpiritAttr();
            esa.appendAttrToStream(st);
        }
        else if(equip->getClass() == Item_Trump || equip->getClass() == Item_Fashion || equip->getClass() == Item_Halo)
        {
            st << ied.maxTRank << ied.trumpExp;
        }
        else if(equip->getClass() == Item_LBling || equip->getClass() == Item_LBwu || equip->getClass() == Item_LBxin)
        {
            ItemLingbaoAttr& lba = (static_cast<ItemLingbao*>(equip))->getLingbaoAttr();
            lba.appendAttrToStream(st);
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

    void writeEnchLog(UInt64 playerid, UInt8 type, UInt8 enchant)
    {
        DB4().PushUpdateData("REPLACE INTO `enchlog` (`playerId`, `type`, `enchant`) VALUES (%"I64_FMT"u, %u, %u)", playerid, type, enchant);
    }

    void enchantToken(Player* player, UInt8 quality, UInt8 slevel, UInt8 level, UInt8 type)
    {
        bool writedb = false;
        Player* pl = player;
        // 强化送代币活动
        if (quality == 0) // 防具
        {
            //if (type)
            {
                if (slevel < 6 && level >= 6)
                {
                    player->AddVar(VAR_COIN_TOKEN, 1);
                    writedb = true;
                    pl->udpLog("huodong", "F_10000_4", "", "", "", "", "act");
                }
                if (slevel < 8 && level >= 8)
                {
                    player->AddVar(VAR_TAEL_TOKEN, 2);
                    writedb = true;
                    pl->udpLog("huodong", "F_10000_5", "", "", "", "", "act");
                }
                if (slevel < 10 && level >= 10)
                {
                    player->AddVar(VAR_GOLD_TOKEN, 3);
                    writedb = true;
                    pl->udpLog("huodong", "F_10000_6", "", "", "", "", "act");
                }
            }
            /*else
            {
                if (slevel < 6 && level >= 6)
                {
                    player->AddVar(VAR_COIN_TOKEN, 1);
                    writedb = true;
                }
                if (slevel < 8 && level >= 8)
                {
                    player->AddVar(VAR_TAEL_TOKEN, 1);
                    writedb = true;
                }
                if (slevel < 10 && level >= 10)
                {
                    player->AddVar(VAR_GOLD_TOKEN, 1);
                    writedb = true;
                }
            }
            */
        }
        else if (quality == 1) // 武器
        {
            //if (type)
            {
                if (slevel < 6 && level >= 6)
                {
                    player->AddVar(VAR_COIN_TOKEN, 1);
                    writedb = true;
                    pl->udpLog("huodong", "F_10000_1", "", "", "", "", "act");
                }
                if (slevel < 8 && level >= 8)
                {
                    player->AddVar(VAR_GOLD_TOKEN, 2);
                    writedb = true;
                    pl->udpLog("huodong", "F_10000_2", "", "", "", "", "act");
                }
                if (slevel < 10 && level >= 10)
                {
                    player->AddVar(VAR_GOLD_TOKEN, 5);
                    writedb = true;
                    pl->udpLog("huodong", "F_10000_3", "", "", "", "", "act");
                }
            }
           /* else
            {
                if (slevel < 6 && level >= 6)
                {
                    player->AddVar(VAR_COIN_TOKEN, 2);
                    writedb = true;
                }
                if (slevel < 8 && level >= 8)
                {
                    player->AddVar(VAR_GOLD_TOKEN, 2);
                    writedb = true;
                }
                if (slevel < 10 && level >= 10)
                {
                    player->AddVar(VAR_GOLD_TOKEN, 3);
                    writedb = true;
                }
            }*/
        }
        else // 法宝
        {
            if (quality == 3)
            {
            //    if (type)
                {
                    if (slevel < 4 && level >= 4)
                    {
                        player->AddVar(VAR_COIN_TOKEN, 1);
                        writedb = true;
                        pl->udpLog("huodong", "F_10000_7", "", "", "", "", "act");
                    }
                    if (slevel < 6 && level >= 6)
                    {
                        player->AddVar(VAR_TAEL_TOKEN, 2);
                        writedb = true;
                        pl->udpLog("huodong", "F_10000_8", "", "", "", "", "act");
                    }
                    if (slevel < 8 && level == 8)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 2);
                        writedb = true;
                        pl->udpLog("huodong", "F_10000_9", "", "", "", "", "act");
                    }
                }
        /*        else
                {
                    if (slevel < 4 && level >= 4)
                    {
                        player->AddVar(VAR_COIN_TOKEN, 1);
                        writedb = true;
                    }
                    if (slevel < 6 && level >= 6)
                    {
                        player->AddVar(VAR_TAEL_TOKEN, 2);
                        writedb = true;
                    }
                    if (slevel < 8 && level >= 8)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 2);
                        writedb = true;
                    }
                }
                */
            }
            if (quality == 4)
            {
               // if (type)
                {
                    if (slevel < 4 && level >= 4)
                    {
                        player->AddVar(VAR_COIN_TOKEN, 1);
                        writedb = true;
                        pl->udpLog("huodong", "F_10000_7", "", "", "", "", "act");
                    }
                    if (slevel < 6 && level >= 6)
                    {
                        player->AddVar(VAR_TAEL_TOKEN, 2);
                        writedb = true;
                        pl->udpLog("huodong", "F_10000_8", "", "", "", "", "act");
                    }
                    if (slevel < 8 && level == 8)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 3);
                        writedb = true;
                        pl->udpLog("huodong", "F_10000_9", "", "", "", "", "act");
                    }
                }
               /* else
                {
                    if (slevel < 4 && level >= 4)
                    {
                        player->AddVar(VAR_COIN_TOKEN, 1);
                        writedb = true;
                    }
                    if (slevel < 6 && level >= 6)
                    {
                        player->AddVar(VAR_TAEL_TOKEN, 2);
                        writedb = true;
                    }
                    if (slevel < 8 && level >= 8)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 3);
                        writedb = true;
                    }
                }
                */
            }
            if (quality == 5)
            {
            //    if (type)
                {
                    if (slevel < 4 && level >= 4)
                    {
                        player->AddVar(VAR_COIN_TOKEN, 1);
                        writedb  = true;
                        pl->udpLog("huodong", "F_10000_7", "", "", "", "", "act");
                    }
                    if (slevel < 6 && level >= 6)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 2);
                        writedb  = true;
                        pl->udpLog("huodong", "F_10000_8", "", "", "", "", "act");
                    }
                    if (slevel < 8 && level >= 8)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 4);
                        writedb  = true;
                        pl->udpLog("huodong", "F_10000_9", "", "", "", "", "act");
                    }
                }
              /*  else
                {
                    if (slevel < 4 && level >= 4)
                    {
                        player->AddVar(VAR_COIN_TOKEN, 1);
                        writedb  = true;
                    }
                    if (slevel < 6 && level >= 6)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 2);
                        writedb  = true;
                    }
                    if (slevel < 8 && level >= 8)
                    {
                        player->AddVar(VAR_GOLD_TOKEN, 4);
                        writedb  = true;
                    }
                }
                */   
            }
        }

        if (writedb)
            writeEnchLog(player->getId(), quality, level);
        player->sendTokenInfo();
    }

#ifdef _FB
    void enchantAct(Player* player, UInt8 quality, UInt8 slevel, UInt8 level, UInt8 type)
    {
        if (type == 0)
        {
            if (slevel < 4 && level >= 4)
                GameAction()->onEnchantAct(player, 4);
            if (slevel < 6 && level >= 6)
                GameAction()->onEnchantAct(player, 6);
            if (slevel < 8 && level >= 8)
                GameAction()->onEnchantAct(player, 8);
            if (slevel < 10 && level >= 10)
                GameAction()->onEnchantAct(player, 10);
        }
        else
        {
            if (level == 4)
                GameAction()->onEnchantAct(player, level);
            if (level == 6)
                GameAction()->onEnchantAct(player, level);
            if (level == 8)
                GameAction()->onEnchantAct(player, level);
            if (level == 10)
                GameAction()->onEnchantAct(player, level);
        }
    }
#else
    void enchantAct(Player* player, UInt8 quality, UInt8 slevel, UInt8 level, UInt8 type)
    {
        if (quality == 0) // 防具
        {
            for (UInt8 l = level; l >= 6 && l > slevel; --l)
                GameAction()->onEnchantAct(player, l, quality, 1);
        }
        else if (quality == 1) // 武器
        {
            for (UInt8 l = level; l >= 6 && l > slevel; --l)
                GameAction()->onEnchantAct(player, l, quality, 0);
        }
        else
        {
            for (UInt8 l = level; l >= 6 && l > slevel; --l)
                GameAction()->onEnchantAct(player, l, quality);
        }
    }
#endif
    void enchantGt11(Player* player, UInt16 id, UInt8 quality, UInt8 slevel, UInt8 level, UInt8 type, UInt8 _class)
    {
        if (type == 0)
        {
            for (UInt8 l = level; l >= 11 && l > slevel; --l)
                GameAction()->onEnchantGt11(player, id, l, _class);
        }
        else
        {
            if ((slevel < 11 && level >= 11) || (slevel < 12 && level >= 12))
                GameAction()->onEnchantGt11(player, id, level, _class);
        }
    }

    UInt8 Package::Enchant( UInt16 fighterId, UInt32 itemId, UInt8 type, UInt16 count, UInt8 level, UInt16& success, UInt16& failed/*, bool protect*/ )
	{
		if (type > 1) return 2;
		Fighter * fgt = NULL;
		UInt8 pos = 0;
        UInt32 failThisTime = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL/* || equip->getClass() == Item_Ring || equip->getClass() == Item_Amulet*/)
			return 2;

        if(equip->getClass() == Item_Fashion)
            return 2;

        const GData::ItemBaseType& itemType =  equip-> GetItemType();
        if(itemType.getId() == 1525 || itemType.getId() == 1526)
            return 2;

        UInt32 item_enchant_l = ITEM_ENCHANT_L1;
        UInt8 quality = 0;
        UInt8 maxEnchantLevel = ENCHANT_LEVEL_MAX;
        if (m_Owner->getVipLevel() >= 11 || World::getEnchantGt11()) // XXX: ==VIP11 -> 11
            ++maxEnchantLevel;
        if (m_Owner->getVipLevel() >= 12 || World::getEnchantGt11()) // XXX: >=VIP12 -> 12
            ++maxEnchantLevel;

        if(equip->getClass() == Item_Weapon)
        {
            quality = 1;
        }
        else if(equip->getClass() == Item_Trump || equip->getClass() == Item_Halo)
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
        if(equip->getClass() == Item_Trump || equip->getClass() == Item_Halo)
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
        bool autoEnch = false;
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
                if ((equip->getClass() == Item_Trump || equip->getClass() == Item_Halo) && ied.enchant == 1)
                {
                    ((ItemTrump*)equip)->fixSkills();
                    if (fgt)
                    {
                        GData::AttrExtra* attr = const_cast<GData::AttrExtra*>(equip->getAttrExtra());
                        if (attr)
                            fgt->addSkillsFromCT(attr->skills, true);
                    }
                }
                enchantUdpLog(equip, ied.enchant);
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
                    autoEnch = true;
                    ++success;
                    flag_suc = true;
                    ++ ied.enchant;

                    if(ied.enchant > 3)
                    {
                        updateHft = true;
                        hf->setHftValue(hft, 0);
                    }
                    if ((equip->getClass() == Item_Trump || equip->getClass() == Item_Halo) && ied.enchant == 1)
                    {
                        ((ItemTrump*)equip)->fixSkills();
                    }
                    enchantUdpLog(equip, ied.enchant);

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

        if(equip->getClass() == Item_Trump || equip->getClass() == Item_Halo)
            GameAction()->doStrong(this->m_Owner, SthTrumpEnchant, 0, 0);
        else
            GameAction()->doStrong(this->m_Owner, SthEnchant, 0, 0);
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

            if(equip->getClass() == Item_Trump || equip->getClass() == Item_Halo)
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

                 if (ied.enchant >= 1)
                     m_Owner->OnHeroMemo(MC_FORGE, MD_LEGEND, 0, 0);
                 if (ied.enchant >= 2)
                 {
                     m_Owner->OnHeroMemo(MC_FORGE, MD_LEGEND, 0, 1);
                     UInt32 thisDay = TimeUtil::SharpDay();
                     UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(m_Owner, created));
                     if(thisDay <= endDay)
                     {
                         UInt32 targetVal = m_Owner->GetVar(VAR_CLAWARD2);
                         if (!(targetVal & TARGET_TRUMP_UPGRADE))
                         {
                             targetVal |=TARGET_TRUMP_UPGRADE;
                             m_Owner->AddVar(VAR_CTS_TARGET_COUNT, 1);
                             m_Owner->SetVar(VAR_CLAWARD2, targetVal);
                             m_Owner->sendNewRC7DayTarget();
                             m_Owner->newRC7DayUdpLog(1152, 10);
                         }
                     }
                 }
 
                 if (ied.enchant >= 4)
                     m_Owner->OnHeroMemo(MC_FORGE, MD_LEGEND, 0, 2);
            }
            else
            {
                //装备强化
                GameAction()->doAttainment(this->m_Owner, 10164, ied.enchant);

                 if(fgt)
                     fgt->CheckEquipEnchantAttainment(ied.enchant);

                 if (ied.enchant >= 2)
                     m_Owner->OnHeroMemo(MC_FORGE, MD_STARTED, 0, 0);
                 if (ied.enchant >= 4)
                 {
                     m_Owner->OnHeroMemo(MC_FORGE, MD_STARTED, 0, 1);
                     UInt32 thisDay = TimeUtil::SharpDay();
                     UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(m_Owner, created));
                     if(thisDay <= endDay)
                     {
                         UInt32 targetVal = m_Owner->GetVar(VAR_CLAWARD2);
                         if (!(targetVal & TARGET_ENHANCE))
                         {
                             targetVal |=TARGET_ENHANCE;
                             m_Owner->AddVar(VAR_CTS_TARGET_COUNT, 1);
                             m_Owner->SetVar(VAR_CLAWARD2, targetVal);
                             m_Owner->sendNewRC7DayTarget();
                             m_Owner->newRC7DayUdpLog(1152, 9);
                         }
                     }
                 }
                 if (ied.enchant >= 6)
                     m_Owner->OnHeroMemo(MC_FORGE, MD_STARTED, 0, 2);
            }

			if(fgt != NULL)
			{
				fgt->setDirty();

                if(equip->getClass() == Item_Halo)
                    fgt->sendModification(0x1f, equip, false);
                else if(equip->getClass() == Item_Trump)
                    fgt->sendModification(0x0a+ pos, equip, false);
                else
                    fgt->sendModification(0x20 + pos, equip, false);
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
                    if (World::_halloween && ied.enchant == 8 &&
                            equip->GetItemType().subClass != Item_Trump &&
                            equip->GetItemType().subClass != Item_Halo)
                    {
                        if (!m_Owner->enchanted8(equip->getId()))
                            m_Owner->sendEnchanted8Box();
                    }
                    SYSMSG_BROADCASTV(275, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId(), ied.enchant);
                }
				break;
			}

            if (World::getTrumpEnchRet())
            {
                enchantAct(m_Owner, quality, oldEnchant, ied.enchant, autoEnch?0:1);
                // enchantToken(m_Owner, quality, oldEnchant, ied.enchant, autoEnch?0:1);
            }
            if (World::get9215Act())
                    enchantToken(m_Owner, quality, oldEnchant, ied.enchant, autoEnch?0:1);
#ifdef _FB
            if (World::getEnchantAct() && (equip->getClass() == Item_Weapon || equip->getClass() == Item_Armor1 || equip->getClass() == Item_Armor2 || equip->getClass() == Item_Armor4 || equip->getClass() == Item_Armor5))
                enchantAct(m_Owner, quality, oldEnchant, ied.enchant, autoEnch?0:1);
#else
            //UInt8 platform = atoi(m_Owner->getDomain());
            if (World::getEnchantAct()/* && platform == 10*/)
                enchantAct(m_Owner, quality, oldEnchant, ied.enchant, autoEnch?0:1);
#endif

            if (ied.enchant >= 11 && World::getEnchantGt11() && (IsWeapon(equip->getClass()) || IsArmor(equip->getClass())))
            {
                UInt8 type = IsWeapon(equip->getClass())?1:2;
                enchantGt11(m_Owner, equip->GetItemType().getId(), quality, oldEnchant, ied.enchant, autoEnch?0:1, type);
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

            if(equip->getClass() == Item_Trump || equip->getClass() == Item_Halo)
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
                    fgt->sendModification(0x0a+ pos, equip, false);
                else if(equip->getClass() == Item_Halo)
                    fgt->sendModification(0x1f, equip, false);
                else
                    fgt->sendModification(0x20 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
		}

		return 1;
	}
    void Package::enchantUdpLog(ItemEquip * equip, UInt8 level)
    {
        static const int logId[] = {1120, 1121,1122,1123,1124,1125};
        char udpStr[64] = {0};
        if (equip->getClass() == Item_Trump || equip->getClass() == Item_Halo) //法宝
        {
            const GData::ItemBaseType& itemType =  equip-> GetItemType();
            sprintf(udpStr, "F_1126_%d_%d", itemType.getId(), level);
        }
        else
        {
            //非橙色装备
            if (equip->getQuality() < 5)
                return;
            int udpId = 0;
            switch (equip->getReqLev())
            {
            case 45:
                udpId = logId[0];
                break;
            case 60:
                udpId = logId[1];
                break;
            case 70:
                udpId = logId[2];
                break;
            case 80:
                udpId = logId[3];
                break;
            case 90:
                udpId = logId[4];
                break;
            case 100:
                udpId = logId[5];
                break;
            }
            if (udpId == 0)
                return;
            sprintf(udpStr, "F_%d_%d", udpId, level);
        }
        m_Owner->udpLog("enchant", udpStr, "", "", "", "", "act");
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
			fgt->sendModification(0x20 + pos, equip, false);
		else
			SendSingleEquipData(equip);

        if (ied.sockets >= 3)
            m_Owner->OnHeroMemo(MC_FORGE, MD_MASTER, 0, 2);
        else if (ied.sockets >= 2)
            m_Owner->OnHeroMemo(MC_FORGE, MD_MASTER, 0, 1);
        else if (ied.sockets >= 1)
            m_Owner->OnHeroMemo(MC_FORGE, MD_MASTER, 0, 0);
		return 0;
	}

	UInt8 Package::MergeGem( UInt32 gemId, UInt8 bindCount, bool protect, UInt32& ogid )
	{
		if (GetItemSubClass(gemId) != Item_Gem) return 2;
		UInt32 lvl;

        if(IsGemId2(gemId))
            lvl = (gemId - 1) % 10 + 10;
        else
            lvl = (gemId - 1) % 10;
		if(lvl >= 11) return 2;

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
			if(9 == lvl)
                ogid = gemId + 491;
            else
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

        GData::ItemGemType * igt = NULL;
        igt = GData::gemTypes[gemId - LGEM_ID];
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

        m_Owner->OnHeroMemo(MC_FORGE, MD_MASTER, 1, 0);
        if (gemLev >= 3)
            m_Owner->OnHeroMemo(MC_FORGE, MD_MASTER, 1, 2);

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
			fgt->sendModification(0x20 + pos, equip, false);
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
             AddItemHistoriesLog(ITEM_DETACH_PROTECT, 1);
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
			fgt->sendModification(0x20 + pos2, equip, false);
		}
		else
			SendSingleEquipData(equip);

        m_Owner->OnHeroMemo(MC_FORGE, MD_MASTER, 1, 1);
		return 0;
	}

    static void pushBackSplitItem( Player* player, std::vector<SplitItemOut>& splitOut, UInt16 itemId, UInt16 count )
    {
        if (itemId == 502)
            player->OnHeroMemo(MC_FORGE, MD_STARTED, 1, 0);
        if (itemId == 518)
            player->OnHeroMemo(MC_FORGE, MD_STARTED, 1, 1);
        if (itemId == 503)
            player->OnHeroMemo(MC_FORGE, MD_STARTED, 1, 2);

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
        if(itemId == 9359)
            return SplitItem(itemId, splitOut, silence);
        UInt8 res = 1;
		if (!IsEquipId(itemId)) return 2;
		ItemBase * item = FindItem(itemId);
		if(item == NULL || item->getQuality() < 2 || item->getReqLev() < 1)
			return 2;

        bool spirit = false;
        UInt8 itemTypeNumMayOut = 2;
        ItemEquip* equip = static_cast<ItemEquip*>(item);
        if (equip->isSpirited())
        {
            spirit = true;
            itemTypeNumMayOut = 6;
        }

		if((m_Size + itemTypeNumMayOut) > (m_Owner->getPacksize() + 50))
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
        GameAction()->doStrong(this->m_Owner, SthSplit, 0,0);
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
            if(t.subClass == Item_Trump || t.subClass == Item_Fashion || t.subClass == Item_Halo)
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

        if(spirit)
        {
            ItemEquip* equip = static_cast<ItemEquip*>(item);
            if (equip->isSpirited())
            {
                UInt8 itemClass = equip->getClass();
                ItemEquipSpiritAttr& esa = equip->getEquipSpiritAttr();
                for (UInt8 i = 0; i < 4; ++i)
                {
                    if (!esa.spLev[i])
                        continue;
                    UInt8 lev = (esa.spLev[i]-1)/5 + 1;
                    UInt32 tmpId = baseSpiritItem + spiritType[i][itemClass - 1] * 100 + lev;
                    UInt32 count = (esa.spLev[i]%5)?((esa.spLev[i]%5)+1):6;
                    if (tmpId && tmpId >= LSOUL_ID && tmpId <= RSOUL_ID)
                    {
                        AddItem(tmpId, count, isBound, silence, FromSplit);
                        pushBackSplitItem( m_Owner, splitOut, tmpId, count );
                    }
                }
            }
        }

        if(itemOutId != 0 && count != 0)
        {
            if(item != NULL)
                DelEquip2(static_cast<ItemEquip *>(item), ToSplit);
            AddItem(itemOutId, count, isBound, silence, FromSplit);

            item = NULL;
            pushBackSplitItem( m_Owner, splitOut, itemOutId, count );
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

            pushBackSplitItem( m_Owner, splitOut, itemOutId, count );
            res = 0;
		}

        if(item != NULL)
            DelEquip2(static_cast<ItemEquip *>(item), ToSplit);

        res &= (spirit?0:1);
		return res;
	}

	UInt8 Package::SplitItem( UInt32 itemId, std::vector<SplitItemOut>& splitOut, /*bool protect,*/ bool silence )
	{
        UInt8 res = 1;
		ItemBase * item = FindItem(itemId, true);
        if(item == NULL)
            item = FindItem(itemId, false);
		if(item == NULL || item->getQuality() < 2 || item->getReqLev() < 1)
			return 2;

        UInt8 itemTypeNumMayOut = 2;

		if((m_Size + itemTypeNumMayOut) > (m_Owner->getPacksize() + 50))
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
        GameAction()->doStrong(this->m_Owner, SthSplit, 0,0);
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

        if(itemOutId != 0 && count != 0)
        {
            if(item != NULL)
                DelItem2(item, 1, ToSplit);
            AddItem(itemOutId, count, isBound, silence, FromSplit);

            item = NULL;
            pushBackSplitItem( m_Owner, splitOut, itemOutId, count );
            res = 0;
        }

		if(got)
		{
			UInt32 itemOutId = ITEM_ENCHANT_L1 + got - 1;
            UInt32 count = 1;
            if(item != NULL)
                DelItem2(item, 1, ToSplit);

            item = NULL;
			AddItem(itemOutId, count, isBound, silence, FromSplit);

            pushBackSplitItem( m_Owner, splitOut, itemOutId, count );
            res = 0;
		}

        if(item != NULL)
            DelItem2(item, 1, ToSplit);

        res &= 1;
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
        UInt32 lvl;
        if(IsGemId2(gemId))
            lvl = (gemId - 1) % 10 + 10;
        else
            lvl = (gemId - 1) % 10;
        if(lvl >= 11)
            return 3;

        if(bindCount > 0 && GetItemNum(gemId, true) < bindCount)
            return 3;
        if(unbindCount > 0 && GetItemNum(gemId, false) < unbindCount)
            return 3;

        UInt32 bindUsed = 0, unbindUsed = 0;
        unbindGemsOut = 0;
        bindGemsOut = 0;
        if(9 == lvl)
            gemIdOut = gemId + 491;
        else
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
            bool useBind = false;
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
                    useBind = true;
                }
                else
                    ++ protectUnbindUsed;
            }
            if(rnd(1000) < GObjectManager::getMergeChance(lvl))
            {
                unbindUsed += 3;
                unbindCount -= 3;
                if (useBind)
                    ++ bindGemsOut;
                else
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
            gemMergeUdpLog(bindGemsOut);
            if(World::getGemMergeAct())
                GameAction()->onMergeGem(m_Owner, lvl + 2, bindGemsOut);
        }
        if(unbindGemsOut > 0)
        {
            AddItem(gemIdOut, unbindGemsOut, false, false, FromMerge);
            gemMergeUdpLog(unbindGemsOut);
            if(World::getGemMergeAct())
                GameAction()->onMergeGem(m_Owner, lvl + 2, unbindGemsOut);
        }

		ConsumeInfo ci(MergeGems,0,0);
		m_Owner->useTael(coinAmount, &ci);
		if(protectBindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectBindUsed, true, ToGemMgerge);
		if(protectUnbindUsed > 0)
			DelItem(ITEM_GEM_PROTECT, protectUnbindUsed, false, ToGemMgerge);

        GameAction()->doStrong(this->m_Owner, SthMergeGem, 0, 0);
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

#if 0
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
				fgt->sendModification(0x20 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
			return 0;
		}
		++ _lastActivateCount;
		return 1;
	}
#endif
	UInt8 Package::ActivateAttr( UInt16 fighterId, UInt32 itemId )
	{
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
		if(equip == NULL) return 1;

		UInt8 q = equip->getQuality();
		ItemEquipData& ied = equip->getItemEquipData();
        UInt8 c = ied.extraAttr2.getCount();
		if(c == 0 || c > 2) return 1;

        if( (ied.tRank == 0) &&
                (equip->GetItemType().subClass == Item_Trump ||
                equip->GetItemType().subClass == Item_Fashion ||
                equip->GetItemType().subClass == Item_Halo))
        {
            return 1;
        }

		UInt8 lv = (equip->getReqLev() + 5) / 10 - 1;
		q -= 3;
		UInt32 amount = GData::moneyNeed[GData::ATTR_ACTIVATE].tael;
		if(m_Owner->getTael() < amount)
		{
			m_Owner->sendMsgCode(0, 1100);
			return 1;
		}

		bool isBound = equip->GetBindStatus();
		if(!DelItemAny(ITEM_ACTIVATE_ATTR, 1, &isBound)) return 1;
        DelItemSendMsg(ITEM_ACTIVATE_ATTR, m_Owner);

		m_Owner->useTael(amount);
		{
			UInt8 protect = 1 | c;
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
            if(equip->GetItemType().subClass == Item_Trump ||
                    equip->GetItemType().subClass == Item_Fashion ||
                    equip->GetItemType().subClass == Item_Halo)
            {
                equip_t = EQUIPTYPE_TRUMP;
                lv = ied.tRank;
            }

            ++ c;
			getRandomAttr2(lv, crr, q, c, protect, types, values, equip_t);
			if(!equip->GetBindStatus() && isBound)
				equip->DoEquipBind();
			ApplyAttr2(equip, types, values);
			if(fgt != NULL)
			{
				fgt->setDirty();
                if(equip->getClass() == Item_Trump)
                    fgt->sendModification(0x0a + pos, equip, false);
                else if(equip->getClass() == Item_Halo)
                    fgt->sendModification(0x1f, equip, false);
                else
                    fgt->sendModification(0x20 + pos, equip, false);
			}
			else
				SendSingleEquipData(equip);
			return 0;
		}
		return 1;
	}


    const GData::ItemBaseType*  Package::CheckBeforeEquipUpgrade(UInt32 typeId)
    {
        const GData::ItemBaseType * itype = GData::itemBaseTypeManager[typeId];
        if(itype == NULL)
            return NULL;
        if(itype->subClass < Item_Weapon || itype->subClass > Item_Ring )
            return NULL;
        if(m_Size >= m_Owner->getPacksize() + 50)
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
            bool b = bind;
            GData::stUseItem&  itm = it->second.stfs[i];
            DelItemAny(itm.id, itm.num, &b);
            if(b && !bind)
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

        UInt32 oldEquipClass = oldEquip->getClass();
        UInt32 oldEquipId = oldEquip->getId();

        if(false == DelEquip2(oldEquip, ToEquipUpgrade))
            return 2;

        ItemEquip*  pEquip = AddUpgradeEquip(newId, oldEquipId, true, bind, ed , maxv);
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

        if (oldEquipClass == Item_Armor2)
            m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 1, 0);
        if (oldEquipClass == Item_Ring)
            m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 1, 1);
        if (oldEquipClass == Item_Weapon)
            m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 1, 2);

        return 0;
    }

    void Package::setItemBind(UInt32 typeId)
    {
        DB4().PushUpdateData("UPDATE `item` SET `bindType` = 1 WHERE `id` = %u  AND `ownerId` = %"I64_FMT"u", typeId, m_Owner->getId());
    }
    UInt8 Package::EquipMove( UInt16 fFighterId, UInt16 tFighterId,UInt32 fromItemId, UInt32 toItemId, UInt8 type)
    {
        UInt8 res = 0;
        Fighter * fFgt = NULL;
        Fighter * tFgt = NULL;
        UInt8 fPos = 0;
        UInt8 tPos = 0;

        ItemEquip * fromEquip = FindEquip(fFgt, fPos, fFighterId, fromItemId);
        ItemEquip * toEquip = FindEquip(tFgt, tPos, tFighterId, toItemId);
        if (fromEquip == NULL || toEquip == NULL)
            return 1;

        if (fromEquip->getClass() != toEquip->getClass() ||
            fromEquip->getQuality() != toEquip->getQuality())
            return 2;

        if(type & 0x08)
        {
            if(fromEquip->getClass() != Item_Fashion || toEquip->getClass() != Item_Fashion)
                return 12;
        }
        else if (toEquip->GetCareer() != 4)
            return 11;

//        if (m_Owner->GetVar(VAR_EQUIP_MOVE_COUNT) >= 8)
//            return 9;
        res = isCanMove(fromEquip, toEquip, type);
        if (res > 0)
            return res;
        res = moveUseMoney(fromEquip, toEquip, type);
        if (res > 0) 
            return res;

        if (!toEquip->GetBindStatus())
        {
			DB4().PushUpdateData("UPDATE `item` SET `bindType` = 1 WHERE `id` = %u and ownerId=%"I64_FMT"u", toEquip->getId(), m_Owner->getId());
            toEquip->SetBindStatus(true);
        }
        if (type & 1)
            res = moveEquipEnchant(fFgt,tFgt,fromEquip, fPos, toEquip, tPos);
        if (type & 2)
            res = moveEquipGem(fFgt,tFgt,fromEquip, fPos, toEquip, tPos);
        if (type & 4)
            res = moveEquipSpirit(fFgt,tFgt,fromEquip, fPos, toEquip, tPos);
        if (type & 0x08)
            res = moveEquipFashion(fFgt,tFgt,fromEquip, fPos, toEquip, tPos);

        return res;
    }
    UInt8 Package::isCanMove(ItemEquip* fromEquip, ItemEquip* toEquip, UInt8 type)
    {
        ItemEquipData& fIed = fromEquip->getItemEquipData();
        ItemEquipData& tIed = toEquip->getItemEquipData();
        if (type & 1)
        {
            if (fIed.enchant  < 2)
                return 4;
            if (fIed.enchant-1 <= tIed.enchant)
                return 5;
            if ((fromEquip->getClass() < Item_Weapon || fromEquip->getClass() > Item_Ring)) 
                return 6;
        }
        if (type & 2)
        {
            if(GetRestPackageSize() < 6)
            {
                m_Owner->sendMsgCode(0, 1011);
                return 7;
            }
            if (fIed.sockets == 0 )
                return 8;
        }
        if (type & 4)
        {
            //注灵条件限制
           if ((fIed.spiritAttr.spLev[0]+fIed.spiritAttr.spLev[1]+fIed.spiritAttr.spLev[2]+fIed.spiritAttr.spLev[3])==0)
               return 10;
        }
        if(type & 0x08)
        {
        }
        return 0;
    }
    UInt8 Package::moveUseMoney(ItemEquip* fromEquip, ItemEquip* toEquip, UInt8 type)
    {
        static const UInt32 s_money1[] = {10,10,10,10,15,20,50,150,900,2100,4600};
        static const UInt32 s_money2[] = {10,10,10,10,10,10,15,40,340,940,2200};
        
        ItemEquipData& fIed = fromEquip->getItemEquipData();
        ItemEquipData& tIed = toEquip->getItemEquipData();
        UInt32 money = 0;
        if (type & 1 && fIed.enchant >= 2)
        {
            if (fromEquip->getClass() == Item_Weapon)
                money += s_money1[fIed.enchant-2];
            else
                 money += s_money2[fIed.enchant-2];
        }
        if (type & 2)
        {
            for (int i = 1; i <= 6; i++)
            {
                //开孔的仙石
                if (tIed.sockets < i && fIed.sockets >= i)
                {
                    if (i <= 2)
                        money += 5;
                    else if (i <=4 )
                        money += 10;
                    else
                        money += 25;
                }
                //一个宝石15仙石
                if (fIed.gems[i-1] > 0)
                    money += 15;
            }
        }
        if (type & 4)
            money += (fIed.spiritAttr.spLev[0]+fIed.spiritAttr.spLev[1]+fIed.spiritAttr.spLev[2]+fIed.spiritAttr.spLev[3])/1;
        if (type & 0x08)
        {
            UInt32 extramoney;

            money += static_cast<UInt32>(fIed.maxTRank) * 1;
            extramoney = fIed.trumpExp / 1000;
            if(fIed.trumpExp > extramoney * 1000)
                extramoney += 1;
            money += extramoney;

            UInt8 lv = fIed.tRank;
            UInt8 q = fromEquip->getQuality() - 3;
            UInt8 crr = fromEquip->GetCareer();
            UInt8 types;
            float v;
            float values;

            types = fIed.extraAttr2.type1;
            if(types)
            {
                v = GObjectManager::getAttrTrumpMax(lv, types-1, q, crr);
                values = fIed.extraAttr2.value1;
                if(values > v * 90)
                    extramoney = 50;
                else if(values > v * 70)
                    extramoney = 20;
                else if(values > v * 40)
                    extramoney = 10;
                else
                    extramoney = 1;
                money += extramoney;
            }

            types = fIed.extraAttr2.type2;
            if(types)
            {
                v = GObjectManager::getAttrTrumpMax(lv, types-1, q, crr);
                values = fIed.extraAttr2.value2;
                if(values > v * 90)
                    extramoney = 50;
                else if(values > v * 70)
                    extramoney = 20;
                else if(values > v * 40)
                    extramoney = 10;
                else
                    extramoney = 1;
                money += extramoney;
            }

            types = fIed.extraAttr2.type3;
            if(types)
            {
                v = GObjectManager::getAttrTrumpMax(lv, types-1, q, crr);
                values = fIed.extraAttr2.value3;
                if(values > v * 90)
                    extramoney = 50;
                else if(values > v * 70)
                    extramoney = 20;
                else if(values > v * 40)
                    extramoney = 10;
                else
                    extramoney = 1;
                money += extramoney;
            }
        }
       if(m_Owner->getGold() < money && cfg.serverNum != 34)
	    {
            m_Owner->sendMsgCode(0, 1101);
            return 3;
        }
        if (money > 0 && cfg.serverNum != 34)
        {
            ConsumeInfo ci(MoveEquip,0,0);
            m_Owner->useGold(money, &ci);
            m_Owner->AddVar(VAR_EQUIP_MOVE_COUNT,1);
        }
        return 0;
    }
    UInt8 Package::moveEquipEnchant(Fighter* fFgt,Fighter* tFgt, ItemEquip* fromEquip, UInt8 fPos, ItemEquip* toEquip, UInt8 tPos)
    {
        ItemEquipData& fIed = fromEquip->getItemEquipData();
        ItemEquipData& tIed = toEquip->getItemEquipData();
        if ((fromEquip->getClass() >= Item_Weapon && fromEquip->getClass() <= Item_Ring)) 
        {
            tIed.enchant = fIed.enchant;
            fIed.enchant = 0; 
            ((ItemTrump*)toEquip)->fixSkills();
            ((ItemTrump*)fromEquip)->fixSkills();

			DB4().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", fIed.enchant, fromEquip->getId());
			DB4().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", tIed.enchant, toEquip->getId());
            enchantUdpLog(fromEquip, fIed.enchant);
            enchantUdpLog(toEquip, tIed.enchant);

            char str[32] = {0};
            sprintf(str, "F_1155_%03d%02d%03d", fromEquip->getReqLev(), tIed.enchant+1, toEquip->getReqLev());
            m_Owner->udpLog("move", str, "", "", "", "", "act");
        }
        if(fFgt != NULL)
        {
            fFgt->setDirty();
            fFgt->sendModification(0x20 + fPos, fromEquip, false);
        }
        else
            SendSingleEquipData(fromEquip);

        if(tFgt != NULL)
        {
            tFgt->setDirty();
            tFgt->sendModification(0x20 + tPos, toEquip, false);
        }
        else
            SendSingleEquipData(toEquip);
        return 0;
    }

    UInt8 Package::moveEquipGem(Fighter* fFgt,Fighter* tFgt, ItemEquip* fromEquip,UInt8 fPos, ItemEquip* toEquip,UInt8 tPos)
    {
        ItemEquipData& fIed = fromEquip->getItemEquipData();
        ItemEquipData& tIed = toEquip->getItemEquipData();
        //转移宝石
        if (tIed.sockets < fIed.sockets)
        {
            tIed.sockets = fIed.sockets;
            DB4().PushUpdateData("UPDATE `equipment` SET `sockets` = %u WHERE `id` = %u",tIed.sockets,toEquip->getId());
        }
        for (int i = 0; i < fIed.sockets; i++)
        {
            if (tIed.gems[i] > 0)
		        AddItem(tIed.gems[i], 1, true, false, FromDetachGem);
            tIed.gems[i] = fIed.gems[i];
            fIed.gems[i] = 0;
        }

        char str[32] = {0};
        sprintf(str, "F_1156_%03d00%03d", fromEquip->getReqLev(),  toEquip->getReqLev());
        m_Owner->udpLog("move", str, "", "", "", "", "act");
    
        DB4().PushUpdateData("UPDATE `equipment` SET `socket1` = %u,`socket2` = %u,`socket3` = %u,`socket4` = %u,`socket5` = %u,`socket6` = %u WHERE `id` = %u",tIed.gems[0],tIed.gems[1],tIed.gems[2],tIed.gems[3],tIed.gems[4],tIed.gems[5],toEquip->getId());
        DB4().PushUpdateData("UPDATE `equipment` SET `socket1` = %u,`socket2` = %u,`socket3` = %u,`socket4` = %u,`socket5` = %u,`socket6` = %u WHERE `id` = %u",fIed.gems[0],fIed.gems[1],fIed.gems[2],fIed.gems[3],fIed.gems[4],fIed.gems[5],fromEquip->getId());
        if(fFgt != NULL)
        {
            fFgt->setDirty();
            fFgt->sendModification(0x20 + fPos, fromEquip, false);
        }
        else
            SendSingleEquipData(fromEquip);

        if(tFgt != NULL)
        {
            tFgt->setDirty();
            tFgt->sendModification(0x20 + tPos, toEquip, false);
        }
        else
            SendSingleEquipData(toEquip);
        return 0;
    }

    //转移时装法宝属性
    UInt8 Package::moveEquipFashion(Fighter* fFgt,Fighter* tFgt, ItemEquip* fromEquip,UInt8 fPos, ItemEquip* toEquip,UInt8 tPos)
    {
        ItemEquipData& fIed = fromEquip->getItemEquipData();
        ItemEquipData& tIed = toEquip->getItemEquipData();

        char str[32] = {0};
        sprintf(str, "F_1158_%03d00%03d", fromEquip->getReqLev(),  toEquip->getReqLev());
        m_Owner->udpLog("move", str, "", "", "", "", "act");

        tIed.tRank = fIed.tRank;
        tIed.maxTRank = fIed.maxTRank;
        tIed.trumpExp = fIed.trumpExp;
        tIed.extraAttr2.type1 = fIed.extraAttr2.type1;
        tIed.extraAttr2.type2 = fIed.extraAttr2.type2;
        tIed.extraAttr2.type3 = fIed.extraAttr2.type3;
        tIed.extraAttr2.value1 = fIed.extraAttr2.value1;
        tIed.extraAttr2.value2 = fIed.extraAttr2.value2;
        tIed.extraAttr2.value3 = fIed.extraAttr2.value3;
        DB4().PushUpdateData("UPDATE `equipment` SET `tRank` = %u,`maxTRank` = %u,`trumpExp` = %u,`attrType1` = %u,`attrType2` = %u,`attrType3` = %u,`attrValue1` = %u,`attrValue2` = %u,`attrValue3` = %u WHERE `id` = %u", tIed.tRank, tIed.maxTRank, tIed.trumpExp, tIed.extraAttr2.type1, tIed.extraAttr2.type2, tIed.extraAttr2.type3, tIed.extraAttr2.value1, tIed.extraAttr2.value2, tIed.extraAttr2.value3, toEquip->getId());

        fIed.tRank = 0;
        fIed.maxTRank = 1;
        fIed.trumpExp = 0;
        fIed.extraAttr2.type1 = 0;
        fIed.extraAttr2.type2 = 0;
        fIed.extraAttr2.type3 = 0;
        fIed.extraAttr2.value1 = 0;
        fIed.extraAttr2.value2 = 0;
        fIed.extraAttr2.value3 = 0;
        DB4().PushUpdateData("UPDATE `equipment` SET `tRank` = %u,`maxTRank` = %u,`trumpExp` = %u,`attrType1` = %u,`attrType2` = %u,`attrType3` = %u,`attrValue1` = %u,`attrValue2` = %u,`attrValue3` = %u WHERE `id` = %u", fIed.tRank, fIed.maxTRank, fIed.trumpExp, fIed.extraAttr2.type1, fIed.extraAttr2.type2, fIed.extraAttr2.type3, fIed.extraAttr2.value1, fIed.extraAttr2.value2, fIed.extraAttr2.value3, fromEquip->getId());

        if(fFgt != NULL)
        {
            fFgt->setDirty();
            fFgt->sendModification(0x20 + fPos, fromEquip, false);
        }
        else
            SendSingleEquipData(fromEquip);

        if(tFgt != NULL)
        {
            tFgt->setDirty();
            tFgt->sendModification(0x20 + tPos, toEquip, false);
        }
        else
            SendSingleEquipData(toEquip);
        return 0;
    }

    UInt8 Package::moveEquipSpirit(Fighter* fFgt,Fighter* tFgt, ItemEquip* fromEquip,UInt8 fPos, ItemEquip* toEquip,UInt8 tPos)
    {
        ItemEquipData& fIed = fromEquip->getItemEquipData();
        ItemEquipData& tIed = toEquip->getItemEquipData();
        
        //直接内存拷贝
        memcpy(&tIed.spiritAttr, &fIed.spiritAttr, sizeof(tIed.spiritAttr));
        memset(&fIed.spiritAttr, 0, sizeof(fIed.spiritAttr));
        fIed.spiritAttr.spForm[0] = uRand(4) + 1;
        fIed.spiritAttr.spForm[1] = uRand(4) + 1;
        fIed.spiritAttr.spForm[2] = uRand(4) + 1;

        char str[32] = {0};
        sprintf(str, "F_1157_%03d00%03d", fromEquip->getReqLev(),  toEquip->getReqLev());
        m_Owner->udpLog("move", str, "", "", "", "", "act");

        
        DB4().PushUpdateData("UPDATE `equipment_spirit` SET `spLev1` = %u,`spLev2` = %u,`spLev3` = %u,`spLev4` = %u, `spform1` = %u, `spform2` = %u, `spform3` = %u WHERE `id` = %u", tIed.spiritAttr.spLev[0],tIed.spiritAttr.spLev[1],tIed.spiritAttr.spLev[2], tIed.spiritAttr.spLev[3],tIed.spiritAttr.spForm[0], tIed.spiritAttr.spForm[1],tIed.spiritAttr.spForm[2],toEquip->getId());
        DB4().PushUpdateData("UPDATE `equipment_spirit` SET `spLev1` = %u,`spLev2` = %u,`spLev3` = %u,`spLev4` = %u, `spform1` = %u, `spform2` = %u, `spform3` = %u WHERE `id` = %u", fIed.spiritAttr.spLev[0],fIed.spiritAttr.spLev[1],fIed.spiritAttr.spLev[2],fIed.spiritAttr.spLev[3], fIed.spiritAttr.spForm[0], fIed.spiritAttr.spForm[1],fIed.spiritAttr.spForm[2],fromEquip->getId());
        
        if(fFgt != NULL)
        {
            fFgt->setDirty();
            fFgt->sendModification(0x20 + fPos, fromEquip, false);
        }
        else
            SendSingleEquipData(fromEquip);

        if(tFgt != NULL)
        {
            tFgt->setDirty();
            tFgt->sendModification(0x20 + tPos, toEquip, false);
        }
        else
            SendSingleEquipData(toEquip);
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
        if((equip->GetItemType().subClass == Item_Trump ||
                    equip->GetItemType().subClass == Item_Fashion ||
                    equip->GetItemType().subClass == Item_Halo) && ied.tRank < 1)
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
        //
        //装备洗练成就
        GameAction()->doAttainment(this->m_Owner, 10175, 0);
        GameAction()->doStrong(this->m_Owner, SthForge, 0, 0);
        m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 0, 0);
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
        if(equip->GetItemType().subClass == Item_Trump ||
                equip->GetItemType().subClass == Item_Fashion ||
                equip->GetItemType().subClass == Item_Halo)
        {
            equip_t = EQUIPTYPE_TRUMP;
            lv = ied.tRank;
        }

		getRandomAttr2(lv, crr, q, ied.extraAttr2.getCount(), protect, types, values, equip_t);

        UInt8 num = 0;
        float v0 = 0;
        if(equip_t == EQUIPTYPE_EQUIP)
            v0 = GObjectManager::getAttrMax(lv, types[0]-1, q, crr)*90;
        else if(equip_t == EQUIPTYPE_TRUMP)
            v0 = GObjectManager::getAttrTrumpMax(lv, types[0]-1, q, crr)*90;
        if ((float)values[0] > v0 && !(protect & 1))
        {
            SYSMSG_BROADCASTV(2203, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId());
        }
        if ((float)values[0] > v0)
            ++num;

        float v1 = 0;
        if(equip_t == EQUIPTYPE_EQUIP)
            v1 = GObjectManager::getAttrMax(lv, types[1]-1, q, crr)*90;
        else if(equip_t == EQUIPTYPE_TRUMP)
            v1 = GObjectManager::getAttrTrumpMax(lv, types[1]-1, q, crr)*90;
        if ((float)values[1] > v1 && !(protect & 2))
        {
            SYSMSG_BROADCASTV(2203, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId());
        }
        if ((float)values[1] > v1)
            ++num;


        float v2 = 0;
        if(equip_t == EQUIPTYPE_EQUIP)
            v2 = GObjectManager::getAttrMax(lv, types[2]-1, q, crr)*90;
        else if(equip_t == EQUIPTYPE_TRUMP)
            v2 = GObjectManager::getAttrTrumpMax(lv, types[2]-1, q, crr)*90;
        if ((float)values[2] > v2 && !(protect & 4))
        {
            SYSMSG_BROADCASTV(2203, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->GetItemType().getId());
        }
        if ((float)values[2] > v2)
            ++num;

#ifdef _FB
        if (num)
        {
            m_Owner->equipForge(fighterId, itemId, num);
        }
#endif


        {
            float v0 = 0;
            if(equip_t == EQUIPTYPE_EQUIP)
                v0 = GObjectManager::getAttrMax(lv, types[0]-1, q, crr)*40;
            if ((float)values[0] > v0 && !(protect & 1))
                m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 0, 1);
        }
        {
            float v0 = 0;
            if(equip_t == EQUIPTYPE_EQUIP)
                v0 = GObjectManager::getAttrMax(lv, types[1]-1, q, crr)*40;
            if ((float)values[1] > v0 && !(protect & 2))
                m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 0, 1);
        }
        {
            float v0 = 0;
            if(equip_t == EQUIPTYPE_EQUIP)
                v0 = GObjectManager::getAttrMax(lv, types[0]-1, q, crr)*70;
            if ((float)values[0] > v0 && !(protect & 1))
                m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 0, 2);
        }
        {
            float v0 = 0;
            if(equip_t == EQUIPTYPE_EQUIP)
                v0 = GObjectManager::getAttrMax(lv, types[1]-1, q, crr)*70;
            if ((float)values[1] > v0 && !(protect & 2))
                m_Owner->OnHeroMemo(MC_FORGE, MD_ADVANCED, 0, 2);
        }

		ApplyAttr2(equip, types, values);
		if(!equip->GetBindStatus() && isBound)
			equip->DoEquipBind();
		if(fgt != NULL)
		{
			fgt->setDirty();
            if(equip->GetItemType().subClass == Item_Trump)
                fgt->sendModification(0x0a + pos, equip, false);
            else if(equip->GetItemType().subClass == Item_Halo)
                fgt->sendModification(0x1f, equip, false);
            else
                fgt->sendModification(0x20 + pos, equip, false);
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
		if(cur > m_Owner->getPacksize() + 50)
			return false;
		if(!item->IncItem(num))
			return false;
		m_Size = cur;
		return true;
	}

    bool Package::TryBuyEquip(UInt32 typeId, UInt32 num, bool bind /*= false */)
    {
		if((UInt32)(GetRestPackageSize()) < num)
			return false;
        return true;
    }

    bool Package::TryBuyItem(UInt32 typeId, UInt32 num, bool bind /*= false */)
    {      
        if (!typeId || !num) return false;
		if (IsEquipTypeId(typeId)) return false;
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
		if(itemType == NULL) return false;
		ITEM_BIND_CHECK(itemType->bindType,bind);
		ItemBase * item = FindItem(typeId, bind);
        if(item)
        {
            UInt16 cur = m_Size;
            UInt16 oldq = item->Size(), newq = item->Size(item->Count() + num);
            cur = cur - oldq + newq;
            if(cur > m_Owner->getPacksize())
                return false;
        }
        else if(itemType->Size(num) > GetRestPackageSize())
        {
            return false;
        }

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

    bool eachFighter(Fighter* fgt)
    {
        return true;
    }

    void Package::FindEquipByTypeId(std::vector<ItemEquip*>& ret, UInt32 id, bool bind)
    {
        if (!m_Owner)
            return;
        for (item_elem_iter i = m_Items.begin(); i != m_Items.end(); ++i)
        {
            if (i->second && IsEquipId(i->second->getId()))
            {
                ItemEquip* item = (ItemEquip*)i->second;
                if (item->GetItemType().getId() == id)
                    ret.push_back(item);
            }
        }
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

    void Package::ItemNotifyEquip(ItemEquip* equip)
    {
        if(equip->getClass() == Item_LBling || equip->getClass() == Item_LBwu || equip->getClass() == Item_LBxin)
        {
            ItemLingbao* lb = static_cast<ItemLingbao*>(equip);
            ItemLingbaoAttr& lba = lb->getLingbaoAttr();
            std::string name;
            lbnameTmpl.getLBName(name, lb);
			SYSMSG_SENDV(4117, m_Owner, lba.lbColor, name.c_str());
			SYSMSG_SENDV(4118, m_Owner, lba.lbColor, name.c_str());
        }
        else
        {
			SYSMSG_SENDV(102, m_Owner, equip->GetItemType().getId(), 1);
			SYSMSG_SENDV(1002, m_Owner, equip->GetItemType().getId(), 1);
        }
    }

    // get item trump exp and delelte item
    UInt8 Package::SmeltItemTrumpExp(Fighter * fgt, UInt32 itemId, bool& bind, UInt32& exp)
    {
        if(itemId > 30000)
        {
            UInt8 pos = 0;
            ItemEquip * item = FindEquip(fgt, pos, 0, itemId);
            if(item == NULL || (item->getClass() != Item_Trump && item->getClass() != Item_Fashion && item->getClass() != Item_Halo))
                return 2;

            bind = item->GetBindStatus();
            ItemEquipData& ied_item = item->getItemEquipData();

            UInt8 item_q = item->getQuality();
            UInt8 item_enchant = ied_item.enchant;
            const GData::ItemBaseType& ibt = item->GetItemType();
            exp = (ibt.trumpExp * GObjectManager::getTrumpSmelt(item_q - 2, item_enchant)) + ied_item.trumpExp * 0.5;
            if(exp == 0)
                return 2;

            if(!DelEquip(itemId, ToTrumpUpgrade))
                return 2;
        }
        else
        {
            ItemBase* item = GetItem(itemId, bind);
            if(item == NULL || item->getClass() != Item_Normal29)
                return 2;
            const GData::ItemBaseType& ibt = item->GetItemType();
            exp = ibt.trumpExp;
            if(exp == 0)
                return 2;

            if(!DelItem2(item, 1, ToTrumpUpgrade))
                return 2;
        }

        return 0;
    }

    UInt8 Package::TrumpUpgrade(UInt16 fighterId, UInt32 trumpId, UInt32 itemId, UInt8 bind)
    {
		Fighter * fgt = NULL;
		UInt8 pos = 0;
        if(trumpId == itemId)
            return 2;
		ItemEquip * trump = FindEquip(fgt, pos, fighterId, trumpId);
		if(trump == NULL || (trump->getClass() != Item_Trump && trump->getClass() != Item_Fashion && trump->getClass() != Item_Halo))
			return 2;

		ItemEquipData& ied_trump = trump->getItemEquipData();
        UInt8 q = trump->getQuality();
        if(q < 2)
            return 2;
        UInt8 l = ied_trump.tRank;
        UInt32 rankUpExp = GObjectManager::getTrumpExpRank(q-2, l);
        if(l >= ied_trump.maxTRank || rankUpExp == 0)
            return 2;

        UInt32 exp = 0;
        bool isBound = (bind > 0);
        UInt8 res = SmeltItemTrumpExp(fgt, itemId, isBound, exp);
        if(res != 0)
            return res;

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
            if (trump->getClass() == Item_Halo)
                fgt->sendModification(0x1f, trump, false);
            else if (trump->getClass() == Item_Fashion)
                fgt->sendModification(0x20, trump, false);
            else
                fgt->sendModification(0x0a+ pos, trump, false);
        }
		else
			SendSingleEquipData(trump);

        m_Owner->OnHeroMemo(MC_FORGE, MD_LEGEND, 1, 0);
        if (ied_trump.tRank >= 2)
            m_Owner->OnHeroMemo(MC_FORGE, MD_LEGEND, 1, 2);
        else if (ied_trump.tRank >= 1)
            m_Owner->OnHeroMemo(MC_FORGE, MD_LEGEND, 1, 1);

        GameAction()->doStrong(m_Owner, SthTrumpUpgrade, 0, 0);
        return 0;
    }

    UInt8 Package::TrumpLOrder(UInt16 fighterId, UInt32 trumpId)
    {
		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * trump = FindEquip(fgt, pos, fighterId, trumpId);
		if(trump == NULL || (trump->getClass() != Item_Trump && trump->getClass() != Item_Fashion && trump->getClass() != Item_Halo))
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
        {
            if (trump->getClass() == Item_Halo)
                fgt->sendModification(0x1f, trump, false);
            else if (trump->getClass() == Item_Fashion)
                fgt->sendModification(0x20, trump, false);
            else
                fgt->sendModification(0x0a + pos, trump, false);
        }
		else
			SendSingleEquipData(trump);
 
        return 0;
    }

    void Package::AttachSpirit(UInt8 type, UInt16 fighterId, UInt32 itemId)
    {

		Fighter * fgt = NULL;
		UInt8 pos = 0;
		ItemEquip * equip = FindEquip(fgt, pos, fighterId, itemId);
        if(equip == NULL)
            return;
        UInt8 itemClass = equip->getClass();
		if(itemClass > Item_Ring || itemClass < Item_Weapon)
			return;

        UInt8 q = equip->getQuality();
        if(q < 5)
            return;

        bool bind = equip->GetBindStatus();
        ItemEquipData& ied_equip = equip->getItemEquipData();
        switch(type)
        {
        case 0:
            {
                UInt32 itemId[3] = {0};
                UInt8 form0 = ied_equip.spiritAttr.spForm[0];
                UInt16 awardLev = 0;
                if(m_Owner->getTael() < 10)
                    return;

                for(int j = 0; j != 3; ++ j)
                {
                    UInt8 form = ied_equip.spiritAttr.spForm[j];
                    if(form == 0)
                        return;
                    UInt8 lev = (ied_equip.spiritAttr.spLev[form - 1] + 5)/5;
                    if(ied_equip.spiritAttr.spLev[form - 1] == SPIRIT_LEVEL_MAX)
                        lev = SPIRIT_LEVEL_MAX / 5;

                    UInt32 tmpId = baseSpiritItem + spiritType[form - 1][itemClass - 1] * 100 + lev;
                    UInt32 same = 1;
                    for (UInt8 i = 0; i < 3; ++i)
                    {
                        if (itemId[i] == tmpId)
                            ++same;
                    }
                    if(GetItemAnyNum(tmpId) < same)
                        return;
                    itemId[j] = tmpId;
                    if(form0 == form && j != 0)
                        ++ awardLev;
                }
                if(awardLev != 2)
                    awardLev = 0;

                for(int k = 0; k != 3; ++ k)
                {
                    bool tmp = bind;
                    DelItemAny(itemId[k], 1, &tmp);
                    if(tmp && !bind)
                        bind = tmp;
                }

                ConsumeInfo ci(EquipAttachSpirit,0,0);
                m_Owner->useTael(10,&ci);
                int idx = uRand(3);
                UInt8 form = ied_equip.spiritAttr.spForm[idx];
                UInt16 spLev = 1;
                for(int i = idx - 1; i >= 0; -- i)
                {
                    if(ied_equip.spiritAttr.spForm[i] == form)
                        ++ spLev;
                }
                ied_equip.spiritAttr.spLev[form - 1] += spLev + awardLev;
                if(SPIRIT_LEVEL_MAX < ied_equip.spiritAttr.spLev[form - 1])
                    ied_equip.spiritAttr.spLev[form - 1] = SPIRIT_LEVEL_MAX;
                ied_equip.spiritAttr.spForm[0] = uRand(4) + 1;
                ied_equip.spiritAttr.spForm[1] = uRand(4) + 1;
                ied_equip.spiritAttr.spForm[2] = uRand(4) + 1;

                DB4().PushUpdateData("UPDATE `equipment_spirit` SET `spLev%u` = %u, `spform1` = %u, `spform2` = %d, `spform3` = %u WHERE `id` = %u", form, ied_equip.spiritAttr.spLev[form - 1], ied_equip.spiritAttr.spForm[0], ied_equip.spiritAttr.spForm[1], ied_equip.spiritAttr.spForm[2], equip->getId());
            }
            break;
        case 1:
            {
                if(!DelItemAny(SPIRIT_TRANS_ITEM, 1, &bind))
                    return;

                ied_equip.spiritAttr.spForm[0] = uRand(4) + 1;
                ied_equip.spiritAttr.spForm[1] = uRand(4) + 1;
                ied_equip.spiritAttr.spForm[2] = uRand(4) + 1;
                DB4().PushUpdateData("UPDATE `equipment_spirit` SET `spform1` = %u, `spform2` = %d, `spform3` = %u WHERE `id` = %u", ied_equip.spiritAttr.spForm[0], ied_equip.spiritAttr.spForm[1], ied_equip.spiritAttr.spForm[2], equip->getId());
            }
            break;
        default:
            return;
        }

        if(!equip->GetBindStatus() && bind)
            equip->DoEquipBind();
		if(fgt != NULL)
        {
            fgt->setDirty();
			fgt->sendModification(0x20 + pos, equip, false);
        }
		else
			SendSingleEquipData(equip);

		Stream st(REP::EQ_SPIRIT);
		st << type << fighterId << itemId << Stream::eos;
		m_Owner->send(st);
    }

    void Package::GenSpirit(ItemEquip* equip)
    {
        ItemEquipSpiritAttr& esa = equip->getEquipSpiritAttr();
        if(equip->getQuality() == 5 && esa.spForm[0] == 0)
        {
            esa.spForm[0] = GRND(4) + 1;
            esa.spForm[1] = GRND(4) + 1;
            esa.spForm[2] = GRND(4) + 1;
            DB4().PushUpdateData("REPLACE INTO `equipment_spirit` (`id`, `splev1`, `splev2`, `splev3`, `splev4`, `spform1`, `spform2`, `spform3`) values (%u, 0, 0, 0, 0, %u, %u, %u)", equip->getId(), esa.spForm[0], esa.spForm[1], esa.spForm[2]);
        }
    }

    void Package::GenSpirit2(ItemEquip* equip)
    {
        ItemEquipSpiritAttr& esa = equip->getEquipSpiritAttr();
        if (equip->getQuality() == 5 && esa.spForm[0] == 0)
        {
            esa.spForm[0] = GRND(4) + 1;
            esa.spForm[1] = GRND(4) + 1;
            esa.spForm[2] = GRND(4) + 1;
            DB4().PushUpdateData("UPDATE `equipment_spirit` SET `spform1` = %u, `spform2` = %u, `spform3` = %u WHERE `id` = %u", esa.spForm[0], esa.spForm[1], esa.spForm[2], equip->getId());
        }
    }

    void Package::enumerate(Visitor<ItemBase>& visitor)
    {
		item_elem_iter iter = m_Items.begin();
		for (; iter != m_Items.end(); ++iter)
        {
            if(!visitor(iter->second))
                return;
        }
    }

    UInt8 Package::Tongling(UInt32 equipId, UInt8 protect, UInt8 bind, std::vector<UInt16>& values)
    {
#define TONGLING_ITEM         9307
#define TONGLING_ITEM_PROTECT 9308
        ItemBase* item = FindItem(equipId);
        if(!item)
            return 2;
        if(Item_LBling > item->GetItemType().subClass || Item_LBxin < item->GetItemType().subClass)
            return 2;
        UInt16 itemId = 0;
        if(protect)
            itemId = TONGLING_ITEM_PROTECT;
        else
            itemId = TONGLING_ITEM;

		ItemLingbao* equiplb = static_cast<ItemLingbao*>(item);
        item = FindItem(itemId, bind);
        if(!equiplb || !item || equiplb->isTongling())
            return 2;

        DelItem2(item, 1, ToLingbao);
        ItemLingbaoAttr& lba = (static_cast<ItemLingbao*>(equiplb))->getLingbaoAttr();
        lba.tongling = 1;
        float factor = 0;
        float totalFactor = 0;
        UInt8 totalCount = 0;
        std::string strValue;
        for(int i = 0; i < 4; ++ i)
        {
            if(protect)
            {
                factor = ((float)(1060 + uRand(90)))/1000;
            }
            else
            {
                factor = ((float)(950 + uRand(170)))/1000;
            }

            UInt16 oldvalue = lba.value[i];
            lba.value[i] = lba.value[i]*factor + 0.9999f;
            strValue += Itoa(lba.value[i], 10);
            if(i < 3)
                strValue += ',';

            if(oldvalue != 0)
            {
                values.push_back(lba.value[i] - oldvalue);
                ++ totalCount;
                totalFactor += factor - 1.0f;
            }
        }

        float pFactor = totalFactor/totalCount;
        if(pFactor < -0.02f)
        {
            if(protect)
                m_Owner->udpLog("Tongling", "F_10000_10", "", "", "", "", "act");
            else
                m_Owner->udpLog("Tongling", "F_10000_5", "", "", "", "", "act");
        }
        else if(pFactor < 0.06f)
        {
            if(protect)
                m_Owner->udpLog("Tongling", "F_10000_11", "", "", "", "", "act");
            else
                m_Owner->udpLog("Tongling", "F_10000_6", "", "", "", "", "act");
        }
        else if(pFactor < 0.09f)
        {
            if(protect)
                m_Owner->udpLog("Tongling", "F_10000_12", "", "", "", "", "act");
            else
                m_Owner->udpLog("Tongling", "F_10000_7", "", "", "", "", "act");
        }
        else if(pFactor < 0.12f)
        {
            if(protect)
                m_Owner->udpLog("Tongling", "F_10000_13", "", "", "", "", "act");
            else
                m_Owner->udpLog("Tongling", "F_10000_8", "", "", "", "", "act");
        }
        else
        {
            if(protect)
                m_Owner->udpLog("Tongling", "F_10000_14", "", "", "", "", "act");
            else
                m_Owner->udpLog("Tongling", "F_10000_9", "", "", "", "", "act");
        }

		if(!equiplb->GetBindStatus() && bind > 0)
			equiplb->DoEquipBind();
        DB4().PushUpdateData("UPDATE `lingbaoattr` SET `values`='%s', `tongling`=1 WHERE `id`=%u", strValue.c_str(), equipId);
        SendSingleEquipData(equiplb);
        return 0;
    }

    UInt8 Package::OpenLingbaoSmelt(UInt16 gujiId, UInt8 type)
    {
#define FULING_ITEM         9309
#define FULING_ITEM_PROTECT 9310
        UInt16 itemId = 0;
        if(type)
            itemId = FULING_ITEM_PROTECT;
        else
            itemId = FULING_ITEM;
        ItemBase* guji = FindItem(gujiId, true);
        if(!guji)
            guji = FindItem(gujiId, false);
        ItemBase* item = FindItem(itemId, true);
        if(!item)
            item = FindItem(itemId, false);
        if(!guji || !item)
            return 2;
        UInt8 gujiClass = guji->GetItemType().subClass;
        if(gujiClass < Item_Guji || gujiClass > Item_Guji_Dossier)
            return 2;

        UInt8 colorIdx = guji->getQuality() - 2;
        UInt8 lvIdx = (guji->getReqLev() - 70)/10;
        UInt8 gujiIdx = gujiClass - Item_Guji;
        UInt8 itemIdx = item->getQuality() == 2 ? 0 : 1;
        if(colorIdx > 3 || lvIdx > 3 || gujiIdx > 16 || itemIdx > 1)
            return 2;

        DelItem2(guji, 1, ToLingbao);
        DelItem2(item, 1, ToLingbao);
        double gujiFactor[17] = {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 10};
        double itemFactor[2] = {1, 1.5};
        double lvFactor[4] = {1, 1.2, 1.4, 1.6};
        double colorFactor[4] = {1, 1, 2, 3};

        m_Owner->udpLog("Tongling", "F_10000_15", "", "", "", "", "act");
        switch(colorIdx)
        {
        case 0:
            break;
        case 1:
            m_Owner->udpLog("Tongling", "F_10000_16", "", "", "", "", "act");
            break;
        case 2:
            m_Owner->udpLog("Tongling", "F_10000_17", "", "", "", "", "act");
            break;
        case 3:
            m_Owner->udpLog("Tongling", "F_10000_18", "", "", "", "", "act");
            break;
        }

        m_lbSmeltInfo.gujiId = gujiId;
        m_lbSmeltInfo.itemId = itemId;
        m_lbSmeltInfo.bind = 1; //(bind1 != 0 || bind2 != 0) ? 1 : 0;
        m_lbSmeltInfo.value = 0;
        m_lbSmeltInfo.maxValue = 1000*gujiFactor[gujiIdx]*itemFactor[itemIdx]*lvFactor[lvIdx]*colorFactor[colorIdx];

        sendLingbaoSmeltInfo();

        DB4().PushUpdateData("INSERT INTO `lingbaosmelt`(`playerId`, `gujiId`, `itemId`, `bind`, `value`, `maxValue`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u)", m_Owner->getId(), m_lbSmeltInfo.gujiId, m_lbSmeltInfo.itemId, m_lbSmeltInfo.bind, m_lbSmeltInfo.value, m_lbSmeltInfo.maxValue);

        return 0;
    }

    UInt8 Package::LingbaoSmelt(UInt32 itemId, UInt16 cnt)
    {
        if(m_lbSmeltInfo.value >= m_lbSmeltInfo.maxValue)
            return 2;

        UInt16 itemCnt = GetItemAnyNum(itemId);
        if(cnt > itemCnt)
            cnt = itemCnt;

		if (IsEquipId(itemId))
            cnt = 1;
        else if(cnt == 0)
            return 2;

        ItemBase* item = FindItem(itemId, true);
        if(!item)
            item = FindItem(itemId, false);
        if(!item)
            return 2;

        const GData::ItemBaseType& ibt = item->GetItemType();
        UInt16 subClass = ibt.subClass;
        if((Item_LBling > subClass || Item_LBxin < subClass) && Item_Normal27 != subClass )
            return 2;

        UInt8 res = 0;
        UInt16 useCnt = 0;
        for(; useCnt < cnt; ++ useCnt)
        {
            UInt32 value = 0;
            value = ibt.trumpExp;
            if(subClass >= Item_LBling && subClass <= Item_LBxin)
            {
                stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
                ItemLingbaoAttr& lba = (static_cast<ItemLingbao*>(item))->getLingbaoAttr();
                UInt8 lv = item->getReqLev();
                UInt8 skillNum = 0;
                if(lba.skill[0])
                    ++ skillNum;
                if(lba.skill[1])
                    ++ skillNum;
                if(lba.tongling)
                    value += lbAttrConf.getSmeltExp(lv, subClass - Item_LBling, lba.type, lba.value, 4, skillNum);
                else
                    value += lbAttrConf.getSmeltExp2(lv, subClass - Item_LBling, lba.lbColor);
            }
            m_lbSmeltInfo.value += value;

            if(m_lbSmeltInfo.value >= m_lbSmeltInfo.maxValue)
            {
                ++ useCnt;
                break;
            }
        }

		if (IsEquipId(itemId))
            DelEquip(item->getId(), ToLingbao);
        else
            DelItemAny(itemId, useCnt, NULL, ToLingbao);

        DB4().PushUpdateData("UPDATE `lingbaosmelt` SET `value`=%u WHERE `playerId`=%"I64_FMT"u", m_lbSmeltInfo.value, m_Owner->getId());

        return res;
    }

    bool Package::FinishLBSmeltSpecial(const GData::ItemBaseType * itype, ItemLingbaoAttr &lbattr, UInt8& attrNum)
    {
        //黄帝卷宗（11113），炎帝卷宗（11114），神农卷宗（11115），女娲卷宗（11116）
        UInt16 lbids[] = { 11113, 11114, 11115, 11116 };
        bool hasSpe = false;
        for(UInt8 i = 0; i < sizeof(lbids) / sizeof(lbids[0]); ++i)
        {
            if(m_lbSmeltInfo.gujiId == lbids[i])
                hasSpe = true;
        }
        if(!hasSpe)
            return false;
        stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
        std::vector<UInt8> allAttrType = lbAttrConf.attrType;
        attrNum = m_lbSmeltInfo.itemId == FULING_ITEM_PROTECT ? 4 : 3;
        const GData::ItemBaseType* guji = GData::itemBaseTypeManager[m_lbSmeltInfo.gujiId];
        for(int i = 0; i < attrNum; ++ i)
        {
            UInt8 size = allAttrType.size();
            UInt8 idx = uRand(size);
            lbattr.type[i] = allAttrType[idx];
            UInt16 chance = uRand(10000);
            float fChance = ((float)(uRand(10000)))/10000;
            float disFactor = lbAttrConf.getDisFactor4(chance, fChance, 5);
            lbattr.value[i] = lbAttrConf.getAttrMax(guji->reqLev, itype->subClass-Item_LBling, lbattr.type[i]-1) * disFactor + 0.9999f;
            allAttrType.erase(allAttrType.begin() + idx);
        }
        return true;
    }

    void Package::FinishLBSmelt()
    {
        if(m_lbSmeltInfo.value < m_lbSmeltInfo.maxValue)
            return;

        const GData::ItemBaseType* guji = GData::itemBaseTypeManager[m_lbSmeltInfo.gujiId];
        const GData::ItemBaseType* item = GData::itemBaseTypeManager[m_lbSmeltInfo.itemId];
        if(!guji || !item)
            return;

        UInt8 lv = guji->reqLev;
        UInt8 lvIdx = (lv - 70)/10;
        if(lvIdx > 3) lvIdx = 3;

        UInt16 gjIdx = guji->subClass - Item_Guji;
        UInt8 lbIdx[17] = {0xFF, 0, 1, 2, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 0xFF};
        UInt16 lbids[4][3] = {
            {11500, 11501, 11502},
            {11503, 11504, 11505},
            {11506, 11507, 11508},
            {11509, 11510, 11511}};

        UInt8 itemIdx = lbIdx[gjIdx];
        if(lbIdx[gjIdx] == 0xFF)
        {
            itemIdx = uRand(3);
        }
        UInt16 lbid = lbids[lvIdx][itemIdx];

        const GData::ItemBaseType * itype = GData::itemBaseTypeManager[lbid];
        if(!itype)
            return;

        ItemLingbaoAttr lbattr;
        stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
        UInt8 attrNum = lbAttrConf.getAttrNum(uRand(100));
        UInt16 subClass = itype->subClass;
        bool fSpecial = true;
        if(!FinishLBSmeltSpecial(itype, lbattr, attrNum))
        {
            fSpecial = false;
            UInt8 minAttrNum = item->quality > 3 ? 3 : 1;
            UInt8 color2 = item->quality;
            UInt8 color = guji->quality;
            if(attrNum < minAttrNum)
                attrNum = minAttrNum;
            std::vector<UInt8> allAttrType = lbAttrConf.attrType;
            UInt8 itemTypeIdx = subClass - Item_LBling;
            // 古籍指定的属性
            {
                UInt8 idx = 0;

                if(gjIdx == 0)
                {
                    UInt8 size = allAttrType.size();
                    idx = uRand(size);
                }
                else if(gjIdx < 4)
                {
                    UInt8 lbAttrIdx[3][4] =
                    { {0, 1, 6, 11}, // 物功，法功，身法，反击
                      {2, 3, 4, 5},  // 物防，法防，生命, 坚韧
                      {7, 8, 9, 10}};// 命中，闪避，暴击，破击
                    idx = lbAttrIdx[gjIdx - 1][uRand(4)];
                }
                else
                {
                    UInt8 lbAttrIdx[12] = {0, 1, 11, 6, 2, 3, 5, 4, 9, 10, 8, 7};
                    idx = lbAttrIdx[gjIdx - 4];
                }

                lbattr.type[0] = allAttrType[idx];
                UInt16 chance = uRand(10000);
                float fChance = ((float)(uRand(10000)))/10000;
                float disFactor = lbAttrConf.getDisFactor4(chance, fChance, color);
                lbattr.value[0] = lbAttrConf.getAttrMax(lv, itemTypeIdx, lbattr.type[0]-1) * disFactor + 0.9999f;
                allAttrType.erase(allAttrType.begin() + idx);
            }
            for(int i = 1; i < attrNum; ++ i)
            {
                UInt8 size = allAttrType.size();
                UInt8 idx = uRand(size);
                lbattr.type[i] = allAttrType[idx];
                UInt16 chance = uRand(10000);
                float fChance = ((float)(uRand(10000)))/10000;
                float disFactor = lbAttrConf.getDisFactor4(chance, fChance, color2);
                // item color only define one attr to it`s color
                color2 = 2;
                //float disFactor = lbAttrConf.getDisFactor3(uRand(10000), fChance);
                lbattr.value[i] = lbAttrConf.getAttrMax(lv, itemTypeIdx, lbattr.type[i]-1) * disFactor + 0.9999f;
                allAttrType.erase(allAttrType.begin() + idx);
            }
        }
        lbattr.lbColor = 2 + lbAttrConf.getColor(lv, subClass-Item_LBling, lbattr.type, lbattr.value, attrNum);
        if(lbattr.lbColor == 5)
        {
            UInt8 skillSwitch = lbAttrConf.getSkillSwitch(uRand(100));
            UInt8 startIdx = 0;
            UInt8 endIdx = 0;
            switch(skillSwitch)
            {
            case 0:
                endIdx = 1;
                break;
            case 1:
                startIdx = 1;
                endIdx = 2;
                break;
            case 2:
                endIdx = 2;
                break;
            }
            for(int i = startIdx; i < endIdx; ++ i)
            {
                UInt16 lbIdx = 0;
                if(i > 0)
                    lbIdx = subClass - Item_LBling + 1;
                UInt8 maxCnt = lbAttrConf.getSkillsMaxCnt(lbIdx, lv);
                UInt16 skillId = lbAttrConf.getSkill(lbIdx, lv, uRand(maxCnt));
                lbattr.skill[i-startIdx] = skillId;
                UInt16 factor = GData::lbSkillManager[skillId]->minFactor;
                lbattr.factor[i-startIdx] = factor + uRand(10000-factor);
            }
        }
        switch(lbattr.lbColor)
        {
        case 2:
            m_Owner->udpLog("Tongling", "F_10000_1", "", "", "", "", "act");
            break;
        case 3:
            m_Owner->udpLog("Tongling", "F_10000_2", "", "", "", "", "act");
            m_Owner->udpLog("Tongling", "F_10000_19", "", "", "", "", "act");
            break;
        case 4:
            m_Owner->udpLog("Tongling", "F_10000_3", "", "", "", "", "act");
            m_Owner->udpLog("Tongling", "F_10000_20", "", "", "", "", "act");
            break;
        case 5:
            m_Owner->udpLog("Tongling", "F_10000_4", "", "", "", "", "act");
            if(fSpecial)
                m_Owner->udpLog("Tongling", "F_10000_22", "", "", "", "", "act");
            else
                m_Owner->udpLog("Tongling", "F_10000_21", "", "", "", "", "act");
            break;
        }

        ItemEquipData edata;
        UInt32 id = IDGenerator::gItemOidGenerator.ID();
        ItemLingbao* equip = new ItemLingbao(id, itype, edata, lbattr);
        bool bind = m_lbSmeltInfo.bind;
        ITEM_BIND_CHECK(itype->bindType,bind);
        if(equip)
            equip->SetBindStatus(bind);

        ItemBase *& e = m_Items[id];
        if(e == NULL)
            ++ m_Size;
        e = equip;

        DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", id, m_Owner->getId(), m_lbSmeltInfo.bind);
        DB4().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `maxTRank`, `trumpExp`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`) VALUES(%u, %u, %u, %u, %u, %d, %u, %d, %u, %d)", id, lbid, edata.maxTRank, edata.trumpExp, edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3);

        std::string strType;
        std::string strValue;
        std::string strSkill;
        std::string strFactor;
        for(int i = 0; i < 4; ++ i)
        {
            strType += Itoa(lbattr.type[i], 10);
            strValue += Itoa(lbattr.value[i], 10);

            if(i < 3)
            {
                strType += ',';
                strValue += ',';
            }

            if(i < 2)
            {
                strSkill += Itoa(lbattr.skill[i], 10);
                strFactor += Itoa(lbattr.factor[i], 10);
                if(i < 1)
                {
                    strSkill += ',';
                    strFactor += ',';
                }
            }
        }
        DB4().PushUpdateData("REPLACE INTO `lingbaoattr`(`id`, `tongling`, `lbcolor`, `types`, `values`, `skills`, `factors`, `battlepoint`) VALUES(%u, %d, %d, '%s', '%s', '%s', '%s', '%u')", id, lbattr.tongling, lbattr.lbColor, strType.c_str(), strValue.c_str(), strSkill.c_str(), strFactor.c_str(), lbattr.battlePoint);

        SendSingleEquipData(equip);

        ItemNotifyEquip(equip);
        DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), lbid, id, FromFuling, TimeUtil::Now());
        OnAddEquipAndCheckAttainment(itype, FromFuling);

        closeLingbaoSmelt();

		Stream st(REP::EQ_LINGBAO);
		st << static_cast<UInt8>(6) << id << Stream::eos;
		m_Owner->send(st);
    }

    void Package::QuitLBSmelt()
    {
        if(m_lbSmeltInfo.gujiId == 0 || m_lbSmeltInfo.itemId == 0)
            return;

        std::vector<MailPackage::MailItem> mitem;
        {
            MailPackage::MailItem item = {m_lbSmeltInfo.itemId, 1};
            mitem.push_back(item);
        }
        {
            MailPackage::MailItem item = {m_lbSmeltInfo.gujiId, 1};
            mitem.push_back(item);
        }

        UInt16 smeltItem[4] = {9338, 9340, 9341, 9342};
        UInt16 smeltExp[4] = {1000, 200, 50, 10};

        UInt32 value = m_lbSmeltInfo.value;
        for(int i = 0; i < 4; ++ i)
        {
            UInt32 cnt = value / smeltExp[i];
            if(cnt > 0)
            {
                MailPackage::MailItem item = {smeltItem[i], cnt};
                mitem.push_back(item);
            }
            value = value % smeltExp[i];
        }

        SYSMSG(title, 4124);
        SYSMSG(content, 4124);

        MailItemsInfo itemsInfo(&(mitem[0]), LingbaoFuling, mitem.size());
        Mail * pmail = m_Owner->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        mailPackageManager.push(pmail->id, &(mitem[0]), mitem.size(), true);

        closeLingbaoSmelt();
    }

    UInt8 Package::closeLingbaoSmelt()
    {
        memset(&m_lbSmeltInfo, 0, sizeof(m_lbSmeltInfo));
        sendLingbaoSmeltInfo();

        DB4().PushUpdateData("DELETE FROM `lingbaosmelt` WHERE `playerId`=%"I64_FMT"u", m_Owner->getId());
        return 0;
    }

    void Package::sendLingbaoSmeltInfo()
    {
		Stream st(REP::EQ_LINGBAO);
		st << static_cast<UInt8>(5);

        st << m_lbSmeltInfo.gujiId;
        st << m_lbSmeltInfo.itemId;
        st << m_lbSmeltInfo.maxValue;
        st << m_lbSmeltInfo.value;
        st << m_lbSmeltInfo.bind;

        st << Stream::eos;
		m_Owner->send(st);
    }

    void Package::testLingbao(UInt32 itemId, UInt32* colorNums, UInt8 size, UInt32* skills, UInt8 size2)
    {
		const GData::ItemBaseType * itype = GData::itemBaseTypeManager[itemId];
        if(!itype || size != 4 || size2 != 2)
            return;

        UInt8 lv = itype->vLev;
        UInt8 subClass = itype->subClass;
        switch(subClass)
        {
        case Item_LBling:
        case Item_LBwu:
        case Item_LBxin:
            {
                ItemLingbaoAttr lbattr;
                getRandomLBAttr(lv, subClass, itype->quality, lbattr);
                UInt8 colorIdx = lbattr.lbColor - 2;
                ++ colorNums[colorIdx];

                if(lbattr.skill[0] != 0)
                {
                    if(lbattr.skill[1] != 0)
                        ++ skills[1];
                    else
                        ++ skills[0];
                }
                else if(lbattr.skill[1] != 0)
                {
                    ++ skills[0];
                }
            }
            break;
        default:
            break;
        }
    }
}
