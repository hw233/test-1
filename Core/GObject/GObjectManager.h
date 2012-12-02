#ifndef _GOBJECTMANAGER_H_
#define _GOBJECTMANAGER_H_

#include <map>
#include "GLocalObjectManager.h"
#include "GGlobalObjectManager.h"
#include "ItemData.h"
#include "AttrFactor.h"

namespace GObject
{
	class ItemEquip;
    class ItemFashion;
    class ItemHalo;
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
    struct YDItem
    {
        UInt32 itemId;
        UInt8  itemNum;
    };

    struct stMergeS
    {
        UInt32  id;
        UInt32  num;
    };
    struct stMergeStf
    {
        std::vector<stMergeS>  m_stfs;
        UInt32  m_to;
    };

    struct stAttrMax
    {
        stAttrMax() { memset( attrMax, 0, sizeof(attrMax)); }
        float attrMax[3][4][9];
    };

    struct stRingHpBase
    {
        stRingHpBase() { memset(hpBase, 0, sizeof(hpBase)); }
        float hpBase[4];
    };

    struct stHftChance
    {
        stHftChance() { times = 0; chance = 0; }
        UInt16 times;
        UInt32 chance;
    };

	class GObjectManager
	{
	public:
		static void loadAllData();

	public:
		static bool InitIDGen();
        static bool loadGVar();
		static bool loadMapData();
		static bool loadFighters();
		static bool loadCopy();
        static bool loadFrontMap();
		static bool loadClanAssist();
		static bool loadClanRobMonster();
		static bool loadAllPlayers();
		static bool loadAllAthletics();
		static bool loadAllAthleticsEvent();
		static bool delayLoad();
		static bool loadAllFriends();
		static bool loadAllClans();
		static bool loadEquipments();
		static bool loadEquipmentsSpirit();
		static bool LoadDungeon();
		static bool unloadEquipments();				//从数据库内卸载所有的装备
		static bool LoadSpecialAward();
		static bool LoadLuckyDraw();
		static bool LoadFighterTrain();
		static bool LoadArena();
        static bool LoadPracticePlace();
        static bool LoadPracticeData();
        static bool LoadTripodData();
        static bool LoadWorldBoss();
        static bool loadTeamCopy();
        static bool LoadTownDeamon();
        static bool InitMoneyLog();
        static bool LoadHeroMemo();
        static bool LoadShuoShuo();
        static bool LoadCFriendAwards();
        static bool LoadWBoss();
        static bool LoadDiscount();
        static bool LoadSoulItemChance();
        static bool loadSecondSoul();
        static bool LoadLuckyLog();
        static bool loadRealItemAward();
        static bool loadNewRelation();
        static bool loadRNR();
        static bool loadSkillStrengthen();
        static bool loadQixi();
        static bool loadArenaExtraBoard();

        static bool addGM(UInt64 id, UInt8 lvl);
        static bool delGM(UInt64 id);

        static UInt8 reRecharge(const std::string& no, UInt16 id, UInt32 num, std::string& err);

