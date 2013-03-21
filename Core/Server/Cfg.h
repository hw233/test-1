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

    std::string	dbLockHost;
	UInt16		dbLockPort;
	std::string	dbLockUser;
	std::string	dbLockPassword;
	std::string	dbLockSource;


	std::string	cryptKey1, cryptKey2;
	std::string	gmCryptKey1, gmCryptKey2;
	std::string	fbrCryptKey1, fbrCryptKey2;
    bool fbVersion;
    bool vtVersion;
    bool debug;

    struct TokenServer
    {
        std::string ip;
        UInt16 port;
    };
    std::vector<TokenServer> tokenServer;
    std::vector<TokenServer> IDQueryMemcached;

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
    int serverNo;
    bool isTestPlatform;

	std::string arenaHost;
	UInt16      arenaPort;

    //yij
    std::string msgCenterHost;
    UInt16      msgCenterPort;

    bool enableLoginLimit;
    UInt16 loginLimit;
    UInt16 onlineLimit;

    std::string sql_consume_tael;
    std::string sql_item_courses;
    std::string sql_item_histories;
    std::string sql_mailitem_histories;

    std::string stateUrl;
    std::string chargeUrl;
    UInt16 warZone;

    std::string mergeList;

    bool udplog;
    bool dclog;
    bool secdclog;
    bool secdclogTest;
    bool unionPlatform;
    bool autoForbid;
    bool autoKick;

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
    inline void setLockDatabase(const char * h, UInt16 p, const char * u, const char * pw, const char * s )
	{dbLockHost = h; dbLockPort = p; dbLockUser = u; dbLockPassword = pw; dbLockSource = s;}
	inline void setCryptKeys(const char * k1, const char * k2) {cryptKey1 = k1; cryptKey2 = k2;}
	inline void setGMCryptKeys(const char * k1, const char * k2) {gmCryptKey1 = k1; gmCryptKey2 = k2;}
	inline void setFBRechargeKeys(const char * k1, const char * k2) {fbrCryptKey1 = k1; fbrCryptKey2 = k2;}
	void setAdminAllowedIP(const char *);
	inline void setOpening( UInt16 y, UInt8 m, UInt8 d )
	{ openYear = y; openMonth = m; openDay = d; }
	inline void setWallow( bool e ) { enableWallow = e; }
	inline void setLimitLuckyDraw( int e ) { limitLuckyDraw = e; }
	inline void setMerged(bool m) { merged = m; }
	inline void setSupportCompress(bool c) { supportCompress = c; }
	inline void setGMCheck(bool c) { GMCheck = c; }
    inline void setTestPlatform(bool c) { isTestPlatform = c; }
	inline void setArenaServer(const char * h, UInt16 p) { arenaHost = h; arenaPort = p; }
	inline void setMsgCenter(const char * h, UInt16 p) { msgCenterHost = h; msgCenterPort = p; }  //yij
    inline void setChannelInfo(const char * sn, int num) { slugName = sn; channelNum = num; }
	inline void setLoginLimit(bool limit) { enableLoginLimit = limit; }
	inline void setLoginMax(UInt16 max) { loginLimit = max; }
	inline void setOnlineLimit(UInt16 limit) { onlineLimit = limit; }
    inline void setStateUrl(const char* url) { stateUrl = url; }
    inline void setChargeUrl(const char* url) { chargeUrl = url; }
    inline void setWarZone(UInt16 zone) { warZone = zone; }
    inline void setServerNum(UInt32 no) { serverNum = no; }
    inline void setServerNo(UInt32 no) { serverNo = no; }
    inline void setMergeList(const char* list) { mergeList = list?list:"";}
    inline void setUdpLog(bool v) { udplog = v; }
    inline void setDCLog(bool v) { dclog = v; }
    inline void setSecDCLog(bool v) { secdclog = v; }
    inline void setSecDCLogTest(bool v) { secdclogTest = v; }
    inline void setUnionPlatform(bool v) { unionPlatform = v; }

    inline void setAutoForbid(bool v) { autoForbid = v; }
    inline void setAutoKick(bool v) { autoKick = v; }


	inline void setVerifyTokenServer(const char* server, UInt16 port)
    {
        TokenServer ts;
        ts.ip = server;
        ts.port = port;
        tokenServer.push_back(ts);
    }

    inline void setIDQueryMemcachedServer(const char* server, UInt16 port)
    {
        TokenServer ts;
        ts.ip = server;
        ts.port = port;
        IDQueryMemcached.push_back(ts);
    }

	bool isAdminIPAllowed(UInt32);

    inline void SetSQLConsumetael(const char* p){sql_consume_tael = p;}
    inline void SetSQLItemCourses(const char* p){ sql_item_courses = p;}
    inline void SetSQLItemHistories(const char* p){ sql_item_histories = p;}
    inline void SetSQLMailItemHistories(const char* p){ sql_mailitem_histories = p;}
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
