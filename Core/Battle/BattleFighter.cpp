#include "Config.h"
#include "BattleFighter.h"
#include "GObject/Item.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "Common/URandom.h"
#include "Script/BattleFormula.h"

namespace Battle
{

BattleFighter::BattleFighter(Script::BattleFormula * bf, GObject::Fighter * f, UInt8 side, UInt8 pos):
	BattleObject(BattleObject::Char, side, pos),
	_strength(0), _agility(0), _physique(0), _intelligence(0),
	_attack(0), _defend(0), _hitrate(0), _evade(0), _critical(0), _pierce(0), _counter(0),
	_maxhp(f->getMaxHP()), _maxAction(0),
	_attackAdd(0), _defendAdd(0), _hitrateAdd(0), _evadeAdd(0), _criticalAdd(0), _pierceAdd(0), _counterAdd(0),
	_maxhpAdd(0), _maxActionAdd(0),
	_formEffect(NULL), _formula(bf), _flag(0)
{
	setFighter(f);
}

void BattleFighter::setFighter( GObject::Fighter * f )
{
	_fighter = f;
}

void BattleFighter::updateAllAttr()
{
	updateBuffExtras();

	_strength = _formula->calcStrength(this);
	_agility = _formula->calcAgility(this);
	_physique = _formula->calcPhysique(this);
	_intelligence = _formula->calcIntelligence(this);
	_attack = _formula->calcAttack(this);
	_defend = _formula->calcDefend(this);
	_hitrate = _formula->calcHitrate(this);
	_evade = _formula->calcEvade(this);
	_critical = _formula->calcCritical(this);
	_pierce = _formula->calcPierce(this);
	_counter = _formula->calcCounter(this);
	_maxAction = _formula->calcAction(this);
	UInt32 oldhp = _maxhp;
	_maxhp = _formula->calcHP(this);

	if((_flag & BlockBoss) == 0 && oldhp > 0 && _hp > 0 && oldhp < _maxhp)
	{
		_hp = _hp + _maxhp - oldhp;
	}
	if(_hp > _maxhp)
	{
		_hp = _maxhp;
	}
}

void BattleFighter::setAttrExtra(UInt8 klass, UInt8 career, UInt8 level)
{
	//float strength = 2.5f;
	//float physique = 6.5f; 
	//float agility = 2.5f;
	//float intelligence = 1.5f;
	//UInt32 attack = 25;
	//UInt32 defend = 30;
	//UInt32 maxhp = 150;
	//float action = 0.2f;
	//float hitrate = 0.15;
	//float evade = 0.25;
	//float critical = 0.15;
	//float pierce = 0.1;
	//float counter = 0.3;
	
	UInt8 i = 1;
	lua_tinker::table factors = _formula->getFactor(klass, career, level);
	_attrbylevel.strength = static_cast<UInt16>(factors.get<float>(i++));
	_attrbylevel.physique = static_cast<UInt16>(factors.get<float>(i ++));
	_attrbylevel.intelligence = static_cast<UInt16>(factors.get<float>(i ++));
	_attrbylevel.agility = static_cast<UInt16>(factors.get<float>(i ++));
	_attrbylevel.hitrate = factors.get<float>(i ++);
	_attrbylevel.evade = factors.get<float>(i ++);
	_attrbylevel.critical = factors.get<float>(i ++);
	_attrbylevel.pierce = factors.get<float>(i ++);
	_attrbylevel.counter = factors.get<float>(i ++);
	_attrbylevel.action = factors.get<float>(i ++);
	_attrbylevel.hp = factors.get<UInt16>(i ++);
	_attrbylevel.attack = factors.get<UInt16>(i ++);
	_attrbylevel.defend = factors.get<UInt16>(i ++);
}

inline void addAttrExtra( GData::AttrExtra& ae, const GData::AttrExtra * ext )
{
	if(ext == NULL)
		return;
	ae += *ext;
}

void BattleFighter::updateBuffExtras()
{
	_attrExtra.reset();
	if(_formEffect != NULL)
	{
		addAttrExtra(_attrExtra, _formEffect->attrExtra);
	}
	addAttrExtra(_attrExtra, _fighter->getAttrExtraEquip());
	UInt32 ext = (_flag & 0x03) + ((_flag >> 2) & 0x03);
	if(_flag & Enh3)
		ext += 2;
	if(_flag & BlockBoss)
	{
		addAttrExtra(_attrExtra, &_attrbylevel);
	}
	if(ext > 0)
	{
		float extAttr = 0.05f * ext;
		_attrExtra.strengthP += extAttr;
		_attrExtra.physiqueP += extAttr;
		_attrExtra.agilityP += extAttr;
		_attrExtra.intelligenceP += extAttr;
	}
}

float BattleFighter::calcAttack( bool& isCritical )
{
	// 计算暴击率
	isCritical = uRand(10000) < _critical * 100;

	float atk = _attack + _attackAdd;
	/* TODO: random
	int extra = (uRand(9)) - 4;
	*/

	// 如果暴击
	if(isCritical)
	{
		atk = atk * 3 / 2;
	}
	return atk;
}

bool BattleFighter::calcHit( BattleFighter * defender )
{
	if(defender->getStunRound() > 0)
		return true;

	// 计算命中值
	float hitrate = _hitrate + _hitrateAdd - defender->getEvade();

	// 必中
	if(hitrate >= 100)
		return true;

	// 计算命中率
	return uRand(10000) < hitrate * 100;
}

bool BattleFighter::calcCounter(bool ranged)
{
	return uRand(ranged ? 20000 : 10000) < (_counter + _counterAdd) * 100;
}

bool BattleFighter::canBeCounter()
{
	GObject::ItemWeapon * wp = _fighter->getWeapon();
	// TODO: no weapon_def return wp == NULL || wp->getWeaponDef().canCounter();
	return wp == NULL;
}

bool BattleFighter::calcPierce()
{
	return uRand(10000) < (_pierce + _pierceAdd) * 100;
}

void BattleFighter::initStats(bool checkEnh)
{
	if(_fighter != NULL)
	{
		if(checkEnh && (_fighter->getId() <= GREAT_FIGHTER_MAX || _fighter->getId() > NPC_FIGHTER_MAX) && _fighter->getOwner() != NULL)
		{
			UInt32 now = TimeUtil::Now();
			if(_fighter->getBuffData(FIGHTER_BUFF_ATTR3, now))
				_flag |= 3;
			else if(_fighter->getBuffData(FIGHTER_BUFF_ATTR2, now))
				_flag |= 2;
			else if(_fighter->getBuffData(FIGHTER_BUFF_ATTR1, now))
				_flag |= 1;
			_flag |= (_fighter->getOwner()->getBuffData(PLAYER_BUFF_HOLY, 0)) << 28;
		}
		if(_fighter->getId() > 4800 && _fighter->getId() < 5000)
		{
			_flag |= BlockBoss;
		}
		updateAllAttr();
		if(_hp == 0)
			_hp = _maxhp;
	}
}

UInt32 BattleFighter::regenHP( UInt32 u )
{
	UInt32 oldhp = _hp;
	if(oldhp >= _maxhp)
	{
		_hp = _maxhp;
		return 0;
	}
	_hp += u;
	if(_hp > _maxhp)
	{
		_hp = _maxhp;
		return _maxhp - oldhp;
	}
	return u;
}

UInt16 BattleFighter::getPortrait()
{
	if(_fighter->isNpc())
		return _fighter->favor;
	else
		return _fighter->getId();
}

void BattleFighter::postInit()
{
	UInt32 holy_level = _flag >> 28;
	if(holy_level)
	{
		switch(holy_level)
		{
		case 2:
			_attack += 100;
			_maxAction = _maxAction * 10 / 11;
			_hitrate += 10.0f;
			_evade += 12.0f;
			_counter += 10.0f;
			_critical += 5.0f;
			_pierce += 5.0f;
			break;
		case 4:
			_attack += 200;
			_maxAction = _maxAction * 10 / 11;
			_hitrate += 50.0f;
			_evade += 12.0f;
			_counter += 10.0f;
			_critical += 5.0f;
			_pierce += 5.0f;
			break;
		case 6:
			_attack += 300;
			_maxAction = _maxAction * 20 / 23;
			_hitrate += 100.0f;
			_evade += 20.0f;
			_counter += 10.0f;
			_critical += 10.0f;
			_pierce += 10.0f;
			break;
		}
	}
}

}
