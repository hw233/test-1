#ifndef _PACKAGE_H_
#define _PACKAGE_H_

#include "Item.h"
#include "GData/GDataManager.h"
#include "GObject/GGlobalObjectManager.h"
#include "Common/URandom.h"

namespace GObject
{
    static const UInt16 PEquip40[] = {2400,2401,2402,2403,2404,2405,2406,2407,2408,2409,2410,2411,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,2422,2423};

    static const UInt16 OEquip50[] = {2544,2545,2546,2547,2548,2549,2550,2551,2552,2553,2554,2555,2556,2557,2558,2559,2560,2561,2562,2563,2564,2565,2566,2567};
    static const UInt16 OEquip60[] = {2568,2569,2570,2571,2572,2573,2574,2575,2576,2577,2578,2579,2580,2581,2582,2583,2584,2585,2586,2587,2588,2589,2590,2591};
    static const UInt16 OEquip70[] = {2592,2593,2594,2595,2596,2597,2598,2599,2600,2601,2602,2603,2604,2605,2606,2607,2608,2609,2610,2611,2612,2613,2614,2615};
    static const UInt16 OEquip80[] = {2616,2617,2618,2619,2620,2621,2622,2623,2624,2625,2626,2627,2628,2629,2630,2631,2632,2633,2634,2635,2636,2637,2638,2639};
    static const UInt16 OEquip90[] = {2640,2641,2642,2643,2644,2645,2646,2647,2648,2649,2650,2651,2652,2653,2654,2655,2656,2657,2658,2659,2660,2661,2662,2663};
    static const UInt16 OEquip100[]= {2664,2665,2666,2667,2668,2669,2670,2671,2672,2673,2674,2675,2676,2677,2678,2679,2680,2681,2682,2683,2684,2685,2686,2687};
    static const UInt16 OEquip110[]= {2936,2937,2938,2939,2940,2941,2942,2943,2944,2945,2946,2947,2948,2949,2950,2951,2952,2953,2954,2955,2956,2957,2958,2959};
    static const UInt16 OEquip120[]= {3000,3001,3002,3003,3004,3005,3006,3007,3008,3009,3010,3011,3012,3013,3014,3015,3016,3017,3018,3019,3020,3021,3022,3023};
    static const UInt16 OEquip130[]= {3064,3065,3066,3067,3068,3069,3070,3071,3072,3073,3074,3075,3076,3077,3078,3079,3080,3081,3082,3083,3084,3085,3086,3087};
    static const UInt16 OEquip140[]= {3128,3129,3130,3131,3132,3133,3134,3135,3136,3137,3138,3139,3140,3141,3142,3143,3144,3145,3146,3147,3148,3149,3150,3151};

    UInt16 getRandOEquip(UInt8 lvl);
    UInt16 getRandPEquip(UInt8 lvl);
    UInt32 getRandGem(UInt8 lev);
#define ITEM_ENCHANT_L1 502   //Ì«ÒÒÕæ½ð
#define ITEM_ENCHANT_L2 503   //Ì«ÒÒ¾«½ð
#define ITEM_ENCHANT_L3 17102  //仙兵仙衣专用强化材料
#define ITEM_ENCHANT_L4 17103  //仙兵仙衣专用强化材料

#define TRUMP_ENCHANT_L1 514   //ÎåÐÐ¾«Ó¢
#define TRUMP_ENCHANT_L2 515  //ÎåÐÐ¾«»ª

#define TRUMP_ENCHANT_L3 17104  //仙兵仙衣专用强化材料
#define TRUMP_ENCHANT_L4 17105  //仙兵仙衣专用强化材料

#define TRUMP_RONGLIAN_XIAN_GEGIN 17106
#define TRUMP_RONGLIAN_XIAN_END 17108

#define ARENA_BET_ITEM1    9081
#define ARENA_BET_ITEM2    9082

#define PURPLEADJVAL_TYPE  1
#define ORANGEADJVAL_TYPE  2

#define PACKAGE_0 1
#define PACKAGE_1 2
#define PACKAGE_2 4
#define PACKAGE_3 8
#define PACKAGE_4 16
#define PACKAGE_5 32
#define PACKAGE_0_3 (PACKAGE_0 | PACKAGE_3)
#define PACKAGE_0_4 (PACKAGE_0 | PACKAGE_4)
#define PACKAGE_1_5 (PACKAGE_1 | PACKAGE_5)
#define PACKAGE_0_3_4 (PACKAGE_0 | PACKAGE_3 | PACKAGE_4)

