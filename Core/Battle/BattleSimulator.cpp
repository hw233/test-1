﻿#include "Config.h"
#include "BattleSimulator.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "Script/BattleFormula.h"
#include "BattleReport.h"
#include "Server/OidGenerator.h"

#include "Common/DirectoryIterator.h"
#include "Server/Cfg.h"
#include "MsgID.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Script/GameActionLua.h"
namespace Battle
{

static UInt8 getPosition(UInt16 loc)
{
#if 0
    if(loc == 0x6FFF)
        return 6;
    if(loc == 0x7FFF)
        return 7;
    if(loc == 0x8FFF)
        return 8;
    loc >>= 8;
    if(loc > 6)
        return 3;
    if(loc > 5)
        return 1;
#endif
    UInt16 tmp = loc;
    if(loc < static_cast<UInt16>(0xF000))
        tmp = loc >> 8;

    UInt16 pos = GObject::GObjectManager::getBattleScene(tmp);
    return static_cast<UInt8>(pos);
}

BattleSimulator::BattleSimulator(UInt32 location, GObject::Player * player, const std::string& name2, UInt8 level2, bool rpt, UInt32 fr): 
    BattleField(), _id(rpt ? IDGenerator::gBattleOidGenerator.ID() : 0), _winner(0), _turns(0), _report(rpt), _fake_turns(fr), _formula(Script::BattleFormula::getCurrent())
{
    _position = getPosition(location);
    player->testBattlePunish();
    _player[0] = player;
    _player[1] = NULL;
    _other_name = name2;
    _other_level = level2;
    _portrait[0] = 0;
    _portrait[1] = 0;
    _cur_fgtlist_idx = 0;
    _activeFgt = NULL;
    memset(_cur_round_except, 0, sizeof(_cur_round_except));
    _except_count = 0;
    _hit_cnt = 0;

    for(int i = 0; i < 2; ++i)
    {
        _teams[i] = 0;
    }
    InitAttainRecord();

    {
        statusFuncTable[e_stAura] = &BattleSimulator::setStatusChange_Aura;
        statusFuncTable2[e_stAura] = &BattleSimulator::setStatusChange2_Aura;
        statusFuncTable[e_stAtk] = &BattleSimulator::setStatusChange_Atk;
        statusFuncTable2[e_stAtk] = &BattleSimulator::setStatusChange2_Atk;
        statusFuncTable[e_stDef] = &BattleSimulator::setStatusChange_Def;
        statusFuncTable2[e_stDef] = &BattleSimulator::setStatusChange2_Def;
        statusFuncTable[e_stMagAtk] = &BattleSimulator::setStatusChange_MagAtk;
        statusFuncTable2[e_stMagAtk] = &BattleSimulator::setStatusChange2_MagAtk;
        statusFuncTable[e_stMagDef] = &BattleSimulator::setStatusChange_MagDef;
        statusFuncTable2[e_stMagDef] = &BattleSimulator::setStatusChange2_MagDef;
        statusFuncTable[e_stTough] = &BattleSimulator::setStatusChange_Tough;
        statusFuncTable2[e_stTough] = &BattleSimulator::setStatusChange2_Tough;
        statusFuncTable[e_stAction] = &BattleSimulator::setStatusChange_Action;
        statusFuncTable2[e_stAction] = &BattleSimulator::setStatusChange2_Action;
        statusFuncTable[e_stEvade] = &BattleSimulator::setStatusChange_Evade;
        statusFuncTable2[e_stEvade] = &BattleSimulator::setStatusChange2_Evade;
        statusFuncTable[e_stCritical] = &BattleSimulator::setStatusChange_Critical;
        statusFuncTable2[e_stCritical] = &BattleSimulator::setStatusChange2_Critical;
        statusFuncTable[e_stPierce] = &BattleSimulator::setStatusChange_Pierce;
        statusFuncTable2[e_stPierce] = &BattleSimulator::setStatusChange2_Pierce;
        statusFuncTable[e_stCounter] = &BattleSimulator::setStatusChange_Counter;
        statusFuncTable2[e_stCounter] = &BattleSimulator::setStatusChange2_Counter;
        statusFuncTable[e_stMagRes] = &BattleSimulator::setStatusChange_MagRes;
        statusFuncTable2[e_stMagRes] = &BattleSimulator::setStatusChange2_MagRes;
        statusFuncTable[e_stCriticalDmg] = &BattleSimulator::setStatusChange_CriDmg;
        statusFuncTable2[e_stCriticalDmg] = &BattleSimulator::setStatusChange2_CriDmg;
        statusFuncTable[e_stHitRate] = &BattleSimulator::setStatusChange_HitR;
        statusFuncTable2[e_stHitRate] = &BattleSimulator::setStatusChange2_HitR;
        statusFuncTable[e_stAtkReduce] = &BattleSimulator::setStatusChange_AtkReduce;
        statusFuncTable2[e_stAtkReduce] = &BattleSimulator::setStatusChange2_AtkReduce;
        statusFuncTable[e_stMagAtkReduce] = &BattleSimulator::setStatusChange_MagAtkReduce;
        statusFuncTable2[e_stMagAtkReduce] = &BattleSimulator::setStatusChange2_MagAtkReduce;
    }
    {
        for(int i = 0; i < GData::TYPE_MAX; ++ i)
        {
            skillStrengthenTable[i] = NULL;
        }
        skillStrengthenTable[GData::TYPE_DISPERSE] = &BattleSimulator::doSkillStrengthen_disperse;
        skillStrengthenTable[GData::TYPE_DAMAG_REDUCE] = &BattleSimulator::doSkillStrengthen_reduce;
        skillStrengthenTable[GData::TYPE_DEBUF_DEFEND] = &BattleSimulator::doSkillStrengthen_debuf_defend;
        skillStrengthenTable[GData::TYPE_ATKADD] = &BattleSimulator::doSkillStrengthen_atkadd;
        skillStrengthenTable[GData::TYPE_ABSORB_ATTACK] = &BattleSimulator::doSkillStrengthen_absorbAtk;
        skillStrengthenTable[GData::TYPE_ADDMAGICATK] = &BattleSimulator::doSkillStrengthen_addMagicAtk;
        skillStrengthenTable[GData::TYPE_ABSORB_MAGATK] = &BattleSimulator::doSkillStrengthen_absorbMagAtk;
        skillStrengthenTable[GData::TYPE_BUF_THERAPY] = &BattleSimulator::doSkillStrengthen_bufTherapy;
        skillStrengthenTable[GData::TYPE_DEBUF_AURA] = &BattleSimulator::doSkillStrengthen_DebufAura;
        skillStrengthenTable[GData::TYPE_ATTACK_FRIEND] = &BattleSimulator::doSkillStrengthen_AttackFriend;
        skillStrengthenTable[GData::TYPE_BLEED_BYSKILL] = &BattleSimulator::doSkillStrengthen_BleedBySkill;
        skillStrengthenTable[GData::TYPE_SHIELD_HP] = &BattleSimulator::doSkillStrengthen_ShieldHP;
        skillStrengthenTable[GData::TYPE_FAKE_DEAD] = &BattleSimulator::doSkillStrengthen_FireFakeDead;
        skillStrengthenTable[GData::TYPE_SNEAD_RECOVER] = &BattleSimulator::doSkillStrengthen_SneakRecover;
        skillStrengthenTable[GData::TYPE_APPEND_SELF_ATTACK] = &BattleSimulator::doSkillStrengthen_SelfAttack;
        skillStrengthenTable[GData::TYPE_DMG_DEEP] = &BattleSimulator::doSkillStrengthen_DmgDeep;
        skillStrengthenTable[GData::TYPE_NINGSHI] = &BattleSimulator::doSkillStrengthen_NingShi;
        skillStrengthenTable[GData::TYPE_DEF_CHANGE] = &BattleSimulator::doSkillStrengthen_DefChange;
        skillStrengthenTable[GData::TYPE_HPP_RECOVER] = &BattleSimulator::doSkillStrengthen_HPPRecover;
    }
    {
        for(int i = 0; i < GData::e_eft_max; ++ i)
        {
            skillEffectExtraTable[i] = NULL;
        }

        skillEffectExtraTable[GData::e_eft_hide] = &BattleSimulator::doSkillEffectExtra_Hide;
        skillEffectExtraTable[GData::e_eft_rnd_fgt_buf_aura] = &BattleSimulator::doSkillEffectExtra_RndFgtBufAura;
        skillEffectExtraTable[GData::e_eft_evade100] = &BattleSimulator::doSkillEffectExtra_Evade100;
        skillEffectExtraTable[GData::e_eft_hide_summon] = &BattleSimulator::doSkillEffectExtra_HideSummon;
        skillEffectExtraTable[GData::e_eft_mark_hide_week] = &BattleSimulator::doSkillEffectExtra_MarkHideWeek;
        skillEffectExtraTable[GData::e_eft_hide_attack] = &BattleSimulator::doSkillEffectExtra_HideAttack;
        skillEffectExtraTable[GData::e_eft_selfside_dao_dmgreduce] = &BattleSimulator::doSkillEffectExtra_SelfSideDaoDmgReduce;
        skillEffectExtraTable[GData::e_eft_selfside_ru_shi_magatk] = &BattleSimulator::doSkillEffectExtra_SelfSideRuShiMagAtk;
        skillEffectExtraTable[GData::e_eft_selfside_buf_aura] = &BattleSimulator::doSkillEffectExtra_SelfSideBufAura;
        skillEffectExtraTable[GData::e_eft_hp_shield] = &BattleSimulator::doSkillEffectExtra_HpShield;
        skillEffectExtraTable[GData::e_eft_self_bleed] = &BattleSimulator::doSkillEffectExtra_SelfBleed;
        skillEffectExtraTable[GData::e_eft_random_shield] = &BattleSimulator::doSkillEffectExtra_RandomShield;
        skillEffectExtraTable[GData::e_eft_self_attack] = &BattleSimulator::doSkillEffectExtra_SelfAttack;
        skillEffectExtraTable[GData::e_eft_random_target_attack] = &BattleSimulator::doSkillEffectExtra_RandomTargetAttack;
        skillEffectExtraTable[GData::e_eft_mark_pet] = &BattleSimulator::doSkillEffectExtra_MarkPet;
        skillEffectExtraTable[GData::e_eft_atk_pet_mark_aura] = &BattleSimulator::doSkillEffectExtra_AtkPetMarkAura;
        skillEffectExtraTable[GData::e_eft_atk_pet_mark_extra_dmg] = &BattleSimulator::doSkillEffectExtra_AtkPetMarkDmg;
        skillEffectExtraTable[GData::e_eft_protect_pet_100] = &BattleSimulator::doSkillEffectExtra_ProtectPet100;
        skillEffectExtraTable[GData::e_eft_pet_atk_100] = &BattleSimulator::doSkillEffectExtra_PetAtk100;
        skillEffectExtraTable[GData::e_eft_sneak_atk] = &BattleSimulator::doSkillEffectExtra_SneakAtk;
        skillEffectExtraTable[GData::e_eft_dec_wave_dmg] = &BattleSimulator::doSkillEffectExtra_DecWaveDmg;
        skillEffectExtraTable[GData::e_eft_lingyou_atk] = &BattleSimulator::doSkillEffectExtra_LingYouAtk;
        skillEffectExtraTable[GData::e_eft_lingyou_magatk] = &BattleSimulator::doSkillEffectExtra_LingYouMagAtk;
        skillEffectExtraTable[GData::e_eft_lingyou_def] = &BattleSimulator::doSkillEffectExtra_LingYouDef;
        skillEffectExtraTable[GData::e_eft_lingyou_magdef] = &BattleSimulator::doSkillEffectExtra_LingYouMagDef;
        skillEffectExtraTable[GData::e_eft_lingshi_bleed] = &BattleSimulator::doSkillEffectExtra_LingShiBleed2;
        skillEffectExtraTable[GData::e_eft_criticaldmgreduce] = &BattleSimulator::doSkillEffectExtra_CriticalDmgReduce;
        skillEffectExtraTable[GData::e_eft_buddha_light] = &BattleSimulator::doSkillEffectExtra_BuddhaLight;
        skillEffectExtraTable[GData::e_eft_zhu_tian_bao_jian] = &BattleSimulator::doSkillEffectExtra_OtherSidePeerlessDisable;
        skillEffectExtraTable[GData::e_eft_trigger_count_max] = &BattleSimulator::doSkillEffectExtra_CheckMaxTrigger;
        skillEffectExtraTable[GData::e_eft_hp_lostp] = &BattleSimulator::doSkillEffectExtra_HpLostP;
        skillEffectExtraTable[GData::e_eft_flaw] = &BattleSimulator::doSkillEffectExtra_Flaw;
        skillEffectExtraTable[GData::e_eft_withstand] = &BattleSimulator::doSkillEffectExtra_Withstand;
        skillEffectExtraTable[GData::e_eft_ru_red_carpet] = &BattleSimulator::doSkillEffectExtra_Ru_RedCarpet;
        skillEffectExtraTable[GData::e_eft_shi_flower] = &BattleSimulator::doSkillEffectExtra_Shi_Flower;
        skillEffectExtraTable[GData::e_eft_dao_rose] = &BattleSimulator::doSkillEffectExtra_Dao_Rose;
        skillEffectExtraTable[GData::e_eft_mo_knot] = &BattleSimulator::doSkillEffectExtra_Mo_Knot;
        skillEffectExtraTable[GData::e_eft_prudent] = &BattleSimulator::doSkillEffectExtra_Prudent;
        skillEffectExtraTable[GData::e_eft_chaos_world] = &BattleSimulator::doSkillEffectExtra_ChaosWorld;
    }
}

BattleSimulator::BattleSimulator(UInt32 location, GObject::Player * player, GObject::Player * player2, bool rpt, UInt32 fr): BattleField(), _id(rpt ? IDGenerator::gBattleOidGenerator.ID() : 0), _winner(0), _turns(0), _report(rpt), _fake_turns(fr), _formula(Script::BattleFormula::getCurrent())
{
    _position = getPosition(location);
    player->testBattlePunish();
    _player[0] = player;
    _player[1] = player2;
    _other_name = player2->getName();
    _other_level = player2->GetLev();
    _portrait[0] = 0;
    _portrait[1] = 0;
    _cur_fgtlist_idx = 0;
    _activeFgt = NULL;
    memset(_cur_round_except, 0, sizeof(_cur_round_except));
    _except_count = 0;
    _hit_cnt = 0;

    for(int i = 0; i < 2; ++i)
    {
        _teams[i] = 0;
    }
    InitAttainRecord();

    {
        statusFuncTable[e_stAura] = &BattleSimulator::setStatusChange_Aura;
        statusFuncTable2[e_stAura] = &BattleSimulator::setStatusChange2_Aura;
        statusFuncTable[e_stAtk] = &BattleSimulator::setStatusChange_Atk;
        statusFuncTable2[e_stAtk] = &BattleSimulator::setStatusChange2_Atk;
        statusFuncTable[e_stDef] = &BattleSimulator::setStatusChange_Def;
        statusFuncTable2[e_stDef] = &BattleSimulator::setStatusChange2_Def;
        statusFuncTable[e_stMagAtk] = &BattleSimulator::setStatusChange_MagAtk;
        statusFuncTable2[e_stMagAtk] = &BattleSimulator::setStatusChange2_MagAtk;
        statusFuncTable[e_stMagDef] = &BattleSimulator::setStatusChange_MagDef;
        statusFuncTable2[e_stMagDef] = &BattleSimulator::setStatusChange2_MagDef;
        statusFuncTable[e_stTough] = &BattleSimulator::setStatusChange_Tough;
        statusFuncTable2[e_stTough] = &BattleSimulator::setStatusChange2_Tough;
        statusFuncTable[e_stAction] = &BattleSimulator::setStatusChange_Action;
        statusFuncTable2[e_stAction] = &BattleSimulator::setStatusChange2_Action;
        statusFuncTable[e_stEvade] = &BattleSimulator::setStatusChange_Evade;
        statusFuncTable2[e_stEvade] = &BattleSimulator::setStatusChange2_Evade;
        statusFuncTable[e_stCritical] = &BattleSimulator::setStatusChange_Critical;
        statusFuncTable2[e_stCritical] = &BattleSimulator::setStatusChange2_Critical;
        statusFuncTable[e_stPierce] = &BattleSimulator::setStatusChange_Pierce;
        statusFuncTable2[e_stPierce] = &BattleSimulator::setStatusChange2_Pierce;
        statusFuncTable[e_stCounter] = &BattleSimulator::setStatusChange_Counter;
        statusFuncTable2[e_stCounter] = &BattleSimulator::setStatusChange2_Counter;
        statusFuncTable[e_stMagRes] = &BattleSimulator::setStatusChange_MagRes;
        statusFuncTable2[e_stMagRes] = &BattleSimulator::setStatusChange2_MagRes;
        statusFuncTable[e_stCriticalDmg] = &BattleSimulator::setStatusChange_CriDmg;
        statusFuncTable2[e_stCriticalDmg] = &BattleSimulator::setStatusChange2_CriDmg;
        statusFuncTable[e_stHitRate] = &BattleSimulator::setStatusChange_HitR;
        statusFuncTable2[e_stHitRate] = &BattleSimulator::setStatusChange2_HitR;
        statusFuncTable[e_stAtkReduce] = &BattleSimulator::setStatusChange_AtkReduce;
        statusFuncTable2[e_stAtkReduce] = &BattleSimulator::setStatusChange2_AtkReduce;
        statusFuncTable[e_stMagAtkReduce] = &BattleSimulator::setStatusChange_MagAtkReduce;
        statusFuncTable2[e_stMagAtkReduce] = &BattleSimulator::setStatusChange2_MagAtkReduce;
    }
    {
        for(int i = 0; i < GData::TYPE_MAX; ++ i)
        {
            skillStrengthenTable[i] = NULL;
        }
        skillStrengthenTable[GData::TYPE_DISPERSE] = &BattleSimulator::doSkillStrengthen_disperse;
        skillStrengthenTable[GData::TYPE_DAMAG_REDUCE] = &BattleSimulator::doSkillStrengthen_reduce;
        skillStrengthenTable[GData::TYPE_DEBUF_DEFEND] = &BattleSimulator::doSkillStrengthen_debuf_defend;
        skillStrengthenTable[GData::TYPE_ATKADD] = &BattleSimulator::doSkillStrengthen_atkadd;
        skillStrengthenTable[GData::TYPE_ABSORB_ATTACK] = &BattleSimulator::doSkillStrengthen_absorbAtk;
        skillStrengthenTable[GData::TYPE_ADDMAGICATK] = &BattleSimulator::doSkillStrengthen_addMagicAtk;
        skillStrengthenTable[GData::TYPE_ABSORB_MAGATK] = &BattleSimulator::doSkillStrengthen_absorbMagAtk;
        skillStrengthenTable[GData::TYPE_BUF_THERAPY] = &BattleSimulator::doSkillStrengthen_bufTherapy;
        skillStrengthenTable[GData::TYPE_DEBUF_AURA] = &BattleSimulator::doSkillStrengthen_DebufAura;
        skillStrengthenTable[GData::TYPE_ATTACK_FRIEND] = &BattleSimulator::doSkillStrengthen_AttackFriend;
        skillStrengthenTable[GData::TYPE_BLEED_BYSKILL] = &BattleSimulator::doSkillStrengthen_BleedBySkill;
        skillStrengthenTable[GData::TYPE_SHIELD_HP] = &BattleSimulator::doSkillStrengthen_ShieldHP;
        skillStrengthenTable[GData::TYPE_FAKE_DEAD] = &BattleSimulator::doSkillStrengthen_FireFakeDead;
        skillStrengthenTable[GData::TYPE_SNEAD_RECOVER] = &BattleSimulator::doSkillStrengthen_SneakRecover;
        skillStrengthenTable[GData::TYPE_APPEND_SELF_ATTACK] = &BattleSimulator::doSkillStrengthen_SelfAttack;
        skillStrengthenTable[GData::TYPE_DMG_DEEP] = &BattleSimulator::doSkillStrengthen_DmgDeep;
        skillStrengthenTable[GData::TYPE_NINGSHI] = &BattleSimulator::doSkillStrengthen_NingShi;
        skillStrengthenTable[GData::TYPE_DEF_CHANGE] = &BattleSimulator::doSkillStrengthen_DefChange;
        skillStrengthenTable[GData::TYPE_HPP_RECOVER] = &BattleSimulator::doSkillStrengthen_HPPRecover;
    }
    {
        for(int i = 0; i < GData::e_eft_max; ++ i)
        {
            skillEffectExtraTable[i] = NULL;
        }

        skillEffectExtraTable[GData::e_eft_hide] = &BattleSimulator::doSkillEffectExtra_Hide;
        skillEffectExtraTable[GData::e_eft_rnd_fgt_buf_aura] = &BattleSimulator::doSkillEffectExtra_RndFgtBufAura;
        skillEffectExtraTable[GData::e_eft_evade100] = &BattleSimulator::doSkillEffectExtra_Evade100;
        skillEffectExtraTable[GData::e_eft_hide_summon] = &BattleSimulator::doSkillEffectExtra_HideSummon;
        skillEffectExtraTable[GData::e_eft_mark_hide_week] = &BattleSimulator::doSkillEffectExtra_MarkHideWeek;
        skillEffectExtraTable[GData::e_eft_hide_attack] = &BattleSimulator::doSkillEffectExtra_HideAttack;
        skillEffectExtraTable[GData::e_eft_selfside_dao_dmgreduce] = &BattleSimulator::doSkillEffectExtra_SelfSideDaoDmgReduce;
        skillEffectExtraTable[GData::e_eft_selfside_ru_shi_magatk] = &BattleSimulator::doSkillEffectExtra_SelfSideRuShiMagAtk;
        skillEffectExtraTable[GData::e_eft_selfside_buf_aura] = &BattleSimulator::doSkillEffectExtra_SelfSideBufAura;
        skillEffectExtraTable[GData::e_eft_hp_shield] = &BattleSimulator::doSkillEffectExtra_HpShield;
        skillEffectExtraTable[GData::e_eft_self_bleed] = &BattleSimulator::doSkillEffectExtra_SelfBleed;
        skillEffectExtraTable[GData::e_eft_random_shield] = &BattleSimulator::doSkillEffectExtra_RandomShield;
        skillEffectExtraTable[GData::e_eft_self_attack] = &BattleSimulator::doSkillEffectExtra_SelfAttack;
        skillEffectExtraTable[GData::e_eft_random_target_attack] = &BattleSimulator::doSkillEffectExtra_RandomTargetAttack;
        skillEffectExtraTable[GData::e_eft_mark_pet] = &BattleSimulator::doSkillEffectExtra_MarkPet;
        skillEffectExtraTable[GData::e_eft_atk_pet_mark_aura] = &BattleSimulator::doSkillEffectExtra_AtkPetMarkAura;
        skillEffectExtraTable[GData::e_eft_atk_pet_mark_extra_dmg] = &BattleSimulator::doSkillEffectExtra_AtkPetMarkDmg;
        skillEffectExtraTable[GData::e_eft_protect_pet_100] = &BattleSimulator::doSkillEffectExtra_ProtectPet100;
        skillEffectExtraTable[GData::e_eft_pet_atk_100] = &BattleSimulator::doSkillEffectExtra_PetAtk100;
        skillEffectExtraTable[GData::e_eft_sneak_atk] = &BattleSimulator::doSkillEffectExtra_SneakAtk;
        skillEffectExtraTable[GData::e_eft_dec_wave_dmg] = &BattleSimulator::doSkillEffectExtra_DecWaveDmg;
        skillEffectExtraTable[GData::e_eft_lingyou_atk] = &BattleSimulator::doSkillEffectExtra_LingYouAtk;
        skillEffectExtraTable[GData::e_eft_lingyou_magatk] = &BattleSimulator::doSkillEffectExtra_LingYouMagAtk;
        skillEffectExtraTable[GData::e_eft_lingyou_def] = &BattleSimulator::doSkillEffectExtra_LingYouDef;
        skillEffectExtraTable[GData::e_eft_lingyou_magdef] = &BattleSimulator::doSkillEffectExtra_LingYouMagDef;
        skillEffectExtraTable[GData::e_eft_lingshi_bleed] = &BattleSimulator::doSkillEffectExtra_LingShiBleed2;
        skillEffectExtraTable[GData::e_eft_criticaldmgreduce] = &BattleSimulator::doSkillEffectExtra_CriticalDmgReduce;
        skillEffectExtraTable[GData::e_eft_buddha_light] = &BattleSimulator::doSkillEffectExtra_BuddhaLight;
        skillEffectExtraTable[GData::e_eft_zhu_tian_bao_jian] = &BattleSimulator::doSkillEffectExtra_OtherSidePeerlessDisable;
        skillEffectExtraTable[GData::e_eft_trigger_count_max] = &BattleSimulator::doSkillEffectExtra_CheckMaxTrigger;
        skillEffectExtraTable[GData::e_eft_hp_lostp] = &BattleSimulator::doSkillEffectExtra_HpLostP;
        skillEffectExtraTable[GData::e_eft_flaw] = &BattleSimulator::doSkillEffectExtra_Flaw;
        skillEffectExtraTable[GData::e_eft_withstand] = &BattleSimulator::doSkillEffectExtra_Withstand;
        skillEffectExtraTable[GData::e_eft_ru_red_carpet] = &BattleSimulator::doSkillEffectExtra_Ru_RedCarpet;
        skillEffectExtraTable[GData::e_eft_shi_flower] = &BattleSimulator::doSkillEffectExtra_Shi_Flower;
        skillEffectExtraTable[GData::e_eft_dao_rose] = &BattleSimulator::doSkillEffectExtra_Dao_Rose;
        skillEffectExtraTable[GData::e_eft_mo_knot] = &BattleSimulator::doSkillEffectExtra_Mo_Knot;
        skillEffectExtraTable[GData::e_eft_prudent] = &BattleSimulator::doSkillEffectExtra_Prudent;
        skillEffectExtraTable[GData::e_eft_chaos_world] = &BattleSimulator::doSkillEffectExtra_ChaosWorld;
    }
}

void BattleSimulator::switchPlayer(GObject::Player* player, UInt8 side)
{
    if(side > 1)
        return;

    _player[side] = player;
}

void BattleSimulator::switchPlayer(const std::string& name, UInt8 level)
{
    _other_name = name;
    _other_level = level;
}

void BattleSimulator::putTeams(const std::string& name, UInt8 level, UInt16 portrait, UInt8 side)
{
    if(side > 1)
        return;
    UInt8 idx = _teams[side];
    if(idx > 1)
        return;
    ++_teams[side];
    _team_name[side].push_back(name);
    _team_level[side].push_back(level);
    _team_portrait[side].push_back(portrait);
}

UInt32 BattleSimulator::clearLastBattle(UInt8 side, bool isLast)
{
    if(side > 1)
        return 0;

    if(_id == 0)
    {
        _id = IDGenerator::gBattleOidGenerator.ID();
        _packet.data<UInt32>(4) = _id;
    }

    UInt32 oldID = _id;
    if(!isLast)
    {
        _id = IDGenerator::gBattleOidGenerator.ID();
        _packet.data<UInt32>(8) = _id;
    }

    battleReport.addReport(oldID, _packet);

    if(!isLast)
    {
        for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; ++fgtlist_idx)
        {
            std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
            size_t c = fgtlist.size();
            for(size_t i = 0; i < c;)
            {
                if(fgtlist[i]->getSide() == side)
                {
                    fgtlist.erase(fgtlist.begin() + i);
                    -- c;
                }
                else
                    ++ i;
            }
        }

        size_t idx = 0;
        for(idx = 0; idx < _onTherapy.size(); ++ idx)
        {
            if(_onTherapy[idx]->getSide() == side)
            {
                _onTherapy.erase(_onTherapy.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onSkillDmg.size(); ++ idx)
        {
            if(_onSkillDmg[idx]->getSide() == side)
            {
                _onSkillDmg.erase(_onSkillDmg.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onOtherDead.size(); ++ idx)
        {
            if(_onOtherDead[idx]->getSide() == side)
            {
                _onOtherDead.erase(_onOtherDead.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onPetProtect.size(); ++ idx)
        {
            if(_onPetProtect[idx]->getSide() == side)
            {
                _onPetProtect.erase(_onPetProtect.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onPetAtk.size(); ++ idx)
        {
            if(_onPetAtk[idx]->getSide() == side)
            {
                _onPetAtk.erase(_onPetAtk.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onOtherConfuseAndForget.size(); ++ idx)
        {
            if(_onOtherConfuseAndForget[idx]->getSide() == side)
            {
                _onOtherConfuseAndForget.erase(_onOtherConfuseAndForget.begin() + idx);
                break;
            }
        }
        _onOtherConfuseAndForgetAtkList[0].clear();
        _onOtherConfuseAndForgetAtkList[1].clear();


        for(int i = 0; i < 25; ++ i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
            if(bo)
                bo->clearDmgNingShi();

            if(getObject(side, i) && !_isBody[side][i])
            {
                deleteObject(side, i);
            }
        }
    }

    _teams[side] = 0;
    _team_name[side].clear();
    _team_level[side].clear();
    _team_portrait[side].clear();

    return oldID;
}

void  BattleSimulator::InitAttainRecord()
{
    for (UInt8 i = 0 ; i< 2; i++)
    {
        _evadeNum[i] = 0;
        _csNum[i] = 0;
        _prNum[i] = 0;
        _fjNum[i] = 0;

        _maxEvade [i] = 0;

         _maxCS[i] = 0;

         _maxPR[i] = 0;
         _maxFJ[i] = 0;
         _maxSkillDmg[i] = 0;
         _maxPeerLessDmg[i] = 0;
         _maxAura[i] = 0;
         _maxCSFactor[i] = 0.0f;

         _attackRound = 0;
         _firstPLDmg[0] = false;
         _getDamageSkillCount[i] = 0;
       /* _skillDmg300[i] = false;
        _skillDmg1k[i] = false;
        _skillDmg5k[i] = false;
        _peerlessDmg1k[i] = false;
        _peerlessDmg5k[i] = false;
        _peerlessDmg1w[i] = false;*/
    }
}
// #############################
// change the battle queue but not pass client check
// prevWin for 组队
void BattleSimulator::start(UInt8 prevWin, bool checkEnh)
{
    if(prevWin != 0xFF && (prevWin == 0 || prevWin > 2))
        return;
    _packet.clear();
    if(prevWin == 0xFF)
    {
        _fgtlist[0].clear();
        _fgtlist[1].clear();
    }

    // [[ Make packet header data
    _packet.init(REP::FIGHT_START);
    _packet << _id << static_cast<UInt32>(0) << _position;
    for(int i = 0; i < 2; ++ i)
        _packet << (_formation[i] ? _formation[i]->getId() : static_cast<UInt16>(0));
    _packet << _player[0]->GetLev() << _other_level;
    _packet << _portrait[0] << _portrait[1];

    _packet << _player[0]->getName() << _other_name;

    //组队
    for(int tidx = 0; tidx < 2; ++tidx)
    {
        _packet << _teams[tidx];
        for(int idx = 0; idx < _teams[tidx]; ++idx)
        {
            _packet << _team_name[tidx][idx] << _team_level[tidx][idx] << _team_portrait[tidx][idx];
        }
    }

    size_t offset = _packet.size();

    _packet << static_cast<UInt16>(0);

    // for the insertFighterStatus can only insert to next fgtlist, so to make sure to insert fighter status to _figtlist[0] and set the _cur_fgtlist to 1
    UInt8 cur_idx = _cur_fgtlist_idx;
    _cur_fgtlist_idx = (cur_idx + 1)%2;
    UInt8 cnt[2] = {0, 0};
    bool loaded[2] = {false, false};
    //bool checkEnh = _player[1] == NULL;
    //bool checkEnh = true; // XXX: 对怪对人都有用
    UInt32 now = TimeUtil::Now();
    for(int i = 0; i < 2; ++ i)
    {
        UInt32 flag = 0;
        UInt32 flag2 = 0;
        if(_player[i] != NULL)
        {
            if(checkEnh)
            {
                // 检查是否有环境buff加成
                if(_player[i]->getBuffData(PLAYER_BUFF_ATTR3, now) > 0)
                    flag = 0x0C;
                else if(_player[i]->getBuffData(PLAYER_BUFF_ATTR2, now) > 0)
                    flag = 0x08;
                else if(_player[i]->getBuffData(PLAYER_BUFF_ATTR1, now) > 0)
                    flag = 0x04;
                if(_player[i]->getBuffData(PLAYER_BUFF_CLANRCENHANCE, now) > 0)
                {
                    flag |= BattleFighter::Enh3;
                }
                if (_player[i]->getBuffData(PLAYER_BUFF_YBUF, now) > 0)
                    flag |= BattleFighter::Enh4;
                if (_player[i]->getBuffData(PLAYER_BUFF_BBUF, now) > 0)
                    flag |= BattleFighter::Enh5;
                if (_player[i]->getBuffData(PLAYER_BUFF_QQVIPBUF, now) > 0)
                    flag |= BattleFighter::Enh6;
                if (_player[i]->getBuffData(PLAYER_BUFF_JOYBUFF, now) > 0)
                {
                    flag |= BattleFighter::Enh7;
                }
                /*if(_player[i]->hasFlag(GObject::Player::Copy)
                 || _player[i]->hasFlag(GObject::Player::AutoCopy)
                 || _player[i]->hasFlag(GObject::Player::InCopyTeam)
                 || _player[i]->hasFlag(GObject::Player::AutoDungeon)
                 || _player[i]->hasFlag(GObject::Player::AutoFrontMap))*/
                if(_player[i]->hasFlag(GObject::Player::AthleticsBuff))
                {
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL1, now) > 0)
                        flag2 |= BattleFighter::AthlEnh1;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL2, now) > 0)
                        flag2 |= BattleFighter::AthlEnh2;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL3, now) > 0)
                        flag2 |= BattleFighter::AthlEnh3;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL4, now) > 0)
                        flag2 |= BattleFighter::AthlEnh4;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL5, now) > 0)
                        flag2 |= BattleFighter::AthlEnh5;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL6, now) > 0)
                        flag2 |= BattleFighter::AthlEnh6;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL7, now) > 0)
                        flag2 |= BattleFighter::AthlEnh7;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL8, now) > 0)
                        flag2 |= BattleFighter::AthlEnh8;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL9, now) > 0)
                        flag2 |= BattleFighter::AthlEnh9;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL11, now) > 0)
                        flag2 |= BattleFighter::AthlEnh31;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL22, now) > 0)
                        flag2 |= BattleFighter::AthlEnh32;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL33, now) > 0)
                        flag2 |= BattleFighter::AthlEnh33;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL44, now) > 0)
                        flag2 |= BattleFighter::AthlEnh34;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL55, now) > 0)
                        flag2 |= BattleFighter::AthlEnh35;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL66, now) > 0)
                        flag2 |= BattleFighter::AthlEnh36;
                    if (_player[i]->getBuffData(PLAYER_BUFF_ATHL77, now) > 0)
                        flag2 |= BattleFighter::AthlEnh37;
                }
                flag2 |= _player[i]->getAthlRivalBuff();
            }
        }
        for(int j = 0; j < 25; ++ j)
        {
            BattleObject * bo = getObject(i, j);
            if(bo != NULL)
            {
                if(bo->isChar())
                {
                    BattleFighter * bf = static_cast<BattleFighter *>(bo);
                    bool ismain = !_isBody[i][j];
                    UInt8 aura = 0;
                    UInt8 maxAura = 0;
                    if(ismain)
                    {
                        if(flag > 0)
                        {
                            bf->addFlag(flag);
                            if(!loaded[i])
                            {
                                cnt[i] |= (flag >> 2) << 5;
                                loaded[i] = true;
                            }
                        }
                        if(flag2 > 0)
                            bf->addFlag2(flag2);
                        if((prevWin-1) != i)
                            bf->initStats(checkEnh);
                        if(checkEnh)
                        {
                            UInt8 justice_roar = (_player[i] != NULL ? _player[i]->getJusticeRoar() : 0);
                            if(justice_roar)
                                bf->AddAura(justice_roar);
                        }

                        aura = bf->getAura();
                        maxAura = bf->getAuraMax();
                    }
                    UInt32 maxhp = bf->getMaxHP();
                    _packet << static_cast<UInt8>(j + 1) << bf->getFighter()->getBattleName();

                    UInt8 clsnsex = bf->getClassAndSex();
                    UInt16 portrait = bf->getPortrait();

                    _packet << clsnsex;
                    _packet << static_cast<UInt8>(_isBody[i][j] > 0 ? (_isBody[i][j] - 1) : bf->getFighter()->getLevel()) << portrait << static_cast<UInt8>(bf->getFlag() & 0x03);
                    _packet << static_cast<UInt8>(bf->getFighter()->getColor())
                        << static_cast<UInt32>(_isBody[i][j] ? 0 : bf->getHP()) << static_cast<UInt32>(maxhp);

                    _packet << aura << maxAura << static_cast<UInt32>(bf->getAttack()) << static_cast<UInt32>(bf->getMagAttack())
                        << static_cast<UInt32>(bf->getDefend()) << static_cast<UInt32>(bf->getMagDefend()) << static_cast<UInt16>(bf->getAction());

                    //add maxAura
                    if(maxAura > _maxAura[i])
                        _maxAura[i] = maxAura;
                    // TODO: up skills
                    _packet << static_cast<UInt16>(bf->getHitrate(NULL) * 100.0f) << static_cast<UInt16>(bf->getEvade(NULL) * 100.0f) << static_cast<UInt16>(bf->getCritical(NULL) * 100.0f) << static_cast<UInt16>(bf->getPierce(NULL) * 100.0f) << static_cast<UInt16>(bf->getCounter(NULL) * 100.0f) << static_cast<UInt16>(bf->getTough(NULL) * 100.0f) << static_cast<UInt16>(bf->getCriticalDmg() * 100.0f) << static_cast<UInt16>(bf->getMagRes(NULL) * 100.0f);
                    _packet << bf->getFighter()->getPeerlessAndLevel();
                    bf->getFighter()->getAllUpSkillAndLevel(_packet);
                    bf->getFighter()->getAllPSkillAndLevel(_packet);
                    bf->getFighter()->getAllSSAndLevel(_packet);

                    if(ismain && (prevWin-1) != i)
                    {
                        bf->postInit();
                        // FighterStatus fs(bf);
                        // Insert into action queue
                        insertFighterStatus(bf);
                    }
                }
                else
                {
                    _packet << static_cast<UInt8>(0) << static_cast<UInt8>(bo->getClass()) << static_cast<UInt8>(j + 1) << static_cast<UInt32>(bo->getHP());
                }
                ++ cnt[i];
            }
        }
        if (_backupObjs[i])
            static_cast<BattleFighter *>(_backupObjs[i])->initStats(false);
    }

    // after insert fighter status to _fgtlist[0] set the _cur_fgtlist_idx to 0
    _cur_fgtlist_idx = cur_idx;
    if(_fgtlist[_cur_fgtlist_idx].empty())
    {
        _packet.clear();
        return;
    }
    _packet.data<UInt8>(offset) = cnt[0];
    _packet.data<UInt8>(offset + 1) = cnt[1];

    //宠物技能符文
    for(int i = 0; i < 2; ++ i)
    {
        if(_backupObjs[i] && static_cast<BattleFighter *>(_backupObjs[i])->getFighter())
            static_cast<BattleFighter *>(_backupObjs[i])->getFighter()->getAllSSAndLevel(_packet);
        else
            _packet << static_cast<UInt8>(0); //技能符文个数为0
    }

    size_t cnt_pos = _packet.size();
    _packet << static_cast<UInt32>(0);
    // ]]

    UInt32 act_count = 0;
    _winner = testWinner();
    act_count += FightersEnter(prevWin);

    UInt32 oldAttackRount = _attackRound;
    if(_winner == 0)
        appendAttackRoundChange();
    while(_winner == 0 && act_count < _fake_turns)
    {
        int pos = findFirstAttacker();

        act_count += doAttack(pos);
        //_attackRound ++ ;
#ifdef _DEBUG
        char path[1024], path2[1024];
        sprintf(path, "%s0/0", cfg.reportPath.c_str());
        sprintf(path2, "%s/%u.log", path, 0);
        File rfile(path);
        rfile.createDirectories();
        FILE * f = fopen(path2, "a+");

        if(f != NULL)
        {
            char szBuf[256] = {0};
            sprintf(szBuf, "rcnt=%d\r\n", act_count);
            fwrite(szBuf, 1, strlen(szBuf), f);
            fclose(f);
        }
        
#endif
        if (oldAttackRount != _attackRound && _winner == 0)
        {
            oldAttackRount = _attackRound;
            act_count += tryPetEnter(0, e_reiatsu_round);
            act_count += tryPetEnter(1, e_reiatsu_round);
            if(_defList.size() > 0 || _scList.size() > 0)
            {
                appendToPacket(0, -1, -1, 0, 0, false, false);
                ++ act_count;
            }
            _getDamageSkillCount[0] = 0;
            _getDamageSkillCount[1] = 0;
        }
    }
    if(_winner == 0)
        _winner = testWinner2();

    _packet << static_cast<UInt8>(_winner);
    _packet.data<UInt32>(cnt_pos) = act_count;
    _packet << Stream::eos;
    //printf("Winner is: %d,  actions: %d\n", _winner, act_count);
#ifdef _DEBUG
    char path[1024], path2[1024];
    sprintf(path, "%s0/0", cfg.reportPath.c_str());
    sprintf(path2, "%s/%u.log", path, 0);
    File rfile(path);
    rfile.createDirectories();
    FILE * f = fopen(path2, "a+");

    if(f != NULL)
    {
        char szBuf[256] = {0};
        sprintf(szBuf, "act_count=%d\r\n\r\n\r\n", act_count);
        fwrite(szBuf, 1, strlen(szBuf), f);
        fclose(f);
    }
#endif

    _turns = act_count;

    if(_report || !cfg.GMCheck)
        battleReport.addReport(_id, _packet);

    CheckAttain();
}
void  BattleSimulator::SendAttainMsgToPlayer( GObject::Player* player, UInt32 id, UInt32 param)
{
#ifdef NO_ATTAINMENT
     return;
#endif
                 stAttainMsg  msg;
                 msg.attainID = id;
                 msg.param = param;
                 GameMsgHdr h(0x244,  player->getThreadId(), player, sizeof(msg));
                 GLOBAL().PushMsg(h, & msg);

}
void BattleSimulator::CheckAttain()
{

    //for(UInt8 i = 0; i < 2; i++)
    //only  attacker
        if(_player[0])
        {
            UInt32 max = _maxEvade[0];
            if( max >= 3  &&  max <= 9 &&  max  > _player[0] -> GetVar(VAR_BATTLE_MISS))
            {
                SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_MISS, max);
            }
            max = _maxCS[0];
            if( max >= 3  &&  max<= 9 && max  > _player[0] -> GetVar(VAR_BATTLE_CS))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_CS, max);
            }
            max = _maxPR[0];
            if( max >= 3  &&  max <= 9 && max  > _player[0] -> GetVar(VAR_BATTLE_PR))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_PR, max);
            }

            max =  _maxFJ[0];
            if( max >=3  &&  max <= 9 && max  > _player[0] -> GetVar(VAR_BATTLE_FJ))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_FJ, max);
            }
            max  = _maxSkillDmg[0];
            if(max >= 300  &&  max >  _player[0] -> GetVar(VAR_BATTLE_SKILL_DMG))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_SKILLDMG, max);
            }
            max  =  _maxPeerLessDmg[0];
            if(max >= 1000  &&  max >  _player[0] -> GetVar(VAR_BATTLE_PEERLESS_DMG))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_PLDMG, max);
            }
            max = _maxAura[0];
            if(max >= 200)
            {
                SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_MAX_AURA, max);
            }

            float fMax = _maxCSFactor[0];
            if(fMax >= 3.0)
            {
                SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_CSFactor, 0);
            }

            if(_firstPLDmg[0])
            {
                   SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_FIRST_PEERLESS_ATTACK, 0);
            }
        }
#if 0
        for (UInt8 i = 0 ; i < 2; i ++)
        {
            printf("miss %u\n",   _maxEvade[i]   );
            printf("%u\n" , _maxCS[i]);
            printf("%u\n" , _maxPR[i]);
            printf("%u\n" , _maxFJ[i]);
            printf("%u\n" , _maxSkillDmg[i]);
            printf("%u\n" , _maxPeerLessDmg[i]);
            printf("%u\n" , _maxAura[i]);
            printf("%f\n" , _maxCSFactor[i]);
        }
#endif
}

void BattleSimulator::insertFighterStatus2Current( BattleFighter* bf )
{
    if(bf->getHP() == 0)
        return;

    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
    int cnt = static_cast<int>(cur_fgtlist.size());
    for(int i = 0; i < cnt ; ++ i)
    {
        UInt32 bf_act = bf->getAction();
        BattleFighter* cur_bf = cur_fgtlist[i];
        UInt32 cur_act = cur_bf->getAction();
        if(cur_act < bf_act)
        {
            cur_fgtlist.insert(cur_fgtlist.begin() + i, bf);
            return;
        }
    }
    cur_fgtlist.insert(cur_fgtlist.end(), bf);
    if(_winner != 0)
        _winner = testWinner();
}
void BattleSimulator::insertFighterStatus( BattleFighter* bf )
{
    if(bf->getHP() == 0)
        return;

    Int8 next_fgtlist_idx = _cur_fgtlist_idx == 0 ? 1 : 0;
    std::vector<BattleFighter*>& next_fgtlist = _fgtlist[next_fgtlist_idx];
    int cnt = static_cast<int>(next_fgtlist.size());
    for(int i = 0; i < cnt ; ++ i)
    {
        UInt32 bf_act = bf->getAction();
        BattleFighter* next_bf = next_fgtlist[i];
        UInt32 next_act = next_bf->getAction();
        if(next_act < bf_act)
        {
            next_fgtlist.insert(next_fgtlist.begin() + i, bf);
            return;
        }
    }
    next_fgtlist.insert(next_fgtlist.end(), bf);
    if(_winner != 0)
        _winner = testWinner();
}

void BattleSimulator::removeFighterStatus( BattleFighter* bf )
{
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[fgtlist_idx];
        size_t c = cur_fgtlist.size();
        size_t i = 0;
        while(i < c)
        {
            if(cur_fgtlist[i] == bf)
            {
                cur_fgtlist.erase(cur_fgtlist.begin() + i);
                -- c;
            }
            else
                ++ i;
        }
    }
}

void BattleSimulator::reQueueFighterStatus(BattleFighter* bf)
{
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[fgtlist_idx];
        size_t c = cur_fgtlist.size();
        size_t i = 0;
        while(i < c)
        {
            if(cur_fgtlist[i] == bf)
            {
                cur_fgtlist.erase(cur_fgtlist.begin() + i);
                -- c;
                std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[fgtlist_idx];
                int cnt = static_cast<int>(cur_fgtlist.size());
                int idx = 0;
                for(; idx < cnt ; ++ idx)
                {
                    UInt32 bf_act = bf->getAction();
                    BattleFighter* cur_list_bf = cur_fgtlist[idx];
                    UInt32 cur_list_act = cur_list_bf->getAction();
                    if(cur_list_act < bf_act)
                    {
                        cur_fgtlist.insert(cur_fgtlist.begin() + idx, bf);
                        break;
                    }
                }

                if( idx == cnt )
                {
                    cur_fgtlist.insert(cur_fgtlist.end(), bf);
                }
                return;

            }
            else
                ++ i;
        }
    }
}

// Find first attacker and update action points
int BattleSimulator::findFirstAttacker()
{
    // Randomly select an attacker in that some fighters have the same action points
    if(_fgtlist[_cur_fgtlist_idx].size() == 0)
    {
        _cur_fgtlist_idx = _cur_fgtlist_idx == 0 ? 1 : 0;
        _attackRound ++ ;
        appendAttackRoundChange();
    }

    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
    size_t c = 1, cnt = cur_fgtlist.size();
    BattleFighter* bf = cur_fgtlist[0];
    UInt32 act = bf->getAction();
    while(c < cnt)
    {
        BattleFighter* bf = cur_fgtlist[c];
        UInt32 act_tmp = bf->getAction();
        if(act != act_tmp)
            break;

        ++ c;
    }

    if(c == 1)
        return 0;
    return _rnd(c);
}

GData::LBSkillItem* BattleSimulator::GetActionCondSkillItem2(BattleFighter* bf, BattleFighter* bo)
{
    GData::LBSkillItem* item = NULL;
    if(_attackRound > 0)
        return item;

    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
    size_t cnt = cur_fgtlist.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(cur_fgtlist[i] == bo)
        {
            item = bf->getActionCondItem2();
            break;
        }
    }

    return item;
}

GData::LBSkillItem* BattleSimulator::GetActionCondSkillItem1(BattleFighter* bf, BattleFighter* bo)
{
    GData::LBSkillItem* item = NULL;
    if(_attackRound > 0)
        return item;

    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
    size_t cnt = cur_fgtlist.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(cur_fgtlist[i] == bo)
        {
            item = bf->getActionCondItem1();
            break;
        }
    }

    return item;
}

// XXX: 取消救援
#if 0
float BattleSimulator::testRescue(BattleFighter *& bf, int counter_deny, AttackPoint * counter_deny_list)
{
    const GData::Formation::GridEffect * effect = bf->getFormationEffect();
    if(effect != NULL && effect->rescue_ratio > 0)
    {
        UInt8 side = bf->getSide();
        BattleObject * rescue_obj = _objs[side][effect->rescue];
        if(rescue_obj != NULL && rescue_obj->isChar() && rescue_obj->getHP() > 0)
        {
            UInt8 ratio = effect->rescue_ratio;
            BattleFighter * target_bf = static_cast<BattleFighter *>(rescue_obj);
            if(target_bf->getFighter()->isMale() != bf->getFighter()->isMale())
                ratio += 3;
            if(_rnd(100) < ratio)
            {
                if(counter_deny > 0)
                {
                    for(int i = 0; i < counter_deny; ++ i)
                    {
                        if(counter_deny_list[i].pos == effect->rescue)
                            return 0.0f;
                    }
                }
                bf = static_cast<BattleFighter *>(rescue_obj);
                return 0.1f;
            }
        }
    }
    return 0.0f;
}
#endif

#if 0
float BattleSimulator::testLink( BattleFighter *& bf, UInt16& skillId )
{
    const GData::Formation::GridEffect * effect = bf->getFormationEffect();
    if(effect != NULL && effect->link_ratio > 0)
    {
        BattleObject * link_obj = _objs[bf->getSide()][effect->link];
        if(link_obj != NULL && link_obj->isChar() && link_obj->getHP() > 0)
        {
            UInt8 ratio = effect->link_ratio;
            BattleFighter * target_bf = static_cast<BattleFighter *>(link_obj);
            if(target_bf->getFighter()->isMale() != bf->getFighter()->isMale())
                ratio += 3;
            if(_rnd(100) < ratio)
            {
                bf = target_bf;
                return 1.0f;
            }
        }
    }
    // TODO:
#if 0
    UInt32 sid = bf->getFighter()->getSkill();
    switch(sid)
    {
    case 301:
    case 302:
    case 310:
        {
            Script::BattleFormula::SkillData& sd = _formula->skillData(2, sid - 301, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                return 0;
            UInt8 myPos = bf->getPos();
            bf = getRandomFighter(bf->getSide(), &myPos, 1);
            if(bf)
            {
                skillId = sid;
                return sd.value1;
            }
            return 0.0f;
        }
    }
#endif
    return 0.0f;
}
#endif

UInt32 BattleSimulator::doSpiritAttack(BattleFighter * bf, BattleFighter* bo, float atk, bool& pr, bool& cs, bool& first)
{
    if(!bf || !bo || bf->getHP() == 0 || bo->getHP() == 0 || bo->isSoulOut())
        return 0;

    UInt32 dmg = 0;
    UInt8 target_stun = bo->getStunRound();
    bool enterEvade = bo->getEvad100();
    bool defend100 = bo->getDefend100();

    bool pr2 = false;
    bool cs2 = false;
    if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(bo, NULL) && !bo->getMoEvade100())))
    {
        pr2 = bf->calcPierce(bo);
        float rate = bf->getCritical(bo);
        if(uRand(10000) < (rate > 0 ? rate : 0) * 100)
        {
            float factor = bf->getCriticalDmg() - bo->getCriticalDmgImmune() - bo->getTough(bf);
            if(factor < 1.25)
                factor = 1.25;
            atk = atk * factor;
            cs2 = true;
        }

        float toughFactor = pr2 ? bo->getTough(bf) : 1.0f;
        float def = getBFDefend(bo);
        float atkreduce = getBFAtkReduce(bo);

        if(bo->hasFlag(BattleFighter::IsMirror))
        {
            dmg = bo->getHP();
        }
        else
        {
            atk *= (1 - bo->getDecWaveDmg());
            dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);
            dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
            dmg = dmg > 0 ? dmg : 1;
        }

        doShieldHPAttack(bo, dmg);
        if(dmg > 0)
        {
            makeDamage(bo, dmg, e_damNormal, e_damagePhysic);
        }
    }
    else if(!defend100 && !enterEvade)
    {
        appendDefStatus(e_damEvade, 0, bo);
        if(bo->getSneakStatus() == e_sneak_on)
        {
            _sneak_atker.push_back(bo);
            bo->nextSneakStatus();
        }
    }
    else
    {
        if(!defend100)
        {
            appendDefStatus(e_damEvade, 0, bo);
            bo->setEvad100(false);
            if(bo->getSneakStatus() == e_sneak_on)
            {
                _sneak_atker.push_back(bo);
                bo->nextSneakStatus();
            }
        }
        else
        {
            appendDefStatus(e_damOut, 0, bo);
            bo->setDefend100(false);
        }
    }
    if(first)
    {
        pr = pr2;
        cs = cs2;
        first = false;
    }

    return dmg;
}

void BattleSimulator::doOtherConfuseForgetAttack(BattleFighter* bf, UInt32& rcnt)
{
    if (!bf)
        return;
    BattleFighter* tmp = _activeFgt;
    _activeFgt = bf;
    int side = 1 - bf->getSide();
    bool cs = false;
    bool pr = false;
    bool first = true;
    int i = 0;
    for(int pos = 0; pos < 25; ++ pos)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
        if(!bo || bo->getHP() == 0)
            continue;
        doOtherConfuseForgetAttackOnce(bf, bo, 1.0f, pr, cs, first);
        ++ i;
    }
    if(_defList.size() > 0 || _scList.size() > 0)
    {

        UInt16 skillId = 0;
        size_t idx = 0;
        const GData::SkillBase* skill = bf->getPassiveSkillOnOtherConfuseAndForget100(idx);
        if (skill)
            skillId = skill->getId();
        appendToPacket( bf->getSide(), bf->getPos(), 0, 2, skillId, cs, pr);
        ++ rcnt;
    }
    _activeFgt = tmp;
}

UInt32 BattleSimulator::doOtherConfuseForgetAttackOnce(BattleFighter * bf, BattleFighter* bo, float atk, bool& pr, bool& cs, bool& first)
{
    if(!bf || !bo || bf->getHP() == 0 || bo->getHP() == 0 || bo->isSoulOut())
        return 0;

    UInt32 dmg = 0;
    UInt8 target_stun = bo->getStunRound();
    bool enterEvade = bo->getEvad100();
    bool defend100 = bo->getDefend100();

    bool pr2 = false;
    bool cs2 = false;
    if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(bo, NULL) && !bo->getMoEvade100())))
    {
        pr2 = bf->calcPierce(bo);
        float rate = bf->getCritical(bo);
        if(uRand(10000) < (rate > 0 ? rate : 0) * 100)
        {
            float factor = bf->getCriticalDmg() - bo->getCriticalDmgImmune() - bo->getTough(bf);
            if(factor < 1.25)
                factor = 1.25;
            atk = atk * factor;
            cs2 = true;
        }

        float toughFactor = pr2 ? bo->getTough(bf) : 1.0f;
        float def = getBFDefend(bo);
        float atkreduce = getBFAtkReduce(bo);

        if(bo->hasFlag(BattleFighter::IsMirror))
        {
            dmg = bo->getHP();
        }
        else
        {
            atk *= (1 - bo->getDecWaveDmg());
            dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);
            dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
            dmg = dmg > 0 ? dmg : 1;
        }

        doShieldHPAttack(bo, dmg);
        if(dmg > 0)
        {
            makeDamage(bo, dmg, e_damNormal, e_damageMagic);
        }
    }
    else if(!defend100 && !enterEvade)
    {
        appendDefStatus(e_damEvade, 0, bo);
        if(bo->getSneakStatus() == e_sneak_on)
        {
            _sneak_atker.push_back(bo);
            bo->nextSneakStatus();
        }
    }
    else
    {
        if(!defend100)
        {
            appendDefStatus(e_damEvade, 0, bo);
            bo->setEvad100(false);
            if(bo->getSneakStatus() == e_sneak_on)
            {
                _sneak_atker.push_back(bo);
                bo->nextSneakStatus();
            }
        }
        else
        {
            appendDefStatus(e_damOut, 0, bo);
            bo->setDefend100(false);
        }
    }
    if(first)
    {
        pr = pr2;
        cs = cs2;
        first = false;
    }

    return dmg;
}

UInt32 BattleSimulator::doXinmoAttack(BattleFighter * bf, BattleObject* bo)
{
    if(!bf || !bo || bf->getHP() == 0 || bo->getHP() == 0)
        return 0;
    UInt32 dmg = 0;

    BattleFighter * area_target = static_cast<BattleFighter *>(bo);
    UInt8 target_stun = area_target->getStunRound();
    bool enterEvade = area_target->getEvad100();
    bool defend100 = area_target->getDefend100();

    appendDefStatus(e_xinmo, 0, bf);
    bool pr = false;
    bool cs = false;
    if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(area_target, NULL))))
    {
        pr = bf->calcPierce(area_target);
        float atk = 0;
        float cf = 0.0f;
        atk = calcAttack(bf, cs, area_target, &cf);
        if(cs )
        {
            UInt8 s = bf->getSide();
            if(s < 2)
                _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

        }

        float toughFactor = pr ? area_target->getTough(bf) : 1.0f;
        float def = getBFDefend(area_target);
        float atkreduce = getBFAtkReduce(area_target);
        dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);

        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        dmg = dmg > 0 ? dmg : 1;

        doShieldHPAttack(area_target, dmg);
        if(dmg > 0)
        {
            makeDamage(area_target, dmg, e_damNormal, e_damagePhysic);
        }
    }
    else if(!defend100 && !enterEvade)
        appendDefStatus(e_damEvade, 0, area_target);
    else
    {
        if(enterEvade)
        {
            appendDefStatus(e_damEvade, 0, area_target);
            area_target->setEvad100(false);
        }

        if(defend100)
        {
            appendDefStatus(e_damOut, 0, area_target);
            area_target->setDefend100(false);
        }

        //printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
    }

    return dmg;
}

UInt32 BattleSimulator::attackOnce(BattleFighter * bf, bool& first, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * area_target_obj, float factor, int counter_deny, AttackPoint * counter_deny_list, std::vector<AttackAct>* atkAct, bool canProtect /* = false */)
{
    if(area_target_obj == NULL || area_target_obj->getHP() == 0 || bf == NULL)
        return 0;
    // if a fighter was attacked
    UInt32 dmg = 0;
    UInt32 magdmg = 0;
    bool cs2 = false;
    bool pr2 = false;
    if(area_target_obj->isChar())
    {
        bool hit = false;
        bool counter100 = false;
        bool poison = false;
        BattleFighter * area_target = static_cast<BattleFighter *>(area_target_obj);
        UInt8 side = area_target->getSide();
        UInt8 pos = area_target->getPos();
        UInt8 target_stun = area_target->getStunRound();
        bool enterEvade = area_target->getEvad100();
        bool defend100 = area_target->getDefend100();
        bool colorStock = false;
        UInt8& colorStockTimes = area_target->getColorStockTimes();
        if(colorStockTimes > 0)
        {
            -- colorStockTimes;
            colorStock = true;
        }
        if(area_target->isSoulOut())
            return 0;

        UInt8& lingquTimes = area_target->getLingQuTimes();
        if(lingquTimes > 0)
        {
            -- lingquTimes;
            colorStock = true;
        }

        GData::LBSkillItem* itemLing = NULL;
        if(first)
            itemLing = GetActionCondSkillItem2(bf, area_target);

        // 雪人
        if(area_target->getId() == 5679)
            target_stun = 1;

        GData::SkillStrengthenBase* ss = NULL;
        if(skill)
            ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
        // target fighter will do not counter while fighter is the same side
        bool can_counter = true;
        if(target_stun > 0 || bf->getSide() == area_target->getSide())
        {
            can_counter = false;
        }

        float aura_factor = 1;
        if(skill && skill->cond == GData::SKILL_PEERLESS)
        {
            aura_factor = 1 + static_cast<float>(bf->getAura()-100) * 0.0025;
            if(aura_factor < 1)
                aura_factor = 1;
            if(bf->isHide() || area_target->isMarkMo())
                aura_factor += 0.1f;
        }

        if(!colorStock && !defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(area_target, skill) && !area_target->getMoEvade100())))
        {
            // 攻击命中，没有被闪避
            hit = true;
            UInt8& deepforgetlast = area_target->getDeepForgetLast();
            if(deepforgetlast > 0 && bf->getSide() != area_target->getSide())
            {
                -- deepforgetlast;
                factor += area_target->getDeepForgetDmgExtra();
                if(deepforgetlast == 0)
                {
                    area_target->setDeepForgetDmgExtra(0, 0);
                    appendDefStatus(e_unDeepForget, 0, area_target);
                    appendDefStatus(e_Forget, 0, area_target);
                }
            }
            UInt8& deepstunlast = area_target->getDeepStunLast();
            if(deepstunlast > 0 && bf->getSide() != area_target->getSide())
            {
                -- deepstunlast;
                factor += area_target->getDeepStunDmgExtra();
                if(deepstunlast == 0)
                {
                    area_target->setDeepStunDmgExtra(0, 0);
                    appendDefStatus(e_unDeepStun, 0, area_target);
                    appendDefStatus(e_Stun, 0, area_target);
                }
            }
            UInt8& deepconfuselast = area_target->getDeepConfuseLast();
            if(deepconfuselast > 0 && bf->getSide() != area_target->getSide())
            {
                -- deepconfuselast;
                factor += area_target->getDeepConfuseDmgExtra();
                if(deepconfuselast == 0)
                {
                    area_target->setDeepConfuseDmgExtra(0, 0);
                    appendDefStatus(e_unDeepConfuse, 0, area_target);
                    appendDefStatus(e_Confuse, 0, area_target);
                }
            }
            factor += area_target->getDeepBlindDmgExtra();
            if(area_target->releaseDeepBlind())
            {
                appendDefStatus(e_unDeepBlind, 0, area_target);
                appendDefStatus(e_blind, 0, area_target);
            }
            factor += area_target->getDarkVigorFactor();

            if (bf && !bf->getStunRound() && !bf->getConfuseRound() && ! bf->getForgetRound())
            {
                // XXX: 现在只做仙宠攻击有标记的散仙加成
                const GData::SkillBase* passiveSkill = NULL;
                size_t idx = 0;
                if (area_target->isPetMark())
                {
                    while (NULL != (passiveSkill = bf->getPassiveSkillOnAtkDmg100(idx)))
                    {
                        if (area_target->isPetMark() && bf->isPet()  && passiveSkill->effect)
                        {
                            appendDefStatus(e_skill, passiveSkill->getId(), bf);
                            float ssfactor = 0.0f;
                            ModifyAttackValue_SkillStrengthen(bf, passiveSkill, ssfactor, true);
                            factor += passiveSkill->effect->atkP * (1 + ssfactor);

                            GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(passiveSkill->getId()));
                            const GData::SkillStrengthenEffect* ef = NULL;
                            if(ss)
                                ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_NINGSHI);
                            if(ef)
                            {
                                int side = 1 - bf->getSide();
                                BattleFighter* bo = getRandomFighter(side, NULL, 0);
                                if(bo)
                                {
                                    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);
                                    bf2->setDmgNingShi(bf, ef->value / 100 * getBFAttack(bf), ef->last);
                                    appendDefStatus(e_dmgNingShi, 0, bf2);
                                }
                            }
                        }
                    }
                }
            }

           {
               size_t idx = 0;
               const GData::SkillBase *skill;
               while(NULL != (skill = bf->getPassiveSkillLingshi100(idx)))
               {
                   if(skill->effect && skill->effect->eft[0] ==  GData::e_eft_lingshi_gaoneng)
                   {
                       factor *= (1.0f + skill->effect->efv[0]);
                       appendDefStatus(e_skill, skill->getId(), bf);
                       break;
                   }
               }
           }

            UInt8& defdeclast = area_target->getDefDecLast();
            if(defdeclast > 0 && bf->getSide() != area_target->getSide())
            {
                UInt8 deflast = area_target->getDefendAddLast();
                UInt8 magdeflast = area_target->getMagDefendAddLast();
                float defAdd = area_target->getDefendAdd();
                float magdefAdd = area_target->getMagDefendAdd();
                if(deflast > 0)
                {
                    defAdd += area_target->_defend * area_target->getDefDec()/100;
                    magdefAdd += area_target->_magdef * area_target->getDefDec()/100;
                    area_target->setDefendAdd(defAdd, deflast);
                    area_target->setMagDefendAdd(magdefAdd, magdeflast);
                    -- defdeclast;

                    UInt8 defdecTimes = area_target->getDefDecTimes();
                    ++ defdecTimes;
                    appendDefStatus(e_defDec, defdecTimes, area_target);
                    area_target->setDefDecTimes(defdecTimes);
                }
                else
                    defdeclast = 0;
            }

            pr2 = bf->calcPierce(area_target);
            if(!pr2)
            {
                pr2 = getItemLing_pr(bf, itemLing);
            }
            float atk = 0;
            float magatk = 0;
            if(NULL != skill && skill->effect != NULL)
            {
                float  cf;
                calcSkillAttack(bf, cs2, area_target, atk, magatk , & cf);
                if(cs2)
                {
                     UInt8 s = bf->getSide();
                    if(s < 2)
                        _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;
                }
                else
                {
                    cs2 = getItemLing_cs(bf, itemLing);
                    if(cs2)
                    {
                        float factor = bf->calcCriticalDmg(area_target);
                        magatk = magatk * factor;
                        atk = atk * factor;
                    }
                }
                float crratk = 0;
                float crrmagatk = 0;
                if(bf->getClass() == e_cls_dao || bf->getClass() == e_cls_mo)
                    crratk = aura_factor * (atk * skill->effect->crrdamP + skill->effect->addcrr * (cs2 ? bf->calcCriticalDmg(area_target) : 1));
                else
                    crrmagatk = aura_factor * (magatk * skill->effect->crrdamP + skill->effect->addcrr * (cs2 ? bf->calcCriticalDmg(area_target) : 1));

                atk = aura_factor * (atk * skill->effect->damageP + skill->effect->adddam * (cs2 ? bf->calcCriticalDmg(area_target) : 1)) + crratk;
                magatk = aura_factor * (magatk * skill->effect->magdamP + skill->effect->addmag * (cs2 ? bf->calcCriticalDmg(area_target) : 1)) + crrmagatk;

                GData::LBSkillItem* itemSkill = bf->getSkillCondItem(SKILL_ID(skill->getId()));
                if(NULL != itemSkill)
                {
                    if(bf->getClass() == e_cls_dao || bf->getClass() == e_cls_mo)
                        atk += aura_factor * itemSkill->ef_value * (cs2 ? bf->calcCriticalDmg(area_target) : 1);
                    else
                        magatk += aura_factor * itemSkill->ef_value * (cs2 ? bf->calcCriticalDmg(area_target) : 1);
                }

                Int32 sg_v = bf->getSkillGradeExtraValue(SKILL_ID(skill->getId()));
                if(sg_v != 0)
                {
                    if(bf->getClass() == e_cls_dao || bf->getClass() == e_cls_mo)
                        atk += aura_factor * sg_v * (cs2 ? bf->calcCriticalDmg(area_target) : 1);
                    else
                        magatk += aura_factor * sg_v * (cs2 ? bf->calcCriticalDmg(area_target) : 1);
                }

                // 伤害提升
                const GData::SkillStrengthenEffect* ef = NULL;
                if(ss)
                    ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_DMG_STRENGHTHEN);
                if (ef)
                {
                    atk *= (ef->value / 100 + 1);
                    magatk *= (ef->value / 100 + 1);
                }
            }
            else
            {
                float cf = 0.0f;
                atk = calcAttack(bf, cs2, area_target, &cf);
                if(cs2)
                {
                    UInt8 s = bf->getSide();
                    if(s < 2)
                        _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

                }
                else
                {
                    cs2 = getItemLing_cs(bf, itemLing);
                    if(cs2)
                    {
                        float factor = bf->calcCriticalDmg(area_target);
                        atk = atk * factor;
                    }
                }
            }

#if 0
            float rescueRate = 0.0f;
            bool rescue = counter_deny >= 0 && (rescueRate = testRescue(area_target, counter_deny, counter_deny_list)) > 0.0f;

            // if rescure, take 1/10 of the damage
            if(rescue)
            {
                pos = area_target->getPos();
                atk *= rescueRate;
            }
#endif
            bool magdmgFlag = false;
            bool dmgFlag = false;
            if(area_target->hasFlag(BattleFighter::IsMirror))
            {
                dmg = area_target->getHP();
                dmgFlag = true;
            }
            else
            {
                float def;
                float magdef;
                float toughFactor = pr2 ? area_target->getTough(bf) : 1.0f;

                if (canProtect)
                {
                    tryProtectDamage(area_target, atk, magatk, factor);
                }
                

                if(magatk)
                {
                    if(counter_deny < 0)
                        magatk *= (1 - area_target->getDecWaveDmg());
                    magdef = getBFMagDefend(area_target);
                    float magatkreduce = getBFMagAtkReduce(area_target);
                    magdmg = _formula->calcDamage(factor * magatk, magdef, bf->getLevel(), toughFactor, magatkreduce);

                    magdmg *= static_cast<float>(950 + _rnd(100)) / 1000;

                    magdmg = magdmg > 0 ? magdmg : 1;
                    magdmgFlag = true;
                    magdmg = doBufMakeDamage(area_target, magdmg);
                }

                if(atk)
                {
                    if(counter_deny < 0)
                        atk *= (1 - area_target->getDecWaveDmg());
                    def = getBFDefend(area_target);
                    float atkreduce = getBFAtkReduce(area_target);
                    dmg = _formula->calcDamage(factor * atk, def, bf->getLevel(), toughFactor, atkreduce);

                    dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

                    dmg = dmg > 0 ? dmg : 1;
                    dmgFlag = true;
                    dmg = doBufMakeDamage(area_target, dmg);
                }
            }

            if(area_target->getMagAtkReduce3Last() > 0)
            {
                area_target->setMagAtkReduce3(0, 0);
                UInt32 value = static_cast<UInt32>(area_target->getMagAtkReduce()*100);
                appendStatusChange(e_stMagAtkReduce, value, 0, area_target);
            }
            if(area_target->getAtkReduce3Last() > 0)
            {
                area_target->setAtkReduce3(0, 0);
                UInt32 value = static_cast<UInt32>(area_target->getAtkReduce()*100);
                appendStatusChange(e_stAtkReduce, value, 0, area_target);
            }

            tryParry(area_target, dmg, magdmg, bf); // 概率招架

            UInt32 dmg3 = dmg + magdmg;
            doShieldHPAttack(area_target, dmg3);


            if(dmg3 > 0)
            {
                if (magdmgFlag)
                    makeDamage(area_target, magdmg, e_damNormal, e_damageMagic);
                if (dmgFlag)
                    makeDamage(area_target, dmg, e_damNormal, e_damagePhysic);

                if((cs2 || pr2) && ss)
                {
                    const GData::SkillStrengthenEffect* ef = NULL;
                    if(cs2)
                        ef = ss->getEffect(GData::ON_SPREAD, GData::TYPE_DAMAG_CS);
                    if(!ef && pr2)
                        ef = ss->getEffect(GData::ON_SPREAD, GData::TYPE_DAMAG_PR);
                    if(ef)
                    {
                        UInt8 side2 = area_target->getSide();
                        UInt8 pos2 = area_target->getPos() + 5; //后方单位
                        for(; pos2 < 25;)
                        {
                            BattleFighter* bo2 = static_cast<BattleFighter*>(getObject(side2, pos2));
                            if(bo2)
                            {
                                UInt32 curDmg;
                                if(magdmgFlag)
                                {
                                    curDmg = ef->value / 100 * magdmg;
                                    makeDamage(bo2, curDmg, e_damNormal, e_damageMagic);
                                }
                                if (dmgFlag)
                                {
                                    curDmg = ef->value / 100 * dmg;
                                    makeDamage(bo2, curDmg, e_damNormal, e_damagePhysic);
                                }
                                break;
                            }
                            pos2 += 5;
                        }
                    }
                }
            }
            //appendDefStatus(e_damNormal, dmg3, area_target);
            //printf("%u:%u %s %u:%u, made %u damage, hp left: %u\n", 1-side, from_pos, cs2 ? "CRITICALs" : "hits", side, pos, dmg, area_target->getHP());
            // killed the target fighter

            if (skill && skill->effect && skill->effect->hppec)
            {
                // 根据技能的百分比额外伤害
                UInt32 exDmg = 0;    

                exDmg = area_target->getMaxHP() * skill->effect->hppec;

                if (skill->effect->maxhpdampec)
                {
                    float maxExDmg = 0;
                    if (atk && atk > magatk)
                    {
                        maxExDmg = atk * skill->effect->maxhpdampec;
                    }
                    else if (magatk && magatk > atk)
                    {
                        maxExDmg = magatk * skill->effect->maxhpdampec;
                    }
                    if (maxExDmg > 0)
                        exDmg = exDmg > maxExDmg? maxExDmg:exDmg;
                }
                if (exDmg > 0)
                {
                    makeDamage(area_target, exDmg, e_damNormal, e_damageTrue);
                }
            }

            if(area_target->getHP() != 0)
            {
                if(NULL != skill && skill->effect != NULL && skill->effect->state & GData::e_state_poison)
                {
                    float rate = skill->prob * 100;
                    if((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000))
                    {
                        // poison
                        poison = true;
                    }
                }

                const GData::SkillStrengthenEffect* ef = NULL;
                if(cs2 && ss)
                    ef = ss->getEffect(GData::ON_CRITICAL, GData::TYPE_BLEED);
                if(!ef && pr2 && ss)
                    ef = ss->getEffect(GData::ON_PIERCE, GData::TYPE_BLEED);
                if (ef)
                {
                    switch(bf->getClass())
                    {
                    case e_cls_ru:
                        appendDefStatus(e_Bleed1, 0, area_target);
                        area_target->setBleed1(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    case e_cls_shi:
                        appendDefStatus(e_Bleed2, 0, area_target);
                        area_target->setBleed2(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    case e_cls_dao:
                        appendDefStatus(e_Bleed3, 0, area_target);
                        area_target->setBleed3(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    case e_cls_mo:
                        appendDefStatus(e_BleedMo, 0, area_target);
                        area_target->setBleedMo(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    }
                }
                if (ss)
                    ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_LINGYAN);
                if (ef)
                {
                    appendDefStatus(e_lingYan, 0, area_target);
                    area_target->setBleedLingYan(ef->value/100*(dmg + magdmg), ef->last, ef->valueExt1, ef->valueExt2);
                }

            }
        }
        else
        {
            // 攻击被闪避
            if(atkAct && !defend100 && !colorStock && can_counter)
            {
                const GData::SkillBase* target_skill = NULL;
                size_t idx = 0;
                while(NULL != (target_skill = area_target->getPassiveSkillAftEvd100(idx)))
                {
                    appendDefStatus(e_skill, target_skill->getId(), area_target);
                    /***************************
                    AttackAct aa = {0};
                    aa.bf = area_target;
                    aa.skill = target_skill;
                    aa.target_side = bf->getSide();
                    aa.target_pos = bf->getPos();
                    atkAct->push_back(aa);
                    *****************************/
                    counter100 = true;
                    break;
                }
                if(counter100 != true)
                {
                    target_skill = area_target->getPassiveSkillAftEvd();
                    if(NULL != target_skill)
                    {
                        appendDefStatus(e_skill, target_skill->getId(), area_target);
                        /***************************
                        AttackAct aa = {0};
                        aa.bf = area_target;
                        aa.skill = target_skill;
                        aa.target_side = bf->getSide();
                        aa.target_pos = bf->getPos();
                        atkAct->push_back(aa);
                        *****************************/
                        counter100 = true;
                    }
                }
            }

            if(!colorStock && !defend100)
            {
                area_target->setEvad100(false);
                appendDefStatus(e_damEvade, 0, area_target);
                if(area_target->getSneakStatus() == e_sneak_on)
                {
                    _sneak_atker.push_back(area_target);
                    area_target->nextSneakStatus();
                }
            }
            else
            {
                hit = true;
                appendDefStatus(e_damOut, 0, area_target);
                if(!colorStock)
                    area_target->setDefend100(false);
            }

            //printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
        }

        if(hit)
        {
            ++ _hit_cnt;
            if(bf->getSide() != area_target->getSide() && counter_deny >= 0 && (!skill || skill->cond == GData::SKILL_ACTIVE))
            {
                float bfAuraAdd = 0;
                float boAuraAdd = 0;
                calcAuraAdd(bf, area_target, skill, bfAuraAdd, boAuraAdd);
                if(bfAuraAdd > 0.1f)
                    setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, bfAuraAdd, 0, false);
                if(boAuraAdd > 0.1f)
                    setStatusChange(bf, area_target->getSide(), area_target->getPos(), 1, 0, e_stAura, boAuraAdd, 0, true);
            }
        }

        UInt32 rhp = 0;
        if(NULL != skill && skill->effect != NULL && (skill->effect->absorb || skill->effect->absorbP))
        {
            rhp = (dmg + magdmg) * skill->effect->absorbP + skill->effect->absorb;
        }
        const GData::SkillStrengthenEffect* ef = NULL;
        if(ss)
            ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_ABSORB);
        if(ef)
        {
            rhp += (dmg + magdmg) * ef->value/100;
        }

        if (ss)
            ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_AURA_GET);
        if (ef)
        {
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAura, ef->value, 0, false);
        }

        if(rhp > 0)
        {
            bf->regenHP(rhp);
            appendDefStatus(e_damAbsorb, rhp, bf);
            onHPChanged(bf);
        }

        // 一次有效的物理攻击，减少上次吸收的物理伤害和被减掉的伤害
        if (dmg > 0)
        {
            if (bf->getAtkSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_Atk, 1);
            if (bf->getAtkDecSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_DecAtk, 1);
            if(counter_deny >= 0)
                doPassiveSkillBePHYDmg(bf, area_target, dmg);
        }
        // 一次有效的法术攻击，减少法术特殊加成效果和被减掉的效果
        if (magdmg > 0)
        {
            if (bf->getMagAtkSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_MagAtk, 1);
            if (bf->getMagAtkDecSpecialLast() > 0 )
                ReduceSpecialAttrLast(bf, e_ss_DecMagAtk, 1);
            if(counter_deny >= 0)
                doPassiveSkillBeMagDmg(bf, area_target, magdmg);
        }

        // 中毒
        if(poison)
        {
            UInt32 dmg = doPoisonAttack(bf, cs2, skill, area_target, factor, atkAct);
            if(counter_deny >= 0)
                doPassiveSkillBegDmg(bf, area_target, dmg);
        }
        else if(dmg + magdmg > 0)
        {
            if(counter_deny >= 0)
                doPassiveSkillBegDmg(bf, area_target, dmg+magdmg);
        }

        std::vector<AttackAct> atkAct2;
        atkAct2.clear();
        doPassiveSkillBeAtk(bf, area_target, &atkAct2, atkAct, dmg + magdmg);
        doSkillAtk2((side == bf->getSide()), &atkAct2);

        // if this fighter can counter
        if(can_counter && counter_deny >= 0 && _winner == 0 && !_isBody[side][pos] && area_target_obj->getHP() > 0 && bf->getHP() > 0)
        {
            BattleFighter * target_fighter = static_cast<BattleFighter *>(area_target_obj);
            // test counter by rolling dice
            if(counter100 || target_fighter->calcCounter(bf, skill))
            {
                if(counter100 || target_fighter->calcHit(bf, NULL))
                {
                    doPassiveSkillOnCounter(target_fighter, bf);

                    bool cs2 = false;
                    float cf = 0.0f;
                    float atk = calcAttack(target_fighter, cs2, bf, &cf);
                    if(cs2)
                    {
                        UInt8 s = target_fighter->getSide();
                        if(s < 2)
                            _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

                    }
                    float def = getBFDefend(bf);
                    bool pr2 = target_fighter->calcPierce(bf);
                    float toughFactor = pr2 ? bf->getTough(target_fighter) : 1.0f;
                    float atkreduce = getBFAtkReduce(bf);
                    float factor = 1 + bf->getDarkVigorFactor();
                    UInt32 dmg2 = _formula->calcDamage(atk * factor, def, target_fighter->getLevel(), toughFactor, atkreduce);

                    dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;

                    dmg2 = dmg2 > 0 ? dmg2 : 1;

                    appendDefStatus(e_damNormal, 0, target_fighter);
                    size_t idx = _defList.size() - 1;
                    _defList[idx].damType2 |= 0x80;
                    if(cs2)
                        _defList[idx].damType2 |= 0x40;
                    if(pr2)
                        _defList[idx].damType2 |= 0x20;

                    makeDamage(bf, dmg2, e_damCounter, e_damagePhysic);
                    if(bf->getMagAtkReduce3Last() > 0)
                    {
                        bf->setMagAtkReduce3(0, 0);
                        UInt32 value = static_cast<UInt32>(bf->getMagAtkReduce()*100);
                        appendStatusChange(e_stMagAtkReduce, value, 0, bf);
                    }

                    if(bf->getAtkReduce3Last() > 0)
                    {
                        bf->setAtkReduce3(0, 0);
                        UInt32 value = static_cast<UInt32>(bf->getAtkReduce()*100);
                        appendStatusChange(e_stAtkReduce, value, 0, bf);
                    }
                }
#if 0
                else
                {
                    //printf("  [Counter] %u:%u attacks %u:%u, but missed!\n", side, pos, 1-side, from_pos);
                    defList[0].damType |= 0x10;
                    defList[0].counterDmg = 0;
                    defList[0].counterLeft = bf->getHP();
                }
#endif
            }
        }
    }
    else // if attacked a barrier
    {
        float atk = calcAttack(bf, cs2, 0, NULL);
        dmg = static_cast<int>(factor * atk) * (950 + _rnd(100)) / 1000;
        makeDamage(static_cast<BattleFighter*>(area_target_obj), dmg, e_damNormal, e_damagePhysic);
        //printf("%u:%u %s ground object, made %u damage, hp left: %u\n", 1-side, from_pos, cs2 ? "CRITICALs" : "hits", dmg, area_target_obj->getHP());
    }

    if(first)
    {
        cs = cs2;
        pr = pr2;
        first = false;
    }
    //to get attainment
    UInt8 s = bf->getSide();
    if(skill && s < 2 &&  _player[s] != NULL)
    {
        UInt32 d = magdmg + dmg;
        if(skill->cond == GData::SKILL_PEERLESS)
        {
            if( 0 == _attackRound )
                _firstPLDmg[s] = true;
           // printf("第一回合释放无双技能%u",  _attackRound);
            _maxPeerLessDmg[s] = std::max( _maxPeerLessDmg[s], d);
        }
        else
        {
            _maxSkillDmg[s] = std::max(_maxSkillDmg[s], d);
        }
    }
    return dmg + magdmg;
}

void BattleSimulator::doPassiveSkillBeAtk(BattleFighter* bf, BattleFighter* bo, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct, UInt32 dmg)
{
    if(bf == NULL || bo == NULL || atkAct == NULL)
        return;

    if(bo->getHP() > 0)
    {
        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        const GData::SkillBase* passiveSkillThorn = NULL;
        const GData::SkillBase* passiveSkillInj = NULL;
        while(NULL != (passiveSkill = bo->getPassiveSkillBeAtk100(idx)))
        {
            if(passiveSkill->effect == NULL)
                continue;
            if(passiveSkillThorn == NULL && (passiveSkill->effect->thorn || passiveSkill->effect->thornP))
            {
                passiveSkillThorn = passiveSkill;
            }
            else if(passiveSkillInj == NULL &&(passiveSkill->effect->inj2hpP || passiveSkill->effect->inj2hp))
            {
                passiveSkillInj = passiveSkill;
            }
            else if(bf->getHP() > 0 && bf->getSide() != bo->getSide())
            {
                AttackAct aa = {0};
                aa.bf = bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();

                atkAct2->push_back(aa);
            }
        }

        passiveSkill = bo->getPassiveSkillBeAtk();
        if(NULL != passiveSkill && passiveSkill->effect != NULL)
        {
            if( passiveSkillThorn == NULL && (passiveSkill->effect->thorn || passiveSkill->effect->thornP))
            {
                passiveSkillThorn = passiveSkill;
            }
            else if(passiveSkillInj == NULL && (passiveSkill->effect->inj2hpP || passiveSkill->effect->inj2hp))
            {
                passiveSkillInj = passiveSkill;
            }
            else if(bf->getHP() > 0 && bf->getSide() != bo->getSide())
            {
                AttackAct aa = {0};
                aa.bf = bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();

                atkAct2->push_back(aa);
            }
        }

        if(passiveSkillInj && dmg)
        {
            AttackAct aa = {0};
            aa.bf = bo;
            aa.skill = passiveSkillInj;
            aa.target_side = bo->getSide();
            aa.target_pos = bo->getPos();
            aa.param = dmg;

            atkAct2->push_back(aa);
        }

        if(passiveSkillThorn && bf->getSide() != bo->getSide() && dmg && bf->getHP() > 0)
        {
            AttackAct aa = {0};
            aa.bf = bo;
            aa.skill = passiveSkillThorn;
            aa.target_side = bf->getSide();
            aa.target_pos = bf->getPos();
            aa.param = dmg;

            atkAct2->push_back(aa);
        }
    }
}

void BattleSimulator::doPassiveSkillOnCounter(BattleFighter* bf, BattleFighter* bo)
{
    if(!bf || !bo)
        return;

    const GData::SkillBase* passiveSkill = bf->getPassiveSkillOnCounter();
    if(passiveSkill)
    {
        appendDefStatus(e_skill, passiveSkill->getId(), bf);
        doSkillEffectExtraCounter(bf, bo, passiveSkill);
    }

    size_t idx = 0;
    while(NULL != (passiveSkill = bf->getPassiveSkillOnCounter100(idx)))
    {
        appendDefStatus(e_skill, passiveSkill->getId(), bf);
        doSkillEffectExtraCounter(bf, bo, passiveSkill);
    }
}

UInt32 BattleSimulator::doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, std::vector<AttackAct>* atkAct)
{
    UInt32 dmg = 0;
    UInt32 dmg2 = abs(calcPoison(bf, skill, area_target, cs)) * factor;
    // 第一波毒
    std::vector<AttackAct> atkAct2;
    atkAct2.clear();
    for( int i = 1; i <= 3; ++ i)
    {
        UInt32 dmg3 = dmg2;
        doSkillState(bf, skill, area_target, 1, i, dmg3, &atkAct2, atkAct);
        doSkillAtk2(false, &atkAct2);
        if(dmg3 > 0)
            dmg += dmg3;
        else
            break;
    }

    UInt8& lingquTimes = area_target->getLingQuTimes();
    if(lingquTimes > 0)
        -- lingquTimes;

    return dmg;
}

void BattleSimulator::doSkillAtk2(bool activeFlag, std::vector<AttackAct>* atkAct)
{
    if(atkAct == NULL)
        return;

    size_t actCnt = atkAct->size();
    for(size_t idx = 0; idx < actCnt; idx++)
    {
        if(_winner != 0)
            break;
        BattleFighter * bf = (*atkAct)[idx].bf;
        if(!bf || bf->getHP() == 0)
            continue;

        int target_side = (*atkAct)[idx].target_side;
        int target_pos = (*atkAct)[idx].target_pos;
        const GData::SkillBase* skill = (*atkAct)[idx].skill;

        appendDefStatus(e_skill, skill->getId(), bf);

        UInt32 skillParam = (*atkAct)[idx].param;
        // 法术抵抗后的技能
        if(skill->cond == GData::SKILL_AFTRES && bf->getSide() != target_side)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(NULL == bo)
                continue;

            const GData::SkillBase* boSkill = GData::skillManager[skillParam];
            if(boSkill == NULL || boSkill->effect == NULL)
                continue;

            //UInt8 state[3] = {0};
            UInt8 idx = 0;

            Int16 nStateLast = boSkill->last;
            UInt32 effect_state = (*atkAct)[idx].param1;
            //if(SKILL_ID(skillParam) == 122)  // 元磁神雷的符文状态被反弹
            if(effect_state & 0xFFFF0000)
            {
                UInt32 nparm = (*atkAct)[idx].param1;
                nStateLast = nparm & 0x0000ffff;
                effect_state = (nparm&0xffff0000) >> 16;
            }

            UInt16 immune = bo->getImmune();
            if(((effect_state & immune) && SKILL_LEVEL(boSkill->getId()) <= bo->getImmuneLevel(effect_state)) || (effect_state & bo->getImmune3()))
            {
                appendDefStatus(e_Immune, 0, bo);
                continue;
            }

            switch(effect_state)
            {
            case GData::e_state_poison:
                {
                    UInt32 dmg = abs(calcPoison(bo, boSkill, bo, false));
                    UInt32 dmg3 = dmg*0.5;
                    makeDamage(bo, dmg3, e_Poison, e_damagePoison);
                    dmg3 = dmg;
                    makeDamage(bo, dmg3, e_Poison, e_damagePoison);
                    dmg3 = dmg*1.5;
                    makeDamage(bo, dmg3, e_UnPoison, e_damagePoison);
                }
                break;
            case GData::e_state_confuse:
                if(bo->getConfuseRound() < 1)
                {
                    bo->setConfuseLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setConfuseRound(nStateLast + 1);
                    appendDefStatus(e_Confuse, 0, bo);
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                }
                break;
            case GData::e_state_stun:
                if(bo->getStunRound() < 1)
                {
                    bo->setStunLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setStunRound(nStateLast + 1);
                    appendDefStatus(e_Stun, 0, bo);
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                }
                break;
            case GData::e_state_forget:
                if(bo->getForgetRound() < 1)
                {
                    bo->setForgetLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setForgetRound(nStateLast + 1);
                    appendDefStatus(e_Forget, 0, bo);
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                }
                break;
            case GData::e_state_weak:
                {
                    // 虚弱，就是治疗效果减半。。。
                    if(bo->getTherapyBuff() != 0)  // 去掉以前的buff
                    {
                        if (bo->getTherapyBuff() > 0)
                            appendDefStatus(e_unTherapyBuff, 0, bo);
                        else
                            appendDefStatus(e_UnWeak, 0, bo);
                    }

                    bo->setTherapyBuff(-0.5f, nStateLast);
                    appendDefStatus(e_Weak, 0, bo);
                }
                break;
            case GData::e_state_mark_mo:
                {
                    bo->setMarkMo(true, nStateLast + 1);
                    appendDefStatus(e_markMo, 0, bo);
                }
                break;
            case GData::e_state_blind:
                {
                    float value = boSkill->effect->hitrate/100;
                    if(value < -0.001f)
                        value = -value;
                    bo->setBlind(value, nStateLast);
                    appendDefStatus(e_blind, 0, bo);
                    calcAbnormalTypeCnt(bo);
                }
                break;
            }
            continue;
        }

        // 被攻击后的技能
        if(skill->cond == GData::SKILL_BEATKED)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));

            if(NULL == bo)
                continue;

            if(bf->getHP() > 0 && _winner == 0 && bo->getHP() > 0)
            {
                UInt32 fdmg = 0;
                UInt32 inj2hp = 0;
                if(fdmg == 0 && (skill->effect->thorn || skill->effect->thornP))
                {
                    fdmg = skillParam * skill->effect->thornP + skill->effect->thorn;
                }
                else if(inj2hp == 0 &&(skill->effect->inj2hpP || skill->effect->inj2hp))
                {
                    inj2hp = skillParam * skill->effect->inj2hpP + skill->effect->inj2hp;
                }

                if(inj2hp > 0)
                {
                    bf->regenHP(inj2hp);
                    appendDefStatus(e_damInj, inj2hp, bf);
                    onHPChanged(bf);
                    continue;
                }
                else if(fdmg > 0 && bf->getSide() != target_side)
                {
                    UInt16 immune = bo->getImmune();
                    if((GData::e_state_dmgback & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel(GData::e_state_dmgback))
                    {
                        appendDefStatus(e_Immune, 0, bo);
                    }
                    else
                    {
                        makeDamage(bo, fdmg, e_damBack, e_damageTrue);
                    }

                    continue;
                }

            }
        }

        GData::Area* area = NULL;
        area = &(GData::areaList[skill->area]);

        AttackPoint ap[25];
        int apcnt = 0;

        if(skill->area != 0 && skill->area != 1 && area->getCount() > 0)
        {
            int x_ = target_pos % 5;
            int y_ = target_pos / 5;
            int cnt = area->getCount();
            int fsize = skill->factor.size();
            for(int i = 1; i < cnt; ++ i)
            {
                GData::Area::Data& ad = (*area)[i];
                int x = x_ + ad.x;
                int y = y_ + ad.y;
                if(x < 0 || x > 4 || y < 0 || y > 4)
                {
                    continue;
                }
                ap[apcnt].pos = x + y * 5;
                int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
                if(fsize != 0)
                    ap[apcnt ++].factor = skill->factor[idx];
                else
                    ap[apcnt ++].factor = 1;
            }
        }

        // 免疫降灵气
        //bool dostatus = true;
        //if(dostatus)
        {
            bool self = false;
            bool flag = ((target_side == bf->getSide()) ? activeFlag : !activeFlag);
            if(1 == skill->area)
            {
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    bool ifDecAura = isImmuneDecAura(skill, target_side, pos);
                    doSkillStatus(flag, bf, skill, target_side, pos, 1, self, ifDecAura);
                }
            }
            else if(0 == skill->area)
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos);
                doSkillStatus(flag, bf, skill, target_side, target_pos, 1, self, ifDecAura);
            }
            else
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos);
                    doSkillStatus(flag, bf, skill, target_side, target_pos, 1, self, ifDecAura);
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    bool ifDecAura = isImmuneDecAura(skill, target_side, ap[i].pos);
                    doSkillStatus(flag, bf, skill, target_side, ap[i].pos, 1, self, ifDecAura);
                }

            }
        }

        // therapy skill
        if(skill->effect->hp > 0 || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
        {
            bool cs = false;
            bool first = true;
            UInt32 rhp = calcTherapy(bf, cs, first, skill);
            if(bf->getSide() != target_side)
            {
                UInt32 hpr = bf->regenHP(rhp);
                if(hpr != 0)
                {
                    appendDefStatus(e_damHpAdd, hpr, bf);
                    onHPChanged(bf);
                }
            }
            else if(1 == skill->area)
            {
                int i = 0;
                int fsize = skill->factor.size();
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    if(bo->getLostHP() == 0)
                        continue;
                    if(bo->isLingQu() || bo->isSoulOut())
                        continue;

                    float factor = 1;
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        factor = skill->factor[idx];

                    UInt32 hpr = bo->regenHP(rhp * factor);
                    ++i;
                    if(hpr == 0)
                        continue;

                    appendDefStatus(e_damHpAdd, hpr, bo);
                    onHPChanged(bo);
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp);
                if(hpr != 0)
                {
                    appendDefStatus(e_damHpAdd, hpr, bo);
                    onHPChanged(bo);
                }
            }
            else
            {
                int fsize = skill->factor.size();
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    float factor = 1;
                    if(fsize > 0)
                        factor = skill->factor[0];
                    UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp * factor);
                    if(hpr != 0)
                    {
                        appendDefStatus(e_damHpAdd, hpr, bo);
                        onHPChanged(bo);
                    }
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(NULL == bo)
                        continue;

                    UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp * ap[i].factor, skill->cond == GData::SKILL_ACTIVE);
                    if(hpr != 0)
                    {
                        appendDefStatus(e_damHpAdd, hpr, bo);
                        onHPChanged(bo);
                    }
                }
            }
        }
        else if( (!(skill->effect->state & GData::e_state_poison)) && (skill->effect->hp < 0 || skill->effect->addhp < 0 || skill->effect->hpP < -0.001) )
        {
            if(bf->getSide() != target_side)
            {
                if(1 == skill->area)
                {
                    int i = 0;
                    int fsize = skill->factor.size();
                    for(UInt8 pos = 0; pos < 25; ++ pos)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        UInt32 dmg = abs(calcPoison(bf, skill, bo, false));
                        int idx = std::min(abs(fsize-1), i);
                        if(fsize > 0)
                            dmg *= skill->factor[idx];
                        ++i;
                        makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                    }
                }
                else if(0 == skill->area)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        UInt32 dmg = abs(calcPoison(bf, skill, bo, false));
                        makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                    }
                }
                else
                {
                    int fsize = skill->factor.size();
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        UInt32 dmg = abs(calcPoison(bf, skill, bo, false));
                        if(fsize > 0)
                            dmg *= skill->factor[0];
                        makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                    }

                    for(int i = 0; i < apcnt; ++ i)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        UInt32 dmg = abs(calcPoison(bf, skill, bo, false)) * ap[i].factor;
                        makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                    }
                }
            }
        }

        // 混乱晕眩封印虚弱墨印至盲状态
        if(skill->effect->state & GData::e_state_c_s_f_w_m_b)
        {
            if(bf->getSide() != target_side)
            {
                if(1 == skill->area)
                {
                    int i = 0;
                    int fsize = skill->factor.size();
                    for(UInt8 pos = 0; pos < 25; ++ pos)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        float factor = 1;
                        int idx = std::min(abs(fsize-1), i);
                        if(fsize > 0)
                            factor = skill->factor[idx];
                        UInt32 dmg = 0;
                        doSkillState(bf, skill, bo, factor, 0, dmg, NULL, NULL);
                        ++ i;
                    }
                }
                else if(0 == skill->area)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        std::vector<AttackAct> atkAct2;
                        atkAct2.clear();
                        UInt32 dmg = 0;
                        doSkillState(bf, skill, bo, 1, 0, dmg, &atkAct2, atkAct);
                        doSkillAtk2(!activeFlag, &atkAct2);
                    }
                }
                else
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        int fsize = skill->factor.size();
                        float factor = 1;
                        if(fsize > 0)
                            factor = skill->factor[0];

                        std::vector<AttackAct> atkAct2;
                        atkAct2.clear();
                        UInt32 dmg = 0;
                        doSkillState(bf, skill, bo, factor, 0, dmg, &atkAct2, atkAct);
                        doSkillAtk2(!activeFlag, &atkAct2);
                    }

                    for(int i = 0; i < apcnt; ++ i)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        std::vector<AttackAct> atkAct2;
                        atkAct2.clear();
                        UInt32 dmg = 0;
                        doSkillState(bf, skill, bo, ap[i].factor, 0, dmg, &atkAct2, atkAct);
                        doSkillAtk2(!activeFlag, &atkAct2);
                    }
                }
            }
        }
    }
}

bool BattleSimulator::doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, float factor, UInt8 poisonTimes, UInt32& dmg, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct)
{
    if(NULL == skill || bf == NULL || skill->effect == NULL || skill->effect->state == 0)
        return false;

    BattleFighter* target_bo = static_cast<BattleFighter*>(bo);
    if(target_bo->isLingQu() || target_bo->getHP() == 0 || target_bo->isSoulOut())
    {
        dmg = 0;
        return false;
    }

    UInt16 effect_state = skill->effect->state;
    getSkillEffectExtraBlind(bf, target_bo, skill, effect_state);

    float prob = skill->prob;  // 前面被扩大了100倍。。。缩小回去先。。>_<
    ModifySingleAttackValue_SkillStrengthen(bf, skill, prob, true);
    prob = prob * 100 * factor;  // 扩大回去。。。汗一个。。。

    if(((skill->cond == GData::SKILL_ACTIVE || skill->cond == GData::SKILL_PEERLESS) && prob <= _rnd(10000)))
    {
        effect_state = 0;
        if(skill->effect->state & GData::e_state_mark_mo)
            effect_state = GData::e_state_mark_mo;
        if(poisonTimes > 0)
            effect_state |= GData::e_state_poison;

        if(effect_state == 0)
            return false;
    }

    UInt16 state[3] = {0};
    UInt8 cnt = 0;
    UInt8 state_idx = 0;
    if(SKILL_ID(skill->getId()) == 136 || SKILL_ID(skill->getId()) == 146) // 光棍, 冰火双剪
    {
        switch(bf->getClass())
        {
        case e_cls_ru:
            effect_state = GData::e_state_confuse;
            break;
        case e_cls_shi:
            return false;
        case e_cls_dao:
            effect_state = GData::e_state_stun;
            break;
        case e_cls_mo:
            effect_state = GData::e_state_blind;
            break;
        }
    }

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    state[0] = effect_state;
    if(effect_state & GData::e_state_c_s_f_w)
    {
        if(effect_state & GData::e_state_confuse)
        {
            state[cnt] = GData::e_state_confuse;
            ++cnt;
        }
        if(effect_state & GData::e_state_stun)
        {
            state[cnt] = GData::e_state_stun;
            ++cnt;
        }
        if(effect_state & GData::e_state_forget)
        {
            state[cnt] = GData::e_state_forget;
            ++cnt;
        }
        if(effect_state & GData::e_state_weak)
        {
            state[cnt] = GData::e_state_weak;
            ++cnt;
        }
        if(cnt > 1)
            state_idx = _rnd(cnt);
    }

    effect_state = state[state_idx];
    UInt16 immune = target_bo->getImmune();
    UInt16 immune2 = target_bo->getImmune2();
    UInt16 colorStock = target_bo->getColorStock();

    effect_state = effect_state & (0xFFFF ^ colorStock);
    if(effect_state == 0)
    {
        appendDefStatus(e_Immune, 0, target_bo);
        return false;
    }
    effect_state = effect_state & (0xFFFF ^ immune2);
    if(effect_state == 0)
    {
        target_bo->setImmune2(0);
        appendDefStatus(e_unImmune2, 0, target_bo);
        return false;
    }

    if(SKILL_LEVEL(skill->getId()) <= target_bo->getImmuneLevel(effect_state))
        effect_state = effect_state & (0xFFFF ^ immune);
    effect_state = effect_state & (0xFFFF ^ target_bo->getImmune3());
    if(effect_state == 0)
    {
        appendDefStatus(e_Immune, 0, target_bo);
        return false;
    }

    if(effect_state & GData::e_state_poison)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_poison, skill, atkAct2, atkAct))
        {
            float dmgfactor[4] = {0, 0.5, 1, 1.5};
            if( poisonTimes > 3 )
                 poisonTimes = 0;
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
            // 如果是抵抗，查找是否有符文强化
            AddExtraDamageAfterResist_SkillStrengthen(bf, target_bo, skill, dmg * dmgfactor[poisonTimes]);
            // 如果被抵抗，上状态
            AddStateAfterPoisonResist_SkillStrengthen(bf, target_bo, skill, poisonTimes);
            dmg = 0;
        }
        else
        {
            float dmgfactor[4] = {0, 0.5, 1, 1.5};
            if( poisonTimes > 3 )
                 poisonTimes = 0;

            dmg = dmg * dmgfactor[poisonTimes];
            doShieldHPAttack(target_bo, dmg);
            if(dmg > 0)
            {
                if(poisonTimes == 3)
                    makeDamage(target_bo, dmg, e_UnPoison, e_damagePoison);
                else
                    makeDamage(target_bo, dmg, e_Poison, e_damagePoison);
            }
        }
    }
    if(effect_state & GData::e_state_confuse)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_confuse, skill, atkAct2, atkAct))
        {
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
        }
        else if(target_bo->getDeepConfuseDmgExtra() < 0.001f)
        {
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_CONFUSE, GData::TYPE_DAMAG_A);
            if(ef)
            {
                target_bo->setConfuseLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setConfuseRound(skill->last + 1);
                else
                    target_bo->setConfuseRound(skill->last);

                target_bo->setDeepConfuseDmgExtra(ef->value/100, ef->last);
                appendDefStatus(e_deepConfuse, 0, target_bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);
            }
            else if(target_bo->getConfuseRound() < 1)
            {
                target_bo->setConfuseLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setConfuseRound(skill->last + 1);
                else
                    target_bo->setConfuseRound(skill->last);

                appendDefStatus(e_Confuse, 0, target_bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);
            }
            for (std::vector<BattleFighter*>::iterator it = _onOtherConfuseAndForget.begin(); it != _onOtherConfuseAndForget.end(); ++it)
            {
                BattleFighter* atk = *it;
                if (!atk || atk->getHP() <= 0 || atk->getSide() == target_bo->getSide() ||
                        atk->getStunRound() || atk->getConfuseRound() || atk->getForgetRound())
                    continue;
                _onOtherConfuseAndForgetAtkList[atk->getSide()].push_back(atk);
            }
        }
    }
    if(effect_state & GData::e_state_stun)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_stun, skill, atkAct2, atkAct))
        {
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
        }
        else if(target_bo->getDeepStunDmgExtra() < 0.001f)
        {
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_STUN, GData::TYPE_DAMAG_A);
            if(ef)
            {
                target_bo->setStunLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setStunRound(skill->last + 1);
                else
                    target_bo->setStunRound(skill->last);

                target_bo->setDeepStunDmgExtra(ef->value/100, ef->last);
                appendDefStatus(e_deepStun, 0, target_bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);
            }
            else if(target_bo->getStunRound() < 1)
            {
                target_bo->setStunLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setStunRound(skill->last + 1);
                else
                    target_bo->setStunRound(skill->last);

                appendDefStatus(e_Stun, 0, target_bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);

            }
        }
    }
    if(effect_state & GData::e_state_forget)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_forget, skill, atkAct2, atkAct))
        {
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
        }
        else if(target_bo->getDeepForgetDmgExtra() < 0.001f)
        {
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_FORGET, GData::TYPE_DAMAG_A);
            if(ef)
            {
                target_bo->setForgetLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setForgetRound(skill->last + 1);
                else
                    target_bo->setForgetRound(skill->last);

                target_bo->setDeepForgetDmgExtra(ef->value/100, ef->last);
                appendDefStatus(e_deepForget, 0, target_bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);
            }
            else if(target_bo->getForgetRound() < 1)
            {
                target_bo->setForgetLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setForgetRound(skill->last + 1);
                else
                    target_bo->setForgetRound(skill->last);
                appendDefStatus(e_Forget, 0, target_bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);
            }
            for (std::vector<BattleFighter*>::iterator it = _onOtherConfuseAndForget.begin(); it != _onOtherConfuseAndForget.end(); ++it)
            {
                BattleFighter* atk = *it;
                if (!atk || atk->getHP() <= 0 || atk->getSide() == target_bo->getSide() ||
                        atk->getStunRound() || atk->getConfuseRound() || atk->getForgetRound())
                    continue;
                _onOtherConfuseAndForgetAtkList[atk->getSide()].push_back(atk);
            }
        }
    }
    if(effect_state & GData::e_state_weak)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_weak, skill, atkAct2, atkAct))
        {
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
        }
        else
        {
            // 虚弱，就是治疗效果减半。。。
            if(target_bo->getTherapyBuff() != 0)  // 去掉以前的buff
            {
                if (target_bo->getTherapyBuff() > 0)
                    appendDefStatus(e_unTherapyBuff, 0, target_bo);
                else
                    appendDefStatus(e_UnWeak, 0, target_bo);
            }
            target_bo->setTherapyBuff(-0.5f, skill->last);
            appendDefStatus(e_Weak, 0, target_bo);
        }
    }
    if(effect_state & GData::e_state_mark_mo)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_mark_mo, skill, atkAct2, atkAct))
        {
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
        }
        else
        {
            target_bo->setMarkMo(true, skill->last + 1);
            appendDefStatus(e_markMo, 0, target_bo);
        }
    }
    if(effect_state & GData::e_state_blind)
    {
        if(doStateMagRes(bf, target_bo, GData::e_state_blind, skill, atkAct2, atkAct))
        {
            AddStateAfterResist_SkillStrengthen(bf, target_bo, skill);
        }
        else
        {
            float value = skill->effect->hitrate/100;
            if(value < -0.001f)
                value = -value;
            target_bo->setBlind(value, skill->last);
            calcAbnormalTypeCnt(bo);
            if(!doSkillStrengthenDeepBlind(bf, target_bo, ss))
                appendDefStatus(e_blind, 0, target_bo);
        }
    }

    return true;
}

bool BattleSimulator::doStateMagRes(BattleFighter* bf, BattleFighter* target_bo, UInt16 state, const GData::SkillBase* skill, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct)
{
    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    float rate = target_bo->getMagRes(bf) * 100;
    bool fres = rate > _rnd(10000);
    if(!fres)
        fres = getItemXin_MagRes(target_bo, state);
    else
        appendDefStatus(e_Res, 0, target_bo);

    if(fres)
    {
        idx = 0;

        if(atkAct)
        {
            if(passiveSkill == NULL)
                passiveSkill = target_bo->getPassiveSkillAftRes();

            if(passiveSkill == NULL)
            {
                idx = 0;
                while(NULL == passiveSkill)
                {
                    size_t oidx = idx;
                    passiveSkill = target_bo->getPassiveSkillAftRes100(idx);
                    if (oidx == idx)
                        break;
                }
            }

            if(NULL != passiveSkill)
            {
                AttackAct aa = {0};
                aa.bf = target_bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();
                aa.param = skill->getId();
                aa.param1 = state;

                atkAct2->push_back(aa);
            }
        }
        return true;
    }

    return false;
}

bool BattleSimulator::doStateMagRes2(BattleFighter* bf, BattleFighter* target_bo, UInt16 state)
{
    float rate = target_bo->getMagRes(bf) * 100;
    bool fres = rate > _rnd(10000);
    if(!fres)
        fres = getItemXin_MagRes(target_bo, state);
    else
        appendDefStatus(e_Res, 0, target_bo);

    if(fres)
        return true;

    return false;
}

bool BattleSimulator::doNormalAttack(BattleFighter* bf, int otherside, int target_pos, std::vector<AttackAct>* atkAct, float factor /* = 1 */, bool canProtect /* = false */)
{
    if(bf == NULL || bf->getHP() == 0 || target_pos < 0 || _winner != 0)
        return false;

    BattleObject* target_object = getObject(otherside, target_pos);
    if(target_object!= NULL)
    {
        // 合击
        if (bf->getSide() != target_object->getSide())
            tryAttackWithPet(bf);
        // find all targets that are hit

        // calculate damage
        bool cs = false;
        bool pr = false;
        bool first = true;

        UInt32 dmg = 0;
        // attack only one target
        dmg += attackOnce(bf, first, cs, pr, NULL, target_object, factor, 0, NULL, atkAct, canProtect);

        int self_side = bf->getSide() == otherside ? 25 : 0;
        appendToPacket(bf->getSide(), bf->getPos(), target_pos + self_side, static_cast<UInt8>(0), static_cast<UInt16>(0), cs, pr);
        return true;
    }

    return false;
}

void BattleSimulator::getSkillTarget(BattleFighter* bf, const GData::SkillBase* skill, int& target_side, int& target_pos, int& cnt)
{
    target_pos = -1;
    if(skill == NULL)
        return;
    if(skill->effect == NULL)
        return;

    target_side = skill->target != GData::e_battle_target_otherside ? bf->getSide() : 1 - bf->getSide();

    if(1 == skill->area)
    {
        cnt = 25;
        target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
    }
    else if( GData::e_battle_target_selfside == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 1;
        excepts[0] = bf->getPos();

        if(skill->effect->aura > 0 || skill->effect->auraP > 0.001)
        {
            UInt8 side = bf->getSide();
            for(int i = 0; i < 25; ++ i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
                if(!bo)  // 雪人
                    continue;

                if(bo->getAura() > 99 || bo->getId() == 5679 || bo->isPet() || bo->isSoulOut())
                {
                    excepts[exceptCnt] = i;
                    ++ exceptCnt;
                }
            }
        }

        BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
        if(NULL == bo)
        {
            int tidx = getSpecificTarget(bf->getSide(), BattleSimulator::isPet);
            if (tidx > 0)
            {
                BattleFighter *pet = static_cast<BattleFighter *> (getObject(bf->getSide(), tidx));
                if (pet && pet->getAura() < 100)
                {
                    target_pos = pet->getPos();
                    target_side = pet->getSide();
                }
                else
                {
                    target_pos = bf->getPos();
                    target_side = bf->getSide();
                }
            }
            else
            {
                target_pos = bf->getPos();
                target_side = bf->getSide();
            }
            cnt = 1;
        }
        else
        {
            target_pos = bo->getPos();
            target_side = bo->getSide();
            cnt = 1;
        }
    }
    else if( GData::e_battle_target_otherside == skill->target )
    {
        cnt = 1;
        target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
    }
    else if ( GData::e_battle_target_otherside_max == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 0;
        UInt8 side = bf->getSide()?0:1;
        for (int i = 0; i < 25; ++i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
            if(!bo)  // 雪人
                continue;

            if(bo->getId() == 5679 || bo->isSoulOut())
            {
                excepts[exceptCnt] = i;
                ++ exceptCnt;
            }
        }
        BattleFighter* bo = getMaxHpFighter(side, excepts, exceptCnt);
        if(NULL == bo)
        {
            return;
        }
        else
        {
            target_pos = bo->getPos();
            target_side = bo->getSide();
            cnt = 1;
        }
    }
    else if ( GData::e_battle_target_otherside_min == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 0;
        UInt8 side = bf->getSide()?0:1;
        for (int i = 0; i < 25; ++i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
            if(!bo)  // 雪人
                continue;

            if(bo->getId() == 5679 || bo->isSoulOut())
            {
                excepts[exceptCnt] = i;
                ++ exceptCnt;
            }
        }
        BattleFighter* bo = getMinHpFighter(side, excepts, exceptCnt);
        if(NULL == bo)
        {
            return;
        }
        else
        {
            target_pos = bo->getPos();
            target_side = bo->getSide();
            cnt = 1;
        }
    }
    else if ( GData::e_battle_target_selfside_max == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 1;
        excepts[0] = bf->getPos();
        UInt8 side = bf->getSide();
        for (int i = 0; i < 25; ++i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
            if(!bo)  // 雪人
                continue;

            if(bo->getId() == 5679 || bo->isSoulOut())
            {
                excepts[exceptCnt] = i;
                ++ exceptCnt;
            }
        }
        BattleFighter* bo = getMaxHpFighter(bf->getSide(), excepts, exceptCnt);
        if(NULL == bo)
        {
            return;
        }
        else
        {
            target_pos = bo->getPos();
            target_side = bo->getSide();
            cnt = 1;
        }
    }
    else if ( GData::e_battle_target_selfside_min == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 1;
        excepts[0] = bf->getPos();
        UInt8 side = bf->getSide();
        for (int i = 0; i < 25; ++i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
            if(!bo)  // 雪人
                continue;

            if(bo->getId() == 5679 || bo->isSoulOut())
            {
                excepts[exceptCnt] = i;
                ++ exceptCnt;
            }
        }
        BattleFighter* bo = getMinHpFighter(bf->getSide(), excepts, exceptCnt);
        if(NULL == bo)
        {
            return;
        }
        else
        {
            target_pos = bo->getPos();
            target_side = bo->getSide();
            cnt = 1;
        }
    }
    else if ( GData::e_battle_target_selfside_atk_max == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 1;
        excepts[0] = bf->getPos();
        UInt8 side = bf->getSide();
        for (int i = 0; i < 25; ++i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
            if(!bo)  // 雪人
                continue;

            if(bo->getId() == 5679 || bo->isSoulOut())
            {
                excepts[exceptCnt] = i;
                ++ exceptCnt;
            }
        }
        BattleFighter* bo = getMaxAtkFighter(bf->getSide(), excepts, exceptCnt);
        if(NULL == bo)
        {
            return;
        }
        else
        {
            target_pos = bo->getPos();
            target_side = bo->getSide();
            cnt = 1;
        }
    }
    else if ( GData::e_battle_target_self == skill->target )
    {
        target_pos = bf->getPos();
        target_side = bf->getSide();
        cnt = 1;
    }
    else
    {
        target_pos = bf->getPos();
        target_side = bf->getSide();
        cnt = 1;
    }
}

void BattleSimulator::doAllSkillStrengthenEffect(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos)
{
    if(!bf || !skill)
        return;

    GData::SkillStrengthenBase*  ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return;

    const std::vector<const GData::SkillStrengthenEffect*>& efs = ss->effect;
    const GData::SkillStrengthenEffect* ef = NULL;
    for(size_t i = 0; i < efs.size(); ++ i)
    {
        ef = efs[i];
        if(ef && ef->target == GData::e_battle_target_self)
        {
            doSkillStrengthenAttack(bf, skill, ef, bf->getSide(), bf->getPos(), true);
        }
        else if(ef && ef->target == GData::e_battle_target_selfside_atk_2nd) // 攻击力第二高的目标
        {
            UInt8 excepts[25] = {0};
            size_t exceptCnt = 1;
            excepts[0] = bf->getPos();
            UInt8 side = bf->getSide();
            for (int i = 0; i < 25; ++i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
                if(!bo)  // 雪人
                    continue;

                if(bo->getId() == 5679 || bo->isSoulOut())
                {
                    excepts[exceptCnt] = i;
                    ++ exceptCnt;
                }
            }
            BattleFighter* bo = get2ndAtkFighter(bf->getSide(), excepts, exceptCnt);
            if(bo)
            {
                int target_side = bo->getSide();
                int target_pos = bo->getPos();
                doSkillStrengthenAttack(bf, skill, ef, target_side, target_pos, true);
            }
        }
        else if (ef)
        {
            doSkillStrengthenAttack(bf, skill, ef, target_side, target_pos, true);
        }
    }
}


bool BattleSimulator::doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct, UInt32 skillParam, UInt8* launchPeerLess /* = NULL */, bool canProtect /* = false */)
{
    if(NULL == skill || target_pos < 0 || bf == NULL)
        return false;
    if(skill->effect == NULL)
        return false;

    if(bf->getHP() == 0 || _winner != 0)
    {
        return false;
    }

    // 合击
    if (bf->getSide() != target_side)
        tryAttackWithPet(bf);

    GData::SkillStrengthenBase*  ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    // 雪球-137 小蜘蛛-434
    if(SKILL_ID(skill->getId()) == 137 || SKILL_ID(skill->getId()) == 434 || SKILL_ID(skill->getId()) == 479)
    {
        static UInt8 skill_prob_137[10][3] = {
            {0, 0, 0},
            {97, 99, 100},
            {85, 96, 100},
            {73, 93, 100},
            {61, 90, 100},
            {49, 87, 100},
            {37, 84, 100},
            {25, 81, 100},
            {13, 78, 100},
            {0, 75, 100}
        };

        UInt8 cnt = 0;
        if(SKILL_ID(skill->getId()) == 137)
        {

            UInt8 level = SKILL_LEVEL(skill->getId());
            if(level > 9)
                level = 9;
            UInt8 roll = _rnd(100);
            if(skill_prob_137[level][0] > roll)
                cnt = 1;
            else if(skill_prob_137[level][1] > roll)
                cnt = 2;
            else if(skill_prob_137[level][2] > roll)
                cnt = 3;
        }
        else
            cnt = 2;

        for(UInt8 i = 0; i < cnt; ++i)
        {
            UInt8 posList[25];
            size_t posCount = 0;
            UInt8 side = bf->getSide();
            UInt8 y = 0;
            while(y < 5)
            {
                UInt8 p = y * 5;
                for(UInt8 x = 0; x < 5; ++ x, ++ p)
                {
                    if(getObject(side, p) == NULL || getObject(side, p)->getHP() == 0)
                        posList[posCount ++] = p;
                }
                ++ y;
            }
            if(posCount == 0)
                break;
            bf->addFlag(BattleFighter::Mirrored);
            UInt8 pos = posList[_rnd(posCount)];
            GObject::Fighter * fgt = NULL;
            if(SKILL_ID(skill->getId()) == 137)
                fgt = globalFighters[5679];
            else if(SKILL_ID(skill->getId()) == 434)
                fgt = globalFighters[6011];
            else
                fgt = globalFighters[7006];
            if(fgt == NULL)
                break;
            BattleFighter * newf = new(std::nothrow) Battle::BattleFighter(_formula, fgt, side, pos);
            if(newf == NULL)
                break;
            newf->initStats(false);
            newf->setSideAndPos(side, pos);
            newf->addFlag(BattleFighter::IsMirror);
            setObject(side, pos, newf);
            appendDefStatus(e_Summon, fgt->getId(), newf);

            //FighterStatus fs(newf);
            // insert fighter into queue by order
            insertFighterStatus(newf);
        }

        if(bf->getBuddhaLightLast() == 0)
        {
            int nChangeAuraNum = -1*bf->getAura() + bf->getAuraLeft(); // 因为天赋术，hero无双之后会留一点灵力
            //setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, nChangeAuraNum, 0, false);
            setStatusChange_Aura2(bf, bf->getSide(), bf->getPos(), NULL, nChangeAuraNum, 0, false);
        }

        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false);

        if(bf->getBuddhaLightLast() > 0)
            initBuddhaLight(bf, true, false);

        return true;
    }

    UInt32 dmg = 0;

    GData::Area* area = NULL;
    area = &(GData::areaList[skill->area]);

    AttackPoint ap[25];
    int apcnt = 0;
    int apcnt2 = 0;

    if(skill->area != 0 && skill->area != 1 && area->getCount() > 0)
    {
        int x_ = target_pos % 5;
        int y_ = target_pos / 5;
        int cnt = area->getCount();
        int fsize = skill->factor.size();
        for(int i = 1; i < cnt; ++ i)
        {
            GData::Area::Data& ad = (*area)[i];
            int x = x_ + ad.x;
            int y = y_ + ad.y;
            if(x < 0 || x > 4 || y < 0 || y > 4)
            {
                continue;
            }
            ap[apcnt].pos = x + y * 5;
            int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
            if(fsize != 0)
                ap[apcnt ++].factor = skill->factor[idx];
            else
                ap[apcnt ++].factor = 1;
        }

        // 检查是否有改变技能范围的符文
        if(ss != NULL)
        {
            apcnt2 = apcnt;
            const std::vector<const GData::SkillStrengthenEffect*>& efs = ss->effect;
            for(size_t j = 0; j < efs.size(); ++ j)
            {
                if(efs[j]->type != GData::TYPE_AREA)
                    continue;
                GData::Area* area = NULL;
                area = &(GData::areaList[efs[j]->area]);
                int cnt = area->getCount();
                for(int i = 1; i < cnt; ++ i)
                {
                    GData::Area::Data& ad = (*area)[i];
                    int x = x_ + ad.x;
                    int y = y_ + ad.y;
                    if(x < 0 || x > 4 || y < 0 || y > 4)
                    {
                        continue;
                    }
                    ap[apcnt2].pos = x + y * 5;
                    ap[apcnt2 ++].factor = efs[j]->value/100;
                }
                break;
            }
        }
    }

    bool specialEf = true;  // 鹊桥技能需要特殊处理
    bool isQueqiao = false;
    bool bQueqiaoState = false;  // 鹊桥技能，是否主目标上buf
    if(SKILL_ID(skill->getId()) == 147 || SKILL_ID(skill->getId()) == 148 || SKILL_ID(skill->getId()) == 149)
    {
        isQueqiao = true;
        specialEf = false;
        int bfCnt = 1;
        assert(apcnt < 6 && skill->factor.size() == 7); // 必须有足够多的技能伤害因子，否则会出错
        for(int i = 0; i < apcnt; ++ i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
            if(!bo || bo->getHP() == 0)
                continue;
            // 鹊桥技能按照主次目标的顺序确定技能伤害因子
            ap[i].factor = skill->factor[i+1];
            ++ bfCnt;
        }
        if(bfCnt == 2)
            specialEf = true;

        if(specialEf)  // 如果目标两人，伤害因子统一用factor[6]
        {
            for(int i = 0; i < apcnt; ++i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                if(!bo || bo->getHP() == 0)
                    continue;
                ap[i].factor = skill->factor[6];
            }
            bQueqiaoState = (uRand(10000) > 5000) ? true : false;
        }
        //#ifdef _DEBUG
        //        fprintf(stderr, "specialEf = %d, isQueqiao = %d, bmainstate = %d\n", specialEf, isQueqiao, bQueqiaoState);
        //#endif
    }

    bool cs = false;
    bool pr = false;
    bool first = true;

    if(ss)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_WEAK, GData::TYPE_IMMUNE);
        if(ef)
            doSkillStrengthen_week(bf, skill, ef, bf->getSide(), bf->getPos(), true);

        ef = ss->getEffect(GData::ON_ATTACKSINGLE, GData::TYPE_INTENSIFYSTATE);
        if(ef && (skill->area != 0 && skill->area != 1)) // 群体攻击技能并且只中一个目标并且有这个类型的加强（apcnt2扩展目标没算）
        {
            int navailable = 1;
            for(int i = 0; i < apcnt; ++i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                if(!bo || bo->getHP() <= 0)
                    continue;
                ++ navailable;
            }
            if(navailable == 1)
                bf->setSingleAttackFlag(true);
        }
    }
    // 免疫降灵气
    //bool dostatus = true;
    //if(dostatus)
    if(specialEf)
    {
        bool self = false;
        bool flag = ((target_side == bf->getSide()) ? false : true);
        if(1 == skill->area)
        {
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, pos);
                doSkillStatus(flag, bf, skill, target_side, pos, 1, self, ifDecAura);
            }
        }
        else if( 0 == skill->area )
        {
            bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos);
            doSkillStatus(flag, bf, skill, target_side, target_pos, 1, self, ifDecAura);
        }
        else if(isQueqiao)  // 伤害两个人的鹊桥技能，随机一个人中状态
        {
            bool bDecAura = false;
            if(bQueqiaoState)  // 主目标
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                     bDecAura = isImmuneDecAura(skill, target_side, target_pos);
                     doSkillStatus(flag, bf, skill, target_side, target_pos, 1, self, bDecAura);
                }
            }
            else // 分目标
            {
                // 这里虽然写了for循环，但是肯定只有一个活人。。。
                for(int i = 0; i < apcnt; ++i)
                {
                    bDecAura = isImmuneDecAura(skill, target_side, ap[i].pos);
                    doSkillStatus(flag, bf, skill, target_side, ap[i].pos, 1, self, bDecAura);
                }
            }
        }
        else
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos);
                doSkillStatus(flag, bf, skill, target_side, target_pos, 1, self, ifDecAura);
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, ap[i].pos);
                doSkillStatus(flag, bf, skill, target_side, ap[i].pos, 1, self, ifDecAura);
            }

        }
    }

    // therapy skill
    if(skill->effect->hp > 0 || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
    {
        UInt32 rhp = calcTherapy(bf, cs, first, skill);
        UInt32 maxRhp = calcMaxTherapy(bf, skill);
        UInt32 hpr_first = 0;
        // 释、普渡慈航
        if(SKILL_ID(skill->getId()) == 18)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                return false;

            int cnt = 3;
            UInt8 excepts[25] = {0};
            size_t exceptCnt = 0;
            bool isFirst = true;
            do
            {
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 rhp2 = rhp * skill->factor[3-cnt];
                    float deFactor = calcTherapyFactor(bo);
                    UInt32 hpr = bo->regenHP(rhp2 * deFactor, skill->cond == GData::SKILL_ACTIVE);
                    if(hpr != 0)
                    {
                        if(hpr_first == 0)
                            hpr_first = hpr;
                        excepts[exceptCnt] = bo->getPos();
                        ++ exceptCnt;
                        appendDefStatus(e_damHpAdd, hpr, bo);
                        if(skill->cond == GData::SKILL_ACTIVE)
                            releaseWeak(bo);
                        onHPChanged(bo);
                    }
                }

                bo = getTherapyTarget3(bf, excepts, exceptCnt, isFirst);
                isFirst = false;

            }while(--cnt);
        }
        else if(bf->getSide() != target_side)
        {
            float deFactor = calcTherapyFactor(bf);
            UInt32 hpr = bf->regenHP(rhp * deFactor, skill->cond == GData::SKILL_ACTIVE);

            if(hpr != 0)
            {
                hpr_first = hpr;
                appendDefStatus(e_damHpAdd, hpr, bf);
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bf);
                onHPChanged(bf);
            }
        }
        else if(1 == skill->area)
        {
            GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_ZXBJ, GData::TYPE_ZXBJ_HP_ADD);

            int i = 0;
            int fsize = skill->factor.size();
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                if(bo->getLostHP() == 0)
                    continue;
                if(bo->isLingQu() || bo->isSoulOut())
                    continue;

                float factor = 1;
                int idx = std::min(abs(fsize-1), i);
                if(fsize > 0)
                    factor = skill->factor[idx];

                float deFactor = calcTherapyFactor(bo);
                UInt32 hpr = bo->regenHP(rhp * factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                ++i;
                if(hpr == 0)
                    continue;
                if(hpr_first == 0 && target_pos == pos)
                    hpr_first = hpr;

                appendDefStatus(e_damHpAdd, hpr, bo);
                onHPChanged(bo);
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bo);

                if(ef)
                {
                    bo->setPeerLessDisableSSLast(bf->getPos(), ef->last);
                    bo->setPeerLessDisableSSHP(bf->getPos(), hpr * ef->value / 100);
                }
            }
        }
        else if(0 == skill->area)
        {
            // 暂时假定只有单体技能能按照百分比回血 && 暂时假定只有单体技能能有加强治疗效果的技能符文
            BattleFighter* bo;
            // 回春术
            if(SKILL_ID(skill->getId()) == 11)
            {
                UInt8 excepts[25] = {0};
                size_t exceptCnt = 0;
                bo = getTherapyTarget2(bf, excepts, exceptCnt);
            }
            else
                bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                return false;

            float deFactor = calcTherapyFactor(bo);

            // 技能符文对治疗效果的加成
            ModifyTherapy_SkillStrengthen(bf, skill, deFactor, true);

            UInt32 hpr = bo->regenHP(rhp * deFactor, skill->cond == GData::SKILL_ACTIVE, skill->effect->hppec, maxRhp);
            if(hpr != 0)
            {
                hpr_first = hpr;
                appendDefStatus(e_damHpAdd, hpr, bo);
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bo);
                onHPChanged(bo);
            }

            // 技能符文在回血时同时回灵
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_AURA_GET);
            if (ef)
                setStatusChange(bo, bo->getSide(), bo->getPos(), 1, skill, e_stAura, ef->value, 0, false);

        }
        else
        {
            int fsize = skill->factor.size();
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                float factor = 1;
                if(fsize > 0)
                    factor = skill->factor[0];
                float deFactor = calcTherapyFactor(bo);
                UInt32 hpr = bo->regenHP(rhp * factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                if(hpr != 0)
                {
                    hpr_first = hpr;
                    appendDefStatus(e_damHpAdd, hpr, bo);
                    if(skill->cond == GData::SKILL_ACTIVE)
                        releaseWeak(bo);
                    onHPChanged(bo);
                }
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                if(NULL == bo)
                    continue;

                float deFactor = calcTherapyFactor(bo);
                UInt32 hpr = bo->regenHP(rhp * ap[i].factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                if(hpr != 0)
                {
                    appendDefStatus(e_damHpAdd, hpr, bo);
                    if(skill->cond == GData::SKILL_ACTIVE)
                        releaseWeak(bo);
                    onHPChanged(bo);
                }
            }
        }

        if(hpr_first && skill->cond == GData::SKILL_ACTIVE)
        {
            for(size_t i = 0; i < _onTherapy.size(); ++ i)
            {
                BattleFighter* bo = _onTherapy[i];
                if(!bo || bo->getHP() == 0)
                    continue;
                const GData::SkillBase* pskill = bo->getPassiveSkillOnTherapy();
                if(!pskill)
                    continue;
                if(SKILL_ID(pskill->getId()) == 513 || bo->getSide() == bf->getSide())
                {
                    UInt32 hpr = bo->regenHP(hpr_first * pskill->effect->hpP, false);
                    if(hpr != 0)
                    {
                        appendDefStatus(e_skill, pskill->getId(), bo);
                        appendDefStatus(e_damHpAdd, hpr, bo);
                        onHPChanged(bo);
                    }
                }
            }
        }

        // 一次有效的治疗
        if(rhp > 0)
        {
            if (bf->getMagAtkSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_MagAtk, 1);
            if (bf->getMagAtkDecSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_DecMagAtk, 1);
        }
    }
    else if( (!(skill->effect->state & GData::e_state_poison)) && (skill->effect->hp < 0 || skill->effect->addhp < 0 || skill->effect->hpP < -0.001) )
    {
        if(bf->getSide() != target_side)
        {
            if(1 == skill->area)
            {
                int i = 0;
                int fsize = skill->factor.size();
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    UInt32 dmg = abs(calcPoison(bf, skill, bo, false));
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        dmg *= skill->factor[idx];
                    ++i;
                    makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(calcPoison(bf, skill, bo, false));
                    makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                }
            }
            else
            {
                int fsize = skill->factor.size();
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(calcPoison(bf, skill, bo, false));
                    if(fsize > 0)
                        dmg *= skill->factor[0];
                    makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    UInt32 dmg = abs(calcPoison(bf, skill, bo, false)) * ap[i].factor;
                    makeDamage(bo, dmg, e_damNormal, e_damagePoison);
                }
            }
        }
    }

    // 驱散
    if(skill->effect->disperse > 0)
    {
        float rate = skill->prob * 100;
        if(bf->getSide() == target_side && ((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                appendDefStatus(e_Disperse, 0, bf);
                if(skill->effect->disperse & GData::e_state_confuse && SKILL_LEVEL(skill->getId()) >= bo->getConfuseLevel())
                {
                    bo->setConfuseLevel(0);
                    bo->setConfuseRound(0);
                    appendDefStatus(e_UnConfuse, 0, bo);
                }

                if(skill->effect->disperse & GData::e_state_stun && SKILL_LEVEL(skill->getId()) >= bo->getStunLevel())
                {
                    bo->setStunLevel(0);
                    bo->setStunRound(0);
                    appendDefStatus(e_UnStun, 0, bo);
                }

                if(skill->effect->disperse & GData::e_state_forget && SKILL_LEVEL(skill->getId()) >= bo->getForgetLevel())
                {
                    bo->setForgetLevel(0);
                    bo->setForgetRound(0);
                    appendDefStatus(e_UnForget, 0, bo);
                }

                // 减治疗效果改成攻击力一样处理，不能被驱散了。。。
                //if(skill->effect->disperse & 0x20 && SKILL_LEVEL(skill->getId()) >= bo->getWeakLevel())
                //{
                //    bo->setWeakLevel(0);
                //    bo->setWeakRound(0);
                //}
            }
        }
    }

    // 伤害技能
    if(skill->effect->state & GData::e_state_poison || skill->effect->damage || skill->effect->damageP || skill->effect->adddam
            || skill->effect->magdam || skill->effect->magdamP || skill->effect->addmag
            || skill->effect->crrdam || skill->effect->crrdamP || skill->effect->addcrr)
    {
        if(SKILL_ID(skill->getId()) == 168)
        {
            static UInt8 skill_prob_168[4] = {0,  0,  50,  100};
            UInt8 cnt;
            UInt8 roll = _rnd(100);
            if(skill_prob_168[0] > roll)
                cnt = 0;
            else if(skill_prob_168[1] > roll)
                cnt = 1;
            else if(skill_prob_168[2] > roll)
                cnt = 2;
            else if(skill_prob_168[3] > roll)
                cnt = 3;
            else
                cnt = 0;

            int fsize = skill->factor.size();
            for(int i = 0; i < cnt + 1; ++ i)
            {
                UInt8 pos = target_pos;
                UInt8 x = pos / 5;
                UInt8 y = pos % 5;
                float factor = 1;

                if(i == cnt)
                {
                    dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos), factor, 0, NULL, atkAct, canProtect);
                    canProtect = false;
                }
                else
                    dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos), factor);

                if(fsize > 1)
                    factor = skill->factor[1];
                else
                    factor = 0.3;
                //UInt16 index = _rnd(10000);
                //if(index < 3000) //后方
                if(1)
                {
                    if(x < 4)
                    {
                        dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos + 5), factor);
                    }
                }
                //else if(index < 6500) //左右两侧
                if(1)
                {
                    if(y > 0)
                    {
                        dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos - 1), factor);
                    }
                    if(y < 4)
                    {
                        dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos + 1), factor);
                    }
                }
                //else //左后右后
                if(1)
                {
                    if(y > 0 && x < 4)
                    {
                        dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos + 4), factor);
                    }
                    if(y < 4 && x < 4)
                    {
                        dmg += attackByJiuzi(bf, first, cs, pr, skill, getObject(target_side, pos + 6), factor);
                    }
                }
            }
            attackByJiuziSS(bf, skill);
        }
        //儒、元磁神雷
        else if(SKILL_ID(skill->getId()) == 122 || SKILL_ID(skill->getId()) == 477)
        {
            static UInt8 skill_prob_122[10][4] = {
                {0, 0, 0},
                {70, 100, 0, 0},
                {60, 100, 0, 0},
                {50, 100, 0, 0},
                {40, 100, 0, 0},
                {30, 90,  100, 0},
                {20, 80,  100, 0},
                {20, 60,  90,  100},
                {10, 40,  80,  100},
                {0,  20,  70,  100}
            };
            UInt8 cnt = 0;
            UInt8 level = SKILL_LEVEL(skill->getId());
            if(level > 9)
                level = 9;
            UInt8 roll = _rnd(100);
            if(skill_prob_122[level][0] > roll)
                cnt = 0;
            else if(skill_prob_122[level][1] > roll)
                cnt = 1;
            else if(skill_prob_122[level][2] > roll)
                cnt = 2;
            else if(skill_prob_122[level][3] > roll)
                cnt = 3;
            for(int i = 0; i < cnt; ++ i)
            {
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), 1);
            }
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), 1, 0, NULL, atkAct, canProtect);
            canProtect = false;
            if(cnt+1 >= 3) // 三次以上有特殊效果
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                AddYuanCiState_SkillStrengthen(bf, bo, skill, cnt+1);
            }
        }
        //道、万剑诀
        else if(SKILL_ID(skill->getId()) == 27 || SKILL_ID(skill->getId()) == 498)
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 0;
            for(int i = 0; i < 25; i++)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, i));
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;
                if(bo->isSoulOut())
                {
                    excepts[exceptCnt] = bo->getPos();
                    ++exceptCnt;
                }
            }

            UInt8 dmgCount[25] = {0};
            int cnt = 8;
            for(int i = 0; i < cnt; ++ i)
            {
                BattleFighter* rnd_bf = getRandomFighter(target_side, excepts, exceptCnt);
                dmg += attackOnce(bf, first, cs, pr, skill, rnd_bf, 1);
                if(rnd_bf)
                    ++ dmgCount[rnd_bf->getPos()];
            }
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), 1, 0, NULL, atkAct, canProtect);
            canProtect = false;
            ++ dmgCount[target_pos];
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_DEBUF_THERAPY);
            if(ef)
            {
                for(int i = 0; i < 25; ++ i)
                {
                    if(dmgCount[i] < 3)
                        continue;

                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, i));
                    //if(bo->getHP() == 0 && bo->getTherapyDecLast() != 0)
                        //continue;
                    if (bo->getHP() == 0)
                        continue;

                    //defList[defCount].damType = e_neishan;
                    if(bo->getTherapyBuff() != 0)  // 去掉以前的buff
                    {
                        if (bo->getTherapyBuff() > 0)
                            appendDefStatus(e_unTherapyBuff, 0, bo);
                        else
                            appendDefStatus(e_UnWeak, 0, bo);
                    }

                    // 减治疗效果以后就叫e_Weak，增加就是 e_TherapyBuff，e_neishang废除了
                    appendDefStatus(e_Weak, 0, bo);
                    //bo->setTherapyDec(ef->value/100, ef->last);
                    bo->setTherapyBuff(-ef->value/100, ef->last);
                }
            }
        }
        //仙宠、十字攻击，两次伤害则混乱（先选取主目标横排攻击，再选取剩下的主目标纵排攻击）(火龙地狱)
        else if(SKILL_ID(skill->getId()) == 53 || SKILL_ID(skill->getId()) == 70 || SKILL_ID(skill->getId()) == 77|| SKILL_ID(skill->getId()) == 631 )
        {
            AtkList atklist;
            getAtkList(bf, skill, atklist);
            size_t cnt = atklist.size();
            std::set<BattleFighter*> hitList;
            bool first = true;
            for(size_t i = 0; i < cnt; ++ i)
            {
                UInt32 tmpDmg = 0;
                BattleFighter* bo = atklist[i].bf;
                float factor = atklist[i].factor;
                tmpDmg = attackOnce(bf, first, cs, pr, skill, bo, factor, first?0:-1, NULL, atkAct, first && canProtect);
                canProtect = false;
                first = false;
                if (tmpDmg)
                {
                    hitList.insert(bo);
                }
                dmg += tmpDmg;
            }

            atklist.clear();
            getAtkList(bf, skill, atklist, 1);
            cnt = atklist.size();
            for(size_t i = 0; i < cnt; ++ i)
            {
                UInt32 tmpDmg = 0;
                BattleFighter* bo = atklist[i].bf;
                float factor = atklist[i].factor;
                tmpDmg = attackOnce(bf, first, cs, pr, skill, bo, factor, -1, NULL, atkAct);
                if(tmpDmg && hitList.find(bo) != hitList.end())
                {
                    const GData::SkillStrengthenEffect* ef = NULL;
                    if(ss)
                        ef = ss->getEffect(GData::ON_CONFUSE, GData::TYPE_DAMAG_A);
                    if(ef)
                    {
                        bo->setConfuseLevel(SKILL_LEVEL(skill->getId()));
                        if(skill->cond == GData::SKILL_BEATKED)
                            bo->setConfuseRound(skill->last + 1);
                        else
                            bo->setConfuseRound(skill->last);

                        bo->setDeepConfuseDmgExtra(ef->value/100, ef->last);
                        appendDefStatus(e_deepConfuse, 0, bo);
                    }
                    else
                    {
                        bo->setConfuseRound(1);
                        appendDefStatus(e_Confuse, 0, bo);
                    }
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                }
                dmg += tmpDmg;
            }
        }
        //仙宠、两次单体攻击，两次伤害则眩晕 (天崩地裂)
        else if(SKILL_ID(skill->getId()) == 48 || SKILL_ID(skill->getId()) == 67 || SKILL_ID(skill->getId()) == 92 || SKILL_ID(skill->getId()) == 640)
        {
            UInt32 dmg1 = 0;
            UInt32 dmg2 = 0;
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            dmg1 = attackOnce(bf, first, cs, pr, skill, bo, 1, 0, NULL, atkAct, canProtect);
            canProtect = false;
            dmg += dmg1;
            if (bf->getHP() > 0 && _winner == 0)
            {
                dmg2 = attackOnce(bf, first, cs, pr, skill, bo, 1, -1, NULL, atkAct, canProtect);
                dmg += dmg2;
                if (dmg1 && dmg2)
                {
                    bo->setStunRound(1);
                    appendDefStatus(e_Stun, 0, bo);
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                    const GData::SkillStrengthenEffect* ef = NULL;
                    if(ss)
                        ef = ss->getEffect(GData::ON_STUN, GData::TYPE_DAMAG_A);
                    if(ef)
                    {
                        bo->setStunLevel(SKILL_LEVEL(skill->getId()));
                        bo->setStunRound(skill->last + 1);

                        bo->setDeepStunDmgExtra(ef->value/100, ef->last);
                        appendDefStatus(e_deepStun, 0, bo);
                        calcAbnormalTypeCnt(bo);
                        calcSilkwormCnt(bo);
                    }
                }
            }
        }
        //儒、青莲剑歌
        else if(SKILL_ID(skill->getId()) == 9)
        {
            int idx = 0;
            for(int i = 0; i < apcnt2; ++ i)
            {
                BattleObject * area_target_obj = getObject(target_side, ap[i].pos);
                if(area_target_obj == NULL || area_target_obj->getHP() == 0 || bf == NULL)
                    continue;
                ++ idx;
            }

            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > idx)
                factor = skill->factor[idx];
            else
                factor = skill->factor[fsize-1];

            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), factor, apcnt, ap, atkAct, canProtect);
            canProtect = false;
            for(int j = 0; j < apcnt; ++ j)
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, ap[j].pos), factor);
            for(int k = apcnt; k < apcnt2; ++ k)
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, ap[k].pos), factor*ap[k].factor);
        }
        // 单体技能
        else if(0 == skill->area)
        {
            size_t hitcnt = 1 + getSkillEffectExtraHitCnt(bf, static_cast<BattleFighter*>(getObject(target_side, target_pos)), skill);
            for(size_t idx = 0; idx < hitcnt; ++ idx)
            {
                float factor = 1.0f;
                UInt32 dmg2 = 0;
                first = true;
                bool cs2 = false;
                bool pr2 = false;
                if(idx < skill->factor.size())
                    factor = skill->factor[idx];
                if (ss)
                {
                    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_INTENSIFYSTATE);
                    if(ef)
                            factor += ef->value / 100;
                }
                int count_deny = 0;
                if (skill->cond == GData::SKILL_ENTER)
                    count_deny = -1;
                if(idx == 0)
                {
                    dmg2 = attackOnce(bf, first, cs2, pr2, skill, getObject(target_side, target_pos), factor, count_deny, NULL, atkAct, canProtect);
                    canProtect = false;
                    cs = cs2;
                    pr = pr2;
                }
                else
                {
                    dmg2 = attackOnce(bf, first, cs2, pr2, skill, getObject(target_side, target_pos), factor, -1, NULL, NULL, canProtect);
                    canProtect = false;
                }

                dmg += dmg2;
                doSkillEffectExtraAbsorb(bf, dmg2, skill);
                doSkillStrenghtenTherapyAnotherMore(bf, dmg2, skill, ss);

                if(dmg2 > 0)
                    doSkillStrenghtenCriticalDamageAroundOne(bf, skill, cs2, ss, target_side, target_pos);
                dmg += doSkillStrenghtenCriticalPierceDmgB(bf, skill, cs2, pr2, ss, target_side, target_pos);
            }
            doSkillStrenghtenHitConfuse(bf, skill, ss, target_side, target_pos);

            BattleFighter* ptarget = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(ptarget && ptarget->getHP() <= 0)
            {
                if (ptarget->getHP() <= 0)  // beat to death!!!
                    bf->setMainTargetDeadFlag(true);
            }
        }
        // 群体技能
        else if(1 == skill->area)
        {
            int i = 0;
            int fsize = skill->factor.size();
            bool silkwormFlag = false;
            for(UInt8 pos = 0; pos < 25; pos++)
            {
                if(getObject(target_side, pos) == NULL || getObject(target_side, pos)->getHP() == 0)
                    continue;
                float factor = 1;
                int idx = std::min(abs(fsize-1), i);
                if(fsize > 0)
                    factor = skill->factor[idx];
                if(skill->effect && skill->effect->eft[0] ==  GData::e_eft_silkworm)
                {
                    doAbnormalStatusClear(getObject(target_side, pos));
                    float factor2 = 5.0;
                    if(skill->effect)
                        factor2 = skill->effect->efv[0] / 100.f;
                    factor *= (1.0 + factor2 * bf->getSilkwormCnt());
                    silkwormFlag = true;
                }
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, pos), factor, -1, NULL, NULL, canProtect);
                canProtect = false;
                doSkillEffectExtraAbsorb(bf, dmg, skill);
                ++i;
            }
            if(silkwormFlag)
                bf->setSilkwormCnt(0);

            GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_YE_HUO);
            if(ef)
            {
                BattleFighter* bo = getRandomFighter(1 - bf->getSide(), NULL, 0);
                if(bo)
                {
                    UInt8 yehuoLeve = bo->getYehuoLevel();
                    float upRate = ef->value / 100.0f;
                    float dmgRate = ef->valueExt1;
                    bo->setYehuoSSUpRate(upRate);
                    bo->setYehuoSSDmgRate(dmgRate * bf->getAttack());
                    if(yehuoLeve < 9)
                    {
                        yehuoLeve += 1;
                        bo->setYehuoLevel(yehuoLeve);
                        appendDefStatus(e_yehuoBleedMo, yehuoLeve, bo);
                    }
                }
            }
        }
        else if(specialEf && isQueqiao)
        {
            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > 0)
                factor = skill->factor[fsize-1];
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), factor, apcnt, ap, atkAct, canProtect);
            canProtect = false;
            for(int i = 0; i < apcnt; ++ i)
            {
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, ap[i].pos), factor);
            }
        }
        else
        {
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_SPREAD, GData::TYPE_XINMO);
            // attack the target on center
            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > 0)
                factor = skill->factor[0];

            bf->setAttackIndex(1); // 标记是第一个目标，供后面使用，哭泣。。。

            UInt32 tmpDmg = 0;
            tmpDmg = attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), skill->factor[0], apcnt, ap, atkAct, canProtect);
            dmg += tmpDmg;
            canProtect = false;
            doSkillEffectExtraAbsorb(bf, dmg, skill);
            // 给主目标及相关目标上相关技能符文效果
            if (ss)
            {
                const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_FIELD_GAPE);
                if (ef)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if (bo->isChar())
                    {
                        UInt8 type = bo->getPos();
                        if (type >= target_pos)
                            type = (type - target_pos) / 5;

                        switch(type)
                        {
                            case 0:
                                bo->setBleedFieldGape(bf->getAttack() * ef->value / 100, ef->last, ef->valueExt1, type);
                                appendDefStatus(e_bleedFieldGape, 0, bo);
                                break;
                            case 1:
                                bo->setBleedFieldGape(bf->getAttack() * ef->value / 100, ef->last, ef->valueExt1, type);
                                appendDefStatus(e_bleedFieldGape2, 0, bo);
                                break;
                            case 2:
                                bo->setBleedFieldGape(bf->getAttack() * ef->value / 100, ef->last, ef->valueExt1, type);
                                appendDefStatus(e_bleedFieldGape3, 0, bo);
                                break;
                        }
                    }
                }
            }
            int nindex = 2;
            for(int i = 0; i < apcnt; ++ i)
            {
                BattleFighter* ptarget = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                if(!ptarget || ptarget->getHP() == 0)  // 活人才需要打，死人略过
                    continue;
                bf->setAttackIndex(nindex++);  // 依次第二、第三记录下来
                tmpDmg = attackOnce(bf, first, cs, pr, skill, getObject(target_side, ap[i].pos), ap[i].factor);
                dmg += tmpDmg;
                doSkillEffectExtraAbsorb(bf, dmg, skill);

                // 给主目标及相关目标上相关技能符文效果
                if (ss)
                {
                    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_FIELD_GAPE);
                    if (ef)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                        if (bo->isChar())
                        {
                            UInt8 type = bo->getPos();
                            if (type >= target_pos)
                                type = (type - target_pos) / 5;

                            switch(type)
                            {
                                case 0:
                                    bo->setBleedFieldGape(bf->getAttack() * ef->value / 100, ef->last, ef->valueExt1, type);
                                    appendDefStatus(e_bleedFieldGape, 0, bo);
                                    break;
                                case 1:
                                    bo->setBleedFieldGape(bf->getAttack() * ef->value / 100, ef->last, ef->valueExt1, type);
                                    appendDefStatus(e_bleedFieldGape2, 0, bo);
                                    break;
                                case 2:
                                    bo->setBleedFieldGape(bf->getAttack() * ef->value / 100, ef->last, ef->valueExt1, type);
                                    appendDefStatus(e_bleedFieldGape3, 0, bo);
                                    break;
                            }
                        }
                    }
                }
            }
            if(ef)
            {
                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bf = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(!bf || bf->getHP() == 0)
                        continue;
                    if(_rnd(10000) < ef->prob * 100)
                    {
                        doXinmoAttack(bf, getObject(target_side, target_pos));
                        break;
                    }
                }
            }
        }

        // 天劫:御雷神针、芭蕉巨扇、盘古神斧, 幽灵碧炎梭
        UInt16 skillId = SKILL_ID(skill->getId());
        if((skillId == 150) || (skillId == 151) || (skillId == 152) || (skillId == 160)
                || (skillId == 153) || (skillId == 154) || (skillId == 155) || (skillId == 161) )
        {
            float rate = skill->prob * 100;
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                int fsize = skill->factor.size();
                float factor = 1;
                if(fsize > 0)
                    factor = skill->factor[0];
                if((rate * factor) > _rnd(10000))
                {
                    if(skillId == 150 || skillId == 153)
                    {
                        UInt32 magdef = getBFMagDefend(bo);
                        UInt32 dmg = _formula->calcDamage(factor*getBFMagAtk(bf)*0.35f, magdef, bf->getLevel(), 1, 0);
                        bo->setAuraBleed(dmg, skill->last, 5);
                        appendDefStatus(e_Bleed1, 0, bo);
                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_AURA_RETURN);
                            if(ef)
                            {
                                bo->setAuraPresent(ef->value, ef->last);
                            }
                        }
                    }
                    else if(skillId == 151 || skillId == 154)
                    {
                        UInt32 magdef = getBFMagDefend(bo);
                        UInt32 dmg = _formula->calcDamage(factor*getBFMagAtk(bf)*0.35f, magdef, bf->getLevel(), 1, 0);
                        bo->setConfuceBleed(dmg, skill->last, 5);
                        appendDefStatus(e_Bleed4, 0, bo);
                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_DAMAG_A);
                            if(ef)
                            {
                                bo->setConfusePresent(ef->value, ef->last);
                            }
                        }
                    }
                    else if(skillId == 152 || skillId == 155)
                    {
                        UInt32 def = getBFDefend(bo);
                        UInt32 dmg = _formula->calcDamage(factor*getBFAttack(bf)*0.35f, def, bf->getLevel(), 1, 0);
                        bo->setStunBleed(dmg, skill->last, 5);
                        appendDefStatus(e_Bleed3, 0, bo);
                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_DAMAG_A);
                            if(ef)
                            {
                                bo->setStunPresent(ef->value, ef->last);
                            }
                        }
                    }
                    else
                    {
                        UInt32 def = getBFDefend(bo);
                        UInt32 dmg = _formula->calcDamage(factor*getBFAttack(bf)*0.35f, def, bf->getLevel(), 1, 0);
                        bo->setBlindBleed(dmg, skill->last, 5);
                        appendDefStatus(e_BleedMo, 0, bo);
                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_DAMAG_A);
                            if(ef)
                            {
                                bo->setBlindPresent(ef->value, ef->last);
                            }
                        }
                    }
                }
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                if(rate > _rnd(10000))
                {
                    if(skillId == 150 || skillId == 153)
                    {
                        UInt32 magdef = getBFMagDefend(bo);
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*getBFMagAtk(bf)*0.35f, magdef, bf->getLevel(), 1, 0);
                        bo->setAuraBleed(dmg, skill->last, 5);
                        appendDefStatus(e_Bleed1, 0, bo);
                        //calcAbnormalTypeCnt(bo);

                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_AURA_RETURN);
                            if(ef)
                            {
                                bo->setAuraPresent(ef->value, ef->last);
                            }
                        }
                    }
                    else if(skillId == 151 || skillId == 154)
                    {
                        UInt32 magdef = getBFMagDefend(bo);
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*getBFMagAtk(bf)*0.35f, magdef, bf->getLevel(), 1, 0);
                        bo->setConfuceBleed(dmg, skill->last, 5);
                        appendDefStatus(e_Bleed4, 0, bo);

                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_DAMAG_A);
                            if(ef)
                            {
                                bo->setConfusePresent(ef->value, ef->last);
                            }
                        }
                    }
                    else if(skillId == 152 || skillId == 155)
                    {
                        UInt32 def = getBFDefend(bo);
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*getBFAttack(bf)*0.35f, def, bf->getLevel(), 1, 0);
                        bo->setStunBleed(dmg, skill->last, 5);
                        appendDefStatus(e_Bleed3, 0, bo);
                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_DAMAG_A);
                            if(ef)
                            {
                                bo->setStunPresent(ef->value, ef->last);
                            }
                        }
                    }
                    else
                    {
                        UInt32 def = getBFDefend(bo);
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*getBFAttack(bf)*0.35f, def, bf->getLevel(), 1, 0);
                        bo->setBlindBleed(dmg, skill->last, 5);
                        appendDefStatus(e_BleedMo, 0, bo);
                        if(ss)
                        {
                            const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLEED, GData::TYPE_DAMAG_A);
                            if(ef)
                            {
                                bo->setBlindPresent(ef->value, ef->last);
                            }
                        }
                    }
                }
            }
        }

        if(skill->cond == GData::SKILL_ACTIVE)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                for(size_t i = 0; i < _onSkillDmg.size(); ++ i)
                {
                    BattleFighter* bf = _onSkillDmg[i];
                    if(!bf || bf->getHP() == 0)
                        continue;

                    UInt32 stun = bf->getStunRound();
                    UInt32 confuse = bf->getConfuseRound();
                    UInt32 forget = bf->getForgetRound();

                    if(stun > 0 || confuse > 0 || forget > 0)
                        continue;

                    if(bf->getSide() == bo->getSide())
                    {
                        continue;
                    }
                    const GData::SkillBase* pskill = bf->getPassiveSkillOnSkillDmg();
                    if(!pskill)
                        continue;

                    appendDefStatus(e_skill, pskill->getId(), bf);
                    float factor = 1.0f;
                    if(bf == _activeFgt)
                        factor = 1.0f;
                    else
                        factor = 5.0f/11;
                    dmg += attackOnce(bf, first, cs, pr, pskill, getObject(target_side, target_pos), factor);
                }
            }

        }
    }

    // 混乱晕眩封印状态
    if((skill->effect->state & GData::e_state_c_s_f_w_m_b) && specialEf)
    {
        if(bf->getSide() != target_side)
        {
            if(1 == skill->area)
            {
                int i = 0;
                int fsize = skill->factor.size();
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    float factor = 1;
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        factor = skill->factor[idx];
                    UInt32 dmg = 0;
                    doSkillState(bf, skill, bo, factor, 0, dmg, NULL, NULL);
                    ++ i;
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    std::vector<AttackAct> atkAct2;
                    atkAct2.clear();
                    UInt32 dmg = 0;
                    doSkillState(bf, skill, bo, 1, 0, dmg, &atkAct2, atkAct);
                    doSkillAtk2(false, &atkAct2);
                }
            }
            else if(isQueqiao) // 两个目标的鹊桥，随机一个上状态
            {
                int ntarpos = target_pos;
                BattleFighter* bo = NULL;
                if(bQueqiaoState) // 主目标上buf
                {
                    bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar()) // 无效的目标
                        bo = NULL;
                }
                else
                {
                    for(int i = 0; i < apcnt; ++ i)
                    {
                        bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                        if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                        {
                            ntarpos = ap[i].pos;
                            break; // 找到目标了
                        }
                    }
                }
               if(bo != NULL)
               {
                   std::vector<AttackAct> atkAct2;
                   atkAct2.clear();
                   UInt32 dmg = 0;
                   doSkillState(bf, skill, bo, 1, 0, dmg, &atkAct2, atkAct);
                   doSkillAtk2(false, &atkAct2);
#ifdef _DEBUG
                   //fprintf(stderr, "i beat you to dizz!! rate = %f\n", rate);
#endif
               }
            }
            else
            {
#ifdef _DEBUG
                //fprintf(stderr, "old rate = %f, new rate = %f\n", rate, nstateRate);
#endif

                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    int fsize = skill->factor.size();
                    float factor = 1;
                    if(fsize > 0)
                        factor = skill->factor[0];
                    std::vector<AttackAct> atkAct2;
                    atkAct2.clear();
                    UInt32 dmg = 0;
                    doSkillState(bf, skill, bo, factor, 0, dmg, &atkAct2, atkAct);
                    doSkillAtk2(false, &atkAct2);
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    std::vector<AttackAct> atkAct2;
                    atkAct2.clear();
                    UInt32 dmg = 0;
                    doSkillState(bf, skill, bo, ap[i].factor, 0, dmg, &atkAct2, atkAct);
                    doSkillAtk2(false, &atkAct2);
                }
            }
        }
    }

    if (skill && skill->cond == GData::SKILL_PEERLESS)
    {
        UInt8 last = bf->getBuddhaLightLast();
        if(last == 0 || (last > 0 && bf->getBlind() > 0.001f))
        {
            int nChangeAuraNum = -1*bf->getAura() + bf->getAuraLeft(); // 因为天赋术，hero无双之后会留一点灵力
            //setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, nChangeAuraNum, 0, false);
            setStatusChange_Aura2(bf, bf->getSide(), bf->getPos(), NULL, nChangeAuraNum, 0, false);
        }
        if (launchPeerLess)
            *launchPeerLess = 1;

        if(last > 0 && bf->getBlind() < 0.001f)
            initBuddhaLight(bf, true, false);
    }

    if(ss && bf->getHP() != 0)
    {
        doAllSkillStrengthenEffect(bf, skill, target_side, target_pos);
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_RANDOM_BLEED);
        if(ef) // 有随机流血的符文
        {
            // 找出所有有效目标，然后随机一个
            std::vector<UInt8> vPos;
            vPos.clear();
            BattleFighter* bo = NULL;
            //BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            //if(bo && bo->getHP() > 0)
                //vPos.push_back(target_pos);
            //for(int i = 0; i < apcnt; ++i)
            for(int pos = 0; pos < 25; ++ pos)
            {
                bo = static_cast<BattleFighter*>(getObject(target_side, pos));
                if(bo && bo->getHP() > 0)
                {
                    vPos.push_back(pos);
                }
            }
            if(vPos.size() > 0)
            {
                size_t index = _rnd(vPos.size()*10000)/10000;
                if(index >= vPos.size())
                    index = 0;  // 不知道会不会随机到最大那个数，保护一下。几万分之一，中了记得买彩票
                bo = static_cast<BattleFighter*>(getObject(target_side, vPos[index]));
                BleedRandom_SkillStrengthen(bf, bo, ef);
            }
        }
        
        // 主目标死亡，有符文要返还灵气
        if(bf->getMainTargetDeadFlag())
        {
            ef = ss->getEffect(GData::ON_TARGET_DEAD, GData::TYPE_AURA_GET);
            if(ef)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, ef->value, 0, false);
            }
        }
    }


    // 有给自己加的属性驻留在临时变量中，取出来，加上去
    if (bf->getHP() > 0)
    {
        float fAtk = 0;
        Int16 nLast = 0;
        int esstype = 0;
        bf->getSkillUsedChangeAttr(fAtk, nLast, esstype);
        if (fAtk > 0)
        {
            SpecialStatus etype = static_cast<SpecialStatus>(esstype);
            SetSpecialAttrChange(bf, skill, etype, nLast, fAtk, true);
        }
        bf->setSkillUsedChangeAttr(0, 0, 0);  // clear
    }

    bf->setSingleAttackFlag(false);  // 攻击完毕，取消此次造成的增加概率的效果
    bf->setMainTargetDeadFlag(false);

    doSkillEffectExtraAttack(bf, target_side, target_pos, skill);

    int self_side = bf->getSide() == target_side ? 25 : 0;
    appendToPacket( bf->getSide(), bf->getPos(), target_pos + self_side, 2, skill->getId(), cs, pr);
    return true;
}

bool BattleSimulator::doSkillStatus2(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt)
{
    // 入场buff类技能
    if(NULL == skill || bf == NULL || target_pos < 0)
        return false;

    if(skill->effect == NULL)
        return false;

    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(NULL == bo)
        return false;

    if(skill->effect->auraP || skill->effect->aura)
    {
        float rate = skill->prob * 100;
        if(rate > _rnd(10000) || (skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS))
        {
            float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
            if(value > 0 && bf->getSide() != target_side)
            {
                float value = bf->_aura * skill->effect->auraP + skill->effect->aura;
                setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAura, value, skill->last, bf->getSide() != 0);
            }
            else
            {
                setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAura, value, skill->last, target_side != 0);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAtk, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtk, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_defend * skill->effect->defP + skill->effect->def;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stDef, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stDef, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagAtk, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtk, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagDef, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagDef, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stTough, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stTough, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->action || skill->effect->actionP)
    {
        float value = bo->_maxAction * skill->effect->actionP + skill->effect->action;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_maxAction * skill->effect->actionP + skill->effect->action;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAction, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAction, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stEvade, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stEvade, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCritical, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCritical, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stPierce, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stPierce, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCounter, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCounter, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagRes, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagRes, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->hitrate)
    {
        float value = skill->effect->hitrate;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stHitRate, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stHitRate, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->atkreduce)
    {
        float value = skill->effect->atkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAtkReduce, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtkReduce, value, skill->last, target_side != 0);
        }
    }

    if(skill->effect->magatkreduce)
    {
        float value = skill->effect->magatkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagAtkReduce, value, skill->last, bf->getSide() != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtkReduce, value, skill->last, target_side != 0);
        }
    }

    return true;
}

bool BattleSimulator::doSkillStatus(bool activeFlag, BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, bool& self, bool ifDecAura)
{
    if(NULL == skill || bf == NULL || target_pos < 0)
        return false;
    if(skill->effect == NULL)
        return false;

    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(NULL == bo)
        return false;

    bool tmpself = self;
    if(skill->effect->auraP || skill->effect->aura)
    {
        if((SKILL_ID(skill->getId()) == 136 || SKILL_ID(skill->getId()) == 146) && bf->getClass() != e_cls_shi) // 光棍, 冰火双剪
            return false;

        float rate = skill->prob * 100;
        //可能是受伤害触发的被动技能，概率已经在受伤害时计算过，这里100%生效
        if((skill->cond == GData::SKILL_ONGETDMG) || rate > _rnd(10000))
        {
            float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
            if(value > 0 && bf->getSide() != target_side)
            {
                if(!self)
                {
                    float value = bf->_aura * skill->effect->auraP + skill->effect->aura;

                    float ssfactor = 0.0f;
                    ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 提升增加灵气效果
                    if (ssfactor != 0)
                        value *= (1 + ssfactor);
                    setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAura, value, skill->last, !activeFlag);
                    tmpself = true;
                }
            }
            else if(!ifDecAura)
            {
                if(value < 0)
                {
                    ModifySingleAttackValue_SkillStrengthen(bf, skill, value, false);
                    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
                    if(ss)
                    {
                        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_AURA_RETURN);
                        if(ef)
                        {
                            std::vector<UInt8> bf_company;
                            bf_company.reserve(25);
                            for(UInt8 i=0; i<25; ++i)
                            {
                                if(i == bf->getPos())
                                    continue;
                                BattleFighter* fighter = static_cast<BattleFighter*>(getObject(bf->getSide(), i));
                                if(fighter && fighter->getHP()>0)
                                    bf_company.push_back(i);
                            }
                            if(bf_company.size() > 0)
                            {
                                float targetvalue = bo->getAura();
                                if(targetvalue+value >= 0)  // 扣除的灵气比拥有的少
                                    targetvalue = -value;
                                float avevalue = targetvalue*ef->value/100/bf_company.size();
                                for(auto p=bf_company.begin(),e=bf_company.end(); p!=e; ++p)
                                    setStatusChange(bf, bf->getSide(), *p, 1, skill, e_stAura, avevalue, skill->last, !activeFlag);
                            }                            
                        }
                    }
                }
                else
                {
                    float ssfactor = 0.0f;
                    ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 提升增加灵气效果
                    if (ssfactor != 0)
                        value *= (1 + ssfactor);
                }

                setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAura, value, skill->last, activeFlag);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        if(bo->getColorStock() != 0)
            return false;
        if(bo->isLingQu() || bo->isSoulOut())
            return false;
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
                float ssfactor = 0.0f;
                ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 提升增加攻击力效果
                if (ssfactor != 0)
                    value *= (1 + ssfactor);
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAtk, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            float ssfactor = 0.0f;
            ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 提升增加攻击力效果
            if (ssfactor != 0)
                value *= (1 + ssfactor);
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAtk, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_defend * skill->effect->defP + skill->effect->def;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stDef, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            if(value < 0)
            {
                float ssfactor = 0.0f;
                ModifySingleAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 增加减防效果, ssfactor取出来的是正数（表格不能填负数。。。所以看下面是减去这个因子>_<）
                if (ssfactor != 0)
                {
                    value = bo->_defend * (skill->effect->defP - ssfactor/100) + skill->effect->def;  // 强化因子改了，重新算
                }
            }
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stDef, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        if(bo->getColorStock() != 0)
            return false;
        if(bo->isLingQu() || bo->isSoulOut())
            return false;
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
                float ssfactor = 0.0f;
                ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 提升增加攻击力效果
                if (ssfactor != 0)
                    value *= (1 + ssfactor);

                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagAtk, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            float ssfactor = 0.0f;
            ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 提升增加攻击力效果
            if (ssfactor != 0)
                value *= (1 + ssfactor);

            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagAtk, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagDef, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            if(value < 0)
            {
                float ssfactor = 0.0f;
                ModifySingleAttackValue_SkillStrengthen(bf, skill, ssfactor, true);  // 增加减防效果
                if (ssfactor != 0)
                {
                    value = bo->_magdef * (skill->effect->magdefP - ssfactor/100) + skill->effect->magdef;
                }
            }
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagDef, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stTough, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stTough, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->action || skill->effect->actionP)
    {
        float value = bo->_maxAction * skill->effect->actionP + skill->effect->action;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_maxAction * skill->effect->actionP + skill->effect->action;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAction, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAction, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stEvade, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stEvade, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stCritical, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stCritical, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stPierce, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stPierce, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stCounter, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stCounter, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagRes, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagRes, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->hitrate && !(skill->effect->state & GData::e_state_blind))
    {
        float value = skill->effect->hitrate;
        UInt8 last = skill->last;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stHitRate, value, last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stHitRate, value, last, activeFlag);
        }
    }

    if(skill->effect->atkreduce)
    {
        float value = skill->effect->atkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAtkReduce, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAtkReduce, value, skill->last, activeFlag);
        }
    }

    if(skill->effect->magatkreduce)
    {
        float value = skill->effect->magatkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagAtkReduce, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagAtkReduce, value, skill->last, activeFlag);
        }
    }

    // 存在技能符文
    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(ss)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_ATK_RETURN);
        if(ef)
        {
            float maxatk = 0;
            UInt8 maxatk_pos = 0;
            UInt8 maxatk_class = 0;
            BattleFighter* fighter = NULL;
            for(UInt8 i=0; i<25; ++i)
            {
                //if( i == bf->getPos())  // 除自己以外的人
                    //continue;

                fighter = static_cast<BattleFighter*>(getObject(bf->getSide(), i));
                if (fighter && fighter->getHP() > 0)
                {
                    float fatkvalue = 0;
                    UInt8 nClass = fighter->getClass();
                    if (nClass == e_cls_ru || nClass == e_cls_shi)
                        fatkvalue = getBFMagAtk(fighter);
                    else
                        fatkvalue = getBFAttack(fighter);
                    if(fatkvalue >maxatk)
                    {
                        maxatk = fatkvalue;
                        maxatk_pos = i;
                        maxatk_class = nClass;
                    }
                }
            }
            fighter = static_cast<BattleFighter*>(getObject(bf->getSide(), maxatk_pos));
            if(fighter && fighter->getHP() > 0)
            {
                float fAtk = bo->_attack * skill->effect->atkP + skill->effect->atk;
                fAtk = -fAtk*ef->value/100;
                float fmagicAtk = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
                fmagicAtk = -fmagicAtk*ef->value/100;
                if (bf->getPos() == maxatk_pos)  // 如果是给自己增加，那么放到技能使用之后，让他下次使用技能再享受，省得看不到buf，以为没效果
                {
                    if(fAtk > 0 && maxatk_class == 3)
                        fighter->setSkillUsedChangeAttr(fAtk, ef->last, e_ss_Atk);
                    if(fmagicAtk > 0 && (maxatk_class == 1 || maxatk_class == 2))
                        fighter->setSkillUsedChangeAttr(fmagicAtk, ef->last, e_ss_MagAtk);
                }
                else
                {
                    if(fAtk > 0 && maxatk_class == 3)
                    {
                        SetSpecialAttrChange(fighter, skill, e_ss_Atk, ef->last, fAtk, true);
                    }
                    if(fmagicAtk > 0 && (maxatk_class == 1 || maxatk_class == 2))
                    {
                        SetSpecialAttrChange(fighter, skill, e_ss_MagAtk, ef->last, fmagicAtk, true);
                    }
                }
            }
            //setStatusChange(bf, bf->getSide(), maxatk_pos, 1, skill, e_stAtk, value, 1, activeFlag);                    
        }
    }    

    if(self != tmpself)
        self = tmpself;

    return true;
}

BattleFighter* BattleSimulator::getTherapyTarget(BattleFighter* bf)
{
    UInt8 side = bf->getSide();
    BattleFighter* pet = NULL;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0 || bo->hasFlag(BattleFighter::IsMirror) || bo->isSummon() || bo->isLingQu() || bo->isSoulOut())
            continue;
        if(bo->getHP() < (bo->getMaxHP() >> 1))
        {
            if(bo->isPet())
            {
                pet = bo;
                continue;
            }

            return bo;
        }
    }

    return pet;
}

BattleFighter* BattleSimulator::getTherapyTarget2(BattleFighter* bf, UInt8 * excepts, size_t exceptCount, bool isFirst /* = false */)
{
    UInt8 side = bf->getSide();
    BattleFighter* bo = NULL;
    float maxHpLost = 0;
    BattleFighter* minBloodBo = NULL;
    float curHpLost;

    for(UInt8 i = 0; i < 25; ++ i)
    {
        bo = static_cast<BattleFighter*>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0 || bo->hasFlag(BattleFighter::IsMirror) || bo->isLingQu() || bo->isSoulOut() || (isFirst && bo->isPet()) || bo->isSummon())
            continue;

        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;

        UInt32 hp = bo->getHP();
        UInt32 maxHp = bo->getMaxHP();
        if(hp < (maxHp >> 1))
        {
            curHpLost = (static_cast<float>(maxHp - hp)) / maxHp;
            if(curHpLost > maxHpLost)
            {
                maxHpLost = curHpLost;
                minBloodBo = bo;
            }
        }
    }

    return minBloodBo;
}

BattleFighter* BattleSimulator::getTherapyTarget3(BattleFighter* bf, UInt8 * excepts, size_t exceptCount, bool isFirst)
{
    UInt8 side = bf->getSide();
    BattleFighter* bo = NULL;
    BattleFighter* retbo = NULL;
    float maxHpLost = 0;
    UInt8 pos = 0;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        bo = static_cast<BattleFighter*>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0 || bo->hasFlag(BattleFighter::IsMirror) || bo->isLingQu() || bo->isSoulOut() || (isFirst && bo->isPet()) || bo->isSummon())
            continue;

        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;

        UInt32 hp = bo->getHP();
        UInt32 maxHp = bo->getMaxHP();

        if(((float)(maxHp - hp))/maxHp > maxHpLost)
        {
            maxHpLost = ((float)(maxHp - hp))/maxHp;
            pos = i;
        }
    }

    if(maxHpLost > 0.001f)
        retbo = static_cast<BattleFighter*>(getObject(side, pos));

    return retbo;
}

UInt32 BattleSimulator::FightersEnter(UInt8 prevWin)
{
    UInt32 rcnt = 0;

    std::vector<BattleFighter*>  cur_fgtlist = _fgtlist[_cur_fgtlist_idx];

    size_t cnt = cur_fgtlist.size();
    for(size_t idx = 0; idx < cnt; idx++)
    {
        BattleFighter* bf = cur_fgtlist[idx];
        if((prevWin-1) != bf->getSide())
        {
            if(bf->getPassiveSkillOnTherapy())
                _onTherapy.push_back(bf);
            if(bf->getPassiveSkillOnSkillDmg())
                _onSkillDmg.push_back(bf);
            if(bf->getPassiveSkillOnOtherDead())
                _onOtherDead.push_back(bf);

            const GData::SkillBase* passiveSkill = NULL;
            size_t skillIdx = 0;
            while(NULL != (passiveSkill = bf->getPassiveSkillEnter100(skillIdx)))
            {
                int target_side, target_pos, cnt;
                getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
                rcnt += doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
            }

            if(NULL != (passiveSkill = bf->getPassiveSkillEnter()))
            {
                int target_side, target_pos, cnt;
                getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
                rcnt += doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
            }
        }

        const GData::SkillBase *skill = bf->getXiangMoChanZhangSkill();
        if(skill && skill->effect)
        {
            const std::vector<UInt16>& eft = skill->effect->eft;
            const std::vector<UInt8>& efl = skill->effect->efl;
            const std::vector<float>& efv = skill->effect->efv;

            size_t cnt = eft.size();
            if(cnt == efl.size() && cnt == efv.size())
            {
                for(size_t i = 0; i < cnt; ++ i)
                    doSkillEffectExtra_HpShield(bf, bf->getSide(), bf->getPos(), skill, i);
            }
        }
    }

    //if(rcnt != 0)
    appendMaxReiatsu();
    appendToPacket(0, -1, -1, 0, 0, false, false);
    ++ rcnt;

    return rcnt;
}

UInt32 BattleSimulator::doSkillAttackAftEnter(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt)
{
    UInt32 rcnt = 0;
    if((skill->cond != GData::SKILL_ENTER && skill->cond != GData::SKILL_ENTER_LINGSHI) || target_pos < 0)
    {
        return rcnt;
    }

    bool useSkillStrengthen = true;
    do {
        appendDefStatus(e_skill, skill->getId(), bf);

        if(cnt == 1)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(NULL == bo)
                break;

            if(skill->effect->immune)
            {
                bo->setImmune(skill->effect->immune);
                bo->setImmuneLevel(skill->effect->immune, SKILL_LEVEL(skill->getId()));
            }
            else if(skill->effect->evade == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setEvad100(true);
                bf->setEvad100CD(skill->cd + 1);
                bf->setEvad100BaseCD(skill->cd + 1);
            }
            else if(skill->effect->def == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setDefend100(true);
                bf->setDefend100CD(skill->cd + 1);
                bf->setDefend100BaseCD(skill->cd + 1);
            }
            else if(skill->effect->hp > 0 || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
            {
                if(SKILL_ID(skill->getId()) == 45 ||
                        SKILL_ID(skill->getId()) == 46 ||
                        SKILL_ID(skill->getId()) == 89 ||
                        SKILL_ID(skill->getId()) == 90
                  )
                {
                    if(bo->hasFlag(BattleFighter::IsMirror) || bo->isSummon())
                    {
                        UInt8 excepts[25] = {0};
                        size_t exceptCnt = 0;
                        bo = getTherapyTarget3(bf, excepts, exceptCnt);
                        if(!bo)
                            return rcnt;
                        target_side = bo->getSide();
                        target_pos = bo->getPos();
                    }
                }
                if (doSkillAttack(bf, skill, target_side, target_pos, 1))
                {
                    ++ rcnt;
                    useSkillStrengthen = false;
                    return rcnt;
                }
            }
            else if ((skill->effect->atk > 0 || skill->effect->atkP > 0) && bf->isPet())
            {
                bf->setPetExAtk(bf->_attack * skill->effect->atkP + skill->effect->atk, skill->getId());
            }
            else if(skill->effect->state & GData::e_state_poison || skill->effect->damage || skill->effect->damageP || skill->effect->adddam
                    || skill->effect->magdam || skill->effect->magdamP || skill->effect->addmag
                    || skill->effect->crrdam || skill->effect->crrdamP || skill->effect->addcrr)
            {
                if (doSkillAttack(bf, skill, target_side, target_pos, 1))
                {
                    ++ rcnt;
                    useSkillStrengthen = false;
                    return rcnt;
                }
            }
            else
            {
                doSkillStatus2(bf, skill, target_side, target_pos, cnt);
            }

            appendDefStatus(e_damNormal, 0, bo);

            break;
        }

        for(int pos = 0; pos < cnt; pos++)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, pos));
            if(NULL == bo)
                continue;

            if(skill->effect->immune)
            {
                bo->setImmune(skill->effect->immune);
                bo->setImmuneLevel(skill->effect->immune, SKILL_LEVEL(skill->getId()));
            }
            else if(skill->effect->evade == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setEvad100(true);
                bf->setEvad100CD(skill->cd + 1);
                bf->setEvad100BaseCD(skill->cd + 1);
            }
            else if(skill->effect->def == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setDefend100(true);
                bf->setDefend100CD(skill->cd + 1);
                bf->setDefend100BaseCD(skill->cd + 1);
            }
            else
            {
                doSkillStatus2(bf, skill, target_side, pos, 1);
            }

            appendDefStatus(e_damNormal, 0, bo);
        }

    } while(false);
    doSkillEffectExtraAttack(bf, target_side, target_pos, skill);
    if(useSkillStrengthen)
        doAllSkillStrengthenEffect(bf, skill, target_side, target_pos);

    return rcnt;
}

// single attack round
UInt32 BattleSimulator::doAttack( int pos )
{
    UInt32 rcnt = 0;
    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
    BattleFighter* bf = cur_fgtlist[pos];

    cur_fgtlist.erase(cur_fgtlist.begin() + pos);
    memset(_cur_round_except, 0, sizeof(_cur_round_except));
    _except_count = 0;
    _hit_cnt = 0;

    // update all action points
#if 0
    UInt32 minact = fs.action;
    for(size_t i = 0; i < _fgtlist.size(); ++ i)
    {
        if(_fgtlist[i].bfgt->getHP() == 0)
            continue;
        _fgtlist[i].addAction(minact);
    }
#endif

    // insert the fighter to next queue by order
#if 0
    UInt32 bPoisonLevel = bf->getPoisonLevel();
    if(bPoisonLevel > 0)
    {
        UInt32 round = bf->getPoisonRound();
        if(round > 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;
            StatusChange scList[50];
            size_t scCount = 0;

            memset(defList, 0, sizeof(defList));
            UInt32 fdmg = bf->getMaxHP() * bPoisonLevel/100;
            -- round;
            defList[defCount].damType = (round > 0) ? e_Poison : e_UnPoison;
            defList[defCount].damage = fdmg;
            defList[defCount].pos = bf->getPos() + 25;
            bf->makeDamage(fdmg);
            defList[defCount].leftHP = bf->getHP();
            ++ defCount;
            bf->setPoisonRound(round);
            if(round == 0)
                bf->setPoisonLevel(0);
            //else
            //{
            //    fs.resetAction();
            //    insertFighterStatus(fs);
            //}
            // killed

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            if(bf->getHP() == 0)
            {
                onDead(bf, &atkAct);
                return 1;
            }
            else if(_winner == 0)
                onDamage(bf, scList, scCount, false);
            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
            rcnt++;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL);
                rcnt++;
            }

            atkAct.clear();
       }
    }
#endif
    //fs.resetAction();

    insertFighterStatus(bf);
    _activeFgt = bf;
    doLingshiModelAttack(bf, 3);

    UInt8 side = bf->getSide();
    UInt8 reiatsuType = 0;
    UInt8 reiatsuType2 = 0;
    UInt8 launchPeerLess = 0;

    BattleFighter* mainTarget = NULL;
    UInt8 attackCnt = 1;
    UInt8 prudentLast = bf->getPrudentLast();
    if(prudentLast > 0)
        attackCnt += 2;
    for(UInt8 cntIndex = 0; cntIndex < attackCnt; cntIndex++)
    {
    rcnt += doDeBufAttack(bf);
    if(bf->getHP() && !_winner && bf->getId() != 5679) // 行动者存活 && 战斗胜负未分 && 行动者不是雪人
    {

        UInt32 stun = bf->getStunRound();
        UInt32 confuse = bf->getConfuseRound();
        UInt32 forget = bf->getForgetRound();
        int target_pos;
        int bleed_target_pos = -1;
        int otherside = 1 - bf->getSide();
        bool target_is_stun = false;
        bool target_is_blind = false;
        if(!stun) // 不是昏迷状态，可以行动
        {
            do 
            {
                if(confuse > 0) // 混乱状态
                {
                    BattleFighter* rnd_bf = NULL;
                    if(_rnd(2) == bf->getSide())
                    {
                        UInt8 myPos = bf->getPos();
                        rnd_bf = getRandomFighter(bf->getSide(), &myPos, 1);
                    }

                    if(NULL == rnd_bf)
                    {
                        target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
                        mainTarget = static_cast<BattleFighter*>(getObject(otherside, target_pos));
                    }
                    else
                    {
                        target_pos = rnd_bf->getPos();
                        otherside = bf->getSide();
                    }

                    if(target_pos < 0)
                        break;

                    std::vector<AttackAct> atkAct;
                    atkAct.clear();
                    if(doNormalAttack(bf, otherside, target_pos, &atkAct))
                    {
                        ++ rcnt;
                        reiatsuType = e_reiatsu_normal_attack;
                    }

                    if(mainTarget && (mainTarget->getDeepStunLast() > 0 || mainTarget->getStunRound() > 0))
                        target_is_stun = true;
                    if(mainTarget && (mainTarget->getDeepBlindDmgExtra() > 0.001f || mainTarget->getBlind() > 0.001f))
                        target_is_blind = true;
                    size_t actCnt = atkAct.size();
                    for(size_t idx = 0; idx < actCnt; idx++)
                    {
                        if(atkAct[idx].bf->getHP() == 0)
                            continue;
                        if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                            ++ rcnt;
                    }

                    atkAct.clear();

                    break;
                }
                else if(forget > 0) // 沉默状态
                {
                    target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
                    mainTarget = static_cast<BattleFighter*>(getObject(otherside, target_pos));
                    if(mainTarget && (mainTarget->getDeepStunLast() > 0 || mainTarget->getStunRound() > 0))
                        target_is_stun = true;
                    if(mainTarget && (mainTarget->getDeepBlindDmgExtra() > 0.001f || mainTarget->getBlind() > 0.001f))
                        target_is_blind = true;

                    if(target_pos < 0)
                        break;

                    std::vector<AttackAct> atkAct;
                    atkAct.clear();
                    if(doNormalAttack(bf, otherside, target_pos, &atkAct, true))
                    {
                        ++ rcnt;
                        reiatsuType = e_reiatsu_normal_attack;
                    }

                    size_t actCnt = atkAct.size();
                    for(size_t idx = 0; idx < actCnt; idx++)
                    {
                        if(atkAct[idx].bf->getHP() == 0)
                            continue;
                        if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                            ++ rcnt;
                    }
                    atkAct.clear();


                    break;
                }
                else // 普通状态，可以使用技能攻击或者普通攻击
                {
                    const GData::SkillBase* skill = NULL;
                    // do preve attack passive skill that must act
                    size_t skillIdx = 0;
                    int cnt = 0;
                    target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
                    mainTarget = static_cast<BattleFighter*>(getObject(otherside, target_pos));
                    if(mainTarget && (mainTarget->getDeepStunLast() > 0 || mainTarget->getStunRound() > 0))
                        target_is_stun = true;
                    if(mainTarget && (mainTarget->getDeepBlindDmgExtra() > 0.001f || mainTarget->getBlind() > 0.001f))
                        target_is_blind = true;
                    doItemLingSkillAttack(bf, mainTarget);
                    bool noPossibleTarget = (target_pos == -1);
                    while(NULL != (skill = bf->getPassiveSkillPreAtk100(skillIdx, noPossibleTarget)))
                    {
                        getSkillTarget(bf, skill, otherside, target_pos, cnt);
                        if(cnt < 1)
                            break;

                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct))
                            ++ rcnt;

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();
                    }

                    skill = bf->getPassiveSkillPreAtk(noPossibleTarget);
                    if(NULL != skill)
                    {
                        getSkillTarget(bf, skill, otherside, target_pos, cnt);
                        if(cnt < 1)
                            break;

                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct))
                            ++ rcnt;

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();
                    }

                    // do active skill
#if 0
                    BattleFighter* therapy_bf = getTherapyTarget(bf);
#else
                    UInt8 excepts[25] = {0};
                    size_t exceptCnt = 0;
                    BattleFighter* therapy_bf = getTherapyTarget2(bf, excepts, exceptCnt);
#endif
                    UInt8 disableLast = bf->getPeerLessDisableLast();
                    skill = bf->getActiveSkill(therapy_bf!= NULL, noPossibleTarget);
                    if(disableLast > 0 && bf->getPeerLessDisableLast() == 0)
                        appendDefStatus(e_unBenevolent, 0, bf);
                    if(bf->getBuddhaLightLast() == 0xFF)
                        initBuddhaLight(bf, false, false);
                    bool canNormal = true;
                    bool extraNormal = false;
                    if(NULL != skill)
                    {
                        canNormal = false;
                        if(NULL != therapy_bf && (skill->effect->hpP > 0 || skill->effect->hp > 0 || skill->effect->addhp > 0) && skill->target == GData::e_battle_target_selfside)
                        {
                            otherside = therapy_bf->getSide();
                            target_pos = therapy_bf->getPos();
                            cnt = 1;
                        }
                        else
                        {
                            getSkillTarget(bf, skill, otherside, target_pos, cnt);
                        }
                        if(cnt < 1)
                            break;

                        // 昊天镜                
                        BattleFighter* ptarget = static_cast<BattleFighter*>(getObject(otherside, target_pos));
                        if(SKILL_ID(skill->getId()) == 132)
                        {
                            float rate = skill->prob * 100;
                            if(rate > _rnd(10000))
                            {
                                canNormal = true;
                                extraNormal = true;
                            }

                            GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
                            // 昊天镜如果有符文，会降对方闪避，升自己命中，此次攻击结束马上清除
                            if(ss)
                            {
                                const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_USEHAOTIAN, GData::TYPE_HAOTIANJING);
                                if(ef)
                                {
                                    bf->setHitChangeByPeerless(ef->value);
                                    if(ptarget)
                                        ptarget->setCounterChangeByPeerless(-ef->valueExt1); // value在数据库里居然是无符号的，只能加负号，汗。。。
                                }
                            }
                        }

                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct, 0, &launchPeerLess, true))
                        {
                            ++ rcnt;
                            if (launchPeerLess)
                                reiatsuType = e_reiatsu_peerless;
                            else
                                reiatsuType = e_reiatsu_skill_attack;
                        }


                        // 攻击完毕，把临时增加的命中反击清除
                        if(true)
                        {
                            bf->setHitChangeByPeerless(0);
                            if(ptarget)
                            {
                                bleed_target_pos = ptarget->getPos();
                                ptarget->setCounterChangeByPeerless(0);
                            }
                        }

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();
                    }

                    if(canNormal) // 可以普通攻击（平砍）
                    {
                        target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
                        bool noPossibleTarget = (target_pos == -1);

                        if(target_pos < 0)
                            break;

                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        float factor = 1;

                        if(doNormalAttack(bf, otherside, target_pos, &atkAct, factor, true))
                        {
                            ++ rcnt;
                            reiatsuType2 = e_reiatsu_skill_attack;
                        }

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();

                        // 普通攻击后的被动技能
                        if(bf->getHP() > 0 && _winner == 0)
                        {
                            size_t idx = 0;
                            const GData::SkillBase* passiveSkill = NULL;
                            BattleFighter* bo = static_cast<BattleFighter*>(getObject(otherside, target_pos));
                            while(NULL != (passiveSkill = bf->getPassiveSkillAftNAtk100(idx, noPossibleTarget)))
                            {
                                // 敌方
                                if(passiveSkill->target == GData::e_battle_target_otherside && (!bo || bo->getHP() == 0))
                                {
                                    continue;
                                }

                                int cnt = 0;
                                getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                                std::vector<AttackAct> atkAct;
                                atkAct.clear();
                                if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                                    ++ rcnt;

                                size_t actCnt = atkAct.size();
                                for(size_t idx = 0; idx < actCnt; idx++)
                                {
                                    if(atkAct[idx].bf->getHP() == 0)
                                        continue;
                                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                        ++ rcnt;
                                }
                                atkAct.clear();
                            }

                            passiveSkill = bf->getPassiveSkillAftNAtk(noPossibleTarget);
                            if(NULL != passiveSkill && (passiveSkill->target != GData::e_battle_target_otherside || (passiveSkill->target == GData::e_battle_target_otherside && bo && bo->getHP() > 0)) )
                            {
                                int cnt = 0;
                                getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                                std::vector<AttackAct> atkAct;
                                atkAct.clear();
                                if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                                    ++ rcnt;

                                size_t actCnt = atkAct.size();
                                for(size_t idx = 0; idx < actCnt; idx++)
                                {
                                    if(atkAct[idx].bf->getHP() == 0)
                                        continue;
                                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                        ++ rcnt;
                                }
                                atkAct.clear();
                            }

                            if (bo->getHP() > 0)
                                bleed_target_pos = bo->getPos();
                        }
                    }
                }

                if(target_is_stun)
                {
                    const GData::SkillBase *skill;
                    size_t idx = 0;
                    while(NULL != (skill = bf->getPassiveSkillLingshi100(idx)))
                    {
                        if(skill->effect && skill->effect->eft[0] ==  GData::e_eft_lingshi_mojian)
                        {
                            StateType eType = e_MAX_STATE;
                            UInt32 dmg = CalcNormalAttackDamage(bf, mainTarget, eType);
                            if (eType != e_MAX_STATE)
                            {
                                dmg *= skill->effect->efv[0];
                                if (eType == e_damNormal)
                                {
                                    makeDamage(mainTarget, dmg, eType, e_damagePhysic);
                                }
                            }
                            appendDefStatus(e_skill, skill->getId(), bf);
                            break;
                        }
                    }
                }
                if(target_is_blind)
                {
                    const GData::SkillBase *skill;
                    size_t idx = 0;
                    while(NULL != (skill = bf->getPassiveSkillLingshi100(idx)))
                    {
                        if(skill->effect && skill->effect->eft[0] ==  GData::e_eft_lingshi_mozhu)
                        {
                            StateType eType = e_MAX_STATE;
                            UInt32 dmg = CalcNormalAttackDamage(bf, mainTarget, eType);
                            if (eType != e_MAX_STATE)
                            {
                                dmg *= skill->effect->efv[0];
                                if (eType == e_damNormal)
                                {
                                    makeDamage(mainTarget, dmg, eType, e_damagePhysic);
                                }
                            }
                            appendDefStatus(e_skill, skill->getId(), bf);
                            break;
                        }
                    }
                }

                // 攻击后的被动技能
                if(bf->getHP() > 0 && _winner == 0)
                {
                    size_t idx = 0;
                    const GData::SkillBase* passiveSkill = NULL;
                    bool noPossibleTarget = (target_pos == -1);
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(otherside, target_pos));
                    while(NULL != (passiveSkill = bf->getPassiveSkillAftAtk100(idx, noPossibleTarget)))
                    {
                        if(passiveSkill->target == GData::e_battle_target_otherside && (!bo || bo->getHP() == 0))
                        {
                            continue;
                        }

                        int cnt = 0;
                        getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                            ++ rcnt;

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();
                    }

                    passiveSkill = bf->getPassiveSkillAftAtk(noPossibleTarget);
                    if(NULL != passiveSkill && (passiveSkill->target != GData::e_battle_target_otherside || (passiveSkill->target == GData::e_battle_target_otherside && bo && bo->getHP() > 0)) )
                    {
                        int cnt = 0;
                        getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                            ++ rcnt;

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();
                    }
                    bo = static_cast<BattleFighter*>(getObject(otherside, bleed_target_pos));
                    if (bf->getChaosWorldId() && bo && bo->getHP() && (bo->getSide() != bf->getSide()) && !bo->isSoulOut() && !bo->getStunRound())
                    {
                        bo->setStunRound(1);
                        appendDefStatus(e_skill, bf->getChaosWorldId(), bf);
                        appendDefStatus(e_Stun, 0, bo);
                    }

                    bo = static_cast<BattleFighter*>(getObject(otherside, bleed_target_pos));
                    while(NULL != (passiveSkill = bf->getPassiveSkillOnAttackBleed100(idx, noPossibleTarget)))
                    {
                        if(passiveSkill->target == GData::e_battle_target_otherside && bo && bo->getHP() &&  (bo->getSide() != bf->getSide()) && !bo->isSoulOut() &&
                                bo->isBleeding())
                        {
                            int cnt = 0;
                            getSkillTarget(bf, passiveSkill, otherside, bleed_target_pos, cnt);
                            std::vector<AttackAct> atkAct;
                            atkAct.clear();
                            if(doSkillAttack(bf, passiveSkill, otherside, bleed_target_pos, cnt, &atkAct))
                                ++ rcnt;

                            size_t actCnt = atkAct.size();
                            for(size_t idx = 0; idx < actCnt; idx++)
                            {
                                if(atkAct[idx].bf->getHP() == 0)
                                    continue;
                                if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                    ++ rcnt;
                            }
                            atkAct.clear();
                        }
                    }
                }

                const GData::SkillBase *skill = bf->getXiangMoChanZhangSkill();
                if(skill && skill->effect)
                {
                    const std::vector<UInt16>& eft = skill->effect->eft;
                    const std::vector<UInt8>& efl = skill->effect->efl;
                    const std::vector<float>& efv = skill->effect->efv;

                    size_t cnt = eft.size();
                    if(cnt == efl.size() && cnt == efv.size())
                    {
                        for(size_t i = 0; i < cnt; ++ i)
                            doSkillEffectExtra_HpShield(bf, bf->getSide(), bf->getPos(), skill, i);
                    }
                }
#if 0
                if(target_object != NULL)
                {
                    // find all targets that are hit
                    GData::Area * area = NULL;
                    GObject::ItemWeapon * weapon = bf->getFighter()->getWeapon();
                    if(weapon != NULL)
                    {
                        // TODO: no weapon_def
                        // area = &weapon->getWeaponDef().getArea();
                        area = &GData::Area::getDefault();
                    }
                    else
                    {
                        area = &GData::Area::getDefault();
                    }
                    int cnt = area->getCount();
                    if(cnt <= 0)
                        return 0;

                    DefStatus defList[25];
                    size_t defCount = 0;
                    StatusChange scList[50];
                    size_t scCount = 0;

                    // calculate damage
                    bool cs = false;
                    bool pr = bf->calcPierce();
                    float atk = bf->calcAttack(cs);

                    float factor = (*area)[0].factor;

                    rcnt += tryPreUseSkill(bf, target_object);
                    UInt32 dmg = 0;
                    if(cnt > 1)
                    {
                        AttackPoint ap[25];
                        int apcnt = 0;
                        int x_ = target_pos % 5;
                        int y_ = target_pos / 5;
                        for(int i = 1; i < cnt; ++ i)
                        {
                            GData::Area::Data& ad = (*area)[i];
                            int x = x_ + ad.x;
                            int y = y_ + ad.y;
                            if(x < 0 || x > 4 || y < 0 || y > 4)
                            {
                                continue;
                            }
                            ap[apcnt].pos = x + y * 5;
                            ap[apcnt].type = ad.type;
                            ap[apcnt ++].factor = ad.factor;
                        }
                        // attack the target on center
                        dmg = attackOnce(bf, atk, cs, pr, target_object, factor, defList, defCount, scList, scCount, apcnt, ap);
                        for(int i = 0; i < apcnt; ++ i)
                        {
                            switch(ap[i].type)
                            {
                                case 0:
                                    factor = ap[i].factor;
                                    attackOnce(bf, atk, cs, pr, _objs[otherside][ap[i].pos], factor, defList, defCount, scList, scCount);
                                    break;
                                case 1:
                                    factor = (*area)[0].factor;
                                case 2:
                                    {
                                        float newfactor = factor * ap[i].factor;
                                        if(attackOnce(bf, atk, cs, pr, _objs[otherside][ap[i].pos], newfactor, defList, defCount, scList, scCount))
                                            factor = newfactor;
                                    }
                                    break;
                            }
                        }
                    }
                    else
                    {
                        // attack only one target
                        dmg = attackOnce(bf, atk, cs, pr, target_object, factor, defList, defCount, scList, scCount, 0);
                    }
                    // damage by skill or buff
                    UInt16 skillid = 0;
                    if(_winner == 0)
                    {
                        if(dmg > 0 && target_object->isChar())
                        {
                            // TODO:
                            BattleFighter * target_fighter = static_cast<BattleFighter *>(target_object);
                            UInt16 myskillid = bf->getFighter()->getSkill();
                            if(target_fighter->getHP() > 0)
                            {
                                switch(myskillid)
                                {
                                    case 202:
                                    case 203:
                                    case 204:
                                    case 209:
                                        {
                                            UInt8 poisonLevel = target_fighter->getPoisonLevel();
                                            if(myskillid - 201 <= poisonLevel)
                                            {
                                                poisonLevel = 0;
                                                break;
                                            }
                                            Script::BattleFormula::SkillData& sd = _formula->skillData(1, myskillid - 201, bf->getFighter()->getSkillLevel());
                                            if(_rnd(100) >= sd.rate)
                                            {
                                                poisonLevel = 0;
                                                break;
                                            }
                                            poisonLevel = static_cast<UInt8>(myskillid - 201);
                                            if(poisonLevel > 3)
                                                poisonLevel = 4;
                                            target_fighter->setPoisonLevel(poisonLevel);
                                            skillid = myskillid;
                                            UInt32 round = sd.value3;
                                            if(round > 0)
                                            {
                                                UInt32 fdmg;
                                                if(target_fighter->hasFlag(BattleFighter::IsMirror))
                                                {
                                                    fdmg = target_fighter->getHP();
                                                }
                                                else
                                                {
                                                    const UInt16 poisonSid[] = {0, 1, 2, 3, 8};
                                                    Script::BattleFormula::SkillData& sd = _formula->skillData(1, poisonSid[poisonLevel], bf->getFighter()->getSkillLevel());
                                                    float pdmg = static_cast<float>(sd.value1) * bf->getAttack();
                                                    fdmg = _formula->calcDamage(pdmg, target_fighter->getDefend());
                                                }
                                                -- round;
                                                defList[defCount].damType = (round > 0) ? 6 : 7;
                                                defList[defCount].damage = fdmg;
                                                defList[defCount].pos = getSidePos(target_fighter);
                                                target_object->makeDamage(fdmg);
                                                defList[defCount].leftHP = target_fighter->getHP();
                                                ++ defCount;
                                                target_fighter->setPoisonRound(round);
                                                if(round == 0)
                                                    target_fighter->setPoisonLevel(0);
                                                // killed
                                                if(target_object->getHP() == 0)
                                                    onDead(target_fighter);
                                                else if(_winner == 0)
                                                {
                                                    onDamage(target_fighter, scList, scCount, true);
                                                    FighterStatus fs2(target_fighter, bf->getAction() * 3 / 4, fdmg);
                                                    insertFighterStatus(fs2);
                                                }
                                            }
                                        }
                                        break;
                                    case 305:
                                    case 306:
                                    case 307:
                                    case 309:
                                        {
                                            UInt8 confuseLevel = target_fighter->getConfuseLevel();
                                            if(myskillid - 304 <= confuseLevel)
                                                break;
                                            Script::BattleFormula::SkillData& sd = _formula->skillData(2, myskillid - 301, bf->getFighter()->getSkillLevel());
                                            if(_rnd(100) >= sd.rate)
                                                break;
                                            if(confuseLevel > 0)
                                            {
                                                target_fighter->_hitrateAdd = 0;
                                            }
                                            confuseLevel = myskillid - 304;
                                            if(confuseLevel > 3)
                                                confuseLevel = 4;
                                            target_fighter->setConfuseLevel(confuseLevel);
                                            skillid = myskillid;
                                            switch(confuseLevel)
                                            {
                                                case 1:
                                                case 2:
                                                    setStatusChange(target_fighter->getSide(), target_pos, 1, myskillid, 4, -sd.value1, scList, scCount, true);
                                                    break;
                                                case 3:
                                                    setStatusChange(target_fighter->getSide(), (target_pos / 5 * 5), 5, myskillid, 4, -sd.value1, scList, scCount, true);
                                                    break;
                                                case 4:
                                                    setStatusChange(target_fighter->getSide(), 0, 25, myskillid, 4, -sd.value1, scList, scCount, true);
                                                    break;
                                            }
                                        }
                                        break;
                                }
                            }
                            if(bf->getHP() > 0)
                            {
                                switch(myskillid)
                                {
                                    case 107:
                                    case 108:
                                        {
                                            Script::BattleFormula::SkillData& sd = _formula->skillData(0, myskillid - 101, bf->getFighter()->getSkillLevel());
                                            if(_rnd(100) >= sd.rate)
                                                break;
                                            UInt32 rhp = static_cast<UInt32>(sd.value1 * dmg);
                                            if(rhp == 0)
                                                rhp = 1;
                                            defList[defCount].damType = 3;
                                            defList[defCount].damage = rhp;
                                            defList[defCount].pos = getSidePos(bf);
                                            bf->regenHP(rhp);
                                            defList[defCount].leftHP = bf->getHP();
                                            onHPChanged(bf);
                                            ++ defCount;
                                            skillid = myskillid;
                                        }
                                        break;
                                }
                                if(_winner == 0 && defCount > 0 && (defList[0].damType & 0x17) == 0 && target_fighter->getHP() > 0)
                                {
                                    UInt8 thornType = target_fighter->getThornLevel();
                                    UInt16 tskillid = target_fighter->getFighter()->getSkill();
                                    switch(tskillid)
                                    {
                                        case 101:
                                        case 102:
                                        case 103:
                                        case 109:
                                            {
                                                if(tskillid - 100 <= thornType)
                                                    break;
                                                Script::BattleFormula::SkillData& sd = _formula->skillData(0, tskillid - 101, target_fighter->getFighter()->getSkillLevel());
                                                if(_rnd(100) >= sd.rate)
                                                    break;
                                                thornType = static_cast<UInt8>(tskillid - 100);
                                                if(thornType > 3)
                                                    thornType = 4;
                                                target_fighter->setThornLevel(thornType);
                                            }
                                            break;
                                    }
                                    if(thornType > 0)
                                    {
                                        const UInt16 thronSid[] = {0, 0, 1, 2, 8};
                                        Script::BattleFormula::SkillData& sd = _formula->skillData(0, thronSid[thornType], target_fighter->getFighter()->getSkillLevel());
                                        float cdmg;
                                        switch(thornType)
                                        {
                                            case 1:
                                            case 2:
                                                cdmg = static_cast<float>(sd.value1);
                                                break;
                                            case 3:
                                            case 4:
                                            default:
                                                cdmg = sd.value1 * target_fighter->getMaxHP();
                                                break;
                                        }
                                        UInt32 fdmg = _formula->calcDamage(cdmg, bf->getDefend());
                                        defList[0].damType = 5;
                                        defList[0].counterDmg = fdmg;
                                        bf->makeDamage(fdmg);
                                        defList[0].counterLeft = bf->getHP();
                                        // killed
                                        if(bf->getHP() == 0)
                                            onDead(bf);
                                        else if(_winner == 0)
                                            onDamage(bf, scList, scCount, false);
                                    }
                                }
                            }
                        }
                        UInt8 confuseLevel = bf->getConfuseLevel();
                        if(confuseLevel > 0)
                        {
                            bf->setConfuseLevel(0);
                            setStatusChange(bf->getSide(), pos, 1, 0, 4, static_cast<UInt32>(0), scList, scCount, false);
                        }
                    }

                    appendToPacket(bf->getSide(), bf->getPos(), target_pos, static_cast<UInt8>(skillid > 0 ? 2 : 0), skillid, cs, pr, defList, defCount, scList, scCount);
                    ++ rcnt;
                    if(_winner == 0 && bf->getHP() > 0)
                        rcnt += tryDelayUseSkill(bf, target_object);

                    if(target_object->getHP() > 0)
                    {
                        BattleFighter * tmp_bf = bf;
                        float atkRate;
                        UInt16 skillId = 0;
                        if((atkRate = testLink(tmp_bf, skillId)) > 0.0f && tmp_bf != NULL)
                        {
                            bool tmp_cs = false;
                            bool tmp_pr = tmp_bf->calcPierce();
                            float tmp_atk = tmp_bf->calcAttack(tmp_cs) * atkRate;
                            defCount = 0;
                            scCount = 0;
                            GData::Area * area2 = NULL;
                            if(tmp_bf->getFighter()->getWeapon() != NULL)
                            {
                                // TODO: no weapon_def
                                // area2 = &tmp_bf->getFighter()->getWeapon()->getWeaponDef().getArea();
                            }
                            else
                            {
                                area2 = &GData::Area::getDefault();
                            }
                            if(area2->getCount() > 0)
                                atkRate *= (*area)[0].factor;
                            if(skillId > 0)
                            {
                                appendToPacket(bf->getSide(), bf->getPos(), tmp_bf->getPos() + 25, 2, skillId, false, false, NULL, 0, NULL, 0);
                                ++ rcnt;
                            }
                            attackOnce(tmp_bf, tmp_atk, tmp_cs, tmp_pr, NULL, target_object, atkRate, defList, defCount, scList, scCount);
                            if(defCount > 0)
                            {
                                appendToPacket(bf->getSide(), tmp_bf->getPos(), target_pos, 0, 0, tmp_cs, tmp_pr, defList, defCount, scList, scCount);
                                ++ rcnt;
                            }
                        }
                    }
                }
#endif
            } while(false);
        }

        // 每回合行动后自动释放技能 （眩晕也可以触发的）（目前只有回血恢复技能）
        if (bf->getHP() && !_winner)
        {
            const GData::SkillBase *skill = NULL;
            size_t idx = 0;
            while(NULL != (skill = bf->getPassiveSkillAftAction100(idx)))
            {
                int cnt = 0;
                getSkillTarget(bf, skill, otherside, target_pos, cnt);
                GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
                if(ss)
                {
                    const GData::SkillStrengthenEffect* ef = NULL;

                    // HP百分比回复
                    ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_HPP_RECOVER);
                    if (ef)
                    {
                        UInt32 rhp = static_cast<UInt32>(bf->getMaxHP() * ef->value / 100);
                        rhp = bf->regenHP(rhp);
                        if (rhp)
                        {
                            appendDefStatus(e_skill, skill->getId(), bf);
                            appendDefStatus(e_damHpAdd, rhp, bf);
#ifdef _BATTLE_DEBUG
                            std::cout << "updateHPPRecoverAftAction: " << ef->value << "," << ef->valueExt1 << "," << ef->valueExt2 << "," << rhp << "." << std::endl << std::endl;
#endif
                        }
                    }

                }
            }
            while(NULL != (skill = bf->getPassiveSkillAftAction()))
            {
                int cnt = 0;
                getSkillTarget(bf, skill, otherside, target_pos, cnt);
                GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
                if(ss)
                {
                    const GData::SkillStrengthenEffect* ef = NULL;

                    // HP百分比回复
                    ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_HPP_RECOVER);
                    if (ef)
                    {
                        UInt32 rhp = static_cast<UInt32>(bf->getMaxHP() * ef->value / 100);
                        rhp = bf->regenHP(rhp);
                        if (rhp)
                        {
                            appendDefStatus(e_skill, skill->getId(), bf);
                            appendDefStatus(e_damHpAdd, rhp, bf);
#ifdef _BATTLE_DEBUG
                            std::cout << "updateHPPRecoverAftAction: " << ef->value << "," << ef->valueExt1 << "," << ef->valueExt2 << "," << rhp << "." << std::endl << std::endl;
#endif
                        }
                    }

                }
            }
        }
    }
    }
    if(prudentLast > 0)
        bf->setPrudentLast(--prudentLast);
    UInt8 prudentLastOtherside = bf->getPrudentHitrateLastOtherside();
    if(prudentLastOtherside > 0)
        bf->setPrudentHitrateLastOtherside(--prudentLastOtherside);
    bf->addActCnt(1);

    rcnt += releaseCD(bf);
    _activeFgt = NULL;

    if (reiatsuType)
    {
        rcnt += tryPetEnter(side, reiatsuType, reiatsuType2?true:false);
    }
    if (reiatsuType2)
        rcnt += tryPetEnter(side, reiatsuType2);

    if(_defList.size() > 0 || _scList.size() > 0)
    {
        appendToPacket(0, -1, -1, 5, 0, false, false);
        ++ rcnt;
    }

    size_t size = _sneak_atker.size();
    if(size != 0)
    {
        for(size_t i = 0; i < size; ++ i)
        {
            bool cs = false;
            bool pr = false;
            if(bf->getHP() == 0 || _winner != 0)
                break;
            BattleFighter* sneaker = _sneak_atker[i];
            _activeFgt = sneaker;
            sneaker->nextSneakStatus();
            if(!sneaker->recoverSneakAtk())
                appendDefStatus(e_unSneakAtk, 0, static_cast<BattleFighter*>(sneaker));

            doSneakAttack(sneaker, bf, pr, cs);

            if(_defList.size() > 0 || _scList.size() > 0)
            {
                appendToPacket(sneaker->getSide(), sneaker->getPos(), 0, 2, 10001, cs, pr);
                ++ rcnt;
            }
        }
        _sneak_atker.clear();
        _activeFgt = NULL;
    }

    if(mainTarget)
    {
        mainTarget->setShieldObj(NULL);

        //UInt8 times = mainTarget->getCounterSpiritTimes();
        float atk = mainTarget->getCounterSpiritAtk();
        if(/*times >= 3 && */atk > 0.001f)
        {
            _activeFgt = mainTarget;
            std::vector<float>& factors = mainTarget->getCounterSpiritFactor();
            int fsize = factors.size();
            int side = 1 - mainTarget->getSide();
            bool cs = false;
            bool pr = false;
            bool first = true;
            int i = 0;
            for(int pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
                if(!bo || bo->getHP() == 0)
                    continue;
                float factor = 1.0f;
                if(fsize > 0)
                {
                    if(i >= fsize)
                        factor = factors[fsize-1];
                    else
                        factor = factors[i];
                }
                doSpiritAttack(mainTarget, bo, atk * factor, pr, cs, first);
                ++ i;
            }
            if(_defList.size() > 0 || _scList.size() > 0)
            {
                UInt16 skillid = mainTarget->getCounterSpiritSkillId();
                appendToPacket( mainTarget->getSide(), mainTarget->getPos(), 0, 2, skillid, cs, pr);
                ++ rcnt;
            }
            mainTarget->clearCounterSpiritSkill();
            _activeFgt = NULL;

            size = _sneak_atker.size();
            if(size != 0)
            {
                for(size_t i = 0; i < size; ++ i)
                {
                    bool cs = false;
                    bool pr = false;
                    if(mainTarget->getHP() == 0 || _winner != 0)
                        break;
                    BattleFighter* sneaker = _sneak_atker[i];
                    _activeFgt = sneaker;
                    sneaker->nextSneakStatus();
                    if(!sneaker->recoverSneakAtk())
                        appendDefStatus(e_unSneakAtk, 0, static_cast<BattleFighter*>(sneaker));

                    doSneakAttack(sneaker, mainTarget, pr, cs);

                    if(_defList.size() > 0 || _scList.size() > 0)
                    {
                        appendToPacket(sneaker->getSide(), sneaker->getPos(), 0, 2, 10001, cs, pr);
                        ++ rcnt;
                    }
                }
                _sneak_atker.clear();
                _activeFgt = NULL;
            }
        }
    }

    for (size_t i = 0; i < _onOtherConfuseAndForgetAtkList[side].size();)
    {
        BattleFighter* atk = _onOtherConfuseAndForgetAtkList[side][i];
        if (!atk || atk->getHP() < 0 ||  atk->getSide() == side ||
                atk->getStunRound() || atk->getConfuseRound() || atk->getForgetRound())
        {
            _onOtherConfuseAndForgetAtkList[side].erase(_onOtherConfuseAndForgetAtkList[side].begin() + i);
            continue;
        }
        doOtherConfuseForgetAttack(atk, rcnt);
        ++i;
    }

    if(bf->getHP() > 0 && _winner == 0 && bf->getAbnormalTypeCnt() >= 3)
    {
        const GData::SkillBase* passiveSkill = NULL;
        if(NULL != (passiveSkill = bf->getAbnormalTypeSkill()))
        {
            _activeFgt = bf;
            bool cs = false;
            bool pr = false;
            doSkillEffectExtra_AbnormalTypeDmg(bf, passiveSkill, cs, pr);
            if(_defList.size() > 0 || _scList.size() > 0)
            {
                appendToPacket(bf->getSide(), bf->getPos(), 0, 2, passiveSkill->getId(), cs, pr);
                ++ rcnt;
            }
            _activeFgt = NULL;
        }
    }
    if(bf->getHP() > 0 && _winner == 0 && bf->getBleedTypeCnt() >= 3)
    {
        const GData::SkillBase* passiveSkill = NULL;
        if(NULL != (passiveSkill = bf->getBleedTypeSkill()))
        {
            _activeFgt = bf;
            doSkillEffectExtra_BleedTypeDmg(bf, passiveSkill);
            if(_defList.size() > 0 || _scList.size() > 0)
            {
                appendToPacket(bf->getSide(), bf->getPos(), 0, 2, passiveSkill->getId(), false, false);
                ++ rcnt;
            }
            _activeFgt = NULL;
        }
    }

    return rcnt;
}

// XXX: 这里删除了一堆已经无效的代码

int BattleSimulator::testWinner()
{
    int alive[2] = { 0, 0 };
    int soulOut[2] = { 0, 0 };
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
        size_t c = fgtlist.size();
        for(size_t i = 0; i < c; ++ i)
        {
            alive[fgtlist[i]->getSide()] ++;
            if(fgtlist[i]->isSoulOut())
                soulOut[fgtlist[i]->getSide()]++;
        }
    }
    if(alive[0] == 0 || alive[0] == soulOut[0])
        return 2;
    else if(alive[1] == 0 || alive[1] == soulOut[1])
        return 1;
    return 0;
}

int BattleSimulator::testWinner2()
{
    if(_player[1] == NULL)
        return 2;

    UInt32 leftHPAll[2] = {0, 0};
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
        size_t c = fgtlist.size();
        for(size_t i = 0; i < c; ++ i)
        {
            leftHPAll[fgtlist[i]->getSide()] += fgtlist[i]->getHP();
        }
    }

    return leftHPAll[0] > leftHPAll[1] ? 1 : 2;
}

void BattleSimulator::appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr)
{
    _packet << static_cast<UInt8>(from_side);
    // attack mode
    _packet << static_cast<UInt8>(atk_type | (cs ? 0x80 : 0) | (pr ? 0x40 : 0)) << add_id;

    //攻击成就判断
    if(from_side < 2 && _player[from_side])
    {
        if(cs)
        {
            _csNum[from_side] ++ ;
            /*
            if(_csNum[from_side] ==3)
                 _cs3ok[from_side] = true;

            if(_csNum[from_side] == 9)
                _cs9ok[from_side] = true;
                */
            _maxCS[from_side] = std::max(  _csNum[from_side],  _maxCS[from_side]);
        }
        if(pr)
        {
            _prNum[from_side] ++ ;
            /*
            if(_prNum[from_side] == 3)
                  _pr3ok[from_side] = true;
            if(_prNum[from_side]== 9)
                _pr9ok[from_side] = true;*/
             _maxPR[from_side] =std:: max(  _prNum[from_side],  _maxPR[from_side]);
        }
    }
    // reserved
    size_t defCount = _defList.size();
    _packet << static_cast<UInt8>(0);
    // attack point
    _packet << static_cast<UInt8>(target_pos);
    // action players count
    _packet << static_cast<UInt8>(1);
    // action players list
    _packet << static_cast<UInt8>(from_pos) << static_cast<UInt8>(0);
    // attacked players count
    _packet << static_cast<UInt8>(defCount);
    // attacked players list
    for(size_t i = 0; i < defCount; ++ i)
    {
        _packet << _defList[i].pos << _defList[i].damType << _defList[i].damType2 << _defList[i].damage << _defList[i].leftHP << static_cast<UInt8>(_defList[i].damageType);
        if(_defList[i].damType == e_damAbsorb)
        {
            _packet << _defList[i].rhp << _defList[i].rLeftHP;
        }

        if((_defList[i].damType2 & 0x80) == 0x80)
        {
            _packet << _defList[i].counterDmg << _defList[i].counterLeft;
        }

        //成就判断
        if(from_side<2)
        {
            bool bDamage = _defList[i].damage != 0;

            if( bDamage)
            {
                if(!cs)
                    _csNum[from_side] = 0; //打断连续情况
                if(!pr)
                    _prNum[from_side] = 0;
            }
            UInt8 o_side = (from_side + 1) %2;
            if(_defList[i].damType == e_damEvade)
            {
                 _evadeNum[o_side] ++ ;
                 /*
                 if(_evadeNum[o_side] == 3)
                    _evade3OK[o_side] = true;
                 if(_evadeNum[o_side] == 9)
                    _evade9ok[o_side] = true;*/
                 _maxEvade[o_side] = std::max(_maxEvade[o_side], _evadeNum[o_side]);
            }
            else if( bDamage)
            {
                _evadeNum[o_side] = 0;
            }

            if((_defList[i].damType2 & 0x80) == 0x80)
            {
                _fjNum[o_side] ++ ;
                /*
                if(_fjNum [o_side] == 3)
                    _fj3ok[o_side] = true;
                if(_fjNum[o_side] == 9)
                    _fj9ok[o_side] = true;*/
                _maxFJ[o_side] = std::max( _maxFJ[o_side] ,  _fjNum[o_side]);
            }
            else if(bDamage)
            {
                _fjNum[o_side] = 0;
            }
        }

    }
    // status change
    size_t scCount = _scList.size();
    _packet << static_cast<UInt8>(scCount);
    for(size_t i = 0; i < scCount; ++ i)
    {
        _packet << _scList[i].pos << _scList[i].statusId << _scList[i].type << _scList[i].data;
    }

#ifdef _DEBUG
    char path[1024], path2[1024];
    sprintf(path, "%s0/0", cfg.reportPath.c_str());
    sprintf(path2, "%s/%u.log", path, 0);
    File rfile(path);
    rfile.createDirectories();
    FILE * f = fopen(path2, "a+");
    if(f == NULL)
        return;

    char szBuf[256] = {0};
    sprintf(szBuf, "from_side=%d\r\n", from_side);
    fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "atk_type=%d\r\n", atk_type);
    fwrite(szBuf, 1, strlen(szBuf), f);
    if(cs)
        sprintf(szBuf, "critical\r\n");
    if(pr)
        sprintf(szBuf, "pierce\r\n");

    sprintf(szBuf, "skillId=%d\r\n", add_id);
    fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "target_pos=%d\r\n", target_pos);
    fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "from_pos=%d\r\n", from_pos);
    fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "defCount=%lu\r\n", defCount);
    fwrite(szBuf, 1, strlen(szBuf), f);
    for(size_t i = 0; i < defCount; ++ i)
    {
        sprintf(szBuf, "defList[%lu].pos=%d\r\n", i, _defList[i].pos);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].damType=%d\r\n", i, _defList[i].damType);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].damage=%d\r\n", i, _defList[i].damage);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].leftHP=%d\r\n", i, _defList[i].leftHP);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].damageType=%d\r\n", i, _defList[i].damageType);
        fwrite(szBuf, 1, strlen(szBuf), f);
        if(_defList[i].damType == e_damAbsorb)
        {
            sprintf(szBuf, "defList[%lu].rhp=%d\r\n", i, _defList[i].rhp);
            fwrite(szBuf, 1, strlen(szBuf), f);
            sprintf(szBuf, "defList[%lu].rLeftHP=%d\r\n", i, _defList[i].rLeftHP);
            fwrite(szBuf, 1, strlen(szBuf), f);
        }

        if((_defList[i].damType2 & 0x80) == 0x80)
        {
            sprintf(szBuf, "defList[%lu].counterDmg=%d\r\n", i, _defList[i].counterDmg);
            fwrite(szBuf, 1, strlen(szBuf), f);
            sprintf(szBuf, "defList[%lu].counterLeft=%d\r\n", i, _defList[i].counterLeft);
            fwrite(szBuf, 1, strlen(szBuf), f);
        }
    }
    // status change
    sprintf(szBuf, "scCount=%lu\r\n", scCount);
    fwrite(szBuf, 1, strlen(szBuf), f);
    for(size_t i = 0; i < scCount; ++ i)
    {
        sprintf(szBuf, "scList[%lu].pos=%d\r\n", i, _scList[i].pos);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "scList[%lu].type=%d\r\n", i, _scList[i].type);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "scList[%lu].data=%d\r\n", i, _scList[i].data);
        fwrite(szBuf, 1, strlen(szBuf), f);
    }

    sprintf(szBuf, "------------------------------------\r\n");
    fwrite(szBuf, 1, strlen(szBuf), f);
    fclose(f);
#endif
    _defList.clear();
    _scList.clear();
}

bool BattleSimulator::applyFighterHP( UInt8 side, GObject::Player * player, bool useRegen, UInt32 sysRegen )
{
    bool res = false;
    UInt32 autohp = 0; // player->getBuffData(PLAYER_BUFF_AUTOHEAL, 0);
    for(int j = 0; j < 5; ++ j)
    {
        GObject::Lineup& pd = player->getLineup(j);
        if(pd.fighter == NULL)
            continue;
        BattleObject * obj = getObject(side, pd.pos);
        if(obj != NULL && obj->isChar())
        {
            BattleFighter * bf = static_cast<BattleFighter *>(obj);
            UInt32 hp = bf->getHP();

            UInt32 oldmaxhp = pd.fighter->getMaxHP();
            UInt32 currhp = pd.fighter->getCurrentHP();
            if(currhp == 0)
                currhp = oldmaxhp;
            UInt32 newhp = (float)hp / bf->getMaxHP() * oldmaxhp;
            if(newhp > currhp)
                newhp = currhp;
            if(useRegen)
            {
                if(newhp == 0)
                    newhp = 1;
                if (!World::getAutoHeal())
                {
                    if(autohp > 0)
                    {
                        if(newhp < oldmaxhp)
                        {
                            if(newhp + autohp >= oldmaxhp)
                            {
                                autohp -= oldmaxhp - newhp;
                                newhp = 0;
                            }
                            else
                            {
                                newhp += autohp;
                                autohp = 0;
                                res = true;
                            }
                        }
                        else
                            newhp = 0;
                    }
                    else
                        res = true;
                }
                else
                {
                    newhp = 0;
                    res = true;
                }
            }
            else
            {
                if(newhp < currhp)
                {
                    newhp = (currhp - newhp) * sysRegen / 100 + newhp;
                    if(newhp == 0)
                        newhp = 1;
                    res = true;
                }
            }
            pd.fighter->setCurrentHP(newhp);
        }
    }
    // player->setBuffData(0, autohp);
    return res;
}

float BattleSimulator::getLostHPPercent(UInt8 side, GObject::Player * player)
{
    float maxhp = 0;
    float hp = 0;
    for(int j = 0; j < 5; ++ j)
    {
        GObject::Lineup& pd = player->getLineup(j);
        if(pd.fighter == NULL)
            continue;
        BattleObject * obj = getObject(side, pd.pos);
        if(obj != NULL && obj->isChar())
        {
            BattleFighter * bf = static_cast<BattleFighter *>(obj);
            hp += bf->getHP();
            maxhp += bf->getMaxHP();
        }
    }

    return 1 - hp/maxhp;
}

void BattleSimulator::getFighterHP( UInt8 side, GObject::Fighter ** fighters, UInt8 * pos, UInt32 * hp, UInt32 percent )
{
    for(int j = 0; j < 5; ++ j)
    {
        if(fighters[j] == NULL)
            continue;
        BattleObject * obj = getObject(side, pos[j]);
        if(obj != NULL && obj->isChar())
        {
            BattleFighter * bf = static_cast<BattleFighter *>(obj);
            UInt32 hp_ = bf->getHP();
            UInt32 maxhp = bf->getMaxHP();
            if(percent > 0)
            {
                hp_ = (maxhp - hp_) * percent / 100 + hp_;
            }
            if(hp_ >= maxhp)
                hp_ = 0;
            else if(hp_ == 0)
                hp_ = 1;
            hp[j] = hp_;
        }
    }
}

BattleFighter * BattleSimulator::newFighter( UInt8 side, UInt8 pos, GObject::Fighter * fgt )
{
    BattleFighter * bf = new(std::nothrow) Battle::BattleFighter(_formula, fgt, side, pos);
    setObject(side, pos, bf);
    return bf;
}

BattleFighter * BattleSimulator::newPet( UInt8 side, UInt8 pos, GObject::Fighter * fgt)
{
    // 加载仙宠
    BattleFighter * bf = new(std::nothrow) Battle::BattleFighter(_formula, fgt, side, pos);
    setPetObject(side, bf);
    return bf;
}

void BattleSimulator::setStatusChange( UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, UInt32 value, bool active )
{
    for(UInt8 i = pos; i < pos + cnt; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo != NULL)
        {
            if(bo->isChar() && _isBody[side][pos] == 0)
            {
                BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
                switch(type)
                {
                case 3:
                    {
                        bfgt->setActionAdd(value);
                        UInt32 value = static_cast<UInt32>(bfgt->getAction());
                        appendStatusChange(e_stAction, value, skillId, bfgt);
                    }
                    break;
                default:
                    {
                        bfgt->setHP(value);
                        //UInt32 value = static_cast<UInt32>(bfgt->getHP());
                        //appendStatusChange(1, value, 0, bfgt);
                    }
                    break;
                }
            }
        }
    }
}

void BattleSimulator::setStatusChange2(BattleFighter* bf, UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, bool active )
{
    for(UInt8 i = pos; i < pos + cnt; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo != NULL)
        {
            if(bo->isChar() && _isBody[side][i] == 0 && bo->getHP() > 0)
            {
                (this->*statusFuncTable2[type])(bf, side, i, skillId, value, last, active);
#if 0
                BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
                StatusChange& sc = scList[scCount];
                sc.pos = static_cast<UInt8>(i);
                if(!active)
                    sc.pos += 25;
                sc.statusId = skillId;
                sc.type = type;
                switch(type)
                {
                case e_stAtk:
                    bfgt->setAttackAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getAttack());
                    break;
                case e_stDef:
                    bfgt->setDefendAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getDefend());
                    break;
                case e_stEvade:
                    bfgt->setEvadeAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getEvade(NULL));
                    break;
                case e_stCritical:
                    bfgt->setCriticalAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getCritical(NULL));
                    break;
                case e_stPierce:
                    bfgt->setPierceAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getPierce(NULL));
                    break;
                case e_stCounter:
                    bfgt->setCounterAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getCounter(NULL));
                    break;
                case e_stAura:
                    bfgt->AddAura(value);
                    sc.data = static_cast<UInt16>(bfgt->getAura());
                    break;
                case e_stTough:
                    bfgt->setToughAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getTough(NULL));
                    break;
                case e_stMagAtk:
                    bfgt->setMagAttackAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagAttack());
                    break;
                case e_stMagDef:
                    bfgt->setMagDefendAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagDefend());
                    break;
                case e_stMagRes:
                    bfgt->setMagResAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagRes(NULL));
                    break;
                case e_stAction:
                    bfgt->setActionAdd2(value);
                    reQueueFighterStatus(bfgt);
                    sc.data = static_cast<UInt16>(bfgt->getAction());
                    break;
                case e_stCriticalDmg:
                    bfgt->setCriticalDmgAdd2(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCriticalDmg() * 100);
                    break;
                case e_stHitRate:
                    bfgt->setHitrateAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getHitrate(NULL));
                    break;
                case e_stAtkReduce:
                    bfgt->setAtkReduce2(value);
                    sc.data = static_cast<UInt16>(bfgt->getAtkReduce());
                    break;
                case e_stMagAtkReduce:
                    bfgt->setMagAtkReduce2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagAtkReduce());
                    break;
                }
                ++ scCount;
#endif
            }
        }
    }
}


void BattleSimulator::setStatusChange(BattleFighter * bf, UInt8 side, UInt8 pos, int cnt, const GData::SkillBase* skill, UInt8 type, float value, UInt16 last, bool active )
{
    for(UInt8 i = pos; i < pos+cnt; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo != NULL)
        {
            if(bo->isChar() && _isBody[side][pos] == 0 && bo->getHP() > 0)
            {
                (this->*statusFuncTable[type])(bf, side, i, skill, value, last, active);
#if 0
                BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
                StatusChange& sc = scList[scCount];
                sc.pos = static_cast<UInt8>(i);
                if(!active)
                    sc.pos += 25;
                if(skill)
                    sc.statusId = skill->getId();
                else
                    sc.statusId = 0;
                sc.type = type;
                if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
                    ++last;
                switch(type)
                {
                case e_stAtk:
                    bfgt->setAttackAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getAttack());
                    break;
                case e_stDef:
                    bfgt->setDefendAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getDefend());
                    break;
                case e_stEvade:
                    bfgt->setEvadeAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getEvade(NULL));
                    break;
                case e_stCritical:
                    bfgt->setCriticalAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCritical(NULL));
                    break;
                case e_stPierce:
                    bfgt->setPierceAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getPierce(NULL));
                    break;
                case e_stCounter:
                    bfgt->setCounterAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCounter(NULL));
                    break;
                case e_stAura:
                    bfgt->AddAura(value);
                    sc.data = static_cast<UInt16>(bfgt->getAura());
                    break;
                case e_stTough:
                    bfgt->setToughAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getTough(NULL));
                    break;
                case e_stMagAtk:
                    bfgt->setMagAttackAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagAttack());
                    break;
                case e_stMagDef:
                    bfgt->setMagDefendAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagDefend());
                    break;
                case e_stMagRes:
                    bfgt->setMagResAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagRes(NULL));
                    break;
                case e_stAction:
                    bfgt->setActionAdd(value, last);
                    reQueueFighterStatus(bfgt);
                    sc.data = static_cast<UInt16>(bfgt->getAction());
                    break;
                case e_stCriticalDmg:
                    bfgt->setCriticalDmgAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCriticalDmg() * 100);
                    break;
                case e_stHitRate:
                    bfgt->setHitrateAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getHitrate(NULL));
                    break;
                case e_stAtkReduce:
                    bfgt->setAtkReduce(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getAtkReduce());
                    break;
                case e_stMagAtkReduce:
                    bfgt->setMagAtkReduce(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagAtkReduce());
                    break;
                }
                ++ scCount;
#endif
            }
        }
    }
}

bool BattleSimulator::onDead(bool activeFlag, BattleObject * bo)
{
    if(!bo->isChar())
        return true;
    size_t skillIdx = 0;
    const GData::SkillBase* revivalCntSkill;
    while(NULL != (revivalCntSkill = static_cast<BattleFighter*>(bo)->getPassiveSkillRevival100(skillIdx)))
    {
        BattleFighter* bf = static_cast<BattleFighter*>(bo);
        UInt16 revivalCnt = bf->getRevivalCnt();
        UInt16 revivalCntMax = 0;
        if(revivalCntSkill->effect)
            revivalCntMax = revivalCntSkill->effect->efv[0];
        if(revivalCnt < revivalCntMax)
        {
            UInt16 times = revivalCnt / 7;
            float hpPercent = 0.8 - 0.05 * times;
            if(hpPercent < 0.3)
                hpPercent = 0.3;
            UInt32 targetHP = bf->getMaxHP() * hpPercent;
            UInt32 nowHP = bf->getHP();
            if (targetHP >= nowHP)
                bf->regenHP(targetHP - nowHP);

            float defendPercent = 0.7 - 0.1 * times;
            if(defendPercent < 0.1)
                defendPercent = 0.1;
            bf->setDefend(defendPercent);
            bf->setRevivalCnt(revivalCnt + 1);
            appendDefStatus(e_skill, revivalCntSkill->getId(), bf);
            return true;
        }
    }

    bool fRevival = false;
    bool fFakeDead = false;
    bool fSummonOrMirror = false;
    bool bIfDead = false;
    do
    {
        if(static_cast<BattleFighter*>(bo)->hasFlag(BattleFighter::IsMirror))
        {
            fSummonOrMirror = true;
            break;
        }
        if(static_cast<BattleFighter*>(bo)->isPet())
            fSummonOrMirror = true;
        if(static_cast<BattleFighter*>(bo)->getLingQu())
            break;
        if(static_cast<BattleFighter*>(bo)->isSoulOut())
            break;

        if(static_cast<BattleFighter*>(bo)->isSummon())
        {
            BattleFighter* summoner = static_cast<BattleFighter*>(bo)->getSummoner();

            if(summoner)
            {
                summoner->setSelfSummon(NULL);
                UInt8 aura = static_cast<BattleFighter*>(bo)->getUnSummonAura();
                setStatusChange(static_cast<BattleFighter*>(bo), summoner->getSide(), summoner->getPos(), 1, 0, e_stAura, aura, 0, false);
            }

            fSummonOrMirror = true;
            break;
        }

        if(static_cast<BattleFighter*>(bo)->doFireFakeDead())
        {
            fFakeDead = true;
            appendDefStatus(e_unFireFakeDead, 0, static_cast<BattleFighter*>(bo));
            break;
        }

        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDeadFake100(idx);
        if(passiveSkill == NULL)
            passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDeadFake();

        if(passiveSkill != NULL)
        {
            switch(SKILL_ID(passiveSkill->getId()))
            {
            case 226:
                {
                    size_t cnt = passiveSkill->factor.size();
                    UInt16 i = (static_cast<BattleFighter*>(bo))->getFakeDeadTimes();
                    if(i >= cnt)
                        i = cnt - 1;

                    if(uRand(10000) < passiveSkill->prob * passiveSkill->factor[i] * 100)
                    {
                        fFakeDead = true;
                        (static_cast<BattleFighter*>(bo))->fakeDead();
                    }
                }
                break;
            case 215:
                {
                    // 兜率宝伞
                    if(uRand(10000) < passiveSkill->prob * 100)
                    {
                        fFakeDead = true;
                        UInt32 rhp = passiveSkill->effect->hpP * static_cast<BattleFighter*>(bo)->getMaxHP();
                        if(rhp == 0)
                            rhp = 1;
                        (static_cast<BattleFighter*>(bo))->regenHP(rhp);
                        onHPChanged(bo);
                    }
                }
                break;
            default:
                {
                }
                break;
            }

            if(fFakeDead || fRevival)
            {
                appendDefStatus(e_skill, passiveSkill->getId(), static_cast<BattleFighter*>(bo));
                break;
            }
        }

        idx = 0;
        bool fend = false;
        for(;;)
        {
            passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead100(idx);
            if(passiveSkill == NULL)
            {
                passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead();
                fend = true;
                if(!passiveSkill)
                    break;
            }

            appendDefStatus(e_skill, passiveSkill->getId(), static_cast<BattleFighter*>(bo));
            BattleFighter* bf = static_cast<BattleFighter*>(bo);
            if(doSkillEffectExtra_Dead(bf, passiveSkill))
            {
                if(static_cast<BattleFighter*>(bo)->isSoulOut())
                    bIfDead = true;
                fFakeDead = true;
            }
            else
            {
                if(!doSkillEffectExtra_LingShiBleed(bf, NULL, passiveSkill, bf->getMaxHP()))
                    doSkillDmg(bf, passiveSkill);
            }
            if(fend)
                break;
        }

    } while(false);

    if(!fFakeDead && !fRevival)
    {
        // remove from action queue
        BattleFighter* toremove = static_cast<BattleFighter *>(bo);
        onDeadLingshi(toremove);

        removeFighterStatus(toremove);
        {
            BattleFighter* bo2 = static_cast<BattleFighter*>(bo);
            if(bo2)
            {
                if(static_cast<BattleFighter*>(bo)->clearDmgNingShi())
                    appendDefStatus(e_unDmgNingShi, 0, bo2);
            }

            std::vector<BattleFighter*> vbo = bo2->getBeiNingShiZhe();
            for(std::vector<BattleFighter*>::iterator it = vbo.begin(); it != vbo.end(); ++ it)
            {
                BattleFighter* bo = *it;
                if(static_cast<BattleFighter*>(bo)->clearDmgNingShi())
                    appendDefStatus(e_unDmgNingShi, 0, bo);
            }
        }


        size_t idx = 0;
        for(idx = 0; idx < _onTherapy.size(); ++ idx)
        {
            if(_onTherapy[idx] == toremove)
            {
                _onTherapy.erase(_onTherapy.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onSkillDmg.size(); ++ idx)
        {
            if(_onSkillDmg[idx] == toremove)
            {
                _onSkillDmg.erase(_onSkillDmg.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onOtherDead.size(); ++ idx)
        {
            if(_onOtherDead[idx] == toremove)
            {
                _onOtherDead.erase(_onOtherDead.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onPetProtect.size(); ++ idx)
        {
            if (_onPetProtect[idx] == toremove)
            {
                _onPetProtect.erase(_onPetProtect.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onPetAtk.size(); ++ idx)
        {
            if (_onPetAtk[idx] == toremove)
            {
                _onPetAtk.erase(_onPetAtk.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onOtherConfuseAndForget.size(); ++ idx)
        {
            if (_onOtherConfuseAndForget[idx] == toremove)
            {
                _onOtherConfuseAndForget.erase(_onOtherConfuseAndForget.begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onOtherConfuseAndForgetAtkList[0].size(); ++ idx)
        {
            if (_onOtherConfuseAndForgetAtkList[0][idx] == toremove)
            {
                _onOtherConfuseAndForgetAtkList[0].erase(_onOtherConfuseAndForgetAtkList[0].begin() + idx);
                break;
            }
        }
        for(idx = 0; idx < _onOtherConfuseAndForgetAtkList[1].size(); ++ idx)
        {
            if (_onOtherConfuseAndForgetAtkList[1][idx] == toremove)
            {
                _onOtherConfuseAndForgetAtkList[1].erase(_onOtherConfuseAndForgetAtkList[1].begin() + idx);
                break;
            }
        }

        if(!fSummonOrMirror)
        {
            if(!static_cast<BattleFighter*>(bo)->isSoulOut())
                bIfDead = true;
        }

        if(!bIfDead)
        {
            _winner = testWinner();
            if(_winner != 0)
                return true;
            else if(bIfDead)
            {
                doItemWuSkillAttack(static_cast<BattleFighter*>(bo));
            }
        }

        initBuddhaLight(toremove, false, true);
    }

    if(bIfDead)
    {
        _winner = testWinner();
        if(_winner != 0)
            return true;

        doItemWuSkillAttack(static_cast<BattleFighter*>(bo));

        // 五彩石
        for(size_t i = 0; i < _onOtherDead.size(); ++ i)
        {
            BattleFighter* bo2 = _onOtherDead[i];
            if(!bo2 || bo2->getHP() == 0 || bo2 == bo)
                continue;
            const GData::SkillBase* pskill = bo2->getPassiveSkillOnOtherDead();
            if(!pskill)
                continue;

            float flag = bo2->getSide() == bo->getSide() ? activeFlag : !activeFlag;

            appendDefStatus(e_skill, pskill->getId(), bo2);

            UInt8 last = pskill->last;
            if(bo2 == _activeFgt)
                ++ last;
            float atkadd = bo2->_attack * pskill->effect->atkP;
            setStatusChange_Atk(bo2, bo2->getSide(), bo2->getPos(), NULL, atkadd, last, !flag);
            float magatkadd = bo2->_magatk * pskill->effect->magatkP;
            setStatusChange_MagAtk(bo2, bo2->getSide(), bo2->getPos(), NULL, magatkadd, last, !flag);

            if(bo2->getSide() == bo->getSide())
            {
                if(bo2->getConfuseRound() != 0)
                {
                    bo2->setConfuseLevel(0);
                    bo2->setConfuseRound(0);

                    if(bo2->getDeepConfuseLast() != 0)
                    {
                        bo2->setDeepConfuseDmgExtra(0, 0);
                        appendDefStatus(e_unDeepConfuse, 0, bo2);
                    }
                    else
                    {
                        appendDefStatus(e_UnConfuse, 0, bo2);
                    }
                }

                if(bo2->getStunRound() != 0)
                {
                    bo2->setStunLevel(0);
                    bo2->setStunRound(0);

                    if(bo2->getDeepStunLast() != 0)
                    {
                        bo2->setDeepStunDmgExtra(0, 0);
                        appendDefStatus(e_unDeepStun, 0, bo2);
                    }
                    else
                    {
                        appendDefStatus(e_UnStun, 0, bo2);
                    }
                }

                if(bo2->getForgetRound() != 0)
                {
                    bo2->setForgetLevel(0);
                    bo2->setForgetRound(0);

                    if(bo2->getDeepForgetLast() != 0)
                    {
                        bo2->setDeepForgetDmgExtra(0, 0);
                        appendDefStatus(e_unDeepForget, 0, bo2);
                    }
                    else
                    {
                        appendDefStatus(e_UnForget, 0, bo2);
                    }
                }

                if(bo2->getBlind() > 0.001f)
                {
                    bo2->resetBlind();
                    if(bo2->getDeepBlindDmgExtra() > 0.001f)
                    {
                        bo2->resetDeepBlind();
                        appendDefStatus(e_unDeepBlind, 0, bo2);
                    }
                    else
                    {
                        appendDefStatus(e_unBlind, 0, bo2);
                    }
                }

                bo2->setColorStock(GData::e_state_c_s_f_b, SKILL_LEVEL(pskill->getId()), last);
                appendDefStatus(e_Immune3, 0, bo2);
            }
        }
    }
    return true;
}

void BattleSimulator::onDamage( BattleObject * bo, bool active, UInt32 dmg)
{
    // 受到伤害且没有死亡时候触发的各种状态技能
    if(!bo)
        return;
    BattleFighter* bo2 = static_cast<BattleFighter*>(bo);
    if(bo2->getHP() * 3 < bo2->getMaxHP() && !bo2->getLingHpShield())
    {
        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        while(NULL != (passiveSkill = bo2->getPassiveSkillOnHP10P100(idx)))
        {
            //心魔
            if(SKILL_ID(passiveSkill->getId()) == 97)
                continue;
            if(passiveSkill->effect == NULL)
                continue;
            break;
        }

        if(!passiveSkill)
            passiveSkill = bo2->getPassiveSkillOnHP10P();

        if(passiveSkill)
        {
            appendDefStatus(e_skill, passiveSkill->getId(), bo2);
            bo2->setLingHpShield(true);
            doSkillEffectExtraAttack(bo2, bo2->getSide(), bo2->getPos(), passiveSkill);
        }
    }
    UInt8& auraCD = bo2->getAuraDecCD();
    if(auraCD > 0)
    {
        -- auraCD;
        if(auraCD == 0 && bo2->getColorStock() == 0 && !bo2->isSoulOut() && !bo2->isLingQu())
        {
            if(bo2->getImmune2() > 0)
            {
                bo2->setImmune2(0);
                appendDefStatus(e_unImmune2, 0, bo2);
            }
            else
            {
                doAuraPresent(bo2);
                setStatusChange_Aura(NULL, bo2->getSide(), bo2->getPos(), NULL, -100, 0, true);
            }
        }
    }
    UInt8& confuceCD = bo2->getConfuceCD();
    if(confuceCD > 0)
    {
        float rate = bo2->getMagRes(NULL) * 100;
        if(confuceCD == 1 && rate > _rnd(10000)) // 免疫混乱
        {
            appendDefStatus(e_Res, 0, bo2);
        }
        else
        {
            -- confuceCD;
        }
        if(confuceCD == 0 && bo2->getColorStock() == 0 && !bo2->isSoulOut() && !bo2->isLingQu())
        {
            if(bo2->getImmune2() > 0)
            {
                bo2->setImmune2(0);
                appendDefStatus(e_unImmune2, 0, bo2);
            }
            else
            {
                if(!doConfusePresent(bo2))
                {
                    bo2->setConfuseLevel(9);
                    bo2->setConfuseRound(1);
                    appendDefStatus(e_Confuse, 0, bo2);
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                }
            }
        }
    }
    UInt8& stunCD = bo2->getStunCD();
    if(stunCD > 0)
    {
        float rate = bo2->getMagRes(NULL) * 100;
        if(stunCD == 1 && rate > _rnd(10000))
        {
            appendDefStatus(e_Res, 0, bo2); // 免疫眩晕
        }
        else
        {
            -- stunCD;
        }
        if(stunCD == 0 && bo2->getColorStock() == 0 && !bo2->isSoulOut() && !bo2->isLingQu())
        {
            if(bo2->getImmune2() > 0)
            {
                bo2->setImmune2(0);
                appendDefStatus(e_unImmune2, 0, bo2);
            }
            else
            {
                if(!doStunPresent(bo2))
                {
                    bo2->setStunLevel(9);
                    bo2->setStunRound(1);
                    appendDefStatus(e_Stun, 0, bo2);
                    calcAbnormalTypeCnt(bo);
                    calcSilkwormCnt(bo);
                }
            }
        }
    }
    UInt8& blindCD = bo2->getBlindCD();
    if(blindCD > 0)
    {
        float rate = bo2->getMagRes(NULL) * 100;
        if(blindCD == 1 && rate > _rnd(10000))
        {
            appendDefStatus(e_Res, 0, bo2);
        }
        else
        {
            -- blindCD;
        }
        if(blindCD == 0 && bo2->getColorStock() == 0 && !bo2->isSoulOut() && !bo2->isLingQu())
        {
            if(bo2->getImmune2() > 0)
            {
                bo2->setImmune2(0);
                appendDefStatus(e_unImmune2, 0, bo2);
            }
            else
            {
                if(!doBlindPresent(bo2))
                {
                    bo2->setBlind(0.75f, 1);
                    appendDefStatus(e_blind, 0, bo2);
                    calcAbnormalTypeCnt(bo);
                }
            }
        }
    }

    if (dmg)
    {
        const GData::SkillBase* passiveSkill = NULL;
        size_t idx = 0;
        while(NULL != (passiveSkill = bo2->getPassiveSkillOnGetDmg100(idx)))
        {
            // XXX: 写死是被动增益技能，触发上限为3/6（有对应技能符文）
            GData::SkillStrengthenBase* ss = bo2->getSkillStrengthen(SKILL_ID(passiveSkill->getId()));
            if (ss)
            {
                const GData::SkillStrengthenEffect* ef = NULL;
                ef = ss->getEffect(GData::ON_GET_SKILL_PROB, GData::TYPE_PROB_ADD);
                if (_getDamageSkillCount[bo2->getSide()] >= 6)
                    break;
            }
            else if (_getDamageSkillCount[bo2->getSide()] >= 3)
                break;
            ++_getDamageSkillCount[bo2->getSide()];
            int target_side, target_pos, cnt;
            getSkillTarget(bo2, passiveSkill, target_side, target_pos, cnt);
            appendDefStatus(e_skill, passiveSkill->getId(), bo2);
            bool self = false;
            bool ifDecAura = isImmuneDecAura(passiveSkill, target_side, target_pos);
            doSkillStatus(false, bo2, passiveSkill, target_side, target_pos, cnt, self, ifDecAura);
            break;
        }
        while(NULL != (passiveSkill = bo2->getPassiveSkillOnGetDmg()))
        {
            // XXX: 写死是被动增益技能，触发上限为3/6（有对应技能符文）
            GData::SkillStrengthenBase* ss = bo2->getSkillStrengthen(SKILL_ID(passiveSkill->getId()));
            if (ss)
            {
                const GData::SkillStrengthenEffect* ef = NULL;
                ef = ss->getEffect(GData::ON_GET_SKILL_PROB, GData::TYPE_PROB_ADD);
                if (_getDamageSkillCount[bo2->getSide()] >= 6)
                    break;
            }
            else if (_getDamageSkillCount[bo2->getSide()] >= 3)
                break;
            ++_getDamageSkillCount[bo2->getSide()];
            int target_side, target_pos, cnt;
            getSkillTarget(bo2, passiveSkill, target_side, target_pos, cnt);
            appendDefStatus(e_skill, passiveSkill->getId(), bo2);
            bool self = false;
            bool ifDecAura = isImmuneDecAura(passiveSkill, target_side, target_pos);
            doSkillStatus(false, bo2, passiveSkill, target_side, target_pos, cnt, self, ifDecAura);
            break;
        }
    }

    onHPChanged(bo2);

    if(bo2->isDmgDeep())
    {
        doShieldHPAttack(bo2, dmg);
        if(dmg > 0)
        {
            UInt32 value = dmg * bo2->getDmgDeep();
            bo2->makeDamage(value);
            appendDefStatus(e_damNormal, value, bo2);
            if(bo2->getHP() == 0)
                onDead(false, bo2);
        }
    }
#if 0
    if(bo->isChar() && bo->getHP() > 0)
    {
        BattleFighter * bf = static_cast<BattleFighter *>(bo);
        bool ref = bf->hasFlag(BattleFighter::MaxHPRef);
        if(!ref)
        {
            if(bf->getFighter()->getSkill() == 106)
            {
                Script::BattleFormula::SkillData& sd = _formula->skillData(0, 5, bf->getFighter()->getSkillLevel());
                if(_rnd(100) < sd.rate)
                {
                    bf->addFlag(BattleFighter::MaxHPRef);
                    ref = true;
                }
            }
        }
        if(ref)
        {
            Script::BattleFormula::SkillData& sd = _formula->skillData(0, 5, bf->getFighter()->getSkillLevel());
            setStatusChange(bf->getSide(), 0, 25, 106, 1, (sd.value1 + bf->getStrength() * sd.value2) * (bf->getMaxHP() - bf->getHP()) / bf->getMaxHP(), scList, scCount, active);
        }
    }
#endif
}

void BattleSimulator::onHPChanged(BattleObject * bo)
{
    // 战斗单位在HP变化时触发的技能效果
    if (bo->isChar())
    {
        BattleFighter * bf = static_cast<BattleFighter *>(bo);
        if (!bf->getHP()) // 已经死了，不做处理
            return;
        UInt8 boSide = bo->getSide();
        UInt8 boPos = bo->getPos();
        if (_hpCheckCache[boSide][boPos])
            return;
        _hpCheckCache[boSide][boPos] = true;

        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        UInt32 hpr = 0;
        const GData::SkillBase* passiveSkill2 = NULL;
        while(NULL != (passiveSkill = bf->getPassiveSkillOnHPChange100(idx)))
        {
            if(passiveSkill->effect == NULL)
                continue;

            if (!passiveSkill || !passiveSkill->effect)
                passiveSkill = bf->getPassiveSkillOnHPChange(idx);
            if (passiveSkill && passiveSkill->effect)
            {
                int target_side, target_pos, cnt;
                getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
                doSkillEffectExtraAttack(bf, target_side, target_pos, passiveSkill);
                GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(passiveSkill->getId()));
                if(ss)
                {
                    const GData::SkillStrengthenEffect* ef = NULL;

                    // HP减少伤害增加
                    ef = ss->getEffect(GData::ON_HPCHANGE, GData::TYPE_ATKADD);
                    if (ef && bf->updateHPPAttackAdd(ef->value, ef->valueExt1 / 100, ef->valueExt2))
                    {
                        /*
                        setStatusChange(bf, bf->getSide(), bf->getPos(), 1, passiveSkill, e_stAtk, bf->getHPAtkAdd(), 0, false);
                        setStatusChange(bf, bf->getSide(), bf->getPos(), 1, passiveSkill, e_stMagAtk, bf->getHPMagAtkAdd(), 0, false);
                        */
                        appendDefStatus(e_skill, passiveSkill->getId(), bf);
                        appendStatusChange(e_stAtk, static_cast<UInt32>(bf->getAttack()), passiveSkill->getId(), bf);
                        appendStatusChange(e_stMagAtk, static_cast<UInt32>(bf->getMagAttack()), passiveSkill->getId(), bf);
                    }

                    ef = NULL;
                    // HP减少减伤增加
                    ef = ss->getEffect(GData::ON_HPCHANGE, GData::TYPE_DAMAG_REDUCE);
                    if (ef && bf->updateHPPAttackReduce(ef->value, ef->valueExt1 / 100, ef->valueExt2))
                    {
                        /*
                        UInt32 value = static_cast<UInt32>(bf->getAtkReduce()*100);
                        setStatusChange(bf, bf->getSide(), bf->getPos(), 1, passiveSkill, e_stAtkReduce, value, 0, false);
                        value = static_cast<UInt32>(bf->getMagAtkReduce()*100);
                        setStatusChange(bf, bf->getSide(), bf->getPos(), 1, passiveSkill, e_stMagAtkReduce, value, 0, false);
                        */
                        appendDefStatus(e_skill, passiveSkill->getId(), bf);
                        UInt32 value = static_cast<UInt32>(bf->getMagAtkReduce()*100);
                        appendStatusChange(e_stMagAtkReduce, value, passiveSkill->getId(), bf);
                        value = static_cast<UInt32>(bf->getAtkReduce()*100);
                        appendStatusChange(e_stAtkReduce, value, passiveSkill->getId(), bf);
                    }

                    ef = NULL;
                    // HP百分比回复
                    ef = ss->getEffect(GData::ON_HPCHANGE, GData::TYPE_HPP_RECOVER);
                    if (ef)
                    {
                        //hpr += bf->updateHPPRecover2Fake(ef->value / 100, ef->valueExt1 / 100, ef->valueExt2);
                        hpr += bf->updateHPPRecover2Fake(ef->value / 100, ef->valueExt1 / 100, ef->valueExt2);
                        if (hpr)
                            passiveSkill2 = passiveSkill;
                    }

                }
            }
        }
        if (hpr && passiveSkill2)
        {
            bf->regenHP(hpr);
            appendDefStatus(e_skill, passiveSkill2->getId(), bf);
            appendDefStatus(e_damHpAdd, hpr, bf);
        }
        bf->updateLastHPLostP();

        _hpCheckCache[boSide][boPos] = false;
    }
}

BattleFighter * BattleSimulator::getRandomFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
    UInt8 posList[25];
    UInt32 posSize = 0;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo == NULL || bo->getHP() == 0)
            continue;
        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;
        posList[posSize ++] = i;
    }
    if(posSize == 0)
        return NULL;
    return static_cast<BattleFighter *>(getObject(side, posList[_rnd(posSize)]));
}

BattleFighter * BattleSimulator::getMaxHpFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
    // 获取血量最高的散仙
    UInt32 maxHp = 0;
    std::vector<UInt8> posList;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo == NULL || bo->getHP() == 0)
            continue;
        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;
        if (bo->getHP() > maxHp)
        {
            maxHp = bo->getHP();
            posList.clear();
            posList.push_back(i);
        }
        else if (bo->getHP() == maxHp)
        {
            posList.push_back(i);
        }
    }
    if(posList.empty())
        return NULL;
    return static_cast<BattleFighter *>(getObject(side, posList[_rnd(posList.size())]));
}

BattleFighter * BattleSimulator::getMinHpFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
    // 获取血量百分比最少的散仙,  初始设2.0确保能找到1个人
    float minHpP = 2.0f;
    int pos = -1;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter *>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0)
            continue;
        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;
        if (static_cast<float>(bo->getHP())/bo->getMaxHP() < minHpP)
        {
            minHpP = static_cast<float>(bo->getHP())/bo->getMaxHP();
            pos = i;
        }
    }
    if(-1 == pos)
        return NULL;
    return static_cast<BattleFighter *>(getObject(side, pos));
}

BattleFighter * BattleSimulator::getMaxAtkFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
    // 获取攻击力最高的散仙
    float maxAtk = 0;
    std::vector<UInt8> posList;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo == NULL || bo->getHP() == 0 || (!bo->isChar()))
            continue;
        BattleFighter * bf = static_cast<BattleFighter *>(bo);
        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;
        float atk = getBFAttack(bf);
        float magatk = getBFMagAtk(bf);
        float tmpAtk  = atk > magatk ? atk : magatk;
        if (tmpAtk > maxAtk)
        {
            maxAtk = tmpAtk;
            posList.clear();
            posList.push_back(i);
        }
        else if (tmpAtk == maxAtk)
        {
            posList.push_back(i);
        }
    }
    if(posList.empty())
        return NULL;
    return static_cast<BattleFighter *>(getObject(side, posList[_rnd(posList.size())]));
}

BattleFighter * BattleSimulator::get2ndAtkFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
    BattleFighter* bo = getMaxAtkFighter(side, excepts, exceptCount);
    if(!bo)
        return NULL;

    excepts[exceptCount] = bo->getPos();
    ++ exceptCount;

    bo = getMaxAtkFighter(side, excepts, exceptCount);
    return bo;
}

BattleFighter * BattleSimulator::getMinAtkFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
    // 获取攻击力最低的散仙
    float minAtk = 0;
    std::vector<UInt8> posList;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleObject * bo = getObject(side, i);
        if(bo == NULL || bo->getHP() == 0 || (!bo->isChar()))
            continue;
        BattleFighter * bf = static_cast<BattleFighter *>(bo);
        bool except = false;
        for(size_t j = 0; j < exceptCount; ++ j)
        {
            if(excepts[j] == i)
            {
                except = true;
                break;
            }
        }
        if(except)
            continue;
        float atk = getBFAttack(bf);
        float magatk = getBFMagAtk(bf);
        float tmpAtk  = atk > magatk ? atk : magatk;
        if (!minAtk || tmpAtk < minAtk)
        {
            minAtk = tmpAtk;
            posList.clear();
            posList.push_back(i);
        }
        else if (tmpAtk == minAtk)
        {
            posList.push_back(i);
        }
    }
    if(posList.empty())
        return NULL;
    return static_cast<BattleFighter *>(getObject(side, posList[_rnd(posList.size())]));
}

UInt32 BattleSimulator::releaseCD(BattleFighter* bf)
{
    UInt32 rcnt = 0;
    if(NULL == bf)
    {
        return 0;
    }

    do
    {
        std::vector<BattleFighter*> vbo = bf->getBeiNingShiZhe();
        for(std::vector<BattleFighter*>::iterator it = vbo.begin(); it != vbo.end(); ++ it)
        {
            BattleFighter* bo = *it;
            if(bo->getHP() == 0)
                continue;
            UInt32 value = bo->getDmgNingShi();
            makeDamage(bo, value, e_damNormal, e_damageTrue);
        }

        bf->releasePetCoAtk();
        bf->releaseSkillCD(1);
        bf->releaseMoEvade100();

        if(bf->releaseDmgDeep())
        {
            appendDefStatus(e_unDmgDeep, 0, bf);
        }
        if(bf->releaseDmgNingShi())
        {
            appendDefStatus(e_unDmgNingShi, 0, bf);
        }

        if(bf->releaseLingQu())
        {
            appendDefStatus(e_unLingQu, 0, static_cast<BattleFighter*>(bf));
            onDead(true, bf);
            break;
        }

        if(bf->releaseSoulOut())
        {
            appendDefStatus(e_unSoulout, 0, static_cast<BattleFighter*>(bf));
            onDead(true, bf);
            break;
        }

        if(bf->releaseSneakAtk())
        {
            appendDefStatus(e_unSneakAtk, 0, static_cast<BattleFighter*>(bf));
        }

        if(bf->releaseCounterSpirit())
        {
            appendDefStatus(e_unCounterSpirit, 0, bf);
            UInt32 value = static_cast<UInt32>(bf->getAttack());
            appendStatusChange(e_stAtk, value, 0, bf);
            value = static_cast<UInt32>(bf->getMagAttack());
            appendStatusChange(e_stMagAtk, value, 0, bf);
            value = static_cast<UInt32>(bf->getDefend());
            appendStatusChange(e_stDef, value, 0, bf);
            value = static_cast<UInt32>(bf->getMagDefend());
            appendStatusChange(e_stMagDef, value, 0, bf);
        }

        if(bf->releaseFireDefend())
        {
            UInt32 value = static_cast<UInt32>(bf->getMagAttack());
            appendStatusChange(e_stMagAtk, value, 0, bf);
        }
        if(bf->releaseFireFakeDead())
        {
            appendDefStatus(e_unFireFakeDead, 0, bf);
        }

        if (bf->releasePetAttackAdd())
        {
            UInt32 value = static_cast<UInt32>(bf->getAttack());
            appendStatusChange(e_stAtk, value, 0, bf);
        }

        if (bf->releasePetMagAtkAdd())
        {
            UInt32 value = static_cast<UInt32>(bf->getMagAttack());
            appendStatusChange(e_stMagAtk, value, 0, bf);
        }

        if(bf->releaseHpSieldSelf())
        {
            appendDefStatus(e_unHpShieldSelf, 0, bf);
        }

        if(bf->releaseMoAttackAdd())
        {
            UInt32 value = static_cast<UInt32>(bf->getAttack());
            appendStatusChange(e_stAtk, value, 0, bf);
        }
        if(bf->releaseMoMagAtkAdd())
        {
            UInt32 value = static_cast<UInt32>(bf->getMagAttack());
            appendStatusChange(e_stMagAtk, value, 0, bf);
        }
        if(bf->releaseMoAtkReduce())
        {
            UInt32 value = static_cast<UInt32>(bf->getAtkReduce()*100);
            appendStatusChange(e_stAtkReduce, value, 0, bf);
        }
        if(bf->releaseMoMagAtkReduce())
        {
            UInt32 value = static_cast<UInt32>(bf->getMagAtkReduce()*100);
            appendStatusChange(e_stMagAtkReduce, value, 0, bf);
        }

        if(bf->getMoAuraBuf() != 0)
        {
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, bf->getMoAuraBuf(), 0, false);
            if(bf->releaseMoAuraBuf())
            {
                appendDefStatus(e_unMoAuraBuf, 0, bf);
            }
        }
        if(bf->releaseSummon())
        {
            appendDefStatus(e_unSummon, 0, bf);
            onDead(true, bf);
            break;
        }
        if(bf->releaseHideBuf())
        {
            appendDefStatus(e_unHide, 0, bf);
        }
        bool isDeepBlind = (bf->getDeepBlindDmgExtra() > 0);
        if(bf->releaseBlind())
        {
            appendDefStatus(isDeepBlind ? e_unDeepBlind : e_unBlind, 0, bf);
        }
        if(bf->releaseMarkMo())
        {
            appendDefStatus(e_unMarkMo, 0, bf);
        }

        UInt32 confuse = bf->getConfuseRound();
        if(confuse > 0)
        {
            -- confuse;

            if(confuse == 0)
            {
                if(0 != bf->getDeepConfuseLast())
                {
                    bf->setDeepConfuseDmgExtra(0, 0);
                    appendDefStatus(e_unDeepConfuse, 0, bf);
                }
                else
                {
                    appendDefStatus(e_UnConfuse, 0, bf);
                }
            }

            bf->setConfuseRound(confuse);
        }

        UInt32 forget = bf->getForgetRound();
        if(forget > 0)
        {
            -- forget;
            if(forget == 0)
            {
                if(0 != bf->getDeepForgetLast())
                {
                    bf->setDeepForgetDmgExtra(0, 0);
                    appendDefStatus(e_unDeepForget, 0, bf);
                }
                else
                {
                    appendDefStatus(e_UnForget, 0, bf);
                }
            }

            bf->setForgetRound(forget);
        }

        UInt32 stun = bf->getStunRound();
        if(stun > 0)
        {
            -- stun;
            if(stun == 0)
            {
                if(0 != bf->getDeepStunLast())
                {
                    bf->setDeepStunDmgExtra(0, 0);
                    appendDefStatus(e_unDeepStun, 0, bf);
                }
                else
                {
                    appendDefStatus(e_UnStun, 0, bf);
                }
            }

            bf->setStunRound(stun);
        }

        UInt8& atkAdd_last = bf->getAttackAddLast();
        if(atkAdd_last > 0)
        {
            -- atkAdd_last;
           if(0 == atkAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAtk, 0, 0, false);
           else
           {
               if (bf->addRoundAttack())
                   appendDefStatus(e_skill, bf->getAttackRoundAddId(), bf);
               if (bf->subRoundAttack())
                   appendDefStatus(e_skill, bf->getAttackRoundSubId(), bf);
           }
        }

        UInt8& magAtkAdd_last = bf->getMagAttackAddLast();
        if(magAtkAdd_last > 0)
        {
            -- magAtkAdd_last;
           if(0 == magAtkAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagAtk, 0, 0, false);
           else
           {
               if (bf->addRoundMagAtk())
                   appendDefStatus(e_skill, bf->getMagAtkRoundAddId(), bf);
           }
        }

        bool clearDefDec = false;
        UInt8& defAdd_last = bf->getDefendAddLast();
        if(defAdd_last > 0)
        {
            -- defAdd_last;
           if(0 == defAdd_last)
           {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stDef, 0, 0, false);
                float defdec = abs(bf->getDefDec());
                if(defdec > 0.001f)
                {
                    bf->setDefDec(0, 0);
                    bf->setDefDecTimes(0);
                    clearDefDec = true;
                }
           }
        }

        UInt8& magDefAdd_last = bf->getMagDefendAddLast();
        if(magDefAdd_last > 0)
        {
            -- magDefAdd_last;
           if(0 == magDefAdd_last)
           {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagDef, 0, 0, false);
                float defdec = abs(bf->getDefDec());
                if(defdec > 0.001f)
                {
                    bf->setDefDec(0, 0);
                    bf->setDefDecTimes(0);
                    clearDefDec = true;
                }
           }
           else
           {
               if (bf->addRoundMagDef())
                   appendDefStatus(e_skill, bf->getMagDefRoundAdd(), bf);
           }

        }

        UInt8& defendChangeSSLast = bf->getDefendChangeSSLast();
        if (defendChangeSSLast > 0)
        {
            -- defendChangeSSLast;
            if (0 == defendChangeSSLast)
            {
                bf->setDefendChangeSS(0, 0);
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stDef, 0, 0, false);
            }
            else
            {
                if (bf->addRoundDef())
                    appendDefStatus(e_skill, bf->getDefRoundAdd(), bf);
            }
        }

        UInt8& magDefendChangeSSLast = bf->getMagDefendChangeSSLast();
        if (magDefendChangeSSLast > 0)
        {
            -- magDefendChangeSSLast;
            if (0 == magDefendChangeSSLast)
            {
                bf->setMagDefendChangeSS(0, 0);
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagDef, 0, 0, false);
            }
        }

        if(clearDefDec)
        {
            bf->setDefDecTimes(0);
            appendDefStatus(e_undefDec, bf->getDefDecTimes(), bf);
        }

        UInt8& colorStockLast = bf->getColorStockLast();
        if(colorStockLast > 0)
        {
            -- colorStockLast;
            if(0 == colorStockLast)
            {
                bf->setColorStock(0, 0, 0);
                appendDefStatus(e_unImmune3, 0, bf);
            }
        }


        UInt8& hitrateAdd_last = bf->getHitrateAddLast();
        if(hitrateAdd_last > 0)
        {
            -- hitrateAdd_last;
           if(0 == hitrateAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stHitRate, 0, 0, false);
        }

        UInt8& evadeAdd_last = bf->getEvadeAddLast();
        if(evadeAdd_last > 0)
        {
            -- evadeAdd_last;
           if(0 == evadeAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stEvade, 0, 0, false);
        }

        UInt8& criticalAdd_last = bf->getCriticalAddLast();
        if(criticalAdd_last > 0)
        {
            -- criticalAdd_last;
           if(0 == criticalAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stCritical, 0, 0, false);
        }

        // TODO
#if 0
        UInt32 criticalDmgAdd_last = bf->getCriticalDmgAddLast();
        if(criticalDmgAdd_last > 0)
        {
            -- criticalDmgAdd_last;
           if(0 == criticalDmgAdd_last)
                setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stCriticalDmg, 0, 0, false);
        }
#endif

        UInt8& pierceAdd_last = bf->getPierceAddLast();
        if(pierceAdd_last > 0)
        {
            -- pierceAdd_last;
           if(0 == pierceAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stPierce, 0, 0, false);
           else
           {
               if (bf->addRoundPierce())
                   appendDefStatus(e_skill, bf->getPierceRoundAddId(), bf);
           }
        }

        UInt8& counterAdd_last = bf->getCounterAddLast();
        if(counterAdd_last > 0)
        {
            -- counterAdd_last;
           if(0 == counterAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stCounter, 0, 0, false);
        }

        UInt8& magResAdd_last = bf->getMagResAddLast();
        if(magResAdd_last > 0)
        {
            -- magResAdd_last;
           if(0 == magResAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagRes, 0, 0, false);
        }

        UInt8& toughAdd_last = bf->getToughAddLast();
        if(toughAdd_last > 0)
        {
            -- toughAdd_last;
           if(0 == toughAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stTough, 0, 0, false);
        }

        UInt8& atkreduce_last = bf->getAtkReduceLast();
        if(atkreduce_last > 0)
        {
            -- atkreduce_last;
           if(0 == atkreduce_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAtkReduce, 0, 0, false);
        }

        UInt8& magatkreduce_last = bf->getMagAtkReduceLast();
        if(magatkreduce_last > 0)
        {
            -- magatkreduce_last;
           if(0 == magatkreduce_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagAtkReduce, 0, 0, false);
        }

        UInt8& evad100CD = bf->getEvad100CD();
        bool evad100 = bf->getEvad100();
        if(evad100CD > 0 && !evad100)
        {
            --evad100CD;
            if(0 == evad100CD)
            {
                bf->setEvad100(true);
                bf->setEvad100CD(bf->getEvad100BaseCD());
            }
        }

        UInt8& defend100CD = bf->getDefend100CD();
        bool defend100 = bf->getDefend100();
        if(defend100CD > 0 && !defend100)
        {
            --defend100CD;
            if(0 == defend100CD)
            {
                bf->setDefend100(true);
                bf->setDefend100CD(bf->getDefend100BaseCD());
            }
        }

        // TODO
#if 0
        UInt32 maxhpAdd_last = bf->getMaxHPAddLast();
        if(maxhpAdd_last > 0)
        {
            -- maxhpAdd_last;
           if(0 == maxhpAdd_last)
                setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stMaxHP, 0, 0, scList, scCount, false);
        }
#endif

        UInt8& maxActionAdd_last = bf->getActionAddLast();
        if(maxActionAdd_last > 0)
        {
            -- maxActionAdd_last;
           if(0 == maxActionAdd_last)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAction, 0, 0, false);
        }

        if (isPet(bf))
            bf->setPetExAtkEnable(false);

        UInt8& flawLast = bf->getFlawLast();
        if (flawLast > 0)
        {
            -- flawLast;
            if (0 == flawLast)
            {
                bf->setFlaw(0, 0, 0);
                appendDefStatus(e_unFlaw, 0, bf);
            }

        }
        bf->resetWithstandCount();

        UInt8 newModeLast = bf->getNewModeLast();
        if(newModeLast > 0)
        {
            --newModeLast;
            bf->setNewModeLast(newModeLast);
            if(newModeLast == 0)
            {
                appendDefStatus(e_unChangeMode, 0 ,bf);
                bf->setChangeStatus(2);
                bf->unUpdateAllPassiveSkillLingshi();
            }
        }
    }while(false);

    if(_defList.size() > 0 || _scList.size() > 0)
    {
        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 5, 0, false, false);
        ++ rcnt;
    }

    return rcnt;
}

void BattleSimulator::releaseWeak(BattleFighter* bo)
{
    UInt8& last = bo->getTherapyBuffLast();
    if(last > 0)
    {
        float factor = bo->getTherapyBuff();
        -- last;
        if(last == 0)
            bo->setTherapyBuff(0, 0);
        if (factor > 0)  // 增益效果
        {
            appendDefStatus(e_unTherapyBuff, 0, bo);
        }
        else
        {
            appendDefStatus(e_UnWeak, 0, bo);
        }
    }

//    UInt32 weak = bo->getWeakRound();
//    if(weak > 0)
//    {
//        -- weak;
//        if(weak == 0)
//        {
//            defList[defCount].damType = e_UnWeak;
//            defList[defCount].damage = 0;
//            defList[defCount].pos = bo->getPos() + 25;
//            defList[defCount].leftHP = bo->getHP();
//            defCount++;
//        }
//
//        bo->setWeakRound(weak);
//    }
}

bool BattleSimulator::isImmuneDecAura(const GData::SkillBase* skill, int target_side, int target_pos)
{
    if(NULL == skill)
        return false;

    if(skill->effect == NULL)
        return false;

    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(NULL == bo)
        return false;

    bool fimm = false;
    if(skill->effect->state & GData::e_state_dec_aura)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
        if(NULL == bo)
            return false;

        UInt16 immune = bo->getImmune();
        if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel(GData::e_state_dec_aura))
        {
            fimm = true;
            appendDefStatus(e_Immune, 0, bo);
        }
    }

    return fimm;
}

void BattleSimulator::setStatusChange2_Atk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setAttackAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAttack());
    appendStatusChange(e_stAtk, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Def(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setDefendAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getDefend());
    appendStatusChange(e_stDef, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Evade(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setEvadeAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getEvade(NULL)*100);
    appendStatusChange(e_stEvade, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Critical(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setCriticalAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getCritical(NULL)*100);
    appendStatusChange(e_stCritical, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Pierce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setPierceAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getPierce(NULL)*100);
    appendStatusChange(e_stPierce, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Counter(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setCounterAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getCounter(NULL)*100);
    appendStatusChange(e_stCounter, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Aura(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->AddAura(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAura());
    appendStatusChange(e_stAura, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Tough(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setToughAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getTough(NULL)*100);
    appendStatusChange(e_stTough, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_MagAtk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setMagAttackAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagAttack());
    appendStatusChange(e_stMagAtk, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_MagDef(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setMagDefendAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagDefend());
    appendStatusChange(e_stMagDef, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_MagRes(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setMagResAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagRes(NULL)*100);
    appendStatusChange(e_stMagRes, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_Action(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setActionAdd2(value);
    reQueueFighterStatus(bfgt);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAction());
    appendStatusChange(e_stAction, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_CriDmg(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setCriticalDmgAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getCriticalDmg() * 100);
    appendStatusChange(e_stCriticalDmg, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_HitR(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setHitrateAdd2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getHitrate(NULL)*100);
    appendStatusChange(e_stHitRate, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_AtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setAtkReduce2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAtkReduce()*100);
    appendStatusChange(e_stAtkReduce, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange2_MagAtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    bfgt->setMagAtkReduce2(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagAtkReduce()*100);
    appendStatusChange(e_stMagAtkReduce, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Atk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<float>& efv = skill->effect->efv;
        const std::vector<UInt8>& efl = skill->effect->efl;
        if(!efv.empty())
        {
            for(size_t i = 0; i < eft.size(); ++ i)
            {
                if(eft[i] == GData::e_eft_round_add)
                {
                    bf->setAttackRoundAdd(efv[i], efl[i], skillId);
                    break;
                }
                else if(eft[i] == GData::e_eft_round_sub)
                {
                    bf->setAttackRoundSub(efv[i], efl[i], skillId);
                    break;
                }
            }
        }
    }
    else
    {
        bf->setAttackRoundAdd(0, 0);
        bf->setAttackRoundSub(0, 0);
    }

    bfgt->setAttackAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAttack());
    appendStatusChange(e_stAtk, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Def(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<float>& efv = skill->effect->efv;
        const std::vector<UInt8>& efl = skill->effect->efl;
        if(!efv.empty())
        {
            for(size_t i = 0; i < eft.size(); ++ i)
            {
                if(eft[i] == GData::e_eft_round_add)
                {
                    bf->setDefRoundAdd(efv[i], efl[i], skillId);
                    break;
                }
            }
        }
    }
    else
        bf->setDefRoundAdd(0, 0);
    bfgt->setDefendAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getDefend());
    appendStatusChange(e_stDef, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Evade(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setEvadeAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getEvade(NULL)*100);
    appendStatusChange(e_stEvade, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Critical(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setCriticalAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getCritical(NULL)*100);
    appendStatusChange(e_stCritical, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Pierce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<float>& efv = skill->effect->efv;
        const std::vector<UInt8>& efl = skill->effect->efl;
        if(!efv.empty())
        {
            for(size_t i = 0; i < eft.size(); ++ i)
            {
                if(eft[i] == GData::e_eft_round_add)
                {
                    bf->setPierceRoundAdd(efv[i], efl[i], skillId);
                    break;
                }
            }
        }
    }
    else
        bf->setPierceRoundAdd(0, 0);
    bfgt->setPierceAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getPierce(NULL)*100);
    appendStatusChange(e_stPierce, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Counter(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setCounterAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getCounter(NULL)*100);
    appendStatusChange(e_stCounter, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Aura(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    if(value < 0)
    {
        if(getItemXin_MagRes(bfgt, GData::e_state_dec_aura))
        {
            return;
        }
    }

    bfgt->AddAura(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAura());
    appendStatusChange(e_stAura, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Aura2(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }

    bfgt->AddAura(value);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAura());
    appendStatusChange(e_stAura, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Tough(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setToughAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getTough(NULL)*100);
    appendStatusChange(e_stTough, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_MagAtk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<float>& efv = skill->effect->efv;
        const std::vector<UInt8>& efl = skill->effect->efl;
        if(!efv.empty())
        {
            for(size_t i = 0; i < eft.size(); ++ i)
            {
                if(eft[i] == GData::e_eft_round_add)
                {
                    bf->setMagAttackRoundAdd(efv[i], efl[i], skillId);
                    break;
                }
            }
        }
    }
    else
        bf->setMagAttackRoundAdd(0, 0);


    UInt32 value2 = static_cast<UInt32>(bfgt->getMagAttack());
    appendStatusChange(e_stMagAtk, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_MagDef(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<float>& efv = skill->effect->efv;
        const std::vector<UInt8>& efl = skill->effect->efl;
        if(!efv.empty())
        {
            for(size_t i = 0; i < eft.size(); ++ i)
            {
                if(eft[i] == GData::e_eft_round_add)
                {
                    bf->setMagDefRoundAdd(efv[i], efl[i], skillId);
                    break;
                }
            }
        }
    }
    else
        bf->setMagDefRoundAdd(0, 0);
    if(isFireDefend(skill))
        bfgt->setFireDefend(value, last);
    else
        bfgt->setMagDefendAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagDefend());
    appendStatusChange(e_stMagDef, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_MagRes(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setMagResAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagRes(NULL)*100);
    appendStatusChange(e_stMagRes, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_Action(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setActionAdd(value, last);
    reQueueFighterStatus(bfgt);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAction());
    appendStatusChange(e_stAction, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_CriDmg(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setCriticalDmgAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getCriticalDmg() * 100);
    appendStatusChange(e_stCriticalDmg, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_HitR(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setHitrateAdd(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getHitrate(NULL)*100);
    appendStatusChange(e_stHitRate, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_AtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setAtkReduce(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getAtkReduce()*100);
    appendStatusChange(e_stAtkReduce, value2, skillId, bfgt);
}

void BattleSimulator::setStatusChange_MagAtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active )
{
    BattleObject * bo = getObject(side, pos);
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    UInt16 skillId = 0;
    if(skill)
    {
        skillId = skill->getId();
        if(skill->cond == GData::SKILL_BEATKED && bf != bfgt)
            ++last;
    }
    bfgt->setMagAtkReduce(value, last);
    UInt32 value2 = static_cast<UInt32>(bfgt->getMagAtkReduce()*100);
    appendStatusChange(e_stMagAtkReduce, value2, skillId, bfgt);
}


bool BattleSimulator::doSkillStrengthen_disperse(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    if(_rnd(10000) < ef->value*100)
    {
        if(bo->getConfuseRound() != 0)
        {
            bo->setConfuseLevel(0);
            bo->setConfuseRound(0);

            if(bo->getDeepConfuseLast() != 0)
            {
                bo->setDeepConfuseDmgExtra(0, 0);
                appendDefStatus(e_unDeepConfuse, 0, bo);
            }
            else
            {
                appendDefStatus(e_UnConfuse, 0, bo);
            }
        }

        if(bo->getStunRound() != 0)
        {
            bo->setStunLevel(0);
            bo->setStunRound(0);

            if(bo->getDeepStunLast() != 0)
            {
                bo->setDeepStunDmgExtra(0, 0);
                appendDefStatus(e_unDeepStun, 0, bo);
            }
            else
            {
                appendDefStatus(e_UnStun, 0, bo);
            }
        }

        if(bo->getForgetRound() != 0)
        {
            bo->setForgetLevel(0);
            bo->setForgetRound(0);

            if(bo->getDeepForgetLast() != 0)
            {
                bo->setDeepForgetDmgExtra(0, 0);
                appendDefStatus(e_unDeepForget, 0, bo);
            }
            else
            {
                appendDefStatus(e_UnForget, 0, bo);
            }
        }

        if(bo->getBlind() > 0.001f)
        {
            bo->resetBlind();
            if(bo->getDeepBlindDmgExtra() > 0.001f)
            {
                bo->resetDeepBlind();
                appendDefStatus(e_unDeepBlind, 0, bo);
            }
            else
            {
                appendDefStatus(e_unBlind, 0, bo);
            }
        }
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_reduce(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    {
        UInt16 skillId = 0;
        if(skill)
            skillId = skill->getId();

        bo->setAtkReduce3(ef->value, ef->last);
        UInt32 value = static_cast<UInt32>(bo->getAtkReduce()*100);
        appendStatusChange(e_stAtkReduce, value, skillId, bo);
    }
    {
        UInt16 skillId = 0;
        if(skill)
            skillId = skill->getId();

        bo->setMagAtkReduce3(ef->value, ef->last);
        UInt32 value = static_cast<UInt32>(bo->getMagAtkReduce()*100);
        appendStatusChange(e_stMagAtkReduce, value, skillId, bo);
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_week(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    if( ((float)(bo->getHP()-1)/bo->getMaxHP()) < ef->value/100)
    {
        bo->setImmune2(GData::e_state_c_s_f_b);
        appendDefStatus(e_Immune2, 0, bo);
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_debuf_defend(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    {
        bo->setDefDec(ef->value, ef->last);
        bo->setDefDecTimes(0);
        appendDefStatus(e_defDec, 0, bo);
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_atkadd(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    bool flag = bf->getSide() == target_side ? !active : active;
    UInt16 last = ef->last + (bf == bo ? 1 : 0);
    float atkadd = bo->_attack * ef->value/100;
    setStatusChange_Atk(bf, target_side, target_pos, skill, atkadd, last, flag );
    float magatkadd = bo->_magatk * ef->value/100;
    setStatusChange_MagAtk(bf, target_side, target_pos, skill, magatkadd, last, flag );

    return true;
}

// 附加value%魔法攻击力，持续last次有效
bool BattleSimulator::doSkillStrengthen_addMagicAtk(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !ef || !skill || bf->getHP() == 0)
        return false;

    float fAddMagAtk = bf->_magatk * ef->value/100;
    SetSpecialAttrChange(bf, skill, e_ss_MagAtk, ef->last, fAddMagAtk, active);

    return true;
}

bool BattleSimulator::doSkillStrengthen_absorbMagAtk(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bf->getHP() == 0)
        return false;

    // bf出手的人，bo被攻击者
    float fAddAttack = bo->_magatk*ef->value/100;
    SetSpecialAttrChange(bf, skill, e_ss_MagAtk, ef->last, fAddAttack, active);
    SetSpecialAttrChange(bo, skill, e_ss_DecMagAtk, ef->last, -fAddAttack, !active);
    return true;
}

// 吸收对方攻击力，作用到下一次攻击
bool BattleSimulator::doSkillStrengthen_absorbAtk(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bf->getHP() == 0)
        return false;

    // bf出手的人，bo被攻击者
    float fAddAttack = bo->_attack*ef->value/100;
    SetSpecialAttrChange(bf, skill, e_ss_Atk, ef->last, fAddAttack, active);
    SetSpecialAttrChange(bo, skill, e_ss_DecAtk, ef->last, -fAddAttack, !active);
    return true;
}

bool BattleSimulator::doSkillStrengthen_BleedBySkill(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bf->getHP() == 0 || bo->getHP() <= 0)
        return false;

   // UInt32 nBleed = (bf, bo, ef->value/100);
    UInt32 nBleed = abs(calcPoison(bf, skill, bo, false)) * ef->value/100;
    UInt8 nClass = bf->getClass();
    if(nBleed > 0)
    {
        /*
        bo->setBleedBySkill(nBleed, ef->last);
        bo->setBleedBySkillClass(nClass);
        */
        if(nClass == e_cls_ru)
        {
            bo->setBleed1(nBleed, ef->last);
            appendDefStatus(e_Bleed1, 0, bo);
        }
        else if(nClass == e_cls_shi)
        {
            bo->setBleed2(nBleed, ef->last);
            appendDefStatus(e_Bleed2, 0, bo);
        }
        else if (nClass == e_cls_dao)
        {
            bo->setBleed3(nBleed, ef->last);
            appendDefStatus(e_Bleed3, 0, bo);
        }
        else
        {
            bo->setBleedMo(nBleed, ef->last);
            appendDefStatus(e_BleedMo, 0, bo);
        }
    }
    return true;
}

bool BattleSimulator::doSkillStrengthen_AttackFriend(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bf->getHP() == 0 || bo->getHP() <= 0)
        return false;

    // 找出本方血最少的人，对他进行攻击
    UInt32 minHP = 0xffffffff;
    UInt8 minHP_pos = 0;
    BattleFighter* fighter = NULL;
    for(UInt8 i=0; i<25; ++i)
    {
        if( i == bo->getPos())  // 除目标以外的人
            continue;

        fighter = static_cast<BattleFighter*>(getObject(target_side, i));
        if(fighter && fighter->getHP() > 0 &&  fighter->getHP() < minHP)
        {
            minHP = fighter->getHP();
            minHP_pos = i;
        }
    }
    fighter = static_cast<BattleFighter*>(getObject(target_side, minHP_pos));
    if(fighter && fighter->getHP() > 0)  // 攻击血最少的人
    {
        StateType eType = e_MAX_STATE;
        UInt32 dmg = CalcNormalAttackDamage(bo, fighter, eType);  // bo 打 fighter
        if (eType == e_MAX_STATE)
            return false;

        dmg *= ef->value/100;
        if (eType == e_damNormal)
        {
            // 这个加进去，可以看到人跑到友人那边攻击一次？
            appendDefStatus(e_xinmo, 0, bf);
            makeDamage(fighter, dmg, eType, e_damagePhysic); // 这里应该都是普通物理攻击吧
        }
    }
    return true;
}

bool BattleSimulator::doSkillStrengthen_ShieldHP(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !ef || !skill || bf->getHP() == 0)
        return false;

    float value = bf->getMaxHP() * (ef->value / 100);
    bf->setShieldHPBuf(value, ef->last);

    return true;
}

UInt32 BattleSimulator::CalcNormalAttackDamage(BattleFighter * bf, BattleObject* bo, StateType& eStateType)
{
    if(!bf || !bo || bf->getHP() == 0 || bo->getHP() == 0)
        return 0;
    UInt32 dmg = 0;

    BattleFighter * area_target = static_cast<BattleFighter *>(bo);
    UInt8 target_stun = area_target->getStunRound();
    bool enterEvade = area_target->getEvad100();
    bool defend100 = area_target->getDefend100();

    bool pr = false;
    bool cs = false;
    if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(area_target, NULL))))
    {
        pr = bf->calcPierce(area_target);
        float atk = 0;
        float cf = 0.0f;
        atk = calcAttack(bf, cs, area_target, &cf);
        if(cs )
        {
            UInt8 s = bf->getSide();
            if(s < 2)
                _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

        }

        float toughFactor = pr ? area_target->getTough(bf) : 1.0f;
        float def = getBFDefend(area_target);
        float atkreduce = getBFAtkReduce(area_target);
        dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);
        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
        dmg = dmg > 0 ? dmg : 1;
        eStateType = e_damNormal;
    }
    else if(!defend100 && !enterEvade)
        eStateType = e_damEvade;
    else
    {
        if(enterEvade)
        {
            eStateType = e_damEvade;
            area_target->setEvad100(false);
        }

        if(defend100)
        {
            eStateType = e_damOut;
            area_target->setDefend100(false);
        }
    }

    return dmg;
}

bool BattleSimulator::doSkillStrengthen_DebufAura( BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active )
{
    BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
    if(!bo || !bf || !ef || !skill || bf->getHP() == 0)
        return false;

    bo->setAuraDec(ef->value, ef->last);
    return true;
}

bool BattleSimulator::doSkillStrengthen_bufTherapy( BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active )
{
    if(!bf || !ef || bf->getHP() <= 0)
        return false;

    if(bf->getTherapyBuff() != 0)  // 去掉以前的buff
    {
        if (bf->getTherapyBuff() > 0)
        {
            appendDefStatus(e_unTherapyBuff, 0, bf);
        }
        else
        {
            appendDefStatus(e_UnWeak, 0, bf);
        }
    }

    appendDefStatus(e_TherapyBuff, 0, bf);
   // bf->setTherapyAdd(ef->value/100, ef->last);
    bf->setTherapyBuff(ef->value/100, ef->last);
    return true;
}

// bOffset标记通知前端的时候要不要+25的偏移。对于本次的出手者来说，本方阵营id需要偏移25，敌方的不用
// skill导致bf的属性etype产生value的改变，持续last次有效，新的覆盖旧的。
void BattleSimulator::SetSpecialAttrChange(BattleFighter* bf, const GData::SkillBase* skill, SpecialStatus eType, Int16 nLast, float value, bool bOffset)
{
    if(!bf || eType >= MAX_SPECIAL_STATUS || eType <= MIN_SPECIAL_STATUS)
        return;

    if(nLast <= 0)
        value = 0;  // 避免这个加成值永远无法被清除

    UInt16 skillId = 0;
    if(skill)
        skillId = skill->getId();
    //sc.type = eType;
    switch(eType)
    {
        case e_ss_Atk:
            {
                bf->setAtkAddSpecial(value, nLast);
                UInt32 value2 = static_cast<UInt32>(bf->getAttack());
                appendStatusChange(e_stAtk, value2, skillId, bf);
            }
            break;
        case e_ss_DecAtk:
            {
                bf->setAtkDecSpecial(value, nLast);
                UInt32 value2 = static_cast<UInt32>(bf->getAttack());
                appendStatusChange(e_stAtk, value2, skillId, bf);
            }
            break;
        case e_ss_MagAtk:
            {
                bf->setMagAtkAddSpecial(value, nLast);
                UInt32 value2 = static_cast<UInt32>(bf->getMagAttack());
                appendStatusChange(e_stMagAtk, value2, skillId, bf);
            }
            break;
        case e_ss_DecMagAtk:
            {
                bf->setMagAtkDecSpecial(value, nLast);
                UInt32 value2 = static_cast<UInt32>(bf->getMagAttack());
                appendStatusChange(e_stMagAtk, value2, skillId, bf);
            }
            break;
        default:
            break;
    }

    return;
}

void BattleSimulator::ReduceSpecialAttrLast(BattleFighter* bf, SpecialStatus eType, Int16 nReduce)
{
    if(!bf || eType >= MAX_SPECIAL_STATUS || eType <= MIN_SPECIAL_STATUS)
        return;

    Int16 nLast = 0;
    switch(eType)
    {
        case e_ss_Atk:
            {
                nLast = bf->getAtkSpecialLast();
                nLast -= nReduce;
                if (nLast > 0)
                    bf->setAtkSpecialLast(nLast);
            }
            break;
        case e_ss_DecAtk:
            {
                nLast = bf->getAtkDecSpecialLast();
                nLast -= nReduce;
                if (nLast > 0)
                    bf->setAtkDecSpecialLast(nLast);
            }
            break;
        case e_ss_MagAtk:
            {
                nLast = bf->getMagAtkSpecialLast();
                nLast -= nReduce;
                if (nLast > 0)
                    bf->setMagAtkSpecialLast(nLast);
            }
            break;
        case e_ss_DecMagAtk:
            {
                nLast = bf->getMagAtkDecSpecialLast();
                nLast -= nReduce;
                if (nLast > 0)
                    bf->setMagAtkDecSpecialLast(nLast);
            }
            break;
        default:
            break;
    }
    if(nLast <= 0)
    {
        SetSpecialAttrChange(bf, NULL, eType, 0, 0, true);
    }
    return;
}


bool BattleSimulator::BleedRandom_SkillStrengthen(BattleFighter* bf, BattleFighter* bo, const GData::SkillStrengthenEffect* ef)
{
    if(!bf || !bo || !ef)
        return false;

    UInt32 nBleed = GetBleedDmg(bf, bo, ef->value/100);
    UInt8 nClass = bf->getClass();
    if(nBleed > 0)
    {
        /*
        bo->setBleedRandom(nBleed, ef->last);
        bo->setBleedAttackClass(nClass);
        */
        switch(nClass)
        {
        case e_cls_ru:
            bo->setBleed1(nBleed, ef->last);
            appendDefStatus(e_Bleed1, 0, bo);
            break;
        case e_cls_shi:
            bo->setBleed2(nBleed, ef->last);
            appendDefStatus(e_Bleed2, 0, bo);
            break;
        case e_cls_dao:
            bo->setBleed3(nBleed, ef->last);
            appendDefStatus(e_Bleed3, 0, bo);
            break;
        case e_cls_mo:
            bo->setBleedMo(nBleed, ef->last);
            appendDefStatus(e_BleedMo, 0, bo);
            break;
        }
    }
    return true;
}

UInt32 BattleSimulator::GetBleedDmg(BattleFighter* bf, BattleFighter* bo, float nfactor)
{
    UInt32 nRetDmg = 0;
    if(!bf || !bo)
        return nRetDmg;
    float def = 0;
    float reduce = 0;
    float attack = 0;
    UInt8 nAtkClass = bf->getClass();
    if(e_cls_dao == nAtkClass || e_cls_mo == nAtkClass) // 道，物攻
    {
        def = getBFDefend(bo);
        reduce = getBFAtkReduce(bo);
        attack = getBFAttack(bf);
    }
    else
    {
        def = getBFMagDefend(bo);
        reduce = getBFMagAtkReduce(bo);
        attack = getBFMagAtk(bf);
    }
    nRetDmg = _formula->calcDamage(nfactor*attack, def, bf->getLevel(), 1.0f, reduce); // 坚韧是在破击的时候起作用的，这里暴击、破击都不算
    return nRetDmg;
}

bool BattleSimulator::AddYuanCiState_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill, const int nAttackCount)
{
    if(!pFighter || !pTarget || !skill || nAttackCount <3)
        return false;
    if(SKILL_ID(skill->getId()) != 122)  // 元磁神雷专用，别的不能用啊。。。
        return false;
    GData::SkillStrengthenBase* ss = pFighter->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return false;
    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_YUANCISHENLEI);
    if(!ef)
        return false;
    // 上状态，判断免疫、抵抗（反弹，反弹不能被反弹），出战报数据
    //std::vector<AttackAct> atkAct2;
    //atkAct2.clear();
    UInt8 nState = 2;  // 混乱
    if(nAttackCount == 3)
        nState = 8;   // 沉默
    float fRate = ef->value*100;  // value%的机会上状态
    if (fRate > _rnd(10000))
        AddSkillStrengthenState(pFighter, pTarget, skill->getId(), nState, ef->last);
    return true;
}

bool BattleSimulator::AddExtraDamageAfterResist_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill, int nDamage)
{
    if(!pFighter || !pTarget || !skill)
        return false;
    GData::SkillStrengthenBase* ss = pFighter->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return false;
    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_RESIST, GData::TYPE_DAMAGE_EXTRA);
    if(!ef)
        return false;

    UInt32 nRealDamage = ef->value/100 * nDamage;  // 伤害值
    makeDamage(pTarget, nRealDamage, e_damNormal, e_damageTrue);

    return true;
}

bool BattleSimulator::AddStateAfterPoisonResist_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill, int nfactor)
{
    if(!pFighter || !pTarget || !skill)
        return false;
    int nIndex = pFighter->getAttackIndex();
    if (nIndex <= 0 || nIndex > 3)  // 只有第1-3的对象才上状态，囧一个
        return false;

    GData::SkillStrengthenBase* ss = pFighter->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return false;
    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_RESIST, GData::TYPE_RESIST_ADDSTATE);
    if(!ef)
        return false;

    float fRate = 0;
    if (nIndex == 1)
    {
        fRate = nfactor*ef->value*100;
    }
    else if(nIndex == 2)
    {
        fRate = nfactor*ef->valueExt1*100;
    }
    else if(nIndex == 3)
    {
        fRate = nfactor*ef->valueExt2*100;
    }

    UInt8 nState = 4;  // dizz
    if (fRate > _rnd(10000))
        AddSkillStrengthenState(pFighter, pTarget, skill->getId(), nState, ef->last);

    return true;
}

bool BattleSimulator::AddStateAfterResist_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill)
{
    if(!pFighter || !pTarget || !skill)
        return false;
    GData::SkillStrengthenBase* ss = pFighter->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return false;
    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_RESIST, GData::TYPE_ADDSTATE);
    if(!ef)
        return false;
    UInt8 nState = ef->valueExt1;
    float fRate = ef->value*100;
    if (fRate > _rnd(10000))
        AddSkillStrengthenState(pFighter, pTarget, skill->getId(), nState, ef->last);

    return true;
}

bool BattleSimulator::AddSkillStrengthenState(BattleFighter* pFighter, BattleFighter* pTarget,const UInt16 nSkillId, const UInt8 nState, const Int16 nLast)
{
    if(!pFighter || !pTarget || pFighter->getHP() <= 0 || pTarget->getHP() < 0)
        return false;
    if(pTarget->isLingQu() || pTarget->isSoulOut())
        return false;

    std::vector<AttackAct> vAttackAct; // 这个存被动技能等造成的更多动作
    vAttackAct.clear();
    // 下面开始上状态的逻辑
    UInt8 arrayState[3] = {0};
    UInt8 nCount = 0;
    UInt8 nIndex = 0;
    arrayState[0] = nState;
    if(nState & 0x2e)
    {
        if(nState & 0x2)
        {
            arrayState[nCount] = 0x2;
            ++nCount;
        }
        if(nState & 0x4)
        {
            arrayState[nCount] = 0x4;
            ++nCount;
        }
        if(nState & 0x8)
        {
            arrayState[nCount] = 0x8;
            ++nCount;
        }
        if(nState & 0x20)
        {
            arrayState[nCount] = 0x20;
            ++nCount;
        }
        if(nCount > 1)
            nIndex = _rnd(nCount);
    }
    UInt16 nImmu = pTarget->getImmune();
    UInt16 nImmu2 = pTarget->getImmune2();
    UInt16 colorStock = pTarget->getColorStock();
    if(arrayState[nIndex] & colorStock)
    {
        appendDefStatus(e_Immune, 0, pTarget);
        return false;
    }

    if(arrayState[nIndex]& nImmu2)
    {
        pTarget->setImmune2(0);
        appendDefStatus(e_unImmune2, 0, pTarget);  // 技能符文里面的那个免疫起效了
        return true;
    }
    if(((arrayState[nIndex] & nImmu) && SKILL_LEVEL(nSkillId) <= pTarget->getImmuneLevel(arrayState[nIndex])) || ((arrayState[nIndex] & pTarget->getImmune3())))
    {
        appendDefStatus(e_Immune, 0, pTarget);
        return true;
    }
    float fResRate = pTarget->getMagRes(pFighter) * 100;
    if(fResRate > _rnd(10000))  // 成功抵抗
    {
        size_t nidx = 0;
        // 执行被动技能的抵抗后效果
        const GData::SkillBase* pPassiveSkill = pTarget->getPassiveSkillAftRes();
        if(!pPassiveSkill)
        {
            nidx = 0;
            while(!pPassiveSkill)
            {
                size_t oidx = nidx;
                pPassiveSkill = pTarget->getPassiveSkillAftRes100(nidx);
                if(oidx == nidx)
                    break;
            }
        }
        if(pPassiveSkill)
        {
            AttackAct aa = {0};
            aa.bf = pTarget;
            aa.skill = pPassiveSkill;
            aa.target_side = pFighter->getSide();
            aa.target_pos = pFighter->getPos();
            aa.param = nSkillId;
            UInt32 nstateLast = arrayState[nIndex];
            nstateLast = (nstateLast<< 16) + nLast;
            aa.param1 = nstateLast;
            vAttackAct.push_back(aa);
            appendDefStatus(e_Res, 0, pTarget);
        }
        else
        {
            appendDefStatus(e_ResR, 0, pTarget);
        }

        if(vAttackAct.size() > 0) // 被动技能作用
            doSkillAtk2(false, &vAttackAct);
        return true;
    }
    switch(arrayState[nIndex])
    {
        case 1:
            {
            }
            break;
        case 2:
            {
                if(pTarget->getDeepConfuseDmgExtra() > 0.001f)
                    break;
                if(pTarget->getConfuseRound() < 1)
                {
                    pTarget->setConfuseLevel(SKILL_LEVEL(nSkillId));
                    pTarget->setConfuseRound(nLast);
                    appendDefStatus(e_Confuse, 0, pTarget);
                    calcAbnormalTypeCnt(pTarget);
                    calcSilkwormCnt(pTarget);
                }
            }
            break;
        case 4:
            {
                if(pTarget->getDeepStunDmgExtra() > 0.001f)
                    break;
                if(pTarget->getStunRound() < 1)
                {
                    pTarget->setStunLevel(SKILL_LEVEL(nSkillId));
                    pTarget->setStunRound(nLast);
                    appendDefStatus(e_Stun, 0, pTarget);
                    calcAbnormalTypeCnt(pTarget);
                    calcSilkwormCnt(pTarget);
                }
            }
            break;
        case 8:
            {
                if(pTarget->getDeepForgetDmgExtra() > 0.001f)
                    break;
                if(pTarget->getForgetRound() < 1)
                {
                    pTarget->setForgetLevel(SKILL_LEVEL(nSkillId));
                    pTarget->setForgetRound(nLast);
                    appendDefStatus(e_Forget, 0, pTarget);
                    calcAbnormalTypeCnt(pTarget);
                    calcSilkwormCnt(pTarget);
                }
            }
            break;
        case 0x20:
            {
                // 虚弱，就是治疗效果减半。。。
                if(pTarget->getTherapyBuff() != 0)  // 去掉以前的buff
                {
                    if (pTarget->getTherapyBuff() > 0)
                    {
                        appendDefStatus(e_unTherapyBuff, 0, pTarget);
                    }
                    else
                    {
                        appendDefStatus(e_UnWeak, 0, pTarget);
                    }
                }
                pTarget->setTherapyBuff(-0.5f, nLast);
                appendDefStatus(e_Weak, 0, pTarget);
//                if(pTarget->getWeakRound() < 1)
//                {
//                    defList[defCount].damType = e_Weak;
//                    pTarget->setWeakLevel(SKILL_LEVEL(nSkillId));
//                    pTarget->setWeakRound(nLast);
//                }
            }
            break;
        default:
            break;
    }
    return true;
}


void BattleSimulator::ModifySingleAttackValue_SkillStrengthen(BattleFighter* bf,const GData::SkillBase* skill, float& fvalue, bool isAdd)
{
    if(!bf->getSingleAttackFlag() || !skill)
        return;

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return;

    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACKSINGLE, GData::TYPE_INTENSIFYSTATE);
    if(ef)
    {
        if(isAdd)
            fvalue += ef->value;
        else
            fvalue -= ef->valueExt1;  // 减掉的值取这个
    }
}

void BattleSimulator::ModifyAttackValue_SkillStrengthen(BattleFighter* bf,const GData::SkillBase* skill, float& fvalue, bool isAdd)
{
    if(!skill)
        return;

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return;

    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_INTENSIFYSTATE);
    if(ef)
    {
        if(isAdd)
            fvalue += ef->value / 100;
        else
            fvalue -= ef->valueExt1 / 100;  // 减掉的值取这个
    }
}


void BattleSimulator::ModifyAttackValue_SkillStrengthen_Other(BattleFighter* bf,const GData::SkillBase* skill, float& fvalue, bool isAdd)
{
    if(!skill)
        return;

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return;

    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_INTENSIFYSTATE_OTHER);
    if(ef)
    {
        if(isAdd)
            fvalue += ef->value / 100;
        else
            fvalue -= ef->valueExt1 / 100;  // 减掉的值取这个
    }
}

void BattleSimulator::ModifyTherapy_SkillStrengthen(BattleFighter* bf, const GData::SkillBase* skill, float& fvalue, bool isAdd)
{
    if(!skill)
        return;

#ifndef _THERAPY_DEBUG

#ifdef _BATTLE_DEBUG
#define _TMP_THERAPY
#endif // #ifdef _BATTLE_DEBUG

#undef _BATTLE_DEBUG
#endif // #ifndef _THERAPY_DEBUG

#ifdef _BATTLE_DEBUG
    std::cout << "skillId = " << skill->getId() << ":" << std::endl;
    std::cout << "ModifyTherapy_SkillStrengthen factor = " << fvalue << "." << std::endl;
#endif
    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(!ss)
        return;

    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_BUF_THERAPY);
    if(ef)
    {
        if(isAdd)
            fvalue += ef->value / 100;
        else
            fvalue -= ef->valueExt1 / 100;  // 减掉的值取这个
#ifdef _BATTLE_DEBUG
        std::cout << "ModifyTherapy_SkillStrengthen factor(0) = " << fvalue << "." << std::endl << std::endl;
#endif
    }
    ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_BUF_THERAPY);
    if(ef)
    {
        if(isAdd)
            fvalue += ef->value / 100;
        else
            fvalue -= ef->valueExt1 / 100;  // 减掉的值取这个
#ifdef _BATTLE_DEBUG
        std::cout << "ModifyTherapy_SkillStrengthen factor(1) = " << fvalue << "." << std::endl << std::endl;
#endif
    }
    ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_INTENSIFYSTATE);
    if (ef)
    {
        if(isAdd)
            fvalue += ef->value / 100;
        else
            fvalue -= ef->valueExt1 / 100;  // 减掉的值取这个
#ifdef _BATTLE_DEBUG
        std::cout << "ModifyTherapy_SkillStrengthen factor(2) = " << fvalue << "." << std::endl << std::endl;
#endif
    }
    ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_EX_HP_ADD_ADD);
    if (ef)
    {
        if(isAdd)
            fvalue += ef->value / 100;
        else
            fvalue -= ef->valueExt1 / 100;  // 减掉的值取这个
#ifdef _BATTLE_DEBUG
        std::cout << "ModifyTherapy_SkillStrengthen factor(3) = " << fvalue << "." << std::endl << std::endl;
#endif
    }
#ifdef _BATTLE_DEBUG
        std::cout << "ModifyTherapy_SkillStrengthen factor(final) = " << fvalue << "." << std::endl << std::endl;
#endif

#ifndef _THERAPY_DEBUG
#ifdef _TMP_THERAPY
#define _BATTLE_DEBUG
#endif //#ifdef _TMP_THERAPY
#endif //#ifndef _THERAPY_DEBUG
}

bool BattleSimulator::doSkillStrengthenAttack(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !ef || !skill)
        return false;
    if((this->skillStrengthenTable[ef->type]) == NULL)
        return false;

    GData::Area* area = NULL;
    area = &(GData::areaList[ef->area]);

    AttackPoint ap[25];
    int apcnt = 0;

    if(ef->area != 0 && ef->area != 1 && area->getCount() > 0)
    {
        int x_ = target_pos % 5;
        int y_ = target_pos / 5;
        int cnt = area->getCount();
        int fsize = ef->factor.size();
        for(int i = 1; i < cnt; ++ i)
        {
            GData::Area::Data& ad = (*area)[i];
            int x = x_ + ad.x;
            int y = y_ + ad.y;
            if(x < 0 || x > 4 || y < 0 || y > 4)
            {
                continue;
            }
            ap[apcnt].pos = x + y * 5;
            int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
            if(fsize != 0)
                ap[apcnt ++].factor = ef->factor[idx];
            else
                ap[apcnt ++].factor = 1;
        }
    }

    if (ef->cond == GData::ON_SKILLUSED)  // 技能使用后对自己使用的符文加强
    {
        (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, target_pos, active);
    }
    else if(1 == skill->area)
    {
        for(UInt8 pos = 0; pos < 25; ++ pos)
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, pos, active);
        }
    }
    else if( 0 == skill->area )
    {
        (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, target_pos, active);
    }
    else
    {
        BattleFighter* bo = static_cast<BattleFighter *>(getObject(target_side, target_pos));
        if(bo != NULL && bo->getHP() != 0 && bo->isChar())
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, target_pos, active);
        }

        for(int i = 0; i < apcnt; ++ i)
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, ap[i].pos, active);
        }

    }

    return true;
}

bool BattleSimulator::doDeBufAttack(BattleFighter* bf)
{
    UInt32 rcnt = 0;
    if(NULL == bf)
        return 0;

    do
    {
        bf->setPreAtk();
        doLingshiModelAttack(bf, 5);
        size_t skillIdx = 0;
        const GData::SkillBase* violentSKill;
        while(NULL != (violentSKill = bf->getPassiveSkillViolent100(skillIdx)))
        {
            UInt16 actCnt = bf->getActCnt();
            bool bRet = doEffectAfterCount(bf, violentSKill, actCnt);
            if(bRet)
                break;
            UInt16 condtion = 15;
            if(violentSKill->effect)
                condtion = violentSKill->effect->efv[0];
            if(actCnt >= condtion && actCnt < condtion + 40)
            {
                UInt32 hpValue = 400000;
                UInt32 hpr = bf->regenHP(hpValue);
                appendDefStatus(e_damHpAdd, hpr, bf);
            }
            if(actCnt == condtion)
            {
                appendDefStatus(e_skill, violentSKill->getId(), bf);
                Int32 atkValue = 400000;
                setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, violentSKill->getId(), e_stAtk, atkValue, violentSKill->last, bf->getSide() != 0);
                setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, violentSKill->getId(), e_stMagAtk, atkValue, violentSKill->last, bf->getSide() != 0);
                bf->setImmune3(GData::e_state_c_s_f);
            }
            else if(actCnt == condtion + 40)
            {
                Int32 atkValue = 0;
                setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, violentSKill->getId(), e_stAtk, atkValue, violentSKill->last, bf->getSide() != 0);
                setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, violentSKill->getId(), e_stMagAtk, atkValue, violentSKill->last, bf->getSide() != 0);
                bf->setImmune3(0);
            }
            break;
        }

        UInt8 yehuoLeve = bf->getYehuoLevel();
        if(yehuoLeve > 0)
        {
            float yehuoSSDmgRate = bf->getYehuoSSDmgRate();
            UInt32 dmg = static_cast<UInt32>(yehuoSSDmgRate * yehuoLeve);
            calcBleedTypeCnt(bf);
            makeDamage(bf, dmg, e_damNormal, e_damageTrue);
            if(yehuoLeve < 9 && static_cast<float>(uRand(10000)) < bf->getYehuoSSUpRate() * 10000)
            {
                yehuoLeve += 1;
                bf->setYehuoLevel(yehuoLeve);
                appendDefStatus(e_yehuoBleedMo, yehuoLeve, bf);
            }
        }
        if(bf->getHP() == 0)
            break;

        /////////////////////////////////////////////////////////
        // 地裂效果造成的流血状态
        UInt8& lastFieldGape = bf->getBleedFieldGapeLast();
        if(lastFieldGape != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setBleedFieldGape(0, 0, 0);
                appendDefStatus(e_unBleedFieldGape, 0, bf, e_damageTrue);
            }
            else
            {
                if (!bf->getStunRound())
                {
                    if(static_cast<float>(uRand(10000) < bf->getBleedFieldGapeStunProb() * 100))
                    {
                        bf->setStunRound(1);
                        calcAbnormalTypeCnt(bf);
                        appendDefStatus(e_Stun, 0, bf);
                    }
                }
                UInt32 dmg = static_cast<UInt32>(bf->getBleedFieldGape()) * factor;
                calcBleedTypeCnt(bf);
                -- lastFieldGape;
                switch(bf->getBleedFieldGapeType())
                {
                    case 0:
                        if(lastFieldGape == 0)
                            makeDamage(bf, dmg, e_unBleedFieldGape, e_damageTrue);
                        else
                            makeDamage(bf, dmg, e_bleedFieldGape, e_damageTrue);
                        break;
                    case 1:
                        if(lastFieldGape == 0)
                            makeDamage(bf, dmg, e_unBleedFieldGape2, e_damageTrue);
                        else
                            makeDamage(bf, dmg, e_bleedFieldGape2, e_damageTrue);
                        break;
                    case 2:
                        if(lastFieldGape == 0)
                            makeDamage(bf, dmg, e_unBleedFieldGape3, e_damageTrue);
                        else
                            makeDamage(bf, dmg, e_bleedFieldGape3, e_damageTrue);
                        break;
                    default:
                        break;
                }
                if(lastFieldGape == 0)
                    bf->setBleedFieldGape(0, 0, 0);
            }
        }
        if(bf->getHP() == 0)
            break;
        // 地裂效果造成的流血状态
        /////////////////////////////////////////////////////////

        float darkVigor = bf->getDarkVigor();
        if(bf->releaseDarkVigor())
        {
            appendDefStatus(e_unDarkVigor, 0, bf);
            doDarkVigorAttack(bf, darkVigor);
        }
        if(bf->getHP() == 0)
            break;

        UInt32 lsBleed = static_cast<UInt32>(bf->getLingShiBleed());
        if(lsBleed > 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setLingShiBleed(0, 0);
                appendDefStatus(e_unLingShiBleed, 0, bf);
            }
            else
            {
                UInt32 dmg = lsBleed * factor;
                calcBleedTypeCnt(bf);
                if(bf->releaseLingShiBleed())
                    makeDamage(bf, dmg, e_unLingShiBleed, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_lingShiBleed, e_damageTrue);
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt32 bleedMo = static_cast<UInt32>(bf->getBleedMo());
        if(bleedMo > 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->resetBleedMo();
                appendDefStatus(e_unBleedMo, 0, bf);
            }
            else
            {
                UInt32 dmg = bleedMo * factor;
                calcBleedTypeCnt(bf);
                if(bf->releaseBleedMo())
                    makeDamage(bf, dmg, e_unBleedMo, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_BleedMo, e_damageTrue);
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt8& last1 = bf->getBleed1Last();
        if(last1 != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setBleed1(0, 0);
                appendDefStatus(e_unBleed1, 0, bf);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getBleed1()) * factor;
                calcBleedTypeCnt(bf);
                -- last1;
                if(last1 == 0)
                    makeDamage(bf, dmg, e_unBleed1, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_Bleed1, e_damageTrue);

                if(last1 == 0)
                    bf->setBleed1(0, 0);
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt8& last2 = bf->getBleed2Last();
        if(last2 != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setBleed2(0, 0);
                appendDefStatus(e_unBleed2, 0, bf, e_damageTrue);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getBleed2()) * factor;
                calcBleedTypeCnt(bf);
                -- last2;
                if(last2 == 0)
                    makeDamage(bf, dmg, e_unBleed2, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_Bleed2, e_damageTrue);
                if(last2 == 0)
                    bf->setBleed2(0, 0);
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt8& last3 = bf->getBleed3Last();
        if(last3 != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setBleed3(0, 0);
                appendDefStatus(e_unBleed3, 0, bf, e_damageTrue);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getBleed3()) * factor;
                calcBleedTypeCnt(bf);
                -- last3;
                if(last3 == 0)
                    makeDamage(bf, dmg, e_unBleed3, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_Bleed3, e_damageTrue);
                if(last3 == 0)
                    bf->setBleed3(0, 0);
            }
        }
        if(bf->getHP() == 0)
            break;

        // 自己点燃自己百分比流血
        UInt8& selflast = bf->getSelfBleedLast();
        if(selflast != 0)
        {
            UInt32 dmg = static_cast<UInt32>(bf->getMaxHP()) * 0.01;
            calcBleedTypeCnt(bf);
            -- selflast;
            if(selflast == 0)
                makeDamage(bf, dmg, e_unSelfBleed, e_damageTrue);
            else
                makeDamage(bf, dmg, e_selfBleed, e_damageTrue);
            if(selflast == 0)
                bf->setSelfBleed(0, 0);
        }

        UInt8& lingYanLast = bf->getBleedLingYanLast();
        if (lingYanLast != 0)
        {
            UInt32 dmg = static_cast<UInt32>(bf->getBleedLingYan());
            calcBleedTypeCnt(bf);
            -- lingYanLast;
            if(lingYanLast == 0)
                makeDamage(bf, dmg, e_unLingYan, e_damageTrue);
            else
                makeDamage(bf, dmg, e_lingYan, e_damageTrue);
            UInt32 prob = static_cast<UInt32>(bf->getBleedLingYanAuraDescProb() * 100);
            float aura = bf->getBleedLingYanAuraDesc();
            if (uRand(10000) < prob)
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, NULL, e_stAura, -aura, 0, false);
            if(lingYanLast == 0)
                bf->setBleedLingYan(0, 0, 0, 0);
        }


        UInt8& ablast = bf->getAuraBleedLast();
        if(ablast != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setAuraBleed(0, 0, 0);
                bf->setAuraPresent(0, 0);
                appendDefStatus(e_unBleed1, 0, bf);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getAuraBleed()) * factor;
                calcBleedTypeCnt(bf);
                -- ablast;
                if(ablast == 0)
                    makeDamage(bf, dmg, e_unBleed1, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_Bleed1, e_damageTrue);
                if(ablast == 0)
                {
                    bf->setAuraBleed(0, 0, 0);
                    bf->setAuraPresent(0, 0);
                }
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt8& cblast = bf->getConfuceBleedLast();
        if(cblast != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setConfuceBleed(0, 0, 0);
                bf->setConfusePresent(0, 0);
                appendDefStatus(e_unBleed4, 0, bf);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getConfuceBleed()) * factor;
                calcBleedTypeCnt(bf);
                -- cblast;
                if(cblast == 0)
                    makeDamage(bf, dmg, e_unBleed4, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_Bleed4, e_damageTrue);
                if(cblast == 0)
                {
                    bf->setConfuceBleed(0, 0, 0);
                    bf->setConfusePresent(0, 0);
                }
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt8& sblast = bf->getStunBleedLast();
        if(sblast != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setStunBleed(0, 0, 0);
                bf->setStunPresent(0, 0);
                appendDefStatus(e_unBleed3, 0, bf);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getStunBleed()) * factor;
                calcBleedTypeCnt(bf);
                -- sblast;
                if(sblast == 0)
                    makeDamage(bf, dmg, e_unBleed3, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_Bleed3, e_damageTrue);

                if(sblast == 0)
                {
                    bf->setStunBleed(0, 0, 0);
                    bf->setStunPresent(0, 0);
                }
            }
        }
        if(bf->getHP() == 0)
            break;

        UInt8& bblast = bf->getBlindBleedLast();
        if(bblast != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            if(bleedout)
            {
                bf->setBlindBleed(0, 0, 0);
                bf->setBlindPresent(0, 0);
                appendDefStatus(e_unBleedMo, 0, bf);
            }
            else
            {
                UInt32 dmg = static_cast<UInt32>(bf->getBlindBleed()) * factor;
                calcBleedTypeCnt(bf);
                -- bblast;
                if(bblast == 0)
                    makeDamage(bf, dmg, e_unBleedMo, e_damageTrue);
                else
                    makeDamage(bf, dmg, e_BleedMo, e_damageTrue);

                if(bblast == 0)
                {
                    bf->setBlindBleed(0, 0, 0);
                    bf->setBlindPresent(0, 0);
                }
            }
        }
        if(bf->getHP() == 0)
            break;

        Int16& nrandomlast = bf->getBleedRandomLast();
        if(nrandomlast != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            UInt8 nClass = bf->getBleedAttackClass();
            StateType eType = e_Bleed1;
            StateType eUnType = e_unBleed1;
            switch(nClass)
            {
            case e_cls_ru:
                eType = e_Bleed1;
                eUnType = e_unBleed1;
                break;
            case e_cls_shi:
                eType = e_Bleed2;
                eUnType = e_unBleed2;
                break;
            case e_cls_dao:
                eType = e_Bleed3;
                eUnType = e_unBleed3;
                break;
            case e_cls_mo:
                eType = e_BleedMo;
                eUnType = e_unBleedMo;
                break;
            }

            if(bleedout)
            {
                bf->setBleedRandom(0, 0);
                appendDefStatus(eUnType, 0, bf);
            }
            else
            {
                UInt32 dmg = bf->getBleedRandom() * factor;
                dmg *= static_cast<float>(950 + _rnd(100))/1000;
                calcBleedTypeCnt(bf);
                -- nrandomlast;
                if(nrandomlast == 0)
                {
                    bf->setBleedRandom(0, 0);
                    makeDamage(bf, dmg, eUnType, e_damageTrue);
                }
                else
                {
                    makeDamage(bf, dmg, eType, e_damageTrue);
                }
            }
        }
        if(bf->getHP() == 0)
            break;

        // 取中毒伤害来流血的debuf。。。
        Int16& nbySkilllast = bf->getBleedBySkillLast();
        if(nbySkilllast != 0)
        {
            GData::LBSkillItem* item = bf->getBleedCondItem();
            float factor = 1 - getItemXin_BleedDec(bf, item);
            bool bleedout = getItemXin_BleedOut(bf, item);
            UInt8 nClass = bf->getBleedBySkillClass();
            StateType eType = e_Bleed1;
            StateType eUnType = e_unBleed1;
            if(nClass == 2)
            {
                eType = e_Bleed2;
                eUnType = e_unBleed2;
            }
            else if(nClass == 3)
            {
                eType = e_Bleed3;
                eUnType = e_unBleed3;
            }

            if(bleedout)
            {
                bf->setBleedBySkill(0, 0);
                appendDefStatus(eUnType, 0, bf);
            }
            else
            {
                UInt32 dmg = bf->getBleedBySkill() * factor;
                dmg *= static_cast<float>(950 + _rnd(100))/1000;
                calcBleedTypeCnt(bf);
                -- nbySkilllast;

                if(nbySkilllast == 0)
                {
                    bf->setBleedBySkill(0, 0);
                    makeDamage(bf, dmg, eUnType, e_damageTrue);
                }
                else
                {
                    makeDamage(bf, dmg, eType, e_damageTrue);
                }
            }
        }

        if(bf->getHP() == 0)
            break;
        const GData::SkillBase *skill = bf->getXiangMoChanZhangSkill();
        if(skill != NULL && bf->getHpShieldSelf() > 0.001f)
        {
            bf->setHpShieldSelf(0, 0);
            appendDefStatus(e_unHpShieldSelf, 0, bf);

            doSkillAttackByCareer(bf, skill);
        }

        if(bf->getHP() == 0)
            break;
        for(UInt8 i = 0; i < 25; i++)
        {
            UInt8 peerlessDisableSSLast = bf->getPeerLessDisableSSLast(i);
            if(peerlessDisableSSLast > 0)
            {
                --peerlessDisableSSLast;
                bf->setPeerLessDisableSSLast(i, peerlessDisableSSLast);
                UInt32 addHP = bf->getPeerLessDisableSSHP(i);
                UInt32 hpr = bf->regenHP(addHP);
                if(hpr > 0)
                {
                    appendDefStatus(e_damHpAdd, hpr, bf);
                    onHPChanged(bf);
                }
            }
        }

        if(bf->getHP() == 0)
            break;
        UInt8 side = bf->getSide();
        BattleFighter* bo;
        UInt8 ruRedCarpetLast = bf->getRuRedCarpet();
        UInt8 shiFlowerLast = bf->getShiFlower();
        UInt8 daoRoselast = bf->getDaoRose();
        UInt8 moKnotLast = bf->getMoKnot();
        if(ruRedCarpetLast > 0 || shiFlowerLast > 0 || daoRoselast > 0 || moKnotLast > 0)
        {
            for(UInt8 pos = 0; pos < 25; pos++)
            {
                bo = static_cast<BattleFighter*>(getObject(side, pos));
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;
                if(bo == bf)
                    continue;

                if(bo->getRuRedCarpet() > 0 || bo->getShiFlower() > 0 || bo->getDaoRose() > 0 || bo->getMoKnot() > 0)
                {
                    if(static_cast<float>(uRand(10000) < bf->getMagRes(bo) * 100))
                        appendDefStatus(e_Res, 0, bf);
                    else
                    {
                        if(ruRedCarpetLast > 0)
                        {
                            bf->setConfuseRound(1);
                            appendDefStatus(e_Confuse, 0, bf);
                            calcAbnormalTypeCnt(bf);
                            calcSilkwormCnt(bf);
                        }
                        if(shiFlowerLast > 0)
                        {
                            float nChangeAuraNum = bf->getShiFlowerAura() * (-1);
                            setStatusChange_Aura2(bf, bf->getSide(), bf->getPos(), NULL, nChangeAuraNum, 0, false);
                        }
                        if(daoRoselast > 0)
                        {
                            bf->setStunRound(1);
                            appendDefStatus(e_Stun, 0, bf);
                            calcAbnormalTypeCnt(bf);
                            calcSilkwormCnt(bf);
                        }
                        if(moKnotLast > 0)
                        {
                            bf->setBlind(0.75f, 1);
                            appendDefStatus(e_blind, 0, bf);
                            calcAbnormalTypeCnt(bf);
                        }
                    }
                    break;
                }
            }

            if(ruRedCarpetLast > 0)
            {
                appendDefStatus(e_unRuRedCarpet, 0, bf);
                bf->setRuRedCarpet(0);
            }
            if(shiFlowerLast > 0)
            {
                appendDefStatus(e_unShiFlower, 0, bf);
                bf->setShiFlower(0, 0);
            }
            if(daoRoselast > 0)
            {
                appendDefStatus(e_unDaoRose, 0, bf);
                bf->setDaoRose(0);
            }
            if(moKnotLast > 0)
            {
                appendDefStatus(e_unMoKnot, 0, bf);
                bf->setMoKnot(0);
            }
        }

    }while(false);

    if(_defList.size() > 0 || _scList.size() > 0)
    {
        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 5, 0, false, false);
        ++ rcnt;
    }

    return rcnt;
}

float BattleSimulator::calcTherapyFactor(BattleFighter* bo)
{
    float factor = 0.0f;
    //factor = 1.0f - calcTherapyDebuf(bo, defList, defCount) + calcTherapyAddBuff(bo, defList, defCount);
    factor = 1.0f + calcTherapyBuff(bo);
    if(factor < 0)
        factor = 0;

    return factor;
}

float BattleSimulator::calcTherapyBuff(BattleFighter* bo)
{
    float factor = 0.0f;
    UInt8& last = bo->getTherapyBuffLast();
    if(last > 0)
    {
        factor = bo->getTherapyBuff();
        // 移到releaseWeak函数处理
//         -- last;
//         if(last == 0)
//             bo->setTherapyBuff(0, 0);
//         if (factor > 0)  // 增益效果
//         {
//             defList[defCount].damType = e_unTherapyBuff;
//         }
//         else
//         {
//             defList[defCount].damType = e_UnWeak;
//         }
//         defList[defCount].damage = 0;
//         defList[defCount].pos = bo->getPos() + 25;
//         defList[defCount].leftHP = bo->getHP();
//         defCount ++;
    }
 
    return factor;
}

//float BattleSimulator::calcTherapyDebuf(BattleFighter* bo, DefStatus* defList, size_t& defCount)
//{
//    float factor = 0.0f;
//    UInt8& last = bo->getTherapyDecLast();
//    if(last > 0)
//    {
//        //factor *= (1.0f - bo->getTherapyDec());
//        factor = bo->getTherapyDec();
//        -- last;
//        if(last == 0)
//            bo->setTherapyDec(0, 0);
//        defList[defCount].damType = e_unneishan;
//        defList[defCount].damage = 0;
//        defList[defCount].pos = bo->getPos() + 25;
//        defList[defCount].leftHP = bo->getHP();
//        defCount ++;
//    }
//
//    return factor;
//}
//
//float BattleSimulator::calcTherapyAddBuff(BattleFighter* bo, DefStatus* defList, size_t& defCount)
//{
//    float factor = 0.0f;
//    UInt8& last = bo->getTherapyAddLast();
//    if(last > 0)
//    {
//        //factor *= (1.0f + bo->getTherapyAdd());
//        factor = bo->getTherapyAdd();
//        -- last;
//        if(last == 0)
//            bo->setTherapyAdd(0, 0);
//        defList[defCount].damType = e_unTherapyBuff;
//        defList[defCount].damage = 0;
//        defList[defCount].pos = bo->getPos() + 25;
//        defList[defCount].leftHP = bo->getHP();
//        defCount ++;
//    }
//
//    return factor;
//}

float BattleSimulator::calcAuraDebuf( BattleFighter* bo )
{
    float factor = bo->getAuraDec();
    UInt8& last = bo->getAuraDecLast();
    if(last > 0)
    {
        -- last;
        if(last == 0)
            bo->setAuraDec(0, 0);
        // 策划说灵力减益buf不做了，不用通知前端
        //         defList[defCount].damType = e_unAuraDeBuff;
        //         defList[defCount].damage = 0;
        //         defList[defCount].pos = bo->getPos() + 25;
        //         defList[defCount].leftHP = bo->getHP();
        //         defCount ++;
    }

    return factor;
}

UInt32 BattleSimulator::doSkillStrenghtenCriticalPierceDmgB(BattleFighter* bf, const GData::SkillBase* skill, bool cs, bool pr, GData::SkillStrengthenBase*  ss, int target_side, int target_pos)
{
    UInt32 dmg = 0;
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
    {
        if(cs)
            ef = ss->getEffect(GData::ON_CRITICAL, GData::TYPE_DAMAG_B);
        if(!ef && pr)
            ef = ss->getEffect(GData::ON_PIERCE, GData::TYPE_DAMAG_B);
    }
    if(ef)
    {
        int pos2 = -1;
        for(int pos = target_pos + 5; pos < 25; pos += 5)
        {
            if(getObject(target_side, pos) != NULL && getObject(target_side, pos)->getHP() != 0)
            {
                pos2 = pos;
                break;
            }
        }

        bool first = false;
        if(pos2 != -1)
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, pos2), ef->value/100);
    }

    return dmg;
}

UInt32 BattleSimulator::getSkillEffectExtraHitCnt(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill)
{
    if(!bo || !skill || !skill->effect)
        return 0;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return 0;

    UInt32 hitCnt = 0;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_double_hit)
            hitCnt += efv[i];
        else if(eft[i] == GData::e_eft_mark_hide_dhit && (bf->isHide() || bo->isMarkMo()))
        {
            hitCnt += efv[i];

            if(bo->releaseMarkMo())
            {
                appendDefStatus(e_unMarkMo, 0, bo);
            }
        }
        /*
        else if (eft[i] == GData::e_eft_atk_pet_mark_extra_dmg && bo->isPetMark())
            hitCnt += efv[i];
            */
    }

    return hitCnt;
}

bool BattleSimulator::doSkillStrengthenDeepBlind(BattleFighter* bf, BattleFighter* bo, GData::SkillStrengthenBase* ss)
{
    bool ret = false;
    if(ss && bf && bo && bf->getHP() > 0 && bo->getHP() > 0)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_BLIND, GData::TYPE_DAMAG_A);
        if(ef)
        {
            bo->setDeepBlind(ef->value/100, ef->last);
            appendDefStatus(e_deepBlind, 0, bo);
            calcAbnormalTypeCnt(bo);
        }
    }

    if(bo->getDeepBlindDmgExtra() > 0.001f)
        ret = true;

    return ret;
}

void BattleSimulator::doSkillEffectExtraAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    const std::vector<float>& efv = skill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efl.size() || cnt != efv.size())
        return;

    for(size_t i = 0; i < cnt; ++ i)
    {
        if((this->skillEffectExtraTable[eft[i]]) == NULL)
            continue;
        (this->*skillEffectExtraTable[eft[i]])(bf, target_side, target_pos, skill, i);
    }
}

void BattleSimulator::doSkillEffectExtra_Hide(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    UInt16 last = skill->effect->efl[eftIdx] + (bf == _activeFgt ? 1 : 0);
    bf->setHideBuf(true, last);
    appendDefStatus(e_hide, 0, bf);
}

void BattleSimulator::doSkillEffectExtra_RndFgtBufAura(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    UInt8 myPos = bf->getPos();
    BattleFighter* bo = getRandomFighter(bf->getSide(), &myPos, 1);
    if(!bo)
        bo = bf;

    UInt16 last = skill->effect->efl[eftIdx];
    bo->setMoAuraBuf(skill->effect->efv[eftIdx], last);
    appendDefStatus(e_moAuraBuf, 0, bo);
}

void BattleSimulator::doSkillEffectExtra_Evade100(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    UInt16 last = skill->effect->efl[eftIdx] + (bf == _activeFgt ? 1 : 0);
    bf->setMoEvade100(skill->effect->efv[eftIdx], last);
}

void BattleSimulator::doSkillEffectExtra_HideSummon(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    UInt8 pos = bf->getPos() % 5;
    int side = bf->getSide();
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
    if(bo && bo->getHP() != 0)
    {
        bo->setHP(0);
        removeFighterStatus(bo);
    }

    BattleFighter * newf = bf->summonSelf(skill->effect->efv[eftIdx], skill->effect->efl[eftIdx]);
    if(newf == NULL)
        return;
    newf->setSideAndPos(side, pos);
    setObject(side, pos, newf);
    insertFighterStatus2Current(newf);

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
        ef = ss->getEffect(GData::ON_SUMMON, GData::TYPE_UNSUMMON_AURA);
    if(ef)
    {
        newf->setUnSummonAura(bf, ef->value);
    }

    {
        appendDefStatus(e_Summon, newf->getPortrait(), newf);
        UInt32 value = static_cast<UInt32>(newf->getAura());
        appendStatusChange(e_stAura, value, 0, newf);
        appendInitDefStatus(newf);
    }
}

void BattleSimulator::doSkillEffectExtra_MarkHideWeek(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(!bo)
        return;

    if(!bf->isHide() && !bo->isMarkMo())
        return;

    UInt16 last = skill->effect->efl[eftIdx] + (bo == _activeFgt ? 1 : 0);
    float value = skill->effect->efv[eftIdx];
    if(value > 0 && bo->getSide() != bf->getSide())
        value *= -1;
    bo->setTherapyBuff(value, last);
    appendDefStatus(e_Weak, 0, bo);

    if(bo->releaseMarkMo())
    {
        appendDefStatus(e_unMarkMo, 0, bo);
    }
}

void BattleSimulator::doSkillEffectExtra_HideAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf->isHide())
        return;

    UInt16 last = skill->effect->efl[eftIdx] + (bf == _activeFgt ? 1 : 0);
    float atkadd = bf->_attack * skill->effect->efv[eftIdx];
    float magatkadd = bf->_magatk * skill->effect->efv[eftIdx];
    bf->setMoAttackAdd(atkadd, last);
    bf->setMoMagAtkAdd(magatkadd, last);
    {
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf->getAttack());
        appendStatusChange(e_stAtk, value, skillId, bf);
    }
    {
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf->getMagAttack());
        appendStatusChange(e_stMagAtk, value, skillId, bf);
    }
}

void BattleSimulator::doSkillEffectExtra_SelfSideDaoDmgReduce(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    int side = bf->getSide();
    for(int i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(!bo || bo->getHP() == 0 || bo->getClass() != e_cls_dao || bo == bf)
            continue;

        UInt16 last = skill->effect->efl[eftIdx] + (bo == _activeFgt ? 1 : 0);
        float atkreduce = skill->effect->efv[eftIdx];
        float magatkreduce = skill->effect->efv[eftIdx];
        bo->setMoAtkReduce(atkreduce, last);
        bo->setMoMagAtkReduce(magatkreduce, last);
        {
            UInt16 skillId = skill != NULL ? skill->getId() : 0;
            UInt32 value = static_cast<UInt32>(bo->getAtkReduce()*100);
            appendStatusChange(e_stAtkReduce, value, skillId, bo);
        }
        {
            UInt16 skillId = skill != NULL ? skill->getId() : 0;
            UInt32 value = static_cast<UInt32>(bo->getMagAtkReduce()*100);
            appendStatusChange(e_stMagAtkReduce, value, skillId, bo);
        }
    }
}

void BattleSimulator::doSkillEffectExtra_SelfSideRuShiMagAtk(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    int side = bf->getSide();
    for(int i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(!bo || bo->getHP() == 0 || (bo->getClass() != e_cls_ru && bo->getClass() != e_cls_shi) || bo == bf)
            continue;

        UInt16 last = skill->effect->efl[eftIdx] + (bo == _activeFgt ? 1 : 0);
        float magatkadd = bo->_magatk * skill->effect->efv[eftIdx];
        bo->setMoMagAtkAdd(magatkadd, last);
        {
            UInt16 skillId = skill != NULL ? skill->getId() : 0;
            UInt32 value = static_cast<UInt32>(bo->getMagAttack());
            appendStatusChange(e_stMagAtk, value, skillId, bo);
        }
    }
}

void BattleSimulator::doSkillEffectExtra_SelfSideBufAura(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    int side = bf->getSide();
    for(int i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(!bo || bo->getHP() == 0)
            continue;
        UInt16 last = skill->effect->efl[eftIdx];
        bo->setMoAuraBuf(skill->effect->efv[eftIdx], last);
        appendDefStatus(e_moAuraBuf1, 0, bo);
    }
}

void BattleSimulator::doSkillEffectExtra_HpShield(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 增加hp百分比的护盾
    BattleFighter* bo;
    if(skill && SKILL_ID(skill->getId()) == 50)
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 0;
        bo = getTherapyTarget3(bf, excepts, exceptCnt);
    }
    else
        bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(!bo)
        return;
    if (!bo->isChar())
        return;
    float hp = bf->getMaxHP() * (skill->effect->efv[eftIdx]);
    if (hp < 1.0f)
        return;

    float factor = 1.0f;
    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(ss)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_SHIELD_HP);
        if(ef)
        {
            factor += ef->value / 100;
        }
        ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_DEF_CHANGE);
        if(ef)
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, target_pos, false);
        }
    }
    hp *= factor;
    bo->addHpShieldSelf(hp, skill->effect->efl[eftIdx]);
    appendDefStatus(e_hpShieldSelf, hp, bo);
}

void BattleSimulator::doSkillEffectExtra_SelfBleed(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 自己流血（自焚）
    //BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    BattleFighter* bo = bf;
    if(!bo)
        return;
    // 自焚的具体数值
    bo->setSelfBleed(skill->effect->efv[eftIdx], skill->effect->efl[eftIdx]);
    appendDefStatus(e_selfBleed, 0, bo);
}

void BattleSimulator::doSkillEffectExtra_RandomShield(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 随机选择上护盾
    //UInt8 myPos = bf->getPos();
    //BattleFighter* bo = getRandomFighter(bf->getSide(), &myPos, 1);
    UInt8 excepts[25] = {0};
    size_t exceptCnt = 0;
    BattleFighter* bo = getTherapyTarget3(bf, excepts, exceptCnt);
    if(!bo)
        return;
    if (!bo->isChar())
        return;

    float ssfactor = 0.0f;

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    if(ss)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_SHIELD_HP);
        if(ef)
        {
            ssfactor += ef->value / 100;
        }
    }

    float factor = 1 + ssfactor;

    float hp = bf->getMaxHP() * (skill->effect->efv[eftIdx]) * factor;
    if (hp < 1.0f)
        return;

    bo->addHpShieldSelf(hp, skill->effect->efl[eftIdx]);
    appendDefStatus(e_hpShieldSelf, hp, bo);
}

void BattleSimulator::doSkillEffectExtra_SelfAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 附加自己百分比的攻击力到指定人身上
    UInt16 last = skill->effect->efl[eftIdx];
    float atkadd = (bf->_magatk) * (skill->effect->efv[eftIdx]);
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo->isChar())
        return;

    float ssfactor = 0.0f;
    ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);
    float factor = 1 + ssfactor;

    atkadd = atkadd * factor;

    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);
    //if (bf2->getClass() == e_cls_dao || bf2->getClass() == e_cls_mo)
    {
        bf2->setPetAttackAdd(atkadd, last);
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf2->getAttack());
        appendStatusChange(e_stAtk, value, skillId, bf2);
    }
    //else
    {
        bf2->setPetMagAtkAdd(atkadd, last);
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf2->getMagAttack());
        appendStatusChange(e_stMagAtk, value, skillId, bf2);
    }
}

void BattleSimulator::doSkillEffectExtra_RandomTargetAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 随机选择增加攻击力
    UInt8 myPos = bf->getPos();
    BattleFighter* bo = getRandomFighter(bf->getSide(), &myPos, 1);
    if(!bo)
        return;
    if (!bo->isChar())
        return;
    float ssfactor = 0.0f;
    ModifyAttackValue_SkillStrengthen_Other(bf, skill, ssfactor, true);
    float factor = 1 + ssfactor;
    setStatusChange_Atk(bf, bo->getSide(), bo->getPos(), skill, bo->_attack * skill->effect->efv[eftIdx] * factor, skill->effect->efl[eftIdx], true);
    setStatusChange_MagAtk(bf, bo->getSide(), bo->getPos(), skill, bo->_magatk * skill->effect->efv[eftIdx] * factor, skill->effect->efl[eftIdx], true);
}

void BattleSimulator::doSkillEffectExtra_MarkPet(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 上神兽印记
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo || !bo->isChar() || !bo->getHP())
        return;
    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);
    bf2->setPetMark(true);
    appendDefStatus(e_petMark, 0, bf2);
}

void BattleSimulator::doSkillEffectExtra_AtkPetMarkAura(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 如果神兽印记恢复自己灵气
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo->isChar())
        return;
    BattleFighter * bf2 = static_cast<BattleFighter *>(bo);
    if (bf->isPet() && bf2->isPetMark())
    {
        bf->AddAura(skill->effect->efv[eftIdx]);
        UInt32 value2 = static_cast<UInt32>(bf->getAura());
        appendStatusChange(e_stAura, value2, 0, bf);
    }
}

void BattleSimulator::doSkillEffectExtra_AtkPetMarkDmg(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 造成额外伤害
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo->isChar())
        return;
    BattleFighter * area_target = static_cast<BattleFighter *>(bo);

    bool first = false;
    bool cs    = false;
    bool pr    = false;

    float ssfactor = 0.0f;
    ModifyAttackValue_SkillStrengthen(bf, skill, ssfactor, true);
    float value = skill->effect->efv[eftIdx] * (1 + ssfactor);
    attackOnce(bf, first, cs, pr, NULL, area_target, value, 1);

}

void BattleSimulator::doSkillEffectExtra_ProtectPet100(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 为目标单位100%发动援护
    BattleObject* bo;
    if(skill && SKILL_ID(skill->getId()) == 51)
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 0;
        bo = getTherapyTarget3(bf, excepts, exceptCnt);
    }
    else
        bo = getObject(target_side, target_pos);
    if (!bo || !bo->isChar())
        return;
    BattleFighter * area_target = static_cast<BattleFighter *>(bo);
    area_target->setPetProtect100(true, static_cast<UInt8>(0), skill);
    appendDefStatus(e_petProtect100, 0, area_target);
}

void BattleSimulator::doSkillEffectExtra_PetAtk100(BattleFighter* bf,  int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 标记为仙宠100与其合击
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo->isChar())
        return;
    BattleFighter * area_target = static_cast<BattleFighter *>(bo);
    area_target->setPetAtk100(skill->effect->efv[eftIdx], skill->effect->efl[eftIdx]);
    appendDefStatus(e_petAtk100, 0, area_target);
}

void BattleSimulator::doSkillEffectExtraAbsorb(BattleFighter* bf, UInt32 dmg, const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return;

    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_selfside_absorb)
        {
            float rhp = (dmg) * efv[i];
            BattleFighter* bo = getTherapyTarget3(bf, _cur_round_except, _except_count);
            if(!bo)
                break;

            float factor = calcTherapyFactor(bo);
            ModifyTherapy_SkillStrengthen(bf, skill, factor, true);
            _cur_round_except[_except_count] = bo->getPos();
            ++ _except_count;
            UInt32 hpr = bo->regenHP(rhp * factor, skill->cond == GData::SKILL_ACTIVE);
            if(hpr != 0)
            {
                appendDefStatus(e_damHpAdd, hpr, bo);
                onHPChanged(bo);
            }
        }
    }
}

void BattleSimulator::doSkillStrenghtenTherapyAnotherMore(BattleFighter* bf, UInt32 dmg, const GData::SkillBase* skill, GData::SkillStrengthenBase*  ss)
{
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
    {
        ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_THERAPY_ANOTHER_MORE);
    }
    if(ef)
    {
        float rhp = (dmg) * ef->value/100;
        BattleFighter* bo = getTherapyTarget3(bf, _cur_round_except, _except_count);
        if(!bo)
            return;

        float factor = calcTherapyFactor(bo);
        _cur_round_except[_except_count] = bo->getPos();
        ++ _except_count;
        UInt32 hpr = bo->regenHP(rhp * factor, skill->cond == GData::SKILL_ACTIVE);
        if(hpr != 0)
        {
            appendDefStatus(e_damHpAdd, hpr, bo);
            onHPChanged(bo);
        }
    }
}

UInt32 BattleSimulator::doSkillStrenghtenCriticalDamageAroundOne(BattleFighter* bf, const GData::SkillBase* skill, bool cs, GData::SkillStrengthenBase*  ss, int target_side, int target_pos)
{
    UInt32 dmg = 0;
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
    {
        if(cs)
            ef = ss->getEffect(GData::ON_CRITICAL, GData::TYPE_DAMAGE_AROUND_ONE);
    }
    if(ef)
    {
        int pos = getNearOnePos(target_side, target_pos);
        bool first = false;
        bool pr = false;
        if(pos != -1)
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, pos), ef->value/100);
    }

    return dmg;
}

void BattleSimulator::getSkillEffectExtraBlind(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill, UInt16& effect_state)
{
    if(!bo || !skill || !skill->effect)
        return;
    const std::vector<UInt16>& eft = skill->effect->eft;

    size_t cnt = eft.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_mark_hide_blind)
        {
            if(!bf->isHide() && !bo->isMarkMo())
                effect_state = effect_state & (0xFFFF ^ GData::e_state_blind);
        }
    }

    return;
}

int BattleSimulator::getNearOnePos(int side, int pos)
{
    int x = pos % 5;
    int y = pos / 5;

    int retpos = -1;
    int minoffset = 10;
    for(int i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(!bo || bo->getHP() == 0 || i == pos)
            continue;
        int xidx = i % 5;
        int yidx = i / 5;
        int offset = abs(x - xidx) + abs(y - yidx);
        if(offset < minoffset)
        {
            minoffset = offset;
            retpos = i;
        }
    }

    return retpos;
}

int BattleSimulator::getSideStartPos(int side)
{
    if(_activeFgt)
        return (side == _activeFgt->getSide() ? 25 : 0);

    return 0;
}

int BattleSimulator::getSidePos(BattleObject* bf)
{
    if(!bf)
        return 0;

    int activeSide = 0;
    int bfPos = bf->getPos();
    int bfSide = bf->getSide();
    if(_activeFgt)
        activeSide = _activeFgt->getSide();

    if(activeSide == bfSide)
        bfPos += 25;

    return bfPos;
}

void BattleSimulator::calcAuraAdd(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill, float& bfAuraAdd, float& boAuraAdd)
{
    if(bf)
    {
        bfAuraAdd = 25+bf->getSoulExtraAura()-calcAuraDebuf(bf);
        if(bfAuraAdd < 0.1f)
            bfAuraAdd = 0;
    }
    if(bo)
    {
        boAuraAdd = 25+bo->getSoulExtraAura()-calcAuraDebuf(bo);
        boAuraAdd += getSkillEffectExtraHideAura(bf, bo, skill);
        if(boAuraAdd < 0.1f)
            boAuraAdd = 0;
    }
}

float BattleSimulator::getSkillEffectExtraHideAura(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill)
{
    if(!bo || !skill || !skill->effect)
        return 0;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return 0;
    float aura = 0;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_hide_aura)
        {
            if(bf->isHide() || bo->isMarkMo())
            {
                if(efv[i] > 0.001f)
                    aura = -efv[i];
                else
                    aura = efv[i];
            }
        }
    }

    return aura;
}

void BattleSimulator::doShieldHPAttack(BattleFighter* bo, UInt32& dmg)
{
    UInt8& colorStockTimes = bo->getColorStockTimes();
    if(colorStockTimes > 0)
    {
        dmg = 0;
        --colorStockTimes;
        return;
    }
    if(bo->isSoulOut())
    {
        dmg = 0;
        return;
    }

    const GData::SkillBase* skill = bo->getBiLanTianYiSkill();
    if(skill && skill->effect && bo->getEvadeCnt() > 2)
    {
        dmg = 0;
        bo->minusEvadeCnt(3);
        UInt8 evadeCnt = bo->getEvadeCnt();
        appendDefStatus(e_skill, skill->getId(), bo);
        if(evadeCnt == 0)
            appendDefStatus(e_unBiLanTianYi, evadeCnt, bo);
        else
            appendDefStatus(e_biLanTianYi, evadeCnt, bo);
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<UInt8>& efl = skill->effect->efl;
        const std::vector<float>& efv = skill->effect->efv;
        size_t cnt = eft.size();
        if(cnt == efl.size() && cnt == efv.size())
        {
            for(size_t i = 0; i < cnt; ++ i)
            {
                if(eft[i] == GData::e_eft_bi_lan_tian_yi)
                {
                    float hp = bo->getMaxHP() * (skill->effect->efv[i]);
                    if (hp < 1.0f)
                        break;
                    bo->addHpShieldSelf(hp, skill->effect->efl[i]);
                    appendDefStatus(e_hpShieldSelf, hp, bo);
                    break;
                }
            }
        }
        return;
    }

    if(bo->makeShieldDamage(dmg))
    {
        appendDefStatus(e_damHPShield, 0, bo);
    }
}

void BattleSimulator::doSkillEffectExtraCounter(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_counter_hate)
        {
            bo->setDarkVigor(efv[i], skill->last);
            appendDefStatus(e_darkVigor, 0, bo);
        }
        else if(eft[i] == GData::e_eft_counter_spirit)
        {
            float atkadd = bf->_attack * skill->effect->atkP + skill->effect->atk;
            float defadd = bf->_defend * skill->effect->defP + skill->effect->def;
            float magatkadd = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
            float magdefadd = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
            bf->addCounterSpiritBuf(atkadd, magatkadd, defadd, magdefadd, skill->last);

            UInt8 times = bf->getCounterSpiritTimes();
            appendDefStatus(e_counterSpirit, times, bf);
            UInt16 skillId = skill->getId();
            UInt32 value = static_cast<UInt32>(bf->getAttack());
            appendStatusChange(e_stAtk, value, skillId, bf);
            value = static_cast<UInt32>(bf->getMagAttack());
            appendStatusChange(e_stMagAtk, value, skillId, bf);
            value = static_cast<UInt32>(bf->getDefend());
            appendStatusChange(e_stDef, value, skillId, bf);
            value = static_cast<UInt32>(bf->getMagDefend());
            appendStatusChange(e_stMagDef, value, skillId, bf);
            //if(times >= 3)
            bf->setCounterSpiritSkill(skill->getId(), efv[i], skill->factor);
        }
    }

    return;
}

bool BattleSimulator::doDarkVigorAttack(BattleFighter* bf, float darkVigor)
{
    bool bfDead = false;
    UInt32 dmg = darkVigor;
    makeDamage(bf, dmg, e_damNormal, e_damageTrue);
 
    struct OffsetPos
    {
        int x;
        int y;
    };
    static OffsetPos offsetPos[4] = { {-1, -1}, {1, -1}, {1, 1}, {-1, 1} };
    static float factor[4] = {0.5, 0.25, 0.125, 0.125};
    int x = bf->getPos()%5;
    int y = bf->getPos()/5;
    int side = bf->getSide();
    for(int i = 0; i < 4; ++ i)
    {
        for(int j = 0; j < 4; ++ j)
        {
            int x2 = (x + offsetPos[j].x * (i + 1));
            int y2 = (y + offsetPos[j].y * (i + 1));
            if(x2 < 0 || y2 < 0 || x2 > 4 || y2 > 4)
                continue;
            int pos =  x2 + y2*5;
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
            if(!bo || bo->getHP() == 0)
                continue;
            UInt32 dmg2 = dmg * factor[i];
            makeDamage(bo, dmg2, e_damNormal, e_damageTrue);
        }
    }

    return bfDead;
}

void BattleSimulator::appendDefStatus(StateType type, UInt32 value, BattleFighter* bf, DamageType damageType /* = e_damageNone */)
{
    if(bf->isSoulOut())
    {
        if(type == e_Bleed1 || type == e_Bleed2 || type == e_Bleed3 || type == e_Bleed4 || type == e_BleedMo || type == e_selfBleed || type == e_lingShiBleed)
            return;
    }

    if(type == e_damEvade)
        addSelfSideEvadeCnt(bf);

    DefStatus defList;
    defList.damType = type;
    defList.damageType = damageType;
    switch(type)
    {
    case e_damAbsorb:
        defList.rhp= value;
        defList.rLeftHP = bf->getHP();
        break;
    default:
        defList.damage = value;
        break;
    }
    defList.leftHP = bf->getHP();
    defList.pos = getSidePos(bf);

    _defList.push_back(defList);
}

void BattleSimulator::appendStatusChange(StatusType type, UInt32 value, UInt16 skillId, BattleFighter* bf)
{
    StatusChange sc;
    sc.pos = getSidePos(bf);
    sc.type = type;
    sc.data = value;
    sc.statusId = skillId;

    _scList.push_back(sc);
    if(type == e_stCounter)
    {
        bf->setCounterCnt(bf->getCounterCnt() + 1);
        doLingshiModelAttack(bf, 2);
    }
    else if(type == e_stCritical)
    {
        bf->setCriticalCnt(bf->getCriticalCnt() + 1);
        doLingshiModelAttack(bf, 4);
    }
}

void BattleSimulator::appendAttackRoundChange()
{
    StatusChange sc;
    sc.pos = 51;
    sc.type = 0;
    sc.data = _attackRound + 1;
    sc.statusId = 0;

    _scList.push_back(sc);
}

void BattleSimulator::appendStatusChangeForReiastu(StatusType type, UInt32 value, UInt16 skillId, UInt8 side)
{
    StatusChange sc;
    int target_pos = getPossibleTarget((side == 0 ? 1 : 0), 0);
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, target_pos));
    if(!bo)
        return;
    appendStatusChange(type, value,0, bo);
}

void BattleSimulator::doItemLingSkillAttack(BattleFighter* bf, BattleFighter* bo)
{
    if(!bf || !bo)
        return;
    GData::LBSkillItem* item = GetActionCondSkillItem1(bf, bo);
    if(!item || !item->base)
        return;

    appendDefStatus(e_ling, 0, bf);

    switch(item->base->ef_type)
    {
    case GData::e_lbef_def_add:
        doItemLing_DefAdd(bf, bo, item->ef_value, item->base->last);
        break;
    case GData::e_lbef_def_dec:
        doItemLing_DefDec(bf, bo, item->ef_value, item->base->last);
        break;
    case GData::e_lbef_atk_add:
        doItemLing_AtkAdd(bf, bo, item->ef_value, item->base->last);
        break;
    case GData::e_lbef_atk_dec:
        doItemLing_AtkDec(bf, bo, item->ef_value, item->base->last);
        break;
    case GData::e_lbef_state:
        doItemLingWu_State(bf, bo, item->ef_value, item->base->last);
        break;
    case GData::e_lbef_aura:
        doItemLing_Aura(bf, bf, item->ef_value, item->base->last);
        break;
    }
}

void BattleSimulator::doItemWuSkillAttack(BattleFighter* bf)
{
    if(!bf)
        return;
    GData::LBSkillItem* item = bf->getDeadCondItem();
    if(!item || !item->base)
        return;

    appendDefStatus(e_wu, 0, bf);

    AtkList atklist;
    getAtkList(bf, item->base, atklist);

    size_t cnt = atklist.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        BattleFighter* bo = atklist[i].bf;
        float factor = atklist[i].factor;
        switch(item->base->ef_type)
        {
        case GData::e_lbef_dmg:
            {
                doItemWu_Dmg(bf, bo, item->ef_value * factor, item->base->last);
            }
            break;
        case GData::e_lbef_state:
            {
                doItemLingWu_State(bf, bo, item->ef_value, item->base->last);
            }
            break;
        case GData::e_lbef_hpshield:
            {
                doItemWu_HPShield(bf, bo, item->ef_value, item->base->last);
            }
            break;
        }
    }
}

void BattleSimulator::doItemLing_DefAdd(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    float value2 = bf->_defend * value/10000;
    setStatusChange_Def(bf, bf->getSide(), bf->getPos(), NULL, value2, last, true);
    value2 = bf->_magdef * value/10000;
    setStatusChange_MagDef(bf, bf->getSide(), bf->getPos(), NULL, value2, last, true);
}

void BattleSimulator::doItemLing_DefDec(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    float value2 = bo->_defend * value/10000 * (-1);
    setStatusChange_Def(bf, bo->getSide(), bo->getPos(), NULL, value2, last, true);
    value2 = bo->_magdef * value/10000 * (-1);
    setStatusChange_MagDef(bf, bo->getSide(), bo->getPos(), NULL, value2, last, true);
}

void BattleSimulator::doItemLing_AtkAdd(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    float value2 = bf->_attack * value/10000;
    setStatusChange_Atk(bf, bf->getSide(), bf->getPos(), NULL, value2, last, true);
    value2 = bf->_magatk * value/10000;
    setStatusChange_MagAtk(bf, bf->getSide(), bf->getPos(), NULL, value2, last, true);
}

void BattleSimulator::doItemLing_AtkDec(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    float value2 = bo->_attack * value/10000 * (-1);
    setStatusChange_Atk(bf, bo->getSide(), bo->getPos(), NULL, value2, last, true);
    value2 = bo->_magatk * value/10000 * (-1);
    setStatusChange_MagAtk(bf, bo->getSide(), bo->getPos(), NULL, value2, last, true);
}

void BattleSimulator::doItemLingWu_State(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    switch(bf->getClass())
    {
    case GObject::e_cls_dao:
        if(!doStateMagRes2(bf, bo, GData::e_state_stun))
        {
            if(bo->getStunRound() < 1)
            {
                if(bo == _activeFgt)
                    bo->setStunRound(last + 1);
                else
                    bo->setStunRound(last);

                appendDefStatus(e_Stun, 0, bo);
                calcAbnormalTypeCnt(bo);
                calcSilkwormCnt(bo);
            }
        }
        break;
    case GObject::e_cls_shi:
        if(!doStateMagRes2(bf, bo, GData::e_state_dec_aura))
        {
            setStatusChange_Aura(bf, bo->getSide(), bo->getPos(), NULL, -100, last, true);
        }
        break;
    case GObject::e_cls_ru:
        if(!doStateMagRes2(bf, bo, GData::e_state_confuse))
        {
            if(bo == _activeFgt)
                bo->setConfuseRound(last + 1);
            else
                bo->setConfuseRound(last);

            appendDefStatus(e_Confuse, 0, bo);
            calcAbnormalTypeCnt(bo);
            calcSilkwormCnt(bo);
        }
        break;
    case GObject::e_cls_mo:
        if(!doStateMagRes2(bf, bo, GData::e_state_blind))
        {
            bo->setBlind(0.75f, last);
            appendDefStatus(e_blind, 0, bo);
            calcAbnormalTypeCnt(bo);
        }
        break;
    }
}

void BattleSimulator::doItemLing_Aura(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    setStatusChange_Aura(bf, bo->getSide(), bo->getPos(), NULL, value, last, true);
}

void BattleSimulator::doItemWu_Dmg(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    float atk = 0;
    float def = 0;
    float reduce = 0;
    bool isPhysic = false;
    if(bf->getClass() == GObject::e_cls_dao || bf->getClass() == GObject::e_cls_mo)
    {
        atk = getBFAttack(bf) * (value/10000);
        def = getBFDefend(bo);
        reduce = getBFAtkReduce(bo);
        isPhysic = true;
    }
    else
    {
        atk = getBFMagAtk(bf) * (value/10000);
        def = getBFMagDefend(bo);
        reduce = getBFMagAtkReduce(bo);
        isPhysic = false;
    }

    UInt32 dmg = _formula->calcDamage(atk, def, bf->getLevel(), 1.0f, reduce);
    makeDamage(bo, dmg, e_damNormal, isPhysic?e_damagePhysic:e_damageMagic);
}

void BattleSimulator::doItemWu_HPShield(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last)
{
    float hp = bf->getMaxHP() * (value / 10000);
    bo->addHpShieldSelf(hp, last);

    appendDefStatus(e_hpShieldSelf, hp, bo);
}

float BattleSimulator::getItemXin_BleedDec(BattleFighter* bf, GData::LBSkillItem* item)
{
    if(!bf)
        return 0;
    if(!item || !item->base)
        return 0;

    if(item->base->ef_type != GData::e_lbef_bleed_dec)
        return 0;

    appendDefStatus(e_xin, 0, bf);

    return (((float)(item->ef_value)) / 10000);
}

bool  BattleSimulator::getItemXin_MagRes(BattleFighter* bf, UInt16 state)
{
    if(!bf)
        return false;
    GData::LBSkillItem* item = bf->getStateCondItem(state);
    if(!item || !item->base)
        return false;

    appendDefStatus(e_xin, 0, bf);

    return true;
}

bool  BattleSimulator::getItemXin_BleedOut(BattleFighter* bf, GData::LBSkillItem* item)
{
    if(!bf)
        return false;
    if(!item || !item->base)
        return false;

    if(item->base->ef_type != GData::e_lbef_bleed_out)
        return false;

    appendDefStatus(e_xin, 0, bf);

    return true;
}


bool BattleSimulator::getItemLing_cs(BattleFighter* bf, GData::LBSkillItem* item)
{
    if(!bf)
        return false;
    if(!item || !item->base)
        return false;

    if(item->base->ef_type != GData::e_lbef_cri)
        return false;

    appendDefStatus(e_ling, 0, bf);

    return true;
}

bool BattleSimulator::getItemLing_pr(BattleFighter* bf, GData::LBSkillItem* item)
{
    if(!bf)
        return false;
    if(!item || !item->base)
        return false;

    if(item->base->ef_type != GData::e_lbef_pri)
        return false;

    appendDefStatus(e_ling, 0, bf);
    return true;
}

void BattleSimulator::getAtkList(BattleFighter* bf, const GData::LBSkillBase* lbskill, AtkList& atkList)
{
    GData::Area* area = NULL;
    area = &(GData::areaList[lbskill->area]);

    int side = bf->getSide();
    if(lbskill->target == 1)
    {
        side = 1 - side;
    }

    switch(lbskill->area)
    {
    case 0:
        if(lbskill->target == 1)
        {
            int target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, target_pos));
            if(!bo)
                break;
            AtkFactor atkFactor;
            atkFactor.bf = bo;
            atkFactor.factor = 1;
            atkList.push_back(atkFactor);
        }
        break;
    case 1:
        {
            int i = 0;
            for(int pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
                if(!bo || bo->getHP() == 0)
                    continue;
                ++ i;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = lbskill->factor[i];
                atkList.push_back(atkFactor);
            }
        }
        break;
    case 9:
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 1;
            excepts[0] = bf->getPos();
            for(int i = 0; i < 2; ++ i)
            {
                BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
                if(!bo)
                    break;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
                excepts[exceptCnt] = bo->getPos();
                ++ exceptCnt;
            }
        }
        break;
    case 10:
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 1;
            excepts[0] = bf->getPos();
            for(int i = 0; i < 3; ++ i)
            {
                BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
                if(!bo)
                    break;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
                excepts[exceptCnt] = bo->getPos();
                ++ exceptCnt;
            }
        }
        break;
    case 11:
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 1;
            excepts[0] = bf->getPos();
            for(int i = 0; i < 4; ++ i)
            {
                BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
                if(!bo)
                    break;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
                excepts[exceptCnt] = bo->getPos();
                ++ exceptCnt;
            }
        }
        break;
    default:
        if(area->getCount() > 0)
        {
            int target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, target_pos));
            if(!bo)
                break;
            int x_ = bo->getPos() % 5;
            int y_ = bo->getPos() / 5;
            int cnt = area->getCount();
            int fsize = lbskill->factor.size();

            {
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
            }
            for(int i = 1; i < cnt; ++ i)
            {
                GData::Area::Data& ad = (*area)[i];
                int x = x_ + ad.x;
                int y = y_ + ad.y;
                if(x < 0 || x > 4 || y < 0 || y > 4)
                {
                    continue;
                }
                int pos = x + y * 5;
                int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
                BattleFighter* bo2 = static_cast<BattleFighter*>(getObject(side, pos));
                if(!bo2 || bo2->getHP() == 0)
                    continue;
                if(fsize != 0)
                {
                    AtkFactor atkFactor;
                    atkFactor.bf = bo2;
                    atkFactor.factor = lbskill->factor[idx];
                    atkList.push_back(atkFactor);
                }
                else
                {
                    AtkFactor atkFactor;
                    atkFactor.bf = bo2;
                    atkFactor.factor = 1;
                    atkList.push_back(atkFactor);
                }
            }
        }
        break;
    }
}

void BattleSimulator::getAtkList(BattleFighter* bf, const GData::SkillBase* skill, AtkList& atkList, Int8 offset /* = 0 */)
{
    if (GData::areaList.find(skill->area + offset) == GData::areaList.end())
        return;
    GData::Area* area = NULL;
    area = &(GData::areaList[skill->area + offset]);

    int side = bf->getSide();
    if(skill->target == 1)
    {
        side = 1 - side;
    }

    switch(skill->area + offset)
    {
    case 0:
        if(skill->target == 1)
        {
            int target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, target_pos));
            if(!bo)
                break;
            AtkFactor atkFactor;
            atkFactor.bf = bo;
            atkFactor.factor = 1;
            atkList.push_back(atkFactor);
        }
        break;
    case 1:
        {
            int i = 0;
            for(int pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
                if(!bo || bo->getHP() == 0)
                    continue;
                ++ i;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = skill->factor[i];
                atkList.push_back(atkFactor);
            }
        }
        break;
    case 9:
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 1;
            excepts[0] = bf->getPos();
            for(int i = 0; i < 2; ++ i)
            {
                BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
                if(!bo)
                    break;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
                excepts[exceptCnt] = bo->getPos();
                ++ exceptCnt;
            }
        }
        break;
    case 10:
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 1;
            excepts[0] = bf->getPos();
            for(int i = 0; i < 3; ++ i)
            {
                BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
                if(!bo)
                    break;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
                excepts[exceptCnt] = bo->getPos();
                ++ exceptCnt;
            }
        }
        break;
    case 11:
        {
            UInt8 excepts[25] = {0};
            UInt8 exceptCnt = 1;
            excepts[0] = bf->getPos();
            for(int i = 0; i < 4; ++ i)
            {
                BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
                if(!bo)
                    break;
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
                excepts[exceptCnt] = bo->getPos();
                ++ exceptCnt;
            }
        }
        break;
    default:
        if(area->getCount() > 0)
        {
            int target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, target_pos));
            if(!bo)
                break;
            int x_ = bo->getPos() % 5;
            int y_ = bo->getPos() / 5;
            int cnt = area->getCount();
            int fsize = skill->factor.size();

            {
                AtkFactor atkFactor;
                atkFactor.bf = bo;
                atkFactor.factor = 1;
                atkList.push_back(atkFactor);
            }
            for(int i = 1; i < cnt; ++ i)
            {
                GData::Area::Data& ad = (*area)[i];
                int x = x_ + ad.x;
                int y = y_ + ad.y;
                if(x < 0 || x > 4 || y < 0 || y > 4)
                {
                    continue;
                }
                int pos = x + y * 5;
                int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
                BattleFighter* bo2 = static_cast<BattleFighter*>(getObject(side, pos));
                if(!bo2 || bo2->getHP() == 0)
                    continue;
                if(fsize != 0)
                {
                    AtkFactor atkFactor;
                    atkFactor.bf = bo2;
                    atkFactor.factor = skill->factor[idx];
                    atkList.push_back(atkFactor);
                }
                else
                {
                    AtkFactor atkFactor;
                    atkFactor.bf = bo2;
                    atkFactor.factor = 1;
                    atkList.push_back(atkFactor);
                }
            }
        }
        break;
    }
}

UInt32 BattleSimulator::makeDamage(BattleFighter* bf, UInt32& u, StateType type, DamageType damageType)
{
    UInt32 uShow = u;
    if(!bf)
        return uShow;


    size_t idx = _defList.size() - 1;
#if 0
    const GData::SkillBase* skill = bf->getBiLanTianYiSkill();
    if(skill && skill->effect && bf->getEvadeCnt() > 2)
    {
        u = 0;
        bf->minusEvadeCnt(3);
        UInt8 evadeCnt = bf->getEvadeCnt();
        appendDefStatus(e_skill, skill->getId(), bf);
        if(evadeCnt == 0)
            appendDefStatus(e_unBiLanTianYi, evadeCnt, bf);
        else
            appendDefStatus(e_biLanTianYi, evadeCnt, bf);
        const std::vector<UInt16>& eft = skill->effect->eft;
        const std::vector<UInt8>& efl = skill->effect->efl;
        const std::vector<float>& efv = skill->effect->efv;
        size_t cnt = eft.size();
        if(cnt == efl.size() && cnt == efv.size())
        {
            for(size_t i = 0; i < cnt; ++ i)
            {
                if(eft[i] == GData::e_eft_bi_lan_tian_yi)
                {
                    float hp = bf->getMaxHP() * (skill->effect->efv[i]);
                    if (hp < 1.0f)
                        break;
                    bf->addHpShieldSelf(hp, skill->effect->efl[i]);
                    appendDefStatus(e_hpShieldSelf, hp, bf);
                    break;
                }
            }
        }
        return;
    }
#endif
    if(u > 0)
    {
        UInt8 count = bf->getSoulProtectCount();
        if(count > 0)
        {
            u /= 10;
            if (u <= 0)
                u = 1;
            uShow = u;
            --count;
            bf->setSoulProtectCount(count);
            if(count == 0)
                appendDefStatus(e_unSoulProtect, 0, bf);
        }
    }

    float& shieldHp = bf->getHpShieldSelf();
    if(shieldHp > 0.001f)
    {
        if(u > shieldHp)
        {
            u -= shieldHp;
            shieldHp = 0;
        }
        else
        {
            shieldHp -= u;
            u = 0;
        }

        if(shieldHp < 1.0f)
        {
            bf->setHpShieldSelf(0, 0);
            appendDefStatus(e_unHpShieldSelf, 0, bf);
        }
        else
        {
            appendDefStatus(e_hpShieldSelf, shieldHp, bf);
        }
    }

    float petShieldHp = bf->getPetShieldHP();
    if (petShieldHp > 0.001f) // 存在仙宠上的护盾
    {
        if (u > petShieldHp)
        {
            u -= petShieldHp;
            petShieldHp = 0;
        }
        else
        {
            petShieldHp -= u;
            u = 0;
        }
        
        if(shieldHp < 1.0f)
        {
            shieldHp = 0;
            appendDefStatus(e_unPetShield, 0, bf);
        }
        bf->setPetShieldHP(shieldHp);
    }

    if(u > 0)
    {
        bf->makeDamage(u);
    }

    // 天佑技能
    if(u > 0 && bf->getHP() < static_cast<UInt32>(0.3f * bf->getMaxHP()))
    {
        const GData::SkillBase* skillBase = bf->getSkillSoulProtect();
        if(skillBase)
        {
            UInt8 last = bf->getSoulProtectLast();
            if(last > 0)
            {
                if(static_cast<UInt32>(skillBase->prob * 100) > uRand(10000))
                {
                    appendDefStatus(e_skill, skillBase->getId(), bf);
                    appendDefStatus(e_soulProtect, 0, bf);
                    bf->setSoulProtectCount(3);
                }
                bf->setSoulProtectLast(--last);
            }
        }

        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        while(NULL != (passiveSkill = bf->getPassiveSkillOnHP10P100(idx)))
        {
            if(passiveSkill->effect == NULL)
                continue;
            if(SKILL_ID(passiveSkill->getId()) == 97)
                break;
        }

        if(passiveSkill)
        {
            UInt8 count = bf->getXinMoCount();
            if(count < 1)
            {
                appendDefStatus(e_skill, passiveSkill->getId(), bf);
                float value = bf->getAttack() * passiveSkill->effect->efv[0];
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, passiveSkill, e_stAtk, value, /*passiveSkill->last*/2, bf->getSide() != 0);
                value = bf->getMagAttack() * passiveSkill->effect->efv[0];
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, passiveSkill, e_stMagAtk, value, /*passiveSkill->last*/2, bf->getSide() != 0);
                bf->setXinMoCount(++count);
            }
        }
    }

    if(type != e_damCounter)
        appendDefStatus(type, uShow, bf, damageType);

    if(bf->getHP() == 0)
    {
        onDead(false, bf);
    }
    else if(_winner == 0)
    {
        onDamage(bf, true, u);
        size_t idx = 0;
        const GData::SkillBase *skill;
        doLingshiModelAttack(bf, 1);
        while(NULL != (skill = bf->getPassiveSkillLingshi100(idx)))
        {
            if(skill->effect && skill->effect->eft[0] ==  GData::e_eft_lingshi_buqu)
            {
                setStatusChange_Def(bf, bf->getSide(), bf->getPos(), skill, getBFDefend(bf) * skill->effect->efv[0], bf->getNewModeLast(), true);
                setStatusChange_MagDef(bf, bf->getSide(), bf->getPos(), skill, getBFMagDefend(bf) * skill->effect->efv[0], bf->getNewModeLast(), true);
                appendDefStatus(e_skill, skill->getId(), bf);
                break;
            }
        }

    }

    if(type == e_damCounter)
    {
        _defList[idx].counterDmg = uShow;
        _defList[idx].counterLeft = bf->getHP();
    }

    return uShow;
}

bool BattleSimulator::doAuraPresent(BattleFighter* bf)
{
    if(!bf)
        return false;

    float aura = (bf->getAura() < 100 ? bf->getAura() : 100) * bf->getAuraPrecent()/100;
    if(aura < 0.1)
        return false;

    UInt8 cnt = 0;
    int side = 1 - bf->getSide();
    std::vector<BattleFighter*> bo;
    for(int pos = 0; pos < 25; ++ pos)
    {
        BattleFighter* bo2 = static_cast<BattleFighter*>(getObject(side, pos));
        if(!bo2 || bo2->getHP() == 0)
            continue;
        ++ cnt;
        bo.push_back(bo2);
    }
    if(cnt > 0)
    {
        aura = aura/cnt;
    }
    if(aura > 0.1)
    {
        for(int i = 0; i < cnt; ++ i)
            setStatusChange_Aura(NULL, bo[i]->getSide(), bo[i]->getPos(), NULL, aura+0.9, 0, true);
    }

    return true;
}

bool BattleSimulator::doConfusePresent(BattleFighter* bf)
{
    float value = bf->getConfucePresent()/100;
    UInt8 last = bf->getConfucePresentCD();
    if(value < 0.001f)
        return false;

    if(bf->getDeepConfuseDmgExtra() < 0.001f)
    {
        bf->setConfuseLevel(9);
        bf->setConfuseRound(1);

        bf->setDeepConfuseDmgExtra(value, last);
        appendDefStatus(e_deepConfuse, 0, bf);
        calcAbnormalTypeCnt(bf);
        calcSilkwormCnt(bf);
    }

    return true;
}

bool BattleSimulator::doStunPresent(BattleFighter* bf)
{
    float value = bf->getStunPresent()/100;
    UInt8 last = bf->getStunPresentCD();
    if(value < 0.001f)
        return false;

    if(bf->getDeepStunDmgExtra() < 0.001f)
    {
        bf->setStunLevel(9);
        bf->setStunRound(1);

        bf->setDeepStunDmgExtra(value, last);
        appendDefStatus(e_deepStun, 0, bf);
        calcAbnormalTypeCnt(bf);
        calcSilkwormCnt(bf);
    }

    return true;
}

bool BattleSimulator::doBlindPresent(BattleFighter* bf)
{
    float value = bf->getBlindPresent()/100;
    UInt8 last = bf->getBlindPresentCD();
    if(value < 0.001f)
        return false;

    if(bf->getDeepBlindDmgExtra() < 0.001f)
    {
        bf->setBlind(0.75f, 1);
        bf->setDeepBlind(value, last);
        appendDefStatus(e_deepBlind, 0, bf);
        calcAbnormalTypeCnt(bf);
    }

    return true;
}

UInt32 BattleSimulator::tryPetEnter(UInt8 side, UInt8 reiatsuType, bool slience /* = false */)
{
    // 仙宠尝试上场（根据增加后的灵压判定是否上场）
    UInt8 val = 0;
    switch (reiatsuType)
    {
        case e_reiatsu_normal_attack:
            val = 4;
            break;
        case e_reiatsu_skill_attack:
            val = 4;
            break;
        case e_reiatsu_peerless:
            val = 6;
            break;
        case e_reiatsu_round:
            val = 5;
        default:
            break;
    }

    if (addReiatsu(side, val))
    {
        return doPetEnter(side);
    }
    if (!slience)
        appendReiatsuChange(side);
    return 0;
}

UInt32 BattleSimulator::doPetEnter(UInt8 side)
{
    // 仙宠入场
    if (side < 0 || side >= 2)
    {
#ifdef _DEBUG
        //printf("doPetEnter side error\n");
#endif
        return 0;
    }
    UInt32 pos = upPetObject(side);
    if (pos >= 25)
    {
        _backupObjs[side] = NULL;
#ifdef _DEBUG
        //printf("doPetEnter pos error, pos = %d\n", pos);
#endif
        return 0;
    }

    UInt32 rcnt = 0;

    BattleObject * bo = getObject(side, pos);
    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
    {
        _backupObjs[side] = NULL;
#ifdef _DEBUG
        //printf("doPetEnter bo error.\n");
#endif
        return 0;
    }
    BattleFighter* bf = static_cast<BattleFighter *>(bo);
    _activeFgt = bf;
    appendReiatsuChange(side);
    appendInitDefStatus(bf);
    appendDefStatus(e_petAppear, bf->getId(), bf);
    insertFighterStatus(bf);
    _backupObjs[side] = NULL;

    if(bf->getPassiveSkillOnTherapy())
        _onTherapy.push_back(bf);
    if(bf->getPassiveSkillOnSkillDmg())
        _onSkillDmg.push_back(bf);
    if(bf->getPassiveSkillOnOtherDead())
        _onOtherDead.push_back(bf);
    if(bf->getPassiveSkillOnPetProtectForce())
        _onPetProtect.push_back(bf);
    if(bf->getPassiveSkillOnAtkDmgForce())
        _onPetAtk.push_back(bf);


    const GData::SkillBase* passiveSkill = NULL;
    size_t skillIdx = 0;
    while(NULL != (passiveSkill = bf->getPassiveSkillEnter100(skillIdx)))
    {
        int target_side, target_pos, cnt;
        getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
        rcnt += doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
    }

    if(NULL != (passiveSkill = bf->getPassiveSkillEnter()))
    {
        int target_side, target_pos, cnt;
        getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
        rcnt += doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
    }

    if(_defList.size() > 0 || _scList.size() > 0)
    {
        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 5, 0, false, false);
        ++ rcnt;
    }
    _activeFgt = NULL;
    return rcnt;
}

bool BattleSimulator::tryProtectDamage(BattleFighter* bf, float& phyAtk, float& magAtk, float factor)
{
     if(bf->isSummon())
         return false;

    // 宠物保护主目标吸收一半伤害，当然，也可能保护完两个都挂了
    int tidx = -1;
    if ((tidx = getSpecificTarget(bf->getSide(), BattleSimulator::isPet)) >= 0)
    {
        BattleFighter * pet = static_cast<BattleFighter *> (getObject(bf->getSide(), tidx));
        if (!pet)
            return false;
        if (pet == bf)
            return false;
        if (bf->getPetProtect100())
            return do100ProtectDamage(bf, pet, phyAtk, magAtk, factor);
        else
        {
            for(size_t i = 0; i < _onPetProtect.size(); ++ i)
            {
                BattleFighter* bf = _onPetProtect[i];
                if(!bf || bf->getHP() == 0)
                    continue;

                UInt32 stun = bf->getStunRound();
                UInt32 confuse = bf->getConfuseRound();
                UInt32 forget = bf->getForgetRound();

                if(stun > 0 || confuse > 0 || forget > 0)
                    continue;
                return doProtectDamage(bf, pet, phyAtk, magAtk, factor);
            }
        }
    }
    return false;
}

bool BattleSimulator::do100ProtectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
    // 宠物100%保护主目标吸收一半伤害
#ifdef _BATTLE_DEBUG
    std::cout << "phyAtk = " << phyAtk << ", magAtk = " << magAtk << "." << std::endl;
#endif
    const GData::SkillBase* pskillOrign = bf->getPetProtect100Skill();
    if (!pskillOrign) 
        return false;
    bf->setPetProtect100(false, static_cast<UInt8>(0), NULL);
    appendDefStatus(e_unPetProtect100, 0, bf);
    const GData::SkillBase* pskill = pet->getPassiveSkillOnPetProtectForce();
    if(!pskill || !pskill->effect)
        return false;
    const std::vector<UInt16>& eft = pskill->effect->eft;
    const std::vector<float>& efv = pskill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return false;


    float ssfactor = 0.0f;

    ModifyAttackValue_SkillStrengthen(pet, pskill, ssfactor, true);  //免伤率提升效果提升100%

    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_pet_protect_reduce)
        {
            if (efv[i] + ssfactor >= 1)
                factor = 0.001f;
            else
                factor *= (1 - efv[i] - ssfactor);
        }
    }

    appendDefStatus(e_skill, pskill->getId(), pet);

    float phyAtkPet;
    float magAtkPet;
    if(ssfactor > 0.001f)
    {
        phyAtkPet = phyAtk * 0.7;
        magAtkPet = magAtk * 0.7;
        phyAtk *= 0.3;
        magAtk *= 0.3;
    }
    else
    {
        phyAtkPet = phyAtk / 2;
        magAtkPet = magAtk / 2;
        phyAtk /= 2;
        magAtk /= 2;
    }
    return protectDamage(bf, pet, phyAtkPet, magAtkPet, factor);
}

bool BattleSimulator::doProtectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
#ifdef _BATTLE_DEBUG
    std::cout << "phyAtk = " << phyAtk << ", magAtk = " << magAtk << "." << std::endl;
#endif
    // 宠物概率保护主目标吸收一半伤害
    for(size_t i = 0; i < _onPetProtect.size(); ++ i)
    {
        const GData::SkillBase* pskill = pet->get2ndProtectSkill();
        bool is2nd;
        if(pskill)
        {
            is2nd = true;
        }
        else
        {
            is2nd = false;
            pskill = pet->getPassiveSkillOnPetProtect();
            if(!pskill)
                return false;
        }
        if(!pskill)
            return false;

        pet->set2ndProtectSkill(0, NULL);
        if(!is2nd)
        {
            GData::SkillStrengthenBase* ss = pet->getSkillStrengthen(SKILL_ID(pskill->getId()));
            if(ss)
            {
                const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_2ND_HAPPEND);
                if (ef)
                    pet->set2ndProtectSkill(ef->value, pskill);
            }
        }

        if(!pskill->effect)
            return false;

        const std::vector<UInt16>& eft = pskill->effect->eft;
        const std::vector<float>& efv = pskill->effect->efv;

        size_t cnt = eft.size();
        if(cnt != efv.size())
            return false;

        float ssfactor = 0.0f;
        ModifyAttackValue_SkillStrengthen(pet, pskill, ssfactor, true);  //免伤率提升效果提升100%

        appendDefStatus(e_skill, pskill->getId(), pet);

        for(size_t i = 0; i < cnt; ++ i)
        {
            if(eft[i] == GData::e_eft_pet_protect_reduce)
            {
                if (efv[i] + ssfactor >= 1)
                    factor = 0.001f;
                else
                    factor *= (1 - efv[i] - ssfactor);
            }
        }

        float phyAtkPet;
        float magAtkPet;
        if(ssfactor > 0.001f)
        {
            phyAtkPet = phyAtk * 0.7;
            magAtkPet = magAtk * 0.7;
            phyAtk *= 0.3;
            magAtk *= 0.3;
        }
        else
        {
            phyAtkPet = phyAtk / 2;
            magAtkPet = magAtk / 2;
            phyAtk /= 2;
            magAtk /= 2;
        }
        return protectDamage(bf, pet, phyAtkPet, magAtkPet, factor);
    }
    return false;
}

bool BattleSimulator::protectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
#ifdef _BATTLE_DEBUG
    std::cout << "phyAtk = " << phyAtk << ", magAtk = " << magAtk << "." << std::endl;
#endif
    UInt32 dmg = 0;
    UInt32 magdmg = 0;
    bool dmgFlag = false;
    bool magdmgFlag = false;
    //bool pr = bf->calcPierce(pet);
    //float toughFactor = pr ? pet->getTough(bf) : 1.0f;
    float toughFactor = 1.0f;

    if(magAtk)
    {
        float magdef = getBFMagDefend(pet);
        float magatkreduce = getBFMagAtkReduce(pet);
        magdmg = _formula->calcDamage(factor * magAtk, magdef, bf->getLevel(), toughFactor, magatkreduce);

        magdmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        magdmg = magdmg > 0 ? magdmg : 1;
        magdmgFlag = true;
        magdmg = doBufMakeDamage(pet, magdmg);
    }

    if(phyAtk)
    {
        float def = getBFDefend(pet);
        float atkreduce = getBFAtkReduce(pet);
        dmg = _formula->calcDamage(factor * phyAtk, def, bf->getLevel(), toughFactor, atkreduce);

        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        dmg = dmg > 0 ? dmg : 1;
        dmgFlag = true;
        dmg = doBufMakeDamage(pet, dmg);
    }


    UInt32 dmg3 = dmg + magdmg;
    doShieldHPAttack(pet, dmg3);
    if(dmg3 > 0)
    {
        if (magdmgFlag)
            makeDamage(pet, magdmg, e_damNormal, e_damageMagic);
        if (dmgFlag)
            makeDamage(pet, dmg, e_damNormal, e_damagePhysic);
    }
#ifdef _BATTLE_DEBUG
    std::cout << "dmg: " << dmg << ", magdmg: " << magdmg << std::endl << std::endl;
#endif

    return true;
}

bool BattleSimulator::hasPetMarked(BattleObject* bo)
{
    // 神兽印记buff
    if (bo && bo->getHP() && bo->isChar())
    {
        BattleFighter* bf = static_cast<BattleFighter *>(bo);
        if (bf->isPetMark())
            return true;
    }
    return false;
}

bool BattleSimulator::isPet(BattleObject* bo)
{
    // 是否是宠物
    if (bo && bo->getHP() > 0 && bo->isChar())
    {
        BattleFighter* bf = static_cast<BattleFighter *>(bo);
        return bf->isPet();
    }
    return false;
}

bool BattleSimulator::isBehindPos(BattleObject* bo, UInt8 targetPos, UInt8 maxLength)
{
    UInt8 pos = bo->getPos();
    if ((pos % 5) != (targetPos % 5))
        return false;
    if ((targetPos / 5 + maxLength) >= (pos / 5))
        return true;
    return false;
}

int BattleSimulator::getPossibleTarget( int side, int idx , BattleFighter * bf /* = NULL */)
{
    //正常的仙宠会优先寻找有神兽印记的目标，并且临时增加攻击力
    if (bf && bf->isPet())
    {
        if( !bf->getStunRound() && !bf->getConfuseRound() && ! bf->getForgetRound())
        {
            int tidx = getSpecificTarget((side == 0 ? 1 : 0), hasPetMarked);
            if (tidx >= 0)
            {
                if(!bf->getPetExAtkId())
                {
                    bf->setPetExAtkEnable(true);
                    appendDefStatus(e_skill, bf->getPetExAtkId(), bf);
                }
                return tidx;
            }
        }
        bf->setPetExAtkEnable(false);
    }
    return static_cast<BattleField *>(this)->getPossibleTarget(side, idx);
}

bool BattleSimulator::tryAttackWithPet(BattleFighter* bf)
{
    if(bf->isSummon())
        return false;
    // 宠物合击
    int tidx = 0;
    if ((tidx = getSpecificTarget(bf->getSide(), BattleSimulator::isPet)) >= 0)
    {
        BattleFighter * pet = static_cast<BattleFighter *> ((*this)(bf->getSide(), tidx));
        if (pet == bf)
            return false;
        if (bf->getPetAtk100Last())
        {
            do100AttackWithPet(bf, pet);
        }
        else
        {
            doAttackWithPet(bf, pet);
        }
    }
    return false;
}

bool BattleSimulator::do100AttackWithPet(BattleFighter* bf, BattleFighter* pet)
{
    // 宠物100%合击
    appendDefStatus(e_unPetAtk100, 0, bf);
    const GData::SkillBase* pskill = pet->getPassiveSkillOnAtkDmgForce();
    if(!pskill)
        return false;

    GData::SkillStrengthenBase* ss = pet->getSkillStrengthen(SKILL_ID(pskill->getId()));
    if(ss)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_2ND_HAPPEND);
        if (ef)
            pet->set2ndCoAtkSkill(ef->value * 100, pskill);
    }

    float atk = getBFMagAtk(pet) * pskill->effect->magatkP;
    bf->setPetCoAtk(atk);
    bf->setPetAtk100(0, 0);
    appendDefStatus(e_skill, pskill->getId(), pet);
    return true;
}

bool BattleSimulator::doAttackWithPet(BattleFighter* bf, BattleFighter* pet)
{
    // 宠物概率合击
    //for(size_t i = 0; i < _onPetAtk.size(); ++ i)
    {
        UInt32 stun = pet->getStunRound();
        UInt32 confuse = pet->getConfuseRound();
        UInt32 forget = pet->getForgetRound();
        if(stun > 0 || confuse > 0 || forget > 0)
            return false;

        const GData::SkillBase* pskill = pet->get2ndCoAtkSkill();
        bool is2nd;
        if(pskill)
        {
            is2nd = true;
        }
        else
        {
            is2nd = false;
            pskill = pet->getPassiveSkillOnAtkDmg();
        }
        if(!pskill)
            return false;

        float ssfactor = 0.0f;
        ModifyAttackValue_SkillStrengthen(pet, pskill, ssfactor, true);
        float factor = 1 + ssfactor;

        float atk = getBFMagAtk(pet) * pskill->effect->magatkP * factor;
        bf->setPetCoAtk(atk);
        appendDefStatus(e_skill, pskill->getId(), pet);
        pet->set2ndCoAtkSkill(0, NULL);

        if(!is2nd)
        {
            GData::SkillStrengthenBase* ss = pet->getSkillStrengthen(SKILL_ID(pskill->getId()));
            if(ss)
            {
                const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_2ND_HAPPEND);
                if (ef)
                    pet->set2ndCoAtkSkill(ef->value, pskill);
            }
        }
    }
    return true;
}

void BattleSimulator::appendMaxReiatsu()
{
        appendStatusChangeForReiastu(e_stMaxReiastu, getToggleReiatsu(0) ,0 , 0);
        appendStatusChangeForReiastu(e_stMaxReiastu, getToggleReiatsu(1) ,0 , 1);

        appendStatusChangeForReiastu(e_stReiastu, getReiatsu(0) ,0 , 0);
        appendStatusChangeForReiastu(e_stReiastu, getReiatsu(1) ,0 , 1);
}

void BattleSimulator::appendReiatsuChange(int side)
{
    if (_backupObjs[side])
        appendStatusChangeForReiastu(e_stReiastu, getReiatsu(side) ,0 , side);
}

UInt32 BattleSimulator::upPetObject(UInt8 side, bool isReplace /* = true */)
{
    // 上场候补选手（如果上场位置有人，则直接顶掉）
    if (side < 0 || side >= 2)
        return 0xff;
    if (_backupObjs[side] == NULL)
        return 0xff;
    int pos = _backupTargetPos[side];
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, pos));
    if(bo != NULL && bo->getHP() > 0)
        removeFighterStatus(bo);

    _backupObjs[side]->setPos(_backupTargetPos[side]);
    setObject(side, pos, _backupObjs[side]);

    return pos;
}

void BattleSimulator::appendInitDefStatus(BattleFighter* bf)
{
    float value = 0;
    value = bf->getAttack();
    appendStatusChange(e_stAtkInit, value, 0, bf);
    value = bf->getDefend();
    appendStatusChange(e_stDefInit, value, 0, bf);
    value = bf->getMagAttack();
    appendStatusChange(e_stMagAtkInit, value, 0, bf);
    value = bf->getMagDefend();
    appendStatusChange(e_stMagDefInit, value, 0, bf);
    value = bf->getTough(NULL);
    appendStatusChange(e_stToughInit, value, 0, bf);
    value = bf->getAction();
    appendStatusChange(e_stActionInit, value, 0, bf);
    value = bf->getEvade(NULL);
    appendStatusChange(e_stEvadeInit, value, 0, bf);
    value = bf->getCritical(NULL);
    appendStatusChange(e_stCriticalInit, value, 0, bf);
    value = bf->getPierce(NULL);
    appendStatusChange(e_stPierceInit, value, 0, bf);
    value = bf->getCounter(NULL);
    appendStatusChange(e_stCounterInit, value, 0, bf);
    value = bf->getMagRes(NULL);
    appendStatusChange(e_stMagResInit, value, 0, bf);
    value = bf->getCriticalDmg();
    appendStatusChange(e_stCriticalDmgInit, value, 0, bf);
    value = bf->getHitrate(NULL);
    appendStatusChange(e_stHitRateInit, value, 0, bf);
    value = bf->getAtkReduce();
    appendStatusChange(e_stAtkReduceInit, value, 0, bf);
    value = bf->getMagAtkReduce();
    appendStatusChange(e_stMagAtkReduce, value, 0, bf);
}

void BattleSimulator::doSkillStrenghtenHitConfuse(BattleFighter* bf, const GData::SkillBase* skill, GData::SkillStrengthenBase* ss, int target_side, int target_pos)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(!bo || !bf || !ss || !skill)
        return;

    const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_HITE, GData::TYPE_ADDSTATE);
    if(!ef || ef->value > _hit_cnt)
        return;
    if(uRand(10000) < (ef->prob * 100))
    {
        UInt32 nState = ef->valueExt1;
        AddSkillStrengthenState(bf, bo, skill->getId(), nState, ef->last);
    }
}

bool BattleSimulator::isFireDefend(const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return false;
    const std::vector<UInt16>& eft = skill->effect->eft;
    size_t cnt = eft.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_fire_def)
            return true;
    }

    return false;
}

bool BattleSimulator::doSkillStrengthen_FireFakeDead(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;

    bf->setFireFakeDead(ef->prob, ef->last);
    appendDefStatus(e_fireFakeDead, 0, bf);

    return true;
}

void BattleSimulator::doSkillEffectExtra_SneakAtk(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(!bf || !bo)
        return;
    UInt16 last = skill->last;
    if(bf->isHide() || bo->isMarkMo())
        ++ last;
    bf->setSneakAtk(skill->effect->efv[eftIdx], e_sneak_on, last);
    appendDefStatus(e_sneakAtk, 0, bf);

    BattleFighter* selfSummon = bf->getSelfSummon();
    if(selfSummon)
        selfSummon->setSneakAtk(skill->effect->efv[eftIdx], e_sneak_on, last);
}

bool BattleSimulator::doSkillStrengthen_SneakRecover(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;

    bf->setRecoverSnakeAtk(ef->value * 100);
    BattleFighter* selfSummon = bf->getSelfSummon();
    if(selfSummon)
        selfSummon->setRecoverSnakeAtk(ef->value * 100);

    return true;
}

bool BattleSimulator::doSkillStrengthen_SelfAttack(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;

    // 附加自己百分比的攻击力到指定人身上
    UInt16 last = ef->last;
    float atkadd = (bf->_magatk) * (ef->value / 100);
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo || !bo->isChar())
        return false;
    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);
    //if (bf2->getClass() == e_cls_dao || bf2->getClass() == e_cls_mo)
    {
        bf2->setPetAttackAdd(atkadd, last);
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf2->getAttack());
        appendStatusChange(e_stAtk, value, skillId, bf2);
    }
    //else
    {
        bf2->setPetMagAtkAdd(atkadd, last);
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf2->getMagAttack());
        appendStatusChange(e_stMagAtk, value, skillId, bf2);
    }
    return true;
}

bool BattleSimulator::doSkillStrengthen_DmgDeep(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;

    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo || !bo->isChar())
        return false;
    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);

    bf2->setDmgDeep(ef->value / 100, ef->last);
    appendDefStatus(e_dmgDeep, 0, bf2);

    return true;
}

bool BattleSimulator::doSkillStrengthen_NingShi(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;

    int side = 1 - bf->getSide();
    BattleFighter* bo = getRandomFighter(side, NULL, 0);
    if (!bo || !bo->isChar())
        return false;
    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);

    bf2->setDmgNingShi(bf, ef->last, ef->value / 100 * getBFAttack(bf));
    appendDefStatus(e_dmgNingShi, 0, bf2);

    return true;
}

bool BattleSimulator::doSkillStrengthen_DefChange(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo || !bo->isChar())
        return false;
    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);

    bf2->setDefendChangeSS(bf2->_defend * ef->value/100, ef->last);
    bf2->setMagDefendChangeSS(bf2->_defend * ef->value, ef->last);
    setStatusChange(bf2, bf2->getSide(), bf2->getPos(), 1, skill, e_stDef, bf2->getDefendChangeSS(), ef->last, false);
    setStatusChange(bf2, bf2->getSide(), bf2->getPos(), 1, skill, e_stMagDef, bf2->getMagDefendChangeSS(), ef->last, false);

    return true;

}

bool BattleSimulator::doSkillStrengthen_HPPRecover(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active)
{
    if(!bf || !skill || !ef)
        return false;

    UInt32 hpr = bf->regenHP(bf->getMaxHP() * ef->value / 100);
    if(hpr > 0)
    {
        appendDefStatus(e_damHpAdd, hpr, bf);
        onHPChanged(bf);
    }

    return true;
}


void BattleSimulator::doSneakAttack(BattleFighter* bf, BattleFighter* bo, bool& pr, bool& cs)
{
    if(!bf || !bo)
        return;

    float sneak_atk =  bf->getSneakAtk();
    if(sneak_atk < 0.001f)
        return;

    UInt32 dmg = 0;
    UInt8 bo_stun = bo->getStunRound();
    bool enterEvade = bo->getEvad100();
    bool defend100 = bo->getDefend100();

    if(!defend100 && (bo_stun > 0 || (!enterEvade && bf->calcHit(bo, NULL) && !bo->getMoEvade100())))
    {
        pr = bf->calcPierce(bo);
        float atk = 0;
        float cf = 0.0f;
        atk = calcAttack(bf, cs, bo, &cf);
        if(cs)
        {
            UInt8 s = bf->getSide();
            if(s < 2)
                _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

        }

        float toughFactor = pr ? bo->getTough(bf) : 1.0f;
        float def = getBFDefend(bo);
        float atkreduce = getBFAtkReduce(bo);
        dmg = _formula->calcDamage(atk * sneak_atk, def, bf->getLevel(), toughFactor, atkreduce);
        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
        dmg = dmg > 0 ? dmg : 1;
        makeDamage(bo, dmg, e_damNormal, e_damagePhysic);
        UInt32 rhp = (dmg >> 1);
        bf->regenHP(rhp);
        appendDefStatus(e_damAbsorb, rhp, bf);
        onHPChanged(bf);

        if(bo->getHP() == 0)
            onDead(false, bo);
        else if(_winner == 0)
            onDamage(bo, true, NULL);
    }
    else if(!defend100 && !enterEvade)
        appendDefStatus(e_damEvade, 0, bo);
    else
    {
        if(defend100)
        {
            appendDefStatus(e_damOut, 0, bo);
            bo->setDefend100(false);
        }
        else
        {
            appendDefStatus(e_damEvade, 0, bo);
            bo->setEvad100(false);
        }
    }
}

void BattleSimulator::doSkillEffectExtra_DecWaveDmg(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf)
        return;
    bf->setDecWaveDmg(skill->effect->efv[eftIdx]);
}

void BattleSimulator::doSkillEffectExtra_LingYouAtk(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf)
        return;
    bf->setLingYouAtk(skill->effect->efv[eftIdx]);
}

void BattleSimulator::doSkillEffectExtra_LingYouMagAtk(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf)
        return;
    bf->setLingYouMagAtk(skill->effect->efv[eftIdx]);
}

void BattleSimulator::doSkillEffectExtra_LingYouDef(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf)
        return;
    bf->setLingYouDef(skill->effect->efv[eftIdx]);
}

void BattleSimulator::doSkillEffectExtra_LingYouMagDef(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf)
        return;
    bf->setLingYouMagDef(skill->effect->efv[eftIdx]);
}

bool BattleSimulator::doSkillEffectExtra_Dead(BattleFighter* bf, const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return false;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return false;

    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_lingqu)
        {
            UInt8 newModeLast = bf->getNewModeLast();
            if(newModeLast > 0)
            {
                bf->setNewModeLast(0);
                appendDefStatus(e_unChangeMode, 0 ,bf);
                bf->setChangeStatus(2);
                bf->unUpdateAllPassiveSkillLingshi();
            }
            onDeadLingshi(bf);

            bf->setLingQu(true, efl[i]);
            appendDefStatus(e_lingQu, 0, bf);
            return true;
        }
        else if(eft[i] == GData::e_eft_soul_out)
        {
            UInt8 newModeLast = bf->getNewModeLast();
            if(newModeLast > 0)
            {
                bf->setNewModeLast(0);
                appendDefStatus(e_unChangeMode, 0 ,bf);
                bf->setChangeStatus(2);
                bf->unUpdateAllPassiveSkillLingshi();
            }
            onDeadLingshi(bf);

            bf->setSoulOut(efv[i], efl[i]);
            appendDefStatus(e_soulout, 0, bf);
            return true;
        }
    }

    return false;
}

void BattleSimulator::doSkillEffectExtra_AbnormalTypeDmg(BattleFighter* bf, const GData::SkillBase* skill, bool& cs, bool& pr)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;

    bool first = true;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_abnormal_type_dmg)
        {
            bf->useAbnormalTypeCnt();
            if(bf->getAbnormalTypeCnt() > 0)
                appendDefStatus(e_abnormalType, bf->getAbnormalTypeCnt(), bf);
            else
                appendDefStatus(e_unAbnormalType, 0, bf);

            AtkList atklist;
            getAtkList(bf, skill, atklist);
            UInt8 cnt2 = atklist.size();
            for(size_t j = 0; j < cnt2; ++ j)
            {
                BattleFighter* target = static_cast<BattleFighter *>(atklist[j].bf);

                UInt8 target_stun = target->getStunRound();
                bool enterEvade = target->getEvad100();
                bool defend100 = target->getDefend100();

                if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(target, NULL))))
                {

                    bool pr2 = bf->calcPierce(target);
                    float cf = 0.0f;
                    bool cs2 = false;
                    if(cs2)
                    {
                        UInt8 s = bf->getSide();
                        if(s < 2)
                            _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

                    }
                    if(first)
                    {
                        cs = cs2;
                        pr = pr2;
                        first = false;
                    }

                    float atk = 0;
                    float def = 0;
                    float reduce = 0;
                    bool isPhysic = false;
                    if(bf->getClass() == GObject::e_cls_dao || bf->getClass() == GObject::e_cls_mo)
                    {
                        atk = efv[i] * calcAttack(bf, cs2, target, NULL);
                        def = getBFDefend(target);
                        reduce = getBFAtkReduce(target);
                        isPhysic = true;
                    }
                    else
                    {
                        atk = efv[i] * calcMagAttack(bf, cs2, target, NULL);
                        def = getBFMagDefend(target);
                        reduce = getBFMagAtkReduce(target);
                        isPhysic = false;
                    }

                    float toughFactor = pr2 ? target->getTough(bf) : 1.0f;
                    float factor = atklist[j].factor;
                    float dmg = _formula->calcDamage(atk * factor, def, bf->getLevel(), toughFactor, reduce);
                    dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
                    dmg = dmg > 0 ? dmg : 1;

                    UInt32 curDmg = dmg;
                    makeDamage(target, curDmg, e_damNormal, isPhysic ? e_damagePhysic : e_damageMagic);
                }
                else if(!defend100 && !enterEvade)
                    appendDefStatus(e_damEvade, 0, target);
                else
                {
                    if(!defend100)
                    {
                        appendDefStatus(e_damEvade, 0, target);
                        target->setEvad100(false);
                    }
                    else
                    {
                        appendDefStatus(e_damOut, 0, target);
                        target->setDefend100(false);
                    }
                }
            }
            return;
        }
    }

    return;
}

void BattleSimulator::doSkillEffectExtra_BleedTypeDmg(BattleFighter* bf, const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;

    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_bleed_type_dmg)
        {
            bf->useBleedTypeCnt();
            if(bf->getBleedTypeCnt() > 0)
                appendDefStatus(e_bleedType, bf->getBleedTypeCnt(), bf);
            else
                appendDefStatus(e_unBleedType, 0, bf);

            if(bf->isSoulOut())
                continue;
            UInt32 hpMax = bf->getMaxHP();
            UInt32 hp = efv[i] * static_cast<float>(hpMax);
            UInt32 hpr = bf->regenHP(hp);
            if(hpr == 0)
                continue;
            appendDefStatus(e_damHpAdd, hpr, bf);
            onHPChanged(bf);
            return;
        }
    }
    return;
}

void BattleSimulator::doSkillEffectExtra_LingShiBleed2(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt8>& efl = skill->effect->efl;

    AtkList atklist;
    getAtkList(bf, skill, atklist);
    UInt8 cnt = atklist.size();
    float dmg = bf->getMaxHP() * efv[eftIdx];
    for(size_t i = 0; i < cnt; ++ i)
    {
        BattleFighter* bo = atklist[i].bf;
        float factor = atklist[i].factor;
        bo->setLingShiBleed(factor*dmg, efl[eftIdx]);
        appendDefStatus(e_lingShiBleed, 0, bo);
    }
}

void BattleSimulator::doSkillEffectExtra_CriticalDmgReduce(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    AtkList atklist;
    getAtkList(bf, skill, atklist);
    UInt8 cnt = atklist.size();
    for(size_t i = 0; i < cnt; ++ i)
    {
        BattleFighter* bo = atklist[i].bf;
        bo->setCriticalDmgReduce(efv[eftIdx]);
    }
}

void BattleSimulator::doSkillEffectExtra_BuddhaLight(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;

    for(size_t i = 0; i < cnt; ++ i)
    {
        GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
        const GData::SkillStrengthenEffect* ef = NULL;
        if(ss)
            ef = ss->getEffect(GData::ON_BUDDHA_LIGHT, GData::TYPE_AURA_GET);

        AtkList atklist;
        getAtkList(bf, skill, atklist);
        UInt8 cnt2 = atklist.size();
        for(size_t j = 0; j < cnt2; ++ j)
        {
            BattleFighter* bo = atklist[j].bf;
            bo->setBuddhaLight(skill->prob, efl[i]);
            if(ef)
                bo->setBuddhaLightValue(ef->value);
            bo->setBuddhaLightLauncher(bf);
        }
        appendDefStatus(e_buddhaLight, 0, bf);
        return;
    }
}

void BattleSimulator::doSkillEffectExtra_OtherSidePeerlessDisable(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;

    target_side = 1 - bf->getSide();
    for(size_t count = 0; count < cnt; ++count)
    {
        for(UInt8 i = 0; i < 25; i++)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, i));
            if(bo == NULL || bo->getHP() == 0)
                continue;
            if(bo->getPeerLessDisableLast() == 0 && static_cast<float>(uRand(10000)) < skill->prob * 100)
            {
                if(static_cast<float>(uRand(10000) < bf->getMagRes(bo) * 100))
                    appendDefStatus(e_Res, 0, bo);
                else
                {
                    bo->setPeerLessDisableLast(efl[count]);
                    appendDefStatus(e_benevolent, 0, bo);
                }
            }
        }
        return;
    }
}

void BattleSimulator::doSkillEffectExtra_CheckMaxTrigger(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
}

void BattleSimulator::doSkillEffectExtra_HpLostP(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    
    float maxCount = 0;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_trigger_count_max)
            maxCount = efv[i];
        break;
    }
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_hp_lostp)
        {
            if (skill->effect->atkreduce || skill->effect->magatkreduce)
            {
                if (bf->updateHPPAttackReduce(skill->effect->atkreduce, efv[i], maxCount))
                    appendDefStatus(e_skill, skill->getId(), bf);
            }
            if (skill->effect->crrdam || skill->effect->crrdamP)
            {
                if (bf->updateHPPAttackAdd(skill->effect->crrdamP, efv[i], maxCount))
                    appendDefStatus(e_skill, skill->getId(), bf);
            }
            if (skill->effect->hppec)
            {
                UInt32 hpr = bf->regenHP(0, false, skill->effect->hppec, skill->effect->maxhpdampec);
                if(hpr != 0)
                    appendDefStatus(e_damHpAdd, hpr, bf);
            }
        }
    }
}

void BattleSimulator::doSkillEffectExtra_Flaw(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_flaw)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo == NULL || bo->getHP() == 0)
                continue;
            bo->addFlaw(efv[i] * 100, efl[i]);
            appendDefStatus(e_flaw, bo->getFlawCount(), bo);
        }
    }
}

void BattleSimulator::doSkillEffectExtra_Withstand(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_withstand)
        {
            bf->setWithstand(efv[i]);
            bf->addWithstandCount();
        }
    }
}

void BattleSimulator::doSkillEffectExtra_Ru_RedCarpet(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
        return;
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    bo->setRuRedCarpet(efl[0]);
    appendDefStatus(e_ruRedCarpet, 0, bo);
}

void BattleSimulator::doSkillEffectExtra_Shi_Flower(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
        return;
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    bo->setShiFlower(efl[0], efv[0]);
    appendDefStatus(e_shiFlower, 0, bo);
}

void BattleSimulator::doSkillEffectExtra_Dao_Rose(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
        return;
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    bo->setDaoRose(efl[0]);
    appendDefStatus(e_daoRose, 0, bo);
}

void BattleSimulator::doSkillEffectExtra_Mo_Knot(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
        return;
    if(!skill || !skill->effect)
        return;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;
    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    bo->setMoKnot(efl[0]);
    appendDefStatus(e_moKnot, 0, bo);
}

void BattleSimulator::doSkillEffectExtra_Prudent(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!bf)
        return;
    if(!skill || !skill->effect)
        return;
    bf->setPrudentLast(4);
    bf->setPrudentHitrate(skill->effect->efv[0]);

    Int32 side = 1 - bf->getSide();
    for(UInt8 i = 0; i < 25; i++)
    {
        BattleObject* bo = getObject(side, i);
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        static_cast<BattleFighter*>(bo)->setPrudentHitrateLastOtherside(4);
    }
    appendDefStatus(e_skill, skill->getId(), bf);
}

bool BattleSimulator::doSkillEffectExtra_LingShiBleed(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill, UInt32 dmg)
{
    if(!skill || !skill->effect)
        return false;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efv.size() || cnt != efl.size())
        return false;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_lingshi_bleed)
        {
            if(!bo)
            {
                AtkList atklist;
                getAtkList(bf, skill, atklist);
                UInt8 cnt = atklist.size();
                for(size_t j = 0; j < cnt; ++ j)
                {
                    BattleFighter* bo = atklist[j].bf;
                    float factor = atklist[j].factor;
                    bo->setLingShiBleed(efv[i]*factor*dmg, efl[i]);
                    appendDefStatus(e_lingShiBleed, 0, bo);
                }
            }
            else
            {
                bo->setLingShiBleed(efv[i]*dmg, efl[i]);
                appendDefStatus(e_lingShiBleed, 0, bo);
            }
            return true;
        }
    }

    return false;
}

void BattleSimulator::doSkillEffectExtra_ChaosWorld(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    if(!skill || !skill->effect)
        return;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efv.size() || cnt != efl.size())
        return;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_chaos_world)
        {
            if(bf)
            {
                if (((bf->getHPP() * 100) <= efv[i]) && !bf->getChaosWorldId())
                {
                    bf->setChaosWorld(skill->getId());
                    appendDefStatus(e_chaosWorld, 0, bf);
                }
                else if (bf->getChaosWorldId())
                {
                    bf->setChaosWorld(0);
                    appendDefStatus(e_unChaosWorld, 0, bf);
                }
            }
            return;
        }
    }

    return;
}

void BattleSimulator::doPassiveSkillBePHYDmg(BattleFighter* bf, BattleFighter* bo, UInt32 dmg)
{
    if(bf == NULL || bo == NULL)
        return;

    if(bo->getHP() == 0 || bf->getHP() == 0 || bf->getSide() == bo->getSide())
        return;

    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    while(NULL != (passiveSkill = bo->getPassiveSkillOnBePHYDmg100(idx)))
    {
        if(passiveSkill->effect == NULL)
            continue;
        break;
    }

    if(!passiveSkill)
        passiveSkill = bo->getPassiveSkillOnBePHYDmg();

    if(NULL == passiveSkill || passiveSkill->effect == NULL || (passiveSkill->effect->thorn && passiveSkill->effect->thornP))
        return;

    appendDefStatus(e_skill, passiveSkill->getId(), bo);
    float atk = dmg * passiveSkill->effect->thornP + passiveSkill->effect->thorn;
    float def = getBFDefend(bf);
    float atkreduce = getBFAtkReduce(bf);
    UInt32 dmg2 = _formula->calcDamage(atk, def, bo->getLevel(), 1, atkreduce);
    dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;
    dmg2 = dmg2 > 0 ? dmg2 : 1;
    makeDamage(bf, dmg2, e_damNormal, e_damagePhysic);
}

void BattleSimulator::doPassiveSkillBeMagDmg(BattleFighter* bf, BattleFighter* bo, UInt32 dmg)
{
    if(bf == NULL || bo == NULL)
        return;

    if(bo->getHP() == 0 || bf->getHP() == 0 || bf->getSide() == bo->getSide())
        return;

    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    while(NULL != (passiveSkill = bo->getPassiveSkillOnBeMagDmg100(idx)))
    {
        if(passiveSkill->effect == NULL)
            continue;
        break;
    }

    if(!passiveSkill)
        passiveSkill = bo->getPassiveSkillOnBeMagDmg();

    if(NULL == passiveSkill || passiveSkill->effect == NULL || (passiveSkill->effect->thorn && passiveSkill->effect->thornP))
        return;

    appendDefStatus(e_skill, passiveSkill->getId(), bo);
    float atk = dmg * passiveSkill->effect->thornP + passiveSkill->effect->thorn;
    float def = getBFMagDefend(bf);
    float atkreduce = getBFMagAtkReduce(bf);
    UInt32 dmg2 = _formula->calcDamage(atk, def, bo->getLevel(), 1, atkreduce);
    dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;
    dmg2 = dmg2 > 0 ? dmg2 : 1;
    makeDamage(bf, dmg2, e_damNormal, e_damageMagic);
}

void BattleSimulator::doPassiveSkillBegDmg(BattleFighter* bf, BattleFighter* bo, UInt32 dmg)
{
    if(bf == NULL || bo == NULL)
        return;

    if(bo->getHP() == 0 || bf->getHP() == 0 || bf->getSide() == bo->getSide())
        return;

    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    while(NULL != (passiveSkill = bo->getPassiveSkillOnBeDmg100(idx)))
    {
        if(passiveSkill->effect == NULL)
            continue;
        break;
    }

    if(!passiveSkill)
        passiveSkill = bo->getPassiveSkillOnBeDmg();

    if(NULL == passiveSkill || passiveSkill->effect == NULL)
        return;

    appendDefStatus(e_skill, passiveSkill->getId(), bo);
    doSkillEffectExtra_LingShiBleed(bo, bf, passiveSkill, dmg);
}

float BattleSimulator::getBFDefend(BattleFighter* bf)
{
    float def = bf->getDefend();
    /*
    int side = bf->getSide();
    BattleFighter* pet = static_cast<BattleFighter*>(getObject(side, _backupTargetPos[side]));
    if(pet && pet->getHP() != 0 && bf != pet)
        def *= (1 + pet->getLingYouDef());
        */

    return def;
}

float BattleSimulator::getBFMagDefend(BattleFighter* bf)
{
    float def = bf->getMagDefend();
    /*
    int side = bf->getSide();
    BattleFighter* pet = static_cast<BattleFighter*>(getObject(side, _backupTargetPos[side]));
    if(pet && pet->getHP() != 0 && bf != pet)
        def *= (1 + pet->getLingYouMagDef());
        */

    return def;
}

float BattleSimulator::getBFAtkReduce(BattleFighter* bf)
{
    float def = bf->getAtkReduce();
    int side = bf->getSide();
    BattleFighter* pet = static_cast<BattleFighter*>(getObject(side, _backupTargetPos[side]));
    if(pet && pet->getHP() != 0 && bf != pet)
        def += pet->getLingYouDef() * 100;

    return def;
}

float BattleSimulator::getBFMagAtkReduce(BattleFighter* bf)
{
    float def = bf->getMagAtkReduce();
    int side = bf->getSide();
    BattleFighter* pet = static_cast<BattleFighter*>(getObject(side, _backupTargetPos[side]));
    if(pet && pet->getHP() != 0 && bf != pet)
        def += pet->getLingYouMagDef() * 100;

    return def;
}

float BattleSimulator::getBFAttack(BattleFighter* bf)
{
    float atk = bf->getAttack();
    int side = bf->getSide();
    BattleFighter* pet = static_cast<BattleFighter*>(getObject(side, _backupTargetPos[side]));
    if(pet && pet->getHP() != 0 && bf != pet)
        atk *= (1 + pet->getLingYouAtk());

    return atk;
}

float BattleSimulator::getBFMagAtk(BattleFighter* bf)
{
    float atk = bf->getMagAttack();
    int side = bf->getSide();
    BattleFighter* pet = static_cast<BattleFighter*>(getObject(side, _backupTargetPos[side]));
    if(pet && pet->getHP() != 0 && bf != pet)
        atk *= (1 + pet->getLingYouMagAtk());

    return atk;
}

void BattleSimulator::calcSkillAttack(BattleFighter* bf, bool& isCritical, BattleFighter* defender, float& atk, float& magatk, float* pCf)
{
    float rate = bf->getCritical(defender);
    isCritical = uRand(10000) < (rate > 0 ? rate : 0) * 100;

    magatk = getBFMagAtk(bf);
	atk = getBFAttack(bf);

    float factor = bf->getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(bf);
    if(factor < 1.25)
        factor = 1.25;

    if(isCritical)
    {
        magatk = magatk * factor;
		atk = atk * factor;
    }

    if(pCf)
        *pCf = factor;

}

float BattleSimulator::calcAttack(BattleFighter* bf, bool& isCritical, BattleFighter* defender, float* pCf)
{
    float rate = bf->getCritical(defender);
    isCritical = uRand(10000) < (rate > 0 ? rate : 0) * 100;

	float atk = getBFAttack(bf);
    float factor = bf->getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(bf);
    if(factor < 1.25)
        factor = 1.25;

	if(isCritical)
	{
		atk = atk * factor;
	}

    if(pCf)
        *pCf = factor;
	return atk;
}

float BattleSimulator::calcMagAttack(BattleFighter* bf, bool& isCritical, BattleFighter* defender , float* pCf)
{
    float rate = bf->getCritical(defender);
    isCritical = uRand(10000) < (rate > 0 ? rate : 0) * 100;
    float magatk = getBFMagAtk(bf);

    float factor = bf->getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(bf);
    if(factor < 1.25)
        factor = 1.25;

    if(isCritical)
    {
        magatk = magatk * factor;
    }

    if(pCf)
        *pCf = factor;
    return magatk;
}

float BattleSimulator::calcTherapy(BattleFighter* bf, bool& isCritical, bool& first, const GData::SkillBase* skill)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    float aura_factor = 1;
    if(skill->cond == GData::SKILL_PEERLESS)
        aura_factor = bf->getAura() / 100;

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
        ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_CRITICAL);
    if(ef)
    {
        if(uRand(10000) < bf->getCritical(NULL)* 100)
        {
            float f = (bf->getCriticalDmg()-1)*ef->value/100 + 1;
            aura_factor *= f;
            if(first)
            {
                isCritical = true;
                first = false;
            }
        }
    }

    Int32 sg_v = bf->getSkillGradeExtraValue(SKILL_ID(skill->getId()));
    GData::LBSkillItem* item = bf->getSkillCondItem(SKILL_ID(skill->getId()));
    if(NULL != item)
        return aura_factor * (getBFMagAtk(bf) * skill->effect->hpP + skill->effect->addhp + skill->effect->hp + item->ef_value + sg_v);

    return aura_factor * (getBFMagAtk(bf) * skill->effect->hpP + skill->effect->addhp + skill->effect->hp + sg_v);
}

float BattleSimulator::calcMaxTherapy(BattleFighter* bf, const GData::SkillBase* skill)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    return getBFMagAtk(bf) * skill->effect->maxhpdampec;
}

float BattleSimulator::calcPoison(BattleFighter* bf, const GData::SkillBase* skill, BattleFighter* defender, bool cs)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    float magatk = getBFMagAtk(bf);
    float atk = getBFAttack(bf);

    float factor = bf->getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(bf);
    if(factor < 1.25)
        factor = 1.25;

    if(!cs)
    {
        factor = 1.25;
    }

    if(bf->getClass() == GObject::e_cls_dao || bf->getClass() == GObject::e_cls_mo)
    {
        return (atk * skill->effect->hpP + skill->effect->hp + skill->effect->addhp) * factor * (950 + uRand(100)) / 1000;
    }

    return (magatk * skill->effect->hpP + skill->effect->hp + skill->effect->addhp) * factor *  (950 + uRand(100)) / 1000;
}

bool BattleSimulator::doSkillDmg(BattleFighter* bf, const GData::SkillBase* skill)
{
   AtkList atklist;
   getAtkList(bf, skill, atklist);
   UInt8 cnt = atklist.size();
   float atk = getBFAttack(bf);
   atk = atk * skill->effect->damageP + skill->effect->adddam;
   float magatk = getBFMagAtk(bf);
   magatk = magatk * skill->effect->magdamP + skill->effect->addmag;
   if(atk == 0 && magatk == 0)
       return false;

   for(size_t i = 0; i < cnt; ++ i)
   {
       BattleFighter* bo = atklist[i].bf;
       if(bo->getSide() == bf->getSide())
           continue;

       float factor = atklist[i].factor;
       UInt32 dmg2 = 0;
       if(atk > 0)
       {
           atk *= factor;
           atk *= (1 - bo->getDecWaveDmg());
           float def = getBFDefend(bo);
           float atkreduce = getBFAtkReduce(bo);
           dmg2 = _formula->calcDamage(atk, def, bf->getLevel(), 1, atkreduce);
           dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;
           dmg2 = dmg2 > 0 ? dmg2 : 1;
           makeDamage(bo, dmg2, e_damNormal, e_damagePhysic);
       }
       else if(magatk > 0)
       {
           magatk *= factor;
           magatk *= (1 - bo->getDecWaveDmg());
           float magdef = getBFMagDefend(bo);
           float magatkreduce = getBFMagAtkReduce(bo);
           dmg2 = _formula->calcDamage(magatk, magdef, bf->getLevel(), 1, magatkreduce);
           dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;
           dmg2 = dmg2 > 0 ? dmg2 : 1;
           makeDamage(bo, dmg2, e_damNormal, e_damageMagic);
       }
   }

   return true;
}

void BattleSimulator::calcAbnormalTypeCnt(BattleObject* bo)
{
    Int32 target_side = 1 - bo->getSide();
    for(UInt8 i = 0; i < 25; i++)
    {
        bo = getObject(target_side, i);
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        size_t idx = 0;
        bool flag = false;
        const GData::SkillBase* passiveSkill = NULL;
        if(NULL != (passiveSkill = static_cast<BattleFighter* >(bo)->getAbnormalTypeSkill()))
        {
            flag = true;
        }
        else if(NULL != (passiveSkill = static_cast<BattleFighter* >(bo)->getPassiveSkillAbnormalTypeDmg100(idx)))
        {
            static_cast<BattleFighter* >(bo)->setAbnormalTypeSkill(passiveSkill);
            flag = true;
        }

        if(flag)
        {
            bool bRet = static_cast<BattleFighter* >(bo)->addAbnormalTypeCnt();
            if(bRet)
            {
                appendDefStatus(e_skill, passiveSkill->getId(), static_cast<BattleFighter* >(bo));
                appendDefStatus(e_abnormalType, static_cast<BattleFighter* >(bo)->getAbnormalTypeCnt(), static_cast<BattleFighter* >(bo));
            }
        }
    }
}

void BattleSimulator::calcBleedTypeCnt(BattleObject* bo)
{
    Int32 target_side = 1 - bo->getSide();
    for(UInt8 i = 0; i < 25; i++)
    {
        bo = getObject(target_side, i);
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        bool flag = false;
        const GData::SkillBase* passiveSkill = NULL;
        if(NULL != (passiveSkill = static_cast<BattleFighter* >(bo)->getBleedTypeSkill()))
        {
            flag = true;
        }
        else if(NULL != (passiveSkill = static_cast<BattleFighter* >(bo)->getPassiveSkillBleedTypeDmg()))
        {
            static_cast<BattleFighter* >(bo)->setBleedTypeSkill(passiveSkill);
            flag = true;
        }

        if(flag)
        {
            bool bRet = static_cast<BattleFighter* >(bo)->addBleedTypeCnt();
            if(bRet)
            {
                appendDefStatus(e_skill, passiveSkill->getId(), static_cast<BattleFighter* >(bo));
                appendDefStatus(e_bleedType, static_cast<BattleFighter* >(bo)->getBleedTypeCnt(), static_cast<BattleFighter* >(bo));
            }
        }
    }
}

void BattleSimulator::initBuddhaLight(BattleFighter* bf, bool auralAdd, bool dead)
{
    for(UInt8 i = 0; i < 25; i++)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(bf->getSide(), i));
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        if(dead && bf != bo->getBuddhaLightLauncher())
            return;
        bo->setBuddhaLight(0, 0);
        if(bo == bo->getBuddhaLightLauncher())
            appendDefStatus(e_unBuddhaLight, 0, bo);
        bo->setBuddhaLightLauncher(NULL);
        if(auralAdd && bo->getBuddhaLightValue() > 0.001)
            setStatusChange_Aura2(bo, bo->getSide(), bo->getPos(), NULL, bo->getBuddhaLightValue(), 0, false);
        bo->setBuddhaLightValue(0);
    }
}

void BattleSimulator::doSkillAttackByCareer(BattleFighter *bf, const GData::SkillBase *skill)
{
    if(!skill)
        return;
    if(!skill->effect)
        return;

    const std::vector<float>& efv = skill->effect->efv;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<UInt8>& efl = skill->effect->efl;

    size_t cnt = eft.size();
    if(cnt != efl.size() || efv.size() != cnt)
        return;
    appendDefStatus(e_skill, skill->getId(), bf);
    bool first = true;
    bool cs = false;
    bool pr = false;

    for(size_t i = 0; i < cnt; ++ i)
    {
        AtkList atklist;
        getAtkList(bf, skill, atklist);
        UInt8 cnt2 = atklist.size();
        for(size_t j = 0; j < cnt2; ++ j)
        {
            BattleFighter* target = static_cast<BattleFighter *>(atklist[j].bf);
            UInt8 target_stun = target->getStunRound();
            bool enterEvade = target->getEvad100();
            bool defend100 = target->getDefend100();

            if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(target, NULL))))
            {
                float dmg;
                UInt32 curDmg;
                bool isPhysic;
                if(target->hasFlag(BattleFighter::IsMirror))
                {
                    curDmg = target->getHP();
                    dmg = curDmg;
                    if(bf->getClass() == GObject::e_cls_dao || bf->getClass() == GObject::e_cls_mo)
                        isPhysic = true;
                    else
                        isPhysic = false;
                }
                else
                {
                    bool pr2 = bf->calcPierce(target);
                    float cf = 0.0f;
                    bool cs2 = false;
                    if(cs2)
                    {
                        UInt8 s = bf->getSide();
                        if(s < 2)
                            _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;
                    }
                    if(first)
                    {
                        cs = cs2;
                        pr = pr2;
                        first = false;
                    }

                    float atk = 0;
                    float def = 0;
                    float reduce = 0;
                    if(bf->getClass() == GObject::e_cls_dao || bf->getClass() == GObject::e_cls_mo)
                    {
                        atk = skill->effect->crrdamP * calcAttack(bf, cs2, target, NULL);
                        def = getBFDefend(target);
                        reduce = getBFAtkReduce(target);
                        isPhysic = true;
                    }
                    else
                    {
                        atk = skill->effect->crrdamP * calcMagAttack(bf, cs2, target, NULL);
                        def = getBFMagDefend(target);
                        reduce = getBFMagAtkReduce(target);
                        isPhysic = false;
                    }

                    float toughFactor = pr2 ? target->getTough(bf) : 1.0f;
                    float factor = atklist[j].factor;
                    dmg = _formula->calcDamage(atk * factor, def, bf->getLevel(), toughFactor, reduce);
                    dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
                    dmg = dmg > 0 ? dmg : 1;

                    curDmg = dmg;
                    doShieldHPAttack(target, curDmg);
                }
                if(curDmg > 0)
                {
                    makeDamage(target, curDmg, e_damNormal, isPhysic ? e_damagePhysic : e_damageMagic);
                }
            }
            else if(!defend100 && !enterEvade)
            {
                appendDefStatus(e_damEvade, 0, target);
                if(target->getSneakStatus() == e_sneak_on)
                {
                    _sneak_atker.push_back(target);
                    target->nextSneakStatus();
                }
            }
            else
            {
                if(!defend100)
                {
                    appendDefStatus(e_damEvade, 0, target);
                    target->setEvad100(false);
                    if(target->getSneakStatus() == e_sneak_on)
                    {
                        _sneak_atker.push_back(target);
                        target->nextSneakStatus();
                    }
                }
                else
                {
                    appendDefStatus(e_damOut, 0, target);
                    target->setDefend100(false);
                }
            }
        }
        return;
    }
}

void BattleSimulator::addSelfSideEvadeCnt(BattleFighter* bf)
{
    for(UInt8 i = 0; i < 25; i++)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(bf->getSide(), i));
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        const GData::SkillBase* skill = bo->getBiLanTianYiSkill();
        if(!skill)
            continue;
        bo->addEvadeCnt(1);
        appendDefStatus(e_skill, skill->getId(), bo);
        appendDefStatus(e_biLanTianYi, bo->getEvadeCnt(), bo);
    }
}

UInt32 BattleSimulator::doBufMakeDamage(BattleFighter* bf, UInt32& u)
{
    UInt32 uShow = u;
    if(!bf)
        return uShow;
    if(u > 0)
    {
        UInt8 count = bf->getSoulProtectCount();
        if(count > 0)
        {
            u /= 10;
            uShow = u;
        }
    }
    return uShow;
}

void BattleSimulator::tryParry( BattleFighter* bf, UInt32& dmg, UInt32& magDmg, BattleFighter* bfAttacker)
{
    // 格挡
    if (bf->getWithstandCount() >= 3)
        return;

    const GData::SkillBase* passiveSkill = NULL;
    float bfAtk = bf->getAttack();
    float bfMagAtk = bf->getMagAttack();
    float dmgReduce = 0.0f;
    size_t idx = 0;
    while(NULL != (passiveSkill = bf->getPassiveSkillOnWithstand100(idx)))
    {
        if(passiveSkill->effect == NULL)
            continue;
        appendDefStatus(e_skill, passiveSkill->getId(), bf);
        doSkillEffectExtraAttack(bf, bfAttacker->getSide(), bfAttacker->getPos(), passiveSkill);
        float factor = bf->getWithstandFactor();
        dmgReduce = (bfAtk + bfMagAtk) * factor;
    }

    passiveSkill = bf->getPassiveSkillOnWithstand();
    if (passiveSkill)
    {
        if(passiveSkill->effect)
        {
            appendDefStatus(e_skill, passiveSkill->getId(), bf);
            doSkillEffectExtraAttack(bf, bfAttacker->getSide(), bfAttacker->getPos(), passiveSkill);
            float factor = bf->getWithstandFactor();
            dmgReduce = (bfAtk + bfMagAtk) * factor;
        }
    }
    if (dmgReduce > 0.1f)
    {
        UInt32 reduce = static_cast<UInt32>(dmgReduce);
        if (dmg < reduce)
        {
            reduce -= dmg;
            if (dmg)
                dmg = 1;
        }
        else
        {
            dmg -= reduce;
            reduce = 0;
        }
        if (magDmg < reduce)
        {
            reduce -= magDmg;
            if (magDmg)
                magDmg = 1;
        }
        else
        {
            magDmg -= reduce;
            reduce = 0;
        }
    }
    return ;
}

void BattleSimulator::calcSilkwormCnt(BattleObject* bo)
{
    if(!bo)
        return;
    Int32 target_side = 1 - bo->getSide();
    for(UInt8 i = 0; i < 25; i++)
    {
        bo = getObject(target_side, i);
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        static_cast<BattleFighter* >(bo)->addSilkwormCnt(1);
    }
}

void BattleSimulator::doAbnormalStatusClear(BattleObject* bo)
{
    if(!bo)
        return;
    BattleFighter* bf = static_cast<BattleFighter *>(bo);
    if(bf->getDeepStunLast())
    {
        bf->setDeepStunDmgExtra(0, 0);
        appendDefStatus(e_unDeepStun, 0, bf);
    }
    if(bf->getStunRound() != 0)
    {
        bf->setStunLevel(0);
        bf->setStunRound(0);
        appendDefStatus(e_UnStun, 0, bf);
    }
    if(bf->getDeepForgetLast() != 0)
    {
        bf->setDeepForgetDmgExtra(0, 0);
        appendDefStatus(e_unDeepForget, 0, bf);
    }
    if(bf->getForgetRound() != 0)
    {
        bf->setForgetLevel(0);
        bf->setForgetRound(0);
        appendDefStatus(e_UnForget, 0, bf);
    }
    if(bf->getDeepConfuseLast() != 0)
    {
        bf->setDeepConfuseDmgExtra(0, 0);
        appendDefStatus(e_unDeepConfuse, 0, bf);
    }
    if(bf->getConfuseRound() != 0)
    {
        bf->setConfuseLevel(0);
        bf->setConfuseRound(0);
        appendDefStatus(e_UnConfuse, 0, bf);
    }
}

//九子神雷
UInt32 BattleSimulator::attackByJiuzi(BattleFighter * bf, bool& first, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * bo, float factor, int counter_deny, AttackPoint * counter_deny_list, std::vector<AttackAct>* atkAct, bool canProtect)
{
    UInt32 curDmg = attackOnce(bf, first, cs, pr, skill, bo, factor, counter_deny, counter_deny_list, atkAct, canProtect);
    if(curDmg > 0)
    {
        UInt8 cnt = static_cast<BattleFighter*>(bo)->getJiuziDmgCnt();
        static_cast<BattleFighter*>(bo)->setJiuziDmgCnt(++cnt);
    }
    return curDmg;
}

void BattleSimulator::attackByJiuziSS(BattleFighter* bf, const GData::SkillBase* skill)
{
    if(!bf || !skill)
        return;
    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
        ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_ADDSTATE);
    if(!ef)
        return;

    Int32 target_side = 1 - bf->getSide();
    for(UInt8 i = 0; i < 25; i++)
    {
        BattleObject* bo = getObject(target_side, i);
        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
            continue;
        BattleFighter* bf2 = static_cast<BattleFighter*>(bo);
        UInt8 count = bf2->getJiuziDmgCnt();
        if(count == 0)
            continue;
        UInt32 rate = (0.1 * 100 + ef->value * count) * 100;
        if(rate > _rnd(10000) && bf2->getConfuseRound() < 1)
        {
            bf2->setConfuseRound(1);
            appendDefStatus(e_Confuse, 0, bf2);
            calcAbnormalTypeCnt(bf2);
        }
        bf2->setJiuziDmgCnt(0);
    }

}

bool BattleSimulator::doEffectAfterCount(BattleFighter* bf, const GData::SkillBase* skill, UInt16 actCnt)
{
    bool bRet = false;
    if(!bf)
        return bRet;
    if(!skill)
        return bRet;
    const GData::SkillEffect* effect = skill->effect;
    if(!effect)
        return bRet;
    UInt16 condtion = effect->efv[0];
    if(condtion > 5)
        return bRet;

    bRet = true;
    if(condtion != actCnt + 1)
        return bRet;
    UInt16 last = effect->efl[0];
    if(last == 0)
        return bRet;

    appendDefStatus(e_skill, skill->getId(), bf);
    UInt8 target_side = bf->getSide();
    for(UInt8 i = 0; i < 25; i++)
    {
        BattleObject* bo2 = getObject(target_side, i);
        if(bo2 == NULL || bo2->getHP() == 0 || !bo2->isChar())
            continue;

        BattleFighter* bo = static_cast<BattleFighter *>(bo2);
        if(effect->auraP > 0.001 || effect->aura > 0)
        {
            float value = bo->_aura * effect->auraP + effect->aura;
            if(value > 0.001f)
                setStatusChange_Aura(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->atkP > 0.001f || effect->atk > 0)
        {
            float value = bo->_attack * effect->atkP + effect->atk;
            if(value > 0.001f)
                setStatusChange_Atk(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->defP > 0.001f || effect->def > 0)
        {
            float value = bo->_defend * effect->defP + effect->def;
            if(value > 0.001f)
                setStatusChange_Def(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->magatkP || effect->magatk > 0)
        {
            float value = bo->_magatk * effect->magatkP + effect->magatk;
            if(value > 0.001f)
                setStatusChange_MagAtk(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->magdefP || effect->magdef > 0)
        {
            float value = bo->_magdef * effect->magdefP + effect->magdef;
            if(value > 0.001f)
                setStatusChange_MagDef(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->tough > 0.001f)
        {
            float value = effect->tough;
            if(value > 0.001f)
                setStatusChange_Tough(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->actionP || effect->action > 0.001f)
        {
            float value = bo->_maxAction * effect->actionP + effect->action;
            if(value > 0.001f)
                setStatusChange_Action(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->hitrate > 0.001f)
        {
            float value = effect->hitrate;
            if(value > 0.001f)
                setStatusChange_HitR(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->evade > 0.001f)
        {
            float value = effect->evade;
            if(value > 0.001f)
                setStatusChange_Evade(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->critical > 0.001f)
        {
            float value = effect->critical;
            if(value > 0.001f)
                setStatusChange_Critical(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->pierce > 0.001f)
        {
            float value = effect->pierce;
            if(value > 0.001f)
                setStatusChange_Pierce(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->counter > 0.001f)
        {
            float value = effect->counter;
            if(value > 0.001f)
                setStatusChange_Counter(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->magres > 0.001f)
        {
            float value = effect->magres;
            if(value > 0.001f)
                setStatusChange_MagRes(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->atkreduce > 0.001f)
        {
            float value = effect->atkreduce;
            if(value > 0.001f)
                setStatusChange_AtkReduce(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else if(effect->magatkreduce > 0.001f)
        {
            float value = effect->magatkreduce;
            if(value > 0.001f)
                setStatusChange_MagAtkReduce(bf, bo->getSide(), bo->getPos(), skill, value, last, true);
        }
        else
        {
        }
    }

    return bRet;
}

UInt32 BattleSimulator::doLingshiModelAttack(BattleFighter* bf, UInt8 flag)
{
    if(!bf)
        return 0;
    if(bf->getChangeStatus() != 0)
        return 0;

    size_t idx = 0;
    const GData::SkillBase* passiveSkill = bf->getPassiveSkillEnterLingshi100(idx);
    if(!passiveSkill)
        return 0;

    //触发类型
    UInt8 type = 0;
    UInt8 last = 0;
    if(passiveSkill->effect)
    {
        UInt8 count1 = passiveSkill->effect->eft.size();
        UInt8 count2 = passiveSkill->effect->efv.size();
        UInt8 count3 = passiveSkill->effect->efl.size();
        if(count1 < 1)
            return 0;
        if(count1 != count2 || count1 != count3 || count2 != count3)
            return 0;
        if(passiveSkill->effect->eft[count1 - 1] != GData::e_eft_lingshi_enter)
            return 0;
        type = passiveSkill->effect->efv[count1 - 1];
        last = passiveSkill->effect->efl[count1 - 1];
    }
    if(flag != type)
        return 0;

    switch(type)
    {
        case 1:
            if(!bf->getHpLess())
                return 0;
            break;
        case 2:
            if(bf->getCounterCnt() < 3)
                return 0;
            break;
        case 3:
            if(_attackRound < 3)
                return 0;
            break;
        case 4:
            if(bf->getCriticalCnt() < 3)
                return 0;
            break;
        case 5:
            if(!bf->getPreAtk())
                return 0;
            break;
        case 6:
            if(bf->getFriendDeadCnt() < 3)
                return 0;
            break;
        case 7:
            if(bf->getEnemyDeadCnt() < 2)
                return 0;
            break;
        default:
            return 0;
            break;
    }
    bf->setChangeStatus(1);
    bf->setNewModeLast(last);

    //改变模型
    //if(passiveSkill->color == 5)
        appendDefStatus(e_changeMode, SKILL_ID(passiveSkill->getId()), bf);
    bf->updateAllPassiveSkillLingshiExceptEnter();
    idx = 0;
    if (bf->getPassiveSkillOnOtherConfuseAndForget100(idx))
        _onOtherConfuseAndForget.push_back(bf);

    Int32 target_side;
    Int32 target_pos;
    Int32 cnt = 0;
    getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
    cnt += doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
    return cnt;
}

void BattleSimulator::onDeadLingshi(BattleFighter* bf)
{
    if(!bf)
        return;
    if(bf->isSoulOut())
        return;
    UInt8 side = bf->getSide();
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(!bo || bo->getHP() == 0 || bo == bf)
            continue;
        bo->setFriendDeadCnt(bo->getFriendDeadCnt() + 1);
        doLingshiModelAttack(bo, 6);
    }

    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(1 - side, i));
        if(!bo || bo->getHP() == 0 || bo == bf)
            continue;
        bo->setEnemyDeadCnt(bo->getEnemyDeadCnt() + 1);
        doLingshiModelAttack(bo, 7);
    }

}

} // namespace Battle

