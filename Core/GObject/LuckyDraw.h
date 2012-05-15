
#ifndef _LUCKYDRAW_H_
#define _LUCKYDRAW_H_

#include "Common/Mutex.h"

namespace GObject
{

class Player;

struct LDItem
{
    UInt16 itemid;
    UInt8 num;
};

struct LDLog
{
    std::string name;
    std::vector<LDItem> items;
};

class LuckyDraw
{
public:
	LuckyDraw() {}
    ~LuckyDraw() {}

    void sendInfo(Player* player);
    void draw(Player* player, UInt8 id, UInt8 num);

private:
    FastMutex _lock;
    std::list<LDLog> _logs;
};

extern LuckyDraw luckyDraw;

}

#endif // _LUCKYDRAW_H_
