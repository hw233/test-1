#include "Config.h"
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

BattleSimulator::BattleSimulator(UInt32 location, GObject::Player * player, const std::string& name2, UInt8 level2, bool rpt, UInt32 fr): BattleField(), _id(rpt ? IDGenerator::gBattleOidGenerator.ID() : 0), _winner(0), _turns(0), _report(rpt), _fake_turns(fr), _formula(Script::BattleFormula::getCurrent())
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
        skillEffectExtraTable[GData::e_eft_random_target_attack] = &BattleSimulator::doSkillEffectExtra_RandomTargetAttack;;
        skillEffectExtraTable[GData::e_eft_mark_pet] = &BattleSimulator::doSkillEffectExtra_MarkPet;
        skillEffectExtraTable[GData::e_eft_atk_pet_mark_aura] = &BattleSimulator::doSkillEffectExtra_AtkPetMarkAura;
        skillEffectExtraTable[GData::e_eft_atk_pet_mark_extra_dmg] = &BattleSimulator::doSkillEffectExtra_AtkPetMarkDmg;
        skillEffectExtraTable[GData::e_eft_protect_pet_100] = &BattleSimulator::doSkillEffectExtra_ProtectPet100;
        skillEffectExtraTable[GData::e_eft_pet_atk_100] = &BattleSimulator::doSkillEffectExtra_PetAtk100;
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

        for(int i = 0; i < 25; ++ i)
        {
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
            static_cast<BattleFighter *>(_backupObjs[i])->initStats(checkEnh);

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
    size_t cnt_pos = _packet.size();
    _packet << static_cast<UInt32>(0);
    // ]]

    UInt32 act_count = 0;
    _winner = testWinner();
    act_count += FightersEnter(prevWin);

    UInt32 oldAttackRount = _attackRound;
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
        if (oldAttackRount != _attackRound)
        {
            oldAttackRount = _attackRound;
            act_count += tryPetEnter(0, e_reiatsu_round);
            act_count += tryPetEnter(1, e_reiatsu_round);
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
}
void BattleSimulator::insertFighterStatus( BattleFighter* bf )
{
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
        atk = bf->calcAttack(cs, area_target, &cf);
        if(cs )
        {
            UInt8 s = bf->getSide();
            if(s < 2)
                _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

        }

        float def;
        float toughFactor = pr ? area_target->getTough(bf) : 1.0f;
        def = area_target->getDefend();
        float atkreduce = area_target->getAtkReduce();
        dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);

        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        dmg = dmg > 0 ? dmg : 1;

        makeDamage(area_target, dmg);
        appendDefStatus(e_damNormal, dmg, area_target, e_damagePhysic);
        // killed the target fighter
        if(area_target->getHP() == 0)
        {
            onDead(false, area_target);
        }
        else if(_winner == 0)
        {
            onDamage(area_target, true, NULL);
        }
    }
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
            if(bf->isHide() || area_target->isMarkMo())
                aura_factor += 0.1f;
        }

        if(!colorStock && !defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(area_target, skill) && !area_target->getMoEvade100())))
        {
            // 攻击命中，没有被闪避
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
                            factor += passiveSkill->effect->atkP;
                        }
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
                bf->calcSkillAttack(cs2, area_target, atk, magatk , & cf);
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
                //atk = bf->calcAttack(cs2, area_target);
                //magatk = bf->calcMagAttack(cs2, area_target);
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
            }
            else
            {
                float cf = 0.0f;
                atk = bf->calcAttack(cs2, area_target, &cf);
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

                if (bf->getSide() != area_target->getSide())
                    tryAttackWithPet(bf, atk, magatk, factor);

                if (canProtect)  
                {
                    tryProtectDamage(area_target, atk, magatk, factor);
                }

                if(magatk)
                {
                    magdef = area_target->getMagDefend();
                    float magatkreduce = area_target->getMagAtkReduce();
                    magdmg = _formula->calcDamage(factor * magatk, magdef, bf->getLevel(), toughFactor, magatkreduce);

                    magdmg *= static_cast<float>(950 + _rnd(100)) / 1000;

                    magdmg = magdmg > 0 ? magdmg : 1;
                    magdmgFlag = true;
                }

                if(atk)
                {
                    def = area_target->getDefend();
                    float atkreduce = area_target->getAtkReduce();
                    dmg = _formula->calcDamage(factor * atk, def, bf->getLevel(), toughFactor, atkreduce);

                    dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

                    dmg = dmg > 0 ? dmg : 1;
                    dmgFlag = true;
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

            UInt32 dmg3 = dmg + magdmg;
            doShieldHPAttack(area_target, dmg3);
            if(dmg3 > 0)
            {
                makeDamage(area_target, dmg3);
                if (magdmgFlag)
                    appendDefStatus(e_damNormal, magdmg, area_target, e_damageMagic);
                if (dmgFlag)
                    appendDefStatus(e_damNormal, dmg, area_target, e_damagePhysic);
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
                    makeDamage(area_target, exDmg);
                    appendDefStatus(e_damNormal, exDmg, area_target, e_damageTrue);
                }
            }

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

            if(area_target->getHP() == 0)
            {
                onDead((side == bf->getSide()), area_target);
            }
            else if(_winner == 0)
            {
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

                onDamage(area_target, !(side == bf->getSide()));

                if(NULL != skill && skill->effect != NULL && skill->effect->state & GData::e_state_poison)
                {
                    float rate = skill->prob * 100;
                    if((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000))
                    {
                        // poison
                        poison = true;
                    }
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
            }
            else
            {
                appendDefStatus(e_damOut, 0, area_target);
                if(!colorStock)
                    area_target->setDefend100(false);

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

            //printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
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

        if(rhp > 0)
        {
            bf->regenHP(rhp);
            appendDefStatus(e_damAbsorb, rhp, bf);
        }

        // 一次有效的物理攻击，减少上次吸收的物理伤害和被减掉的伤害
        if (dmg > 0)
        {
            if (bf->getAtkSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_Atk, 1);
            if (bf->getAtkDecSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_DecAtk, 1);
        }
        // 一次有效的法术攻击，减少法术特殊加成效果和被减掉的效果
        if (magdmg > 0)
        {
            if (bf->getMagAtkSpecialLast() > 0)
                ReduceSpecialAttrLast(bf, e_ss_MagAtk, 1);
            if (bf->getMagAtkDecSpecialLast() > 0 )
                ReduceSpecialAttrLast(bf, e_ss_DecMagAtk, 1);
        }

        // 中毒
        if(poison)
        {
            doPoisonAttack(bf, cs2, skill, area_target, factor, atkAct);
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
                    float atk = target_fighter->calcAttack(cs2, bf, &cf);
                    if(cs2)
                    {
                        UInt8 s = target_fighter->getSide();
                        if(s < 2)
                            _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

                    }
                    //float atk = target_fighter->getAttack();
                    float def = bf->getDefend();
                    bool pr2 = target_fighter->calcPierce(bf);
                    float toughFactor = pr2 ? bf->getTough(target_fighter) : 1.0f;
                    float atkreduce = bf->getAtkReduce();
                    float factor = 1 + bf->getDarkVigorFactor();
                    UInt32 dmg2 = _formula->calcDamage(atk * factor, def, target_fighter->getLevel(), toughFactor, atkreduce);

                    dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;

                    dmg2 = dmg2 > 0 ? dmg2 : 1;
                    UInt32 dmg3 = dmg2;

                    makeDamage(bf, dmg2);
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

                    _defList[0].damType2 |= 0x80;
                    if(cs2)
                        _defList[0].damType2 |= 0x40;
                    if(pr2)
                        _defList[0].damType2 |= 0x20;
                    _defList[0].counterDmg = dmg3;
                    // killed the fighter
                    if(bf->getHP() == 0)
                    {
                        onDead(true, bf);
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bf, false);
                    }
                    _defList[0].counterLeft = bf->getHP();
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
        float atk = bf->calcAttack(cs2, 0, NULL);
        dmg = static_cast<int>(factor * atk) * (950 + _rnd(100)) / 1000;
        makeDamage(static_cast<BattleFighter*>(area_target_obj), dmg);
        appendDefStatus(e_damNormal, dmg, static_cast<BattleFighter*>(area_target_obj), e_damagePhysic);
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
    if(!passiveSkill)
    {
        size_t idx = 0;
        while(!passiveSkill)
        {
            size_t oidx = idx;
            passiveSkill = bf->getPassiveSkillOnCounter100(idx);
            if (oidx == idx)
                break;
        }
    }
    if(!passiveSkill)
        return;

    appendDefStatus(e_skill, passiveSkill->getId(), bf);

    float value = getSkillEffectExtraCounterDarkVigor(passiveSkill);
    bo->setDarkVigor(value, passiveSkill->last);
    appendDefStatus(e_darkVigor, 0, bo);
}

UInt32 BattleSimulator::doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, std::vector<AttackAct>* atkAct)
{
    UInt32 dmg = 0;
    UInt32 dmg2 = abs(bf->calcPoison(skill, area_target, cs)) * factor;
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

    return dmg;
}

void BattleSimulator::doSkillAtk2(bool activeFlag, std::vector<AttackAct>* atkAct)
{
    if(atkAct == NULL)
        return;

    size_t actCnt = atkAct->size();
    for(size_t idx = 0; idx < actCnt; idx++)
    {
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

            UInt8 state[3] = {0};
            UInt8 idx = 0;

            Int16 nStateLast = boSkill->last;
            UInt32 effect_state = 0;
            if(SKILL_ID(skillParam) == 122)  // 元磁神雷的符文状态被反弹
            {
                UInt32 nparm = (*atkAct)[idx].param1;
                nStateLast = nparm & 0x0000ffff;
                effect_state = (nparm&0xffff0000) >> 16;
            }
            else
                effect_state = (*atkAct)[idx].param1;

            UInt16 immune = bo->getImmune();
            if((effect_state & immune) && SKILL_LEVEL(boSkill->getId()) <= bo->getImmuneLevel(effect_state))
            {
                appendDefStatus(e_Immune, 0, bo);
                continue;
            }

            switch(effect_state)
            {
            case GData::e_state_poison:
                {
                    UInt32 dmg = abs(bo->calcPoison(boSkill, bo, false));
                    UInt32 dmg3 = dmg*0.5;
                    makeDamage(bo, dmg3);
                    appendDefStatus(e_Poison, dmg3, bo, e_damagePoison);
                    if(bo->getHP() == 0)
                    {
                        if(onDead(!activeFlag, bo))
                            break;
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, activeFlag);
                    }

                    dmg3 = dmg;
                    makeDamage(bo, dmg3);
                    appendDefStatus(e_Poison, dmg3, bo, e_damagePoison);
                    if(bo->getHP() == 0)
                    {
                        if(onDead(!activeFlag, bo))
                            break;
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, activeFlag);
                    }

                    dmg3 = dmg*1.5;
                    makeDamage(bo, dmg3);
                    appendDefStatus(e_UnPoison, dmg3, bo, e_damagePoison);
                }
                break;
            case GData::e_state_confuse:
                if(bo->getConfuseRound() < 1)
                {
                    bo->setConfuseLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setConfuseRound(nStateLast + 1);
                    appendDefStatus(e_Confuse, 0, bo);
                }
                break;
            case GData::e_state_stun:
                if(bo->getStunRound() < 1)
                {
                    bo->setStunLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setStunRound(nStateLast + 1);
                    appendDefStatus(e_Stun, 0, bo);
                }
                break;
            case GData::e_state_forget:
                if(bo->getForgetRound() < 1)
                {
                    bo->setForgetLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setForgetRound(nStateLast + 1);
                    appendDefStatus(e_Forget, 0, bo);
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
                }
                break;
            }

            if(bo->getHP() == 0)
            {
                onDead(!activeFlag, bo);
            }
            else if((_winner == 0) && (state[idx] == 1))
            {
                onDamage(bo, activeFlag);
            }
            continue;
        }

        // 被攻击后的技能
        if(skill->cond == GData::SKILL_BEATKED)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));

            if(NULL == bo)
                continue;

            if(bf->getHP() > 0 && bo->getHP() > 0)
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
                    continue;
                }
                else if(fdmg > 0 && bf->getSide() != target_side)
                {
                    UInt16 immune = bo->getImmune();
                    if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel(GData::e_state_dmgback))
                    {
                        appendDefStatus(e_Immune, 0, bo);
                    }
                    else
                    {
                        makeDamage(bo, fdmg);
                        appendDefStatus(e_damBack, fdmg, bo, e_damageTrue);
                    }

                    if(bo->getHP() == 0)
                    {
                        onDead(!activeFlag, bo);
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, activeFlag);
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
            UInt32 rhp = bf->calcTherapy(cs, first, skill);
            if(bf->getSide() != target_side)
            {
                UInt32 hpr = bf->regenHP(rhp);
                if(hpr != 0)
                    appendDefStatus(e_damHpAdd, hpr, bf);
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

                    float factor = 1;
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        factor = skill->factor[idx];

                    UInt32 hpr = bo->regenHP(rhp * factor);
                    ++i;
                    if(hpr == 0)
                        continue;

                    appendDefStatus(e_damHpAdd, hpr, bo);
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp);
                if(hpr != 0)
                    appendDefStatus(e_damHpAdd, hpr, bo);
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
                        appendDefStatus(e_damHpAdd, hpr, bo);
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(NULL == bo)
                        continue;

                    UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp * ap[i].factor, skill->cond == GData::SKILL_ACTIVE);
                    if(hpr != 0)
                        appendDefStatus(e_damHpAdd, hpr, bo);
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

                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                        int idx = std::min(abs(fsize-1), i);
                        if(fsize > 0)
                            dmg *= skill->factor[idx];
                        ++i;
                        makeDamage(bo, dmg);
                        appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                        if(bo->getHP() == 0)
                        {
                            onDead(!activeFlag, bo);
                        }
                        else if(_winner == 0)
                        {
                            onDamage(bo, activeFlag);
                        }
                    }
                }
                else if(0 == skill->area)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                        makeDamage(bo, dmg);
                        appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                        if(bo->getHP() == 0)
                        {
                            onDead(activeFlag, bo);
                        }
                        else if(_winner == 0)
                        {
                            onDamage(bo, !activeFlag);
                        }
                    }
                }
                else
                {
                    int fsize = skill->factor.size();
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                        if(fsize > 0)
                            dmg *= skill->factor[0];
                        makeDamage(bo, dmg);
                        appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                        if(bo->getHP() == 0)
                        {
                            onDead(activeFlag, bo);
                        }
                        else if(_winner == 0)
                        {
                            onDamage(bo, !activeFlag);
                        }
                    }

                    for(int i = 0; i < apcnt; ++ i)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false)) * ap[i].factor;
                        makeDamage(bo, dmg);
                        appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                        if(bo->getHP() == 0)
                        {
                            onDead(activeFlag, bo);
                        }
                        else if(_winner == 0)
                        {
                            onDamage(bo, !activeFlag);
                        }
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
                makeDamage(target_bo, dmg);
                if(poisonTimes == 3)
                    appendDefStatus(e_UnPoison, dmg, target_bo, e_damagePoison);
                else
                    appendDefStatus(e_Poison, dmg, target_bo, e_damagePoison);
            }

            if(target_bo->getHP() == 0)
            {
                onDead(false, target_bo);
            }
            else if(_winner == 0)
            {
                onDamage(target_bo, true);
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
            }
            else if(target_bo->getConfuseRound() < 1)
            {
                target_bo->setConfuseLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setConfuseRound(skill->last + 1);
                else
                    target_bo->setConfuseRound(skill->last);

                appendDefStatus(e_Confuse, 0, target_bo);
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
            }
            else if(target_bo->getStunRound() < 1)
            {
                target_bo->setStunLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setStunRound(skill->last + 1);
                else
                    target_bo->setStunRound(skill->last);

                appendDefStatus(e_Stun, 0, target_bo);
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
            }
            else if(target_bo->getForgetRound() < 1)
            {
                target_bo->setForgetLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setForgetRound(skill->last + 1);
                else
                    target_bo->setForgetRound(skill->last);
                appendDefStatus(e_Forget, 0, target_bo);
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
    if(bf == NULL || bf->getHP() == 0 || target_pos < 0)
        return false;

    BattleObject* target_object = getObject(otherside, target_pos);
    if(target_object!= NULL)
    {
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

                if(bo->getId() == 5679)
                {
                    excepts[exceptCnt] = i;
                    ++ exceptCnt;
                }
            }
        }

        BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
        if(NULL == bo)
        {
            target_pos = bf->getPos();
            target_side = bf->getSide();
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

            if(bo->getId() == 5679)
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

            if(bo->getId() == 5679)
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

            if(bo->getId() == 5679)
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

            if(bo->getId() == 5679)
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

            if(bo->getId() == 5679)
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
    else
    {
        target_pos = bf->getPos();
        target_side = bf->getSide();
        cnt = 1;
    }
}

