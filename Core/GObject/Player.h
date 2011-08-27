#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GObjectManager.h"
#include "TaskData.h"
#include "EventBase.h"
#include "GData/LootTable.h"
#include "GData/AttrExtra.h"

#include "Common/AtomicVal.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"

#include "Server/WorldServer.h"
#include "kingnet_analyzer.h"

namespace Battle
{
	class BattleSimulator;
}

namespace GData
{
	class NpcGroup;
	struct LootItem;
	struct DungeonLevel;
	struct AttrExtra;
	struct Attr2Extra;
}

namespace GObject
{
#define PLAYER_BUFF_COUNT			0x19
#define PLAYER_BUFF_DISPLAY_MAX		0x0C
#define PLAYER_BUFF_AUTOHEAL		0x00
#define PLAYER_BUFF_ATTR1			0x01
#define PLAYER_BUFF_ATTR2			0x02
#define PLAYER_BUFF_ATTR3			0x03
#define PLAYER_BUFF_PROTECT			0x04
#define PLAYER_BUFF_TRAINP1			0x05
#define PLAYER_BUFF_TRAINP2			0x06
#define PLAYER_BUFF_CLANMOVE		0x07	//宗族城战行走BUFF
#define PLAYER_BUFF_BANCHAT			0x08
#define PLAYER_BUFF_BOSSWEAK		0x09
#define PLAYER_BUFF_TRAINP3			0x0A	//汤圆升级加速+70%
#define PLAYER_BUFF_TRAINP4			0x0B	//汤圆升级加速+50%
#define PLAYER_BUFF_ATHLETICS		0x0C	//处于竞技buffer, 世界线程处理
#define PLAYER_BUFF_PKLOCK			0x0D
#define PLAYER_BUFF_WEAK			0x0E
#define PLAYER_BUFF_HOLY			0x0F
#define PLAYER_BUFF_ATTACKING		0x10
#define PLAYER_BUFF_CLANBATTING		0x11	//处在宗族城战
#define PLAYER_BUFF_CLANRECOVE		0x12	//
#define PLAYER_BUFF_REENTERCLAN		0x13
#define PLAYER_BUFF_CLANRCENHANCE	0x14
#define PLAYER_BUFF_PWDLOCK			0x15	//5次密码错误 锁定10分钟
#define PLAYER_BUFF_PRACTICE1       0x16	//修炼加成20%
#define PLAYER_BUFF_PRACTICE2       0x17	//修炼加成50%
#define PLAYER_BUFF_XTHTYT          0x18	//已使用了先天混沌元胎

#define CLAN_TASK_MAXCOUNT          5       // 帮派每日最大任务数
#define SHIMEN_TASK_MAXCOUNT        5       // 师门每日最大任务数
#define YAMEN_TASK_MAXCOUNT         5       // 师门每日最大任务数

	class Map;
	class Player;
	class ItemBase;
	class TaskMgr;
	class Trade;
	class Sale;
	class Package;
	class Fighter;
	struct SpotData;
	class Clan;
	class EventBase;
	class ClanBattle;
	class MailBox;
	class Athletics;
    struct PracticeData;
    class AttainMgr;

	class EventAutoBattle : public EventBase
	{
	public:
		EventAutoBattle(Player * player, UInt32 interval, UInt32 count, GData::NpcGroup * ng, UInt32 final)
			: EventBase(player, interval, count), _npcGroup(ng), _finalEnd(final)
		{}
		virtual UInt32 GetID() const { return EVENT_AUTOBATTLE; }
		void Process(UInt32);
		void notify();
		bool instantComplete();

		void updateDB(bool);

	private:
		float calcExpEach(UInt32);

	private:
		GData::NpcGroup * _npcGroup;
		UInt32 _finalEnd;
	};

	class EventFighterTrain : public EventBase
	{
	public:
		EventFighterTrain(Player * player, UInt32 interval, UInt32 count, Fighter * fighter, UInt32 final)
			: EventBase(player, interval, count), _fighter(fighter), _end(final)
		{}
		virtual UInt32 GetID() const { return EVENT_FIGHTERAUTOTRAINING; }
		virtual bool Equal(UInt32 id, size_t fgtId) const;
		void Process(UInt32);
		bool Accelerate(UInt32);
		inline UInt32 GetEnd() { return _end; }

	private:
		Fighter * _fighter;
		UInt32 _end;
	};

    class EventPlayerPractice : public EventBase
    {
    public:
		EventPlayerPractice(Player * player, UInt32 interval, UInt32 count, UInt32 final)
			: EventBase(player, interval, count), _end(final)
		{}

