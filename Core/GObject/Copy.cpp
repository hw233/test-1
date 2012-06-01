
#include "Copy.h"
#include "Player.h"
#include "GData/CopyTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "GData/Money.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Mail.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "TeamCopy.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "LuckyDraw.h"

namespace GObject
{
#define PRIVILEGE_COUNT 1
#define MAX_COPY_ID 7
UInt8 PlayerCopy::_activeCount = 0;
static UInt16 spots[] = {776, 2067, 5906, 8198, 12818, 10512, 0x1411};

static UInt8 GetCopyIdBySpots(UInt16 currentSpot)
{
    for(UInt8 i = 0; i < sizeof(spots)/sizeof(spots[0]); i++)
    {
        if(spots[i] == currentSpot)
            return (i+1);
    }
    return 1;
}


void PlayerCopy::setCopyActiveCount(UInt8 c) { _activeCount = c; }

void autoClear(Player* pl, bool complete = false, UInt8 id = 0, UInt8 floor = 0, UInt8 spot = 0)
{
    if (!pl)
        return;

    if (complete)
    {
        Stream st(REP::AUTO_COPY);
        st << static_cast<UInt8>(6) << id << floor << spot << pl->getCopyCompleteGold() << Stream::eos;
        pl->send(st);
        pl->resetCopyCompleteGold();
    }

    PopTimerEvent(pl, EVENT_AUTOCOPY, pl->getId());
    pl->setBuffData(PLAYER_BUFF_AUTOCOPY, 0, true);
    pl->delFlag(Player::AutoCopy);
    DB3().PushUpdateData("DELETE FROM `autocopy` WHERE playerId = %"I64_FMT"u", pl->getId());
}

void PlayerCopy::sendAllInfo(Player* pl)
{
}

UInt8 PlayerCopy::getFreeCount()
{
    if (World::_wday == 6)
        return _activeCount + FREECNT * 2;
    return _activeCount + FREECNT;
}

UInt8 PlayerCopy::getGoldCount(UInt8 vipl)
{
    if (vipl == 1)
        return 1;
    if (vipl == 2)
        return 2;
    if (vipl >= 3)
        return 3;
    return 0; // TODO:
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
    UInt8 count = PLAYER_DATA(pl, copyGoldCnt);
    count <<= 4;
    count |= PLAYER_DATA(pl, copyFreeCnt);
    st << count;
    count = getGoldCount(pl->getVipLevel());
    count <<=4;
    count |= getFreeCount();
    st << count;

    if(pl->isBD() && World::getBlueDiamondAct()) {
        count = pl->GetVar(VAR_DIAMOND_BLUE);
        if(count > PRIVILEGE_COUNT)
            count = 0;
        count <<= 4;
        st << count;
        count = PRIVILEGE_COUNT;
        count <<= 4;
        st << count;
    } else if (pl->isYD() && World::getYellowDiamondAct()) {
        count = pl->GetVar(VAR_DIAMOND_YELLOW);
        if(count > PRIVILEGE_COUNT)
            count = 0;
        st << count;
        count = PRIVILEGE_COUNT;
        st << count;
    } else {
        count = 0;
        st << count;
        count = 0;
        st << count;
    }
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
    for (std::map<UInt8, CopyData>::iterator it = pl_copy.begin(), end = pl_copy.end(); it != end; ++it)
        st << static_cast<UInt8>(it->first);
}

bool copyCheckLevel(Player* pl, UInt8 id)
{
    if (!pl)
        return false;

    if(id == 0xff)
    {
        UInt8 realCopyId = GetCopyIdBySpots(PLAYER_DATA(pl, location));
        id = realCopyId;
    }
    if (!id)
        return false;

    static UInt8 lvls[] = {30, 45, 60, 70, 80, 90, 100};
    //static UInt16 spots[] = {776, 2067, 5906, 8198, 12818, 10512};

    if (id > sizeof(lvls)/sizeof(UInt8))
        return false;

    if (pl->getLocation() != spots[id-1])
    {
        SYSMSG_SENDV(2243, pl);
        return false;
    }

    if (pl->GetLev() < lvls[id-1]) {
        SYSMSG_SENDV(2109, pl, pl->GetLev(), lvls[id-1]);
        return false;
    }
    return true;
}

UInt8 PlayerCopy::checkCopy(Player* pl, UInt8 id, UInt8& lootlvl)
{
    if (!pl)
        return 1;

    if (!pl->hasChecked())
        return 1;

    if (!copyCheckLevel(pl, id))
        return 1;
    //diamond privilege
    if(id == 0xff)
    {
        if(pl->isBD() && World::getBlueDiamondAct()) {
            if(pl->GetVar(VAR_DIAMOND_BLUE) < PRIVILEGE_COUNT) {
                pl->AddVar(VAR_DIAMOND_BLUE, 1);
                return 0;
            }
        } else if(pl->isYD() && World::getYellowDiamondAct()) {
            if(pl->GetVar(VAR_DIAMOND_YELLOW) < PRIVILEGE_COUNT) {
                pl->AddVar(VAR_DIAMOND_YELLOW, 1);
                return 0;
            }
        }
        SYSMSG_SENDV(2000, pl);
        return 1;
    }

    if (PLAYER_DATA(pl, copyFreeCnt) < getFreeCount()) {
        ++PLAYER_DATA(pl, copyFreeCnt);
        DB1().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = %u, `copyGoldCnt` = %u WHERE `id` = %"I64_FMT"u",
                PLAYER_DATA(pl, copyFreeCnt), PLAYER_DATA(pl, copyGoldCnt), pl->getId());
        return 0;
    } else if (PLAYER_DATA(pl, copyGoldCnt) < getGoldCount(pl->getVipLevel())) {
        if (pl->getGold() < GData::moneyNeed[GData::COPY_ENTER1+PLAYER_DATA(pl, copyGoldCnt)].gold) {
            pl->sendMsgCode(0, 1104);
            return 1;
        }

        ConsumeInfo ci(EnterCopy,0,0);
        pl->useGold(GData::moneyNeed[GData::COPY_ENTER1+PLAYER_DATA(pl, copyGoldCnt)].gold, &ci);

        ++PLAYER_DATA(pl, copyGoldCnt);
        DB1().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = %u, `copyGoldCnt` = %u WHERE `id` = %"I64_FMT"u",
                PLAYER_DATA(pl, copyFreeCnt), PLAYER_DATA(pl, copyGoldCnt), pl->getId());
        lootlvl = PLAYER_DATA(pl, copyGoldCnt);
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

