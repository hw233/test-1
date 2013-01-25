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
    FromTJ                      = 72,
    IDIPBuyItem                 = 73,
    RC7DayLogin                 = 74,
    MoveEquip                   = 75,
    ThanksGivingDay             = 76,
    LogArenaExtraAct            = 77,
    FightTransform              = 78,
    AutoJobHunter               = 79,
    AutoJobHunterComplete       = 80,
    EnumFirstRecharge1          = 81,
    EnumFirstRecharge2          = 82,
    EnumFirstRecharge3          = 83,
    EnumFirstRecharge4          = 84,
    EnumCopyFrontWin            = 85,
    Enum3366Gift                = 86,
    BUYJOYBUFF                  = 87, //购买心悦buff
    EnumFEASTGIFT               = 88, //圣诞-登录礼包人人有
    //繁体私有占150～200
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
    BuChang1530,
    BuChangNewRC7Day,
    NEWJOBHIRE,
    BuChangMo,
    NewCountryBattleAward,
};
enum ItemFrom
{
	FromDungeon = 1, //决战之地（之前注释是副本）
	FromBox = 2, //宝箱
	FromTrade = 3, //交易买入
	FromSale = 4, //交易卖出
	FromMerge = 5, //合成
	FromExchangeSet = 6, //装备部位置换
	FromExchange = 7, //装备置换
	FromAthletAward = 8, //斗剑场奖励
	FromMail = 9, //邮件
	FromNpc = 10, //NPC
	FromVipAward = 11, //vip奖励
	FromOnlineAward = 12, //在线奖励
	FromTaskAward = 13, //任务奖励
	FromLuckyDraw = 14, //探险
	FromDetachGem = 15, //宝石拆卸
	FromClanBattleAward = 16, //帮战奖励
	FromNpcBuy = 17, //NPC购买
	FromSplit = 18, //装备分解
    FromTripod = 19, //九疑鼎
    FromFCMerge = 20, //法宝合成
    FromPExp = 21, //散仙解雇时返回的物品
    FromHeroIsland = 22, //英雄岛
    FromTeamCopy = 23, //组队副本
    FromAttainment = 24, //成就
    FromDailyActivity = 25, //活跃度
    FromEquipUpgrade = 26, //装备炼化
    FromClan = 27, //帮派
    FromTownDeamon = 28, //锁妖塔
	FromSSDTAward = 29, //搜索地图活动奖励
	FromHappyAward = 30, //欢乐转盘奖励(或感恩节活动)
    FromIDIP = 31, //新用户注册邀请好友奖励（或其他活动）
    FromQixi = 32, //七夕（或类似活动）
    FromKillMonster = 33, //斩妖除魔
    FromNewRC7DayLogin = 34, //注册七日登录奖励
    FromNewRC7DayRecharge = 35, //注册七日充值返礼
    FromNewRC7DayTarget = 36, //七日目标
    FromDailyStrengthen = 37, //变强秘宝
    FromJobHunter = 38, // 寻墨游戏

    //80 ~ 100，繁体占用
    FromDreamer = 39, // 水晶梦境
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
