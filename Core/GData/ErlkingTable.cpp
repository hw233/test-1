#include "ErlkingTable.h"

namespace GData
{
    ErlkingData erlkingData;

    void ErlkingData::setErlkingInfo(DBErlkingConfig & data)
    {
        erlkingInfo info;

        info.copyId = data.copyId;
        info.conditionA = data.conditionA;
        info.conditionB = data.conditionB;
        info.npcgroup= data.npcgroup;

        _erlkingInfo.insert(std::make_pair(info.copyId, info));
    }

    ErlkingData::erlkingInfo * ErlkingData::getErlkingInfo(UInt8 copyId)
    {
        std::map<UInt32, erlkingInfo>::iterator iter = _erlkingInfo.find(copyId);
        if(iter != _erlkingInfo.end())
            return &(iter->second);

        return NULL;
    }
}
