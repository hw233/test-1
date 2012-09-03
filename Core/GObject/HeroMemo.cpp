
#include "Config.h"
#include "Country.h"
#include "HeroMemo.h"
#include "Player.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "Script/GameActionLua.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "ShuoShuo.h"

namespace GObject
{

std::vector<UInt16> HeroMemo::m_maxSoul;

HeroMemo::HeroMemo(Player* player) : m_owner(player), m_heroSoul(0), m_maxAwardIdx(0)
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
    st << static_cast<UInt8>(1) << chapter << diff << group << item << Stream::eos;
    m_owner->send(st);

    ++m_heroSoul;

    if (m_heroSoul >= 30)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_30);
    if (m_heroSoul >= 40)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_40);
    if (m_heroSoul >= 50)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_50);
    if (m_heroSoul >= 60)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_60);
    if (m_heroSoul >= 70)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_70);
    if (m_heroSoul >= 80)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_80);

    if (testCanGetAward(m_owner, m_maxAwardIdx+1))
    {
        if (!m_awards.size())
            m_awards.resize(m_maxSoul.size());
        m_awards[m_maxAwardIdx] = 1;
        ++m_maxAwardIdx;
    }

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
    if (!idx)
        return;
    if (idx > m_awards.size())
        return;
    if (m_awards[idx-1] != 1)
        return;

    if (!m_owner)
        return;
    if (GameAction()->getHeroMemoAward(m_owner, idx, m_heroSoul))
    {
        m_awards[idx-1] = 2;

        Stream st(REP::HEROMEMO);
        st << static_cast<UInt8>(2) << idx << Stream::eos;
        m_owner->send(st);

        updateToDB();
    }
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
    else
    {
        m_awards.resize(m_maxSoul.size());
        count = m_maxSoul.size();
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


    for (UInt32 i = 0; i < count; ++i)
    {
        bool ret = testCanGetAward(m_owner, i+1);
        if (ret && !m_awards[i])
        {
            m_awards[i] = 1;
            m_maxAwardIdx = i+1;
        }
        else if (m_awards[i] == 2)
        {
            m_maxAwardIdx = i+1;
        }
    }

    if (m_heroSoul >= 30)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_30);
    if (m_heroSoul >= 40)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_40);
    if (m_heroSoul >= 50)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_50);
    if (m_heroSoul >= 60)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_60);
    if (m_heroSoul >= 70)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_70);
    if (m_heroSoul >= 80)
        m_owner->GetShuoShuo()->setShuoSafe(SS_HM_80);
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

void HeroMemo::addMaxSoul(UInt16 soul)
{
    m_maxSoul.push_back(soul);
}

bool HeroMemo::testCanGetAward(Player* player, UInt8 idx)
{
    if (!player || !idx)
        return false;

    UInt8 size = m_maxSoul.size();
    if (idx >= size)
        return false;

    for (UInt8 i = 0; i < size; ++i)
    {
        if (i+1 == idx && player->GetHeroMemo()->getHeroSoul() >= m_maxSoul[i])
            return true;
    }
    return false;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