	FastMutex::ScopedLock lk(_mutex); // XXX: ???

    //if (!id)
    //    id = 1;

    if (pl->hasFlag(Player::AutoCopy)) {
        pl->sendMsgCode(0, 1414);
        return;
    }

    if(pl->hasFlag(Player::InCopyTeam))
    {
        pl->sendMsgCode(0, 2106);
        return;
    }

    if(id == 0xff)
    {
        if(pl->isBD() && World::getBlueDiamondAct()) {
            if(pl->GetVar(VAR_DIAMOND_BLUE) >= PRIVILEGE_COUNT) {
                pl->sendMsgCode(0, 1998);
                return;
            }
        } else if(pl->isYD() && World::getYellowDiamondAct()){
            if(pl->GetVar(VAR_DIAMOND_YELLOW) >= PRIVILEGE_COUNT) {
                pl->sendMsgCode(1, 1999);
                return;
            }
        }
    }

    CopyData& tcd = getCopyData(pl, id, true);
    if (tcd.floor && tcd.spot)
        return;

    UInt8 lootlvl = 0;
    UInt8 ret = checkCopy(pl, id, lootlvl);
    if(id == 0xff)
    {
        UInt8 realCopyId = GetCopyIdBySpots(PLAYER_DATA(pl, location));
        id = realCopyId;
    }
    if (!ret) {
        if (!tcd.floor) {
            tcd.floor = 1;
            tcd.spot = 1;
            tcd.lootlvl = lootlvl;
        }

        DB3().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u, `lootlvl`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u",
                tcd.floor, tcd.spot, lootlvl, pl->getId(), id);

        GameAction()->doAty(pl,  AtyCopy , 0,0);
    }

    Stream st(REP::COPY_INFO);
    st << static_cast<UInt8>(1) << id << ret << Stream::eos;
    pl->send(st);
}

UInt8 PlayerCopy::getCopyFloors(UInt8 id)
{
    UInt8 floors = 0;
    for (UInt8 f = 1; f <= GData::copyMaxManager[id]; ++f)
        floors += GData::copyManager[id<<8|f].size()-1;
    return floors;
}

