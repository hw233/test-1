#include "Config.h"
#include "GObjectDBExecHelper.h"
#include "GData/FairyPetTable.h"
#include "GData/SkillTable.h"
#include "Common/URandom.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "Script/BattleFormula.h"
#include "Server/SysMsg.h"
#include "Log/Log.h"
#include "Fighter.h"
#include "MsgID.h"
#include "FairyPet.h"
#include "Script/GameActionLua.h"
#include "GObject/Country.h"
#include "GData/HunPoData.h"
#include "Package.h"
#include "Item.h"
#include "GData/SevenSoul.h"


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
        memset(_equips, 0, PET_EQUIP_UPMAX * sizeof(ItemPetEq *));

        memset(_soulLevel, 0, sizeof(_soulLevel));
        memset(_skillIndex, 0, sizeof(_skillIndex));
        _petType = getSevenSoulPetType();
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
		ItemPetEq * equip = static_cast<ItemPetEq *>(GObjectManager::fetchEquipment(data.equip1, false));
        //setEquip(equip, 0, false);
        _equips[0] = equip;
		equip = static_cast<ItemPetEq *>(GObjectManager::fetchEquipment(data.equip2, false));
        //setEquip(equip, 1, false);
        _equips[1] = equip;
		equip = static_cast<ItemPetEq *>(GObjectManager::fetchEquipment(data.equip3, false));
        //setEquip(equip, 2, false);
        _equips[2] = equip;

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

    void FairyPet::AddSHFromDB(DBSanHun & shData)
    {
        m_SanHunLvl.insert(std::make_pair(shData.sanhunId, shData.curLvl));
    }

    void FairyPet::UpdateToDB()
    {
        _overTime = _overTime ? _overTime : TimeUtil::SharpDayT(1);
        DB2().PushUpdateData("REPLACE INTO `fairyPet` (`id`, `playerId`, `onBattle`, `petLev`, `petBone`, `pinjieBless`, `genguBless`, `chong`, `overTime`, `xiaozhou`, `dazhou`, `evolve`, `equip1`, `equip2`, `equip3`) VALUES (%u, %" I64_FMT "u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", 
                getId(), _owner->getId(), _onBattle, _petLev, _petBone, _pinjieBless, _genguBless, _chong, _overTime, _xiaozhou, _dazhou,
                _evolve, _equips[0] ? _equips[0]->getId() : 0, _equips[1] ? _equips[1]->getId() : 0, _equips[2] ? _equips[2]->getId() : 0);
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
        pet->_innateTrump = NULL;
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
        setDirty();
    }

    void FairyPet::delSkills(std::string& skills)
    {
        StringTokenizer tk(skills, ",");
        const GData::SkillBase* s = NULL;
        std::vector<const GData::SkillBase*> vt_skills;
        for (size_t i = 0; i < tk.count(); ++i)
        {
            s = GData::skillManager[::atoi(tk[i].c_str())];
            if (s)
                vt_skills.push_back(s);
        }
        if (vt_skills.size())
            delSkillsFromCT(vt_skills, false);
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
            GameAction()->doStrong(_owner, SthPinJieUp, 0, 0); 
            _owner->GuangGunCompleteTask(0,13);
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
            GameAction()->doStrong(_owner, SthPinJieUp, 0, 0); 
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
                GameAction()->doStrong(_owner, SthPinJieUp, 0, 0); 
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
            GameAction()->doStrong(_owner, SthGenguUp, 0, 0); 
            _owner->GuangGunCompleteTask(0,14);
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
            GameAction()->doStrong(_owner, SthGenguUp, 0, 0); 
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

    void FairyPet::AppendEquipData(Stream& st)
    {
        size_t pos = st.size();
        UInt8 count = 0;
        st << count;
        for(UInt8 i = 0; i < PET_EQUIP_UPMAX; ++ i)
        {
            if(_equips[i])
            {
                count |= (1<<i);
                st << _equips[i]->getId() << static_cast<UInt8>(_equips[i]->GetBindStatus() ? 1 : 0);
                st << static_cast<UInt16>(_equips[i]->GetItemType().getId());
                _equips[i]->getPetEqAttr().appendAttrToStream(st);
            }
        }
        st.data<UInt8>(pos) = count;
    }

    void FairyPet::sendModification(ItemPetEq * equip, UInt8 pos)
    {
		Stream st(REP::FAIRY_PET);
		st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x04);
        st << getId() << pos;
        if(equip)
        {
            st << equip->getId() << static_cast<UInt8>(equip->GetBindStatus() ? 1 : 0);
            st << equip->GetTypeId();
            equip->getPetEqAttr().appendAttrToStream(st);
        }
		st << Stream::eos;
		_owner->send(st);
    }

    ItemPetEq * FairyPet::findEquip(UInt8 pos)
    {
        if(pos >= PET_EQUIP_UPMAX)
            return NULL;
        return _equips[pos];
    }

    ItemPetEq * FairyPet::findEquip(UInt32 id, UInt8& pos)
    {
        pos = 0xFF;
        for(UInt8 i = 0; i < PET_EQUIP_UPMAX; ++ i)
        {
            if(_equips[i] && _equips[i]->getId() == id)
            {
                pos = i;
                return _equips[i];
            }
        }
        return NULL;
    }

    ItemPetEq * FairyPet::setEquip(ItemPetEq * eq, UInt8& pos, bool writedb)
    {
        if(pos >= PET_EQUIP_UPMAX)
        {
            pos = 0xFF;
            return eq;
        }
        for(UInt8 i = 0; i < PET_EQUIP_UPMAX; ++ i)
        {
            if(i != pos && _equips[i] && eq && _equips[i]->getClass() == eq->getClass())
            {
                pos = 0xFF;
                return eq;
            }
        }
        ItemPetEq * old = _equips[pos];
        if(old == NULL && eq == NULL)
        {
            pos = 0xFF;
            return old;
        }
        setDirty();
        _equips[pos] = eq;
        if(writedb)
        {
            sendModification(eq, pos);
            UpdateToDB();
        }
        std::string skills = "";
        UInt16 oldskill = 0;
        if (old)
        {
            UInt16 oldskill = old->getPetEqAttr().skill;
            UInt16 newskill = 0;
            for(int i = 0; i < PET_EQUIP_UPMAX; ++ i)
            {
                if(_equips[i] != NULL)
                {
                    UInt16 skill1 = _equips[i]->getPetEqAttr().skill;
                    if(SKILL_ID(oldskill) == SKILL_ID(skill1) && newskill < skill1)
                    {
                        newskill = skill1;
                    }
                }
            }
            if(newskill == 0 || newskill != oldskill)
            {
                skills = Itoa(oldskill);
                delSkills(skills);
            }
        }
        if (eq)
        {
            eq->DoEquipBind(true);
            UInt16 eqskill = eq->getPetEqAttr().skill;
            bool find = false;
            if(SKILL_ID(oldskill) != SKILL_ID(eqskill))
            {
                for(int i = 0; i < PET_EQUIP_UPMAX; ++ i)
                {
                    if(_equips[i] != NULL && _equips[i] != eq)
                    {
                        UInt16 skill1 = _equips[i]->getPetEqAttr().skill;
                        if(SKILL_ID(eqskill) == SKILL_ID(skill1) && eqskill <= skill1)
                        {
                            find = true;
                            break;
                        }
                    }
                }
                if(!find)
                {
                    skills = Itoa(eq->getPetEqAttr().skill);
                    setSkills(skills, false);
                }
            }
        }
        updateToDBPetSkill();
        return old;
    }

    void FairyPet::upgradeEquipSkill(ItemPetEq * eq)
    {
        if(!eq)
            return;

        UInt16 eqskill = eq->getPetEqAttr().skill;
        bool find = false;
        for(int i = 0; i < PET_EQUIP_UPMAX; ++ i)
        {
            if(_equips[i] != NULL && _equips[i] != eq)
            {
                UInt16 skill1 = _equips[i]->getPetEqAttr().skill;
                if(SKILL_ID(eqskill) == SKILL_ID(skill1) && eqskill <= skill1)
                {
                    find = true;
                    break;
                }
            }
        }
        if(!find)
        {
            std::string skills = Itoa(eq->getPetEqAttr().skill);
            setSkills(skills, false);
            updateToDBPetSkill();
        }
    }

    void FairyPet::rebuildEquipAttr()
    {
	    _attrExtraEquip.reset();
        float effect = 0;
        for(UInt8 i = 0; i < PET_EQUIP_UPMAX; ++ i)
        {
            if(_equips[i] == NULL)
                continue;
            ItemPetEqAttr& peAttr = _equips[i]->getPetEqAttr();
            effect = GData::pet.getEquipAttreffect(peAttr.lv, _equips[i]->getClass());
            switch(_equips[i]->getQuality())
            {
                case Item_Green:
                    effect *= 0.4;
                    break;
                case Item_Blue:
                    effect *= 0.6;
                    break;
                case Item_Purple:
                    effect *= 0.8;
                    break;
                case Item_Yellow:
                    effect *= 1.0;
                    break;
                default:
                    effect *= 0.4;
                    break;
            }
            if(effect <= 0)
                continue;
            switch(_equips[i]->getClass())
            {
                case Item_PetEquip:
                    _attrExtraEquip.hp += effect;
                    break;
                case Item_PetEquip1:
                    _attrExtraEquip.attack += effect;
                    break;
                case Item_PetEquip2:
                    _attrExtraEquip.magatk += effect;
                    break;
                case Item_PetEquip3:
                    _attrExtraEquip.defend += effect;
                    break;
                case Item_PetEquip4:
                    _attrExtraEquip.magdef += effect;
                    break;
                case Item_PetEquip5:
                    _attrExtraEquip.crilvl += effect;
                    break;
                case Item_PetEquip6:
                    _attrExtraEquip.pirlvl += effect;
                    break;
                case Item_PetEquip7:
                    _attrExtraEquip.hitrlvl += effect;
                    break;
                case Item_PetEquip8:
                    _attrExtraEquip.evdlvl += effect;
                    break;
                case Item_PetEquip9:
                    _attrExtraEquip.counterlvl += effect;
                    break;
                case Item_PetEquip10:
                    _attrExtraEquip.toughlvl += effect;
                    break;
                default:
                    break;
            }
            for(UInt8 i = 0; i < sizeof(peAttr.gems)/sizeof(peAttr.gems[0]); ++ i)
            {
                if(peAttr.gems[i] == 0 || GetItemSubClass(peAttr.gems[i]) != Item_PetGem)
                    continue;
                GData::ItemGemType * igt = GData::petGemTypes[peAttr.gems[i] - LPETGEM_ID];
                addAttrExtra(_attrExtraEquip, igt->attrExtra);
            }
        }
        addSHAttr(_attrExtraEquip);
        //七魄属性
        _attrExtraEquip.hitrlvl += 4 * _soulLevel[0];
        _attrExtraEquip.evdlvl += 4 * _soulLevel[1];
        _attrExtraEquip.crilvl += 4 * _soulLevel[2];
        _attrExtraEquip.toughlvl += 4 * _soulLevel[3];
        _attrExtraEquip.defend += 240 * _soulLevel[4];
        _attrExtraEquip.magdef += 240 * _soulLevel[4];
        _attrExtraEquip.attack += 60 * _soulLevel[5];
        _attrExtraEquip.magatk += 60 * _soulLevel[5];
        _attrExtraEquip.hp += 400 * _soulLevel[6];

	    _maxHP = Script::BattleFormula::getCurrent()->calcHP(this);
        
    }

    void FairyPet::GMSHUpLvl(UInt32 petId, UInt8 sanhunId, UInt8 lvl)
    {
        if(!_owner)
            return;

        UInt8 sanhunLvl = 0;
        bool mark = false;

        std::map<UInt8, UInt8>::iterator iter = m_SanHunLvl.find(sanhunId);
        if(iter == m_SanHunLvl.end())
            mark = true;
        else
            sanhunLvl = iter->second;

        UInt16 count = 0;
        UInt32 money = 0;
        for(UInt8 i=0; i<lvl; i++)
        {
            bool res = checkSanHunUp(sanhunId, sanhunLvl);
            if(!res)
                return;

            GData::HunPoData::sanhunInfo * shInfo = GData::hunpoData.getSanHunInfo(sanhunLvl+1);
            if(!shInfo)
                return;

            if(shInfo->money2 > 0)
            {
                count += shInfo->money2;
            }

            if(shInfo->money1 > 0)
            {
                money += shInfo->money1;
            }
                
            sanhunLvl++;
        }

        UInt16 toolCount = _owner->GetPackage()->GetItemAnyNum(9427);
        if(toolCount >= count)
        {
            if(count > 0)
            {
                if(!_owner->GetPackage()->DelItemAny(9427, count, NULL, ToSanHunUp))
                    return;
            }
        }
        else
            return;

        UInt32 shouhun = _owner->GetVar(VAR_FAIRYPET_SHOUHUN);
        if(shouhun >= money)
        {
            if(money > 0)
            {
                _owner->SetVar(VAR_FAIRYPET_SHOUHUN, shouhun - money);
                SYSMSG_SENDV(4947, _owner, money);
                SYSMSG_SENDV(4948, _owner, money);
            }
        }
        else
            return;

        if(mark)
            m_SanHunLvl.insert(std::make_pair(sanhunId, sanhunLvl));
        else
            iter->second = sanhunLvl;

        DB4().PushUpdateData("REPLACE INTO `fairyPet_sanhun` VALUES(%u, %" I64_FMT "u, %u, %u)", getId(), _owner->getId(), sanhunId, sanhunLvl);

        setDirty();

        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x07);
        st << static_cast<UInt8>(1);
        st << petId << sanhunId << sanhunLvl << static_cast<UInt32>(shouhun - money);
        st << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::upgradeSH(UInt32 petId, UInt8 sanhunId, UInt8 opt)
    {
        if(!_owner)
            return;

        UInt8 sanhunLvl = 0;
        bool mark = false;

        std::map<UInt8, UInt8>::iterator iter = m_SanHunLvl.find(sanhunId);
        if(iter == m_SanHunLvl.end())
            mark = true;
        else
            sanhunLvl = iter->second;

        bool res = checkSanHunUp(sanhunId, sanhunLvl);
        if(!res)
            return;

        GData::HunPoData::sanhunInfo * shInfo = GData::hunpoData.getSanHunInfo(sanhunLvl+1);
        if(!shInfo)
            return;

        if(shInfo->money2 > 0)
        {
            if(!_owner->GetPackage()->DelItemAny(9427, shInfo->money2, NULL, ToSanHunUp))
                return;
        }
        
        UInt32 curShouHun = 0;
        if(shInfo->money1 > 0)
        {
            UInt32 shouhun = _owner->GetVar(VAR_FAIRYPET_SHOUHUN);
            UInt32 money = shInfo->money1;
            curShouHun =  shouhun - money;

            _owner->SetVar(VAR_FAIRYPET_SHOUHUN, curShouHun);
            SYSMSG_SENDV(4947, _owner, money);
            SYSMSG_SENDV(4948, _owner, money);

            DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%" I64_FMT "u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, _owner->getId(), SanHunUpForPet, 0, 0, money, TimeUtil::Now());
        }

        sanhunLvl++;

        if(mark)
            m_SanHunLvl.insert(std::make_pair(sanhunId, sanhunLvl));
        else
            iter->second = sanhunLvl;

        DB4().PushUpdateData("REPLACE INTO `fairyPet_sanhun` VALUES(%u, %" I64_FMT "u, %u, %u)",getId(), _owner->getId(), sanhunId, sanhunLvl);

        setDirty();

        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x07);
        st << opt;
        st << petId <<sanhunId << sanhunLvl << curShouHun;
        st << Stream::eos;
        _owner->send(st);
        _owner->GuangGunCompleteTask(0,12);
    }

    bool FairyPet::checkSanHunUp(UInt8 sanhunId, UInt8 sanhunLvl)
    {
        if(!_owner)
            return false;
        
        if(_owner->GetLev() < 70)
            return false;

        if(sanhunLvl >= SANHUN_MAXLVL)
            return false;

        GData::HunPoData::sanhunInfo * shInfo = GData::hunpoData.getSanHunInfo(sanhunLvl+1);
        if(!shInfo)
            return false;

        if(shInfo->money1 > 0)
        {
            UInt32 shouhun = _owner->GetVar(VAR_FAIRYPET_SHOUHUN);
            UInt32 money = shInfo->money1;
            if(shouhun < money)
                return false;
        }

        if(shInfo->money2 > 0)
        {
            UInt16 toolCount = _owner->GetPackage()->GetItemAnyNum(9427);
            UInt16 count = shInfo->money2;
            if(toolCount < count)
                return false;
        }

        UInt8 tempLvlA = 0;
        UInt8 tempLvlB = 0;
        switch(sanhunId)
        {
            case MINGHUN:
                tempLvlA = findSHLvl(TIANHUN);
                tempLvlB = findSHLvl(DIHUN);
                break;
            case TIANHUN:
                tempLvlA = findSHLvl(MINGHUN);
                tempLvlB = findSHLvl(DIHUN);
                break;
            case DIHUN:
                tempLvlA = findSHLvl(TIANHUN);
                tempLvlB = findSHLvl(MINGHUN);
                break;
        }

        if((sanhunLvl+1) <= 20)
        {
            return true;
        }
        else if((sanhunLvl+1) <= 40)
        {
            if(tempLvlA < 20 || tempLvlB < 20)
                return false;
        }
        else if((sanhunLvl+1) <= 60)
        {
            if(tempLvlA < 40 || tempLvlB < 40)
                return false;
        }
        else if((sanhunLvl+1) <= 80)
        {
            if(tempLvlA < 60 || tempLvlB < 60)
                return false;
        }
        else if((sanhunLvl+1) <= 100)
        {
            if(tempLvlA < 80 || tempLvlB < 80)
                return false;
        }

        return true;
    }

    UInt8 FairyPet::findSHLvl(UInt8 sanhunId)
    {
        std::map<UInt8, UInt8>::iterator iter = m_SanHunLvl.find(sanhunId);
        if(iter != m_SanHunLvl.end())
            return iter->second;

        return 0;
    }

    void FairyPet::setSHLvl(UInt8 sanhunId, UInt8 lvl)
    {
        std::map<UInt8, UInt8>::iterator iter = m_SanHunLvl.find(sanhunId);
        if(iter != m_SanHunLvl.end())
             iter->second = lvl;
        else
            m_SanHunLvl.insert(std::make_pair(sanhunId, lvl));

        DB4().PushUpdateData("REPLACE INTO `fairyPet_sanhun` VALUES(%u, %" I64_FMT "u, %u, %u)",getId(), _owner->getId(), sanhunId, lvl);
    }

    void FairyPet::addSHAttr(GData::AttrExtra& ae)
    {
        if(!_owner)
            return;

        std::map<UInt8, UInt8>::iterator iter = m_SanHunLvl.begin();
        for(; iter != m_SanHunLvl.end(); iter++)
        {
            GData::HunPoData::sanhunInfo * shInfo = GData::hunpoData.getSanHunInfo(iter->second);
            if(!shInfo)
                continue;

            if(shInfo->sanhunLvl > 0 && shInfo->sanhunLvl <= 100)
            {
                switch(iter->first)
                {
                    case MINGHUN:
                        ae.hp += shInfo->attr1;
                        ae.attack += shInfo->attr2;
                        ae.defend += shInfo->attr3;
                        ae.magatk += shInfo->attr2;
                        ae.magdef += shInfo->attr3;
                        break;
                    case TIANHUN:
                        ae.crilvl += shInfo->attr4;
                        ae.pirlvl += shInfo->attr5;
                        ae.hitrlvl += shInfo->attr6;
                        break;
                    case DIHUN:
                        ae.evdlvl += shInfo->attr7;
                        ae.counterlvl += shInfo->attr8;
                        ae.toughlvl += shInfo->attr9;
                        break;
                }
            }
        }
    }

    void FairyPet::sendHunPoInfo(Stream & st)
    {
        UInt8 count = 0;
        UInt16 offset = 0;

        offset = st.size();
        st << count;
        std::map<UInt8, UInt8>::iterator iter = m_SanHunLvl.begin();
        for(; iter != m_SanHunLvl.end(); iter++)
        {
            st << iter->first << iter->second;
            count++;
        }

        st.data<UInt8>(offset) = count;
    }

    UInt8 FairyPet::getSevenSoulPetType()
    {
        UInt32 petId = getId();
        UInt8 petType;
        switch(petId)
        {
            case 501:
            case 502:
                petType =1;
            break;
            case 503:
                petType =2;
            break;
            case 513:
                petType =3;
            break;
            case 504:
            case 505:
                petType =4;
            break;
            case 506:
                petType =5;
            break;
            case 514:
                petType =6;
            break;
            case 507:
            case 508:
                petType =7;
            break;
            case 509:
                petType =8;
            break;
            case 515:
                petType =9;
            break;
            case 510:
            case 511:
                petType =10;
            break;
            case 512:
                petType =11;
            break;
            case 516:
                petType =12;
            break;
            default:
                petType =1;
            break;
        }
        return petType;
    }

    bool FairyPet::checkSevenSoulLevel(UInt8 soulIndex)
    {
        if(!_owner)
            return false;
        if(soulIndex == 0 || soulIndex > 7)
            return false;

        if(_soulLevel[soulIndex - 1] == 0)
        {
            _soulLevel[soulIndex - 1] = 1;
            DB4().PushUpdateData("REPLACE INTO `pet_sevensoul` VALUES(%" I64_FMT "u, %u, %u, %u, 0)", _owner->getId(), getId(), soulIndex, _soulLevel[soulIndex - 1]);
            setDirty();

            UInt16 skillId = GData::sevenSoul.getSkillId(_petType, soulIndex, _skillIndex[soulIndex - 1]);
            if(soulIndex > 4)
            {
                UInt16 skill_id = SKILLANDLEVEL(skillId, 1);
                std::string skills = Itoa(skill_id);
                setSkills(skills, false);
                updateToDBPetSkill();
            }
            UInt16 ssLevel = GData::sevenSoul.getSkillLevel(_soulLevel[soulIndex - 1]);
            petSSAdd(SKILLANDLEVEL(skillId, ssLevel));
            return true;
        }
        return false;
    }

    void FairyPet::sendSevenSoul()
    {
        if(!_owner)
            return;

        if(_owner->GetLev() >= 80)
            checkSevenSoulLevel(1);

        const UInt8 type = 0;
        const UInt8 count = 7;
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x08) << type << getId() << _owner->getXianpo() << count;
        for(UInt8 i = 0; i < count; i++)
        {
            st << _soulLevel[i];
            st << _skillIndex[i];
        }
        st << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::upgradeSevenSoul(UInt8 sevenSoulIndex)
    {
        if(sevenSoulIndex >= 7)
            return;
        if(!_owner)
            return;
        UInt8 soulLevel = _soulLevel[sevenSoulIndex];
        if(soulLevel == 0 || soulLevel >= 25)
            return;

        UInt32 needNum = GData::sevenSoul.getNeedSoulNum(soulLevel);
        UInt32 curNum = _owner->getXianpo();
        UInt8 ret;
        if(curNum >= needNum)
        {
            ConsumeInfo ci(SevenSoulUpgrade, 0, 0);
            _owner->useXianpo(needNum, &ci);

            UInt8 oldSSLevel = GData::sevenSoul.getSkillLevel(_soulLevel[sevenSoulIndex]);
            ++_soulLevel[sevenSoulIndex];
            DB4().PushUpdateData("UPDATE `pet_sevensoul` SET `soulLevel` = %u WHERE `playerId` = %" I64_FMT "u AND petId = %u AND soulId = %u", _soulLevel[sevenSoulIndex], _owner->getId(), getId(), sevenSoulIndex + 1);
            setDirty();

            UInt8 newSSLevel = GData::sevenSoul.getSkillLevel(_soulLevel[sevenSoulIndex]);
            if(oldSSLevel != newSSLevel)
            {
                UInt16 skillId = GData::sevenSoul.getSkillId(_petType, sevenSoulIndex + 1, _skillIndex[sevenSoulIndex]);
                petSSAdd(SKILLANDLEVEL(skillId, newSSLevel));
            }

            if(sevenSoulIndex < 6 && (_soulLevel[sevenSoulIndex] >= GData::sevenSoul.getConditonValue(sevenSoulIndex + 2)))
            {
                checkSevenSoulLevel(sevenSoulIndex + 2);
            }

            ret = 0;
        }
        else
            ret = 1;

        UInt8 type = 1;
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x08) << type << getId() << sevenSoulIndex << ret << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::switchSevenSoulSkill(UInt8 sevenSoulIndex, UInt8 skillIndex)
    {
        if(sevenSoulIndex >= 7)
            return;
        if(skillIndex >= 2)
            return;
        if(!_owner)
            return;

        UInt8 aimSkillIndex;
        if(_skillIndex[sevenSoulIndex] == 0)
            aimSkillIndex = 1;
        else
            aimSkillIndex = 0;

        UInt8 ret;
        if(aimSkillIndex == skillIndex)
        {
            std::string oldSkill = "";
            std::string newSkill = "";
            ret = 0;

            UInt16 oldSkillId = GData::sevenSoul.getSkillId(_petType, sevenSoulIndex + 1, _skillIndex[sevenSoulIndex]);
            UInt16 sLevel = getSkillLevelBySkill(oldSkillId);

            _skillIndex[sevenSoulIndex] = skillIndex;
            DB4().PushUpdateData("UPDATE `pet_sevensoul` SET `skillIndex` = %u WHERE `playerId` = %" I64_FMT "u AND petId = %u AND soulId = %u", skillIndex, _owner->getId(), getId(), sevenSoulIndex + 1);

            UInt16 newSkillId = GData::sevenSoul.getSkillId(_petType, sevenSoulIndex + 1, _skillIndex[sevenSoulIndex]);

            petSSErase(oldSkillId);
            UInt16 old_skill_id = SKILLANDLEVEL(oldSkillId, sLevel);
            oldSkill = Itoa(old_skill_id);
            delSkills(oldSkill);

            UInt16 new_skill_id = SKILLANDLEVEL(newSkillId, sLevel);
            newSkill = Itoa(new_skill_id);
            setSkills(newSkill, false);
            UInt16 ssLevel = GData::sevenSoul.getSkillLevel(_soulLevel[sevenSoulIndex]);
            petSSAdd(SKILLANDLEVEL(newSkillId, ssLevel));
            for(UInt8 i = 0; i < INIT_SKILL_UPMAX; ++ i)
            {
                if(SKILL_ID(_initskl[i]) == oldSkillId)
                    _initskl[i] = SKILLANDLEVEL(newSkillId, 1);
            }
#ifdef _DEBUG
            TRACE_LOG("delSkills(%s), addSkills(%s).", oldSkill.c_str(), newSkill.c_str());
#endif
            updateToDBPetSkill();
        }
        else
            ret = 1;

        const UInt8 type = 2;
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x08) << type << getId() << sevenSoulIndex << ret << Stream::eos;
        _owner->send(st);
    }

    void FairyPet::loadPlayerSevenSoul(UInt8 soulId, UInt8 soulLevel, UInt8 skillIndex)
    {
        if(soulId >= 1 && soulId <= 7)
        {
            _soulLevel[soulId - 1] = soulLevel;
            _skillIndex[soulId - 1] = skillIndex;
            loadSkillFromSevenSoul(soulId - 1);
        }
    }

    void FairyPet::loadSkillFromSevenSoul(UInt8 soulId)
    {
        if(soulId >= 7)
            return;
        if(_soulLevel[soulId] == 0 || _soulLevel[soulId] > 25)
            return;
        if(_skillIndex[soulId] >= 2)
            return;

        UInt16 skillId = GData::sevenSoul.getSkillId(_petType, soulId + 1, _skillIndex[soulId]);
        if (!skillId)
            return;

        UInt16 ssLevel = GData::sevenSoul.getSkillLevel(_soulLevel[soulId]);
        if(soulId >= INIT_SKILL_UPMAX)
            return petSSAdd(SKILLANDLEVEL(skillId, ssLevel));

        UInt16 conflictSkillId = GData::sevenSoul.getAnotherSimilarSkill(skillId);
        UInt8 i;
        bool update = true;
        for(i = 0; i < INIT_SKILL_UPMAX; ++ i)
        {
            if (SKILL_ID(_initskl[i]) == conflictSkillId)
            {
                _initskl[i] = SKILLANDLEVEL(skillId, 1);
                break;
            }
            else if (SKILL_ID(_initskl[i]) == skillId)
            {
                //if(GVAR.GetVar(GVAR_SEVENSOUL_FIX) > 0)
                    update = false;
                break;
            }
        }
        if(i >= INIT_SKILL_UPMAX)
            return;
        if(update)
        {
            GData::Pet::PinjieData* pjd = GData::pet.getLevTable(_petLev);
            UInt8 sLevel = 0;
            if(pjd)
                sLevel = pjd->skillLev[i];
            if(sLevel == 0) 
                sLevel = 1;
            std::string oldSkill = Itoa(SKILLANDLEVEL(conflictSkillId, sLevel));
            delSkills(oldSkill);
            std::string newSkill = Itoa(SKILLANDLEVEL(skillId, sLevel));
            setSkills(newSkill, false);
        }
        petSSAdd(SKILLANDLEVEL(skillId, ssLevel));
    }

    void FairyPet::updateSevenSoul(UInt8 soulId)
    {
        if(soulId >= 7)
            return;
        if(_soulLevel[soulId] == 0 || _soulLevel[soulId] > 25)
            return;
        if(_skillIndex[soulId] >= 2)
            return;

        UInt16 skillId = GData::sevenSoul.getSkillId(_petType, soulId + 1, _skillIndex[soulId]);
        if (!skillId)
            return;

        UInt16 ssLevel = GData::sevenSoul.getSkillLevel(_soulLevel[soulId]);
        if(soulId >= INIT_SKILL_UPMAX)
            return petSSAdd(SKILLANDLEVEL(skillId, ssLevel));

        UInt16 conflictSkillId = GData::sevenSoul.getAnotherSimilarSkill(skillId);
        UInt8 i;
        for(i = 0; i < INIT_SKILL_UPMAX; ++ i)
        {
            if (SKILL_ID(_initskl[i]) == conflictSkillId)
            {
                _initskl[i] = SKILLANDLEVEL(skillId, 1);
                break;
            }
            else if (SKILL_ID(_initskl[i]) == skillId)
            {
                break;
            }
        }
        if(i >= INIT_SKILL_UPMAX)
            return;
        if(1)
        {
            GData::Pet::PinjieData* pjd = GData::pet.getLevTable(_petLev);
            UInt8 sLevel = 0;
            if(pjd)
                sLevel = pjd->skillLev[i];
            if(sLevel == 0)
                sLevel = 1;
            std::string oldSkill = Itoa(SKILLANDLEVEL(conflictSkillId, sLevel));
            delSkills(oldSkill);
            std::string newSkill = Itoa(SKILLANDLEVEL(skillId, sLevel));
            setSkills(newSkill, false);
        }
        petSSAdd(SKILLANDLEVEL(skillId, ssLevel));
    }

    void FairyPet::deleteSevenSoul()
    {
        std::vector<const GData::SkillBase*> vSkills;
        for(UInt8 i = 0; i < 7; i++)
        {
            if(_soulLevel[i] == 0 || _soulLevel[i] > 25)
                continue;
            UInt16 skillId = GData::sevenSoul.getSkillId(_petType, i + 1, _skillIndex[i]);
            petSSErase(skillId);
        }
        DB4().PushUpdateData("DELETE FROM `pet_sevensoul` WHERE `playerId` = %" I64_FMT "u AND petId = %u", _owner->getId(), getId());
    }

    UInt16 FairyPet::getSkillLevelBySkill(UInt16 skillId)
    {
        GData::Pet::PinjieData* pjd = GData::pet.getLevTable(_petLev);
        if(!pjd)
            return 1;
        for(UInt8 i = 0; i < INIT_SKILL_UPMAX; ++ i)
        {
            if(SKILL_ID(_initskl[i]) == skillId)
            {
                UInt16 lev = pjd->skillLev[i];
                if(lev == 0)
                    lev = 1;
                return lev;
            }
        }
        return 1;
    }

    void FairyPet::getSevenSoulFromAnother(FairyPet* pet)
    {
        if(!pet || !_owner)
            return;

        for(UInt8 soulIndex = 1; soulIndex <= 7; soulIndex++)
        {
            if(_soulLevel[soulIndex - 1] < pet->_soulLevel[soulIndex - 1])
            {
                UInt16 skillId;
                if(_soulLevel[soulIndex - 1] > 0)
                {
                    skillId = GData::sevenSoul.getSkillId(_petType, soulIndex, _skillIndex[soulIndex - 1]);
                    if(skillId)
                    {
                        std::string skills = Itoa(SKILLANDLEVEL(skillId, 1));
                        delSkills(skills);
                        petSSErase(skillId);
                    }
                }

                _soulLevel[soulIndex - 1] = pet->_soulLevel[soulIndex - 1];
                _skillIndex[soulIndex - 1] = pet->_skillIndex[soulIndex - 1];
                DB4().PushUpdateData("REPLACE INTO `pet_sevensoul` VALUES(%" I64_FMT "u, %u, %u, %u, %u)", _owner->getId(), getId(), soulIndex, _soulLevel[soulIndex - 1], _skillIndex[soulIndex -1]);
                if(soulIndex > INIT_SKILL_UPMAX)
                {
                    skillId = GData::sevenSoul.getSkillId(_petType, soulIndex, _skillIndex[soulIndex - 1]);
                    std::string newSkill = Itoa(SKILLANDLEVEL(skillId, 1));
                    setSkills(newSkill, false);
                }

                updateSevenSoul(soulIndex - 1);
            }
        }
        setDirty();
        updateToDBPetSkill();
    }

    void FairyPet::sendSevenSoulInfo(Stream & st)
    {
        const UInt8 count = 7;
        st << count;
        for(UInt8 i = 0; i < count; i++)
        {
            st << _soulLevel[i];
            st << _skillIndex[i];
        }
    }

}

