#include "Config.h"
#include "WorldServer.h"
#include "GData/GDataManager.h"
#include "GObject/GObjectManager.h"
#include "GObject/World.h"
#include "GObject/Country.h"
#include "GObject/Var.h"
#include "Battle/BattleReport.h"
#include "GObject/ClanManager.h"
#include "DB/DBWorker.h"
#include "WorkerThread.h"
#include "DB/DBConnectionMgr.h"
#include "Login/LoginWorker.h"
#include "Cfg.h"
#include "SysMsg.h"
#include "Common/TimeUtil.h"
#include "kingnet_analyzer.h"
#include "GObject/DCWorker.h"
#include "GObject/DCLogger.h"
#include "GObject/SortWorker.h"

const char* s_HelpInfo = "";
//////////////////////////////////////////////////////////////////////////
WorldServer::WorldServer() : m_IsActive(false), curl(NULL)
{
	memset(m_AllWorker, 0x00, sizeof(m_AllWorker));
	m_TcpService = NULL;
    curl = curl_easy_init();
}

WorldServer::~WorldServer()
{
	for (int i = 0; i < MAX_THREAD_NUM; i++)
	{
		delete m_AllWorker[i];
	}
	//delete m_TcpService;
    if (curl) curl_easy_cleanup(curl);
}

bool WorldServer::ParseCommandInfor(Int32 argc, char * argv[])
{
	int status = 0;
	for(int i = 1; i < argc; ++ i)
	{
		switch(status)
		{
		case 1:
			cfg.setFilename(argv[i]);
			break;
		default:
			if(strcmp(argv[i], "--config") == 0)
			{
				status = 1;
			}
			break;
		}
	}
	return true;
}

