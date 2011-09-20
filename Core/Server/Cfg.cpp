#include "Config.h"
#include "Cfg.h"
#include "Network/Network.h"
#include "Script/ConfigScript.h"
#include "Common/StringTokenizer.h"

Cfg::Cfg( ): tcpPort(8888), serverLogId(0), dbDataPort(3306), dbObjectPort(3306),
	openYear(2010), openMonth(5), openDay(1), enableWallow(false), limitLuckyDraw(0),
	merged(false), supportCompress(true), GMCheck(true), channelNum(0), serverNum(0), arenaPort(0),
	_filename("conf/config.lua"), enableLoginLimit(false), loginLimit(10000), onlineLimit(0)
{
}

void Cfg::load(const char * scriptStr)
{
	Script::ConfigScript script(this);
	if(scriptStr != NULL)
	{
		script.runScript(scriptStr);
		return;
	}
	script.doFile(_filename.c_str());
}

Cfg::IPMask Cfg::parseAddress(const std::string& addr)
{
	StringTokenizer tk(addr, "/");
	Cfg::IPMask mask = {0, 0};
	if(tk.count() == 0)
		return mask;
	if(tk.count() > 1)
		mask.mask = 32 - atoi(tk[1].c_str());
	mask.addr = ntohl(inet_addr(tk[0].c_str()));
	return mask;
}

void Cfg::setAdminAllowedIP( const char * str )
{
	StringTokenizer tk(str, " ");
	for(size_t i = 0; i < tk.count(); ++ i)
	{
		IPMask ipm = parseAddress(tk[i]);
		if(ipm.mask < 32)
			_adminIPAllowed.push_back(ipm);
	}
}

bool Cfg::isAdminIPAllowed( UInt32 ip )
{
	if(_adminIPAllowed.empty())
		return true;
	for(size_t i = 0; i < _adminIPAllowed.size(); ++ i)
	{
		if(!((_adminIPAllowed[i].addr ^ ip) >> _adminIPAllowed[i].mask))
			return true;
	}
	return false;
}

Cfg cfg;
