#ifndef _COUNTRYOUTERMSGHANDLER_H_
#define _COUNTRYOUTERMSGHANDLER_H_

#include "MsgTypes.h"
#include "MsgID.h"

#include "GData/Store.h"
#include "GData/ExpTable.h"
#include "GData/LootTable.h"
#include "GObject/GObjectManager.h"
#include "GObject/Boss.h"
#include "GObject/Clan.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GData/Money.h"
#include "GObject/CountryBattle.h"
#include "GObject/Mail.h"
#include "GObject/Map.h"
#include "GObject/MapCollection.h"
#include "GObject/MapObject.h"
#include "GObject/MOAction.h"
#include "GObject/Package.h"
#include "GObject/Trade.h"
#include "GObject/TaskMgr.h"
#include "GObject/AttainMgr.h"
#include "GObject/ActivityMgr.h"
#include "GObject/Athletics.h"
#include "GObject/Dungeon.h"
#include "GObject/ChatItem.h"
#include "GObject/Sale.h"
#include "GObject/LuckyDraw.h"
#include "GObject/SpecialAward.h"
#include "MsgFunc.h"
#include "CountryMsgStruct.h"
#include "Server/SysMsg.h"
#include "Battle/BattleSimulator.h"
#include "GMHandler.h"
#include "GObject/Copy.h"
#include "GObject/FrontMap.h"
#include "GData/Money.h"
#include "GObject/WBossMgr.h"
#include "GObject/HeroIsland.h"
#include "GObject/Var.h"
#include "GObject/ClanRankBattle.h"

#include "Common/Serialize.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/BinaryReader.h"
#include "LoginMsgHandler.h"
#include "GObject/SaleMgr.h"
#include "GObject/TeamCopy.h"
#include "GObject/HeroMemo.h"
#include "GObject/ShuoShuo.h"
#include "GObject/CFriend.h"
#include "GObject/TownDeamon.h"
#include "GObject/Arena.h"
#include "GObject/SingleHeroStage.h"
#include "GObject/PracticePlace.h"

#include "GObject/Tianjie.h"
#include "Memcached.h"
#include "GObject/RechargeTmpl.h"

struct NullReq
{
	UInt32 ticket;
	MESSAGE_DEF1(REQ::KEEP_ALIVE, UInt32, ticket);
};

struct SelectCountry
{
    UInt8 _country;
	MESSAGE_DEF1(REQ::CAMPS_CHOICE, UInt8, _country);
};

struct NewGuildReq
{
    UInt16 _step;
	MESSAGE_DEF1(REQ::NEW_HAND_STEP, UInt16, _step);
};

struct PlayerInfoReq
{
	MESSAGE_DEF(REQ::USER_INFO);
};

struct PurchaseBookReq
{
	UInt8 _pos;
	MESSAGE_DEF1(REQ::BOOK_BUY, UInt8, _pos);
};

struct StatusChangeReq
{
	UInt8 _id;
	MESSAGE_DEF1(REQ::STATE_CHANGE, UInt8, _id);
};

struct FighterEquipReq
{
	UInt32 _id;
	UInt8 _part;
	UInt32 _equipId;
	MESSAGE_DEF3(REQ::CHANGE_EQ, UInt32, _id, UInt8, _part, UInt32, _equipId);
};

struct RecruitFighterReq
{
	UInt8 _pos;
	MESSAGE_DEF1(REQ::HIRE_HERO, UInt8, _pos);
};

struct FighterDismissReq
{
	UInt32	_fgtid;
	MESSAGE_DEF1(REQ::FIRE_HERO, UInt32, _fgtid);
};

struct FighterRegenReq
{
	UInt32	_fgtid;
	MESSAGE_DEF1(REQ::ADD_HP, UInt32, _fgtid);
};

struct FighterTrainReq
{
	UInt32 _fgtId;
	UInt8 _type;
	MESSAGE_DEF2(REQ::POTENTIAL, UInt32, _fgtId, UInt8, _type);
};

struct TakeOnlineRewardReq
{
    UInt8 _flag;
	MESSAGE_DEF1(REQ::REWARD, UInt8, _flag);
};

struct EnchantReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt8 _type;
    UInt16 _count;
    UInt8 _level;
	// UInt8 _protect;
	MESSAGE_DEF5(REQ::EQ_ENHANCE, UInt16, _fighterId, UInt32, _itemid, UInt8, _type, UInt16, _count, UInt8, _level/*, UInt8, _protect*/);
};

struct OpenSocketReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	MESSAGE_DEF2(REQ::EQ_PUNCH, UInt16, _fighterId, UInt32, _itemid);
};

#if 0
struct MergeGemReq
{
	UInt16 _itemid;
	UInt8 _bindNum;
	UInt8 _protect;
	MESSAGE_DEF3(REQ::MERGE_GEM, UInt16, _itemid, UInt8, _bindNum, UInt8, _protect);
};
#endif

struct AttachGemReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt16 _gemid;
	UInt8 _bind;
	MESSAGE_DEF4(REQ::EQ_EMBED, UInt16, _fighterId, UInt32, _itemid, UInt16, _gemid, UInt8, _bind);
};

struct DetachGemReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt8 _pos;
	UInt8 _protect;
	MESSAGE_DEF4(REQ::EQ_UN_EMBED, UInt16, _fighterId, UInt32, _itemid, UInt8, _pos, UInt8, _protect);
};

#if 0
struct SplitReq
{
	UInt32 _itemid;
	UInt8 _protect;
	MESSAGE_DEF2(REQ::EQ_UN_SINGLE, UInt32, _itemid, UInt8, _protect);
};
#endif

#if 0
struct ExchangeReq
{
	typedef Array<UInt32, 5> ExchangeItemType;
	UInt32 _itemid[5];
	MESSAGE_DEF1(REQ::EQ_EXCHANGE, ExchangeItemType, _itemid);
};
#endif

struct ForgeReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	//i UInt8 _type;
	UInt8 _protect;
	MESSAGE_DEF3(REQ::EQ_PURIFY, UInt16, _fighterId, UInt32, _itemid, /*UInt8, _type,*/ UInt8, _protect);
};

struct BatchMergeReq
{
    UInt16 _gemId;
    UInt16 _unBindNum;
    UInt16 _bindNum;
    UInt8 _protect;
    MESSAGE_DEF4(REQ::GEM_UPGRADE, UInt16, _gemId, UInt16, _unBindNum, UInt16, _bindNum, UInt8, _protect);
};

#if 0
struct ExchangeSetReq
{
	typedef Array<UInt32, 3> ExchangeSetItemType;
	UInt32 _itemid[3];
	UInt8 _flag;
	MESSAGE_DEF2(REQ::EQ_EXCHANGE_POS, ExchangeSetItemType, _itemid, UInt8, _flag);
};
#endif

#if 0
struct ActivateAttrReq
{
	UInt16 _fighterId;
	UInt32 _itemId;
	UInt32 _itemId2;
	MESSAGE_DEF3(REQ::EQ_ACTIVE, UInt16, _fighterId, UInt32, _itemId, UInt32, _itemId2);
};
#endif
struct ActivateAttrReq
{
	UInt16 _fighterId;
	UInt32 _itemId;
	MESSAGE_DEF2(REQ::EQ_ACTIVATE, UInt16, _fighterId, UInt32, _itemId);
};

struct EquipMoveReq
{
	UInt16 _ffighterId;
	UInt16 _tfighterId;
	UInt32 _fitemId;
	UInt32 _titemId;
    UInt8  _type;
	MESSAGE_DEF5(REQ::EQ_MOVE, UInt16, _ffighterId,UInt16, _tfighterId, UInt32, _fitemId,UInt32, _titemId,UInt8,_type);
};

#if 0
struct OutCitySwitchStruct
{
	UInt8 _mapid;

	MESSAGE_DEF1(REQ::CITY_OUTSIDE_MOVE, UInt8, _mapid);
};
#endif

struct InCitySwitchStruct
{
	UInt16 _locid;

	MESSAGE_DEF1(REQ::MAP_LOCATE, UInt16, _locid);
};

struct CityTransportReq
{
	UInt16 _locid;
	UInt8 flag;

	MESSAGE_DEF2(REQ::MAP_TRANSPORT, UInt16, _locid, UInt8, flag);
};

struct DungeonOpReq
{
	UInt8 op;
	UInt8 type;
	MESSAGE_DEF2(REQ::BABEL_JOIN, UInt8, op, UInt8, type);
};

struct DungeonInfoReq
{
	UInt8 op;
	UInt8 type;
	MESSAGE_DEF2(REQ::BABEL_UPDATE, UInt8, op, UInt8, type);
};

struct DungeonBattleReq
{
	UInt8 type;
	UInt8 level;
	MESSAGE_DEF2(REQ::BABEL_START, UInt8, type, UInt8, level);
};

struct DungeonAutoReq
{
	UInt8 type;
    UInt8 mtype;
	MESSAGE_DEF2(REQ::BABEL_AUTO_START, UInt8, type, UInt8, mtype);
};

struct DungeonCompleteAutoReq
{
	MESSAGE_DEF(REQ::BABEL_END);
};

struct DailyReq
{
	MESSAGE_DEF(REQ::DAILY);
};

#if 0
struct ChallengePlayerReq
{
	std::string _name;
	MESSAGE_DEF1(REQ::CHANGE_NAME, std::string, _name);
};
#endif

struct AttackNpcReq
{
	UInt32 _npcId;
	MESSAGE_DEF1(REQ::ATTACK_NPC, UInt32, _npcId);
};

struct AutoBattleReq
{
	UInt32 _npcId;
	MESSAGE_DEF1(REQ::TASK_HOOK, UInt32, _npcId);
};

struct CancelAutoBattleReq
{
	MESSAGE_DEF(REQ::TASK_HOOK_STOP);
};

struct InstantAutoBattleReq
{
	MESSAGE_DEF(REQ::TASK_HOOK_ADD);
};

struct CountryBattleJoinStruct
{
	UInt8 _action;

	MESSAGE_DEF1(REQ::CAMPS_WAR_JOIN, UInt8, _action);
};

struct LanchChallengeReq
{
	std::string target;
	MESSAGE_DEF1(REQ::LANCHCHALLENGE, std::string, target);
};

struct RequestChallengeReq
{
	UInt8 res;
	std::string attacker;
	MESSAGE_DEF2(REQ::REQUESTCHALLENGE, UInt8, res, std::string, attacker);
};
struct BattleEndReq
{
	MESSAGE_DEF(REQ::FIGHT_EXIT);
};

struct CopyReq
{
    UInt8 type;
    UInt8 id;
    MESSAGE_DEF2(REQ::COPY_DATA, UInt8, type, UInt8, id);
};

struct NpcTriggerReq
{
	UInt32 m_NpcId;

	MESSAGE_DEF1(REQ::DIALOG_START, UInt32, m_NpcId);
};

struct TaskNpcActionReq
{
	UInt32	m_NpcId;
	UInt8	m_ActionType;
	UInt32	m_ActionID;		//taskId
	UInt8	m_ActionStep;

	TaskNpcActionReq() : m_NpcId(0), m_ActionType(0), m_ActionID(0), m_ActionStep(0) {};

	MESSAGE_DEF4(REQ::NPC_INTERACT, UInt32, m_NpcId, UInt8, m_ActionType, UInt32, m_ActionID, UInt8, m_ActionStep);
};

struct CollectNpcActionReq
{
	UInt32 m_NpcId;

	CollectNpcActionReq() : m_NpcId(0) {};

	MESSAGE_DEF1(REQ::COLLECTNPCACTION, UInt32, m_NpcId);
};

struct PlayerGetTaskReq
{
	MESSAGE_DEF(REQ::CURR_TASK_LIST);
};

struct CanAcceptTaskReq
{
	MESSAGE_DEF(REQ::CURR_AVAILABLE_TASK);
};

struct CanAcceptTaskToken
{
	//UInt8  m_Token;		//0：表示删???1: 表示添加
	UInt32 m_TaskId;    //更新的任务ID
};
S11N_TRAITS_1(CanAcceptTaskToken, UInt32, m_TaskId);

struct TaskActionReq
{
	UInt32	m_TaskId;	//
	UInt8	m_Action;	// 0:任务接受,1:任务提交,2:任务放弃,3:付费提交
	UInt32  m_ItemId;	// 奖励物品ID
	UInt16  m_ItemNum;	// 奖励物品的数???

	TaskActionReq() : m_TaskId(0), m_Action(0), m_ItemId(0), m_ItemNum(0) {};

	MESSAGE_DEF4(REQ::TASK_ACTION, UInt32, m_TaskId, UInt8, m_Action, UInt32, m_ItemId, UInt16, m_ItemNum);
};

struct StoreBuyReq
{
	UInt8 _type;
	UInt16 _itemId;
	UInt16 _count;
	MESSAGE_DEF3(REQ::STORE_BUY, UInt8, _type, UInt16, _itemId, UInt16, _count);
};

struct ChatReq
{
	UInt8 _type;
	std::string _text;
	MESSAGE_DEF2(REQ::CHAT, UInt8, _type, std::string, _text);
};

struct PrivChatReq
{
	UInt64 _id;
	std::string _text;
	MESSAGE_DEF2(REQ::WHISPER, UInt64, _id, std::string, _text);
};

struct ChatItemReq
{
	UInt8 _type;
	UInt64 _playerId;
	UInt32 _id;
	MESSAGE_DEF3(REQ::FLAUNT, UInt8, _type, UInt64, _playerId, UInt32, _id);
};

struct QueryPackageItemReq
{
	MESSAGE_DEF(REQ::PACK_INFO);
};

void OnQueryPackageItemReq( GameMsgHdr& hdr, QueryPackageItemReq& req )
{
	MSG_QUERY_PLAYER(pl);

	pl->GetPackage()->SendPackageItemInfor();
}

struct UseItemReq
{
	UInt32 m_ItemId;
	UInt8  m_ItemBindType;
	UInt16 m_ItemNum;
	UInt32 m_Param;
    UInt8  m_Type;
	MESSAGE_DEF5(REQ::PACK_USE, UInt32, m_ItemId, UInt8, m_ItemBindType, UInt16, m_ItemNum, UInt32, m_Param, UInt8, m_Type);
};

struct UseItemOtherReq
{
	UInt32 m_ItemId;
	UInt8  m_ItemBindType;
	UInt16 m_ItemNum;
    std::string m_OtherName;
	MESSAGE_DEF4(REQ::PACK_USE_OTHER, UInt32, m_ItemId, UInt8, m_ItemBindType, UInt16, m_ItemNum, std::string, m_OtherName);
};

struct BugInfoReq
{
  std::string _title;
  std::string _content;
  MESSAGE_DEF2(REQ::BUG,std::string, _title,std::string,_content);
};

struct YellowDiamondInfoReq
{
    MESSAGE_DEF(REQ::YD_INFO);
};

struct YellowDiamondAwardRcvReq
{
    UInt8 m_type;
    UInt8 m_d3d6;
    MESSAGE_DEF2(REQ::YD_AWARD_RCV, UInt8, m_type, UInt8, m_d3d6);
};

struct YellowDiamondGetPacksReq
{
    std::string key;
    MESSAGE_DEF1(REQ::YD_GETPACKS, std::string, key);
};

void OnUseItemReq( GameMsgHdr& hdr, UseItemReq& req )
{
	MSG_QUERY_PLAYER(pl);
	if (GetItemSubClass(req.m_ItemId) == Item_Task)
		pl->GetPackage()->UseTaskItem(req.m_ItemId, req.m_ItemBindType);
	else
	{
		pl->GetPackage()->UseItem(req.m_ItemId, req.m_ItemNum, req.m_Type, req.m_Param, req.m_ItemBindType);
	}
}

void OnUseItemOtherReq( GameMsgHdr& hdr, UseItemOtherReq& req )
{
	MSG_QUERY_PLAYER(pl);
    pl->GetPackage()->UseItemOther(req.m_ItemId, req.m_ItemNum, req.m_OtherName, req.m_ItemBindType);
}

struct ExtendPackageReq
{
	MESSAGE_DEF(REQ::PACK_EXTEND);
};

struct MailClickReq
{
	UInt32 _mailId;
	UInt8 _action;
	MESSAGE_DEF2(REQ::MAIL_CHANGE, UInt32, _mailId, UInt8, _action);
};

struct MailListReq
{
	UInt8	_start;
	UInt8	_count;
	MESSAGE_DEF2(REQ::MAIL_ID_LIST, UInt8, _start, UInt8, _count);
};

struct MailReq
{
	UInt32	_id;
	MESSAGE_DEF1(REQ::MAIL_CONTENT, UInt32, _id);
};

struct MailSendReq
{
	std::string	_target;
	std::string	_title;
	std::string	_content;
	MESSAGE_DEF3(REQ::MAIL_SEND, std::string, _target, std::string, _title, std::string, _content);
};

struct FriendListReq
{
	UInt8 _type;
	UInt8 _start;
	UInt8 _count;
	MESSAGE_DEF3(REQ::FRIEND_LIST, UInt8, _type, UInt8, _start, UInt8, _count);
};

struct FriendOpReq
{
	UInt8 _op;
	std::string _name;
	MESSAGE_DEF2(REQ::FRIEND_ACTION, UInt8, _op, std::string, _name);
};


#if 0
struct FriendActReq
{
	MESSAGE_DEF(REQ::FRIEND_STATUS);
};
#endif

void OnExtendPackageReq( GameMsgHdr& hdr, ExtendPackageReq& )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;
	pl->ExtendPackageSize();
}

#if 0
struct AthleticsDataReq
{
	MESSAGE_DEF(REQ::ARENA_FIGHT_INFO);
};
#endif
#if 0
struct FighterTrain2Req
{
	UInt32 _heroID;
	UInt8  _priceType;
	UInt32 _time;
	MESSAGE_DEF3(0x3F, UInt32, _heroID, UInt8, _priceType, UInt32, _time);
};
#endif

struct PracticeHookAddReq
{
    MESSAGE_DEF(REQ::PRACTICE_HOOK_ADD);
};

struct AthleticsRefreshMartialReq
{
    UInt8 _type;
	MESSAGE_DEF1(REQ::ATHLETICS_REFRESH_MARTIAL, UInt8, _type);
};

struct TrumpLOrderReq
{
    UInt16 _fgtId;
    UInt32 _itemId;
    MESSAGE_DEF2(REQ::EQ_TRUMP_L_ORDER, UInt16, _fgtId, UInt32, _itemId);
};
struct EquipUpgradeReq
{
    UInt16 _fgtId;
    UInt32 _itemId;
    MESSAGE_DEF2(REQ::EQ_UPGRADE, UInt16, _fgtId, UInt32, _itemId);

};

struct EquipSpiritReq
{
    UInt8 _type;
    UInt16 _fgtId;
    UInt32 _itemId;
    MESSAGE_DEF3(REQ::EQ_SPIRIT, UInt8, _type, UInt16, _fgtId, UInt32, _itemId);
};

struct GetHeroMemoAward
{
    UInt8 _idx;
    MESSAGE_DEF1(REQ::HEROMEMO, UInt8, _idx);
};

struct GetShuoShuoAward
{
    UInt8 _idx;
    MESSAGE_DEF1(REQ::SSAWARD, UInt8, _idx);
};

struct GetCFriendAward
{
    UInt8 _flag;
    UInt8 _idx;
    MESSAGE_DEF2(REQ::CFRIEND, UInt8, _flag, UInt8, _idx);
};

struct GetOfflineExp
{
    MESSAGE_DEF(REQ::OFFLINEEXP);
};

struct UseToken
{
    UInt8 _type;
    MESSAGE_DEF1(REQ::TOKEN, UInt8, _type);
};

