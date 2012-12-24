#include "Config.h"
#include "MsgID.h"
#include "Server/SysMsg.h"
#include "Player.h"
#include "NewRelation.h"
#include "Country.h"
#include "Script/GameActionLua.h"

namespace GObject
{

NewRelation::NewRelation(Player* player):_owner(player), m_mood(0), m_sign("")
{
}

NewRelation::~NewRelation()
{
}

void NewRelation::LoadFromDB(UInt8 mood, std::string& sign)
{
    m_mood = mood;
    m_sign = sign;
}

UInt8 NewRelation::getMood()
{
    return m_mood;
}

void NewRelation::setMood(UInt8 status)
{
    if(status != m_mood)
    {
        m_mood = status;
        DB().PushUpdateData("REPLACE INTO `new_relation` (`playerId`, `mood`, `sign`) VALUES (%"I64_FMT"u, %u, '%s')", this->_owner->getId(), m_mood, m_sign.c_str());
    }
}

std::string& NewRelation::getSign()
{
    return m_sign;
}

void NewRelation::setSign(std::string& status)
{
    if(status == m_sign)
        return;

    m_sign = status;
    DB().PushUpdateData("REPLACE INTO `new_relation` (`playerId`, `mood`, `sign`) VALUES (%"I64_FMT"u, %u, '%s')", this->_owner->getId(), m_mood, m_sign.c_str());
}

void NewRelation::beAttack(Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup, Player *defer)
{
    if(!atker || !defer)
        return;
	Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, atker, defer);
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
	defer->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
    if(atker->isOnline())
        atker->send(st);
    if(defer->isOnline())
        defer->send(st);
    atker->delGlobalFlag(Player::Challenging);
    defer->delGlobalFlag(Player::BeChallenging);
}

void NewRelation::attack(Player *atker, Player *defer)
{
    if(!atker || !defer)
        return;

    if(atker->hasGlobalFlag(Player::Challenging) || atker->hasGlobalFlag(Player::Challenging) || atker->getBuffLeft(PLAYER_BUFF_N_ATHLETICS) > 0)
        return;
    if(defer->hasGlobalFlag(Player::Challenging) || defer->hasGlobalFlag(Player::Challenging) || defer->getBuffLeft(PLAYER_BUFF_N_ATHLETICS) > 0)
        return;

    atker->addGlobalFlag(Player::Challenging);
    defer->addGlobalFlag(Player::BeChallenging);
    atker->setBuffData(PLAYER_BUFF_N_ATHLETICS, TimeUtil::Now() + 180);
    defer->setBuffData(PLAYER_BUFF_N_ATHLETICS, TimeUtil::Now() + 180);

    GameAction()->doStrong(atker, SthChallenge, 0, 0);
    UInt8 tid = defer->getThreadId();
	if(tid == atker->getThreadId())
	{
		bool res;

        Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, atker, defer);
		atker->PutFighters( bsim, 0, true );
		defer->PutFighters( bsim, 1, true );
		bsim.start();
		res = bsim.getWinner() == 1;

        Stream st(REP::ATTACK_NPC);
        st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
        if(atker->isOnline())
            atker->send(st);
        if (defer->isOnline())
            defer->send(st);
        atker->delGlobalFlag(Player::Challenging);
        defer->delGlobalFlag(Player::BeChallenging);
    }
    else
    {
        printf("[test][%s:%d], different thread\n", __func__, __LINE__);
        struct NewRelationBeData
        {
            Player * attacker;
            UInt16 formation;
            UInt16 portrait;
            Lineup lineup[5];
        };
	    NewRelationBeData abd = { atker, atker->getFormation(), static_cast<UInt16>(atker->getMainFighter() != NULL ? atker->getMainFighter()->getId() : 0) };
	    for(int i = 0; i < 5; ++ i)
		    abd.lineup[i] = atker->getLineup(i);
        GameMsgHdr hdr(0x223, tid, defer, sizeof(NewRelationBeData));
        GLOBAL().PushMsg(hdr, &abd);
    }
}

void NewRelation::countrySend(Player* responder, UInt8 type, std::string& name)
{
    Stream st(REP::NEWRELATION);
    st << type;
    st << name;
    st << Stream::eos;
    responder->send(st);
}

