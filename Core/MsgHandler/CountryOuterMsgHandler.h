#ifndef _COUNTRYOUTERMSGHANDLER_H_
#define _COUNTRYOUTERMSGHANDLER_H_

#include "MsgTypes.h"

#include "GData/Store.h"
#include "GData/ExpTable.h"
#include "GData/LootTable.h"
#include "GObject/GObjectManager.h"
#include "GObject/Boss.h"
#include "GObject/Clan.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GObject/CountryBattle.h"
#include "GObject/Mail.h"
#include "GObject/Map.h"
#include "GObject/MapCollection.h"
#include "GObject/MapObject.h"
#include "GObject/MOAction.h"
#include "GObject/Package.h"
#include "GObject/Trade.h"
#include "GObject/TaskMgr.h"
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

#include "Common/Serialize.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/BinaryReader.h"

struct NullReq
{
	UInt32 ticket;
	MESSAGE_DEF1(0x00, UInt32, ticket);
};

struct PlayerInfoReq
{
	MESSAGE_DEF(0x14);
};

struct PurchaseBookReq
{
	UInt8 _pos;
	MESSAGE_DEF1(0x1B, UInt8, _pos);
};

struct StatusChangeReq
{
	UInt8 _id;
	MESSAGE_DEF1(0x16, UInt8, _id);
};

struct FighterEquipReq
{
	UInt32 _id;
	UInt8 _part;
	UInt32 _equipId;
	MESSAGE_DEF3(0x21, UInt32, _id, UInt8, _part, UInt32, _equipId);
};

struct RecruitFighterReq
{
	UInt8 _pos;
	MESSAGE_DEF1(0x22, UInt8, _pos);
};

struct FighterDismissReq
{
	UInt32	_fgtid;
	MESSAGE_DEF1(0x27, UInt32, _fgtid);
};

struct FighterRegenReq
{
	UInt32	_fgtid;
	MESSAGE_DEF1(0x29, UInt32, _fgtid);
};

struct FighterTrainReq
{
	UInt32 _fgtId;
	UInt8 _type;
	MESSAGE_DEF2(0x2C, UInt32, _fgtId, UInt8, _type);
};

struct TakeOnlineRewardReq
{
	MESSAGE_DEF(0x38);
};

struct LuckyDrawInfoReq
{
	MESSAGE_DEF(0x3D);
};

struct LuckyDrawReq
{
	UInt8 _type;
	UInt8 _times;
	MESSAGE_DEF2(0x3E, UInt8, _type, UInt8, _times);
};

struct EnchantReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt8 _type;
	UInt8 _protect;
	MESSAGE_DEF4(0x40, UInt16, _fighterId, UInt32, _itemid, UInt8, _type, UInt8, _protect);
};

struct OpenSocketReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	MESSAGE_DEF2(0x41, UInt16, _fighterId, UInt32, _itemid);
};

struct MergeGemReq
{
	UInt16 _itemid;
	UInt8 _bindNum;
	UInt8 _protect;
	MESSAGE_DEF3(0x42, UInt16, _itemid, UInt8, _bindNum, UInt8, _protect);
};

struct AttachGemReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt16 _gemid;
	UInt8 _bind;
	MESSAGE_DEF4(0x43, UInt16, _fighterId, UInt32, _itemid, UInt16, _gemid, UInt8, _bind);
};

struct DetachGemReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt8 _pos;
	UInt8 _protect;
	MESSAGE_DEF4(0x44, UInt16, _fighterId, UInt32, _itemid, UInt8, _pos, UInt8, _protect);
};

#if 0
struct SplitReq
{
	UInt32 _itemid;
	UInt8 _protect;
	MESSAGE_DEF2(0x45, UInt32, _itemid, UInt8, _protect);
};
#endif

#if 0
struct ExchangeReq
{
	typedef Array<UInt32, 5> ExchangeItemType;
	UInt32 _itemid[5];
	MESSAGE_DEF1(0x46, ExchangeItemType, _itemid);
};
#endif

struct ForgeReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt8 _type;
	UInt8 _protect;
	MESSAGE_DEF4(0x47, UInt16, _fighterId, UInt32, _itemid, UInt8, _type, UInt8, _protect);
};

struct ForgeAnswerReq
{
	UInt16 _fighterId;
	UInt32 _itemid;
	UInt8 _answer;
	MESSAGE_DEF3(0x48, UInt16, _fighterId, UInt32, _itemid, UInt8, _answer);
};

#if 0
struct ExchangeSetReq
{
	typedef Array<UInt32, 3> ExchangeSetItemType;
	UInt32 _itemid[3];
	UInt8 _flag;
	MESSAGE_DEF2(0x4A, ExchangeSetItemType, _itemid, UInt8, _flag);
};
#endif

#if 0
struct ActivateAttrReq
{
	UInt16 _fighterId;
	UInt32 _itemId;
	UInt32 _itemId2;
	MESSAGE_DEF3(0x4C, UInt16, _fighterId, UInt32, _itemId, UInt32, _itemId2);
};
#endif

struct OutCitySwitchStruct
{
	UInt8 _mapid;

	MESSAGE_DEF1(0x50, UInt8, _mapid);
};

struct InCitySwitchStruct
{
	UInt16 _locid;

	MESSAGE_DEF1(0x51, UInt16, _locid);
};

struct CityTransportReq
{
	UInt16 _locid;
	UInt8 flag;

	MESSAGE_DEF2(0x52, UInt16, _locid, UInt8, flag);
};

struct DungeonOpReq
{
	UInt8 op;
	UInt8 type;
	UInt8 difficulty;
	MESSAGE_DEF3(0x58, UInt8, op, UInt8, type, UInt8, difficulty);
};

struct DungeonInfoReq
{
	UInt8 op;
	UInt8 type;
	UInt8 difficulty;
	MESSAGE_DEF3(0x59, UInt8, op, UInt8, type, UInt8, difficulty);
};

struct DungeonBattleReq
{
	UInt8 type;
	UInt8 difficulty;
	UInt8 level;
	MESSAGE_DEF3(0x5A, UInt8, type, UInt8, difficulty, UInt8, level);
};

struct DungeonAutoReq
{
	UInt8 type;
	MESSAGE_DEF1(0x5B, UInt8, type);
};

struct DungeonCompleteAutoReq
{
	MESSAGE_DEF(0x5C);
};

struct DailyReq
{
	MESSAGE_DEF(0x5F);
};

struct ChallengePlayerReq
{
	std::string _name;
	MESSAGE_DEF1(0x60, std::string, _name);
};