bool WorldServer::Init(const char * scriptStr, const char * serverName, int num)
{
	if(serverName != NULL)
	{
		const char * p = strstr(serverName, "_s");
		if(p != NULL)
		{
			cfg.channelName = std::string(serverName, p);
			cfg.serverNum = atoi(p + 2);
			cfg.slugName = cfg.channelName;
		}
	}
    Network::Initialize();
	//读取配置文件
	TimeUtil::Init();
    GObject::VarSystem::Init();
	cfg.load(scriptStr);
	globalSysMsg.load();
	Battle::battleReport.init();
#ifdef _FB
    _analyzer.Init("./conf/udplogfb.xml");
#else
    _analyzer.Init();
#endif
#ifdef _FB
#else
    GObject::dclogger.init();
#endif

    if(!cfg.serverNum)
    {
        if (!num)
        {
            srand(time(NULL));
            cfg.serverNum = cfg.tcpPort+rand()%1000;
        }
        else
            cfg.serverNum = num;
    }

    cfg.serverLogId = cfg.serverNum;

#ifdef _FB
    cfg.fbVersion = true;
#else
#endif

#ifdef _DEBUG
    cfg.debug = true;
#endif

	//数据库连接操作， 连接池创建
	DB::gDataDBConnectionMgr = new DB::DBConnectionMgr();
	DB::gDataDBConnectionMgr->Init( cfg.dbDataHost.c_str(), cfg.dbDataUser.c_str(), cfg.dbDataPassword.c_str(), cfg.dbDataSource.c_str(), 1, 32, cfg.dbDataPort );

	DB::gObjectDBConnectionMgr = new DB::DBConnectionMgr();
	DB::gObjectDBConnectionMgr->Init( cfg.dbObjectHost.c_str(), cfg.dbObjectUser.c_str(), cfg.dbObjectPassword.c_str(), cfg.dbObjectSource.c_str(), 4, 32, cfg.dbObjectPort );

	DB::gLogDBConnectionMgr = new DB::DBConnectionMgr();
	DB::gLogDBConnectionMgr->Init( cfg.dbLogHost.c_str(), cfg.dbLogUser.c_str(), cfg.dbLogPassword.c_str(), cfg.dbLogSource.c_str(), 4, 32, cfg.dbLogPort );

	int worker;

	worker = WORKER_THREAD_WORLD;
	m_AllWorker[worker] = new WorkerThread<GObject::World>(new GObject::World());

	for (worker = WORKER_THREAD_COUNTRY_1; worker <= WORKER_THREAD_NEUTRAL; worker++)
	{
		m_AllWorker[worker] = new WorkerThread<GObject::Country>(new GObject::Country(worker));
	}

	worker = WORKER_THREAD_LOGIN;
	m_AllWorker[WORKER_THREAD_LOGIN] = new WorkerThread<Login::LoginWorker>(new Login::LoginWorker());

	worker = WORKER_THREAD_SORT;
	m_AllWorker[worker] = new WorkerThread<GObject::SortWorker>(new GObject::SortWorker(0, WORKER_THREAD_SORT));

	worker = WORKER_THREAD_DC;
	m_AllWorker[worker] = new WorkerThread<GObject::DCWorker>(new GObject::DCWorker(0, WORKER_THREAD_DC));

	worker = WORKER_THREAD_DB;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB));
	worker = WORKER_THREAD_DB1;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB1));
	worker = WORKER_THREAD_DB2;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB2));
	worker = WORKER_THREAD_DB3;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB3));
	worker = WORKER_THREAD_DB4;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB4));
	worker = WORKER_THREAD_DB5;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB5));
	worker = WORKER_THREAD_DB6;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB6));
	worker = WORKER_THREAD_DB7;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB7));
	worker = WORKER_THREAD_DB8;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(0, WORKER_THREAD_DB8));

	worker = WORKER_THREAD_DB_LOG;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(1, WORKER_THREAD_DB_LOG));
	worker = WORKER_THREAD_DB_LOG1;
	m_AllWorker[worker] = new WorkerThread<DB::DBWorker>(new DB::DBWorker(1, WORKER_THREAD_DB_LOG1));

	worker = WORKER_THREAD_DC;
	m_AllWorker[worker]->Run();

	//启动数据库线程处理
	worker = WORKER_THREAD_DB;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB1;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB2;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB3;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB4;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB5;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB6;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB7;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB8;
	m_AllWorker[worker]->Run();

	worker = WORKER_THREAD_DB_LOG;
	m_AllWorker[worker]->Run();
	worker = WORKER_THREAD_DB_LOG1;
	m_AllWorker[worker]->Run();

	GData::GDataManager::LoadAllData();
	//GObject::GObjectManager::InitGlobalObject();
	GObject::GObjectManager::InitIDGen();	    //将各表的最大ID值存入缓存
	GObject::GObjectManager::loadAllData();

	return true;
}

void WorldServer::UnInit()
{
    Down();

	DB::gObjectDBConnectionMgr->UnInit();
	delete DB::gObjectDBConnectionMgr;
	DB::gObjectDBConnectionMgr = NULL;

	DB::gDataDBConnectionMgr->UnInit();
	delete DB::gDataDBConnectionMgr;
	DB::gDataDBConnectionMgr = NULL;

	DB::gLogDBConnectionMgr->UnInit();
	delete DB::gLogDBConnectionMgr;
	DB::gLogDBConnectionMgr = NULL;

	Network::Uninitialize();
}

void WorldServer::SetActive(bool active)
{
	m_IsActive = active;
}

bool WorldServer::IsActive() const
{
	return m_IsActive;
}

void WorldServer::Run()
{
	int worker;

	worker = WORKER_THREAD_SORT;
	m_AllWorker[worker]->Run();

	worker = WORKER_THREAD_WORLD;
	m_AllWorker[worker]->Run();

	//启动工作线程
	for (worker = WORKER_THREAD_COUNTRY_1; worker <= WORKER_THREAD_NEUTRAL; worker++)
	{
		m_AllWorker[worker]->Run();
	}

	//启动登录线程
	worker = WORKER_THREAD_LOGIN;
	m_AllWorker[worker]->Run();

	m_TcpService = new Network::TcpServerWrapper(cfg.tcpPort);
	m_TcpService->Start();

    Up();

	//主线程等待所有子线程结束
	for (worker = 0; worker < MAX_THREAD_NUM; worker++)
	{
		m_AllWorker[worker]->Wait();
	}

	m_TcpService->Join();

	delete m_TcpService;
}