bool BattleSimulator::doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct, UInt32 skillParam, UInt8* launchPeerLess /* = NULL */, bool canProtect /* = false */)
{
    if(NULL == skill || target_pos < 0 || bf == NULL)
        return false;
    if(skill->effect == NULL)
        return false;

    if(bf->getHP() == 0)
    {
        return false;
    }

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

        int nChangeAuraNum = -1*bf->getAura() + bf->getAuraLeft(); // 因为天赋术，hero无双之后会留一点灵力
        //setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, nChangeAuraNum, 0, false);
        setStatusChange_Aura2(bf, bf->getSide(), bf->getPos(), NULL, nChangeAuraNum, 0, false);

        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false);

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
        UInt32 rhp = bf->calcTherapy(cs, first, skill);
        UInt32 maxRhp = bf->calcMaxTherapy(skill);
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
                    }
                }

                bo = getTherapyTarget2(bf, excepts, exceptCnt);

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
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bo);
            }
        }
        else if(0 == skill->area)
        {
            // 暂时假定只有单体技能能按照百分比回血
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                return false;

            float deFactor = calcTherapyFactor(bo);
            UInt32 hpr = bo->regenHP(rhp * deFactor, skill->cond == GData::SKILL_ACTIVE, skill->effect->hppec, maxRhp);
            if(hpr != 0)
            {
                hpr_first = hpr;
                appendDefStatus(e_damHpAdd, hpr, bo);
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bo);
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
                float deFactor = calcTherapyFactor(bo);
                UInt32 hpr = bo->regenHP(rhp * factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                if(hpr != 0)
                {
                    hpr_first = hpr;
                    appendDefStatus(e_damHpAdd, hpr, bo);
                    if(skill->cond == GData::SKILL_ACTIVE)
                        releaseWeak(bo);
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
                if(bo->getSide() == bf->getSide())
                {
                    const GData::SkillBase* pskill = bo->getPassiveSkillOnTherapy();
                    if(!pskill)
                        continue;
                    UInt32 hpr = bo->regenHP(hpr_first * pskill->effect->hpP, false);
                    if(hpr != 0)
                    {
                        appendDefStatus(e_skill, pskill->getId(), bo);
                        appendDefStatus(e_damHpAdd, hpr, bo);
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

                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        dmg *= skill->factor[idx];
                    ++i;
                    makeDamage(bo, dmg);
                    appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                    if(bo->getHP() == 0)
                    {
                        onDead(false, bo);
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, true);
                    }
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                    makeDamage(bo, dmg);
                    appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                    if(bo->getHP() == 0)
                    {
                        onDead(false, bo);
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, true);
                    }
                }
            }
            else
            {
                int fsize = skill->factor.size();
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                    if(fsize > 0)
                        dmg *= skill->factor[0];
                    makeDamage(bo, dmg);
                    appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                    if(bo->getHP() == 0)
                    {
                        onDead(false, bo);
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, true);
                    }
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false)) * ap[i].factor;
                    makeDamage(bo, dmg);
                    appendDefStatus(e_damNormal, dmg, bo, e_damagePoison);

                    if(bo->getHP() == 0)
                    {
                        onDead(false, bo);
                    }
                    else if(_winner == 0)
                    {
                        onDamage(bo, true);
                    }
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
        //儒、元磁神雷
        if(SKILL_ID(skill->getId()) == 122 || SKILL_ID(skill->getId()) == 477)
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
            if(cnt+1 >= 3) // 三次以上有特殊效果
            {
                BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
                AddYuanCiState_SkillStrengthen(bf, bo, skill, cnt+1);
            }
        }
        //道、万剑诀
        else if(SKILL_ID(skill->getId()) == 27)
        {
            UInt8 dmgCount[25] = {0};
            int cnt = 8;
            for(int i = 0; i < cnt; ++ i)
            {
                BattleFighter* rnd_bf = getRandomFighter(target_side, NULL, 0);
                dmg += attackOnce(bf, first, cs, pr, skill, rnd_bf, 1);
                if(rnd_bf)
                    ++ dmgCount[rnd_bf->getPos()];
            }
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), 1, 0, NULL, atkAct, canProtect);
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
        else if(SKILL_ID(skill->getId()) == 53)
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
                    bo->setConfuseRound(1);
                    appendDefStatus(e_Confuse, 0, bo);
                }
                dmg += tmpDmg;
            }

        }
        //仙宠、两次单体攻击，两次伤害则眩晕 (天崩地裂)
        else if(SKILL_ID(skill->getId()) == 48)
        {
            UInt32 dmg1 = 0;
            UInt32 dmg2 = 0;
            BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            dmg1 = attackOnce(bf, first, cs, pr, skill, bo, 1, 0, NULL, atkAct, canProtect);
            dmg += dmg1;
            if (bf->getHP() > 0)
            {
                dmg2 = attackOnce(bf, first, cs, pr, skill, bo, 1, -1, NULL, atkAct, canProtect);
                dmg += dmg2;
                if (dmg1 && dmg2)
                {
                    bo->setStunRound(1);
                    appendDefStatus(e_Stun, 0, bo);
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
                float factor = 1;
                UInt32 dmg2 = 0;
                first = true;
                bool cs2 = false;
                bool pr2 = false;
                if(idx < skill->factor.size())
                    factor = skill->factor[idx];
                int count_deny = 0;
                if (skill->cond == GData::SKILL_ENTER)
                    count_deny = -1;
                if(idx == 0)
                {
                    dmg2 = attackOnce(bf, first, cs2, pr2, skill, getObject(target_side, target_pos), factor, count_deny, NULL, atkAct, canProtect);
                    cs = cs2;
                    pr = pr2;
                }
                else
                    dmg2 = attackOnce(bf, first, cs2, pr2, skill, getObject(target_side, target_pos), factor, count_deny, NULL, NULL, canProtect);

                dmg += dmg2;
                doSkillEffectExtraAbsorb(bf, dmg2, skill);
                doSkillStrenghtenTherapyAnotherMore(bf, dmg2, skill, ss);

                if(dmg2 > 0)
                    doSkillStrenghtenCriticalDamageAroundOne(bf, skill, cs2, ss, target_side, target_pos);
                dmg += doSkillStrenghtenCriticalPierceDmgB(bf, skill, cs2, pr2, ss, target_side, target_pos);
            }

            BattleFighter* ptarget = static_cast<BattleFighter*>(getObject(target_side, target_pos));
            if(ptarget && ptarget->getHP() <= 0)  // beat to death!!!
            {
                bf->setMainTargetDeadFlag(true);
            }
        }
        // 群体技能
        else if(1 == skill->area)
        {
            int i = 0;
            int fsize = skill->factor.size();
            for(UInt8 pos = 0; pos < 25; pos++)
            {
                if(getObject(target_side, pos) == NULL || getObject(target_side, pos)->getHP() == 0)
                    continue;
                float factor = 1;
                int idx = std::min(abs(fsize-1), i);
                if(fsize > 0)
                    factor = skill->factor[idx];
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, pos), factor, 0, NULL, NULL, canProtect);
                doSkillEffectExtraAbsorb(bf, dmg, skill);
                ++i;
            }
        }
        else if(specialEf && isQueqiao)
        {
            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > 0)
                factor = skill->factor[fsize-1];
            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), factor, apcnt, ap, atkAct, canProtect);
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

            dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, target_pos), skill->factor[0], apcnt, ap, atkAct, canProtect);
            doSkillEffectExtraAbsorb(bf, dmg, skill);
            int nindex = 2;
            for(int i = 0; i < apcnt; ++ i)
            {
                BattleFighter* ptarget = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                if(!ptarget || ptarget->getHP() == 0)  // 活人才需要打，死人略过
                    continue;
                bf->setAttackIndex(nindex++);  // 依次第二、第三记录下来
                dmg += attackOnce(bf, first, cs, pr, skill, getObject(target_side, ap[i].pos), ap[i].factor);
                doSkillEffectExtraAbsorb(bf, dmg, skill);
            }
            if(ef)
            {
                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bf = static_cast<BattleFighter*>(getObject(target_side, ap[i].pos));
                    if(!bf || bf->getHP() == 0)
                        continue;
                    if(_rnd(10000) < ef->value * 100)
                    {
                        doXinmoAttack(bf, getObject(target_side, target_pos));
                        break;
                    }
                }
            }
        }

        // 天劫:御雷神针、芭蕉巨扇、盘古神斧
        UInt16 skillId = SKILL_ID(skill->getId());
        if((skillId == 150) || (skillId == 151) || (skillId == 152)
                || (skillId == 153) || (skillId == 154) || (skillId == 155))
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
                        UInt32 magdef = bo->getMagDefend();
                        UInt32 dmg = _formula->calcDamage(factor*bf->getMagAttack()*0.35f, magdef, bf->getLevel(), 1, 0);
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
                        UInt32 magdef = bo->getMagDefend();
                        UInt32 dmg = _formula->calcDamage(factor*bf->getMagAttack()*0.35f, magdef, bf->getLevel(), 1, 0);
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
                    else
                    {
                        UInt32 def = bo->getDefend();
                        UInt32 dmg = _formula->calcDamage(factor*bf->getAttack()*0.35f, def, bf->getLevel(), 1, 0);
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
                        UInt32 magdef = bo->getMagDefend();
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*bf->getMagAttack()*0.35f, magdef, bf->getLevel(), 1, 0);
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
                        UInt32 magdef = bo->getMagDefend();
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*bf->getMagAttack()*0.35f, magdef, bf->getLevel(), 1, 0);
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
                    else
                    {
                        UInt32 def = bo->getDefend();
                        UInt32 dmg = _formula->calcDamage(ap[i].factor*bf->getAttack()*0.35f, def, bf->getLevel(), 1, 0);
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
        int nChangeAuraNum = -1*bf->getAura() + bf->getAuraLeft(); // 因为天赋术，hero无双之后会留一点灵力
        //setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, nChangeAuraNum, 0, false);
        setStatusChange_Aura2(bf, bf->getSide(), bf->getPos(), NULL, nChangeAuraNum, 0, false);
        if (launchPeerLess)
            *launchPeerLess = 1;
    }

    if(ss && bf->getHP() != 0)
    {
        const std::vector<const GData::SkillStrengthenEffect*>& efs = ss->effect;
        const GData::SkillStrengthenEffect* ef = NULL;
        for(size_t i = 0; i < efs.size(); ++ i)
        {
            ef = efs[i];
            if(ef->target == GData::e_battle_target_self)
                doSkillStrengthenAttack(bf, skill, ef, bf->getSide(), bf->getPos(), true);
            else
                doSkillStrengthenAttack(bf, skill, ef, target_side, target_pos, true);
        }

        ef = ss->getEffect(GData::ON_SKILLUSED, GData::TYPE_RANDOM_BLEED);
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

                setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAura, value, skill->last, activeFlag);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        if(bo->getColorStock() != 0)
            return false;
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAtk, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
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
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagAtk, value, skill->last, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
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
                        fatkvalue = fighter->getMagAttack();
                    else
                        fatkvalue = fighter->getAttack();
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
    for(UInt8 i = 0; i < 25; ++ i)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0 || bo->hasFlag(BattleFighter::IsMirror) || bo->isSummon())
            continue;
        if(bo->getHP() < (bo->getMaxHP() >> 1))
        {
            return bo;
        }
    }

    return NULL;
}

