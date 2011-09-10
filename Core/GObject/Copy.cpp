
#include "Copy.h"
#include "Player.h"
#include "GData/CopyTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "GData/Money.h"
#include "Server/SysMsg.h"
#include "Mail.h"

namespace GObject
{


void PlayerCopy::sendAllInfo(Player* pl)
{
}

UInt8 PlayerCopy::getGoldCount(UInt8 vipl)
{
    return 3;
    if (vipl == 2)
        return 1;
    if (vipl == 3)
        return 2;
    if (vipl >= 4)
        return 3;
    return 3; // TODO:
}

void PlayerCopy::sendInfo(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

	FastMutex::ScopedLock lk(_mutex);
    CopyData& cd = getCopyData(pl, id, true);
    Stream st(REP::COPY_INFO);
    st << static_cast<UInt8>(0);
    st << id;
    st << cd.floor;
    st << cd.spot;
    UInt8 count = getGoldCount(pl->getVipLevel())-PLAYER_DATA(pl, copyGoldCnt);
    count <<= 4;
    
    count |= FREECNT-PLAYER_DATA(pl, copyFreeCnt);
    st << count;
    st << Stream::eos;
    pl->send(st);
}

UInt8 PlayerCopy::getCopySize(Player* pl)
{
    if(!pl)
        return 0;

    return static_cast<UInt8>(m_copys[pl->getId()].size());
}

void PlayerCopy::buildInfo(Player* pl, Stream& st)
{
    if(!pl)
        return;

    std::map<UInt8, CopyData>& pl_copy = m_copys[pl->getId()];
    for(std::map<UInt8, CopyData>::iterator it = pl_copy.begin(); it != pl_copy.end(); ++ it )
    {
        st << static_cast<UInt8>(it->first);
    }
}

UInt8 PlayerCopy::checkCopy(Player* pl, UInt8 id)
{
    static UInt8 lvls[] = {30, 45, 60, 70, 80, 90};
    if (pl->GetLev() < lvls[id-1]) {
        SYSMSG_SENDV(2109, pl, pl->GetLev(), lvls[id-1]);
        return 1;
    }

    if (PLAYER_DATA(pl, copyFreeCnt) < FREECNT) {
        ++PLAYER_DATA(pl, copyFreeCnt);
        return 0;
    } else if (PLAYER_DATA(pl, copyGoldCnt) < getGoldCount(pl->getVipLevel())) {
        if (pl->getGold() < GData::moneyNeed[GData::COPY_ENTER1+PLAYER_DATA(pl, copyGoldCnt)].gold) {
            pl->sendMsgCode(0, 1101);
            return 1;
        }
        ++PLAYER_DATA(pl, copyGoldCnt);

        ConsumeInfo ci(EnterCopy,0,0);
        pl->useGold(20*PLAYER_DATA(pl, copyGoldCnt)); // 第1次20,第2次40
        return 0;
    } else {
        SYSMSG_SENDV(2000, pl);
    }
    return 1;
}


void PlayerCopy::enter(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

	FastMutex::ScopedLock lk(_mutex);

    if (!id)
        id = 1;

    if (pl->hasFlag(Player::AutoCopy))
    {
        pl->sendMsgCode(0, 1414);
        return;
    }

    UInt8 ret = checkCopy(pl, id);

    if (!ret) {
        CopyData& tcd = getCopyData(pl, id, true);
        if (!tcd.floor) {
            tcd.floor = 1;
            tcd.spot = 1;
        }

        DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = %u, `copyGoldCnt` = %u WHERE `id` = %"I64_FMT"u", PLAYER_DATA(pl, copyFreeCnt), PLAYER_DATA(pl, copyGoldCnt), pl->getId());
        DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.floor, tcd.spot, pl->getId(), id);
    }

    Stream st(REP::COPY_INFO);
    st << static_cast<UInt8>(1) << id << ret << Stream::eos;
    pl->send(st);
}

UInt8 PlayerCopy::getCopyFloors(UInt8 id)
{
    UInt8 floors = 0;
    for (UInt8 f = 1; f <= GData::copyMaxManager[id]; ++f)
    {
        floors += GData::copyManager[id<<8|f].size()-1;
    }
    return floors;
}

