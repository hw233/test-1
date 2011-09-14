#ifndef _SERVERTYPES_H_
#define _SERVERTYPES_H_

struct ServerInfor
{
	std::string m_Ip;		//�������󶨵�IP
	UInt16		m_Port;		//�����������Ķ˿�
	std::string m_DBIp;
	UInt16		m_DBPort;
	//some other ...
};

enum
{
	WORKER_THREAD_COUNTRY_1 = 0,	//����
	WORKER_THREAD_COUNTRY_2,		//����
	WORKER_THREAD_NEUTRAL,			//����
	WORKER_THREAD_WORLD,			//�����߳�
	WORKER_THREAD_LOGIN,			//��¼
	WORKER_THREAD_DB,				//DB
	WORKER_THREAD_DB1,				//DB1
	WORKER_THREAD_DB_LOG,			//��̨LOG
	MAX_THREAD_NUM
};

#define COUNTRY_EMEI WORKER_THREAD_COUNTRY_1
#define COUNTRY_KUNLUN WORKER_THREAD_COUNTRY_2
#define COUNTRY_NEUTRAL WORKER_THREAD_NEUTRAL
#define COUNTRY_MAX (WORKER_THREAD_NEUTRAL+1)

#define LEVEL_MAX (60)
#define CLAN_LEVEL_MAX (10)
#define NEWGUILDSTEP_MAX (34)

#endif // _SERVERTYPES_H_
