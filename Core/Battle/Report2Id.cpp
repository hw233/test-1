#include"Report2Id.h"

namespace Battle
{
    Report2IdTable report2IdTable;
    
    UInt32 Report2Id::GetRecentReportId()
    {
        //按照发生时间进行排序
        if( vecReportOneRound.empty() )
            return 0;
        LessActTime<ReportOneRound*> lessActTime;
        std::sort(vecReportOneRound.begin(), vecReportOneRound.end(), lessActTime);
        return (vecReportOneRound.back())->GetReportId();
    }

    void Report2IdTable::Insert(UInt32 roomId,UInt8 cityId,UInt16 actId, UInt32 reportId,UInt32 actTime)
    {
        Report2Id* report = new Report2Id(roomId,cityId);
        ReportOneRound* reportOneRound = new ReportOneRound(actId,reportId);
        reportOneRound->SetTime(actTime);
        report->InsertReportOneRound(reportOneRound);
        _vecReportId.push_back(report);
    }

    UInt32 Report2IdTable::GetRecentReportId(UInt32 roomId, UInt8 cityId)
    {
        for( auto it = _vecReportId.begin(); it != _vecReportId.end(); ++it )
        {
            if( (*it)->GetRoomId() == roomId && (*it)->GetCityId() == cityId )
            {
                return (*it)->GetRecentReportId();
            }
        }
        return 0;
    }
     
};
