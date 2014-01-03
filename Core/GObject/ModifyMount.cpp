#include "Config.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "GData/RideConfig.h"
#include "GObjectDBExecHelper.h"
#include "Player.h"
#include "MsgID.h"
#include "ModifyMount.h"

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
        DB2().PushUpdateData("REPLACE INTO `modify_mount` (`id`, `playerId`, `chips`) VALUES (%u, %" I64_FMT "u, '%s')",
                getId(), _owner->getId(), chipStr.c_str());
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

    bool ModifyMount::addChip(UInt32 itemId)
    {
        if(!itemId || hasChip(itemId))
            return false;
        UInt8 pos = GData::ride.getChipPos(getId(), itemId);
        if(pos >= MOUNTCHIP_MAX)
            return false;
        _chips[pos] = itemId;
        updateToDB();

        Stream st(REP::MODIFY_MOUNT);
        st << static_cast<UInt8>(2);
        appendMountInfo(st);
        st << Stream::eos;
        _owner->send(st);
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

}
