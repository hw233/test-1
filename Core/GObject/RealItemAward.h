#ifndef _REAL_ITEM_AWARD_H_
#define _REAL_ITEM_AWARD_H_

#include "Common/Mutex.h"
#include <vector>
#include <map>

class Stream;

namespace GObject
{

class Player;

struct RealItemAward
{
    UInt32 cd;
    std::string card_no;
    std::string card_psw;
};

class RealItemAwardMgr
{
public:
    void load(UInt32 id, UInt32 cd, std::string& card_no, std::string& card_psw);
    bool hasAward(UInt32 id);
    void getAward(Player* pl, UInt32 id);
    void getInfo(Stream& st);
    bool addAward(UInt32 id, UInt32 cd, std::string& card_no, std::string& card_psw);
    bool delAward(UInt32 id);
private:
    FastMutex m_mutex;
    std::map<UInt32, RealItemAward> m_awards;
};

extern RealItemAwardMgr realItemAwardMgr;

}

#endif
