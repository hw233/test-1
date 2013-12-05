#include "Config.h"
#include "ArenaServerWar.h"
#include "Player.h"
#include "Package.h"
#include "MsgID.h"
#include "GObjectDBExecHelper.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "Server/SysMsg.h"
#include "MapCollection.h"

namespace GObject
{

ServerWarMgr serverWarMgr;
ServerWarBoss serverWarBoss;

static const int aIndex[5] = {0, 8, 12, 14, 15};
#define GET_PROGRESS_NAME(n, pp) char n[1024]; { SysMsgItem * mi = globalSysMsg[782 + pp]; if(mi != NULL) mi->get(n); else n[0] = 0; }

const static UInt8 progress_accept[5][14] = {
  // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12, 13
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},   // 0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // 1
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},   // 2
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // 3
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},   // 4
};

//_round:1=进16强 2=进8强 3=进4强 4=进半决赛 5=进决赛
//_round:1=进8强 2=进4强 3=进半决赛 4=进决赛
const static UInt32 ArenaLastAwards[2][6][4] = {
    {
        {9063, 6000, 0, 0},
        {9062, 7000, 206, SERVERWAR_VALUE_XIUWEI1},
        {9061, 8000, 205, SERVERWAR_VALUE_XIUWEI2},
        {9060, 10000, 204, SERVERWAR_VALUE_XIUWEI3},
        {9059, 12000, 203, SERVERWAR_VALUE_XIUWEI4},
        {9058, 15000, 202, SERVERWAR_VALUE_XIUWEI5}
    },
    {
        {9066, 1000, 0, 0},
        {9066, 1500, 0, 0},
        {9065, 2000, 0, 0},
        {9065, 3000, 0, 0},
        {9064, 4000, 0, 0},
        {9063, 5000, 0, 0}
    }
};

const static UInt32 ArenaLastAwards2[2][6] = {
    { 600, 700, 800, 900, 1000, 1200 },
    { 50, 100, 200, 300, 400, 500 },
};

bool server_enum_send_status(Player * player, void * data )
{
    if(player == NULL)
        return true;

    GObject::serverWarMgr.sendStatus(player);
    return true;
}

bool server_enum_send_active(Player * player, void * data )
{
    if(player == NULL)
        return true;

    GObject::serverWarMgr.sendActive(player);
    return true;
}

bool server_addLonghun(Player * player, void * data)
{
    if(!player || !data)
        return true;
    UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
    if(!GET_BIT_8(value, 1))
        return true;
    UInt32 money = *reinterpret_cast<UInt32 *>(data);

    GameMsgHdr hdr(0x384, player->getThreadId(), player, sizeof(money));
    GLOBAL().PushMsg(hdr, &money);
    return true;
}

static void writeMailLog(Player * player, Mail * mail)
{
    if(!player || !mail)
        return;
    std::string strItems;
    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text,\
        content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(),
        mail->id, ArenaAward, mail->title.c_str(), mail->content.c_str(), strItems.c_str(), mail->recvTime);
}

bool server_sendMail(Player * player, EnumMailStruct * ems)
{
    if(!player || !ems)
        return true;
    UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
    if(!GET_BIT_8(value, 1))
        return true;
    SYSMSGV(g, 710 + ems->group);     // 天/地榜
    if(ems->round == 0)
    {
        GET_PROGRESS_NAME(p, -1); // xx强赛
        if(ems->isWon)
        {
            SYSMSGV(title, 810, g, p);
            SYSMSGV(content, 812, ems->session, cfg.serverNo, g, ems->twon, ems->tloss, p, ems->longhun);
            Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(player, mail);
        }
        else
        {
            SYSMSGV(title, 813, g, p);
            SYSMSGV(content, 815, ems->session, cfg.serverNo, g, ems->twon, ems->tloss, ems->longhun);
            Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(player, mail);
        }
    }
    else
    {
        //emd->round:1=进16强 2=进8强 3=进4强 4=进半决赛 5=进决赛
        //_round:1=进8强 2=进4强 3=进半决赛 4=进决赛
        UInt8 tmp = ems->isWon ? 1 : 0;
        UInt32 data = ArenaLastAwards2[ems->group][ems->round+tmp-1];    //龙魂
        server_addLonghun(player, &data);

        SYSMSGV(title, 818, g);
        SYSMSGV(content, 820, ems->session, cfg.serverNo, data);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
        writeMailLog(player, mail);
    }
    return true;
}

bool server_addBuffData(Player * player, void * data)
{
    if(!player) return true;
    UInt32 value = GVAR.GetVar(GVAR_SERVERWAR_XIUWEI);
    UInt32 time = GVAR.GetOverTime(GVAR_SERVERWAR_XIUWEI);

    UInt8 buffId = 0;
    if(value == SERVERWAR_VALUE_XIUWEI1)
        buffId = SERVERWAR_BUFF_XIUWEI1;
    else if(value == SERVERWAR_VALUE_XIUWEI2)
        buffId = SERVERWAR_BUFF_XIUWEI2;
    else if(value == SERVERWAR_VALUE_XIUWEI3)
        buffId = SERVERWAR_BUFF_XIUWEI3;
    else if(value == SERVERWAR_VALUE_XIUWEI4)
        buffId = SERVERWAR_BUFF_XIUWEI4;
    else if(value == SERVERWAR_VALUE_XIUWEI5)
        buffId = SERVERWAR_BUFF_XIUWEI5;

    if(buffId)
        player->addBuffData(buffId, time-TimeUtil::Now());
    return true;
}

UInt32 calculateExp(Player * player, UInt8 flag)
{
    if(!player) return 0;
    UInt8 plvl = player->GetLev();
    UInt32 exp = (plvl - 10) * ((plvl > 99 ? 99 : plvl) / 10) * 5 + 25;
    if(flag)
        exp *= 60*5;
    else
        exp *= 60*2;
    return exp;
}

void ServerWarMgr::loadFromDB(DBArenaServerWar& dbasw)
{
    Player * player = globalPlayers[dbasw.playerId];
    if (player == NULL) return;
    switch(dbasw.type)
    {
    case 1:
        _signSort.insert(SWPlayerData(player, dbasw.battlePoint));
        break;
    case 2:
        _warSort.insert(std::make_pair(player, static_cast<UInt8>(dbasw.pos)));
        break;
    }
}

void ServerWarMgr::pushBetFromDB(Player * player, DBArenaBet& dbab)
{
    if(!player || dbab.state >= 5 || dbab.group > 2)
        return;
	BetInfo binfo;
	binfo.state = dbab.state;
	binfo.round = dbab.round;
	binfo.group = dbab.group;
	binfo.recieved = dbab.recieved;
	binfo.pos = dbab.pos;
	binfo.type = dbab.tael;
    int i = 0;
    if(dbab.group > 0)
        i = dbab.group - 1;
	_playerBet[player].betList[i][dbab.state].push_back(binfo);
}

void ServerWarMgr::updateSignupToDB(Player * player)
{
    if(!player) return;
    DB1().PushUpdateData("REPLACE INTO `arena_serverWar`(`playerId`, `type`, `pos`, `battlePoint`) VALUES(%" I64_FMT "u, 1, 0, %u)", player->getId(), player->getBattlePoint());
}

void ServerWarMgr::clearWarSort()
{
    _warSort.clear();
    DB1().PushUpdateData("DELETE FROM `arena_serverWar` WHERE `type` = 2");
}

void ServerWarMgr::setWarSort()
{
    clearWarSort();
    UInt8 i = 0;
    for(SWPDSort::iterator it = _signSort.begin(); it != _signSort.end(); ++ it)
    {
        if(i >= SERVERWAR_MAX_MEMBER)
            return;
        if((*it).player && (*it).player->GetLev() >= LIMIT_LEVEL)
        {
            _warSort.insert(std::make_pair((*it).player, i));
            DB1().PushUpdateData("REPLACE INTO `arena_serverWar`(`playerId`, `type`, `pos`) VALUES(%" I64_FMT "u, 2, %u)", (*it).player->getId(), i);
            ++ i;
        }
    }
}

void ServerWarMgr::signup(Player * player)
{
    if (!player || _progress != e_war_sign)
        return;
    UInt8 result = 0;
    if (player->GetLev() < LIMIT_LEVEL)
        result = 1;
    if (!result && findSWPlayerData(player))
        result = 2;
    if(!result)
    {
        insertSWPlayerData(player, false);
        updateSignupToDB(player);
    }

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01);
	st << result << Stream::eos;
	player->send(st);
}

void ServerWarMgr::challenge(Player * atker, std::string& name)
{
    if (!atker || _progress != e_war_challenge || atker->GetLev() < LIMIT_LEVEL)
        return;
    atker->fixName(name);
    Player * defer = globalNamedPlayers[name];
    if (!defer || atker == defer)
        return;
    std::map<Player *, UInt8>::iterator it = _warSort.find(atker);
    if (it != _warSort.end())
        return;
    it = _warSort.find(defer);
    if (it == _warSort.end())
    {
        atker->sendMsgCode(0, 1418);
        sendWarSortInfo(atker);
        return;
    }
    if(atker->inServerWarChallengeCD())
    {
        Stream st(REP::SERVERWAR_ARENA_OP);
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(5);
        st << static_cast<UInt8>(2) << atker->getServerWarChallengeCD() << Stream::eos;
        atker->send(st);
        return;
    }

    GameMsgHdr hdr(0x386, defer->getThreadId(), defer, sizeof(Player *));
    GLOBAL().PushMsg(hdr, &atker);
}

void ServerWarMgr::notifyChallengeResult(Player* atker, Player* defer, bool win)
{
    if(!atker || !defer) return;
    std::map<Player *, UInt8>::iterator it = _warSort.find(defer);
    if (it == _warSort.end())
        return;
    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(5);
	st << static_cast<UInt8>(win ? 0 : 1) << atker->getServerWarChallengeCD() << Stream::eos;
	atker->send(st);

    UInt8 pos = it->second;
    if (win)
    {
        _warSort.erase(it);
        _warSort.insert(std::make_pair(atker, pos));

        DB1().PushUpdateData("REPLACE INTO `arena_serverWar`(`playerId`, `type`, `pos`) VALUES(%" I64_FMT "u, 2, %u)", atker->getId(), pos);
        DB1().PushUpdateData("DELETE FROM `arena_serverWar` WHERE `playerId` = %" I64_FMT "u AND `type` = 2", defer->getId());
        if(defer->isOnline())
        {
            SYSMSG_SENDV(180, defer, atker->getName().c_str());
            SYSMSG_SENDV(1080, defer, atker->getName().c_str());
        }
        Stream st(REP::SERVERWAR_ARENA_OP);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x08);
        st << static_cast<UInt8>(0x02) << pos;
        st << atker->getName() << atker->getCountry();
        st << static_cast<UInt8>(atker->getMainFighter()->getId());
        st << atker->GetLev() << atker->getBattlePoint();
        st << Stream::eos;
        atker->send(st);
        defer->send(st);

        if(cfg.isTestPlatform())
        {
            if(!findSWPlayerData(defer))
            {
                insertSWPlayerData(defer, false);
                updateSignupToDB(defer);
            }
        }
    }
}

