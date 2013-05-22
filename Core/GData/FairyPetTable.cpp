#include "Common/StringTokenizer.h"
#include "FairyPetTable.h"


namespace GData
{

Pet pet;

void Pet::setLevTable(DBPinJie& dbpj)
{
    PinjieData pd;
    StringTokenizer tk(dbpj.bless, "|");
    UInt8 cnt = tk.count();
    for(UInt8 idx = 0; idx < cnt; ++idx)
    {
        stBless stb;
        StringTokenizer ntk(tk[idx].c_str(), ",");
        stb.times = atoi(ntk[0].c_str());
        stb.prob = atoi(ntk[1].c_str());
        pd.bless.push_back(stb);
    }
    StringTokenizer tk1(dbpj.skillLev, ",");
    cnt = tk1.count();
    for(UInt8 idx = 0; idx < cnt && idx < 4; ++idx)
    {
        pd.skillLev[idx] = atoi(tk1[idx].c_str());
    }
    pd.lev = dbpj.id;
    pd.name = dbpj.name;
    pd.consume = dbpj.consume;
    pd.prob = dbpj.prob;

    _levData.insert(std::make_pair(pd.lev, pd));
}

void Pet::setBoneTable(Pet::GenguData& gd)
{
    _boneData.insert(std::make_pair(gd.id, gd));
}

void Pet::setLingyaTable(Pet::LingyaData& ly)
{
    _lingyaData.insert(std::make_pair(ly.petId, ly));
}

void Pet::setEqExpTable(EquipExpData& eqd)
{
    _equipExpData.insert(std::make_pair(eqd.level, eqd));
}

void Pet::setEqAttreffect(EquipAttreffect& eqattr)
{
    _equipAttreffect.insert(std::make_pair(eqattr.level, eqattr));
}

Pet::PinjieData * Pet::getLevTable(UInt16 id)
{
    std::map<UInt16, PinjieData>::iterator iter = _levData.find(id);
    if(iter != _levData.end())
        return &(iter->second);
    return NULL;
}

Pet::GenguData * Pet::getBoneTable(UInt16 id)
{
    std::map<UInt16, GenguData>::iterator iter = _boneData.find(id);
    if(iter != _boneData.end())
        return &(iter->second);
    return NULL;
}

Pet::LingyaData * Pet::getLingyaTable(UInt32 id)
{
    std::map<UInt32, LingyaData>::iterator iter = _lingyaData.find(id);
    if(iter != _lingyaData.end())
        return &(iter->second);
    return NULL;
}

UInt16 Pet::getPinjieBless(UInt16 id, UInt16 v)
{
    PinjieData * pjd = getLevTable(id);
    if(!pjd)
        return 0;
    UInt8 size = pjd->bless.size();
    if(!size)
        return 0;
    for(int i = size - 1; i >= 0; -- i)
    {
        if(pjd->bless[i].times <= v)
            return pjd->bless[i].prob;
    }
    return 0;
}

float Pet::getPetPotential(UInt16 id)
{
    GenguData * ggd = getBoneTable(id);
    return ggd ? ggd->growRate : 1.0f;
}

UInt32 Pet::getEquipExpData(UInt8 lvl, int quality)
{
    if(quality < 0 || quality > 4)
        return 0;
    std::map<UInt8, EquipExpData>::iterator iter = _equipExpData.find(lvl);
    if(iter == _equipExpData.end())
        return 0;
    return iter->second.levExp[quality];
}

UInt8 Pet::getEquipSkillLev(UInt8 lvl)
{
    std::map<UInt8, EquipExpData>::iterator iter = _equipExpData.find(lvl);
    if(iter == _equipExpData.end())
        return 0;
    return iter->second.skillLev;
}

UInt8 Pet::getEquipMaxLev()
{
    return _equipExpData.size() > PETEQUIP_MAXLEV ? _equipExpData.size() : PETEQUIP_MAXLEV;
}

float Pet::getEquipAttreffect(UInt8 lvl, ItemClass subClass)
{
    int idx = subClass - Item_PetEquip;
    if(idx < 0 || idx >= PETEQUIP_ATTR)
        return 0;
    std::map<UInt8, EquipAttreffect>::iterator iter = _equipAttreffect.find(lvl);
    if(iter == _equipAttreffect.end())
        return 0;
    return iter->second.effect[idx];
}

}
