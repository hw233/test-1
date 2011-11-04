
#include "DCLogger.h"
#include "Common/Itoa.h"
#include "Player.h"
#include "Server/Cfg.h"
#include "Log/Log.h"
#include <sstream>

namespace GObject
{

bool DCLogger::init()
{
    m_logger = new (std::nothrow) CLogger();
    if (!m_logger)
        return false;

#if 0
    char buf[32] = {0};
    snprintf(buf, sizeof(buf), "app%u", _APPID); 
    std::string appname = buf;
#else
    std::string appname = "appoperlog";
#endif
    if (m_logger->init(appname))
        return false;

    version = _VERSION;
    appid = _APPID;

    memset(m_onlineNum_domain, 0, sizeof(m_onlineNum_domain));
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        m_domain[i] = i + 1;
    }

    return true;
}

void DCLogger::incDomainOnlineNum(UInt8 domain)
{
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        if(domain == m_domain[i])
            ++ m_onlineNum_domain[i];
    }
}

void DCLogger::decDomainOnlineNum(UInt8 domain)
{
    for(int i = 0; i < MAX_DOMAIN; ++i)
    {
        if(domain == m_domain[i] && m_onlineNum_domain[i] > 0)
            -- m_onlineNum_domain[i];
    }
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
    if (m_logger && m_logger->write_baselog(LT_BASE, data, true))
        return false;
    TRACE_LOG("%s", data.c_str());
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
    if (m_logger && m_logger->write_baselog(LT_BASE, data, true))
        return false;
    TRACE_LOG("%s", data.c_str());
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
    msg << "&onlinetime=";
    msg << time(NULL) - player->getLastOnline(); // TODO:
    msg << "&source=";
    msg << player->getSource();

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_BASE, data, true))
        return false;
    TRACE_LOG("%s", data.c_str());
#endif

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
    msg << (int)domain;
    msg << "&worldid=";
    msg << cfg.serverNum;
    msg << "&optype=5&actionid=14";
    msg << "&user_num=";
    msg << num;
    msg << "&opuid=";
    msg << cfg.serverNum;

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_BASE, data, true))
        return false;
    TRACE_LOG("%s", data.c_str());
#endif

    return true;
}

void DCLogger::fee(Player* player, Int32 c)
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
    msg << "&optype=1&actionid=5";
    msg << "&opuid=";
    msg << player->getId();
    msg << "&opopenid=";
    msg << player->getOpenId();
    msg << "&key=";
    msg << player->getOpenKey();
    msg << "&modifyfee=";
    msg << c; // TODO:

#ifdef _DEBUG
    fprintf(stderr, "%s\n", msg.str().c_str());
#endif

#ifndef _DEBUG
    std::string data = msg.str();
    FastMutex::ScopedLock lck(m_lck);
    if (m_logger && m_logger->write_baselog(LT_BASE, data, true))
        return false;
    TRACE_LOG("%s", data.c_str());
#endif
}

DCLogger dclogger;

} // namespace GObject