void ServerWarMgr::attackPlayer(Player* atker, Player* defer)
{
    if(!atker || !defer) return;
	UInt8 tid = defer->getThreadId();
	if(tid == atker->getThreadId())
	{
		Battle::BattleSimulator bsim(Battle::BS_NEWCOUNTRYBATTLE, atker, defer);
		atker->PutFighters( bsim, 0, true );
		defer->PutFighters( bsim, 1, true );
		bsim.start();
		bool res = bsim.getWinner() == 1;

		Stream st(REP::ATTACK_NPC);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << static_cast<UInt64>(0)  << Stream::eos;
		atker->send(st);

        struct SWResNotify
        {
            Player * peer;
            bool win;
        };
        SWResNotify notify = { defer, res };
        GameMsgHdr hdr2(0x1EA, WORKER_THREAD_WORLD, atker, sizeof(SWResNotify));
        GLOBAL().PushMsg(hdr2, &notify);

		return;
	}

	struct SWBeAttackData
	{
		Player * attacker;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	SWBeAttackData swbad = { atker, atker->getFormation(), static_cast<UInt16>(atker->getMainFighter() ? atker->getMainFighter()->getId() : 0) };
	for(int i = 0; i < 5; ++ i)
		swbad.lineup[i] = atker->getLineup(i);
	GameMsgHdr hdr(0x387, tid, defer, sizeof(SWBeAttackData));
	GLOBAL().PushMsg(hdr, &swbad);
}

void ServerWarMgr::beAttackByPlayer(Player* defer, Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup)
{
    if(!atker || !defer) return;
	Battle::BattleSimulator bsim(Battle::BS_NEWCOUNTRYBATTLE, atker, defer);
	bsim.setFormation( 0, formation );
	bsim.setPortrait( 0, portrait );
	for(int i = 0; i < 5; ++ i)
	{
		if(lineup[i].fighter != NULL)
		{
			Battle::BattleFighter * bf = bsim.newFighter(0, lineup[i].pos, lineup[i].fighter);
			bf->setHP(0);
		}
	}
    atker->PutPets(bsim, 0);
	defer->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() <<static_cast<UInt64>(0) << Stream::eos;
	atker->send(st);

	struct SWResNotify
	{
		Player * peer;
		bool win;
	};

	SWResNotify notify = { defer, res };
	GameMsgHdr hdr(0x1EA, WORKER_THREAD_WORLD, atker, sizeof(SWResNotify));
	GLOBAL().PushMsg(hdr, &notify);
}

void ServerWarMgr::sendWarSortInfo(Player * player)
{
    if(NULL == player) return;
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02);
    st << leftTime << player->getServerWarChallengeCD();
    st << static_cast<UInt8>(_warSort.size());
    std::map<Player *, UInt8>::iterator it = _warSort.begin();
    for(; it != _warSort.end(); ++ it)
    {
        st << it->second;
        if(it->first)
        {
            st << it->first->getName() << it->first->getCountry();
            st << static_cast<UInt8>(it->first->getMainFighter()->getId());
            st << it->first->GetLev() << it->first->getBattlePoint();
        }
        else
        {
            st << "" << static_cast<UInt8>(0) << static_cast<UInt8>(0);
            st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
        }
    }
	st << Stream::eos;
	player->send(st);
}

void ServerWarMgr::commitLineup(Player * player)
{
    if (!player || !isOpen())
        return;
    switch(_progress)
    {
    case e_war_sign:
        {
            if (!findSWPlayerData(player))
                return;
            insertSWPlayerData(player);
            updateSignupToDB(player);
        }
        break;
	case e_war_challenge:
        {
            std::map<Player *, UInt8>::iterator it = _warSort.find(player);
            if (it == _warSort.end())
                return;
        }
        break;
    case e_war_group:
    case e_war_group_end:
    case e_war_16_end:
    case e_war_16:
    case e_war_8:
    case e_war_4:
    case e_war_2:
    case e_war_1:
        {
            std::map<Player *, UInt8>::iterator it = _warSort.find(player);
            if (it == _warSort.end())
                return;
            GameMsgHdr hdr(0x383, player->getThreadId(), player, 0);
            GLOBAL().PushMsg(hdr, NULL);
        }
        return;
    default:
        return;
    }

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x04);
    st << static_cast<UInt8>(0);
	st << Stream::eos;
	player->send(st);
}

#define WAIQUAN_MAX 13
#define NEIQUAN_MAX 9
const static UInt32 waiQuan[WAIQUAN_MAX][2] = {
    {0xFFFFFFFF, 0xFFFFFFFF},
    {518, 5}, {55, 5}, {502, 5},
    {510, 3}, {55, 3}, {520, 5},
    {518, 3}, {502, 3}, {510, 4},
    {520, 5}, {502, 7}, {0, 0}
};
const static UInt32 neiQuan[NEIQUAN_MAX][2] = {
    {0xFFFFFFFF, 0xFFFFFFFF}, 
    {5041, 3}, {5011, 3}, {51, 2},
    {5041, 2}, {0, 0}, {5011, 2},
    {51, 2}, {5041, 2}
};
void ServerWarMgr::jiJianTai_operate(Player * player)
{
    if(!player || player->GetLev() < 50 || _progress != e_war_challenge)
        return;
    UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI);
    UInt32 data = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
    UInt8 pos = GET_BIT_8(value, 0);
    UInt8 flag = GET_BIT_8(value, 1);
    UInt16 itemId = value >> 16;
    if(GET_BIT_8(data, 0) >= 20 || itemId > 0)     //次数已满,道具未消耗
        return;
    if(!flag && pos <= WAIQUAN_MAX && waiQuan[pos] == 0)
        return;
    if(flag && pos <= NEIQUAN_MAX && neiQuan[pos] == 0)
        return;
    UInt8 rnd = uRand(6) + 1;
    pos += rnd;
    if(!flag)   //外圈
    {
        if(pos >= WAIQUAN_MAX)
        {
            pos -= WAIQUAN_MAX;
            pos += 1;
            itemId = waiQuan[pos][0];
        }
        else
            itemId = waiQuan[pos][0];
        if(itemId == 0)
        {
            pos = 0;
            flag = 1;
        }
    }
    else    //内圈
    {
        if(pos >= NEIQUAN_MAX)
        {
            pos -= NEIQUAN_MAX;
            pos += 1;
            itemId = neiQuan[pos][0];
        }
        else
            itemId = neiQuan[pos][0];
        if(itemId == 0)
        {
            pos = 0;
            flag = 0;
        }
    }
    value = SET_BIT_8(value, 0, pos);
    value = SET_BIT_8(value, 1, flag);
    value |= itemId << 16;
    player->SetVar(VAR_SERVERWAR_JIJIANTAI, value);

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x04) << static_cast<UInt8>(1);
    st << rnd << pos << flag << itemId;
    if(itemId) 
        st << calculateExp(player, flag);
    else    //在跳转点
        st << static_cast<UInt32>(0);
	st << Stream::eos;
	player->send(st);
}

void ServerWarMgr::jiJianTai_complete(Player * player, UInt8 type)
{
    if(!player || player->GetLev() < 50 || _progress != e_war_challenge)
        return;
    UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI);
    UInt32 data = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
    UInt32 itemId = value >> 16;
    UInt8 times = GET_BIT_8(data, 0);
    UInt8 succTimes = GET_BIT_8(data, 1);
    if(times >= 20 || succTimes > times)
        return;
    if(!type && !itemId)    //放弃祭剑
        return;
    if(type)    //立刻完成
    {
        /*
        Fighter * ft = player->getMainFighter();
        if(!ft) return;
        UInt32 pexp = (5+ft->getAcuPraAdd()+ft->getSoulPracticeAddOn())*(1+(ft->getCapacity()-5)*0.16);
        */

        UInt32 itemNum = 0, jiJianCnt = 0;
        UInt8 pos = GET_BIT_8(value, 0);
        UInt8 flag = GET_BIT_8(value, 1);
        if(!flag)
        {
            if(!pos || pos >= WAIQUAN_MAX || itemId != waiQuan[pos][0])
                return;
            itemNum = waiQuan[pos][1];
            jiJianCnt = 2;
            //pexp *= 60*1;
        }
        else
        {
            if(!pos || pos >= NEIQUAN_MAX || itemId != neiQuan[pos][0])
                return;
            itemNum = neiQuan[pos][1];
            jiJianCnt = 5;
            //pexp *= 60*2;
        }
        if(player->GetPackage()->GetItemAnyNum(itemId) < itemNum)
            return;
		SYSMSG_SENDV(104, player, itemId);
		SYSMSG_SENDV(1004, player, itemId);
        ++ succTimes;
        data = SET_BIT_8(data, 1, succTimes);
        struct DelItemInfo
        {
            UInt32 id;
            UInt16 num;
            UInt16 toWhere;
        };
        DelItemInfo item;
        item.id = itemId;
        item.num = itemNum;
        item.toWhere = ToServerWarJijian;
        GameMsgHdr hdr(0x256, player->getThreadId(), player, sizeof(DelItemInfo));
        GLOBAL().PushMsg(hdr, &item);
        //addExp addPExp
        UInt64 exp = calculateExp(player, flag);
        GameMsgHdr hdr1(0x203, player->getThreadId(), player, sizeof(exp));
        GLOBAL().PushMsg(hdr1, &exp);
        /*
        GameMsgHdr hdr2(0x238, getThreadId(), this, sizeof(pexp));
        GLOBAL().PushMsg(hdr2, &pexp);
        */
        GVAR.AddVar(GVAR_SERVERWAR_JIJIANTAI, jiJianCnt);

        Stream st(REP::SERVERWAR_ARENA_OP);
        st << static_cast<UInt8>(0x04) << static_cast<UInt8>(6);
        st << GVAR.GetVar(GVAR_SERVERWAR_JIJIANTAI);
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }
    ++ times;
    value = CLR_BIT_8(value, 2);    //清除itemId
    value = CLR_BIT_8(value, 3);    //清除itemId
    data = SET_BIT_8(data, 0, times);
    player->SetVar(VAR_SERVERWAR_JIJIANTAI, value);
    player->SetVar(VAR_SERVERWAR_JIJIANTAI1, data);

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x04);
    if(type)
        st << static_cast<UInt8>(2) << static_cast<UInt8>(0);
    else
        st << static_cast<UInt8>(3);
    st << times << succTimes;
    st << Stream::eos;
    player->send(st);
}

void ServerWarMgr::jiJianTai_convert(Player * player)
{
    if(!player || player->GetLev() < 50 || _progress != e_war_challenge)
        return;
	if(!player->hasChecked())
		return;
    UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI);
    UInt32 data = player->GetVar(VAR_SERVERWAR_JIJIANTAI);
    UInt8 pos = GET_BIT_8(value, 0);
    UInt8 flag = GET_BIT_8(value, 1);
    if(GET_BIT_8(data, 0) >= 20 || flag > 1) //内圈不允许跳到外圈
        return;
    if(pos >= WAIQUAN_MAX || waiQuan[pos][0] == 0)
        return;
    UInt32 gold = 5;
    if(player->getGold() < gold)
    {
        player->sendMsgCode(0, 1104);
        return;
    }
    pos  = 0;
    flag = 1;
    value = SET_BIT_8(value, 0, pos);
    value = SET_BIT_8(value, 1, flag);
    value = CLR_BIT_8(value, 2);    //清除itemId
    value = CLR_BIT_8(value, 3);    //清除itemId
    player->SetVar(VAR_SERVERWAR_JIJIANTAI, value);

    GameMsgHdr hdr(0x385, player->getThreadId(), player, sizeof(gold));
    GLOBAL().PushMsg(hdr, &gold);

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x04) << static_cast<UInt8>(4);
    st << pos << flag;
    st << Stream::eos;
    player->send(st);
}