struct AttackNpcReq
{
	UInt32 _npcId;
	MESSAGE_DEF1(0x61, UInt32, _npcId);
};

struct AutoBattleReq
{
	UInt32 _npcId;
	MESSAGE_DEF1(0x6D, UInt32, _npcId);
};

struct CancelAutoBattleReq
{
	MESSAGE_DEF(0x6E);
};

struct InstantAutoBattleReq
{
	MESSAGE_DEF(0x6F);
};

struct CountryBattleJoinStruct
{
	UInt8 _action;

	MESSAGE_DEF1(0x62, UInt8, _action);
};
struct LanchChallengeReq
{
	std::string target;
	MESSAGE_DEF1(0x72, std::string, target);
};

struct RequestChallengeReq
{
	UInt8 res;
	std::string attacker;
	MESSAGE_DEF2(0x73, UInt8, res, std::string, attacker);
};
struct BattleEndReq
{
	MESSAGE_DEF(0x77);
};

struct NpcTriggerReq
{
	UInt32 m_NpcId;

	MESSAGE_DEF1(0x80, UInt32, m_NpcId);
};

struct TaskNpcActionReq
{
	UInt32	m_NpcId;
	UInt8	m_ActionType;
	UInt32	m_ActionID;		//taskId
	UInt8	m_ActionStep;

	TaskNpcActionReq() : m_NpcId(0), m_ActionType(0), m_ActionID(0), m_ActionStep(0) {};

	MESSAGE_DEF4(0x81, UInt32, m_NpcId, UInt8, m_ActionType, UInt32, m_ActionID, UInt8, m_ActionStep);
};

struct CollectNpcActionReq
{
	UInt32 m_NpcId;

	CollectNpcActionReq() : m_NpcId(0) {};

	MESSAGE_DEF1(0x89, UInt32, m_NpcId);
};

struct PlayerGetTaskReq
{
	MESSAGE_DEF(0x82);
};

struct CanAcceptTaskReq
{
	MESSAGE_DEF(0x83);
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
	UInt8	m_Action;	//0：接???1：提???2：放???
	UInt32  m_ItemId;	//奖励物品ID
	UInt16  m_ItemNum;	//奖励物品的数???

	TaskActionReq() : m_TaskId(0), m_Action(0), m_ItemId(0), m_ItemNum(0) {};

	MESSAGE_DEF4(0x85, UInt32, m_TaskId, UInt8, m_Action, UInt32, m_ItemId, UInt16, m_ItemNum);
};

struct StoreBuyReq
{
	UInt8 _type;
	UInt16 _itemId;
	UInt16 _count;
	MESSAGE_DEF3(0xB1, UInt8, _type, UInt16, _itemId, UInt16, _count);
};

struct ChatReq
{
	UInt8 _type;
	std::string _text;
	MESSAGE_DEF2(0xF0, UInt8, _type, std::string, _text);
};

struct PrivChatReq
{
	UInt64 _id;
	std::string _text;
	MESSAGE_DEF2(0xF1, UInt64, _id, std::string, _text);
};

struct ChatItemReq
{
	UInt8 _type;
	UInt64 _playerId;
	UInt32 _id;
	MESSAGE_DEF3(0xF3, UInt8, _type, UInt64, _playerId, UInt32, _id);
};

struct QueryPackageItemReq
{
	MESSAGE_DEF(0x30);
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
	MESSAGE_DEF4(0x33, UInt32, m_ItemId, UInt8, m_ItemBindType, UInt16, m_ItemNum, UInt32, m_Param);
};

struct BugInfoReq
{
  std::string _title;
  std::string _content;
  MESSAGE_DEF2(0xF8,std::string, _title,std::string,_content);
};

void OnUseItemReq( GameMsgHdr& hdr, UseItemReq& req )
{
	MSG_QUERY_PLAYER(pl);
	if (GetItemSubClass(req.m_ItemId) == Item_Task)
		pl->GetPackage()->UseTaskItem(req.m_ItemId, req.m_ItemBindType);
	else
	{
		pl->GetPackage()->UseItem(req.m_ItemId, req.m_ItemNum, req.m_Param, req.m_ItemBindType);
	}
}

struct ExtendPackageReq
{
	MESSAGE_DEF(0x34);
};

struct MailClickReq
{
	UInt32 _mailId;
	UInt8 _action;
	MESSAGE_DEF2(0xA5, UInt32, _mailId, UInt8, _action);
};

struct MailListReq
{
	UInt8	_start;
	UInt8	_count;
	MESSAGE_DEF2(0xA6, UInt8, _start, UInt8, _count);
};

struct MailReq
{
	UInt32	_id;
	MESSAGE_DEF1(0xA1, UInt32, _id);
};

struct MailSendReq
{
	std::string	_target;
	std::string	_title;
	std::string	_content;
	MESSAGE_DEF3(0xA3, std::string, _target, std::string, _title, std::string, _content);
};

struct FriendListReq
{
	UInt8 _type;
	UInt8 _start;
	UInt8 _count;
	MESSAGE_DEF3(0xA8, UInt8, _type, UInt8, _start, UInt8, _count);
};

struct FriendOpReq
{
	UInt8 _op;
	std::string _name;
	MESSAGE_DEF2(0xA9, UInt8, _op, std::string, _name);
};

struct FriendActReq
{
	MESSAGE_DEF(0xAB);
};

void OnExtendPackageReq( GameMsgHdr& hdr, ExtendPackageReq& )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;
	pl->ExtendPackageSize();
}

struct GreatFighterMetReq
{
	MESSAGE_DEF(0xB8);
};

struct GreatFighterInfoReq
{
	UInt32 _fgtId;
	MESSAGE_DEF1(0xB9, UInt32, _fgtId);
};

struct AthleticsDataReq
{
	MESSAGE_DEF(0xD1);
};

struct FighterTrainListReq
{
	MESSAGE_DEF(0x2D);
};

