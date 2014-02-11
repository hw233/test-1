#include "ClanBuilding.h"

#include "GData/ClanBuildingTable.h"
#include "Log/Log.h"
#include "Server/WorldServer.h"
#include "Clan.h"
#include "Player.h"

namespace GObject
{
    static const char* BuildingsTableName[5] =
    {
        "unknownLeve",
        "phyAtkLevel",
        "magAtkLevel",
        "actionLevel",
        "hpLevel"
    };
    
    static const UInt8 BUILDINGS_COUNT = 4;

    ClanBuilding::ClanBuilding()
        : _type(0), _owner(NULL)
    {
        _levelType = eLevelUpByLevel;
    }

    ClanBuilding::~ClanBuilding()
    {
    }

    bool ClanBuilding::init(UInt8 type, ClanBuildingOwner* owner)
    {
        _type = type;
        _owner = owner;
        setExp(0, false);
        return true;
    }

    bool ClanBuilding::loadFromDB(UInt16 level, UInt32 updateTime)
    {
        UInt32 exp = getTableLevelExp(level);
        BuildingTmpl::loadFromDB(exp, level, updateTime);
        return true;
    }

    UInt32 ClanBuilding::getTableLevelExp(UInt16 level) const
    {
        if (GData::clanBuildingList.size() <= _type)
        {
            WARN_LOG("ClanBuilding: GData::clanBuildingList.size() = %d.", (UInt32) GData::clanBuildingList.size());
            return 0;
        }
        GData::ClanBuildingTable& clanBuildingTable = GData::clanBuildingList[_type];
        if (clanBuildingTable.size() <= level)
        {
            WARN_LOG("ClanBuilding: GData::clanBuildingList.size() = %d.", (UInt32) GData::clanBuildingList.size());
            return 0;
        }
        return clanBuildingTable[level].needExp;
    }

    UInt16 ClanBuilding::getTableLevel(UInt32 exp) const
    {
        if (GData::clanBuildingList.size() <= _type)
        {
            WARN_LOG("ClanBuilding: GData::clanBuildingList.size() = %d.", (UInt32) GData::clanBuildingList.size());
            return 0;
        }
        GData::ClanBuildingTable& clanBuildingTable = GData::clanBuildingList[_type];
        for (GData::ClanBuildingTable::iterator it = clanBuildingTable.begin(); it != clanBuildingTable.end(); ++it)
        {
            if ((*it).needExp > exp)
                return (*it).level - 1;
        }
        return clanBuildingTable.size() - 1;
    }

    UInt16 ClanBuilding::getMaxLevel() const
    {
        if (GData::clanBuildingList.size() <= _type)
        {
            WARN_LOG("ClanBuilding: GData::clanBuildingList.size() = %d.", (UInt32) GData::clanBuildingList.size());
            return 0;
        }
        GData::ClanBuildingTable& clanBuildingTable = GData::clanBuildingList[_type];
        if (GData::clanBuildingList.empty())
        {
            WARN_LOG("clanBuildingList.size() == 0.");
            return 0;
        }
        return clanBuildingTable.size() - 1;
    }

    UInt32 ClanBuilding::getAddVal() const
    {
        if (GData::clanBuildingList.size() <= _type)
        {
            WARN_LOG("ClanBuilding: GData::clanBuildingList.size() = %d.", (UInt32) GData::clanBuildingList.size());
            return 0;
        }
        GData::ClanBuildingTable& clanBuildingTable = GData::clanBuildingList[_type];
        if (clanBuildingTable.size() <= getLevel())
        {
            WARN_LOG("ClanBuilding: GData::clanBuildingList.size() = %d.", (UInt32) GData::clanBuildingList.size());
            return 0;
        }
        return clanBuildingTable[getLevel()].value;
    }

    bool ClanBuilding::saveToDB() const
    {
        // 保存到数据库
        DB5().PushUpdateData("UPDATE `clan_buildings` set `%s` = %u, `updateTime` = %u where `clanId` = %" I64_FMT "u", 
                BuildingsTableName[_type], getLevel(), getUpdateTime(), _owner->getClanId());
        return true;
    }

    ClanBuildingOwner::ClanBuildingOwner(Clan* clan)
        : _clan(clan), _energy(0)
    {
        if (_clan)
            DB2().PushUpdateData("INSERT IGNORE INTO `clan_buildings` (`clanId`) VALUES (%" I64_FMT "u)", _clan->getId()); 
    {
        // 从数据库加载
        if (!_buildings.empty())
        {
            WARN_LOG("init ClanBuildingOwner when _buildings not empty.");
            return ;
        }
        _buildings.resize(GData::clanBuildingList.size());
        UInt8 index = 0;
        for (GData::ClanBuildingList::iterator it = GData::clanBuildingList.begin(); it != GData::clanBuildingList.end(); ++it)
        {
            _buildings[index].init(index, this);
            ++index;
        }
        if (_buildings.size() <= BUILDINGS_COUNT)
        {
            WARN_LOG("_buildings.size = %u.", static_cast<UInt32>(_buildings.size()));
            return ;
        }
        return ;
    }
    }

