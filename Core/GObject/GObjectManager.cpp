#include "Config.h"
#include "GObjectManager.h"
#include "DB/DBConnectionMgr.h"                                                                                                   
#include "GObjectDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/OidGenerator.h"
#include "Common/Itoa.h"

#include "Common/LoadingCounter.h"
#include "Common/StringTokenizer.h"
#include "Common/DirectoryIterator.h"

#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "GObject/Item.h"

#include "GObject/Player.h"
namespace GObject
{
    URandom GRND(time(0)); 
    std::map<UInt32, ItemEquip *> GObjectManager::equips;

	bool GObjectManager::InitIDGen()
	{
        return true;
	}
	void GObjectManager::loadAllData()
    { 
        if(!loadGVar())
        {
            fprintf(stderr, "loadGVar error!\n");
            std::abort();
        }

		if(!loadAllPlayers())
        {
            fprintf(stderr, "loadAllPlayers error!\n");
            std::abort();
        }
    } 
    bool GObjectManager::loadAllPlayers()
    { 
        std::string last_id;

        using namespace std::placeholders;

        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading players:");
        lc.reset(200);
        // load players
        DBPlayerData dbpd;
		if(execu->Prepare("SELECT `id`,`accounts`,`password`,`name` FROM `player`", dbpd) != DB::DB_OK)
            return false;
		while(execu->Next() == DB::DB_OK)
		{
#define LOAD_LINEUP(lus, lud) \
			{ \
				StringTokenizer tk(lus, "|"); \
				for(size_t z = 0; z < tk.count(); ++ z) \
				{ \
					if(z > 4) break; \
					StringTokenizer tk2(tk[z], ","); \
					if(tk2.count() < 2) \
						continue; \
					lud[z].fid = atoi(tk2[0].c_str()); \
					lud[z].pos = atoi(tk2[1].c_str()); \
				} \
			}
        }

        std::string id = dbpd.id;
        Player * pl = new Player();
        //XXX
        pl->setId(id);
        pl->setAccounts(dbpd.accounts);
        pl->setPassword(dbpd.password);

        globalPlayers.add(id, pl);  //手机号
        globalAccountsPlayers.add(dbpd.accounts, pl); //帐号
        globalNamedPlayers.add(pl->getName(), pl);

        lc.prepare("Loading player vars:");
        lc.reset(100);
		last_id = "";
        pl = NULL;
        DBPlayerVar playerVar;
        if(execu->Prepare("SELECT `playerId`, `id`, `data`, `over` FROM `var` ORDER BY `playerId`", playerVar) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(playerVar.playerId != last_id)
            {
                last_id = playerVar.playerId;
                pl = globalPlayers[last_id];
            }
            if(pl == NULL) continue;
            pl->LoadVar(playerVar.id, playerVar.data, playerVar.overTime);
        }
        lc.finalize();

        return true;
    } 

    bool GObjectManager::loadGVar()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading RNR");
		lc.reset(1000);
        DBGVar gvar;
        if(execu->Prepare("SELECT `id`, `data`, `over` FROM `gvar` ORDER BY `id`", gvar) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
			lc.advance();
            GVarSystem::Instance().LoadVar(gvar.id, gvar.data, gvar.overTime);
        }
        lc.finalize();
        return true;
    }
}
