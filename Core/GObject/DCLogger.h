
#ifndef DCLOGGER_H_
#define DCLOGGER_H_

#include "Config.h"
#include "Common/Mutex.h"
#include "dcapi_cpp.h"

struct UserStruct
{
	std::string _name;
	UInt8 _class;
    UInt8 _level;
    UInt8 _level1;
    UInt8 _isYear;
    std::string _platform;
    std::string _openid;
    std::string _openkey;
    std::string _via;
    std::string _invited;
    UInt8 _rp;
    std::string _clientIp;
};


using namespace DataCollector;

#if 0
    1   qzone
    2   pengyou
    2   xiaoyou
    3   qqweibo
    4   qplus
    5   caifutong
    10  qqgame
    11  q3366
    12  kingxin(s3)
    16  gamelife
    17  qqunion
    40  facebook
    41  6waves
    42  113
    43  pps
    60  kingnet(xy)
    61  union_xy
    62  gamebase
    63  pubgame
#endif

namespace GObject
{
#define MAX_DOMAIN 256


class Player;

class DCLogger
{
public:
    DCLogger() {}
    ~DCLogger() {}

    bool init();

    bool reg(Player* player);
    bool login(Player* player);
    bool login_sec(Player* player);
    bool create_sec(UserStruct const &nu, Player* player = NULL);
    bool protol_sec(Player* player, int cmd);
    bool trade_sec(Player* saller, Player* buyer, UInt32 itemId, UInt32 itemCount, UInt32 price);
    bool gold_sec(Player* player, UInt32 count, UInt32 changeType, bool isIncrease);
    bool tael_sec(Player* player, UInt32 count, UInt32 changeType, bool isIncrease);
    bool coupon_sec(Player* player, UInt32 count, UInt32 changeType, bool isIncrease);
    bool logout(Player* player);
    bool online(UInt32 num, UInt8 domain);
    bool consume(Player* player, UInt32 total, UInt32 c);
    bool fee(Player* player, UInt32 total, Int32 c);
    bool blue(Player* player);
    bool d3d6(Player* player);
    UInt8 getDomain_sec(Player* player);

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

