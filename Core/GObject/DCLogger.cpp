
#include "DCLogger.h"
#include "Common/Itoa.h"
#include "Player.h"
#include "Server/Cfg.h"
#include <sstream>

namespace GObject
{

bool DCLogger::init()
{
    m_logger = new (std::nothrow) CLogger();
    if (!m_logger)
        return false;

    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "app%u", _APPID); 
    std::string appname = buf;
    if (m_logger->init(appname))
        return false;

    version = _VERSION;
    appid = _APPID;

    return true;
}

bool DCLogger::reg(Player* player)
{
#ifndef _DEBUG
    if (!m_logger)
        return false;
#endif

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

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_NORMAL, data, true))
        return false;
#endif

    return true;
}

bool DCLogger::login(Player* player)
{
#ifndef _DEBUG
    if (!m_logger)
        return false;
#endif
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

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_NORMAL, data, true))
        return false;
#endif

    return true;
}

bool DCLogger::logout(Player* player)
{
#ifndef _DEBUG
    if (!m_logger)
        return false;
#endif
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
    msg << "&source=";
    msg << player->getSource();

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_NORMAL, data, true))
        return false;
#endif

    return true;
}

bool DCLogger::online(UInt32 num)
{
#ifndef _DEBUG
    if (!m_logger)
        return false;
#endif
    std::ostringstream msg;

    msg << "version=";
    msg << version;
    msg << "&appid=";
    msg << appid;
    msg << "&time=";
    msg << time(NULL);
    msg << "&domain="; // TODO:
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=5&actionid=14";
    msg << "&user_num=";
    msg << num;

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_NORMAL, data, true))
        return false;
#endif

    return true;
}

DCLogger dclogger;

} // namespace GObject

