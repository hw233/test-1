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

        if (!LoadMapInfo())  
        {
            fprintf(stderr, "Load LoadMapInfo Error !\n");
            std::abort();
        }
        if (!LoadMonster())  
        {
            fprintf(stderr, "Load LoadMonster Error !\n");
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
        if(execu->Prepare("SELECT `id`,`name`,`skillType`,buffId,`damage`,`damageP`,`trerapy`,`trerapyP` FROM `skillEffect`", dbskeffect) != DB::DB_OK)
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
        if(execu->Prepare("SELECT `id`,`name`,`skillCondId`,`skillScopeId`,`skillEffectId`,`cd`,`actionBeforeCd`,`actionCostCd`,`actionBackCd`,`mpCost` FROM `skill`", dbskill) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            //skill[dbskill].LoadSkill(dbskill.skillCondId, dbskill.skillScopeId, dbskill.skillEffectId);
            Skill* se = new Skill(dbskill.id, dbskill.name);
            se->LoadSkill(dbskill.skillCondId, dbskill.skillScopeId, dbskill.skillEffectId, dbskill.cd, dbskill.actionBeforeCd, dbskill.actionCd,dbskill.actionBackCd, dbskill.mpCost);

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

            sb->SetCount(dbbuff.count);
            sb->SetSide(dbbuff.side);
            sb->SetType(dbbuff.type);
            skillBuffManager.add(sb);
        }
        return true;
    } 

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
            GObject::Monster* mon = new GObject::Monster(monsterInfo.id,monsterInfo.groupId,monsterInfo.name,monsterInfo.power,monsterInfo.money,monsterInfo.prob,monsterInfo.itemId,monsterInfo.itemNum);
            GObject::monsterTable.InsertMonster(mon);
            lc.advance();
        }
        lc.finalize();
        return true;
    }
}


