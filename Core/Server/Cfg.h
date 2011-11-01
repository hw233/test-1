#ifndef _CFG_H_
#define _CFG_H_

class Cfg
{
public:
	Cfg();
	void load(const char * = NULL);
	inline void setFilename(const char * fn) {_filename = fn;}

	UInt16		tcpPort;
    UInt32      serverIp;
    std::string ifName;
	std::string	scriptPath;
	std::string reportPath;
	UInt32		serverLogId;
	std::string announceFile;

	std::string	dbDataHost;
	UInt16		dbDataPort;
	std::string	dbDataUser;
	std::string	dbDataPassword;
	std::string	dbDataSource;

	std::string	dbObjectHost;
	UInt16		dbObjectPort;
	std::string	dbObjectUser;
	std::string	dbObjectPassword;
	std::string	dbObjectSource;

	std::string	dbLogHost;
	UInt16		dbLogPort;
	std::string	dbLogUser;
	std::string	dbLogPassword;
	std::string	dbLogSource;

	std::string	cryptKey1, cryptKey2;

    struct TokenServer
    {
        std::string ip;
        UInt16 port;
    };
    std::vector<TokenServer> tokenServer;

	UInt16 openYear;
	UInt8 openMonth, openDay;

	bool enableWallow;
	int limitLuckyDraw;

	bool merged;
	bool supportCompress;
	bool GMCheck;

	std::string channelName;
	std::string slugName;
	int channelNum;
	int serverNum;

	std::string arenaHost;
	UInt16      arenaPort;

    bool enableLoginLimit;
    UInt16 loginLimit;
    UInt16 onlineLimit;

public:
	inline void setTcpPort(UInt16 p) {tcpPort = p;}
	void setIfName(const char* iname);
	inline void setScriptPath(const char * p) {scriptPath = p;}
	inline void setReportPath(const char * p) {reportPath = p;}
	inline void setServerLogId(UInt32 id) {/*serverLogId = id;*/}
	inline void setAnnounceFile(const char * p) {announceFile = p;}
	inline void setDataDatabase(const char * h, UInt16 p, const char * u, const char * pw, const char * s )
	{dbDataHost = h; dbDataPort = p; dbDataUser = u; dbDataPassword = pw; dbDataSource = s;}
	inline void setObjectDatabase(const char * h, UInt16 p, const char * u, const char * pw, const char * s )
	{dbObjectHost = h; dbObjectPort = p; dbObjectUser = u; dbObjectPassword = pw; dbObjectSource = s;}
	inline void setLogDatabase(const char * h, UInt16 p, const char * u, const char * pw, const char * s )
	{dbLogHost = h; dbLogPort = p; dbLogUser = u; dbLogPassword = pw; dbLogSource = s;}
	inline void setCryptKeys(const char * k1, const char * k2) {cryptKey1 = k1; cryptKey2 = k2;}
	void setAdminAllowedIP(const char *);
	inline void setOpening( UInt16 y, UInt8 m, UInt8 d )
	{ openYear = y; openMonth = m; openDay = d; }
	inline void setWallow( bool e ) { enableWallow = e; }
	inline void setLimitLuckyDraw( int e ) { limitLuckyDraw = e; }
	inline void setMerged(bool m) { merged = m; }
	inline void setSupportCompress(bool c) { supportCompress = c; }
	inline void setGMCheck(bool c) { GMCheck = c; }
	inline void setArenaServer(const char * h, UInt16 p) { arenaHost = h; arenaPort = p; }
	inline void setChannelInfo(const char * sn, int num) { slugName = sn; channelNum = num; }
	inline void setLoginLimit(bool limit) { enableLoginLimit = limit; }
	inline void setLoginMax(UInt16 max) { loginLimit = max; }
	inline void setOnlineLimit(UInt16 limit) { onlineLimit = limit; }

	inline void setVerifyTokenServer(const char* server, UInt16 port)
    {
        TokenServer ts;
        ts.ip = server;
        ts.port = port;
        tokenServer.push_back(ts);
    }

	bool isAdminIPAllowed(UInt32);

private:
	struct IPMask
	{
		UInt32 addr;
		UInt32 mask;
	};

	IPMask parseAddress(const std::string& addr);

	std::string _filename;
	std::vector<IPMask> _adminIPAllowed;
};

extern Cfg cfg;

#endif // _CFG_H_
