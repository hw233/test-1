#ifndef LOG_INC
#define LOG_INC

#define APP_ERROR		1
#define APP_WARN		2
#define APP_DEBUG		3
#define APP_INFO		4
#define APP_TRACE		5

enum PurchaseType
{
    Item=1,
    JoinClanBattle,
    ExtendPackage,
    DayTaskFlushColor,
    InstantAutoBattle,
    Explore,
    AutoProcessTask,
    FlushBookStore,
    ResourceLost,		// used for pk lose resource
    Transport,
    ClanCreate,
    EnchantEquipment,
    OpenEquipmentSocket,
    MergeGems,
    AttachGems,
    DetachGems,
    SplitEquipment,
    ExchangeEquipment,
    ForgeEquipment,
    BigFighter,
    RecruitFighter,
    FlushFighter,
	VipEnterDungeon,
	DungeonAutoConsume,
	ClanDonate,
	AccTrainFighter,
	TrainFighter,
	ClanBattleRecov,
	PurchaseBook,
    Practice,
    AddPracticeSlot,
    EnterCopy,
    EnterFrontMap,
    ShimenTask,
    ShimenTaskFresh,
    YamenTask,
    YamenTaskFresh,
    AutoCopyComplete,
    InstantPracticeAcc,
    FlushAthletics,
    PurchaseSale,
    TrumpUpgrade,
    TrumpLOrder,
};

enum IncomingType
{
    InFromSale = 128,
};

enum MoneyType
{
  Gold=1,
  Coupon,
  Tael,
  Coin,
  Achievement,
};

enum MailItemType
{
	AthliticisTimeAward=1,
	BossFBAward,
	BossAward,
	FirstReChargeAward,
	VipAward,
	SaleTimeOut,
	SaleBuy,
	SaleCancel,
	TradeMail,
	DismissFighter,
	BackStage,
	Activity,
	DismissCitta,
    AutoCopy,
    CountryBattleAward,
	SaleSell,
};
enum ItemFrom
{
	FromDungeon = 1,
	FromBox,
	FromTrade,
	FromSale,
	FromMerge,
	FromExchangeSet,
	FromExchange,
	FromAthletAward,
	FromMail,
	FromNpc,
	FromVipAward,
	FromOnlineAward,
	FromTaskAward,
	FromLuckyDraw,
	FromDetachGem,
	FromClanBattleAward,
	FromNpcBuy,
	FromSplit,
    FromTripod,
    FromFCMerge,
    FromPExp,
    FromHeroIsland,
    FromAttainment,
};

enum ItemTo
{
	ToTrade = 128,
	ToSale,
	ToSplit,
	ToDelete,
	ToSellNpc,
	ToExchange,
	ToExchangeSet,
	ToDesdroy,
	ToDetachGemDesdroy,	
	ToUse,
	ToClanDonate,
	ToGemMgerge,
	ToActivateAttr,
	ToGemAttach,	
	ToTrainFighter,	
	ToTrumpUpgrade,	
};


struct ConsumeInfo
{
    ConsumeInfo():purchaseType(0),itemId(0),itemNum(0){}
    ConsumeInfo(UInt32 purchaseType_,UInt32 itemId_,UInt32 itemNum_):purchaseType(purchaseType_),itemId(itemId_),itemNum(itemNum_){}

    UInt32 purchaseType;
    UInt32 itemId;
    UInt32 itemNum;
};

class Log
{
public:
	Log(UInt8 lev, const std::string& dir);

	virtual ~Log();

public:
	void OutError(const char * fmt, ...);
	void OutWarn(const char * fmt, ...);
	void OutDebug(const char * fmt, ...);
	void OutInfo(const char * fmt, ...);
	void OutTrace(const char * fmt, ...);
	void OutLog(UInt8 lev, const char * fmt, ...);
	//只是把要输出的日志，打包成字符串

	void FlushLog(UInt8 lev, tm& aTm, const char* buffer);

protected:
	FILE* GetFileHandler(UInt8 lev, tm& aTm);	//fail : NULL
private:
	UInt8		m_LogLev;
	std::string m_LogDir;		//日志目录

	FILE*		m_ErrorFile;
	FILE*		m_WarnFile;
	FILE*		m_DebugFile;
	FILE*		m_InfoFile;
	FILE*		m_TraceFile;
	tm			m_aTm[16];
};


//////////////////////////////////////////////////////////////////////////

#define ERROR_LOG(fmt, ...) \
	do {	\
		LOG()->OutError(fmt "\n", ##__VA_ARGS__);	\
	} while(0)


#define WARN_LOG(fmt, ...)	\
	do {	\
		LOG()->OutWarn(fmt "\n", ##__VA_ARGS__);	\
	} while(0)


#define DEBUG_LOG(fmt, ...)	\
	do {	\
		LOG()->OutDebug(fmt "\n", ##__VA_ARGS__);	\
	} while(0)


#define INFO_LOG(fmt, ...)	\
	do {	\
		LOG()->OutInfo(fmt "\n", ##__VA_ARGS__);	\
	} while(0)

#define TRACE_LOG(fmt, ...)	\
	do {	\
		LOG()->OutTrace(fmt "\n", ##__VA_ARGS__);	\
	} while(0)

#include "Server/WorkerThread.h"
#define LOG()			WorkerThread<WorkerRunner<> >::LocalWorker().GetLog()

extern Log gLog;                            \

#endif
