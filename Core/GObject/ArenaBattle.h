
#ifndef ARENABATTLE_H_
#define ARENABATTLE_H_

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"

#include <vector>
#include <map>

#define ARENA_LEVEL 70
#define ARENA_MAN 32

namespace GObject
{

class Player;

class ArenaBattle : public Singleton<ArenaBattle>
{
public:
    ArenaBattle();
    ~ArenaBattle();

    void addLevelMan(bool init = false);
    inline UInt32 getLevelMan() const { return m_levelMan; }

    void push(Player* player);

private:
    UInt32 m_levelMan; // 本区70级及以上人数
    UInt8 m_progress;
};

} // namespace GObject

#endif // ARENABATTLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

