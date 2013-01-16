
#ifndef CFRIEND_H_
#define CFRIEND_H_

#include "Config.h"

namespace GObject
{

enum {
    CF_INVITED = 0,
    CF_RECALL,    //好友召回
    CF_GIVELIFT,  //赠送好友礼物
    CF_GETLIFE,   //领取好友赠送的礼物
    CF_INV3,      //邀请好友人数
    CF_INV15,
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
    CF_RANK500_1,
    CF_RANK500_2,
    CF_RANK500_5,
    CF_RANK500_10,
    CF_RANK200_1,
    CF_RANK200_2,
    CF_RANK200_5,
    CF_RANK200_10,
    CF_INVITED2,    //邀请2位好友成功
    CF_INVITED5,
    CF_INVITED10,
    CF_INVITED20,
    CF_MAX,
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

    bool getAward(UInt8 idx);
    void updateCFriend(UInt8 idx);
    void sendCFriend();
    void updateRecordData();
    void reset(bool = true);
    void recallFriend();
    void giveLift();
    void getLift();
    void useTickets(UInt8);
    void lastCFTicketsAward(UInt16, UInt16);
    void setCFriendSuccess(UInt8);

private:
    Player* m_owner;
    std::vector<UInt8> m_cf; // 0-未达 1-已达 2-已领取
    UInt8 m_maxIdx;
};

} // namespace GObject

#endif // CFRIEND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

