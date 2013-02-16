#ifndef _SERVERTYPES_H_
#define _SERVERTYPES_H_

struct ServerInfor
{
	std::string m_Ip;		//服务器绑定的IP
	UInt16		m_Port;		//服务器监听的端口
	std::string m_DBIp;
	UInt16		m_DBPort;
	//some other ...
};

enum
{
	WORKER_THREAD_COUNTRY_1 = 0,	//峨嵋
	WORKER_THREAD_COUNTRY_2,		//昆仑
	WORKER_THREAD_NEUTRAL,			//中立
	WORKER_THREAD_WORLD,			//世界线程
	WORKER_THREAD_LOGIN,			//登录
	WORKER_THREAD_SORT,				//排序
#ifndef _WIN32
	WORKER_THREAD_DC,				//
    WORKER_THREAD_OPEN_API,               //
#endif
	WORKER_THREAD_DB,				//7  DB
	WORKER_THREAD_DB1,				//8  DB1 player,trade
	WORKER_THREAD_DB2,				//9  DB2 fighter
    WORKER_THREAD_DB3,              //10  DB3 dungeon_player,player_copy,player_front,auto_battle,discount
	WORKER_THREAD_DB4,				//11  DB4 item,equipment,sale
	WORKER_THREAD_DB5,				//12 DB5 task_instance, clan
	WORKER_THREAD_DB6,				//13 athletics,tripod
	WORKER_THREAD_DB7,				//14 player_buff
	WORKER_THREAD_DB8,				//15 recharge
	WORKER_THREAD_DB_LOG,			//后台LOG
	WORKER_THREAD_DB_LOG1,			//后台LOG1
	MAX_THREAD_NUM
};

#define COUNTRY_EMEI WORKER_THREAD_COUNTRY_1
#define COUNTRY_KUNLUN WORKER_THREAD_COUNTRY_2
#define COUNTRY_NEUTRAL WORKER_THREAD_NEUTRAL
#define COUNTRY_MAX (WORKER_THREAD_NEUTRAL+1)

#define LEVEL_MAX (140)
#define CLAN_LEVEL_MAX (10)
#define NEWGUILDSTEP_MAX (34)

#endif // _SERVERTYPES_H_
