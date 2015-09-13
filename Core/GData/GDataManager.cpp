#include "Config.h" 
#include "GDataManager.h"
#include "DB/DBConnectionMgr.h"
#include "GDataDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/ServerTypes.h"
#include "Server/Cfg.h"
#include "Common/StringTokenizer.h"
#include "Common/URandom.h"
#include "GData/ItemType.h"
#include "GObject/Item.h"
#include "GData/ExpTable.h"
#include "GData/SkillTable.h"
#include "GObject/Fighter.h"
#include "GData/Map.h"
#include "GObject/Monster.h"
#include "Common/LoadingCounter.h"
#include "Script/lua_tinker.h"
#include "Server/Cfg.h"
#include "Server/WorldServer.h"
#include "GData/BattleAward.h"
#include "GData/ClanBattleBase.h"
#include "GData/BattleMap.h"
#include "GData/ExploitTable.h"
#include "GData/Robot.h"
#include "GData/GlobalPVPName.h"
#include "GData/VipTable.h"

namespace GData
{
    //静态成员申明区
    ItemBaseTypeManager     itemBaseTypeManager;
    ItemBaseTypeNameManager itemBaseTypeNameManager;
    ObjectMapT<GObject::ItemWeapon> npcWeapons;

    bool GDataManager::LoadAllData()
    {
        if (!LoadExpData())  
        {
            fprintf(stderr, "Load LoadExpData Error !\n");
            std::abort();
        }
        
        
        if (!LoadFighterBase())  
        {
            fprintf(stderr, "Load LoadFighterBase Error !\n");
            std::abort();
        }
       
        if (!LoadItemTypeData2())  
        {
            fprintf(stderr, "Load ItemTypeData Error !\n");
            std::abort();
        }

        if (!LoadSkillConditionData())  
        {
            fprintf(stderr, "Load LoadSkillConditionData Error !\n");
            std::abort();
        }

        if (!LoadSkillScopeData())  
        {
            fprintf(stderr, "Load LoadSkillScopeData Error !\n");
            std::abort();
        }
        if (!LoadSkillEffectData())  
        {
            fprintf(stderr, "Load LoadSkillEffectData Error !\n");
            std::abort();
        }

        if (!LoadSkillData())  
        {
            fprintf(stderr, "Load LoadSkillData Error !\n");
            std::abort();
        }


        if (!LoadSkillBuff())  
        {
            fprintf(stderr, "Load LoadSkillBuff Error !\n");
            std::abort();
        }

        /*if (!LoadMapInfo())  
        {
            fprintf(stderr, "Load LoadMapInfo Error !\n");
            std::abort();
        }*/
        if (!LoadMonster())  
        {
            fprintf(stderr, "Load Monster Error !\n");
            std::abort();
        }
        if( ! LoadMapConfig() )
        {
            fprintf(stderr, "Load LoadMapInfo Error !\n");
            std::abort();
        }
        if (!LoadBattleAwardData())  
        {
            fprintf(stderr, "Load BattleAward Error !\n");
            std::abort();
        }
        if( !LoadClanBattleBase())
        {
            fprintf(stderr, "Load ClanBattleBase Error !\n");
            std::abort();
        }
        if( !LoadBattleMap() )
        {
            fprintf(stderr, "Load BattleMap Error !\n");
            std::abort();
        }
        if( !LoadExploitPointInfo())
        {
            fprintf(stderr, "Load ExploitPointInfo Error !\n");
            std::abort();
        }

        if( !LoadGlobalRobot())
        {
            fprintf(stderr, "Load Global Robot Error !\n");
            std::abort();
        }

        if( !LoadGlobalPVPName())
        {
            fprintf(stderr, "Load Global PVP Name Error !\n");
            std::abort();
        }

        if( !LoadPlayerVip() )
        {
            fprintf(stderr, "Load player vip Error !\n");
            std::abort();
        }
        return true;
    }
    bool GDataManager::LoadExpData()
    {    
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBExp dbexp;
        if(execu->Prepare("SELECT `lvl`, `exp` FROM `lvl_exp`", dbexp) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            expTable.setTable(dbexp.lvl, dbexp.exp);
        }    
#ifdef _DEBUG
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(22159159243-1 ));
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(21501576053-1 ));
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(20856473348-1 ));
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(14551108403-1 ));
#endif
        return true;
    }

    bool GDataManager::LoadItemTypeData2()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBItemType2 idt;
        //if(execu->Prepare("SELECT `id`, `name`, `subClass`, `career`, `reqLev`, `vLev`, `coin`, `quality`, `maxQuantity`, `bindType`, `energy`, `trumpExp`, `data`, `enchant`, `attrId`, `salePriceUp` FROM `item_template`", idt) != DB::DB_OK)
        //if(execu->Prepare("SELECT `id`, `name`, `subClass`,`maxQuantity`, `coin` FROM `item_template`", idt) != DB::DB_OK)
        if(execu->Prepare("SELECT `id`, `name`, `subClass`,`maxQuantity`, `coin` FROM `item_template2`", idt) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            if(idt.typeId == 0)
                continue;

            ItemBaseType * wt;
            switch(idt.subClass)
            {
                default:
                    {
                        wt = new ItemNormalType(idt.typeId, idt.name);
                    }
                    break;
            }
            wt->subClass = static_cast<ItemClass>(idt.subClass);  //物品类型
            wt->price = idt.coin;   // 出售价格
            wt->maxQuantity = idt.maxQuantity;
            //wt->bindType = idt.bindType;
            itemBaseTypeManager.add(wt);
            itemBaseTypeNameManager.add(wt->getName(), wt);
        }
        return true;
    }

    bool GDataManager::LoadSkillConditionData()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkillCondition dbskcond;
        if(execu->Prepare("SELECT `id`,`name`,`cond` ,`prob`,`distance`,`priority` FROM `skillCondition`", dbskcond) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            SkillCondition* sc = new SkillCondition(dbskcond.id, dbskcond.name);
            if(!sc)
                return false;
            sc->cond = dbskcond.cond;
            sc->prob = dbskcond.prob;
            //sc->cd = dbskcond.cd;
            sc->distance = dbskcond.distance;
            sc->priority = dbskcond.priority;
            skillConditionManager.add(sc);
        }    
        return true;
    } 

    bool GDataManager::LoadSkillScopeData()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkillScope dbskscope;
        if(execu->Prepare("SELECT `id`,`name`,`area` ,`x`,`y`,`radx`,`rady` FROM `skillScope`", dbskscope) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            SkillScope* ss = new SkillScope(dbskscope.id, dbskscope.name);
            if(!ss)
                return false;
            ss->area = dbskscope.area;
            ss->x = dbskscope.x;
            ss->y = dbskscope.y;
            ss->radx = dbskscope.radx;
            ss->rady = dbskscope.rady;

            skillScopeManager.add(ss);
        }    
        return true;
    } 

    bool GDataManager::LoadSkillEffectData()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkillEffect dbskeffect;
        if(execu->Prepare("SELECT `id`,`name`,`skillType`,buffId,`damage`,`damageP`,`trerapy`,`trerapyP`,`avoidhurt` FROM `skillEffect`", dbskeffect) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            SkillEffect* se = new SkillEffect(dbskeffect.id, dbskeffect.name);
            if(!se)
                return false;
            se->skillType = dbskeffect.skillType;
            se->buffId   = dbskeffect.buffId;
            se->damage = dbskeffect.damage;
            se->damageP = dbskeffect.damageP;
            se->trerapy = dbskeffect.trerapy;
            se->trerapyP = dbskeffect.trerapyP;
            se->avoidhurt = dbskeffect.avoidhurt;

            skillEffectManager.add(se);
        }    
        return true;
    } 

    bool GDataManager::LoadFighterBase()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBFighterBase dbfb;
        if(execu->Prepare("SELECT `id`,`name`,`color`,`typeId`,`childType`,`speed`,`bodySize`,`skills`,`hp`,`attack`,`defend`,`magatk`,`magdef`,`critical`,`criticalDef`,`hit`,`evade` FROM `fighter_base`", dbfb) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            GObject::Fighter* fgt = new GObject::Fighter(dbfb.id,NULL);
            if(!fgt)
                continue;
            fgt->SetName(dbfb.name);
            fgt->SetColor(dbfb.color);
            fgt->SetTypeId(dbfb.typeId);
            fgt->SetChildTypeId(dbfb.childType);
            fgt->SetSpeed(dbfb.speed);
            fgt->SetRad(dbfb.bodySize);
            fgt->SetBaseAttr(dbfb.hp,dbfb.attack,dbfb.defend,dbfb.magatk,dbfb.magdef,dbfb.critical, dbfb.criticalDef, dbfb.hit, dbfb.evade);

            fgt->SetSkill(dbfb.skills);
            GObject::globalFighters.add(fgt);
        }    

        DBFighterBaseSkill dbfbs;
        if(execu->Prepare("SELECT `id`,`levelLimit`,`skillId` FROM`fighter_base_skill`", dbfbs) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            GObject::Fighter* fgt = GObject::globalFighters[dbfbs.id];
            if(!fgt)
                continue;
            fgt->SetBaseSkill(dbfbs.levelLimit,dbfbs.skillId);
        }  

        return true;
    } 

    bool GDataManager::LoadSkillData()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkill dbskill;
        if(execu->Prepare("SELECT `id`,`name`,`skillCondId`,`skillScopeId`,`skillEffectId`,`cd`,`actionBeforeCd`,`actionCostCd`,`actionBackCd`,`frozeTime`,`mpCost`,`superSkill`,`attackCount`,`lstTime` FROM `skill`", dbskill) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            //skill[dbskill].LoadSkill(dbskill.skillCondId, dbskill.skillScopeId, dbskill.skillEffectId);
            Skill* se = new Skill(dbskill.id, dbskill.name);
            se->LoadSkill(dbskill.skillCondId, dbskill.skillScopeId, dbskill.skillEffectId, dbskill.cd*100, dbskill.actionBeforeCd*100, dbskill.actionCd*100, dbskill.actionBackCd * 100,dbskill.frozeTime * 100, dbskill.mpCost, dbskill.superSkill, dbskill.attackCount, dbskill.lstTime * 100);

            skillManager.add(se);
        }
        return true;
    } 

    bool GDataManager::LoadSkillBuff()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkillBuff dbbuff;
        if(execu->Prepare("SELECT `id`,`name`,`attrId`,`valueP`,`value`,`count`,`side`,`type` FROM `skillBuff`", dbbuff) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            SkillBuff* sb = new SkillBuff(dbbuff.id, dbbuff.name);
            
            StringTokenizer st(dbbuff.attrIds,",");
            for(UInt8 i = 0; i < st.count(); ++i)
                sb->attrIds.push_back(::atoi(st[i].c_str()));

            StringTokenizer st1(dbbuff.valueP,",");
            for(UInt8 i = 0; i < st1.count(); ++i)
                sb->valueP.push_back(::atoi(st1[i].c_str()));

            StringTokenizer st2(dbbuff.value,",");
            for(UInt8 i = 0; i < st2.count(); ++i)
                sb->value.push_back(::atoi(st2[i].c_str()));

            sb->SetCount(dbbuff.count * 100);
            sb->SetSide(dbbuff.side);
            sb->SetType(dbbuff.type);
            skillBuffManager.add(sb);
        }
        return true;
    } 
    
    /*
    bool GDataManager::LoadMapInfo()
    { 
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBMap dbmap;
        if(execu->Prepare("SELECT `id`,`floor`,`info` FROM `map`", dbmap) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            map.loadMapInfo(dbmap.id , dbmap.info);
        }
        return true;
    } 
    */


    bool GDataManager::LoadMonster()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Monster");
        lc.reset(1000);
        DBMonster monsterInfo;
        if(execu->Prepare("SELECT `id`,`groupId`,`name`,`power`,`money`,`prob`,`itemId`,`itemNum` FROM `monster`", monsterInfo) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            GObject::Monster* mon = new(std::nothrow) GObject::Monster(monsterInfo.id,monsterInfo.groupId,monsterInfo.name,monsterInfo.power,monsterInfo.money,monsterInfo.prob,monsterInfo.itemId,monsterInfo.itemNum);
            if( mon == NULL )
                return false;
            GObject::monsterTable.InsertMonster(mon);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GDataManager::LoadMapConfig()
    {
        lua_State * L = lua_open();
        vecInfo tileInfo;
        vecInfo campInfo;
        luaL_openlibs(L);
        {
            if(!cfg.scriptPath.empty())
            {
                lua_tinker::table pkg = lua_tinker::get<lua_tinker::table>(L, "package");
                const char * path = pkg.get<const char *>("path");
                std::string newpath = path;
                newpath = newpath + ";" + cfg.scriptPath + "?.lua" + ";" + cfg.scriptPath + "?/init.lua";
                pkg.set("path", newpath.c_str());
            }

            std::string path = cfg.scriptPath+"ParseMapInfo.lua";
            lua_tinker::dofile(L,path.c_str());
            lua_tinker::table AllTile = lua_tinker::call<lua_tinker::table>(L,"GetAllMap"); 
            lua_tinker::table AllForce = lua_tinker::call<lua_tinker::table>(L,"GetAllForce"); 
            lua_tinker::table AllForceIds = lua_tinker::call<lua_tinker::table>(L,"GetAllActForceIds");
            lua_tinker::table AllDirection = lua_tinker::call<lua_tinker::table>(L,"GetAllDirect2Force");
            lua_tinker::table AllSoldiers = lua_tinker::call<lua_tinker::table>(L,"GetAllSoldiers");

            if( AllTile.size() != AllForce.size() )
            {
                lua_close(L);
                return false;
            }
            //处理这个table
            for(UInt8 i = 0; i < AllTile.size(); ++i)
            {
                //TODO
                lua_tinker::table map = AllTile.get<lua_tinker::table>(i+1);
                UInt8 width = static_cast<UInt8>(map.get<UInt8>(1));
                UInt8 height = static_cast<UInt8>(map.get<UInt8>(2));
                for( UInt8 j = 2 ; j < map.size() ; ++j )
                {
                   tileInfo.push_back(static_cast<UInt8>(map.get<UInt8>(j+1)));
                }
                lua_tinker::table force = AllForce.get<lua_tinker::table>(i+1);
                for(UInt8 j = 0 ; j < force.size() ; ++j )
                {
                    campInfo.push_back(static_cast<UInt8>(force.get<UInt8>(j+1)));
                }
                MapInfo* info = new(std::nothrow) MapInfo(width,height);
                if( info == NULL )
                {
                    lua_close(L);
                    return false;
                }

                info->SetTileInfo(tileInfo);
                info->SetCampInfo(campInfo);

                lua_tinker::table direct = AllDirection.get<lua_tinker::table>(i+1);
                std::vector<UInt8> vecForceId;
                std::vector<UInt8> vecDirection;
                for( UInt8 j = 0; j < direct.size(); ++j )
                {
                    if( j % 2 == 0 )
                    {
                        vecForceId.push_back(static_cast<UInt8>(direct.get<UInt8>(j+1)));
                    }
                    else
                    {
                        vecDirection.push_back(static_cast<UInt8>(direct.get<UInt8>(j+1)));
                    }
                }
                for(UInt8 j = 0 ; j < vecForceId.size(); ++j)
                {
                    info->InsertCampDir(vecForceId[j],vecDirection[j]);
                }

                lua_tinker::table soldiers = AllSoldiers.get<lua_tinker::table>(i+1);
                std::vector<NpcInfo> vecNpcInfo;
                for( UInt8 j = 0 ; j < soldiers.size() ; ++j )
                {
                    lua_tinker::table npcInfo = soldiers.get<lua_tinker::table>(j+1);
                    UInt16 npcId = npcInfo.get<UInt16>(1);
                    UInt8  x     = npcInfo.get<UInt8>(2);
                    UInt8  y     = npcInfo.get<UInt8>(3);
                    vecNpcInfo.push_back(NpcInfo(npcId,x,y));
                }

                lua_tinker::table forceIds = AllForceIds.get<lua_tinker::table>(i+1);
                std::vector<UInt8> vecCamp;
                for( UInt8 j = 0 ; j < forceIds.size(); ++j)
                {
                    vecCamp.push_back(static_cast<UInt8>(forceIds.get<UInt8>(j+1)));

                }
                info->SetNpcInfo(vecNpcInfo);
                info->SetActCamp(vecCamp);
                GData::mapTable.loadMapInfo(i+1,info);  //第0位不存数据
                tileInfo.clear();
                campInfo.clear();
                vecNpcInfo.clear();
                vecCamp.clear();

            }
        }
        lua_close(L);
        return true;
    }


    bool GDataManager::LoadBattleAwardData()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading BattleAward");
        lc.reset(1000);
        DBBattleAward AwardInfo;
        if(execu->Prepare("SELECT `mapId`,`exp`,`moneyNum`,`itemIds`,`itemNums` FROM `battleAward`", AwardInfo) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            GData::BattleAward* award = new(std::nothrow) GData::BattleAward(AwardInfo.mapId,AwardInfo.exp,AwardInfo.moneyNum);
            if( award == NULL )
                return false;
            std::vector<UInt32> vecId;
            std::vector<UInt32> vecNum;
            StringTokenizer st(AwardInfo.itemIds,",");
            for(UInt8 i = 0; i < st.count(); ++i)
                vecId.push_back(::atoi(st[i].c_str()));

            StringTokenizer st1(AwardInfo.itemNums,",");
            for(UInt8 i = 0; i < st1.count(); ++i)
                vecNum.push_back(::atoi(st[i].c_str()));

            if( vecId.size() != vecNum.size() )
                return false;
            std::vector<ItemInfo> vecItem;
            for(UInt8 i = 0 ; i < vecId.size() ; ++i )
            {
              vecItem.push_back(ItemInfo(vecId[i],vecNum[i]));
            }
            award->SetItems(vecItem);
            GData::battleAwardTable.InsertBattleAward(award);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GDataManager::LoadClanBattleBase()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Clan BattleBase");
        lc.reset(1000);
        DBClanBattleBase battleBase;
        if(execu->Prepare("SELECT `battleId`,`explimit`,`forcenum`,`playermin`,`playermax` FROM `corps_campaign_base`", battleBase) != DB::DB_OK)
            return false;

        while(execu->Next() == DB::DB_OK)
        {
            GData::ClanBattleBase* base = new(std::nothrow) ClanBattleBase(battleBase.battleId,battleBase.explimit,battleBase.forcenum,battleBase.playermin,battleBase.playermax);
            if( base == NULL )
                return false;
            GData::clanBattleBaseTable.InsertBase(base);
            lc.advance();
        }
        lc.finalize();
        return true;
    }


    bool GDataManager::LoadBattleMap()
    {
        lua_State * L = lua_open();
        luaL_openlibs(L);
        {
            if(!cfg.scriptPath.empty())
            {
                lua_tinker::table pkg = lua_tinker::get<lua_tinker::table>(L, "package");
                const char * path = pkg.get<const char *>("path");
                std::string newpath = path;
                newpath = newpath + ";" + cfg.scriptPath + "?.lua" + ";" + cfg.scriptPath + "?/init.lua";
                pkg.set("path", newpath.c_str());
            }

            std::string path = cfg.scriptPath+"paseCampaignMap.lua";
            lua_tinker::dofile(L,path.c_str());
            lua_tinker::table AllBattleMap = lua_tinker::call<lua_tinker::table>(L,"GetAllBattleMap"); 

            if( AllBattleMap.size() == 0 )
            {
                lua_close(L);
                return false;
            }
            //处理这个table
            std::vector<UInt8> links;
            std::vector<GData::SingleMapInfo*> mapInfo;

            for(UInt8 i = 0; i < AllBattleMap.size(); ++i)
            {
                //TODO

                lua_tinker::table battleMap = AllBattleMap.get<lua_tinker::table>(i+1);
                for(UInt8 j = 0 ; j < battleMap.size() ; ++j)
                {
                    lua_tinker::table singleMap = battleMap.get<lua_tinker::table>(j+1);
                    UInt8 mapId = singleMap.get<UInt8>(1);
                    UInt8 force = singleMap.get<UInt8>(2);
                    lua_tinker::table Links = singleMap.get<lua_tinker::table>(3);
                    for(UInt8 k = 0 ; k < Links.size() ;  ++k )
                    {
                        links.push_back(static_cast<UInt8>(Links.get<UInt8>(k+1)));
                    }
                    GData::SingleMapInfo* singleInfo = new(std::nothrow) SingleMapInfo(mapId,force);
                    singleInfo->SetLinks(links);
                    links.clear();
                    mapInfo.push_back(singleInfo);
                }
                GData::BattleMapInfo* battleMapInfo = new(std::nothrow) BattleMapInfo(i+1);
                battleMapInfo->SetSingleMapInfo(mapInfo);
                mapInfo.clear();
                GData::battleMapTable.loadBattleMap(battleMapInfo);
            }
        }
        lua_close(L);
        return true;
    }

    bool GDataManager::LoadExploitPointInfo()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading ExploitPointInfo");
        lc.reset(1000);
        DBExploitPointInfo info;
        if(execu->Prepare("SELECT `id`,`type`,`openLevel`,`ability`,`transform`,`perMin`,`perMax` FROM `exploit_point`", info) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            GData::ExploitPoint* exploitInfo = new GData::ExploitPoint(info.id,info.type,info.openLevel,info.ability,info.transform,info.perMin,info.perMax);
            if( exploitInfo == NULL )
                return false;
            GData::exploitTable.LoadExpoit(exploitInfo);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GDataManager::LoadGlobalRobot()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Global Robot");
        lc.reset(1000);
        DBGlobalRobot robot;
        if(execu->Prepare("SELECT `index`,`front`,`fgt1`,`fgt2`,`fgt3`,`fgt4`,`fgt5` FROM `robot`", robot) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            GData::robotInfo.LoadRobot(robot.index, robot.front, robot.fgt1, robot.fgt2, robot.fgt3, robot.fgt4, robot.fgt5);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GDataManager::LoadGlobalPVPName()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading Global Robot");
        lc.reset(1000);
        DBGlobalPVPName pvpName;
        if(execu->Prepare("SELECT `index`,`name` from `globalPVPName`", pvpName) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            GData::globalPVPName.LoadNames(pvpName.index, pvpName.name);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GDataManager::LoadPlayerVip()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        LoadingCounter lc("Loading player_vip");
        lc.reset(1000);
        DBPlayerVip playerVip;
        if(execu->Prepare("SELECT `level`,`needCoin`,`itemIds`,`itemNums` FROM `player_vip`", playerVip) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            GData::VipBase* base = new(std::nothrow) GData::VipBase(playerVip.level,playerVip.needCoin);
            if( base == NULL )
                return false;
            std::vector<UInt32> vecId;
            std::vector<UInt32> vecNum;
            StringTokenizer st(playerVip.itemIds,",");
            for(UInt8 i = 0; i < st.count(); ++i)
                vecId.push_back(::atoi(st[i].c_str()));

            StringTokenizer st1(playerVip.itemNums,",");
            for(UInt8 i = 0; i < st1.count(); ++i)
                vecNum.push_back(::atoi(st[i].c_str()));

            if( vecId.size() != vecNum.size() )
                return false;
            std::vector<Gift> vecGifts;
            for(UInt8 i = 0 ; i < vecId.size() ; ++i )
            {
              vecGifts.push_back(Gift(vecId[i],vecNum[i]));
            }
            base->SetGifts(vecGifts);
            GData::vipTable.InsertVipBase(base);
            lc.advance();
        }
        lc.finalize();
        return true;
    }

}


