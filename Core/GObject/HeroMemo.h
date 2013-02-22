
#ifndef HEROMEMO_H_
#define HEROMEMO_H_

#include "Config.h"

namespace GObject
{

class Player;

enum MemoChapter
{
    MC_FIGHTER = 0,
    MC_CITTA,
    MC_FORGE,
    MC_SKILL,
    MC_SLAYER,
    MC_ATHLETICS,
    MC_CONTACTS,
    MC_MAX
};

enum MemoDiff
{
    MD_STARTED = 0,
    MD_ADVANCED,
    MD_MASTER,
    MD_LEGEND,
    MD_MAX
};

class MemoItem
{
public:
    MemoItem()
    {
        memset(m_item, 0x00, sizeof(m_item));
    }

    inline UInt32 size() const { return sizeof(m_item); }

    const UInt8& operator[](UInt32 idx) const { return m_item[idx];}
    UInt8& operator[](UInt32 idx) { return m_item[idx];}

private:
    UInt8 m_item[3];
};

class Memo
{
public:
    Memo() {}
    ~Memo() {}

    inline UInt32 size() const { return m_items.size(); }
    inline void resize(UInt32 size) { m_items.resize(size); }

    const MemoItem& operator[](UInt32 idx) const { return m_items[idx]; }
    MemoItem& operator[](UInt32 idx)
    {
        if (idx >= m_items.size())
            m_items.resize(idx+1);
        return m_items[idx];
    }

private:
    std::vector<MemoItem> m_items;
};

class HeroMemo
{
public:
    HeroMemo(Player* player);
    ~HeroMemo();

    void setMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item, UInt8 value);
    UInt8 getMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item);

    inline UInt16 getHeroSoul() const { return m_heroSoul; }

    void sendHeroMemoInfo();
    void getAward(UInt8 idx);

    void flushAward();

    void updateToDB();
    void loadFromDB(const char* awards, const char* memos);

    static bool testCanGetAward(Player* player, UInt8 idx);
    static void addMaxSoul(UInt16 soul);

private:
    static std::vector<UInt16> m_maxSoul;

private:
    Player* m_owner;
    UInt16 m_heroSoul;
    Memo m_memos[MC_MAX][MD_MAX];
    std::vector<UInt8> m_awards;
    UInt8 m_maxAwardIdx;
};

} // namespace GObject

#endif // HEROMEMO_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