struct UseMDSoul
{
    UInt8 _type;
    MESSAGE_DEF1(REQ::USESOUL, UInt8, _type);
};

struct SvrSt
{
    UInt8 _type;
    MESSAGE_DEF1(REQ::SVRST, UInt8, _type);
};

struct YBBuf
{
    UInt8 _type;
    MESSAGE_DEF1(REQ::YBBUF, UInt8, _type);
};

struct GetAward
{
    UInt8 _type;
    UInt8 _opt;
    MESSAGE_DEF2(REQ::GETAWARD, UInt8, _type, UInt8, _opt);
};

struct GuideUdp
{
    UInt8 _type;
    std::string p1;
    std::string p2;
    MESSAGE_DEF3(REQ::GUIDEUDP, UInt8, _type, std::string, p1, std::string, p2);
};


void OnSellItemReq( GameMsgHdr& hdr, const void * buffer)
{
	UInt16 bodyLen = hdr.msgHdr.bodyLen;
	if (bodyLen < 2) return ;
	const UInt8 * data = reinterpret_cast<const UInt8*>(buffer);
	UInt16 itemCount = *reinterpret_cast<const UInt16*>(data);
	UInt16 totalSize = 2 + itemCount * (4 + 1 + 2);
	if (bodyLen < totalSize) return ;

	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;

	UInt16 offset = 2;
	UInt32 price = 0;
    UInt16 canDestroyNum = 0;
	for (UInt16 i = 0; i < itemCount; ++i)
	{
		UInt32 itemId = *reinterpret_cast<const UInt32*>(data+offset);
		bool  bindType = *reinterpret_cast<const bool*>(data+offset+4);
		UInt16 itemNum = *reinterpret_cast<const UInt16*>(data+offset+5);
		offset += 7;
		if (IsEquipId(itemId))
		{
			price += pl->GetPackage()->SellEquip(itemId);
		}
		else
		{
			price += pl->GetPackage()->SellItem(itemId, itemNum, bindType);
		}
        if(World::canDestory(itemId))
            ++canDestroyNum;
	}
	if(price > 0)
	{
        pl->getTael(price);
	}
    if(canDestroyNum > 0)
    {
		SYSMSG_SEND(116, pl);
		SYSMSG_SEND(1016, pl);
    }
    else
    {
		SYSMSG_SEND(172, pl);
		SYSMSG_SEND(1072, pl);
    }
}


void OnDestroyItemReq( GameMsgHdr& hdr, const void * buffer )
{
	UInt16 bodyLen = hdr.msgHdr.bodyLen;
	if (bodyLen < 2) return ;
	const UInt8 * data = reinterpret_cast<const UInt8*>(buffer);
	UInt16 itemCount = *reinterpret_cast<const UInt16*>(data);
	UInt16 totalSize = 2 + itemCount * (4 + 1 + 2);
	if (bodyLen < totalSize) return ;

	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;

	UInt16 offset = 2;
    UInt16 canDestroyNum = 0;
	for (UInt16 i = 0; i < itemCount; ++i)
	{
		UInt32 itemId = *reinterpret_cast<const UInt32*>(data+offset);
		bool  bindType = *reinterpret_cast<const bool*>(data+offset+4);
		UInt16 itemNum = *reinterpret_cast<const UInt16*>(data+offset+4+1);
		offset += 7;
        pl->addItem(itemId, itemNum, bindType);
        if(World::canDestory(itemId))
            ++canDestroyNum;
	}
    if(canDestroyNum > 0)
    {
        SYSMSG_SEND(115, pl);
        SYSMSG_SEND(1015, pl);
    }
    else
    {
        SYSMSG_SEND(171, pl);
        SYSMSG_SEND(1071, pl);
    }
}

void OnTripodReq( GameMsgHdr& hdr, const void* data )
{
 	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0;
    br >> type;
    if (type == 0)
    {
        player->sendTripodInfo();
    }
    else if (type == 1)
    {
        if (!player->hasChecked())
            return;
        UInt8 num = 0;
        UInt32 id1 = 0;
        UInt32 id2 = 0;
        br >> num;
        if (num < 2)
            return;
        br >> id1;
        br >> id2;

        player->makeFire(id1, id2);
    }
    else if (type == 2)
    {
        player->getAward();
    }
}

void OnFlushTaskColorReq( GameMsgHdr& hdr, const void* data)
{
 	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 cyc = 0;
	UInt8 type = 0;
    br >> cyc;
	br >> type;
	UInt8 color = 0;
	UInt16 count = 1;
	switch(type)
	{
	case 2:
		if(!player->hasChecked())
			return;
		count = 1;
		break;
	case 3:
		if(!player->hasChecked())
			return;
		br >> color >> count;
		break;
	}

	player->flushTaskColor(cyc, type, color, count);
}

struct DayTaskAutoCompletedReq
{
	UInt32 m_DayTaskId;
	MESSAGE_DEF1(REQ::DAYTASKAUTOCOMPLETED, UInt32, m_DayTaskId);
};

void OnDayTaskAutoCompletedReq( GameMsgHdr& hdr, DayTaskAutoCompletedReq& req )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;

	TaskMgr* taskMgr = pl->GetTaskMgr();
	taskMgr->AddAutoTask(req.m_DayTaskId);
}

struct QueryAutoCompletedTaskTimeReq
{
	UInt32 m_TaskId;
	MESSAGE_DEF1(REQ::AUTO_COMPLETED_TASK, UInt32, m_TaskId);
};

void OnQueryAutoCompletedTaskTimeReq( GameMsgHdr& hdr, QueryAutoCompletedTaskTimeReq& req )
{
	MSG_QUERY_PLAYER(pl);

	TaskMgr* taskMgr = pl->GetTaskMgr();
	Stream st(REP::AUTO_COMPLETED_TASK);
	st << req.m_TaskId << taskMgr->GetAutoTaskLeftTime(req.m_TaskId) << Stream::eos;
	pl->send(st);
}

struct QueryDayTaskCompletedCountReq
{
	UInt32 m_DayTaskId;
	MESSAGE_DEF1(REQ::QUERYDAYTASK, UInt32, m_DayTaskId);
};

void OnQueryDayTaskCompletedCountReq( GameMsgHdr& hdr, QueryDayTaskCompletedCountReq& req )
{
	MSG_QUERY_PLAYER(pl);

	DayTaskData* taskData = pl->GetTaskMgr()->GetDayTaskData(req.m_DayTaskId);
	if (taskData == NULL) return ;
	Stream st(REP::QUERYDAYTASK);
	st << req.m_DayTaskId << taskData->m_PreTaskQuality << static_cast<UInt16>(taskData->m_Count) << static_cast<UInt16>(taskData->m_MaxCount) << Stream::eos;
	pl->send(st);
}

struct ConveyBattleReq
{
	UInt32 m_TaskId;	//任务ID
	MESSAGE_DEF1(REQ::CONVEYBATTLE, UInt32, m_TaskId);
};

void OnConveyBattleReq( GameMsgHdr& hdr, ConveyBattleReq& req )
{
	MSG_QUERY_PLAYER(pl);

	if (req.m_TaskId == 0)
		return ;

	TaskMgr* taskMgr = pl->GetTaskMgr();
	if (!taskMgr->IsInConvey())
		return ;

	if (req.m_TaskId != pl->GetTaskMgr()->GetConveyTask())
		return ;

	//检查此护送任务是否已???
	if (!taskMgr->HasAcceptedTask(req.m_TaskId))
		return ;

	UInt16 spot = pl->getLocation();
	UInt32 monster = GameAction()->GetConveyMonster(pl, req.m_TaskId, spot);
	if (monster == 0) return ;

	bool ret = pl->attackNpc(monster, 0xFFFFFFFF, pl->GetLev() <= 20);
	GameAction()->UpdateConveyStep(pl, req.m_TaskId, spot, ret);
}

void OnNullReq( GameMsgHdr& hdr, NullReq& nr )
{
	MSG_QUERY_PLAYER(player);
	Stream st(REP::KEEP_ALIVE);
	st << nr.ticket << Stream::eos;
	player->send(st);
}

void OnAttainReq( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 op = 0;
	br >> op;

    switch( op )
    {
    case 0:
        player->GetAttainMgr()->sendAttainment();
        break;
    case 1:
        UInt16 attainId = 0;
        br >> attainId;
        GameAction()->finishAttainment(player, attainId);
        break;
    }
}

void OnSelectCountry( GameMsgHdr& hdr, SelectCountry& req )
{
	MSG_QUERY_PLAYER(player);
    UInt8 country = req._country;
    if (country > 2)
        return;
    if (player->getCountry() != country)
    {   //before leave thread
        player->OnSelectCountry();
        CURRENT_COUNTRY().PlayerLeave(player);
        player->setCountry(country);
        Stream st(REP::CAMP_SELECT);
        st << country << Stream::eos;
        player->send(st);

        PlayerData& pd = player->getPlayerData();
        CountryEnterStruct ces(true, pd.inCity ? 1 : 0, pd.location);
        GameMsgHdr hdr(0x1F0, player->getThreadId(), player, sizeof(CountryEnterStruct));
        GLOBAL().PushMsg( hdr, &ces );
    }
}

void OnNewGuildReq( GameMsgHdr& hdr, NewGuildReq& req)
{
    MSG_QUERY_CONN_PLAYER(conn, pl);
    pl->setNewGuildTaskStep(req._step);
}

void OnPlayerInfoReq( GameMsgHdr& hdr, PlayerInfoReq& )
{
	MSG_QUERY_CONN_PLAYER(conn, pl);

	pl->setOnline(true);

	GObject::Map * map = GObject::Map::FromSpot(PLAYER_DATA(pl, location));
	if(map == NULL)
		return;
	{
		GObject::Clan *clan = pl->getClan();
		if(clan != NULL)
			clan->broadcastMemberInfo(pl);
	}
	{
		Stream st;
		pl->makePlayerInfo(st);
		conn->send(&st[0], st.size());
	}
    {
        if(!pl->GetVar(VAR_AWARD_NEWREGISTER) && pl->GetLev() == 1)
            pl->sendNewRegisterAward(0);  //0:表示新用户注册还可以邀请好友进行抽奖
        pl->CheckCanAwardBirthday(); //生日罗盘许愿星(周年庆活动)
        pl->getAwardLogin(2); // 2012/10/14登录抽奖合作活动
        pl->getThanksGivingDay(2); //感恩节活动,qq大厅登录礼包
    }
	{
		Stream st;
		pl->makeFighterList(st);
		conn->send(&st[0], st.size());
	}
    {
        Stream st;
        pl->makeFighterSSList(st);
		conn->send(&st[0], st.size());
    }
	{
		Stream st;
		pl->makeFormationInfo(st);
		conn->send(&st[0], st.size());
	}
	{
		if(PLAYER_DATA(pl, inCity))
			map->SendCityNPCs(pl);
		map->SendAtCity(pl, PLAYER_DATA(pl, inCity) == 1);
        globalCountryBattle.sendForNewCB(pl);
	}
	pl->GetMailBox()->notifyNewMail();
	UInt8 level = pl->GetLev();
	pl->sendDailyInfo();
    ClanRankBattleMgr::Instance().sendDaily(pl);
	{
		Stream st;
		pl->makeSenconPWDInfo(st);
		conn->send(&st[0], st.size());
	}
	if (level >= 30)
	{
		GameMsgHdr hdr(0x19F, WORKER_THREAD_WORLD, pl, 0);
		GLOBAL().PushMsg(hdr, NULL);
	}
	{
		Stream st;
		World::makeActivityInfo(st);
		conn->send(&st[0], st.size());
	}
    {
        pl->sendNewGuild();
    }
    {
        pl->sendAutoCopy();
    }
    {
        pl->sendAutoFrontMap();
    }
    {
        pl->sendSecondInfo();
    }
    {
        TeamCopyPlayerInfo* tcp = pl->getTeamCopyPlayerInfo();
        if (tcp && tcp->getPass(4) && (pl->GetVar(VAR_EX_JOB_ENABLE) == 0))
        {
            pl->SetVar(VAR_EX_JOB_ENABLE, 1);
        }
        Stream st(REP::EXJOB);
        st << static_cast<UInt8>(0);
        st << static_cast<UInt8>(pl->GetVar(VAR_EX_JOB_ENABLE));
        st << Stream::eos;
        pl->send(st);
        pl->sendAutoJobHunter();       // 为什么上面的语句都加大括号？？？
    }
#ifdef _FB
    // XXX: do not need
#else
    pl->sendWallow();
#endif
    pl->sendEvents();
    //pl->GetPackage()->SendPackageItemInfor();
    {
        TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
        tcpInfo->sendAwardInfo();
    }
    pl->sendDeamonAwardsInfo();

    UInt32 now = TimeUtil::Now();
    pl->GetHeroMemo()->sendHeroMemoInfo();
    pl->GetShuoShuo()->sendShuoShuo();
    pl->GetCFriend()->sendCFriend();
    pl->GetStrengthenMgr()->CheckTimeOver(now);
    pl->sendRechargeInfo();
    pl->sendConsumeInfo();
    pl->sendRechargeNextRetInfo(now);
    pl->sendRC7DayInfo(now);
    pl->sendRF7DayInfo(now);
    pl->sendMDSoul(0);
    pl->sendSSDTInfo();
    pl->send11DailyInfo();
    pl->sendHappyInfo();
    pl->sendYBBufInfo(pl->GetVar(VAR_YBBUF), pl->GetVar(VAR_QQVIP_BUF));
    pl->sendAthlBufInfo();
    pl->sendConsumeAwardInfo(0);
    luckyDraw.notifyDisplay(pl);
    if (World::getRechargeActive())
    {
        GObject::RechargeTmpl::instance().sendStreamInfo(pl);
        GObject::RechargeTmpl::instance().sendScoreInfo(pl);
    }
    pl->sendSSToolbarInfo();

    if (World::getTrumpEnchRet() || World::get9215Act())
        pl->sendTokenInfo();

    if(World::getFourCopAct())
        pl->sendFourCopAct();

    {
        UInt32 exp = pl->GetVar(VAR_OFFLINE_EXP);
        if (exp)
        {
            Stream st(REP::OFFLINEEXP);
            st << pl->GetVar(VAR_OFFLINE_EXP) << static_cast<UInt32>(pl->GetVar(VAR_OFFLINE_PEXP)*pl->getMainFighter()->getPracticeInc()*0.8f);
            UInt32 equip = pl->GetVar(VAR_OFFLINE_EQUIP);
            if(equip)
            {
                UInt32 dayCnt = equip / (24 * 3600);
                UInt8 lvl = pl->GetLev();
                GData::LootResult lr;

                pl->_equipAward.clear();
                if(dayCnt > 365)
                    dayCnt = 365;
                if(dayCnt >= 7)
                {
                    lr.id = getRandOEquip(lvl);
                    lr.count = 1;
                    pl->_equipAward.push_back(lr);
                }
                if(dayCnt >= 14)
                {
                    lr.id = getRandOEquip(lvl);
                    lr.count = 1;
                    pl->_equipAward.push_back(lr);
                    lr.id = GameAction()->getRandTrump(lvl);
                    lr.count = 1;
                    pl->_equipAward.push_back(lr);
                }
                if(dayCnt >= 21)
                {
                    lr.id = getRandOEquip(lvl);
                    lr.count = 1;
                    pl->_equipAward.push_back(lr);
                    lr.id = GameAction()->getRandTrump(lvl);
                    lr.count = 1;
                    pl->_equipAward.push_back(lr);
                }
                if(dayCnt >= 30)
                {
                    int times = dayCnt / 30;
                    while(times--)
                    {
                        lr.id = getRandOEquip(lvl);
                        lr.count = 1;
                        pl->_equipAward.push_back(lr);
                        lr.id = GameAction()->getRandTrump(lvl);
                        lr.count = 1;
                        pl->_equipAward.push_back(lr);
                    }
                }
                st << static_cast<UInt8>(pl->_equipAward.size());
                for(UInt16 i = 0; i < pl->_equipAward.size(); i++)
                {
                    st << pl->_equipAward[i].id;
                }
            }

            st<< Stream::eos;
            pl->send(st);
        }
    }

    if (pl->getSysDailog())
    {
        Stream st(REP::SYSDAILOG);
        st << Stream::eos;
        pl->send(st);
        pl->setSysDailog(false);
    }

    {
        worldBoss.sendBossInfo(pl);
    }

    {
        GObject::arena.sendActive(pl);
        GObject::shStageMgr.sendActive(pl);
    }

    {
        pl->sendSoSoMapInfo();
    }
    {
        GObject::Tianjie::instance().getTianjieData(pl, true);
        if (World::getConsume918())
        {
            Stream st(REP::DAILY_DATA);
            st << static_cast<UInt8>(17) << pl->GetVar(VAR_CONSUME_918) << Stream::eos;
            pl->send((st));
        }
        pl->sendSysUpdate();
    }
    //if (World::getNeedRechargeRank() || time(NULL) <= World::getRechargeEnd() + 24*60*60)
    if (World::getNeedRechargeRank())
    {
        GameMsgHdr hdr(0x1C3, WORKER_THREAD_WORLD, pl, 0);
        GLOBAL().PushMsg(hdr, NULL);
    }
    //if (World::getNeedConsumeRank() || time(NULL) <= World::getConsumeEnd() + 24*60*60)
    if (World::getNeedConsumeRank())
    {
        GameMsgHdr hdr(0x1C4, WORKER_THREAD_WORLD, pl, 0);
        GLOBAL().PushMsg(hdr, NULL);
    }
    pl->sendYearRPInfo();
    //if(World::getYearActive())
    //    pl->sendYearActInfo();
    pl->sendFirstRecharge(true);
    pl->sendCopyFrontAllAward();
    pl->sendGoodVoiceInfo();
    pl->send3366GiftInfo();
    pl->sendFeastGiftAct();
}

void OnPlayerInfoChangeReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 field = 0;
	br >> field;
	switch (field)
	{
        case 0x06:
            {
                UInt32 id;
                br >> id;
                player->changeTitle(static_cast<UInt8>(id));
            }
            break;
        case 0x09:
            {
                UInt32 mounts;
                br >> mounts;
                player->setMounts(static_cast<UInt8>(mounts));
            }
            break;
        case 0x10:
            {
                UInt32 step;
                br >> step;
                player->setNewGuildTaskStep(step);
            }
            break;
        default:
            return;
	}
}

void OnBookStoreListReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0;
	br >> type;
    if (type && !player->hasChecked())
        return;
    player->listBookStore(type);
}

void OnPurchaseBookReq( GameMsgHdr& hdr, PurchaseBookReq& pbr )
{
	MSG_QUERY_PLAYER(player);
    if (!player->hasChecked())
        return;
	if(pbr._pos >= 6)
		return;
	UInt32 id = player->purchaseBook(pbr._pos);
	Stream st(REP::BOOK_SHOP_BUY);
	st << static_cast<UInt8>(id > 0 ? 1 : 0) << pbr._pos << Stream::eos;
	player->send(st);
}