void WorldServer::Shutdown()
{
	int worker;

    Down();

	//关闭网络线程
	m_TcpService->UnInit();

	Thread::sleep(2000);

	//关闭所有工作线程
	for (worker = 0; worker < MAX_THREAD_NUM; worker++)
	{
		if(worker <= WORKER_THREAD_DC)
			m_AllWorker[worker]->Shutdown();
	}

	Thread::sleep(2000);
	m_AllWorker[WORKER_THREAD_DB]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB1]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB2]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB3]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB4]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB5]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB6]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB7]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB8]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB_LOG]->Shutdown();
	m_AllWorker[WORKER_THREAD_DB_LOG1]->Shutdown();
}

#define MAX_RET_LEN 1024

static int recvret(char* data, size_t size, size_t nmemb, char* buf)
{
    size_t nsize = size * nmemb;
    if (nsize > MAX_RET_LEN) {
        bcopy(data, buf, MAX_RET_LEN);
        return MAX_RET_LEN;
    }   

    bcopy(data, buf, nsize);
    return nsize;
}

void WorldServer::State(const char* action, int serverNum)
{
    if (!curl || !action || !serverNum)
        return;
    char url[4096] = {0};
    snprintf(url, sizeof(url), "%s&state=%s&server=s%d", cfg.stateUrl.c_str(), action, serverNum);

    char buffer[MAX_RET_LEN] = {0};
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recvret);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    fprintf(stderr, "URL: %s\n", url);

    CURLcode res = curl_easy_perform(curl);
    if (CURLE_OK == res)
    {
        // TODO:
        fprintf(stderr, "URL: %s [OK]\n", url);
    }
    else
    {
        fprintf(stderr, "URL: %s [ERROR]\n", url);
    }
}

void WorldServer::Up()
{
    State("open", cfg.serverNum);
}

void WorldServer::Down()
{
    State("close", cfg.serverNum);
}

GObject::Country& WorldServer::GetCountry(UInt8 worker)
{
	assert(worker==WORKER_THREAD_NEUTRAL || worker==WORKER_THREAD_COUNTRY_1 || worker==WORKER_THREAD_COUNTRY_2);
	return Worker<GObject::Country>(worker);
}

GObject::World& WorldServer::GetWorld()
{
	return Worker<GObject::World>(WORKER_THREAD_WORLD);
}

GObject::DCWorker& WorldServer::GetSort()
{
	return Worker<GObject::DCWorker>(WORKER_THREAD_SORT);
}

GObject::DCWorker& WorldServer::GetDC()
{
	return Worker<GObject::DCWorker>(WORKER_THREAD_DC);
}

DB::DBWorker& WorldServer::GetDB()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB);
}

DB::DBWorker& WorldServer::GetDB1()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB1);
}

DB::DBWorker& WorldServer::GetDB2()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB2);
}

DB::DBWorker& WorldServer::GetDB3()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB3);
}

DB::DBWorker& WorldServer::GetDB4()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB4);
}

DB::DBWorker& WorldServer::GetDB5()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB5);
}

DB::DBWorker& WorldServer::GetDB6()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB6);
}

DB::DBWorker& WorldServer::GetDB7()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB7);
}

DB::DBWorker& WorldServer::GetDB8()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB8);
}

DB::DBWorker& WorldServer::GetDBLog()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB_LOG);
}

DB::DBWorker& WorldServer::GetDBLog1()
{
	return Worker<DB::DBWorker>(WORKER_THREAD_DB_LOG1);
}

Login::LoginWorker& WorldServer::GetLogin()
{
	return Worker<Login::LoginWorker>(WORKER_THREAD_LOGIN);
}
