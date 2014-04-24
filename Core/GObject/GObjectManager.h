﻿#ifndef _GOBJECTMANAGER_H_
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
    class ItemInnateTrump;
	class ItemWeapon;
	class ItemArmor;
    class ItemLingbao;
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
        float attrMax[3][5][9];
    };

    struct stLbAttrMax
    {
        stLbAttrMax() { memset( attrMax, 0, sizeof(attrMax)); }
        float attrMax[3][12];
    };

    struct stLBAttrConf
    {
        stLBAttrConf()
        {
            memset( attrNumChance, 0, sizeof(attrNumChance));
            memset( skillSwitchChance, 0, sizeof(skillSwitchChance));
            memset( dis, 0, sizeof(dis));
            memset( disChance, 0, sizeof(disChance));
            memset( colorVal, 0, sizeof(colorVal));
        }

        UInt8 getAttrNum(UInt16 chance)
        {
            for(int i = 0; i < 4; ++ i)
            {
                if(chance < attrNumChance[i])
                    return (i + 1);
            }
            return 1;
        }

        UInt8 getSkillSwitch(UInt16 chance)
        {
            int i = 0;
            for(; i < 3; ++ i)
            {
                if(chance < skillSwitchChance[i])
                    break;
            }
            return i;
        }

        float getDisFactor(UInt16 chance)
        {
            int maxIdx = 9;
            UInt16 lastDisChance = 0;
            for(int i = 0; i < 9; ++ i)
            {
                if(chance < disChance[i])
                {
                    float fChance = ((float)(chance + 1 - lastDisChance)) / (disChance[i] - lastDisChance);
                    float fDis = ((float)(dis[i]) + (dis[i+1] - dis[i])*fChance) / dis[maxIdx];
                    return fDis;
                }
                lastDisChance = disChance[i];
            }
            return 0;
        }

        float getDisFactor2(UInt16 chance, UInt8 color)
        {
            int maxIdx = 9;
            UInt16 lastDisChance = 0;
            UInt8 colorIdx = color - 2;
            if(colorIdx > 3)
                colorIdx = 0;
            float minDis = ((float)(colorVal[colorIdx]))/400;

            float fChance = 0;
            for(int i = 0; i < 9; ++ i)
            {
                if(chance < disChance[i])
                {
                    if(fChance < 0.00001f)
                        fChance = ((float)(chance + 1 - lastDisChance)) / (disChance[i] - lastDisChance);
                    float fDis = ((float)(dis[i]) + (dis[i+1] - dis[i])*fChance) / dis[maxIdx];
                    if(fDis > minDis)
                        return fDis;
                }
                lastDisChance = disChance[i];
            }
            return 0;
        }

        float getDisFactor3(UInt16 chance, float fChance)
        {
            int maxIdx = 9;
            UInt16 lastDisChance = 0;
            for(int i = 0; i < 9; ++ i)
            {
                if(chance < disChance[i])
                {
                    float fDis = ((float)(dis[i]) + (dis[i+1] - dis[i])*fChance) / dis[maxIdx];
                    return fDis;
                }
                lastDisChance = disChance[i];
            }
            return 0;
        }

        float getDisFactor4(UInt16 chance, float fChance, UInt8 color)
        {
            int maxIdx = 9;
            UInt16 lastDisChance = 0;
            UInt8 colorIdx = color - 2;
            if(colorIdx > 3)
                colorIdx = 0;
            float minDis = ((float)(colorVal[colorIdx]))/400;

            for(int i = 0; i < 9; ++ i)
            {
                if(chance < disChance[i])
                {
                    float fDis = ((float)(dis[i]) + (dis[i+1] - dis[i])*fChance) / dis[maxIdx];
                    if(fDis > minDis)
                    {
                        return fDis;
                    }
                    else
                    {
                        fChance *= 0.5;
                    }
                }
                lastDisChance = disChance[i];
            }
            return 0;
        }

        UInt8 getSkillsMaxCnt(UInt8 lbIdx, UInt8 lvl)
        {
            if(lbIdx > 3)
                return 0;
            std::map<UInt8, std::vector<UInt16>>::iterator it = skills[lbIdx].find(lvl);
            if(it == skills[lbIdx].end())
                return 0;
            std::vector<UInt16>& lv_skill = it->second;
            return lv_skill.size();
        }

        UInt16 getSkill(UInt8 lbIdx, UInt8 lv, UInt16 chance)
        {
            if(lbIdx > 3)
                return 0;
            std::map<UInt8, std::vector<UInt16>>::iterator it = skills[lbIdx].find(lv);
            if(it == skills[lbIdx].end())
                return 0;

            std::vector<UInt16>& lv_skill = it->second;
            if(chance > lv_skill.size())
                return 0;

            return lv_skill[chance];
        }

        float getAttrMax(UInt8 lv, UInt8 itemTypeIdx, UInt8 attrTypeIdx)
        {
            if(itemTypeIdx > 2 || attrTypeIdx > 11)
                return 0;
            std::map<UInt8, stLbAttrMax>::iterator it = lbAttrMax.find(lv);
            if(it == lbAttrMax.end())
                return 0;
            stLbAttrMax& lbAttr = it->second;
            return lbAttr.attrMax[itemTypeIdx][attrTypeIdx];
        }

        UInt8 getColor(UInt8 lv, UInt8 itemTypeIdx, UInt8* at, UInt16* av, UInt8 size)
        {
            float colorP = 0;
            for(int i = 0; i < size; ++ i)
            {
                if(at[i] > 0)
                    colorP += ((float)(av[i])/getAttrMax(lv, itemTypeIdx, at[i]-1))*100;
            }
            for(int j = 3; j > 0; -- j)
            {
                if(colorP > colorVal[j])
                    return j;
            }
            return 0;
        }

        UInt16 getSmeltExp(UInt8 lv, UInt8 itemTypeIdx, UInt8* at, UInt16* av, UInt8 size, UInt8 skillNum, UInt8 color)
        {
            float colorP = 0;
            float lvFactor[] = {1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0};
            UInt8 lvIdx = (lv-70)/10;
            UInt16 skillExp = 100;
            if(lvIdx > 6)
                lvIdx = 6;

            for(int i = 0; i < size; ++ i)
            {
                if(at[i] > 0)
                    colorP += ((float)(av[i])/getAttrMax(lv, itemTypeIdx, at[i]-1))*100;
            }
            UInt8 colFactor = 1;//宝具颜色系数
            if(color >= 5)
                colFactor = 10;

            return colorP*lvFactor[lvIdx]*colFactor + skillExp * skillNum;
        }

        UInt16 getSmeltExp2(UInt8 lv, UInt8 itemTypeIdx, UInt8 color)
        {
            float lvFactor[] = {1, 1.2, 1.4, 1.6, 1.8, 2.0, 2.2, 2.4, 2.6, 2.8, 3.0};
            UInt8 lvIdx = (lv-70)/10;
            if(lvIdx > 6)
                lvIdx = 6;

            if(color < 2)
                color = 2;
            if(color > 5)
                color = 5;
            UInt8 colFactor = 1;//宝具颜色系数
            if(color >= 5)
                colFactor = 10;
            return colorVal[color - 2]*lvFactor[lvIdx] * colFactor;
        }

        UInt8 attrNumChance[4];
        UInt8 skillSwitchChance[3];
        UInt16 dis[11];
        UInt16 disChance[11];
        UInt16 colorVal[4];
        std::vector<UInt8> attrType;
        std::map<UInt8, std::vector<UInt16>> skills[4]; // map<灵宝等级, vector<技能id>>
		std::map<UInt8, stLbAttrMax> lbAttrMax;  // 灵宝属性上限
    };

    struct stRingHpBase
    {
        stRingHpBase() { memset(hpBase, 0, sizeof(hpBase)); }
        float hpBase[5];
    };

    struct stHftChance
    {
        stHftChance() { times = 0; chance = 0; }
        UInt16 times;
        UInt32 chance;
    };

    struct stZhyAttrMax
    {
        stZhyAttrMax() { memset( attrMax, 0, sizeof(attrMax)); }
        float attrMax[14];
    };

    struct stZhyExtraAttr
    {
        stZhyExtraAttr() : id(0), level(0), type1(0), type2(0), attrMax(0.0f) { }
        UInt16 id;
        UInt8 level;
        UInt8 type1;
        UInt8 type2;
        float attrMax;
    };

    struct stZHYAttrConf
    {
        stZHYAttrConf()
        {
            memset( attrNumChance, 0, sizeof(attrNumChance));
            memset( extraSwitchChance, 0, sizeof(extraSwitchChance));
            memset( dis, 0, sizeof(dis));
            memset( disChance, 0, sizeof(disChance));
            memset( colorVal, 0, sizeof(colorVal));
        }

        UInt16 getExtraAttrid(UInt8 lv, bool isFull);
        float getDisFactor2(UInt8 color);

        UInt8 getAttrNum(UInt16 chance)
        {
            for(int i = 0; i < 4; ++ i)
            {
                if(chance < attrNumChance[i])
                    return (i + 1);
            }
            return 1;
        }

        UInt8 getSkillSwitch(UInt16 chance)
        {
            int i = 0;
            for(; i < 4; ++ i)
            {
                if(chance < extraSwitchChance[i])
                    break;
            }
            return i;
        }

        UInt8 getSkillSwitch1(UInt16 chance)
        {
            static UInt16 mergeChance[4] = {0, 33, 90, 100};
            int i = 0;
            for(; i < 4; ++ i)
            {
                if(chance < mergeChance[i])
                    break;
            }
            return i;
        }

        float getAttrMax(UInt8 lv, UInt8 attrTypeIdx)
        {
            if(attrTypeIdx > 13)
                return 0;
            std::map<UInt8, stZhyAttrMax>::iterator it = zhyAttrMax.find(lv);
            if(it == zhyAttrMax.end())
                return 0;
            stZhyAttrMax& zhyAttr = it->second;
            return zhyAttr.attrMax[attrTypeIdx];
        }

        UInt8 getColor(UInt8 lv, UInt8* at, UInt16* av, UInt8 size)
        {
            float colorP = 0;
            for(int i = 0; i < size; ++ i)
            {
                if(at[i] > 0)
                    colorP += ((float)(av[i])/getAttrMax(lv, at[i]-1))*100;
            }
            for(int j = 3; j > 0; -- j)
            {
                if(colorP > colorVal[j])
                    return j;
            }
            return 0;
        }

        stZhyExtraAttr * getExtraAttr(UInt16 attrId)
        {
            std::map<UInt16, stZhyExtraAttr>::iterator it = extraAttrMax.find(attrId);
            if(it == extraAttrMax.end())
                return NULL;
            return &(it->second);
        }

        float getExtraAttrMax(UInt16 attrId)
        {
            std::map<UInt16, stZhyExtraAttr>::iterator it = extraAttrMax.find(attrId);
            if(it == extraAttrMax.end())
                return 0;
            return it->second.attrMax;
        }

        float getDisFactor(UInt16 chance)
        {
            int maxIdx = 9;
            UInt16 lastDisChance = 0;
            for(int i = 0; i < 9; ++ i)
            {
                if(chance < disChance[0][i])
                {
                    float fChance = ((float)(chance + 1 - lastDisChance)) / (disChance[0][i] - lastDisChance);
                    float fDis = ((float)(dis[0][i]) + (dis[0][i+1] - dis[0][i])*fChance) / dis[0][maxIdx];
                    return fDis;
                }
                lastDisChance = disChance[0][i];
            }
            return 0;
        }

        void getAttrColor(UInt8 lv, UInt8* at, UInt16* av, UInt8 size, UInt8* colors)
        {
            static float colorLvl[4] = { 30, 60, 80, 100 }; //绿 蓝 紫 橙
            for(int i = 0; i < size; ++ i)
            {
                if(at[i] > 0)
                {
                    float colorP = ((float)(av[i])/getAttrMax(lv, at[i]-1))*100;
                    for(int j = 0; j < 4; ++ j)
                    {
                        if(colorP <= colorLvl[j])
                        {
                            colors[i] = j + 2;
                            break;
                        }
                    }
                }
            }
        }

        UInt8 attrNumChance[4];
        UInt8 extraSwitchChance[3];
        UInt16 dis[4][11];
        UInt16 disChance[4][11];
        UInt16 colorVal[4];
        std::vector<UInt8> attrType;
		std::map<UInt8, stZhyAttrMax> zhyAttrMax;  // 阵元属性上限
		std::map<UInt8, std::vector<UInt16>> extraAttrType[2];  // 阵元属性类型
		std::map<UInt16, stZhyExtraAttr> extraAttrMax;  // 阵元额外属性上限
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
        static bool loadXJFrontMap();
		static bool loadClanAssist();
		static bool loadClanRobMonster();
		static bool loadAllPlayers();
		static bool loadAllAthletics();
		static bool loadAllAthleticsEvent();
		static bool delayLoad();
		static bool loadAllFriends();
		static bool loadAllPrayInfo();
		static bool loadAllPresentBoxInfo();
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
        static bool loadPetTeamCopy();
        static bool loadPlayerPetTeamCopy();
        static bool loadPetTeamCopyLog();
        static bool loadMoBao();
        static bool LoadTownDeamon();
        static bool InitMoneyLog();
        static bool LoadHeroMemo();
        static bool LoadShuoShuo();
        static bool LoadCFriendAwards();
        static bool LoadWBoss();
        static bool LoadDiscount();
        static bool LoadSoulItemChance();
        static bool loadSecondSoul();
        static bool loadFighterXingchen();
        static bool loadFighterXinMo();
        static bool loadTempItem();
        static bool loadJiguanshu();
        static bool loadJiguanyu();
        static bool loadTuzhi();
        static bool loadKeyin();
        static bool LoadLuckyLog();
        static bool loadRealItemAward();
        static bool loadNewRelation();
        static bool loadRNR();
        static bool loadSkillStrengthen();
        static bool loadQixi();
        static bool loadGuangGun();
        static bool loadSnow();
        static bool loadArenaExtraBoard();
        static bool loadCopyFrontWin();
        static bool loadLBSkill();
        static bool loadLingbaoAttr();
        static bool loadLingbaoSmelt();
        static bool fixItem9383Leader();
        static bool loadPetEquipAttr();
	    static bool loadZhenyuanAttr();
	    static bool loadLingshiAttr();
        static bool loadFairySpar();
        static bool loadCollectCard();
        //static bool loadSanHun();
        static bool loadTeamArenaData();
        static bool loadTeamArenaSkill();
	    static bool LoadTeamArenaBets();
	    static bool LoadTeamPendingPlayers();
	    static bool LoadArenaServerWar();
	    static bool LoadServerWarBets();
	    static bool LoadPlayerNamed();
	    static bool LoadMarriage();
	    static bool LoadReplyMarriage();
	    static bool LoadMarriedLog();
	    static bool LoadMarriedCouple();
	    static bool LoadSevenSoul();
        static bool loadZhenwei();
        static bool loadQiShiBan();

        static bool loadGoback();
        static bool loadApplyList();
        static bool loadTeamMember();
        static bool loadInactiveMember();
        static bool LoadPlayerModifyMounts();
        static bool loadSkillGrade();

        static bool addGM(UInt64 id, UInt8 lvl);
        static bool delGM(UInt64 id);

        static UInt8 reRecharge(const std::string& no, UInt16 id, UInt32 num, std::string& err);

        static void pushEquipment(ItemEquip* e);
		static ItemEquip * fetchEquipment(UInt32, bool = true);  //获取装备
		static ItemHalo * fetchHalo(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemInnateTrump * fetchInnateTrump(UInt32);
		static ItemFashion * fetchFashion(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemWeapon * fetchWeapon(UInt32);    //获取武器，基于fetchEquipment函数实现
		static ItemArmor * fetchArmor(UInt32);      //获取盔甲,基于fetchEquipment函数实现
        static void checkLingbaoAttrType(ItemLingbao* lb);

		static bool InitGlobalObject();
		static bool resetClanData(UInt32);

        static bool loadEquipForge();
        static bool loadFightersPCChance();
        static bool loadZhenyuanConfig();
        static bool loadAttrFactor();

        static bool loadQQVipAward();

        static bool loadJobHunter();
        static bool loadDreamer();

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

		static stLBAttrConf& getLBAttrConf() { return _lbAttrConf; }
		static stZHYAttrConf& getZHYAttrConf() { return _zhyAttrConf; }

        static UInt16 getAttrTypeChance(UInt8 q, UInt8 idx) { return _attrTypeChances[q][idx]; }
        static UInt16 getAttrChance( UInt8 q, UInt8 idx ) { return _attrChances[q][idx]; }
        static float  getAttrMax( UInt8 lvl, UInt8 t, UInt8 q, UInt8 crr );
        static UInt16 getAttrDics(UInt8 q, UInt8 idx) { return _attrDics[q][idx]; }
        static float  getAttrTrumpMax( UInt8 lvl, UInt8 t, UInt8 q, UInt8 crr );

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
            if(q > 10)
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

        static float getRingHpFromEnchant(UInt8 lvl, UInt8 crr, UInt8 enchant);
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
        static std::vector<stHftChance> _hft_chance[11][12];

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

        // 灵宝属性上限
		static stLBAttrConf _lbAttrConf;

		static stZHYAttrConf _zhyAttrConf;

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