        virtual UInt32 GetID() const { return EVENT_PLAYERPRACTICING; }
        virtual bool Equal(UInt32 id, size_t playerid) const;
        void Process(UInt32);
		bool Accelerate(UInt32);
        inline UInt32 GetEnd() { return _end; }

	private:
        UInt32 _end;
    };

    class EventPlayerTripod : public EventBase
    {
    public:
		EventPlayerTripod(Player * player, UInt32 interval, UInt32 count)
			: EventBase(player, interval, count)
		{}

        virtual UInt32 GetID() const { return EVENT_PLAYERPRTRIPOD; }
        virtual bool Equal(UInt32 id, size_t playerid) const;
        void Process(UInt32);
		bool Accelerate(UInt32);
    };

	struct Lineup
	{
		Lineup(): fid(0), pos(0), fighter(NULL) {}
		inline bool available() { return fid != 0; }
		void updateId();
		UInt32 fid;
		UInt8 pos;
		Fighter * fighter;
	};
	struct LeavingData
	{
		UInt32 timeout;
		Fighter * fighter;
	};

	struct TrainFighterData
	{
		UInt8  priceType;
		UInt32 price;
		UInt32 traintime;
		UInt32 checktime;
		UInt32 trainend;
		UInt32 accExp;
		float  factor;
	}; 

	struct SecondPWDInfo
	{
		std::string secondPWD;
		UInt8 isLocked;
		UInt8 errCount;
		std::string questionForPWD;
		std::string answerForPWD;
		SecondPWDInfo():isLocked(1), errCount(0){}
	};

	struct PlayerData
	{
		static const UInt16 INIT_PACK_SIZE = 100;
		PlayerData()
			: gold(0), coupon(0), tael(0), coin(0), status(0), country(0),
			title(0), achievement(0), location(0), inCity(false), lastOnline(0),
			newGuild(0), packSize(INIT_PACK_SIZE), mounts(0), gmLevel(0), icCount(0), nextIcReset(0),
			formation(0), totalRecharge(0), lastExp(0), lastResource(0),
			rewardStep(0), nextRewardItem(0), nextRewardCount(0), nextRewardTime(0),
			nextExtraReward(0), tavernBlueCount(0), tavernPurpleCount(0), tavernOrangeCount(0),
            smFinishCount(0), smFreeCount(0), smAcceptCount(0), ymFinishCount(0), ymFreeCount(0), ymAcceptCount(0),
            clanTaskId(0), ctFinishCount(0),
			created(0), lockExpireTime(0), wallow(1), battlecdtm(0), dungeonCnt(0), dungeonEnd(0),
            copyFreeCnt(0), copyGoldCnt(0), copyUpdate(0), frontFreeCnt(0), frontGoldCnt(0), frontUpdate(0)
		{
            memset(tavernId, 0, sizeof(tavernId));
            memset(fshimen, 0, sizeof(fshimen));
            memset(fsmcolor, 0, sizeof(fsmcolor));
            memset(shimen, 0, sizeof(shimen));
            memset(smcolor, 0, sizeof(smcolor));
            memset(fyamen, 0, sizeof(fyamen));
            memset(fymcolor, 0, sizeof(fymcolor));
            memset(yamen, 0, sizeof(yamen));
            memset(ymcolor, 0, sizeof(ymcolor));
            memset(bookStore, 0, sizeof(bookStore));
        }


