#include "Config.h"
#include "GlobalObject.h"

bool GlobalObject::Init()
{
	return true;
}

void GlobalObject::UnInit()
{
	//释放消息池中所有的消息
	for (int i = 0; i < MAX_THREAD_NUM; i++)
	{
		FastMutex::ScopedLock lock(m_MsgQueueCs[i]);
		while (!m_MsgQueue[i].Empty())
		{
			MsgHdr* msg = m_MsgQueue[i].Pop();
			delete[] (char*)msg;
		}
		m_MsgQueue[i].Clear();
	}
}
