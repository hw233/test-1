#include "Config.h"
#include "MsgHandler.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
//#include "GObject/DCLogger.h"
#endif
#endif
#endif
#include "MsgID.h"
#include "GMHandler.h"

void MsgHandler::DeregisterAllMsg()
{
	for (int i = 0; i < MAX_MSG_NUM; i++)
	{
		//delete a NULL pointer is also safe
        if (m_HandlerList[i])
        {
            TRACE_LOG("m_HandlerList[%u] delete", i);
        }
		delete (m_HandlerList[i]);
		m_HandlerList[i] = NULL;
	}
}

bool MsgHandler::ProcessMsg()
{
    // RunnerWorker.Shutdown()后只允许DB再处理未处理完的消息
    if (isRunnerShutdown() && (m_Worker < WORKER_THREAD_DB || m_Worker == WORKER_THREAD_LOAD))
    {
        fprintf(stderr, "%s: RunnerWorker(%d).Shutdown()后只允许DB再处理未处理完的消息.\n", __PRETTY_FUNCTION__, m_Worker);
        return false;
    }

	Handler* handler = NULL;
	MsgQueue tmp;
	MsgQueue msgQueue;
	if(!GLOBAL().GetMsgQueue( m_Worker, msgQueue ))
		return false;
	MsgHdr* hdr = NULL;
	do
	{
		hdr = msgQueue.Pop();
		assert( hdr->cmdID < MAX_MSG_NUM );
        handler = m_HandlerList[hdr->cmdID];
        if (handler != NULL)
        {
            if (!handler->m_Wrapper(handler->m_Func, hdr))
            {
                //Error
			}
		}
		else
		{
			//Error
		}
        GLOBAL().FreeMsgBlock((char *)hdr);
	}
	while (!msgQueue.Empty());
	return true;
}
