
#include "ShuoShuo.h"
#include "Player.h"
#include "Country.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Common/Itoa.h"
#include "Common/Stream.h"
#include "MsgID.h"

#define MIN_ITEM 5

namespace GObject
{

ShuoShuo::ShuoShuo(Player* player) : m_owner(player), m_maxIdx(0)
{
    m_updateTime = TimeUtil::Now();
}
ShuoShuo::~ShuoShuo()
{
}

void ShuoShuo::loadFromDB(UInt32 update, const char* ss)
{
    if (!ss)
        return;

    StringTokenizer shuo(ss, ",");
    UInt32 count = shuo.count();
    UInt32 c = count;
    if (count < MIN_ITEM)
        count = MIN_ITEM;

    m_ss.resize(count);
    for (UInt8 i = 0; i < c; ++i)
        m_ss[i] = atoi(shuo[i].c_str());

    m_maxIdx = count;
    m_updateTime = update;

    if (TimeUtil::SharpDay(0, TimeUtil::Now()) != TimeUtil::SharpDay(0, m_updateTime))
    {
        for (UInt8 i = 0; i < MIN_ITEM; ++i)
            m_ss[i] = 0;
        updateToDB();
    }
}

void ShuoShuo::updateToDB()
{
    std::string ss;
    UInt32 size = m_ss.size();
    for (UInt8 i = 0; i < size; ++i)
    {
        ss += Itoa(m_ss[i]);
        if (i != size - 1)
            ss += ",";
    }

    m_updateTime = TimeUtil::Now();
    DB().PushUpdateData("REPLACE INTO `shuoshuo` (`playerId`, `updateTime`, `shuoshuo`) VALUES (%"I64_FMT"u, %u, '%s')",
            m_owner->getId(), m_updateTime, ss.c_str());
}

void ShuoShuo::setShuoSafe(UInt8 idx, UInt8 status)
{
    if (!getShuoShuo(idx))
        setShuoShuo(idx, status);
}

void ShuoShuo::setShuoShuo(UInt8 idx, UInt8 status)
{
    if (!World::getShuoShuo())
        return;
    bool w = false;
    if (m_ss.size() < MIN_ITEM)
    {
        m_ss.resize(MIN_ITEM, 0);
        m_maxIdx = MIN_ITEM;
        w = true;
    }

    if (idx >= m_maxIdx)
    {
        m_maxIdx = idx+1;
        w = true;
    }

    if (m_ss.size() < m_maxIdx)
        m_ss.resize(m_maxIdx, 0);

    UInt8 o = m_ss[idx];
    m_ss[idx] = status;

    if (o != status || w)
    {
        updateToDB();
        updateShuoShuo(idx);
    }
}

UInt8 ShuoShuo::getShuoShuo(UInt8 idx)
{
    if (idx >= m_maxIdx)
        return 0;
    return m_ss[idx];
}

void ShuoShuo::getAward(UInt8 idx)
{
    if (!World::getShuoShuo())
        return;
    if (!idx)
        return;
    if (idx > m_ss.size())
        return;
    if (m_ss[idx-1] != 1)
        return;

    if (GameAction()->getShuoShuoAward(m_owner, idx))
    {
        m_ss[idx-1] = 2;
        updateToDB();
        updateShuoShuo(idx-1);
    }
}

void ShuoShuo::updateShuoShuo(UInt8 idx)
{
    Stream st(REP::SSAWARD);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(idx+1);
    st << m_ss[idx];
    st << Stream::eos;
    m_owner->send(st);
}

void ShuoShuo::sendShuoShuo()
{
    Stream st(REP::SSAWARD);
    st << static_cast<UInt8>(1);
    UInt8 size = m_ss.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
        st << m_ss[i];
    st << Stream::eos;
    m_owner->send(st);
}

void ShuoShuo::reset(bool online)
{
    if (m_ss.size() < MIN_ITEM)
        m_ss.resize(MIN_ITEM, 0);
    for (UInt8 i = 0; i < MIN_ITEM; ++i)
    {
        m_ss[i] = 0;
        if (online)
            updateShuoShuo(i);
    }
    updateToDB();
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

