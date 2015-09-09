#pragma once
#ifndef __REPORT2ID_H__
#define __REPORT2ID_H__
#include"Config.h"
#include"Common/TimeUtil.h"

namespace Battle
{

    //一回合
    class ReportOneRound
    {
        public:
            ReportOneRound(UInt16 a,UInt32 r) : actId(a),reportId(r){}
            UInt16 GetActId() const { return actId;}
            UInt32 GetReportId() const { return reportId;}
            UInt32 GetTime() const { return time;}
            void SetTime(UInt32 t) { time = t;}
        private:
            UInt16 actId;       //回合Id
            UInt32 reportId;    //战报Id
            UInt32 time;        //战斗发生时间
    };

    template<class T=ReportOneRound*>
    struct LessActTime
    {
        bool operator() (T round1,T round2)
        {
            if( round1->GetTime() < round2->GetTime() )
            {
                return true;
            }
            return false;
        }
    };



    //一个房间的一个城的多个回合的战术
    class Report2Id
    {
        public:
            Report2Id(UInt32 r,UInt8 c) : roomId(r) , cityId(c) {}
            UInt32 GetRoomId() const { return roomId;}
            UInt8  GetCityId() const { return cityId;}
            UInt32 GetRecentReportId();
            std::vector<ReportOneRound*> GetReportOneRounds() const { return vecReportOneRound;}
            void SetVecRepoortOneRound(std::vector<ReportOneRound*> vecReports) { vecReportOneRound = vecReports;}
            void InsertReportOneRound(ReportOneRound* reportRound) { vecReportOneRound.push_back(reportRound);}
            ReportOneRound* GetEarliestReport();
            UInt32 GetEarliestTime();
            ReportOneRound* GetNewestReport();
        private:
            UInt32 roomId;
            UInt8  cityId;
            std::vector<ReportOneRound*> vecReportOneRound;

    };

    
    /*
    class Report2Id
    {
        public:
            Report2Id(UInt32 r,UInt8 c,UInt16 a, UInt32 i) : roomId(r),cityId(c),actId(a) , id(i) {}
            UInt32 GetRoomId() const { return roomId;}
            UInt8  GetCityId() const { return cityId;}
            UInt32 GetReportId() const { return id;}
            UInt16 GetActId() const { return actId;}
            UInt32 GetTime() const { return time;}
            void SetTime(UInt32 t) { time = t;}

        private:
            UInt32 roomId;
            UInt8  cityId;
            UInt16 actId;
            UInt32 id;
            UInt32 time;
    };
    */

    class Report2IdTable
    {
        public:
            void Insert(UInt32 roomId,UInt8 cityId,UInt16 actId, UInt32 reportId,UInt32 actTime);
            Report2Id* GetReport2Id(UInt32 roomId,UInt8 cityId);
            UInt32 GetRecentReportId(UInt32 roomId, UInt8 mapId);
            std::vector<Battle::Report2Id*> GetReport2Ids(UInt32 roomId);
            void RemoveReports(UInt32 roomId);
        private:
            std::vector<Report2Id*> _vecReportId;

    };

    extern Report2IdTable report2IdTable;

}
#endif

