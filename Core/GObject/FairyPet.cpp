﻿#include "Config.h"
#include "GObjectDBExecHelper.h"
#include "GData/FairyPetTable.h"
#include "Common/URandom.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "Server/SysMsg.h"
#include "Log/Log.h"
#include "Fighter.h"
#include "MsgID.h"
#include "FairyPet.h"
#include "Script/GameActionLua.h"
#include "GObject/Country.h"


namespace GObject
{
    extern URandom GRND;

    FairyPet::FairyPet(UInt32 id, Player * owner): Fighter(id, owner),
        _petLev(50), _petBone(0), _onBattle(false)
    {
        _pinjieBless = 0;
        _genguBless = 0;
        _chong = 0;
        _lingya = 0;
        _xiaozhou = 0;
        _dazhou = 0;
        _overTime = 0;
        _evolve = 0;
        memset(_initskl, 0, sizeof(_initskl));
    }

    void FairyPet::LoadFromDB(DBFairyPetData& data)
    {
        _onBattle = data.onBattle ? true : false;
        _petLev = data.petLev;
        _petBone = data.petBone;
        _chong = data.chong;
        _pinjieBless = data.pinjieBless;
        _genguBless = data.genguBless;
        _xiaozhou = data.xiaozhou;
        _dazhou = data.dazhou;
        _overTime = data.overTime;
        _evolve = data.evolve;
    }


    void FairyPet::LoadInitSkills(std::string& skills)
    {
        StringTokenizer tk(skills, ",");
        if(tk.count() > INIT_SKILL_UPMAX)
            return;
        for(UInt8 i = 0; i < tk.count(); ++ i)
        {
            _initskl[i] = atoi(tk[i].c_str());
        }
    }

    void FairyPet::UpdateToDB()
    {
        _overTime = _overTime ? _overTime : TimeUtil::SharpDayT(1);
        DB2().PushUpdateData("REPLACE INTO `fairyPet` (`id`, `playerId`, `onBattle`, `petLev`, `petBone`, `pinjieBless`, `genguBless`, `chong`, `overTime`, `xiaozhou`, `dazhou`, `evolve`) VALUES (%u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", 
                getId(), _owner->getId(), _onBattle, _petLev, _petBone, _pinjieBless, _genguBless, _chong, _overTime, _xiaozhou, _dazhou, _evolve);
    }

    bool FairyPet::checkTimeOver()
    {
        UInt32 now = TimeUtil::Now();
        if(now < _overTime)
            return false;
        _overTime = TimeUtil::SharpDayT(1 , now);
        reset(4);
        UpdateToDB();
        return true;
    }

    FairyPet * FairyPet::clone(Player * player)
    {
        FairyPet * pet = new FairyPet(*this);
        if(player != NULL)
        {
            pet->_level = 50;
            pet->_exp = 0;
        }
        pet->_owner = player;
        pet->_halo = NULL;
        pet->_fashion = NULL;
        pet->_weapon = NULL;
        pet->_ring = NULL;
        pet->_amulet = NULL;
        pet->_attrDirty = true;
        pet->_bPDirty = true;
        pet->_pexpMax = 100000;
        if(m_2ndSoul != NULL)
            pet->m_2ndSoul = new SecondSoul(*m_2ndSoul);

        memset(pet->_armor, 0, 5 * sizeof(ItemEquip *));
        memset(pet->_trump, 0, TRUMP_UPMAX * sizeof(ItemEquip*));
        return pet;
    }

    void FairyPet::boneUp()
    {
        ++ _petBone;
        if(GET_REMAINDER(_petBone) > 2)
            _petBone = (_petBone / 10 + 1) * 10;
        setPotential(GData::pet.getPetPotential(_petBone));
    }

    void FairyPet::levUp()
    {
        ++ _petLev;
        setLevel(_petLev);
        updateToDB(2, _petLev);
        initSkillUp();
        _owner->fairyPetUdpLog(10000, GET_REMAINDER(_petLev) ? GET_REMAINDER(_petLev) : 10);
    }

    void FairyPet::initSkillUp()
    {
        //触发初始技能升级
        UInt8 value = GET_REMAINDER(getPetLev());
        if(value  == 8 || value == 9)
        {
            GData::Pet::PinjieData * pjd = GData::pet.getLevTable(_petLev);
            if(!pjd) return;
            std::string skills = "";
            for(UInt8 i = 0; i < INIT_SKILL_UPMAX; ++ i)
            {
                UInt8 lev = pjd->skillLev[i];
                if(_initskl[i] == 0 || lev <= 0 || lev > 9)
                    continue;
                skills += Itoa(_initskl[i]+lev-1);
                if(i < INIT_SKILL_UPMAX - 1)
                    skills += ",";
            }
            setSkills(skills, false);
            updateToDBPetSkill();
        }
    }

