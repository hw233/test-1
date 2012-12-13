#ifndef _BATTLEREPORT_H_
#define _BATTLEREPORT_H_

#include "Common/Mutex.h"

namespace Battle
{

class BattleReport
{
public:
	void init();
	void addReport(UInt32, std::vector<UInt8>&);
	std::vector<UInt8>* operator[](UInt32 id);
	std::vector<UInt8>* getTitleBattle();
private:
	void loadReport(UInt32, std::vector<UInt8>&);
    void travelRptLife();
private:
	FastMutex _mutex;
	std::map<UInt32, std::vector<UInt8> > _reports;

    struct RptLife
    {
        UInt32 _rptId;
        UInt32 _loadTime;
        bool operator< (const RptLife& rptLife) const
        {
            if(_loadTime == rptLife._loadTime)
                return _rptId < rptLife._rptId;

            return _loadTime < rptLife._loadTime;
        }
    };
    typedef std::set<RptLife> RptLifeSet;
    typedef RptLifeSet::iterator RptLifeSetIt;

    RptLifeSet _rptLife;
};

extern BattleReport battleReport;

}

#endif // _BATTLEREPORT_H_