void OnStatusChangeReq( GameMsgHdr& hdr, StatusChangeReq& scr )
{
#if 0
	if(scr._id < 1 || scr._id > 8)
		return;
	MSG_QUERY_PLAYER(player);
	UInt8 v = 1 << (scr._id - 1);
	if(v == Player::SGPunish)
	{
		if(cfg.GMCheck)
		{
			player->addStatus(Player::SGPunish);
			player->testBattlePunish();
		}
		return;
	}
	if(player->hasStatus(v))
	{
		if(v == 1)
		{
			if(!player->canClosePK())
			{
				player->sendMsgCode(0, 2001, player->getBuffData(PLAYER_BUFF_PKLOCK) - TimeUtil::Now());
				return;
			}
			//SYSMSG_SEND(146, player);
			//SYSMSG_SEND(1046, player);
			GObject::Map * map = GObject::Map::FromSpot(PLAYER_DATA(player, location));
			if(map != NULL)
				map->changebyStatus(player);
		}
		player->removeStatus(v);
	}
	else
	{
		if(v == 1)
		{
			player->setBuffData(PLAYER_BUFF_PKLOCK, TimeUtil::Now() + 20 * 60);
			//SYSMSG_SEND(145, player);
			//SYSMSG_SEND(1045, player);
			GObject::Map * map = GObject::Map::FromSpot(PLAYER_DATA(player, location));
			if(map != NULL)
				map->changebyStatus(player);
		}
		player->addStatus(v);
	}
	GObject::Map::NotifyPlayerEnter(player);
#endif
}

void OnFormationReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0;
	br >> type;

    switch(type)
    {
    case 0:
        player->sendFormationList();
        break;
    case 1:
        {
            UInt16 formationId = 0;
            br >> formationId;
            player->formationLevUp(formationId);
        }
        break;
    }
}

void OnSetFormationReq( GameMsgHdr& hdr, const void * buffer )
{
	UInt32 blen = hdr.msgHdr.bodyLen;
	if(blen < 2)
		return;
	const UInt8 * buf = reinterpret_cast<const UInt8 *>(buffer);
	UInt16 f = *reinterpret_cast<const UInt16 *>(buf);
	UInt8 c = buf[2];
	if(c > 5 || blen < 3 + (sizeof(UInt8) + sizeof(UInt32)) * c)
		return;
	MSG_QUERY_PLAYER(player);

    if(!player->checkFormation(f))
        return;

	for(UInt8 i = 0; i < c; ++ i)
	{
		UInt32 pos = 3 + (sizeof(UInt8) + sizeof(UInt32)) * i;
		UInt8 p = *(buf + pos + sizeof(UInt32));

        bool find = false;
        for(UInt8 k = 0; k < 5; ++ k)
        {
            if((*GData::formationManager[f])[k].pos == p)
            {
                find = true;
                break;
            }
        }
        if(!find)
            return;
    }

	for(UInt8 i = 0; i < c; ++ i)
	{
		UInt32 pos = 3 + (sizeof(UInt8) + sizeof(UInt32)) * i;
		UInt32 fgtid = *reinterpret_cast<const UInt32 *>(buf + pos);
		if (player->hasTrainFighter(fgtid))
			continue;
		UInt8 p = *(buf + pos + sizeof(UInt32));
		bool dup = false;
		for(UInt8 j = 0; j < i; ++ j)
		{
			Lineup& pdata2 = player->getLineup(j);
			if(pdata2.fid == fgtid || pdata2.pos == p)
			{
				Lineup& pdata = player->getLineup(i);
				pdata.fid = 0;
				pdata.pos = 0;
				dup = true;
				break;
			}
		}
		if(dup) continue;
		Lineup& pdata = player->getLineup(i);
		pdata.fid = fgtid;
		pdata.pos = p;
	}
	for(UInt8 i = c; i < 5; ++ i)
	{
		Lineup& pdata = player->getLineup(i);
		pdata.fid = 0;
		pdata.pos = 0;
	}

    SYSMSG_SENDV(2102, player);

	player->updateBattleFighters();
    player->setFormation(f);

	Stream st;
	player->makeFormationInfo(st);
	player->send(st);

    player->OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 1, 1);
    if (c == 5)
        player->OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 1, 0);
    if (f % 100 > 1)
        player->OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 1, 2);
}

void OnFighterInfoReq( GameMsgHdr& hdr, const void * data )
{
	if(hdr.msgHdr.bodyLen < 1)
		return;
	const UInt8 * buf = static_cast<const UInt8 *>(data);
	UInt8 c = buf[0];
	if(hdr.msgHdr.bodyLen < 1 + sizeof(UInt32) * c)
		return;
	const UInt32 * idlist = reinterpret_cast<const UInt32 *>(buf + 1);
	MSG_QUERY_PLAYER(player);
	Stream st(REP::FIGHTER_INFO);
	st << static_cast<UInt8>(0);
	UInt8 cnt = 0;
	for(UInt8 i = 0; i < c; ++ i)
	{
		if(player->makeFighterInfo(st, idlist[i]))
			++ cnt;
	}
	st.data<UInt8>(4) = cnt;
	st << Stream::eos;
	player->send(st);
}

struct FighterLeaveStruct
{
	UInt8 _result;
	UInt32 _fgtid;
	MESSAGE_DEF2(REP::FIGHTER_DISMISS, UInt8, _result, UInt32, _fgtid);
};

void OnFighterEquipReq( GameMsgHdr& hdr, FighterEquipReq& fer )
{
	MSG_QUERY_PLAYER(player);
	GObject::Fighter * fgt = player->findFighter(fer._id);
	if(fgt == NULL)
		return;
	if(fer._part == 0)
	{
		static UInt8 p[16] = {0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x0a, 0x0b, 0x0c, 0x60, 0x61, 0x62};
		ItemEquip * e[16] = {fgt->getHalo(), fgt->getFashion(), fgt->getWeapon(), fgt->getArmor(0), fgt->getArmor(1),
            fgt->getArmor(2), fgt->getArmor(3), fgt->getArmor(4), fgt->getAmulet(),
            fgt->getRing(), fgt->getTrump(0), fgt->getTrump(1), fgt->getTrump(2),
            fgt->getLingbao(0), fgt->getLingbao(1), fgt->getLingbao(2)};
		fgt->sendModification(16, p, e, false);
		return;
	}

    if(!player->hasChecked())
        return;

    int idx;
    switch(fer._part)
    {
    case 0x29:
        {
            idx = (fer._equipId >> 16) & 0xFFFF;
            UInt8 v = fer._equipId & 0xFFFF;
            fgt->setAcupoints(idx, v, true, false);
        }
        break;
    case 0x30:
        fgt->setPeerless(static_cast<UInt16>(fer._equipId), true);
        break;
    case 0x33:
        fgt->setHideFashion(static_cast<UInt8>(fer._equipId));
        break;
    case 0x2a:
        {
            UInt16 skill = (fer._equipId >> 16) & 0xFFFF;
            idx = fer._equipId & 0xFFFF;
            fgt->upSkill(skill, idx, true);
        }
        break;
    case 0x2b:
        {
            return; // XXX: 取消卸载技能功能
            UInt16 skill = (fer._equipId >> 16) & 0xFFFF;
            idx = fer._equipId & 0xFFFF;
            fgt->offSkill(skill, true);
        }
        break;
    case 0x2c:
        {
            UInt16 citta = (fer._equipId >> 16) & 0xFFFF;
            idx = fer._equipId & 0xFFFF;
            fgt->upCitta(citta, idx, true);
        }
        break;
    case 0x2d:
        {
            UInt16 citta = (fer._equipId >> 16) & 0xFFFF;
            idx = fer._equipId & 0xFFFF;
            fgt->offCitta(citta, true, true);
        }
        break;
    case 0x2e:
        {
            UInt16 citta = (fer._equipId >> 16) & 0xFFFF;
            idx = fer._equipId & 0xFFFF;
            if (idx == 1)
                fgt->lvlUpCitta(citta, true);
            else if (idx == 2)
                fgt->delCitta(citta, true);
        }
        break;
    default:
        {
            ItemEquip * equip;
            player->GetPackage()->EquipTo(fer._equipId, fgt, fer._part, equip);
        }
        break;
    }
}

void OnRecruitListReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0;
	br >> type;
	UInt8 color = 5;
	UInt16 count = 0;
	switch(type)
	{
	case 1:
		if(!player->hasChecked())
			return;
		count = 1;
		break;
	case 2:
		if(!player->hasChecked())
			return;
		br >> color >> count;
		break;
	}
	player->listRecruit(type, color, count);
}

void OnRecruitFighterReq( GameMsgHdr& hdr, RecruitFighterReq& rfr )
{
	MSG_QUERY_PLAYER(player);
	if(rfr._pos >= 6)
		return;
	UInt32 id = player->hireRecruit(rfr._pos);
	Stream st(REP::HOTEL_PUB_HIRE);
	st << static_cast<UInt8>(id > 0 ? 0 : 1) << rfr._pos << Stream::eos;
	player->send(st);
	if (id != 0)
    {   //将新招募的散仙放入修炼位
        UInt32 fgts[1] = { id };
        GObject::practicePlace.sitdown(player, fgts, 1);
    }
    GameAction()->RunOperationTaskAction0(player, 3);
}

void OnFighterDismissReq( GameMsgHdr& hdr, FighterDismissReq& fdr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	FighterLeaveStruct rep;
	rep._fgtid = fdr._fgtid;
	GObject::Fighter * fgt = player->removeFighter(fdr._fgtid);
	Stream st(REP::FIGHTER_DISMISS);
	if(fgt == NULL)
	{
		rep._result = 1;
		player->send(rep);
		return;
	}

    UInt64 exp = 0;

    if(fgt->getClass() == 4)
    {
        exp = fgt->getExp() > GData::expTable.getLevelMin(70) ? fgt->getExp() - GData::expTable.getLevelMin(70) : 0;
        exp /= 2;
    }
    else
        exp = fgt->getExp() / 2;

	if(exp >= 25000 || (fgt->getClass() == 4))
	{
		UInt16 rCount1 = static_cast<UInt16>(exp / 50000000);
		exp = exp % 50000000;
		UInt16 rCount2 = static_cast<UInt16>(exp / 500000);
		exp = exp % 500000;
		UInt16 rCount3 = static_cast<UInt16>(exp / 5000);
		
	    UInt64 pexp = fgt->getPExp() * 0.6;
        UInt16 rCount4 = static_cast<UInt16>(pexp / 1000000);
        pexp = pexp % 1000000;
		UInt16 rCount5 = static_cast<UInt16>(pexp / 10000);
        pexp = pexp % 10000;
		UInt16 rCount6 = static_cast<UInt16>(pexp / 100);
		SYSMSG(title, 236);
		SYSMSGV(content, 237, fgt->getLevel(), fgt->getColor(), fgt->getName().c_str());
		MailPackage::MailItem mitem[6] = {{14, rCount1}, {13, rCount2}, {12, rCount3}, {31, rCount4}, {30, rCount5}, {29, rCount6}};
		MailItemsInfo itemsInfo(mitem, DismissFighter, 6);
		GObject::Mail * pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
		if(pmail != NULL)
		{
			GObject::mailPackageManager.push(pmail->id, mitem, 6, true);
		}
	}
    fgt->delAllCitta();
	delete fgt;
	rep._fgtid = fdr._fgtid;
	rep._result = 0;
	player->send(rep);
    JobHunter* jobHunter = player->getJobHunter();
    if (jobHunter)
        jobHunter->AddToFighterList(fdr._fgtid);
}

void OnFighterRegenReq( GameMsgHdr& hdr, FighterRegenReq& frr )
{
	MSG_QUERY_PLAYER(player);
	if(player->hasFlag(Player::CountryBattle | Player::ClanBattling))
	{
		player->sendMsgCode(2, 1054);
		return;
	}
	GObject::Fighter * fgt = player->findFighter(frr._fgtid);
	if(fgt == NULL)
		return;
	player->GetPackage()->AutoRegen(fgt);
}

void OnFighterTrainReq( GameMsgHdr& hdr, FighterTrainReq& ftr )
{
	MSG_QUERY_PLAYER(player);
    if (ftr._type > 4 && (ftr._type&0xF) == 5)
    {   //天赋洗炼
        UInt8 result = player->trainFighter(ftr._fgtId, ftr._type);
        if(result != 2)
            GameAction()->doStrong(player, SthGenius, 0, 0);
        return;
    }

	Stream st(REP::POTENCIAL);
    UInt8 result = player->trainFighter(ftr._fgtId, ftr._type);
	st << ftr._type << result << Stream::eos;;
	player->send(st);
    if(result != 2)
    {
        if(1 == ftr._type || 2 == ftr._type) //资质洗炼
            GameAction()->doStrong(player, SthCapacity, 0, 0);
        if(3 == ftr._type || 4 == ftr._type) //潜力洗炼
            GameAction()->doStrong(player, SthPotential, 0, 0);
    }
}

void OnTakeOnlineRewardReq( GameMsgHdr& hdr, TakeOnlineRewardReq& req)
{
	MSG_QUERY_PLAYER(player);
    if (!req._flag) {
        if(!player->takeOnlineReward())
            player->sendMsgCode(1, 1011);
    }
	player->sendOnlineReward();
}

void OnLuckyDrawReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    br >> type;

    if (type == 0)
    {
        UInt8 id;
        UInt8 times;
        UInt8 bind;
        br >> id;
        br >> times;
        br >> bind;
        luckyDraw.draw(player, id, times, bind==1?true:false);
    }
    else if (type == 1)
    {
        luckyDraw.sendInfo(player);
    }
}

void OnCountryActReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 opt = 0;
    br >> opt;

    if(!player->hasChecked())
        return;
    switch(opt)
    {
        /** 周岁红包送不停 **/
        case 1:
        {
	        if(!World::getYearActive())
		        return;
            UInt8 opt2 = 0;
            br >> opt2;
            if(opt2 == 1)
            {
                UInt8 type = 0;
                br >> type;
                if(type == 1 || type == 2)
                    player->getYearActAward(type);
            }else if(opt2 == 2)
                player->sendYearActInfo();
        }
        break;
        case 2:
        {
            if(!World::getKillMonsterAct())
                return;
            UInt8 type = 0;
            br >> type;
            if(type == 0)
            {
                GameMsgHdr hdr(0x1FF, WORKER_THREAD_WORLD, player, 0);
                GLOBAL().PushMsg(hdr, NULL);
            }
            else if(type == 1)
                player->getKillMonsterAward();
            else if(type == 2)
                player->checkLastKillMonsterAward();
        }
        break;

        case 3:
        {
            UInt8 step;
            UInt8 type;
            UInt8 career;
            br >> step;
            br >> type;
            br >> career;
            player->FirstRechargeAct(step, type, career);
        }
        break;

        case 4:
        {
            UInt8 type;
            UInt8 copy_or_front;
            UInt8 index;

            if(!World::getCopyFrontWinSwitch())
                return;
            br >> type;
            br >> copy_or_front;
            br >> index;

            if(type == 0)
            {
                UInt8 indexPut;
                br >> indexPut;
                player->getCopyFrontAwardByIndex(copy_or_front, index, indexPut);
            }
            else if(type == 1)
                player->freshCopyFrontAwardByIndex(copy_or_front, index);
            else if(type == 2)
                player->closeCopyFrontAwardByIndex(copy_or_front, index);
        }

        case 5:
        {
            UInt8 type;
            if(!World::getGoodVoiceAct())
                return;
            br >> type;
            player->getGoodVoiceAward(type);
        }
        break;

        case 6:
        {
            UInt8 type;
            if(!World::get3366GiftAct())
                return;
            br >> type;
            player->get3366GiftAward(type);
        }
        break;

        case 7:
        {
            UInt8 type;
            if(!World::getFeastLoginAct())
                return;
            br >> type;
            player->getFeastGiftAward(type);
        }
        break;

        default:
        break;
    }
}

void OnEnchantReq( GameMsgHdr& hdr, EnchantReq& er )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::EQ_TO_STRONG);
    UInt16 success = 0;
    UInt16 failed = 0;
	st << player->GetPackage()->Enchant(er._fighterId, er._itemid, er._type, er._count, er._level, success, failed/*, er._protect > 0*/) << er._fighterId << er._itemid;

    if(er._count != 0)
        st << success << failed;

    st << Stream::eos;

	player->send(st);
    GameAction()->RunOperationTaskAction1(player, 1, 2);
}

void OnOpenSocketReq( GameMsgHdr& hdr, OpenSocketReq& osr )
{
	MSG_QUERY_PLAYER(player);
    if (!player->hasChecked())
        return;
	UInt8 result = player->GetPackage()->OpenSocket(osr._fighterId, osr._itemid);
	Stream st(REP::EQ_TO_PUNCH);
	st << result << osr._fighterId << osr._itemid << Stream::eos;
	player->send(st);
    if(result != 2)
        GameAction()->doStrong(player, SthOpenSocket, 0, 0);
}

#if 0
void OnMergeGemReq( GameMsgHdr& hdr, MergeGemReq& mgr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::MERGE_GEM);
	UInt32 ogid = 0;
	st << player->GetPackage()->MergeGem(mgr._itemid, mgr._bindNum, mgr._protect > 0, ogid) << static_cast<UInt16>(ogid) << Stream::eos;
	player->send(st);
}
#endif

void OnAttachGemReq( GameMsgHdr& hdr, AttachGemReq& agr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	UInt8 result = player->GetPackage()->AttachGem(agr._fighterId, agr._itemid, agr._gemid, agr._bind > 0);
	Stream st(REP::EQ_EMBED);
	st << player->GetPackage()->AttachGem(agr._fighterId, agr._itemid, agr._gemid, agr._bind > 0) << agr._fighterId << agr._itemid << Stream::eos;
	player->send(st);
    if(!result)
        GameAction()->doStrong(player, SthAttachGem, 0, 0);
}

void OnDetachGemReq( GameMsgHdr& hdr, DetachGemReq& dgr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	UInt8 result = player->GetPackage()->DetachGem(dgr._fighterId, dgr._itemid, dgr._pos, dgr._protect);
	Stream st(REP::EQ_UN_EMBED);
	st << result << dgr._fighterId << dgr._itemid << dgr._pos << Stream::eos;
	player->send(st);
    if(result != 2)
        GameAction()->doStrong(player, SthDetachGem, 0, 0);
}

#if 0
void OnSplitReq( GameMsgHdr& hdr, SplitReq& sr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::EQ_UN_SINGLE);
	UInt32 enchangId = 0;
	UInt8 count = 0;
	UInt8 r = player->GetPackage()->Split(sr._itemid, enchangId, count /*sr._protect > 0*/);
	st << r << static_cast<UInt16>(enchangId) << count << Stream::eos;
	player->send(st);
    GameAction()->RunOperationTaskAction1(player, 1, 1);
}
#endif

#if 0
void OnExchangeReq( GameMsgHdr& hdr, ExchangeReq& er )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::EQ_EXCHANGE);
	UInt32 rid = 0;
	UInt8 r = player->GetPackage()->Exchange(er._itemid, rid);
	st << r << rid << Stream::eos;
	player->send(st);
	//GameAction()->onExchange(player);
}
#endif

void OnForgeReq( GameMsgHdr& hdr, ForgeReq& fr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	UInt8 types[3]; Int16 values[3];
	UInt8 r = player->GetPackage()->Forge(fr._fighterId, fr._itemid, /*fr._type,*/ types, values, fr._protect);
	Stream st(REP::EQ_PURIFY);
	st << r << fr._fighterId << fr._itemid << Stream::eos;
	player->send(st);
}

#if 0
void OnExchangeSetReq( GameMsgHdr& hdr, ExchangeSetReq& esr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::EQ_EXCHANGE_POS);
	UInt32 rid = 0;
	UInt8 r = player->GetPackage()->ExchangeSet(esr._itemid, esr._flag, rid);
	st << r << rid << Stream::eos;
	player->send(st);
}
#endif

void OnBatchSplitReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 flag;
	UInt16 count;
	br >> flag >> count;

	Package * pkg = player->GetPackage();
    UInt32 amount = 0;
    std::vector<SplitItemOut> splitOut;

	for(UInt16 i = 0; i < count; ++ i)
	{
		amount += GData::moneyNeed[GData::SPLIT].tael;

		UInt32 itemId;
		br >> itemId;

		if(player->getTael() < amount)
		{
			player->sendMsgCode(0, 1100);
            break;
		}

		if(pkg->Split(itemId, splitOut, /*false,*/ true) == 2)
        {
            amount -= GData::moneyNeed[GData::SPLIT].tael;
			break;
        }
	}

    if(amount > 0)
    {
        ConsumeInfo ci(SplitEquipment,0,0);
        player->useTael(amount, &ci);
    }

	Stream st(REP::EQ_BATCH_DECOMPOSE);
	st << flag;

    UInt16 cnt = splitOut.size();
    st << cnt;
    for(UInt16 idx = 0; idx < cnt; ++idx)
    {
        SYSMSG_SENDV(102, player, splitOut[idx].itemId, splitOut[idx].count);
        SYSMSG_SENDV(1002, player, splitOut[idx].itemId, splitOut[idx].count);
        st << splitOut[idx].itemId << splitOut[idx].count;
    }

    st << Stream::eos;
	player->send(st);
}

void OnBatchMergeReq( GameMsgHdr& hdr, BatchMergeReq& bmr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;

	if(player->GetPackage()->GetRestPackageSize() < (bmr._unBindNum > 0 ? 1 : 0) + (bmr._bindNum > 0 ? 1 : 0))
	{
		player->sendMsgCode(0, 1011);
		return;
	}

    UInt16 gemIdOut = 0;
    UInt16 gemUnbindOut = 0;
    UInt16 gemBindOut = 0;
    UInt16 succTimes = 0;
    UInt16 failedTimes = 0;
	UInt8 result = player->GetPackage()->BatchMergeGem(bmr._gemId, bmr._unBindNum, bmr._bindNum, bmr._protect, gemIdOut, gemUnbindOut, gemBindOut, succTimes, failedTimes);
	Stream st(REP::GEM_BATCH_UPGRADE);
	st << result << gemIdOut << gemUnbindOut << gemBindOut << succTimes << failedTimes;
	st << Stream::eos;
	player->send(st);
}

#if 0
void OnBatchMergeReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt16 count = 0;
	br >> count;
	if(player->GetPackage()->GetRestPackageSize() < count)
	{
		player->sendMsgCode(0, 1011);
		return;
	}
	std::map<UInt16, UInt32, GObject::_GemCompare > gems;
	for(UInt16 i = 0; i < count; ++ i)
	{
		UInt16 id = 0, num = 0;
		UInt8 bind = 0;
		br >> id >> bind >> num;
		if(id > 0 && num > 0)
		{
			if(bind) gems[id] += (static_cast<UInt32>(num) << 16);
			else gems[id] += static_cast<UInt32>(num);
		}
	}
	std::vector<UInt32> gemsOut;
	UInt8 result = player->GetPackage()->BatchMergeGem(gems, gemsOut);
	Stream st(REP::GEM_BATCH_UPGRADE);
	st << result << static_cast<UInt16>(gemsOut.size());
	for(std::vector<UInt32>::iterator it = gemsOut.begin(); it != gemsOut.end(); ++ it)
	{
		st << static_cast<UInt16>((*it) & 0x7FFF) << static_cast<UInt8>((*it) & 0x8000 ? 1 : 0) << static_cast<UInt16>((*it) >> 16);
	}
	st << Stream::eos;
	player->send(st);
}
#endif

#if 0
void OnActivateAttrReq( GameMsgHdr& hdr, ActivateAttrReq& aar )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::EQ_ACTIVE);
	st << player->GetPackage()->ActivateAttr(aar._fighterId, aar._itemId, aar._itemId2) << aar._fighterId << aar._itemId << Stream::eos;
	player->send(st);
}
#endif
void OnActivateAttrReq( GameMsgHdr& hdr, ActivateAttrReq& aar )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(REP::EQ_ACTIVATE);
	st << player->GetPackage()->ActivateAttr(aar._fighterId, aar._itemId) << aar._fighterId << aar._itemId << Stream::eos;
	player->send(st);
}
void OnEquipMoveReq( GameMsgHdr& hdr, EquipMoveReq& aar )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
    UInt8 res = player->GetPackage()->EquipMove(aar._ffighterId, aar._tfighterId, aar._fitemId,aar._titemId,aar._type);
    UInt8 count = player->GetVar(VAR_EQUIP_MOVE_COUNT);
	Stream st(REP::EQ_MOVE);
	st <<  res << count << Stream::eos;
	player->send(st);
}


#if 0
void OutCitySwitchReq( GameMsgHdr& hdr, OutCitySwitchStruct& lms )
{
	MSG_QUERY_PLAYER(pl);
	pl->moveTo(static_cast<UInt16>(lms._mapid) << 8, false);
}
#endif

void InCitySwitchReq( GameMsgHdr& hdr, InCitySwitchStruct& sms )
{
	MSG_QUERY_PLAYER(pl);
	pl->moveTo(sms._locid, true);
}

void OnTransportReq( GameMsgHdr& hdr, CityTransportReq& ctr )
{
	MSG_QUERY_PLAYER(pl);
	UInt32 viplvl = pl->getVipLevel();

    if (!pl->isYD() && !pl->isBD() && !pl->isQQVIP())
    {
        if(ctr.flag == 0)
        {
            if(viplvl < 3)
            {
                // XXX: vip不收钱
                //if(pl->getTael() < 1)
                //		return;
                //ConsumeInfo ci(Transport,0,0);
                //pl->useTael(1,&ci);
            }
        }
        else
        {
            if(viplvl == 0)
            {
                if(pl->getTael() < GData::moneyNeed[GData::JUMP_MAP].tael)
                {
                    pl->sendMsgCode(0, 1103);
                    return;
                }
                if (!pl->hasChecked())
                    return;
                ConsumeInfo ci(Transport,0,0);
                pl->useTael(GData::moneyNeed[GData::JUMP_MAP].tael,&ci);
            }
        }
    }

	pl->moveTo(ctr._locid, true);
}

void OnDungeonOpReq( GameMsgHdr& hdr, DungeonOpReq& dor )
{
	MSG_QUERY_PLAYER(pl);
	if(pl->getThreadId() != WORKER_THREAD_NEUTRAL)
		return;
	GObject::Dungeon * dg = GObject::dungeonManager[dor.type];
	if(dg == NULL)
		return;
	Stream st(REP::COPY_JOIN);
	st << dor.op;
	UInt8 result = 0;
	switch(dor.op)
	{
	case 0:
		result = dg->playerEnter(pl);
		break;
	case 1:
		result = dg->playerLeave(pl);
		break;
	case 2:
		result = dg->playerContinue(pl);
		break;
	case 3:
		result = dg->playerBreak(pl);
		break;
	default:
		break;
	}
    if (result == 4)
        return;
	st << result << dor.type << Stream::eos;
	pl->send(st);
}

void OnDungeonInfoReq( GameMsgHdr& hdr, DungeonInfoReq& dir )
{
	MSG_QUERY_PLAYER(pl);
	if(pl->getThreadId() != WORKER_THREAD_NEUTRAL)
		return;
	GObject::Dungeon * dg = GObject::dungeonManager[dir.type];
	if(dg == NULL)
		return;

	switch(dir.op)
	{
	case 0:
		dg->sendDungeonInfo(pl);
		break;
	case 1:
		dg->sendMyLootInfo(pl);
		break;
	case 2:
		break;
    case 3:
        dg->doJusticeRoar(pl);
        break;
	default:
		break;
	}
}

void OnDungeonBattleReq( GameMsgHdr& hdr, DungeonBattleReq& dbr )
{
	MSG_QUERY_PLAYER(pl);
	if(pl->getThreadId() != WORKER_THREAD_NEUTRAL)
		return;

	if(pl->GetPackage()->GetRestPackageSize() < 4)
	{
		pl->sendMsgCode(1, 1014);
		return;
	}
	GObject::Dungeon * dg = GObject::dungeonManager[dbr.type];
	if(dg == NULL)
		return;

	dg->startChallenge(pl);
}

void OnDungeonAutoReq( GameMsgHdr& hdr, DungeonAutoReq& dar )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;
	if(pl->getThreadId() != WORKER_THREAD_NEUTRAL)
		return;

	if(dar.type == 0)
	{
		pl->cancelAutoDungeon();
		return;
	}
	if(pl->GetPackage()->GetRestPackageSize() < 1)
	{
		pl->sendMsgCode(1, 1014);
		return;
	}
	GObject::Dungeon * dg = GObject::dungeonManager[dar.type];
	if(dg == NULL)
		return;
	dg->autoChallenge(pl, dar.mtype);
    pl->OnHeroMemo(MC_SLAYER, MD_STARTED, 0, 1);
}

void OnDungeonCompleteAutoReq( GameMsgHdr& hdr, DungeonCompleteAutoReq& )
{
	MSG_QUERY_PLAYER(pl);
    if (!pl->hasChecked())
        return;
	GameMsgHdr hdr2(0x181, WORKER_THREAD_WORLD, pl, 0);
	GLOBAL().PushMsg(hdr2, NULL);
}

void OnAutoCopy( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 id = 0;
    brd >> type;
    brd >> id;

	if((type == 0 || type == 2) && pl->GetPackage()->GetRestPackageSize() < 1)
	{
		pl->sendMsgCode(1, 1014);
		return;
	}

    switch (type)
    {
        case 0:
            {
                UInt8 mtype = 0;
                brd >> mtype;
                pl->startAutoCopy(id, mtype);
            }
            break;

        case 1:
            pl->cancelAutoCopy(id);
            break;

        case 2:
            pl->instantAutoCopy(id);
            break;

        default:
            break;
    }
}

void OnAutoFrontMap( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;


    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 id = 0;
    brd >> type;
    brd >> id;

	if((pl->GetPackage()->GetRestPackageSize() < 1) && (type != 1))
	{
		pl->sendMsgCode(1, 1014);
		return;
	}

    switch (type)
    {
        case 0:
            {
                UInt8 mtype = 0;
                brd >> mtype;
                pl->startAutoFrontMap(id, mtype);
            }
            break;

        case 1:
            pl->cancelAutoFrontMap(id);
            break;

        case 2:
            pl->instantAutoFrontMap(id);
            break;

        default:
            break;
    }
}

struct Dungeon_Enum
{
	Player * player;
	Stream& st;
};

void OnDailyReq( GameMsgHdr& hdr, DailyReq& )
{
	MSG_QUERY_PLAYER(player);
	UInt8 id = player->getThreadId();
	if(id == WORKER_THREAD_NEUTRAL)
	{
		player->sendDailyInfo();
	}
	else
	{
		GameMsgHdr hdr(0x1D0, WORKER_THREAD_NEUTRAL, player, 0);
		GLOBAL().PushMsg(hdr, NULL);
	}
}

void OnNpcTriggerReq( GameMsgHdr& hdr, NpcTriggerReq& req )
{
	MSG_QUERY_PLAYER(player);

	GObject::Map* map = player->GetMap();
	if (map == NULL) return ;
	GObject::MapObject* mo = map->GetObject(req.m_NpcId);
	if (mo == NULL) return ;
	mo->Action(player);
}


void OnTaskNpcActionReq(GameMsgHdr& hdr, TaskNpcActionReq& req)
{
	MSG_QUERY_PLAYER(player)

	GObject::Map* map = player->GetMap();
	if (map == NULL) return ;
	switch (req.m_ActionType)
	{
	case 0x04:
		player->regenAll();
		break;
	case 0x02:
		MOAction::CollectActionStep(player, req.m_NpcId);
		break;
	case 0x30:
		GameAction()->exchangeExtraReward(player, req.m_ActionID);
		break;
	case 0x01:
	case 0x52:
		MOAction::TaskActionStep(player, req.m_NpcId, req.m_ActionID, req.m_ActionStep);
		break;
	case 0x60:
		MOAction::RunDayCopyTaskStep(player, req.m_NpcId, req.m_ActionID);
    case 0x70:
		MOAction::RunActiveTaskStep(player, req.m_NpcId, req.m_ActionID);
        break;
	default:
		return ;
	}
}


void OnPlayerGetTaskReq(GameMsgHdr& hdr, PlayerGetTaskReq& req)
{
	MSG_QUERY_PLAYER(player);

	player->GetTaskMgr()->SendCurrTaskInfor();
}

void OnTaskActionReq(GameMsgHdr& hdr, TaskActionReq& req)
{
 	MSG_QUERY_PLAYER(player);

	bool succ = false;
    bool succ1 = false;
	switch (req.m_Action)
	{
	case 0:
		//接受, 走脚???
		succ = GameAction()->AcceptTask(player, req.m_TaskId);
		break;
	case 1:
		//提交, 直接走脚???
		succ = GameAction()->SubmitTask(player, req.m_TaskId, req.m_ItemId, req.m_ItemNum); //提交
        if (succ)
        {
            player->addAwardByTaskColor(req.m_TaskId, false);
            player->finishClanTask(req.m_TaskId);
        }
		break;
	case 2:
		//放弃
		succ = GameAction()->AbandonTask(player, req.m_TaskId);
		break;
    case 3:
        if (!player->hasChecked())
            return;
        // 师门，衙门任务立即完成
        succ = player->addAwardByTaskColor(req.m_TaskId, true);
		succ1 = player->GetTaskMgr()->CompleteClanTaskDirect(req.m_TaskId);
        if (succ1)
        {
            GameAction()->SubmitTask(player, req.m_TaskId, req.m_ItemId, req.m_ItemNum); //提交
            succ1 = player->finishClanTask(req.m_TaskId);
        }
        else
        {
            if(succ) //完成衙门 师门
                player->GetTaskMgr()->CheckTaskAttainment(req.m_TaskId, NULL);
        }

        break;
	default:
		return ;
	}
	if (succ || succ1)
	{
		TaskActionResp resp;
		resp.m_TaskId = req.m_TaskId;
		resp.m_Action = req.m_Action;
		player->send(resp);
	}
}

void OnCanAcceptTaskReq(GameMsgHdr& hdr, CanAcceptTaskReq& req)
{
	MSG_QUERY_PLAYER(player);

	player->GetTaskMgr()->SendCanAcceptTaskInfor();
}

struct CountryBattleJoinReply
{
	UInt8 result;
	MESSAGE_DEF1(REP::COUNTRY_WAR_JOIN, UInt8, result);
};

void CountryBattleJoinReq( GameMsgHdr& hdr, CountryBattleJoinStruct& req )
{
	MSG_QUERY_PLAYER(player);
    if(WORLD().isNewCountryBattle())
		return;
	if(!PLAYER_DATA(player, inCity))
		return;
	UInt16 loc = PLAYER_DATA(player, location);
	GObject::SpotData * spot = GObject::Map::Spot(loc);
	if(spot == NULL || !spot->m_CountryBattle)
		return;

	CountryBattleJoinReply rep;
	CountryBattle * cb = spot->GetCountryBattle();
    if(!cb) return;
	if(req._action == 0)
	{
		rep.result = cb->playerEnter(player) ? 0 : 2;
	}
	else
	{
		cb->playerLeave(player);
		rep.result = 1;
	}
    if(rep.result == 0)
        player->countryBattleUdpLog(1090, player->getCountry());
	player->send(rep);
}

void NewCountryBattleJoinReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    if(!WORLD().isNewCountryBattle())
		return;
	if(!PLAYER_DATA(player, inCity))
		return;
	UInt16 loc = PLAYER_DATA(player, location);
	GObject::SpotData * spot = GObject::Map::Spot(loc);
	if(spot == NULL || !spot->m_NewCountryBattle)
		return;

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;
	NewCountryBattle * ncb = spot->GetNewCountryBattle();
    if(!ncb) return;
	if(type == 0)
		ncb->playerEnter(player);
	else if(type == 1)
		ncb->playerLeave(player);
    else if(type == 2)
        ncb->sendSelfInfo(player);
    else if(type == 3)
    {
        UInt8 skillId = 0;
        brd >> skillId;
        ncb->useSkill(player, skillId);
    }
    else if(type == 4)
    {
        UInt8 kind = 0;
        brd >> kind;
        ncb->buySkill(player, kind);
    }
}

void OnLanchChallengeReq( GameMsgHdr& hdr, LanchChallengeReq& lcr)
{
	MSG_QUERY_PLAYER(player);
	GObject::Player *target = globalNamedPlayers[player->fixName(lcr.target)];
	if(target == NULL)//error
		return ;
	if(player->getCountry() != target->getCountry())
		return;
	UInt32 now = TimeUtil::Now();
	UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
	if(buffLeft > 0)
	{
		player->sendMsgCode(0, 1407, buffLeft - now);
		return ;
	}
	if( target->getBuffData(PLAYER_BUFF_ATTACKING) > 0 )
	{
		player->sendMsgCode(0, 1409);
		return ;
	}
	UInt8 tid = player->getThreadId();
	if( target->getThreadId() != tid || !target->hasStatus(0x02) || !player->hasStatus(0x02) )
	{
		Stream st1(REP::REQUESTCHALLENGE);
		st1 << static_cast<UInt8>(1) << lcr.target << Stream::eos;
		player->send(st1);
		return;
	}

	Stream st(REP::LANCHCHALLENGE);
	st << player->getCountry() << player->getName() << Stream::eos;
	target->send(st);
}

void OnRequestChallengeReq( GameMsgHdr& hdr, RequestChallengeReq& rcr)
{
	MSG_QUERY_PLAYER(player);
	GObject::Player *attacker = globalNamedPlayers[player->fixName(rcr.attacker)];
	if(attacker == NULL)//error
		return ;
	Stream st(REP::REQUESTCHALLENGE);
	if(rcr.res == 1)// refuse
	{
		st << static_cast<UInt8>(1) << player->getName() << Stream::eos;
		attacker->send(st);
		return;
	}
	if(rcr.res == 2)// another is challenging player
	{
		st << static_cast<UInt8>(2) << player->getName() << Stream::eos;
		attacker->send(st);
		return;
	}
	UInt32 now = TimeUtil::Now();
	UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
	if(buffLeft > 0)
	{
		player->sendMsgCode(0, 1407, buffLeft - now);
		return ;
	}
	if( attacker->getBuffData(PLAYER_BUFF_ATTACKING) > 0)
	{
		player->sendMsgCode(0, 1409);
		return ;
	}

	UInt8 tid = player->getThreadId();
	if( attacker->getThreadId() != tid || !attacker->hasStatus(0x02) || !player->hasStatus(0x02) )
		return;

	st << static_cast<UInt8>(0) << player->getName() << Stream::eos;
	attacker->send(st);


	Battle::BattleSimulator bsim(player->getLocation(), player, attacker);
	player->PutFighters( bsim, 0, true );
	attacker->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;
	int turns = bsim.getTurns();

	Stream st1(REP::ATTACK_NPC);
	st1 << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
	player->send(st1);
	st1.data<UInt8>(4) = static_cast<UInt8>(res ? 0 : 1);
	attacker->send(st1);

	player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + turns * 2);
	attacker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + turns * 2);
}