UInt8 PlayerCopy::fight(Player* pl, UInt8 id, bool ato)
{
    if (!pl || !id)
        return 0;

	FastMutex::ScopedLock lk(_mutex);
    if (pl->hasFlag(Player::AutoCopy) && !ato)
    {
        pl->sendMsgCode(0, 1414);
        return 0;
    }

    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor) {
        return 0;
    }

    if (PLAYER_DATA(pl, copyFreeCnt) > FREECNT && PLAYER_DATA(pl, copyGoldCnt) > getGoldCount(pl->getVipLevel()))
        return 0;

    if (!GData::copyManager[id<<8|tcd.floor].size())
        return 0;

    UInt32 fgtid = GData::copyManager[id<<8|tcd.floor][tcd.spot];
    if (fgtid) {
        std::vector<UInt32> loot;
        if (pl->attackCopyNpc(fgtid, 1, id, ato, &loot)) {
            bool nextfloor = false;
            if (tcd.spot >= (GData::copyManager[id<<8|tcd.floor].size() - 1))
                nextfloor = true;

            if (nextfloor) {
                ++tcd.floor;
                tcd.spot = 1;
            } else
                ++tcd.spot;

            if (tcd.floor > GData::copyMaxManager[id]) {
                if (ato)
                {
                    Stream st(REP::AUTO_COPY);
                    UInt8 size = loot.size();
                    if (size)
                    {
                        UInt8 rsize = loot[0];
                        if (rsize != (size-1)/2)
                        {
                            st << static_cast<UInt8>(5);
                            PopTimerEvent(pl, EVENT_AUTOCOPY, pl->getId());
                            pl->setBuffData(PLAYER_BUFF_AUTOCOPY, 0, true);
                            pl->delFlag(Player::AutoCopy);
                            DB().PushUpdateData("DELETE FROM `autocopy` WHERE playerId = %"I64_FMT"u", pl->getId());
#if 0
                            SYSMSG(title, 555);
                            SYSMSGV(content, 556);
                            MailPackage::MailItem mitem[size/2-rsize];
                            for (UInt8 i = rsize*2+1, j = 0; i < size; i += 2, ++j)
                            {
                                mitem[j].id = loot[i];
                                mitem[j].count = loot[i+1];
                            }
                            MailItemsInfo itemsInfo(mitem, MailItemType::AutoCopy, size/2-rsize); 
                            pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0, true, &itemsInfo);
#endif
                        }
                        else
                        {
                            st << static_cast<UInt8>(4);
                        }
                        st << id << tcd.floor << tcd.spot;

                        st << static_cast<UInt8>((size-1)/2);
                        for (UInt8 i = 1, c = 0; i < size && c < rsize; i += 2, ++c)
                        {
                            st << loot[i] << static_cast<UInt8>(loot[i+1]);
                        }
                    }
                    else
                        st << static_cast<UInt8>(4) << id << tcd.floor << tcd.spot << static_cast<UInt8>(0);

                    st << Stream::eos;
                    pl->send(st);
                }
                else
                {
                    Stream st(REP::COPY_INFO);
                    st << static_cast<UInt8>(5);
                    st << id;
                    st << Stream::eos;
                    pl->send(st);
                }

                tcd.floor = 0;
                tcd.spot = 0;
                DB().PushUpdateData("DELETE FROM `player_copy` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.spot, pl->getId(), id);
                return 2;
            } else {
                if (ato)
                {
                    Stream st(REP::AUTO_COPY);
                    UInt8 size = loot.size();
                    if (size)
                    {
                        UInt8 rsize = loot[0];
                        if (rsize != (size-1)/2)
                        {
                            st << static_cast<UInt8>(5);
                        }
                        else
                        {
                            st << static_cast<UInt8>(3);
                        }
                        st << id << tcd.floor << tcd.spot;

                        st << static_cast<UInt8>((size-1)/2);
                        for (UInt8 i = 1; i < rsize; i += 2)
                        {
                            st << loot[i] << static_cast<UInt8>(loot[i+1]);
                        }

                        for (UInt8 i = rsize*2+1; i < size; i += 2)
                        {
                            // TODO: MAIL
                        }
                    }
                    else
                        st << static_cast<UInt8>(3) << id << tcd.floor << tcd.spot << static_cast<UInt8>(0);

                    st << Stream::eos;
                    pl->send(st);
                }
                else
                {
                    Stream st(REP::COPY_INFO);
                    st << static_cast<UInt8>(6);
                    st << id << tcd.floor << tcd.spot;
                    st << Stream::eos;
                    pl->send(st);
                }
            }

            DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.floor, tcd.spot, pl->getId(), id);
            return 1;
        }
    }
    return 0;
}

void PlayerCopy::reset(Player* pl, UInt8 id)
{
    if (!pl)
        return;

	FastMutex::ScopedLock lk(_mutex);
    Stream st(REP::COPY_INFO);
    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor)
    {
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tcd.floor = 0;
    tcd.spot = 0;
    DB().PushUpdateData("DELETE FROM `player_copy` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.spot, pl->getId(), id);

    st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);
}

void PlayerCopy::addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot)
{
    CopyData& cd = m_copys[playerId][id];
    cd.floor = floor;
    cd.spot = spot;
}

CopyData& PlayerCopy::getCopyData(Player* pl, UInt8 id, bool update)
{
    static CopyData nulldata;
    if (!pl || !id)
        return nulldata;
    return getCopyData(pl, pl->getId(), id, update);
}

