#include "Config.h"
#include "BattleReport.h"
#include "Server/OidGenerator.h"
#include "Server/Cfg.h"
#include "Common/URandom.h"
#include "Common/DirectoryIterator.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "GData/GDataDBExecHelper.h"
#include "Server/WorldServer.h"

namespace Battle
{

BattleReport battleReport;

int lastMaxId = 0;
void BattleReport::init()
{
	try
	{
		UInt32 maxid = 0;
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor()); 
        if (execu.get() != NULL && execu->isConnected())     
        {
            GData::DBReportId dbexp;
            if(execu->Prepare("SELECT maxid from reportid", dbexp) == DB::DB_OK)
            {
                if(execu->Next() == DB::DB_OK)
                {
                    maxid = dbexp.id;
                }
                else
                {
                    DB1().PushUpdateData("insert into reportid values(0)");
                }
            }
            if (maxid > 0)
            {
                lastMaxId = maxid;
	    	    IDGenerator::gBattleOidGenerator.Init(maxid);
                return;
            }
        }
		DirectoryIterator dirit(cfg.reportPath);
		DirectoryIterator end;
		while (dirit != end)
		{
			DirectoryIterator dirit2(dirit.path().toString());
			while (dirit2 != end)
			{
				DirectoryIterator dirit3(dirit2.path().toString());
				while (dirit3 != end)
				{
					UInt32 id = atoi(dirit3.path().getBaseName().c_str());
					if(id > 0)
					{
						if(id > maxid)
							maxid = id;
						//_reports[id].clear();
                        _reports.insert(std::make_pair(id, std::vector<UInt8>()));
					}
					++dirit3;
				}
				++dirit2;
			}
			++dirit;
		}
		IDGenerator::gBattleOidGenerator.Init(maxid);
	}
	catch(...)
	{
		return;
	}
}

std::vector<UInt8>* BattleReport::operator[]( UInt32 id )
{
	FastMutex::ScopedLock lk(_mutex);
	std::map<UInt32, std::vector<UInt8> >::iterator it = _reports.find(id);
	if(it == _reports.end())
    {
        std::vector<UInt8> v;
        loadReport(id, v);
        if(!v.empty())
            _reports[id] = v;
        return &(_reports[id]);
    }
    else if(it->second.empty())
	{
		loadReport(id, it->second);
		if(it->second.empty())
		{
			_reports.erase(it);
			return NULL;
		}
	}
	return &it->second;
}

std::vector<UInt8>* BattleReport::getTitleBattle()
{
	return (*this)[0xFFFFFFFF - uRand(4)];
}

void BattleReport::addReport( UInt32 id, std::vector<UInt8>& v )
{
	FastMutex::ScopedLock lk(_mutex);
	_reports[id] = v;
	char path[1024], path2[1024];
	sprintf(path, "%s%u/%u", cfg.reportPath.c_str(), id / 100000, (id / 1000) % 100);
	sprintf(path2, "%s/%u.dat", path, id);
	File rfile(path);
	rfile.createDirectories();
	FILE * f = fopen(path2, "wb");
	if(f == NULL)
		return;
	fwrite(&v[0], 1, v.size(), f);
	fclose(f);
    DB1().PushUpdateData("update reportid set maxid = %d where maxid=%d", id, lastMaxId);
    lastMaxId = id;
}

void BattleReport::loadReport( UInt32 id, std::vector<UInt8>& v )
{
	char path[1024];
	sprintf(path, "%s%u/%u/%u.dat", cfg.reportPath.c_str(), id / 100000, (id / 1000) % 100, id);
	FILE * f = fopen(path, "rb");
	if(f == NULL)
		return;
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	v.resize(size);
	fread(&v[0], 1, size, f);
	fclose(f);
}

}
