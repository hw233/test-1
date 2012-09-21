#ifndef CLANDYNAMICMSG_INC
#define CLANDYNAMICMSG_INC

namespace GObject
{

class Clan;

struct CDMValType
{
	UInt8		type;
	UInt32		intval;
	std::string strval;

	CDMValType(UInt8 t = 0xFF) : type(t), intval(0) {}
	CDMValType(UInt8 t, UInt32 v) : type(t), intval(v) {}
	CDMValType(UInt8 t, const std::string& v) : type(t), strval(v) {}
};
struct CDMsg
{
	UInt8 type;			//宗族动态信息类型
	std::vector<CDMValType> value;

	CDMsg(UInt8 t = 0xFF) : type(t) {}
};

struct ClanBattleReport
{
	std::string battlerName;
	std::string clanName;
	UInt8		battlerLev;
	UInt16		reliveCount;
	UInt16		serailWins;
	UInt16		maxSerailWins;
	UInt8		side;
	UInt8		northEdurance;
	UInt8		southEdurance;
	UInt32		grabAchieve;

	ClanBattleReport(const std::string& bn = "", const std::string& cn = "", UInt8 bl = 0, UInt16 rc = 0, UInt16 sw = 0, UInt16 msw = 0, UInt8 sd = 0, UInt8 ne = 0, UInt8 se = 0, UInt32 ga = 0)
		: battlerName(bn), clanName(cn), battlerLev(bl), reliveCount(rc), serailWins(sw), maxSerailWins(msw), side(sd), northEdurance(ne), southEdurance(se), grabAchieve(ga) {};
};
typedef std::pair<UInt8, std::vector<ClanBattleReport> > ClanBattleReportElem;
typedef std::map<UInt32, ClanBattleReportElem, std::greater<UInt32> > ClanBattleReportElems;	//时间 -> 战报

struct CDMKey
{
	UInt32 key1;
	UInt32 key2;

	CDMKey(UInt32 k1 = 0, UInt32 k2 = 0) : key1(k1), key2(k2) {}
};
struct CDMKeyLess
{
	bool operator() (const CDMKey& cdmk1, const CDMKey& cdmk2) const
	{
		if (cdmk1.key1 == cdmk2.key1)
			return cdmk1.key2 > cdmk2.key2;
		return cdmk1.key1 > cdmk2.key1;
	}
};

class ClanDynamicMsg
{
public:
	ClanDynamicMsg();
	~ClanDynamicMsg();

public:
	UInt16 getCDMsgSize(UInt8);
	void makeCDMsgInfor(Stream&, UInt8, UInt16, UInt16);
	void makeCBMsgInfor(Stream&, UInt32);

public:
	bool addCDMsg(UInt8, Stream * = NULL);
	bool addCDMsg(UInt8, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, UInt32, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, const std::string&, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, UInt32, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, const std::string&, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, UInt32, UInt32, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, const std::string&, UInt32, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, UInt32, UInt32, Stream * = NULL);
	bool addCDMsg(UInt8, const std::string&, const std::string&, const std::string&, const std::string&, UInt32, Stream * = NULL);

public:
	void addCBReportFromDB(UInt32, ClanBattleReport&);
	void addCBResultFromDB(UInt32, UInt32, UInt8);
	ClanBattleReportElem& getCBReport(UInt32);

public:
	void cleanClanDynamicMsg();

private:
	static const UInt8	MSIZE = 22;
	std::multimap<CDMKey, CDMsg *, CDMKeyLess > _cdm[4];	//人事 城战 技艺 其他
	ClanBattleReportElems _cbrElem;
	UInt32 _key;
	Mutex _mutex;
};

}

#endif