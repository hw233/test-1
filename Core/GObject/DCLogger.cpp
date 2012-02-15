
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
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        if(domain == m_domain[i])
            ++m_onlineNum_domain[i];
    }
}

void DCLogger::decDomainOnlineNum(UInt8 domain)
{
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        if(domain == m_domain[i] && m_onlineNum_domain[i] > 0)
            --m_onlineNum_domain[i];
    }
}

bool DCLogger::reg(Player* player)
{
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

bool DCLogger::login(Player* player)
{
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

bool DCLogger::logout(Player* player)
{
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

void DCLogger::online()
{
    for(int i = 0; i < MAX_DOMAIN; ++ i)
    {
        if(m_onlineNum_domain[i])
            online(m_onlineNum_domain[i], m_domain[i]);
    }
}

bool DCLogger::online(UInt32 num, UInt8 domain)
{
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

bool DCLogger::fee(Player* player, UInt32 total, Int32 c)
{
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
    msg << "&optype=1&actionid=5";
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

bool DCLogger::blue(Player* player)
{
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

bool DCLogger::d3d6(Player* player)
{
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

    DC().Push(msg.str().c_str(), msg.str().length());
    return true;
}

DCLogger dclogger;

} // namespace GObject

