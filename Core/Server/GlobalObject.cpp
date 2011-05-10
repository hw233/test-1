#include "Config.h"
#include "GlobalObject.h"

bool GlobalObject::Init()
{
	return true;
}

void GlobalObject::UnInit()
{
	//�ͷ���Ϣ�������е���Ϣ
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
