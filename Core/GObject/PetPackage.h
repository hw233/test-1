#ifndef _PETPACKAGE_H_
#define _PETPACKAGE_H_


namespace GObject
{

	class PetPackage:public Package
	{
	public:
		PetPackage(Player* player);
		~PetPackage();
		void UnInit();

	private:
        //仙宠背包相关
        UInt16 m_ItemSize;
        UInt16 m_EquipSize;
		ItemCont m_PetItems;
		ItemCont m_PetEquips;
        static const UInt16 INIT_PETGEM_PACK_SIZE = 20;   //初始仙宠宝石背包大小
        static const UInt16 INIT_PETARM_PACK_SIZE = 100;  //初始仙宠装备背包大小
	public:
		inline UInt16 GetPetGemPgRestSize() const
        {
            return INIT_PETGEM_PACK_SIZE > m_ItemSize ? (INIT_PETGEM_PACK_SIZE - m_ItemSize) : 0;
        }
		inline UInt16 GetPetEqPgRestSize() const
        {
            return INIT_PETARM_PACK_SIZE > m_EquipSize ? (INIT_PETARM_PACK_SIZE - m_EquipSize) : 0;
        }
		inline ItemBase * FindPetItem(UInt32 id, bool bind = false)
		{
			item_elem_iter iter = m_PetItems.find(ItemKey(id, bind));
			if(iter == m_PetItems.end())
				return NULL;
			return iter->second;
		}
		inline ItemPetEq * FindPetEquip(UInt32 id, bool bind = false)
		{
            item_elem_iter iter = m_PetEquips.find(ItemKey(id, bind));
			if(iter == m_PetEquips.end())
				return NULL;
			return static_cast<ItemPetEq *>(iter->second);
		}

        ItemBase * AddItemFromDB(UInt32 id, UInt32 num, bool bind);
        ItemBase * AddExistEquip(ItemPetEq * equip, bool fromDB = false);
	    ItemPetEq * FindPetEquip( FairyPet *& pet, UInt8& pos, UInt16 petId, UInt32 id );
		ItemBase*  AddPetEquip(UInt32 typeId, bool bind = false, bool silence = false, UInt8 FromWhere = 0);
		ItemBase*  AddPetEquip(ItemEquip *, UInt8 FromWhere = 0);
		ItemBase*  AddPetEquipN(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt8 FromWhere = 0);
	    bool TryAddPetItem( ItemBase * item, UInt16 num );
        ItemBase* AddPetItem(UInt32 typeId, UInt32 num, bool bind = false, bool notify = false, UInt8 FromWhere = 0);
	    bool TryDelPetItem( ItemBase * item, UInt16 num );
	    bool DelPetItem(UInt32 id, UInt16 num, bool bind, UInt8 toWhere = 0);
	    UInt8 MergePetGem(UInt32 gemId1, UInt32 gemId2, UInt32& ogid);
	    UInt8 AttachPetGem(UInt32 petId, UInt32 itemId, UInt32 gemId);
	};

}

#endif
