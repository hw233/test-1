﻿#include "Config.h"
#include "Player.h"
#include "Country.h"
#include "Athletics.h"
#include "AthleticsRank.h"
#include "Server/OidGenerator.h"
#include "Script/GameActionLua.h"
#include "Battle/BattleSimulator.h"
#include "Server/SysMsg.h"
#include "Mail.h"
#include "Package.h"
#include "MsgID.h"
#include "Script/BattleFormula.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"

namespace GObject
{

Athletics::Athletics(Player * player) : _owner(player), _hasEnterAthletics(false)
{
    memset(&PlayerPInfo, 0, sizeof(PlayerPInfo));
}

Athletics::~Athletics()
{
	for (std::size_t i = 0; i < _athleticses.size(); ++i)
	{
		SAFE_DELETE(_athleticses[i]);
	}
	_athleticses.clear();
}

void Athletics::setEnterAthletics(bool enter)
{
	_hasEnterAthletics = enter;
}

bool Athletics::hasEnterAthletics()
{
	return _hasEnterAthletics;
}

void Athletics::enterAthleticsNotify(UInt8 row)
{
	if (row == 0)
	{
		SYSMSG(title, 318);
		SYSMSGV(content, 320);
		//_owner->GetMailBox()->newMail(NULL, 0x01, title, content);
	}
	else if (row == 1)
	{
		SYSMSG(title, 318);
		SYSMSGV(content, 321);
		//_owner->GetMailBox()->newMail(NULL, 0x01, title, content);
	}
}

bool Athletics::addAthleticsDataFromDB(UInt32 id, UInt8 side, Player * target, UInt8 winSide, UInt32 repid, UInt32 time)
{
	if (_athleticses.size() >= 15)
		return false;
	AthleticsData * data = new AthleticsData(id, side, target, winSide, repid, time);
	_athleticses.push_front(data);
	return true;
}

UInt32 Athletics::addAthleticsData(UInt8 side, Player * target, UInt8 win, UInt32 repid, UInt32 now)
{
	AthleticsData * data = new(std::nothrow) AthleticsData();
	if (data == NULL)
		return -1;
	data->id = IDGenerator::gAthleticsRecordOidGenerator.ID();
	data->side = side;
	data->win = win;
	data->target = target;
	data->reptid = repid;
	data->time = now;
	//data->awardType = 0xFF;
	//data->awardCount = 0;
	if (_athleticses.size() >= 15)
	{
		AthleticsData * del = _athleticses.front();
		UInt32 id = del->id;
		GameMsgHdr hdr(0x214, del->target->getThreadId(), del->target, sizeof(UInt32));
		GLOBAL().PushMsg(hdr, &id);
		SAFE_DELETE(del);
		_athleticses.pop_front();
	}
	_athleticses.push_back(data);
	DB6().PushUpdateData("INSERT INTO `athletics_record` (`id`, `atker`, `defer`, `repid`, `time`, `winSide`) VALUES(%u, %" I64_FMT "u, %" I64_FMT "u, %u, %u, %u)", data->id, (side == 0 ? _owner->getId() : target->getId()), (side == 0 ? target->getId() : _owner->getId()), data->reptid, data->time, win);
	return data->id;
}

void Athletics::addAthleticsDataFromTarget(UInt8 side, Player * target, UInt32 id, UInt8 win, UInt32 repid, UInt32 now)
{
	AthleticsData * data = new(std::nothrow) AthleticsData(id, side, target, win, repid, now);
	if (data == NULL)
		return ;
	if (_athleticses.size() >= 15)
	{
		AthleticsData * del = _athleticses.front();
		UInt32 id = del->id;
		GameMsgHdr hdr(0x214, del->target->getThreadId(), del->target, sizeof(UInt32));
		GLOBAL().PushMsg(hdr, &id);
		SAFE_DELETE(del);
		_athleticses.pop_front();
	}
	_athleticses.push_back(data);
}

void Athletics::notifyAthleticsData(UInt16 count)
{
    return;
#if 0
	if (count > static_cast<UInt16>(_athleticses.size()))
		count = static_cast<UInt16>(_athleticses.size());

	std::deque<AthleticsData *>::reverse_iterator rit = _athleticses.rbegin();
	Stream st(REP::FIGHT_INFO);
	st << count;
	for (UInt16 i = 0; rit != _athleticses.rend() && i < count; ++rit, ++count)
	{
		st << (*rit)->side <<  (*rit)->win << (*rit)->awardType << (*rit)->awardCount << (*rit)->target->getName() << (*rit)->target->getCountry() << (*rit)->time << (*rit)->reptid;
	}
	st << Stream::eos;
	_owner->send(st);
#endif
}

void Athletics::notifyAthleticsData2(AthleticsData * data)
{
    return;
#if 0
	Stream st(REP::FIGHT_INFO);
	st << static_cast<UInt16>(1);
	st << data->side << data->win << data->awardType << data->awardCount << data->target->getName() << data->target->getCountry() << data->time << data->reptid;
	st << Stream::eos;
	_owner->send(st);
#endif
}

void Athletics::listAthleticsReport()
{
    UInt8 count = 15;
	if (15 > _athleticses.size())
		count = static_cast<UInt8>(_athleticses.size());

	Stream st(REP::ARENA_IFNO);
    st << static_cast<UInt16>(0x08);

	std::deque<AthleticsData *>::reverse_iterator rit = _athleticses.rbegin();
	st << count;
	for (UInt16 i = 0; rit != _athleticses.rend() && i < count; ++rit, ++i)
	{
		st << (*rit)->target->getName() << (*rit)->target->getCountry() << static_cast<UInt8>((*rit)->win != (*rit)->side) << (*rit)->reptid;
	}

    st << Stream::eos;

    _owner->send(st);
}

void Athletics::notifyAthleticsData2(UInt32 id)
{
	std::deque<AthleticsData *>::reverse_iterator found = _athleticses.rbegin();
	for (; found != _athleticses.rend(); ++ found)
	{
		if ((*found)->id == id)
			break;
	}
	if (found != _athleticses.rend())
	{
		notifyAthleticsData2(*found);
	}
}

void Athletics::notifyAthleticsDeferData(UInt8 side, Player * target, UInt32 id, UInt8 win, UInt32 rptid, UInt32 time)
{
	struct AthleticsDeferData
	{
		UInt32 id;
		UInt8 win;
		UInt8 side;
		Player * target;
		UInt32 rptid;
		UInt32 time;
	};
    side = 1 - side;
	AthleticsDeferData data = { id, win, side, _owner, rptid, time };
	GameMsgHdr hdr(0x213, target->getThreadId(), target, sizeof(AthleticsDeferData));
	GLOBAL().PushMsg(hdr, &data);
}

void Athletics::notifyDropAthleticsData(UInt32 id)
{
	for (std::size_t i = 0; i < _athleticses.size(); ++i)
	{
		if (_athleticses[i]->id == id)
			return ;
	}
	DB6().PushUpdateData("DELETE FROM `athletics_record` WHERE `id` = %u", id);
}

void Athletics::adjustAthlDeferBuffData(Player *atker, Player *defer, bool reset)
{
    if(!atker || !defer)
        return;

    UInt8 index;
    UInt8 rivalType;
    UInt32 bufFlag = Battle::BattleFighter::AthlEnh31;
    for(index = 0; index < 5; index++)
    {
        if(PlayerPInfo.eRival[index] == defer->getId())
            break;
    }
    if(index >= 5)
        return;
    rivalType = PlayerPInfo.eRivalType[index];
    if(rivalType == 0 || rivalType > 7)
        return;
    if(reset)
    {
        defer->setAthlRivalBuff(0);
    }
    else
    {
        bufFlag = bufFlag << (rivalType - 1);
        defer->setAthlRivalBuff(bufFlag);
    }
}

void Athletics::attack(Player * defer)
{
    if(_owner->checkClientIP())
        _owner->SetVar(VAR_DROP_OUT_ITEM_MARK, 0);
	UInt8 tid = defer->getThreadId();
	if(tid == _owner->getThreadId())
	{
		struct AthleticsResult
		{
			UInt32	id;
			UInt8	side;
			Player * defender;
			bool	result;
		};
		bool res;
		UInt32 reptid, time;
		time = TimeUtil::Now();
        _owner->addFlag(GObject::Player::AthleticsBuff);
		Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, _owner, defer);
		_owner->PutFighters( bsim, 0, true );
		defer->PutFighters( bsim, 1, true );
		bsim.start();
		res = bsim.getWinner() == 1;
        _owner->delFlag(GObject::Player::AthleticsBuff);
		reptid = bsim.getId();
        if (res)
           GameAction()->RunOperationTaskAction0(_owner, 4);
		Stream st(REP::ATTACK_NPC);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId()<<static_cast<UInt64>(0) << Stream::eos;
		_owner->send(st);
		UInt32 id = addAthleticsData(0, defer, res ? 0 : 1, reptid, time);
		notifyAthleticsDeferData(0, defer, id, res ? 0 : 1, reptid, time);
		AthleticsResult ar = { id, 0, defer, res };
		GameMsgHdr hdr(0x19D, WORKER_THREAD_WORLD, _owner, sizeof(AthleticsResult));
		GLOBAL().PushMsg(hdr, &ar);

        //in same thread
        Player* winner =  res ? _owner : defer;

        GameAction()->doAttainment(winner,  Script::ATHLETICS_WIN , 1);
        if (res)
        {
            _owner->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 0);
            _owner->OnShuoShuo(SS_ATHWIN);
            _owner->setContinuousRFAward(1);
        }
		return;
	}
	struct AthleticsBeData
	{
		Player * attacker;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	AthleticsBeData abd = { _owner, _owner->getFormation(), static_cast<UInt16>(_owner->getMainFighter() != NULL ? _owner->getMainFighter()->getId() : 0) };
	for(int i = 0; i < 5; ++ i)
		abd.lineup[i] = _owner->getLineup(i);
	GameMsgHdr hdr(0x215, tid, defer, sizeof(AthleticsBeData));
	GLOBAL().PushMsg(hdr, &abd);
}