BattleFighter* BattleSimulator::getTherapyTarget2(BattleFighter* bf, UInt8 * excepts, size_t exceptCount)
{
    UInt8 side = bf->getSide();
    BattleFighter* bo = NULL;
    BattleFighter* boSummon = NULL;
    UInt32 maxHpLost = 0;
    UInt8 pos = 0;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        bo = static_cast<BattleFighter*>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0 || bo->hasFlag(BattleFighter::IsMirror))
            continue;
        if(bo->isSummon())
        {
            boSummon = bo;
            continue;
        }

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
            return bo;
        }

        if(maxHp - hp > maxHpLost)
        {
            maxHpLost = maxHp - hp;
            pos = i;
        }
    }

    if(maxHpLost != 0)
        bo = static_cast<BattleFighter*>(getObject(side, pos));
    else if(boSummon && boSummon->getHP() < boSummon->getMaxHP())
        bo = boSummon;

    return bo;
}

BattleFighter* BattleSimulator::getTherapyTarget3(BattleFighter* bf, UInt8 * excepts, size_t exceptCount)
{
    UInt8 side = bf->getSide();
    BattleFighter* bo = NULL;
    BattleFighter* boSummon = NULL;
    UInt32 maxHpLost = 0;
    UInt8 pos = 0;
    for(UInt8 i = 0; i < 25; ++ i)
    {
        bo = static_cast<BattleFighter*>(getObject(side, i));
        if(bo == NULL || bo->getHP() == 0 || bo->hasFlag(BattleFighter::IsMirror))
            continue;
        if(bo->isSummon())
        {
            boSummon = bo;
            continue;
        }

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

        if(((float)(maxHp - hp))/maxHp > ((float)(maxHpLost))/maxHp)
        {
            maxHpLost = maxHp - hp;
            pos = i;
        }
    }

    if(maxHpLost != 0)
        bo = static_cast<BattleFighter*>(getObject(side, pos));
    else if(boSummon && boSummon->getHP() < boSummon->getMaxHP())
        bo = boSummon;

    return bo;
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
                UInt32 res = doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
                if(rcnt == 0 && res != 0)
                    rcnt = 1;
            }

            if(NULL != (passiveSkill = bf->getPassiveSkillEnter()))
            {
                int target_side, target_pos, cnt;
                getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
                UInt32 res = doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt);
                if(rcnt == 0 && res != 0)
                    rcnt = 1;
            }
        }
    }

    //if(rcnt != 0)
    appendMaxReiatsu();
    appendToPacket(0, -1, -1, 0, 0, false, false);

    return 1;
}

