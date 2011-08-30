#include "Config.h"
#include "Fighter.h"
#include "Country.h"
#include "TaskMgr.h"
#include "Server/WorldServer.h"
#include "MsgID.h"
#include "Player.h"
#include "Clan.h"
#include "Package.h"
#include "GData/ExpTable.h"
#include "GData/SkillTable.h"
#include "GData/CittaTable.h"
#include "GData/AcuPraTable.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "Common/TimeUtil.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Script/BattleFormula.h"
#include "GData/FighterProb.h"
#include "GObject/Mail.h"

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
	_id(id), _owner(owner), _class(0), _level(1), _exp(0), _pexp(0), _pexpMax(0), _potential(1.0f),
    _capacity(1.0f), _color(2), _hp(0), _cittaslot(CITTA_INIT), _weapon(NULL),
    _ring(NULL), _amulet(NULL), _attrDirty(false), _maxHP(0), _bPDirty(false), _battlePoint(0.0f),
    _praadd(0), favor(0), reqFriendliness(0), strength(0), physique(0),
    agility(0), intelligence(0), will(0), soulMax(0), soul(0), baseSoul(0), aura(0), tough(0),
    attack(0), defend(0), maxhp(0), action(0), peerless(0), talent(0),
    hitrate(0), evade(0), critical(0), criticaldmg(0), pierce(0), counter(0), magres(0)
{
    memset(_acupoints, 0, sizeof(_acupoints));
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
    //TODO
	static UInt16 atk_enc[] = {0, 10, 20, 40, 60, 100, 200, 300, 450, 600, 800, 1050, 1500};
	return _weapon ? (atk_enc[_weapon->getItemEquipData().enchant]) : 0;
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
			def += def_enc[enc];
			hp += hp_enc[enc];
		}
	}
}