struct FighterTrain2Req
{
	UInt32 _heroID;
	UInt8  _priceType;
	UInt32 _time;
	MESSAGE_DEF3(0x2F, UInt32, _heroID, UInt8, _priceType, UInt32, _time);
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
	UInt32 coin = 0;
	for (UInt16 i = 0; i < itemCount; ++i)
	{
		UInt32 itemId = *reinterpret_cast<const UInt32*>(data+offset);
		bool  bindType = *reinterpret_cast<const bool*>(data+offset+4);
		UInt16 itemNum = *reinterpret_cast<const UInt16*>(data+offset+5);
		offset += 7;
		if (IsEquipId(itemId))
		{
			coin += pl->GetPackage()->SellEquip(itemId);
		}
		else
		{
			coin += pl->GetPackage()->SellItem(itemId, itemNum, bindType);
		}
	}
	if(coin > 0)
	{
		SYSMSG_SEND(116, pl);
		SYSMSG_SEND(1016, pl);
		pl->getCoin(coin);
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
	for (UInt16 i = 0; i < itemCount; ++i)
	{
		UInt32 itemId = *reinterpret_cast<const UInt32*>(data+offset);
		bool  bindType = *reinterpret_cast<const bool*>(data+offset+4);
		UInt16 itemNum = *reinterpret_cast<const UInt16*>(data+offset+4+1);
		offset += 7;
		if (IsEquipId(itemId))
		{
			pl->GetPackage()->DelEquip(itemId, ToDesdroy);
		}
		else
		{
			pl->GetPackage()->DelItem(itemId, itemNum, bindType);
		}
	}
	SYSMSG_SEND(115, pl);
	SYSMSG_SEND(1015, pl);
}

struct FlushTaskColorReq
{
	UInt32 m_DayTaskId;
	UInt8  m_FlushToken;
	UInt32 m_FlushGoldTotal;
	UInt8  m_TaskColor;

	MESSAGE_DEF4(0x8B, UInt32, m_DayTaskId, UInt8, m_FlushToken, UInt32, m_FlushGoldTotal, UInt8, m_TaskColor);
};

void OnFlushTaskColorReq( GameMsgHdr& hdr, FlushTaskColorReq& req )
{
 	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;

	TaskMgr* taskMgr = pl->GetTaskMgr();
	UInt8 color = 0;
	UInt32 nextFlushTime = 0;
	if (req.m_FlushToken == 0)
		taskMgr->FlushTaskColor(req.m_DayTaskId, color, nextFlushTime);
	else
		taskMgr->FlushTaskColor(req.m_DayTaskId, req.m_TaskColor, req.m_FlushGoldTotal, color, nextFlushTime);
	Stream st(0x8B);
	st << req.m_DayTaskId << color << nextFlushTime << Stream::eos;
	pl->send(st);
}

struct DayTaskAutoCompletedReq
{
	UInt32 m_DayTaskId;
	MESSAGE_DEF1(0x8C, UInt32, m_DayTaskId);
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
	MESSAGE_DEF1(0x8D, UInt32, m_TaskId);
};

void OnQueryAutoCompletedTaskTimeReq( GameMsgHdr& hdr, QueryAutoCompletedTaskTimeReq& req )
{
	MSG_QUERY_PLAYER(pl);

	TaskMgr* taskMgr = pl->GetTaskMgr();
	Stream st(0x8D);
	st << req.m_TaskId << taskMgr->GetAutoTaskLeftTime(req.m_TaskId) << Stream::eos;
	pl->send(st);
}

struct QueryDayTaskCompletedCountReq
{
	UInt32 m_DayTaskId;
	MESSAGE_DEF1(0x8F, UInt32, m_DayTaskId);
};

void OnQueryDayTaskCompletedCountReq( GameMsgHdr& hdr, QueryDayTaskCompletedCountReq& req )
{
	MSG_QUERY_PLAYER(pl);

	DayTaskData* taskData = pl->GetTaskMgr()->GetDayTaskData(req.m_DayTaskId);
	if (taskData == NULL) return ;
	Stream st(0x8F);
	st << req.m_DayTaskId << taskData->m_PreTaskQuality << static_cast<UInt16>(taskData->m_Count) << static_cast<UInt16>(taskData->m_MaxCount) << Stream::eos;
	pl->send(st);
}

struct ConveyBattleReq
{
	UInt32 m_TaskId;	//任务ID
	MESSAGE_DEF1(0x88, UInt32, m_TaskId);
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
	Stream st(0x00);
	st << nr.ticket << Stream::eos;
	player->send(st);
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
		Stream st;
		pl->makeFighterList(st);
		conn->send(&st[0], st.size());
	}
	{
		Stream st;
		pl->makeFormationInfo(st);
		conn->send(&st[0], st.size());
	}
	{
		Stream st;
		pl->makeTrainFighterInfo(st);
		conn->send(&st[0], st.size());
	}
	{
		if(PLAYER_DATA(pl, inCity))
			map->SendCityNPCs(pl);
		map->SendAtCity(pl, PLAYER_DATA(pl, inCity) == 1);
	}
	pl->GetMailBox()->notifyNewMail();
	UInt8 level = pl->GetLev();
	if(level >= 5)
	{
		pl->sendOnlineReward();
	}
	pl->sendDailyInfo();
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
	pl->sendWallow();
	pl->sendEvents();
    pl->GetPackage()->SendPackageItemInfor();
}

void OnPlayerInfoChangeReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);	
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 field = 0;
	br >> field;
	switch (field)
	{
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
	UInt8 color = 5;
	UInt16 count = 0;
	switch(type)
	{
	case 1:
		count = 1;
		break;
	case 2:
		br >> color >> count;
		break;
	}
	player->listBookStore(type, color, count);
}

void OnPurchaseBookReq( GameMsgHdr& hdr, PurchaseBookReq& pbr )
{
	MSG_QUERY_PLAYER(player);
	if(pbr._pos >= 6)
		return;
	UInt32 id = player->purchaseBook(pbr._pos);
	Stream st(0x1B);
	st << static_cast<UInt8>(id > 0 ? 1 : 0) << pbr._pos << Stream::eos;
	player->send(st);
}

void OnStatusChangeReq( GameMsgHdr& hdr, StatusChangeReq& scr )
{
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
			SYSMSG_SEND(146, player);
			SYSMSG_SEND(1046, player);
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
			SYSMSG_SEND(145, player);
			SYSMSG_SEND(1045, player);
			GObject::Map * map = GObject::Map::FromSpot(PLAYER_DATA(player, location));
			if(map != NULL)
				map->changebyStatus(player);
		}
		player->addStatus(v);
	}
	GObject::Map::NotifyPlayerEnter(player);
}