void Athletics::beAttack(Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup)
{
    atker->addFlag(GObject::Player::AthleticsBuff);
	Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, atker, _owner);
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
	_owner->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;
    atker->delFlag(GObject::Player::AthleticsBuff);

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId()<<static_cast<UInt64>(0) << Stream::eos;
	atker->send(st);

	struct AthleticsResult
	{
		UInt32 id;
		UInt8 side;
		Player * atker;
		bool result;
	};
	UInt32 time = TimeUtil::Now();
	UInt32 id = addAthleticsData(1, atker, res ? 0 : 1, bsim.getId(), time);
	notifyAthleticsDeferData(1, atker, id, res ? 0 : 1, bsim.getId(), time);
	AthleticsResult ar = {id, 1, atker, res };

	GameMsgHdr hdr(0x19D, WORKER_THREAD_WORLD, _owner, sizeof(AthleticsResult));
	GLOBAL().PushMsg(hdr, &ar);

	struct AthleticsResNotify
	{
		Player * peer;
		bool win;
	};
	AthleticsResNotify notify = { _owner, res };
	GameMsgHdr hdr2(0x220, atker->getThreadId(), atker, sizeof(AthleticsResNotify));
	GLOBAL().PushMsg(hdr2, &notify);

    if( !res)
    {
        //owner win
        GameAction()->doAttainment(_owner,  Script::ATHLETICS_WIN , 1);
    }
    else
    {
        //in other thread
        stAttainMsg  msg;
        msg.attainID = Script:: ATHLETICS_WIN ;
        msg.param = 1;
        GameMsgHdr h(0x244,   atker->getThreadId(), atker, sizeof(msg));
        GLOBAL().PushMsg(h, & msg);
        atker->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 0);
        atker->OnShuoShuo(SS_ATHWIN);
        atker->setContinuousRFAward(1);
    }

}

void Athletics::notifyAttackRes(Player *, bool win)
{
	if (win)
	{
		GameAction()->RunOperationTaskAction0(_owner, 4);
	}
}

void Athletics::updateAthleticsAwardData(UInt32 id, UInt8 type, UInt32 value)
{
    return;
#if 0
	std::deque<AthleticsData *>::reverse_iterator found = _athleticses.rbegin();
	for (; found != _athleticses.rend(); ++ found)
	{
		if ((*found)->id == id)
			break;
	}
	if (found != _athleticses.rend())
	{
		(*found)->awardType = type;
		(*found)->awardCount = value;
		notifyAthleticsData2(*found);
	}
#endif
}