UInt8 PlayerCopy::fight(Player* pl, UInt8 id, bool ato, bool complete)
{
    if (!pl || !id || id > MAX_COPY_ID)
        return 0;

	FastMutex::ScopedLock lk(_mutex); // XXX:

    if (pl->hasFlag(Player::AutoCopy) && !ato) {
        pl->sendMsgCode(0, 1414);
        return 0;
    }

    if(pl->hasFlag(Player::InCopyTeam))
    {
        pl->sendMsgCode(0, 2106);
        return 0;
    }

    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor)
        return 0;

    if (PLAYER_DATA(pl, copyFreeCnt) > getFreeCount() &&
            PLAYER_DATA(pl, copyGoldCnt) > getGoldCount(pl->getVipLevel()))
        return 0;

    if (!GData::copyManager[id<<8|tcd.floor].size())
        return 0;

    UInt32 fgtid = GData::copyManager[id<<8|tcd.floor][tcd.spot];
    if (!fgtid)
        return 0;

    pl->OnHeroMemo(MC_SLAYER, MD_ADVANCED, 0, 0);
    std::vector<UInt16> loot;
    if (pl->attackCopyNpc(fgtid, 1, id, World::_wday==6?2:1, tcd.lootlvl, ato, &loot)) {
        if (ato)
            pl->checkLastBattled();
        bool nextfloor = false;

        if (tcd.spot >= (GData::copyManager[id<<8|tcd.floor].size() - 1))
            nextfloor = true;

        if (nextfloor) {
            GameAction()->onCopyFloorWin(pl, id, tcd.floor, tcd.spot, tcd.lootlvl);
            ++tcd.floor;
            tcd.spot = 1;
        } else {
            ++tcd.spot;
        }

        if (tcd.floor > GData::copyMaxManager[id]) {
            if (ato) {
                Stream st(REP::AUTO_COPY);
                UInt8 size = loot.size();
                if (size >= 1) {
                    UInt8 rsize = loot[0];
                    if (rsize != size/2)
                        st << static_cast<UInt8>(5);
                    else
                        st << static_cast<UInt8>(4);

                    st << id << tcd.floor << tcd.spot;
                    st << static_cast<UInt8>(rsize);

                    for (UInt8 i = 1, c = 0; i < size && c < rsize; i += 2, ++c)
                        st << loot[i] << static_cast<UInt8>(loot[i+1]);
                } else {
                    st << static_cast<UInt8>(4) << id << tcd.floor << tcd.spot << static_cast<UInt8>(0);
                }

                st << Stream::eos;
                pl->send(st);

                autoClear(pl, complete, id, tcd.floor, tcd.spot);
            } else {
                Stream st(REP::COPY_INFO);
                st << static_cast<UInt8>(5);
                st << id;
                st << Stream::eos;
                pl->send(st);
            }

            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 fourthDay = TimeUtil::SharpDay(3, PLAYER_DATA(pl, created));
            std::cout << id << thisDay << fourthDay << std::endl;
            if(id == 2 && thisDay == fourthDay && !pl->GetVar(VAR_CLAWARD2))
                pl->SetVar(VAR_CLAWARD2, 1);

            GameAction()->onCopyWin(pl, id, tcd.floor, tcd.spot, tcd.lootlvl);

            pl->OnHeroMemo(MC_SLAYER, MD_ADVANCED, 0, 2);
            if (!pl->GetShuoShuo()->getShuoShuo(id-1 + SS_COPY1))
                pl->OnShuoShuo(id-1 + SS_COPY1);

            TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
            if(tcpInfo && tcpInfo->getPass(id, 0) == false)
            {
                tcpInfo->setPass(id, 0, true, true);
                luckyDraw.notifyPass(pl, id);
            }

            tcd.floor = 0;
            tcd.spot = 0;
            DB3().PushUpdateData("DELETE FROM `player_copy` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", pl->getId(), id);
            return 2;
        } else {
            if (ato) {
                Stream st(REP::AUTO_COPY);
                UInt8 size = loot.size();
                if (size) {
                    UInt8 rsize = loot[0];
                    if (rsize != size/2) {
                        autoClear(pl, complete, id, tcd.floor, tcd.spot);
                        st << static_cast<UInt8>(5);
                    } else {
                        st << static_cast<UInt8>(3);
                    }
                    st << id << tcd.floor << tcd.spot;

                    st << static_cast<UInt8>(size/2);
                    for (UInt8 i = 1; i < rsize*2+1; i += 2)
                        st << loot[i] << static_cast<UInt8>(loot[i+1]);
                } else {
                    st << static_cast<UInt8>(3) << id << tcd.floor << tcd.spot << static_cast<UInt8>(0);
                }

                st << Stream::eos;
                pl->send(st);
            } else {
                Stream st(REP::COPY_INFO);
                st << static_cast<UInt8>(6);
                st << id << tcd.floor << tcd.spot;
                st << Stream::eos;
                pl->send(st);
            }
        }

        DB3().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u",
                tcd.floor, tcd.spot, pl->getId(), id);
        return 1;
    } else {
        if (ato) {
            Stream st(REP::AUTO_COPY);
            st << static_cast<UInt8>(2) << id << tcd.floor << tcd.spot << Stream::eos;
            pl->send(st);
            autoClear(pl, complete, id, tcd.floor, tcd.spot);
        } else {
            Stream st(REP::COPY_INFO);
            st << static_cast<UInt8>(2) << id << tcd.floor << tcd.spot << Stream::eos;
            pl->send(st);
        }
    }
    return 0;
}