void PlayerCopy::getCount(Player* pl, UInt8* free, UInt8* gold, bool lock)
{
    if (!pl)
        return;

    UInt64 playerId = pl->getId();
    if (lock)
    {
        FastMutex::ScopedLock lk(_mutex);
        if (pl && TimeUtil::Day(TimeUtil::Now()) != TimeUtil::Day(PLAYER_DATA(pl, copyUpdate))) {
            PLAYER_DATA(pl, copyUpdate) = TimeUtil::Now();
            PLAYER_DATA(pl, copyFreeCnt) = 0;
            PLAYER_DATA(pl, copyGoldCnt) = 0;
            if (free)
                *free = 0;
            if (gold)
                *gold = 0;
            DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
        } else {
            if (free)
                *free = PLAYER_DATA(pl, copyFreeCnt);
            if (gold)
                *gold = PLAYER_DATA(pl, copyGoldCnt);
        }
    } else {
        if (TimeUtil::Day(TimeUtil::Now()) != TimeUtil::Day(PLAYER_DATA(pl, copyUpdate))) {
            PLAYER_DATA(pl, copyUpdate) = TimeUtil::Now();
            PLAYER_DATA(pl, copyFreeCnt) = 0;
            PLAYER_DATA(pl, copyGoldCnt) = 0;
            if (free)
                *free = 0;
            if (gold)
                *gold = 0;
            DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
        } else {
            if (free)
                *free = PLAYER_DATA(pl, copyFreeCnt);
            if (gold)
                *gold = PLAYER_DATA(pl, copyGoldCnt);
        }
    }
}

CopyData& PlayerCopy::getCopyData(Player* pl, UInt64 playerId, UInt8 id, bool update)
{
    static CopyData nulldata;
    CopyData& cd = m_copys[playerId][id];
    if (!cd.floor) {
        if (update)
            DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`) VALUES(%"I64_FMT"u, %u, %u, %u)", playerId, id, cd.floor, cd.spot);
    }

    getCount(pl, NULL, NULL);
    return cd;
}

void PlayerCopy::failed(Player* pl, UInt8 id)
{
    if (!pl)
        return;

	FastMutex::ScopedLock lk(_mutex);
    CopyData& cd = getCopyData(pl, id); 
    if (!cd.floor)
        return;

    Stream st(REP::AUTO_COPY);
    st << static_cast<UInt8>(2) << id << cd.floor << cd.spot << Stream::eos;
    pl->send(st);
}

void PlayerCopy::autoBattle(Player* pl, UInt8 id, UInt8 type, bool init)
{
    if (!pl || !id)
        return;

    switch (type)
    {
        case 0:
            {
                if (!init)
                {
                    if (pl->hasFlag(Player::AutoCopy))
                    {
                        pl->sendMsgCode(0, 1414);
                        return;
                    }

                    if (GData::moneyNeed[GData::COPY_AUTO1+id-1].tael > pl->getTael())
                    {
                        pl->sendMsgCode(0, 1100);
                        return;
                    }
                }

                UInt8 floors = getCopyFloors(id);
                if (floors >= 1)
                    --floors;

                EventAutoCopy* event = new (std::nothrow) EventAutoCopy(pl, 60, floors, id);
                if (event)
                {
                    PushTimerEvent(event);
                    pl->addFlag(Player::AutoCopy);
                    pl->setBuffData(PLAYER_BUFF_AUTOCOPY, 1, true);
                    DB().PushUpdateData("REPLACE INTO `autocopy` (`playerId`, `id`) VALUES (%"I64_FMT"u, %u)", pl->getId(), id);

                    CopyData& tcd = getCopyData(pl, id);
                    if (!tcd.floor) {
                        tcd.floor = 1;
                        tcd.spot = 1;
                    }
                    Stream st(REP::AUTO_COPY);
                    st << static_cast<UInt8>(0) << id << tcd.floor << tcd.spot << Stream::eos; 
                    pl->send(st);
                }
            }
            break;

        case 1:
            {
                if (!pl->hasFlag(Player::AutoCopy))
                {
                    pl->sendMsgCode(0, 1415);
                    return;
                }
                PopTimerEvent(pl, EVENT_AUTOCOPY, pl->getId());
                pl->setBuffData(PLAYER_BUFF_AUTOCOPY, 0, true);
                pl->delFlag(Player::AutoCopy);
                DB().PushUpdateData("DELETE FROM `autocopy` WHERE playerId = %"I64_FMT"u", pl->getId());

                CopyData& cd = getCopyData(pl, id);
                Stream st(REP::AUTO_COPY);
                st << static_cast<UInt8>(1) << id << cd.floor << cd.spot << Stream::eos;
                pl->send(st);
            }
            break;

        case 2:
            {
                if (!pl->hasFlag(Player::AutoCopy))
                {
                    pl->sendMsgCode(0, 1415);
                    return;
                }
            }
            break;

        default:
            break;
    }
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

