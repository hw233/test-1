
#ifndef CFRIEND_H_
#define CFRIEND_H_

#include "Config.h"

namespace GObject
{

enum {
    CF_INVITED = 0,
    CF_INV1,
    CF_INV3,
    CF_INV5,
    CF_INV10,
    CF_INV20,
    CF_INV30,
    CF_45_1,
    CF_45_3,
    CF_45_5,
    CF_45_10,
    CF_45_20,
    CF_45_30,
    CF_50_1,
    CF_50_3,
    CF_50_5,
    CF_50_10,
    CF_50_20,
    CF_50_30,
    CF_60_1,
    CF_60_3,
    CF_60_5,
    CF_60_10,
    CF_60_20,
    CF_60_30,
    CF_70_1,
    CF_70_3,
    CF_70_5,
    CF_70_10,
    CF_70_20,
    CF_70_30,
};

class Player;

class CFriend
{
public:
    CFriend(Player* player);
    ~CFriend();

    void loadFromDB(const char* cf);
    void updateToDB();

    void setCFriendSafe(UInt8 idx);
    void setCFriend(UInt8 idx, UInt8 status);
    UInt8 getCFriend(UInt8 idx);
    void setCFriendNum(UInt8 num);

    void getAward(UInt8 idx);
    void updateCFriend(UInt8 idx);
    void sendCFriend();

private:
    Player* m_owner;
    std::vector<UInt8> m_cf; // 0-未达 1-已达 2-已领取
    UInt8 m_maxIdx;
};

} // namespace GObject

#endif // CFRIEND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

