#include "Config.h"
#include "LoginWorker.h"

namespace Login
{

LoginWorker::LoginWorker()
{

}

LoginWorker::~LoginWorker()
{

}

bool LoginWorker::Init()
{
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