		std::string name;           // 名字
		UInt32 gold;	            // 元宝
		UInt32 coupon;	            // 礼券
		UInt32 tael;	            // 银两
		UInt32 coin;	            // 铜钱
		UInt32 status;              // 状态:0x01 - pk打开 0x02 - 切磋开 0x04 - 小秘书关 0x80 - 禁止探险
		UInt8 country;              // 国家
		UInt8 title;                // 头衔
		UInt32 achievement;         // 战功
		UInt16 location;            // 位置
		UInt8 inCity;               // 城市
		UInt32 lastOnline;          // 上次上线时间
		UInt64 newGuild;            // 新手引导任务
		UInt16 packSize;            // 玩家背包容量
		UInt8 mounts;               // 坐骑
		UInt8 gmLevel;              //
		UInt8 icCount;              // 挂机加速次数
		UInt32 nextIcReset;         // 
		UInt16 formation;            // 
		Lineup lineup[5];           // 
		UInt32 totalRecharge;       // 
		UInt32 lastExp;             // 
		UInt64 lastResource;        // 
		UInt8 rewardStep;           // 
		UInt16 nextRewardItem;      // 
		UInt8 nextRewardCount;      // 
		UInt32 nextRewardTime;      // 
		UInt32 nextExtraReward;     // 
		UInt16 tavernId[6];         // 
		UInt8 tavernBlueCount;      // 
		UInt8 tavernPurpleCount;    // 
		UInt16 tavernOrangeCount;    // 
        UInt32 fshimen[6];          // 刷出的师门任务
		UInt8 fsmcolor[6];          // 刷出的师门任务的颜色
		UInt32 shimen[6];           // 师门任务
		UInt8 smcolor[6];           // 师门任务颜色
        UInt8 smFinishCount;        // 师门任务当日完成次数
        UInt8 smFreeCount;          // 师门任务当日免费刷新次数
        UInt8 smAcceptCount;        // 师门任务当日接受次数
        UInt32 fyamen[6];           // 刷出的衙门任务
		UInt8 fymcolor[6];          // 刷出的衙门任务的颜色
		UInt32 yamen[6];            // 衙门任务
		UInt8 ymcolor[6];           // 衙门任务颜色
        UInt8 ymFinishCount;        // 衙门任务当日完成次数
        UInt8 ymFreeCount;          // 衙门任务当日免费刷新次数
        UInt8 ymAcceptCount;        // 衙门任务当日接受次数
        UInt32 clanTaskId;          // 帮派任务
        UInt8 ctFinishCount;        // 帮派任务当日完成个数
		UInt16 bookStore[6];        // 
		UInt32 created;             // 
		UInt32 lockExpireTime;      // 
		UInt8 wallow;               // 
		UInt32 battlecdtm;          // 
        UInt8 dungeonCnt;           // 通天塔当前次数
        UInt32 dungeonEnd;          // 通天塔当天结束时间
        UInt8 copyFreeCnt;          // 副本免费次数
        UInt8 copyGoldCnt;          // 副本收费次数
        UInt32 copyUpdate;          // 副本次数更新时间
        UInt8 frontFreeCnt;         // 阵图免费次数
        UInt8 frontGoldCnt;         // 阵图收费次数
        UInt32 frontUpdate;         // 阵图次数更新时间
        std::vector<UInt16> formations; // 已学会阵法
	};