void Athletics::defendergainsource(Player * atker, UInt32 id, UInt8 type, UInt32 award)
{
    return;
#if 0
	const static UInt32 Index1[]  = {315, 339, 316, 317, 356};
	const static UInt32 Index2[]  = {340, 341, 342, 357};

	if (type == 0xFF)
		return notifyAthleticsData2(id);


	SYSMSG(title, 318);
	//SYSMSG(szType, Index1[type]);
	//SYSMSGV(boxname, Index2[color], szType);
	//SYSMSGV(content, 319, atker->getName().c_str(), atker->getName().c_str(), boxname, award, szType);
	//_owner->GetMailBox()->newMail(NULL, 0x01, title, content);
	//AddUserSource(_owner, type, award);
	updateAthleticsAwardData(id, type, award);
#endif
}

void Athletics::attackergainsource(UInt32 id, UInt8 type, UInt32 award)
{
    return;
#if 0
	if (type == 0xFF)
		return notifyAthleticsData2(id);

	AddUserSource(_owner, type, award, true);
	updateAthleticsAwardData(id, type, award);
#endif
}

void Athletics::GetBoxAddSourceNotify(UInt8 type, UInt32 count)
{
    return;
	AddUserSource(_owner, type, count);
}

void Athletics::AddUserSource(Player *player, UInt8 type, UInt32 count, bool delay)
{
    return;

	switch(type)
	{
		case 0:
			if(delay)
				_owner->pendExp(count);
			else
				_owner->AddExp(count);
			break;
		case 1:
			if(delay)
				_owner->pendCoin(count);
			else
				_owner->getCoin(count);
			break;
		case 2:
			if(delay)
				_owner->pendTael(count);
			else
				_owner->getTael(count);
			break;
		case 3:
			if(delay)
				_owner->pendCoupon(count);
			else
				_owner->getCoupon(count);
			break;
		case 4:
			if(delay)
				_owner->pendAchievement(count);
			else
				_owner->getAchievement(count);
			break;
		default:
			break;
	}
}


bool Athletics::addAthleticsExtraAward(UInt32 EquipId, UInt8 rank)
{
    return false;

	if(EquipId == 0 || rank < 1 || rank > 10)
		return false;//error

	UInt16 grid = 1 + _owner->GetPackage()->GetItemUsedGrids(8913, 1, true);
	if(rank == 1)
		grid += _owner->GetPackage()->GetItemUsedGrids(8999, 3, true) + _owner->GetPackage()->GetItemUsedGrids(9000, 1, true);
	else if(rank == 2)
		grid += _owner->GetPackage()->GetItemUsedGrids(8999, 2, true);
	else if(rank == 3)
		grid += _owner->GetPackage()->GetItemUsedGrids(8999, 1, true);

	if (grid > _owner->GetPackage()->GetRestPackageSize())
	{
		_owner->sendMsgCode(0, 1011);
		return false;
	}
	_owner->GetPackage()->AddEquip(EquipId, true, false, FromAthletAward);
	_owner->GetPackage()->AddItem(8913, 1, true, false, FromAthletAward);

	if(rank == 1)
	{
		_owner->GetPackage()->AddItem(8999, 3, true, false, FromAthletAward);
		_owner->GetPackage()->AddItem(9000, 1, true, false, FromAthletAward);
	}
	else if(rank == 2)
		_owner->GetPackage()->AddItem(8999, 2, true, false, FromAthletAward);
	else  if(rank == 3)
		_owner->GetPackage()->AddItem(8999, 1, true, false, FromAthletAward);

	return true;
}
#if 0
void Athletics::updateMartial(const MartialData* md)
{
    if(!md)
        return;


    if(md->idx > 4)
        return;
    gAthleticsRank.setOrginal_martial(_owner, md->idx, md->defer);
    gAthleticsRank.setOrginal_canAttack(_owner, md->idx, md->canAttack);
    //listAthleticsMartial();
}
#endif
void Athletics::attackMartial(Player* defer)
{
    struct AthleticsResult
    {
        UInt32 id;
        UInt8 side;
        Player * defer;
        UInt8 result;
    };

    UInt8 res = 2;
    UInt32 id = 0;

    do
    {
        UInt8 idx = 0xFF;
        for(UInt8 i = 0; i < 5; ++i)
        {
            if(globalPlayers[PlayerPInfo.eRival[i]] == defer)
            {
                idx = i;
                break;
            }
        }

        if(idx > 4)
            break;

        if(0 == PlayerPInfo.eCanAttack[idx])
            break;

        if(_owner->checkClientIP())
            _owner->SetVar(VAR_DROP_OUT_ITEM_MARK, 0);
        UInt8 tid = defer->getThreadId();
        if(_owner->getThreadId() != tid)
        {
            struct AthleticsBeData
            {
                Player * attacker;
                UInt16 formation;
                UInt16 portrait;
                Lineup lineup[5];
            };
            AthleticsBeData abd = { _owner, _owner->getFormation(), static_cast<UInt16>(_owner->getMainFighter() != NULL ? _owner->getMainFighter()->getId() : 0) };
            for(int i = 0; i < 5; ++ i)
                abd.lineup[i] = _owner->getLineup(i);
            GameMsgHdr hdr(0x234, tid, defer, sizeof(AthleticsBeData));
            GLOBAL().PushMsg(hdr, &abd);

            return;
        }

        UInt32 reptid;
        _owner->addFlag(GObject::Player::AthleticsBuff);
        adjustAthlDeferBuffData(_owner, defer, false);
        Battle::BattleSimulator bsim = Battle::BattleSimulator(Battle::BS_ATHLETICS1, _owner, defer);
        defer->PutFighters( bsim, 1, true );
        _owner->PutFighters( bsim, 0, true );
        bsim.start();
        res = bsim.getWinner() == 1;
        adjustAthlDeferBuffData(_owner, defer, true);
        _owner->delFlag(GObject::Player::AthleticsBuff);
        reptid = bsim.getId();

        Stream st(REP::ATTACK_NPC);
        st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId()<<static_cast<UInt64>(0) << Stream::eos;
        _owner->send(st);

        awardMartial(defer, res);

        //UInt32 time = TimeUtil::Now();
		id = 0;//addAthleticsData(0, defer, res ? 0 : 1, reptid, time);
		//notifyAthleticsDeferData(0, defer, id, res ? 0 : 1, reptid, time);

        Player* winner =  res ? _owner : defer;

        GameAction()->doAttainment(winner,  Script::ATHLETICS_WIN , 1);
        if (res)
        {
            _owner->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 1);
            _owner->OnShuoShuo(SS_ATHWIN);
            _owner->setContinuousRFAward(1);
        }
    }while(false);

	AthleticsResult ar = {id, 0, defer, res };

	GameMsgHdr hdr(0x1F2, WORKER_THREAD_WORLD, _owner, sizeof(AthleticsResult));
	GLOBAL().PushMsg(hdr, &ar);
}

