#include "Config.h"
#include "ConfigScript.h"

#include "Server/Cfg.h"

namespace Script
{

ConfigScript::ConfigScript( Cfg * cfg )
{
	class_add<Cfg>("Cfg");
	class_def<Cfg>("setTcpPort", &Cfg::setTcpPort);
	class_def<Cfg>("setIfName", &Cfg::setIfName);
	class_def<Cfg>("setScriptPath", &Cfg::setScriptPath);
	class_def<Cfg>("setReportPath", &Cfg::setReportPath);
	class_def<Cfg>("setServerLogId", &Cfg::setServerLogId);
	class_def<Cfg>("setAnnounceFile", &Cfg::setAnnounceFile);
	class_def<Cfg>("setDataDatabase", &Cfg::setDataDatabase);
	class_def<Cfg>("setObjectDatabase", &Cfg::setObjectDatabase);
	class_def<Cfg>("setLogDatabase", &Cfg::setLogDatabase);
	class_def<Cfg>("setLockDatabase", &Cfg::setLockDatabase);
	class_def<Cfg>("setCryptKeys", &Cfg::setCryptKeys);
	class_def<Cfg>("setGMCryptKeys", &Cfg::setGMCryptKeys);
	class_def<Cfg>("setFBRechargeKeys", &Cfg::setFBRechargeKeys);
	class_def<Cfg>("setAdminAllowedIP", &Cfg::setAdminAllowedIP);
	class_def<Cfg>("setOpening", &Cfg::setOpening);
	class_def<Cfg>("setMergeList", &Cfg::setMergeList);
	class_def<Cfg>("setWallow", &Cfg::setWallow);
	class_def<Cfg>("setLimitLuckyDraw", &Cfg::setLimitLuckyDraw);
	class_def<Cfg>("setMerged", &Cfg::setMerged);
	class_def<Cfg>("setSupportCompress", &Cfg::setSupportCompress);
	class_def<Cfg>("setGMCheck", &Cfg::setGMCheck);
    class_def<Cfg>("setTestPlatform", &Cfg::setTestPlatform);
	class_def<Cfg>("setArenaServer", &Cfg::setArenaServer);
    class_def<Cfg>("setMsgCenter", &Cfg::setMsgCenter);   //yij
	class_def<Cfg>("setChannelInfo", &Cfg::setChannelInfo);
	class_def<Cfg>("setLoginLimit", &Cfg::setLoginLimit);
	class_def<Cfg>("setLoginMax", &Cfg::setLoginMax);
	class_def<Cfg>("setVerifyTokenServer", &Cfg::setVerifyTokenServer);
    class_def<Cfg>("setIDQueryMemcachedServer", &Cfg::setIDQueryMemcachedServer);
	class_def<Cfg>("setOnlineLimit", &Cfg::setOnlineLimit);
	class_def<Cfg>("setStateUrl", &Cfg::setStateUrl);
	class_def<Cfg>("setChargeUrl", &Cfg::setChargeUrl);
	class_def<Cfg>("setWarZone", &Cfg::setWarZone);
	class_def<Cfg>("setServerNum", &Cfg::setServerNum);
	class_def<Cfg>("setServerNo", &Cfg::setServerNo);
	class_def<Cfg>("setUdpLog", &Cfg::setUdpLog);
	class_def<Cfg>("setDCLog", &Cfg::setDCLog);
    class_def<Cfg>("setSecDCLog", &Cfg::setSecDCLog);
    class_def<Cfg>("setSecDCLogTest", &Cfg::setSecDCLogTest);   // 安全上报内测区调试用
    class_def<Cfg>("setUnionPlatform", &Cfg::setUnionPlatform);
    class_def<Cfg>("setAutoForbid", &Cfg::setAutoForbid);       // 设置自动封交易功能
    class_def<Cfg>("setAutoKick", &Cfg::setAutoKick);           // 设置openid校验自动踢人功能

	set("cfg", cfg);
}

}