void ServerWarMgr::jiJianTai_openBox(Player * player, UInt8 idx)
{
    if(!player || idx > 3 || player->GetLev() < 50 || _progress != e_war_challenge)
        return;
    UInt32 data = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
    UInt8 info = GET_BIT_8(data, 2);
    UInt8 succTimes = GET_BIT_8(data, 1);
    if(GET_BIT(info, idx))
        return;
    UInt8 cnt = 0;
    for(UInt8 i = 0; i < 4; ++ i)
    {
        if(GET_BIT(info, i))
            ++ cnt;
    }
    if(cnt >= succTimes/5 || idx+1 > succTimes/5)
        return;
    //AddItem
    const UInt32 items[4][3][2] = {
        { {15, 2}, {56, 1}, {57, 1} },
        { {9371, 2}, {56, 2}, {57, 2} },
        { {503, 2}, {49, 2}, {514, 2} },
        { {509, 1}, {134, 1}, {1325, 1} },
    };
    for(UInt8 i = 0; i < 3; ++ i)
    {
        struct ItemAdd
        {
            UInt16 item;
            UInt16 num;
            bool bind;
            UInt16 fromWhere;
        };
        ItemAdd ia;
        ia.item = items[idx][i][0];
        ia.num = items[idx][i][1];
        ia.bind = true;
        ia.fromWhere = FromServerWarJijian;
        GameMsgHdr hdr(0x241, player->getThreadId(), player, sizeof(ia));
        GLOBAL().PushMsg(hdr, &ia);
    }

    info = SET_BIT(info, idx);
    data = SET_BIT_8(data, 2, info);
    player->SetVar(VAR_SERVERWAR_JIJIANTAI1, data);

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x04) << static_cast<UInt8>(5);
	st << static_cast<UInt8>(GET_BIT_8(data, 2));
    st << Stream::eos;
    player->send(st);
}

void ServerWarMgr::sendjiJianTaiInfo(Player * player)
{
    if(!player) return;
    UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI);
    UInt32 data = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
    UInt8 flag = GET_BIT_8(value, 1);

    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x04) << static_cast<UInt8>(0);
    st << GVAR.GetVar(GVAR_SERVERWAR_JIJIANTAI);
    st << static_cast<UInt8>(GET_BIT_8(value, 0)) << flag;
    st << static_cast<UInt16>(value >> 16) << calculateExp(player, flag);
    st << static_cast<UInt8>(GET_BIT_8(data, 0)) << static_cast<UInt8>(GET_BIT_8(data, 1));
	st << static_cast<UInt8>(GET_BIT_8(data, 2));
    st << Stream::eos;
	player->send(st);
}

static UInt32 MAX_CNT = 20000;
void ServerWarMgr::enterArena()
{
    if(!isOpen() || _warSort.size() < SERVERWAR_MAX_MEMBER || cfg.isTestPlatform())
        return;
    struct SWarEnterData {
        Stream st;
        std::map<Player *, UInt8> warSort;

        SWarEnterData(Stream& st2, std::map<Player *, UInt8>& warSort2) : st(st2), warSort(warSort2) {}
    };

    UInt8 attr = 0;
    UInt32 jiJianCnt = GVAR.GetVar(GVAR_SERVERWAR_JIJIANTAI);
    if(jiJianCnt >= MAX_CNT * 0.25 && jiJianCnt < MAX_CNT * 0.5)
        attr = 3;
    else if(jiJianCnt >= MAX_CNT * 0.5 && jiJianCnt < MAX_CNT * 0.75)
        attr = 6;
    else if(jiJianCnt >= MAX_CNT * 0.75 && jiJianCnt < MAX_CNT)
        attr = 9;
    else if(jiJianCnt >= MAX_CNT)
        attr = 12;
    Stream st(SERVERWARREQ::ENTER, 0xEE);
    st << attr << static_cast<UInt8>(_warSort.size());

    SWarEnterData * swed = new SWarEnterData(st, _warSort);
    std::map<Player *, UInt8>::iterator it = _warSort.begin();
	GameMsgHdr hdr(0x382, it->first->getThreadId(), it->first, sizeof(SWarEnterData*));
	GLOBAL().PushMsg(hdr, &swed);
}

void ServerWarMgr::enterArena_neice()
{
    if(!isOpen() || !cfg.isTestPlatform())
        return;
    UInt8 attr = 0;
    UInt32 jiJianCnt = GVAR.GetVar(GVAR_SERVERWAR_JIJIANTAI);
    if(jiJianCnt >= MAX_CNT * 0.25 && jiJianCnt < MAX_CNT * 0.5)
        attr = 3;
    else if(jiJianCnt >= MAX_CNT * 0.5 && jiJianCnt < MAX_CNT * 0.75)
        attr = 6;
    else if(jiJianCnt >= MAX_CNT * 0.75 && jiJianCnt < MAX_CNT)
        attr = 9;
    else if(jiJianCnt >= MAX_CNT)
        attr = 12;

    SWPDSort signSortTmp = _signSort;
    int size = signSortTmp.size();
    int tmpSize = 0;
    if(size % 10)
        tmpSize = 10 - size % 10;

    std::map<Player *, UInt8> warSortTmp;
    UInt8 i = 0;
    size_t playerSize = globalPlayers.size();
    while(tmpSize > 0)
    {
        UInt32 rnd = uRand(playerSize);
        GlobalPlayers::iterator it = globalPlayers.begin();
        std::advance(it, rnd);
        if(it != globalPlayers.end() && it->second && it->second->GetLev() >= LIMIT_LEVEL)
        {
            warSortTmp.insert(std::make_pair(it->second, i++));
            tmpSize --;
        }
    }
    for(int j = size%10; j > 0; -- j)
    {
        UInt32 rnd = uRand(signSortTmp.size());
        SWPDSort::iterator it = signSortTmp.begin();
        std::advance(it, rnd);
        if(it != signSortTmp.end() && (*it).player)
        {
            warSortTmp.insert(std::make_pair((*it).player, i++));
            signSortTmp.erase(it);
        }
    }

    size = size / 10 * 10;
    while(size-- > 0)
    {
        if(warSortTmp.size() == 10)
        {
            struct SWarEnterData {
                Stream st;
                std::map<Player *, UInt8> warSort;

                SWarEnterData(Stream& st2, std::map<Player *, UInt8>& warSort2) : st(st2), warSort(warSort2) {}
            };

            Stream st(SERVERWARREQ::ENTER, 0xEE);
            st << attr << static_cast<UInt8>(warSortTmp.size());

            SWarEnterData * swed = new SWarEnterData(st, warSortTmp);
            std::map<Player *, UInt8>::iterator it = warSortTmp.begin();
            GameMsgHdr hdr(0x382, it->first->getThreadId(), it->first, sizeof(SWarEnterData*));
            GLOBAL().PushMsg(hdr, &swed);

            i = 0;
            warSortTmp.clear();
        }

        UInt32 rnd = uRand(signSortTmp.size());
        SWPDSort::iterator it = signSortTmp.begin();
        std::advance(it, rnd);
        if(it != signSortTmp.end() && (*it).player)
        {
            warSortTmp.insert(std::make_pair((*it).player, i++));
            signSortTmp.erase(it);
        }
    }
}

//TODO
bool clear_var_jiJian(Player * player, void * data )
{
    if(player == NULL)
        return true;

    player->SetVar(VAR_SERVERWAR_JIJIANTAI, 0);
    player->SetVar(VAR_SERVERWAR_JIJIANTAI1, 0);
    return true;
}

void ServerWarMgr::readFrom(BinaryReader& brd)
{
	UInt8 progress = 0, reg = 0;
	brd >> _session >> reg >> progress >> _nextTime;
    bool fStatus = false;
	if(progress != _progress)
	{
		_progress = progress;
		_notified = 0;
        fStatus = true;
	}
    if(_progress != e_war_nextbegin)
        GObject::World::setArenaState(GObject::ARENA_XIANJIE_CHUANQI);

	switch(_progress)
	{
    case e_war_nextbegin:
		if(!_signSort.empty())
        {
            DB1().PushUpdateData("DELETE FROM `arena_serverWar` WHERE `type` = 1");
			_signSort.clear();
        }
	case e_war_sign:
        clearPreliminaryBattels();
		if(!_warSort.empty())
            clearWarSort();
        if(!_playerBet.empty())
        {
			_playerBet.clear();
            DB1().PushUpdateData("DELETE FROM `arena_serverWar_bet`");
        }
        //TODO
        if(cfg.isTestPlatform())
        {
            globalPlayers.enumerate(clear_var_jiJian, static_cast<void *>(NULL));
        }
        break;
    case e_war_sign_end:
        setWarSort();
		break;
	case e_war_challenge:
		break;
	case e_war_challenge_end:
        enterArena();
        enterArena_neice();
		break;
    case e_war_group:
        if(!GVAR.GetVar(GVAR_SERVERWAR_ISENTER))
        {
            setWarSort();
            enterArena();
            enterArena_neice();
        }
        break;
    case e_war_group_end:
        readTeams(brd);
        break;
	case e_war_16:
        if(reg == 1)
            readTeams(brd);
        readHistories(brd);
		break;
	case e_war_16_end:
	case e_war_8:
    case e_war_4:
    case e_war_2:
    case e_war_1:
    case e_war_end:
        if(reg == 1)
        {
            readTeams(brd);
            readHistories(brd);
        }

        bool oldstatus = (_status > 0);
        readElimination(brd);
        if(oldstatus != (_status > 0))
            fStatus = true;
        break;
	}
    if(fStatus)
    {
        globalPlayers.enumerate(server_enum_send_status, static_cast<void *>(NULL));
    }

    if(!_loaded || _progress == e_team_sign)
    {
		_loaded = true;
        globalPlayers.enumerate(server_enum_send_active, static_cast<void *>(NULL));
    }
}

