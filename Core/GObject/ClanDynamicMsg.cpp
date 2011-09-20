#include "Config.h"
#include "Clan.h"
#include "Server/WorldServer.h"
#include "MsgID.h"

namespace GObject
{
//人事0 城战1 技艺2 其他3 
//人事0x0001 城战0x0010 技艺0x0100 其他0x1000
static const UInt8 CDMTypeCvt[] = { 0, 0, 1, 1, 1, 1, 1, 1, 2, 0, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1, 1, 1, 3 };
static const UInt8 CDMTCvt[] = { 0x01, 0x02, 0x04, 0x08 };

ClanDynamicMsg::ClanDynamicMsg() : _key(0) 
{
}

ClanDynamicMsg::~ClanDynamicMsg()
{
	for (UInt8 i = 0; i < 4; ++ i)
	{
		std::multimap<CDMKey, CDMsg *, CDMKeyLess>::iterator offset = _cdm[i].begin();
		for (; offset != _cdm[i].end(); ++ offset)
			delete offset->second;
		_cdm[i].clear();
	}
}

UInt16 ClanDynamicMsg::getCDMsgSize(UInt8 type)
{
	
	UInt16 sz = 0;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		if ((type & CDMTCvt[i]) == CDMTCvt[i])
		{
			sz += _cdm[i].size();
		}
	}
	return sz;
}

void ClanDynamicMsg::makeCDMsgInfor(Stream& st, UInt8 type, UInt16 start, UInt16 count)
{
	Mutex::ScopedLock lk(_mutex);
	std::multimap<CDMKey, CDMsg *, CDMKeyLess> merge;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		if ((type & CDMTCvt[i]) == CDMTCvt[i])
			merge.insert(_cdm[i].begin(), _cdm[i].end());
	}
	if (merge.empty())
		return;
	UInt16 sz = merge.size();
	UInt16 end = start + count;
	if (end > sz)
		end = sz;
	if (end < start)
		count = 0;
	else
		count = end - start;
	UInt32 now = TimeUtil::Now();
	CDMsg * msg = NULL;
	std::multimap<CDMKey, CDMsg *, CDMKeyLess>::iterator offset = merge.begin();
	std::advance(offset, start);
	for (UInt16 j = 0; offset != merge.end() && j < count; ++ j, ++ offset)
	{
		msg= offset->second;
		st << msg->type << (now - offset->first.key1);
		for (std::vector<CDMValType>::iterator it = msg->value.begin(); it != msg->value.end(); ++ it)
		{
			if (it->type == 0)
				st << it->intval;
			else if (it->type == 1)
				st << it->strval;
		}
	}
}

void ClanDynamicMsg::makeCBMsgInfor(Stream& st, UInt32 time)
{
	ClanBattleReportElems::iterator found = _cbrElem.find(time);
	if (found == _cbrElem.end())
	{
		st << static_cast<UInt8>(1) << static_cast<UInt16>(0);
		return;
	}
	ClanBattleReportElem& elem = found->second;
	st << elem.first << static_cast<UInt16>(elem.second.size());
	std::vector<ClanBattleReport>::iterator iter = elem.second.begin();
	for (; iter != elem.second.end(); ++ iter)
	{
		st << iter->battlerName << iter->clanName << iter->battlerLev << iter->reliveCount << iter->serailWins << iter->maxSerailWins \
			<< iter->side << iter->northEdurance << iter->southEdurance << iter->grabAchieve;
	}
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, UInt32 val, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(0, val));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, UInt32 val1, UInt32 val2, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(0, val1));
		msg->value.push_back(CDMValType(0, val2));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1,val));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, const std::string& val3, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		msg->value.push_back(CDMValType(1, val3));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, UInt32 val2, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(0, val2));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, UInt32 val2, UInt32 val3, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(0, val2));
		msg->value.push_back(CDMValType(0, val3));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, UInt32 val3, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		msg->value.push_back(CDMValType(0, val3));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, const std::string& val3, UInt32 val4, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		msg->value.push_back(CDMValType(1, val3));
		msg->value.push_back(CDMValType(0, val4));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << val4 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, UInt32 val2, UInt32 val3, UInt32 val4, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(0, val2));
		msg->value.push_back(CDMValType(0, val3));
		msg->value.push_back(CDMValType(0, val4));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << val4 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, const std::string& val3, UInt32 val4, UInt32 val5, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		msg->value.push_back(CDMValType(1, val3));
		msg->value.push_back(CDMValType(0, val4));
		msg->value.push_back(CDMValType(0, val5));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << val4 << val5 << Stream::eos;
		}
		return true;	
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, UInt32 val3, UInt32 val4, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		msg->value.push_back(CDMValType(0, val3));
		msg->value.push_back(CDMValType(0, val4));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << val4 << Stream::eos;
		}
		return true;
	}
	return false;
}

bool ClanDynamicMsg::addCDMsg(UInt8 type, const std::string& val1, const std::string& val2, const std::string& val3, const std::string& val4, UInt32 val5, Stream * pst)
{
	Mutex::ScopedLock lk(_mutex);
	if (type <= MSIZE)
	{
		CDMsg * msg = new CDMsg(type);
		msg->value.push_back(CDMValType(1, val1));
		msg->value.push_back(CDMValType(1, val2));
		msg->value.push_back(CDMValType(1, val3));
		msg->value.push_back(CDMValType(1, val4));
		msg->value.push_back(CDMValType(0, val5));
		_cdm[CDMTypeCvt[type]].insert(std::make_pair(CDMKey(TimeUtil::Now(), _key++), msg));
		if (pst != NULL)
		{
			Stream& st = *pst;
			st.init(REP::CLAN_DINFO_UPDATE);
			st << type << static_cast<UInt32>(0) << val1 << val2 << val3 << val4 << val5 << Stream::eos;
		}
		return true;		
	}
	return false;
}

void ClanDynamicMsg::addCBReportFromDB(UInt32 time, ClanBattleReport& rpt)
{
	ClanBattleReportElem& elem = _cbrElem[time];
	elem.second.push_back(rpt);
}

void ClanDynamicMsg::addCBResultFromDB(UInt32 clanId, UInt32 time, UInt8 result)
{
	ClanBattleReportElem& elem = _cbrElem[time];
	elem.first = result;
	CDMsg * msg = new CDMsg(result == 0 ? 3 : 2);
	msg->value.push_back(CDMValType(0, clanId));
	msg->value.push_back(CDMValType(0, time));
	_cdm[CDMTypeCvt[3]].insert(std::make_pair(CDMKey(TimeUtil::SharpDay(0, time), _key++), msg));
}

ClanBattleReportElem& ClanDynamicMsg::getCBReport(UInt32 time)
{
	return _cbrElem[time];
}

void ClanDynamicMsg::cleanClanDynamicMsg()
{
	UInt32 deletedDay = TimeUtil::SharpDay(-7);
	{
		Mutex::ScopedLock lk(_mutex);
		for (UInt8 i = 0; i < 4; ++ i)
		{
			std::multimap<CDMKey, CDMsg *, CDMKeyLess>::iterator iter =	_cdm[i].lower_bound(CDMKey(deletedDay, 0));
			for (; iter != _cdm[i].end();)
			{
				delete iter->second;
				_cdm[i].erase(iter ++);
			}
		}
	}
	ClanBattleReportElems::iterator deletedStart = _cbrElem.find(deletedDay);
	_cbrElem.erase(deletedStart, _cbrElem.end());
	DB5().PushUpdateData("DELETE FROM `clan_battle_result` WHERE `battleTime` < %u", deletedDay);
}
	

}
