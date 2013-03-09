#include "FairyPetTable.h"


namespace GData
{

Pet pet;

void Pet::setLevTable(Pet::PinjieData& pd)
{
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

}