void ServerWarMgr::readTeams(BinaryReader& brd)
{
    clearPreliminaryBattels();
    for(UInt8 i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
    {
        UInt32 count = 0;
        brd >> count;
        for(UInt32 z = 0; z < count; ++ z)
        {
            ServerPreliminaryPlayer spp;
            int cid, sid;
            brd >> cid >> sid >> spp.serverName >> spp.battlePoint >> spp.support >> spp.attrRatio;
            spp.channelId = cid;
            spp.serverId = sid;
            spp.readPlayerInfo(brd);

            ServerPreliminaryPlayerListIterator it = _preliminaryServers_list[i].insert(_preliminaryServers_list[i].end(), spp);
            _preliminaryServers[i][sid] = it;
            _preliminaryServers_list_set[i].insert(it);
        }

        for(std::map<Player *, PlayerBetData>::iterator it = _playerBet.begin(); it != _playerBet.end(); ++ it)
        {
            for(int i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
            {
                std::vector<BetInfo>& blist = it->second.betList[i][0];
                if(blist.empty())
                    continue;

                for(std::vector<BetInfo>::iterator bit = blist.begin(); bit != blist.end(); ++ bit)
                {
                    BetInfo& bi = *bit;
                    if(bi.round == 1 && bi.group == i+1 && bi.state == 0)
                    {
                        ServerPreliminaryPlayerListIterator pit = _preliminaryServers_list[i].begin();
                        if(bi.pos > _preliminaryServers_list[i].size())
                            continue;
                        std::advance(pit, bi.pos);
                        if(pit != _preliminaryServers_list[i].end())
                        {
                            ServerPreliminaryPlayer& spp = *pit;
                            spp.betMap[it->first] = bi.type;
                        }
                    }
                }
            }
        }
    }
}

void ServerWarMgr::readHistories(BinaryReader& brd)
{
    for(UInt8 i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
    {
        UInt32 cnt = 0;
        brd >> cnt;
        for(size_t j = 0; j < cnt; ++ j)
        {
            int sid = 0, cid = 0;
            std::string serverName;
            brd >> cid >> sid >> serverName;
            UInt16 bCount = 0;
            brd >> bCount;
            std::vector<ServerPreliminaryBattle> spbVec;
            for(UInt16 p = 0; p < bCount; ++ p)
            {
                ServerPreliminaryBattle spb;
                spb.serverName = serverName;
                spb.readPreReport(brd);
                spbVec.push_back(spb);
            }

            //本服淘汰赛战报放在此处！
            ServerPreliminaryPlayerListMap::iterator spit = _preliminaryServers[i].find(sid);
            if(spit == _preliminaryServers[i].end())
                continue;
            ServerPreliminaryPlayer& spp = *(spit->second);
            spp.battlesVec.clear();
            spp.battlesVec = spbVec;
        }
    }
}

void ServerWarMgr::readElimination(BinaryReader& brd)
{
    for(UInt8 i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
    {
        for(int j = 0; j < 16; ++ j)
        {
            int cid, sid;
            UInt32 battlePoint = 0;
            UInt32 support = 0;
            std::string serverName;
            UInt8 attrRatio = 0;
            brd >> cid >> sid >> serverName >> battlePoint >> support >> attrRatio;
            _finalIdx[i][0][j] = j;
            _finals[i][j].channelId = cid;
            _finals[i][j].serverId = sid;
            _finals[i][j].serverName = serverName;
            _finals[i][j].battlePoint = battlePoint;
            _finals[i][j].support = support;
            _finals[i][j].attrRatio = attrRatio;
            _finals[i][j].readPlayerInfo(brd);

        }
        _round = 0;
        UInt8 status;
        brd >> status >> _round;
        if(status != _status)
        {
            _status = status;
            if(_notified)
                _notified = 0;
        }

        for(int j = 0; j < aIndex[_round]; ++ j)
        {
            UInt8 bCount = 0;
            brd >> bCount >> _finalBattles[i][j].wonFlag;
            _finalBattles[i][j].reset();
            for(UInt8 p = 0; p < bCount; ++ p)
            {
                PairServerReport psr;
                psr.readServerReport(brd);
                _finalBattles[i][j].battles.push_back(psr);
            }
        }

        calcFinalBet(i);
    }
    if(!_notified)
        _notified = 1;
}

void ServerWarMgr::pushPreliminary(BinaryReader& brd)
{
    int cid = 0, sid = 0;
    std::string serverName;
	brd >> cid >> sid >> serverName;

    ServerPreliminaryBattle spb;
    spb.serverName = serverName;
    spb.readPreReport(brd);

    UInt8 won = spb.won;
    UInt8 type = spb.type;
    if(type < 0 || type > SWAR_ENTERED_MAX-1)
        return;

    GET_PROGRESS_NAME(p, -1);
    int g_type = 709;
    if(type == SWAR_ENTERED_STAGE_1)
        g_type = 710;
    else if(type == SWAR_ENTERED_STAGE_2)
        g_type = 711;
    SYSMSGV(g, g_type);

    if(won == 3 || won == 4 || won == 2)
    {
        bool flag[5] = {false, false, false, true, true};
        ServerPreliminaryPlayerListMap::iterator pit = _preliminaryServers[type-1].find(sid);
        if(pit != _preliminaryServers[type-1].end())
        {
            ServerPreliminaryPlayerListIterator ppit = pit->second;
            ServerPreliminaryPlayer& pp = *ppit;
            UInt16 pos = std::distance(_preliminaryServers_list[type-1].begin(), ppit);
            calcBet(pp, pos, 0, type-1, flag[won], p);
        }
    }

    if(!cfg.isTestPlatform() && (cid != cfg.channelNum || sid != cfg.serverNo))
        return;
    ServerPreliminaryPlayerListMap::iterator spit = _preliminaryServers[type-1].find(sid);
    if(spit == _preliminaryServers[type-1].end())
        return;

    ServerPreliminaryPlayer& spp = *(spit->second);
    if(cfg.isTestPlatform() && spp.battlesVec.size() && spp.battlesVec.back().won == 2)
        return;

    if(won == 3 || won == 4)
    {   //进16强
        UInt32 twon = 0, tloss = 0;
        for(std::vector<ServerPreliminaryBattle>::iterator it = spp.battlesVec.begin(); it != spp.battlesVec.end(); ++ it)
        {
            if(it->won == 1 || it->won == 3)
                ++ twon;
            else
                ++ tloss;
        }
        const UInt32 score[] = { 25, 50 };
        UInt32 award = score[0]*tloss + score[1]*twon;

        SYSMSGV(title, 810, g, p);
        SYSMSGV(content, 811, _session, spp.serverId, p, twon, tloss, award, g);
        if(cfg.isTestPlatform())
        {
            sendTeamMail_neice(spp, title, content);

            SYSMSGV(content2, 812, _session, spp.serverId, g, twon, tloss, p, award);
            sendTeamMail_neiceJiJian(spp, title, content2);
        }
        else
        {
            sendTeamMail(title, content);
            EnumMailStruct mailStruct = EnumMailStruct(type-1, 0, _session, true, twon, tloss, award);
            globalPlayers.enumerate(server_sendMail, &mailStruct);
        }
    }
    else
    {
        spp.battlesVec.push_back(spb);

        UInt32 score1[3] = { 25, 50, 25 };
        if(cfg.isTestPlatform())
            addLonghun_neice(spp, score1[won]);
        else
        {
            addLonghun(score1[won]);
            globalPlayers.enumerate(server_addLonghun, &score1[won]);
        }
        if(won == 2)    //16强被淘汰
        {
            UInt32 twon = 0, tloss = 0;
            for(std::vector<ServerPreliminaryBattle>::iterator it = spp.battlesVec.begin(); it != spp.battlesVec.end(); ++ it)
            {
                if(it->won == 1 || it->won == 3)
                    ++ twon;
                else
                    ++ tloss;
            }
            const UInt32 score[] = { 25, 50 };
            UInt32 award = score[0]*tloss + score[1]*twon;

            SYSMSGV(title, 813, g, p);
            SYSMSGV(content, 814, _session, spp.serverId, g, twon, tloss, award);
            if(cfg.isTestPlatform())
            {
                sendTeamMail_neice(spp, title, content);

                SYSMSGV(content2, 815, _session, spp.serverId, g, twon, tloss, award);
                sendTeamMail_neiceJiJian(spp, title, content2);
            }
            else
            {
                sendTeamMail(title, content);

                EnumMailStruct mailStruct = EnumMailStruct(type-1, 0, _session, false, twon, tloss, award);
                globalPlayers.enumerate(server_sendMail, &mailStruct);

                EnumMailStruct mailStruct1 = EnumMailStruct(type-1, 0, _session);
                globalPlayers.enumerate(server_sendMail, &mailStruct1);
            }
        }

        if(cfg.isTestPlatform() && sid != cfg.serverNo)
            return;
        //更新16强战报
        Stream st(REP::SERVERWAR_ARENA_OP);
        st << static_cast<UInt8>(0x03) << static_cast<UInt8>(1);
        st << spp.serverId << spp.serverName;
        spp.battlesVec[spp.battlesVec.size()-1].appendPreReport(st);
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }
}

void ServerWarMgr::addLonghun(UInt32 money)
{
    if(money == 0) return;
    std::map<Player *, UInt8>::iterator it = _warSort.begin();
    for(; it != _warSort.end(); ++ it)
    {
        if(!it->first)
            continue;
        GameMsgHdr hdr(0x384, it->first->getThreadId(), it->first, sizeof(money));
        GLOBAL().PushMsg(hdr, &money);
    }
}

void ServerWarMgr::calcFinalBet(int i)
{
    UInt32 r = _round;
    if(_status == 0)
        ++ r;
    if(r > 1)
    {
        GET_PROGRESS_NAME(p, _round+1);
        for(UInt32 j = 1; j < r; ++ j)
        {
            int starti = aIndex[j - 1], endi = aIndex[j];
            for(int k = starti; k < endi; ++ k)
            {
                if(_finalBattles[i][k].winner(j-1) == 0)
                {
                    UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2];
                    _finalIdx[i][j][k - starti] = nidx;
                    if(_status == 0 && j == r - 1)
                    {
                        int pos1 = nidx;
                        int pos2 = _finalIdx[i][j-1][(k - starti) * 2 + 1];
                        calcBet(_finals[i][pos1], pos1, j, i, true, p);
                        calcBet( _finals[i][pos2], pos2, j, i, false, p);
                    }
                }
                else
                {
                    UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2 + 1];
                    _finalIdx[i][j][k - starti] = nidx;
                    if(_status == 0 && j == r - 1)
                    {
                        int pos1 = nidx;
                        int pos2 = _finalIdx[i][j-1][(k - starti) * 2];
                        calcBet(_finals[i][pos1], pos1, j, i, true, p);
                        calcBet( _finals[i][pos2], pos2, j, i, false, p);
                    }
                }
            }
        }
    }
    if(_loaded)
    {
        Stream st(REP::SERVERWAR_ARENA_OP);
        st << static_cast<UInt8>(0x03);
        st << static_cast<UInt8>(2) << Stream::eos;
        NETWORK()->Broadcast(st);
        if(!_notified)
        {
            if(_status == 0 && _round > 0)
            {
                GET_PROGRESS_NAME(pp, _round+1);
                SYSMSGV(g, 710 + i);
                SYSMSGV(title, 810, g, pp);
                SYSMSGV(title2, 813, g, pp);
                if(_round < 4)
                {
                    int starti = aIndex[_round - 1];
                    int endi = aIndex[_round];
                    for(int idx = starti; idx < endi; ++ idx)
                    {
                        ServerEliminationPlayer& ep1 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2]];
                        ServerEliminationPlayer& ep2 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2 + 1]];
                        UInt8 winCount = _finalBattles[i][idx].winCount(_round-1);
                        const UInt8 totalCount[4] = { 3, 5, 5, 7 };
                        const UInt8 winCount_mod[4] = {1, 2, 2, 3};
                        if(winCount > winCount_mod[_round-1])
                        {
                            if(!cfg.isTestPlatform())    //公测区
                            {
                                if(ep1.serverId == cfg.serverNo)
                                {
                                    SYSMSGV(content, 816, _session, cfg.serverNo, pp, winCount, totalCount[_round-1] - winCount);
                                    sendTeamMail(title, content);
                                }
                                if(ep2.serverId == cfg.serverNo)
                                {
                                    SYSMSGV(content, 817, _session, cfg.serverNo, pp, totalCount[_round-1] - winCount, winCount);
                                    sendTeamMail(title2, content);
                                }
                            }
                            else    //内测区
                            {
                                SYSMSGV(content, 816, _session, ep1.serverId, pp, winCount, totalCount[_round-1] - winCount);
                                sendTeamMail_neice(ep1, title, content);

                                SYSMSGV(content2, 817, _session, ep2.serverId, pp, totalCount[_round-1] - winCount, winCount);
                                sendTeamMail_neice(ep2, title2, content2);
                            }
                        }
                        else
                        {
                            if(!cfg.isTestPlatform())    //公测区
                            {
                                if(ep2.serverId == cfg.serverNo)
                                {
                                    SYSMSGV(content, 816, _session, cfg.serverNo, pp, totalCount[_round-1] - winCount, winCount);
                                    sendTeamMail(title, content);
                                }
                                if(ep1.serverId == cfg.serverNo)
                                {
                                    SYSMSGV(content, 817, _session, cfg.serverNo, pp, winCount, totalCount[_round-1] - winCount);
                                    sendTeamMail(title2, content);
                                }
                            }
                            else    //内测区
                            {
                                SYSMSGV(content, 816, _session, ep2.serverId, pp, totalCount[_round-1] - winCount, winCount);
                                sendTeamMail_neice(ep2, title, content);

                                SYSMSGV(content2, 817, _session, ep1.serverId, pp, winCount, totalCount[_round-1] - winCount);
                                sendTeamMail_neice(ep1, title2, content2);
                            }
                        }
                    }
                }
                else
                {
                    int idx = 0;
                    UInt8 nidx = 0;
                    UInt8 nidx2 = 0xFF;
                    for(UInt8 j = 1; j < 5; ++ j)
                    {
                        int starti = aIndex[j - 1];
                        int endi = aIndex[j];
                        for(idx = starti; idx < endi; ++ idx)
                        {
                            if(_finalBattles[i][idx].winner(j-1) == 0)  //攻方赢
                            {
                                nidx = _finalIdx[i][j-1][(idx - starti) * 2 + 1];   //淘汰
                                if(j == 4)
                                    nidx2 = _finalIdx[i][j-1][(idx - starti) * 2];  //冠军
                            }
                            else        //守方赢
                            {
                                nidx = _finalIdx[i][j-1][(idx - starti) * 2];
                                if(j == 4)
                                    nidx2 = _finalIdx[i][j-1][(idx - starti) * 2 + 1];
                            }
                            ServerEliminationPlayer& ep = _finals[i][nidx];
                            if(!cfg.isTestPlatform())    //公测区
                            {
                                if(ep.serverId == cfg.serverNo)
                                {
                                    giveTeamLastAward(i, j+1, 0);
                                    EnumMailStruct mailStruct = EnumMailStruct(i, j+1, _session);
                                    globalPlayers.enumerate(server_sendMail, &mailStruct);
                                }
                            }
                            else    //内测区
                            {
                                giveTeamLastAward_neice(ep, i, j+1, 0);
                            }

                            if(nidx2 != (UInt8)(0xFF))
                            {
                                ServerEliminationPlayer& ep = _finals[i][nidx2];
                                if(!cfg.isTestPlatform())    //公测区
                                {
                                    if(ep.serverId == cfg.serverNo)
                                    {
                                        giveTeamLastAward(i, j+1, 1);
                                        EnumMailStruct mailStruct = EnumMailStruct(i, j+1, _session, true);
                                        globalPlayers.enumerate(server_sendMail, &mailStruct);
                                    }
                                }
                                else    //内测区
                                {
                                    giveTeamLastAward_neice(ep, i, j+1, 1);
                                    EnumMailStruct mailStruct = EnumMailStruct(i, j+1, _session);
                                    globalPlayers.enumerate(server_sendMail, &mailStruct);
                                }
                                if(i == 0)  //天榜第一刷仙界boss
                                {
                                    UInt8 type = 1;
                                    GameMsgHdr hdr(0x1EB, WORKER_THREAD_WORLD, NULL, sizeof(type));
                                    GLOBAL().PushMsg(hdr, &type);
                                }
                            }
                        }
                    }
                }
            }
        }
        if(_status == 0 && _progress != e_team_8)
        {
            for(int j = 0; j < 16; ++ j)
                _finals[i][j].resetBet();

            for(std::map<Player *, PlayerBetData>::iterator it = _playerBet.begin(); it != _playerBet.end(); ++ it)
            {
                for(int sidx = 1; sidx < 5; ++ sidx)
                {
                    std::vector<BetInfo>& blist = it->second.betList[i][sidx];
                    if(blist.empty())
                        continue;
                    int cnt = blist.size();
                    for(int j = 0; j < cnt; ++ j)
                    {
                        BetInfo& bi = blist[j];
                        if(bi.group == i + 1 && bi.recieved == 0)
                        {
                            _finals[i][bi.pos].betMap[it->first] = bi.type;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(std::map<Player *, PlayerBetData>::iterator it = _playerBet.begin(); it != _playerBet.end(); ++ it)
        {
            for(int sidx = 1; sidx < 5; ++ sidx)
            {
                std::vector<BetInfo>& blist = it->second.betList[i][sidx];
                if(blist.empty())
                    continue;
                int cnt = blist.size();
                for(int j = 0; j < cnt; ++ j)
                {
                    BetInfo& bi = blist[j];
                    if(bi.group == i + 1 && bi.recieved == 0)
                    {
                        _finals[i][bi.pos].betMap[it->first] = bi.type;
                    }
                }
            }
        }
    }
}

void ServerWarMgr::calcBet(ServerPreliminaryPlayer& pp, UInt16 pos, UInt8 state, UInt8 group, bool won, const char * t)
{
	for(std::map<Player *, UInt8>::iterator it = pp.betMap.begin(); it != pp.betMap.end(); ++ it)
	{
        Player* player = it->first;
        std::map<Player *, PlayerBetData>::iterator iter = _playerBet.find(player);
		std::vector<BetInfo>& blist = iter->second.betList[group][state];

        bool recieved = true;
		for(std::vector<BetInfo>::iterator bit = blist.begin(); bit != blist.end(); ++ bit)
		{
            if(bit->pos == pos)
            {
                if(bit->recieved == 0)
                {
                    recieved = false;
                    bit->recieved = 1;
                    DB1().PushUpdateData("REPLACE INTO `arena_serverWar_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u, %u)", player->getId(), bit->round, bit->state, bit->group, bit->recieved, bit->pos, bit->type);
                }
                break;
            }
        }
        if(recieved)
            continue;

        SYSMSGV(g, 710 + group);
		if(won)
		{
            SYSMSGV(title, 821, t, pp.serverId);

            UInt32 rew = it->second == 0 ? 100 : 50;
            GameMsgHdr hdr(0x384, player->getThreadId(), player, sizeof(rew));
            GLOBAL().PushMsg(hdr, &rew);

            SYSMSGV(content, 822, _session, t, g, pp.serverId, rew);
            Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(player, mail);
		}
		else
		{
            SYSMSGV(title, 823, t, pp.serverId);
			SYSMSGV(content, 824, _session, t, g, pp.serverId);
			Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(player, mail);
		}
	}
}

void ServerWarMgr::sendTeamMail(const std::string& title, const std::string& content)
{
    if(title.empty() || content.empty())
        return;
    std::map<Player *, UInt8>::iterator it = _warSort.begin();
    for(; it != _warSort.end(); ++ it)
    {
        if(it->first)
        {
            Mail * mail = it->first->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(it->first, mail);
        }
    }
}

void ServerWarMgr::sendTeamMail_neice(ServerEliminationPlayer& sep, const std::string& title, const std::string& content)
{
    if(title.empty() || content.empty())
        return;
    PInfoSort::iterator it = sep.pInfoSet.begin();
    for(; it != sep.pInfoSet.end(); ++ it)
    {
        Player * player = globalPlayers[(*it).playerId];
        if(player)
        {
            Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(player, mail);
        }
    }
}

void ServerWarMgr::sendTeamMail_neiceJiJian(ServerEliminationPlayer& sep, const std::string& title, const std::string& content)
{
    if(title.empty() || content.empty())
        return;
    PInfoSort::iterator it = sep.pInfoSet.begin();
    for(; it != sep.pInfoSet.end(); ++ it)
    {
        Player * player = globalPlayers[(*it).playerId];
        if(player)
        {
            UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
            if(!GET_BIT_8(value, 1))
                continue;
            Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            writeMailLog(player, mail);
        }
    }
}

void ServerWarMgr::addLonghun_neice(ServerEliminationPlayer& sep, UInt32 money)
{
    if(money == 0) return;
    PInfoSort::iterator it = sep.pInfoSet.begin();
    for(; it != sep.pInfoSet.end(); ++ it)
    {
        Player * player = globalPlayers[(*it).playerId];
        if(player)
        {
            UInt32 value = player->GetVar(VAR_SERVERWAR_JIJIANTAI1);
            if(GET_BIT_8(value, 1))     //祭剑获得
                money *= 2;
            GameMsgHdr hdr(0x384, player->getThreadId(), player, sizeof(money));
            GLOBAL().PushMsg(hdr, &money);
        }
    }
}

void ServerWarMgr::giveTeamLastAward(UInt8 group, UInt8 idx, UInt8 type)
{
    //group 0天榜 1地榜
    if(group > 1 || idx < 1 || idx > 5 || type > 1)
        return;
    SYSMSGV(g, 710 + group);
    SYSMSGV(title, 818, g);
    std::map<Player *, UInt8>::iterator it = _warSort.begin();
    for(; it != _warSort.end(); ++ it)
    {
        Player * pl = it->first;
        if(pl == NULL) continue;
        UInt32 data = ArenaLastAwards[group][idx+type-1][1];    //龙魂
        GameMsgHdr hdr(0x384, pl->getThreadId(), pl, sizeof(data));
        GLOBAL().PushMsg(hdr, &data);

        SYSMSGV(content, 819, _session, cfg.serverNo, data);
        UInt16 itemId = ArenaLastAwards[group][idx+type-1][0];
        MailPackage::MailItem item[] = { {itemId, 1}, };
        MailItemsInfo itemsInfo(item, ArenaAward, 1);
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
             mailPackageManager.push(mail->id, item, 1, true);

        UInt8 title = ArenaLastAwards[group][idx+type-1][2];
        if(title)
        {
            UInt32 now = TimeUtil::Now();
            pl->setTitle(title, TimeUtil::SharpDayT(1) - now + 7 * 86400);
        }
    }
    UInt8 prac = ArenaLastAwards[group][idx+type-1][3];
    if(prac)
    {
        GVAR.SetVar(GVAR_SERVERWAR_XIUWEI, prac, TimeUtil::SharpDayT(1)+100);
        globalPlayers.enumerate(server_addBuffData, static_cast<void *>(NULL));
    }
}

void ServerWarMgr::giveTeamLastAward_neice(ServerEliminationPlayer& sep, UInt8 group, UInt8 idx, UInt8 type)
{
    //group 0天榜 1地榜
    if(group > 1 || idx < 1 || idx > 5 || type > 1)
        return;
    SYSMSGV(g, 710 + group);
    SYSMSGV(title, 818, g);
    PInfoSort::iterator it = sep.pInfoSet.begin();
    for(; it != sep.pInfoSet.end(); ++ it)
    {
        Player * pl = globalPlayers[(*it).playerId];
        if(pl == NULL) continue;
        UInt32 data = ArenaLastAwards[group][idx+type-1][1];    //龙魂
        GameMsgHdr hdr(0x384, pl->getThreadId(), pl, sizeof(data));
        GLOBAL().PushMsg(hdr, &data);

        SYSMSGV(content, 819, _session, cfg.serverNo, data);
        UInt16 itemId = ArenaLastAwards[group][idx+type-1][0];
        MailPackage::MailItem item[] = { {itemId, 1}, };
        MailItemsInfo itemsInfo(item, ArenaAward, 1);
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
             mailPackageManager.push(mail->id, item, 1, true);

        UInt8 title = ArenaLastAwards[group][idx+type-1][2];
        if(title)
        {
            UInt32 now = TimeUtil::Now();
            pl->setTitle(title, TimeUtil::SharpDayT(1) - now + 7 * 86400);
        }
    }
    UInt8 prac = ArenaLastAwards[group][idx+type-1][3];
    if(prac && prac == SERVERWAR_VALUE_XIUWEI5)
    {
        GVAR.SetVar(GVAR_SERVERWAR_XIUWEI, prac, TimeUtil::SharpDayT(1)+100);
        globalPlayers.enumerate(server_addBuffData, static_cast<void *>(NULL));
    }
}

void ServerWarMgr::sendStatus(Player* pl)
{
    if(pl == NULL) return;
    // 0 - 可报名阶段
    // 1 - 挑战阶段
    // 2 - 分组阶段
    // 3 - 16强投注阶段
    // 4 - 16强战斗阶段
    // 5 - 8强投注阶段
    // 6 - 8强战斗阶段
    // 7 - 4强投注阶段
    // 8 - 4强战斗阶段
    // 9 - 半决赛投注阶段
    // 10 - 半决赛战斗阶段
    // 11 - 决赛阶段投注阶段
    // 12 - 决赛阶段战斗阶段
    // 13 - 决赛阶段结束（不可报名
    // 14 - 无法战斗
    //                     0  1  2  3  4  5   6  7   8  9  10 11 12  13
    const UInt8 state[] = {0, 1, 2, 4, 6, 8, 10, 12, 1, 2, 3, 5, 13, 14};
    UInt8 progress = state[_progress];
    if(_progress >= e_war_4 && _progress <= e_war_1 && _status == 0)
        progress -= 1;
	Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x00);
    st << _session << progress;
    if(progress >= 3)
    {
        UInt8 group = 0;
        for(UInt8 i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
        {
            ServerPreliminaryPlayerListMap::iterator it = _preliminaryServers[i].find(cfg.serverNo);
            if(it != _preliminaryServers[i].end())
            {
                group = i+1;
                break;
            }
        }
        st << group;
    } 
    st << Stream::eos;
    pl->send(st);
}

void ServerWarMgr::sendActive(Player* pl)
{
    if(pl == NULL) return;
    if(arena.active())
    {
        arena.sendActive(pl);
        return;
    }
    else if(teamArenaMgr.active())
    {
        teamArenaMgr.sendActive(pl);
        return;
    }
    Stream st(REP::SERVER_ARENA_OP);
    UInt8 flag = static_cast<UInt8>(active() ? ARENA_XIANJIE_CHUANQI : 0);
    if(_progress == e_war_nextbegin)
        flag |= 0x10;
    st << static_cast<UInt8>(0) << flag << Stream::eos;
    pl->send(st);
}

void ServerWarMgr::sendSignupInfo(Player * pl)
{
    if(pl == NULL || _progress != e_war_sign)
        return;
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;

	Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01);
    st << static_cast<UInt8>(findSWPlayerData(pl) ? 1 : 0);
    st << leftTime << static_cast<UInt16>(_signSort.size());
    st << Stream::eos;
    pl->send(st);
}

void ServerWarMgr::bet(BinaryReader& brd, Player * player)
{
    if(!player) return;
    UInt8 state = 0, group = 0, tael = 0;
    brd >> state >> tael >> group;
    if(state > 4 || tael > 1 || group < 1 || group > 2)
        return;
    UInt16 pos = 0;
    int serverId = 0;
    if(state < 1)
        brd >> serverId;
    else
        brd >> pos;
    UInt8 r = 0;
    if(player->GetPackage()->GetItemAnyNum(ARENA_BET_ITEM1) < 1 && tael == 1)
        r = 3;
    else if(player->GetPackage()->GetItemAnyNum(ARENA_BET_ITEM2) < 1 && tael == 0)
        r = 4;
    else if(state < 1)
        r = bet1(player, state, group, serverId, tael);
    else
        r = bet2(player, state, group, pos, tael);
    if(r == 0xFF)
        return;
    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x02);
    st << r << state << group;
    if(state < 1)
        st << serverId;
    else
        st << pos;
    st << Stream::eos;
    player->send(st);
}

UInt8 ServerWarMgr::bet1( Player * player, UInt8 state, UInt8 group, int sid, UInt8 type )
{
	if(group < 1 || group > 2 || state != 0)
		return 0xFF;
    if(progress_accept[state][_progress] == 0)
        return 0xFF;

    int gIdx = group - 1;
    ServerPreliminaryPlayerListMap::iterator pit = _preliminaryServers[gIdx].find(sid);
    if(pit == _preliminaryServers[gIdx].end())
        return 0xFF;

    ServerPreliminaryPlayerListIterator it = pit->second;
    ServerPreliminaryPlayer& pp = *it;

    UInt16 pos = std::distance(_preliminaryServers_list[gIdx].begin(), it);
    std::map<Player*, PlayerBetData>::iterator ait = _playerBet.find(player);
    if((ait != _playerBet.end() && ait->second.betList[gIdx][state].size() >= 1))
        return 0xFF;
    if(pp.betMap.find(player) != pp.betMap.end())
        return 2;
    pp.betMap[player] = type;

	Stream st(SERVERWARREQ::BET, 0xEE);
	st << group << pos << Stream::eos;
	NETWORK()->SendToServerWar(st);
	BetInfo binfo;
	binfo.state = state;
	binfo.round = 1;
	binfo.group = group;
	binfo.recieved = 0;
	binfo.pos = pos;
	binfo.type = type;
	_playerBet[player].betList[gIdx][state].push_back(binfo);

	DB1().PushUpdateData("REPLACE INTO `arena_serverWar_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.state, binfo.group, binfo.recieved, binfo.pos, binfo.type);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = type;
	GLOBAL().PushMsg(hdr, &data);

	return type;
}

UInt8 ServerWarMgr::bet2( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type )
{
    //group 1天榜 2地榜
	if(group < 1 || group > 2 || state > 4 || state < 1 || _status > 0)
		return 0xFF;
    if(progress_accept[state][_progress] == 0)
        return 0xFF;

    UInt16 pos2 = pos;
    int gIdx = group - 1;
    UInt8 fidx = 0;
    switch(state)
    {
    case 1:     //_round 0
        fidx = pos2 >> _round;
        break;
    case 2:     //_round 1
    case 3:     //_round 2
    case 4:     //_round 3
        pos2 = _finalIdx[gIdx][0][pos];
        fidx = pos2 >> _round;
        break;
    }
    if(_finalIdx[gIdx][_round][fidx] != pos2 || pos > 15)
        return 0xFF;

    if( (fidx % 2) == 1)
        fidx -= 1;
    else
        fidx += 1;

    UInt8 pos3 = _finalIdx[gIdx][_round][fidx];
    if(_finals[gIdx][pos2].betMap.find(player) != _finals[gIdx][pos2].betMap.end()
            || _finals[gIdx][pos3].betMap.find(player) != _finals[gIdx][pos3].betMap.end())
        return 2;
    _finals[gIdx][pos2].betMap[player] = type;

	Stream st(SERVERWARREQ::BET, 0xEE);
	st << group << pos2 << Stream::eos;
	NETWORK()->SendToServerWar(st);
	BetInfo binfo;
	binfo.state = state;
	binfo.round = _round + 1;
	binfo.group = group;
	binfo.recieved = 0;
	binfo.pos = pos;
	binfo.type = type;
	_playerBet[player].betList[gIdx][state].push_back(binfo);
	DB1().PushUpdateData("REPLACE INTO `arena_serverWar_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.state, binfo.group, binfo.recieved, binfo.pos, binfo.type);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = type;
	GLOBAL().PushMsg(hdr, &data);
	return type;
}

void ServerWarMgr::updateBattlePoint(BinaryReader& brd)
{
    int cid = 0, sid = 0;
    UInt64 pid = 0;
    UInt32 battlePoint = 0, battlePoint1 = 0;
    brd >> cid >> sid >> pid >> battlePoint >> battlePoint1;

    for(UInt8 i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
    {
        ServerPreliminaryPlayerListMap::iterator pit = _preliminaryServers[i].find(sid);
        if(pit != _preliminaryServers[i].end())
        {
            ServerPreliminaryPlayerListIterator ppit = pit->second;
            ServerPreliminaryPlayer& pp = *ppit;
            _preliminaryServers_list_set[i].erase(ppit);
            pp.battlePoint = battlePoint;
            PInfoSort::iterator it = pp.pInfoSet.begin();
            for(; it != pp.pInfoSet.end(); ++ it)
            {
                if((*it).playerId == pid)
                {
                    PlayerInfo pInfo = *it;
                    pInfo.battlePoint = battlePoint1;
                    pp.pInfoSet.erase(it);
                    pp.pInfoSet.insert(pInfo);
                    break;
                }
            }
            _preliminaryServers_list_set[i].insert(ppit);
            break;
        }
    }
    bool find = false;
    for(int i = 0; i < 2; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            if(_finals[i][j].serverId == sid)
            {
                _finals[i][j].battlePoint = battlePoint;
                PInfoSort::iterator it = _finals[i][j].pInfoSet.begin();
                for(; it != _finals[i][j].pInfoSet.end(); ++ it)
                {
                    if((*it).playerId == pid)
                    {
                        PlayerInfo pInfo = *it;
                        pInfo.battlePoint = battlePoint1;
                        _finals[i][j].pInfoSet.erase(it);
                        _finals[i][j].pInfoSet.insert(pInfo);
                        break;
                    }
                }
                find = true;
                break;
            }
        }
        if(find) break;
    }
}

void ServerWarMgr::updateSuport(UInt8 type, UInt8 flag, UInt16 pos)
{
    if(flag == 1)
    {
        if(type == 1)
        {
            ServerPreliminaryPlayerListIterator it = _preliminaryServers_list[0].begin();
            size_t pos2 = pos;
            if(pos2 > _preliminaryServers_list[0].size())
                pos2 = _preliminaryServers_list[0].size();
            std::advance(it, pos2);
            if(it != _preliminaryServers_list[0].end())
            {
                ServerPreliminaryPlayer& pp = *it;
                ++ pp.support;
            }
        }
        else
        {
            ServerPreliminaryPlayerListIterator it = _preliminaryServers_list[1].begin();
            size_t pos2 = pos;
            if(pos2 > _preliminaryServers_list[1].size())
                pos2 = _preliminaryServers_list[1].size();
            std::advance(it, pos2);
            if(it != _preliminaryServers_list[1].end())
            {
                ServerPreliminaryPlayer& pp = *it;
                ++ pp.support;
            }
        }
    }
    else if(flag == 2)
    {
        if(pos > 15)
            return;
        if(type == 1)
            ++ _finals[0][pos].support;
        else
            ++ _finals[1][pos].support;
    }
}

void ServerWarMgr::sendPreliminary(Player* player, UInt8 group, UInt8 flag, UInt16 start, UInt8 len)
{
    if(player == NULL || group < 1 || group > 2)
        return;
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;

	Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x03);
    st << flag;
    if(flag == 0)
    {
        ServerPreliminaryPlayerListMap::iterator spit = _preliminaryServers[group-1].find(cfg.serverNo);
        if(spit == _preliminaryServers[group-1].end())
            return;

        st << leftTime;
        ServerPreliminaryPlayer& spp = *(spit->second);
        spp.appendServerData(st);
        UInt8 cnt = static_cast<UInt8>(spp.battlesVec.size());
        st << cnt;
        for(UInt8 i = 0; i < cnt; ++ i)
        {
            st << spp.serverId << spp.serverName;
            spp.battlesVec[i].appendPreReport(st);
        }
    }
    else
    {
        st << group;
        std::map<Player *, PlayerBetData>::iterator bit =  _playerBet.find(player);
        if(bit == _playerBet.end())
        {
            st << static_cast<UInt8>(1);
        }
        else
        {
            PlayerBetData& pbd = bit->second;
            std::vector<BetInfo>& blist = pbd.betList[group-1][0];
            UInt8 cnt = blist.size();
            if(cnt > 1)
                cnt = 1;
            st << static_cast<UInt8>(1 - cnt);
        }
        UInt16 totalPreNum = _preliminaryServers_list_set[group-1].size();
        st << totalPreNum << start;
        size_t offset = st.size();
        UInt8 premNum = 0;
        st << premNum;
        ServerPreliminaryPlayersSet::iterator setIt = _preliminaryServers_list_set[group-1].begin();
        size_t pos = start;
        if(pos > _preliminaryServers_list_set[group-1].size())
            pos = _preliminaryServers_list_set[group-1].size();
        std::advance(setIt, pos);
        for(int i = 0; setIt != _preliminaryServers_list_set[group-1].end() && i < len; ++ setIt)
        {
            ++ premNum;
            ++ i;
            ServerPreliminaryPlayerListIterator pit = *setIt;
            ServerPreliminaryPlayer& spp = *pit;
            UInt8 fSupport = 0;
            std::map<Player *, UInt8>::iterator it = spp.betMap.find(player);
            if(it != spp.betMap.end())
                fSupport = it->second + 1;
            spp.appendServerData(st);
            st << fSupport;
        }
        st.data<UInt8>(offset) = static_cast<UInt8>(premNum);
    }

    st << Stream::eos;
    player->send(st);
}

void ServerWarMgr::sendElimination(Player * player, UInt8 type, UInt8 group)
{
	if(_progress <= e_war_16 || (_progress >= e_war_sign_end && _progress < e_war_16_end))
		return;
    if(group > 2 || group < 1)
        return;
	bool resultOnly = (group & 0x80) > 0;
    UInt8 r = _round;
    switch(type)
    {
    case 0x04:  //进8强
        break;
    case 0x05:  //进4强
        if(r < 1)
            return;
        break;
    case 0x06:  //进2强
        if(r < 2)
            return;
        break;
    case 0x07:  //进1强
        if(r < 3)
            return;
        break;
    default:
        return;
    }
	Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x01) << type << group;
    int gIdx = group - 1;
	std::map<Player *, PlayerBetData>::iterator iter = _playerBet.find(player);
	if(iter == _playerBet.end())
		st << static_cast<UInt8>(0);
	else
	{
        size_t offset = st.size();
        UInt8 cnt = 0;
        st << cnt;
        std::vector<BetInfo>& blist = iter->second.betList[gIdx][type-0x04+1];
        for(std::vector<BetInfo>::iterator it = blist.begin(); it != blist.end(); ++ it)
        {
            if(it->group != group)
                continue;
            ++ cnt;
            UInt8 pos = it->pos;
            UInt8 g_pos = (pos << 3) | it->round;
            st << g_pos << it->type;
        }
        st.data<UInt8>(offset) = cnt;
	}
	if(!resultOnly)
	{
        for(int i = 0; i < 16; ++ i)
        {
            _finals[gIdx][i].appendServerData(st);
        }
	}

    r = type-0x04 + 1;
    st << static_cast<UInt8>(r);
	for(int k = 0; k < aIndex[r]; ++ k)
	{
        std::vector<PairServerReport>& battles = _finalBattles[gIdx][k].battles;
        UInt8 cnt = battles.size();
        st << cnt;
        for(size_t i = 0; i < cnt; ++ i)
        {
            battles[i].appendServerReport(st);
        }
	}
	st << Stream::eos;
	player->send(st);
}

bool ServerWarMgr::hasLastLeaderBoard()
{
    std::map<UInt16, LeaderServer>::iterator it = _leaderBoard.find(_session-1);
    if(it != _leaderBoard.end())
        return true;
    return false;
}

void ServerWarMgr::updateLeaderBoard(BinaryReader& brd)
{
    UInt16 cnt = 0;
    brd >> cnt;
    for(int i = 0; i < cnt; ++ i)
    {
        LeaderServer lserver;
        UInt16 session = 0;
        UInt8 memCnt = 0;
        brd >> session >> lserver.serverName >> lserver.serverId;
        brd >> memCnt;
        for(UInt8 m = 0; m < memCnt; ++ m)
        {
            PlayerInfo pInfo; 
            pInfo.readInfo(brd);
            lserver.members.insert(pInfo);
        }
        _leaderBoard[session] = lserver;
    }
}

void ServerWarMgr::sendLeaderBoard(Player* pl)
{   //历届冠军战队
    if(pl == NULL) return;
	Stream st(REP::SERVER_ARENA_LB);
    st << static_cast<UInt8>(6) << static_cast<UInt16>(_leaderBoard.size());
    for(std::map<UInt16, LeaderServer>::iterator iter = _leaderBoard.begin(); iter != _leaderBoard.end(); ++ iter)
    {
        PInfoSort& members = iter->second.members;
        st << iter->first << iter->second.serverName << static_cast<UInt8>(members.size());
        for(PInfoSort::iterator it = members.begin(); it != members.end(); ++ it)
        {
            (*it).appendInfo(st);
        }
    }
    st << Stream::eos;

    pl->send(st);
}

void ServerWarMgr::sendLastLeaderBoard(Player* pl)
{   //上一届冠军战队
    if(pl == NULL) return;
	Stream st(REP::SERVER_ARENA_LB);
    st << static_cast<UInt8>(5);
    std::map<UInt16, LeaderServer>::iterator iter = _leaderBoard.find(_session-1);
    if(iter != _leaderBoard.end())
    {
        PInfoSort& members = iter->second.members;
        st << iter->second.serverName << static_cast<UInt8>(members.size());
        for(PInfoSort::iterator it = members.begin(); it != members.end(); ++ it)
        {
            (*it).appendInfo(st);
        }
        st << static_cast<UInt8>(pl->GetVar(VAR_TEAMARENA_WORSHIP));
    }
    st << Stream::eos;

    pl->send(st);
}

//SreverWar Boss
static const UInt32 ServerWarBossId[] = { 7317, 7318};
#define AWARD_AREA1 3
#define AWARD_AREA2 10
#define AWARD_MAN 20
ServerWarBoss::ServerWarBoss()
    : _prepareTime(0), _appearTime(0), _disappearTime(0), _prepareStep(0)
{
    _npcid = 0;
    _loc = 0;
    _count = 0;
    _disappeared = false;
    _final = false;
    _percent = 100;
}

bool ServerWarBoss::isServerWarBoss(UInt32 npcId)
{
    for (UInt8 i = 0; i < sizeof(ServerWarBossId)/sizeof(UInt32); ++i)
    {
        if(ServerWarBossId[i] == npcId)
            return true;
    }
    return false;

}

void ServerWarBoss::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);
    if (now >= _disappearTime)
    {
        disappear();
        return;
    }
    broadcastTV(now);
}