void OnSetFormationReq( GameMsgHdr& hdr, const void * buffer )
{
	UInt32 blen = hdr.msgHdr.bodyLen;
	if(blen < 2)
		return;
	const UInt8 * buf = reinterpret_cast<const UInt8 *>(buffer);
	UInt8 f = buf[0];
	UInt8 c = buf[1];
	if(c > 5 || blen < 2 + (sizeof(UInt8) + sizeof(UInt32)) * c)
		return;
	MSG_QUERY_PLAYER(player);

	for(UInt8 i = 0; i < c; ++ i)
	{
		UInt32 pos = 2 + (sizeof(UInt8) + sizeof(UInt32)) * i;
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

	player->updateBattleFighters();

	player->setFormation(f);

	Stream st;
	player->makeFormationInfo(st);
	player->send(st);
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
	Stream st(0x23);
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

struct GreatFighterTaskReq
{
	UInt16 start;
	UInt16 count;
	MESSAGE_DEF2(0x24, UInt16, start, UInt16, count);
};

void OnGreatFighterTaskValReq( GameMsgHdr& hdr, GreatFighterTaskReq& req )
{
	MSG_QUERY_PLAYER(player);

	Stream st;
	player->makeGreatFighterTaskValList(st, req.start, req.count);
	player->send(st);
}

void OnGreatFighterActionReq( GameMsgHdr& hdr, const void * data )
{
	if (hdr.msgHdr.bodyLen < 5)
		return ;
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 token;
	UInt32 fightId;
	brd >> token >> fightId;
	if(player->hasFighter(fightId))
		return;
	GObject::Fighter& fighter = GObject::getGreatFighter(fightId);
	if (fighter.getId() == 0) return ;
	if (token == 0)
	{
		if(!player->hasChecked())
			return;
		UInt16 submitMaxCount = 10;
		if(fighter.getColor() >= 4)
		{
			UInt32 viplvl = player->getVipLevel();
			if(viplvl >= 7)
				submitMaxCount = 30;
			else if(viplvl >= 5)
				submitMaxCount = 20;
		}
		if (player->getGreatFighterFavorSubmitCount(fightId) < submitMaxCount)
		{
			if (player->GetPackage()->DelItemAny(fighter.favor, 4))
			{
				const GData::ItemBaseType * ibt = GData::itemBaseTypeManager[fighter.favor];
				const UInt32 expGainFromFavor[] = {0, 0, 1000, 2000, 5000, 7500, 10000};
				player->AddExp(expGainFromFavor[ibt->quality]);
				player->submitGreatFighterFavor(fightId);
                GameAction()->RunOperationTaskAction0(player, 5);
			}
		}
	}
	else if (token == 1)
	{
		UInt32 friendliness = player->getGreatFighterFriendliness(fightId);
		if(fighter.reqFriendliness > friendliness)
			return;
		if(player->isFighterFull())
		{
			player->sendMsgCode(0, 2006);
			return;
		}
		if (GameAction()->RunAutoBattleAction(player, fightId, -1))
		{
			player->takeFighter(fightId, true);
		}
	}
}

struct FighterLeaveStruct
{
	UInt8 _result;
	UInt32 _fgtid;
	MESSAGE_DEF2(0x27, UInt8, _result, UInt32, _fgtid);
};

void OnFighterEquipReq( GameMsgHdr& hdr, FighterEquipReq& fer )
{
	MSG_QUERY_PLAYER(player);
	GObject::Fighter * fgt = player->findFighter(fer._id);
	if(fgt == NULL)
		return;
	if(fer._part == 0)
	{
		static UInt8 p[8] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};
		ItemEquip * e[8] = {fgt->getWeapon(), fgt->getArmor(0), fgt->getArmor(1), fgt->getArmor(2), fgt->getArmor(3), fgt->getArmor(4), fgt->getAmulet(), fgt->getRing()};
		fgt->sendModification(8, p, e, false);
		return;
	}
	ItemEquip * equip;
	player->GetPackage()->EquipTo(fer._equipId, fgt, fer._part, equip);
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
	Stream st(0x22);
	st << static_cast<UInt8>(id > 0 ? 0 : 1) << rfr._pos << Stream::eos;
	player->send(st);
	if (id != 0)

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
	Stream st(0x27);
	if(fgt == NULL)
	{
		rep._result = 1;
		player->send(rep);
		return;
	}
	UInt64 exp = fgt->getExp() / 2;
	if(exp >= 25000)
	{
		exp += 25000;
		UInt16 rCount1 = static_cast<UInt16>(exp / 80000000);
		exp = exp % 80000000;
		UInt16 rCount2 = static_cast<UInt16>(exp / 2000000);
		exp = exp % 2000000;
		UInt16 rCount3 = static_cast<UInt16>(exp / 50000);
		SYSMSG(title, 236);
		SYSMSGV(content, 237, fgt->getLevel(), fgt->getColor(), fgt->getName().c_str());
		MailPackage::MailItem mitem[3] = {{9017, rCount1}, {9016, rCount2}, {8995, rCount3}};
		MailItemsInfo itemsInfo(mitem, DismissFighter, 3);
		GObject::Mail * pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
		if(pmail != NULL)
		{
			GObject::mailPackageManager.push(pmail->id, mitem, 3, true);
		}
	}
	delete fgt;
	rep._fgtid = fdr._fgtid;
	rep._result = 0;
	player->send(rep);
}

void OnFighterRegenReq( GameMsgHdr& hdr, FighterRegenReq& frr )
{
	MSG_QUERY_PLAYER(player);
	if(player->hasFlag(Player::CountryBattle | Player::ClanBattling))
	{
		player->sendMsgCode(2, 2051);
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
	Stream st(0x2C);
	st << player->trainFighter(ftr._fgtId, ftr._type) << Stream::eos;;
	player->send(st);
}

void OnLuckyDrawInfoReq( GameMsgHdr& hdr, LuckyDrawInfoReq& )
{
	MSG_QUERY_PLAYER(player);
	GObject::luckyDraw.sendInfo(player);
}

void OnTakeOnlineRewardReq( GameMsgHdr& hdr, TakeOnlineRewardReq& )
{
	MSG_QUERY_PLAYER(player);
	if(!player->takeOnlineReward())
		player->sendMsgCode(1, 2016);
	player->sendOnlineReward();
}

void OnLuckyDrawReq( GameMsgHdr& hdr, LuckyDrawReq& ldr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	UInt8 type = ldr._type - 1;
	if(type  > 3)
		return;
	UInt32 cost = 0;
	bool bind = false;
	switch(type)
	{
	case 0:
		{
			if(player->getTael() < static_cast<UInt32>(ldr._times * 9))
				return;
			ConsumeInfo ci(Explore,0,0);
			player->useTael(ldr._times * 9,&ci);
			UInt8 lev = player->GetLev();
			if(lev >= 70)
				type = 6;
			else if(lev >= 50)
				type = 5;
			bind = true;
			break;
		}
	case 1:
		{
			if(player->getGold() < static_cast<UInt32>(ldr._times * 5))
				return;
			cost = ldr._times * 5;
			ConsumeInfo ci(Explore,0,0);
			player->useGold(cost,&ci);
			break;
		}
	case 2:
	case 3:
		{
			if(player->getGold() < static_cast<UInt32>(ldr._times * 9))
				return;
			cost = ldr._times * 9;
			ConsumeInfo ci(Explore,0,0);
			player->useGold(cost,&ci);
			UInt8 lev = player->GetLev();
			if(type == 3)
				type = 7;
			else if(lev >= 80)
				type = 4;
			else if(lev >= 70)
				type = 3;
			break;
		}
	}
	std::vector<UInt16> equips;
	std::map<UInt32, UInt8> items;
	for(UInt8 i = 0; i < ldr._times; ++ i)
	{
		GData::LootResult lr = GObject::luckyDraw.doLuckyDraw(type);
		if(IsEquipTypeId(lr.id))
		{
			for(UInt16 j = 0; j < lr.count; ++ j)
				equips.push_back(lr.id);
		}
		else
		{
			items[lr.id] += lr.count;
		}
	}
	if(cost > 0)
	{
		GObject::luckyDraw.pushCost(player, cost);
	}
	Stream st(0x3E);
	Stream broadcastst;
	st << ldr._type << ldr._times << static_cast<UInt8>(equips.size() + items.size());
	std::vector<UInt16>::iterator it1;
	std::map<UInt32, UInt8>::iterator it2;
	for(it1 = equips.begin(); it1 != equips.end(); ++ it1)
	{
		const GData::ItemBaseType * baseType = GData::itemBaseTypeManager[*it1];
		UInt8 quality = (baseType == NULL) ? 0 : baseType->quality;
		GObject::ItemBase * item = player->GetPackage()->AddEquip2(*it1, quality >= 5, bind, FromLuckyDraw);
		if(item != NULL)
		{
			st << item->getId() << *it1;
			if(quality >= 5)
			{
				Stream st(0x3F);
				st << static_cast<UInt8>(1) << player->getName() << player->getCountry() << ldr._type << *it1 << Stream::eos;
				broadcastst.append(&st[0], st.size());
			}

			GObject::luckyDraw.pushResult(player, item->getQuality(), item->GetItemType().getId(), 1, ldr._type);
		}
		else
			st << static_cast<UInt32>(0) << *it1;
	}
	for(it2 = items.begin(); it2 != items.end(); ++ it2)
	{
		const GData::ItemBaseType * baseType = GData::itemBaseTypeManager[it2->first];
		UInt8 quality = (baseType == NULL) ? 0 : baseType->quality;
		GObject::ItemBase * item = player->GetPackage()->AddItem2(it2->first, it2->second, quality >= 5, bind, FromLuckyDraw);
		st << it2->first << it2->second;
		if(item != NULL)
		{
			if(item->getQuality() >= 5)
			{
				Stream st(0x3F);
				st << static_cast<UInt8>(1) << player->getName() << player->getCountry() << ldr._type << static_cast<UInt16>(it2->first) << it2->second << Stream::eos;
				broadcastst.append(&st[0], st.size());
			}
			GObject::luckyDraw.pushResult(player, item->getQuality(), static_cast<UInt16>(it2->first), it2->second, ldr._type);
		}
	}
	st << Stream::eos;
	player->send(st);
	if(broadcastst.size() > 0)
		NETWORK()->Broadcast(broadcastst);
}

void OnEnchantReq( GameMsgHdr& hdr, EnchantReq& er )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x40);
	st << player->GetPackage()->Enchant(er._fighterId, er._itemid, er._type, er._protect > 0) << er._fighterId << er._itemid << Stream::eos;
	player->send(st);
    GameAction()->RunOperationTaskAction1(player, 1, 2);
}

