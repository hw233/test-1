#ifndef _EVENTBASE_INC_
#define _EVENTBASE_INC_

#include "EventTimer.h"
#include "Server/WorldServer.h"
#include "Common/RefCountedObject.h"

enum EventID
{
	EVENT_TASKAUTOCOMPLETE = 1,
	EVENT_AUTOBATTLE = 2,
	EVENT_FIGHTERAUTOTRAINING = 3,
	EVENT_DUNGEONAUTO = 4,
    EVENT_PLAYERPRACTICING = 5,
    EVENT_PLAYERPRTRIPOD = 6,
    EVENT_AUTOCOPY = 7,
    EVENT_TIMETICK = 8,
    EVENT_AUTOFRONTMAP = 9,
    EVENT_TLZAUTO = 10,
    EVENT_JOBHUNTER = 11,
    EVENT_REFRESHOPENKEY = 12,
};

namespace GObject
{

class World;

class EventBase: public RefCountedObject
{
public:
	EventBase(Player * pl, UInt32 interval, UInt32 count = 1): m_Player(pl), m_Timer(interval, count), m_EventData(NULL) {}
	virtual ~EventBase();

public:
	void Destroy();
	virtual UInt32 GetID() const = 0;
	virtual void Process(UInt32) = 0;
	virtual bool Equal(UInt32 id, size_t) const { return id == GetID(); }

	virtual bool IsWorldEvent() const	{ return false; }

	inline Player* GetPlayer() { return m_Player; }
	inline EventTimer& GetTimer() { return m_Timer; }
	inline bool Next()					{ return m_Timer.DecCount(); }
	inline bool IsActive() const		{ return m_Timer.IsActive(); }
	inline UInt32 GetInterval() const	{ return m_Timer.GetInterval(); }
	inline void SetEventData(void * ed) { m_EventData = ed; }
	void * GetEventData() const { return m_EventData; }

protected:
	Player *	m_Player;
	EventTimer	m_Timer;
	void *		m_EventData;
};

class EventWrapper
{
public:
	typedef std::multimap<Player *, EventBase *> EventSet;
	typedef EventSet::iterator iterator;
	typedef EventSet::const_iterator const_iterator;

public:
	EventWrapper() {};
	virtual ~EventWrapper();

public:
	void AddTimerEvent(EventBase * event);
	void DelTimerEvent(Player * player, UInt32 id, size_t data);
	void DelTimerEvent(EventBase * event);
	void DelTimerEvent(iterator it);
	iterator FindTimerEvent(Player * player, UInt32 id, size_t data);
	EventBase * RemoveTimerEvent(Player * player, UInt32 id, size_t data);
	EventBase * RemoveTimerEvent(EventBase * event);
	inline iterator end() { return m_Events.end(); }
	inline const_iterator end() const { return m_Events.end(); }

private:
	static void Event_CB(EventBase *);

protected:
	EventSet m_Events;
};

extern EventWrapper eventWrapper;

#define PushTimerEvent(event) do{	\
	if(CURRENT_THREAD_ID() == WORKER_THREAD_WORLD) \
		GObject::eventWrapper.AddTimerEvent(event); \
	else \
	{ \
		GameMsgHdr hdr(0x199, WORKER_THREAD_WORLD, event->GetPlayer(), sizeof(EventBase*));	\
		GLOBAL().PushMsg(hdr, &event);	\
	} \
}while(0)


#define PopTimerEvent(player, tid, tdata) \
do {	\
	if(CURRENT_THREAD_ID() == WORKER_THREAD_WORLD) \
		GObject::eventWrapper.DelTimerEvent(player, tid, tdata); \
	else \
	{ \
		struct EventMsgData \
		{ \
			UInt32 id; \
			size_t data; \
		}; \
		EventMsgData emd = {tid, tdata}; \
		GameMsgHdr hdr(0x198, WORKER_THREAD_WORLD, player, sizeof(EventMsgData));	\
		GLOBAL().PushMsg(hdr, &emd);	\
	} \
}while(0)



}

#endif
