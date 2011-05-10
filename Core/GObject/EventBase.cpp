#include "Config.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "TaskMgr.h"
#include "EventBase.h"
#include "Player.h"

namespace GObject
{

EventWrapper eventWrapper;

EventBase::~EventBase()
{
}

void EventBase::Destroy()
{
	if(m_EventData != NULL)
	{
		WORLD().RemoveTimer(m_EventData);
		m_EventData = NULL;
	}
}

EventWrapper::~EventWrapper()
{
	for(EventSet::iterator it = m_Events.begin(); it != m_Events.end(); ++ it)
	{
		EventBase * ev = it->second;
		if(ev != NULL)
			ev->release();
	}
	m_Events.clear();
}

void EventWrapper::Event_CB(EventBase * ev)
{
	bool removeEvent = !ev->Next();
	if(ev->IsWorldEvent())
	{
		ev->Process(ev->GetTimer().GetLeftTimes());
		if(removeEvent)
		{
			eventWrapper.DelTimerEvent(ev);
		}
	}
	else
	{
		if(removeEvent)
		{
			if(eventWrapper.RemoveTimerEvent(ev) == NULL)
				return;
		}
		if(!removeEvent)
			ev->duplicate();
		struct EventMsg
		{
			EventBase * ev;
			UInt32 leftCount;
		};
		EventMsg evm = {ev, ev->GetTimer().GetLeftTimes()};
		GameMsgHdr hdr(0x289, ev->GetPlayer()->getThreadId(), ev->GetPlayer(), sizeof(EventMsg));
		GLOBAL().PushMsg(hdr, &evm);
	}
}

void EventWrapper::AddTimerEvent(EventBase * ev)
{
	void * evdata = WORLD().AddTimer(ev->GetInterval() * 1000, Event_CB, ev, 0xFFFFFFFF);
	if(evdata == NULL)
		return;
	ev->SetEventData(evdata);
	m_Events.insert(std::make_pair(ev->GetPlayer(), ev));
}

void EventWrapper::DelTimerEvent(Player * player, UInt32 id, size_t data)
{
	EventSet::iterator it = m_Events.lower_bound(player);
	while(it != m_Events.end() && it->first == player)
	{
		EventBase * ev = it->second;
		if(ev->Equal(id, data))
		{
			m_Events.erase(it ++);
			if(ev != NULL)
			{
				ev->Destroy();
				ev->release();
			}
			continue;
		}
		++ it;
	}
}

void EventWrapper::DelTimerEvent( EventBase * event )
{
	Player * player = event->GetPlayer();
	EventSet::iterator it = m_Events.lower_bound(player);
	while(it != m_Events.end() && it->first == player)
	{
		EventBase * ev = it->second;
		if(ev == event)
		{
			m_Events.erase(it);
			if(ev != NULL)
			{
				ev->Destroy();
				ev->release();
			}
			return;
		}
		++ it;
	}
}

void EventWrapper::DelTimerEvent( iterator it )
{
	EventBase * ev = it->second;
	m_Events.erase(it);
	if(ev != NULL)
	{
		ev->Destroy();
		ev->release();
	}
}

EventWrapper::iterator EventWrapper::FindTimerEvent( Player * player, UInt32 id, size_t data )
{
	EventSet::iterator it = m_Events.lower_bound(player);
	while(it != m_Events.end() && it->first == player)
	{
		EventBase * ev = it->second;
		if(ev == NULL)
		{
			m_Events.erase(it ++);
			continue;
		}
		if(ev->Equal(id, data))
		{
			return it;
		}
		++ it;
	}
	return m_Events.end();
}

EventBase * EventWrapper::RemoveTimerEvent( Player * player, UInt32 id, size_t data )
{
	EventSet::iterator it = m_Events.lower_bound(player);
	while(it != m_Events.end() && it->first == player)
	{
		EventBase * ev = it->second;
		if(ev == NULL)
		{
			m_Events.erase(it ++);
			continue;
		}
		if(ev->Equal(id, data))
		{
			m_Events.erase(it);
			ev->Destroy();
			return ev;
		}
		++ it;
	}
	return NULL;
}

EventBase * EventWrapper::RemoveTimerEvent( EventBase * event )
{
	Player * player = event->GetPlayer();
	EventSet::iterator it = m_Events.lower_bound(player);
	while(it != m_Events.end() && it->first == player)
	{
		EventBase * ev = it->second;
		if(ev == event)
		{
			m_Events.erase(it);
			if(ev != NULL)
				ev->Destroy();
			return ev;
		}
		++ it;
	}
	return NULL;
}

}
