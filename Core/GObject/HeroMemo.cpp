
#include "HeroMemo.h"
#include "Player.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "Common/Stream.h"
#include "MsgID.h"

namespace GObject
{

HeroMemo::HeroMemo(Player* player) : m_owner(player), m_heroSoul(0)
{}

HeroMemo::~HeroMemo()
{}

void HeroMemo::setMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item, UInt8 value)
{
    if (!value)
        return;
    if (chapter > MC_MAX || diff > MD_MAX)
        return;

    if (m_memos[chapter][diff][group][item])
        return;

    m_memos[chapter][diff][group][item] = value;

    Stream st(REP::HEROMEMO);
    st << chapter << diff << group << item << Stream::eos;
    m_owner->send(st);

    ++m_heroSoul;

    updateToDB();
}

UInt8 HeroMemo::getMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item)
{
    if (chapter > MC_MAX || diff > MD_MAX ||
            group > m_memos[chapter][diff].size() ||
            item > m_memos[chapter][diff][group].size())
        return 0;
    return m_memos[chapter][diff][group][item];
}

void HeroMemo::sendHeroMemoInfo()
{
    Stream st(REP::HEROMEMO);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(MC_MAX);
    st << static_cast<UInt8>(MD_MAX);
    for (UInt8 i = 0; i < MC_MAX; ++i)
    {
        for (UInt8 j = 0; j < MD_MAX; ++j)
        {
            UInt8 groups = m_memos[i][j].size();
            st << static_cast<UInt8>(groups);
            for (UInt8 k = 0; k < groups; ++k)
            {
                for (UInt8 l = 0; l < 3; ++l)
                {
                    st << static_cast<UInt8>(m_memos[i][j][k][l]);
                }
            }
        }
    }
    UInt8 awards = m_awards.size();
    st << static_cast<UInt8>(awards);
    for (UInt8 i = 0; i < awards; ++i)
    {
        st << m_awards[i];
    }
    st << Stream::eos;
    m_owner->send(st);
}

void HeroMemo::getAward(UInt8 idx)
{
    if (idx > m_awards.size())
        return;
    if (m_awards[idx-1] != 1)
        return;

    // TODO:


    m_awards[idx-1] = 2;
    updateToDB();
}

void HeroMemo::loadFromDB(const char* awards, const char* memos)
{
    if (!awards || !memos)
        return;

    StringTokenizer award(awards, ",");
    UInt32 count = award.count();
    if (count)
    {
        m_awards.resize(count);
        for (UInt32 i = 0; i < count; ++i)
            m_awards[i] = atoi(award[i].c_str());
    }

    StringTokenizer chapter(memos, "|");
    UInt8 chapters = std::min<UInt8>(chapter.count(), MC_MAX);
    for (UInt8 i = 0; i < chapters; ++i)
    {
        StringTokenizer diff(chapter[i], ",");
        UInt8 diffs = std::min<UInt8>(diff.count(), MD_MAX);
        for (UInt8 j = 0; j < diffs; ++j)
        {
            StringTokenizer group(diff[j], ";");
            UInt8 groups = group.count();
            for (UInt8 k = 0; k < groups; ++k)
            {
                m_memos[i][j].resize(groups);
                StringTokenizer item(group[k], "%");
                for (UInt8 l = 0; l < 3; ++l)
                {
                    UInt8 val = atoi(item[l].c_str());
                    m_memos[i][j][k][l] = val;
                    if (val)
                        ++m_heroSoul;
                }
            }
        }
    }
}

void HeroMemo::updateToDB()
{
    std::string awards;
    std::string memos;

    UInt32 size = m_awards.size();
    for (UInt32 i = 0; i < size; ++i) 
    {   
        awards += Itoa(m_awards[i]);
        if(i != size - 1)
            awards += ",";
    }

    for (UInt8 i = 0; i < MC_MAX; ++i)
    {
        for (UInt8 j = 0; j < MD_MAX; ++j)
        {
            UInt32 size = m_memos[i][j].size();
            for (UInt8 k = 0; k < size; ++k)
            {
                for (UInt8 l = 0; l < 3; ++l)
                {
                    memos += Itoa(m_memos[i][j][k][l]);
                    if (l != 2)
                        memos += "%";
                }
                if (k != size - 1)
                    memos += ";";
            }
            if (j != MD_MAX -1)
                memos += ",";
        }
        if (i != MC_MAX - 1)
            memos += "|";
    }

    DB().PushUpdateData("REPLACE INTO `heromemo` (`playerId`, `awards`, `memos`) VALUES (%"I64_FMT"u, '%s', '%s')"  ,
            this->m_owner->getId(), awards.c_str(), memos.c_str());
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

