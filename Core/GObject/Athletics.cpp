#include "Config.h"
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

namespace GObject
{

Athletics::Athletics(Player * player) : _owner(player), _hasEnterAthletics(false)
{
    memset(_martial, 0, sizeof(_martial));
    memset(_canAttack, 0, sizeof(_canAttack));
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
	DB6().PushUpdateData("INSERT INTO `athletics_record` (`id`, `atker`, `defer`, `repid`, `time`, `winSide`) VALUES(%u, %"I64_FMT"u, %"I64_FMT"u, %u, %u, %u)", data->id, (side == 0 ? _owner->getId() : target->getId()), (side == 0 ? target->getId() : _owner->getId()), data->reptid, data->time, win);
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
	if (count > static_cast<UInt16>(_athleticses.size()))
		count = static_cast<UInt16>(_athleticses.size());

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

void Athletics::attack(Player * defer)
{
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

		Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, _owner, defer);
		_owner->PutFighters( bsim, 0, true );
		defer->PutFighters( bsim, 1, true );
		bsim.start();
		res = bsim.getWinner() == 1;
		reptid = bsim.getId();
        if (res)
           GameAction()->RunOperationTaskAction0(_owner, 4);

		Stream st(REP::ATTACK_NPC);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
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
            _owner->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 0);
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
	_owner->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
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

void Athletics::updateMartial(const MartialData* md)
{
    if(!md)
        return;

    if(md->idx > 2)
        return;

    _martial[md->idx] = md->defer;
    _canAttack[md->idx] = md->canAttack;
    listAthleticsMartial();
}

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

    GameAction()->doAty(this->_owner, AtyAthletics, 0, 0);
    do
    {
        UInt8 idx = 0xFF;
        for(UInt8 i = 0; i < 3; ++i)
        {
            if(_martial[i] == defer)
            {
                idx = i;
                break;
            }
        }

        if(idx > 2)
            break;

        if(0 == _canAttack[idx])
            break;

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
        Battle::BattleSimulator bsim = Battle::BattleSimulator(Battle::BS_ATHLETICS1, _owner, defer);
        defer->PutFighters( bsim, 1, true );
        _owner->PutFighters( bsim, 0, true );
        bsim.start();
        res = bsim.getWinner() == 1;
        reptid = bsim.getId();

        Stream st(REP::ATTACK_NPC);
        st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
        _owner->send(st);

        awardMartial(defer, res);

        UInt32 time = TimeUtil::Now();
		id = addAthleticsData(0, defer, res ? 0 : 1, reptid, time);
		notifyAthleticsDeferData(0, defer, id, res ? 0 : 1, reptid, time);
       
        Player* winner =  res ? _owner : defer;

        GameAction()->doAttainment(winner,  Script::ATHLETICS_WIN , 1);
        if (res)
            winner->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 1);
    }while(false);

	AthleticsResult ar = {id, 0, defer, res };

	GameMsgHdr hdr(0x1F2, WORKER_THREAD_WORLD, _owner, sizeof(AthleticsResult));
	GLOBAL().PushMsg(hdr, &ar);
}

void Athletics::beAttackMartial(Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup)
{
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
	_owner->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
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
    }


}

void Athletics::awardMartial(Player* defer, bool win)
{
    GObject::LastAthAward la = {0};
    if(win)
    {
        for(UInt8 i = 0; i < 3; ++i)
        {
            if(_martial[i] == defer)
            {
                _canAttack[i] = 0;
                break;
            }
        }

        UInt8 wins = _owner->getBuffData(PLAYER_BUFF_AMARTIAL_WIN) + 1;
        if(wins >= 5)
        {
            _owner->GetPackage()->AddItem(6, 1, 1, true, FromAthletAward);
            la.itemId = 6;
            la.itemCount = 1;
            wins = 0;

            _owner->OnHeroMemo(MC_ATHLETICS, MD_STARTED, 0, 2);
        }
        la.prestige = 10 * (World::_wday == 3 ? 2 : 1);
        _owner->getPrestige(la.prestige, false);
        _owner->setBuffData(PLAYER_BUFF_AMARTIAL_WIN, wins, true);

        listAthleticsMartial();
    }
    else
    {
        la.prestige = 5 * (World::_wday == 3 ? 2 : 1);
        _owner->getPrestige(la.prestige, false);
    }

    _owner->delayNotifyAthleticsAward(&la);
}

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

void Athletics::listAthleticsMartial()
{
	Stream st(REP::ARENA_IFNO);
    st << static_cast<UInt16>(0x10);

    UInt8 needRefresh = true;
    UInt8 wins =  5 - _owner->getBuffData(PLAYER_BUFF_AMARTIAL_WIN);
	st << wins << static_cast<UInt8>(3);
	for (UInt8 i = 0; i < 3; ++i)
	{
        if(_martial[i] != 0)
        {
            Player* pl = _martial[i];
            if( pl != NULL && _canAttack[i] != 0)
                needRefresh = false;

            if(pl == NULL)
                st << "" << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
            else
                st << pl->getName() << pl->getCountry() << pl->GetClass() << static_cast<UInt8>(pl->GetClassAndSex() & 0x0F) << pl->GetLev() << _canAttack[i];
        }
        else
        {
            st << "" << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
        }
	}

    st << Stream::eos;

    _owner->send(st);

    if(needRefresh)
    {
        GameMsgHdr hdr2(0x1F1, WORKER_THREAD_WORLD, _owner, 0);
        GLOBAL().PushMsg(hdr2, NULL);
    }
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
        ConsumeInfo ci(AthleticPaging,0,0);
        _owner->useGold(cost, &ci);
        msg.moneyEnough = 1;
   }
   GameMsgHdr hdr(0x19A, WORKER_THREAD_WORLD, _owner, sizeof(msg));
   GLOBAL().PushMsg(hdr, &msg);
}
}