void OnOpenSocketReq( GameMsgHdr& hdr, OpenSocketReq& osr )
{
	MSG_QUERY_PLAYER(player);
	Stream st(0x41);
	st << player->GetPackage()->OpenSocket(osr._fighterId, osr._itemid) << osr._fighterId << osr._itemid << Stream::eos;
	player->send(st);
}

void OnMergeGemReq( GameMsgHdr& hdr, MergeGemReq& mgr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x42);
	UInt32 ogid = 0;
	st << player->GetPackage()->MergeGem(mgr._itemid, mgr._bindNum, mgr._protect > 0, ogid) << static_cast<UInt16>(ogid) << Stream::eos;
	player->send(st);
}

void OnAttachGemReq( GameMsgHdr& hdr, AttachGemReq& agr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x43);
	st << player->GetPackage()->AttachGem(agr._fighterId, agr._itemid, agr._gemid, agr._bind > 0) << agr._fighterId << agr._itemid << Stream::eos;
	player->send(st);
}

void OnDetachGemReq( GameMsgHdr& hdr, DetachGemReq& dgr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x44);
	st << player->GetPackage()->DetachGem(dgr._fighterId, dgr._itemid, dgr._pos, dgr._protect) << dgr._fighterId << dgr._itemid << dgr._pos << Stream::eos;
	player->send(st);
}

#if 0
void OnSplitReq( GameMsgHdr& hdr, SplitReq& sr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x45);
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
	Stream st(0x46);
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
	UInt8 r = player->GetPackage()->Forge(fr._fighterId, fr._itemid, fr._type, types, values, fr._protect);
	Stream st(0x47);
	st << r << fr._fighterId << fr._itemid << Stream::eos;
	player->send(st);
}

#if 0
void OnExchangeSetReq( GameMsgHdr& hdr, ExchangeSetReq& esr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x4A);
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
	UInt16 rcount[4] = {0, 0, 0, 0};
	for(UInt16 i = 0; i < count; ++ i)
	{
		UInt32 itemId;
		br >> itemId;
		UInt32 outId;
		UInt8 outCount;
		if(pkg->Split(itemId, outId, outCount, /*false,*/ true) < 2)
		{
			switch(outId)
			{
			case ITEM_ENCHANT_L1:
				rcount[0] += outCount;
				break;
			case ITEM_ENCHANT_L2:
				rcount[1] += outCount;
				break;
			case ITEM_ENCHANT_L3:
				rcount[2] += outCount;
				break;
			case ITEM_ENCHANT_L4:
				rcount[3] += outCount;
				break;
			default:
				break;
			}
		}
		else
			break;
	}
	Stream st(0x49);
	st << flag << rcount[0] << rcount[1] << rcount[2] << rcount[3] << Stream::eos;
	player->send(st);
}

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
		player->sendMsgCode(0, 2016);
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
	Stream st(0x4B);
	st << result << static_cast<UInt16>(gemsOut.size());
	for(std::vector<UInt32>::iterator it = gemsOut.begin(); it != gemsOut.end(); ++ it)
	{
		st << static_cast<UInt16>((*it) & 0x7FFF) << static_cast<UInt8>((*it) & 0x8000 ? 1 : 0) << static_cast<UInt16>((*it) >> 16);
	}
	st << Stream::eos;
	player->send(st);
}

