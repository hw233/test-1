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

    
    ReportOneRound* Report2Id::GetEarliestReport()
    {
        if( vecReportOneRound.empty())
            return NULL;
        LessActTime<ReportOneRound*> lessActTime;
        std::sort(vecReportOneRound.begin(), vecReportOneRound.end(), lessActTime);
        return (vecReportOneRound.front());
    }

    UInt32 Report2Id::GetEarliestTime()
    {
        UInt32 time = TimeUtil::Now();
        ReportOneRound* roundReport = GetEarliestReport();
        if( roundReport != NULL )
        {
            time = roundReport->GetTime();
        }
        return time;
    }

    void Report2IdTable::Insert(UInt32 roomId,UInt8 cityId,UInt16 actId, UInt32 reportId,UInt32 actTime)
    {
        //
        Report2Id* report = GetReport2Id(roomId,cityId);
        UInt8 flag = 0;
        if( report == NULL )
        {
            report = new Report2Id(roomId,cityId);
            flag = 1;
        }
        ReportOneRound* reportOneRound = new ReportOneRound(actId,reportId);
        reportOneRound->SetTime(actTime);
        report->InsertReportOneRound(reportOneRound);
        if( flag == 1 )
        {
            _vecReportId.push_back(report);
        }
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

    Report2Id* Report2IdTable::GetReport2Id(UInt32 roomId,UInt8 cityId)
    {
        for( auto it = _vecReportId.begin(); it != _vecReportId.end(); ++it )
        {
            if( (*it)->GetRoomId() == roomId && (*it)->GetCityId() == cityId )
            {
                return (*it);
            }
        }
        return NULL;
    }

    std::vector<Battle::Report2Id*> Report2IdTable::GetReport2Ids(UInt32 roomId)
    {
        std::vector<Report2Id*> vecReport;
        for(auto it = _vecReportId.begin(); it != _vecReportId.end(); ++it )
        {
            if( (*it)->GetRoomId() == roomId )
            {
                vecReport.push_back((*it));
            }
        }
        return vecReport;
    }

};
