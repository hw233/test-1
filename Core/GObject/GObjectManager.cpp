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
#include "GObject/Player2Id.h"
#include "GObject/Fighter.h"
#include "FVar.h"
namespace GObject
{
    URandom GRND(time(0)); 
    std::map<UInt32, ItemEquip *> GObjectManager::equips;

    bool GObjectManager::InitIDGen()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        UInt32 maxId = 0;
        execu->Extract("SELECT max(`id`) FROM `player_id`", maxId);
        IDGenerator::gPlayerOidGenerator.Init(maxId);
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
        if(!loadPlayerId())
        {
            fprintf(stderr, "loadPlayerId error!\n");
            std::abort();
        }
        if(!loadFighter())
        {
            fprintf(stderr, "loadFighter error!\n");
            std::abort();
        }
        if(!loadFighterVar())
        {
            fprintf(stderr, "loadFighterVar error!\n");
            std::abort();
        }
    } 
    bool GObjectManager::loadAllPlayers()
    { 
        IDTYPE last_id;

        using namespace std::placeholders;

        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading players:");
        lc.reset(200);
        // load players
        DBPlayerData dbpd;
        if(execu->Prepare("SELECT `id`,`name` FROM `player`", dbpd) != DB::DB_OK)
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

            IDTYPE id = dbpd.id;
            Player * pl = new Player(id);
            pl->SetName(dbpd.name);
            //XXX
            //pl->setId(id);
            //pl->setAccounts(dbpd.accounts);
            //pl->setPassword(dbpd.password);

            if(id)
                globalPlayers.add(id, pl);  //手机号
            //if(dbpd.accounts.empty())
            //    globalAccountsPlayers.add(dbpd.accounts, pl); //帐号
            globalNamedPlayers.add(pl->GetName(), pl);
        }

        lc.prepare("Loading player vars:");
        lc.reset(100);
        last_id = IDTYPE();
        Player* pl = NULL;

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
        LoadingCounter lc("Loading GVar");
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

    bool GObjectManager::loadPlayerId()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading player_id");
        lc.reset(1000);
        DBPlayer2Id p2i;
        if(execu->Prepare("SELECT `id`,`phoneId`, `accounts`, `password` FROM `player_id` ORDER BY `id`", p2i) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            player2Id.InsertId(p2i.phoneId , p2i.accounts , p2i.password , p2i.id);
        }
        lc.finalize();
        return true;
    }
    bool GObjectManager::loadFighterVar()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading FighterVar");

        DBFighterVar fighterVar;
        IDTYPE last_id = 0;
        UInt32 fighterId = 0;
        Player* pl = NULL;
        Fighter* fgt = NULL;
        if(execu->Prepare("SELECT `playerId`, `fighterId`,`id`, `data`, `over` FROM `fvar` ORDER BY `playerId`", fighterVar) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(fighterVar.playerId != last_id)
            {
                last_id = fighterVar.playerId;
                pl = globalPlayers[last_id];
            }
            if(pl == NULL) continue;

            if( fighterVar.fighterId != fighterId)
            {
                fighterId = fighterVar.fighterId;
                fgt = pl->findFighter(fighterId);
            }
            if(fgt == NULL) continue;
            fgt->GetFVar()->LoadFVar(fighterVar.id,fighterVar.data,fighterVar.overTime);
        }
        lc.finalize();
        return true;
    } 

    bool GObjectManager::loadFighter()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Fighter");

        DBFighterInfo fighterInfo;
        IDTYPE last_id = 0;
        Player* pl = NULL;
        if(execu->Prepare("SELECT `playerId`, `fighterId`,`experience`, `weapon`, `armor1` , `armor2`,`armor3`,`armor4`,`armor5` FROM `fighter` ORDER BY `playerId`",fighterInfo) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(fighterInfo.playerId != last_id)
            {
                last_id = fighterInfo.playerId;
                pl = globalPlayers[last_id];
            }
            if(pl == NULL) continue;

            //if( fighterInfo.fighterId != fighterId)
            //{
            //    fighterId = fighterInfo.fighterId;
            //    fgt = pl->findFighter(fighterInfo.fighterId);
            //}
            //if(fgt == NULL) continue;

            Fighter * fgt = globalFighters[fighterInfo.fighterId];
            if(fgt == NULL)
                continue;
            Fighter * fgt2 = fgt->Clone(pl);
            if(fgt2 == NULL)
                continue;
            fgt2->SetExp(fighterInfo.experience);
            fgt2->GetFVar()->SetFVar(FVAR_WEAPON_ENCHANT,100);


            pl->addFighter(fgt2, false, true);
        }
        lc.finalize();
        return true;
    }

    ItemEquip * GObjectManager::fetchEquipment( UInt32 id, bool record )
    {
        if(id == 0)
            return NULL;
        std::map<UInt32, ItemEquip *>::iterator it = equips.find(id);
        if(it == equips.end())
        {
            return NULL;
        }
        ItemEquip * base = it->second;
        equips.erase(it);
        return base;
    }
    ItemWeapon * GObjectManager::fetchWeapon( UInt32 id )
    {
        ItemEquip * equip = fetchEquipment(id);
        if(equip == NULL)
            return NULL;
        if(equip->GetItemType().subClass != static_cast<UInt8>(Item_Weapon))
        {
            delete equip;
            return NULL;
        }
        return static_cast<ItemWeapon *>(equip);
    }
    ItemArmor * GObjectManager::fetchArmor( UInt32 id )
    {
        ItemEquip * equip = fetchEquipment(id);
        if(equip == NULL)
            return NULL;
        if(equip->GetItemType().subClass < static_cast<UInt8>(Item_Armor1) || equip->GetItemType().subClass > static_cast<UInt8>(Item_Armor5))
        {
            delete equip;
            return NULL;
        }
        return static_cast<ItemArmor *>(equip);
    }
}