#if 0
void OnActivateAttrReq( GameMsgHdr& hdr, ActivateAttrReq& aar )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	Stream st(0x4C);
	st << player->GetPackage()->ActivateAttr(aar._fighterId, aar._itemId, aar._itemId2) << aar._fighterId << aar._itemId << Stream::eos;
	player->send(st);
}
#endif

void OutCitySwitchReq( GameMsgHdr& hdr, OutCitySwitchStruct& lms )
{
	MSG_QUERY_PLAYER(pl);
	pl->moveTo(static_cast<UInt16>(lms._mapid) << 8, false);
}

void InCitySwitchReq( GameMsgHdr& hdr, InCitySwitchStruct& sms )
{
	MSG_QUERY_PLAYER(pl);
	pl->moveTo(sms._locid, true);
}

void OnTransportReq( GameMsgHdr& hdr, CityTransportReq& ctr )
{
	MSG_QUERY_PLAYER(pl);
	if(!pl->hasChecked())
		return;
	UInt32 viplvl = pl->getVipLevel();
	if(ctr.flag == 0)
	{
		if(viplvl < 3)
		{
			if(pl->getTael() < 1)
					return;
			ConsumeInfo ci(Transport,0,0);
			pl->useTael(1,&ci);
		}
	}
	else
	{
		if(viplvl == 0)
		{
			if(pl->getTael() < 10)
			{
				pl->sendMsgCode(0, 2070);
				return;
			}
			ConsumeInfo ci(Transport,0,0);
			pl->useTael(10,&ci);
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
	Stream st(0x58);
	st << dor.op;
	UInt8 result = 0;
	switch(dor.op)
	{
	case 0:
		result = dg->playerEnter(pl, dor.difficulty - 1);
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
	st << result << dor.type << dor.difficulty << Stream::eos;
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
		pl->sendMsgCode(1, 2019);
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

	if(pl->GetPackage()->GetRestPackageSize() < 4)
	{
		pl->sendMsgCode(1, 2019);
		return;
	}
	if(dar.type == 0)
	{
		pl->cancelAutoDungeon();
		return;
	}
	GObject::Dungeon * dg = GObject::dungeonManager[dar.type];
	if(dg == NULL)
		return;
	dg->autoChallenge(pl);
}

void OnDungeonCompleteAutoReq( GameMsgHdr& hdr, DungeonCompleteAutoReq& )
{
	MSG_QUERY_PLAYER(pl);
	GameMsgHdr hdr2(0x181, WORKER_THREAD_WORLD, pl, 0);
	GLOBAL().PushMsg(hdr2, NULL);
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
	case 0x50:
		MOAction::GreatFighterActionStep(player, req.m_NpcId);
		break;
	case 0x51:
		MOAction::RunGreatCtrlActionStep(player, req.m_NpcId);
		break;
	case 0x60:
		MOAction::RunDayCopyTaskStep(player, req.m_NpcId, req.m_ActionID);
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
	switch (req.m_Action)
	{
	case 0:
		//接受, 走脚???
		succ = GameAction()->AcceptTask(player, req.m_TaskId);
		break;
	case 1:
		//提交, 直接走脚???
		succ = GameAction()->SubmitTask(player, req.m_TaskId, req.m_ItemId, req.m_ItemNum); //提交
		break;
	case 2:
		//放弃
		succ = GameAction()->AbandonTask(player, req.m_TaskId);
		break;
	default:
		return ;
	}
	if (succ)
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
	MESSAGE_DEF1(0x62, UInt8, result);
};

void CountryBattleJoinReq( GameMsgHdr& hdr, CountryBattleJoinStruct& req )
{
	MSG_QUERY_PLAYER(player);
	if(!PLAYER_DATA(player, inCity))
		return;
	UInt16 loc = PLAYER_DATA(player, location);
	GObject::SpotData * spot = GObject::Map::Spot(loc);
	if(spot == NULL || !spot->m_CountryBattle)
		return;

	CountryBattleJoinReply rep;
	CountryBattle * cb = spot->GetCountryBattle();
	if(req._action == 0)
	{
		rep.result = cb->playerEnter(player) ? 0 : 2;
	}
	else
	{
		cb->playerLeave(player);
		rep.result = 1;
	}
	player->send(rep);
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
		player->sendMsgCode(0, 2035, buffLeft - now);
		return ;
	}
	if( target->getBuffData(PLAYER_BUFF_ATTACKING) > 0 )
	{
		player->sendMsgCode(0, 2037);
		return ;
	}
	UInt8 tid = player->getThreadId();
	if( target->getThreadId() != tid || !target->hasStatus(0x02) || !player->hasStatus(0x02) )
	{
		Stream st1(0x73);
		st1 << static_cast<UInt8>(1) << lcr.target << Stream::eos;
		player->send(st1);
		return;
	}	
	
	Stream st(0x72);
	st << player->getCountry() << player->getName() << Stream::eos;
	target->send(st);
}

void OnRequestChallengeReq( GameMsgHdr& hdr, RequestChallengeReq& rcr)
{
	MSG_QUERY_PLAYER(player);
	GObject::Player *attacker = globalNamedPlayers[player->fixName(rcr.attacker)];
	if(attacker == NULL)//error
		return ;
	Stream st(0x73);
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
		player->sendMsgCode(0, 2035, buffLeft - now);
		return ;
	}
	if( attacker->getBuffData(PLAYER_BUFF_ATTACKING) > 0)
	{
		player->sendMsgCode(0, 2037);
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

	Stream st1(0x61);
	st1 << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
	player->send(st1);
	st1.data<UInt8>(4) = static_cast<UInt8>(res ? 0 : 1);
	attacker->send(st1);

	player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + turns * 2);
	attacker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + turns * 2);
}

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
		player->sendMsgCode(0, 2039);
		return ;
	}
	if(player->getBuffData(PLAYER_BUFF_PROTECT, now) > 0)
	{
		player->sendMsgCode(0, 2038);
		return ;
	}
	UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
	if(buffLeft > 0)
	{
		player->sendMsgCode(0, 2035, buffLeft - now);
		return ;
	}
	if(target->getBuffData(PLAYER_BUFF_ATTACKING, now) > 0)
	{
		player->sendMsgCode(0, 2037);
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
	UInt32 tael = 0;
	UInt32 Achievement = 0;

	if(player->challenge(target, NULL, &turns))
	{
		if(lev <= 5 && !GObject::challengeCheck.hasPair(player, target))
		{
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
		}

		player->moveToHome();
		SYSMSG_SENDV(1043, target, player->getCountry(), player->getName().c_str());
		SYSMSG_SENDV(144, player, target->getCountry(), target->getName().c_str());
		SYSMSG_SENDV(1044, player, target->getCountry(), target->getName().c_str());
		if(lev1 >= 60 && lev2 >= 60)
		{
			SYSMSG_BROADCASTV(370 + now % 2,  player->getCountry(), player->getName().c_str(), target->getCountry(), target->getName().c_str(), target->getCountry(), target->getName().c_str());
		}
	}
	player->setBuffData(PLAYER_BUFF_ATTACKING, now + turns * 2);
	target->setBuffData(PLAYER_BUFF_ATTACKING, now + turns * 2);
	player->setBuffData(PLAYER_BUFF_PKLOCK, now + 20 * 60);
	target->addFoe(player);
}

