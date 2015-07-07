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
#include "GObject/Friend.h"
#include "Mail.h"
#include "Battle/ClanBattleRoom.h"
#include "Battle/ClanBattleDistribute.h"
#include "Battle/ClanBattleComment.h"
#include "Battle/ClanOrder.h"
#include "Battle/Report2Id.h"
#include "Battle/ClanBattleCityStatus.h"

namespace GObject
{
    URandom GRND(time(0)); 
    std::map<UInt32, ItemEquip *> GObjectManager::equips;

    bool GObjectManager::InitIDGen()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        UInt32 maxId = 0;
        execu->Extract("SELECT max(`id`&0xffffffffff) FROM `player_id`", maxId);
        IDGenerator::gPlayerOidGenerator.Init(maxId);

        //std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        execu->Extract("SELECT max(`id`) FROM `mail`", maxId);
        IDGenerator::gMailOidGenerator.Init(maxId);

        execu->Extract("SELECT max(`id`) FROM `clan`", maxId);
        IDGenerator::gClanOidGenerator.Init(maxId<100?100:maxId);
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
        if(!loadAccountPwd())
        {
            fprintf(stderr, "loadAccountPwd error!\n");
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

        if(!loadFriend())
        {
            fprintf(stderr, "loadFriend error!\n");
            std::abort();
        }

        if(!loadItem())
        {
            fprintf(stderr, "loadItem error!\n");
            std::abort();
        }
        if(!loadClan())
        {
            fprintf(stderr, "loadClan error!\n");
            std::abort();
        }
        if(!loadClanPlayer())
        {
            fprintf(stderr, "loadClanPlayer error!\n");
            std::abort();
        }
        if(!loadMail())
        {
            fprintf(stderr, "loadMail error!\n");
            std::abort();
        }
        if(!loadClanBattleRooms())
        {
            fprintf(stderr, "clanBattle Rooms error!\n");
            std::abort();
        }
        if( !loadClanBattlePos())
        {
            fprintf(stderr, "clanBattle pos error!\n");
            std::abort();
        }
        if( !loadClanBattleComment())
        {
            fprintf(stderr, "clanBattle comment error!\n");
            std::abort();
        }
        if( !loadClanBattleOrder())
        {
            fprintf(stderr, "clanBattle comment error!\n");
            std::abort();
        }
        if( !loadReport2Id())
        {
            fprintf(stderr, "load report2id error!\n");
            std::abort();
        }
        if( !loadClanBattleAllCityStatus() )
        {
            fprintf(stderr, "load city status error!\n");
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
            globalPlayerVec.push_back(pl);
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
        if(execu->Prepare("SELECT `id`,`phoneId`, `accounts` FROM `player_id` ORDER BY `id`", p2i) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            player2Id.InsertId(p2i.phoneId , p2i.accounts , p2i.id);
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
            //fgt2->GetFVar()->SetFVar(FVAR_WEAPON_ENCHANT,100);


            pl->addFighter(fgt2, false, true);
        }
        lc.finalize();
        return true;
    }
    bool GObjectManager::loadAccountPwd()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading account_pwd");
        lc.reset(1000);
        DBAccountPwd ap;
        if(execu->Prepare("SELECT `accounts`,`password` FROM `account_pwd`", ap) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            player2Id.InsertAccount(ap.accounts,ap.password);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadFriend()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Friend");
        lc.reset(1000);
        DBFriend ap;
        if(execu->Prepare("SELECT `type`,`playerId`,`friendId` FROM `friends`", ap) != DB::DB_OK)
            return false;
        IDTYPE last_id = 0;
        Player* pl = NULL;
        while(execu->Next() == DB::DB_OK)
        {
            if(ap.playerId != last_id)
            {
                last_id = ap.playerId;
                pl = globalPlayers[last_id];
            }
            if(pl == NULL) continue;
            Player* friendOne = globalPlayers[ap.friendId];
            if(!friendOne)
                continue ;
            pl->GetFriendManager()->AddFriend(ap.type,friendOne);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadItem()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Item");
        lc.reset(1000);
        DBItem item;
        if(execu->Prepare("SELECT `playerId`,`itemId`,`count` FROM `item`", item) != DB::DB_OK)
            return false;
        Player* pl = NULL;
        while(execu->Next() == DB::DB_OK)
        {
            //if(ap.playerId != last_id)
            {
                pl = globalPlayers[item.playerId];
            }
            if(pl == NULL) continue;
            pl->GetPackage()->AddItemFromDB(item.itemId, item.count);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadClan()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Clan");
        lc.reset(1000);
        DBClan clan;
        if(execu->Prepare("SELECT `clanId`,`name`,`picIndex`,`announcement`,`announcement2`,`creater`,`leader`,`level`,`contribute`,`personMax` ,`battleRoomId` ,`clanFame`,`conquests` ,`forceId` FROM `clan`", clan) != DB::DB_OK)
            return false;
        Player* creater = NULL;
        Player* leader = NULL;
        while(execu->Next() == DB::DB_OK)
        {
            //if(ap.playerId != last_id)
            creater = globalPlayers[clan.creater];
            if(!creater)
                continue;
            Clan * pclan = new Clan(clan.clanId, clan.name, creater);

            leader = globalPlayers[clan.leader];
            if(!leader)
                continue;
            pclan->LoadClanInfo(leader, clan.announcement,clan.announcement2, clan.personMax);
            pclan->SetLevel(clan.level);
            pclan->SetPicIndex(clan.picIndex);
            pclan->SetClanBattleRoomId(clan.battleRoomId);
            pclan->SetClanFame(clan.clanFame);
            pclan->SetConquests(clan.conquests);
            pclan->SetBattleForceId(clan.forceId);
            globalClan.add(clan.clanId, pclan);

            globalNamedClans.add(pclan->GetName(), pclan);
            
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadClanPlayer()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Clan");
        lc.reset(1000);
        DBClanPlayer clanp;
        if(execu->Prepare("SELECT `clanId`,`playerId`,`position`,`contribute`,`enterTime`,`isClanBattle` FROM `clan_player`", clanp) != DB::DB_OK)
            return false;
        Clan * clan = NULL;
        UInt32 last_clanId = 0;
        while(execu->Next() == DB::DB_OK)
        {
            //if(ap.playerId != last_id)
            if(last_clanId != clanp.clanId)
                clan = globalClan[clanp.clanId];
            if(!clan)
                continue;
            Player* player = globalPlayers[clanp.playerId];
            clan->LoadPlayer(player);
            player->SetClanPos(clanp.position);
            player->SetVar(VAR_CLAN_CONT, clanp.contribute, 1);
            player->SetVar(VAR_CLAN_ENTER, clanp.enterTime, 1);
            player->SetJoinClanBattle(clanp.isClanBattle);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadMail()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Mail");
        lc.reset(1000);
        DBMail mailInfo;
        if(execu->Prepare("SELECT `id`,`playerId`,`contextId`,`items`,`option`,`overTime` FROM `mail`", mailInfo) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            Player* pl = globalPlayers[mailInfo.playerId];
            if(!pl)
                continue;
            Mail * mail = new Mail();
            mail->LoadMailInfo(mailInfo.id, pl, mailInfo.contextId, mailInfo.items, mailInfo.option, mailInfo.overTime);
            globalMails.add(mailInfo.id, mail);
            pl->AddMail(mailInfo.id,0);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadClanApply()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading ClanApply");
        lc.reset(1000);
        DBClanApply clanApply;
        if(execu->Prepare("SELECT `clanId`,`playerId`,`time` FROM `player_apply_clan`", clanApply) != DB::DB_OK)
            return false;
        Clan * clan = NULL;
        UInt32 last_clanId = 0;
        while(execu->Next() == DB::DB_OK)
        {
            //if(ap.playerId != last_id)
            if(last_clanId != clanApply.clanId)
                clan = globalClan[clanApply.clanId];
            if(!clan)
                continue;
            Player* player = globalPlayers[clanApply.playerId];
            if(!player)
                continue;
            clan->Apply(player,1);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadClanBattlePos()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading ClanApply");
        lc.reset(1000);
        DBClanBattlePos cbp;
        if(execu->Prepare("SELECT `mapId`,`playerId`,`fighterId`,`posx`,`posy` , `mainFighterHP`,`soldiersHP` FROM `clan_battle_pos`", cbp) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            Player* player = globalPlayers[cbp.playerId];
            if(!player)
                continue;
            Battle::battleDistribute.PutFighter(cbp.mapId,player,cbp.fighterId,cbp.posx,cbp.posy,0,1);
            std::vector<UInt32> vecSoldierHP;
            StringTokenizer st(cbp.soldiersHP,",");
            for( UInt8 i =0 ; i < st.count() ; ++i )
            {
                vecSoldierHP.push_back(::atoi(st[i].c_str()));
            }
            Battle::battleDistribute.SetMainFighterAndSoldiersHP(cbp.mapId,player,cbp.posx,cbp.posy,vecSoldierHP,cbp.mainFighterHP);
            player->InsertClanBattleFighter(cbp.mapId,cbp.fighterId,cbp.posx,cbp.posy);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadClanBattleRooms()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading ClanBattleRooms");
        lc.reset(1000);
        DBClanBattleRoom room;
        if(execu->Prepare("SELECT `roomId`,`forceId`,`battleId`,`clans`,`fighterNum`,`buildTime` FROM `clan_battle_room`", room) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            std::vector<UInt32> vecClan;
            StringTokenizer st(room.clans,",");
            for(UInt8 i = 0; i < st.count(); ++i)
            {
                vecClan.push_back(::atoi(st[i].c_str()));
            }
            Battle::clanBattleRoomManager.loadBattleRoom(room.roomId,room.forceId,room.battleId,vecClan,room.fighterNum,room.buildTime);
            lc.advance();
        }
        lc.finalize();
        return true;
    }


    bool GObjectManager::loadClanBattleComment()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading ClanBattle Comments");
        lc.reset(1000);
        DBClanBattleComment comment;
        if(execu->Prepare("SELECT `roomId`,`forceId`,`mapId`,`playerId`,`message` FROM `clan_battle_comment`", comment) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            Battle::roomCommentManager.LoadAllComments(comment.roomId,comment.forceId,comment.mapId,comment.playerId,comment.comment);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadClanBattleOrder()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading ClanBattle Order");
        lc.reset(1000);
        DBClanBattleOrder order;
        if(execu->Prepare("SELECT `roomId`,`forceId`,`mapId`,`order` FROM `clan_battle_order`", order) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            Battle::roomOrderManager.loadAllClanOrders(order.roomId,order.forceId,order.mapId,order.order);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadReport2Id()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading report2id");
        lc.reset(1000);
        DBReport2Id report2Id;
        if(execu->Prepare("SELECT `roomId`,`cityId`,`actId`,`reportId`,`time` FROM `report2id`",report2Id) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            Battle::report2IdTable.Insert(report2Id.roomId,report2Id.cityId,report2Id.actId,report2Id.reportId,report2Id.time);
            lc.advance();
        }
        lc.finalize();
        return true;
    }


    bool GObjectManager::loadClanBattleAllCityStatus()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading clanBattle all city status");
        lc.reset(1000);
        DBCityStatus cityStatus;
        if(execu->Prepare("SELECT `roomId`,`battleId`,`cityId`,`ownforce` FROM `clan_battle_citystatus`",cityStatus) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            Battle::roomAllCityStatusManager.loadCityStatus(cityStatus.roomId,cityStatus.battleId,cityStatus.cityId,cityStatus.ownforce);
            lc.advance();
        }
        lc.finalize();
        return true;
    }
    //关于equipment的提取，
    /*
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
       */
}
