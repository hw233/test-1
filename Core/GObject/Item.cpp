#include "Config.h"
#include "Item.h"
#include "Server/WorldServer.h"

namespace GObject
{

void ItemEquip::DoEquipBind(bool checkType)
{
	if(!m_BindStatus && (!checkType || _itemBaseType->bindType > 0))
	{
		m_BindStatus = true;
		DB4().PushUpdateData("UPDATE `item` SET `bindType` = 1 WHERE `id` = %u", _id);
	}
}

}