UInt32 BattleSimulator::doSkillAttackAftEnter(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt)
{
    UInt32 rcnt = 0;
    if(skill->cond != GData::SKILL_ENTER || target_pos < 0)
    {
        return rcnt;
    }

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
                if (doSkillAttack(bf, skill, target_side, target_pos, 1))
                {
                    ++ rcnt;
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

    UInt8 side = bf->getSide();
    UInt8 reiatsuType = 0;
    UInt8 reiatsuType2 = 0;
    UInt8 launchPeerLess = 0;

    BattleFighter* mainTarget = NULL;
    do {
        rcnt += doDeBufAttack(bf);
        if(bf->getHP() == 0)
            break;

        UInt32 stun = bf->getStunRound();
        UInt32 confuse = bf->getConfuseRound();
        UInt32 forget = bf->getForgetRound();
        if(stun > 0)
        {
            break;
        }

        // 雪人
        if(bf->getId() == 5679)
            break;

        int target_pos;
        int otherside = 1 - bf->getSide();
        if(confuse > 0)
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
        else if(forget > 0)
        {
            target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
            mainTarget = static_cast<BattleFighter*>(getObject(otherside, target_pos));

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
        else
        {
            const GData::SkillBase* skill = NULL;
            // do preve attack passive skill that must act
            size_t skillIdx = 0;
            int cnt = 0;
            target_pos = getPossibleTarget(bf->getSide(), bf->getPos(), bf);
            mainTarget = static_cast<BattleFighter*>(getObject(otherside, target_pos));
            doItemLingSkillAttack(bf, mainTarget);
            bool noPossibleTarget = (target_pos == -1);
            while(NULL != (skill = bf->getPassiveSkillPrvAtk100(skillIdx, noPossibleTarget)))
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
            BattleFighter* therapy_bf = getTherapyTarget(bf);
            skill = bf->getActiveSkill(therapy_bf!= NULL, noPossibleTarget);
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
                        ptarget->setCounterChangeByPeerless(0);
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

            if(canNormal)
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
                if(bf->getHP() > 0)
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
                }
            }
        }

        // 攻击后的被动技能
        if(bf->getHP() > 0)
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

            while(NULL != (passiveSkill = bf->getPassiveSkillOnAttackBleed100(idx, noPossibleTarget)))
            {
                if(passiveSkill->target == GData::e_battle_target_otherside && bo && bo->getHP() && 
                        (bo->getBleedRandomLast() || bo->getBleedBySkillLast() || //bo->getBleedBySkillClass() || bo->getBleedAttackClass() || 
                         bo->getBleed1Last() || bo->getBleed2Last() || bo ->getBleed3Last() || bo->getAuraBleedLast() || bo->getStunBleedLast() || bo->getConfuceBleedLast() ||
                         bo->getBleedMoLast() || bo->getSelfBleedLast()))
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
    }
    while(false);

    if (reiatsuType)
    {
        rcnt += tryPetEnter(side, reiatsuType, reiatsuType2?true:false);
    }
    if (reiatsuType2)
        rcnt += tryPetEnter(side, reiatsuType2);

    if(mainTarget)
        mainTarget->setShieldObj(NULL);

    rcnt += releaseCD(bf);
    _activeFgt = NULL;
    return rcnt;
}

// try to pre-use skill before attack
UInt32 BattleSimulator::tryPreUseSkill( BattleFighter * bf, BattleObject * target_object )
{
    return 0;
}

struct _PosAndHP
{
    UInt32 hp;
    UInt8 pos;
    bool operator <(const _PosAndHP& other) const
    {
        return hp > other.hp;
    }
};

// try to post-use skill after attack
UInt32 BattleSimulator::tryDelayUseSkill( BattleFighter * bf, BattleObject * target_object )
{
#if 0
    UInt16 skillId = bf->getFighter()->getSkill();
    switch(skillId)
    {
    case 104:
    case 105:
    case 110:
        {
            Script::BattleFormula::SkillData& sd = _formula->skillData(0, skillId - 101, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                break;

            float atk = sd.value1 * bf->getAttack();
            UInt8 side = target_object->getSide();
            UInt8 pos = target_object->getPos(), minpos = pos / 5 * 5, maxpos = minpos + 5;
            size_t defCount = 0, scCount = 0;
            DefStatus defList[5];
            StatusChange scList[50];
            for(UInt8 i = minpos; i < maxpos; ++ i)
            {
                BattleObject * bo = _objs[side][i];
                if(bo == NULL || bo->getHP() == 0)
                    continue;
                UInt32 dmg;
                if(bo->isChar())
                    dmg = _formula->calcDamage(atk, static_cast<BattleFighter *>(bo)->getDefend());
                else
                    dmg = static_cast<UInt32>(atk);
                bo->makeDamage(dmg);
                defList[defCount].pos = i;
                defList[defCount].damType = 0;
                defList[defCount].damage = dmg;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
                if(bo->getHP() == 0)
                    onDead(bo);
                else if(_winner == 0)
                    onDamage(bo, scList, scCount, true);
            }
            if(defCount > 0)
            {
                appendToPacket(bf->getSide(), bf->getPos(), pos, 2, skillId, false, false, defList, defCount, scList, scCount);
                return 1;
            }
        }
        return 0;
    case 201:
        {
            if(target_object->getHP() == 0)
                return 0;
            Script::BattleFormula::SkillData& sd = _formula->skillData(1, skillId - 201, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                return 0;
            UInt8 pos = target_object->getPos();
            float atk = bf->getAttack();
            UInt32 dmg;
            if(target_object->isChar())
            {
                float def = static_cast<BattleFighter *>(target_object)->getDefend();
                dmg = _formula->calcDamage(atk, def);
            }
            else
            {
                dmg = static_cast<UInt32>(atk);
            }
            size_t defCount = 0;
            DefStatus defList[5];
            for(UInt32 i = 0; i < sd.value3; ++ i)
            {
                UInt32 fdmg = static_cast<UInt32>((sd.value1 + _rnd(static_cast<UInt32>(sd.value2 - sd.value1) + 1)) / 100.0f * dmg);
                target_object->makeDamage(fdmg);
                defList[defCount].pos = pos;
                defList[defCount].damType = 0;
                defList[defCount].damage = fdmg;
                defList[defCount].leftHP = target_object->getHP();
                ++ defCount;
                if(target_object->getHP() == 0)
                    break;
            }
            StatusChange scList[50];
            size_t scCount = 0;
            if(target_object->isChar())
            {
                if(target_object->getHP() == 0)
                    onDead(target_object);
                else if(_winner == 0)
                    onDamage(target_object, scList, scCount, true);
            }
            appendToPacket(bf->getSide(), bf->getPos(), pos, 2, skillId, false, false, defList, defCount, scList, scCount);
            return 1;
        }
        return 0;
    case 205:
    case 208:
        {
            if(bf->hasFlag(BattleFighter::Mirrored))
                return 0;
            Script::BattleFormula::SkillData& sd = _formula->skillData(1, skillId - 201, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                return 0;
            UInt8 posList[25];
            size_t posCount = 0;
            UInt8 side = bf->getSide();
            UInt8 y = 0;
            while(y < 5)
            {
                UInt8 p = y * 5;
                for(UInt8 x = 0; x < 5; ++ x, ++ p)
                {
                    if(_objs[side][p] == NULL || _objs[side][p]->getHP() == 0)
                        posList[posCount ++] = p;
                }
                if(posCount > 0)
                    break;
                ++ y;
            }
            if(posCount == 0)
                return 0;
            bf->addFlag(BattleFighter::Mirrored);
            UInt8 pos = posList[_rnd(posCount)];
            BattleFighter * newf = new(std::nothrow) Battle::BattleFighter(*bf);
            if(newf == NULL)
                return 0;
            newf->setSideAndPos(side, pos);
            newf->addFlag(BattleFighter::IsMirror);
            setObject(side, pos, newf);
            newf->_attack = newf->_attack * sd.value1;
            newf->_attackAdd = newf->_attackAdd * sd.value1;
            newf->_defendAdd = 0;
            FighterStatus fs(newf);
            fs.action = 0;
            // insert fighter into queue by order
            insertFighterStatus(fs);
            appendToPacket(bf->getSide(), bf->getPos(), pos + 25, 2, skillId, false, false, NULL, 0, NULL, 0);
            return 1;
        }
        return 0;
    case 206:
    case 207:
    case 210:
        {
            Script::BattleFormula::SkillData& sd = _formula->skillData(1, skillId - 201, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                return 0;
            float atk = sd.value1 * bf->getAttack();
            UInt8 side = target_object->getSide();
            size_t defCount = 0, scCount = 0;
            DefStatus defList[25];
            StatusChange scList[50];
            for(UInt8 i = 0; i < 25; ++ i)
            {
                BattleObject * bo = _objs[side][i];
                if(bo == NULL || bo->getHP() == 0)
                    continue;
                UInt32 dmg;
                if(bo->isChar())
                    dmg = _formula->calcDamage(atk, static_cast<BattleFighter *>(bo)->getDefend());
                else
                    dmg = static_cast<UInt32>(atk);
                bo->makeDamage(dmg);
                defList[defCount].pos = getSidePos(bo);
                defList[defCount].damType = e_damNormal;
                defList[defCount].damage = dmg;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
                if(bo->getHP() == 0)
                    onDead(bo);
                else if(_winner == 0)
                    onDamage(bo, scList, scCount, true);
            }
            if(defCount > 0)
            {
                appendToPacket(bf->getSide(), bf->getPos(), target_object->getPos(), 2, skillId, false, false, defList, defCount, scList, scCount);
                return 1;
            }
        }
        return 0;
    case 303:
        {
            Script::BattleFormula::SkillData& sd = _formula->skillData(2, 2, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                return 0;
            UInt8 side = bf->getSide();
            UInt32 rhp = sd.value3 + static_cast<UInt32>(sd.value1 * bf->getIntelligence());
            UInt32 maxrcount = sd.value4;
            _PosAndHP rlist[25];
            UInt32 rcount = 0;
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                BattleObject * bo = _objs[side][pos];
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;
                rlist[rcount].hp = static_cast<BattleFighter *>(bo)->getLostHP();
                if(rlist[rcount].hp == 0)
                    continue;
                rlist[rcount].pos = pos;
                if(rcount >= maxrcount)
                {
                    std::push_heap(rlist, rlist + maxrcount + 1);
                    std::pop_heap(rlist, rlist + maxrcount + 1);
                }
                else
                {
                    ++ rcount;
                    std::push_heap(rlist, rlist + rcount);
                }
            }
            DefStatus defList[25];
            size_t defCount = 0;
            for(UInt32 i = 0; i < rcount; ++ i)
            {
                BattleObject * bo = _objs[side][rlist[i].pos];
                UInt32 hpr = static_cast<BattleFighter *>(bo)->regenHP(rhp);
                if(hpr == 0)
                    continue;
                defList[defCount].pos = getSidePos(bo);
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
            }
            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, 303, false, false, defList, defCount, NULL, 0);
        }
        return 1;
    case 304:
    case 308:
        {
            Script::BattleFormula::SkillData& sd = _formula->skillData(2, skillId - 301, bf->getFighter()->getSkillLevel());
            if(_rnd(100) >= sd.rate)
                return 0;
            StatusChange scList[50];
            size_t scCount = 0;
            UInt8 side = target_object->getSide();
            DefStatus defList[25];
            float atk = sd.value1;
            for(UInt32 i = 0; i < sd.value3; ++ i)
            {
                BattleFighter * tf = getRandomFighter(side, NULL, 0);
                if(tf != NULL)
                    defList[i].pos = tf->getPos();
                else
                    defList[i].pos = 12;
                BattleObject * bo = _objs[side][defList[i].pos];
                if(bo == NULL || bo->getHP() == 0)
                {
                    defList[i].damType = e_damNormal;
                    defList[i].damage = 0;
                    defList[i].leftHP = 0;
                    continue;
                }
                UInt32 dmg;
                if(bo->isChar())
                {
                    dmg = _formula->calcDamage(atk, static_cast<BattleFighter *>(bo)->getDefend());
                    static_cast<BattleFighter *>(bo)->setStunRound(sd.value4 + 1);
                }
                else
                    dmg = static_cast<UInt32>(atk);
                bo->makeDamage(dmg);
                defList[i].damType = e_damNormal;
                defList[i].damage = dmg;
                defList[i].leftHP = bo->getHP();
                if(bo->getHP() == 0)
                    onDead(bo);
                else if(_winner == 0)
                    onDamage(bo, scList, scCount, true);
            }
            appendToPacket(bf->getSide(), bf->getPos(), target_object->getPos(), 2, skillId, false, false, defList, sd.value3, scList, scCount);
        }
        return 1;
    default:
        return 0;
    }
#endif
    return 0;
}

int BattleSimulator::testWinner()
{
    int alive[2] = { 0, 0 };
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
        size_t c = fgtlist.size();
        for(size_t i = 0; i < c; ++ i)
        {
            alive[fgtlist[i]->getSide()] ++;
        }
    }
    if(alive[0] == 0)
        return 2;
    else if(alive[1] == 0)
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

    bool fRevival = false;
    bool fFakeDead = false;
    bool fSummonOrMirror = false;
    do
    {
        if(static_cast<BattleFighter*>(bo)->hasFlag(BattleFighter::IsMirror))
        {
            fSummonOrMirror = true;
            break;
        }
        if(static_cast<BattleFighter*>(bo)->isPet())
        {
            fSummonOrMirror = true;
            break;
        }
    
        if(static_cast<BattleFighter*>(bo)->isSummon())
        {
            BattleFighter* summoner = static_cast<BattleFighter*>(bo)->getSummoner();
            UInt8 aura = static_cast<BattleFighter*>(bo)->getUnSummonAura();

            if(summoner)
                setStatusChange(static_cast<BattleFighter*>(bo), summoner->getSide(), summoner->getPos(), 1, 0, e_stAura, aura, 0, false);

            fSummonOrMirror = true;
            break;
        }

        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead100(idx);
        if(passiveSkill == NULL)
            passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead();

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
                    }
                }
                break;
            default:
                {
                    if(!(static_cast<BattleFighter*>(bo))->isRevival())
                    {
                        fRevival = true;
                        (static_cast<BattleFighter*>(bo))->setRevival();
                        //doSkillAttackAftEnter(static_cast<BattleFighter*>(bo));
                    }
                }
                break;
            }

            if(fFakeDead || fRevival)
            {
                appendDefStatus(e_skill, passiveSkill->getId(), static_cast<BattleFighter*>(bo));
                break;
            }
        }
    } while(false);

    if(!fFakeDead && !fRevival)
    {
        // remove from action queue
        BattleFighter* toremove = static_cast<BattleFighter *>(bo);
        removeFighterStatus(toremove);

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

        // re-test winner
        _winner = testWinner();

        if(_winner == 0)
        {
            doItemWuSkillAttack(static_cast<BattleFighter*>(bo));
        }

        if(!fSummonOrMirror)
        {
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
    }

    return true;
}

