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
	WORKER_THREAD_DB1,				//DB1 player
	WORKER_THREAD_DB2,				//DB2 fighter
    WORKER_THREAD_DB3,              //DB3 dungeon_player,player_copy,player_front,task_instance
	WORKER_THREAD_DB4,				//DB4 item,equipment
	WORKER_THREAD_DB5,				//DB5 task_instance
	WORKER_THREAD_DB_LOG,			//��̨LOG
	WORKER_THREAD_DB_LOG1,			//��̨LOG1
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