	class Player:
		public GObjectBaseT<Player, UInt64>
	{
		friend struct GData::DungeonLevel;
		friend class Dungeon;
	public:
		enum PlayerGlobalFlag
		{
			ClanBattleFlag = 0x00000001,
			CanTaskInit	= 0x00000002,
			Challenging = 0x00000010,		//竞技状态, 世界线程处理
			BeChallenging = 0x00000020,		//非竞技状态
			SGPunish	= 0x00000040,		//变速惩罚
			AllGlobalFlags	= 0xFFFFFFFF
		};

		enum PlayerFlag
		{
			CountryBattle	= 0x00000001,
			Training		= 0x00000002,
			AutoDungeon		= 0x00000004,
			ClanBattling	= 0x00000008,
			AllFlags		= 0xFFFFFFFF
		};

		enum PlayerStatus
		{
			PK = 0x01,
			Challenge = 0x02,
			TopupRewarded = 0x04,
			NoLuckyDraw = 0x80,
			FirstHPLoss = 0x10000,
			FirstDeath = 0x20000,
			AutoCB = 0x40000,
		};

		struct FriendActStruct {
			Player * player;
			UInt8 type;
			union {
				Player * target;
				char str[16];
				UInt64 num;
			};
			inline void assignFriendAct(Player * arg)
			{
				target = arg;
			}
			inline void assignFriendAct(const char * arg)
			{
				strncpy(str, arg, 16);
				str[15] = 0;
			}
			inline void assignFriendAct(UInt64 arg)
			{
				num = arg;
			}
		};

	private:
		struct GreatFighterTaskVal
		{
			UInt16 friendliness;
			UInt16 submitFavorCount;
			UInt32 submitFavorDay;
			GreatFighterTaskVal() : friendliness(0), submitFavorCount(0), submitFavorDay(0) {}
			GreatFighterTaskVal(UInt16 friendliness_, UInt16 submitFavorCount_, UInt32 submitFavorDay_)
				: friendliness(friendliness_), submitFavorCount(submitFavorCount_), submitFavorDay(submitFavorDay_)
			{
			}
		};

	public:
		Player(UInt64);
		~Player();

	public:
		bool Load();
		inline void recalcVipLevel() { _vipLevel = calcVipLevel(); }
		bool Init();
		void UnInit();

		UInt8 GetCountryThread();

		void Login();

		void Reconnect();

		void Logout(bool = false);	//玩家下线操作
		void selfKick();

		void sendWallow();
		void makeWallow(Stream& st);

		void checkLastBattled();

		void checkHPLoss();
		void checkDeath();

		void checkLevUp(UInt8, UInt8);
        bool formationLevUp(UInt16);
        bool addNewFormation(UInt16 newformationId, bool writedb = false);
        void sendFormationList();
        bool checkFormation(UInt16);
        bool checkFormation_ID(UInt16);

	public:
		void sendTopupMail(const char* title, const char* content, UInt32 gold, UInt8 num);
	public:
		inline bool isOnline() const { return _isOnline; }
		inline void setOnline(bool o) { _isOnline = o; }

		inline UInt16 getFormation() const { return _playerData.formation; }
		inline Lineup& getLineup(int idx) { return _playerData.lineup[idx]; }

		inline void SetSessionID(int session) { _session = session; }
		inline int GetSessionID() const { return _session; }

		inline UInt8 getThreadId() const		{ return static_cast<UInt8>(_threadId.value()); }
		inline void setThreadId(UInt8 cny)	 { _threadId = cny; }

		inline UInt8 getCountry() const		{ return _playerData.country; }
		void setCountry(UInt8 cny);

		inline UInt16 getLocation()			{ return _playerData.location; }
		inline void setLocation(UInt16 loc)	{ _playerData.location = loc; }

		inline bool isInCity()				{ return _playerData.inCity ? 1 : 0; }
		inline void setInCity(bool ic)		{ _playerData.inCity = ic ? 1 : 0; }
		inline UInt32 getLastOnline()		{ return _playerData.lastOnline; }
		inline UInt32 getCreated()			{ return _playerData.created; }

		inline PlayerData& getPlayerData()	{ return _playerData; }
		inline void setPlayerData(PlayerData& pd)	{ _playerData = pd; }

		void setGMLevel(UInt8 l);
		inline UInt8 getGMLevel()			{ return _playerData.gmLevel; }

		UInt32 getTotalRecharge()			{ return _playerData.totalRecharge; }
		void addTotalRecharge(UInt32);
		void setTotalRecharge( UInt32 r );
		void genOnlineRewardItems();
		bool takeOnlineReward();
		UInt32 getOnlineReward();
		void writeOnlineRewardToDB();

        inline void setLockExpireTime(UInt32 let)   {_playerData.lockExpireTime=let;}
		inline UInt32   getLockExpireTime() {return _playerData.lockExpireTime;}

		UInt32 getBuffData(UInt8 idx, UInt32 tm = TimeUtil::Now());
		UInt32 getBuffLeft(UInt8 idx, UInt32 tm = TimeUtil::Now());
		void setBuffData(UInt8, UInt32, bool = true);
		void addBuffData(UInt8, UInt32);
		void testBattlePunish();

		inline const std::string& getName() { return _playerData.name; }
		inline const char * getPName() { return _playerData.name.c_str(); }
		inline const std::string& getBattleName() { if(_battleName.empty()) rebuildBattleName(); return _battleName; }
		void rebuildBattleName();

		inline bool hasFlag(UInt32 flag) { return (_flag & flag) != 0; }
		inline void addFlag(UInt32 flag) { _flag |= flag; }
		inline void delFlag(UInt32 flag) { _flag &= ~flag; }

		inline bool hasGlobalFlag(UInt32 flag) { return (_gflag & flag) == flag; }
		inline void addGlobalFlag(UInt32 flag) { _gflag |= flag; }
		inline void delGlobalFlag(UInt32 flag) { _gflag &= ~flag; }

		UInt8 GetClass() const;
		UInt8 GetClassAndSex() const;
		bool IsMale() const;
		UInt8 GetLev() const;
		UInt64 GetExp() const;
		void AddExp(UInt64, UInt8 = 0);
		void AddPExp(UInt32);
		void pendExp(UInt32, bool = false);
		void setLevelAndExp(UInt8, UInt64);
		inline UInt32 getPendExp() { return _playerData.lastExp & 0x7FFFFFFF; }
		bool regenHP(UInt32);

		void pendTael(UInt32);
		void pendCoupon(UInt32);
		void pendCoin(UInt32);
		void pendAchievement(UInt32);
		inline void pendDungeon(UInt16 d) { _lastDungeon = d; }

		void sendMsgCode(UInt8, UInt32, UInt32 = 0);

		void sendModification(UInt8, UInt32, bool = true);
		void updateDB(UInt8, UInt32);

		UInt32 getGold(UInt32 c = 0);
		UInt32 useGold(UInt32 c,ConsumeInfo * ci=NULL);
		bool holdGold(UInt32 c, UInt8);

		UInt32 getGoldOrCoupon();
		UInt32 useGoldOrCoupon(UInt32,ConsumeInfo * ci=NULL);//return gold

		UInt32 getCoupon(UInt32 c = 0);
		UInt32 useCoupon(UInt32 c,ConsumeInfo * ci=NULL);

		UInt32 getTael(UInt32 c = 0);
		UInt32 useTael(UInt32 c,ConsumeInfo * ci=NULL);
		void useTael2(UInt32 c, Player *attacker, ConsumeInfo * ci = NULL);

		UInt32 getCoin(UInt32 c = 0);
		UInt32 useCoin(UInt32 c, ConsumeInfo * ci=NULL, bool notify = true);
		bool holdCoin(UInt32 c, UInt8);

		void addTrainFighterFromDB(UInt32, UInt8, UInt32, UInt32, UInt32, UInt32);
		bool delTrainFighter(UInt32, bool = false);
		bool hasTrainFighter(UInt32);
		bool addTrainFighter(UInt32, UInt8, UInt32);
		bool accTrainFighter(UInt32, UInt32);
		bool cancelTrainFighter(UInt32);
		void makeTrainFighterInfo(Stream&);
		TrainFighterData* getTrainFighterData(UInt32);

		UInt32 addStatus(UInt32 s);
		UInt32 removeStatus(UInt32 s);
		inline bool hasStatus(UInt32 s) { return (_playerData.status & s) == s; }
		inline bool hasStatusAny(UInt32 s) { return (_playerData.status & s) > 0; }
		inline UInt32 addStatusBit(UInt32 bitStart, UInt32 s)
		{ return addStatus(s << bitStart); }
		UInt32 setStatusBit(UInt32 bitStart, UInt32 bitCount, UInt32 s);
		inline UInt32 removeStatusBit(UInt32 bitStart, UInt32 s)
		{ return removeStatus(s << bitStart); }
		inline bool hasStatusBit(UInt32 bitStart, UInt32 s)
		{ return (_playerData.status & (s << bitStart)) == (s << bitStart); }
		inline UInt32 getStatusBit(UInt32 bitStart, UInt32 bitCount)
		{ return (_playerData.status >> bitStart) & (((1 << bitCount) - 1)); }
		bool canClosePK();

		void setTitle(UInt8 s);
		inline UInt8 getTitle() { return _playerData.title; }

		UInt32 getAchievement(UInt32 a = 0);
		UInt32 useAchievement(UInt32 a,ConsumeInfo * ci=NULL);
		void useAchievement2( UInt32 a, Player *attacker, ConsumeInfo * ci = NULL);

		void incIcCount();
		void resetIcCount();
		UInt8 getIcCount();

		inline UInt16 getPacksize() { return _playerData.packSize; }
        inline UInt8 getMounts() { return _playerData.mounts; }
        bool setMounts(UInt8 mounts);

		inline size_t getFighterCount() { return _fighters.size(); }
		bool isFighterFull() const;
		inline bool isMainFighter(UInt32 id) { return id > 0 && id < 10; }
        void upInitCitta(Fighter* fgt);
		void addFighter(Fighter * fgt, bool = true);
		Fighter * takeFighter(UInt32, bool = true);
		void notifyAddFighter(Fighter * fgt);
		Fighter * findFighter(UInt32);
		Fighter * getMainFighter();
		bool hasFighter(UInt32);
		Fighter * removeFighter(UInt32);
		Fighter * removeFighterFromLineup(UInt32);
		void autoLineup(Fighter *);
		void sendAllFightersInfoToGM();
		template<typename _FN>
		void foreachFighter(_FN fn)
		{
			std::for_each(_fighters.begin(), _fighters.end(), fn);
		}
		template<typename _FN>
		void foreachLineup(_FN fn)
		{
			for(int i = 0; i < 5; ++ i)
			{
				if(_playerData.lineup[i].fighter != NULL)
					_FN(_playerData.lineup[i].fighter);
			}
		}

		UInt16 addFightCurrentHp(UInt32 id, UInt16 hp, bool battle = false);
		void   addFightCurrentHpAll(UInt16 hp);

		void updateBattleFighters(bool = true);
		void storeFighters();
		bool setFormation(UInt16);
		void makePlayerInfo(Stream&);
		void makeFormationInfo(Stream&);
		void makeFighterList(Stream&);
		void makeFighterInfo(Stream&, Fighter *, bool = true);
		bool makeFighterInfo(Stream&, UInt32);

		void autoRegenAll();
		void regenAll();

		void   checkGreatFighterFriendliness(UInt32 id);
		bool   setGreatFighterTaskVal(UInt32 id, UInt16 friendliness, UInt16 submitFavorCount, UInt32 submitFavorDay);
		void   setGreatFighterFriendliness(UInt32 id, UInt16 friendliness, bool = true);
		UInt16 getGreatFighterFriendliness(UInt32 id);

        UInt32 getClientAddress();

		void   sendGreatFighterFavorSubmitCount(UInt32 id);
		bool   submitGreatFighterFavor(UInt32 id);
		UInt16 getGreatFighterFavorSubmitCount(UInt32 id);
		UInt32 getGreatFighterFavor(UInt32 id);

		bool setNewGuildTaskStep(UInt32);
        void sendNewGuild();

        bool OperationTaskAction(int type);

	private:
		void  greatFighterFavorSubmitCheck(GreatFighterTaskVal *, UInt32, UInt32 = TimeUtil::SharpDay(0));

	public:
		Map* GetMap();
		SpotData * GetMapSpot();

	public:
		void send(const void *, int);
		inline void send(Stream& st)
		{ send(&st[0], st.size()); }
		template <typename MsgType>
		inline void send(MsgType& msg)
		{ if(_isOnline) NETWORK()->SendMsgToClient(_session, msg); 	}

	public:
		void moveTo(UInt16, bool);
		void moveToHome();
		void moveToNeutralHome();

		//战斗相关
		bool challenge(Player *, UInt32 * = NULL, int * = NULL, bool = true, UInt32 = 0);
		bool attackNpc(UInt32, UInt32 = 0xFFFFFFFF, bool = false);
        bool attackCopyNpc(UInt32, UInt8, UInt8);
		bool autoBattle(UInt32);
		void pushAutoBattle(UInt32, UInt16, UInt16);
		void pushAutoDungeon(UInt32, UInt32, UInt8);
		void cancelAutoBattle();
		void cancelAutoBattleNotify();
		void instantAutoBattle();
		void cancelAutoDungeon();
		void sendEvents();
		void sendAutoBattleEvent(EventBase *);
		void sendOnlineReward();
		void sendDailyInfo();

		inline UInt32 getNextExtraReward()
		{ return _playerData.nextExtraReward; }
		void setNextExtraReward(UInt32 ner);
		void exchangeForExtraReward();

		//宗族系统
		const std::string& getClanName() const;
		void setClan(Clan * c);
		inline Clan * getClan() { return _clan; }
		inline void setClanBattle(ClanBattle * c)  { _clanBattle = c; }
		inline ClanBattle * getClanBattle() { return _clanBattle; }

		//////////////////////////////////////////////////////////////////////////
		//组队系统
		inline bool IsInTeam() const { return false; }	//TODO

	public:
		UInt16   GetFreePackageSize();
		bool     ExtendPackageSize();

		Package* GetPackage() { return m_Package; }
		TaskMgr* GetTaskMgr() { return m_TaskMgr; }
		MailBox* GetMailBox() { return m_MailBox; }
		AttainMgr* GetAttainMgr() { return m_AttainMgr; }

		Trade* GetTrade()			{ return m_Trade; }
		Sale* GetSale()				{ return m_Sale; }
		Athletics* GetAthletics()	{ return m_Athletics; }


	// 好友系统
	public:
		void addFriendFromDB(Player *);
		void addBlockFromDB(Player *);
		void addFoeFromDB(Player *);
		bool addFriend(Player *);
		void delFriend(Player *);
		bool addBlock(Player *);
		bool delBlock(Player *);
		inline bool hasBlock(Player *pl) { Mutex::ScopedLock lk(_mutex); return _hasBlock(pl); }
		bool addFoe(Player *);
		bool delFoe(Player *);
		inline bool isFriendFull() { return _friends[0].size() >= 20; }
		bool testCanAddFriend(Player *);

		void sendFriendList(UInt8, UInt8, UInt8);

		void PutFighters(Battle::BattleSimulator&, int side, bool fullhp = false);

		inline void setNextTavernUpdate(UInt32 n) { _nextTavernUpdate = n; }
		void writeTavernIds();
		void writeShiMen();
		void writeYaMen();
        bool addAwardByTaskColor(UInt32, bool = false);
        void delColorTask(UInt32);

        bool finishClanTask(UInt32);
        void delClanTask();
        void buildClanTask();
        void resetClanTask();
        UInt32 getClanTaskId();
        bool isClanTask(UInt32);
        bool isClanTaskFull();
        void writeClanTask();
        void AddClanBuilding(UInt32);
        void AddClanContrib(UInt32);

		void listRecruit(UInt8 type, UInt8, UInt16);
		void flushTaskColor(UInt8 tasktype, UInt8 type, UInt8, UInt16, bool = false);
        void sendColorTask(UInt8 ttype, UInt16 ncount);
        bool ColorTaskOutOfAccept(UInt8 type, bool = false);
        void ColorTaskAccept(UInt8 type, UInt32 taskid);
        void ColorTaskAbandon(UInt8 type, UInt32 taskid);
        void clearFinishCount();
		UInt16 calcNextTavernUpdate(UInt32);
		UInt32 hireRecruit(UInt8);
		void updateNextTavernUpdate(UInt32);

		void exceptAvailableFighters(std::map<UInt32, UInt32>&);
        UInt32 getColorFighterNum(UInt8 color);

		inline void setNextBookStoreUpdate(UInt32 n) { _nextBookStoreUpdate = n; }
		void writeBookStoreIds();
		void listBookStore(UInt8 type);
		UInt16 calcNextBookStoreUpdate(UInt32);
		UInt32 purchaseBook(UInt8);
		void updateNextBookStoreUpdate(UInt32);

		UInt8 trainFighter(UInt32 id, UInt8 type);

		inline UInt32 getVipLevel() { return _vipLevel; }
		bool isDungeonPassed(UInt8 id);

		template<typename T>
		inline void notifyFriendAct(UInt8 type, T arg)
		{
#if 0
			Mutex::ScopedLock lk(_mutex);
			if(_friends[0].empty())
				return;
			std::set<Player *>::iterator it;
			for(it = _friends[0].begin(); it != _friends[0].end(); ++ it)
			{
				FriendActStruct * fas = new FriendActStruct;
				fas->type = type;
				fas->player = this;
				fas->assignFriendAct(arg);
				Player * player = *it;
				player->pushFriendAct(fas);
			}
#endif
		}

		void sendFriendActList();

		std::string& fixName(std::string& name);
		inline void patchMergedName() { patchMergedName(_id, _playerData.name); }
		static void patchMergedName(UInt64 id, std::string& name);

		void autoCB(bool = true);

	public:
		static void setTavernInterval(UInt32);
		static void setTavernRate(UInt32);

        static void setRecruitCost(UInt32 recruit_cost);
        static void setTavernBlueCount(UInt32 tavernBlueCount);
        static void setTavernPurpleCount(UInt32 tavernPurpleCount);
        static void setTavernOrangeCount(UInt32 tavernPurpleCount);

		inline Mutex& getMutex() { return _mutex; }

	private:
		UInt32 calcVipLevel();
		Player * _findFriend( UInt8 type, std::string& name );
		bool _hasFriend( UInt8 type, Player * pl ) const;
		inline Player * _findFriend(std::string& name) { return _findFriend(0, name); }
		inline bool _hasFriend(Player *pl) const { return _hasFriend(0, pl); }
		inline Player * _findBlock(std::string& name) { return _findFriend(1, name); }
		inline bool _hasBlock(Player *pl) const { return _hasFriend(1, pl); }
		inline Player * _findFoe(std::string& name) { return _findFriend(2, name); }
		inline bool _hasFoe(Player *pl) const { return _hasFriend(2, pl); }

		void appendFriendAct(Stream&, FriendActStruct*);
		void pushFriendAct(FriendActStruct * fas);
		void addFriendInternal(Player *, bool, bool = true);
		void delFriendInternal(Player *, bool = true);

		void sendVIPMails(UInt8, UInt8);
		void checkIcExpire(bool = true);
		void sendBlockBossMail(UInt8, UInt8);

	private:
		Mutex _mutex;

		std::map<UInt32, Fighter *> _fighters;
		std::map<UInt32, TrainFighterData *> _trainFighters;
		std::map<UInt32, GreatFighterTaskVal> _greatFighterTaskVal;
		std::set<UInt32> _greatFighterFull;

		Package* m_Package;

		std::set<Player *> _friends[3];
		std::vector<FriendActStruct *> _friendActs;

		TaskMgr* m_TaskMgr;
		Trade* m_Trade;
		Sale* m_Sale;
		Athletics* m_Athletics;

		AttainMgr* m_AttainMgr;

		MailBox* m_MailBox;

		bool _isOnline;

		PlayerData _playerData;
		UInt32 _buffData[PLAYER_BUFF_COUNT];

		AtomicVal<UInt8> _threadId;
		AtomicVal<int> _session;
		bool _availInit;

		UInt32 _vipLevel;

		Clan * _clan;
		ClanBattle * _clanBattle;
		std::string _battleName;
		UInt32 _flag, _gflag;

		UInt32 _onlineDuration; // for wallow use
		UInt32 _offlineTime; // for wallow use
		UInt32 _nextTavernUpdate;
		UInt32 _nextBookStoreUpdate;

		UInt8 _bossLevel;
		GData::NpcGroup *_ng;

		SecondPWDInfo _pwdInfo;

		// Last battled monster
		GData::NpcGroup * _lastNg;
		UInt16 _lastDungeon;
		std::vector<GData::LootResult> _lastLoot;

        static UInt32 _recruit_cost;
        static UInt32 _tavernBlueCount;
        static UInt32 _tavernPurpleCount;
        static UInt32 _tavernOrangeCount;
		static UInt32 _tavernInterval, _tavernRate;
		static UInt32 _bookStoreInterval, _bookStoreRate;
		UInt32 _exchangeTicketCount;//use for exchange plane ticket (new year activity)

        UInt32 _praplace;

        // 通天塔正义之吼
        UInt8 _justice_roar;
	public:
		void setTicketCount(UInt32 cnt, bool writedb = true);
		inline UInt32 getTicketCount(){return _exchangeTicketCount;}

        inline bool isPracticing() { return _praplace; }
        inline void setPracticingPlaceSlot(UInt32 place) { _praplace = place; }
        inline UInt32 getPracticePlaceSlot() { return _praplace; }
        inline UInt32 getPracticePlace() { return _praplace>>16&0xffff; }
        inline UInt32 getPracticeSlot() { return _praplace&0xffff; }
        float getPracticeBufFactor();

        inline void setJusticeRoar(UInt8 v) { _justice_roar = v; }
        inline UInt8 getJusticeRoar() { return _justice_roar; }

	protected:
		inline void setBlockBossByLevel();
	public:
		bool attackBlockBoss();
		UInt8 getBlockBossLevel(){return _bossLevel;};
		void setBossLevel(UInt8 lvl, bool writedb = true);
	public:
		UInt8 unLockSecondPWD(std::string pwd);
		void lockSecondPWD();
	public:
		void setSecondPWDDB(std::string pwd, std::string question, std::string answer);
		UInt8 activeSecondPWD(std::string pwd, std::string question, std::string answer);
		UInt8 forgetSecondPWD(std::string pwd, std::string answer);
		UInt8 deactiveSecondPWD(std::string answer);
		UInt8 changeSecondPWD(std::string oldPWD, std::string newPWD);
		std::string& getQuestionForPWD(){ return _pwdInfo.questionForPWD; }
		void makeSenconPWDInfo(Stream& st);
		bool hasChecked();

    public:
        void payPractice(UInt8 place, UInt16 slot, UInt8 type, UInt8 priceType, UInt8 time, UInt8 prot);
        void addPracticeFighter(UInt32* fighters, size_t size);

    private:
        UInt32 m_tripodAwdId;
        UInt8 m_tripodAwdNum;
    public:
        void setTripodAwdIdNum(UInt32 id, UInt8 num)
        {
            m_tripodAwdId = id;
            m_tripodAwdNum = num;
        }
        void getTripodAwdIdNum(UInt32& id, UInt8& num)
        {
            id = m_tripodAwdId;
            num = m_tripodAwdNum;
        }

    public:
        void udpLog(const char* umsg, const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count = 1);
    private:
        CUserLogger* m_ulog;
	};

#define PLAYER_DATA(p, n) p->getPlayerData().n