	class Fighter;
	class Player;

    struct SplitItemOut
    {
        UInt16 itemId;
        UInt16 count;
    };

	struct ItemKey
	{
		ItemKey(UInt32 id_ = 0, bool bind_ = false):
			id(id_), bind(bind_) { }
		ItemKey(UInt32 id_, UInt8 bind_):
			id(id_), bind(bind_ > 0) { }
		UInt32 id;
		bool bind;
		bool operator < (const ItemKey& other) const
		{
			if(id == other.id)
			{
				if(IsEquipId(id))
					return false;
				return bind < other.bind;
			}
			return id < other.id;
		}
	};

	struct _GemCompare
	{
		bool operator() (UInt16 id1, UInt16 id2)
		{
			UInt16 lv1 = (id1 - 1) % 10;
			UInt16 lv2 = (id2 - 1) % 10;
			if(lv1 == lv2)
				return id1 < id2;
			return lv1 > lv2;
		}
	};

    struct LingbaoSmeltInfo
    {
        UInt16 gujiId;
        UInt16 itemId;
        UInt32 value;
        UInt32 maxValue;
        UInt8 bind;
        UInt8 counts;
        UInt8 purpleAdjVal;
        UInt8 orangeAdjVal;

        LingbaoSmeltInfo() : gujiId(0), itemId(0), value(0), maxValue(0), bind(0), counts(0), purpleAdjVal(0), orangeAdjVal(0) {}

		const LingbaoSmeltInfo& operator= (LingbaoSmeltInfo& info)
        {
            gujiId = info.gujiId;
            itemId = info.itemId;
            value = info.value;
            maxValue = info.maxValue;
            bind = info.bind;
            counts = info.counts;
            purpleAdjVal = info.purpleAdjVal;
            orangeAdjVal = info.orangeAdjVal;

            return *this;
        }
    };

	class Package
	{
	public:
		Package(Player* player);
		~Package();

	public:
		bool Init();
		void UnInit();

	public:
		ItemBase*  AddItem(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt16 fromWhere = 0);
		ItemBase*  AddItem2(ItemBase* item, UInt16 fromWhere = 0);
		ItemBase*  AddItem2(UInt32 typeId, UInt32 num, bool notify, bool bind = false, UInt16 fromWhere = 0);
		ItemBase*  AddItemFromDB(UInt32 id, UInt32 num, bool bind);
		ItemBase * AddExistEquip(ItemEquip *);
		ItemBase*  AddEquip(UInt32 typeId, bool bind = false, bool silence = false, UInt16 FromWhere = 0);
		ItemBase*  AddEquip2(ItemEquip *, UInt16 FromWhere = 0);
		ItemBase*  AddEquip2(UInt32 typeId, bool notify, bool bind = false, UInt16 FromWhere = 0);
		ItemBase*  AddEquipN(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt16 FromWhere = 0);
		ItemBase*  AddEquipEnchant(UInt32 typeId, UInt8 enchant, bool notify, bool bind = false, UInt16 FromWhere = 0);

