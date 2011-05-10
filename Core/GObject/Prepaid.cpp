#include "Config.h"
#include "Prepaid.h"
#include "Server/WorldServer.h"

namespace GObject
{

Prepaid prepaid;

void Prepaid::push( UInt64 id, UInt32 g, bool writedb )
{
	UInt32& ng = _paidSet[id];
	ng += g;
	if(writedb)
	{
		DB().PushUpdateData("REPLACE INTO `prepaid`(`id`, `gold`) VALUES(%"I64_FMT"u, %u)", id, ng);
	}
}

UInt32 Prepaid::pop( UInt64 id )
{
	std::map<UInt64, UInt32>::iterator it = _paidSet.find(id);
	if(it == _paidSet.end())
		return 0;
	UInt32 r = it->second;
	DB().PushUpdateData("DELETE FROM `prepaid` WHERE `id` = %"I64_FMT"u", id);
	_paidSet.erase(it);
	return r;
}

}