void BattleSimulator::onDamage( BattleObject * bo, bool active, std::vector<AttackAct>* atkAct)
{
    if(!bo)
        return;
    BattleFighter* bo2 = static_cast<BattleFighter*>(bo);

    UInt8& auraCD = bo2->getAuraDecCD();
    if(auraCD > 0)
    {
        -- auraCD;
        if(auraCD == 0)
        {
            doAuraPresent(bo2);
            setStatusChange_Aura(NULL, bo2->getSide(), bo2->getPos(), NULL, -100, 0, true);
        }
    }
    UInt8& confuceCD = bo2->getConfuceCD();
    if(confuceCD > 0)
    {
        float rate = bo2->getMagRes(NULL) * 100;
        if(confuceCD == 1 && rate > _rnd(10000))
        {
            appendDefStatus(e_Res, 0, bo2);
        }
        else
        {
            -- confuceCD;
        }
        if(confuceCD == 0)
        {
            if(!doConfusePresent(bo2))
            {
                bo2->setConfuseLevel(9);
                bo2->setConfuseRound(1);
                appendDefStatus(e_Confuse, 0, bo2);
            }
        }
    }
    UInt8& stunCD = bo2->getStunCD();
    if(stunCD > 0)
    {
        float rate = bo2->getMagRes(NULL) * 100;
        if(stunCD == 1 && rate > _rnd(10000))
        {
            appendDefStatus(e_Res, 0, bo2);
        }
        else
        {
            -- stunCD;
        }
        if(stunCD == 0)
        {
            if(!doStunPresent(bo2))
            {
                bo2->setStunLevel(9);
                bo2->setStunRound(1);
                appendDefStatus(e_Stun, 0, bo2);
            }
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
    // 获取血量最少的散仙
    UInt32 minHp = 0;
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
        if (!minHp || bo->getHP() < minHp)
        {
            minHp = bo->getHP();
            posList.clear();
            posList.push_back(i);
        }
        else if (bo->getHP() == minHp)
        {
            posList.push_back(i);
        }
    }
    if(posList.empty())
        return NULL;
    return static_cast<BattleFighter *>(getObject(side, posList[_rnd(posList.size())]));
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
        float tmpAtk  = bf->getAttack() > bf->getMagAttack() ? bf->getAttack():bf->getMagAttack();
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
        float tmpAtk  = bf->getAttack() > bf->getMagAttack() ? bf->getAttack():bf->getMagAttack();
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

    bf->releaseSkillCD(1);
    bf->releaseMoEvade100();

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
        bf->setHP(0);
        onDead(true, bf);
        appendDefStatus(e_unSummon, 0, bf);
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
    }

    UInt8& magAtkAdd_last = bf->getMagAttackAddLast();
    if(magAtkAdd_last > 0)
    {
        -- magAtkAdd_last;
       if(0 == magAtkAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagAtk, 0, 0, false);
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
    }
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
    }
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
    }
    bfgt->setMagAttackAdd(value, last);
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
    }
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
    UInt32 nBleed = abs(bf->calcPoison(skill, bo, false)) * ef->value/100;
    UInt8 nClass = bf->getClass();
    if(nBleed > 0)
    {
        bo->setBleedBySkill(nBleed, ef->last);
        bo->setBleedBySkillClass(nClass);
        if(nClass == e_cls_ru)
            appendDefStatus(e_Bleed1, 0, bo);
        else if(nClass == e_cls_shi)
            appendDefStatus(e_Bleed2, 0, bo);
        else
            appendDefStatus(e_Bleed3, 0, bo);
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
        appendDefStatus(eType, dmg, fighter, e_damagePhysic); // 这里应该都是普通物理攻击吧
        if (eType == e_damNormal)
        {
            // 这个加进去，可以看到人跑到友人那边攻击一次？
            appendDefStatus(e_xinmo, 0, bf);
            makeDamage(fighter, dmg);
            if(fighter->getHP() == 0)
            {
                onDead(false, fighter);
            }
            else if(_winner == 0)
            {
                onDamage(fighter, true, NULL);
            }
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
        atk = bf->calcAttack(cs, area_target, &cf);
        if(cs )
        {
            UInt8 s = bf->getSide();
            if(s < 2)
                _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

        }

        float def;
        float toughFactor = pr ? area_target->getTough(bf) : 1.0f;
        def = area_target->getDefend();
        float atkreduce = area_target->getAtkReduce();
        dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);
        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;
        dmg = dmg > 0 ? dmg : 1;
        eStateType = e_damNormal;
    }
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
        bo->setBleedRandom(nBleed, ef->last);
        bo->setBleedAttackClass(nClass);
        switch(nClass)
        {
        case e_cls_ru:
            appendDefStatus(e_Bleed1, 0, bo);
            break;
        case e_cls_shi:
            appendDefStatus(e_Bleed2, 0, bo);
            break;
        case e_cls_dao:
            appendDefStatus(e_Bleed3, 0, bo);
            break;
        case e_cls_mo:
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
        def = bo->getDefend();
        reduce = bo->getAtkReduce();
        attack = bf->getAttack();
    }
    else
    {
        def = bo->getMagDefend();
        reduce = bo->getMagAtkReduce();
        attack = bf->getMagAttack();
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
    makeDamage(pTarget, nRealDamage);
    appendDefStatus(e_damNormal, nRealDamage, pTarget, e_damageTrue);

    if(pTarget->getHP() == 0)
    {
        onDead(false, pTarget);
    }
    else if(_winner == 0)
    {
        onDamage(pTarget, true, NULL);
    }

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
    if(arrayState[nIndex]& nImmu2)
    {
        pTarget->setImmune2(0);
        appendDefStatus(e_unImmune2, 0, pTarget);  // 技能符文里面的那个免疫起效了
        return true;
    }
    if((arrayState[nIndex] & nImmu) && SKILL_LEVEL(nSkillId) <= pTarget->getImmuneLevel(arrayState[nIndex]))
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
        float darkVigor = bf->getDarkVigor();
        if(bf->releaseDarkVigor())
        {
            appendDefStatus(e_unDarkVigor, 0, bf);
            doDarkVigorAttack(bf, darkVigor);
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
                makeDamage(bf, dmg);
                if(bf->releaseBleedMo())
                    appendDefStatus(e_unBleedMo, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_BleedMo, dmg, bf, e_damageTrue);

                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
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
                makeDamage(bf, dmg);
                -- last1;
                if(last1 == 0)
                    appendDefStatus(e_unBleed1, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_Bleed1, dmg, bf, e_damageTrue);

                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }

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
                makeDamage(bf, dmg);
                -- last2;
                if(last2 == 0)
                    appendDefStatus(e_unBleed2, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_Bleed2, dmg, bf, e_damageTrue);


                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
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
                makeDamage(bf, dmg);
                -- last3;
                if(last3 == 0)
                    appendDefStatus(e_unBleed3, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_Bleed3, dmg, bf, e_damageTrue);

                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
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
            makeDamage(bf, dmg);
            -- last3;
            if(last3 == 0)
                appendDefStatus(e_unSelfBleed, dmg, bf, e_damageTrue);
            else
                appendDefStatus(e_selfBleed, dmg, bf, e_damageTrue);

            if(bf->getHP() == 0)
            {
                onDead(true, bf);
            }
            else if(_winner == 0)
            {
                onDamage(bf, false);
            }
            if(selflast == 0)
                bf->setSelfBleed(0, 0);
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
                makeDamage(bf, dmg);
                -- ablast;
                if(ablast == 0)
                    appendDefStatus(e_unBleed1, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_Bleed1, dmg, bf, e_damageTrue);

                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
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
                makeDamage(bf, dmg);
                -- cblast;
                if(cblast == 0)
                    appendDefStatus(e_unBleed4, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_Bleed4, dmg, bf, e_damageTrue);

                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
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
                makeDamage(bf, dmg);
                -- sblast;
                if(sblast == 0)
                    appendDefStatus(e_unBleed3, dmg, bf, e_damageTrue);
                else
                    appendDefStatus(e_Bleed3, dmg, bf, e_damageTrue);

                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
                if(sblast == 0)
                {
                    bf->setStunBleed(0, 0, 0);
                    bf->setStunPresent(0, 0);
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
                makeDamage(bf, dmg);
                -- nrandomlast;
                if(nrandomlast == 0)
                {
                    bf->setBleedRandom(0, 0);
                    appendDefStatus(eUnType, dmg, bf, e_damageTrue);
                }
                else
                {
                    appendDefStatus(eType, dmg, bf, e_damageTrue);
                }
                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
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
                makeDamage(bf, dmg);
                -- nbySkilllast;

                if(nbySkilllast == 0)
                {
                    bf->setBleedBySkill(0, 0);
                    appendDefStatus(eUnType, dmg, bf, e_damageTrue);
                }
                else
                {
                    appendDefStatus(eType, dmg, bf, e_damageTrue);
                }
                if(bf->getHP() == 0)
                {
                    onDead(true, bf);
                }
                else if(_winner == 0)
                {
                    onDamage(bf, false);
                }
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
        else if (eft[i] == GData::e_eft_atk_pet_mark_extra_dmg && bo->isPetMark())
            hitCnt += efv[i];
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

    {
        appendDefStatus(e_Summon, newf->getPortrait(), newf);
        UInt32 value = static_cast<UInt32>(newf->getAura());
        appendStatusChange(e_stAura, value, 0, newf);
    }

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
        ef = ss->getEffect(GData::ON_SUMMON, GData::TYPE_UNSUMMON_AURA);
    if(ef)
    {
        newf->setUnSummonAura(bf, ef->value);
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
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(target_side, target_pos));
    if(!bo)
        return;
    if (!bo->isChar())
        return;
    float hp = bf->getMaxHP() * (skill->effect->efv[eftIdx]);
    if (hp < 1.0f)
        return;
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
    UInt8 myPos = bf->getPos();
    BattleFighter* bo = getRandomFighter(bf->getSide(), &myPos, 1);
    if(!bo)
        return;
    if (!bo->isChar())
        return;
    float hp = bf->getMaxHP() * (skill->effect->efv[eftIdx]);
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
    BattleFighter* bf2 = static_cast<BattleFighter*>(bo);
    if (bf2->getClass() == e_cls_dao || bf2->getClass() == e_cls_mo)
    {
        bf2->setPetAttackAdd(atkadd, last);
        UInt16 skillId = skill != NULL ? skill->getId() : 0;
        UInt32 value = static_cast<UInt32>(bf2->getAttack());
        appendStatusChange(e_stAtk, value, skillId, bf2);
    }
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
    setStatusChange_Atk(bf, bo->getSide(), bo->getPos(), skill, bo->_attack * skill->effect->efv[eftIdx], skill->effect->efl[eftIdx], true);
    setStatusChange_MagAtk(bf, bo->getSide(), bo->getPos(), skill, bo->_magatk * skill->effect->efv[eftIdx], skill->effect->efl[eftIdx], true);
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
    // Nothing to do.
    /*
    // 造成额外伤害

    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo->isChar())
        return;
    BattleFighter * area_target = static_cast<BattleFighter *>(bo);

    bool first = false;
    bool cs    = false;
    bool pr    = false;
    attackOnce(bf, first, cs, pr, NULL, area_target, skill->effect->efv[eftIdx], 1);
    */
}

void BattleSimulator::doSkillEffectExtra_ProtectPet100(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx)
{
    // 为目标单位100%发动援护
    BattleObject * bo = getObject(target_side, target_pos);
    if (!bo->isChar())
        return;
    BattleFighter * area_target = static_cast<BattleFighter *>(bo);
    //area_target->setPetProtect100(skill->effect->efv[eftIdx], skill->effect->efl[eftIdx]);
    area_target->setPetProtect100(true, 0);
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
            _cur_round_except[_except_count] = bo->getPos();
            ++ _except_count;
            UInt32 hpr = bo->regenHP(rhp * factor, skill->cond == GData::SKILL_ACTIVE);
            if(hpr != 0)
            {
                appendDefStatus(e_damHpAdd, hpr, bo);
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
    if(bo->makeShieldDamage(dmg))
    {
        appendDefStatus(e_damHPShield, 0, bo);
    }
}

float BattleSimulator::getSkillEffectExtraCounterDarkVigor(const GData::SkillBase* skill)
{
    if(!skill || !skill->effect)
        return 0;
    const std::vector<UInt16>& eft = skill->effect->eft;
    const std::vector<float>& efv = skill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return 0;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_counter_hate)
        {
            return efv[i];
        }
    }

    return 0;
}

