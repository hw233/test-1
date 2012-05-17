
#include "CFriend.h"
#include "Player.h"
#include "Country.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Common/Itoa.h"
#include "Common/Stream.h"
#include "MsgID.h"

#define MIN_ITEM 2

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
    if (count < MIN_ITEM)
    {
        m_cf.resize(MIN_ITEM, 0);
        count = MIN_ITEM;
        updateToDB();
    }

    if (!m_owner->GetVar(VAR_INVITES) && m_cf[1])
    {
        m_cf[1] = 0;
        updateToDB();
    }

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
    if (m_cf.size() < MIN_ITEM)
    {
        m_cf.resize(MIN_ITEM, 0);
        m_maxIdx = MIN_ITEM;
        w = true;
    }

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

        if (!m_owner->GetVar(VAR_INVITED))
        {
            GameAction()->doAty(m_owner, AtyInvited, 0, 0);
            m_owner->SetVar(VAR_INVITED, 1);
        }
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
    UInt32 invited = m_owner->GetVar(VAR_INVITES);
    if (invited >= 2)
        return;
    if (invited == 1)
        setCFriendSafe(CF_INV1);
    m_owner->AddVar(VAR_INVITES, 1);
}

void CFriend::reset(bool online)
{
    bool w = false;
    if (m_cf.size() < MIN_ITEM)
    {
        m_cf.resize(MIN_ITEM, 0); 
        w = true;
    }
    if (m_cf[1])
    {
        m_cf[1] = 0;
        w = true;
    }
    if (online && w)
        updateCFriend(1);
    if (w)
        updateToDB();
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

