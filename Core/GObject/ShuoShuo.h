
#ifndef SHUOSHUO_H_
#define SHUOSHUO_H_

#include "Config.h"

namespace GObject
{

enum {
    SS_WBOSS = 0,
    SS_ACT100,
    SS_ATHWIN,
    SS_CTRYBATTLE,
    SS_CLANRANK = 4,
    SS_40,
    SS_50,
    SS_60,
    SS_70,
    SS_80,
    SS_90,
    SS_100 = 11,
    SS_COPY1 = 12,
    SS_COPY2,
    SS_COPY3,
    SS_COPY4,
    SS_COPY5,
    SS_COPY6,
    SS_FM1 = 18,
    SS_FM2,
    SS_FM3,
    SS_FM4,
    SS_FM5,
    SS_FM6,
    SS_FM7,
    SS_FM8,
    SS_FM9,
    SS_FM10,
    SS_FM11,
    SS_FM12 = 29,
    SS_OE = 30,
    SS_TRUMP,
    SS_CITTA,
    SS_SHIMEN,
    SS_YAMEN,
    SS_TEAMCP,
    SS_FMTLVLUP,
    SS_MAX
};

class Player;

class ShuoShuo
{
public:
    ShuoShuo(Player* player);
    ~ShuoShuo();

    void loadFromDB(UInt32 update, const char* ss);
    void updateToDB();

    void setShuoShuo(UInt8 idx, UInt8 status);
    UInt8 getShuoShuo(UInt8 idx);

    void getAward(UInt8 idx);
    void updateShuoShuo(UInt8 idx);
    void sendShuoShuo();
    void reset(bool = true);

private:
    Player* m_owner;
    std::vector<UInt8> m_ss; // 0-未达 1-已达 2-已领取
    UInt8 m_maxIdx;
    UInt32 m_updateTime;
};

} // namespace GObject

#endif // SHUOSHUO_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