#if 0
void OnChallengePlayerReq( GameMsgHdr& hdr, ChallengePlayerReq& cpr )
{
	MSG_QUERY_PLAYER(player);
	player->cancelAutoBattle();
	player->cancelAutoDungeon();
	GObject::Player * target = globalNamedPlayers[player->fixName(cpr._name)];
	if(target == NULL)
		return;
	UInt8 tid = player->getThreadId();
	UInt8 lev1 = player->GetLev();
	UInt8 lev2 = target->GetLev();
	UInt8 lev = lev1 > lev2 ? lev1 - lev2 : lev2 - lev1;
	if(target->getThreadId() != tid || !player->hasStatus(Player::PK) || !target->hasStatus(Player::PK) )
		return;

	UInt32 now = TimeUtil::Now();
	if(target->getBuffData(PLAYER_BUFF_PROTECT, now) > 0)
	{
		player->sendMsgCode(0, 1411);
		return ;
	}
	if(player->getBuffData(PLAYER_BUFF_PROTECT, now) > 0)
	{
		player->sendMsgCode(0, 1410);
		return ;
	}
	UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
	if(buffLeft > 0)
	{
		player->sendMsgCode(0, 1407, buffLeft - now);
		return ;
	}
	if(target->getBuffData(PLAYER_BUFF_ATTACKING, now) > 0)
	{
		player->sendMsgCode(0, 1409);
		return ;
	}

	if(tid != WORKER_THREAD_NEUTRAL)
	{
		if(tid == player->getCountry())
		{
			if(target->getCountry() == player->getCountry())
				return;
		}
		else
			return;
	}
	TRACE_LOG("%s(%"I64_FMT"u) challenge %s(%"I64_FMT"u)", player->getName().c_str(), player->getId(), target->getName().c_str(), target->getId());
	int turns;
	//UInt32 tael = 0;
	//UInt32 Achievement = 0;

	if(player->challenge(target, NULL, &turns))
	{
		if(lev <= 5 && !GObject::challengeCheck.hasPair(player, target))
		{
#if 0
			GObject::challengeCheck.pushPair(player, target);
			if(target->getTael() > 519)
			{
				tael = (target->getTael() - 500) / 20;
				ConsumeInfo ci1(ResourceLost, 0, 0);
				target->useTael2(tael, player, &ci1);
				player->getTael(tael);
			}
			Achievement = target->getAchievement() / 20;
			ConsumeInfo ci2(ResourceLost, 0, 0);
			target->useAchievement2(Achievement, player, &ci2);
			player->getAchievement(Achievement);
#endif
		}

		target->moveToHome();
		SYSMSG_SENDV(1043, player, target->getCountry(), target->getName().c_str());
		SYSMSG_SENDV(144, target, player->getCountry(), player->getName().c_str());
		SYSMSG_SENDV(1044, target, player->getCountry(), player->getName().c_str());
		if(lev1 >= 60 && lev2 >= 60)
		{
			SYSMSG_BROADCASTV(347, player->getCountry(), player->getName().c_str(), target->getCountry(), target->getName().c_str());
		}
	}
	else
	{
		if(lev <= 5 && !GObject::challengeCheck.hasPair(target, player))
		{
#if 0
			GObject::challengeCheck.pushPair(target, player);
			if(player->getTael() > 519)
			{
				tael = (player->getTael() - 500) / 20;
				ConsumeInfo ci1(ResourceLost, 0, 0);
				player->useTael2(tael, target, &ci1);
				target->getTael(tael);
			}
			Achievement = player->getAchievement() / 20;
			ConsumeInfo ci2(ResourceLost, 0, 0);
			player->useAchievement2(Achievement, target, &ci2);
			target->getAchievement(Achievement);
#endif
		}

		player->moveToHome();
		SYSMSG_SENDV(1043, target, player->getCountry(), player->getName().c_str());
		SYSMSG_SENDV(144, player, target->getCountry(), target->getName().c_str());
		SYSMSG_SENDV(1044, player, target->getCountry(), target->getName().c_str());
		if(lev1 >= 60 && lev2 >= 60)
		{
			//SYSMSG_BROADCASTV(370 + now % 2,  player->getCountry(), player->getName().c_str(), target->getCountry(), target->getName().c_str(), target->getCountry(), target->getName().c_str());
		}
	}
	player->setBuffData(PLAYER_BUFF_ATTACKING, now + turns * 2);
	target->setBuffData(PLAYER_BUFF_ATTACKING, now + turns * 2);
	player->setBuffData(PLAYER_BUFF_PKLOCK, now + 20 * 60);
	target->addFoe(player);
}
#endif

void OnAttackNpcReq( GameMsgHdr& hdr, AttackNpcReq& anr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->isInCity())
	{
		player->sendMsgCode(0, 1408);
		return;
	}
	player->cancelAutoBattle();
	player->cancelAutoDungeon();
	UInt16 loc = player->getLocation();
	GObject::Map * map = Map::FromSpot(loc);
	if(map == NULL)
	{
		player->sendMsgCode(0, 1408);
		return;
	}
	GObject::MapObject * mo = map->GetObject(anr._npcId);
	if(mo == NULL || mo->GetSpot() != loc)
	{
		player->sendMsgCode(0, 1408);
		return;
	}

	if (GObject::Tianjie::instance().isTjNpc(anr._npcId, loc))
	{
	    GObject::Tianjie::instance().attack(player, loc, anr._npcId);
	    return;
	}

    if (WBossMgr::isWorldBoss(anr._npcId))
        worldBoss.attack(player, loc, anr._npcId);
    else
        player->attackNpc(anr._npcId, 0xFFFFFFFF, player->GetLev() <= 20);
}

void OnAutoBattleReq( GameMsgHdr& hdr, AutoBattleReq& abr )
{
	MSG_QUERY_PLAYER(player);
	player->autoBattle(abr._npcId);
}

void OnCancelAutoBattleReq( GameMsgHdr& hdr, CancelAutoBattleReq& )
{
	MSG_QUERY_PLAYER(player);
	GameMsgHdr hdr2(0x179, WORKER_THREAD_WORLD, player, 0);
	GLOBAL().PushMsg(hdr2, 0);
}

void OnInstantAutoBattleReq( GameMsgHdr& hdr, InstantAutoBattleReq& )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	player->instantAutoBattle();
}

void kick(Player* pl)
{
    TcpConnection conn = NETWORK()->GetConn(pl->GetSessionID());
    if (conn)
    {
        Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
        if (cl)
        {
            pl->SetSessionID(-1);
            pl->testBattlePunish();
            pl->setOnline(false);
            static UInt8 kick_pkt[4] = {0x00, 0x00, 0xFF, REP::BE_DISCONNECT};
            cl->send(kick_pkt, 4);
            cl->SetPlayer(NULL);
            cl->pendClose();
        }
    }
}

void OnBattleEndReq( GameMsgHdr& hdr, BattleEndReq& req )
{
	MSG_QUERY_PLAYER(player);
	UInt32 now = TimeUtil::Now();

    TeamCopyPlayerInfo* tcpInfo = player->getTeamCopyPlayerInfo();
    if(tcpInfo)
    {
        tcpInfo->sendAwardInfo();
    }

    player->addLastTjScore();

	player->checkLastBattled();

    if(now <= PLAYER_DATA(player, battlecdtm))
		return ;
}

void OnCopyReq( GameMsgHdr& hdr, CopyReq& req )
{
	MSG_QUERY_PLAYER(player);
	if(!player->isInCity())
	{
		player->sendMsgCode(0, 1408);
		return;
	}
	player->cancelAutoBattle();
	player->cancelAutoDungeon();
	UInt16 loc = player->getLocation();
	GObject::Map * map = Map::FromSpot(loc);
	if(map == NULL)
	{
		player->sendMsgCode(0, 1408);
		return;
	}
    switch (req.type) {
        case 0:
            GObject::playerCopy.sendInfo(player, req.id);
            break;

        case 1:
            GObject::playerCopy.enter(player, req.id);
            break;

        case 2:
            GObject::playerCopy.reset(player, req.id);
            break;

        case 3:
            break;

        case 4:
            GObject::playerCopy.fight(player, req.id);
            break;

        default:
            break;
    }
}

void OnFrontMapReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
	if(!player->isInCity())
	{
		player->sendMsgCode(0, 1408);
		return;
	}
	player->cancelAutoBattle();
	player->cancelAutoDungeon();
	UInt16 loc = player->getLocation();
	GObject::Map * map = Map::FromSpot(loc);
	if(map == NULL)
	{
		player->sendMsgCode(0, 1408);
		return;
	}

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 id = 0;
    UInt8 param = 0;
    brd >> type;
    brd >> id;

    switch (type) {
        case 0:
            brd >> param; // flag
            GObject::frontMap.sendInfo(player, id, param?true:false);
            break;

        case 1:
            GObject::frontMap.enter(player, id);
            break;

        case 2:
            GObject::frontMap.reset(player, id);
            break;

        case 3:
            break;

        case 4:
            brd >> param; // spot
            GObject::frontMap.fight(player, id, param);
            break;
        case 5:
            //GObject::frontMap.sendFrontMap(player, id);
            break;

        default:
            break;
    }
}

void OnStoreBuyReq( GameMsgHdr& hdr, StoreBuyReq& lr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
    if (!lr._count && lr._type != 125)
        lr._count = 1;
	UInt32 price = 0;
    if (lr._type == 1)
        price = GData::store.getPrice(lr._type, lr._itemId, lr._count, 0); // XXX: when discount need one item id
    else if(lr._type == 125) //蓝砖超人活动页
    {
        if(!World::getBDSuperman())
            return;
        price = GameAction()->GetBDSupermanPrice(player, lr._itemId, lr._count > 0);
        price = price * 1;
        if(!price)
            return;
    }
    else
        price = GData::store.getPrice(lr._type, lr._itemId);
	Stream st(REP::STORE_BUY);
	if(price == 0 || price == 0xFFFFFFFF)
	{
        // 客户端商城界面有错误，重新更新界面
		st << static_cast<UInt8>(3);
        GData::store.sendList(lr._type, player);
	}
	else
	{
        if (lr._type > 1 && lr._type < 8)
            price *= lr._count;
        else if(lr._type >= PURCHASE3 && lr._type <= PURCHASE4)
            price *= lr._count;
        switch(lr._type)
		{
        case 1:
            {
                // 折扣商品的购买
                UInt8 discountType = lr._count;
                UInt8 varoff = GData::store.getDisTypeVarOffset(discountType);
                UInt8 exType = 0;
                UInt32 exValue = 0;

                exType = GData::store.getExDiscount(discountType, exValue);

                switch(exType)
                {
                    case 1:
                        // 消费限购
                        player->GetVar(VAR_DISCOUNT_CONSUME1 + discountType - 4);
                        break;
                    case 2:
                        // 充值限购
                        player->GetVar(VAR_DISCOUNT_RECHARGE1 + discountType - 4);
                        break;
                    default:
                        break;
                }

                if (varoff == 0xff)
                    return;

                if (player->GetVar(VAR_DISCOUNT_1+varoff) >= GData::store.getDiscountLimit(discountType))
                {
                    player->sendMsgCode(0, 1020);
                    return;
                }

                UInt16 items[4] = {0};
                UInt8 c = GData::store.getItemsByDiscount(discountType, items);
                if (!c) return;
                if (player->GetPackage()->GetRestPackageSize() < c)
                {
                    // 背包空间不足
                    player->sendMsgCode(0, 1011);
                    return;
                }

                // 获取价格
                price = 0;
                for (UInt8 i = 0; i < c; ++i)
                    price += GData::store.getPrice(1, items[i], discountType, i);

                if(PLAYER_DATA(player, gold) < price)
                {
                    // 玩家货币不足
                    st << static_cast<UInt8>(1);
                }
                else
                {
                    // 检查完毕，购买道具
                    for (UInt8 i = 0; i < c; ++i)
                    {
                        GObject::ItemBase * item;
                        if(IsEquipTypeId(items[i]))
                            item = player->GetPackage()->AddEquipN(items[i], 1, true, false, FromNpcBuy);
                        else
                            item = player->GetPackage()->AddItem(items[i], 1, true, false, FromNpcBuy);
                    }

                    UInt8 logVarOffset = 0;
                    switch (discountType)
                    {
                        case 4:
                            logVarOffset = DiscountSp1 - Discount3;
                            break;
                        case 5:
                            logVarOffset = DiscountSp2 - Discount3;
                            break;
                        case 6:
                            logVarOffset = DiscountSp3 - Discount3;
                            break;
                        case 7:
                            logVarOffset = Discount3 - Discount3;
                            break;
                        case 8:
                            logVarOffset = Discount5 - Discount3;
                            break;
                        case 9:
                            logVarOffset = Discount8 - Discount3;
                            break;
                        default:
                            logVarOffset = 0xff;
                    }

                    if (logVarOffset != 0xff)
                    {
                        ConsumeInfo ci(Discount3+logVarOffset, 0, 0);
                        player->useGold(price, &ci);
                        player->discountLog(discountType);
                    }
                    st << static_cast<UInt8>(0);

                    //GameAction()->doAty(player, AtyBuy, 0, 0);

                    player->AddVar(VAR_DISCOUNT_1+varoff, 1);   // 更新购买次数（限购商品）
                    player->sendDiscountLimit();
                }
            }
            break;
		case 4:
			if(PLAYER_DATA(player, coupon) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				GObject::ItemBase * item;
				if(IsEquipTypeId(lr._itemId))
                {
                    if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                        item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                    else
                        item = NULL;
                }
				else
                {
                    if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                        item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                    else
                        item = NULL;
                }
				if(item == NULL)
					st << static_cast<UInt8>(2);
				else
				{
				    ConsumeInfo ci(Item,lr._itemId,lr._count);
					player->useCoupon(price,&ci);
					st << static_cast<UInt8>(0);

                    //GameAction()->doAty( player, AtyBuy, 0,0);
                }
			}
			break;
		case 5:
			if(PLAYER_DATA(player, tael) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				GObject::ItemBase * item;
				if(IsEquipTypeId(lr._itemId))
                {
                    if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                        item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                    else
                        item = NULL;
                }
				else
                {
                    if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                        item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                    else
                        item = NULL;
                }
				if(item == NULL)
					st << static_cast<UInt8>(2);
				else
				{
				    ConsumeInfo ci(Item,lr._itemId,lr._count);
					player->useTael(price,&ci);
					st << static_cast<UInt8>(0);
                    //GameAction()->doAty( player,AtyBuy, 0,0);
                }
			}
			break;
#if 0
		case 5:
			if(PLAYER_DATA(player, coin) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				GObject::ItemBase * item;
				if(IsEquipTypeId(lr._itemId))
					item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, false, false, FromNpcBuy);
				else
					item = player->GetPackage()->AddItem(lr._itemId, lr._count, false, false, FromNpcBuy);
				if(item == NULL)
					st << static_cast<UInt8>(2);
				else
				{
				    ConsumeInfo ci(Item,lr._itemId,lr._count);
					player->useCoin(price,&ci);
					st << static_cast<UInt8>(0);

				}
			}
			break;
#endif
		case 6:
            // 荣誉购买
			if(PLAYER_DATA(player, achievement) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
                bool buyFgt = false;
				GObject::ItemBase * item = NULL;
				if(IsEquipTypeId(lr._itemId))
                {
                    if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                        item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                    else
                        item = NULL;
                }
				else if (IsFighterTypeId(lr._itemId))
                {
                    buyFgt = player->addFighterFromItem(lr._itemId, price);
                    if (buyFgt)
                        price /= lr._count;
                }
                else
                {
                    if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                        item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                    else
                        item = NULL;
                }
				if(item == NULL && !buyFgt)
					st << static_cast<UInt8>(2);
				else if (item || buyFgt)
				{
					ConsumeInfo ci(Item,lr._itemId, lr._count);
					player->useAchievement(price,&ci);
					st << static_cast<UInt8>(0);
                    //GameAction()->doAty( player, AtyBuy, 0,0);
                    player->storeUdpLog(1141, 6, lr._itemId, lr._count);
				}
			}
			break;
        case 7:
            // 声望购买
            {
                UInt32 prestige = player->getPrestige();
                if (prestige < price)
                {
                    st << static_cast<UInt8>(1);
                }
                else
                {
                    bool buyFgt = false;
                    GObject::ItemBase * item = NULL;
                    if(IsEquipTypeId(lr._itemId))
                    {
                        if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    else if (IsFighterTypeId(lr._itemId))
                    {
                        buyFgt = player->addFighterFromItem(lr._itemId, price);
                        if (buyFgt)
                            price /= lr._count;
                    }
                    else
                    {
                        if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    if(item == NULL && !buyFgt)
                        st << static_cast<UInt8>(2);
                    else if (item || buyFgt)
                    {
                        ConsumeInfo ci(Item,lr._itemId, lr._count);
                        player->usePrestige(price,&ci);
                        st << static_cast<UInt8>(0);

                        //GameAction()->doAty(player, AtyBuy, 0,0);
                        player->storeUdpLog(1141, 7, lr._itemId, lr._count);
                    }
                }
            }
            break;
        case 8:
            {
                UInt32 priceID = price&0xFFFF;
                UInt32 priceNum = (price>>16)&0xFF;
                priceNum *= lr._count;

                if (player->GetPackage()->GetItemAnyNum(priceID) < priceNum)
                {
                    st << static_cast<UInt8>(1);
                }
                else
                {
                    GObject::ItemBase * item;
                    if(IsEquipTypeId(lr._itemId))
                    {
                        if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    else
                    {
                        if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }

                    if(item == NULL)
                        st << static_cast<UInt8>(2);
                    else
                    {
                        bool bind = true;
                        ConsumeInfo ci(PrintDemon, lr._itemId, lr._count);
                        player->GetPackage()->DelItemAny(priceID, priceNum, &bind);
                        player->useDemonLog(priceID, priceNum, &ci);
                        st << static_cast<UInt8>(0);
                    }
                }
            }
            break;
        case PURCHASE3:
        case PURCHASE3+1:
        case PURCHASE3+2:
        case PURCHASE3+3:
            {
                UInt32 arena = player->GetVar(VAR_MONEY_ARENA);
                if (arena < price)
                {
                    st << static_cast<UInt8>(1);
                }
                else
                {
                    GObject::ItemBase * item;
                    if(IsEquipTypeId(lr._itemId))
                    {
                        if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    else
                    {
                        if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    if(item == NULL)
                        st << static_cast<UInt8>(2);
                    else
                    {
                        //player->SetVar(VAR_MONEY_ARENA, arena - price);
				        ConsumeInfo ci(Item,lr._itemId,lr._count);
					    player->useMoneyArena(price,&ci);
                        st << static_cast<UInt8>(0);
                        //GameAction()->doAty( player,AtyBuy, 0,0);
                    }
                }
            }
            break;
        case PURCHASE4:
            {
                UInt32 proffer = player->getClanProffer();
                if (proffer < price)
                {
                    st << static_cast<UInt8>(1);
                }
                else
                {
                    GObject::ItemBase * item = NULL;
                    if(IsEquipTypeId(lr._itemId))
                    {
                        if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    else
                    {
                        if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, true))
                            item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
                        else
                            item = NULL;
                    }
                    if(item == NULL)
                        st << static_cast<UInt8>(2);
                    else
                    {
                        ConsumeInfo ci(Item,lr._itemId, lr._count);
                        player->useClanProffer(price,&ci);
                        st << static_cast<UInt8>(0);

                        //GameAction()->doAty(player, AtyBuy, 0,0);
                    }
                }
            }
            break;
        default:
#if 0 // XXX: 不需要这个检查
            if (player->GetPackage()->GetRestPackageSize() < lr._count)
            {
                // 背包空间不足
                player->sendMsgCode(0, 1011);
                return;
            }
#endif
			if(PLAYER_DATA(player, gold) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				bool bind = false;
                if(lr._type == 125 && World::getBDSuperman())     //蓝砖超人活动
                    bind = true;
				GObject::ItemBase * item;
                if(IsEquipTypeId(lr._itemId))
                {
                    if (player->GetPackage()->TryBuyEquip(lr._itemId, lr._count, bind))
                        item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, bind, false, FromNpcBuy);
                    else
                        item = NULL;
                }
                else
                {
                    if (player->GetPackage()->TryBuyItem(lr._itemId, lr._count, bind))
                        item = player->GetPackage()->AddItem(lr._itemId, lr._count, bind, false, FromNpcBuy);
                    else
                        item = NULL;
                }
                if(item == NULL)
                    st << static_cast<UInt8>(2);
				else
				{
					ConsumeInfo ci(Item,lr._itemId,lr._count);
                    player->useGold(price,&ci);
                    player->consumeGold(price);
					st << static_cast<UInt8>(0);

                    if (lr._type == PURCHASE1 + 1 )
                    {
                        bool flag = false;
                        UInt32 now = TimeUtil::Now();
                        if ((GVAR.GetVar(GVAR_DISCOUNT_TYPE1) == 1)
                                && GVAR.GetVar(GVAR_DISCOUNT_BEGIN1) < now
                                && GVAR.GetVar(GVAR_DISCOUNT_END1) > now)
                        {
                            player->AddVar(VAR_DISCOUNT_CONSUME1, price);
                            flag = true;
                        }
                        else
                            player->SetVar(VAR_DISCOUNT_CONSUME1, 0);
                        if ((GVAR.GetVar(GVAR_DISCOUNT_TYPE2) == 1)
                                && GVAR.GetVar(GVAR_DISCOUNT_BEGIN2) < now
                                && GVAR.GetVar(GVAR_DISCOUNT_END2) > now)
                        {
                            player->AddVar(VAR_DISCOUNT_CONSUME2, price);
                            flag = true;
                        }
                        else
                            player->SetVar(VAR_DISCOUNT_CONSUME2, 0);
                        if ((GVAR.GetVar(GVAR_DISCOUNT_TYPE3) == 1)
                                && GVAR.GetVar(GVAR_DISCOUNT_BEGIN3) < now
                                && GVAR.GetVar(GVAR_DISCOUNT_END3) > now)
                        {
                            player->AddVar(VAR_DISCOUNT_CONSUME3, price);
                            flag = true;
                        }
                        else
                            player->SetVar(VAR_DISCOUNT_CONSUME3, 0);
                        if (flag)
                            player->sendDiscountLimit();
                    }

                    //GameAction()->doAty(player, AtyBuy ,0,0);
                }
			}
			break;
		}
	}
	st << lr._type << lr._itemId << lr._count << price << Stream::eos;
	player->send(st);
}

struct ChatRep
{
	UInt8 type;
	std::string name;
	UInt8 cny;
	UInt8 sex;
	UInt8 office;
	UInt8 guard;
	std::string text;
    UInt8 level;
	MESSAGE_DEF8(REP::CHAT, UInt8, type, std::string, name, UInt8, cny, UInt8, sex, UInt8, office, UInt8, guard, std::string, text, UInt8, level);
};

static bool inCountry(const Network::TcpConduit * conduit, UInt8 country)
{
	const Network::GameClient * cl = static_cast<const Network::GameClient *>(conduit);
	Player * pl = cl->GetPlayer();
	return pl != NULL && pl->getCountry() == country;
}

#define ITEM_SPEAKER 16
#define ITEM_FLOWER 440
#define ITEM_QIXI_TALK 9123

#ifdef _WIN32
int inet_aton(const char *cp, struct in_addr *inp)
{
    unsigned long addr = inet_addr(cp);
    if(addr == INADDR_NONE)
        return -1;
    inp->s_addr = addr;
    return 0;
}
#endif

int ToMsgCenter(Stream st)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	int32_t sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd == -1)
	{
		return E_FAIL;
	}
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(cfg.msgCenterPort);
	if(inet_aton(cfg.msgCenterHost.c_str(), &addr.sin_addr) < 0)
    {
		close(sockfd);
		return E_FAIL;
	}
    Stream stMsg = st;
    stMsg.pop_front(4); //将协议头先删除
    stMsg.prepend((UInt8*)&cfg.serverNum, sizeof(cfg.serverNum));//插入serverid号

    int packlen = stMsg.size();

    UInt8 buf[4] = {0, 0, 0xFF, REP::CHAT};
    memcpy(buf, &packlen, 2);

    stMsg.prepend(buf, sizeof(buf));

    int cmd = REP::CHAT;
    stMsg.prepend((UInt8*)&cmd, sizeof(cmd));

    packlen = stMsg.size();
    stMsg.prepend((UInt8*)&packlen, sizeof(packlen));

    int len = 0;
	if((len = sendto(sockfd, (const char*)(UInt8*)stMsg, stMsg.size(), 0, (struct sockaddr*)&addr, sizeof(struct sockaddr_in))) < 0)
	{
		close(sockfd);
		return E_FAIL;
	}

    close(sockfd);
    return 0;
}
void OnChatReq( GameMsgHdr& hdr, ChatReq& cr )
{
	using namespace std::placeholders;
	MSG_QUERY_PLAYER(player);

    if(player->getBuffData(PLAYER_BUFF_BANCHAT)!=0)
        return;

	if(gmHandler.Handle(cr._text, player))
		return;

	Stream st(REP::CHAT);
	UInt8 office = player->getTitle(), guard = 0;
    guard = player->getPF();
	st << cr._type << player->getName() << player->getCountry() << static_cast<UInt8>(player->IsMale() ? 0 : 1)
        << office << guard << cr._text << player->GetLev() << Stream::eos;
	switch(cr._type)
	{
	case 0xFF:
		break;
	case 1:
		NETWORK()->Broadcast(st, std::bind(inCountry, _1, player->getCountry()));
		break;
	case 2:
		{
			GameMsgHdr hdr(0x160, WORKER_THREAD_WORLD, player, st.size());
			GLOBAL().PushMsg(hdr, static_cast<UInt8 *>(st));
		}
		break;
	case 3:
        {
            if (!player->hasChecked())
                return;
            if(!player->GetPackage()->DelItemAny(ITEM_SPEAKER, 1))
                break;
            NETWORK()->Broadcast(st);
            break;
        }
    case 8:
        {
            if (!player->hasChecked())
                return;
            if(!player->GetPackage()->DelItemAny(ITEM_FLOWER, 1))
                break;
            NETWORK()->Broadcast(st);
            break;
        }
    case 9:
        {
            if (!player->hasChecked())
                return;
            if(!player->GetPackage()->DelItemAny(ITEM_QIXI_TALK, 1))
                break;
            NETWORK()->Broadcast(st);
            break;
        }
	default:
		NETWORK()->Broadcast(st);
		break;
	}
	if (cr._type == 0 && cfg.msgCenterPort > 0) //只监听世界
	{
		ToMsgCenter(st);
	}

}

