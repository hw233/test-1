#include "Config.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "CoupleUpgrade.h"

namespace GData
{

CoupleUpgrade cu;

void CoupleUpgrade::loadUpgradeTable(DBCoupleInfo& dbci)
{
    if(dbci.level <= 0)
        return;
    CoupleUpgradeData cud;
    cud.levelUpExp = dbci.levelExp;
    cud.hp = dbci.hp;
    cud.attak = dbci.attak;
    cud.magic_attak = dbci.magic_attak;
    cud.df_critical = dbci.df_critical;
    cud.action = dbci.action;
    
    _coupleUpgData.insert(std::make_pair(dbci.level,cud));          

}

CoupleUpgradeData* CoupleUpgrade::getUpgradeData(UInt8 level)
{
    std::map<UInt8,CoupleUpgradeData>::iterator it = _coupleUpgData.find(level);
    if(it != _coupleUpgData.end())
        return &(it->second);
    return NULL; 
}















}