		ItemBase*  AddEquipFromDB(UInt32 id, bool bind);
		ItemBase*  Add(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt16 FromWhere = 0);
		ItemBase*  SetItem(UInt32 typeId, UInt32 num, bool bind = false);
		bool   DelItemAll(UInt32 id, bool bind = false);
		bool   DelItem(UInt32 id, UInt16 num, bool bind = false, UInt16 toWhere = 0);
		bool   DelItem2(ItemBase* item, UInt16 num, UInt16 toWhere = 0);
		bool   DelItemAny(UInt32 id, UInt16 num, bool * = NULL, UInt16 toWhere = 0);
		bool   DelItemAny2(UInt32 id, UInt16 num);
        bool   eraseEquip(UInt32 id);     //删除背包道具容器
		bool   DelEquip(UInt32 id, UInt16 toWhere = 0);
		bool   DelEquip2(ItemEquip *, UInt16 toWhere = 0);
		//bool   DelEquip3(ItemEquip *);
        UInt8  GetPart(ItemEquip*);
		bool   EquipTo(UInt32 id, Fighter * fgt, UInt8 part, ItemEquip *&, bool = false);
		UInt32 SellItem(UInt32 id, UInt16 num, bool bind = false);
		UInt32 SellEquip(UInt32 id);
		bool   UseItem(UInt32 id, UInt16 num, UInt8 type, UInt32 param = 0, UInt8 bind = 0, UInt32 toId = 0);
		bool   UseItemOther(UInt32 id, UInt16 num, std::string& name, UInt8 bind = 0);
		bool   UseTaskItem(UInt32 id, UInt8 bind = 0);
		UInt16 Size() const  { return m_Size; }
		ItemBase*  GetItem(UInt32 id, bool bind = false);
		ItemEquip*  GetEquip(UInt32 id);
        inline UInt8 FormulaMerge(UInt32 id, UInt8 bind, UInt32 Mnum, UInt32 toId) { return FCMerge(id, bind, Mnum, toId); }
        inline UInt8 CittaMerge(UInt32 id, UInt8 bind, UInt32 Mnum, UInt32 toId) { return FCMerge(id, bind, Mnum, toId); }
        inline UInt8 TrumpMerge(UInt32 id, UInt8 bind, UInt32 Mnum, UInt32 toId) { return FCMerge(id, bind, Mnum, toId); }
        UInt8 FCMerge(UInt32 id, UInt8 bind, UInt32 Mnum, UInt32 toId);

		UInt16 GetItemNum(UInt32 id, bool bind = false);
		UInt16 GetItemAnyNum(UInt32 id);
		UInt32 GetItemTypeId(UInt32 id);	//equip & item
		bool ExistItem(UInt32 id);	//equip & item
	public:
		void DelItemSendMsg(UInt32 itemid, Player *player);//use for scripte send send msg to client
        UInt8 GetItemCareer(UInt32 itemid, UInt8 bind);

	public:
		static ItemBase* BuildItem(UInt32 id, UInt32 num, bool bind = false);
		static ItemBase* BuildEquipFromDB(UInt32 id, bool bind = false);
		void ItemNotify(UInt32 id, UInt16 num = 1);
		bool MoveEquipFromPack(ItemEquip *);
		void ItemNotifyEquip(ItemEquip*);

	public:
		UInt16 GetItemUsedGrids(UInt32 id, UInt16 num, bool bind = false);
		inline UInt16 GetUsedPackageSize(UInt8 type = 0) const
        {
            if(type == 0)
                return m_Size;
            else if(type == 1)
                return m_SizeSoul;
            else if(type == 2)
                return m_SizeLS;
            else if(type == 3)
                return m_SizeGem;
            else if(type == 4)
                return m_SizeFormula + m_SizeZY;
            else if(type == 5)
                return m_SizeSL;
            return 0;
        }
		inline UInt16 GetMaxPackageSize() const { return m_Owner->getPacksize(); }
		inline UInt16 GetRestPackageSize(UInt8 type = 0) const { return (m_Owner->getPacksize(type) > GetUsedPackageSize(type)) ? (m_Owner->getPacksize(type) - GetUsedPackageSize(type)) : 0; }
		UInt16 GetRestPackageSizeMin(UInt32 type = PACKAGE_0);
		inline bool IsFull() const { return m_Owner->getPacksize() <= m_Size; }

	public:
		void SendPackageItemInfor();
		void SendSingleEquipData(ItemEquip *);
		void SendDelEquipData(ItemEquip *);
		void SendItemData(ItemBase *);

