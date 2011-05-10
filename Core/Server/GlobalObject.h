#ifndef GLOBALDATA_INC
#define GLOBALDATA_INC

#include "Common/Singleton.h"
#include "Common/Queue.h"
#include "MsgHandler/MsgTypes.h"
#include "Server/ServerTypes.h"
#include "Common/Mutex.h"

class GlobalObject : public Singleton<GlobalObject>
{ 
protected:
	GlobalObject() {};
	virtual ~GlobalObject() {};
	friend class Singleton<GlobalObject>;

public:
	bool Init();

	void UnInit();

public:
	template <typename MsgHdrType>
	void PushMsg(const MsgHdrType& hdr, void* msgBody);

	inline bool GetMsgQueue(UInt8 workerId, MsgQueue& popMsgQueue)
	{
		assert( workerId>=0 && workerId<MAX_THREAD_NUM );
		{
			FastMutex::ScopedLock lock(m_MsgQueueCs[workerId]);
			if(m_MsgQueue[workerId].Empty())
				return false;
			popMsgQueue = m_MsgQueue[workerId];
			m_MsgQueue[workerId].Clear();
		}
		return true;
	}

private:
	MsgQueue			m_MsgQueue[MAX_THREAD_NUM];
	FastMutex			m_MsgQueueCs[MAX_THREAD_NUM];
};

//////////////////////////////////////////////////////////////////////////
template <typename MsgHdrType>
inline void GlobalObject::PushMsg(const MsgHdrType& hdr, void* msgBody)
{
	UInt8 workerId = hdr.msgHdr.desWorkerID;
	if( workerId >= (UInt8)MAX_THREAD_NUM )
		return;
	char* buffer = new(std::nothrow) char[sizeof(MsgHdrType) + hdr.msgHdr.bodyLen];
	if(buffer == NULL)
		return;
	memcpy(buffer, &hdr, sizeof(MsgHdrType));
	if(msgBody != NULL && hdr.msgHdr.bodyLen > 0)
		memcpy(buffer+sizeof(MsgHdrType), msgBody, hdr.msgHdr.bodyLen);

	FastMutex::ScopedLock lock(m_MsgQueueCs[workerId]);
	m_MsgQueue[workerId].Push((MsgHdr*)buffer);
}

#define GLOBAL()	GlobalObject::Instance()

#endif
