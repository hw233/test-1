
#ifndef SHUOSHUO_H_
#define SHUOSHUO_H_

#include "Config.h"

namespace GObject
{

enum {
    SS_WBOSS            = 0,
    SS_STH100           = 1,
    SS_ATHWIN           = 2,
    SS_CTRYBATTLE       = 3,
    SS_CLANRANK         = 4,
    SS_40               = 5,
    SS_50               = 6,
    SS_60               = 7,
    SS_70               = 8,
    SS_80               = 9,
    SS_90               = 10,
    SS_100              = 11,
    SS_COPY1            = 12,
    SS_COPY2            = 13,
    SS_COPY3            = 14,
    SS_COPY4            = 15,
    SS_COPY5            = 16,
    SS_COPY6            = 17,
    SS_FM1              = 18,
    SS_FM2              = 19,
    SS_FM3              = 20,
    SS_FM4              = 21,
    SS_FM5              = 22,
    SS_FM6              = 23,
    SS_FM7              = 24,
    SS_FM8              = 25,
    SS_FM9              = 26,
    SS_FM10             = 27,
    SS_FM11             = 28,
    SS_FM12             = 29,
    SS_OE               = 30,
    SS_TRUMP            = 31,
    SS_CITTA            = 32,
    SS_SHIMEN           = 33,
    SS_YAMEN            = 34,
    SS_TEAMCP           = 35,
    SS_FMTLVLUP         = 36,
    SS_HM_30            = 37,
    SS_HM_40            = 38,
    SS_HM_50            = 39,
    SS_HM_60            = 40,
    SS_HM_70            = 41,
    SS_HM_80            = 42,
    SS_PUBTST_PKG       = 43, // 每天领取完公测剑侠红包分享
    SS_SLLP             = 44, // 生日罗盘分享
    SS_MO_HIRE          = 45,
    SS_MO_STRENGTH      = 46,
    SS_MO_BATTLE        = 47,
    SS_MO_COPY          = 48,
    /** 繁体占用60~70 **/
    SS_GEM_FRESH        = 60,
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

    void setShuoSafe(UInt8 idx, UInt8 status = 1);
    void setShuoShuo(UInt8 idx, UInt8 status);
    UInt8 getShuoShuo(UInt8 idx);

    void getAward(UInt8 idx);
    void updateShuoShuo(UInt8 idx);
    void sendShuoShuo();
    void reset(bool = true);
    void resetMin();

    //For GM
    void clearAllForGM()
    {
        for(UInt8 i = 0; i < m_ss.size(); ++ i)
            m_ss[i] = 0;
        updateToDB();
    }
private:
    Player* m_owner;
    std::vector<UInt8> m_ss; // 0-未达 1-已达 2-已领取
    UInt32 m_updateTime;
};

} // namespace GObject

#endif // SHUOSHUO_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

