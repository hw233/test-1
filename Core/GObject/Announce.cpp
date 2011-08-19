#include "Config.h"
#include "Announce.h"
#include "Common/TimeUtil.h"
#include "Server/Cfg.h"
#include "Server/WorldServer.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecHelper.h"
#include "Common/StringTokenizer.h"
#include "MsgID.h"

namespace GObject
{

struct DBAnnounce
{
	UInt32 startTime;
	UInt32 interval;
	UInt8 color;
	UInt32 endTime;
	std::string msg;
	std::string link;
};

}

namespace DB {

SPECIALBEGIN(GObject::DBAnnounce)
SPECIALDEF(6)
(
	UInt32, startTime,
	UInt32, interval,
	UInt8, color,
	UInt32, endTime,
	std::string, msg,
	std::string, link
)
SPECIALEND()

}

namespace GObject
{

Announce announce;

Announce::Announce()
{
}

void Announce::reload()
{
	UInt32 curtime = TimeUtil::Now();
	_items.clear();
	std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
	if (execu.get() == NULL || !execu->isConnected()) return;
	DBAnnounce dba;
	if(execu->Prepare("SELECT `startTime`, `interval`, `color`, `endTime`, `msg`, `link` FROM `announce`", dba) != DB::DB_OK)
		return;
	while(execu->Next() == DB::DB_OK)
	{
		if(dba.interval == 0)
			continue;
		AnnItem item;
		item.interval = dba.interval;
		item.text = dba.msg;
		item.timeEnd = dba.endTime;
		item.type = dba.color;
		UInt32 timeStart;
		if(dba.startTime < 1000000)
		{
			timeStart = curtime / item.interval * item.interval + dba.startTime;
		}
		else
			timeStart = dba.startTime;
		if(timeStart < curtime)
		{
			timeStart = curtime + item.interval - (curtime - timeStart) % item.interval;
		}
		if(timeStart > dba.endTime)
			continue;
		_items.insert(std::make_pair(timeStart, item));
	}
}

void Announce::process( UInt32 curtime )
{
	if(_items.empty()) return;

	std::multimap<UInt32, AnnItem>::iterator it = _items.begin();
	while(it != _items.end())
	{
		if(it->first > curtime)
			return;
		AnnItem& ai = it->second;
		Stream st(REP::SYSTEM_INFO);
		st << static_cast<UInt8>(ai.type + 0x10) << ai.text << Stream::eos;
		NETWORK()->Broadcast(st);
		UInt32 cnt = (curtime - it->first) / ai.interval + 1;
		UInt32 nexttime = ai.interval * cnt + it->first;
		if(nexttime < ai.timeEnd)
			_items.insert(std::make_pair(nexttime, ai));
		_items.erase(it ++);
	}
}

}
