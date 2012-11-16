
#include "DCLogger.h"
#include "Common/Itoa.h"
#include "Player.h"
#include "Server/Cfg.h"
#include "Server/SysMsg.h"
#include "Log/Log.h"
#include "DCWorker.h"
#include <sstream>

class SysMsgItem;

namespace GObject
{

#ifndef _FB
#ifndef _VT
#ifndef _WIN32


#endif
#endif
#endif

static const char *msgVersion = "0.1";
static const char *ver = "1.5";

bool DCLogger::init()
{
    version = _VERSION;
    appid = _APPID;

    memset(m_onlineNum_domain, 0, sizeof(m_onlineNum_domain));
    for(int i = 0; i < MAX_DOMAIN; ++i)
        m_domain[i] = i + 1;

    return true;
}

void DCLogger::incDomainOnlineNum(UInt8 domain)
{
    if (!cfg.dclog)
        return;
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        if(domain == m_domain[i])
            ++m_onlineNum_domain[i];
    }
}

void DCLogger::decDomainOnlineNum(UInt8 domain)
{
    if (!cfg.dclog)
        return;
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        if(domain == m_domain[i] && m_onlineNum_domain[i] > 0)
            --m_onlineNum_domain[i];
    }
}

bool DCLogger::reg(Player* player)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=3&actionid=2";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&source=";
    msg << player->getSource();
    msg << "&touid=&toopenid=&level=&itemid=&itemtype=&itemcnt=&modifyexp=&totalexp=&modifycoin=&totalcoin=&modifyfee=&totalfee=&onlinetime=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

bool DCLogger::login(Player* player)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=4&actionid=1";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&source=";
    msg << player->getSource();
    msg << "&touid=&toopenid=&level=&itemid=&itemtype=&itemcnt=&modifyexp=&totalexp=&modifycoin=&totalcoin=&modifyfee=&totalfee=&onlinetime=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

bool DCLogger::login_sec(Player* player)
{
    if (!cfg.secdclog)
        return true;
    if (!cfg.dclog)
        return true;
    if (!player)
        return false;

    std::ostringstream msg;


    msg << "APPV=";
    msg << version;
    msg << "&MSGV=";
    msg << msgVersion;
    msg << "&VER=";
    msg << ver;
    msg << "&APPID=";
    msg << appid;
    msg << "&OID=";
    msg << player->getOpenId();
    msg << "&WID=";
    msg << cfg.serverNum;
    msg << "&UIP=";
    msg << player->getClientIp();
    msg << "&OKY=";
    msg << player->getOpenKey();
    msg << "&SIP=";
    char serverIp[20];
    in_addr iaddr;
    iaddr.s_addr = cfg.serverIp;
    strncpy(serverIp, inet_ntoa(iaddr), 20);
    msg << serverIp;
    msg << "&MTM=";
    msg << TimeUtil::GetTick() / 1000;
    msg << "&DOM=";
    msg << player->getDomain();
    msg << "&MLV=0";
    msg << "&AID=1";

    msg << "&ACT=" << static_cast<UInt64>(static_cast<UInt64>(player->getCreated()) * 1000);
    msg << "&PAY=" << static_cast<UInt32>(player->getTotalRecharge()? 1:0);
    msg << "&RID=" << player->getId();
    msg << "&RNA=" << player->getName();
    msg << "&RLV=" << static_cast<UInt32>(player->GetLev());
    msg << "&EXP=" << static_cast<UInt32>(player->GetExp());
    msg << "&RTN=2";
    msg << "&RTI=" << "job";
    msg << "&RTP=" << static_cast<UInt32>(player->GetClass());
    msg << "&RTI=" << "country";
    msg << "&RTP=" << static_cast<UInt32>(player->getCountry());
    msg << "&GTN=2";
    msg << "&GTI=tael";
    msg << "&GOD=" << static_cast<UInt32>(player->getTael());
    msg << "&GTI=Coupon";
    msg << "&GOD=" << static_cast<UInt32>(player->getCoupon());
    msg << "&CTN=1";
    msg << "&CTI=gold";
    msg << "&CAS=" << static_cast<UInt32>(player->getGold());
    msg << "&RCT=" << static_cast<UInt64>(static_cast<UInt64>(player->getCreated()) * 1000);

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length(), LT_SECDATA);
#endif
#endif
#endif
    return true;
}

bool DCLogger::create_sec(UserStruct const &nu, Player* player /* = NULL */)
{
    if (!cfg.secdclog)
        return true;
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    if (player)
        return login_sec(player);


    msg << "APPV=";
    msg << version;
    msg << "&MSGV=";
    msg << msgVersion;
    msg << "&VER=";
    msg << ver;
    msg << "&APPID=";
    msg << appid;
    msg << "&OID=";
    msg << nu._openid;
    msg << "&WID=";
    msg << cfg.serverNum;
    msg << "&UIP=";
    msg << nu._clientIp;
    msg << "&OKY=";
    msg << nu._openkey;
    msg << "&SIP=";
    char serverIp[20];
    in_addr iaddr;
    iaddr.s_addr = cfg.serverIp;
    strcpy(serverIp, inet_ntoa(iaddr));
    msg << serverIp;
    msg << "&MTM=";
    msg << TimeUtil::GetTick() / 1000;
    msg << "&DOM=";
    msg << static_cast<UInt16> (atoi(nu._platform.c_str()));
    msg << "&MLV=0";
    msg << "&AID=1";

    msg << "&ACT=" << static_cast<UInt64>(0);
    msg << "&PAY=" << static_cast<UInt32>(0);
    msg << "&RID=" << static_cast<UInt64>(0);
    msg << "&RNA=" << nu._name;
    msg << "&RLV=" << static_cast<UInt32>(0);
    msg << "&EXP=" << static_cast<UInt32>(0);
    msg << "&RTN=2";
    msg << "&RTI=" << "job";
    msg << "&RTP=" << static_cast<UInt32>(nu._class);
    msg << "&RTI=" << "country";
    msg << "&RTP=" << static_cast<UInt32>(2);
    msg << "&GTN=2";
    msg << "&GTI=tael";
    msg << "&GOD=" << static_cast<UInt32>(0);
    msg << "&GTI=coin";
    msg << "&GOD=" << static_cast<UInt32>(0);
    msg << "&CTN=1";
    msg << "&CTI=gold";
    msg << "&CAS=" << static_cast<UInt32>(0);
    msg << "&RCT=" << static_cast<UInt64>(0);

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length(), LT_SECDATA);
#endif
#endif
#endif
    return true;
}