void ServerWarBoss::calcNext(UInt32 now)
{
    if (cfg.GMCheck)
    {
        _prepareTime = TimeUtil::SharpWeek(0, now) + 6*86400 + 19 * 60 * 60 + 15 * 60;
        _appearTime = _prepareTime + 15 * 60;
        _disappearTime = _appearTime + 30 * 60;
    }
    else
    {
        _prepareTime = now + 30;
        _appearTime = _prepareTime + 1 * 60;
        _disappearTime = _appearTime + 30 * 60;
    }
}

void ServerWarBoss::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime && !_prepareStep)
        _prepareStep = 1;

    switch (_prepareStep)
    {
        case 1:
            SYSMSG_BROADCASTV(547, 15);
            fprintf(stderr, "15 mins\n");
            _prepareStep = 2;
            break;

        case 2:
            if (now < _appearTime - 10 * 60)
                return;
            fprintf(stderr, "10 mins\n");
            SYSMSG_BROADCASTV(547, 10);
            _prepareStep = 3;
            break;

        case 3:
            if (now < _appearTime - 5 * 60)
                return;
            fprintf(stderr, "5 mins\n");
            SYSMSG_BROADCASTV(547, 5);
            _prepareStep = 4;
            break;

        case 4:
            if (now < _appearTime)
                return;
            _prepareStep = 5;
            appear(ServerWarBossId[0]);
            break;

        default:
            break;
    }
}