void Athletics::beAttackMartial(Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup)
{
    atker->addFlag(GObject::Player::AthleticsBuff);
    adjustAthlDeferBuffData(atker, _owner, false);
	Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, atker, _owner);
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
	_owner->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;
    adjustAthlDeferBuffData(atker, _owner, true);
    atker->delFlag(GObject::Player::AthleticsBuff);

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId()<<static_cast<UInt64>(0) << Stream::eos;
	atker->send(st);

	struct AthleticsResult
	{
		UInt32 id;
		UInt8 side;
		Player * atker;
		bool result;
	};
	//UInt32 time = TimeUtil::Now();
	UInt32 id = 0;//addAthleticsData(1, atker, res ? 0 : 1, bsim.getId(), time);
	//notifyAthleticsDeferData(1, atker, id, res ? 0 : 1, bsim.getId(), time);
	AthleticsResult ar = {id, 1, atker, res };

	GameMsgHdr hdr(0x1F2, WORKER_THREAD_WORLD, _owner, sizeof(AthleticsResult));
	GLOBAL().PushMsg(hdr, &ar);

	struct AthleticsResNotify
	{
		Player * peer;
		bool win;
	};
	AthleticsResNotify notify = { _owner, res };
	GameMsgHdr hdr2(0x235, atker->getThreadId(), atker, sizeof(AthleticsResNotify));
	GLOBAL().PushMsg(hdr2, &notify);

    if( !res)
    {
        //owner win
        GameAction()->doAttainment(_owner,  Script::ATHLETICS_WIN , 1);
    }
    else
    {
        //in other thread
        stAttainMsg  msg;
        msg.attainID = Script:: ATHLETICS_WIN ;
        msg.param = 1;
        GameMsgHdr h(0x244,   atker->getThreadId(), atker, sizeof(msg));
        GLOBAL().PushMsg(h, & msg);
        atker->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 1);
        atker->OnShuoShuo(SS_ATHWIN);
        atker->setContinuousRFAward(1);
    }
}

void Athletics::awardMartial(Player* defer, bool win)
{
    GObject::LastAthAward la = {0};
    if(win)
    {
        UInt8 i;
        for(i = 0; i < 5; ++i)
        {
            if(globalPlayers[PlayerPInfo.eRival[i]] == defer)
            {
                if(PlayerPInfo.eCanAttack[i] != 0)
                {
                    PlayerPInfo.eCanAttack[i] = 0;
                    char column[32];
                    sprintf(column, "eCanAttack%u", i + 1);
                    DB6().PushUpdateData("UPDATE `athletics_rank` SET `%s` = %u WHERE `ranker` = %" I64_FMT "u", column, PlayerPInfo.eCanAttack[i], _owner->getId());
                }
                break;
            }
        }
        if(i >= 5)
            return;
        UInt8 wins = _owner->getBuffData(PLAYER_BUFF_AMARTIAL_WIN) + 1;
        /*
        if(wins >= 5)
        {
            _owner->GetPackage()->AddItem(6, 1, 1, true, FromAthletAward);
            la.itemId = 6;
            la.itemCount = 1;
            wins = 0;

            _owner->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 2);
        }
        */

        UInt8 index;
        UInt8 category;
        UInt32 bufFlag;
        UInt32 bufFlag2;
        index = PlayerPInfo.eSelectIndex;
        if(index== 0 || index > 5)
            index = 1;
        if(PlayerPInfo.eRivalType[i] != 0)
        {
            category = (PlayerPInfo.eCombine[index - 1] >> 16) & 0xFF;
            if(category > 8)
                category = 0;
            bufFlag = 1 << category;
            _owner->adjustAthlBuffData(bufFlag);

            bufFlag2 = 1 << (16 + PlayerPInfo.eRivalType[i] - 1);
            _owner->adjustAthlBuffData2(bufFlag2);
        }

        //la.prestige = 10 * (World::_wday == 3 ? 2 : 1);
        //_owner->getPrestige(la.prestige, false);
        _owner->setBuffData(PLAYER_BUFF_AMARTIAL_WIN, wins, true);

        listAthleticsMartial();
    }
    else
    {
        //la.prestige = 5 * (World::_wday == 3 ? 2 : 1);
        //_owner->getPrestige(la.prestige, false);
    }

    _owner->delayNotifyAthleticsAward(&la);
}
#if 0
void Athletics::updateMartialHdr(const MartialHeader* mh)
{
    Player* defer = mh->md.defer;

    MartialData md = {0};

    md.idx = mh->md.idx;
    md.defer = defer;
    md.canAttack = 1;
    _owner->GetAthletics()->updateMartial(&md);

#if 0
    if(_owner == mh->owner)
    {
        if(_owner->getThreadId() == defer->getThreadId())
        {
            //Battle::BattleSimulator* bsim = new Battle::BattleSimulator(Battle::BS_ATHLETICS1, _owner, defer);
            //defer->PutFighters( *bsim, 1, true );
        }
        else
        {
            GameMsgHdr hdr2(0x330, defer->getThreadId(), defer, sizeof(MartialHeader));
            GLOBAL().PushMsg(hdr2, (void*)mh);
        }
    }
    else if(_owner == defer)
    {
        //Battle::BattleSimulator* bsim = new Battle::BattleSimulator(Battle::BS_ATHLETICS1, mh->owner, defer);
        //defer->PutFighters( *bsim, 1, true );
        if(_owner->getThreadId() == defer->getThreadId())
        {
        }
        else
        {
            GameMsgHdr hdr2(0x331, mh->owner->getThreadId(), mh->owner, sizeof(MartialData));
            GLOBAL().PushMsg(hdr2, &md);
        }
    }
#endif
}
#endif
#define COUNT_PER        5
#define COUNT_MAX        9
void getAthlRandomCategory(UInt8 *arr, UInt8 count)
{
    UInt8 diffculty;
    UInt8 i, j;

    if(!arr)
        return;

    for(i = 0; i < count;)
    {
        diffculty = uRand(COUNT_MAX);
        for(j = 0; j < i; j++)
        {
           if(arr[j] == diffculty)
               break;
        }
        if(j == i)
            arr[i++] = diffculty;
        else
            continue;
    }
}

