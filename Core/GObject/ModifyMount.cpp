#include "Config.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "Log/Log.h"
#include "Server/SysMsg.h"
#include "Script/GameActionLua.h"
#include "GObject/Country.h"
#include "GData/RideConfig.h"
#include "GObjectDBExecHelper.h"
#include "Player.h"
#include "MsgID.h"
#include "ModifyMount.h"
#include "Package.h"

namespace GObject
{

    void ModifyMount::updateToDB()
    {
        std::string chipStr;
        for(UInt8 i = 0; i < MOUNTCHIP_MAX; ++ i)
        {
            chipStr += Itoa(_chips[i]);
            if(i < MOUNTCHIP_MAX-1)
                chipStr += ",";
        }
        DB2().PushUpdateData("REPLACE INTO `modify_mount` (`id`, `playerId`, `chips`, `curfloor`, `curfloor1`, `failtimes`)\
                VALUES (%u, %" I64_FMT "u, '%s', %u, %u, %u)", getId(), _owner->getId(), chipStr.c_str(), _curFloor, _curFloor1, _failTimes);
    }

    bool ModifyMount::hasChip(UInt32 itemId)
    {
        for(UInt8 i = 0; i < MOUNTCHIP_MAX; ++ i)
        {
            if(_chips[i] == itemId)
                return true;
        }
        return false;
    }

    bool ModifyMount::hasFullChips()
    {
        UInt8 cnt = 0;
        for(UInt8 i = 0; i < MOUNTCHIP_MAX; ++ i)
        {
            if(_chips[i])
                ++ cnt;
        }
        return cnt >= MOUNTCHIP_MAX;
    }

    bool ModifyMount::addChip(UInt32 itemId)
    {
        if(!itemId || hasChip(itemId))
            return false;
        UInt8 pos = GData::ride.getChipPos(getId(), itemId);
        if(pos >= MOUNTCHIP_MAX)
            return false;
        _chips[pos] = itemId;
        updateToDB();
        _owner->setLineupDirty();

        if(hasFullChips())
        {
            UInt32 item_id = GData::ride.getMountItemId(getId());
            SYSMSG_BROADCASTV(4158, _owner->getCountry(), _owner->getName().c_str(), item_id);
        }

        sendMountInfo();
        return true;
    }

    void ModifyMount::setChipFromDB(UInt8 pos, UInt32 itemId)
    {
        if(pos >= MOUNTCHIP_MAX)
            return;
        _chips[pos] = itemId;
    }

    void ModifyMount::appendMountInfo(Stream& st)
    {
        UInt8 flag = 0;
        for(UInt8 i = 0; i < MOUNTCHIP_MAX; ++ i)
        {
            if(_chips[i])
                flag |= 1<<i;
        }
        st << getId() << flag;
        st << _curFloor << _curFloor1 << _failTimes;
    }

    void ModifyMount::addAttrExtra(GData::AttrExtra& attr)
    {
        UInt8 cnt = 0;
        for(UInt8 i = 0; i < MOUNTCHIP_MAX; ++ i)
        {
            if(_chips[i])
            {
                GData::ItemGemType * igt = GData::mountTypes[_chips[i]-LMOUNT_ID];
                if(igt)
                    attr += *(igt->attrExtra);
                ++ cnt;
            }
        }
        if(cnt >= MOUNTCHIP_MAX)
        {
            UInt32 itemId = GData::ride.getMountItemId(getId());
            if(itemId)
            {
                GData::ItemGemType * igt = GData::mountTypes[itemId-LMOUNT_ID];
                if(igt)
                    attr += *(igt->attrExtra);
            }
        }
    }