        static void pushEquipment(ItemEquip* e);
		static ItemEquip * fetchEquipment(UInt32, bool = true);  //获取装备
		static ItemHalo * fetchHalo(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemFashion * fetchFashion(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemWeapon * fetchWeapon(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemArmor * fetchArmor(UInt32);      //获取盔甲,基于fetchEquipment函数实现

		static bool InitGlobalObject();
		static bool resetClanData(UInt32);

        static bool loadEquipForge();
        static bool loadFightersPCChance();
        static bool loadAttrFactor();

        static bool loadQQVipAward();

        static void setFFTypeChance(UInt32);
        static void setFFAttrChance(UInt32);
        static void setFFAttrMaxValProp(UInt32);
        static void setFFAttrMaxVal(UInt32);

        static UInt8 getFFType();
        static UInt16 getFFValue(UInt8,bool = false);
        static UInt8 getFFMaxVal(UInt8);

        static UInt8 getFFAttr(UInt8& type, UInt16& value);

        static UInt32 getEnchantCost() { return _enchant_cost; }
        static UInt32 getMergeCost() { return _merge_cost; }
        static UInt32 getDetachCost() { return _detach_cost; }
        static UInt32 getSplitCost() { return _split_cost; }
        static UInt32 getForgeCost() { return _forge_cost; }
        static UInt32 getSplitChance( UInt8 q, UInt8 l_h ) { return _split_chance[q][l_h]; }
        static UInt32 getMergeChance( UInt8 lvl ) { return _merge_chance[lvl]; }
        static UInt32 getEnchantChance( UInt8 t, UInt8 q, UInt8 lvl ) { return _enchant_chance[t][q][lvl]; }
        static UInt8 getEnchantMax( UInt8 vip ) { return _enchant_max[vip]; }
        static UInt32 getSocketChance( UInt8 s ) { return _socket_chance[s]; }
        static UInt32 getMinPotential() { return _min_potential; }
        static UInt32 getMaxPotential() { return _max_potential; }
        static UInt32 getMinCapacity() { return _min_capacity; }
        static UInt32 getMaxCapacity() { return _max_capacity; }
        static std::vector<UInt32>& getPotentialChance() { return _potential_chance; }
        static std::vector<UInt32>& getCapacityChance() { return _capacity_chance; }
        static UInt8 getEvadeFactor() { return _evade_factor; }
        static UInt8 getHitrateFactor() { return _hitrate_factor; }
        static UInt8 getCriticalFactor() { return _critcal_factor; }
        static UInt8 getPierceFactor() { return _pierce_factor; }
        static UInt8 getToughFactor() { return _tough_factor; }

        static UInt32 getTrumpLOrderChance( UInt8 q, UInt8 lvl ) { return _trump_lorder_chance[q][lvl]; }
        static UInt32 getTrumpExpRank( UInt8 q, UInt8 lvl ) { return _trump_exp_rank[q][lvl]; }
        static AttrFactor& getTrumpTRankFactor( UInt8 q, UInt8 l ) { return _trump_rank_factor[q][l]; }
        static UInt16 getTrumpMaxRankChance( UInt8 idx )
        {
            if(idx >= _trump_maxrank_chance.size())
                return 0;

            return _trump_maxrank_chance[idx];
        }

        static float getTrumpSmelt(UInt8 q, UInt8 enchant)
        {
            if(q > 3)
                q = 0;
            int size = _trump_smelt[q].size();

            if(enchant == 0 || enchant > size)
                return 1;

            UInt8 idx = enchant - 1;
            return _trump_smelt[q][idx];
        }

        static float getHiterateMax() { return _hiterate_max; }
        static float getEvadeMax() { return _evade_max; }
        static float getCriticalMax() { return _critical_max; }
        static float getPierceMax() { return _pierce_max; }
        static float getToughMax() { return _tough_max; }
        static float getCounterMax() { return _counter_max; }
        static float getMagResMax() { return _mres_max; }
        static float getCriticalDmgMax() { return _cridmg_max; }


        static UInt16 getAttrTypeChance(UInt8 q, UInt8 idx) { return _attrTypeChances[q][idx]; }
        static UInt16 getAttrChance( UInt8 q, UInt8 idx ) { return _attrChances[q][idx]; }
        static float  getAttrMax( UInt8 lvl, UInt8 t, UInt8 q, UInt8 crr )
        {
            if(q > 2)
                q = 2;
            if(t > 8)
                t = 8;
            if(crr > 3)
                crr = 3;
            std::map<UInt8, stAttrMax*>::iterator it = _attrMax.find(lvl);
            stAttrMax* attr = NULL;
            if(it != _attrMax.end())
                attr = it->second;
            else
                attr = _attrMax[10];

            return attr->attrMax[q][crr][t];
        }
        static UInt16 getAttrDics(UInt8 q, UInt8 idx) { return _attrDics[q][idx]; }
        static float  getAttrTrumpMax( UInt8 lvl, UInt8 t, UInt8 q, UInt8 crr )
        {
            if(q > 2)
                q = 2;
            if(t > 8)
                t = 8;
            if(crr > 3)
                crr = 3;
            std::map<UInt8, stAttrMax*>::iterator it = _attrTrumpMax.find(lvl);
            stAttrMax* attr = NULL;
            if(it != _attrTrumpMax.end())
                attr = it->second;
            else
                attr = _attrTrumpMax[0];

            return attr->attrMax[q][crr][t];
        }

        static UInt32 getColorFighterChance(UInt16 idx, UInt8 f_g, UInt8 color )
        {
            if(!f_g)
            {
                if( idx > _color_chance_free.size() - 1 )
                    idx = _color_chance_free.size() - 1;

                return _color_chance_free[idx][color];
            }
            else
            {
                if( idx > _color_chance_gold.size() - 1 )
                    idx = _color_chance_gold.size() - 1;

                return _color_chance_gold[idx][color];
            }
        }

        static UInt16 getBattleScene(UInt16 location)
        {
            std::map<UInt16, UInt16>::iterator it = _battle_scene.find(location);
            if(it != _battle_scene.end())
                return it->second;
            else
                return 0;
        }

        static UInt8 getYDMaxCount()
        {
            return _yellow_diamond_award.size();
        }

        static std::vector<YDItem>& getYDItem(UInt8 qqvipl)
        {
            if(qqvipl > _yellow_diamond_award.size() - 1)
                qqvipl = _yellow_diamond_award.size() - 1;

            return _yellow_diamond_award[qqvipl];
        }

        static UInt8 getD3D6MaxCount()
        {
            return _d3d6_diamond_award.size();
        }

        static std::vector<YDItem>& getD3D6Item(UInt8 qqvipl)
        {
            if(qqvipl > _d3d6_diamond_award.size() - 1)
                qqvipl = _d3d6_diamond_award.size() - 1;

            return _d3d6_diamond_award[qqvipl];
        }

        static UInt8 getQPlusMaxCount()
        {
            return _qplus_diamond_award.size();
        }

        static std::vector<YDItem>& getQPlusItem(UInt8 qqvipl)
        {
            if(qqvipl > _qplus_diamond_award.size() - 1)
                qqvipl = _qplus_diamond_award.size() - 1;

            return _qplus_diamond_award[qqvipl];
        }

        static std::vector<YDItem>& getYearYDItem()
        {
            return _year_yellow_diamond_award;
        }

        static std::vector<UInt32>& getYDGem()
        {
            return _yellow_diamond_gem;
        }

        static UInt32 getTeamMatieralChance(UInt8 q)
        {
            if(q > 2)
                q = 2;

            return _team_m_chance[q];
        }

        static UInt32 getTeamMatieralItemFromSplit(UInt8 q)
        {
            if(q > 2)
                q = 2;

            return _team_m_item[q];
        }

        static UInt32 getOrangeTeamMatieralChance(UInt32 itemId, UInt8 type)
        {
            if(type > 2)
                type = 0;

            std::map<UInt32, UInt32>::iterator it = _team_om_chance[type].find(itemId);
            if(it != _team_om_chance[type].end())
                return it->second;
            else
                return 0;
        }

        static UInt32 getOrangeTeamMatieralItemFromSplit(UInt32 itemId)
        {
            std::map<UInt32, UInt32>::iterator it = _team_om_item.find(itemId);
            if(it != _team_om_item.end())
                return it->second;
            else
                return 0;
        }

        static UInt32 getChanceFromHft(UInt8 q, UInt8 lv, UInt16 v)
        {
            if(q > 5)
                q = 0;
            if(lv > 11)
                lv = 0;

            UInt8 cnt = _hft_chance[q][lv].size();
            for(int i = cnt - 1; i >= 0; --i)
            {
                if(_hft_chance[q][lv][i].times <= v)
                    return _hft_chance[q][lv][i].chance;
            }

            return 0;
        }

        static float getRingHpFromEnchant(UInt8 lvl, UInt8 crr, UInt8 enchant)
        {
            if(enchant == 0)
                return 0;

            if(crr > 3)
                crr = 0;
            if(enchant > 12)
                enchant = 0;
            -- enchant;

            std::map<UInt8, stRingHpBase*>::iterator it = _ringHpBase.find(lvl);
            if(it == _ringHpBase.end())
            {
                UInt8 lvl2 = lvl - lvl%10;
                it = _ringHpBase.find(lvl2);
            }

            stRingHpBase* ringHp = NULL;
            if(it == _ringHpBase.end())
                ringHp = _ringHpBase[10];
            else
                ringHp = it->second;

            return ringHp->hpBase[crr] * _ringHpFactor[enchant];
        }

        static UInt32 getSoulEnchantChance(UInt8 soulStateLevel, UInt8 itemColor)
        {
            if(soulStateLevel < 1 || itemColor < 2)
                return 0;
            UInt8 lvlIdx = soulStateLevel - 1;
            UInt8 colorIdx = itemColor - 2;
            if(lvlIdx >= _soulEnchantChance.size())
                return 0;
            if(colorIdx >= _soulEnchantChance[lvlIdx].size())
                return 0;

            return _soulEnchantChance[lvlIdx][colorIdx];
        }

        static UInt32 getDecSoulStateExp(UInt8 soulStateLevel)
        {
            if(soulStateLevel < 1)
                return 0;
            UInt8 lvlIdx = soulStateLevel - 1;
            if(lvlIdx >= _decSoulStateExp.size())
                return 0;

            return _decSoulStateExp[lvlIdx];
        }

       	private:
		static std::map<UInt32, ItemEquip *> equips;
        static UInt32 _enchant_cost;
        static UInt32 _merge_cost;
        static UInt32 _detach_cost;
        static UInt32 _split_cost;
        static UInt32 _forge_cost;
        static UInt32 _split_chance[4][2];
        static UInt32 _merge_chance[11];
		static UInt32 _enchant_chance[2][6][12];
		static UInt8  _enchant_max[11];
		static UInt32 _trump_lorder_chance[6][12];
		static UInt32 _trump_exp_rank[6][12];
		static AttrFactor _trump_rank_factor[6][12];

		static UInt16 _attrTypeChances[3][9];
		static UInt16 _attrChances[3][9];
		static std::map<UInt8, stAttrMax*> _attrMax;
		static std::map<UInt8, stAttrMax*> _attrTrumpMax;
        static UInt16 _attrDics[3][9];

        static UInt32 _socket_chance[6];
        static UInt32 _min_potential;
        static UInt32 _max_potential;
        static UInt32 _min_capacity;
        static UInt32 _max_capacity;
        static std::vector<UInt32> _potential_chance;
        static std::vector<UInt32> _capacity_chance;
        static UInt8 _evade_factor;
        static UInt8 _hitrate_factor;
        static UInt8 _critcal_factor;
        static UInt8 _pierce_factor;
        static UInt8 _tough_factor;
        static float _hiterate_max;
        static float _evade_max;
        static float _critical_max;
        static float _pierce_max;
        static float _tough_max;
        static float _counter_max;
        static float _mres_max;
        static float _cridmg_max;

        // 项链戒指强化
        static std::map<UInt8, stRingHpBase*> _ringHpBase;
        static float _ringHpFactor[12];

        // 天赋洗炼
        static std::vector<UInt32> _FFTypeChance;
        static std::vector<UInt32> _FFAttrChance;
        static std::vector<UInt32> _FFAttrMaxValProb;
        static std::vector<UInt32> _FFAttrMaxVal;

        // 酒馆武将品质概率
        static std::vector<std::vector<UInt32>> _color_chance_gold;
        static std::vector<std::vector<UInt32>> _color_chance_free;

        //战斗背景
        static std::map<UInt16, UInt16> _battle_scene;

        //黄钻物品奖励
        static std::vector<std::vector<YDItem>> _yellow_diamond_award;
        static std::vector<std::vector<YDItem>> _d3d6_diamond_award;
        static std::vector<std::vector<YDItem>> _qplus_diamond_award;
        static std::vector<YDItem>              _year_yellow_diamond_award;
        static std::vector<UInt32>              _yellow_diamond_gem;

		static std::vector<UInt16> _trump_maxrank_chance;
        static std::vector<float> _trump_smelt[4];

        // 强化人品
        static std::vector<stHftChance> _hft_chance[6][12];

       // static std::vector<stMergeStf>  _mergeStfs;
        typedef std::vector<stMergeStf>   vMergeStfs;
        typedef std::map <UInt32,  std::vector<UInt32> >  mMergeStfsIndex;
        static  vMergeStfs   _vMergeStfs;
        static  mMergeStfsIndex  _mMergeStfsIndex;

        //static std::map<UInt32, UInt32>  _EUpgradeIdMap;
        static UInt32 _team_m_chance[3];
        static UInt32 _team_m_item[3];
        static std::map<UInt32, UInt32> _team_om_chance[3];
        static std::map<UInt32, UInt32> _team_om_item;

        static std::vector<std::vector<UInt32>> _soulEnchantChance;
        static std::vector<UInt32> _decSoulStateExp;

        public:
        static  vMergeStfs  getMergeStfs( UInt32 id)
        {
            vMergeStfs  re;

            mMergeStfsIndex ::iterator it = _mMergeStfsIndex.find(id);
            if(it != _mMergeStfsIndex.end())
            {
                std::vector<UInt32>& v = it->second;
                for(UInt32 i = 0 ; i < v.size(); i ++)
                {
                    if(v[i] < _vMergeStfs.size())
                        re.push_back(_vMergeStfs[v[i]]);
                }

            }
            return re;
        }
        /*
        static UInt32 getEUpgradeId(UInt32 id)
        {
            std::map<UInt32, UInt32> ::iterator it = _EUpgradeIdMap.find(id);
            if(it != _EUpgradeIdMap.end())
                return it->second;
             return 0;
        }*/

	};
}

#endif // _GOBJECTMANAGER_H_