bool BattleSimulator::doDarkVigorAttack(BattleFighter* bf, float darkVigor)
{
    bool bfDead = false;
    UInt32 dmg = darkVigor;
    makeDamage(bf, dmg);
    appendDefStatus(e_damNormal, dmg, bf, e_damageTrue);

    if(bf->getHP() == 0)
    {
        bfDead = true;
        onDead(false, bf);
    }
    else if(_winner == 0)
    {
        onDamage(bf, true, NULL);
    }
 
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
            makeDamage(bo, dmg2);
            appendDefStatus(e_damNormal, dmg2, bo, e_damageTrue);

            if(bo->getHP() == 0)
            {
                onDead(false, bo);
            }
            else if(_winner == 0)
            {
                onDamage(bo, true, NULL);
            }
        }
    }

    return bfDead;
}

void BattleSimulator::appendDefStatus(StateType type, UInt32 value, BattleFighter* bf, DamageType damageType /* = e_damageNone */)
{
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
}

void BattleSimulator::appendStatusChangeForReiastu(StatusType type, UInt32 value, UInt16 skillId, UInt8 side)
{
    StatusChange sc;
    int target_pos = getPossibleTarget((side == 0 ? 1 : 0), 0);
    BattleFighter* bo = static_cast<BattleFighter*>(getObject(side, target_pos));
    if(!bo)
        return;
    appendStatusChange(e_stReiastu, value,0, bo);
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
        }
        break;
    case GObject::e_cls_mo:
        if(!doStateMagRes2(bf, bo, GData::e_state_blind))
        {
            bo->setBlind(0.75f, last);
            appendDefStatus(e_blind, 0, bo);
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
        atk = bf->getAttack() * (value/10000);
        def = bo->getDefend();
        reduce = bo->getAtkReduce();
        isPhysic = true;
    }
    else
    {
        atk = bf->getMagAttack() * (value/10000);
        def = bo->getMagDefend();
        reduce = bo->getMagAtkReduce();
        isPhysic = false;
    }

    UInt32 dmg = _formula->calcDamage(atk, def, bf->getLevel(), 1.0f, reduce);
    makeDamage(bo, dmg);
    appendDefStatus(e_damNormal, dmg, bo, isPhysic?e_damagePhysic:e_damageMagic);

    if(bo->getHP() == 0)
    {
        onDead(false, bo);
    }
    else if(_winner == 0)
    {
        onDamage(bo, true, NULL);
    }
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