void OnPrivChatReq( GameMsgHdr& hdr, PrivChatReq& pcr )
{
	MSG_QUERY_PLAYER(player);
	if(player->getBuffData(PLAYER_BUFF_BANCHAT)!=0)
        return;

	ChatRep rep;
	rep.type = 0xFF;
	GObject::Player * pl = GObject::globalPlayers[pcr._id];
	if(pl == NULL || pl->GetSessionID() == -1)
	{
		rep.cny = 0;
		rep.sex = 0;
		rep.office = player->getTitle();
		rep.guard = player->getPF();
		rep.level = player->GetLev();
		player->send(rep);
	}
	else
	{
		rep.name = player->getName();
		rep.text = pcr._text;
		rep.cny = player->getCountry();
		rep.sex = player->IsMale() ? 0 : 1;
		rep.office = player->getTitle();
		rep.guard = player->getPF();
		rep.level = player->GetLev();
		pl->send(rep);
	}
}

void OnChatItemReq( GameMsgHdr& hdr, ChatItemReq& oir )
{
	MSG_QUERY_PLAYER(player);
	chatItem.post(oir._type, oir._playerId, oir._id, player);
}

void OnSubmitBugInfo(GameMsgHdr& hdr,BugInfoReq& biq)
{
    MSG_QUERY_PLAYER(player);
    int pos=biq._title.find_first_of(":");
    int pos2=biq._content.find_first_of(":");
    std::string  type=biq._title.substr(1,pos-3);


    std::string  title=biq._title.substr(pos+1);
    std::string content=biq._content.substr(pos2+1);
	DBLOG1().PushUpdateData("insert into complaints (server_id,player_id,submit_type,submit_time,title,content) values(%u,%"I64_FMT"u,'%s',%u,'%s','%s')",
		cfg.serverLogId, hdr.player->getId(), type.c_str(), TimeUtil::Now(), title.c_str(), content.c_str());
}

void OnYellowDiamondInfo(GameMsgHdr& hdr, YellowDiamondInfoReq& ydInfo)
{
    MSG_QUERY_PLAYER(player);
    player->sendYellowDiamondInfo();
}

void OnYellowDiamondAwardRcv(GameMsgHdr& hdr, YellowDiamondAwardRcvReq& ydar)
{
    MSG_QUERY_PLAYER(player);
    player->rcvYellowDiamondAward(ydar.m_type, ydar.m_d3d6);
}

void OnYellowDiamondGetPacksRcv(GameMsgHdr& hdr, YellowDiamondGetPacksReq& ydar)
{
    MSG_QUERY_PLAYER(player);
    struct Key
    {
        GObject::Player* player;
        char key[128];
    } key;

    key.player = player;
    snprintf(key.key, sizeof(key.key), "%s", ydar.key.c_str());

    UInt8 type = 0;
    if (isdigit(key.key[0]) && key.key[1] == '-')
        type = key.key[0] - '0';
    if (!type || type == 9)
        type = 3;

    if (type && !GameAction()->testTakePackSize(player, type))
        return;
    if (type && !GameAction()->testTakePack(player, type, player->GetVar(VAR_KEYPACK1+type-1)))
    {
		player->sendMsgCode(1, 1018);
        return;
    }

	LoginMsgHdr hdr1(0x300, WORKER_THREAD_LOGIN, 0, player->GetSessionID(), sizeof(key));
	GLOBAL().PushMsg(hdr1, &key);
}

struct TradeListReq
{
	UInt16 _index;
	UInt16 _num;

	TradeListReq() : _index(0), _num(0) {}
	MESSAGE_DEF2(REQ::TRADE_LIST, UInt16, _index, UInt16, _num);
};

struct TradeDataReq
{
	UInt32 _tradeId;

	TradeDataReq() : _tradeId(0) {}
	MESSAGE_DEF1(REQ::TRADE_DATA, UInt32, _tradeId);
};

S11N_TRAITS_2(TradeItemChange, UInt32, _itemId, UInt16, _itemNum);

struct TradeLaunchReq
{
	std::string _name;
	std::string _title;
	UInt32 _coin;
	UInt32 _gold;
	std::vector<TradeItemChange> _items;

	TradeLaunchReq() : _coin(0), _gold(0) {}
	MESSAGE_DEF5(REQ::TRADE_LAUNCH, std::string, _name, std::string, _title, UInt32, _coin, UInt32, _gold, std::vector<TradeItemChange>, _items);
};

struct TradeReplyReq
{
	UInt32 _id;
	UInt32 _coin;
	UInt32 _gold;
	std::vector<TradeItemChange> _items;

	TradeReplyReq() : _id(0), _coin(0), _gold(0) {}
	MESSAGE_DEF4(REQ::TRADE_REPLY, UInt32, _id, UInt32, _coin, UInt32, _gold, std::vector<TradeItemChange>, _items);
};

struct TradeOperateReq
{
	UInt32 _id;
	UInt8  _op;

	TradeOperateReq() : _id(0), _op(0) {}
	MESSAGE_DEF2(REQ::TRADE_OPERATE, UInt32, _id, UInt8, _op);
};

S11N_TRAITS_4(GObject::SaleSellData, UInt32, id, UInt16, count, UInt32, price, UInt8, priceType);
struct SaleSellReq
{
	std::vector<GObject::SaleSellData> _data;
	MESSAGE_DEF1(REQ::SALE_SELL, std::vector<GObject::SaleSellData>, _data);
};

void OnTradeListReq(GameMsgHdr& hdr, TradeListReq& req)
{
	MSG_QUERY_PLAYER(player);

	player->GetTrade()->sendTradeList(req._index, req._num);
}

void OnTradeDataReq(GameMsgHdr& hdr, TradeDataReq& req)
{
	MSG_QUERY_PLAYER(player);

	player->GetTrade()->sendTradeData(req._tradeId);
}

void OnTradeLaunchReq(GameMsgHdr& hdr, TradeLaunchReq& req)
{
	 MSG_QUERY_PLAYER(player);
	 if(!player->hasChecked())
		 return;

	 player->GetTrade()->launchTrade(req._name, req._title, req._coin, req._gold, req._items);
}

void OnTradeReplyReq(GameMsgHdr& hdr, TradeReplyReq& req)
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	player->GetTrade()->replyTrade(req._id, req._coin, req._gold, req._items);
}

void OnTradeOperate(GameMsgHdr& hdr, TradeOperateReq& req)
{
	MSG_QUERY_PLAYER(player);

	Trade* trade = player->GetTrade();
	switch(req._op)
	{
	case 1:
		trade->confirmTrade(req._id);
		break;
	case 2:
		trade->cancelTrade(req._id);
		break;
	case 3:
		trade->deleteTrade(req._id);
		break;
	default:
		return;
	}
}


void OnSaleSellReq( GameMsgHdr& hdr, SaleSellReq& req )
{
	MSG_QUERY_PLAYER(player);
    if(GObject::gSaleMgr.getOnOff() == 0)
    {
        return;
    }

	if(!player->hasChecked())
		return;

    if (player->GetLev() < 45)
        return;
    if (player->getForbidSale())
    {
        player->sendMsgCode(0, 1040);  
        return;
    }

	player->GetSale()->sellSaleReq(req._data);
}

void OnMailList2Req( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(hdr.msgHdr.bodyLen < 1)
		return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 cnt;
	brd >> cnt;
	if(hdr.msgHdr.bodyLen < cnt * sizeof(UInt32) + 1)
		return;
	player->GetMailBox()->listMail(cnt, &brd.data<UInt32>(1));
}

void OnMailListReq( GameMsgHdr& hdr, MailListReq& mlr )
{
	MSG_QUERY_PLAYER(player);
	player->GetMailBox()->listMailID(mlr._start, mlr._count);
}

void OnMailReq( GameMsgHdr& hdr, MailReq& mr )
{
	MSG_QUERY_PLAYER(player);
	player->GetMailBox()->readMail(mr._id);
}

void OnMailDelReq( GameMsgHdr& hdr, const void * buffer )
{
	UInt32 blen = hdr.msgHdr.bodyLen;
	if(blen < 1)
		return;
	const UInt8 * buf = reinterpret_cast<const UInt8 *>(buffer);
	UInt8 c = buf[0];
	if(blen < 1 + sizeof(UInt32) * c)
		return;
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	const UInt32 * idlist = reinterpret_cast<const UInt32 *>(buf + 1);

	GObject::MailBox * mailBox = player->GetMailBox();
	//UInt16 omb = mailBox->getNewMails();
	for(UInt8 i = 0; i < c; ++ i)
	{
		mailBox->delMail(idlist[i], true);
	}
	//if(mailBox->getNewMails() != omb)
	//{
	//	mailBox->notifyNewMail();
	//}
	std::vector<UInt8> rep;
	rep.resize(4 + blen);
    *(UInt16*)(&rep[0]) = blen;
    rep[2] = 0xFF;
    rep[3] = REP::MAIL_DELETE;
	memcpy(&rep[4], buffer, blen);
	player->send(&rep[0], rep.size());
	if(c > 0)
		SYSMSG_SEND(139, player);
}

struct MailSendRep
{
	UInt8 _res;

	MESSAGE_DEF1(REP::MAIL_SEND, UInt8, _res);
};

void OnMailSendReq( GameMsgHdr& hdr, MailSendReq& msr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(msr._target)];
	if(pl == NULL)
	{
		MailSendRep rep;
		rep._res = 1;
		player->send(rep);
		return;
	}
	if(player->hasBlock(pl))
	{
		player->sendMsgCode(1, 1500);
		return;
	}
	if(pl->hasBlock(player))
	{
		player->sendMsgCode(1, 1502);
		return;
	}
	MailSendRep rep;
	rep._res = 0;
	pl->GetMailBox()->newMail(player, 0x03, msr._title, msr._content);
}


void OnMailClickReq( GameMsgHdr& hdr, MailClickReq& mcr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	player->GetMailBox()->clickMail(mcr._mailId, mcr._action);
}

void OnFriendListReq( GameMsgHdr& hdr, FriendListReq& flr )
{
	if(flr._type > 4)
		return;
	MSG_QUERY_PLAYER(player);
    if(flr._type == 4)
    {
        struct ClanMemberListReq
        {
            UInt8 _type;
            UInt8 _start;
            UInt8 _count;
        };
        ClanMemberListReq cmlr = {flr._type, flr._start, flr._count};
        GameMsgHdr hdr1(0x1D0, WORKER_THREAD_WORLD, player, sizeof(cmlr));
        GLOBAL().PushMsg(hdr1, &cmlr);
    }
    else
	    player->sendFriendList(flr._type, flr._start, flr._count);
}

void OnFriendOpReq( GameMsgHdr& hdr, FriendOpReq& fr )
{
	MSG_QUERY_PLAYER(player);
    player->patchDeleteDotS(fr._name);
    GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(fr._name)];
    if(pl == NULL || pl == player)
	{
		player->sendMsgCode(0, 1506);
		return;
	}
	switch(fr._op)
	{
	case 1:
		{
			if(!player->testCanAddFriend(pl))
				return;
			SYSMSGV(title, 207, player->getCountry(), player->getName().c_str());
			SYSMSGV(content, 208, player->getCountry(), player->getName().c_str());
			pl->GetMailBox()->newMail(player, 0x13, title, content);
		}
		break;
	case 2:
		player->delFriend(pl);
		pl->delFriend(player);
		break;
	case 3:
		player->addBlock(pl);
		break;
	case 4:
		player->delBlock(pl);
		break;
	case 5:
		player->addFoe(pl);
		break;
	case 6:
		player->delFoe(pl);
		break;
    case 7:
        player->addCFriend(pl);
        break;
    case 8:
        player->delCFriend(pl);
        pl->delCFriend(player);
        break;
	}
}

#if 0
void OnFriendActReq( GameMsgHdr& hdr, FriendActReq& )
{
	MSG_QUERY_PLAYER(player);
	player->sendFriendActList();
}
#endif

#if 0
void OnAthleticsDataReq( GameMsgHdr& hdr, AthleticsDataReq& )
{
	MSG_QUERY_PLAYER(player);
	player->GetAthletics()->notifyAthleticsData(15);
}
#endif

struct BlockBossReq
{
	MESSAGE_DEF(REP::BLOCKBOSS);
};
void OnBlockBossReq( GameMsgHdr& hdr, BlockBossReq& )
{
    return; // TODO:
	MSG_QUERY_PLAYER(player);
	if(player->GetLev() < 30)
		return;
	UInt8 buffer[2];
	*reinterpret_cast<UInt8 *>(buffer) = player->getBlockBossLevel();
	GameMsgHdr hdr1(0x1A5, WORKER_THREAD_WORLD, player, 1);
	GLOBAL().PushMsg(hdr1, buffer);
}

struct AttackBlockBossReq
{
	MESSAGE_DEF(REQ::ATTACK_BLOCKBOSS);
};

void OnAttackBlockBossReq( GameMsgHdr& hdr, AttackBlockBossReq& )
{
    return; // TODO:
	MSG_QUERY_PLAYER(player);
	player->attackBlockBoss();
}

struct PwdQuestion
{
	MESSAGE_DEF(REQ::PWD_QUESTION);
};

