#ifndef _WORLDINNERMSGHANDLER_H_
#define _WORLDINNERMSGHANDLER_H_

#include "Common/Serialize.h"
#include "MsgTypes.h"
#include "MsgID.h"
#include "MsgFunc.h"
#include "CountryMsgStruct.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "Server/SysMsg.h"
#include "Script/WorldScript.h"
#include "Script/BattleFormula.h"
#include "GObject/World.h"
#include "Common/Itoa.h"
#include <set>

/*
void OnWorldCupResult( GameMsgHdr& hdr, const void * data )
{
    struct WorldCupRes
    {
       UInt8 num;  
       UInt32 res;
    };
	const  WorldCupRes* wc = reinterpret_cast<const WorldCupRes *>(data);
    WORLD().WorldCupAward(wc->num,wc->res);
}
*/

/*
void OnPushTimerEvent( GameMsgHdr& hdr, const void * data )
{
	GObject::EventBase* event = *reinterpret_cast<GObject::EventBase * const*>(data);
	GObject::eventWrapper.AddTimerEvent(event);
}


void OnPopTimerEvent( GameMsgHdr& hdr, const void * data )
{
	struct EventMsgData
	{
		UInt32 id;
		size_t data;
	};
	const EventMsgData* emd = reinterpret_cast<const EventMsgData *>(data);
	GObject::eventWrapper.DelTimerEvent(hdr.player, emd->id, emd->data);
}

void OnSearchEvents( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct EventQuery
	{
		bool autoBattle;
		bool autoDungeon;
	};
	const EventQuery * eq = reinterpret_cast<const EventQuery *>(data);
	if(eq->autoBattle)
	{
		GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_AUTOBATTLE, 0);
		if(it != GObject::eventWrapper.end())
		{
			GObject::EventBase * event = it->second;
			event->duplicate();
			if(event == NULL) return;

			GameMsgHdr hdr1(0x28C, player->getThreadId(), player, sizeof(GObject::EventBase *));
			GLOBAL().PushMsg(hdr1, &event);
		}
	}
	if(eq->autoDungeon)
	{
		GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_DUNGEONAUTO, 0);
		if(it != GObject::eventWrapper.end())
		{
			GObject::EventDungeonAuto * event = static_cast<GObject::EventDungeonAuto *>(it->second);
			GameMsgHdr hdr1(0x28D, player->getThreadId(), player, sizeof(GObject::Dungeon));
			GObject::Dungeon * dg = event->GetDungeon();
			GLOBAL().PushMsg(hdr1, &dg);
		}
	}
    //GObject::arena.sendActive(player);
}
*/

#endif // _WORLDINNERMSGHANDLER_H_