	public:
		// Ìú½³
		UInt8 Enchant(UInt16 fighterId, UInt32 itemId, UInt8 type, UInt16 count, UInt8 level, UInt16& success, UInt16& failed,UInt16& bless/*, bool protect*/);
        void  enchantUdpLog(ItemEquip * equip, UInt8 level);
        void  OnFailEnchAttainment( UInt32 f);
		UInt8 OpenSocket(UInt16 fighterId, UInt32 itemId);
		UInt8 MergeGem(UInt32 gemId, UInt8 bindCount, bool protect, UInt32& ogid);
		UInt8 AttachGem(UInt16 fighterId, UInt32 itemId, UInt32 gemId, bool bind);
		UInt8 DetachGem(UInt16 fighterId, UInt32 itemId, UInt8 pos, UInt8 protect);
		UInt8 Split(UInt32 itemId, std::vector<SplitItemOut>& splitOut, /*bool protect,*/ bool silence = false);
		UInt8 SplitItem(UInt32 itemId, std::vector<SplitItemOut>& splitOut, /*bool protect,*/ bool silence = false);
		UInt8 Exchange(UInt32 * itemId, UInt32& resultId);
		UInt8 Forge(UInt16 fighterId, UInt32 itemId, /*UInt8 t,8*/ UInt8 * types, Int16 * values, UInt8 protect,UInt8 flag = 0);
		UInt8 ExchangeSet(UInt32 * itemId, UInt8 type, UInt32& resultId);
		// UInt8 BatchMergeGem(std::map<UInt16, UInt32, _GemCompare >& gems, std::vector<UInt32>& gemsOut);
		UInt8 BatchMergeGem(UInt16 gemId, UInt16 unbindCount, UInt16 bindCount, UInt8 protect, UInt16& gemIdOut, UInt16& unbindGemsOut, UInt16& bindGemsOut, UInt16& succTimes, UInt16& failedTimes);
		//UInt8 ActivateAttr(UInt16 fighterId, UInt32 itemId, UInt32 itemId2);
		UInt8 ActivateAttr(UInt16 fighterId, UInt32 itemId);
        UInt8 TrumpUpgrade(UInt16 fighterId, UInt32 trumpId, UInt32 itemId, UInt8 bind);
		UInt8 TrumpLOrder(UInt16 fighterId, UInt32 trumpId ,UInt8 opt = 0);
        UInt8 SmeltItemTrumpExp(Fighter * fgt, UInt32 itemId, bool& bind, UInt32& exp, UInt8 flag = 0);

		void AutoRegen(Fighter *);

		static void AppendEquipData(Stream&, ItemEquip *, bool = true);
		static void AppendItemData(Stream&, ItemBase *);
		void ApplyAttr2(ItemEquip *, UInt8 *, Int16 *);

		static UInt32 GetItemIdByName(const std::string&);
		static const GData::ItemBaseType * GetItemBaseType(UInt32);
		static const GData::ItemBaseType * GetItemBaseType(const std::string&);
        ItemEquip * FindEquip(Fighter *& fgt, UInt8& pos, UInt16 fgtId, UInt32 id);
        void FindEquipByTypeId(std::vector<ItemEquip*>& ret, UInt32 id, bool bind = true);
        void FindEquipByTypeIdFromItemTemp(std::vector<ItemEquip*>& ret, UInt32 id, bool bind = true);

        ItemEquip* AddUpgradeEquip(UInt32 typeId, UInt32 oldEquipId, bool notify, bool bind , ItemEquipData& ed , float*);
        const GData::ItemBaseType*  CheckBeforeEquipUpgrade(UInt32 typeId);
        UInt8 EquipUpgrade( UInt16 fighterId, UInt32 itemId , UInt32* pNewID, UInt16* pFgtId);

        //×°±¸ÊôÐÔ×ªÒÆ
        UInt8 EquipMove( UInt16 ffighterId, UInt16 tfighterId,UInt32 fromItemId, UInt32 toItemId, UInt8 type, UInt8 mark);
        UInt8 moveEquipEnchant(Fighter*,Fighter*, ItemEquip*, UInt8, ItemEquip*, UInt8, UInt8);
        UInt8 moveEquipGem(Fighter*,Fighter*, ItemEquip*, UInt8, ItemEquip*, UInt8, UInt8);
        UInt8 moveEquipSpirit(Fighter*,Fighter*, ItemEquip*, UInt8, ItemEquip*, UInt8, UInt8);
        UInt8 isCanMove(ItemEquip* fromEquip, ItemEquip* toEquip, UInt8 type, UInt8 mark);
        UInt8 moveUseMoney(ItemEquip* fromEquip, ItemEquip* toEquip, UInt8 type);
        UInt8 moveEquipFashion(Fighter*,Fighter*, ItemEquip*, UInt8, ItemEquip*, UInt8); 
        UInt8 moveEquipPurify(Fighter*,Fighter*, ItemEquip*, UInt8, ItemEquip*, UInt8); 
        UInt8 moveDeductMoney(ItemEquip* fromEquip, ItemEquip* toEquip, UInt8 type);
        UInt32 getPackageEquipCount();
        void setItemBind(UInt32 typeId);

	private:
		static UInt16 GetNewUsedGrids(const GData::ItemBaseType& type, UInt16 num);
		static UInt16 GetNewUsedGrids(UInt32 id, UInt16 num = 1);
		bool TryAddItem(ItemBase * item, UInt16 num);
		bool TryDelItem(ItemBase * item, UInt16 num);

