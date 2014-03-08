#include "ClanBuilding.h"

#include "GData/ClanBuildingTable.h"
#include "Log/Log.h"
#include "Server/WorldServer.h"
#include "Clan.h"
#include "Player.h"
#include "MsgID.h"
#include "Var.h"
#include "Server/SysMsg.h"

namespace GObject
{
    static const char* BuildingsTableName[6] =
    {
        "unknownLeve",
        "phyAtkLevel",
        "magAtkLevel",
        "actionLevel",
        "hpLevel",
        "oracleLevel"
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
            process(player ,  vals);
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
                            struct powerHold
                            {
                                UInt8 leftId ;
                                UInt32 clanId ;
                                UInt8 num ;
                                powerHold(UInt8 leftId_ , UInt32 clanId_,UInt8 num_):leftId(leftId_),clanId(clanId_),num(num_){}
                            };
                            powerHold ph(leftId,_clan->getId(),num);
                            GameMsgHdr hdr(0x394, player->getThreadId(), player, sizeof(powerHold));
                            GLOBAL().PushMsg(hdr, &ph);

                        }
                        {
                            GameMsgHdr hdr(0x395, player->getThreadId(), player, 0);
                            GLOBAL().PushMsg(hdr, NULL);
                        }

                    }
                    break;
                case 0x03:
                    {
                        if(_clan->getStatueLevel() < 10 )
                        {
                            player->sendMsgCode(2,4024);
                            return ;
                        }
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
                        std::string playerName ;;
                        brd >> leaderName ; 
                        brd >> playerName ; 
                        GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(leaderName)];
                        GObject::Player * member = GObject::globalNamedPlayers[player->fixName(playerName)];
                        if( pl == NULL || member == NULL)
                            return ;
                        LeaveTeam(pl,member,player,1);
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
                    {
                        UInt8 leftId = 0; 
                        brd >> leftId ;
                        AttackLeftAddr(leftId,player); 
                    }
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
                        if(pos1 != 0 && pos2 != 0)
                            playerId = 0;
                        if(pos1 !=0 && pos2 == 0)
                        {
                           UInt32 val = player->GetVar(VAR_LEFTADDR_POWER) ;
                           if(player->getLeftAddrEnter())
                           {
                               break ;
                           }
                           if(val > 0 )
                           {
                               player->SetVar(VAR_LEFTADDR_POWER ,val -1);
                               player->setLeftAddrEnter(true);
                           }
                        }
                        struct TeamChange
                        {
                            UInt8 leftId ; 
                            UInt32 clanId ;
                            UInt64 playerId;
                            UInt8 pos1;
                            UInt8 pos2;
                            TeamChange(UInt8 leftId_ ,UInt32 clanId_,UInt64 playerId_ ,UInt8 pos1_ ,UInt8 pos2_):leftId(leftId_),clanId(clanId_),playerId(playerId_),pos1(pos1_),pos2(pos2_){}
                        };
                        TeamChange tc(leftId,_clan->getId(),playerId , pos1 ,pos2);
                        GameMsgHdr hdr(0x393, player->getThreadId(), player, sizeof(TeamChange));
                        GLOBAL().PushMsg(hdr, &tc);
                        {
                            GameMsgHdr hdr(0x395, player->getThreadId(), player, 0);
                            GLOBAL().PushMsg(hdr, NULL);
                        }
                    }
                    break;
               case 0x0A:
                    {
                        UInt32 battleId = 0;
                        brd >> battleId;
                        GameMsgHdr hdr(0x396, player->getThreadId(), player, sizeof(UInt32));
                        GLOBAL().PushMsg(hdr, &battleId);
                    }
                    break;
                case 0x0B:
                    LineUp(player);
                    break;
            }
            if(type < 9 && type!=7)
                sendAttackTeamInfo(player); 
        }
        return;
    }

    // 帮派建筑相关操作
    void ClanBuildingOwner::process(Player *player, std::vector<UInt8> vals)
    {
        UInt8 op = vals[0];
        switch (op)
        {
            case opGetInfo:
                sendClanBuildingInfo(player);
                break;
            case opUpgrade:
                {
                    if(_clan->getClanRank(player) < 3 )
                    {
                        player->sendMsgCode(2, 4027);
                        return ;
                    }
                    if (vals.size() < 2)
                        return;
                    UInt8 buildingType = vals[1];
                    upgradeBuilding(player, buildingType);
                    sendClanBuildingInfo(player);
                }
                break;
            //case opUnknown:
            default:
                break;
        }
    }

    void ClanBuildingOwner::upgradeBuilding(Player *player, UInt8 type)
    {
        UInt16 maxLevel = getAddVal(ClanBuilding::eClanBuildingOracle);
        if (getLevel(type) >= maxLevel && type != 5)
        {
            player->sendMsgCode(2, 4025); // :
            return;
        }
        UInt32 uErr = tryLevelUp(type);
        if (uErr == 0)
        {
            player->sendMsgCode(0, 4028); // :
            //player->sendMsgCode(0, 1100);// 
            UpdateEnergy();
            _clan->notifyUpdateStatueAttr();
        }
        else if ( uErr == 2)
            player->sendMsgCode(2, 4026);// 
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
           return  _buildings[type].getAddVal();
        return 0;
    }
    void ClanBuildingOwner::AddBattlesInfo(struct ClanBuildBattleInfo cbbi)
    {
        if(battles_deque.size() == 20 )
            battles_deque.pop_front();
        battles_deque.push_back(cbbi);
        std::vector<Player *> vec;
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leftId == cbbi.leftId )
            {
                vec = it->second ;
                leftAttackTeams.erase(it);
                break;
            }
        }
        std::map<Player *, UInt8> warSort;
        for(UInt8 i = 0 ; i < vec.size() ; ++i)
        {
            if(vec[i] == NULL)
                continue ;
            vec[i]->setLeftAddrEnter(false);
            UInt32 val = vec[i]->GetVar(VAR_LEFTADDR_POWER);
            if(val < 3)
                return ;
            vec[i]->SetVar(VAR_LEFTADDR_POWER , val - 3);
            sendAttackTeamInfo(vec[i]);
            {
                if(cbbi.res == 0)
                {
                    SYSMSG_SENDV(4303, vec[i], cbbi.leftId ); 
                }
                else
                {
                    SYSMSG_SENDV(4302, vec[i], cbbi.leftId ); 
                }
            }
        }
    } 
    void ClanBuildingOwner::SendBattlesInfo( Stream & st)
    {
        UInt32 size  = battles_deque.size(); 
        st << static_cast<UInt8>(size); 
        for(UInt32 i = 0 ; i < size ; ++i )
        {
            ClanBuildBattleInfo cbbi = battles_deque[i]; 
            st <<static_cast<UInt32>(cbbi.battleTime);
            st << static_cast<UInt8>(cbbi.type);
            st << static_cast<UInt8>(cbbi.leftId);
            st << cbbi.name ;
            st <<static_cast<UInt8>(cbbi.res);
            st << static_cast<UInt32>(cbbi.battleId);
            st << static_cast<UInt32>(cbbi.energy);
        }
    }
    void ClanBuildingOwner::CreateTeam(Player * leader ,UInt8 leftId)
    {
        if(_clan->getClanRank(leader) < 2 )
            return ;
        if(leader->GetVar(VAR_LEFTADDR_POWER) < 3 )
            return ;
        if(leader->getLeftAddrEnter())
            return ;
        UInt32 now  =   TimeUtil::Now();
        if( ( now  - leader->GetVar(VAR_LEFTADDR_CREATE) ) < 300)
        {
            leader->sendMsgCode(2,4022);
            return ;
        }
        LeftAttackLeader leftAttLeader(leader , leftId);
        if(leftAttackTeams.size() > 3 )
        {
            leader->sendMsgCode(2,4023);
            return ;
        }
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first == leftAttLeader )
                return ;
        }
        std::vector<Player *> vec ;
        vec.push_back(leader);
        leftAttackTeams.insert(make_pair(leftAttLeader , vec));
        Stream st ;
        SYSMSGVP(st, 4300, leader->getName().c_str(),leftId);
        _clan->broadcast(st);
        leader->setLeftAddrEnter(true);
        leader->SetVar(VAR_LEFTADDR_CREATE ,now);
    }
    void ClanBuildingOwner::ChangeTeamMember(Player * leader , UInt8 first , UInt8 second)
    {
        if(first == 0 || first > 5)
            return ;
        if(second == 0 || second > 5)
            return ;
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leader == leader )
            {
                if( ( first > it->second.size()) ||( second > it->second.size() ) )
                    return ;
                Player * pl = it->second.at(first-1);
                it->second.at(first-1) = it->second.at(second-1);
                it->second.at(second-1) = pl;
                sendAttackTeamInfo(it->second.at(first-1));
                sendAttackTeamInfo(it->second.at(second-1));
                break;
            }
        }
    }
    void ClanBuildingOwner::LeaveTeam(Player * leader, Player * player , Player * opter , UInt8 broad)
    {
        if( opter != player)
        {
            if(leader != opter && leader != player && _clan->getClanRank(opter) < 3 )  //强踢成员
            {
                opter->sendMsgCode(2,4020);
                return ;
            }
            if(leader == player &&  _clan->getClanRank(opter) <= _clan->getClanRank(leader) )  //强解队伍
            {
                opter->sendMsgCode(2,4021);
                return ;
            }
        }
        if(player == NULL )
            return ;
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            bool flag = false ;
            if(it->first.leader == leader || leader == NULL )
            {
                if(it->first.leader == player)
                {
                    flag = true;
                }
                std::vector<Player *> vec = it->second;
                for(std::vector<Player *>::iterator it_vec = it->second.begin(); it_vec != it->second.end(); )
                {
                    if(*it_vec == player || flag )
                    {
                        (*it_vec)->setLeftAddrEnter(false);
                        it->second.erase(it_vec);
                        it_vec = it->second.begin();
                    }
                    else
                        ++it_vec;
                }
                if(flag)
                {
                    if(broad)
                    {
                        Stream st ;
                        SYSMSGVP(st, 4301, opter->getName().c_str(),it->first.leftId);
                        _clan->broadcast(st);
                    }
                    leftAttackTeams.erase(it);
                }
                for(UInt8 i = 0 ; i < vec.size() ; ++i)
                {
                    if(vec[i]!=NULL)
                        sendAttackTeamInfo(vec[i]);
                }
                if(leader!=NULL)
                    break;
            }
        }
        
    }
    void ClanBuildingOwner::EnterTeam(Player * leader,  Player * player)
    {
        if(player->GetVar(VAR_LEFTADDR_POWER) < 3 )
            return ;
        if(player->getLeftAddrEnter())
            return ;
        std::vector<Player *> vec;
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leader == leader )
            {
                if(it->second.size() > 4)
                    return ;
                vec = it->second ;
                for(std::vector<Player *>::iterator it_vec = vec.begin(); it_vec != vec.end(); ++ it_vec )
                {
                    if(*it_vec == player )
                    {
                        return ;
                    }
                }
                it->second.push_back(player);
                for(std::vector<Player *>::iterator it_vec = vec.begin(); it_vec != vec.end(); ++it_vec)
                {
                    sendAttackTeamInfo(*it_vec);
                }
                player->setLeftAddrEnter(true);
                break;
            }
        }

    }
    void ClanBuildingOwner::AttackLeftAddr(UInt8 leftId ,Player * player)
    {
        for(std::map< LeftAttackLeader , std::vector<Player *> >::iterator it = leftAttackTeams.begin() ; it != leftAttackTeams.end() ; ++it)
        {
            if(it->first.leftId == leftId )
            {
                if( ( it->first.leader != player || !it->first.leader->getLeftAddrEnter() ) && _clan->getClanRank(player) < 3)
                    return ;
                std::string leaderName = it->first.leader->getName();
                UInt8 leftId = it->first.leftId;
                if(it->second.size() != 5)
                    return ;
                std::vector<Player *> vec = it->second ;

                it->first.leader->setLeftAddrEnter(false);
                //LeaveTeam(player , player ,player);

                std::vector<Player *> warSort = vec;
                struct SWarEnterData {
                    Stream st;
                    std::vector<Player *> warSort;
                    UInt8 pos ;
                    SWarEnterData(Stream& st2, std::vector<Player *>& warSort2) : st(st2), warSort(warSort2) ,pos(0) {}
                };
                for(UInt8 i = 0 ; i < vec.size() ; ++i )
                {
                    if(!vec[i])
                        return ;
                }
                Stream st(SERVERLEFTREQ::ENTER, 0xEE);
                st<<static_cast<UInt64>(player->getId()) << _clan->getId()<<_clan->getName() << leaderName/*领队*/  << leftId << static_cast<UInt8>(0) << static_cast<UInt8>(warSort.size()); 
                SWarEnterData * swed = new SWarEnterData(st, vec);

                std::vector<Player *>::iterator itw = warSort.begin();
                GameMsgHdr hdr(0x391, (*itw)->getThreadId(), *itw, sizeof(SWarEnterData*));
                GLOBAL().PushMsg(hdr, &swed);
                return;
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
        if(player == NULL)
            return ;
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
                if(vec[i]==NULL)
                    continue;
                st <<static_cast<UInt8>(vec[i]->GetClassAndSex()) <<vec[i]->getName() << vec[i]->GetLev() << vec[i]->getBattlePoint();
            }
        }
        st << Stream::eos;
        player->send(st);
        
    }
    void ClanBuildingOwner::UpdateEnergy()
    {
        DB5().PushUpdateData("UPDATE `clan_buildings` set `fairylandEnergy` = %u, `updateTime` = %u where `clanId` = %" I64_FMT "u", _energy , TimeUtil::Now() , _clan->getId());
    }
}