void ServerWarBoss::appear(UInt32 npcId)
{
    GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
    if(it == GData::npcGroups.end())
        return;
    if(!_loc)
    {
        std::vector<UInt16> spots;
        Map::GetAllSpot(spots);
        if (!spots.size()) return;

        UInt16 spot = spots[uRand(spots.size())];
        if (!spot) return;

        _loc = spot;
    }
    _npcid = npcId;

    _ng = it->second;
    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();
    if (!sz) return;


    Map * map = Map::FromSpot(_loc);
    if (!map) return;
    UInt8 thrId = mapCollection.getCountryFromSpot(_loc);

    MOData mo;
    mo.m_ID = npcId;
    mo.m_Hide = false;
    mo.m_Spot = _loc;
    mo.m_Type = 6;
    mo.m_ActionType = 0;

    if(CURRENT_THREAD_ID() != thrId)
    {
        GameMsgHdr hdr1(0x329, thrId, NULL, sizeof(mo));
        GLOBAL().PushMsg(hdr1, &mo);
    }
    else
    {
        map->AddObject(mo);
        map->Show(mo.m_ID, true, mo.m_Type);
    }

    printf("loc:%u, npcId:%u\n", _loc, _npcid);
    sendId();
    sendLoc();
    sendCount();
    sendBossInfo(1, NULL);
}

