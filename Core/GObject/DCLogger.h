
#ifndef DCLOGGER_H_
#define DCLOGGER_H_

#include "Config.h"
#include "Common/Mutex.h"
#include "dcapi_cpp.h"

using namespace DataCollector;

namespace GObject
{
#define MAX_DOMAIN 15

class Player;

class DCLogger
{
public:
    DCLogger() {}
    ~DCLogger() {}

    bool init();

    bool reg(Player* player);
    bool login(Player* player);
    bool logout(Player* player);
    bool online(UInt32 num, UInt8 domain);
    bool fee(Player* player, UInt32 total, Int32 c);
    bool blue(Player* player);
    bool d3d6(Player* player);

    void online();

    void normal();
    void incDomainOnlineNum(UInt8 domain);
    void decDomainOnlineNum(UInt8 domain);
    void getOnline(UInt32* buf)
    {
        if (!buf) return;
        memcpy(buf, m_onlineNum_domain, sizeof(m_onlineNum_domain));
    }

private:
    UInt32 version;
    UInt32 appid;
    UInt32 m_onlineNum_domain[MAX_DOMAIN];
    UInt32 m_domain[MAX_DOMAIN];
};

extern DCLogger dclogger;

} // namespace GObject

#endif // DCLOGGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

