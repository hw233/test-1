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
#include "Common/Itoa.h"
#include "Server/WorldServer.h"
#include "Common/StringTokenizer.h"
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

void LuckyDraw::draw(Player* player, UInt8 id, UInt8 num, bool bind)
{
    if (!player)
        return;
    FastMutex::ScopedLock lock(_lock);

    if (!player->isCopyPassed(id+1))
        return;

    Stream st(REP::LUCKYDRAW);
    st << static_cast<UInt8>(0);

    lua_tinker::table items = GameAction()->luckyDraw(player, id, num, bind);
    Int32 size = items.size();
    if (size)
    {
        std::vector<LDItem> its;
        st << static_cast<UInt8>(size);
        for (int i = 1; i <= size; ++i)
        {
            lua_tinker::table item = items.get<lua_tinker::table>(i);
            if (item.size() == 2)
            {
                UInt16 itemid = item.get<UInt16>(1);
                UInt8 num = item.get<UInt16>(2);
                st << itemid;
                st << num;

                LDItem i;
                i.itemid = itemid;
                i.num = num;
                its.push_back(i);
            }
        }
        pushLog(player->getName(), its);
    }
    else
        st << static_cast<UInt8>(0);

    st << Stream::eos;
    player->send(st);
}

void LuckyDraw::pushLog(const std::string& name, const std::vector<LDItem>& i)
{
    LDLog l;
    l.name = name;
    l.items = i;

    UInt32 size = _logs.size();
    if (size >= 10)
        _logs.pop_back();
    _logs.push_front(l);

    std::string its;
    UInt32 sz = i.size();
    for (UInt32 j = 0; j < sz; ++j)
    {
        its += Itoa(i[j].itemid);
        its += ",";
        its += Itoa(i[j].num);
    }

    if (name.empty() || its.empty())
        return;
    DB6().PushUpdateData("INSERT INTO `luckylog` VALUES (0, '%s', '%s')", name.c_str(), its.c_str());
}

void LuckyDraw::pushLog(const std::string& name, const std::string& its)
{
    LDLog l;
    l.name = name;

    StringTokenizer items(its, ",");
    UInt32 sz = items.count();
    if (!sz)
        return;
    for (UInt32 i = 0; i < sz; ++i)
    {
        LDItem it;
        it.itemid = atoi(items[i].c_str());
        it.num = atoi(items[i+1].c_str());
        l.items.push_back(it);
    }
    _logs.push_back(l);
}

LuckyDraw luckyDraw;

}

