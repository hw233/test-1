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
#include "HeroMemo.h"
#include "AttainMgr.h"
#include "GData/SpiritAttrTable.h"
#include "SecondSoul.h"
#include "GVar.h"

namespace GObject
{

GlobalFighters globalFighters;

static float enc_factor[] = {0, 0.05f, 0.10f, 0.16f, 0.23f, 0.31f, 0.40f, 0.51f, 0.64f, 0.80f, 1.00f, 1.25f, 1.51f};
#define SOUL_EXP_ITEM 8000
#define SOUL_SKILL_DEFAULT_ITEM 8565

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
	_id(id), _owner(owner), _class(0), _level(1), _exp(0), _pexp(0),  _pexpAddTmp(0) , _pexpMax(0), _potential(1.0f),
    _capacity(1.0f), _color(2), _hp(0), _cittaslot(CITTA_INIT), _fashion(NULL), _weapon(NULL),
    _ring(NULL), _amulet(NULL), _attrDirty(false), _maxHP(0), _bPDirty(false),
    _expFlush(false), _expMods(0), _expEnd(0), _pexpMods(0), _forceWrite(false), _battlePoint(0.0f), _praadd(0),
    _attrType1(0), _attrValue1(0), _attrType2(0), _attrValue2(0), _attrType3(0), _attrValue3(0),
    favor(0), reqFriendliness(0), strength(0), physique(0),
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
    m_2ndSoul = NULL;
    _iswboss = false;
    _wbextatk = 0;
    _wbextmagatk = 0;
}

Fighter::~Fighter()
{
	SAFE_DELETE(_fashion);
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
	if(pot < 1.499f)
		return 2;
	if(pot < 1.799f)
		return 3;
	if(pot < 2.099f)
		return 4;
	return 10;
}

UInt32 Fighter::getFashionId()
{
	return _fashion ? _fashion->getId() : 0;
}

UInt32 Fighter::getFashionTypeId()
{
	return _fashion ? _fashion->GetTypeId() : 0;
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
        if(oldLevel != _level && _level > 29 && _owner)
        {
            LevelChange data = { oldLevel, _level };
            GameMsgHdr hdr(0x1F0, WORKER_THREAD_WORLD, _owner, sizeof(LevelChange));
            GLOBAL().PushMsg(hdr, &data);
        }
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
        worldBoss.setLevel(_level);
        _owner->sendLevelPack(_level);
        _expFlush = true;
	}
	else
	{
		sendModification(3, _exp);
        _expFlush = false;
	}
	return r;
}

bool Fighter::addPExp( Int32 e, bool writedb, bool force )
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

        _pexpAddTmp += e;
    }

    if (e < 0)
    {
        SYSMSG_SENDV(2006, _owner, _color, getName().c_str(), -e);
        SYSMSG_SENDV(2007, _owner, _color, getName().c_str(), -e);
    }
    else
    {
        SYSMSG_SENDV(2004, _owner, _color, getName().c_str(), e);
        SYSMSG_SENDV(2005, _owner, _color, getName().c_str(), e);
    }

    if (_pexp >= 5000 && _owner)
        _owner->OnHeroMemo(MC_CITTA, MD_STARTED, 0, 1);
    if (_pexp >= 10000 && _owner)
        _owner->OnHeroMemo(MC_CITTA, MD_STARTED, 0, 2);

    _forceWrite = force;
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
            if(oldLevel != _level && _level > 29)
            {
                LevelChange data = { oldLevel, _level };
                GameMsgHdr hdr(0x1F0, WORKER_THREAD_WORLD, _owner, sizeof(LevelChange));
                GLOBAL().PushMsg(hdr, &data);
            }

			SYSMSG_SENDV(101, _owner, _level);
			_owner->checkLevUp(oldLevel, _level);
            /*
            if(_level >= 40)
            {
                UInt32 thisDay = TimeUtil::SharpDay();
                UInt32 firstDay = TimeUtil::SharpDay(0, PLAYER_DATA(_owner, created));
                if(thisDay == firstDay && !_owner->GetVar(VAR_CLAWARD2))
                {
                     _owner->SetVar(VAR_CLAWARD2, 1);
                     _owner->sendRC7DayInfo(TimeUtil::Now());
                }
            }
            */
		}
        worldBoss.setLevel(l);
	}
	else if(_exp != e)
	{
		_exp = e;
		sendModification(3, _exp);
	}
    _owner->sendLevelPack(_level);
}

bool Fighter::isExpFull()
{
    if (!_owner)
        return true;

    if (_exp >= getExpMax())
        return true;
    return false;
}

UInt64 Fighter::getExpMax()
{
    if (!_owner)
        return 0;
    bool isMain = _owner->isMainFighter(_id);
    if (isMain)
    {
        UInt64 maxexp = GData::expTable.getLevelMin(LEVEL_MAX);
        return maxexp;
    }
    UInt64 mexp = _owner->GetExp();
    return mexp;
}