UInt8 Fighter::getColor2( float pot )
{
	if(pot < 0.299f)
		return 10;
	if(pot < 0.999f)
		return 0;
	if(pot < 1.199f)
		return 1;
	if(pot < 1.399f)
		return 2;
	if(pot < 1.799f)
		return 3;
	if(pot < 2.099f)
		return 4;
	return 10;
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
		_attrDirty = true;
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

bool Fighter::addPExp( Int32 e, bool writedb )
{
    if (e < 0)
    {
        if (_pexp <= (UInt32)-e)
            _pexp = 0;
        else
            _pexp += e;
    }
    else
    {
        _pexp += e;
        if (_pexp > _pexpMax)
            _pexp = _pexpMax;
    }

	bool isMain = _owner->isMainFighter(_id);
    if (e < 0)
    {
        if(isMain)
        {
            SYSMSG_SENDV(2006, _owner, -e);
        }
        SYSMSG_SENDV(2007, _owner, _color, getName().c_str(), -e);
    }
    else
    {
        if(isMain)
        {
            SYSMSG_SENDV(2004, _owner, e);
        }
        SYSMSG_SENDV(2005, _owner, _color, getName().c_str(), e);
    }

    sendModification(6, _pexp);
    return true;
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
    if (t >= 0x50 && t < 0x50 + getMaxTrumps())
    {
        UInt32 trumps[TRUMP_UPMAX] = {0};
        if (getAllTrumpId(trumps)) {
            std::string str;
            if (value2string(trumps, getMaxTrumps(), str)) {
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
	case 6: DB().PushUpdateData("UPDATE `fighter` SET `practiceExp` = %"I64_FMT"u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v, _id, _owner->getId());
        break;
    case 7:
            break;
    case 8:
            break;
    case 9:
            break;

    case 0x29:
        {
            std::string str;
            if (value2string(_acupoints, ACUPOINTS_MAX, str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `acupoints` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x31: // peerless
        break;
    case 0x32: // cittaslot
        // DB().PushUpdateData("UPDATE `fighter` SET `cittaslot` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _cittaslot, _id, _owner->getId());
        break;
    case 0x60:
        { // skill
            std::string str;
            if (value2string(_skill, getUpSkillsMax(), str)) {
                DB().PushUpdateData("UPDATE `fighter` SET `skill` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x61:
        { // skills
            if (_skills.size()) {
                std::string str;
                if (value2string(&_skills[0], _skills.size(), str)) {
                    DB().PushUpdateData("UPDATE `fighter` SET `skills` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
                }
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
            if (_cittas.size()) {
                std::string str;
                if (value2string(&_cittas[0], _cittas.size(), str)) {
                    DB().PushUpdateData("UPDATE `fighter` SET `cittas` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
                }
            }
        }
        break;
    case 0x64: // passive
        break;

	case 0x21: field = "weapon"; break;
	case 0x22: field = "armor1"; break;
	case 0x23: field = "armor2"; break;
	case 0x24: field = "armor3"; break;
	case 0x25: field = "armor4"; break;
	case 0x26: field = "armor5"; break;
	case 0x27: field = "amulet"; break;
	case 0x28: field = "ring"; break;
	case 0x30: field = "peerless"; break;
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

void Fighter::sendModificationAcupoints( UInt8 t, int idx, bool writedb )
{
	if(_owner == NULL)
		return;
	Stream st(REP::CHANGE_EQUIPMENT);
	st << getId() << static_cast<UInt8>(1) << t;
    st << static_cast<UInt8>(idx) << _acupoints[idx] << getSoul() << getMaxSoul();
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
	Stream st(REP::CHANGE_EQUIPMENT);
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
	Stream st(REP::CHANGE_EQUIPMENT);
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
	Stream st(REP::CHANGE_EQUIPMENT);
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
	Stream st(REP::CHANGE_EQUIPMENT);
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
	Stream st(REP::CHANGE_EQUIPMENT);
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
	Stream st(REP::CHANGE_EQUIPMENT);
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
	Stream st(REP::CHANGE_EQUIPMENT);
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

bool Fighter::hasTrumpType(UInt32 trumpid)
{
    for (UInt8 i = 0; i < TRUMP_UPMAX; ++i)
    {
        if (_trump[i])
        {
        }
    }
    return false;
}

bool Fighter::canSetTrump(UInt8 idx, UInt32 trumpid)
{
    if (idx == 0)
        return true;

    if (_level >= 60 && idx == 2)
    {
        return !hasTrumpType(trumpid);
    }

    if (_potential >= 1.5 && _capacity >= 7 && idx == 1)
    {
        return !hasTrumpType(trumpid);
    }

    return false;
}

ItemEquip ** Fighter::setTrump( std::string& trumps, bool writedb )
{
    if (!trumps.length())
        return 0;

    StringTokenizer tk(trumps, ",");
    for (size_t i = 0; i < tk.count() && static_cast<int>(i) < getMaxTrumps(); ++i)
    {
        UInt32 trump = ::atoi(tk[i].c_str());
        if (trump) {
            setTrump(trump, i, writedb);
        }
    }

    return &_trump[0];
}

ItemEquip* Fighter::setTrump( UInt32 trump, int idx, bool writedb )
{
    if (canSetTrump(idx, trump))
    {
        ItemEquip* t = 0;
        t = GObjectManager::fetchEquipment(trump);
        return setTrump(t, idx, writedb);
    }
    return 0;
}

ItemEquip* Fighter::setTrump( ItemEquip* trump, int idx, bool writedb )
{
    ItemEquip* t = 0;
    if (trump || canSetTrump(idx, trump->getId()))
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
            t = _trump[idx];
            _trump[idx] = trump;

            if (trump)
                trump->DoEquipBind(true);

            if (trump)
            { // up
                const GData::AttrExtra* attr = trump->getAttrExtra();
                if (attr)
                {
                    addSkillsFromCT(attr->skills, writedb);
                }
            }
            else
            { // off
                if (t)
                {
                    const GData::AttrExtra* attr = t->getAttrExtra();
                    if (attr)
                    {
                        const GData::SkillBase* s = 0;
                        for (size_t i = 0; i < attr->skills.size(); ++i)
                        {
                            s = attr->skills[i];
                            if (s)
                            {
                                if (s->cond == GData::SKILL_PEERLESS)
                                    delPeerless(s->getId(), writedb);
                                else if (s->cond == GData::SKILL_ACTIVE)
                                    delSkill(s->getId(), writedb);
                                else
                                    offPassiveSkill(s->getId(), s->cond, s->prob>=100.0f, writedb);
                            }
                        }
                    }
                }
            }

            _attrDirty = true;
            _bPDirty = true;
            sendModification(0x50+idx, _trump[idx], writedb);
        }
    }
    return t;
}

int Fighter::getAllTrumpId( UInt32* trumps, int size )
{
    if (!trumps || !size)
        return 0;

    for (int i = 0; i < getMaxTrumps(); ++i)
    {
        if (_trump[i])
            trumps[i] = _trump[i]->getId();
        else
            trumps[i] = 0;
    }
    return getMaxTrumps();
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

inline void addAttrExtra( GData::AttrExtra& ae, const GData::AttrExtra * ext )
{
	if(ext == NULL)
		return;
	ae += *ext;
}

inline void addAttrExtra( GData::AttrExtra& ae, const GData::CittaEffect* ce )
{
	if(ce == NULL)
		return;
	ae += *ce;
}

inline void addEquipAttr2( GData::AttrExtra& ae, UInt8 type, UInt16 value, UInt8 level )
{
	switch(type)
	{
    case 1:
        break;
        ae.hp += value;
    case 2:
        ae.action += value;
        break;
	case 3:
        ae.hitrlvl += value/10;
		break;
	case 4:
        ae.evdlvl += value/10;
		break;
	case 5:
        ae.crilvl += value/10;
		break;
	case 6:
        ae.pirlvl += value/10;
		break;
    case 7:
        ae.counterlvl += value/10;
        break;
	case 8:
        ae.toughlvl += value/10;
		break;
    case 9:
        ae.mreslvl += value/10;
	}
}

// TODO:
inline void addEquipAttr2( GData::AttrExtra& ae, const GData::CittaEffect* ce, UInt8 level)
{
}

inline void addEquipAttr2( GData::AttrExtra& ae, const ItemEquipAttr2& ext, UInt8 level )
{
	if(ext.type1 != 0)
	{
		addEquipAttr2(ae, ext.type1, ext.value1, level);
	}
	if(ext.type2 != 0)
	{
		addEquipAttr2(ae, ext.type2, ext.value2, level);
	}
	if(ext.type3 != 0)
	{
		addEquipAttr2(ae, ext.type3, ext.value3, level);
	}
}

inline void testEquipInSet(UInt32 * setId, UInt32 * setNum, UInt32 id)
{
    if (id < 2000)
        return;

	id -= 2000;
    id /= 8;
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

void Fighter::addAttr( const GData::CittaEffect* ce )
{
	addAttrExtra(_attrExtraEquip, ce);
	addEquipAttr2(_attrExtraEquip, ce, _level);
}

void Fighter::addAttr( ItemEquip * equip )
{
	addAttrExtra(_attrExtraEquip, equip->getAttrExtra());
	addEquipAttr2(_attrExtraEquip, equip->getEquipAttr2(), _level);
	ItemEquipData& ied = equip->getItemEquipData();
	for(UInt8 i = 0; i < ied.sockets; ++ i)
	{
		if(ied.gems[i] != 0)
		{
			GData::ItemGemType * igt = GData::gemTypes[ied.gems[i] - LGEM_ID];
			addAttrExtra(_attrExtraEquip, igt->attrExtra);
		}
	}
}

void Fighter::rebuildEquipAttr()
{
	_attrExtraEquip.reset();
	_attrExtraEquip.reset();

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
		_attrExtraEquip += *iest->attrExtra[idx];
		_attrExtraEquip += *iest->attrExtra[idx];
	}
	_attrExtraEquip.attack += getWeaponAttack();
	_attrExtraEquip.magatk += getWeaponAttack();
	UInt16 armorDefend, armorHP;
	getArmorDefendAndHP(armorDefend, armorHP);
	_attrExtraEquip.defend += armorDefend;
	_attrExtraEquip.magdef += armorDefend;
	_attrExtraEquip.hp += armorHP;

    for (int i = 0; i < getUpCittasNum(); ++i)
    {
        if (_citta[i])
        {
            const GData::CittaBase* cb = GData::cittaManager[_citta[i]];
            if (cb)
            {
                if (cb->effect)
                    addAttr(cb->effect);
            }
        }
    }

    // 帮派秘术对额外属性的加成
    Clan* clan = _owner == NULL ? NULL : _owner->getClan();
    if(clan != NULL)
    {
        _attrExtraEquip.hp += clan->getSkillHPEffect(_owner);
        _attrExtraEquip.attack += clan->getSkillAtkEffect(_owner);
        _attrExtraEquip.defend += clan->getSkillDefendEffect(_owner);
        _attrExtraEquip.magatk += clan->getSkillMagAtkEffect(_owner);
        _attrExtraEquip.magdef += clan->getSkillMagDefentEffect(_owner);
    }

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
    fgt->_pexpMax = 100000; // XXX: 100000
	memset(fgt->_armor, 0, 5 * sizeof(ItemEquip *));
    memset(fgt->_trump, 0, TRUMP_UPMAX * sizeof(ItemEquip*));
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
    for(int idx = 0; idx < TRUMP_UPMAX; ++ idx)
    {
        if(_trump[idx] != NULL && _trump[idx]->getId() == id)  // 法宝
        {
            pos = idx;
            return _trump[idx];
        }
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
bool Fighter::learnSkill(UInt16 skill)
{
	if(skill == _skill)
	{
		if(_owner != NULL) _owner->sendMsgCode(0, 1700);
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
	return true;
}
#endif

bool Fighter::skillLevelUp( UInt16 skill, UInt8 lv )
{
    int idx = hasSkill(skill);
    if (idx < 0)
        return false;
    if (SKILL_LEVEL(_skills[idx])+1 != lv)
    {
		if(_owner != NULL) _owner->sendMsgCode(0, 2155);
		return false;
    }

    idx = isSkillUp(skill);
    if (idx >= 0)
        upSkill(SKILLANDLEVEL(skill, lv), idx);

    if (addNewSkill(SKILLANDLEVEL(skill, lv)))
    {
        if(_owner != NULL && _owner->isOnline())
        {
            SYSMSG_SENDV(162, _owner, skill, lv);
            SYSMSG_SENDV(1062, _owner, _color, getName().c_str(), skill, lv);
        }
        return true;
    }

    return false;
}

void Fighter::getAllUpSkillAndLevel( Stream& st )
{
    Int8 skills = getUpSkillsNum();
    st << skills;
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (_skill[i]) {
            st << static_cast<UInt8>(i);
            st << _skill[i];
        }
    }
}

UInt16 Fighter::getPSkillsNum()
{
    return getP100SkillsNum() + getPnSkillsNum();
}

UInt16 Fighter::getP100SkillsNum()
{
    UInt16 size = 0;
    for (size_t i = 0; i < GData::SKILL_PASSIVES-GData::SKILL_PASSSTART; ++i)
    {
        size += _passkl[i].size(); 
    }
    return size;
}

UInt16 Fighter::getPnSkillsNum()
{
    UInt16 size = 0;
    for (size_t i = 0; i < GData::SKILL_PASSIVES-GData::SKILL_PASSSTART; ++i)
    {
        size += _rpasskl[i].size(); 
    }
    return size;
}

void Fighter::getAllPSkillAndLevel(Stream& st)
{
    UInt8 size = getPSkillsNum();
    st << size;
    for (size_t i = 0; i < GData::SKILL_PASSIVES-GData::SKILL_PASSSTART; ++i)
    {
        for (size_t j = 0; j < _passkl[i].size(); ++j)
        {
            st << _passkl[i][j];
        }
    }
    for (size_t i = 0; i < GData::SKILL_PASSIVES-GData::SKILL_PASSSTART; ++i)
    {
        for (size_t j = 0; j < _rpasskl[i].size(); ++j)
        {
            st << _rpasskl[i][j];
        }
    }
}

UInt8 Fighter::getSkillsNum()
{
    UInt8 c = 0;
    for (size_t i = 0; i < _skills.size(); ++i)
        if (_skills[i]) ++c;
    return c;
}

void Fighter::getAllSkillsAndLevel( Stream& st )
{
    UInt8 skills = getSkillsNum();
    st << skills;
    for (int i = 0; i < skills; ++i)
    {
        if (_skills[i])
        {
            st << _skills[i];
        }
    }
}

void Fighter::getAllSkillAndLevel( Stream& st )
{
    getAllUpSkillAndLevel(st);
    getAllPSkillAndLevel(st);
    getAllSkillsAndLevel(st);
}

void Fighter::getAllUpCittaAndLevel( Stream& st )
{
    UInt8 cittas = getUpCittasNum();
    st << cittas;
    for (int i = 0; i < cittas; ++i)
    {
        if (_citta[i]) {
            st << static_cast<UInt8>(i);
            st << _citta[i];
        }
    }
}

UInt8 Fighter::getCittasNum()
{
    UInt8 c = 0;
    for (size_t i = 0; i < _cittas.size(); ++i)
        if (_cittas[i]) ++c;
    return c;
}

void Fighter::getAllCittasAndLevel( Stream& st )
{
    // XXX: 只发送没有装备的
    UInt8 cittas = getCittasNum();
    st << cittas;
    for (int i = 0; i < cittas; ++i)
    {
        if (_cittas[i])
        {
            st << _cittas[i];
        }
    }
}

void Fighter::getAllCittaAndLevel( Stream& st )
{
    getAllUpCittaAndLevel(st);
    getAllCittasAndLevel(st);
}

void Fighter::getAllPeerlessAndLevel( Stream& st )
{
    UInt8 peerlesses = getPeerlessNum();
    st << peerlesses;
    for (int i = 0; i < peerlesses; ++i)
    {
        if (_peerless[i])
            st << _peerless[i];
    }
}

void Fighter::offPeerless( bool writedb )
{
    setPeerless(0, writedb);
}

int Fighter::hasPeerless( UInt16 pl )
{
    for (size_t i = 0; i < _peerless.size(); ++i)
    {
        if (_peerless[i] == pl || SKILL_ID(_peerless[i]) == SKILL_ID(pl))
            return static_cast<int>(i);
    }
    return -1;
}

bool Fighter::addNewPeerless( UInt16 pl, bool writedb )
{
    int op = 0;
    int idx = hasPeerless(pl);
    if (idx >= 0)
    {
        if (pl != _peerless[idx])
        { // upgrade
            _peerless[idx] = pl;
            int i = isPeerlessUp(pl);
            if (i >= 0)
                upPeerless(pl, writedb);
            op = 3;
        }
        else
            return false;
    }
    else
    {
        idx = static_cast<int>(_peerless.size());
        _peerless.push_back(pl);
        op = 1;
    }

    sendModification(0x31, pl, op/*1add,2del,3mod*/, writedb);
    return true;
}

bool Fighter::delPeerless( UInt16 pl, bool writedb )
{
    for (size_t i = 0; i < _peerless.size(); ++i)
    {
        if (SKILL_ID(_peerless[i]) == SKILL_ID(pl))
        {
            if (isPeerlessUp(pl))
                offPeerless(writedb);

            std::vector<UInt16>::iterator it = _peerless.begin();
            std::advance(it, i);
            _peerless.erase(it);

            sendModification(0x31, pl, 2/*1add,2del,3mod*/, writedb);
            return true;
        }
    }
    return false;
}

void Fighter::getAllPeerless( Stream& st )
{
    UInt8 pls = _peerless.size();
    st << pls;
    for (int i = 0; i < pls; ++i)
    {
        if (_peerless[i])
            st << _peerless[i];
    }
#if 0
    std::for_each(_peerless.begin(), _peerless.end(), [&st](UInt16& pl){ st << pl; });
#endif
}

UInt32 Fighter::getTrumpId( int idx )
{
    return (idx >= 0 && idx < getMaxTrumps() && _trump[idx]) ? _trump[idx]->getId() : 0;
}

void Fighter::getAllTrumps( Stream& st )
{
    // XXX: append to armor
    // st << static_cast<UInt8>(TRUMP_UPMAX);
    for (int i = 0; i < TRUMP_UPMAX; ++i)
    {
        st << getTrumpId(i);
    }
}

void Fighter::getAllAcupointsBits( Stream& st )
{
    for (int i = 0; i < ACUPOINTS_MAX; ++i)
    {
        st << _acupoints[i];
    }
}

void Fighter::setPeerless( UInt16 pl, bool writedb )
{
    if (peerless == pl)
        return;

    if (pl)
    {
        int idx = hasPeerless(pl);
        if (idx < 0)
            return;
    }

    peerless = pl;
    _attrDirty = true;
    _bPDirty = true;
    sendModification(0x30, peerless);
}

void Fighter::setAcupoints( std::string& acupoints, bool writedb )
{
    if (!acupoints.length())
        return;

    StringTokenizer tk(acupoints, ",");
    for (size_t i = 0; i < tk.count() && i < ACUPOINTS_MAX; ++i)
    {
        setAcupoints(i, ::atoi(tk[i].c_str()), writedb, true); // XXX: must be less then 255
    }
    
    for (UInt8 i = 0; i < ACUPOINTS_MAX; ++i)
    {
        if (_acupoints[i] && _acupoints[i] < 3)
            _praadd += _acupoints[i];
        else if (_acupoints[i] == 3)
            _praadd += 2;
    }
}

// XXX: 穴道 id (0-14) lvl [1-3]
bool Fighter::setAcupoints( int idx, UInt8 v, bool writedb, bool init )
{
    UInt8 cittaslot = _cittaslot;
    Int32 soulmax = soulMax;
    UInt32 pexp = _pexpMax;
    if (idx >= 0  && idx < ACUPOINTS_MAX && v <= getAcupointsCntMax())
    {
        if (_acupoints[idx] == v)
            return false;

        const GData::AcuPra* pap = GData::acupraManager[idx<<8|v];
        if (!pap)
            return false;

        if (!init)
        {
            if (pap->needlvl > getLevel())
                return false;

            if (pap->pra > getPExp())
                return false;
            addPExp(-pap->pra, writedb);
        }

        soulMax += pap->soulmax;
        _pexpMax += pap->pramax;
        _cittaslot += pap->citslot;

        if (v < 3)
            ++_praadd; // 第3层不加

        _acupoints[idx] = v;
        _attrDirty = true;
        _bPDirty = true;
        sendModificationAcupoints(0x29, idx, writedb);

    //    if (pexp != _pexpMax)
            sendModification(7, _pexpMax);
    //    if (soulmax != soulMax)
            sendModification(9, soulMax);
    //    if (cittaslot != _cittaslot)
            sendModification(0x32, getUpCittasMax());
        return true;
    }
    return false;
}

bool Fighter::incAcupointsBit( int idx, bool writedb )
{
    if (idx >= 0 && idx < ACUPOINTS_MAX)
    {
        if (_acupoints[idx] < getAcupointsCntMax())
        {
            return setAcupoints(idx, _acupoints[idx]+1, writedb);
        }
    }
    return false;
}

void Fighter::setUpSkills( std::string& skill, bool writedb )
{
    if (!skill.length())
        return;

    StringTokenizer tk(skill, ",");
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
        if (skill == _skill[i] || SKILL_ID(skill) == SKILL_ID(_skill[i])) {
            if (skill != _skill[i])
                skill = _skill[i];
            return i;
        }
    }
    return -1;
}

void Fighter::getUpSkillAndLevel(std::vector<UInt16>& skills)
{
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (_skill[i])
            skills.push_back(_skill[i]);
    }
}

UInt16 Fighter::getUpSkillsNum()
{
    UInt8 c = 0;
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (_skill[i])
            ++c;
    }
    return c;
}

bool Fighter::upSkill( UInt16 skill, int idx, bool writedb )
{
    if (!skill)
        return false;
    if (!(idx >= 0 && idx < getUpSkillsMax())) // dst
        return false;

    bool ret = false;
    int src = isSkillUp(skill);
    if (src < 0)
    {
        UInt8  max = getUpSkillsMax();
        UInt16 i = getUpSkillsNum();
        if (!i)
        {
            _skill[0] = skill;
            idx = 0;
            ret = true;
        }
        else if (i < max && _skill[idx])
        { // insert
            for (int j = max - 1; j >= idx+1; --j)
            {
                _skill[j] = _skill[j-1];;
                _skill[j-1] = 0;
                sendModification(0x60, _skill[j], j, false);
            }

            _skill[idx] = skill;
            ret = true;
        }
        else
        { // replace
            _skill[idx] = skill;
            ret = true;
            // flip

        }
    }
    else
    {
        if (src != idx)
        {
            //if (_skill[idx])
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

#if 0
    UInt8 max = getUpSkillsMax();
    int i = idx;
    for (; i <= max - 1; ++i)
    {
        _skill[i] = _skill[i+1];
        _skill[i+1] = 0;
        sendModification(0x60, _skill[i], i, false);
    }
    if (!i)
        _skill[i] = 0;
#else
    _skill[idx] = 0;
#endif

    _attrDirty = true;
    _bPDirty = true;
    sendModification(0x60, 0, idx, writedb);
    return true;
}

bool Fighter::updateSkill( UInt16 skill, UInt16 nskill, bool sync, bool writedb )
{
    int idx = hasSkill(skill);
    if (idx < 0)
        return false;

    std::vector<UInt16>::iterator it = _skills.begin();
    std::advance(it, idx);
    *it = nskill;

    idx = isSkillUp(skill);
    if (idx >= 0) {
        _skill[idx] = nskill;
        sendModification(0x60, nskill, idx, writedb);
    }

    _attrDirty = true;
    _bPDirty = true;
    if (sync)
        sendModification(0x61, skill, 3/*1add,2del,3mod*/, writedb);
    return true;
}

bool Fighter::delSkill( UInt16 skill, bool writedb, bool sync, bool offskill )
{
    int idx = hasSkill(skill);
    if (idx < 0)
        return false;

    if (offskill)
        offSkill(skill);

    std::vector<UInt16>::iterator it = _skills.begin();
    std::advance(it, idx);
    *it = 0;
    _skills.erase(it);

    _attrDirty = true;
    _bPDirty = true;
    if (sync)
        sendModification(0x61, skill, 2/*1add,2del,3mod*/, writedb);
    return true;
}

void Fighter::setSkills( std::string& skills, bool writedb )
{
    if (!skills.length())
        return;

    StringTokenizer tk(skills, ",");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        addNewSkill(::atoi(tk[i].c_str()), writedb);
    }
}

bool Fighter::addNewSkill( UInt16 skill, bool writedb )
{
    if (!skill)
        return false;
    const GData::SkillBase* s = GData::skillManager[skill];
    if (!s)
        return false;
    if (!skill) return false;
    int op = 0;
    int idx = hasSkill(skill);
    if (idx >= 0)
    {
        if (skill != _skills[idx])
        { // upgrade
            _skills[idx] = skill;
            int i = isSkillUp(skill);
            if (i >= 0)
                upSkill(skill, i, writedb);
            op = 3;
        }
        else
            return false;
    }
    else
    {
        idx = static_cast<int>(_skills.size());
        _skills.push_back(skill);
        op = 1;
    }

    idx = isSkillUp(skill); // XXX: hack
    if (idx >= 0) {
        _skill[idx] = skill;
        sendModification(0x60, skill, idx, writedb);
    }

    sendModification(0x61, skill, op, writedb);
    return true;
}

void Fighter::setUpCittas( std::string& citta, bool writedb )
{
    if (!citta.length())
        return;

    StringTokenizer tk(citta, ",");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        upCitta(::atoi(tk[i].c_str()), i, writedb);
    }
}

int Fighter::isCittaUp( UInt16 citta )
{
    for (int i = 0; i < getUpCittasMax(); ++i)
    {
        if (citta == _citta[i] || CITTA_ID(citta) == CITTA_ID(_citta[i])) {
            if (citta != _citta[i])
                citta = _citta[i];
            return i;
        }
    }
    return -1;
}

bool Fighter::upCitta( UInt16 citta, int idx, bool writedb )
{
    if (!citta)
        return false;
    // XXX: need this???
    const GData::CittaBase* cb = GData::cittaManager[citta];
    if (!cb)
        return false;

    if (hasCitta(citta) < 0)
        return false;

    if (!(idx >= 0 && idx < getUpCittasMax())) // dst
        return false;

    bool swap = false;
    bool ret = false;
    int src = isCittaUp(citta);
    if (src < 0)
    {
        if (cb->needsoul > getMaxSoul() - getSoul())
            return false;

        if (idx < getUpCittasNum()) // XXX: no we all append
        {
            if (getUpCittasNum() < getUpCittasMax()) {
                for (int j = getUpCittasMax() - 1; j >= idx+1; --j)
                {
                    _citta[j] = _citta[j-1];
                    _citta[j-1] = 0;
                    if (_citta[j])
                        sendModification(0x62, _citta[j], j, writedb);
                }
            }
        } else
            idx = getUpCittasNum();

        if (_citta[idx])
            offCitta(_citta[idx], false, true, writedb);

        _citta[idx] = citta;
        ret = true;
    }
    else
    {
        if (src != idx)
        {
            if (_citta[idx])
            { // swap
                sendModification(0x62, _citta[idx], src, writedb);

                _citta[src] ^= _citta[idx];
                _citta[idx] ^= _citta[src];
                _citta[src] ^= _citta[idx];
                ret = true;
                swap = true;
            }
        }
        else
        { // upgrade
            if (_citta[idx] != citta)
            {
                if (cb->needsoul > getMaxSoul() - getSoul())
                    return false;

                // XXX: do not send message to client
                offCitta(_citta[idx], false, false, writedb); // delete skills was taken out by old citta first
                _citta[idx] = citta;
                ret = true;
            }
        }
    }

    if (ret && !swap)
    {
        addSkillsFromCT(skillFromCitta(citta), writedb);

        {
            soul += cb->needsoul;
            if (cb->needsoul)
                sendModification(8, soul);
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

bool Fighter::lvlUpCitta(UInt16 citta, bool writedb)
{
    const GData::CittaBase* cb = GData::cittaManager[citta];
    if (!cb)
        return false;

    cb = GData::cittaManager[citta+1];
    if (!cb)
        return false;

    if (getPExp() >= cb->pexp)
    {
        int i = hasCitta(citta);
        if (i < 0)
            return false;
        return addNewCitta(citta+1, writedb, false);
    }
    return false;
}

void Fighter::addSkillsFromCT(const std::vector<const GData::SkillBase*>& skills, bool writedb)
{
    if (skills.size())
    {
        const GData::SkillBase* s = 0;
        for (size_t i = 0; i < skills.size(); ++i)
        {
            s = skills[i];
            if (s)
            {
                if (s->cond == GData::SKILL_PEERLESS)
                    addNewPeerless(s->getId(), writedb);
                else if (s->cond == GData::SKILL_ACTIVE)
                    addNewSkill(s->getId(), writedb);
                else if (s->cond == GData::SKILL_PREATK ||
                        s->cond == GData::SKILL_AFTATK ||
                        s->cond == GData::SKILL_AFTNATK ||
                        s->cond == GData::SKILL_BEATKED ||
                        s->cond == GData::SKILL_AFTEVD ||
                        s->cond == GData::SKILL_AFTRES ||
                        s->cond == GData::SKILL_DEAD ||
                        s->cond == GData::SKILL_ENTER ||
                        s->cond == GData::SKILL_DEAD)
                {
                    upPassiveSkill(s->getId(), s->cond, (s->prob >= 100.0f), writedb);
                }
                else
                {
                    // error
                }
            }
        }
    }
}

// XXX: 发送时先发送100％触发技能,后发送概率触发技能
bool Fighter::upPassiveSkill(UInt16 skill, UInt16 type, bool p100, bool writedb)
{
    if (type < GData::SKILL_PASSSTART || type >= GData::SKILL_PASSIVES)
        return false;

    bool ret = false;
    size_t lastsize = 0;
    if (!p100)
    {
        for (UInt16 i = 0; i < type-GData::SKILL_PASSSTART; ++i)
            lastsize += _passkl[i].size();
    }

    UInt16 idx = type - GData::SKILL_PASSSTART;
    if (p100)
    { // 100%
        for (size_t j = 0; j < _passkl[idx].size(); ++j)
        {
            if (SKILL_ID(_passkl[idx][j]) == SKILL_ID(skill))
            {
                if (skill != _passkl[idx][j])
                { // upgrade
                    ret = true;
                    _passkl[idx][j] = skill;
                    sendModification(0x64, skill, static_cast<int>(lastsize + j), writedb);
                }
                break;
            }
        }

        if(!ret)
        {  // up
            ret = true;
            _passkl[idx].push_back(skill);
            sendModification(0x64, skill, static_cast<int>(lastsize + _passkl[idx].size()), writedb);
        }
    }
    else
    {
        for (size_t j = 0; j < _rpasskl[idx].size(); ++j)
        {
            if (SKILL_ID(_rpasskl[idx][j]) == SKILL_ID(skill))
            {
                if (skill != _rpasskl[idx][j])
                { // upgrade
                    ret = true;
                    _rpasskl[idx][j] = skill;
                    sendModification(0x33, skill, static_cast<int>(lastsize + j), writedb);
                    break;
                }
            }
        }

        if (!ret)
        { // up
            ret = true;
            _rpasskl[idx].push_back(skill);
            sendModification(0x33, skill, static_cast<int>(lastsize + _rpasskl[idx].size()), writedb);
        }
    }

    return ret;
}

bool Fighter::upPassiveSkill(UInt16* skill, UInt8 size, bool writedb)
{
    if (!skill || !size)
        return false;

    const GData::SkillBase* s  = 0;
    std::vector<const GData::SkillBase*> skills;
    for (size_t i = 0; i < size; ++i)
    {
        s = GData::skillManager[skill[i]];
        if (s && s->cond)
        {
            skills.push_back(s);
        }
    }
    if (skills.size())
        addSkillsFromCT(skills, writedb);
    return true;
}

bool Fighter::offPassiveSkill(UInt16 skill, UInt16 type, bool p100, bool writedb)
{
    if (type < GData::SKILL_PASSSTART || type >= GData::SKILL_PASSIVES)
        return false;

    bool ret = false;
    size_t lastsize = 0;

    if (!p100)
    {
        for (UInt16 i = 0; i < type-GData::SKILL_PASSSTART; ++i)
            lastsize += _passkl[i].size(); 
    }

    UInt16 idx = type - GData::SKILL_PASSSTART;
    if (p100)
    { // 100%
        for (size_t j = 0; j < _passkl[idx].size(); ++j)
        {
            if (SKILL_ID(_passkl[idx][j]) == SKILL_ID(skill))
            { // off
                std::vector<UInt16>::iterator i = _passkl[idx].begin();
                std::advance(i, j);
                _passkl[idx].erase(i);
                ret = true;
            }
        }
    }
    else
    {
        for (size_t j = 0; j < _passkl[idx].size(); ++j)
        {
            if (SKILL_ID(_passkl[idx][j]) == SKILL_ID(skill))
            { // off
                std::vector<UInt16>::iterator i = _rpasskl[idx].begin();
                std::advance(i, j);
                _passkl[idx].erase(i);
                ret = true;
            }
        }
    }

    if (ret)
    { // off
        if (p100)
        {
            _passkl[idx].push_back(skill);
            sendModification(0x64, skill, static_cast<int>(lastsize + _passkl[idx].size()), writedb);
        }
        else
        {
            _rpasskl[idx].push_back(skill);
            sendModification(0x64, skill, static_cast<int>(lastsize + _rpasskl[idx].size()), writedb);
        }
    }

    return true;
}

const std::vector<const GData::SkillBase*>& Fighter::skillFromCitta(UInt16 citta)
{
    static std::vector<const GData::SkillBase*> null;
    const GData::CittaBase* cb = GData::cittaManager[citta];
    if (cb)
    {
        if (cb->effect)
            return cb->effect->skill;
    }
    return null;
}

void Fighter::setCittas( std::string& cittas, bool writedb )
{
    if (!cittas.length())
        return;

    StringTokenizer tk(cittas, ",");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        addNewCitta(::atoi(tk[i].c_str()), writedb, true);
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

bool Fighter::addNewCitta( UInt16 citta, bool writedb, bool init )
{
    if (!citta)
        return false;
    const GData::CittaBase* cb = GData::cittaManager[citta];
    if (!cb)
        return false;

    int op = 0;
    int idx = hasCitta(citta);
    if (idx >= 0)
    {
        if (_cittas[idx] != citta)
        { // upgrade
            if (_cittas[idx] >= citta) // XXX: 不可能降级
                return false;

            int i = isCittaUp(citta);
            if (i >= 0)
                upCitta(citta, i, writedb);
            _cittas[idx] = citta;
            op = 3;
        }
        else
            return false;
    }
    else
    {
        idx = _cittas.size();
        _cittas.push_back(citta);
        op = 1;
    }

    if (!init)
        addPExp(-cb->pexp, writedb);

    _attrDirty = true;
    _bPDirty = true;
    sendModification(0x63, citta, op/*1add,2del,3mod*/, writedb);
    return true;
}

bool Fighter::offCitta( UInt16 citta, bool flip, bool offskill, bool writedb )
{
    int idx = isCittaUp(citta);
    if (idx < 0)
        return false;

    if (citta != _citta[idx])
        citta = _citta[idx];

    const std::vector<const GData::SkillBase*>& skills = skillFromCitta(citta);
    if (skills.size())
    {
        for (size_t i = 0; i < skills.size(); ++i)
        {
            if (skills[i]->cond == GData::SKILL_PEERLESS)
                delPeerless(skills[i]?skills[i]->getId():0, writedb);
            else if (skills[i]->cond == GData::SKILL_ACTIVE)
                delSkill(skills[i]?skills[i]->getId():0, writedb, true, offskill);
            else
                offPassiveSkill(skills[i]?skills[i]->getId():0, skills[i]->cond, skills[i]->prob>=100.0f, writedb);
        }
    }

    _citta[idx] = 0;

    {
        const GData::CittaBase* cb = GData::cittaManager[citta];
        if (!cb)
            return false;
        soul -= cb->needsoul;
        if (cb->needsoul)
            sendModification(8, soul);
    }

    _attrDirty = true;
    _bPDirty = true;

    int i = idx;
    if (flip) {
        for (; i < getUpCittasNum(); ++i)
        {
            _citta[i] = _citta[i+1];
            _citta[i+1] = 0;
            if (_citta[i])
                sendModification(0x62, _citta[i], i, writedb);
        }
    }
    sendModification(0x62, 0, i, writedb);
    return true;
}

bool Fighter::delCitta( UInt16 citta, bool writedb )
{
    int idx = hasCitta(citta);
    if (idx < 0)
        return false;

    offCitta(citta, true, true, writedb);

    std::vector<UInt16>::iterator it = _cittas.begin();
    std::advance(it, idx);
    *it = 0;
    _cittas.erase(it);

    {
        const GData::CittaBase* cb = GData::cittaManager[citta];
        const GData::CittaBase* yacb = cb;
        if (cb) {
            UInt32 exp = cb->pexp;
            UInt8 id = CITTA_ID(citta);
            UInt8 lvl = CITTA_LEVEL(citta);
            for (UInt8 i = 1; i <= lvl; ++i) {
                cb = GData::cittaManager[CITTAANDLEVEL(id, lvl)];
                if (cb)
                    exp += cb->pexp;
            }

            // 29-100, 30-10000, 31-1000000
            exp *= 0.6;
            if (exp) {
                UInt16 rCount1 = static_cast<UInt16>(exp / 1000000);
                exp = exp % 1000000;
                UInt16 rCount2 = static_cast<UInt16>(exp / 10000);
                exp = exp % 10000;
                UInt16 rCount3 = static_cast<UInt16>(exp / 100);

                SYSMSG(title, 2105);
                SYSMSGV(content, 2106, getLevel(), getColor(), getName().c_str(), yacb->type, yacb->getName().c_str(), lvl);
                MailPackage::MailItem mitem[3] = {{31, rCount1}, {30, rCount2}, {29, rCount3}};
                MailItemsInfo itemsInfo(mitem, DismissCitta, 3);
                GObject::Mail * pmail = _owner->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(pmail != NULL)
                {    
                    GObject::mailPackageManager.push(pmail->id, mitem, 3, true);
                }   
            }
        }
    }

    _attrDirty = true;
    _bPDirty = true;
    sendModification(0x63, citta, 2/*1add,2del,3mod*/, writedb);
    return true;
}

UInt8 Fighter::getUpCittasNum()
{
    UInt8 c = 0;
    for (int i = 0; i < getUpCittasMax(); ++i)
    {
        if (_citta[i])
            ++c;
    }
    return c;
}

float Fighter::getPracticeInc()
{
    float ret = Script::BattleFormula::getCurrent()->calcPracticeInc(this);
    return ret;
}

float Fighter::getPracticeBufFactor()
{
    return _owner->getPracticeBufFactor();
}

float Fighter::getClanTechAddon( int place )
{
    if (_owner)
    {
        GObject::Clan* clan = _owner->getClan();
        if (clan)
        {
            return clan->getClanTechAddon();
        }
    }
    return 0.0;
}

UInt16 Fighter::getPracticePlace()
{
    return _owner->getPracticePlace();
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

	Int8 color = 0;
    UInt8 colors = 4;
    UInt8 free_gold = 0;
	switch(type)
	{
    case 0:
    case 1:
        {
            UInt32 roll = uRand(100000);
            for(int i = 0; i < 4; ++ i)
            {
                if(roll < GObject::GObjectManager::getColorFighterChance(rate, type, i))
                {
                    color = i;
                    colors = 1;
                    break;
                }
            }
        }
        free_gold = type;
        break;
	case 2:
	case 3:
    case 4:
		color = type - 1;
        colors = 1;
        free_gold = 1;
		break;
	default:
		color = 0;
        colors = 4;
        free_gold = 0;
		break;
	}

    if(color == 3 && colors != 1)
        printf("color = %d, colors=%d\n", color, colors);
	//UInt32 fgtId = GameAction()->onTavernFlush(color);
	//if(fgtId > 0 && !pl->hasFighter(fgtId) && excepts.find(fgtId) == excepts.end())
	//{
	//	return _fighters[fgtId].fighter;
	//}

	std::map<UInt32, UInt32> idset[4];
    UInt32 bs = 0;
    for(Int8 i = 0; i < colors; ++ i)
    {
        Int8 tmpColor = color + i;
//        while(tmpColor > -1)
//        {
            idset[tmpColor] = _summonSet[free_gold][tmpColor];
            pl->exceptAvailableFighters(idset[tmpColor]);
            for(std::set<UInt32>::iterator it = excepts.begin(); it != excepts.end(); ++ it)
                idset[tmpColor].erase(*it);
            for(std::set<UInt32>::iterator it = excepts2.begin(); idset[tmpColor].size() > 6 && it != excepts2.end(); ++ it)
                idset[tmpColor].erase(*it);
/*
            if(colors == 1 && idset[tmpColor].empty())
            {
               -- tmpColor;
            }
            else
            {
                break;
            }
*/
//        }

//        if(tmpColor < 0)
//        {
//            break;
//        }

        size_t size = idset[tmpColor].size();
        if(size == 0)
            continue;

        std::map<UInt32, UInt32>::iterator it;
        for(it = idset[tmpColor].begin(); it != idset[tmpColor].end(); ++ it)
        {
            bs += it->second;
        }
    }

    if(bs == 0)
        return NULL;

    UInt32 r = uRand(bs);
    for(Int8 j = colors-1; j > -1; -- j)
    {
        size_t size = idset[color+j].size();
        if(size == 0)
            continue;

        std::map<UInt32, UInt32>::iterator it;
        for(it = idset[color+j].begin(); it != idset[color+j].end(); ++ it)
        {
            if(r < it->second)
            {
                Fighter* fighter = _fighters[it->first].fighter;
                return _fighters[it->first].fighter;
            }
            r -= it->second;
        }
    }


    return NULL;
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
    for(std::map<UInt32, GData::FighterProb>::iterator it = GData::fighterProb.begin(); it != GData::fighterProb.end(); ++ it)
    {
        UInt32 i = it->first;
        Fighter * fgt = _fighters[i].fighter;
        if(fgt == NULL) continue;
        UInt8 color = fgt->getColor();
        GData::FighterProb& prob = it->second;
        switch(color)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            if(prob.free)
                _summonSet[0][color][i] = prob.free;
            if(prob.gold)
                _summonSet[1][color][i] = prob.gold;
            break;
        }
    }
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
