
#include "CFriend.h"
#include "Player.h"
#include "Country.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Common/Itoa.h"
#include "Common/Stream.h"
#include "MsgID.h"

namespace GObject
{

CFriend::CFriend(Player* player) : m_owner(player), m_maxIdx(0)
{
}
CFriend::~CFriend()
{
}

void CFriend::loadFromDB(const char* cf)
{
    if (!cf)
        return;

    StringTokenizer cfriend(cf, ",");
    UInt32 count = cfriend.count();
    m_cf.resize(count);
    for (UInt8 i = 0; i < count; ++i)
        m_cf[i] = atoi(cfriend[i].c_str());
    m_maxIdx = count;
}

void CFriend::updateToDB()
{
    std::string cf;
    UInt32 size = m_cf.size();
    for (UInt8 i = 0; i < size; ++i)
    {
        cf += Itoa(m_cf[i]);
        if (i != size - 1)
            cf += ",";
    }

    if (!m_owner->getInvitedBy())
    {
        DB3().PushUpdateData("REPLACE INTO `cfriend_awards` (`playerId`, `invitedId`, `awards`) VALUES (%"I64_FMT"u, 0, '%s')", m_owner->getId(), cf.c_str());
    }
    else
    {
        DB3().PushUpdateData("UPDATE `cfriend_awards` SET `awards` = '%s' WHERE `playerId` = %"I64_FMT"u",
                cf.c_str(), m_owner->getId());
    }
}

void CFriend::setCFriendSafe(UInt8 idx)
{
    if (!getCFriend(idx))
        setCFriend(idx, 1);
}

void CFriend::setCFriend(UInt8 idx, UInt8 status)
{
    if (!World::getCFriend())
        return;
    bool w = false;
    if (idx >= m_maxIdx)
    {
        m_maxIdx = idx+1;
        w = true;
    }

    if (m_cf.size() < m_maxIdx)
        m_cf.resize(m_maxIdx, 0);

    UInt8 o = m_cf[idx];
    m_cf[idx] = status;

    if (o != status || w)
    {
        updateToDB();
        updateCFriend(idx);
    }
}

UInt8 CFriend::getCFriend(UInt8 idx)
{
    if (idx >= m_maxIdx)
        return 0;
    return m_cf[idx];
}

void CFriend::getAward(UInt8 idx)
{
    if (!World::getCFriend())
        return;
    if (!idx)
        return;
    if (idx > m_cf.size())
        return;
    if (m_cf[idx-1] != 1)
        return;

    if (GameAction()->onGetCFriendAward(m_owner, idx))
    {
        m_cf[idx-1] = 2;
        updateToDB();
        updateCFriend(idx-1);
    }
}

void CFriend::updateCFriend(UInt8 idx)
{
    Stream st(REP::CFRIEND);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(idx+1);
    st << m_cf[idx];
    st << Stream::eos;
    m_owner->send(st);
}

void CFriend::sendCFriend()
{
    Stream st(REP::CFRIEND);
    st << static_cast<UInt8>(1);
    UInt8 size = m_cf.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
        st << m_cf[i];
    st << Stream::eos;
    m_owner->send(st);
}

void CFriend::setCFriendNum(UInt8 num)
{
    if (!num)
        return;

    static UInt8 nums[] = {1,3,5,10,15,20};
    UInt8 sz = sizeof(nums)/sizeof(UInt8);
    UInt32 invited = m_owner->GetVar(VAR_INVITES);
    if (invited > nums[sz-1] + 5)
        return;
    invited += 1;
    for (UInt8 i = 0; i < sz; ++i)
    {
        if (invited < nums[i])
            break;
        setCFriendSafe(CF_INV1+i);
    }
    m_owner->AddVar(VAR_INVITES, 1);
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

