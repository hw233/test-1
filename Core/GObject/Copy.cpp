
#include "Copy.h"
#include "Player.h"
#include "GData/CopyTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"

namespace GObject
{

void PlayerCopy::sendAllInfo(Player* pl)
{
}

void PlayerCopy::sendInfo(Player* pl, UInt8 id)
{
}

void PlayerCopy::enter(Player* pl, UInt8 id, UInt8 type)
{
    if (!pl)
        return;

    CopyData& tcd = m_copys[pl->getId()][id];
    if (!tcd.floor)
    {
        DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`, `freeCount`, `goldCount`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u)", pl->getId(), id, tcd.floor, tcd.spot, tcd.freeCount, tcd.goldCount);
    }

    if (type == 0) {
        if (tcd.freeCount > 1) {
            // TODO: msg
            return;
        }
        ++tcd.freeCount;
    } else if (type == 1) {
        if (tcd.goldCount > 2) {
            // TODO: msg
            return;
        }
        ++tcd.goldCount;
    }

    DB().PushUpdateData("UPDATE `player_copy` SET `freeCount` = %u, `goldCount` = %u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.freeCount, tcd.goldCount, pl->getId(), id);
}

void PlayerCopy::next(Player* pl, UInt8 id)
{
    if (!pl)
        return;

    CopyData& tcd = m_copys[pl->getId()][id];
    if (!tcd.floor)
    {
        tcd.floor = 1;
        tcd.spot = 1;
        tcd.freeCount = 1;
        DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`, `freeCount`, `goldCount`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u)", pl->getId(), id, tcd.floor, tcd.spot, tcd.freeCount, tcd.goldCount);
    } else {
        if (tcd.freeCount > 2 || tcd.goldCount > 3)
            return;
    }

    UInt32 fgtid = GData::copyManager[id<<8|tcd.floor][tcd.spot];
    if (fgtid) {
        GData::NpcGroups::iterator it = GData::npcGroups.find(fgtid);
        if(it == GData::npcGroups.end())
            return;

        GData::NpcGroup* ng = it->second;
        if (!ng) {
            // TODO: notify client
            return;
        }

        Battle::BattleSimulator bsim(pl->getLocation(), pl, ng->getName(), ng->getLevel(), false, 0);
        pl->PutFighters(bsim, 0);
        ng->putFighters(bsim);

        bsim.start();
        Stream& st = bsim.getPacket();
        if(st.size() <= 8)
            return;

        // TODO: loot
        pl->send(st);
    }
}

void PlayerCopy::reset(Player* pl, UInt8 id)
{
    if (!pl)
        return;

    CopyData& tcd = m_copys[pl->getId()][id];
    tcd.floor = 0;
    tcd.spot = 0;
    ++tcd.freeCount;
    DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u",
            pl->getId(), tcd.floor, tcd.spot, id);
    // TODO: notify client
}

void PlayerCopy::addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot, UInt8 free, UInt8 gold)
{
    CopyData& cd = m_copys[playerId][id];
    cd.floor = floor;
    cd.spot = spot;
    cd.freeCount = free;
    cd.goldCount = gold;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

