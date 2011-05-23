#include "Config.h"
#include "Fighter.h"
#include "Country.h"
#include "TaskMgr.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "Package.h"
#include "GData/ExpTable.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "Common/TimeUtil.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Script/BattleFormula.h"

namespace GObject
{

GlobalFighters globalFighters;

Fighter& getGreatFighter(UInt32 id)
{
	static Fighter null(0, NULL);
	if(id > GREAT_FIGHTER_MAX)
		return null;
	Fighter * fgt = globalFighters[id];
	if(fgt)
		return *fgt;
	return null;
}

bool existGreatFighter(UInt32 id)
{
	if(id > GREAT_FIGHTER_MAX)
		return false;
	return globalFighters[id] != NULL;
}

Fighter::Fighter(UInt32 id, Player * owner):
	_id(id), _owner(owner), _class(0), _level(1), _exp(0), _pexp(0), _potential(1.0f),_capacity(1.0f),
	_color(2), _hp(0), _peerless(0), _weapon(NULL), _ring(NULL), _amulet(NULL),
	_attrDirty(false), _maxHP(0), _bPDirty(false), _battlePoint(0.0f),
	favor(0), reqFriendliness(0), strength(0), physique(0), agility(0), intelligence(0),
	attack(0), defend(0), maxhp(0), action(0), hitrate(0), evade(0), critical(0), pierce(0), counter(0)
{
    memset(_bloodbit, 0, sizeof(_bloodbit));
    memset(_skill, 0, sizeof(_skill));
    //_skills.resize(32); // 默认为32个
    memset(_citta, 0, sizeof(_citta));
    //_cittas.resize(32); // 默认为32个
	memset(_armor, 0, 5 * sizeof(ItemEquip *));
	memset(_trump, 0, sizeof(_trump));
	memset(_buffData, 0, FIGHTER_BUFF_COUNT * sizeof(UInt32));
}

Fighter::~Fighter()
{
	SAFE_DELETE(_weapon);
	SAFE_DELETE(_ring);
	SAFE_DELETE(_amulet);
	for(int i = 0; i < 5; ++ i)
	{
		SAFE_DELETE(_armor[i]);
	}
}

const std::string& Fighter::getName()
{
	if(_id < 7 && _owner != NULL)
		return _owner->getName();
	else
		return _name;
}

const std::string& Fighter::getBattleName()
{
	if(_id < 7 && _owner != NULL)
		return _owner->getBattleName();
	else
		return _name;
}

UInt16 Fighter::getWeaponAttack()
{
	static UInt16 atk_enc[] = {0, 10, 20, 40, 60, 100, 200, 300, 450, 600, 800, 1050, 1500};
	return _weapon ? (_weapon->getAttack() + atk_enc[_weapon->getItemEquipData().enchant]) : 0;
}

void Fighter::getArmorDefendAndHP(UInt16& def, UInt16& hp)
{
	def = 0;
	hp = 0;
	for(int i = 0; i < 5; ++ i)
	{
		static UInt16 def_enc[] = {0, 15, 25, 35, 45, 60, 90, 120, 150, 200, 300, 450, 700};
		static UInt16 hp_enc[] = {0, 0, 0, 0, 0, 0, 0, 0, 200, 300, 600, 900, 1200};
		if(_armor[i] != NULL)
		{
			UInt8 enc = _armor[i]->getItemEquipData().enchant;
			def += _armor[i]->getDefend() + def_enc[enc];
			hp += hp_enc[enc];
		}
	}
}

UInt8 Fighter::getColor2( float pot )
{
	if(pot < 0.299f)
		return 5;
	if(pot < 0.699f)
		return 0;
	if(pot < 0.899f)
		return 1;
	if(pot < 1.199f)
		return 2;
	if(pot < 1.399f)
		return 3;
	if(pot < 1.599f)
		return 4;
	return 5;
}

UInt32 Fighter::getWeaponId()
{
	return _weapon ? _weapon->getId() : 0;
}

UInt32 Fighter::getArmorId( int idx )
{
	if(idx >= 0 && idx < 5)
	{
		if(_armor[idx] != NULL)
			return _armor[idx]->getId();
	}
	return 0;
}

UInt32 Fighter::getAmuletId()
{
	return _amulet ? _amulet->getId() : 0;
}

UInt32 Fighter::getRingId()
{
	return _ring ? _ring->getId() : 0;
}

bool Fighter::addExp( UInt64 e )
{
	if(e == 0 || _level >= LEVEL_MAX)
		return false;
	if(e > 0xFFFFFFFFFFFFFFFFull - _exp)
		_exp = 0xFFFFFFFFFFFFFFFFull;
	else
		_exp += e;
	if(_owner != NULL)
	{
		UInt64 mexp = _owner->GetExp();
		if(mexp < _exp)
			_exp = mexp;
	}
	UInt8 oldLevel = _level;
	bool r = GData::expTable.testLevelUp(_level, _exp);
	bool isMain = _owner->isMainFighter(_id);
	if(isMain)
	{
		SYSMSG_SENDV(100, _owner, e);
	}
	SYSMSG_SENDV(1000, _owner, _color, getName().c_str(), e);
	if(r)
	{
		_bPDirty = true;
		_hp = 0;
		UInt8 t[3] = {1, 2, 3};
		UInt64 v[3] = {0, _level, _exp};
		sendModification(3, t, v);
		SYSMSG_SENDV(1001, _owner, _color, getName().c_str(), _level);
		if (_owner != NULL && isMain)
		{
			SYSMSG_SENDV(101, _owner, _level);
			_owner->checkLevUp(oldLevel, _level);
		}
	}
	else
	{
		sendModification(3, _exp);
	}
	return r;
}

void Fighter::setLevelAndExp( UInt8 l, UInt64 e )
{
	if(_level != l)
	{
		UInt8 oldLevel = _level;
		_level = l;
		_exp = e;
		_hp = 0;
		UInt8 t[3] = {1, 2, 3};
		UInt64 v[3] = {0, _level, _exp};
		sendModification(3, t, v);
		SYSMSG_SENDV(1001, _owner, _color, getName().c_str(), _level);
		if (_owner != NULL && _owner->isMainFighter(_id))
		{
			SYSMSG_SENDV(101, _owner, _level);
			_owner->checkLevUp(oldLevel, _level);
		}
	}
	else if(_exp != e)
	{
		_exp = e;
		sendModification(3, _exp);
	}
}

void Fighter::updateToDB( UInt8 t, UInt64 v )
{
	const char * field = NULL;
	if(t >= 0x40 && t < 0x40 + FIGHTER_BUFF_COUNT)
	{
		if(_owner == NULL)
			return;
		if(v > 0)
			DB().PushUpdateData("REPLACE INTO `fighter_buff`(`playerId`, `id`, `buffId`, `data`) VALUES(%"I64_FMT"u, %u, %u, %u)", _owner->getId(), _id, t - 0x40, v);
		else
			DB().PushUpdateData("DELETE FROM `fighter_buff` WHERE `playerId` = %"I64_FMT"u AND `id` = %u AND `buffId` = %u", _owner->getId(), _id, t - 0x40);
		return;
	}
    if (t >= 0x50 && t < 0x50 + TRUMP_UPMAX)
    {
        UInt32 trumps[TRUMP_UPMAX] = {0};
        if (getAllTrumpId(trumps)) {
            std::string str;
            if (value2string(trumps, TRUMP_UPMAX, str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `trump` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
    }

	switch(t)
	{
	case 1: field = "hp"; break;
	case 2: field = "level"; break;
	case 3: DB().PushUpdateData("UPDATE `fighter` SET `experience` = %"I64_FMT"u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v, _id, _owner->getId());
		return;
	case 4:
		if(_id <= GREAT_FIGHTER_MAX && _owner != NULL)
			DB().PushUpdateData("UPDATE `fighter` SET `potential` = %u.%02u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v / 100, v % 100, _id, _owner->getId());
		return;
    case 5:
		if(_id <= GREAT_FIGHTER_MAX && _owner != NULL)
			DB().PushUpdateData("UPDATE `fighter` SET `capacity` = %u.%02u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v / 100, v % 100, _id, _owner->getId());
        return;

    case 0x29:
        {
            std::string str;
            if (value2string(_bloodbit, BLOODBIT_MAX, str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `bloodbit` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x60:
        { // skill
            std::string str;
            if (value2string(_skill, getUpSkillsNum(), str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `skill` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x61:
        { // skills
            std::string str;
            if (value2string(&_skills[0], _skills.size(), str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `skills` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x62:
        { // citta
            std::string str;
            if (value2string(_citta, getUpCittasNum(), str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `citta` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x63:
        { // cittas 
            std::string str;
            if (value2string(&_cittas[0], _cittas.size(), str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `cittas` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;

	case 0x21: field = "weapon"; break;
	case 0x22: field = "armor1"; break;
	case 0x23: field = "armor2"; break;
	case 0x24: field = "armor3"; break;
	case 0x25: field = "armor4"; break;
	case 0x26: field = "armor5"; break;
	case 0x27: field = "amulet"; break;
	case 0x28: field = "ring"; break;
	}
	if(field != NULL)
	{
		if(_id <= GREAT_FIGHTER_MAX && _owner != NULL)
			DB().PushUpdateData("UPDATE `fighter` SET `%s` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", field, v, _id, _owner->getId());
	}
}

void Fighter::sendModification( UInt8 t, UInt64 v )
{
	sendModification(1, &t, &v);
}

void Fighter::sendModificationBloodBit( UInt8 t, int idx, bool writedb )
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << _bloodbit[idx];
    if (writedb)
    {
        updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}

#if 1
void Fighter::sendModification( UInt8 t, UInt16 value, int idx, bool writedb)
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << value;
    if (writedb)
    {
        updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}
#else
void Fighter::sendModificationUpSkill( UInt8 t, UInt16 skill, int idx, bool writedb)
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << skill;
    if (writedb)
    {
        updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}

void Fighter::sendModificationSkills( UInt8 t, UInt16 skill, int idx, bool writedb )
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << skill;
    if (writedb)
    {
        updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}

void Fighter::sendModificationUpCitta( UInt8 t, UInt16 citta, int idx, bool writedb )
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << citta;
    if (writedb)
    {
        updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}

void Fighter::sendModificationCittas( UInt8 t, UInt16 citta, int idx, bool writedb )
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << citta;
    if (writedb)
    {
        updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}
#endif

void Fighter::sendModification( UInt8 n, UInt8 * t, UInt64 * v )
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << n;
	for(UInt8 i = 0; i < n; ++ i)
	{
		st << t[i];
		if(t[i] >= 0x40)
		{
			st << static_cast<UInt32>(v[i] - TimeUtil::Now());
		}
		else if(t[i] == 0x03)
		{
			st << v[i];
		}
		else
		{
			st << static_cast<UInt32>(v[i]);
		}
		updateToDB(t[i], v[i]);
	}
	st << Stream::eos;
	_owner->send(st);
}

void Fighter::sendModification( UInt8 t, ItemEquip * v, bool writedb )
{
	sendModification(1, &t, &v, writedb);
}

void Fighter::sendModification( UInt8 n, UInt8 * t, ItemEquip ** v, bool writedb )
{
	if(_owner == NULL)
		return;
	Stream st(0x21);
	st << getId() << n;
	for(UInt8 i = 0; i < n; ++ i)
	{
		ItemEquip * equip = v[i];
		st << t[i];
		if(equip == NULL)
		{
			st << static_cast<UInt32>(0);
			if(writedb)
				updateToDB(t[i], 0);
		}
		else
		{
			st << equip->getId() << static_cast<UInt8>(equip->GetBindStatus() ? 1 : 0)
				<< static_cast<UInt16>(equip->GetItemType().getId()) << equip->getItemEquipData().enchant;
			ItemEquipData& ied = equip->getItemEquipData();
			st << ied.sockets;
			for(UInt8 z = 0; z < ied.sockets; ++ z)
			{
				st << ied.gems[z];
			}
			ItemEquipAttr2& ea2 = equip->getEquipAttr2();
			ea2.appendAttrToStream(st);
			if(writedb)
				updateToDB(t[i], equip->getId());
		}
	}
	st << Stream::eos;
	_owner->send(st);
}

ItemWeapon * Fighter::setWeapon( ItemWeapon * w, bool writedb )
{
	ItemWeapon * r = _weapon;
	_weapon = w;
	if(writedb)
	{
		_attrDirty = true;
		_bPDirty = true;
		if(w != NULL)
		{
			w->DoEquipBind(true);
		}
		sendModification(0x21, w);
	}
	return r;
}

ItemArmor * Fighter::setArmor( int idx, ItemArmor * a, bool writedb )
{
	ItemArmor * r = _armor[idx];
	_armor[idx] = a;
	if(writedb)
	{
		_attrDirty = true;
		_bPDirty = true;
		if(a != NULL)
		{
			a->DoEquipBind(true);
		}
		sendModification(0x22 + idx, a);
	}
	return r;
}

ItemEquip * Fighter::setAmulet( ItemEquip * a, bool writedb )
{
	ItemEquip * r = _amulet;
	_amulet = a;
	if(writedb)
	{
		_attrDirty = true;
		_bPDirty = true;
		if(a != NULL)
		{
			a->DoEquipBind(true);
		}
		sendModification(0x27, a);
	}
	return r;
}

ItemEquip * Fighter::setRing( ItemEquip * r, bool writedb )
{
	ItemEquip * rr = _ring;
	_ring = r;
	if(writedb)
	{
		_attrDirty = true;
		_bPDirty = true;
		if(r != NULL)
		{
			r->DoEquipBind(true);
		}
		sendModification(0x28, r);
	}
	return rr;
}

ItemEquip ** Fighter::setTrump( std::string& trumps, bool writedb )
{
    if (!trumps.length())
        return 0;

    StringTokenizer tk(trumps, "|");
    for (size_t i = 0; i < tk.count() && i < TRUMP_UPMAX; ++i)
    {
        setTrump(::atoi(tk[i].c_str()), i, writedb);
    }

    return &_trump[0];
}

ItemEquip* Fighter::setTrump( UInt32 trump, int idx, bool writedb )
{
    ItemEquip* t = 0;
    t = GObjectManager::fetchEquipment(trump);
    return setTrump(t, idx, writedb);
}

ItemEquip* Fighter::setTrump( ItemEquip* trump, int idx, bool writedb )
{
    ItemEquip* t = 0;
    if (idx >= 0 && idx < TRUMP_UPMAX)
    {
        if
            (
                (!_trump[idx] && trump) ||
                (
                    _trump[idx] &&
                    (
                        (trump && _trump[idx]->getId() != trump->getId()) ||
                        !trump
                    )
                )
            )
        {
            _attrDirty = true;
            _bPDirty = true;

            t = _trump[idx];
            _trump[idx] = trump;

            if (trump)
                trump->DoEquipBind(true);

            sendModification(0x50+idx, _trump[idx], writedb);
        }
    }
    return t;
}

int Fighter::getAllTrumpId( UInt32* trumps, int size )
{
    if (!trumps || !size)
        return 0;

    for (int i = 0; i < TRUMP_UPMAX; ++i)
    {
        if (_trump[i])
            trumps[i] = _trump[i]->getId();
        else
            trumps[i] = 0;
    }
    return TRUMP_UPMAX;
}

void Fighter::setCurrentHP( UInt16 hp, bool writedb )
{
	if(_hp == hp)
		return;

	if(writedb)
	{
		UInt32 maxhp = getMaxHP();
		if(hp >= maxhp)
		{
			if(_hp == 0)
				return;
			hp = 0;
		}
		_hp = hp;
		sendModification(1, hp);
	}
	else
		_hp = hp;
}

UInt32 Fighter::regenHP( UInt32 hp )
{
	if(_hp == 0)
		return 0;
	setCurrentHP(hp + _hp);
	return _hp;
}

void Fighter::setBuffData( UInt8 id, UInt32 data, bool writedb )
{
	if(id >= FIGHTER_BUFF_COUNT || _buffData[id] == data)
		return;
	_buffData[id] = data;
	if(writedb)
		sendModification(0x40 + id, data);
}

UInt32 Fighter::getBuffData( UInt8 idx, UInt32 now )
{
	if(idx > FIGHTER_BUFF_COUNT)
		return 0;
	if(_buffData[idx] > 0 && _buffData[idx] <= now)
	{
		_buffData[idx] = 0;
		updateToDB(0x40 + idx, 0);
		return 0;
	}
	return _buffData[idx];
}

UInt32 Fighter::getMaxHP()
{
	checkDirty();
	return _maxHP;
}

void Fighter::setPotential( float p, bool writedb )
{
	_potential = p;
	_color = getColor2(p);
	if(_owner != NULL)
	{
		if(writedb)
		{
			_attrDirty = true;
			_bPDirty = true;
			sendModification(4, static_cast<UInt32>(p * 100 + 0.5f));
		}
	}
}

void Fighter::setCapacity( float c, bool writedb )
{
    _capacity = c;
    if (writedb && _owner) {
        _attrDirty = true;
        _bPDirty = true;
        sendModification(5, static_cast<UInt32>(c * 100 + 0.5f));
    }
}

inline void addAttr1Extra( GData::Attr1Extra& ae, const GData::Attr1Extra * ext )
{
	if(ext == NULL)
		return;
	ae += *ext;
}

inline void addAttr2Extra( GData::Attr2Extra& ae, const GData::Attr2Extra * ext )
{
	if(ext == NULL)
		return;
	ae += *ext;
}

inline void addEquipAttr2( GData::Attr2Extra& ae, UInt8 type, UInt16 value )
{
	switch(type)
	{
	case 1:
		ae.hp += value;
		break;
	case 2:
		ae.action += static_cast<float>(value) / 10000;
		break;
	case 3:
		ae.hitrate += static_cast<float>(value) / 100;
		break;
	case 4:
		ae.evade += static_cast<float>(value) / 100;
		break;
	case 5:
		ae.critical += static_cast<float>(value) / 100;
		break;
	case 6:
		ae.pierce += static_cast<float>(value) / 100;
		break;
	case 7:
		ae.counter += static_cast<float>(value) / 100;
		break;
	}
}

inline void addEquipAttr2( GData::Attr2Extra& ae, const ItemEquipAttr2& ext )
{
	if(ext.type1 != 0)
	{
		addEquipAttr2(ae, ext.type1, ext.value1);
	}
	if(ext.type2 != 0)
	{
		addEquipAttr2(ae, ext.type2, ext.value2);
	}
	if(ext.type3 != 0)
	{
		addEquipAttr2(ae, ext.type3, ext.value3);
	}
}

inline void testEquipInSet(UInt32 * setId, UInt32 * setNum, UInt32 id)
{
	id /= 20;
	if(id == 0)
		return;
	for(int i = 0; i < 8; ++ i)
	{
		if(setId[i] == 0)
		{
			setId[i] = id;
			setNum[i] = 1;
			return;
		}
		if(setId[i] == id)
		{
			++ setNum[i];
			return;
		}
	}
}

void Fighter::addAttr( ItemEquip * equip )
{
	addAttr1Extra(_attr1ExtraEquip, equip->getAttr1Extra());
	addEquipAttr2(_attr2ExtraEquip, equip->getEquipAttr2());
	ItemEquipData& ied = equip->getItemEquipData();
	for(UInt8 i = 0; i < ied.sockets; ++ i)
	{
		if(ied.gems[i] != 0)
		{
			GData::ItemGemType * igt = GData::gemTypes[ied.gems[i] - 5000];
			addAttr1Extra(_attr1ExtraEquip, igt->attr1Extra);
			addAttr2Extra(_attr2ExtraEquip, igt->attr2Extra);
		}
	}
}

void Fighter::rebuildEquipAttr()
{
	_attr1ExtraEquip.reset();
	_attr2ExtraEquip.reset();

	UInt32 setId[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	UInt32 setNum[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	ItemEquip * equip = getWeapon();
	if(equip != NULL)
	{
		if(equip->getQuality() >= 4)
			testEquipInSet(setId, setNum, equip->GetItemType().getId());
		addAttr(equip);
	}

	for(int i = 0; i < 5; ++ i)
	{
		equip = getArmor(i);
		if(equip != NULL)
		{
			if(equip->getQuality() >= 4)
				testEquipInSet(setId, setNum, equip->GetItemType().getId());
			addAttr(equip);
		}
	}

	equip = getRing();
	if(equip != NULL)
	{
		if(equip->getQuality() >= 4)
			testEquipInSet(setId, setNum, equip->GetItemType().getId());
		addAttr(equip);
	}

	equip = getAmulet();
	if(equip != NULL)
	{
		if(equip->getQuality() >= 4)
			testEquipInSet(setId, setNum, equip->GetItemType().getId());
		addAttr(equip);
	}

	for(int i = 0; i < 8; ++ i)
	{
		if(setId[i] == 0)
			break;
		if(setNum[i] < 2)
			continue;
		const GData::ItemEquipSetType * iest = GData::itemEquipSetTypeManager[setId[i]];
		if(iest == NULL)
			continue;
		UInt32 idx = setNum[i] / 2 - 1;
		_attr1ExtraEquip += *iest->attr1Extra[idx];
		_attr2ExtraEquip += *iest->attr2Extra[idx];
	}
	_attr2ExtraEquip.attack += getWeaponAttack();
	UInt16 armorDefend, armorHP;
	getArmorDefendAndHP(armorDefend, armorHP);
	_attr2ExtraEquip.defend += armorDefend;
	_attr2ExtraEquip.hp += armorHP;

	_maxHP = Script::BattleFormula::getCurrent()->calcHP(this);
}

void Fighter::rebuildBattlePoint()
{
	_battlePoint = Script::BattleFormula::getCurrent()->calcBattlePoint(this);
}

Fighter * Fighter::clone(Player * player)
{
	Fighter * fgt = new Fighter(*this);
	if(player != NULL)
	{
		fgt->_level = 1;
		fgt->_exp = 0;
	}
	fgt->_owner = player;
	fgt->_weapon = NULL;
	fgt->_ring = NULL;
	fgt->_amulet = NULL;
	fgt->_attrDirty = true;
	fgt->_bPDirty = true;
	memset(fgt->_armor, 0, 5 * sizeof(ItemEquip *));
	return fgt;
}

ItemEquip * Fighter::findEquip( UInt32 id, UInt8& pos )
{
	pos = 0;
	if(_weapon != NULL && _weapon->getId() == id)
	{
		return _weapon;
	}
	for(int i = 0; i < 5; ++ i)
	{
		if(_armor[i] != NULL && _armor[i]->getId() == id)
		{
			pos = i + 1;
			return _armor[i];
		}
	}
	if(_amulet != NULL && _amulet->getId() == id)
	{
		pos = 6;
		return _amulet;
	}
	if(_ring != NULL && _ring->getId() == id)
	{
		pos = 7;
		return _ring;
	}
	return NULL;
}

void Fighter::removeEquip( UInt8 pos, ItemEquip * equip, UInt8 toWhere )
{
	bool found = false;
	switch(pos)
	{
	case 0:
		if(_weapon == equip)
		{
			_weapon = NULL;
			sendModification(0x21, NULL, true);
			found = true;
		}
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		{
			UInt8 i = pos - 1;
			if(_armor[i] == equip)
			{
				_armor[i] = NULL;
				sendModification(0x22 + i, NULL, true);
				found = true;
			}
		}
		break;
	case 6:
		if(_amulet == equip)
		{
			_amulet = NULL;
			sendModification(0x28, NULL, true);
			found = true;
		}
		break;
	case 7:
		if(_ring == equip)
		{
			_ring = NULL;
			sendModification(0x27, NULL, true);
			found = true;
		}
		break;
	}
	if(found)
	{
		_attrDirty = true;
		_bPDirty = true;
		DB().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		DB().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", equip->getId());
		if(toWhere != 0 && equip->getQuality() >= 4)
		{
			DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, _owner->getId(), equip->GetItemType().getId(), equip->getId(), toWhere, TimeUtil::Now());
		}
		SAFE_DELETE(equip);
	}
}

Fighter * Fighter::cloneWithEquip(Player * player)
{
	Fighter * fgt = new Fighter(*this);
	fgt->_owner = player;
	if(_weapon != NULL)
		fgt->_weapon = new ItemWeapon(*_weapon);
	else
		fgt->_weapon = NULL;
	if(_ring != NULL)
		fgt->_ring = new ItemEquip(*_ring);
	else
		fgt->_ring = NULL;
	if(_amulet != NULL)
		fgt->_amulet = new ItemEquip(*_amulet);
	else
		fgt->_amulet = NULL;
	for(int i = 0; i < 5; ++ i)
	{
		if(_armor[i] != NULL)
			fgt->_armor[i] = new ItemArmor(*_armor[i]);
		else
			fgt->_armor[i] = NULL;
	}
	fgt->_attrDirty = true;
	return fgt;
}

#if 0
void Fighter::setSkill( UInt16 skill, bool writedb /*= true*/ )
{
	if(_skill == skill)
		return;
	_skill = skill;
	if(writedb)
	{
		sendModification(0x11, _skill * 100 + _skillLevel);
	}
}

void Fighter::setSkillLevel( UInt8 level, bool writedb /*= true*/ )
{
	if(_skillLevel == level)
		return;
	_skillLevel = level;
	if(writedb)
	{
		sendModification(0x11, _skill * 100 + _skillLevel);
	}
}
#endif

bool Fighter::learnSkill(UInt16 skill)
{
    // TODO:
#if 0
	if(skill == _skill)
	{
		if(_owner != NULL) _owner->sendMsgCode(0, 2151);
		return false;
	}
	const UInt8 colors[3][20] = {{1, 2, 3, 2, 3, 4, 3, 4}, {4, 1, 2, 3, 2, 3, 3, 4}, {2, 3, 4, 3, 1, 2, 3, 4}};
	UInt8 cls = static_cast<UInt8>((skill / 100) - 1);
	UInt8 idx = static_cast<UInt8>((skill % 100) - 1);
	if(cls != ((_class - 1) / 2))
	{
		if(_owner != NULL) _owner->sendMsgCode(0, 2152);
		return false;
	}
	if(skill == 106 || skill == 201 || skill == 303)
	{
		if(_id > 6)
		{
			if(_owner != NULL) _owner->sendMsgCode(0, 2153);
			return false;
		}
	}
	else if(colors[cls][idx] > _color)
	{
		if(_owner != NULL) _owner->sendMsgCode(0, 2154);
		return false;
	}
	setSkillAndLevel(skill, (_skillLevel > 0) ? (_skillLevel - 1) : 0);
	if(_owner != NULL && _owner->isOnline())
	{
		SYSMSG_SENDV(162, _owner, _skill, _skillLevel);
		SYSMSG_SENDV(1062, _owner, _color, getName().c_str(), _skill, _skillLevel);
	}
#endif
	return true;
}

bool Fighter::skillLevelUp( UInt16 skill, UInt8 lv )
{
    // TODO: 
#if 0
	if(_skillLevel + 1 != lv)
	{
		if(_owner != NULL) _owner->sendMsgCode(0, 2155);
		return false;
	}
	setSkillLevel(lv);
	if(_owner != NULL && _owner->isOnline())
	{
		SYSMSG_SENDV(162, _owner, _skill, _skillLevel);
		SYSMSG_SENDV(1062, _owner, _color, getName().c_str(), _skill, _skillLevel);
	}
#endif
	return true;
}

#if 0
void Fighter::setSkillAndLevel( UInt16 data )
{
	_skill = data / 100;
	_skillLevel = data % 100;
}

void Fighter::setSkillAndLevel( UInt16 skill, UInt8 level, bool writedb /*= true*/ )
{
	if(_skill == skill && _skillLevel == level)
		return;
	_skill = skill;
	_skillLevel = level;
	if(writedb)
	{
		sendModification(0x11, _skill * 100 + _skillLevel);
	}
}
#endif

void Fighter::getAllUpSkillAndLevel( Stream& st )
{
    int skills = getUpSkillsNum();
    st << skills;
    for (int i = 0; i < skills; ++i)
    {
        if (_skill[i])
            st << _skill[i];
    }
}

void Fighter::getAllSkillsAndLevel( Stream& st )
{
    int skills = getSkillsNum();
    st << skills;
    for (int i = 0; i < skills; ++i)
    {
        if (_skills[i])
            st << _skills[i];
    }
}

void Fighter::getAllSkillAndLevel( Stream& st )
{
    getAllUpSkillAndLevel(st);
    getAllSkillsAndLevel(st);
}


void Fighter::getAllUpCittaAndLevel( Stream& st )
{
    UInt8 cittas = getUpCittasNum();
    st << cittas;
    for (int i = 0; i < cittas; ++i)
    {
        if (_citta[i])
            st << _citta[i];
    }
}

void Fighter::getAllCittasAndLevel( Stream& st )
{
    int cittas = getCittasNum();
    st << cittas;
    for (int i = 0; i < cittas; ++i)
    {
        if (_cittas[i])
            st << _cittas[i];
    }
}

void Fighter::getAllCittaAndLevel( Stream& st )
{
    getAllUpCittaAndLevel(st);
    getAllCittasAndLevel(st);
}

UInt32 Fighter::getTrumpId( int idx )
{
    return (idx >= 0 && idx < TRUMP_UPMAX && _trump[idx]) ? _trump[idx]->getId() : 0;
}

void Fighter::getAllTrumps( Stream& st )
{
    // XXX: append to armor
    // st << TRUMP_UPMAX;
    for (int i = 0; i < TRUMP_UPMAX; ++i)
    {
        st << getTrumpId(i);
    }
}

void Fighter::getAllBloodBits( Stream& st )
{
    for (int i = 0; i < BLOODBIT_MAX; ++i)
    {
        st << _bloodbit[i];
    }
}

void Fighter::setPeerless( UInt16 peerless, bool writedb )
{
    _peerless = peerless;
    // TODO:
    if (writedb)
    {
    }
}

void Fighter::setBloodBits( std::string& bloodbit, bool writedb )
{
    if (!bloodbit.length())
        return;

    StringTokenizer tk(bloodbit, "|");
    for (size_t i = 0; i < tk.count() && i < BLOODBIT_MAX; ++i)
    {
        setBloodBit(::atoi(tk[i].c_str()), writedb); // XXX: must be less then 255
    }

}

bool Fighter::setBloodBit( int idx, UInt8 v, bool writedb )
{
    if (idx >= 0  && idx < BLOODBIT_MAX && v <= getBloodCntMax())
    {
        _bloodbit[idx] = v;
        if (writedb)
        {
            _attrDirty = true;
            _bPDirty = true;
            sendModificationBloodBit(0x29, idx, writedb);
        }
        return true;
    }
    return false;
}

bool Fighter::incBloodBit( int idx, bool writedb )
{
    if (idx >= 0 && idx < BLOODBIT_MAX)
    {
        if (_bloodbit[idx] < getBloodCntMax())
        {
            return setBloodBit(idx, _bloodbit[idx]+1, writedb);
        }
    }
    return false;
}

void Fighter::setUpSkills( std::string& skill, bool writedb )
{
    if (!skill.length())
        return;

    StringTokenizer tk(skill, "|");
    for (size_t i = 0; i < tk.count() && i < getUpSkillsMax(); ++i)
    {
        upSkill(::atoi(tk[i].c_str()), i, writedb);
    }
}

int Fighter::hasSkill( UInt16 skill )
{
    for (size_t i = 0; i < _skills.size(); ++i)
    {
        if (_skills[i] == skill || SKILL_ID(_skills[i]) == SKILL_ID(skill))
            return static_cast<int>(i);
    }
    return -1;
}

int Fighter::isSkillUp(UInt16 skill)
{
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (skill == _skill[i] || SKILL_ID(skill) == SKILL_ID(_skill[i]))
            return i;
    }
    return -1;
}

int Fighter::getUpSkillsNum()
{
    int c = 0;
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (_skill[i])
            ++c;
    }
    return c;
}

bool Fighter::upSkill( UInt16 skill, int idx, bool writedb )
{
    if (!(idx >= 0 && idx < getUpSkillsMax())) // dst
        return false;

    bool ret = false;
    int src = isSkillUp(skill);
    if (src < 0)
    {
        int i = getUpSkillsNum();
        if (i < getUpSkillsMax())
        { // insert
            for (int j = getUpSkillsMax() - 1; j >= idx+1; ++j)
            {
                _skill[j] = _skill[j-1];;
                _skill[j-1] = 0;
            }
            _skill[i] = skill;
            idx = i;
            ret = true;
        }
        else
        { // replace
            _skill[idx] = skill;
            ret = true;
        }
    }
    else
    {
        if (src != idx)
        {
            if (_skill[idx])
            { // swap
                sendModification(0x60, _skill[idx], src, false);

                _skill[src] ^= _skill[idx];
                _skill[idx] ^= _skill[src];
                _skill[src] ^= _skill[idx];
                ret = true;
            }
        }
        else
        { // upgrade
            if (_skill[idx] != skill)
            {
                _skill[idx] = skill;
                ret = true;
            }
        }
    }

    if (ret)
    {
        _attrDirty = true;
        _bPDirty = true;
        sendModification(0x60, skill, idx, writedb);
    }

    return ret;
}

bool Fighter::offSkill( UInt16 skill, bool writedb )
{
    int idx = isSkillUp(skill);
    if (idx < 0)
        return false;

    // XXX: ? if (SKILL_ID(_skill[idx]) == SKILL_ID(skill))
    _skill[idx] = 0;
    for (int i = idx; i < getUpSkillsMax() - 1; ++i)
    {
        _skill[i] = _skill[i+1];
        _skill[i+1] = 0;
    }

    _attrDirty = true;
    _bPDirty = true;
    sendModification(0x60, 0, idx, writedb);
    return true;
}

void Fighter::setSkills( std::string& skills, bool writedb )
{
    if (!skills.length())
        return;

    StringTokenizer tk(skills, "|");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        addNewSkill(::atoi(tk[i].c_str()));
    }
}

bool Fighter::addNewSkill( UInt16 skill, bool writedb )
{
    int idx = hasSkill(skill);
    if (idx > 0)
    {
        if (skill != _skills[idx])
        { // upgrade
            _skills[idx] = skill;
            int i = isSkillUp(skill);
            if (i >= 0)
                upSkill(skill, i, writedb);
        }
        else
            return false;
    }
    else
    {
        idx = static_cast<int>(_skills.size());
        _skills.push_back(skill);
    }

    sendModification(0x61, skill, idx, writedb);
    return true;
}

void Fighter::setUpCittas( std::string& citta, bool writedb )
{
    if (!citta.length())
        return;

    StringTokenizer tk(citta, "|");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        upCitta(::atoi(tk[i].c_str()), i, writedb);
    }
}

int Fighter::isCittaUp( UInt16 citta )
{
    for (int i = 0; i < getUpCittasMax(); ++i)
    {
        if (citta == _citta[i] || CITTA_ID(citta) == CITTA_ID(_citta[i]))
            return i;
    }
    return -1;
}

bool Fighter::upCitta( UInt16 citta, int idx, bool writedb )
{
    if (!(idx >= 0 && idx < getUpCittasMax())) // dst
        return false;

    bool ret = false;
    int src = isCittaUp(citta);
    if (src < 0)
    {
        int i = getUpCittasNum();
        if (i < getUpCittasMax())
        { // insert
            for (int j = getUpCittasMax() - 1; j >= idx+1; ++j)
            {
                _citta[j] = _citta[j-1];;
                _citta[j-1] = 0;
            }
            _citta[i] = citta;
            idx = i;
            ret = true;
        }
        else
        { // replace
            _citta[idx] = citta;
            ret = true;
        }
    }
    else
    {
        if (src != idx)
        {
            if (_citta[idx])
            { // swap
                sendModification(0x62, _citta[idx], src, false);

                _citta[src] ^= _citta[idx];
                _citta[idx] ^= _citta[src];
                _citta[src] ^= _citta[idx];
                ret = true;
            }
        }
        else
        { // upgrade
            if (_citta[idx] != citta)
            {
                _citta[idx] = citta;
                ret = true;
            }
        }
    }

    if (ret)
    {
        _attrDirty = true;
        _bPDirty = true;
        sendModification(0x62, citta, idx, writedb);
    }

    return ret;
}

void Fighter::setCittas( std::string& cittas, bool writedb )
{
    if (!cittas.length())
        return;

    StringTokenizer tk(cittas, "|");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        addNewCitta(::atoi(tk[i].c_str()), writedb);
    }
}

int Fighter::hasCitta( UInt16 citta )
{
    for (size_t i = 0; i < _cittas.size(); ++i)
    {
        if (_cittas[i] == citta || CITTA_ID(_cittas[i]) == CITTA_ID(citta))
            return static_cast<int>(i);
    }
    return -1;
}

bool Fighter::addNewCitta( UInt16 citta, bool writedb )
{
    int idx = hasCitta(citta);
    if (idx > 0)
    {
        if (_cittas[idx] != citta)
        { // upgrade
            _cittas[idx] = citta;
            int i = isCittaUp(citta);
            if (i >= 0)
                upCitta(citta, writedb);
        }
        else
            return false;
    }
    else
    {
        idx = _cittas.size();
        _cittas.push_back(citta);
    }

    sendModification(0x63, citta, idx, writedb);
    return true;
}

UInt8 Fighter::getUpCittasNum()
{
    UInt8 c = 0;
    for (int i = 0; i < CITTA_UPMAX; ++i)
    {
        if (_citta[i])
            ++c;
    }
    return c;
}

Fighter * GlobalFighters::getRandomOut()
{
	if(_fighters.empty())
		return NULL;
	UInt32 id = 7 + uRand(_maxFgtId - 6);
	return _fighters[id].fighter;
}

Fighter * GlobalFighters::getRandomOut( Player * pl )
{
	if(_fighters.empty())
		return NULL;

	UInt32 id = 7 + uRand(_maxFgtId - 6 - pl->getFighterCount());

	while(_fighters[id].fighter != NULL && pl->findFighter(_fighters[id].fighter->getId()) != NULL)
	{
		++ id;
		if(id > GREAT_FIGHTER_MAX)
			id = 7;
	}

	return _fighters[id].fighter;
}

Fighter * GlobalFighters::getRandomOut( Player * pl, std::set<UInt32>& excepts, std::set<UInt32>& excepts2, UInt8 type, UInt32 rate )
{
	if(_fighters.empty())
		return NULL;

	UInt8 color = 1;
	switch(type)
	{
	case 0:
		if(uRand(200 * 100 / rate) == 0)
			color = 2;
		break;
	case 1:
		{
			UInt32 rnd = uRand(400 * 100 / rate);
			if(rnd == 0)
				color = 3;
			else if(rnd < 5)
				color = 2;
			break;
		}
	case 2:
		color = 2;
		break;
	case 3:
		color = 3;
		break;
	default:
		color = 1;
		break;
	}

	//UInt32 fgtId = GameAction()->onTavernFlush(color);
	//if(fgtId > 0 && !pl->hasFighter(fgtId) && excepts.find(fgtId) == excepts.end())
	//{
	//	return _fighters[fgtId].fighter;
	//}

	std::map<UInt32, UInt32> idset;
	while(idset.empty() && color > 0)
	{
		-- color;

		idset = _summonSet[color];
		pl->exceptAvailableFighters(idset);
		for(std::set<UInt32>::iterator it = excepts.begin(); it != excepts.end(); ++ it)
			idset.erase(*it);
		for(std::set<UInt32>::iterator it = excepts2.begin(); idset.size() > 6 && it != excepts2.end(); ++ it)
			idset.erase(*it);
	}

	size_t size = idset.size();
	if(size == 0)
		return NULL;

	if(color == 2)
	{
		std::map<UInt32, UInt32>::iterator it;
		UInt32 bs = 0;
		for(it = idset.begin(); it != idset.end(); ++ it)
		{
			bs += it->second;
		}
		UInt32 r = uRand(bs);
		for(it = idset.begin(); it != idset.end(); ++ it)
		{
			if(r < it->second)
			{
				return _fighters[it->first].fighter;
			}
			r -= it->second;
		}
		return NULL;
	}
	else
	{
		std::map<UInt32, UInt32>::iterator it = idset.begin();
		if(size > 1)
			std::advance(it, uRand(size));
		return _fighters[it->first].fighter;
	}
}

void GlobalFighters::setSpot( UInt32 id, UInt16 spot )
{
	FastMutex::ScopedLock lk(_mutex);
	if(id > GREAT_FIGHTER_MAX)
		return;
	_fighters[id].spot = spot;
}

UInt16 GlobalFighters::getSpot( UInt32 id )
{
	FastMutex::ScopedLock lk(_mutex);
	if(id > GREAT_FIGHTER_MAX)
		return 0;
	if(_fighters[id].fighter == NULL)
		return 0;
	return _fighters[id].spot;
}

void GlobalFighters::buildSummonSet()
{
	for(UInt32 i = 7; i < 134; ++ i)
	{
		Fighter * fgt = _fighters[i].fighter;
		if(fgt == NULL) continue;
		UInt8 color = fgt->getColor();
		switch(color)
		{
		case 1:
		case 2:
			_summonSet[color - 1][i] = 0;
			break;
		}
	}
	_summonSet[2][8] = 3;
	_summonSet[2][11] = 6;
	_summonSet[2][13] = 4;
	_summonSet[2][24] = 6;
	_summonSet[2][25] = 3;
	_summonSet[2][26] = 6;
	_summonSet[2][28] = 3;
	_summonSet[2][30] = 3;
	_summonSet[2][35] = 2;
	_summonSet[2][37] = 6;
	_summonSet[2][44] = 6;
	_summonSet[2][45] = 6;
	_summonSet[2][48] = 2;
	_summonSet[2][51] = 3;
	_summonSet[2][55] = 6;
	_summonSet[2][56] = 3;
	_summonSet[2][63] = 6;
	_summonSet[2][66] = 2;
	_summonSet[2][77] = 2;
	_summonSet[2][85] = 4;
	_summonSet[2][87] = 4;
	_summonSet[2][102] = 4;
	_summonSet[2][106] = 6;
	_summonSet[2][107] = 3;
	_summonSet[2][117] = 4;
	_summonSet[2][118] = 4;
	_summonSet[2][122] = 4;
	_summonSet[2][123] = 6;
	_summonSet[2][131] = 6;
	_summonSet[2][132] = 4;
}

void GlobalFighters::setAllDirty()
{
	size_t sz = _fighters.size();
	for(size_t i = 0; i < sz; ++ i)
	{
		if(_fighters[i].fighter != NULL)
			_fighters[i].fighter->setDirty();
	}
}

}