    public:
        bool TryBuyItem(UInt32 typeId, UInt32 num, bool bind = false);
        bool TryBuyEquip(UInt32 typeId, UInt32 num, bool bind = false);

    private:

        void OnAddEquipAndCheckAttainment(const GData::ItemBaseType * itype, UInt16 FromWhere);
	public:
        void  AddItemHistoriesLog(UInt32 itemId, UInt32 num);
        void  AddItemCoursesLog(UInt32 typeId, UInt32 num, UInt16 fromWhere);

		ItemBase * FindItem(UInt32 id, bool bind = false)
		{
            item_elem_iter iter;
            if(GetItemSubClass(id) == Item_Soul)
            {
			    iter = m_ItemsSoul.find(ItemKey(id, bind));
                if(iter == m_ItemsSoul.end())
                    return NULL;
            }
            else if(GetItemSubClass(id) == Item_Gem || GetItemSubClass(id) == Item_EvolutionGem)
            {
			    iter = m_ItemsGem.find(ItemKey(id, bind));
                if(iter == m_ItemsGem.end())
                    return NULL;
            }
            else if(GetItemSubClass(id) == Item_Formula)
            {
			    iter = m_ItemsFormula.find(ItemKey(id, bind));
                if(iter == m_ItemsFormula.end())
                    return NULL;
            }
            else if(GetItemSubClass(id) == Item_SL)
            {
			    iter = m_ItemsSL.find(ItemKey(id, bind));
                if(iter == m_ItemsSL.end())
                    return NULL;
            }
            else
            {
			    iter = m_Items.find(ItemKey(id, bind));
                if(iter == m_Items.end())
                {
                    iter = m_ItemsZY.find(ItemKey(id, bind));
                    if(iter == m_ItemsZY.end())
                        return NULL;
                }
            }
			return iter->second;
		}

        void udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op);
        void cittaUdpLog(UInt8 type, UInt32 id, UInt32 num);
        void secondSoulItemUdpLog(UInt8 type, UInt32 id, UInt32 num);
        void qixiUdpLog(UInt32 id, UInt32 num);
        void gemMergeUdpLog(UInt32 num);
        void multiMergeUdpLog(UInt32 num);

        void AttachSpirit(UInt8 type, UInt16 fighterId, UInt32 itemId);
        static void GenSpirit(ItemEquip* equip);
        static void GenSpirit2(ItemEquip* equip);

        void enumerate(Visitor<ItemBase>& visitor);

        UInt8 Tongling(UInt32 equipId, UInt8 protect, UInt8 bind, std::vector<UInt16>& values);
        UInt8 OpenLingbaoSmelt(UInt16 gujiId, UInt8 type);
        UInt8 LingbaoSmelt(UInt32 itemId, UInt16 cnt);
        UInt8 closeLingbaoSmelt();
        void QuitLBSmelt();
        void sendLingbaoSmeltInfo();
        void loadLingbaoSmeltInfo(LingbaoSmeltInfo& info) { m_lbSmeltInfo = info; }
        void FinishLBSmelt();
        bool FinishLBSmeltSpecial(const GData::ItemBaseType * itype, ItemLingbaoAttr& lbattr, UInt8& attrNum);
        void testLingbao(UInt32 itemId, UInt32* colorNums, UInt8 size, UInt32* skills, UInt8 size2);
        void AddAdjVal(UInt8 AdjValMark, UInt8 nums);
        void Probability(float & disFactor);
    public:
        ItemBase * AddRetItemToPackage(UInt32 typeId, UInt32 num, bool bind, bool silence, UInt16 FromWhere);
        ItemBase * AddTempItemFromDB(TempItemData &);
        ItemBase * AddTempEquipFromDB(TempItemData &);
        UInt32 AddTemporaryItem(UInt32 itemId, UInt32 sellCount, bool bind);
        bool RetrieveTemporaryItem(UInt32 itemId, UInt32 sellCount, bool bind);
        bool CheckTemporaryItem();
        bool DelTempEquip(ItemEquip * equip, UInt16 toWhere, bool sendMark = false);
        bool DelTempItem(ItemBase* item, UInt32 num, UInt16 toWhere, bool sendMark = false);
        bool TryAddTempItem(ItemBase * item, UInt32 num);
        bool TryDelTempItem(ItemBase * item, UInt32 num);
        bool TryBuySoulItem(UInt32 typeId, UInt32 num, bool bind /*= false */);
        bool TryBuyGemItem(UInt32 typeId, UInt32 num, bool bind /*= false */);
        bool TryBuyFormulaItem(UInt32 typeId, UInt32 num, bool bind /*= false */);
        bool TryBuySLItem(UInt32 typeId, UInt32 num, bool bind /*= false */);
        void SendTempItemInfo();
        void SendSingleTempEquipData(ItemEquip * equip);
        void SendTempItemData(ItemBase * item);
        void SendDelTempEquipData(ItemEquip * equip);
        void AppendTempItemData(Stream& st, ItemBase * item);
        void AppendTempZhenyuanData(Stream& st, ItemZhenyuan * zhenyuan);
        void AppendTempEquipData(Stream& st, ItemEquip * equip, bool hascount = true);

