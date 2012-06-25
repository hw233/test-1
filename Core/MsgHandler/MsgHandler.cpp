#include "Config.h"
#include "MsgHandler.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"

void MsgHandler::DeregisterAllMsg()
{
	for (int i = 0; i < MAX_MSG_NUM; i++)
	{
		//delete a NULL pointer is also safe
		delete (m_HandlerList[i]);
		m_HandlerList[i] = NULL;
	}
}

bool MsgHandler::ProcessMsg()
{
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
		if(hdr->cmdID >= MIN_INNER_CHECK_MSG) // Throttler to check thread id
		{
			GameMsgHdr * ihdr = reinterpret_cast<GameMsgHdr *>(hdr);
			if(ihdr->player != NULL)     // Push msg to follow players' thread if the player is just switched to another thread
			{
				UInt8 tid = ihdr->player->getThreadId();
				if(tid != m_Worker)
				{
					ihdr->msgHdr.desWorkerID = tid;
					GLOBAL().PushMsg(*ihdr, ihdr + 1);
                    delete[] (char *)hdr;
					continue;
				}
			}
		}
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
        delete[] (char *)hdr;
	}
	while (!msgQueue.Empty());
	return true;
}