void Athletics::listAthleticsMartial()
{
	Stream st(REP::ARENA_IFNO);
    st << static_cast<UInt16>(0x20);
    st << PlayerPInfo.ePhysical;
    st << gAthleticsRank.GetMaxPhysical(_owner->getVipLevel());

    if(_owner->getBuffData(PLAYER_BUFF_AMARTIAL_WIN) > 5)
    {
        _owner->setBuffData(PLAYER_BUFF_AMARTIAL_WIN, 0);
    }
    UInt8 wins =  5 - _owner->getBuffData(PLAYER_BUFF_AMARTIAL_WIN);
    st << wins;
    /** 选择过 **/
    if(PlayerPInfo.eSelectIndex != 0)
    {
        st << static_cast<UInt8>(1);
        UInt8 index = PlayerPInfo.eSelectIndex;
        if(index == 0 || index > 5)
            index = 1;
        UInt32 dbValue = PlayerPInfo.eCombine[index - 1];
        UInt8 athlDiffculty = dbValue >> 24;
        UInt8 athlCategory = (dbValue >> 16) & 0xFF;
        st << athlDiffculty;
        st << athlCategory;
        UInt8 tmp4 = dbValue & 0xFF;
        UInt32 tmp;
        char a[64];
        std::string curAward;
        tmp = GameAction()->onGetAthlRandomAward(athlDiffculty, tmp4);
        sprintf(a, "%u:%u", (tmp >> 16), (tmp & 0xFFFF));
        curAward.assign(a);
        st << curAward;
        UInt8 count = 0;
        UInt8 i;
        for(i = 0; i < 5; i++)
        {
            if(globalPlayers[PlayerPInfo.eRival[i]] != NULL)
                count++;
        }
        st << count;
        for(i = 0; i < 5/*count*/; i++)
        {
            Player* pl = globalPlayers[PlayerPInfo.eRival[i]];
            if(pl)
                st << pl->getName() << pl->getCountry() << pl->GetClass() << static_cast<UInt8>(pl->GetClassAndSex() & 0x0F) << pl->GetLev() << PlayerPInfo.eCanAttack[i] << PlayerPInfo.eRivalType[i] << pl->getPF();
            //else
            //    st << "" << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
        }
        st << Stream::eos;
        _owner->send(st);

        //if(needRefresh)
        //{
        //    GameMsgHdr hdr2(0x1F1, WORKER_THREAD_WORLD, _owner, 0);
        //    GLOBAL().PushMsg(hdr2, NULL);
        //}
    }
    else
    {
        st << static_cast<UInt8>(0);
        if(PlayerPInfo.eCombine[0] == 0 && PlayerPInfo.eCombine[1] == 0 && PlayerPInfo.eCombine[2] == 0 && PlayerPInfo.eCombine[3] == 0 && PlayerPInfo.eCombine[4] == 0)
        {
            st << static_cast<UInt8>(COUNT_PER);
            UInt8 a[COUNT_PER];
            getAthlRandomCategory(a, COUNT_PER);
            for(UInt8 i = 0; i < COUNT_PER; i++)
            {
                UInt8 tmp1, tmp2, tmp3, tmp4;
                UInt32 dbValue;
                tmp1 = GameAction()->onGetAthlRandomDiffculty();
                st << tmp1;
                tmp2 = a[i];
                st << tmp2;
                tmp3 = 0;
                st << tmp3;
                UInt8 totalChoice = GameAction()->onGetAthlRandomMaxValue(tmp1);
                tmp4 = uRand(totalChoice);
                UInt32 tmp;
                char a[64];
                std::string curAward;
                tmp = GameAction()->onGetAthlRandomAward(tmp1, tmp4);
                sprintf(a, "%u:%u", (tmp >> 16), (tmp & 0xFFFF));
                curAward.assign(a);
                st << curAward;
                dbValue = (tmp1 << 24) + (tmp2 << 16) + (tmp3 << 8) + tmp4;
                if(PlayerPInfo.eCombine[i] != dbValue)
                {
                    char column[32];
                    sprintf(column, "eCombine%u", i + 1);
                    PlayerPInfo.eCombine[i] = dbValue;
                    DB6().PushUpdateData("UPDATE `athletics_rank` SET `%s` = %u WHERE `ranker` = %" I64_FMT "u", column, PlayerPInfo.eCombine[i], _owner->getId());
                }
            }
            st << Stream::eos;
            _owner->send(st);
        }
        else
        {
            st << static_cast<UInt8>(COUNT_PER);
            UInt32 dbValue;
            UInt8 tmp1, tmp2, tmp3, tmp4;
            for(UInt8 i = 0; i < COUNT_PER; i++)
            {
                dbValue = PlayerPInfo.eCombine[i];
                tmp1 = dbValue >> 24;
                st << tmp1;
                tmp2 = (dbValue >> 16) & 0xFF;
                st << tmp2;
                tmp3 = (dbValue >> 8) & 0xFF;
                st << tmp3;
                tmp4 = dbValue & 0xFF;
                UInt32 tmp;
                char a[64];
                std::string curAward;
                tmp = GameAction()->onGetAthlRandomAward(tmp1, tmp4);
                sprintf(a, "%u:%u", (tmp >> 16), (tmp & 0xFFFF));
                curAward.assign(a);
                st << curAward;
            }
            st << Stream::eos;
            _owner->send(st);
        }
    }

}