void ServerWarBoss::startBoss()
{
    appear(ServerWarBossId[1]);
    _hp.clear();
    _hp.resize(_ng->getList().size());

    SYSMSG_BROADCASTV(553, _npcid);

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    TRACE_LOG("-----------------------------------bossBaseHp:%d, baseAttack:%d\n", _hp[0], nflist[0].fighter->getBaseAttack());

    const Int32 WBOSS_MAX_ATK= 100000;
    const float WBOSS_BASE_TIME = 300.f;
    const float WBOSS_ATK_FACTOR = 0.5f;
    const Int32 WBOSS_MIN_HP = 20000000;
    const Int32 WBOSS_MAX_HP = 350000000;
    const float WBOSS_MAX_ASC_HP_FACTOR = 1.40f;
    const float WBOSS_MAX_ASC_ATK_FACTOR = 1.40f;

    Int32 extatk = 0;
    Int32 extmagatk = 0;
    Int32 atk = 0;
    Int32 matk = 0;
    Int32 ohp = 0;
    float lastTime = 0;

    atk = worldBoss.getLastAtk(0);
    matk = worldBoss.getLastMAtk(0);
    ohp = worldBoss.getLastHP(0);
    Int32 baseatk = nflist[0].fighter->getBaseAttack();
    Int32 basematk = nflist[0].fighter->getBaseMagAttack();
    lastTime = worldBoss.getLastTime(0);

    if (lastTime == 0)
        return;

    float hp_factor = (float)WBOSS_BASE_TIME / (float)lastTime;
    if(hp_factor > WBOSS_MAX_ASC_HP_FACTOR)
        hp_factor = WBOSS_MAX_ASC_HP_FACTOR;
    ohp = ohp * hp_factor;
    if (ohp < WBOSS_MIN_HP)
        ohp = WBOSS_MIN_HP;
    if (ohp > WBOSS_MAX_HP)
        ohp = WBOSS_MAX_HP;

    _hp[0] = ohp;
    nflist[0].fighter->setBaseHP(ohp);

    float atk_factor = (WBOSS_BASE_TIME/(float)lastTime - 1.f) * WBOSS_ATK_FACTOR;
    if(atk_factor > WBOSS_MAX_ASC_ATK_FACTOR)
        atk_factor = WBOSS_MAX_ASC_ATK_FACTOR;
    extatk = atk_factor * (atk + baseatk) + atk;
    if(extatk > WBOSS_MAX_ATK)
        extatk = WBOSS_MAX_ATK;
    else if(extatk < 0)
        extatk = 0;
    nflist[0].fighter->setExtraAttack(extatk);

    extmagatk = atk_factor * (matk + basematk) + matk;
    if(extmagatk > WBOSS_MAX_ATK)
        extmagatk = WBOSS_MAX_ATK;
    else if(extmagatk < 0)
        extmagatk = 0;
    nflist[0].fighter->setExtraMagAttack(extmagatk);

    nflist[0].fighter->setDirty();
    TRACE_LOG("-----------------------------------bossExtraHp:%d, extraAttack:%d\n", _hp[0], nflist[0].fighter->getExtraAttack());
}

bool ServerWarBoss::attack(Player* pl, UInt16 loc, UInt32 npcId)
{
    if (pl->getLocation() != loc || loc != _loc)
        return false;

    if (pl->getThreadId() !=  mapCollection.getCountryFromSpot(loc))
        return false;

    if (_npcid != npcId || !_ng || _disappeared || !_percent)
    {
        SYSMSG_BROADCASTV(551);
        return false;
    }

    if (pl->GetPackage()->GetRestPackageSize() <= 0)
    {
        pl->sendMsgCode(0, 1011);
        return false;
    }

    bool res = attackBoss(pl, npcId, 1, _final);
    if (res && !_final)
    {
        ++_count;
        if (_count <= 10)
        {
            SYSMSG_BROADCASTV(552, pl->getCountry(), pl->getName().c_str(), loc, _count, npcId);

            deleteBoss();
            if(_count == 10)
            {
                _final = true;
                startBoss();

                sendHpMax();
                sendHp();
            }
            else
                appear(npcId);
        }
    }
    else if (res && _final)
    {
        disappear();
    }
    return res;
}

