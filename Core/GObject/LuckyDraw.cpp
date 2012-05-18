#include "Config.h"
#include "LuckyDraw.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "Item.h"
#include "Script/GameActionLua.h"
#include "Common/URandom.h"
#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "MsgID.h"

namespace GObject
{

void LuckyDraw::sendInfo(Player* player)
{
    if (!player)
        return;
    FastMutex::ScopedLock lock(_lock);
    
    Stream st(REP::LUCKYDRAW);

    st << static_cast<UInt8>(1);
    st << static_cast<UInt8>(5);
    for (UInt8 i = 2; i < 7; ++i)
    {
        if (player->isCopyPassed(i))
            st << static_cast<UInt8>(1);
        else
            st << static_cast<UInt8>(0);
    }

    UInt8 sz = _logs.size();
    st << sz;
    for (std::list<LDLog>::iterator i = _logs.begin(), e = _logs.end(); i != e; ++i)
    {
        st << (*i).name;

        UInt16 s = (*i).items.size();
        st << s;
        for (UInt8 j = 0; j < s; ++j)
        {
            st << (*i).items[j].itemid;
            st << (*i).items[j].num;
        }
    }
    st << Stream::eos;
    player->send(st);
}

void LuckyDraw::draw(Player* player, UInt8 id, UInt8 num)
{
    if (!player)
        return;
    FastMutex::ScopedLock lock(_lock);

    Stream st(REP::LUCKYDRAW);
    st << static_cast<UInt8>(0);

    lua_tinker::table items = GameAction()->luckyDraw(player, id, num);
    if (items.size())
    {
    }
    else
        st << static_cast<UInt8>(0);

    st << Stream::eos;
    player->send(st);
}

LuckyDraw luckyDraw;

}
