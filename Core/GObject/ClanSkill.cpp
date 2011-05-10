#include "Config.h"

#include "Common/Stream.h"
#include "Server/SysMsg.h"
#include "GData/ClanSkillTable.h"
#include "Player.h"
#include "ClanSkill.h"
#include "Clan.h"

namespace GObject
{

static UInt8 clanSkillDonateType[] = { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }; //宗族等级1、宗族经验2、南门防护3(南门耐久度)、北门防护4（北门耐久度）、护卫等级5(NPC等级)、护卫数量6(NPC数量)、(青龙、白虎、朱雀、玄武7)、城门图腾8、宗祠图腾9

ClanSkill::ClanSkill(Clan * c) : _clanLev(0), _clan(c)
{
	
}

ClanSkill::~ClanSkill()
{

}


void ClanSkill::addSkillFromDB(UInt8 skillId, UInt8 level, UInt16 extra)
{
	_skills.insert(std::make_pair(skillId, ClanSkillData(skillId, clanSkillDonateType[skillId], level, extra)));
	if (skillId == 1) _clanLev = level;
}

void ClanSkill::buildSkill()
{
	addSkill(1, 1, 1, 0);
	for (UInt8 i = 2; i <= 9; ++ i)
		addSkill(i, 2, 0, 0);
}

bool ClanSkill::donate(Player * player, UInt8 id, UInt16 type, UInt16 count)
{
	Skills::iterator found = _skills.find(id);
	if (found == _skills.end())
		return false;
	ClanSkillData& skill = found->second;
	switch (type)
	{
	case 1:
		{
			if (skillLevelUp(skill.skillId, skill.level, skill.extra, count))
			{
				Stream st;
				SYSMSGVP(st, 426);
				_clan->broadcast(st);
			}
			Stream st(0x78);
			st << static_cast<UInt8>(3);
			makeSkillInfo(st, skill);
			st << Stream::eos;
			_clan->broadcast(st);
			DB().PushUpdateData("UPDATE `clan_skill` SET `level` = %u, `extra` = %d WHERE `clanId` = %u AND `skillId` = %u", skill.level, skill.extra, _clan->getId(), skill.skillId);
			return true;
		}
		break;
	case 2:
		{
			skillLevelUp(skill.skillId, skill.level, skill.extra, count);
			Stream st(0x78);
			st << static_cast<UInt8>(3);
			makeSkillInfo(st, skill);
			st << Stream::eos;
			_clan->broadcast(st);
			DB().PushUpdateData("REPLACE INTO `clan_skill`(`clanId`, `skillId`, `level`, `extra`) VALUES(%u, %u, %u, %u);", _clan->getId(), skill.skillId, skill.level, skill.extra);

			return true;
		}
		break;
	default:
		{	
			//捐献喜好品
			//const UInt8 index[] = {0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7};
			//if(id > 10  || id < 7)
			//	return false;
			//if(_skills.find(index[id]) == _skills.end())
			//	return false;

			//return true;
		}
	}

	return false;
}

void ClanSkill::makeSkillInfo(Stream& st)
{
	std::map<UInt8, ClanSkillData>::iterator offset = _skills.begin();
	for (; offset != _skills.end(); ++ offset)
		st << offset->second.skillId << (GData::clanSkillTable[offset->second.skillId][offset->second.level].accNeeds + static_cast<UInt32>(offset->second.extra));
}

void ClanSkill::makeSkillInfo(Stream& st, ClanSkillData& skill)
{
	st << skill.skillId << (GData::clanSkillTable[skill.skillId][skill.level].accNeeds + static_cast<UInt32>(skill.extra));
}

bool ClanSkill::skillLevelUp(UInt8 id, UInt8& level, UInt16& extra, UInt16 count)
{
	bool r = false;
	extra += count;
	GData::SingleClanSkillTable & skillTable = GData::clanSkillTable[id];
	UInt8 maxLev = static_cast<UInt8>(skillTable.size()) - 1;
	while (level < maxLev && extra >= skillTable[level+1].needs && getClanLev() >= skillTable[level+1].clanLev)
	{
		r = true;
		++ level;
		extra -= skillTable[level].needs;
	}
	if (r)
	{
		Stream st;
		_clan->getClanDynamicMsg()->addCDMsg(8, static_cast<UInt32>(id), static_cast<UInt32>(level), &st);
		_clan->broadcast(st);
	}
	return r;
}

bool ClanSkill::skillLevelDown(UInt8 id, UInt8& level, UInt16& extra, UInt16 count)
{
	bool r = false;
	if (extra >= count)
		extra -= count;
	else
	{
		UInt32 accu = extra;
		GData::SingleClanSkillTable & skillTable = GData::clanSkillTable[id];
		while (level > 1)
		{
			r = true;
			accu += skillTable[level].needs;
			if (accu >= count)
			{
				-- level;
				extra = accu - count;
				return r;
			}
			else
				-- level;
		}
		if (level == 1)
			extra = 0;
	}
	return r;
}

void ClanSkill::addSkill(UInt8 id, UInt16 flag, UInt8 level, UInt16 extra)
{
	_skills[id] = ClanSkillData(id, flag, level, extra);
	DB().PushUpdateData("REPLACE INTO `clan_skill`(`clanId`, `skillId`, `level`, `extra`) VALUES(%u, %u, %u, %d)", _clan->getId(), id, level, extra);
}

UInt8 ClanSkill::getLev(UInt8 id)
{
	Skills::iterator found = _skills.find(id);
	if (found == _skills.end())
		return 0;
	return found->second.level;
}

Int32 ClanSkill::getExtra(UInt8 id)
{
	Skills::iterator found = _skills.find(id);
	if (found == _skills.end())
		return 0; 
	return found->second.extra;
}

UInt16 ClanSkill::getSouthEdurance()
{
	Skills::iterator found = _skills.find(3);
	if (found == _skills.end())
		return 0;
	return GData::clanSkillTable[3][found->second.level].effect1;
}

UInt16 ClanSkill::getNorthEdurance()
{
	Skills::iterator found = _skills.find(4);
	if (found == _skills.end())
		return 0;
	return GData::clanSkillTable[4][found->second.level].effect1;
}

UInt16 ClanSkill::getBattleAchieve()
{
	Skills::iterator found = _skills.find(1);
	if (found == _skills.end())
		return 0;
	UInt8 lev = found->second.level + 1;
	if (lev <= GData::clanSkillTable[1].size() - 1)
		return GData::clanSkillTable[1][lev].needs;
	else
		return GData::clanSkillTable[1][found->second.level].needs;
}

UInt16 ClanSkill::getClanAchieve()
{
	Int32 achieve = static_cast<Int32>(GData::clanSkillTable[1][getLev(1)].accNeeds) + getExtra(1);
	return static_cast<UInt16>(achieve <= 0 ? 0 : achieve);
}

UInt32 ClanSkill::getAtuobattleSpeed() //挂机加速 
{
	Mutex::ScopedLock lk(_mutex);
	Skills::iterator found = _skills.find(2);
	if (found == _skills.end())
		return 0;
	return GData::clanSkillTable[2][found->second.level].effect1;
}

bool ClanSkill::getHoldAssist(UInt16& cnt, UInt32& mt)
{
	cnt = 0;
	mt = 0;

	Skills::iterator found = _skills.find(5);
	if (found != _skills.end())
		mt = GData::clanSkillTable[5][found->second.level].effect1; 
	
	found = _skills.find(6);
	if (found != _skills.end())
		cnt = GData::clanSkillTable[6][found->second.level].effect1;

	return mt != 0 && cnt != 0;
}

UInt32 ClanSkill::getHoldTotemGuarder()
{
	Skills::iterator found = _skills.find(9);
	if (found == _skills.end())
		return 0;
	return GData::clanSkillTable[9][found->second.level].effect1;
}

UInt32 ClanSkill::getHoldCityGuarder()
{
	Skills::iterator found = _skills.find(8);
	if (found == _skills.end())
		return 0;
	return GData::clanSkillTable[8][found->second.level].effect1;
}


bool ClanSkill::isSkillFull(UInt8 id)
{
	Skills::iterator found = _skills.find(id);
	if (found == _skills.end())
		return false;
	return getLev(id) >= static_cast<UInt8>(GData::clanSkillTable[id].size()) - 1;
}

bool ClanSkill::addAchieve(UInt16 ach)
{
	Skills::iterator found = _skills.find(1);
	if (found == _skills.end())
		return false;
	ClanSkillData& skill = found->second;
	skillLevelUp(skill.skillId, skill.level, skill.extra, ach);
	Stream st(0x78);
	st << static_cast<UInt8>(3);
	makeSkillInfo(st, skill);
	st << Stream::eos;
	_clan->broadcast(st);
	DB().PushUpdateData("UPDATE `clan_skill` SET `level` = %u, `extra` = %d WHERE `clanId` = %u AND `skillId` = %u", skill.level, skill.extra, _clan->getId(), skill.skillId);

	return true;
}

bool ClanSkill::delAchieve(UInt16 ach)
{
	Skills::iterator found = _skills.find(1);
	if (found == _skills.end())
		return false;
	ClanSkillData& skill = found->second;
	skillLevelDown(skill.skillId, skill.level, skill.extra, ach);
	Stream st(0x78);
	st << static_cast<UInt8>(3);
	makeSkillInfo(st, skill);
	st << Stream::eos;
	_clan->broadcast(st);
	DB().PushUpdateData("UPDATE `clan_skill` SET `level` = %u, `extra` = %d WHERE `clanId` = %u AND `skillId` = %u", skill.level, skill.extra, _clan->getId(), skill.skillId);

	return true;
}

}