void OnAttackNpcReq( GameMsgHdr& hdr, AttackNpcReq& anr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->isInCity())
	{
		player->sendMsgCode(0, 2036);
		return;
	}
	player->cancelAutoBattle();
	player->cancelAutoDungeon();
	UInt16 loc = player->getLocation();
	GObject::Map * map = Map::FromSpot(loc);
	if(map == NULL)
	{
		player->sendMsgCode(0, 2036);
		return;
	}
	GObject::MapObject * mo = map->GetObject(anr._npcId);
	if(mo == NULL || mo->GetSpot() != loc)
	{
		player->sendMsgCode(0, 2036);
		return;
	}
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

void OnBattleEndReq( GameMsgHdr& hdr, BattleEndReq& )
{
	MSG_QUERY_PLAYER(player);
	UInt32 now = TimeUtil::Now();
	if(now < PLAYER_DATA(player, battlecdtm))
		return ;
	player->checkLastBattled();
	player->setBuffData(PLAYER_BUFF_ATTACKING, 0);
}

void OnStoreBuyReq( GameMsgHdr& hdr, StoreBuyReq& lr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	UInt32 price = GData::store.getPrice(lr._type, lr._itemId);
	Stream st(0xB1);
	if(price == 0 || price == 0xFFFFFFFF)
	{
		st << static_cast<UInt8>(3);
	}
	else
	{
		price *= lr._count;
		switch(lr._type)
		{
		case 4:
			if(PLAYER_DATA(player, coupon) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				GObject::ItemBase * item;
				if(IsEquipTypeId(lr._itemId))
					item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
				else
					item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
				if(item == NULL)
					st << static_cast<UInt8>(2);
				else
				{
				    ConsumeInfo ci(Item,lr._itemId,lr._count);
					player->useCoupon(price,&ci);
					st << static_cast<UInt8>(0);
                }
			}
			break;
		case 6:
			if(PLAYER_DATA(player, tael) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				GObject::ItemBase * item;
				if(IsEquipTypeId(lr._itemId))
					item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
				else
					item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
				if(item == NULL)
					st << static_cast<UInt8>(2);
				else
				{
				    ConsumeInfo ci(Item,lr._itemId,lr._count);
					player->useTael(price,&ci);
					st << static_cast<UInt8>(0);
                }
			}
			break;
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
		case 7:
			if(PLAYER_DATA(player, achievement) < price)
			{
				st << static_cast<UInt8>(1);
			}
			else
			{
				GObject::ItemBase * item;
				if(IsEquipTypeId(lr._itemId))
					item = player->GetPackage()->AddEquipN(lr._itemId, lr._count, true, false, FromNpcBuy);
				else
					item = player->GetPackage()->AddItem(lr._itemId, lr._count, true, false, FromNpcBuy);
				if(item == NULL)
					st << static_cast<UInt8>(2);
				else
				{
					ConsumeInfo ci(Item,lr._itemId, lr._count);
					player->useAchievement(price,&ci);
					st << static_cast<UInt8>(0);
				}
			}
			break;
		default:
			if(PLAYER_DATA(player, gold) < price)
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
                    player->useGold(price,&ci);
					st << static_cast<UInt8>(0);
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
	MESSAGE_DEF7(0xF0, UInt8, type, std::string, name, UInt8, cny, UInt8, sex, UInt8, office, UInt8, guard, std::string, text);
};

static bool inCountry(const Network::TcpConduit * conduit, UInt8 country)
{
	const Network::GameClient * cl = static_cast<const Network::GameClient *>(conduit);
	Player * pl = cl->GetPlayer();
	return pl != NULL && pl->getCountry() == country;
}

#define ITEM_SPEAKER 8941

void OnChatReq( GameMsgHdr& hdr, ChatReq& cr )
{
	using namespace std::placeholders;
	MSG_QUERY_PLAYER(player);

    if(player->getBuffData(PLAYER_BUFF_BANCHAT)!=0)
        return;

	if(gmHandler.Handle(cr._text, player))
		return;

	Stream st(0xF0);
	UInt8 office = player->getTitle(), guard = 0;
	st << cr._type << player->getName() << player->getCountry() << static_cast<UInt8>(player->IsMale() ? 0 : 1)
		<< office << guard << cr._text << Stream::eos;
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
		if(!player->GetPackage()->DelItemAny(ITEM_SPEAKER, 1))
			break;
	default:
		NETWORK()->Broadcast(st);
		break;
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
		rep.guard = 0;
		player->send(rep);
	}
	else
	{
		rep.name = player->getName();
		rep.text = pcr._text;
		rep.cny = player->getCountry();
		rep.sex = player->IsMale() ? 0 : 1;
		rep.office = player->getTitle();
		rep.guard = 0;
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
	DBLOG().PushUpdateData("insert into complaints (server_id,player_id,submit_type,submit_time,title,content) values(%u,%"I64_FMT"u,'%s',%u,'%s','%s')",
		cfg.serverLogId, hdr.player->getId(), type.c_str(), TimeUtil::Now(), title.c_str(), content.c_str());
}


struct TradeListReq
{
	UInt16 _index;
	UInt16 _num;

	TradeListReq() : _index(0), _num(0) {}
	MESSAGE_DEF2(0xC0, UInt16, _index, UInt16, _num);
};

struct TradeDataReq
{
	UInt32 _tradeId;

	TradeDataReq() : _tradeId(0) {}
	MESSAGE_DEF1(0xC1, UInt32, _tradeId);
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
	MESSAGE_DEF5(0xC2, std::string, _name, std::string, _title, UInt32, _coin, UInt32, _gold, std::vector<TradeItemChange>, _items);
};

struct TradeReplyReq
{
	UInt32 _id;
	UInt32 _coin;
	UInt32 _gold;
	std::vector<TradeItemChange> _items;

	TradeReplyReq() : _id(0), _coin(0), _gold(0) {}
	MESSAGE_DEF4(0xC3, UInt32, _id, UInt32, _coin, UInt32, _gold, std::vector<TradeItemChange>, _items);
};

struct TradeOperateReq
{
	UInt32 _id;
	UInt8  _op;

	TradeOperateReq() : _id(0), _op(0) {}
	MESSAGE_DEF2(0xC4, UInt32, _id, UInt8, _op);
};

S11N_TRAITS_4(GObject::SaleSellData, UInt32, id, UInt16, count, UInt32, price, UInt8, priceType);
struct SaleSellReq
{
	std::vector<GObject::SaleSellData> _data;
	MESSAGE_DEF1(0xC6, std::vector<GObject::SaleSellData>, _data);
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
	if(!player->hasChecked())
		return;

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
	UInt16 omb = mailBox->getNewMails();
	for(UInt8 i = 0; i < c; ++ i)
	{
		mailBox->delMail(idlist[i], true);
	}
	if(mailBox->getNewMails() != omb)
	{
		mailBox->notifyNewMail();
	}
	std::vector<UInt8> rep;
	rep.resize(4 + blen);
	*reinterpret_cast<UInt32 *>(&rep[0]) = 0xA2FF0000 | blen;
	memcpy(&rep[4], buffer, blen);
	player->send(&rep[0], rep.size());
	if(c > 0)
		SYSMSG_SEND(139, player);
}

struct MailSendRep
{
	UInt8 _res;

	MESSAGE_DEF1(0xA3, UInt8, _res);
};

void OnMailSendReq( GameMsgHdr& hdr, MailSendReq& msr )
{
	MSG_QUERY_PLAYER(player);
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
		player->sendMsgCode(1, 2086);
		return;
	}
	if(pl->hasBlock(player))
	{
		player->sendMsgCode(1, 2087);
		return;
	}
	MailSendRep rep;
	rep._res = 0;
	pl->GetMailBox()->newMail(player, 0x03, msr._title, msr._content);
}


void OnMailClickReq( GameMsgHdr& hdr, MailClickReq& mcr )
{
	MSG_QUERY_PLAYER(player);
	player->GetMailBox()->clickMail(mcr._mailId, mcr._action);
}

void OnFriendListReq( GameMsgHdr& hdr, FriendListReq& flr )
{
	if(flr._type > 2)
		return;
	MSG_QUERY_PLAYER(player);
	player->sendFriendList(flr._type, flr._start, flr._count);
}

void OnFriendOpReq( GameMsgHdr& hdr, FriendOpReq& fr )
{
	MSG_QUERY_PLAYER(player);
	GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(fr._name)];
	if(pl == NULL || pl == player)
	{
		player->sendMsgCode(0, 1003);
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
	}
}