void Athletics::listAthleticsMartial2(UInt8 type, bool update)
{
    if(type == 0)
        update = true;
    if(type == 3)
        type = 0;
    Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
    st << type;
    if(PlayerPInfo.eSelectIndex != 0)
    {
        UInt8 count = 0;
        UInt8 i;
        for(i = 0; i < 5; i++)
        {
            if(globalPlayers[PlayerPInfo.eRival[i]] != NULL)
                count++;
        }
        st << count;
        for(i = 0; i < count; i++)
        {
            Player* player = globalPlayers[PlayerPInfo.eRival[i]];
            if(player)
                st << player->getName() << player->getCountry() << player->GetClass() << static_cast<UInt8>(player->GetClassAndSex() & 0x0F) << player->GetLev() << PlayerPInfo.eCanAttack[i] << PlayerPInfo.eRivalType[i] << player->getPF();
        }
    }
    else
    {
        if(update || (PlayerPInfo.eCombine[0] == 0 && PlayerPInfo.eCombine[1] == 0 && PlayerPInfo.eCombine[2] == 0 && PlayerPInfo.eCombine[3] == 0 && PlayerPInfo.eCombine[4] == 0))
        {
            st << static_cast<UInt8>(COUNT_PER);
            UInt8 a[COUNT_PER];
            getAthlRandomCategory(a, COUNT_PER);
            for(UInt8 i = 0; i < COUNT_PER; i++)
            {
                UInt8 tmp1, tmp2, tmp3, tmp4;
                UInt32 dbValue;
                tmp1 = GameAction()->onGetAthlRandomDiffculty();
                st << tmp1;
                tmp2 = a[i];
                st << tmp2;
                tmp3 = 0;
                st << tmp3;
                UInt8 totalChoice = GameAction()->onGetAthlRandomMaxValue(tmp1);
                tmp4 = uRand(totalChoice);
                UInt32 tmp;
                char a[64];
                std::string curAward;
                tmp = GameAction()->onGetAthlRandomAward(tmp1, tmp4);
                sprintf(a, "%u:%u", (tmp >> 16), (tmp & 0xFFFF));
                curAward.assign(a);
                st << curAward;
                dbValue = (tmp1 << 24) + (tmp2 << 16) + (tmp3 << 8) + tmp4;
                if(PlayerPInfo.eCombine[i] != dbValue)
                {
                    char column[32];
                    sprintf(column, "eCombine%u", i + 1);
                    PlayerPInfo.eCombine[i] = dbValue;
                    DB6().PushUpdateData("UPDATE `athletics_rank` SET `%s` = %u WHERE `ranker` = %" I64_FMT "u", column, PlayerPInfo.eCombine[i], _owner->getId());
                }
            }
        }
        else
        {
            st << static_cast<UInt8>(COUNT_PER);
            UInt32 dbValue;
            UInt8 tmp1, tmp2, tmp3, tmp4;
            for(UInt8 i = 0; i < COUNT_PER; i++)
            {
                dbValue = PlayerPInfo.eCombine[i];
                tmp1 = dbValue >> 24;
                st << tmp1;
                tmp2 = (dbValue >> 16) & 0xFF;
                st << tmp2;
                tmp3 = (dbValue >> 8) & 0xFF;
                st << tmp3;
                tmp4 = dbValue & 0xFF;
                UInt32 tmp;
                char a[64];
                std::string curAward;
                tmp = GameAction()->onGetAthlRandomAward(tmp1, tmp4);
                sprintf(a, "%u:%u", (tmp >> 16), (tmp & 0xFFFF));
                curAward.assign(a);
                st << curAward;
            }
        }
    }
    st << Stream::eos;
    _owner->send(st);
}

void  Athletics:: PayForPaging(UInt8 type)
{
    UInt8   MoneyEnough = 0 ;
    UInt32 amount = 100 ;
    if(_owner->getTael() < amount)
    {
       _owner->sendMsgCode(0, 1100);
    }
    else
    {
        ConsumeInfo ci(AthleticPaging,0,0);
        _owner->useTael(amount,&ci);
        MoneyEnough = 1;
    }

    GObject::AthleticsPay  msg;
    msg.type = type;
    msg.moneyEnough = MoneyEnough;

    GameMsgHdr hdr(0x19A, WORKER_THREAD_WORLD, _owner, sizeof(msg));
    GLOBAL().PushMsg(hdr, &msg);
}

void Athletics:: PayForKillCD(UInt8 type)
{
    UInt32 cost = 1;
    GObject::AthleticsPay msg;
    msg.type = AthleticsRank::AthleticsPayForKillCD;
    msg.moneyEnough = 0;
   if(_owner->getGold() < cost)
       _owner->sendMsgCode(0, 1104);
   else
   {
        ConsumeInfo ci(AthleticKillCD,0,0);
        _owner->useGold(cost, &ci);
        msg.moneyEnough = 1;
   }
   GameMsgHdr hdr(0x19A, WORKER_THREAD_WORLD, _owner, sizeof(msg));
   GLOBAL().PushMsg(hdr, &msg);
}

/*UInt8 getRivalRandomDiffculty()
{
    UInt16 rate;
    UInt8 rivalDiffculty;
    rate = uRand(1000);
    if(rate >= 996)
        rivalDiffculty = 7;
    else if(rate >= 992)
        rivalDiffculty = 6;
    else if(rate >= 988)
        rivalDiffculty = 5;
     else if(rate >= 984)
        rivalDiffculty = 4;
     else if(rate >= 980)
        rivalDiffculty = 3;
     else if(rate >= 976)
        rivalDiffculty = 2;
     else if(rate >= 972)
        rivalDiffculty = 1;
     else
        rivalDiffculty = 0;
    return rivalDiffculty;
}*/

UInt8 getRivalRandomDiffculty()
{
    UInt16 rate;
    UInt8 rivalDiffculty;
    rate = uRand(1000);
    if(rate >= 970)
        rivalDiffculty = 7;
    else if(rate >= 940)
        rivalDiffculty = 6;
    else if(rate >= 910)
        rivalDiffculty = 5;
     else if(rate >= 880)
        rivalDiffculty = 4;
     else if(rate >= 850)
        rivalDiffculty = 3;
     else if(rate >= 820)
        rivalDiffculty = 2;
     else if(rate >= 790)
        rivalDiffculty = 1;
     else
        rivalDiffculty = 0;
    return rivalDiffculty;
}