void BattleSimulator::makeDamage(BattleFighter* bf, UInt32& u)
{
    if(!bf)
        return;
    float& shieldHp = bf->getHpShieldSelf();
    if(shieldHp > 0)
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
    if (petShieldHp > 0)
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
        const GData::SkillBase* passiveSkill = NULL;
        while(NULL != (passiveSkill = bf->getPassiveSkillOnGetDmg()))
        {
            // XXX: 写死是被动增益技能
            if (_getDamageSkillCount[bf->getSide()] >= 3)
                break;
            ++_getDamageSkillCount[bf->getSide()];
            int target_side, target_pos, cnt;
            getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
            //doSkillAttack(bf, passiveSkill, target_side, target_pos, cnt, NULL);
            appendDefStatus(e_skill, passiveSkill->getId(), bf);
            doSkillStatus2(bf, passiveSkill, target_side, target_pos, cnt);

        }
        bf->makeDamage(u);
    }
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
#ifdef _DEBUG
        //printf("doPetEnter pos error, pos = %d\n", pos);
#endif
        return 0;
    }

    UInt32 rcnt = 0;

    BattleObject * bo = _objs[side][pos];
    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
    {
#ifdef _DEBUG
        //printf("doPetEnter bo error.\n");
#endif
        return 0;
    }
    BattleFighter* bf = static_cast<BattleFighter *>(bo);
    _activeFgt = bf;
    appendReiatsuChange(side);
    appendDefStatus(e_petAppear, bf->getId(), bf);
    insertFighterStatus(bf);

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
    // 宠物保护主目标吸收一半伤害，当然，也可能保护完两个都挂了
    int tidx = -1;
    if ((tidx = getSpecificTarget(bf->getSide(), BattleSimulator::isPet)) >= 0)
    {
        BattleFighter * pet = static_cast<BattleFighter *> (_objs[bf->getSide()][tidx]);
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
    bf->setPetProtect100(false, 0);
    appendDefStatus(e_unPetProtect100, 0, bf);
    const GData::SkillBase* pskill = pet->getPassiveSkillOnPetProtectForce();
    if(!pskill || !pskill->effect)
        return false;
    const std::vector<UInt16>& eft = pskill->effect->eft;
    const std::vector<float>& efv = pskill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return false;
    for(size_t i = 0; i < cnt; ++ i)
    {
        if(eft[i] == GData::e_eft_pet_protect_reduce)
        {
            factor *= efv[i];
        }
    }
    {
    const GData::SkillBase* pskill = pet->getPassiveSkillOnPetProtectForce();
    if(!pskill || !pskill->effect)
        return false;
    const std::vector<UInt16>& eft = pskill->effect->eft;
    const std::vector<float>& efv = pskill->effect->efv;

    size_t cnt = eft.size();
    if(cnt != efv.size())
        return false;

    appendDefStatus(e_skill, pskill->getId(), pet);
    }
    return protectDamage(bf, pet, phyAtk, magAtk, factor);
}