void OnFriendActReq( GameMsgHdr& hdr, FriendActReq& )
{
	MSG_QUERY_PLAYER(player);
	player->sendFriendActList();
}

void OnGreatFighterMetReq( GameMsgHdr& hdr, GreatFighterMetReq& )
{
	MSG_QUERY_PLAYER(player);
	player->sendGreatFighterMet();
}

void OnGreatFighterInfoReq( GameMsgHdr& hdr, GreatFighterInfoReq& gfir )
{
	if(gfir._fgtId > GREAT_FIGHTER_MAX)
		return;
	MSG_QUERY_PLAYER(player);
	Stream st(0xB9);
	st << gfir._fgtId << player->getGreatFighterFriendliness(gfir._fgtId);
	st << "" << "" << Stream::eos;
	player->send(st);
}

void OnAthleticsDataReq( GameMsgHdr& hdr, AthleticsDataReq& )
{
	MSG_QUERY_PLAYER(player);
	player->GetAthletics()->notifyAthleticsData(15);
}

struct BlockBossReq
{
	MESSAGE_DEF(0xD5);
};
void OnBlockBossReq( GameMsgHdr& hdr, BlockBossReq& )
{
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
	MESSAGE_DEF(0xD6);
};

void OnAttackBlockBossReq( GameMsgHdr& hdr, AttackBlockBossReq& )
{
	MSG_QUERY_PLAYER(player);
	player->attackBlockBoss();
}

struct PwdQuestion
{
	MESSAGE_DEF(0xCB);
};

void OnPwdQuestionReq( GameMsgHdr& hdr, PwdQuestion& )
{
	MSG_QUERY_PLAYER(player);
	Stream st(0xCB);
	st << player->getQuestionForPWD() << Stream::eos;
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
	Stream st(0xCA);
	st << op << res << Stream::eos;
	player->send(st);
}

struct LockPwdReq
{
	UInt8 flag;
	std::string pwd;
	MESSAGE_DEF2(0xCE, UInt8, flag, std::string, pwd);
};

void OnLockPwdReq( GameMsgHdr& hdr, LockPwdReq&  lpd)
{
	MSG_QUERY_PLAYER(player);
	UInt8 res = 0;
	if(lpd.flag == 0)
		res = player->unLockSecondPWD(lpd.pwd);
	else
		player->lockSecondPWD();

	Stream st(0xCE);
	st << lpd.flag << res << Stream::eos;
	player->send(st);
}

void OnFighterTrainListReq( GameMsgHdr& hdr, FighterTrainListReq& )
{
	MSG_QUERY_PLAYER(player);
	Stream st;
	player->makeTrainFighterInfo(st);
	player->send(st);
}

void OnFighterTrain2Req( GameMsgHdr& hdr, FighterTrain2Req& req )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	player->addTrainFighter(req._heroID, req._priceType, req._time);
}

void OnFighterTrainOpReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt32 fighterId = 0;
	UInt8 type = 0;
	brd >> fighterId >> type;
	if(fighterId == 0)
		return;
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

#endif // _COUNTRYOUTERMSGHANDLER_H_