    ClanBuildingOwner::~ClanBuildingOwner()
    {
    }

    bool ClanBuildingOwner::loadFromDB( UInt32 fairylandEnergy, 
            UInt16 phyAtkLevel, UInt16 magAtkLevel, UInt16 actionLevel, UInt16 hpLevel, 
            UInt16 updateTime)
    {
        // 从数据库加载
        _buildings.clear();
        _buildings.resize(GData::clanBuildingList.size());
        UInt8 index = 0;
        for (GData::ClanBuildingList::iterator it = GData::clanBuildingList.begin(); it != GData::clanBuildingList.end(); ++it)
        {
            _buildings[index].init(index, this);
            ++index;
        }
        if (_buildings.size() <= BUILDINGS_COUNT)
        {
            WARN_LOG("_buildings.size = %u.", static_cast<UInt32>(_buildings.size()));
            return false;
        }
        _energy = fairylandEnergy;
        _buildings[1].loadFromDB(phyAtkLevel, updateTime);
        _buildings[2].loadFromDB(magAtkLevel, updateTime);
        _buildings[3].loadFromDB(actionLevel, updateTime);
        _buildings[4].loadFromDB(hpLevel, updateTime);
        return false;
    }

    UInt64 ClanBuildingOwner::getClanId() const
    {
        if (!_clan)
            return 0;
        return _clan->getId();
    }

    UInt16 ClanBuildingOwner::getBuildingLevel(UInt8 type) const
    {
        if (type >= ClanBuilding::eClanBuildingMax)
            return 0;
        return _buildings[type].getLevel();
    }

    UInt32 ClanBuildingOwner::getEnergy() const
    {
        return _energy;
    }

    // 客户端协议返回调用帮派建筑相关操作
    void ClanBuildingOwner::processFromBrd(Player *player, BinaryReader& brd)
    {
        UInt8 type = 0;
        if (brd.empty())
            return;
        brd >> type;
        std::vector<UInt8> vals;
        while(!brd.empty())
        {
            UInt8 val;
            brd >> val;
            vals.push_back(val);
        }
        process(player, type, vals);
        return;
    }

    // 帮派建筑相关操作
    void ClanBuildingOwner::process(Player *player, UInt8 type, std::vector<UInt8> vals)
    {
        switch (type)
        {
            case opGetInfo:
                sendClanBuildingInfo(player);
                break;
            case opUpgrade:
                {
                    if (vals.size() < 1)
                        return;
                    UInt8 buildingType = vals[0];
                    upgradeBuilding(player, buildingType);
                }
                break;
            case opUnknown:
            default:
                break;
        }
    }

    void ClanBuildingOwner::upgradeBuilding(Player *player, UInt8 type)
    {
        UInt32 uErr = tryLevelUp(type);
        if (uErr == 0)
            player->sendMsgCode(0, 1100);// TODO
        else
            player->sendMsgCode(0, 1101, uErr);// TODO

    }

    // 帮派建筑升级
    UInt32 ClanBuildingOwner::tryLevelUp(UInt8 type)
    {
        if (type >= ClanBuilding::eClanBuildingMax)
            return eErrTypeWrong;
        return _buildings[type].tryLevelUp(_energy);
    }

    // TODO:: GM操作改变建筑等级
    void ClanBuildingOwner::levelChange(Int32 iVal)
    {
        for (UInt8 type = ClanBuilding::eClanBuildingUnknown + 1; type < ClanBuilding::eClanBuildingMax; ++type)
        {
            UInt32 level = _buildings[type].getLevel();
            if (iVal < 0)
            {
                if ((Int32)level <= -iVal)
                    level = 0;
                else
                    level -= (-iVal);
            }
            else
                level += iVal;
            _buildings[type].setLevel(level);
        }
    }

    // TODO: 发送帮派建筑信息 
    void ClanBuildingOwner::sendClanBuildingInfo(Player *player)
    {
        if (!player)
            return;
        /*
        Stream st(REP::CLAN_FAIRYLAND);
        st << static_cast<UInt8>(0);
        player->send(st);
        */
        return;
    }

    UInt16 ClanBuildingOwner::getLevel(UInt8 type) const
    {
        if (type >= ClanBuilding::eClanBuildingMax)
            return 0;
        return _buildings[type].getLevel();
    }

    UInt32 ClanBuildingOwner::getAddVal(UInt8 type) const
    {
        if (type > ClanBuilding::eClanBuildingUnknown && type < ClanBuilding::eClanBuildingMax)
            _buildings[type].getAddVal();
        return 0;
    }
        
}

