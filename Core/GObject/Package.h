#ifndef _PACKAGE_H_
#define _PACKAGE_H_

#include "Item.h"
#include "GData/GDataManager.h"
#include "Common/URandom.h"

namespace GObject
{
#define ITEM_ENCHANT_L1 502   //太乙真金
#define ITEM_ENCHANT_L2 503   //太乙精金
#define TRUMP_ENCHANT_L1 514   //五行精英
#define TRUMP_ENCHANT_L2 515  //五行精华

	class Fighter;
	class Player;

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

	class Package
	{
		typedef std::map<ItemKey, ItemBase *> ItemCont;
		typedef ItemCont::iterator item_elem_iter;
	public:
		Package(Player* player);
		~Package();

	public:
		bool Init();
		void UnInit();

	public:
		ItemBase*  AddItem(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt8 fromWhere = 0);
		ItemBase*  AddItem2(ItemBase* item, UInt8 fromWhere = 0);
		ItemBase*  AddItem2(UInt32 typeId, UInt32 num, bool notify, bool bind = false, UInt8 fromWhere = 0);
		ItemBase*  AddItemFromDB(UInt32 id, UInt32 num, bool bind);
		ItemBase * AddExistEquip(ItemEquip *);
		ItemBase*  AddEquip(UInt32 typeId, bool bind = false, bool silence = false, UInt8 FromWhere = 0);
		ItemBase*  AddEquip2(ItemEquip *, UInt8 FromWhere = 0);
		ItemBase*  AddEquip2(UInt32 typeId, bool notify, bool bind = false, UInt8 FromWhere = 0);
		ItemBase*  AddEquipN(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt8 FromWhere = 0);
		ItemBase*  AddEquipFromDB(UInt32 id, bool bind);
		ItemBase*  Add(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt8 FromWhere = 0);
		ItemBase*  SetItem(UInt32 typeId, UInt32 num, bool bind = false);
		bool   DelItemAll(UInt32 id, bool bind = false);
		bool   DelItem(UInt32 id, UInt16 num, bool bind = false);
		bool   DelItem2(ItemBase* item, UInt16 num);
		bool   DelItemAny(UInt32 id, UInt16 num, bool * = NULL);
		bool   DelEquip(UInt32 id, UInt8 toWhere = 0);
		bool   DelEquip2(ItemEquip *, UInt8 toWhere = 0);
		bool   DelEquip3(ItemEquip *);
		bool   EquipTo(UInt32 id, Fighter * fgt, UInt8 part, ItemEquip *&, bool = false);
		UInt32 SellItem(UInt32 id, UInt16 num, bool bind = false);
		UInt32 SellEquip(UInt32 id);
		bool   UseItem(UInt32 id, UInt16 num, UInt32 param = 0, UInt8 bind = 0);
		bool   UseTaskItem(UInt32 id, UInt8 bind = 0);
		UInt16 Size() const  { return m_Size; }
		ItemBase*  GetItem(UInt32 id, bool bind = false);
		ItemEquip*  GetEquip(UInt32 id);
        inline bool FormulaMerge(UInt32 id, bool bind) { return FCMerge(id, bind); }
        inline bool CittaMerge(UInt32 id, bool bind) { return FCMerge(id, bind); }
        inline bool TrumpMerge(UInt32 id, bool bind) { return FCMerge(id, bind); } 
        bool FCMerge(UInt32 id, bool bind);

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

	public:
		UInt16 GetItemUsedGrids(UInt32 id, UInt16 num, bool bind = false);
		inline UInt16 GetUsedPackageSize() const { return m_Size; }
		inline UInt16 GetMaxPackageSize() const { return m_Owner->getPacksize(); }
		inline UInt16 GetRestPackageSize() const { return (m_Owner->getPacksize() > m_Size) ? (m_Owner->getPacksize() - m_Size) : 0; }
		inline bool IsFull() const { return m_Owner->getPacksize() <= m_Size; }

	public:
		void SendPackageItemInfor();
		void SendSingleEquipData(ItemEquip *);
		void SendDelEquipData(ItemEquip *);
		void SendItemData(ItemBase *);

	public:
		// 铁匠
		UInt8 Enchant(UInt16 fighterId, UInt32 itemId, UInt8 type, UInt16 count, UInt8 level, UInt16& success, UInt16& failed/*, bool protect*/);
		UInt8 OpenSocket(UInt16 fighterId, UInt32 itemId);
		UInt8 MergeGem(UInt32 gemId, UInt8 bindCount, bool protect, UInt32& ogid);
		UInt8 AttachGem(UInt16 fighterId, UInt32 itemId, UInt32 gemId, bool bind);
		UInt8 DetachGem(UInt16 fighterId, UInt32 itemId, UInt8 pos, UInt8 protect);
		UInt8 Split(UInt32 itemId, UInt32& enchantId, UInt8& count, /*bool protect,*/ bool silence = false);
		UInt8 Exchange(UInt32 * itemId, UInt32& resultId);
		UInt8 Forge(UInt16 fighterId, UInt32 itemId, /*UInt8 t,8*/ UInt8 * types, Int16 * values, UInt8 protect);
		UInt8 ExchangeSet(UInt32 * itemId, UInt8 type, UInt32& resultId);
		// UInt8 BatchMergeGem(std::map<UInt16, UInt32, _GemCompare >& gems, std::vector<UInt32>& gemsOut);
		UInt8 BatchMergeGem(UInt16 gemId, UInt16 unbindCount, UInt16 bindCount, UInt8 protect, UInt16& gemIdOut, UInt16& unbindGemsOut, UInt16& bindGemsOut, UInt16& succTimes, UInt16& failedTimes);
		UInt8 ActivateAttr(UInt16 fighterId, UInt32 itemId, UInt32 itemId2);

		void AutoRegen(Fighter *);

		static void AppendEquipData(Stream&, ItemEquip *, bool = true);
		static void AppendItemData(Stream&, ItemBase *);
		void ApplyAttr2(ItemEquip *, UInt8 *, Int16 *);

		static UInt32 GetItemIdByName(const std::string&);
		static const GData::ItemBaseType * GetItemBaseType(UInt32);
		static const GData::ItemBaseType * GetItemBaseType(const std::string&);
		ItemEquip * FindEquip(Fighter *& fgt, UInt8& pos, UInt16 fgtId, UInt32 id);

	private:
		static UInt16 GetNewUsedGrids(const GData::ItemBaseType& type, UInt16 num);
		static UInt16 GetNewUsedGrids(UInt32 id, UInt16 num = 1);
		bool TryAddItem(ItemBase * item, UInt16 num);
		bool TryDelItem(ItemBase * item, UInt16 num);

	public:
		inline ItemBase * FindItem(UInt32 id, bool bind = false)
		{
			item_elem_iter iter = m_Items.find(ItemKey(id, bind));
			if(iter == m_Items.end())
				return NULL;
			return iter->second;
		}

        void udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op);

	private:
		enum {ItemClassSize = Item_Weapon + 1};

		ItemCont m_Items;
		Player* m_Owner;
		UInt16 m_Size;		//already used grids
		UInt8 _lastActivateLv;
		UInt8 _lastActivateQ;
		UInt8 _lastActivateCount;
	};

}

#endif