void ServerWarBoss::deleteBoss()
{
    Map * map = Map::FromSpot(_loc);
    if (!map) return;
    UInt8 thrId = mapCollection.getCountryFromSpot(_loc);
    
    if(CURRENT_THREAD_ID() != thrId)
    {
        struct MapNpc
        {
            UInt16 loc;
            UInt32 npcId;
        };

        MapNpc mapNpc = {_loc, _npcid};
        GameMsgHdr hdr(0x328, thrId, NULL, sizeof(MapNpc));
        GLOBAL().PushMsg(hdr, &mapNpc);
    }
    else
    {
        map->Hide(_npcid);
        map->DelObject(_npcid);
    }
}

void ServerWarBoss::disappear()
{
    if (_disappeared)
        return;
    deleteBoss();
    sendBossInfo(2, NULL);
    TRACE_LOG("disappear: %u, loc: %u", _npcid, _loc);

    _count = 0;
    _disappeared = true;
    _final = false;
    _percent = 100;
    _npcid = 0;
    _loc = 0;
    _ng = NULL;
    _hp.clear();
    if(!cfg.GMCheck)
    {
        _prepareStep = 0;
        _prepareTime = 0;
        _disappeared = false;
    }

    UInt8 type = 0;
    GameMsgHdr hdr(0x1EB, WORKER_THREAD_WORLD, NULL, sizeof(type));
    GLOBAL().PushMsg(hdr, &type);
    sendLoc();
    sendHp();
}


bool ServerWarBoss::attackBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final)
{
    static UInt32 sendflag = 7;
    ++sendflag;

    if (!pl) return false;
    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return false;
    }
    pl->checkLastBattled();

    GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
    if(it == GData::npcGroups.end())
        return false;

    Battle::BattleSimulator bsim(Battle::BS_WBOSS, pl, _ng->getName(), _ng->getLevel(), false);
    pl->PutFighters(bsim, 0);
    _ng->putFighters(bsim);

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();

    if (final)
    {
        for(size_t i = 0; i < sz; ++ i)
        {
            if(_hp[i] == 0xFFFFFFFF)
                continue;
            GData::NpcFData& nfdata = nflist[i];
            Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
            bf->setHP(_hp[i]);
        }
    }

    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;
 
    if (sz && final)
    {
        UInt32 oldHP = _hp[0];
        for(size_t i = 0; i < sz; ++ i)
        {
            GData::NpcFData& nfdata = nflist[i];
            Battle::BattleObject * obj = bsim(1, nfdata.pos);
            if(obj == NULL || !obj->isChar())
                continue;
            Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
            UInt32 nHP = bfgt->getHP();
            if(nHP == 0)
                nHP = 0xFFFFFFFF;
            if(_hp[i] != 0xFFFFFFFF && _hp[i] != nHP)
                _hp[i] = nHP;
        }

        if(oldHP != 0xFFFFFFFF)
        {
            if(oldHP == 0)
                oldHP = nflist[0].fighter->getMaxHP();
            UInt32 newHP = (_hp[0] == 0xFFFFFFFF) ? 0 : _hp[0];
            if(oldHP > newHP)
            {
                UInt32 damage = oldHP - newHP;
                UInt32 exp = ((float)damage / nflist[0].fighter->getMaxHP()) * _ng->getExp() * expfactor;
                if (exp < 1000)
                    exp = 1000;
                pl->pendExp(exp);
                if (!(sendflag % 8))
                    sendDmg(damage);

                AttackInfo info(pl, damage);
                AtkInfoType::iterator i = _atkinfo.begin();
                while (i != _atkinfo.end())
                {
                    if ((*i).player == pl)
                    {
                        info += *i;
                        _atkinfo.erase(i);
                        break;
                    }
                    ++i;
                }
                bool ret = _atkinfo.insert(info).second;
                TRACE_LOG("WBOSS INSERT ret: %u (pid: %" I64_FMT "u, dmg: %u)", ret, pl->getId(), damage);

                UInt8 newPercent = ((float)newHP / nflist[0].fighter->getMaxHP()) * 100;

                if (newPercent > 100)
                    newPercent = 100;
                if (_percent < newPercent)
                    _percent = newPercent;
                if (!newPercent)
                {
                    SYSMSG_BROADCASTV(550, nflist[0].fighter->getId());
                    _percent = 0;
                    _hp[0] = 0;
                    reward(pl);
                    res = true;
                    if (sendflag % 8)
                        sendHp();

                    nflist[0].fighter->setExtraAttack(0);
                    nflist[0].fighter->setExtraMagAttack(0);
                    nflist[0].fighter->setWBoss(false);
                }
                else if (newPercent <= 5 && _percent - newPercent >= 5)
                {
                    SYSMSG_BROADCASTV(548, pl->getCountry(), pl->getName().c_str(), nflist[0].fighter->getId(), newPercent);
                    _percent = newPercent;
                }
                else if (_percent - newPercent >= 10)
                {
                    SYSMSG_BROADCASTV(548, pl->getCountry(), pl->getName().c_str(), nflist[0].fighter->getId(), newPercent);
                    _percent = newPercent;
                }
                if (!(sendflag % 8))
                    sendHp();
            }
            else
            {
                TRACE_LOG("WBOSS OLDHP(%u) < NEWHP(%u)", oldHP, newHP);
            }
        }
    }

    if(res)
    {
        ret = 0x0101;
        if (!final)
        {
            pl->_lastNg = _ng;
            pl->pendExp(_ng->getExp()*expfactor);
            _ng->getLoots(pl, pl->_lastLoot, 1, NULL);
        }
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(pl, lastExp) << static_cast<UInt8>(0);
    UInt8 size = pl->_lastLoot.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
    {
        st << pl->_lastLoot[i].id << pl->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    pl->send(st);
    bsim.applyFighterHP(0, pl);

    pl->setBuffData(PLAYER_BUFF_ATTACKING, now + 30);
    return res;
}

void ServerWarBoss::reward(Player* player)
{
    static UInt16 trumpFrag = 6195;
    static UInt16 gems[] = {5003,5013,5023,5033,5043,5053,5063,5073,5083,5093,5103,5113,5123,5133,5143,};

    size_t sz = _atkinfo.size();
    if (!sz) return;

    UInt32 lucky1 = uRand(sz);
    UInt32 lucky2 = uRand(sz);
    while (lucky1 == lucky2 && sz > 1)
        lucky2 = uRand(sz);

    std::set<UInt32> comp;
    std::set<UInt32> breath;
    std::set<UInt32> gem;

    UInt32 luckynum = (float)10 * sz / 100;
    getRandList(sz, luckynum, gem); // 宝石

    luckynum = (float)5 * sz / 100;
    getRandList(sz, luckynum, breath); // 凝神丹
    getRandList(sz, luckynum, comp); // 补髓丹

    bool notify4_10 = false;
    bool notify11_20 = false;
    UInt32 j = 0;
    for (AtkInfoType::reverse_iterator i = _atkinfo.rbegin(); i != _atkinfo.rend(); ++i, ++j)
    {
        if (j < AWARD_AREA1)
        {
            MailPackage::MailItem item1[] = {{trumpFrag,4},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            SYSMSG_BROADCASTV(557, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), trumpFrag, 4);
        }

        if (j >= AWARD_AREA1 && j < AWARD_AREA2)
        {
            MailPackage::MailItem item1[] = {{trumpFrag,2},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            if (!notify4_10)
            {
                SYSMSG_BROADCASTV(558, trumpFrag, 2);
                notify4_10 = true;
            }
        }

        if (j >= AWARD_AREA2 && j < AWARD_MAN)
        {
            MailPackage::MailItem item1[] = {{trumpFrag,1},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            if (!notify11_20)
            {
                SYSMSG_BROADCASTV(571, trumpFrag, 1);
                notify11_20 = true;
            }
        }

        if (j == lucky1 || j == lucky2) // 法宝碎片
        {
            MailPackage::MailItem item[] = {{trumpFrag,1},};
            (*i).player->sendMailItem(562, 563, item, 1, false);
            SYSMSG_BROADCASTV(560, (*i).player->getCountry(), (*i).player->getName().c_str(), trumpFrag, 1);
        }

        if (j >= AWARD_MAN)
        {
            if (comp.find(j) != comp.end())
            {
                MailPackage::MailItem item[] = {{506,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }

            if (breath.find(j) != breath.end())
            {
                MailPackage::MailItem item[] = {{508,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }

            if (gem.find(j) != gem.end())
            {
                UInt8 idx = uRand(sizeof(gems)/sizeof(UInt16));
                MailPackage::MailItem item[] = {{gems[idx],1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }
        }

        {
            /*
            MailPackage::MailItem item[] = {{133,1},{MailPackage::Tael,1000}};
            (*i).player->sendMailItem(568, 569, item, 2);
            */
            UInt8 size = 2 + ((*i).player->GetLev() >= 60 ? 1 : 0);
            MailPackage::MailItem * item = new MailPackage::MailItem [size];
            item[0].id = 133;
            item[0].count = 1;
            item[1].id = MailPackage::Tael;
            item[1].count = 1000;
            if((*i).player->GetLev() >= 60)
            {
                item[2].id = 9390;
                item[2].count = 1;
            }
            (*i).player->sendMailItem(568, 569, item, size);
        }
    }

    if (player)
    {
        MailPackage::MailItem item[] = {{56,5},{MailPackage::Tael,20000},};
        player->sendMailItem(566, 567, item, 2);
        SYSMSG_BROADCASTV(559, player->getCountry(), player->getName().c_str(), 56, 5, 20000);
    }
    _atkinfo.clear();
}

void ServerWarBoss::getRandList(UInt32 sz, UInt32 num, std::set<UInt32>& ret)
{
    if (sz > AWARD_MAN)
    {
        if (sz - AWARD_MAN <= num)
        {
            for (UInt32 j = 0; j < sz - AWARD_MAN; ++j)
                ret.insert(j+AWARD_MAN);
            return;
        }
    }
}

void ServerWarBoss::sendHpMax(Player* player)
{
    if (!_ng)
        return;
    if (!_hp.size())
        return;

    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(0);

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();
    if (!sz) return;

    st << nflist[0].fighter->getMaxHP();
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void ServerWarBoss::sendHp(Player* player)
{
    if (!_hp.size())
        return;

    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(1);
    st << _hp[0];
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void ServerWarBoss::sendDmg(UInt32 damage)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(2);
    st << damage;
    st << Stream::eos;
    NETWORK()->Broadcast(st);
}

void ServerWarBoss::sendCount(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(3);
    st << static_cast<UInt32>(_count);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void ServerWarBoss::sendId(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(4);
    st << static_cast<UInt32>(_npcid);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void ServerWarBoss::sendLoc(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(5);
    st << static_cast<UInt32>(_loc);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void ServerWarBoss::sendDaily(Player* player)
{
    if (!player)
        return;
    if (_npcid && !_disappeared)
    {
        sendBossInfo(1, player);
        sendId(player);
        sendLoc(player);
        sendHpMax(player);
        sendHp(player);
        sendCount(player);
    }
}

void ServerWarBoss::sendBossInfo(UInt8 state, Player * player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(0) << state;
    st << static_cast<UInt16>(_npcid);
    if(_ng)
    {
        std::vector<GData::NpcFData>& nflist = _ng->getList();
        st << nflist[0].fighter->getName();
    }
    else
        st << "";
    st << Stream::eos;

    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

}
