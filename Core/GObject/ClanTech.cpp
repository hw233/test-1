#include "Config.h"

#include "Common/Stream.h"
#include "Server/SysMsg.h"
#include "GData/ClanTechTable.h"
#include "Player.h"
#include "ClanTech.h"
#include "Clan.h"
#include "GObject/PracticePlace.h"

namespace GObject
{

static UInt8 clanTechDonateType[] = { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 }; //宗族等级1、宗族经验2、南门防护3(南门耐久度)、北门防护4（北门耐久度）、护卫等级5(NPC等级)、护卫数量6(NPC数量)、(青龙、白虎、朱雀、玄武7)、城门图腾8、宗祠图腾9

ClanTech::ClanTech(Clan * c) : _clanLev(0), _clan(c)
{
	
}

ClanTech::~ClanTech()
{

}


void ClanTech::addTechFromDB(UInt8 techId, UInt8 level, UInt16 extra)
{
	_techs.insert(std::make_pair(techId, ClanTechData(techId, clanTechDonateType[techId], level, extra)));
	if (techId == 1) _clanLev = level;
}

void ClanTech::buildTech()
{
    UInt8 techNum = GData::clanTechTable.size();
	for (UInt8 i = 1; i <= techNum; ++ i)
		addTech(i, 0, 0, 0);
}

bool ClanTech::donate(Player * player, UInt8 id, UInt16 type, UInt16 count)
{
	Techs::iterator found = _techs.find(id);
	if (found == _techs.end())
		return false;
	ClanTechData& tech = found->second;
	switch (type)
	{
	case 1:
		{
			if (techLevelUp(tech.techId, tech.level, tech.extra, count))
			{
				Stream st;
				SYSMSGVP(st, 426);
				_clan->broadcast(st);
			}
			Stream st(0x99);
			st << static_cast<UInt8>(3);
			makeTechInfo(st, tech);
			st << Stream::eos;
			_clan->broadcast(st);
			DB().PushUpdateData("UPDATE `clan_tech` SET `level` = %u, `extra` = %d WHERE `clanId` = %u AND `techId` = %u", tech.level, tech.extra, _clan->getId(), tech.techId);
			return true;
		}
		break;
	case 2:
		{
			techLevelUp(tech.techId, tech.level, tech.extra, count);
			Stream st(0x99);
			st << static_cast<UInt8>(3);
			makeTechInfo(st, tech);
			st << Stream::eos;
			_clan->broadcast(st);
			DB().PushUpdateData("REPLACE INTO `clan_tech`(`clanId`, `techId`, `level`, `extra`) VALUES(%u, %u, %u, %u);", _clan->getId(), tech.techId, tech.level, tech.extra);

			return true;
		}
		break;
	default:
		{	
			//捐献喜好品
			//const UInt8 index[] = {0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7};
			//if(id > 10  || id < 7)
			//	return false;
			//if(_techs.find(index[id]) == _techs.end())
			//	return false;

			//return true;
		}
	}

	return false;
}

void ClanTech::makeTechInfo(Stream& st)
{
	std::map<UInt8, ClanTechData>::iterator offset = _techs.begin();
	for (; offset != _techs.end(); ++ offset)
		st << offset->second.techId << (GData::clanTechTable[offset->second.techId][offset->second.level].accNeeds + static_cast<UInt32>(offset->second.extra));
}

void ClanTech::makeTechInfo(Stream& st, ClanTechData& tech)
{
	st << tech.techId << (GData::clanTechTable[tech.techId][tech.level].accNeeds + static_cast<UInt32>(tech.extra));
}

bool ClanTech::techLevelUp(UInt8 id, UInt8& level, UInt16& extra, UInt16 count)
{
	bool r = false;
	extra += count;
	GData::SingleClanTechTable & techTable = GData::clanTechTable[id];
	UInt8 maxLev = static_cast<UInt8>(techTable.size()) - 1;
	while (level < maxLev && extra >= techTable[level+1].needs && getClanLev() >= techTable[level+1].clanLev)
	{
		r = true;
		++ level;
		extra -= techTable[level].needs;
        switch(id)
        {
        case CLAN_TECH_MEMBER_COUNT:
            _clan->setMaxMemberCount(getMemberCount());
            break;
        case CLAN_TECH_PRACTICE_SLOT:
            practicePlace.addSlotFromTech(_clan->getOwner());
            break;
        }
	}
	if (r)
	{
		Stream st;
		_clan->getClanDynamicMsg()->addCDMsg(8, static_cast<UInt32>(id), static_cast<UInt32>(level), &st);
		_clan->broadcast(st);
	}
	return r;
}

bool ClanTech::techLevelDown(UInt8 id, UInt8& level, UInt16& extra, UInt16 count)
{
	bool r = false;
	if (extra >= count)
		extra -= count;
	else
	{
		UInt32 accu = extra;
		GData::SingleClanTechTable & techTable = GData::clanTechTable[id];
		while (level > 1)
		{
			r = true;
			accu += techTable[level].needs;
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

void ClanTech::addTech(UInt8 id, UInt16 flag, UInt8 level, UInt16 extra)
{
	_techs[id] = ClanTechData(id, flag, level, extra);
	DB().PushUpdateData("REPLACE INTO `clan_tech`(`clanId`, `techId`, `level`, `extra`) VALUES(%u, %u, %u, %d)", _clan->getId(), id, level, extra);
}

UInt8 ClanTech::getLev(UInt8 id)
{
	Techs::iterator found = _techs.find(id);
	if (found == _techs.end())
		return 0;
	return found->second.level;
}

Int32 ClanTech::getExtra(UInt8 id)
{
	Techs::iterator found = _techs.find(id);
	if (found == _techs.end())
		return 0; 
	return found->second.extra;
}

UInt16 ClanTech::getSouthEdurance()
{
	Techs::iterator found = _techs.find(3);
	if (found == _techs.end())
		return 0;
	return GData::clanTechTable[3][found->second.level].effect1;
}

UInt16 ClanTech::getNorthEdurance()
{
	Techs::iterator found = _techs.find(4);
	if (found == _techs.end())
		return 0;
	return GData::clanTechTable[4][found->second.level].effect1;
}

UInt16 ClanTech::getBattleAchieve()
{
	Techs::iterator found = _techs.find(1);
	if (found == _techs.end())
		return 0;
	UInt8 lev = found->second.level + 1;
	if (lev <= GData::clanTechTable[1].size() - 1)
		return GData::clanTechTable[1][lev].needs;
	else
		return GData::clanTechTable[1][found->second.level].needs;
}

UInt16 ClanTech::getClanAchieve()
{
	Int32 achieve = static_cast<Int32>(GData::clanTechTable[1][getLev(1)].accNeeds) + getExtra(1);
	return static_cast<UInt16>(achieve <= 0 ? 0 : achieve);
}

UInt32 ClanTech::getAtuobattleSpeed() //挂机加速 
{
	Mutex::ScopedLock lk(_mutex);
	Techs::iterator found = _techs.find(2);
	if (found == _techs.end())
		return 0;
	return GData::clanTechTable[2][found->second.level].effect1;
}

bool ClanTech::getHoldAssist(UInt16& cnt, UInt32& mt)
{
	cnt = 0;
	mt = 0;

	Techs::iterator found = _techs.find(5);
	if (found != _techs.end())
		mt = GData::clanTechTable[5][found->second.level].effect1; 
	
	found = _techs.find(6);
	if (found != _techs.end())
		cnt = GData::clanTechTable[6][found->second.level].effect1;

	return mt != 0 && cnt != 0;
}

UInt32 ClanTech::getHoldTotemGuarder()
{
	Techs::iterator found = _techs.find(9);
	if (found == _techs.end())
		return 0;
	return GData::clanTechTable[9][found->second.level].effect1;
}

UInt32 ClanTech::getHoldCityGuarder()
{
	Techs::iterator found = _techs.find(8);
	if (found == _techs.end())
		return 0;
	return GData::clanTechTable[8][found->second.level].effect1;
}


bool ClanTech::isTechFull(UInt8 id)
{
	Techs::iterator found = _techs.find(id);
	if (found == _techs.end())
		return false;
	return getLev(id) >= static_cast<UInt8>(GData::clanTechTable[id].size()) - 1;
}

bool ClanTech::addAchieve(UInt16 ach)
{
	Techs::iterator found = _techs.find(1);
	if (found == _techs.end())
		return false;
	ClanTechData& tech = found->second;
	techLevelUp(tech.techId, tech.level, tech.extra, ach);
	Stream st(0x99);
	st << static_cast<UInt8>(3);
	makeTechInfo(st, tech);
	st << Stream::eos;
	_clan->broadcast(st);
	DB().PushUpdateData("UPDATE `clan_tech` SET `level` = %u, `extra` = %d WHERE `clanId` = %u AND `techId` = %u", tech.level, tech.extra, _clan->getId(), tech.techId);

	return true;
}

bool ClanTech::delAchieve(UInt16 ach)
{
	Techs::iterator found = _techs.find(1);
	if (found == _techs.end())
		return false;
	ClanTechData& tech = found->second;
	techLevelDown(tech.techId, tech.level, tech.extra, ach);
	Stream st(0x99);
	st << static_cast<UInt8>(3);
	makeTechInfo(st, tech);
	st << Stream::eos;
	_clan->broadcast(st);
	DB().PushUpdateData("UPDATE `clan_tech` SET `level` = %u, `extra` = %d WHERE `clanId` = %u AND `techId` = %u", tech.level, tech.extra, _clan->getId(), tech.techId);

	return true;
}

UInt32 ClanTech::getPracticeSpeed()
{
	Mutex::ScopedLock lk(_mutex);
    Techs::iterator found = _techs.find(CLAN_TECH_PRACTICE_SPEED);
    if(found == _techs.end())
        return 0;

	return GData::clanTechTable[CLAN_TECH_PRACTICE_SPEED][found->second.level].effect1;
}

UInt32 ClanTech::getPracticeSlot()
{
	Mutex::ScopedLock lk(_mutex);
    Techs::iterator found = _techs.find(CLAN_TECH_PRACTICE_SLOT);
    if(found == _techs.end())
        return 0;

	return GData::clanTechTable[CLAN_TECH_PRACTICE_SLOT][found->second.level].effect1;
}

UInt32 ClanTech::getMemberCount()
{
	Mutex::ScopedLock lk(_mutex);
    Techs::iterator found = _techs.find(CLAN_TECH_MEMBER_COUNT);
    if(found == _techs.end())
        return 0;

	return GData::clanTechTable[CLAN_TECH_MEMBER_COUNT][found->second.level].effect1;
}

UInt32 ClanTech::getSkillExtend()
{
	Mutex::ScopedLock lk(_mutex);
    Techs::iterator found = _techs.find(CLAN_TECH_SKILL_EXTEND);
    if(found == _techs.end())
        return 0;

	return GData::clanTechTable[CLAN_TECH_SKILL_EXTEND][found->second.level].effect1;
}

}
