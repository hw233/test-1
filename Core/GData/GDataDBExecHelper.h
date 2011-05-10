#ifndef _GDATADBEXECHELPER_H_
#define _GDATADBEXECHELPER_H_

#include "DB/DBExecHelper.h"

namespace GData
{

struct DBAreaDef
{
	UInt32 id;
	UInt8  side;
	std::string area;
	std::string rate;
};

//Weapon class definitions read from DB
struct DBWeaponDef
{
	UInt32 id;
	std::string name;
	UInt32 class_req;
	UInt32 area_effect;
	UInt8 can_counter;
};

struct DBItemType
{
	UInt32		typeId;
	std::string	name;
	UInt8		subClass;
	UInt32		coin;
	UInt16		reqLev;
	UInt8		quality;
	UInt16		maxQuantity;
	UInt8		bindType;
	UInt8		canDestroy;
	UInt16		value;
	UInt16		data;
	UInt32		attr1Extra;
	UInt32		attr2Extra;
};

struct DBEquipSetType
{
	UInt32 id;
	std::string name;
	UInt16 attrId[4];
};

struct DBAttr1Extra
{
	UInt32 id;
	std::string strength;
	std::string physique;
	std::string agility;
	std::string intelligence;
};

struct DBAttr2Extra
{
	UInt32 id;
	std::string attack;
	std::string defend;
	std::string hp;
	float action;
	float hitrate;
	float evade;
	float critical;
	float pierce;
	float counter;
};

struct DBFormation
{
	UInt32 id;
	std::string name;
	std::string eyes;
	std::string grids;
	UInt8 leastNum;
	UInt16 skillId;
};

struct DBClanSkillType
{
	UInt16 id;
	UInt8  level;
	UInt32 needs;
	UInt8  clanLev;
	UInt32 effect1;
	UInt32 effect2;
};


struct DBExp
{
	UInt8 lvl;
	UInt64 exp;
};

struct DBLootItem
{
	UInt32 id;
	std::string table;
	UInt8 isPack;
};

struct DBStoreItem
{
	UInt8 type;
	UInt16 itemId;
	UInt16 price;
};

}


namespace DB {

SPECIALBEGIN(GData::DBAreaDef)
SPECIALDEF(4)
	(
	UInt32,		id,
	UInt8,		side,
	std::string,area,
	std::string,rate
	)
SPECIALEND()

SPECIALBEGIN(GData::DBWeaponDef)
SPECIALDEF(5)
	(
	UInt32,		id,
	std::string,name,
	UInt32,		class_req,
	UInt32,		area_effect,
	UInt8,		can_counter
	)
SPECIALEND()

SPECIALBEGIN(GData::DBItemType)
SPECIALDEF(13)
	(
	UInt32,		typeId,
	std::string,name,
	UInt8,		subClass,
	UInt32,		coin,
	UInt16,		reqLev,
	UInt8,		quality,
	UInt16,		maxQuantity,
	UInt8,		bindType,
	UInt8,		canDestroy,
	UInt16,		value,
	UInt16,		data,
	UInt32,		attr1Extra,
	UInt32,		attr2Extra
	)
SPECIALEND()

SPECIALBEGIN(GData::DBEquipSetType)
SPECIALDEF(6)
	(
	UInt32,	id,
	std::string,name,
	UInt16,	attrId[0],
	UInt16,	attrId[1],
	UInt16,	attrId[2],
	UInt16,	attrId[3]
	)
SPECIALEND()

SPECIALBEGIN(GData::DBAttr1Extra)
SPECIALDEF(5)
	(
	UInt32, id,
	std::string, strength,
	std::string, physique,
	std::string, agility,
	std::string, intelligence
	)
SPECIALEND()

SPECIALBEGIN(GData::DBAttr2Extra)
SPECIALDEF(10)
	(
	UInt32, id,
	std::string, attack,
	std::string, defend,
	std::string, hp,
	float, action,
	float, hitrate,
	float, evade,
	float, critical,
	float, pierce,
	float, counter
	)
SPECIALEND()

SPECIALBEGIN(GData::DBFormation)
SPECIALDEF(6)
	(
	UInt32, id,
	std::string, name,
	std::string, eyes,
	std::string, grids,
	UInt8, leastNum,
	UInt16, skillId
	)
SPECIALEND()

SPECIALBEGIN(GData::DBExp)
SPECIALDEF(2)
	(
	UInt8,			lvl,
	UInt64,			exp
	)
SPECIALEND()

SPECIALBEGIN(GData::DBLootItem)
SPECIALDEF(3)
	(
	UInt32, id,
	std::string, table,
	UInt8, isPack
	)
SPECIALEND()

SPECIALBEGIN(GData::DBStoreItem)
SPECIALDEF(3)
	(
	UInt8, type,
	UInt16, itemId,
	UInt16, price
	)
SPECIALEND()


SPECIALBEGIN(GData::DBClanSkillType)
SPECIALDEF(6)
	(
	UInt16, id,
	UInt8,  level,
	UInt32, needs,
	UInt8, clanLev,
	UInt32, effect1,
	UInt32, effect2
	)
SPECIALEND()

}


#endif // _GDATADBEXECHELPER_H_
