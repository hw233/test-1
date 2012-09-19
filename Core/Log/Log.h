#ifndef LOG_INC
#define LOG_INC

#define APP_ERROR		1
#define APP_WARN		2
#define APP_DEBUG		3
#define APP_INFO		4
#define APP_TRACE		5

enum PurchaseType
{
    Item                        = 1,
    JoinClanBattle              = 2,
    ExtendPackage               = 3,
    DayTaskFlushColor           = 4,
    InstantAutoBattle           = 5,
    Explore                     = 6,
    AutoProcessTask             = 7,
    FlushBookStore              = 8,
    ResourceLost                = 9,		// used for pk lose resource
    Transport                   = 10,
    ClanCreate                  = 11,
    EnchantEquipment            = 12,
    OpenEquipmentSocket         = 13,
    MergeGems                   = 14,
    AttachGems                  = 15,
    DetachGems                  = 16,
    SplitEquipment              = 17,
    ExchangeEquipment           = 18,
    ForgeEquipment              = 19,
    BigFighter                  = 20,
    RecruitFighter              = 21,
    FlushFighter                = 22,
	VipEnterDungeon             = 23,
	DungeonAutoConsume          = 24,
	ClanDonate                  = 25,
	AccTrainFighter             = 26,
	TrainFighter                = 27,
	ClanBattleRecov             = 28,
	PurchaseBook                = 29,
    Practice                    = 30,
    AddPracticeSlot             = 31,
    EnterCopy                   = 32,
    EnterFrontMap               = 33,
    ShimenTask                  = 34,
    ShimenTaskFresh             = 35,
    YamenTask                   = 36,
    YamenTaskFresh              = 37,
    AutoCopyComplete            = 38,
    InstantPracticeAcc          = 39,
    FlushAthletics              = 40,
    PurchaseSale                = 41,
    TrumpUpgrade                = 42,
    TrumpLOrder                 = 43,
    AutoFrontMapComplete        = 44,
    AthleticKillCD              = 45,
    HeroIslandSkill             = 46,
    HeroIslandAuto              = 47,
    TeamCopyAwardRoll           = 48,
    DailyActivity               = 49,
    ClanRankBattleSkill         = 50,
    ForEquipUpgrade             = 51,
    PrintDemon                  = 52, // 除魔印消费
    EnterAutoFrontMap           = 53,
    EnterAutoCopy               = 54,
    TownDeamonAuto              = 55,
    EquipAttachSpirit           = 56,
    AthleticPaging              = 57,
    Discount3                   = 58,
    Discount5                   = 59,
    Discount8                   = 60,
    LuckyDraw                   = 61,
    ArenaBet                    = 62,
    AthleticKillCD2             = 63,
    AthleticPhysical            = 64,
    TianjieDonate               = 65,
    TianjieTask                 = 66,
    DiscountSp1                 = 67,
    DiscountSp2                 = 68,
    DiscountSp3                 = 69,
    ArenaFromCard               = 70,
    ArenaFromMail               = 71,
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
    AutoFrontMap,
    ClanBattleAward,
    RealItemAwardActive,
    NewDrawingGameAward,
    SplitCitta,
    BlueDiamondCmd,
    ClanCopyAward,
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
    FromTeamCopy,
    FromAttainment,
    FromDailyActivity,
    FromEquipUpgrade,
    FromClan,
    FromTownDeamon,
	FromSSDTAward,
	FromHappyAward,
    FromIDIP,
    FromQixi,
    FromKillMonster, //斩妖除魔
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
	ToDesdroy, // ToTripod
	ToDetachGemDesdroy,
	ToUse,
	ToClanDonate,
	ToGemMgerge,
	ToActivateAttr,
	ToGemAttach,
	ToTrainFighter,
	ToTrumpUpgrade,
    ToEquipUpgrade,
    ToForgeFighter,
    ToSecondSoul,
    ToArenaBet,
    ToSkillStrengthenOpen,
	ToSkillStrengthenUpgrade,
    ToQixi,
};

struct IncommingInfo
{
    IncommingInfo():incommingType(0),itemId(0),itemNum(0){}
    IncommingInfo(UInt32 incommingType_,UInt32 itemId_,UInt32 itemNum_):incommingType(incommingType_),itemId(itemId_),itemNum(itemNum_){}

    UInt32 incommingType;
    UInt32 itemId;
    UInt32 itemNum;
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


#ifndef _JSON_PROXY_
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
#else

#define ERROR_LOG(fmt, ...)
#define WARN_LOG(fmt, ...)
#define DEBUG_LOG(fmt, ...)
#define INFO_LOG(fmt, ...)
#define TRACE_LOG(fmt, ...)

#endif

extern Log gLog;                            \

#endif