static void msgTips(UInt16 index, Player *player, Player *defer = NULL)
{
    UInt8 type;
    if(!player)
        return;
    if(index == 4011)
    {
        SYSMSG_SENDV(index, player, player->getCountry(), player->getNameNoSuffix(player->getName()));

        printf("[test][%s:%d], %d:%d\n", __func__, __LINE__, player->getThreadId(), defer->getThreadId());
        if(defer)
        {
            std::string name = player->getName();
            if(player->getThreadId() == defer->getThreadId())
            {
                Stream st2(REP::NEWRELATION);
                type = 5;
                st2 << type;
                st2 << name;
                st2 << Stream::eos;
                defer->send(st2);
            }
            else
            {
                struct stNewRelationReq
                {
                    UInt8 type;
                    std::string atkName;
                };
                stNewRelationReq nwr;
                nwr.type = 5;
                nwr.atkName = name;
                GameMsgHdr hdr(0x224, player->getThreadId(), defer, sizeof(stNewRelationReq));
                GLOBAL().PushMsg(hdr, &nwr);
            }
        }
    }
    else
    {
        SYSMSG_SENDV(index, player);
        Stream st(REP::NEWRELATION);
        type = 4;
        st << type;
        st << static_cast<UInt8>(0);
        st << Stream::eos;
        player->send(st);
    }
}

void NewRelation::challengeRequest(Player *atker, std::string& name)
{
    if(!atker)
        return;
    if(atker->getBuffData(PLAYER_BUFF_ATTACKING) > 0)
        return msgTips(4010, atker);
    if(atker->hasFlag(GObject::Player::CountryBattle))
        return msgTips(4010, atker);
    if(atker->hasFlag(GObject::Player::ClanBattling))
        return msgTips(4010, atker);
    if(atker->hasFlag(GObject::Player::InHeroIsland) )
        return msgTips(4010, atker);
    if(atker->hasFlag(GObject::Player::InCopyTeam))
        return msgTips(4010, atker);
    // beacse of the same value of AutoCopy and Challenging:
    // AutoCopy    = 0x00000010
    // Challenging = 0x00000010
    if(!(atker->hasFlag(GObject::Player::AutoCopy)) && (atker->hasFlag(GObject::Player::Challenging) || atker->hasFlag(GObject::Player::BeChallenging)))
        return msgTips(4010, atker);
    if(atker->getBuffLeft(PLAYER_BUFF_N_ATHLETICS) != 0)
        return msgTips(4016, atker);

    Player * defer;
    defer = globalNamedPlayers[atker->fixName(name)];
	if (defer == NULL)
        return;
    if(defer == atker)
		return msgTips(4012, atker);
    if(defer->getBuffData(PLAYER_BUFF_ATTACKING) > 0)
        return msgTips(4012, atker);
    if(defer->hasFlag(GObject::Player::CountryBattle))
        return msgTips(4012, atker);
    if(defer->hasFlag(GObject::Player::ClanBattling))
        return msgTips(4012, atker);
    if(defer->hasFlag(GObject::Player::InHeroIsland) )
        return msgTips(4012, atker);
    if(defer->hasFlag(GObject::Player::InCopyTeam))
        return msgTips(4012, atker);
    if(!(defer->hasFlag(GObject::Player::AutoCopy)) && (defer->hasFlag(GObject::Player::Challenging) || defer->hasFlag(GObject::Player::BeChallenging)))
        return msgTips(4012, atker);
    if(defer->getBuffLeft(PLAYER_BUFF_N_ATHLETICS) != 0)
        return msgTips(4017, atker);

    msgTips(4011, atker, defer);
}

static void msgTips2(UInt16 index, Player *sender, UInt16 index2, Player *responder)
{
    if(sender)
        SYSMSG_SENDV(index, sender);
    if(responder)
        SYSMSG_SENDV(index2, responder);
}

void NewRelation::challengeRespond(Player *responder, std::string& name, UInt8 accept)
{
    if(!responder)
        return;

    Player * sender;
    sender = globalNamedPlayers[responder->fixName(name)];
    if(sender == NULL)
        return;
    if(accept)
    {
        if(sender == responder)
            return msgTips2(4010, sender, 4015, responder);
        if(sender->getBuffData(PLAYER_BUFF_ATTACKING) > 0 || sender->getBuffData(PLAYER_BUFF_ATTACKING) > 0)
            return msgTips2(4010, sender, 4015, responder);
        if(sender->hasFlag(GObject::Player::CountryBattle))
            return msgTips2(4010, sender, 4015, responder);
        if(sender->hasFlag(GObject::Player::ClanBattling))
            return msgTips2(4010, sender, 4015, responder);
        if(sender->hasFlag(GObject::Player::InHeroIsland) )
            return msgTips2(4010, sender, 4015, responder);
        if(sender->hasFlag(GObject::Player::InCopyTeam))
            return msgTips2(4010, sender, 4015, responder);
        if(!(sender->hasFlag(GObject::Player::AutoCopy)) && (sender->hasFlag(GObject::Player::Challenging) || sender->hasFlag(GObject::Player::BeChallenging)))
            return msgTips2(4010, sender, 4015, responder);
        if(sender->getBuffLeft(PLAYER_BUFF_N_ATHLETICS) != 0)
            return msgTips2(4016, sender, 4017, responder);

        attack(sender, responder);
    }
    else
    {
        msgTips2(4014, sender, 0, NULL);
    }
}


}


