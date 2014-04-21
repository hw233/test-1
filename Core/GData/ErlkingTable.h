#ifndef _ERLKINGTABLE_H_
#define _ERLKINGTABLE_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{

class ErlkingData
{
    public:
        struct erlkingInfo
        {
            UInt8 copyId;       // 副本Id
            UInt8 conditionA;   // 条件A
            UInt8 conditionB;   // 条件B
            UInt32 npcgroup;    // 妖王Id
            UInt32 itemId;      // 首次开打掉落物品Id

            erlkingInfo(): copyId(0), conditionA(0), conditionB(0), npcgroup(0), itemId(0) {}
        };

    private:
        std::map<UInt32, erlkingInfo> _erlkingInfo;

    public:
        void setErlkingInfo(DBErlkingConfig &);

        erlkingInfo * getErlkingInfo(UInt8 copyId);
};

extern ErlkingData erlkingData;

}

#endif