void OnPwdQuestionReq( GameMsgHdr& hdr, PwdQuestion& )
{
	MSG_QUERY_PLAYER(player);
	Stream st(REP::SECOND_PWD);
    player->makeSenconPWDInfo(st);
	player->send(st);
}

void OnOpPwdReq( GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 res = 0;
	UInt8 op = 0;
	brd >> op;
	std::string pwd;
	std::string question;
	std::string answer;
	if(op == 0)
	{
		brd >> pwd >> question >> answer;
		res = player->activeSecondPWD(pwd, question, answer);
	}
	else if(op == 1)
	{
		brd >> pwd >> answer;
		res = player->forgetSecondPWD(pwd, answer);
	}
	else if(op == 2)
	{
		brd >> answer;
		res = player->deactiveSecondPWD(answer);
	}
	else
	{
		std::string newPWD;
		brd >> pwd >> newPWD;
		res = player->changeSecondPWD(pwd, newPWD);
	}
	Stream st(REP::OP_PWD);
	st << op << res << Stream::eos;
	player->send(st);
}

struct LockPwdReq
{
	UInt8 flag;
	std::string pwd;
	MESSAGE_DEF2(REQ::PWD_LOCK, UInt8, flag, std::string, pwd);
};

void OnLockPwdReq( GameMsgHdr& hdr, LockPwdReq&  lpd)
{
	MSG_QUERY_PLAYER(player);
	UInt8 res = 0;
	if(lpd.flag == 0)
		res = player->unLockSecondPWD(lpd.pwd);
	else
		player->lockSecondPWD();

	Stream st(REP::PWD_LOCK);
	st << lpd.flag << res << Stream::eos;
	player->send(st);
}

#if 0
void OnFighterTrain2Req( GameMsgHdr& hdr, FighterTrain2Req& req )
{
    return; // TODO:
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	player->addTrainFighter(req._heroID, req._priceType, req._time);
}
#endif

void OnFighterTrainOpReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt32 fighterId = 0;
	UInt8 type = 0;
    UInt8 reqType = 0;
    brd >> reqType;
    if(reqType == 0)
    {
        Stream st;
        player->makeTrainFighterInfo(st);
        player->send(st);
        return;
    }

	if(!player->hasChecked())
		return;

	brd >> fighterId >> type;
	if(fighterId == 0)
		return;
    if(reqType == 1)
    {
        UInt32 hrs = 0;
        brd >> hrs;
        player->addTrainFighter(fighterId, type, hrs);
    }
    else if(reqType == 2)
    {
        switch (type)
        {
        case 0:
            {
                UInt32 hrs = 0;
                brd >> hrs;
                if(hrs > 0)
                    player->accTrainFighter(fighterId, hrs);
            }
            break;
        case 1:
            player->cancelTrainFighter(fighterId);
            break;
        }
    }
}

void OnHeroIslandReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;

    if (PLAYER_DATA(player,location) != 8977)
        return;

    switch (type)
    {
        case 0:
            GObject::heroIsland.playerInfo(player);
            break;
        case 1:
            GObject::heroIsland.playerEnter(player);
            break;
        case 2:
            GObject::heroIsland.getIdentity(player);
            break;
        case 3:
            GObject::heroIsland.startCompass(player);
            break;
        case 4:
            {
                UInt8 spot = 0;
                brd >> spot;
                GObject::heroIsland.moveTo(player, spot);
            }
            break;
        case 5:
            {
                UInt8 atype;
                brd >> atype;
                if (atype == 0)
                {
                    UInt16 id = 0;
                    brd >> id;
                    GObject::heroIsland.attack(player, atype, id);
                }
                else if (atype == 1)
                {
                    UInt64 id = 0;
                    brd >> id;
                    GObject::heroIsland.attack(player, atype, id);
                }
            }
            break;
        case 6:
            //GObject::heroIsland.commitCompass(player);
            break;
        case 7:
            {
                if (!player->hasChecked())
                    return;
                UInt8 skillid = 0;
                UInt8 type = 0;
                brd >> skillid;
                brd >> type;
                GObject::heroIsland.useSkill(player, skillid, type);
            }
            break;
        case 8:
            {
                UInt8 id = 0xFF;
                brd >> id;
                GObject::heroIsland.getAward(player, id, 1);
            }
            break;
        case 9:
            {
                UInt8 start = 0;
                UInt8 pgsz = 0;
                brd >> start;
                brd >> pgsz;
                GObject::heroIsland.listRank(player, start, pgsz);
            }
            break;
        case 10:
            {
                GObject::heroIsland.playerLeave(player);
            }
            break;
        case 11:
            {
                GObject::heroIsland.sendAtoCfg(player);
            }
            break;
        case 12:
            {
                std::string cfg;
                brd >> cfg;
                GObject::heroIsland.saveAtoCfg(player, cfg);
            }
            break;
        case 13:
            {
                if (!player->hasChecked())
                    return;
                UInt8 onoff = 0;
                brd >> onoff;
                GObject::heroIsland.setAto(player, onoff);
            }
            break;
        default:
            break;
    }
}

void OnClanRankBattleReqInit(GameMsgHdr& hdr,const void* data)
{
    MSG_QUERY_PLAYER(player);

	if(CURRENT_THREAD_ID() != WORKER_THREAD_NEUTRAL)
    {
        hdr.msgHdr.desWorkerID = WORKER_THREAD_NEUTRAL;
        GLOBAL().PushMsg(hdr, (void*)data);
        return;
    }

	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;

    switch(type)
    {
        case 0: //请求帮会战状态信息
            {
                ClanRankBattleMgr::Instance().SendInitStatus(player);
            }
            break;
        case 1: //报名进入战斗
            {
                UInt8 field = 0;
                brd >> field;
                ClanRankBattleMgr::Instance().Signup(player, field);
            }
            break;
        case 2: //取消报名
            {
                ClanRankBattleMgr::Instance().Signout(player);
            }
            break;
        default:
            break;
    }
}


void OnClanRankBattleReq(GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);

	if(player->getThreadId() != WORKER_THREAD_NEUTRAL) return;
    if(player->getLocation() != RANK_BATTLE_LOCATION) return;

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;

    switch(type)
    {
        case 0: //请求帮派战状态信息
            {
                ClanRankBattleMgr::Instance().SendBattleStatus(player);
            }
            break;
        case 1: //请求帮派战技能信息
            {
                ClanRankBattleMgr::Instance().SendSkillList(player);
            }
            break;
        case 2: //请求帮派战报名玩家信息
            {
                ClanRankBattleMgr::Instance().SendPlayerList(player);
            }
            break;
        case 3: //设置出战顺序
            {
                UInt64 playerId = 0;
                UInt8 fieldId = 0;
                UInt8 pos = 0;
                brd >> playerId >> fieldId >> pos;
                Player* member = globalPlayers[playerId];
                if(member == NULL) return;
                ClanRankBattleMgr::Instance().AdjustPlayerPos(player, member, fieldId, pos);
            }
            break;
        case 4: //购买使用帮派战技能
            {
                if (!player->hasChecked())
                    return;
                UInt8 skillId = 0;
                brd >> skillId;
                ClanRankBattleMgr::Instance().UseSkill(player, skillId);
            }
            break;
        case 5: //请求战况信息
            {
                ClanRankBattleMgr::Instance().SendBattleInfo(player);
            }
            break;
        case 6: //出战顺序按等级排序
            {
                ClanRankBattleMgr::Instance().SortClanPlayers(player);
            }
            break;
        case 7: //请求战报
            {
                UInt8 fightId = 0;
                brd >> fightId;
                ClanRankBattleMgr::Instance().SendBattleReport(player, fightId);
            }
            break;
        default:
            break;
    }
}

void OnClanRankBattleSortList(GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);

	if(CURRENT_THREAD_ID() != WORKER_THREAD_NEUTRAL)
    {
        hdr.msgHdr.desWorkerID = WORKER_THREAD_NEUTRAL;
        GLOBAL().PushMsg(hdr, (void*)data);
        return;
    }

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt16 startId = 0;
    UInt8 count = 0;
    brd >> startId >> count;

    ClanRankBattleMgr::Instance().SendSortList(player, startId, count);
}

void OnClanCopyReq (GameMsgHdr& hdr, const void * data )
{
    // TODO: 帮派副本系统的请求协议
    MSG_QUERY_PLAYER(player);

	GObject::Clan * clan = player->getClan();
	if(clan == NULL)
	{
		Stream st(REP::CLAN_COPY);
		st << static_cast<UInt8>(0);
		st << Stream::eos;
		player->send(st);
		return;
	}

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 command = 0;
    UInt8 val = 0;
    brd >> type;
    brd >> command;
    switch(type)
    {
        case CLAN_COPY_TAB_INFO:
            // 请求帮派副本信息
            if (command == 0)
                clan->sendClanCopyInfo(player);
            else
            {
                brd >> val;
                clan->clanCopyTabOperate(player, command, val);
            }
            break;
        case CLAN_COPY_MEMBER_LIST_OP:
            // 帮派副本成员操作
            clan->clanCopyMemberOperate(player, command, brd);
            break;
        case CLAN_COPY_BATTLE:
            // 帮派副本的战斗操作
            clan->clanCopyBattleOperate(player, command, brd);
            break;
        default:
                break;
    }
}


void OnPracticeHookAddReq( GameMsgHdr& hdr, PracticeHookAddReq& req)
{
    MSG_QUERY_PLAYER(player);
    if (!player->hasChecked())
        return;
    player->accPractice();
}

void OnRefreshMartialReq( GameMsgHdr& hdr, AthleticsRefreshMartialReq& req )
{
    MSG_QUERY_PLAYER(player);
    if (!player->hasChecked())
        return;
#if 0
    if(player->getTael() < 100)
        return;

    ConsumeInfo ci(FlushAthletics,0,0);
    player->useTael(100, &ci);

    GameMsgHdr hdr2(0x1F1, WORKER_THREAD_WORLD, player, 0);
    GLOBAL().PushMsg(hdr2, NULL);
#endif
    //GameMsgHdr hdr2(0x1F8, WORKER_THREAD_WORLD, player, 1);
    //GLOBAL().PushMsg(hdr2, &(req._type));
    player->GetAthletics()->updateAthleticsP(player, req._type);
}

void OnTrumpUpgrade( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 res = 0;
    UInt16 fgtId = 0;
    UInt32 trumpId = 0;
    UInt16 itemCnt = 0;
    UInt32 amount = 0;

	Package * pkg = player->GetPackage();

    br >> fgtId >> trumpId >> itemCnt;
	for(UInt16 i = 0; i < itemCnt; ++ i)
	{

		UInt32 itemId;
        UInt8 bind = 0;
        UInt16 num = 1;
		br >> itemId;
        if(itemId < 30000)
            br >> bind >> num;

        for(int j = 0; j < num; ++ j)
        {
            amount += GData::moneyNeed[GData::TRUMPUPGRADE].tael;
            if(player->getTael() < amount)
            {
                res = 3;
                amount -= GData::moneyNeed[GData::TRUMPUPGRADE].tael;
                break;
            }

            res = pkg->TrumpUpgrade(fgtId, trumpId, itemId, bind);
            if(res == 2)
            {
                if( i > 0 )
                    res = 0;
                amount -= GData::moneyNeed[GData::TRUMPUPGRADE].tael;
                break;
            }
        }
	}

    ConsumeInfo ci(TrumpUpgrade,0,0);
    player->useTael(amount, &ci);

	Stream st(REP::EQ_TRUMP_UPGRADE);
	st << res << fgtId << trumpId << Stream::eos;
	player->send(st);
}

void OnTrumpLOrder( GameMsgHdr& hdr, TrumpLOrderReq& req)
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;

    UInt8 res = 0;

    UInt32 amount = GData::moneyNeed[GData::TRUMPLORDER].tael;
    if(player->getTael() < amount)
    {
        player->sendMsgCode(0, 1100);
        return;
    }

	Package * pkg = player->GetPackage();
    res = pkg->TrumpLOrder(req._fgtId, req._itemId);
    if(res != 2)
    {
        ConsumeInfo ci(TrumpLOrder,0,0);
        player->useTael(amount, &ci);
        GameAction()->doStrong(player, SthTrumpLOrder, 0, 0);
    }

	Stream st(REP::EQ_TRUMP_L_ORDER);
	st << res << req._fgtId << req._itemId << Stream::eos;
	player->send(st);
}

void OnEquipUpgrade( GameMsgHdr& hdr, EquipUpgradeReq& req)
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;
    Package * pkg = player->GetPackage();
    UInt32 newId = 0;
    UInt16 fid = req._fgtId;
    UInt8 res = pkg->EquipUpgrade(req._fgtId, req._itemId,  &newId , &fid);

    Stream st(REP::EQ_UPGRADE);
    if(res == 0)
        st << res << fid << newId << Stream::eos;
    else
        st << res << fid << req._itemId << Stream::eos;

    player->send(st);
}

void OnEquipSpirit( GameMsgHdr& hdr, EquipSpiritReq& req)
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;
    Package * pkg = player->GetPackage();
    pkg->AttachSpirit(req._type, req._fgtId, req._itemId);
}

void OnActivityList( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    //BinaryReader brd(data, hdr.msgHdr.bodyLen);
    //ActivityMgr* mgr = player->GetActivityMgr();
    //mgr->ActivityList(7);

}
void OnActivityReward(  GameMsgHdr& hdr, const void * data)
{
    /*
    MSG_QUERY_PLAYER(player);
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    ActivityMgr* mgr = player->GetActivityMgr();
    UInt8 type = 0;
    brd >> type;
    switch(type )
    {
        case 0:
            if (!player->hasChecked())
                return;
            //mgr->ChangeOnlineReward();
            break;

        case 1:
            // getDailyReward
            //mgr->GetReward(2);
            break;
        case 2:
            UInt16 flag = 0;
            brd >> flag;
            mgr->GetReward(flag);
            break;

    }
    */
    MSG_QUERY_PLAYER(player);
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;
    switch(type )
    {
        case 3:
            if (World::getRechargeActive())
                player->sendRechargeInfo();
            break;
        case 4:
            if (!World::getRechargeActive())
                return;
            UInt32 itemId = 0;
            brd >> itemId;
            int n = -1;
            UInt8 res = GObject::RechargeTmpl::instance().getItem(player, itemId, n);
            Stream st(REP::ACTIVITY_REWARD);
            st << static_cast<UInt8>(11);
            st << res;
            if ( 0 == res)
            {
                st << player->GetVar(VAR_RECHARGE_SCORE) << itemId << n;
            }
            st << Stream::eos;
            player->send(st);
            break;

    }
 
}

void OnFourCopReq( GameMsgHdr& hdr, const void* data)
{
    if(!World::getFourCopAct())
        return;
    MSG_QUERY_PLAYER(pl);
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 opt = 0;
    br >> type;
    br >> opt;

    /** 赠送神捕令 **/
    if(0 == type)
    {
        UInt16 count = 0;
        UInt16 tmpCnt = 0;
        br >> count;

        if(count > pl->GetPackage()->GetItemAnyNum(9209))
        {
            //sendMsgCode(0, 1011);
            return;
        }
        pl->GetPackage()->DelItemAny(9209, count);
        switch(opt)
        {
            case 1:
                pl->AddVar(VAR_LX_CNT, count);
                tmpCnt = pl->GetVar(VAR_LX_CNT);
            break;
            case 2:
                pl->AddVar(VAR_WQ_CNT, count);
                tmpCnt = pl->GetVar(VAR_WQ_CNT);
            break;
            case 3:
                pl->AddVar(VAR_TS_CNT, count);
                tmpCnt = pl->GetVar(VAR_TS_CNT);
            break;
            case 4:
                pl->AddVar(VAR_ZM_CNT, count);
                tmpCnt = pl->GetVar(VAR_ZM_CNT);
            break;
            default:
            break;
        }
        Stream st(REP::FOURCOP);
        st << opt << tmpCnt << Stream::eos;
        pl->send(st);
    }
    /** 点击宝箱领取奖励 **/
    else
    {
        if(pl->GetPackage()->GetRestPackageSize() < 1)
        {
            pl->sendMsgCode(0, 1011);
            return;
        }
        UInt16 tmpCnt;
        switch(opt)
        {
            case 1:
                tmpCnt = pl->GetVar(VAR_LX_CNT);
                if(tmpCnt >= 10)
                {
                    tmpCnt -= 10;
                    pl->SetVar(VAR_LX_CNT, tmpCnt);
                    pl->GetPackage()->AddItem(9210, 1, true);
                }
            break;
            case 2:
                tmpCnt = pl->GetVar(VAR_WQ_CNT);
                if(tmpCnt >= 10)
                {
                    tmpCnt -= 10;
                    pl->SetVar(VAR_WQ_CNT, tmpCnt);
                    pl->GetPackage()->AddItem(9211, 1, true);
                }
            break;
            case 3:
                tmpCnt = pl->GetVar(VAR_TS_CNT);
                if(tmpCnt >= 10)
                {
                    tmpCnt -= 10;
                    pl->SetVar(VAR_TS_CNT, tmpCnt);
                    pl->GetPackage()->AddItem(9212, 1, true);
                }
            break;
            case 4:
                tmpCnt = pl->GetVar(VAR_ZM_CNT);
                if(tmpCnt >= 10)
                {
                    tmpCnt -= 10;
                    pl->SetVar(VAR_ZM_CNT, tmpCnt);
                    pl->GetPackage()->AddItem(9213, 1, true);
                }
            break;
            default:
            break;
        }
        Stream st(REP::FOURCOP);
        st << opt << tmpCnt << Stream::eos;
        pl->send(st);
    }
}
void OnTianjieReq( GameMsgHdr& hdr, const void* data)
{
    GObject::Tianjie::instance().onTianjieReq(hdr, data);
}

void OnTeamCopyReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    if (player->isJumpingMap())
        return;

    switch(op)
    {
    case 0x0:
        {
            if(!player->hasChecked())
                return;

            bool res = 0;
            Stream st(REP::TEAM_COPY_REQ);
            st << static_cast<UInt8>(0x00);

            UInt8 resCopyId = 0;
            UInt8 resT = 0;
            UInt8 type = 0;
            br >> type;
            if(type == 0)
            {
                UInt8 copyId = 0;
                UInt8 t = 0;
                br >> copyId >> t;
                res = teamCopyManager->enterTeamCopy(player, copyId, t);
                if(res)
                {
                    resCopyId = copyId;
                    resT = t;
                }
            }
            else
                res = teamCopyManager->leaveTeamCopy(player);

            st << resCopyId << resT;
            st << Stream::eos;
            player->send(st);
        }
        break;
    case 0x1:
        {
            UInt32 start = 0;
            UInt8 count = 0;
            UInt8 type = 0;
            br >> start >> count >> type;
            teamCopyManager->reqTeamList(player, start, count, type);
        }
        break;
    case 0x2:
        {
            if(!player->hasChecked())
                return;

            std::string pwd;
            UInt8 upLevel = 0;
            UInt8 dnLevel = 0;

            if(br.data<UInt16>(br.pos()) > 6)
                break;

            br >> pwd;
            br >> dnLevel >> upLevel;

            Stream st(REP::TEAM_COPY_REQ);
            st << static_cast<UInt8>(0x02);

            UInt32 teamId = teamCopyManager->createTeam(player, pwd, upLevel, dnLevel);
            st << teamId;

            st << Stream::eos;
            player->send(st);
        }
        break;
    case 0x3:
        {
            if(!player->hasChecked())
                return;

            UInt32 teamId = 0;
            std::string pwd;
            br >> teamId;

            if(br.data<UInt16>(br.pos()) > 6)
                break;

            br >> pwd;
            Stream st(REP::TEAM_COPY_REQ);
            st << static_cast<UInt8>(0x03);

            UInt32 teamId2 = teamCopyManager->joinTeam(player, teamId, pwd);
            st << teamId2;

            st << Stream::eos;
            player->send(st);
        }
        break;
    case 0x4:
        {
            teamCopyManager->leaveTeam(player);
        }
        break;
    case 0x5:
        {
            if(!player->hasChecked())
                return;

            Stream st(REP::TEAM_COPY_REQ);
            st << static_cast<UInt8>(0x05);
            bool res = teamCopyManager->quikJoinTeam(player);
            st << static_cast<UInt8>(res ? 1 : 0) << Stream::eos;
            player->send(st);
        }
        break;
    case 0x10:
        {
            teamCopyManager->reqTeamInfo(player);
        }
        break;
    case 0x11:
        {
            UInt64 playerId = 0;
            br >> playerId;
            teamCopyManager->handoverLeader(player, playerId);
        }
        break;
    case 0x12:
        {
            UInt64 playerId = 0;
            br >> playerId;
            teamCopyManager->teamKick(player, playerId);
        }
        break;
    case 0x13:
        {
            UInt8 idx0 = 0;
            UInt8 idx1 = 0;
            //UInt8 idx2 = 0;
            br >> idx0 >> idx1;  // >> idx2;
            teamCopyManager->reQueueTeam(player, idx0, idx1);
        }
        break;
    case 0x14:
        {
            UInt8 type = 0;
            br >> type;
            teamCopyManager->teamBattleStart(player, type);
        }
        break;
    case 0x0F:
        {
            TeamCopyPlayerInfo* tcpInfo = player->getTeamCopyPlayerInfo();
            tcpInfo->reqTeamCopyInfo();
        }
        break;
    case 0x0D:
        {
            if(!player->hasChecked())
                return;

            UInt8 type = 0;
            br >> type;
            TeamCopyPlayerInfo* tcpInfo = player->getTeamCopyPlayerInfo();
            tcpInfo->rollAward(type);
        }
        break;
    case 0x0E:
        {
            if(!player->hasChecked())
                return;

            TeamCopyPlayerInfo* tcpInfo = player->getTeamCopyPlayerInfo();
            bool res = tcpInfo->getAward();
            Stream st(REP::TEAM_COPY_REQ);
            st << static_cast<UInt8>(0x0E) << static_cast<UInt8>(res ? 1 : 0) << Stream::eos;
            player->send(st);
        }
        break;
    default:
        return;
    }
}

void OnNewRelationReq( GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(pl);
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 mood = 0;
    std::string status;
    br >> type;

    if(type > 5)
        return;

    Stream st(REP::NEWRELATION);
    st << type;
    /** 关系 **/
    switch(type)
    {
        case 0:
            st << pl->GetNewRelation()->getMood();
            break;
        case 1:
            br >> mood;
            st << mood;
            pl->GetNewRelation()->setMood(mood);
            break;
        case 2:
            st << pl->GetNewRelation()->getSign();
            break;
        case 3:
            br >> status;
            st << status;
            pl->GetNewRelation()->setSign(status);
            break;
        case 4:
            br >> status;
            pl->GetNewRelation()->challengeRequest(pl, status);
            return;//isn't break
        case 5:
            br >> status;
            br >> mood;
            pl->GetNewRelation()->challengeRespond(pl, status, mood);
            return;//isn't break
        default:
            break;
    }

    st << Stream::eos;
    pl->send(st);
}

#if 0
void OnTownDeamonReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    if(op !=0x08 && (PLAYER_DATA(player, location) != 0x1414 || player->GetLev() < 40))
        return;

    if(op != 0x08)
    {
       townDeamonManager->checkStartTime(player);
    }

    switch(op)
    {
    case 0x00:
        {
            townDeamonManager->showTown(player);
        }
        break;
    case 0x01:
        {
            UInt16 level = 0;
            br >> level;
            townDeamonManager->showLevelTown(player, level);
        }
        break;
    case 0x02:
        {
            UInt16 start = 0;
            UInt16 count = 0;
            br >> start >> count;
            townDeamonManager->listDeamons(player, start, count);
        }
        break;
    case 0x03:
        {
            if(!player->hasChecked())
                return;

            UInt8 count = 0;
            br >> count;
            townDeamonManager->useAccItem(player, count);
        }
        break;
    case 0x04:
        {
            if(!player->hasChecked())
                return;

            UInt8 count = 0;
            br >> count;
            townDeamonManager->useVitalityItem(player, count);
        }
        break;
    case 0x05:
        {
            if(!player->hasChecked())
                return;

            UInt16 level = 0;
            UInt8 type = 0;
            br >> level >> type;
            townDeamonManager->challenge(player, level, type);
        }
        break;
    case 0x06:
        {
            if(!player->hasChecked())
                return;

            townDeamonManager->cancelDeamon(player);
        }
        break;
    case 0x07:
        {
            if(!player->hasChecked())
                return;

            UInt16 levels = 0;
            br >> levels;
            townDeamonManager->autoCompleteQuite(player, levels);
        }
        break;
    case 0x08:
        {
            player->getDeamonAwards();
        }
    default:
        return;
    }
}
#endif

void OnGetHeroMemoAward( GameMsgHdr& hdr, GetHeroMemoAward& req)
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;
    player->GetHeroMemo()->getAward(req._idx);
}

void OnGetShuoShuoAward( GameMsgHdr& hdr, GetShuoShuoAward& req)
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;
    player->GetShuoShuo()->getAward(req._idx);
}

void OnGetCFriendAward( GameMsgHdr& hdr, GetCFriendAward& req )
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;
    if (req._flag == 1)
        player->GetCFriend()->setCFriendNum(req._idx);
    else
        player->GetCFriend()->getAward(req._idx);
}

void OnGetOfflineExp( GameMsgHdr& hdr, GetOfflineExp& req )
{
    MSG_QUERY_PLAYER(player);
    player->getOfflineExp();
}

void OnSecondSoulReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    switch(op)
    {
    case 0x00:
        {
            UInt16 fighterId = 0;
            br >> fighterId;
            GObject::Fighter * fgt = player->findFighter(fighterId);
            if(fgt)
                fgt->send2ndSoulInfo();
        }
        break;
    case 0x01:
        {
            UInt16 fighterId = 0;
            UInt8 cls = 0;
            br >> fighterId >> cls;
            GObject::Fighter * fgt = player->findFighter(fighterId);
            if(fgt)
            {
                bool res = false;
                res = fgt->openSecondSoul(cls);
                Stream st(REP::SECOND_SOUL);
                st << static_cast<UInt8>(1);
                st << fighterId << static_cast<UInt8>(res ? 0 : 1) << Stream::eos;
                player->send(st);
            }
        }
        break;
    case 0x02:
        {
            UInt16 fighterId = 0;
            br >> fighterId;
            GObject::Fighter * fgt = player->findFighter(fighterId);
            if(fgt)
            {
                fgt->practiceLevelUp();
            }
        }
        break;
    case 0x03:
        {
            UInt16 fighterId = 0;
            UInt16 itemNum = 0;
            br >> fighterId >> itemNum;

            GObject::Fighter * fgt = player->findFighter(fighterId);
            if(!fgt)
                break;

            std::vector<SoulItemExp> soulItemExpOut;
            for(int i = 0; i < itemNum; ++ i)
            {
                UInt16 itemId = 0;
                UInt8 bind = 0;
                br >> itemId >> bind;
                if(!fgt->enchantSoul(itemId, bind != 0, soulItemExpOut))
                    break;
            }

            UInt16 infoNum = soulItemExpOut.size();
            Stream st(REP::SECOND_SOUL);
            st << static_cast<UInt8>(3);
            st << fighterId << infoNum;
            for( int j = 0; j < infoNum; ++ j)
            {
                st << soulItemExpOut[j].itemId << soulItemExpOut[j].res << soulItemExpOut[j].exp;
                player->GetPackage()->secondSoulItemUdpLog(soulItemExpOut[j].res + 1, soulItemExpOut[j].itemId, 1);
            }
            st << Stream::eos;
            player->send(st);
        }
        break;
    case 0x04:
        {
            UInt16 fighterId = 0;
            UInt16 itemId1 = 0;
            UInt16 itemId2 = 0;
            UInt8 bind = 0;

            br >> fighterId >> itemId1 >> itemId2 >> bind;
            GObject::Fighter * fgt = player->findFighter(fighterId);
            if(!fgt)
                break;

            UInt8 idx = fgt->getSoulSkillIdx(itemId1);
            if(idx == 0xFF)
                break;

            bool res = false;
            res = fgt->equipSoulSkill(idx, itemId2, bind != 0);

            Stream st(REP::SECOND_SOUL);
            st << static_cast<UInt8>(4);
            st << fighterId << static_cast<UInt8>(res ? 0 : 1) << Stream::eos;
            st << Stream::eos;
            player->send(st);
        }
        break;
    }
}

void OnUseToken( GameMsgHdr& hdr, UseToken& req )
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;

    if (World::getTrumpEnchRet() || World::get9215Act())
    {
        player->useToken(req._type);
    }
}

void OnMDSoul( GameMsgHdr& hdr, UseMDSoul& req )
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;

    if (World::getMayDay() || World::getCompassAct())
    {
        if (req._type == 0)
            player->sendMDSoul(0);
        else if (req._type == 1)
            player->getMDItem();
        else if (req._type == 2)
            player->useMDSoul();
    }
}

void OnSvrSt( GameMsgHdr& hdr, SvrSt& req )
{
    MSG_QUERY_PLAYER(player);
    player->svrSt(req._type);
}

void OnRC7Day( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;

    // XXX: 不使用老版本新注册七日活动
    //return; // XXX: 不使用老版本新注册七日活动

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    if (op !=6 && op !=7 )
        return;

    switch(op)
    {
        case 1:
        case 2:
        case 3:
            player->getContinuousReward(op);
            break;

        case 4:
            {
                UInt8 idx = 0;
                br >> idx;
                player->getContinuousReward(op, idx);
            }
            break;

        case 5:
            player->turnOnRC7Day();
            break;

        case 6:
            player->getYearRPPackage();
            break;
        case 7:
            player->getYearRPReward();
            break;

        default:
            break;
    }
}

void OnRF7Day( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
         return;

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    switch (op)
    {
        case 1:
        case 2:
        case 3:
            player->getContinuousRewardRF(op);
            break;

        case 4:
            {
                UInt8 idx = 0;
                br >> idx;
                player->getContinuousRewardRF(op, idx);
            }
            break;

        case 5:
            player->turnOnRF7Day();
            break;

        default:
            break;
    }
}

void OnNewRC7Day(GameMsgHdr& hdr, const void* data )
{
    // 新版注册七日活动
	MSG_QUERY_PLAYER(player);

    if (!World::getRC7Day())
        return;

    UInt32 now = TimeUtil::Now();
    UInt32 now_sharp = TimeUtil::SharpDay(0, now);
    UInt32 created_sharp = TimeUtil::SharpDay(0, player->getCreated());
    if (created_sharp > now_sharp)
        return; // 创建时间错误（穿越了）

    if (now_sharp - created_sharp > 7 * 24*60*60)
        return; // 玩家注册时间超过7日，无法参与活动

#define CREATE_OFFSET(c, n) (((n) - (c)) / (24*60*60))
    UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
#undef CREATE_OFFSET
    if (off >= 7)
        return; // 玩家注册时间超过7日，无法参与活动
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    switch(op)
    {
        case 0:
            // 刷新新注册七日活动页面信息
            player->sendNewRC7DayInfo();
            break;
        case 1:
            // 登录签到
            {
                UInt8 val = 0;
                br >> val;
                player->getNewRC7DayLoginAward(val, off);
            }
            break;
        case 2:
            // 神龙许愿（新注册充值奖励）
            {
                UInt8 val = 0;
                br >> val;
                player->getNewRC7DayRechargeAward(val);
            }
            break;
        case 3:
            // 每日目标抽奖
            {
                UInt8 val = 0;
                br >> val;
                player->getNewRC7DayTargetAward(val);
            }
            break;
        default:
            break;
    }
}

void OnYBBuf( GameMsgHdr& hdr, YBBuf& req )
{
    MSG_QUERY_PLAYER(player);
    player->recvYBBuf(req._type);
}

void OnGetAward( GameMsgHdr& hdr, GetAward& req )
{
    MSG_QUERY_PLAYER(player);
    player->getAward(req._type, req._opt);
}

void OnGuideUdp( GameMsgHdr& hdr, GuideUdp& req )
{
    MSG_QUERY_PLAYER(player);
    player->guideUdp(req._type, req.p1, req.p2);
}

void OnActivitySignIn( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    ActivityMgr* mgr = player->GetActivityMgr();
    mgr->CheckTimeOver();
    UInt8 type = 0;
    brd >> type;
    switch(type)
    {
        case 0x00:
            {
                //每日签到
                mgr->ActivitySignIn();
            }
            break;
        case 0x01:
            {
                //刷新待兑换的道具
                mgr->RefreshProps();
            }
            break;
        case 0x02:
            {
                //积分兑换道具
                mgr->ExchangeProps();
            }
            break;
        case 0x03:
            {
                //积分兑换道具
                mgr->SendActivityInfo();
            }
            break;
        default:
            return;
            break;

    }
    //player->send(st);
}

void OnSkillStrengthen( GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(pl);
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt32 fighterid = 0;
    br >> type;
    br >> fighterid;

    Fighter* fgt = pl->findFighter(fighterid);
    if (!fgt)
        return;
    if (type == 1)
    {
        UInt16 skillid = 0;
        br >> skillid;
        fgt->SSOpen(skillid);
    }
    else if (type == 2)
    {
        UInt16 skillid = 0;
        UInt16 num = 0;
        br >> skillid;
        br >> num;

        for (UInt16 i = 0; i < num; ++i)
        {
            UInt32 itemid = 0;
            UInt16 itemnum = 0;
            UInt8 bind = 0;
            br >> itemid;
            br >> itemnum;
            br >> bind;

            if (!fgt->SSUpgrade(skillid, itemid, itemnum, bind))
                break;
        }
    }
}

void OnMakeStrong( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    StrengthenMgr* mgr = player->GetStrengthenMgr();
    mgr->CheckTimeOver();
    UInt8 type = 0;
    brd >> type;
    switch(type)
    {
        case 0x01: //变强之魂信息改变
            {
                mgr->SendStrengthenInfo();
            }
            break;
        case 0x02: //变强之路信息
            {
                mgr->SendStrengthenRank();
            }
            break;
        case 0x03: //请求及打开变强秘宝宝箱
            {
                UInt8 boxId = 0;
                UInt8 index = 0;
                brd >> boxId;
                brd >> index;
                if(boxId > 8 || index > STRONGBOX_CNT)
                    return;
                mgr->SendOpenChestsInfo(boxId, index);
            }
            break;
        case 0x04: //每天一次变强之吼
            {
                mgr->EveryDayRoar();
            }
            break;
        default:
            return;
            break;
    }
}

void OnExJob( GameMsgHdr & hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    br >> type;
    if (type == 0)
    {
        player->SetVar(VAR_EX_JOB_ENABLE, 2);
        return;
    }

    JobHunter * jobHunter = player->getJobHunter();
    if (!jobHunter)
        return;
    switch (type)
    {
        case 1:
            // 墨家长老页面
            {
                if (br.left() == 0)
                {
                    // 刷新页面请求
                    jobHunter->SendFighterList();
                    return;
                }
                UInt16 fighterId = 0;
                br >> fighterId;
                jobHunter->OnHireFighter(fighterId);
            }
            break;
        case 2:
            // 寻墨页面
            {
                if (br.left() == 0)
                {
                    jobHunter->SendGameInfo(type);
                    return;
                }
                UInt8 val = 0;
                br >> val;
                switch (val)
                {
                    case 0:
                        // 放弃寻墨游戏
                        jobHunter->OnAutoStop();
                        jobHunter->OnAbort(false);
                        break;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        jobHunter->OnRequestStart(val);
                        break;
                    case 5: 
                        // 老虎机转盘转动
                        jobHunter->OnUpdateSlot();
                        break;
                    default:
                        break;
                }
            }
            break;
        case 3:
            {
                UInt16 fId = 0;
                UInt16 tId = 0;
                UInt8 t = 0;
                br >> fId;
                br >> tId;
                br >> t;
                Stream st(REP::EXJOB);
                UInt8 res = 0;
                res = player->fightTransform(fId, tId, t);
                st << type << res << Stream::eos;
                player->send(st);
            }

            break;
        default:
            break;
    }
}

void OnJobHunter( GameMsgHdr & hdr, const void * data )
{
    MSG_QUERY_PLAYER(player);
    BinaryReader br(data, hdr.msgHdr.bodyLen);

    JobHunter * jobHunter = player->getJobHunter();
    if (!jobHunter)
        return;

    UInt8 type = 0;
    UInt8 val = 0;
    UInt8 val2 = 0;
    br >> type;
    if (br.left())
        br >> val;
    if (br.left())
        br >> val2;
    jobHunter->OnCommand(type, val, val2);
}

void OnAutoJobHunter( GameMsgHdr & hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    BinaryReader br(data, hdr.msgHdr.bodyLen);

    JobHunter * jobHunter = player->getJobHunter();
    if (!jobHunter)
        return;

    UInt8 type = 0;
    br >> type;
    jobHunter->OnAutoCommand(type);
}

void OnEquipLingbaoReq( GameMsgHdr & hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 opt = 0;
    br >> opt;

	Package * pkg = player->GetPackage();

    UInt8 res = 0;
    Stream st(REP::EQ_LINGBAO);
    st << opt;
    switch(opt)
    {
    case 1:
        {
            UInt32 equipId = 0;
            UInt8 protect = 0;
            UInt8 bind = 0;
            std::vector<UInt16> values;
            br >> equipId >> protect >> bind;
            res = pkg->Tongling(equipId, protect, bind, values);
            UInt8 cnt = values.size();
            st << res;
            st << cnt;
            for(UInt8 i = 0; i < cnt; ++ i)
            {
                st << values[i];
            }
            st << Stream::eos;
            player->send(st);
        }
        break;
    case 2:
        {
            UInt16 gujiId = 0; // 古籍id
            UInt8 type = 0; // 是否使用高级空宝具
            br >> gujiId >> type;
            res = pkg->OpenLingbaoSmelt(gujiId, type);
            st << res << Stream::eos;
            player->send(st);
        }
        break;
    case 3:
        {
            UInt16 count = 0;
            br >> count;
            UInt8 res = 0;
            for(int i = 0; i < count; ++ i)
            {
                UInt16 cnt = 0;
                br >> cnt;
                UInt32 itemId = 0;
                br >> itemId;
                res = pkg->LingbaoSmelt(itemId, cnt);

                if(res != 0)
                    break;
            }
            pkg->sendLingbaoSmeltInfo();
        }
        break;
    case 4:
        {
            pkg->closeLingbaoSmelt();
        }
        break;
    case 5:
        {
            pkg->sendLingbaoSmeltInfo();
        }
        break;
    case 6:
        {
            pkg->FinishLBSmelt();
        }
        break;
    }
}

void OnDreamer( GameMsgHdr & hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    BinaryReader br(data, hdr.msgHdr.bodyLen);

    Dreamer * dreamer = player->getDreamer();
    if (!dreamer)
        return;

    UInt8 type = 0;
    br >> type;
    UInt8 val  = 0xFF;
    UInt8 val2 = 0xFF;
    if (br.left())
        br >> val;
    if (br.left())
        br >> val2;
    dreamer->OnCommand(type, val, val2);
}



#endif // _COUNTRYOUTERMSGHANDLER_H_