bool BattleSimulator::doProtectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
    // 宠物概率保护主目标吸收一半伤害
    for(size_t i = 0; i < _onPetProtect.size(); ++ i)
    {
        const GData::SkillBase* pskill = pet->getPassiveSkillOnPetProtect();
        if(!pskill || !pskill->effect)
            return false;
        const std::vector<UInt16>& eft = pskill->effect->eft;
        const std::vector<float>& efv = pskill->effect->efv;

        size_t cnt = eft.size();
        if(cnt != efv.size())
            return false;

        appendDefStatus(e_skill, pskill->getId(), pet);
        for(size_t i = 0; i < cnt; ++ i)
        {
            if(eft[i] == GData::e_eft_pet_protect_reduce)
            {
                factor *= efv[i];
            }
        }
        return protectDamage(bf, pet, phyAtk, magAtk, factor);
    }
    return false;
}

bool BattleSimulator::protectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
    // 吸收一半伤害给仙宠
    
    phyAtk /= 2;
    magAtk /= 2;
    UInt32 dmg = 0;
    UInt32 magdmg = 0;
    bool dmgFlag = false;
    bool magdmgFlag = false;
    bool pr = bf->calcPierce(pet);
    float toughFactor = pr ? pet->getTough(bf) : 1.0f;

    if(magAtk)
    {
        float magdef = pet->getMagDefend();
        float magatkreduce = pet->getMagAtkReduce();
        magdmg = _formula->calcDamage(factor * magAtk, magdef, bf->getLevel(), toughFactor, magatkreduce);

        magdmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        magdmg = magdmg > 0 ? magdmg : 1;
        magdmgFlag = true;
    }

    if(phyAtk)
    {
        float def = pet->getDefend();
        float atkreduce = pet->getAtkReduce();
        dmg = _formula->calcDamage(factor * phyAtk, def, bf->getLevel(), toughFactor, atkreduce);

        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        dmg = dmg > 0 ? dmg : 1;
        dmgFlag = true;
    }


    UInt32 dmg3 = dmg + magdmg;
    doShieldHPAttack(pet, dmg3);
    if(dmg3 > 0)
    {
        makeDamage(pet, dmg3);
        if (magdmgFlag)
            appendDefStatus(e_damNormal, magdmg, pet, e_damageMagic);
        if (dmgFlag)
            appendDefStatus(e_damNormal, dmg, pet, e_damagePhysic);
        if(pet->getHP() == 0)
        {
            onDead(false, pet);
        }
        else if(_winner == 0)
        {
            onDamage(pet, true, NULL);
        }
    }

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

bool BattleSimulator::tryAttackWithPet(BattleFighter* bf, float& phyAtk, float& magAtk, float factor)
{
    // 宠物合击
    int tidx = 0;
    if ((tidx = getSpecificTarget(bf->getSide(), BattleSimulator::isPet)) >= 0)
    {
        BattleFighter * pet = static_cast<BattleFighter *> ((*this)(bf->getSide(), tidx));
        if (bf->getPetAtk100Last())
            return do100AttackWithPet(bf, pet, phyAtk, magAtk, factor);
        else
        {
            for(size_t i = 0; i < _onPetAtk.size(); ++ i)
            {
                BattleFighter* bf = _onPetAtk[i];
                if(!bf || bf->getHP() == 0)
                    continue;

                UInt32 stun = bf->getStunRound();
                UInt32 confuse = bf->getConfuseRound();
                UInt32 forget = bf->getForgetRound();

                if(stun > 0 || confuse > 0 || forget > 0)
                    continue;
                return doAttackWithPet(bf, pet, phyAtk, magAtk, factor);
            }
        }
    }
    return false;
}

bool BattleSimulator::do100AttackWithPet(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
    // 宠物100%合击
    bf->setPetAtk100(false, 0);
    appendDefStatus(e_unPetAtk100, 0, bf);
    const GData::SkillBase* pskill = pet->getPassiveSkillOnAtkDmgForce();
    if(!pskill)
        return false;
    appendDefStatus(e_skill, pskill->getId(), pet);
    return attackWithPet(bf, pet, phyAtk, magAtk, factor);
}

bool BattleSimulator::doAttackWithPet(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
    // 宠物概率合击
    for(size_t i = 0; i < _onPetAtk.size(); ++ i)
    {
        const GData::SkillBase* pskill = pet->getPassiveSkillOnAtkDmg();
        if(!pskill)
            return false;

        appendDefStatus(e_skill, pskill->getId(), pet);
        return attackWithPet(bf, pet, phyAtk, magAtk, factor);
    }
    return false;
}

bool BattleSimulator::attackWithPet(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor)
{
    // 宠物合击提升攻击力
    
    phyAtk += pet->getMagAttack() * bf->getPetAtk100();
    magAtk += pet->getMagAttack() * bf->getPetAtk100();

    return true;
}

void BattleSimulator::appendMaxReiatsu()
{
        appendStatusChangeForReiastu(e_stReiastu, getToggleReiatsu(0) ,0 , 0);
        appendStatusChangeForReiastu(e_stReiastu, getToggleReiatsu(1) ,0 , 1);
}

void BattleSimulator::appendReiatsuChange(int side)
{
    if (_backupObjs[side])
        appendStatusChangeForReiastu(e_stReiastu, getReiatsu(side) ,0 , side);
}

}
