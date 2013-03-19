#include "Config.h"
#include "GObjectDBExecHelper.h"
#include "GData/FairyPetTable.h"
#include "Common/URandom.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "MsgID.h"
#include "FairyPet.h"


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
        if(isDel)
            DB2().PushUpdateData("DELETE FROM `fairyPet` WHERE id = %u AND `playerId` = %"I64_FMT"u", getId(), _owner->getId());
        else
            DB2().PushUpdateData("REPLACE INTO `fairyPet` (`id`, `playerId`, `onBattle`, `petLev`, `petBone`, `pinjieBless`, `genguBless`, `chong`, `overTime`, `xiaozhou`, `dazhou`) VALUES (%u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", 
            getId(), _owner->getId(), _onBattle, _petLev, _petBone, _pinjieBless, _genguBless, _chong, _overTime, _xiaozhou, _dazhou);
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
        if(_petBone % 10 > 2)
            _petBone = (_petBone / 10 + 1) * 10;
        GData::Pet::GenguData * ggd = GData::pet.getBoneTable(_petBone);
        setPotential(ggd->growRate);
    }

    void FairyPet::levUp()
    {
        ++ _petLev;
        setLevel(_petLev);
        updateToDB(2, _petLev);
        //触发初始技能升级
        if(_petLev % 10 == 8 || _petLev % 10 == 9)
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
    {
        return GData::pet.getPinjieBless(getPetLev(), _pinjieBless);
    }

    bool FairyPet::canLevUp()
    {
        UInt8 limit = _owner->getMainFighter()->getLevel() / 10 * 10 + 9;
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
        _owner->SetVar(VAR_FAIRYPET_LONGYUAN, longYuan - pjd->consume);
        if(GRND(10000) <= pjd->prob + getPinjieBless())
        {   //成功
            levUp();
            reset(1);
            setLevel(getPetLev());
            updateToDB(2, getLevel());
			SYSMSG_SENDV(4132, _owner);
        }
        else
        { //失败
            addPinjieBless(1);
			SYSMSG_SENDV(4133, _owner);
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
        while(true)
        {
            longYuan = _owner->GetVar(VAR_FAIRYPET_LONGYUAN);
            if(longYuan < pjd->consume)
                break;
            ++ num;
            _owner->SetVar(VAR_FAIRYPET_LONGYUAN, longYuan - pjd->consume);
            if(GRND(10000) <= pjd->prob + getPinjieBless())
            {   //成功
                levUp();
                reset(1);
                setLevel(getPetLev());
                isSucc = 1;
                updateToDB(2, getLevel());
                break;
            }
            else
            {   //失败
                addPinjieBless(1);
            }
        }
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
        if(GRND(10000) <= ggd->baseProb + getGenguBless())
        {   //成功
            reset(2);
            reset(3);
            boneUp();
			SYSMSG_SENDV(4134, _owner);
        }
        else
        {   //失败
            addChongNum(-5);
            if(getChongNum() < ggd->limit)
                reset(2);
            else
                addGenguBless(-500);
			SYSMSG_SENDV(4135, _owner);
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
        UInt32 fengSui = _owner->GetVar(VAR_FAIRYPET_FENGSUI);
        UInt8 chong = 0;
        bool isLucky = GRND(10000) <= 100 ? true : false;
        if(!type)
        {
            if(fengSui < ggd->consume1)
                return;
            UInt32 xiaoZhou = getXiaozhou();
            if(xiaoZhou / 10 >= FREE_LIMIT && !(xiaoZhou % 10)) 
            {
                xiaoZhou = (xiaoZhou / 10 + 1) * 10 + 1;
            }
            else
            {
                _owner->SetVar(VAR_FAIRYPET_FENGSUI, fengSui - ggd->consume1);
                xiaoZhou = (xiaoZhou / 10 + 1) * 10 + xiaoZhou % 10;
            }
            if(isLucky)
            {
                chong = 2;
		        SYSMSG_SENDV(4136, _owner, chong);
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
            if(fengSui < ggd->consume2)
                return;
            UInt32 daZhou = getDazhou();
            if(daZhou / 10 >= FREE_LIMIT && !(daZhou % 10))
            {
                daZhou = (daZhou / 10 + 1) * 10 + 1;
            }
            else
            {
                _owner->SetVar(VAR_FAIRYPET_FENGSUI, fengSui - ggd->consume2);
                daZhou = (daZhou / 10 + 1) * 10 + daZhou % 10;
            }
            if(isLucky)
            {
                chong = 6;
		        SYSMSG_SENDV(4136, _owner, chong);
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
        st << getPetLev() << getPinjieBless();
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

}

