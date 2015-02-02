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

        if (!LoadItemTypeData())  
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

        if (!LoadFighterBase())  
        {
            fprintf(stderr, "Load LoadFighterBase Error !\n");
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

    bool GDataManager::LoadItemTypeData()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;
        DBItemType idt;
        if(execu->Prepare("SELECT `id`, `name`, `subClass`, `career`, `reqLev`, `vLev`, `coin`, `quality`, `maxQuantity`, `bindType`, `energy`, `trumpExp`, `data`, `enchant`, `attrId`, `salePriceUp` FROM `item_template`", idt) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            if(idt.typeId == 0)
                continue;

            ItemBaseType * wt;
            switch(idt.subClass)
            {
                case Item_Weapon:
                    {
                        wt = new ItemWeaponType(idt.typeId, idt.name, idt.attrExtra);
                        if (IsEquipId(idt.typeId))
                        {
                            GObject::ItemEquipData ied;
                            npcWeapons.add(new GObject::ItemWeapon(idt.typeId, wt, ied));
                        }
                    }
                    break;
                case Item_Armor1:
                case Item_Armor2:
                case Item_Armor3:
                case Item_Armor4:
                case Item_Armor5:
                    {
                        wt = new ItemEquipType(idt.typeId, idt.name, idt.attrExtra);
                    }
                default:
                    {
                        wt = new ItemNormalType(idt.typeId, idt.name);
                    }
                    break;
            }
            wt->subClass = static_cast<ItemClass>(idt.subClass);
            wt->price = idt.coin;
            wt->reqLev = idt.reqLev;
            wt->vLev = idt.vLev;
            wt->quality = idt.quality;
            wt->maxQuantity = idt.maxQuantity;
            wt->bindType = idt.bindType;
            wt->energy = idt.energy;
            wt->trumpExp = idt.trumpExp;
            wt->data = idt.data;
            wt->career = idt.career;
            wt->salePriceUp = idt.salePriceUp;
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
        if(execu->Prepare("SELECT `id`,`name`,`skillType`,`damage`,`damageP`,`trerapy`,`trerapyP` FROM `skillEffect`", dbskeffect) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {    
            SkillEffect* se = new SkillEffect(dbskeffect.id, dbskeffect.name);
            if(!se)
                return false;
            se->skillType = dbskeffect.skillType;
            //se->actionCd = dbskeffect.actionCd;
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
        if(execu->Prepare("SELECT `id`,`name`,`color`,`typeId`,`childType`,`bodySize`,`attack`,`defend`,`magatk`,`magdef`,`critical`,`evade` FROM `fighter_base`", dbfb) != DB::DB_OK)
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
            fgt->SetRad(dbfb.bodySize);
            fgt->SetBaseAttr(dbfb.attack,dbfb.defend,dbfb.magatk,dbfb.magdef,dbfb.critical,dbfb.evade);
            //fgt->setSkills(dbfb.skill);
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
        if(execu->Prepare("SELECT `id`,`name`,`skillCondId`,`skillScopeId`,`skillEffectId`,`cd`,`actionCostCd`,`actionBackCd` FROM `skill`", dbskill) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            //skill[dbskill].LoadSkill(dbskill.skillCondId, dbskill.skillScopeId, dbskill.skillEffectId);
            Skill* se = new Skill(dbskill.id, dbskill.name);
            se->LoadSkill(dbskill.skillCondId, dbskill.skillScopeId, dbskill.skillEffectId, dbskill.cd, dbskill.actionCd,dbskill.actionBackCd);

            skillManager.add(se);
        }
        return true;
    } 
}


