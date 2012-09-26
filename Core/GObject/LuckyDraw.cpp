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
#include "GData/ItemType.h"
#include "Server/SysMsg.h"
#include "GVar.h"

namespace GObject
{

void LuckyDraw::sendInfo(Player* player)
{
    if (!player)
        return;
    FastMutex::ScopedLock lock(_lock);

    Stream st(REP::LUCKYDRAW);

    st << static_cast<UInt8>(1);
    st << static_cast<UInt8>(6);
    for (UInt8 i = 2; i < 8; ++i)
    {
        if (GVAR.GetVar(GVAR_TCPASS) >= i)
            st << static_cast<UInt8>(1);
        else
            st << static_cast<UInt8>(0);
    }

    UInt8 sz = _logs.size();
    st << sz;
    for (std::list<LDLog>::iterator i = _logs.begin(), e = _logs.end(); i != e; ++i)
    {
        Player* pl = globalNamedPlayers[(*i).name]; // fixName?
        if (!pl)
            continue;

        st << (*i).name;
        st << pl->getCountry();
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

bool merge(std::vector<LDItem>& its, UInt16 itemid, UInt8 num)
{
    for (std::vector<LDItem>::iterator it = its.begin(), end = its.end(); it != end; ++it)
    {
        if ((*it).itemid == itemid)
        {
            (*it).num += num;
            return true;
        }
    }
    return false;
}

void LuckyDraw::draw(Player* player, UInt8 id, UInt8 num, bool bind)
{
    if (!player)
        return;
    FastMutex::ScopedLock lock(_lock);

    if (GVAR.GetVar(GVAR_TCPASS) < id)
        return;

    Stream st(REP::LUCKYDRAW);
    st << static_cast<UInt8>(0);

    lua_tinker::table items = GameAction()->luckyDraw(player, id, num, bind);
    Int32 size = items.size();
    if (size)
    {
        std::vector<LDItem> its;
        std::vector<LDItem> logits;
        for (int i = 1; i <= size; ++i)
        {
            lua_tinker::table item = items.get<lua_tinker::table>(i);
            if (item.size() == 2)
            {
                UInt16 itemid = item.get<UInt16>(1);
                UInt8 num = item.get<UInt16>(2);

                LDItem i;
                i.itemid = itemid;
                i.num = num;

                bool merged = merge(its, itemid, num);
                if (!merged)
                    its.push_back(i);

                const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[itemid];
                if (itemType->quality == 5)
                {
                    if (!merge(logits, itemid, num))
                        logits.push_back(i);
                    SYSMSG_BROADCASTV(2204, player->getCountry(), player->getName().c_str(), id, itemid);
                }
            }
        }

#ifdef _FB
        if(WORLD().getJune())
        {
            GameMsgHdr hdr2(0x1C0, WORKER_THREAD_WORLD, player, sizeof(num));
            GLOBAL().PushMsg(hdr2, &num);
        }
#endif

        UInt8 sz = its.size();
        st << static_cast<UInt8>(sz);
        for (UInt8 i = 0; i < sz; ++i)
        {
            st << its[i].itemid;
            st << its[i].num;
        }

        if (logits.size())
            pushLog(player->getName(), logits);
        player->luckyDrawUdpLog(id, static_cast<UInt8> (bind ? 1:2), num);
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
        if (j != sz - 1)
            its += ",";
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
    for (UInt32 i = 0; i < sz; i+=2)
    {
        LDItem it;
        it.itemid = atoi(items[i].c_str());
        it.num = atoi(items[i+1].c_str());
        l.items.push_back(it);
    }
    _logs.push_back(l);
}

void LuckyDraw::notifyPass(UInt8 id)
{
    if (id <= 1 || id == 8)
        return;
    UInt8 maxid = GVAR.GetVar(GVAR_TCPASS);
    if (id > maxid)
    {
        Stream st(REP::LUCKYDRAW);
        st << static_cast<UInt8>(2);
        st << static_cast<UInt8>(id -1);
        st << Stream::eos;
        NETWORK()->Broadcast(st);
        GVAR.SetVar(GVAR_TCPASS, id);
    }
}

void LuckyDraw::setPass(UInt8 id)
{
    UInt8 maxid = GVAR.GetVar(GVAR_TCPASS);
    if (id > maxid)
        GVAR.SetVar(GVAR_TCPASS, id);
}

void LuckyDraw::notifyDisplay(Player* player)
{
    Stream st(REP::LUCKYDRAW);
    st << static_cast<UInt8>(3);
    bool passed = false;
    if (GVAR.GetVar(GVAR_TCPASS))
        passed = true;
#if 0
    for (UInt8 i = 2; i < 8; ++i)
    {
        if (player->isCopyPassed(i))
        {
            passed = true;
            break;
        }
    }
#endif

    if (passed)
        st << static_cast<UInt8>(1);
    else
        st << static_cast<UInt8>(0);

    st << Stream::eos;
    player->send(st);
}

LuckyDraw luckyDraw;

}