void Athletics::updateAthleticsMartial(Player *pl)
{
    if(!pl)
        return;
    UInt64 idIdx[5] = {0};
    UInt8 level = pl->GetLev();
    UInt8 index = PlayerPInfo.eSelectIndex;
    if(index == 0 || index > 5)
        index = 1;
    UInt8 diffculty = (PlayerPInfo.eCombine[index - 1] >> 24) & 0xFF;
    if(diffculty == 2)
        ;
    else if(diffculty == 3)
        level += 1;
    else if(diffculty == 4)
        level += 2;
    else if(diffculty == 5)
        level += 3;
    else
        level -= 1;

    UInt32 size = 0;
    std::map<UInt16, UInt64> mapList;
    UInt32 mapCnt = 0;
    UInt32 j;
    GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(level);
    if(it != GObject::globalLevelsPlayers.end())
    {
        GObject::LevelPlayers* lvPlayer = it->second;
        size = lvPlayer->size();
        for(j = 0; j < size; j++)
            mapList[mapCnt++] = (*lvPlayer)[j];
    }
    UInt8 cnt = 1;
    UInt16 k;
    URandom rnd(time(NULL));
    Int32 rollId;
    Int32 rollId2;
    bool repeat;
    while(size < 100 && (level - cnt) >= 29)
    {
        it = GObject::globalLevelsPlayers.find(level - cnt);
        ++cnt;
        if(it == GObject::globalLevelsPlayers.end())
            continue;
        GObject::LevelPlayers* lvPlayer2 = it->second;
        size += lvPlayer2->size();
        for(j = 0; j < lvPlayer2->size(); j++)
            mapList[mapCnt++] = (*lvPlayer2)[j];
    }

    UInt32 showCnt = size;
    if(showCnt > 5)
        showCnt = 5;
    for(k = 0; k < showCnt; k++)
    {
        rollId = rnd(size);
        rollId2 = rollId;
        while(rollId < static_cast<Int32>(size))
        {
            if(mapList[rollId] == pl->getId())
            {
                rollId += 1;
                continue;
            }
            repeat = false;
            for(j = 0; j < k; j++)
            {
                if(mapList[rollId] == idIdx[j])
                {
                    repeat = true;
                    break;
                }
            }
            if(repeat)
            {
                rollId += 1;
                continue;
            }
            break;
        }
        if(rollId == static_cast<Int32>(size))
        {
            rollId = rollId2 - 1;
            while(rollId >= 0)
            {
                if(mapList[rollId] == pl->getId())
                {
                    rollId -= 1;
                    continue;
                }
                repeat = false;
                for(j = 0; j < k; j++)
                {
                    if(mapList[rollId] == idIdx[j])
                    {
                        repeat = true;
                        break;
                    }
                }
                if(repeat)
                {
                    rollId -= 1;
                    continue;
                }
                break;
            }
        }
        if(rollId == -1)
            break;
        idIdx[k] = mapList[rollId];
    }

    UInt8 i;
    for(i = 0; i < 5; ++i)
    {
        char column1[32];
        char column2[32];
        char column3[32];
        sprintf(column1, "eRival%u", i + 1);
        sprintf(column2, "eCanAttack%u", i + 1);
        sprintf(column3, "eRivalType%u", i + 1);
        if(globalPlayers[idIdx[i]])
        {
             PlayerPInfo.eRival[i] = idIdx[i];
             PlayerPInfo.eCanAttack[i] = 1;
             PlayerPInfo.eRivalType[i] = getRivalRandomDiffculty();
        }
        else
        {
            PlayerPInfo.eRival[i] = 0;
            PlayerPInfo.eCanAttack[i] = 0;
            PlayerPInfo.eRivalType[i] = 0;
        }
        DB6().PushUpdateData("UPDATE `athletics_rank` SET `%s` = %" I64_FMT "u, `%s` = %u, `%s` = %u WHERE `ranker` = %" I64_FMT "u", column1, PlayerPInfo.eRival[i], column2, PlayerPInfo.eCanAttack[i], column3, PlayerPInfo.eRivalType[i], pl->getId());

    }
}

void Athletics::updateAthleticsP(Player* pl, UInt8 type)
{
    if(!pl)
        return;
    switch(type)
    {
        case 0:
            {
                if(pl->getTael() < 100)
                {
                    pl->sendMsgCode(0, 1100);
                    return;
                }
                ConsumeInfo ci(FlushAthletics,0,0);
                pl->useTael(100, &ci);
                UInt8 para = type;
                listAthleticsMartial2(para);
                pl->athleticsUdpLog(1030);
                pl->athleticsUdpLog(1030, 1);
            }
            break;
        case 1:
            {
                if(pl->getTael() < 100)
                {
                    pl->sendMsgCode(0, 1100);
                    return;
                }
                ConsumeInfo ci(FlushAthletics,0,0);
                pl->useTael(100, &ci);
                updateAthleticsMartial(pl);
                UInt8 para = type;
                listAthleticsMartial2(para);
                pl->athleticsUdpLog(1030);
                pl->athleticsUdpLog(1030, 2);
            }
            break;
        case 2:
            {
                UInt8 index = PlayerPInfo.eSelectIndex;
                if(index == 0 || index > 5)
                    index = 1;
                UInt32 dbValue = PlayerPInfo.eCombine[index - 1];
                UInt8 status = 2;
                dbValue = (dbValue & 0xFFFF00FF) | (status << 8);
                PlayerPInfo.eSelectIndex = 0;
                char column[32];
                sprintf(column, "eCombine%u", index);
                PlayerPInfo.eCombine[index - 1] = dbValue;
                DB6().PushUpdateData("UPDATE `athletics_rank` SET `eSelectIndex` = %u, `%s` = %u WHERE `ranker` = %" I64_FMT "u", PlayerPInfo.eSelectIndex, column, PlayerPInfo.eCombine[index - 1], pl->getId());
                pl->setBuffData(PLAYER_BUFF_AMARTIAL_WIN, 0);
                Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
                st << type << Stream::eos;
                pl->send(st);
                UInt8 para = 3;
                listAthleticsMartial2(para);
            }
            break;
        case 3:
            {
                if(pl->getBuffData(PLAYER_BUFF_ATHLETICS_P) == 0)
                    return;
                if(pl->getGold() < 1)
                {
                    pl->sendMsgCode(0, 1104);
                    return;
                }
                ConsumeInfo ci(AthleticKillCD2,0,0);
                pl->useGold(1, &ci);
                pl->setBuffData(PLAYER_BUFF_ATHLETICS_P, 0);
                pl->athleticsUdpLog(1031);
                Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
                st << type << Stream::eos;
                pl->send(st);
            }
            break;
        case 4:
            {
                if(PlayerPInfo.ePhysical >= gAthleticsRank.GetMaxPhysical(pl->getVipLevel()))
                    return;
                if(pl->GetVar(VAR_PHYSICAL_BUY) >= gAthleticsRank.GetMaxPhysical(pl->getVipLevel()))
                {
                    pl->sendMsgCode(0, 1496);
                    return;
                }
                pl->AddVar(VAR_PHYSICAL_BUY, 1);
                if(pl->getGold() < 1)
                {
                    pl->sendMsgCode(0, 1104);
                    return;
                }
                ConsumeInfo ci(AthleticPhysical,0,0);
                pl->useGold(1, &ci);
                PlayerPInfo.ePhysical += 1;
                pl->athleticsUdpLog(1029);
                DB6().PushUpdateData("UPDATE `athletics_rank` SET `ePhysical` = %u WHERE `ranker` = %" I64_FMT "u", PlayerPInfo.ePhysical, pl->getId());
                Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
                st << type << PlayerPInfo.ePhysical << Stream::eos;
                pl->send(st);
            }
            break;
        default:
            break;
    }

}