bool DCLogger::protol_sec(Player* player, int cmd)
{
    if (!cfg.secdclog)
        return true;
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    static const char *msgVersion = "0.1";
    static const char *ver = "1.5";

    msg << "APPV=";
    msg << version;
    msg << "&MSGV=";
    msg << msgVersion;
    msg << "&VER=";
    msg << ver;
    msg << "&APPID=";
    msg << appid;
    msg << "&OID=";
    msg << player->getOpenId();
    msg << "&WID=";
    msg << cfg.serverNum;
    msg << "&UIP=";
    msg << player->getClientIp();
    msg << "&OKY=";
    msg << player->getOpenKey();
    msg << "&SIP=";
    char serverIp[20];
    in_addr iaddr;
    iaddr.s_addr = cfg.serverIp;
    strncpy(serverIp, inet_ntoa(iaddr), 20);
    msg << serverIp;
    msg << "&MTM=";
    msg << TimeUtil::GetTick() / 1000;
    msg << "&DOM=";
    msg << player->getDomain();
    msg << "&MLV=0";
    msg << "&AID=20";

    msg << "&RID=";
    msg << player->getId();
    msg << "&PID=" << static_cast<UInt32>(cmd);
    msg << "&FID=";
    msg << "&PTP=" << static_cast<UInt32>(cmd ? 1 : 0);
    msg << "&RST=0";
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length(), LT_SECDATA);
#endif
#endif
#endif
    return true;
}

bool DCLogger::logout(Player* player)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=4&actionid=9";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&onlinetime=";
    msg << time(NULL) - player->getLastOnline(); // TODO:
    msg << "&source=";
    msg << player->getSource();
    msg << "&touid=&toopenid=&level=&itemid=&itemtype=&itemcnt=&modifyexp=&totalexp=&modifycoin=&totalcoin=&modifyfee=&totalfee=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

void DCLogger::online()
{
    if (!cfg.dclog)
        return;
    for(int i = 0; i < MAX_DOMAIN; ++ i)
    {
        if(m_onlineNum_domain[i])
            online(m_onlineNum_domain[i], m_domain[i]);
    }
}

bool DCLogger::online(UInt32 num, UInt8 domain)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain="; // TODO:
    msg << (int)domain;
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=5&actionid=14";
    msg << "&user_num=";
    msg << num;
    msg << "&opuid=";
    msg << cfg.serverNum;
    msg << "&userip=&svrip=&opopenid=&touid=&toopenid=&level=&source=&itemid=&itemtype=&itemcnt=&modifyexp=&totalexp=&modifycoin=&totalcoin=&modifyfee=&totalfee=&onlinetime=&key=&keycheckret=&safebuf=&remark=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

bool DCLogger::consume(Player* player, UInt32 total, UInt32 c)
{
    if (!cfg.dclog)
        return true;
    stringstream msg;
    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=1&actionid=5";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&itemid=29999";
    msg << "&itemtype=29";
    msg << "&itemcnt=1";
    msg << "&modifyfee=";
    msg << c*10;
    msg << "&totalfee=";
    msg << total*10;
    msg << "&touid=&toopenid=&level=&source=&modifyexp=&totalexp=&modifycoin=&totalcoin=&onlinetime=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

bool DCLogger::fee(Player* player, UInt32 total, Int32 c)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=1&actionid=15";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&itemid=29999";
    msg << "&itemtype=29";
    msg << "&itemcnt=";
    msg << (c+9)/10;
    msg << "&modifyfee=";
    msg << c*10;
    if (total)
    {
        msg << "&totalfee=";
        msg << total*10;
    }
    else
        msg << "&totalfee=";
    msg << "&touid=&toopenid=&level=&source=&modifyexp=&totalexp=&modifycoin=&totalcoin=&onlinetime=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

bool DCLogger::blue(Player* player)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=5&actionid=101";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&touid=&toopenid=&level=&source=&itemid=&itemtype=&itemcnt=&modifyexp=&totalexp=&modifycoin=&totalcoin=&modifyfee=&totalfee=&onlinetime=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

bool DCLogger::d3d6(Player* player)
{
    if (!cfg.dclog)
        return true;
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&userip=";
    msg << player->getClientAddress();
    msg << "&svrip=";
    msg << cfg.serverIp;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain=";
    msg << player->getDomain();
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=5&actionid=102";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&touid=&toopenid=&level=&source=&itemid=&itemtype=&itemcnt=&modifyexp=&totalexp=&modifycoin=&totalcoin=&modifyfee=&totalfee=&onlinetime=&keycheckret=&safebuf=&remark=&user_num=";

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    DC().Push(msg.str().c_str(), msg.str().length());
#endif
#endif
#endif
    return true;
}

DCLogger dclogger;

} // namespace GObject

