
#ifndef DCLOGGER_H_
#define DCLOGGER_H_

#include "Config.h"
#include "Common/Mutex.h"
#include "dcapi_cpp.h"

using namespace DataCollector;

namespace GObject
{

class Player;

class DCLogger
{
public:
    DCLogger() : m_logger(NULL) {}
    ~DCLogger() { delete m_logger; }

    bool init();

    bool reg(Player* player);
    bool login(Player* player);
    bool logout(Player* player);
    bool online(UInt32 num, UInt8 domain);

    void normal();

private:
    CLogger* m_logger;
    FastMutex m_lck;

    UInt32 version;
    UInt32 appid;
    UInt32 svrip;
    UInt32 area; // worldid
    std::string pf; // domain
};

extern DCLogger dclogger;

} // namespace GObject

#endif // DCLOGGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