void Athletics::RequestSubDir(Player* player, UInt8 athlDiffculty, UInt8 athlCategory)
{
    UInt8 i;
    for(i = 0; i < 5; i++)
    {
        UInt32 dbValue = PlayerPInfo.eCombine[i];
        UInt8 diffcult = dbValue >> 24;
        UInt8 category = (dbValue >> 16) & 0xFF;
        if(athlDiffculty == diffcult && athlCategory == category)
            break;
    }
    if(i >= 5)
        return;
    UInt8 status = (PlayerPInfo.eCombine[i] >> 8) & 0xFF;
    if(status != 0)
        return;
    if(PlayerPInfo.ePhysical < 3)
    {
        player->sendMsgCode(0, 1497);
        return;
    }
    UInt32 now = TimeUtil::Now();
    if(player->getEPhysicalTime() >= now)
        return ;
    player->setEPhysicalTime(now);
    PlayerPInfo.ePhysical -= 3;
    Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
    st << static_cast<UInt8>(4) << PlayerPInfo.ePhysical << Stream::eos;
    player->send(st);
    PlayerPInfo.eSelectIndex = i + 1;
    DB6().PushUpdateData("UPDATE `athletics_rank` SET `ePhysical` = %u, `eSelectIndex` = %u WHERE `ranker` = %" I64_FMT "u", PlayerPInfo.ePhysical, PlayerPInfo.eSelectIndex, player->getId());
    updateAthleticsMartial(player);
    //player->GetAthletics()->listAthleticsMartial();
    //GameMsgHdr hdr(0x232, player->getThreadId(), player, 0);
    //GLOBAL().PushMsg(hdr, NULL);
    listAthleticsMartial();
}
#if 0
void Athletics::process()
{
    RankList::iterator it;
    UInt8 row;
    AthleticsRankData *data;
    UInt32 Viplvl;
    bool needSave;

    for(row = 0; row <= 1; ++row)
    {
        for(it = _ranks[row].begin(); it != _ranks[row].end(); ++it)
        {
            data = *(it->second);
            if(!data->ranker)
                continue;
            needSave = false;
            //考虑5分钟误差
            Viplvl = data->ranker->getVipLevel();
            if(((TimeUtil::Now() % 86400 < 300) || (TimeUtil::Now() % 86400 > 86100)) && (Viplvl >= 6))
            {
                needSave = true;
                data->ePhysical += 6;
            }
            if(data->ePhysical == MaxPhysical[Viplvl])
            {
                if(needSave)
                    DB6().PushUpdateData("UPDATE `athletics_rank` SET `ePhysical` = %u WHERE `ranker` = %" I64_FMT "u", data->ePhysical, data->ranker->getId());
                continue;
            }
            else if(data->ePhysical > MaxPhysical[Viplvl])
                data->ePhysical = MaxPhysical[Viplvl];
            else
                data->ePhysical += 1;
            DB6().PushUpdateData("UPDATE `athletics_rank` SET `ePhysical` = %u WHERE `ranker` = %" I64_FMT "u", data->ePhysical, data->ranker->getId());
        }
    }
#endif

    void Athletics::PhysicalCheck(void)
    {
        UInt32 Viplvl = _owner->getVipLevel();
        bool needSave = false;

        //考虑5分钟误差
        Viplvl = _owner->getVipLevel();
        if((((TimeUtil::Now() - 8 * 3600) % 86400 < 300) || ((TimeUtil::Now() - 8 * 3600) % 86400 > 86100)) && (Viplvl >= 6))
        {
            needSave = true;
            PlayerPInfo.ePhysical += 6;
        }
        if(PlayerPInfo.ePhysical == gAthleticsRank.GetMaxPhysical(Viplvl))
        {
            if(needSave)
                DB6().PushUpdateData("UPDATE `athletics_rank` SET `ePhysical` = %u WHERE `ranker` = %" I64_FMT "u", PlayerPInfo.ePhysical, _owner->getId());
            return;
        }
        else if(PlayerPInfo.ePhysical > gAthleticsRank.GetMaxPhysical(Viplvl))
            PlayerPInfo.ePhysical = gAthleticsRank.GetMaxPhysical(Viplvl);
        else
            PlayerPInfo.ePhysical += 1;
        DB6().PushUpdateData("UPDATE `athletics_rank` SET `ePhysical` = %u WHERE `ranker` = %" I64_FMT "u", PlayerPInfo.ePhysical, _owner->getId());
    }

}
