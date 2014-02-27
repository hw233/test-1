#include "ClanBuilding.h"

#include "GData/ClanBuildingTable.h"
#include "Log/Log.h"
#include "Server/WorldServer.h"
#include "Clan.h"
#include "Player.h"
#include "MsgID.h"
#include "Var.h"

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
            UInt16 phyAtkLevel, UInt16 magAtkLevel, UInt16 actionLevel, UInt16 hpLevel, UInt16 oracleLevel,
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
        _buildings[5].loadFromDB(oracleLevel, updateTime);
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
        if(type < 2)
        {
            std::vector<UInt8> vals;
            while(brd.left())
            {
                UInt8 val;
                brd >> val;
                vals.push_back(val);
            }
            process(player, type, vals);
        }
        else
        {
            switch(type)
            {
                case 0x02:
                    {
                        UInt8 op = 0 ;
                        brd >>op ;
                        if(op ==1 )
                        {
                            UInt8 leftId = 0 ;
                            UInt8 num = 0;
                            brd >>leftId ;
                            brd >> num ; 
                            if(!player->giveLeftPowerHold(num))
                                break;
                            Stream st(SERVERLEFTREQ::LEFTADDR_POWERHOLD, 0xEE);
                            st << leftId ;
                            st << static_cast<UInt32>(_clan->getId());
                            st << num;
                            st << Stream::eos;
                            NETWORK()->SendToServerLeft(st);
                        }
                        {
                            Stream st(SERVERLEFTREQ::LEFTADDR_INFO, 0xEE);
                            st << player->getId() ; 
                            st << Stream::eos;
                            NETWORK()->SendToServerLeft(st);
                        }

                    }
                    break;
                case 0x03:
                    {
                        UInt8 leftId = 0;
                        brd >>leftId ;
                        CreateTeam(player , leftId);
                    }
                    break ; 
                case 0x04:
                    {
                        std::string leaderName ;
                        brd >> leaderName ; 
                        GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(leaderName)];
                        EnterTeam(pl,player);
                    }
                    break;
                case 0x05:
                    {
                        std::string leaderName ;
                        std::string playerName;
                        brd >> leaderName ; 
                        GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(leaderName)];
                        GObject::Player * member = GObject::globalNamedPlayers[player->fixName(playerName)];
                        LeaveTeam(pl,member);
                    }
                    break;
                case 0x06:
                    {
                        std::string leaderName ;
                        UInt8 first = 0;
                        UInt8 second = 0;
                        brd >> leaderName ; 
                        GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(leaderName)];
                        if( player != pl)
                            return ;
                        brd >> first ;
                        brd >> second ;
                        ChangeTeamMember(player ,first ,second );
                    }
                    break;
                case 0x07:
                    AttackLeftAddr(player); 
                    break;
                case 0x08:
                    {
                        UInt8 leftId = 0;
                        UInt8 pos1 = 0;
                        UInt8 pos2 = 0;
                        UInt64 playerId = player->getId();
                        brd >> leftId ;
                        brd >> pos1 ;
                        brd >> pos2 ;
                        if(pos1 == 0 && pos2 == 0)
                            playerId = 0;
                        Stream st(SERVERLEFTREQ::LEFTADDR_SWITCHPLAYER, 0xEE);
                        st << leftId ; 
                        st << _clan->getId() ;
                        st << playerId ; 
                        st << pos1;
                        st << pos2;
                        st << Stream::eos;
                        NETWORK()->SendToServerLeft(st);
                    }
                    break;
               case 0x0A:
                    {
                        UInt32 battleId = 0;
                        brd >> battleId;
                        UInt64 playerId1 = player->getId();
                        Stream st(SERVERLEFTREQ::BATTLE_REPORT, 0xEE);
                        st << playerId1 << battleId ; 
                        st << Stream::eos;
                        NETWORK()->SendToServerLeft(st);
                    }
                    break;
            }
            if(type < 9)
                sendAttackTeamInfo(player); 
        }
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
        UInt16 maxLevel = getAddVal(ClanBuilding::eClanBuildingOracle);
        if (getLevel(type) >= maxLevel)
        {
            player->sendMsgCode(0, 1102); // TODO:
            return;
        }
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
        Stream st(REP::CLAN_FAIRYLAND);
        st << static_cast<UInt8>(1);
        st << static_cast<UInt8>(0);
        st << static_cast<UInt32>(_energy);
        st << getLevel(1);
        st << getLevel(2);
        st << getLevel(3);
        st << getLevel(4);
        st << getLevel(5);
        st << static_cast<UInt8>(WORLD().getLeftAddrConnection());
        SendBattlesInfo(st); 
        st << Stream::eos;
        player->send(st);
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
    void ClanBuildingOwner::AddBattlesInfo(struct ClanBuildBattleInfo cbbi)
    {
        if(battles_deque.size() == 20 )
            battles_deque.pop_front();
        battles_deque.push_back(cbbi);
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leftId == cbbi.leftId )
            {
                std::vector<Player *> vec = it->second ;
                std::map<Player *, UInt8> warSort;
                for(UInt8 i = 0 ; i < 5 ; ++i)
                {
                    UInt32 value = vec[i]->GetVar(VAR_LEFTADDR_POWER);
                    if(value < 3)
                        value = 0 ;
                    else 
                        value = value - 3 ;
                    vec[i]->SetVar(VAR_LEFTADDR_POWER ,value);
                }
                leftAttackTeams.erase(it);
                break;
            }
        }
    } 
    void ClanBuildingOwner::SendBattlesInfo( Stream & st)
    {
        UInt32 size  = battles_deque.size(); 
        st << static_cast<UInt32>(size); 
        for(UInt32 i = 0 ; i < size ; ++i )
        {
            ClanBuildBattleInfo cbbi = battles_deque[i]; 
            st <<static_cast<UInt32>(cbbi.battleTime);
            st << static_cast<UInt8>(cbbi.type);
            st << static_cast<UInt8>(cbbi.leftId);
            st << cbbi.name ;
            st <<static_cast<UInt8>(cbbi.res);
            st << static_cast<UInt32>(cbbi.battleId);
        }
    }
    void ClanBuildingOwner::CreateTeam(Player * leader ,UInt8 leftId)
    {
        if(leader->GetVar(VAR_LEFTADDR_POWER) < 3 )
            return ;
        if(leader->getLeftAddrEnter() || leader->GetVar(VAR_LEFTADDR_ENTER))
            return ;
        LeftAttackLeader leftAttLeader(leader , leftId);
        if(leftAttackTeams.size() > 3 )
            return ;
        std::vector<Player *> vec ;
        vec.push_back(leader);
        leftAttackTeams.insert(make_pair(leftAttLeader , vec));
        leader->setLeftAddrEnter(true);
    }
    void ClanBuildingOwner::ChangeTeamMember(Player * leader , UInt8 first , UInt8 second)
    {
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leader == leader )
            {
                std::vector<Player *> vec = it->second;
                if(vec.size() < second)
                    return ;
                Player * pl = vec[first];
                vec[first] = vec[second];
                vec[second] = pl;
                break;
            }
        }
    }
    void ClanBuildingOwner::LeaveTeam(Player * leader, Player * player)
    {
        if(player == NULL )
            return ;
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leader == leader )
            {
                if(leader == player)
                {
    //                leftAttackTeams.erase(it);
                    leader = NULL;
                }
                std::vector<Player *> vec = it->second;
                for(std::vector<Player *>::iterator it_vec = vec.begin(); it_vec != vec.end(); )
                {
                    if(*it_vec == player || leader == NULL )
                    {
                        (*it_vec)->setLeftAddrEnter(false);
                        vec.erase(it_vec++);
                    }
                    else
                        ++it_vec;
                }
                break;
            }
            if(leader == NULL)
            {
                leftAttackTeams.erase(it);
                return ;
            }
        }
        
    }
    void ClanBuildingOwner::EnterTeam(Player * leader,  Player * player)
    {
        if(player->GetVar(VAR_LEFTADDR_POWER) < 3 )
            return ;
        if(player->getLeftAddrEnter() || player->GetVar(VAR_LEFTADDR_ENTER))
            return ;
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leader == leader )
            {
                if(it->second.size() > 4)
                    return ;
                it->second.push_back(player);
                break;
            }
        }
        player->setLeftAddrEnter(true);
    }
    void ClanBuildingOwner::AttackLeftAddr(Player * player)
    {
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leader == player )
            {
                if(it->second.size() != 5)
                    return ;
                std::vector<Player *> vec = it->second ;
                std::map<Player *, UInt8> warSort;
                for(UInt8 i = 0 ; i < 5 ; ++i)
                {
                    if(vec[i]->GetVar(VAR_LEFTADDR_POWER) < 3 )
                        return ;
                    warSort.insert(std::make_pair(vec[i], i)); 
                }
                struct SWarEnterData {
                    Stream st;
                    std::map<Player *, UInt8> warSort;
                    SWarEnterData(Stream& st2, std::map<Player *, UInt8>& warSort2) : st(st2), warSort(warSort2) {}
                };

                Stream st(SERVERLEFTREQ::ENTER, 0xEE);
                st<<_clan->getId()<<_clan->getName() << player->getName()/*领队*/  << it->first.leftId << static_cast<UInt8>(0) << static_cast<UInt8>(warSort.size()); 
                SWarEnterData * swed = new SWarEnterData(st, warSort);
                std::map<Player *, UInt8>::iterator it = warSort.begin();
                GameMsgHdr hdr(0x391, it->first->getThreadId(), it->first, sizeof(SWarEnterData*));
                GLOBAL().PushMsg(hdr, &swed);

            }
        }
    }
    void ClanBuildingOwner::LineUp(Player * player)
    {
        GameMsgHdr hdr(0x392, player->getThreadId(), player, 0);
        GLOBAL().PushMsg(hdr, NULL);
    }
    void ClanBuildingOwner::sendAttackTeamInfo(Player *player)
    {
        Stream st(REP::CLAN_FAIRYLAND);
        st <<static_cast<UInt8>(9);
        st <<static_cast<UInt8>(player->GetVar(VAR_LEFTADDR_POWER));
        st <<static_cast<UInt8>( leftAttackTeams.size() ); 
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            st << it->first.leader->getName();
            st << static_cast<UInt8>( it->first.leftId );
            std::vector<Player* > vec = it->second;
            st << static_cast<UInt8>( vec.size() );
            for(UInt8 i = 0 ; i < vec.size() ; ++i )
            {
                st <<static_cast<UInt8>(vec[i]->GetClassAndSex()) <<vec[i]->getName() << vec[i]->GetLev() << vec[i]->getBattlePoint();
            }
        }
        st << Stream::eos;
        player->send(st);
        
    }
}