    void FairyPet::addChongNum(int num)
    {
        if(_chong + num <= 0)
            reset(3);
        else
            _chong += num;
    }

    void FairyPet::addGenguBless(int prob)
    {
        if(_genguBless + prob <= 0)
            reset(2);
        else
            _genguBless += prob;
    }

    void FairyPet::addPinjieBless(UInt16 times)
    {
        _pinjieBless += times;
    }

    UInt16 FairyPet::getPinjieBless()
    {   //获得的失败加成概率
        return GData::pet.getPinjieBless(getPetLev(), _pinjieBless);
    }

    bool FairyPet::canLevUp()
    {
        UInt16 limit = _owner->getMainFighter()->getLevel() / 10 * 10 + 9;
        if(limit > getPetLev())
            return true;
        return false;
    }

    bool FairyPet::canBoneUp()
    {
        GData::Pet::LingyaData * lyd = GData::pet.getLingyaTable(getId());
        if(lyd != NULL)
            return getPetBone() < lyd->finalBone;
        return false;
        /*
        if(0 == color)  //绿色仙宠
            return getPetBone() < 22;
        else if(1 == color)  //蓝色仙宠
            return getPetBone() < 42;
        else if(2 == color)  //紫色仙宠
            return getPetBone() < 62;
        else if(3 == color)  //橙色仙宠
            return getPetBone() < 82;
        */
    }

    void FairyPet::upgradeLev()
    {
        if(!isPet() || !_owner) return;
        if(!canLevUp())
            return;
        GData::Pet::PinjieData * pjd = GData::pet.getLevTable(getPetLev());
        if(!pjd) return;
        UInt32 longYuan = _owner->GetVar(VAR_FAIRYPET_LONGYUAN);
        if(longYuan < pjd->consume)
            return;
        ConsumeInfo ci(PinjieUpForPet, 0, 0);
        _owner->useLongyuan(pjd->consume, &ci);
        if(GRND(PROB_BASE) <= pjd->prob + getPinjieBless())
        {   //成功
            levUp();
            reset(1);
            setLevel(getPetLev());
            updateToDB(2, getLevel());
            if(GET_REMAINDER(getPetLev()))
                _owner->sendMsgCode(0, 4000);
        }
        else
        { //失败
            addPinjieBless(1);
            if(GET_REMAINDER(getPetLev()) == 9)
                _owner->sendMsgCode(0, 4003);
            else
                _owner->sendMsgCode(0, 4001);
            UInt8 value = GET_REMAINDER(getPetLev()) + 10;
            _owner->fairyPetUdpLog(10000, value > 10 ? value : 20);
        }
        sendPinjieInfo();
        UpdateToDB();
    }

    void FairyPet::upgradeLevAuto()
    {
        if(!isPet() || !_owner) return;
        if(!canLevUp())
            return;
        GData::Pet::PinjieData * pjd = GData::pet.getLevTable(getPetLev());
        if(!pjd) return;
        UInt32 longYuan = _owner->GetVar(VAR_FAIRYPET_LONGYUAN);
        if(longYuan < pjd->consume)
            return;
        UInt8 num = 0, isSucc = 0;
        UInt32 used = 0;
        while(true)
        {
            if(longYuan < pjd->consume + used)
                break;
            ++ num;
            used += pjd->consume;
            if(GRND(PROB_BASE) <= pjd->prob + getPinjieBless())
            {   //成功
                levUp();
                reset(1);
                setLevel(getPetLev());
                updateToDB(2, getLevel());
                isSucc = 1;
                break;
            }
            else
            {   //失败
                addPinjieBless(1);
                UInt8 value = GET_REMAINDER(getPetLev()) + 10;
                _owner->fairyPetUdpLog(10000, value > 10 ? value : 20);
            }
        }
        ConsumeInfo ci(PinjieUpForPet, 0, 0);
        _owner->useLongyuan(used, &ci);
        UpdateToDB();
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x03);
        st << getId() << isSucc << num;
        st << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::upgradeBone()
    {
        if(!isPet() || !_owner) return;
        if(!canBoneUp())
            return;
        GData::Pet::GenguData * ggd = GData::pet.getBoneTable(getPetBone());
        if(!ggd) return;
        if(getChongNum() < ggd->limit)
            return;
        if(GRND(PROB_BASE) <= ggd->baseProb + getGenguBless())
        {   //成功
            reset(2);
            reset(3);
            boneUp();
            //_owner->sendMsgCode(0, 4002);
            if(GET_REMAINDER(getPetBone()) == 2 && getColor() > 1)
                SYSMSG_BROADCASTV(4135, _owner->getCountry(), _owner->getName().c_str(), getColor(), getName().c_str(), getPetBone());
        }
        else
        {   //失败
            UInt16 sub = (getChongNum()+ 1) / 2;
            addChongNum(0 - sub);
            if(getChongNum() < ggd->limit)
                reset(2);
            else
                addGenguBless(0 - 100*sub);
            _owner->sendMsgCode(0, 4003);
        }
        sendGenguInfo();
        UpdateToDB();
    }