void Fighter::updateToDB( UInt8 t, UInt64 v )
{
	const char * field = NULL;
	if(t >= 0x40 && t < 0x40 + FIGHTER_BUFF_COUNT)
	{
		if(_owner == NULL)
			return;
		if(v > 0)
			DB1().PushUpdateData("REPLACE INTO `fighter_buff`(`playerId`, `id`, `buffId`, `data`) VALUES(%"I64_FMT"u, %u, %u, %u)", _owner->getId(), _id, t - 0x40, v);
		else
			DB1().PushUpdateData("DELETE FROM `fighter_buff` WHERE `playerId` = %"I64_FMT"u AND `id` = %u AND `buffId` = %u", _owner->getId(), _id, t - 0x40);
		return;
	}
    if (t >= 0x0a && t < 0x0a+ getMaxTrumps())
    {
        UInt32 trumps[TRUMP_UPMAX] = {0};
        if (getAllTrumpId(trumps)) {
            std::string str;
            if (value2string(trumps, getMaxTrumps(), str)) {
                DB2().PushUpdateData("UPDATE `fighter` SET `trump` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
    }

	switch(t)
	{
	case 1: field = "hp"; break;
	case 2: field = "level"; break;
	case 3:
        {
#if 0
            UInt32 now = time(NULL);
            ++_expMods;
            if (_expFlush || _expMods >= 10 || now > _expEnd) // XXX: 等级变化，10次变化，10分钟
            {
#endif
                DB2().PushUpdateData("UPDATE `fighter` SET `experience` = %"I64_FMT"u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v, _id, _owner->getId());
#if 0
                _expFlush = false;
                _expMods = 0;
                _expEnd = now + 10*60;
            }
#endif
        }
		return;

	case 4:
        if(_id <= GREAT_FIGHTER_MAX && _owner != NULL)
            DB2().PushUpdateData("UPDATE `fighter` SET `potential` = %u.%02u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v / 100, v % 100, _id, _owner->getId());
        return;
    case 5:
        if(_id <= GREAT_FIGHTER_MAX && _owner != NULL)
            DB2().PushUpdateData("UPDATE `fighter` SET `capacity` = %u.%02u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v / 100, v % 100, _id, _owner->getId());
        return;
	case 6:
        {
            ++_pexpMods;
            if (_pexpMods >= 3 || _forceWrite) // XXX: 半小时一次
            {
                DB2().PushUpdateData("UPDATE `fighter` SET `practiceExp` = %"I64_FMT"u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", v, _id, _owner->getId());
                _pexpMods = 0;
                _forceWrite = false;
            }

            if(_pexpAddTmp > 0)
            {
               if(CURRENT_THREAD_ID() <= WORKER_THREAD_NEUTRAL)
                      GameAction()->doAttainment( _owner ,Script::AddPExp , _pexpAddTmp );
               _pexpAddTmp = 0;
            }
            return;
        }
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
                DB2().PushUpdateData("UPDATE `fighter` SET `acupoints` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x31: // peerless
        break;
    case 0x32: // cittaslot
        // DB2().PushUpdateData("UPDATE `fighter` SET `cittaslot` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _cittaslot, _id, _owner->getId());
        break;
    case 0x2a:
        { // skill
            std::string str;
            if (value2string(_skill, getUpSkillsMax(), str)) {
                DB2().PushUpdateData("UPDATE `fighter` SET `skill` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x2b:
        { // skills
            if (_skills.size()) {
                std::string str;
                if (value2string(&_skills[0], _skills.size(), str)) {
                    DB2().PushUpdateData("UPDATE `fighter` SET `skills` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
                }
            }
            else
            {
                DB2().PushUpdateData("UPDATE `fighter` SET `skills` = '' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _id, _owner->getId());
            }
        }
        break;
    case 0x2c:
        { // citta
            std::string str;
            if (value2string(_citta, getUpCittasNum(), str)) {
                DB2().PushUpdateData("UPDATE `fighter` SET `citta` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
            }
        }
        break;
    case 0x2d:
        { // cittas
            if (_cittas.size()) {
                std::string str;
                if (value2string(&_cittas[0], _cittas.size(), str)) {
                    DB2().PushUpdateData("UPDATE `fighter` SET `cittas` = '%s' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), _id, _owner->getId());
                }
            }
            else
            {
                DB2().PushUpdateData("UPDATE `fighter` SET `cittas` = '' WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _id, _owner->getId());
            }
        }
        break;
    case 0x2e: // passive
        break;

	case 0x20: field = "fashion"; break;
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
			DB2().PushUpdateData("UPDATE `fighter` SET `%s` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", field, v, _id, _owner->getId());
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

void Fighter::sendModification( UInt8 t, UInt16 value, int idx, bool writedb)
{
	if(_owner == NULL)
		return;
	Stream st(REP::CHANGE_EQUIPMENT);
	st << getId() << static_cast<UInt8>(1) << t;
    if (t == 0x30)
    {
        st << static_cast<UInt32>(value);
        if (writedb)
            updateToDB(t, value);
    }
    else
    {
        st << static_cast<UInt8>(idx) << value;
        if (writedb)
            updateToDB(t, 0);
    }
	st << Stream::eos;
	_owner->send(st);
}

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

            UInt8 itemClass = equip->getClass();
            UInt8 q = equip->getQuality();
            if(itemClass >= Item_Weapon && itemClass <= Item_Ring && q == 5)
            {
                ItemEquipSpiritAttr& esa = equip->getEquipSpiritAttr();
                esa.appendAttrToStream(st);
            }

            if(equip->getClass() == Item_Trump || equip->getClass() == Item_Fashion)
            {
                st << ied.maxTRank << ied.trumpExp;
            }
			if(writedb)
				updateToDB(t[i], equip->getId());
		}
	}
	st << Stream::eos;
	_owner->send(st);
}

ItemEquip * Fighter::setFashion( ItemFashion* r, bool writedb )
{
	ItemEquip * rr = _fashion;
	_fashion = r;
	if(writedb)
	{
		_attrDirty = true;
		_bPDirty = true;
		if(r != NULL)
		{
			r->DoEquipBind(true);
		}
		sendModification(0x20, r);
	}
	return rr;
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
            CheckEquipEnchantAttainment( w->getItemEquipData().enchant);
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
            CheckEquipEnchantAttainment( a->getItemEquipData().enchant);
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
    if (!_owner)
        return false;

    ItemEquip* ie = _owner->GetPackage()->GetEquip(trumpid);
    if (!ie)
        return false;

    bool pass = (ie->GetItemType().getId() >= 1500 && ie->GetItemType().getId() <= 1599);
    for (UInt8 i = 0; i < TRUMP_UPMAX; ++i)
    {
        if (_trump[i])
        {
            if (pass && _trump[i]->GetItemType().getId() >= 1500 && _trump[i]->GetItemType().getId() <= 1599)
                return false;
            if (_trump[i]->GetItemType().getId() == ie->GetItemType().getId())
                return true;
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

    if (_potential + 0.005f >= 1.5f && _capacity >= 7 && idx == 1)
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

UInt32  Fighter:: getTrumpNum()
{

    UInt32 num = 0;
     for (int i = 0; i < getMaxTrumps(); ++i)
     {
        if (_trump[i])
            num ++ ;
     }
     return num;
}
ItemEquip* Fighter::setTrump( ItemEquip* trump, int idx, bool writedb )
{
    ItemEquip* t = 0;
    if (!trump || canSetTrump(idx, trump->getId()))
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
            if (t)
            {
                const GData::AttrExtra* attr = t->getAttrExtra();
                if (attr)
                {
                    delSkillsFromCT(attr->skills, writedb);
                }
            }

            _trump[idx] = trump;
            if (trump)
                trump->DoEquipBind(true);

            if (trump)
            { // up
                const GData::AttrExtra* attr = trump->getAttrExtra();
                if (attr)
                {
                    addSkillsFromCT(attr->skills, writedb, true);
                }
            }

            _attrDirty = true;
            _bPDirty = true;

            if(writedb  &&  getTrumpNum() == 2)
            {
                //判断穿法宝的成就
                GameAction()->doAttainment(_owner, 10211, 0);
            }

            sendModification(0x0a+idx, _trump[idx], writedb);

            if (trump && writedb && _owner && trump->GetItemType().getId() >= 1600 && trump->GetItemType().getId() <= 1699)
                _owner->OnHeroMemo(MC_SKILL, MD_ADVANCED, 0, 0);
            if (trump && writedb && _owner && trump->GetItemType().getId() >= 1500 && trump->GetItemType().getId() <= 1599)
                _owner->OnHeroMemo(MC_SKILL, MD_MASTER, 0, 0);
            if (trump && writedb && _owner)
            {
                UInt8 cnt  = 0;
                for (UInt8 i = 0; i < TRUMP_UPMAX; ++i)
                {
                    if (_trump[i])
                    {
                        if (_trump[i]->GetItemType().getId() >= 1500 && _trump[i]->GetItemType().getId() <= 1599)
                            ++cnt;
                    }
                }
                if (cnt >= 2)
                    _owner->OnHeroMemo(MC_SKILL, MD_MASTER, 0, 2);
            }
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

void Fighter::setCurrentHP( UInt32 hp, bool writedb )
{
	if(_hp && _hp == hp)
		return;

	if(writedb)
	{
		UInt32 maxhp = getMaxHP();
		if(hp >= maxhp)
		{
			//if(_hp == 0)
			//	return;
			hp = 0;
		}
		_hp = hp;
		sendModification(1, hp);
	}
	else
		_hp = hp;
}

void Fighter::addHPPercent( UInt8 p, bool writedb )
{
    UInt32 maxhp = getMaxHP();
    if (!_hp)
        _hp = maxhp;
    UInt32 hp = (p / (float)100) * maxhp;
    _hp += hp;
    if (_hp > maxhp)
        _hp = maxhp;

    if (writedb)
        sendModification(1, _hp);
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
        ae.hp += value;
        break;
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

inline void addTalentAttr( GData::AttrExtra& ae, UInt8 type, UInt16 value )
{
    if (!type)
        return;

	switch(type)
	{
    case 1:
        ae.strengthP += ((double)value/10000.f);
        break;
    case 2:
        ae.physiqueP += ((double)value/10000.f);
        break;
	case 3:
        ae.agilityP += ((double)value/10000.f);
		break;
	case 4:
        ae.intelligenceP += ((double)value/10000.f);
		break;
	case 5:
        ae.willP += ((double)value/10000.f);
		break;
    case 6:
        ae.tough += ((double)value/100.f);
        break;
    case 7:
        ae.actionP += ((double)value/10000.f);
        break;
    case 8:
        ae.hitrate += ((double)value/100.f);
        break;
    case 9:
        ae.evade += ((double)value/100.f);
        break;
    case 10:
        ae.critical += ((double)value/100.f);
        break;
    case 11:
        ae.criticaldmg += ((double)value/10000.f);
        break;
    case 12:
        ae.pierce += ((double)value/100.f);
        break;
    case 13:
        ae.counter += ((double)value/100.f);
        break;
    case 14:
        ae.magres += ((double)value/100.f);
        break;
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

inline void addEquipSpiritAttr( GData::AttrExtra& ae, const ItemEquipSpiritAttr& esa, ItemClass	itemClass )
{
    UInt16 lev0 = esa.spLev[0];
    UInt16 lev1 = esa.spLev[1];
    UInt16 lev2 = esa.spLev[2];
    UInt16 lev3 = esa.spLev[3];
    switch(itemClass)
    {
    case Item_Weapon:
        if(lev0 > 0)
        {
            ae.attack += GData::spiritAttrTable[lev0-1].attack;
            ae.magatk += GData::spiritAttrTable[lev0-1].attack;
        }
        if(lev1 > 0)
            ae.pirlvl += GData::spiritAttrTable[lev1-1].pierce_lvl;
        if(lev2 > 0)
            ae.crilvl += GData::spiritAttrTable[lev2-1].critical_lvl;
        if(lev3 > 0)
            ae.action += GData::spiritAttrTable[lev3-1].action;
        break;
    case Item_Armor1:
        if(lev0 > 0)
        {
            ae.defend += GData::spiritAttrTable[lev0-1].defend;
            ae.magdef += GData::spiritAttrTable[lev0-1].defend;
        }
        if(lev1 > 0)
            ae.action += GData::spiritAttrTable[lev1-1].action;
        if(lev2 > 0)
            ae.pirlvl += GData::spiritAttrTable[lev2-1].pierce_lvl;
        if(lev3 > 0)
            ae.hp += GData::spiritAttrTable[lev3-1].hp;
        break;
    case Item_Armor3:
        if(lev0 > 0)
        {
            ae.defend += GData::spiritAttrTable[lev0-1].defend;
            ae.magdef += GData::spiritAttrTable[lev0-1].defend;
        }
        if(lev1 > 0)
            ae.action += GData::spiritAttrTable[lev1-1].action;
        if(lev2 > 0)
            ae.crilvl += GData::spiritAttrTable[lev2-1].critical_lvl;
        if(lev3 > 0)
            ae.hp += GData::spiritAttrTable[lev3-1].hp;
        break;
    case Item_Armor2:
    case Item_Armor4:
    case Item_Armor5:
        if(lev0 > 0)
        {
            ae.defend += GData::spiritAttrTable[lev0-1].defend;
            ae.magdef += GData::spiritAttrTable[lev0-1].defend;
        }
        if(lev1 > 0)
            ae.action += GData::spiritAttrTable[lev1-1].action;
        if(lev2 > 0)
            ae.toughlvl += GData::spiritAttrTable[lev2-1].tough_lvl;
        if(lev3 > 0)
            ae.hp += GData::spiritAttrTable[lev3-1].hp;
        break;
    case Item_Amulet:
        if(lev0 > 0)
        {
            ae.attack += GData::spiritAttrTable[lev0-1].attack;
            ae.magatk += GData::spiritAttrTable[lev0-1].attack;
        }
        if(lev1 > 0)
            ae.hp += GData::spiritAttrTable[lev1-1].hp;
        if(lev2 > 0)
            ae.pirlvl += GData::spiritAttrTable[lev2-1].pierce_lvl;
        if(lev3 > 0)
            ae.criticaldmg += ((double)GData::spiritAttrTable[lev3-1].critical_dmg/100.f);
        break;
    case Item_Ring:
        if(lev0 > 0)
        {
            ae.attack += GData::spiritAttrTable[lev0-1].attack;
            ae.magatk += GData::spiritAttrTable[lev0-1].attack;
        }
        if(lev1 > 0)
            ae.hp += GData::spiritAttrTable[lev1-1].hp;
        if(lev2 > 0)
            ae.crilvl += GData::spiritAttrTable[lev2-1].critical_lvl;
        if(lev3 > 0)
            ae.criticaldmg += ((double)GData::spiritAttrTable[lev3-1].critical_dmg/100.f);
        break;
    default:
        return;
    }
}

inline void testEquipInSet(UInt32 * setId, UInt32 * setNum, UInt32 id)
{
    if(id - 1750 < 8)
    {
        id = 1;
    }
    else
    {
        if (id < 2000)
            return;

        id -= 2000;
        id /= 8;
    }

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
    if (!ce)
        return;
	addAttrExtra(_attrExtraEquip, ce);
	addEquipAttr2(_attrExtraEquip, ce, _level);
}

void Fighter::addAttr( ItemEquip * equip )
{
    if (!equip)
        return;
	addAttrExtra(_attrExtraEquip, equip->getAttrExtra());
	addEquipAttr2(_attrExtraEquip, equip->getEquipAttr2(), _level);
    addEquipSpiritAttr(_attrExtraEquip, equip->getEquipSpiritAttr(), equip->getClass());
	ItemEquipData& ied = equip->getItemEquipData();
	for(UInt8 i = 0; i < ied.sockets; ++ i)
	{
		if(ied.gems[i] != 0)
		{
            GData::ItemGemType * igt = NULL;
			igt = GData::gemTypes[ied.gems[i] - LGEM_ID];
			addAttrExtra(_attrExtraEquip, igt->attrExtra);
		}
	}
}

void Fighter::addTrumpAttr( ItemEquip* trump )
{
    if (!trump)
        return;
    const GData::AttrExtra* pae = trump->getAttrExtra();
    if (!pae)
        return;
    GData::AttrExtra ae(*pae);
	ItemEquipData& ied = trump->getItemEquipData();

    UInt8 q = trump->getQuality();
    UInt8 l = ied.tRank;

    if(l > 0 && q > 1)
    {
        AttrFactor af = GObjectManager::getTrumpTRankFactor(q-2, l-1);
        if(trump->GetItemType().getId() < 1600)
            af.aura = 0;
        else
            af.auraMax = 0;

        ae *= af;

    }

	addAttrExtra(_attrExtraEquip, &ae);

	addEquipAttr2(_attrExtraEquip, trump->getEquipAttr2(), _level);
}

void Fighter::rebuildEquipAttr()
{
	_attrExtraEquip.reset();

	UInt32 setId[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	UInt32 setNum[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	ItemEquip * equip = getFashion();
    if (equip != NULL)
    { // XXX: like trump
        addTrumpAttr(equip);
    }

	equip = getWeapon();
	if(equip != NULL)
	{
		if(equip->getQuality() >= 4)
			testEquipInSet(setId, setNum, equip->GetItemType().getId());
		addAttr(equip);

        const GData::AttrExtra * ext = equip->getAttrExtra();

        _attrExtraEquip.attack += ext->attack * enc_factor[equip->getItemEquipData().enchant];
        _attrExtraEquip.magatk += ext->magatk * enc_factor[equip->getItemEquipData().enchant];
	}

	for(int i = 0; i < 5; ++ i)
	{
		equip = getArmor(i);
		if(equip != NULL)
		{
			if(equip->getQuality() >= 4)
				testEquipInSet(setId, setNum, equip->GetItemType().getId());
			addAttr(equip);

            const GData::AttrExtra * ext = equip->getAttrExtra();
            _attrExtraEquip.defend += ext->defend * enc_factor[equip->getItemEquipData().enchant];
            _attrExtraEquip.magdef += ext->magdef * enc_factor[equip->getItemEquipData().enchant];
		}
	}

	equip = getRing();
	if(equip != NULL)
	{
		if(equip->getQuality() >= 4)
			testEquipInSet(setId, setNum, equip->GetItemType().getId());
		addAttr(equip);

        _attrExtraEquip.hp += GObjectManager::getRingHpFromEnchant(equip->getValueLev(), equip->GetCareer(), equip->getItemEquipData().enchant);
	}

	equip = getAmulet();
	if(equip != NULL)
	{
		if(equip->getQuality() >= 4)
			testEquipInSet(setId, setNum, equip->GetItemType().getId());
		addAttr(equip);
        _attrExtraEquip.hp += GObjectManager::getRingHpFromEnchant(equip->getValueLev(), equip->GetCareer(), equip->getItemEquipData().enchant);
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
		int idx = setNum[i] / 2 - 1;
        while(idx >= 0)
        {
            _attrExtraEquip += *iest->attrExtra[idx];
            --idx;
        }
		//_attrExtraEquip += *iest->attrExtra[idx];
	}
#if 0
	_attrExtraEquip.attack += getWeaponAttack();
	_attrExtraEquip.magatk += getWeaponAttack();
	UInt16 armorDefend, armorHP;
	getArmorDefendAndHP(armorDefend, armorHP);
	_attrExtraEquip.defend += armorDefend;
	_attrExtraEquip.magdef += armorDefend;
	_attrExtraEquip.hp += armorHP;
#endif

    addTalentAttr(_attrExtraEquip, getAttrType1(), getAttrValue1());
    addTalentAttr(_attrExtraEquip, getAttrType2(), getAttrValue2());
    addTalentAttr(_attrExtraEquip, getAttrType3(), getAttrValue3());

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

    bool hasActiveTrump = false;
    for(int i = 0; i < getMaxTrumps(); ++i)
    {
		ItemTrump* trump = static_cast<ItemTrump*>(getTrump(i));

		if(trump != NULL)
        {
            if(!hasActiveTrump && trump->getId() >= 1600)
            {
                addTrumpAttr(trump);
                hasActiveTrump = true;
            }
            else
            {
                addTrumpAttr(trump);
            }
        }
    }

    if (_owner)
    {
        // 帮派秘术对额外属性的加成
        _attrExtraEquip.hp += _owner->getClanSkillHPEffect();
        _attrExtraEquip.attack += _owner->getClanSkillAtkEffect();
        _attrExtraEquip.defend += _owner->getClanSkillDefendEffect();
        _attrExtraEquip.magatk += _owner->getClanSkillMagAtkEffect();
        _attrExtraEquip.magdef += _owner->getClanSkillMagDefentEffect();
        _attrExtraEquip.action += _owner->getClanSkillActionEffect();
        _attrExtraEquip.hitrlvl += _owner->getClanSkillHitrLvlEffect();
#if 0
        const GData::AttrExtra* ae = _owner->getHIAttr();
        if (ae)
            addAttrExtra(_attrExtraEquip, ae);
#endif
    }

    if(m_2ndSoul)
    {
        m_2ndSoul->addAttr(_attrExtraEquip);
    }

    if (isWBoss())
    {
        _attrExtraEquip.attack += _wbextatk;
        _attrExtraEquip.magatk += _wbextmagatk;
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
	fgt->_fashion = NULL;
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
    if(_fashion != NULL && _fashion->getId() == id)
    {
        pos = 0;
        return _fashion;
    }
	if(_weapon != NULL && _weapon->getId() == id)
	{
        pos = 1;
		return _weapon;
	}
	for(int i = 0; i < 5; ++ i)
	{
		if(_armor[i] != NULL && _armor[i]->getId() == id)
		{
			pos = i + 2;
			return _armor[i];
		}
	}
	if(_amulet != NULL && _amulet->getId() == id)
	{
		pos = 7;
		return _amulet;
	}
	if(_ring != NULL && _ring->getId() == id)
	{
		pos = 8;
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
        if (_fashion == equip)
        {
            _fashion = NULL;
			sendModification(0x20, NULL, true);
			found = true;
        }
        break;
	case 1:
		if(_weapon == equip)
		{
			_weapon = NULL;
			sendModification(0x21, NULL, true);
			found = true;
		}
		break;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		{
			UInt8 i = pos - 2;
			if(_armor[i] == equip)
			{
				_armor[i] = NULL;
				sendModification(0x22 + i, NULL, true);
				found = true;
			}
		}
		break;
	case 7:
		if(_amulet == equip)
		{
			_amulet = NULL;
			sendModification(0x27, NULL, true);
			found = true;
		}
		break;
	case 8:
		if(_ring == equip)
		{
			_ring = NULL;
			sendModification(0x28, NULL, true);
			found = true;
		}
		break;
	}
	if(found)
	{
		_attrDirty = true;
		_bPDirty = true;
		DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", equip->getId());
		DB4().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", equip->getId());
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
    if (_fashion != NULL)
        fgt->_fashion = new ItemFashion(*_fashion);
    else
        fgt->_fashion = NULL;
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
    UInt8 skills = getUpSkillsNum();
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

void Fighter::getAllPSkillAndLevel4Arena(Stream& st)
{
    UInt8 size = 0;
    for (size_t i = 0; i < GData::SKILL_PASSIVES-GData::SKILL_PASSSTART; ++i)
    {
        size = _passkl[i].size();
        st << size;
        for (size_t j = 0; j < _passkl[i].size(); ++j)
        {
            st << _passkl[i][j];
        }
    }
    for (size_t i = 0; i < GData::SKILL_PASSIVES-GData::SKILL_PASSSTART; ++i)
    {
        size = _rpasskl[i].size();
        st << size;
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
            // st << static_cast<UInt8>(i);
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
        if (SKILL_ID(_peerless[i]) == SKILL_ID(pl))
            return static_cast<int>(i);
    }
    return -1;
}

bool Fighter::addNewPeerless( UInt16 pl, bool writedb, bool up )
{
    int op = 0;
    int idx = hasPeerless(pl);
    if (idx >= 0)
    {
        if (pl != _peerless[idx])
        { // upgrade
            _peerless[idx] = pl;
            if (isPeerlessUp(pl) || up)
            {
                upPeerless(pl, writedb);
                if (up)
                    op = 1;
                else
                    op = 3;
            }
            else
                op = 3;
        }
        else
            return false;
    }
    else
    {
        idx = static_cast<int>(_peerless.size());
        _peerless.push_back(pl);
        if (up)
            upPeerless(pl, writedb);
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
    if (_owner && writedb)
        _owner->OnHeroMemo(MC_SKILL, MD_ADVANCED, 0, 1);
    sendModification(0x30, peerless, 0, writedb);
}

UInt8 Fighter::getAcupointCnt()
{
    UInt8 c = 0;
    for (UInt8 i = 0; i < ACUPOINTS_MAX; ++i)
    {
        if (!_acupoints[i])
            break;
        ++c;
    }
    return c;
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
    UInt8 vMax =  getAcupointsCntMax();
    if (idx >= 0  && idx < ACUPOINTS_MAX && v <= vMax)
    {
        if (_acupoints[idx] >= v)
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

            _acupoints[idx] = v;
            if (_acupoints[idx] < 3)
                ++_praadd; // 第3层不加

            if (_owner && writedb)
                _owner->OnHeroMemo(MC_CITTA, MD_STARTED, 1, 0);
            if (_owner && writedb && idx == 1 && _acupoints[idx] == 3)
                _owner->OnHeroMemo(MC_CITTA, MD_STARTED, 1, 1);
            if (_owner && writedb && idx == 2 && _acupoints[idx] == 3)
                _owner->OnHeroMemo(MC_CITTA, MD_STARTED, 1, 2);
        }
        else
        {
            _acupoints[idx] = v;
        }

        soulMax += pap->soulmax;

        if(!init)
        //增加元神力后 查看成就
            GameAction()->doAttainment(this->_owner, Script::AddSoulMax , soulMax);
        _pexpMax += pap->pramax;
        _cittaslot += pap->citslot;

        _attrDirty = true;
        _bPDirty = true;
        sendModificationAcupoints(0x29, idx, writedb);
        sendModification(7, _pexpMax);
        sendModification(9, getMaxSoul() );
        sendModification(0x32, getUpCittasMax());
        if(!init && v ==vMax )
            GameAction()->doAttainment(this->_owner, Script::AddAcupoint, idx); //增加穴道的成就
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
        upSkill(::atoi(tk[i].c_str()), i, writedb, false);
    }
}

int Fighter::hasSkill( UInt16 skill )
{
    for (size_t i = 0; i < _skills.size(); ++i)
    {
        if (SKILL_ID(_skills[i]) == SKILL_ID(skill))
            return static_cast<int>(i);
    }
    return -1;
}

int Fighter::isSkillUp(UInt16 skill)
{
    if (!SKILL_ID(skill))
        return -1;
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (SKILL_ID(skill) == SKILL_ID(_skill[i]))
            return i;
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

bool Fighter::testMutual( UInt16 skill )
{
    UInt16 mutualSkills[] = {
        1,5,
        2,6,
        3,7,
        10,28,
        11,15,
        11,18,
        12,16,
        15,18,
        19,23,
        20,24,
        21,25,
    };

    UInt16 j = 0;
    UInt16 id = SKILL_ID(skill);
    for (UInt16 i = 0; i < sizeof(mutualSkills)/sizeof(UInt16); ++i)
    {
        if (mutualSkills[i] != id)
            continue;

        if (i % 2)
            j = i - 1;
        else
            j = i + 1;

        if (isSkillUp(SKILLANDLEVEL(mutualSkills[j], 0)) >= 0)
        {
            if(_owner != NULL)
                _owner->sendMsgCode(0, 1703);
            return true;
        }
    }

    return false;
}

bool Fighter::upSkill( UInt16 skill, int idx, bool writedb, bool online )
{
    if (!skill)
        return false;
    if (!(idx >= 0 && idx < getUpSkillsMax())) // dst
        return false;

    if (hasSkill(skill) < 0)
        return false;

    if (testMutual(skill))
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
                sendModification(0x2a, _skill[j], j, false);
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

        SSSendSSInfo(skill);
    }
    else
    {
        if (src != idx)
        {
            //if (_skill[idx])
            { // swap
                sendModification(0x2a, _skill[idx], src, false);

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
        sendModification(0x2a, skill, idx, writedb);

        if (online)
        {
            if (_owner && getUpSkillsNum() == 1)
                _owner->OnHeroMemo(MC_SKILL, MD_STARTED, 0, 0);
            else if (_owner && getUpSkillsNum() == 2)
                _owner->OnHeroMemo(MC_SKILL, MD_STARTED, 0, 1);
            else if (_owner && getUpSkillsNum() == 3)
                _owner->OnHeroMemo(MC_SKILL, MD_STARTED, 0, 2);
        }
    }

    return ret;
}

bool Fighter::offSkill( UInt16 skill, bool writedb )
{
    int idx = isSkillUp(skill);
    if (idx < 0)
        return false;

    _attrDirty = true;
    _bPDirty = true;
#if 1
    UInt8 max = getUpSkillsMax();
    int i = idx;
    for (; i < max - 1; ++i)
    {
        _skill[i] = _skill[i+1];
        _skill[i+1] = 0;
        sendModification(0x2a, _skill[i], i, false);
    }
    _skill[i] = 0;
    sendModification(0x2a, 0, i, writedb);
#else
    _skill[idx] = 0;
    sendModification(0x2a, 0, idx, writedb);
#endif
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
        sendModification(0x2a, nskill, idx, writedb);
    }

    _attrDirty = true;
    _bPDirty = true;
    if (sync)
        sendModification(0x2b, skill, 3/*1add,2del,3mod*/, writedb);
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
        sendModification(0x2b, skill, 2/*1add,2del,3mod*/, writedb);
    return true;
}

void Fighter::setSkills( std::string& skills, bool writedb )
{
    if (!skills.length())
        return;

    StringTokenizer tk(skills, ",");

    {
        //addNewSkill(::atoi(tk[i].c_str()), writedb);
    }

    bool up = false;
    if (_id >= 999)
        up = true;

    const GData::SkillBase* s  = 0;
    std::vector<const GData::SkillBase*> vt_skills;
    for (size_t i = 0; i < tk.count(); ++i)
    {
        s = GData::skillManager[::atoi(tk[i].c_str())];
        if (s)
        {
            vt_skills.push_back(s);
        }
    }

    if (vt_skills.size())
        addSkillsFromCT(vt_skills, writedb, up, false);
}

bool Fighter::addNewSkill( UInt16 skill, bool writedb, bool up, bool online )
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
            if (i >= 0 || up)
            {
                upSkill(skill, i, writedb, online);
                if (up)
                    op = 1;
                else
                    op = 3;
            }
        }
        else
            return false;
    }
    else
    {
        idx = static_cast<int>(_skills.size());
        _skills.push_back(skill);
        if (up)
        {
            UInt8  max = getUpSkillsMax();
            UInt16 i = getUpSkillsNum();
            if(i < max)
                upSkill(skill, i, writedb, online);
        }
        op = 1;
    }

    idx = isSkillUp(skill); // XXX: hack
    if (idx >= 0) {
        _skill[idx] = skill;
        sendModification(0x2a, skill, idx, writedb);
    }

    sendModification(0x2b, skill, op, writedb);
    return true;
}

void Fighter::setUpCittas( std::string& citta, bool writedb )
{
    if (!citta.length())
        return;

    StringTokenizer tk(citta, ",");
    for (size_t i = 0; i < tk.count(); ++i)
    {
        upCitta(::atoi(tk[i].c_str()), i, writedb, false, false);
    }
}

int Fighter::isCittaUp( UInt16 citta )
{
    for (int i = 0; i < getUpCittasMax(); ++i)
    {
        if (CITTA_ID(citta) == CITTA_ID(_citta[i]))
            return i;
    }
    return -1;
}

bool Fighter::upCitta( UInt16 citta, int idx, bool writedb, bool lvlup, bool online )
{
    if (!citta)
        return false;
    const GData::CittaBase* cb = GData::cittaManager[citta];
    if (!cb)
        return false;

    // XXX: 只能装备3个主动技能
    if (!lvlup && cb->effect && cb->effect->skill.size() && getUpSkillsNum() >= 3)
        return false;

    int cidx = hasCitta(citta);
    if (cidx < 0)
        return false;
    if (!lvlup && _cittas[cidx] != citta)
        return false;

    int op = 0;
    bool ret = false;
    int src = isCittaUp(citta);
    if (src < 0)
    {
        if (cb->needsoul > getMaxSoul() - getSoul())
        {
            _owner->sendMsgCode(0, 1057);
            return false;
        }

        idx = getUpCittasNum();
        if (!(idx >= 0 && idx < getUpCittasMax())) // dst
            return false;

        if (_citta[idx])
            offCitta(_citta[idx], false, true, writedb);

        _citta[idx] = citta;
        ret = true;
        op = 1;
    }
    else
    {
        idx = src;
        if (_citta[idx] != citta) // upgrade
        {
            const GData::CittaBase* yacb = GData::cittaManager[_citta[idx]];
            if (!yacb)
                return false;
            if (cb->needsoul > getMaxSoul() - (getSoul() - yacb->needsoul))
                return false;

            // XXX: do not send message to client
            offCitta(_citta[idx], false, false, writedb); // delete skills was taken out by old citta first
            _citta[idx] = citta;
            ret = true;
            op = 3;
        }
    }

    if (ret)
    {
        _attrDirty = true;
        _bPDirty = true;
        sendModification(0x2c, citta, op/*1add,2del,3mod*/, writedb);

        if(online && writedb && CURRENT_THREAD_ID() <= WORKER_THREAD_NEUTRAL)
            //装备上心法成就
            GameAction()->doAttainment(_owner, 10084,getUpCittasNum());

        if (online && _owner && writedb)
            _owner->OnHeroMemo(MC_CITTA, MD_ADVANCED, 0, 1);
    }

    if (ret)
    {
        bool up = true;//_owner?(_owner->getMainFighter()?_owner->getMainFighter()->getLevel()>=10:true):false;
        /*
        if (!writedb)
            up = false;
            */
        addSkillsFromCT(skillFromCitta(citta), writedb, up, online);

        {
            soul += cb->needsoul;
            if (cb->needsoul)
                sendModification(0x8, soul);
        }
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
        if (_cittas[i] != citta)
            return false;
        bool re =  addNewCitta(citta+1, writedb, false);

        if(re )
        {
            //升级心法成功
            UInt16 curLev = CITTA_LEVEL(citta + 1);
            if(curLev == 9)
            {
                if (_owner && writedb)
                    _owner->OnHeroMemo(MC_CITTA, MD_ADVANCED, 0, 2);

                //check lev9 attain
                GameAction()->doAttainment(_owner, 10071,cb->type);

                UInt32 num9 = 0;     //所有心法9级的
                UInt32 num9Type8 = 0;
                for (size_t i = 0; i < _cittas.size(); ++i)
                {
                    if(CITTA_LEVEL(_cittas[i]) >= 9 )
                    {
                        const GData::CittaBase* cb = GData::cittaManager[_cittas[i]];
                        if(cb)
                        {
                            num9 ++;
                            if(cb->type >= 8)
                                num9Type8 ++;
                        }
                    }
                }
                if(num9>=6)
                {
                     GameAction()->doAttainment(_owner, 10073, num9);
                }
                if(num9Type8 >=3)
                {
                     GameAction()->doAttainment(_owner,  10086, num9Type8);
                }
            }

        }
        return re;
    }
    else
    {
		if(_owner != NULL) ;
            SYSMSG_SENDV(2008, _owner);
    }
    return false;
}

void Fighter::delSkillsFromCT(const std::vector<const GData::SkillBase*>& skills, bool writedb)
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
                    delPeerless(s->getId(), writedb);
                else if (s->cond == GData::SKILL_ACTIVE)
                    delSkill(s->getId(), writedb);
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
                    offPassiveSkill(s->getId(), s->cond, s->prob>=100.0f, writedb);
                }
                else
                {
                    // error
                }
            }
        }
    }
}

void Fighter::addSkillsFromCT(const std::vector<const GData::SkillBase*>& skills, bool writedb, bool up, bool online)
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
                    addNewPeerless(s->getId(), writedb, up);
                else if (s->cond == GData::SKILL_ACTIVE)
                    addNewSkill(s->getId(), writedb, up, online);
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
    UInt16 idx = type - GData::SKILL_PASSSTART;
    if (p100)
    { // 100%
        for (size_t j = 0; j < _passkl[idx].size(); ++j)
        {
            if (SKILL_ID(_passkl[idx][j]) == SKILL_ID(skill))
            {
                ret = true;
                if (skill != _passkl[idx][j])
                { // upgrade
                    _passkl[idx][j] = skill;
                    sendModification(0x2e, skill, 3/*1add,2del,3mod*/, writedb);
                }
                break;
            }
        }

        if(!ret)
        {  // up
            ret = true;
            _passkl[idx].push_back(skill);
            sendModification(0x2e, skill, 1/*1add,2del,3mod*/, writedb);
        }
    }
    else
    {
        for (size_t j = 0; j < _rpasskl[idx].size(); ++j)
        {
            if (SKILL_ID(_rpasskl[idx][j]) == SKILL_ID(skill))
            {
                ret = true;
                if (skill != _rpasskl[idx][j])
                { // upgrade
                    _rpasskl[idx][j] = skill;
                    sendModification(0x2e, skill, 3/*1add,2del,3mod*/, writedb);
                    break;
                }
            }
        }

        if (!ret)
        { // up
            ret = true;
            _rpasskl[idx].push_back(skill);
            sendModification(0x2e, skill, 1/*1add,2del,3mod*/, writedb);
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
        for (size_t j = 0; j < _rpasskl[idx].size(); ++j)
        {
            if (SKILL_ID(_rpasskl[idx][j]) == SKILL_ID(skill))
            { // off
                std::vector<UInt16>::iterator i = _rpasskl[idx].begin();
                std::advance(i, j);
                _rpasskl[idx].erase(i);
                ret = true;
            }
        }
    }

    if (ret) // off
        sendModification(0x2e, skill, 2/*1add,2del,3mod*/, writedb);

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
        if (CITTA_ID(_cittas[i]) == CITTA_ID(citta))
            return static_cast<int>(i);
    }
    return -1;
}

bool Fighter::addNewCitta( UInt16 citta, bool writedb, bool init, bool split )
{
    if (!citta)
        return false;
    if (_cittas.size() >= 40)
    {
        if (_owner)
            _owner->sendMsgCode(0, 1704);
        return false;
    }
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
                upCitta(citta, i, writedb, true, init?false:true);
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

    if (!init && cb->pexp)
        addPExp(-cb->pexp, writedb);

    _attrDirty = true;
    _bPDirty = true;
    sendModification(0x2d, citta, op/*1add,2del,3mod*/, writedb);

    if (!init && _owner && writedb)
        _owner->OnHeroMemo(MC_CITTA, MD_ADVANCED, 0, 0);

    if(!init && CURRENT_THREAD_ID() <= WORKER_THREAD_NEUTRAL)
    {
    //获取心法
        GameAction()->doAttainment(this->_owner, 10061, getCittasNum());
        GameAction()->doAttainment(this->_owner, 10074, cb->type);
    }
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
    if (skills.size() && offskill)
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
            sendModification(0x8, soul);
    }

    _attrDirty = true;
    _bPDirty = true;

    int i = idx;
    if (flip) {
        for (; i < getUpCittasNum(); ++i)
        {
            _citta[i] = _citta[i+1];
            _citta[i+1] = 0;
            //if (_citta[i])
            //    sendModification(0x2c, _citta[i], i, writedb);
        }
    }
    //sendModification(0x2c, 0, i, writedb);
    if (flip)
        sendModification(0x2c, citta, 2/*1add,2del,3mod*/, writedb);
    return true;
}

void Fighter::delAllCitta( bool writedb )
{
    std::vector<UInt16> cittas = _cittas;
    for (size_t i = 0; i < cittas.size(); ++i)
    {
        delCitta(cittas[i], writedb);
    }
}

bool Fighter::delCitta( UInt16 citta, bool writedb )
{
    int idx = hasCitta(citta);
    if (idx < 0)
        return false;

    std::vector<UInt16>::iterator it = _cittas.begin();
    std::advance(it, idx);

    if (*it != citta)
        return false;

    offCitta(citta, true, true, writedb);

    *it = 0;
    _cittas.erase(it);

    {
        const GData::CittaBase* cb = GData::cittaManager[citta];
        const GData::CittaBase* yacb = cb;
        if (cb) {
            UInt32 exp = cb->pexp;
            UInt8 id = CITTA_ID(citta);
            UInt8 lvl = CITTA_LEVEL(citta);
            for (UInt8 i = 1; i < lvl; ++i) {
                cb = GData::cittaManager[CITTAANDLEVEL(id, i)];
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
    sendModification(0x2d, citta, 2/*1add,2del,3mod*/, writedb);

    //散功成就
    GameAction()->doAttainment(_owner, 10081, 0);
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

float Fighter::getPracticeIncByDiamond()
{
    return _owner->getPracticeIncByDiamond();
}

float Fighter::getPracticeIncByQQVip()
{
    return _owner->getPracticeIncByQQVip();
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

void  Fighter::CheckEquipEnchantAttainment(UInt8 e)
{
    if(e >= 8 &&  _owner->GetAttainMgr()->CanAttain(Script::ONE_FIGHTER_ENCHANT_8))
    {
        if(IsEquipEnchantLev(8))
            GameAction()->doAttainment(this->_owner, Script::ONE_FIGHTER_ENCHANT_8, 1);
    }

    if(e>= 9 &&  _owner->GetAttainMgr()->CanAttain(Script::FIVE_FIGHTER_ENCHANT_9))
    {
        if(_owner->IsFighterEquipEnchantLev(9 , 5))
            GameAction()->doAttainment(this->_owner, Script::FIVE_FIGHTER_ENCHANT_9, 1);
    }

}
bool  Fighter::IsEquipEnchantLev(UInt8 e)
{
    if( _weapon == NULL  ||   _weapon-> getItemEquipData().enchant  < e)
        return false;

    for(int i = 0; i < 5; ++ i)
    {
       if(_armor[i] == NULL  || _armor[i]->getItemEquipData().enchant  < e)
       {
           return false;
       }
    }
    return true;
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

Fighter * GlobalFighters::getRandomOut( Player * pl, std::set<UInt32>& excepts, std::set<UInt32>& excepts2, UInt8 type, UInt32 rib, UInt32 rip, UInt32 rio)
{
	if(_fighters.empty())
		return NULL;

	Int8 color = 0;
    UInt8 colors = 1;
    UInt8 free_gold = 0;
    UInt32 chance4color[3] = {0};
    chance4color[0] = GObject::GObjectManager::getColorFighterChance(rib, type, 1);
    chance4color[1] = chance4color[0] + GObject::GObjectManager::getColorFighterChance(rip, type, 2);
    chance4color[2] = chance4color[1] + GObject::GObjectManager::getColorFighterChance(rio, type, 3);

	switch(type)
	{
    case 0:
    case 1:
        {
            UInt32 roll = uRand(100000);
            for(int i = 0; i < 3; ++ i)
            {
                if(roll < chance4color[i])
                {
                    color = i + 1;
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

Int16 Fighter::getMaxSoul()
{
    if(_owner == NULL)
        return soulMax + _elixirattr.soul;
    else
        return soulMax + _elixirattr.soul + _owner->getClanSkillMaxSoulEffect() + get2ndSounSoulMax();
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

void Fighter::sendMaxSoul()
{
	if(_owner == NULL)
		return;

	Stream st(REP::CHANGE_EQUIPMENT);
	st << getId() << static_cast<UInt8>(1);
    st << static_cast<UInt8>(9) << static_cast<UInt32>(getMaxSoul());
	st << Stream::eos;
	_owner->send(st);
}

void Fighter::setAttrType1(UInt8 t)
{
    _attrType1 = t;
}

void Fighter::setAttrValue1(UInt16 v)
{
    _attrValue1 = v;
}

void Fighter::setAttrType2(UInt8 t)
{
    if (_potential + 0.005f >= 1.2f)
        _attrType2 = t;
}

void Fighter::setAttrValue2(UInt16 v)
{
    if (_potential + 0.005f >= 1.2f)
        _attrValue2 = v;
}

void Fighter::setAttrType3(UInt8 t)
{
    if (_potential + 0.005f >= 1.5f && _capacity >= 7.0)
        _attrType3 = t;
}

void Fighter::setAttrValue3(UInt16 v)
{
    if (_potential + 0.005f >= 1.5f && _capacity >= 7.0)
        _attrValue3 = v;
}

UInt8 Fighter::getAttrType1(bool notify)
{
    UInt8 ret = 1;
    if (!_attrType1)
        ret = forge(1, 0, true);
    if (!ret)
        updateForgeAttr(notify);
    return _attrType1;
}

UInt16 Fighter::getAttrValue1(bool notify)
{
    UInt8 ret = 1;
    if (!_attrType1)
        ret = forge(1, 0, true);
    if (!ret)
        updateForgeAttr(notify);
    return _attrValue1;
}

UInt8 Fighter::getAttrType2(bool notify)
{
    UInt8 ret = 1;
    if (_potential + 0.005f >= 1.2f && !_attrType2)
        ret = forge(2, 0, true);
    if (!ret)
        updateForgeAttr(notify);
    return _attrType2;
}

UInt16 Fighter::getAttrValue2(bool notify)
{
    UInt8 ret = 1;
    if (_potential + 0.005f >= 1.2f && !_attrType2)
        ret = forge(2, 0, true);
    if (!ret)
        updateForgeAttr(notify);
    return _attrValue2;
}

UInt8 Fighter::getAttrType3(bool notify)
{
    UInt8 ret = 1;
    if (_potential + 0.005f >= 1.5f && _capacity >= 7.0 && !_attrType3)
        ret = forge(3, 0, true);
    if (!ret)
        updateForgeAttr(notify);
    return _attrType3;
}

UInt16 Fighter::getAttrValue3(bool notify)
{
    UInt8 ret = 1;
    if (_potential + 0.005f >= 1.5f && _capacity >= 7.0 && !_attrType3)
        ret = forge(3, 0, true);
    if (!ret)
        updateForgeAttr(notify);
    return _attrValue3;
}

UInt8 Fighter::forge(UInt8 which, UInt8 lock, bool initmain)
{
#define FF_FORGE_ITEM 516
#define FF_FORGE_PROT_ITEM 547

    if (!_owner)
        return 1;

    switch (which)
    {
        case 1:
            {
                UInt8 type = 0;
                do
                {
                    type = GObjectManager::getFFType();
                    if (!type)
                        return 1;
                }
                while (type == _attrType2 || type == _attrType3);

                UInt16 value = GObjectManager::getFFValue(type, initmain);
                if (!value)
                    return 1;

                _attrType1 = type;
                _attrValue1 = value; // XXX: /10000

                return 0;
            }
            break;

        case 2:
            {
                UInt8 type = 0;
                do
                {
                    type = GObjectManager::getFFType();
                    if (!type)
                        return 1;
                }
                while (type == _attrType1 || type == _attrType3);

                UInt16 value = GObjectManager::getFFValue(type, initmain);
                if (!value)
                    return 1;

                _attrType2 = type;
                _attrValue2 = value; // XXX: /10000

                return 0;
            }
            break;

        case 3:
            {
                UInt8 type = 0;
                do
                {
                    type = GObjectManager::getFFType();
                    if (!type)
                        return 1;
                }
                while (type == _attrType2 || type == _attrType1);

                UInt16 value = GObjectManager::getFFValue(type, initmain);
                if (!value)
                    return 1;

                _attrType3 = type;
                _attrValue3 = value; // XXX: /10000

                return 0;
            }
            break;

        default:
            {
                if (lock == 0x7)
                    return 1;

                if (!_owner->GetPackage()->DelItemAny(FF_FORGE_ITEM, 1, NULL, ToForgeFighter))
                    return 2;
                _owner->GetPackage()->AddItemHistoriesLog(FF_FORGE_ITEM, 1);
                _owner->OnHeroMemo(MC_FIGHTER, MD_MASTER, 0, 2);

                UInt8 ret = 1;
                if (!lock)
                {
                    ret = forge(1);
                    if (ret)
                        return ret;
                    if (_potential + 0.005f >= 1.2f || _attrType2)
                    {
                        ret = forge(2);
                        if (ret)
                            return ret;
                    }
                    if ((_potential + 0.005f >= 1.5f && _capacity >= 7.0f) || _attrType3)
                    {
                        ret = forge(3);
                        if (ret)
                            return ret;
                    }
                    broadcastForge(lock);
                    return 0;
                }

                UInt8 num = 0;
                if (lock & 0x1)
                    ++num;
                if (lock & 0x2)
                    ++num;
                if (lock & 0x4)
                    ++num;

                if (!_owner->GetPackage()->DelItemAny(FF_FORGE_PROT_ITEM, num, NULL, ToForgeFighter))
                    return 2;
                _owner->GetPackage()->AddItemHistoriesLog(FF_FORGE_PROT_ITEM, num);

                if (!(lock & 0x1))
                {
                    ret = forge(1);
                    if (ret)
                        return ret;
                }
                if (!(lock & 0x2))
                {
                    if (_potential + 0.005f >= 1.2f || _attrType2)
                    {
                        ret = forge(2);
                        if (ret)
                            return ret;
                    }
                }
                if (!(lock & 0x4))
                {
                    if ((_potential + 0.005f >= 1.5f && _capacity >= 7.0f) || _attrType3)
                    {
                        ret = forge(3);
                        if (ret)
                            return ret;
                    }
                }

                broadcastForge(lock);
                return 0;
            }
            break;
    }

    return 1;
}

void Fighter::updateForgeAttr(bool notify)
{
    if (!_owner)
        return;

    if (notify)
    {
        Stream st(REP::CHANGE_EQUIPMENT);
        st << getId() << static_cast<UInt8>(1) << static_cast<UInt8>(0x0d);
        st << _attrType1 << _attrValue1 << _attrType2 << _attrValue2 << _attrType3 << _attrValue3 << Stream::eos;
        _owner->send(st);
    }

    DB2().PushUpdateData("UPDATE `fighter` SET `attrType1` = %u, `attrValue1` = %u, `attrType2` = %u, `attrValue2` = %u, `attrType3` = %u, `attrValue3` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _attrType1, _attrValue1, _attrType2, _attrValue2, _attrType3, _attrValue3, _id, _owner->getId());

    _attrDirty = true;
    _bPDirty = true;
}

void Fighter::broadcastForge(UInt8 lock)
{
    bool b = false;
    if (!(lock & 0x1) && (((((double)(_attrValue1-_attrValue1%10) / 100.f) / GObjectManager::getFFMaxVal(_attrType1))) > 0.909f))
        b = true;
    if (!(lock & 0x2) && (((((double)(_attrValue2-_attrValue2%10) / 100.f) / GObjectManager::getFFMaxVal(_attrType2))) > 0.909f))
        b = true;
    if (!(lock & 0x4) && (((((double)(_attrValue3-_attrValue3%10) / 100.f) / GObjectManager::getFFMaxVal(_attrType3))) > 0.909f))
        b = true;

    if (b)
    {
        fprintf(stderr, "%u, %u, %u, %u, %u, %u\n", _attrType1, _attrValue1,  _attrType2, _attrValue2,  _attrType3, _attrValue3);
        SYSMSG_BROADCASTV(2330, _owner->getCountry(), _owner->getName().c_str(), getColor(), getName().c_str());
    }
}

float Fighter::getSoulPracticeAddOn()
{
    if(!m_2ndSoul)
        return 0;
    return m_2ndSoul->getPracticeAddOn();
}

float Fighter::getSoulPracticeFactor()
{
    if(!m_2ndSoul)
        return 0;
    return m_2ndSoul->getPracticeFactor();
}

bool Fighter::openSecondSoul(UInt8 cls)
{
    if(m_2ndSoul || _level < 60 || cls < 1 || cls > 3)
        return false;

    m_2ndSoul = new SecondSoul(this, cls);
    // xx: TODO
    //UInt16 skillId = m_2ndSoul->getSkillIdOfItem(SOUL_SKILL_DEFAULT_ITEM);
    //m_2ndSoul->setSoulSkill(0, skillId, false);

    m_2ndSoul->insertIntoDB();
    m_2ndSoul->sendInfo(_owner);
    sendMaxSoul();
    return true;
}

void Fighter::setSecondSoul(SecondSoul* secondSoul)
{
    if(m_2ndSoul)
        return;

    m_2ndSoul = secondSoul;
}

UInt8 Fighter::getSoulExtraAura()
{
    if(!m_2ndSoul)
        return 0;

    return m_2ndSoul->getExtraAura();
}

bool Fighter::practiceLevelUp()
{
    if(!m_2ndSoul)
        return false;

    Stream st(REP::SECOND_SOUL);
    st << static_cast<UInt8>(2) << static_cast<UInt16>(getId());
    if(m_2ndSoul->practiceLevelUp(_pexp))
    {
        sendModification(6, _pexp);
        st << static_cast<UInt8>(0) << m_2ndSoul->getPracticeLevel();
    }
    else
        st << static_cast<UInt8>(1);

    st << Stream::eos;

    _owner->send(st);
    return true;
}

void Fighter::enchantSoul(UInt32 itemId, bool bind, std::vector<SoulItemExp>& soulItemExpOut)
{
    if(!m_2ndSoul)
        return;
    std::map<UInt32, UInt32>::iterator it = GData::GDataManager::m_soulItemExp.find(itemId);
    if(it == GData::GDataManager::m_soulItemExp.end())
        return;

    if(!_owner->GetPackage()->DelItem(itemId, 1, bind, ToSecondSoul))
    {
        return;
    }

    UInt32 exp = it->second;
    const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[itemId];

    UInt8 itemColor = itemType->quality;
    UInt8 soulColor = m_2ndSoul->getSoulColor();
    UInt8 soulStateLevel = m_2ndSoul->getStateLevel();

    bool succ = false;
    SoulItemExp sie;

    if(itemColor >= soulColor)
    {
        succ = true;
    }
    else
    {
        UInt16 rnd = uRand(10000);
        if(itemId == SOUL_EXP_ITEM)
            itemColor = 6;
        if(rnd < GObjectManager::getSoulEnchantChance(soulStateLevel, itemColor))
            succ = true;
    }

    sie.itemId = itemId;
    if(succ)
    {
        sie.res = 0;
        sie.exp = exp;

        UInt32 msoul = get2ndSounSoulMax();
        m_2ndSoul->addStateExp(exp);
        UInt32 yamsoul = get2ndSounSoulMax();

        if (msoul != yamsoul && World::getEnchantAct())
            GameAction()->onSoulEnchantMaxSoul(_owner, msoul, yamsoul);
    }
    else
    {
        exp = GObjectManager::getDecSoulStateExp(soulStateLevel);
        sie.res = 1;
        sie.exp = exp;
        m_2ndSoul->decStateExp(exp);
    }

    soulItemExpOut.push_back(sie);
}

bool Fighter::equipSoulSkill(UInt8 idx, UInt32 itemId, bool bind)
{
    if(!m_2ndSoul)
        return false;

    const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[itemId];
    if( m_2ndSoul->getSkillNum1() > MAX_SKILL_NUM_1 || m_2ndSoul->getSkillNum2() > MAX_SKILL_NUM_2 )
    {
        itemId = 0;
        idx = 0;
    }

    if(itemId != 0 && idx == MAX_SKILL_NUM)
    {
        if( (m_2ndSoul->getSkillNum1() == MAX_SKILL_NUM_1 && itemType->subClass == Item_SL1)
                || (m_2ndSoul->getSkillNum2() == MAX_SKILL_NUM_2 && itemType->subClass == Item_SL2) )
            return false;
        -- idx;
    }

    UInt16 skillId = 0;
    if(itemId != 0)
    {
        if (_owner->GetPackage()->GetItemNum(itemId, bind) == 0)
            return false;

        skillId = m_2ndSoul->getSkillIdOfItem(itemId);
    }

    if(_owner->GetFreePackageSize() < 1)
    {
        _owner->sendMsgCode(0, 1011);
        return false;
    }

    UInt32 itemOut = 0xFFFFFFFF;
    itemOut = m_2ndSoul->setSoulSkill(idx, skillId, true);
    if(0xFFFFFFFF == itemOut)
        return false;

    _owner->GetPackage()->DelItem(itemId, 1, bind);
    if(0 != itemOut)
        _owner->GetPackage()->AddItem(itemOut, 1, true, true);

    m_2ndSoul->sendSoulSkill(_owner);
    return true;
}

void Fighter::send2ndSoulInfo()
{
    if(!m_2ndSoul)
    {
        Stream st(REP::SECOND_SOUL);
        st << static_cast<UInt8>(0) << static_cast<UInt16>(getId()) << static_cast<UInt8>(0) << Stream::eos;
        _owner->send(st);
        return;
    }

    m_2ndSoul->sendInfo(_owner);
}

Int16 Fighter::get2ndSounSoulMax()
{
    if(!m_2ndSoul)
        return 0;

    return m_2ndSoul->getSoulMax();
}

bool Fighter::changeSecondSoulClass(UInt8 cls)
{
    if(!m_2ndSoul)
    {
        _owner->sendMsgCode(0, 1072);
        return false;
    }

    return m_2ndSoul->setClass(cls);
}

void Fighter::getAttrExtraEquip(Stream& st)
{
    checkDirty();
    GData::AttrExtra& attr = _attrExtraEquip;
	st << attr.strength << attr.physique << attr.agility << attr.intelligence << attr.will << attr.soul << attr.aura;
	st << attr.auraMax << attr.attack << attr.magatk << attr.defend << attr.magdef << attr.hp << attr.tough << attr.action;
	st << attr.hitrate << attr.evade << attr.critical << attr.criticaldmg << attr.pierce << attr.counter << attr.magres;

	st << attr.strengthP << attr.physiqueP << attr.agilityP << attr.intelligenceP << attr.willP << attr.soulP << attr.auraP;
    st << attr.auraMaxP << attr.attackP << attr.magatkP << attr.defendP << attr.magdefP << attr.hpP << attr.toughP << attr.actionP;
	st << attr.hitrateP << attr.evadeP << attr.criticalP << attr.criticaldmgP << attr.pierceP << attr.counterP << attr.magresP;

    st << attr.hitrlvl << attr.evdlvl << attr.crilvl << attr.pirlvl << attr.counterlvl << attr.mreslvl << attr.toughlvl;
}

bool Fighter::changeSecondSoulXinxiu(UInt8 xinxiu)
{
    if(!m_2ndSoul)
    {
        _owner->sendMsgCode(0, 1072);
        return false;
    }

    return m_2ndSoul->setXinxiu(xinxiu);
}

UInt8 Fighter::getSoulSkillIdx(UInt16 itemId)
{
    if(!m_2ndSoul)
        return 0xFF;

    if(itemId == 0)
        return MAX_SKILL_NUM;

    UInt16 skillId = m_2ndSoul->getSkillIdOfItem(itemId);

    return m_2ndSoul->getSoulSkillIdx(skillId);
}

bool Fighter::addElixirAttrByOffset(UInt8 off, Int32 v)
{
    static UInt8 off2type[] = {
        0x10, 0x14, 0x11, 0x12, 0x13, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
    };

#define MAXVAL(x,y) { if (x > y) x = y; }
#define MV 200
#define MV1 10
    bool ret = false;

    Stream st(REP::CHANGE_EQUIPMENT);
    st << getId() << static_cast<UInt8>(1);

    if (off == 0)
    {
        _elixirattr.strength += v;
        MAXVAL(_elixirattr.strength, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.strength);
        ret = true;
    }
    if (off == 1)
    {
        _elixirattr.physique += v;
        MAXVAL(_elixirattr.physique, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.physique);
        ret = true;
    }
    if (off == 2)
    {
        _elixirattr.agility += v;
        MAXVAL(_elixirattr.agility, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.agility);
        ret = true;
    }
    if (off == 3)
    {
        _elixirattr.intelligence += v;
        MAXVAL(_elixirattr.intelligence, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.intelligence);
        ret = true;
    }
    if (off == 4)
    {
        _elixirattr.will += v;
        MAXVAL(_elixirattr.will, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.will);
        ret = true;
    }
    if (off == 5)
    {
        _elixirattr.soul += v;
        MAXVAL(_elixirattr.soul, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.soul);
        sendModification(9, getMaxSoul());
        ret = true;
    }
    if (off == 6)
    {
        _elixirattr.attack += v;
        MAXVAL(_elixirattr.attack, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.attack);
        ret = true;
    }
    if (off == 7)
    {
        _elixirattr.defend += v;
        MAXVAL(_elixirattr.defend, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.defend);
        ret = true;
    }
    if (off == 8)
    {
        _elixirattr.critical += v;
        MAXVAL(_elixirattr.critical, MV1);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.critical);
        ret = true;
    }
    if (off == 9)
    {
        _elixirattr.pierce += v;
        MAXVAL(_elixirattr.pierce, MV1);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.pierce);
        ret = true;
    }
    if (off == 10)
    {
        _elixirattr.evade += v;
        MAXVAL(_elixirattr.evade, MV1);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.evade);
        ret = true;
    }
    if (off == 11)
    {
        _elixirattr.counter += v;
        MAXVAL(_elixirattr.counter, MV1);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.counter);
        ret = true;
    }
    if (off == 12)
    {
        _elixirattr.tough += v;
        MAXVAL(_elixirattr.tough, MV1);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.tough);
        ret = true;
    }
    if (off == 13)
    {
        _elixirattr.action += v;
        MAXVAL(_elixirattr.action, MV);
        st << off2type[off] << static_cast<UInt32>(_elixirattr.action);
        ret = true;
    }

    if (ret)
    {
        st << Stream::eos;
        _owner->send(st);

        DB1().PushUpdateData("REPLACE INTO `elixir` (`id`, `playerId`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `attack`, `defend`, `critical`, `pierce`, `evade`, `counter`, `tough`, `action`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", _id, _owner->getId(), _elixirattr.strength, _elixirattr.physique, _elixirattr.agility, _elixirattr.intelligence, _elixirattr.will, _elixirattr.soul, _elixirattr.attack, _elixirattr.defend, _elixirattr.critical, _elixirattr.pierce, _elixirattr.evade, _elixirattr.counter, _elixirattr.tough, _elixirattr.action);
    }
    return ret;
#undef MV
#undef MAXVAL
}

Int32 Fighter::getElixirAttrByOffset(UInt8 off)
{
    if (off == 0)
        return _elixirattr.strength;
    if (off == 1)
        return _elixirattr.physique;
    if (off == 2)
        return _elixirattr.agility;
    if (off == 3)
        return _elixirattr.intelligence;
    if (off == 4)
        return _elixirattr.will;
    if (off == 5)
        return _elixirattr.soul;
    if (off == 6)
        return _elixirattr.attack;
    if (off == 7)
        return _elixirattr.defend;
    if (off == 8)
        return _elixirattr.critical;
    if (off == 9)
        return _elixirattr.pierce;
    if (off == 10)
        return _elixirattr.evade;
    if (off == 11)
        return _elixirattr.counter;
    if (off == 12)
        return _elixirattr.tough;
    if (off == 13)
        return _elixirattr.action;
    return 0;
}

void Fighter::appendElixirAttr(Stream& st)
{
    st << static_cast<UInt16>(_elixirattr.strength);
    st << static_cast<UInt16>(_elixirattr.agility);
    st << static_cast<UInt16>(_elixirattr.intelligence);
    st << static_cast<UInt16>(_elixirattr.will);
    st << static_cast<UInt16>(_elixirattr.physique);
    st << static_cast<UInt16>(_elixirattr.soul);
    st << static_cast<UInt16>(_elixirattr.attack);
    st << static_cast<UInt16>(_elixirattr.defend);
    st << static_cast<UInt16>(_elixirattr.critical);
    st << static_cast<UInt16>(_elixirattr.pierce);
    st << static_cast<UInt16>(_elixirattr.evade);
    st << static_cast<UInt16>(_elixirattr.counter);
    st << static_cast<UInt16>(_elixirattr.tough);
    st << static_cast<UInt16>(_elixirattr.action);
}

void Fighter::appendElixirAttr2(Stream& st)
{
    st << _elixirattr.strength;
    st << _elixirattr.agility;
    st << _elixirattr.intelligence;
    st << _elixirattr.will;
    st << _elixirattr.physique;
    st << _elixirattr.soul;
    st << _elixirattr.attack;
    st << _elixirattr.defend;
    st << _elixirattr.critical;
    st << _elixirattr.pierce;
    st << _elixirattr.evade;
    st << _elixirattr.counter;
    st << _elixirattr.tough;
    st << _elixirattr.action;
}


UInt16 Fighter::getBattlePortrait()
{
    UInt16 portrait = 0;
    UInt32 fashion = getFashionTypeId();

    switch(fashion)
    {
    case 1700:
        portrait = 1072;
        break;
    case 1701:
        portrait = 1074;
        break;
    case 1702:
        portrait = 1063;
        break;
    case 1703:
        portrait = 1064;
        break;
    case 1704:
        portrait = 1076;
        break;
    case 1705:
        portrait = 1077;
        break;
    case 1706:
        portrait = 1088;
        break;
    case 1707:
        portrait = 1090;
        break;
    }

    return portrait;
}

UInt8 Fighter::SSGetLvl(UInt16 skillid)
{
    if (!_owner)
        return 0;
    UInt32 sid = SKILL_ID(skillid);
    std::map<UInt16, SStrengthen>::iterator i = m_ss.find(sid);
    if (i == m_ss.end())
        return 0;
    return i->second.lvl;
}

SStrengthen* Fighter::SSGetInfo(UInt16 skillid)
{
    if (!_owner)
        return 0;
    UInt32 sid = SKILL_ID(skillid);
    std::map<UInt16, SStrengthen>::iterator i = m_ss.find(sid);
    if (i == m_ss.end())
        return NULL;
    return &i->second;
}

bool Fighter::appendFighterSSInfo(Stream& st, UInt16 skillid)
{
    SStrengthen* ss = SSGetInfo(skillid);
    if (!ss)
        return false;
    return appendFighterSSInfo(st, skillid, ss);
}

bool Fighter::appendFighterSSInfo(Stream& st, UInt16 skillid, SStrengthen* ss)
{
    if (ss)
    {
        st << skillid << ss->maxVal << ss->curVal << ss->lvl << ss->maxLvl;
        return true;
    }
    return false;
}

void Fighter::makeFighterSSInfo(Stream& st)
{
    st << getId();
    size_t offset = st.size();
    st << static_cast<UInt8>(0);
    UInt8 c = 0;
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (_skill[i])
        {
            if (appendFighterSSInfo(st, _skill[i]))
                ++c;
        }
    }
    st.data<UInt8>(offset) = c;
}

void Fighter::getAllSSAndLevel(Stream& st)
{
    size_t offset = st.size();
    st << static_cast<UInt8>(0);
    UInt8 c = 0;
    for (int i = 0; i < getUpSkillsMax(); ++i)
    {
        if (_skill[i])
        {
            SStrengthen* ss = SSGetInfo(_skill[i]);
            if (ss)
            {
                ++c;
                UInt16 skill_id = SKILL_ID(_skill[i]);
                st << static_cast<UInt16>(SKILLANDLEVEL(skill_id, ss->lvl));
            }
        }
    }
    st.data<UInt8>(offset) = c;
}

#define SS_MAXLVL 9
void Fighter::SSOpen(UInt16 id)
{
    if (!_owner)
        return;
    int idx = isSkillUp(id);
    if (idx < 0)
        return;

    UInt16 sid = SKILL_ID(id);
    if (GData::skill2item.find(sid) == GData::skill2item.end())
        return;
#if 0 // XXX: 使用固定物品
    UInt16 itemId = GData::skill2item[sid];
    if (!itemId)
        return;
#endif

    std::map<UInt16, SStrengthen>::iterator i = m_ss.find(sid);
    if (i != m_ss.end())
    {
        UInt8 mlvl = getUpSkillLevel(idx);
        if (i->second.maxLvl >= mlvl && mlvl == 9)
        {
            _owner->sendMsgCode(0, 1021);
            return;
        }
        if (i->second.maxLvl >= mlvl)
        {
            _owner->sendMsgCode(0, 1027);
            return;
        }
    }

    if (!GData::GDataManager::getMaxStrengthenVal(sid, 0))
    {
        _owner->sendMsgCode(0, 1024);
        return;
    }

    UInt16 itemId = 550;
    if (i->second.maxLvl >= 4)
        itemId = 551;
    Package* pkg = _owner->GetPackage();
    ItemBase* item = pkg->FindItem(itemId, true);
    if (!item)
        item = pkg->FindItem(itemId, false);
    if (!item)
        return;

    //
    if (item->getClass() != Item_Trump)
    {
        if(!pkg->DelItem2(item, 1, ToSkillStrengthenOpen))
            return;
    }
    else
    {
        if(!pkg->DelEquip(itemId, ToSkillStrengthenOpen))
            return;
    }

    if (i == m_ss.end())
    {
        UInt32 prob = GData::GDataManager::getSkillStrengthenProb(sid, 0);
        if (uRand(10000) <= prob)
        {
            SStrengthen ss;
            ss.father = itemId;
            ss.maxVal = GData::GDataManager::getMaxStrengthenVal(sid, 0);
            if (!ss.maxVal)
                return;
            ss.maxLvl = 1;
            m_ss[sid] = ss;
            SSUpdate2DB(id, ss);
            _owner->skillStrengthenLog(1, 1);
            _owner->sendMsgCode(0, 1023);
        }
        else
        {
            _owner->skillStrengthenLog(1, 0);
            _owner->sendMsgCode(0, 1024);
        }
    }
    else
    {
        UInt32 prob = GData::GDataManager::getSkillStrengthenProb(sid, i->second.maxLvl);
        if (uRand(10000) <= prob)
        {
            ++i->second.maxLvl;
            i->second.maxVal = GData::GDataManager::getMaxStrengthenVal(sid, i->second.lvl);
            SSUpdate2DB(id, i->second);
            _owner->skillStrengthenLog(1, 1);
            _owner->sendMsgCode(0, 1023);
        }
        else
        {
            _owner->skillStrengthenLog(1, 0);
            _owner->sendMsgCode(0, 1024);
        }
    }
}

UInt8 Fighter::SSUpgrade(UInt16 id, UInt32 itemId, bool bind)
{
    if (!_owner)
        return 0;

    UInt32 sid = SKILL_ID(id);
    std::map<UInt16, SStrengthen>::iterator i = m_ss.find(sid);
    if (i == m_ss.end())
    {
        _owner->sendMsgCode(0, 1022);
        return 0;
    }

    SStrengthen& ss = m_ss[sid];
    if (!ss.maxVal) // full
    {
        _owner->sendMsgCode(0, 1021);
        return 0;
    }
    if (GetItemSubClass(itemId) != Item_Citta)
        return 0;

    int idx = isSkillUp(id);
    if (idx < 0)
        return 0;

    if (ss.lvl >= ss.maxLvl)
    {
        _owner->sendMsgCode(0, 1028);
        return 0;
    }

    if (GData::skill2item.find(sid) == GData::skill2item.end())
        return 0;

    Package* pkg = _owner->GetPackage();

    ItemBase* item = pkg->FindItem(itemId, bind);
    if (!item)
        return 0;

    const GData::ItemBaseType& ibt = item->GetItemType();
    UInt32 exp = ibt.trumpExp;
    if (!exp)
        return 0;

    if (item->getClass() != Item_Trump)
    {
        if(!pkg->DelItem2(item, 1, ToSkillStrengthenOpen))
            return 0;
    }
    else
    {
        if(!pkg->DelEquip(itemId, ToSkillStrengthenOpen))
            return 0;
    }

    ss.curVal += exp;
    _owner->skillStrengthenLog(2, exp);

    UInt8 ret = 1;
    while (ss.curVal >= ss.maxVal)
    {
        ss.curVal -= ss.maxVal;
        ++ss.lvl;

        ss.maxVal = GData::GDataManager::getMaxStrengthenVal(sid, ss.lvl);
        if (ss.lvl >= ss.maxLvl)
        {
            ss.curVal = 0;
            if (ss.lvl == 9) // XXX: max level
                ss.maxVal = 0;
            ret = 0;
            break;
        }
    }

    SSUpdate2DB(id, ss);
    _owner->sendMsgCode(0, 1025);
    return ret;
}

void Fighter::SSErase(UInt16 id)
{
    if (!_owner)
        return;
    UInt32 sid = SKILL_ID(id);
    std::map<UInt16, SStrengthen>::iterator i = m_ss.find(sid);
    if (i == m_ss.end())
        return;
    m_ss.erase(sid);
    SSDeleteDB(sid);
}

void Fighter::SSSendSSInfo(UInt16 skill)
{
    UInt32 sid = SKILL_ID(skill);
    std::map<UInt16, SStrengthen>::iterator i = m_ss.find(sid);
    if (i == m_ss.end())
        return;
    if (isSkillUp(skill) < 0)
        return;
    SSNotify(skill, i->second);
}

void Fighter::SSUpdate2DB(UInt16 id, SStrengthen& ss)
{
    DB1().PushUpdateData("REPLACE INTO `skill_strengthen` (`id`, `playerId`, `skillid`, `father`, `maxVal`, `curVal`, `lvl`, `maxLvl`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u)", getId(), _owner->getId(), SKILL_ID(id), ss.father, ss.maxVal, ss.curVal, ss.lvl, ss.maxLvl);
    SSNotify(id, ss);
}

void Fighter::SSNotify(UInt16 id, SStrengthen& ss)
{
    Stream st(REP::SKILLSTRENGTHEN);
    st << static_cast<UInt8>(1) << getId();
    appendFighterSSInfo(st, id, &ss);
    st << Stream::eos;
    _owner->send(st);
}

void Fighter::SSDeleteDB(UInt16 id)
{
    DB1().PushUpdateData("DELETE FROM `skill_strengthen` WHERE `id` = %u AND `playerId` = %"I64_FMT"u AND `skillid` = %u", getId(), _owner->getId(), id);
}

void Fighter::SSFromDB(UInt16 id, SStrengthen& ss)
{
    if (!_owner)
        return;
    // XXX: DO Delete
    m_ss[id] = ss;
}

}

