
#include "CFriend.h"
#include "Player.h"
#include "Country.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Common/Itoa.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Package.h"

//#define MIN_ITEM 2

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
    m_cf.resize(CF_MAX);
    for (UInt8 i = 0; i < count && i < CF_MAX; ++i)
        m_cf[i] = atoi(cfriend[i].c_str());
    bool toDB = false;
    if (!m_owner->GetVar(VAR_INVITES))
    {
        for (UInt8 i = CF_INV3; i <= CF_INV30; ++i)
            m_cf[i] = 0;
        toDB = true;
    }
    if (!m_owner->GetVar(VAR_INVITEDSUCCESS))
    {
        for (UInt8 i = CF_INVITED2; i <= CF_INVITED20; ++i)
            m_cf[i] = 0;
        toDB = true;
    }
    if (toDB)
        updateToDB();

    m_maxIdx = CF_MAX;
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
    if (idx >= CF_MAX)
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

bool CFriend::getAward(UInt8 idx)
{
    if (!World::getCFriend())
        return false;
    if (!idx)
        return false;
    if (idx > m_cf.size())
        return false;
    if (m_cf[idx] != 1)
        return false;

    if (GameAction()->onGetCFriendAward(m_owner, idx))
    {
        m_cf[idx] = 2;
        updateToDB();
        updateCFriend(idx);
        updateRecordData();

        if(idx >= CF_INV3 && idx <= CF_INV30)
            GameAction()->doStrong(m_owner, SthInvited, 0, 0);
        return true;
    }
    return false;
}

void CFriend::updateCFriend(UInt8 idx)
{
    Stream st(REP::CFRIEND);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(idx);
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
    updateRecordData();
}

void CFriend::updateRecordData()
{
    Stream st(REP::CFRIEND);
    st << static_cast<UInt8>(2);
    st << static_cast<UInt16>(m_owner->GetVar(VAR_INVITES));
    st << static_cast<UInt16>(m_owner->GetVar(VAR_INVITEDSUCCESS));
    st << static_cast<UInt16>(m_owner->GetVar(VAR_CFRIENDTICKETS));
    st << static_cast<UInt16>(m_owner->getCFrendsNum());
    st << Stream::eos;
    m_owner->send(st);
}

void CFriend::setCFriendNum(UInt8 num)
{
    if(num == 0)
        return;
    UInt32 invited = m_owner->GetVar(VAR_INVITES);
    if(invited + num >= 30)
        setCFriendSafe(CF_INV30);
    else if(invited + num >= 15)
        setCFriendSafe(CF_INV15);
    else if(invited + num >= 3)
        setCFriendSafe(CF_INV3);
    m_owner->AddVar(VAR_INVITES, num);
    updateRecordData();
}

void CFriend::reset(bool online)
{
    if (m_cf.size() < CF_MAX)
        m_cf.resize(CF_MAX, 0);
    for (UInt8 i = CF_RECALL; i <= CF_INV30; ++i)
    {
        m_cf[i] = 0;
        if (online)
            updateCFriend(i);
    }
    if (!m_owner->GetVar(VAR_INVITEDSUCCESS))
    {
        for (UInt8 i = CF_INVITED2; i <= CF_INVITED20; ++i)
        {
            m_cf[i] = 0;
            if (online)
                updateCFriend(i);
        }
    }
    updateToDB();
}

void CFriend::recallFriend()
{
    if (m_cf.size() < CF_MAX)
        m_cf.resize(CF_MAX, 0);
    setCFriendSafe(CF_RECALL);
    /*
    if (getCFriend(CF_RECALL) != 2)
    {
        m_cf[CF_RECALL] = 1;
        if (!getAward(CF_RECALL))
            m_cf[CF_RECALL] = 0;
    }
    */
}

void CFriend::giveLift()
{
    if (m_cf.size() < CF_MAX)
        m_cf.resize(CF_MAX, 0);
    setCFriendSafe(CF_GIVELIFT);
    /*
    if (getCFriend(CF_GIVELIFT) != 2)
    {
        m_cf[CF_GIVELIFT] = 1;
        if (!getAward(CF_GIVELIFT))
            m_cf[CF_GIVELIFT] = 0;
    }
    */
}

void CFriend::getLift()
{
    if (m_cf.size() < CF_MAX)
        m_cf.resize(CF_MAX, 0);
    setCFriendSafe(CF_GETLIFE);
    /*
    if (getCFriend(CF_GETLIFE) != 2)
    {
        m_cf[CF_GETLIFE] = 1;
        if (!getAward(CF_GETLIFE))
            m_cf[CF_GETLIFE] = 0;
    }
    */
}

void CFriend::useTickets(UInt8 type)
{
    if(type == 0)
    {
        UInt8 id = GameAction()->onUseTickets(m_owner);
        if(id == 0)
            return;
        Stream st(REP::CFRIEND);
        st << static_cast<UInt8>(3);
        st << id << Stream::eos;
        m_owner->send(st);
        updateRecordData();
    }
    else
        m_owner->checkLastCFTicketsAward();

}

void CFriend::setCFriendSuccess(UInt8 num)
{
    if(num == 0)
        return;
    UInt32 var = m_owner->GetVar(VAR_INVITEDSUCCESS);
    if(var + num >= 20)
        setCFriendSafe(CF_INVITED20);
    else if(var + num >= 10)
        setCFriendSafe(CF_INVITED10);
    else if(var + num >= 5)
        setCFriendSafe(CF_INVITED5);
    else if(var + num >= 2)
        setCFriendSafe(CF_INVITED2);
    m_owner->AddVar(VAR_INVITEDSUCCESS, num);
    updateRecordData();
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