    void FairyPet::reset(UInt8 type)
    {
        switch(type)
        {
            case 1:
                _pinjieBless = 0;
                break;
            case 2:
                _genguBless = 0;
                break;
            case 3:
                _chong = 0;
                break;
            case 4:
                _dazhou = 0;
                _xiaozhou = 0;
            default:
                break;
        }
    }

    void FairyPet::useZhoutian(UInt8 type)
    {
#define FREE_LIMIT 7
        GData::Pet::GenguData * ggd = GData::pet.getBoneTable(getPetBone());
        if(!ggd) return;
        if(getGenguBless() + ggd->baseProb >= PROB_BASE)
            return;
        UInt32 fengSui = _owner->GetVar(VAR_FAIRYPET_FENGSUI);
        UInt8 chong = 0;
        bool isLucky = GRND(PROB_BASE) <= 100 ? true : false;
        if(!type)
        {
            UInt32 xiaoZhou = getXiaozhou();
            if(xiaoZhou / 10 >= FREE_LIMIT && !GET_REMAINDER(xiaoZhou))
            {
                xiaoZhou = (xiaoZhou / 10 + 1) * 10 + 1;
            }
            else
            {
                if(fengSui < ggd->consume1)
                    return;
                ConsumeInfo ci(GenguUpForPet, 0, 0);
                _owner->useFengsui(ggd->consume1, &ci);
                xiaoZhou = (xiaoZhou / 10 + 1) * 10 + GET_REMAINDER(xiaoZhou);
            }
            if(isLucky)
            {
                chong = 2;
		        SYSMSG_SENDV(4132, _owner, chong);
            }
            else
                chong = 1;
            setXiaozhou(xiaoZhou);
            int num = getChongNum() + chong - ggd->limit;
            if(num > 0)
                addGenguBless(num * 100);
        }
        else
        {
            UInt32 daZhou = getDazhou();
            if(daZhou / 10 >= FREE_LIMIT && !GET_REMAINDER(daZhou))
            {
                daZhou = (daZhou / 10 + 1) * 10 + 1;
            }
            else
            {
                if(fengSui < ggd->consume2)
                    return;
                ConsumeInfo ci(GenguUpForPet, 0, 0);
                _owner->useFengsui(ggd->consume2, &ci);
                daZhou = (daZhou / 10 + 1) * 10 + GET_REMAINDER(daZhou);
            }
            if(isLucky)
            {
                chong = 6;
		        SYSMSG_SENDV(4132, _owner, chong);
            }
            else
                chong = 3;
            setDazhou(daZhou);
            int num = getChongNum() + chong - ggd->limit;
            if(num > 0)
                addGenguBless(num * 100);
        }
        addChongNum(chong);
        sendGenguInfo();
        UpdateToDB();
    }

    void FairyPet::sendPinjieInfo()
    {
        GData::Pet::PinjieData * pjd = GData::pet.getLevTable(getPetLev());
        if(!pjd || !_owner) return;
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x00);
        st << getId();
        st << _owner->GetVar(VAR_FAIRYPET_LONGYUAN);
        st << getPetLev() << _pinjieBless;
        st << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::sendGenguInfo()
    {
        GData::Pet::GenguData * ggd = GData::pet.getBoneTable(getPetBone());
        if(!ggd || !_owner) return;
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01);
        st << getId();
        st << _owner->GetVar(VAR_FAIRYPET_FENGSUI);
        st << getPetBone();
        st << getXiaozhou();
        st << getDazhou();
        st << getChongNum();
        st << static_cast<UInt32>(getGenguBless() + ggd->baseProb);
        st << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::sendPetEvolve()
    {
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x07);
        st << static_cast<UInt8>(0);
        st << getId() << getPetEvolve() << Stream::eos;
        _owner->send(st);
    }

    UInt16 FairyPet::addPetEvolveInlua(UInt16 num)
    {
        UInt8 evolve = getPetEvolve();
        if(!num || evolve >= getPetEvMax() || getPetBone() < EVOLVE_BONE_LIMIT || getColor() != 2)
            return 0;
        UInt32 newId = GameAction()->getYellowPetId(getId());
        if(_owner->findFairyPet(newId))
            return 0;
        UInt16 n = 0;
        for(UInt16 i = 0; i < num; ++ i)
        {
            ++ n;
            evolve += (GRND(9) + 1);
            if(evolve >= getPetEvMax())
                break;
        }
        setPetEvolve(evolve);
        UpdateToDB();
        sendPetEvolve();
        return n;
    }
}