	typedef GGlobalObjectManagerT<Player, UInt64> GlobalPlayers;
	extern GlobalPlayers globalPlayers;
	extern GlobalPlayers newPlayers;
	typedef GGlobalObjectManagerIStringT<Player> GlobalNamedPlayers;
	extern GlobalNamedPlayers globalNamedPlayers;

	class ChallengeCheck
	{
	public:
		struct PlayerPair
		{
			Player * atker;
			Player * defer;
			inline bool operator < (const PlayerPair& other ) const
			{
				if(atker == other.atker)
					return defer < other.defer;
				return atker < other.atker;
			}
		};
		inline bool hasPair(Player * atker, Player * defer)
		{
			FastMutex::ScopedLock lk(_mutex);
			struct PlayerPair p = {atker, defer};
			return _pairs.find(p) != _pairs.end();
		}
		inline void pushPair(Player * atker, Player * defer)
		{
			FastMutex::ScopedLock lk(_mutex);
			struct PlayerPair p = {atker, defer};
			_pairs.insert(p);
		}
		inline void clear()
		{
			FastMutex::ScopedLock lk(_mutex);
			_pairs.clear();
		}
	private:
		std::set<PlayerPair> _pairs;
		FastMutex _mutex;
	};
	extern ChallengeCheck challengeCheck;

}

#endif // _PLAYER_H_