        void SendSingleZhenyuanData(ItemZhenyuan *);
        static void AppendZhenyuanData(Stream&, ItemZhenyuan *);
	    ItemBase* AddZhenYuanN(UInt32 typeId, UInt32 num, bool bind, bool silence, UInt16 FromWhere = 0);
        ItemZhenyuan * newZhenyuanToDB(const GData::ItemBaseType *, ItemZhenyuanAttr&, bool);
	    ItemBase* AddZhenYuan(UInt32 typeId, bool bind, bool notify, UInt16 FromWhere = 0);
	    void MergeZhenyuan(UInt32* zhyIds, UInt8 count);
        /**********灵侍begin***********/
	    ItemLingshi* GetLingshi(UInt32 id);
	    ItemBase* AddLingShiN(UInt32 typeId, UInt32 num, bool bind, bool silence, UInt16 FromWhere = 0);
	    ItemBase* AddLingShi(UInt32 typeId, bool bind, bool notify, UInt16 FromWhere);
	    void lingshiUpgrade(UInt16 fighterId, UInt32 equipId, std::string& idStr);
        UInt32 lingshiUpgrade(ItemLingshi * equip, ItemLingshi * eatEq);
        void lingshiTrain(UInt16 fighterId, UInt32 lsId, bool type);
        void lingshiBreak(UInt16 fighterId, UInt32 lsId, bool type);
	    bool DelLingshi(UInt32 id, UInt16 toWhere = 0);
	    bool DelLingshi2(ItemLingshi * lingshi, UInt16 toWhere = 0);
        void SendLingshiTrainInfo();
        void SendSingleLingshiData(ItemLingshi *, UInt8);
	    static void AppendLingshiData(Stream&, ItemLingshi *);
	    void SendLSPackageItemInfor();
        void setLingshi(Fighter *, UInt32, UInt8);
        /**********灵侍end***********/
        UInt8 lingBaoUpLevel( UInt16 fighterId, UInt8 type, UInt16 count, UInt8 level, UInt16& success, UInt16& failed,UInt8& enLevel, UInt16& bless /*, bool protect*/ );
    protected:
		typedef std::map<ItemKey, ItemBase *> ItemCont;
		typedef ItemCont::iterator item_elem_iter;
		Player* m_Owner;

	private:
		enum {ItemClassSize = Item_Weapon + 1};

		ItemCont m_Items;
		ItemCont m_ItemsSoul;
		ItemCont m_ItemsLS;     //灵侍背包
		ItemCont m_ItemsGem; //宝石背包
		ItemCont m_ItemsFormula; //阵法背包
		ItemCont m_ItemsZY; //阵元背包
		ItemCont m_ItemsSL; //元神背包
        ItemCont m_ItemsTemporary; //临时物品
		UInt16 m_Size;		//already used grids
		UInt16 m_SizeSoul;  //already used soul grids
		UInt16 m_SizeLS;    //already used lingshi grids
		UInt16 m_SizeGem;  //宝石
		UInt16 m_SizeFormula;  //阵法
		UInt16 m_SizeZY;  //阵元
		UInt16 m_SizeSL;  //元神
        UInt16 m_TempItemSize;
		UInt8 _lastActivateLv;
		UInt8 _lastActivateQ;
		UInt8 _lastActivateCount;

        LingbaoSmeltInfo m_lbSmeltInfo; // 灵宝附灵值
	};

}

#endif
