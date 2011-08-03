
#include "FrontMap.h"
#include "Player.h"
#include "GData/FrontMapTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "Common/Stream.h"

namespace GObject
{

void FrontMap::sendAllInfo(Player* pl)
{
}

void FrontMap::sendInfo(Player* pl, UInt8 id)
{
}

void FrontMap::enter(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    UInt8 ret = 1;
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (PLAYER_DATA(pl, frontFreeCnt) < 2) {
        ++PLAYER_DATA(pl, frontFreeCnt);
        ret = 0;
    } else if (PLAYER_DATA(pl, frontGoldCnt) < 3) {
        ++PLAYER_DATA(pl, frontGoldCnt);
        ret = 0;

        ConsumeInfo ci(EnterFrontMap,0,0);
        pl->useGold(20*PLAYER_DATA(pl, frontGoldCnt));
    }
}

void FrontMap::fight(Player* pl, UInt8 id, UInt8 spot)
{
    if (!pl || !id || !spot)
        return;

    if (PLAYER_DATA(pl, frontFreeCnt) > 2 && PLAYER_DATA(pl, frontGoldCnt) > 3)
        return;

    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (spot > GData::frontMapMaxManager[id]) {
        // TODO: 
        return;
    }

    if (spot > tmp.size()) {
        tmp.resize(spot+1);
        tmp[spot].count = 0;
        tmp[spot].status = 0;
        DB().PushUpdateData("REPLACE INTO `player_frontmap` SET `playerId`=%"I64_FMT"u,`id`=%u,`spot`=%u,`count`=0,`status`=0", pl->getId(), id, spot); 
    }

    UInt8 count = tmp[spot].count;
    if (count >= GData::frontMapManager[id<<8|spot][spot].count) {
        // TODO:
        return;
    }

    bool ret = false;
    UInt32 fgtid = GData::frontMapManager[id<<8|spot][spot].fighterId;
    if (!fgtid) {
        if (pl->attackCopyNpc(fgtid)) {
            ret = true;
        }
    }

    ++tmp[spot].count;
    tmp[spot].status = 1;
    if (ret)
        DB().PushUpdateData("UPDATE `player_frontmap` SET `count`=%u,`status`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u AND `spot`=%u", tmp[spot].count, tmp[spot].status, pl->getId(), id, spot);
    else
        DB().PushUpdateData("UPDATE `player_frontmap` SET `count`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", spot, tmp[spot].count, tmp[spot].status, pl->getId(), id);
    // TODO:
    return;
}

void FrontMap::reset(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (tmp.size() < 1)
        return;
    tmp.clear();
    DB().PushUpdateData("DELETE FROM `player_frontmap` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", pl->getId(), id);
    // TODO:
}

void FrontMap::addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status)
{
    if (!playerId || !id || !spot)
        return;
    std::vector<FrontMapData>& tmp = m_frts[playerId][id];
    if (tmp.size() < spot)
        tmp.resize(spot+1);
    tmp[spot].count = count;
    tmp[spot].status = status?1:0;
}

std::vector<FrontMapData>& FrontMap::getFrontMapData(Player* pl, UInt8 id, bool update)
{
    static std::vector<FrontMapData> nulldata;
    if (!pl || !id)
        return nulldata;
    return getFrontMapData(pl, pl->getId(), id, update);

}

std::vector<FrontMapData>& FrontMap::getFrontMapData(Player* pl, UInt64 playerId, UInt8 id, bool update)
{
    static std::vector<FrontMapData> nulldata;
#if 0
    std::vector<FrontMapData>& cd = m_frts[playerId][id];
    if (!cd.size()) {
        PLAYER_DATA(pl, frontUpdate) = TimeUtil::Now();
        if (update) {
            DB().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = 0, `frontGoldCnt` = 0, `frontUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
            DB().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%"I64_FMT"u, %u, %u, %u, %u)", playerId, id, cd.spot, cd.count, cd.status);
        }   
    } else {
        if (pl && TimeUtil::Day(TimeUtil::Now()) != TimeUtil::Day(PLAYER_DATA(pl, frontUpdate))) {
            if (pl) {
                PLAYER_DATA(pl, frontUpdate) = TimeUtil::Now();
                PLAYER_DATA(pl, frontFreeCnt) = 0;
                PLAYER_DATA(pl, frontGoldCnt) = 0;
            }   

            DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
        }   
    }   
#endif
    return nulldata; 
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