void PlayerCopy::reset(Player* pl, UInt8 id)
{
    if (!pl)
        return;

	FastMutex::ScopedLock lk(_mutex); // XXX:

    Stream st(REP::COPY_INFO);
    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor) {
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tcd.floor = 0;
    tcd.spot = 0;

    DB3().PushUpdateData("DELETE FROM `player_copy` WHERE `playerId` = %"I64_FMT"u AND `id` = %u",
            tcd.spot, pl->getId(), id);

    st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);

    if (pl->hasFlag(Player::AutoCopy))
    {
        autoClear(pl);
        Stream st(REP::AUTO_COPY);
        st << static_cast<UInt8>(1) << id << tcd.floor << tcd.spot << Stream::eos;
        pl->send(st);
    }
}

void PlayerCopy::addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot, UInt8 lootlvl)
{
    CopyData& cd = m_copys[playerId][id];
    cd.floor = floor;
    cd.spot = spot;
    cd.lootlvl = lootlvl;
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
    if (lock) {
        FastMutex::ScopedLock lk(_mutex);
        if (!TimeUtil::SameDay(TimeUtil::Now(), PLAYER_DATA(pl, copyUpdate)) ||
                getFreeCount() < PLAYER_DATA(pl, copyFreeCnt) || getGoldCount(pl->getVipLevel()) < PLAYER_DATA(pl, copyGoldCnt)) {
            PLAYER_DATA(pl, copyUpdate) = TimeUtil::Now();
            PLAYER_DATA(pl, copyFreeCnt) = 0;
            PLAYER_DATA(pl, copyGoldCnt) = 0;
            if (free)
                *free = 0;
            if (gold)
                *gold = 0;
            DB1().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
        } else {
            if (free)
                *free = PLAYER_DATA(pl, copyFreeCnt);
            if (gold)
                *gold = PLAYER_DATA(pl, copyGoldCnt);
        }
    } else {
        if (!TimeUtil::SameDay(TimeUtil::Now(), PLAYER_DATA(pl, copyUpdate)) ||
                getFreeCount() < PLAYER_DATA(pl, copyFreeCnt) || getGoldCount(pl->getVipLevel()) < PLAYER_DATA(pl, copyGoldCnt)) {
            PLAYER_DATA(pl, copyUpdate) = TimeUtil::Now();
            PLAYER_DATA(pl, copyFreeCnt) = 0;
            PLAYER_DATA(pl, copyGoldCnt) = 0;
            if (free)
                *free = 0;
            if (gold)
                *gold = 0;
            DB1().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
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
    UInt8 idTmp = id;
    if(id == 0xff)
    {
        UInt8 realCopyId = GetCopyIdBySpots(PLAYER_DATA(pl, location));
        id = realCopyId;
    }
    CopyData& cd = m_copys[playerId][id];
    if (!cd.floor && update) {
        DB3().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`) VALUES(%"I64_FMT"u, %u, %u, %u)",
                playerId, id, cd.floor, cd.spot);
    }

    if(idTmp != 0xff)
        getCount(pl, NULL, NULL);
    return cd;
}

void PlayerCopy::autoBattle(Player* pl, UInt8 id, UInt8 type, UInt8 mtype, bool init)
{
    if (!pl || !id)
        return;

    if(pl->hasFlag(Player::InCopyTeam))
    {
        pl->sendMsgCode(0, 2106);
        return;
    }

    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor)
        return;

    switch (type) {
        case 0:
            {
                if (!init) {
                    if (pl->hasFlag(Player::AutoCopy)) {
                        pl->sendMsgCode(0, 1414);
                        return;
                    }

                    if (!copyCheckLevel(pl, id))
                        return;

                    pl->OnHeroMemo(MC_SLAYER, MD_ADVANCED, 0, 1);

                    bool girl = (World::getGirlDay() && !pl->IsMale());
                    if (!World::getNewYear() &&
                            !girl &&
                            !World::getNetValentineDay())
                    {
                        // XXX: moneyNeed must greater than 1000
                        UInt32 pref = 0;
                        UInt8 div = 1;
                        if (pl->getVipLevel() >= 4)
                            pref = 1000;
                        if (World::_wday == 6)
                            div = 2;

                        if (mtype == 1)
                        {
                            if (GData::moneyNeed[GData::COPY_AUTO].gold / div > pl->getGoldOrCoupon()) {
                                pl->sendMsgCode(0, 1101);
                                return;
                            } else {
                                ConsumeInfo ci(EnterAutoCopy,0,0);
                                pl->useGoldOrCoupon(GData::moneyNeed[GData::COPY_AUTO].gold/div, &ci);
                            }
                        }
                        else
                        {
                            if ((GData::moneyNeed[GData::COPY_AUTO1+id-1].tael - pref)/div > pl->getTael()) {
                                pl->sendMsgCode(0, 1100);
                                return;
                            } else {
                                ConsumeInfo ci(EnterAutoCopy,0,0);
                                pl->useTael((GData::moneyNeed[GData::COPY_AUTO1+id-1].tael - pref)/div, &ci);
                            }
                        }
                    }
                }

                UInt8 floors = 0;
                UInt8 sp = tcd.spot;
                for (UInt8 f = tcd.floor; f <= GData::copyMaxManager[id]; ++f) {
                    UInt8 s = GData::copyManager[id<<8|f].size();
                    for (UInt8 i = sp; i < s; ++i)
                        ++floors;
                    sp = 1;
                }

                UInt8 secs = 0;
                if (cfg.GMCheck)
                    secs = 60;
                else
                    secs = 20;

                EventAutoCopy* event = new (std::nothrow) EventAutoCopy(pl, secs, floors, id);
                if (!event) return;
                PushTimerEvent(event);

                pl->addFlag(Player::AutoCopy);
                pl->setBuffData(PLAYER_BUFF_AUTOCOPY, id, true);
                DB3().PushUpdateData("REPLACE INTO `autocopy` (`playerId`, `id`) VALUES (%"I64_FMT"u, %u)", pl->getId(), id);

                Stream st(REP::AUTO_COPY);
                st << static_cast<UInt8>(0) << id << tcd.floor << tcd.spot << Stream::eos; 
                pl->send(st);
            }
            break;

        case 1:
            {
                if (!pl->hasFlag(Player::AutoCopy)) {
                    pl->sendMsgCode(0, 1415);
                    return;
                }

                autoClear(pl);

                Stream st(REP::AUTO_COPY);
                st << static_cast<UInt8>(1) << id << tcd.floor << tcd.spot << Stream::eos;
                pl->send(st);
            }
            break;

        case 2:
            {
                if (!pl->hasFlag(Player::AutoCopy)) {
                    pl->sendMsgCode(0, 1415);
                    return;
                }

                if (!World::getNewYear() && !pl->isYD() && !pl->isBD())
                {
                    if (pl->getVipLevel() < 6)
                        return;
                }

                if (!pl->hasChecked())
                    return;

                if (GData::moneyNeed[GData::COPY_IM].gold > pl->getGoldOrCoupon())
                {
                    pl->sendMsgCode(0, 1101);
                    return;
                }

                ConsumeInfo ci(AutoCopyComplete,0,0);
                pl->useGoldOrCoupon(GData::moneyNeed[GData::COPY_IM].gold, &ci);
                pl->addCopyCompleteGold(GData::moneyNeed[GData::COPY_IM].gold);

                autoClear(pl);
                pl->addFlag(Player::AutoCopy);

                UInt8 sp = tcd.spot;
                for (UInt8 f = tcd.floor; f <= GData::copyMaxManager[id]; ++f) {
                    UInt8 s = GData::copyManager[id<<8|f].size();
                    for (UInt8 i = sp; i < s; ++i) {
                        if (!fight(pl, id, true, true))
                            goto _over;
                    }
                    sp = 1;
                }
                pl->setBuffData(PLAYER_BUFF_ATTACKING, 0);
            }
_over:
            break;

        default:
            break;
    }
}

void PlayerCopy::sendAutoCopy(Player* pl)
{
    UInt8 id = pl->getBuffData(PLAYER_BUFF_AUTOCOPY);
    if (!id)
        return;

    FastMutex::ScopedLock lk(_mutex);
    CopyData& tcd = getCopyData(pl, id); 
    if (!tcd.floor)
        return;

    Stream st(REP::AUTO_COPY);
    st << static_cast<UInt8>(0) << id << tcd.floor << tcd.spot << Stream::eos; 
    pl->send(st);
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

