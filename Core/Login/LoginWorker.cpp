#include "Config.h"
#include "LoginWorker.h"
#include "Server/Cfg.h"
#include "MsgHandler/Memcached.h"
namespace Login
{

LoginWorker::LoginWorker() : m_count(0), m_current(0)
{

}

LoginWorker::~LoginWorker()
{

}

bool LoginWorker::Init()
{
	initPlatformLogin();
    return true;
}

void LoginWorker::UnInit()
{
}

std::string LoginWorker::GetLogName()
{
	return "log/Login/";
}

}