    void ModifyMount::cangjianya(UInt8 floors, bool isAuto)
    {
        if(!_owner) return;
        Package * pkg = _owner->GetPackage();
        if(!pkg) return;
        UInt8 curFloor = _curFloor;
        if(isFullFloor())
            curFloor = _curFloor1;
        if(curFloor >= MOUNTCHIP_MAX)
            curFloor = 0;
        if(floors > MOUNTCHIP_MAX || floors <= curFloor)
            floors = curFloor + 1;
        GData::Ride::CangjianData * cjd = GData::ride.getCangjianTable(curFloor+1);
        if(NULL == cjd) return;
        int itemNum = pkg->GetItemAnyNum(MOUNT_CANGJIANID);
        int cost = 0, cost1 = 0;
        int leftcnt = _owner->GetVar(VAR_MOUNT_CANGJIANYA_LEFT_CNT);
        if(itemNum + leftcnt < (GData::ride.isUseTwo(getId()) ? 2 : 1))
            return;
        UInt32 failNum = 0;
        if(isAuto)  //自动唤剑
        {
            while(curFloor < floors)
            {
                cjd = GData::ride.getCangjianTable(curFloor+1);
                if(NULL == cjd) break;
                UInt16 bless = GData::ride.getCangjianBless(curFloor+1, leftcnt > 0 ? _failTimes/5 : _failTimes);
                if(GData::ride.isUseTwo(getId()))
                {
                    if(leftcnt > 1)
                    {
                        cost += 2;
                        leftcnt -= 2;
                    }
                    else if(itemNum > 1)
                    {
                        cost1 += 2;
                        itemNum -= 2;
                    }
                    else if((leftcnt == 1 && itemNum >= 1) || (itemNum == 1 && leftcnt >= 1))
                    {
                        ++ cost;
                        ++ cost1;
                        -- leftcnt;
                        -- itemNum;
                    }
                    else
                        break;
                }
                else
                {
                    if(leftcnt > 0)
                    {
                        ++ cost;
                        -- leftcnt;
                    }
                    else if(itemNum > 0)
                    {
                        ++ cost1;
                        -- itemNum;
                    }
                    else
                        break;
                }
                if(uRand(10000) <= cjd->prob + bless)
                {   //成功
                    _failTimes = 0;
                    ++ curFloor;
                    UInt32 itemId = GData::ride.getMountItemId(getId());
                    UInt32 chipId = GameAction()->getMountChipByCangjian(itemId, curFloor); 
                    if(chipId == 0)
                        break;
                    if(_owner->hasMountChip(chipId))
                        pkg->Add(MOUNT_COSTID, cjd->otherNum, true, false, FromBox);
                    else
                    {
		                const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[chipId];
                        if(itemType && itemType->quality == Item_Yellow)
                            SYSMSG_BROADCASTV(4159, _owner->getCountry(), _owner->getName().c_str(), chipId);
                        pkg->Add(chipId, 1, true, true, FromBox);
                        pkg->UseItem(chipId, 1, 0, 0, 1);
                    }
                    /*
                    if(isFullFloor())
                        break;
                    */
                    if(curFloor >= MOUNTCHIP_MAX)
                        break;
                }
                else
                {   //失败
                    ++ _failTimes;
                    ++ failNum;
                }
                if(leftcnt <= 0 && itemNum <= 0)
                    break;
            }
        }
        else    //唤剑一次
        {
            UInt16 bless = GData::ride.getCangjianBless(curFloor+1, leftcnt > 0 ? _failTimes/5 : _failTimes);
            if(GData::ride.isUseTwo(getId()))
            {
                if(leftcnt > 1)
                {
                    cost += 2;
                    leftcnt -= 2;
                }
                else if(itemNum > 1)
                {
                    cost1 += 2;
                    itemNum -= 2;
                }
                else if((leftcnt == 1 && itemNum >= 1) || (itemNum == 1 && leftcnt >= 1))
                {
                    ++ cost;
                    ++ cost1;
                    -- leftcnt;
                    -- itemNum;
                }
            }
            else
            {
                if(leftcnt > 0)
                {
                    ++ cost;
                    -- leftcnt;
                }
                else if(itemNum > 0)
                {
                    ++ cost1;
                    -- itemNum;
                }
            }
            if(uRand(10000) <= cjd->prob + bless)
            {   //成功
                _failTimes = 0;
                ++ curFloor;
                UInt32 itemId = GData::ride.getMountItemId(getId());
                UInt32 chipId = GameAction()->getMountChipByCangjian(itemId, curFloor); 
                if(!chipId || _owner->hasMountChip(chipId))
                    pkg->Add(MOUNT_COSTID, cjd->otherNum, true, false, FromBox);
                else
                {
                    const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[chipId];
                    if(itemType && itemType->quality == Item_Yellow)
                        SYSMSG_BROADCASTV(4159, _owner->getCountry(), _owner->getName().c_str(), chipId);
                    pkg->Add(chipId, 1, true, true, FromBox);
                    pkg->UseItem(chipId, 1, 0, 0, 1);
                }
            }
            else
            {   //失败
                ++ _failTimes;
                ++ failNum;
            }
        }
        if(isFullFloor())
            setCurfoor1(curFloor);
        else
            setCurfoor(curFloor);
        _owner->SetVar(VAR_MOUNT_CANGJIANYA_LEFT_CNT, leftcnt);
        if(cost1 > 0)
        {
            pkg->DelItemAny(MOUNT_CANGJIANID, cost1, NULL, ToUse);
            pkg->DelItemSendMsg(MOUNT_CANGJIANID, _owner);
        }
        if(failNum > 0)
            pkg->Add(MOUNT_COSTID, failNum, true, false, FromBox);
        updateToDB();
        sendMountInfo();
        _owner->check_Cangjianya();
        GameAction()->doStrong(_owner, SthCangjiya, 0,0);
    }

    void ModifyMount::sendMountInfo()
    {
        Stream st(REP::MODIFY_MOUNT);
        st << static_cast<UInt8>(2);
        appendMountInfo(st);
        st << Stream::eos;
        _owner->send(st);
    }
}
