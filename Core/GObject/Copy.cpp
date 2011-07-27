
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

void PlayerCopy::enter(Player* pl, UInt8 id)
{
    if (!pl)
        return;

    CopyData& tcd = m_copys[pl->getId()][id];
    if (!tcd.floor)
    {
        DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`) VALUES(%"I64_FMT"u, %u, %u, %u)", pl->getId(), id, tcd.floor, tcd.spot);
    }
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
        DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`) VALUES(%"I64_FMT"u, %u, %u, %u)", pl->getId(), id, tcd.floor, tcd.spot);
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
    DB().PushUpdateData("UPDATE `player_copy` SET `id`=%u,`floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u",
            pl->getId(), id, tcd.floor, tcd.spot);
    // TODO: notify client
}

void PlayerCopy::addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot)
{
    CopyData& cd = m_copys[playerId][id];
    cd.floor = floor;
    cd.spot = spot;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

