#ifndef	SERVER_INC
#define SERVER_INC

#include "Common/Singleton.h"
#include "Network/TcpServerWrapper.h"
#include "ServerTypes.h"
#include "GObject/World.h"
#include "DB/DBWorker.h"
#include "Login/LoginWorker.h"
#include "WorkerThread.h"

extern const char* s_HelpInfo;

namespace GObject
{
	class Country;
}

class BaseThread;
class WorldServer : public Singleton<WorldServer>
{
public:
	void Run();
	bool ParseCommandInfor(Int32 argc, char * argv[]);

protected:
	WorldServer();
	virtual ~WorldServer();
	friend class Singleton<WorldServer>;

public:
	bool Init(const char * = NULL, const char * = NULL);
	void Shutdown();
	void UnInit();
	void SetActive(bool active);
	bool IsActive() const;
	
public:
	GObject::Country& GetCountry(UInt8 worker);
	GObject::World& GetWorld();
	DB::DBWorker& GetDB();
	DB::DBWorker& GetDBLog();
	Login::LoginWorker& GetLogin();

	inline Network::TcpServerWrapper* GetTcpService() {return m_TcpService;}

protected:
	template <typename WorkerType>
	inline WorkerType& Worker(UInt8 worker)
	{
		return (dynamic_cast<WorkerThread<WorkerType>*>(m_AllWorker[worker]))->Worker();
	}

private:
	bool	m_IsActive;
	ServerInfor m_ServerInfor;

	//////////////////////////////////////////////////////////////////////////
public:
	BaseThread*	m_AllWorker[MAX_THREAD_NUM];
	Network::TcpServerWrapper* m_TcpService;
};

#define SERVER()		WorldServer::Instance()
#define NETWORK()		SERVER().GetTcpService()
#define WORLD()			SERVER().GetWorld()
#define DB()			SERVER().GetDB()
#define DBLOG()			SERVER().GetDBLog()
#define LOGIN()			SERVER().GetLogin()

#define CURRENT_THREAD_ID() WorkerThread<WorkerRunner<> >::LocalWorker().GetThreadID()

#endif

