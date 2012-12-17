#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "GObjectManager.h"
#include "TaskData.h"
#include "EventBase.h"
#include "Var.h"
#include "GVar.h"
#include "GData/LootTable.h"
#include "GData/AttrExtra.h"

#include "Common/AtomicVal.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"

#include "Server/WorldServer.h"
#include "Battle/BattleSimulator.h"
#ifndef _WIN32
#include "kingnet_analyzer.h"
#endif
#include "Script/lua_tinker.h"
#include "Mail.h"
#include "GObject/NewRelation.h"
#include "StrengthenMgr.h"
#include "JobHunter.h"

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
#define PLAYER_BUFF_AUTOHEAL		0x00
#define PLAYER_BUFF_ATTR1			0x01
#define PLAYER_BUFF_ATTR2			0x02
#define PLAYER_BUFF_ATTR3			0x03
#define PLAYER_BUFF_PROTECT			0x04    //??v?ӳ?
#define PLAYER_BUFF_TRAINP1			0x05
#define PLAYER_BUFF_TRAINP2			0x06
#define PLAYER_BUFF_CLANMOVE		0x07	//??????ս????BUFF
#define PLAYER_BUFF_BANCHAT			0x08
#define PLAYER_BUFF_BOSSWEAK		0x09
#define PLAYER_BUFF_TRAINP3			0x0A	//??Բ????????+80%
#define PLAYER_BUFF_TRAINP4			0x0B	//??Բ????????+50%
#define PLAYER_BUFF_ATHLETICS		0x0C	//???ھ???buffer, ?=??̴߳???
#define PLAYER_BUFF_PKLOCK			0x0D
#define PLAYER_BUFF_WEAK			0x0E
#define PLAYER_BUFF_HOLY			0x0F
#define PLAYER_BUFF_ATTACKING		0x10
#define PLAYER_BUFF_CLANBATTING		0x11	//??????????ս
#define PLAYER_BUFF_CLANRECOVE		0x12	//
#define PLAYER_BUFF_REENTERCLAN		0x13
#define PLAYER_BUFF_CLANRCENHANCE	0x14
#define PLAYER_BUFF_PWDLOCK			0x15	//5?????????? ????10????
#define PLAYER_BUFF_ADVANCED_P_HOOK	0x16    //随身修为加速符: 20%
#define PLAYER_BUFF_PRACTICE1       0x17	//修为加速: 50%
//#define PLAYER_BUFF_PRACTICE2       0x17	//??v?ӳ?50% XXX: ??ʱ????
//#define PLAYER_BUFF_XTHTYT          0x18	//??ʹ????????????Ԫ̥
#define PLAYER_BUFF_ADVANCED_HOOK	0x18    //随身挂机加速符: 50%
#define PLAYER_BUFF_WBOSS           0x19	//?Ѷ??????=?BOSS????
#define PLAYER_BUFF_YDOTR           0x20    //??????????ֵ?ܶ?
#define PLAYER_BUFF_AUTOCOPY        0x21	//?Զ?????
#define PLAYER_BUFF_ONLINE          0x22	//?ۻ?????ʱ??
#define PLAYER_BUFF_HIFIGHT         0x23    //Ӣ?۵???????ȴ
#define PLAYER_BUFF_HIWEAK          0x24
#define PLAYER_BUFF_HIMASTER_SOUL   0x25
#define PLAYER_BUFF_HIMOVE          0x26

#define PLAYER_BUFF_HIRA1           0x27    //  5489
#define PLAYER_BUFF_HIRA2           0x28    //  5490
#define PLAYER_BUFF_HIRA3           0x29    //  5491
#define PLAYER_BUFF_HIRA4           0x2A    //  5492
#define PLAYER_BUFF_HIRA5           0x2B    //  5493
#define PLAYER_BUFF_HIRA6           0x2C    //  5494
#define PLAYER_BUFF_HIRA7           0x2D    //  5495
#define PLAYER_BUFF_HIRA8           0x2E    //  5496
#define PLAYER_BUFF_HIRA9           0x2F    //  5497
#define PLAYER_BUFF_HIRA10          0x30    //  5498
#define PLAYER_BUFF_HIRA11          0x31    //  5499
#define PLAYER_BUFF_HIRA12          0x32    //  5500
#define PLAYER_BUFF_HIRA13          0x33    //  5501
#define PLAYER_BUFF_HIRA14          0x34    //  5502
#define PLAYER_BUFF_HIRA15          0x35    //  5503
#define PLAYER_BUFF_HIRA16          0x36    //  5504
#define PLAYER_BUFF_HIRA17          0x37    //  5505
#define PLAYER_BUFF_HIRA18          0x38    //  5506
#define PLAYER_BUFF_HIRA19          0x39    //  5507
#define PLAYER_BUFF_HIRA20          0x3A    //  5508

#define PLAYER_BUFF_HIPG            0x3B    // ?̹?֮f
#define PLAYER_BUFF_HIBT            0x3C    // ???篿?
#define PLAYER_BUFF_HILN            0x3D    // ??Ԫ??ŭ
#define PLAYER_BUFF_HIJZ            0x3E    // ???н???
#define PLAYER_BUFF_HIESCAPE        0x3F    // Ӣ?۵?????

#define PLAYER_BUFF_AMARTIAL_WIN    0x40    // ??????wʤ??????
#define PLAYER_BUFF_YBUF            0x41
#define PLAYER_BUFF_BBUF            0x42
#define PLAYER_BUFF_N_ATHLETICS     0x44    //邀请斗剑冷却

#define PLAYER_BUFF_ATHLETICS_P     0x45    //历练冷却时间
#define PLAYER_BUFF_QQVIPBUF        0x46
#define PLAYER_BUFF_SUFFER          0x47    //陷害间隔
#define PLAYER_BUFF_JOYBUFF         0x48    //心悦会员

#define PLAYER_BUFF_ATHL1           0x51
#define PLAYER_BUFF_ATHL2           0x52
#define PLAYER_BUFF_ATHL3           0x53
#define PLAYER_BUFF_ATHL4           0x54
#define PLAYER_BUFF_ATHL5           0x55
#define PLAYER_BUFF_ATHL6           0x56
#define PLAYER_BUFF_ATHL7           0x57
#define PLAYER_BUFF_ATHL8           0x58
#define PLAYER_BUFF_ATHL9           0x59

#define PLAYER_BUFF_DISPLAY_MAX		0x5F
#define PLAYER_BUFF_COUNT			0x5F

#define CLAN_TASK_MAXCOUNT          5       // ????ÿ????????????
#define SHIMEN_TASK_MAXCOUNT        5       // ʦ??ÿ????????????
#define YAMEN_TASK_MAXCOUNT         5       // ʦ??ÿ????????????

#define MAX_PRACTICE_FIGHTRES       10      // ??????vɢ????

#define MAX_TRIPOD_SOUL 100000
#define POINT_PERMIN (60*10)

#define MAX_FRIENDS 30
#define MAX_CFRIENDS 50

#define QIXI_MAX_STEPS  24
#if 0
#define ARENA_WEEK_START      2
#define ARENA_WEEK_END        3
#define ARENA_SINGUP_START    (TimeUtil::Now()-TimeUtil::SharpDay(0))/3600*3600
#define ARENA_SINGUP_END      ARENA_SINGUP_START+30*60
#define ARENA_SUFFER_END      ARENA_SINGUP_END+15*60
#else
#define ARENA_WEEK_START      2
#define ARENA_WEEK_END        3
#define ARENA_SINGUP_START    13*3600
#define ARENA_SINGUP_END      ARENA_SINGUP_START+30*60
#define ARENA_SUFFER_END      ARENA_SINGUP_END+15*60
#endif
#define ARENA_ACT_SYSTEM          10
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
    struct TeamData;
    class TeamCopyPlayerInfo;
    class ActivityMgr;
    class HeroMemo;
    class ShuoShuo;
    class CFriend;
    class HoneyFall;
    struct DeamonPlayerData;

    struct TripodData
    {
        TripodData()
        {
            memset(this, 0x00, sizeof(*this));
            quality = 2;
            needgen = 1;
        }

        UInt32 soul;    // Ԫ??ֵ
        UInt8 fire;     // ????: 0-??ͨ?Ļ? 1-??Ŀ?λ? 2-???????? 3-??ľ???? 4-v??ڤ?? 5-???????? 6-????????
        UInt8 quality;  // ????Ʒ?? 1-?? 2-?? 3-6 4-?? 5-??
        UInt8 awdst;    // ????״̬ 0-??v?? 1-δ??ȡ
        UInt8 needgen;  // ??Ҫ???????ɽ???
        UInt32 itemId;
        UInt8 num;
    };

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
		bool instantComplete();

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

    class EventAutoCopy : public EventBase
    {
    public:
		EventAutoCopy(Player * player, UInt32 interval, UInt32 count, UInt8 id)
			: EventBase(player, interval, count), id(id)
		{}

        virtual UInt32 GetID() const { return EVENT_AUTOCOPY; }
        virtual bool Equal(UInt32 id, size_t playerid) const;
        void Process(UInt32);
		bool Accelerate(UInt32);

    private:
        UInt8 id;
    };

    class EventAutoFrontMap : public EventBase
    {
    public:
		EventAutoFrontMap(Player * player, UInt32 interval, UInt32 count, UInt8 id, UInt8 spot)
			: EventBase(player, interval, count), id(id), spot(spot)
		{}

        virtual UInt32 GetID() const { return EVENT_AUTOFRONTMAP; }
        virtual bool Equal(UInt32 id, size_t playerid) const;
        void Process(UInt32);
		bool Accelerate(UInt32);

    private:
        UInt8 id;
        UInt8 spot;
    };

    class EventPlayerTimeTick : public EventBase
    {
    public:
		EventPlayerTimeTick(Player * player, UInt32 interval, UInt32 count, UInt8 type)
			: EventBase(player, interval, count), type(type)
		{}

        virtual UInt32 GetID() const { return EVENT_TIMETICK; }
        virtual bool Equal(UInt32 id, size_t playerid) const;
        void Process(UInt32);
		bool Accelerate(UInt32);

    private:
        UInt8 type;
    };

    class EventTlzAuto : public EventBase
    {
    public:
	    EventTlzAuto(Player * player, UInt32 interval,UInt32 count);

    public:
	    virtual UInt32 GetID() const { return EVENT_TLZAUTO; }
	    void Process(UInt32);
	    void complete() const;
        void notify(bool isBeginAuto = false);

    };

    class EventAutoJobHunter : public EventBase
    {
    public:
		EventAutoJobHunter(Player * player, UInt32 interval, UInt32 count, UInt8 id)
			: EventBase(player, interval, count), id(id)
		{}

        virtual UInt32 GetID() const { return EVENT_JOBHUNTER; }
        virtual bool Equal(UInt32 id, size_t playerid) const;
        void Process(UInt32);
		bool Accelerate(UInt32);

    private:
        UInt8 id;
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

    struct PracticeFighterExp
    {
        UInt32   goldUse;
        UInt32   fids[MAX_PRACTICE_FIGHTRES];
        UInt8    counts[MAX_PRACTICE_FIGHTRES];
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

    struct LastAthAward
    {
        UInt32 prestige;
        UInt32 itemId;
        UInt8  itemCount;
    };

    struct LevelChange
    {
        UInt8 oldLv;
        UInt8 newLv;
    };

    struct CopyTeamPage
    {
        UInt8 copyId;
        UInt8 t;   //?Ѷ?
        UInt32 start;
        UInt32 end;
        UInt8 type;
    };

    struct ClanSkill
    {
        UInt8 id;
        UInt8 level;	//???? = 0?? ??ʾ?˼?????δ??????
    };

    // Use in Qixi Active
    // Use in Wansheng Active
    struct QixiInfo
    {
        Player* lover;
        UInt8 bind;
        UInt8 pos;
        UInt8 event;
        UInt32 score;
        QixiInfo() : lover(NULL), bind(0), pos(0), event(0), score(0) {}
    };

	struct PlayerData
	{
		static const UInt16 INIT_PACK_SIZE = 150;
		PlayerData()
			: gold(0), coupon(0), tael(0), coin(0), prestige(0), status(0), country(0),
			title(0), achievement(0), attainment(0) , qqvipl(0), qqvipyear(0), qqawardgot(0), qqawardEnd(0), ydGemId(0), location(0), inCity(false), lastOnline(0),
			newGuild(0), packSize(INIT_PACK_SIZE), mounts(0), gmLevel(0), icCount(0), nextIcReset(0),picCount(0) , nextPIcReset(0),
			formation(0), totalRecharge(0), lastExp(0), lastResource(0),
			rewardStep(0), nextRewardItem(0), nextRewardCount(0), nextRewardTime(0),
			nextExtraReward(0), tavernBlueCount(0), tavernPurpleCount(0), tavernOrangeCount(0),
            smFinishCount(0), smFreeCount(0), smAcceptCount(0), ymFinishCount(0), ymFreeCount(0), ymAcceptCount(0),
            clanTaskId(0), ctFinishCount(0),
			created(0), lockExpireTime(0), wallow(1), battlecdtm(0), dungeonCnt(0), dungeonEnd(0),
            copyFreeCnt(0), copyGoldCnt(0), copyUpdate(0), frontFreeCnt(0), frontGoldCnt(0), frontUpdate(0)
#ifdef _ARENA_SERVER
            , entered(0)
#endif
		{
            memset(tavernId, 0, sizeof(tavernId));
            memset(fshimen, 0, sizeof(fshimen));
            memset(fsmcolor, 0, sizeof(fsmcolor));
            //memset(shimen, 0, sizeof(shimen));
            //memset(smcolor, 0, sizeof(smcolor));
            memset(fyamen, 0, sizeof(fyamen));
            memset(fymcolor, 0, sizeof(fymcolor));
            //memset(yamen, 0, sizeof(yamen));
            //memset(ymcolor, 0, sizeof(ymcolor));
            memset(bookStore, 0, sizeof(bookStore));
            formations.reserve(32);
            shimen.reserve(32);
            smcolor.reserve(32);
            yamen.reserve(32);
            ymcolor.reserve(32);
            memset(tjEvent1, 0, sizeof(tjEvent1));
            memset(tjColor1, 0, sizeof(tjColor1));
            memset(tjExp1, 0, sizeof(tjExp1));
            lastTjEventScore = 0;
            lastTjTotalScore = 0;
            isHHBlue = false;
        }


		std::string name;           // ????
		UInt32 gold;	            // Ԫ??
		UInt32 coupon;	            // ??ȯ
		UInt32 tael;	            // ??
		UInt32 coin;	            // ͭǮ
        UInt32 prestige;            // ????
		UInt32 status;              // ״̬:0x01 - pk???? 0x02 - ?д轿0x04 - С?????? 0x80 - ??ֹ̽??
		UInt8 country;              // ????
		UInt8 title;                // ͷ??
		UInt32 achievement;         // ս??
        UInt32 attainment;          //
        UInt8 qqvipl;               // QQ VIP?ȼ?
        UInt8 qqvipl1;              // 3366ʱ??QQ VIP?ȼ?
        UInt8 qqvipyear;            // QQ VIP?Ƿ?????
        UInt32 qqawardgot;          // QQ VIP?????Ƿ?????ȡ
        UInt32 qqawardEnd;          // QQ ??????ȡ????ʱ??
        UInt32 ydGemId;             // QQ VIP??ʯ????
		UInt16 location;            // λ??
		UInt8 inCity;               // ???? // 现在始终为true
		UInt32 lastOnline;          // ?ϴ?????ʱ??
		UInt64 newGuild;            // ????????????
		UInt16 packSize;            // ???ұ?????
		UInt8 mounts;               // ????
		UInt8 gmLevel;              //
		UInt8 icCount;              // ?һ????ٴ???
		UInt32 nextIcReset;         //
        UInt8  picCount;            // ???ٴ???
        UInt32 nextPIcReset;        // ???ٴ???????ʱ??
		UInt16 formation;           //
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
        UInt32 fshimen[6];          // ˢ????ʦ??????
		UInt8 fsmcolor[6];          // ˢ????ʦ??????????ɫ
        std::vector<UInt32> shimen; // ʦ??????
        std::vector<UInt8> smcolor; // ʦ????????ɫ
        UInt8 smFinishCount;        // ʦ?????????????ɴ???
        UInt8 smFreeCount;          // ʦ??????????????ˢ?´???
        UInt8 smAcceptCount;        // ʦ?????????ս??ܴ???
        UInt32 fyamen[6];           // ˢ????????????
		UInt8 fymcolor[6];          // ˢ????????????????ɫ
        std::vector<UInt32> yamen;  // ????????
        std::vector<UInt8> ymcolor; // ??????????ɫ
        UInt8 ymFinishCount;        // ???????????????ɴ???
        UInt8 ymFreeCount;          // ????????????????ˢ?´???
        UInt8 ymAcceptCount;        // ???????????ս??ܴ???
        UInt32 clanTaskId;          // ????????
        UInt8 ctFinishCount;        // ???????????????ɸ???
		UInt16 bookStore[6];        //
		UInt32 created;             //
		UInt32 lockExpireTime;      //
		UInt8 wallow;               //
		UInt32 battlecdtm;          //
        UInt8 dungeonCnt;           // ͨ??????ǰ????
        UInt32 dungeonEnd;          // ͨ????????????ʱ??
        UInt8 copyFreeCnt;          // ???????Ѵ???
        UInt8 copyGoldCnt;          // ?????շѴ???
        UInt32 copyUpdate;          // ????????????ʱ??
        UInt8 frontFreeCnt;         // ??ͼ???Ѵ???
        UInt8 frontGoldCnt;         // ??ͼ?շѴ???
        UInt32 frontUpdate;         // ??ͼ????????ʱ??
        std::vector<UInt16> formations; // ??ѧ??????
#ifdef _ARENA_SERVER
        UInt8 entered;
#endif
        short tjEvent1[3];       //天劫事件1的3个据点npc
        UInt8 tjColor1[3];       //颜色
        int   tjExp1[3];         //经验  
        int   lastTjEventScore;      //天劫事件积分
        int   lastTjTotalScore;      //天劫活动积分

        bool isHHBlue;
        std::string nameNoSuffix;     //(合服)不带后缀的用户名
        std::map<UInt8, UInt32> titleAll;      //玩家所有的称号id
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
			Challenging = 0x00000010,		//????״̬, ?=??̴߳???
			BeChallenging = 0x00000020,		//?Ǿ???״̬
			SGPunish	= 0x00000040,		//???ٳͷ?
            AthPayForPage = 0x00000080,     //in athletics range for paging
            AthPayForKillCD=0x00000100,
			AllGlobalFlags	= 0xFFFFFFFF
		};

		enum PlayerFlag
		{
			CountryBattle	= 0x00000001,
			Training		= 0x00000002,
			AutoDungeon		= 0x00000004,
			ClanBattling	= 0x00000008,
			AutoCopy        = 0x00000010,
			Copy            = 0x00000020,
            InHeroIsland    = 0x00000040,
            AutoFrontMap    = 0x00000080,
            InCopyTeam      = 0x00000100,
            ClanRankBattle  = 0x00000200,
            AutoTlz         = 0x00000400,
            AthleticsBuff   = 0x80000000,
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

	public:
		Player(UInt64);
		~Player();

        inline UInt8 getShiMenMax()
        {
            if (isOffical())
                return SHIMEN_TASK_MAXCOUNT + Player::_shiMenActiveCount - 1;
            return SHIMEN_TASK_MAXCOUNT + Player::_shiMenActiveCount;
        }

        inline UInt8 getYaMenMax()
        {
            if (isOffical())
                return YAMEN_TASK_MAXCOUNT + Player::_yaMenActiveCount - 1;
            return YAMEN_TASK_MAXCOUNT + Player::_yaMenActiveCount;
        }

        inline UInt8 getClanTaskMax()
        {
            if (isOffical())
                return CLAN_TASK_MAXCOUNT - 1;
            return CLAN_TASK_MAXCOUNT;
        }

#ifdef _ARENA_SERVER
        UInt64 getOriginId();
        static UInt64 getOriginId(UInt64 id);

        inline int getChannelId() { return _channelId; }
        inline int getServerId() { return _serverId; }
        int getRealServerId();
        inline int getCSId() { return (_serverId << 8) + _channelId; }
        int getRealCSId();

        void setEntered(UInt8 e);
        inline bool isEntered() const { return _playerData.entered != 0xFF; }
        inline UInt8 getEntered() const { return _playerData.entered; }
#endif

	public:
		bool Load();
		inline void recalcVipLevel() { _vipLevel = calcVipLevel(); }
		bool Init();
		void UnInit();

		UInt8 GetCountryThread();

        //玩家每日签到接口
        void ActivitySignIn();
        void SendNextdayTime(UInt32 nextDay);

		void Login();
        void sendCreateMail();
        void sendOpenAct(UInt32);
        void continuousLogin(UInt32 now);
        void continuousLoginRF(UInt32 now);
        void getContinuousReward(UInt8 type, UInt8 idx = 0);
        void getContinuousRewardRF(UInt8 type, UInt8 idx = 0);
        void turnOnRC7Day();
        void turnOnRF7Day();
        void addRC7DayRecharge(UInt32 r);
        void addRF7DayRecharge(UInt32 r);
        void sendRC7DayInfo(UInt32 now = TimeUtil::Now());
        void sendRF7DayInfo(UInt32 now = TimeUtil::Now());
        void setContinuousRFAward(UInt32 no);
        void sendFourCopAct();
		void Reconnect();

		void Logout(bool = false);	//???????߲???
		void selfKick();

        UInt32 GetOnlineTimeToday();
        UInt32 GetOnlineTimeTodaySinceLastLogin(UInt32 now);

    private:
        void LogoutSaveOnlineTimeToday();
    public:
		void sendWallow();
		void makeWallow(Stream& st);

		void checkLastBattled();
        void addLastTjScore();
		void checkHPLoss();
		void checkDeath();

        void OnAddOneFriend();
        void OnFriendLevUp(UInt8 nLev);
		void checkLevUp(UInt8, UInt8);
        bool formationLevUp(UInt16);
        bool addNewFormation(UInt16 newformationId, bool writedb = false);
        void sendFormationList();
        bool checkFormation(UInt16);
        bool checkFormation_ID(UInt16);
        void sendNationalDayOnlineAward();
        void sendHalloweenOnlineAward(UInt32, bool = false);
        void sendLevelPack(UInt8);
        //void resetThanksgiving();
        void offlineExp(UInt32);
        void getOfflineExp();

        void sendShusanLoveTitleCard(int);
        void sendMayDayTitleCard(int);
        void sendJuneHappyTitleCard(int pos);
        void sendPExpCard(int pos);
        void sendRechargeRankAward(int pos);
        void sendConsumeRankAward(int pos);
        void sendKillMonsterRankAward(UInt8 index, Int32 pos);
        UInt32 getEventState(UInt32 type);

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

        void OnSelectCountry();
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

        inline void setQQVipl(UInt8 lvl)
        {
            _playerData.qqvipl = lvl;
            if(lvl > 8 && lvl < 10)
                _playerData.qqvipl = 8;
            else if(lvl > 17 && lvl < 20)
                _playerData.qqvipl = 17;
            else if (lvl > 26 && lvl < 30)
                _playerData.qqvipl = 26;
            else if (lvl > 38 && lvl < 40)
                _playerData.qqvipl = 38;
            else if (lvl > 47 && lvl < 50)
                _playerData.qqvipl = 47;
            else if (lvl > 57 && lvl < 60)
                _playerData.qqvipl = 57;

            if (_playerData.qqvipl >= 50 && _playerData.qqvipl < 60)
            {
                _playerData.qqvipl -= 40;
                _playerData.isHHBlue = true; // XXX: 豪华蓝钻
            }
        }
        inline void setQQVipl1(UInt8 lvl)
        {
            _playerData.qqvipl1 = lvl;
            if (lvl > 7 && lvl < 10)
                _playerData.qqvipl1 = 7;
            else if (lvl >= 20 && lvl < 30)
                 _playerData.qqvipl1 = 16;
            else if (lvl >= 50 && lvl < 60)
            {
                 _playerData.qqvipl1 -= 40;
                 if (_playerData.qqvipl >= 30 && _playerData.qqvipl < 40)
                     _playerData.isHHBlue = true;
            }
        }
        inline UInt8 getQQVipl() { return _playerData.qqvipl; }
        inline UInt8 getQQVipl1() { return _playerData.qqvipl1; }
        inline void setQQVipYear(bool is) { _playerData.qqvipyear = is?1:0; }
        inline bool getQQVipYear() { return _playerData.qqvipyear; }
        inline UInt8 getPF()
        {
            if (_playerData.qqvipl >= 1 && _playerData.qqvipl <= 9)
                return (2<<4)|_playerData.qqvipl;
            if (_playerData.qqvipl >= 10 && _playerData.qqvipl <= 19)
            {
                if (_playerData.isHHBlue)
                    return (5<<4)|(_playerData.qqvipl-10);
                else
                    return (1<<4)|(_playerData.qqvipl-10);
            }
            if (_playerData.qqvipl == 20)
            {
                if (_playerData.isHHBlue)
                    return (5<<4)|(_playerData.qqvipl-10);
                else
                    return (1<<4)|(_playerData.qqvipl1-10);
            }
            if (_playerData.qqvipl >= 20 && _playerData.qqvipl <= 29)
                return (3<<4)|(_playerData.qqvipl-20);
            if (_playerData.qqvipl >= 30 && _playerData.qqvipl <= 39)
                return (4<<4)|(_playerData.qqvipl1>40?_playerData.qqvipl1-40:0);
            if (_playerData.qqvipl >= 40 && _playerData.qqvipl <= 49)
                return (4<<4)|(_playerData.qqvipl-40);
            return 0;
        }
        // XXX: 1-9 黄钻等级
        //      10-19 蓝钻等级
        //      20-29 3366等级,另qqvipl1 为蓝钻等级
        //      30-39 Q+等级,另qqvipl1为会员等级(现归属QQ会员)
        //      40-49 QQ会员等级
        //      50-60 豪华蓝钻(需要转换成蓝钻)
        inline bool isYD() const
        {
            //return (_playerData.qqvipl >= 1 && _playerData.qqvipl <= 9) || (_playerData.qqvipl >= 30 && _playerData.qqvipl <= 39);
            return (_playerData.qqvipl >= 1 && _playerData.qqvipl <= 9);
        }
        inline bool isBD() const
        {
            //if (_playerData.qqvipl >= 10 && _playerData.qqvipl <= 19)
            if (_playerData.qqvipl >= 11 && _playerData.qqvipl <= 19) //qqvipl为10代表蓝钻0级，不是蓝钻用户
                return true;
            if (_playerData.qqvipl >= 21 && _playerData.qqvipl <= 29 && _playerData.qqvipl1 >= 11 && _playerData.qqvipl1 <= 19) //qqvipli1为10代表蓝钻0级，不是蓝钻用户
                    return true;
            return false;
        }
        inline bool isQQVIP() const
        {
            return (_playerData.qqvipl > 40 && _playerData.qqvipl <= 49) || (_playerData.qqvipl1 > 40 && _playerData.qqvipl1 <= 49) || (_playerData.qqvipl >= 30 && _playerData.qqvipl <= 39 && _playerData.qqvipl1 > 40 && _playerData.qqvipl1 <= 49);
        }

        inline bool is3366AndLevel4() const
        {
            return (_playerData.qqvipl >= 24 && _playerData.qqvipl <= 29);
        }

		UInt32 getTotalRecharge()			{ return _playerData.totalRecharge; }
		void addTotalRecharge(UInt32);
		void setTotalRecharge( UInt32 r );
		void genOnlineRewardItems();
		bool takeOnlineReward();
		UInt32 getOnlineReward();
		void writeOnlineRewardToDB();

        inline void setLockExpireTime(UInt32 let)   {_playerData.lockExpireTime=let;}
		inline UInt32   getLockExpireTime() {return _playerData.lockExpireTime;}

        void initHeroMemo();
        void initShuoShuo();

		UInt32 getBuffData(UInt8 idx, UInt32 tm = TimeUtil::Now());
		UInt32 getBuffLeft(UInt8 idx, UInt32 tm = TimeUtil::Now());
		void setBuffData(UInt8, UInt32, bool = true);
		void addBuffData(UInt8, UInt32);
		void testBattlePunish();


        UInt32 GetVar(UInt32 id);
        Int32 GetVarS(Int32 id);
        void LoadVar(UInt32 id, UInt32 val, UInt32 overTime);
        void SetVar(UInt32 id, UInt32 val);
        void AddVar(UInt32 id, UInt32 val);
        void AddVarS(UInt32 id, Int32 val);

        UInt32 GetVarNow(UInt32 id, UInt32 now);
        void SetVarNow(UInt32 id,  UInt32 val, UInt32 now);
        void AddVarNow(UInt32 id , UInt32 val, UInt32 now);

        void SetVarOffset(UInt32 offset);

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
        UInt8 GetColor() const;
        UInt8 getPortraitAndColor() const;
		UInt64 GetExp() const;
		void AddExp(UInt64, UInt8 = 0);
		void AddPExp(UInt32);
		void AddPExpBy(Player*,UInt32);
		void AddItemBy(Player*,UInt16,UInt16,bool);
		void pendExp(UInt32, bool = false);
		void setLevelAndExp(UInt8, UInt64);
		inline UInt32 getPendExp() { return _playerData.lastExp & 0x7FFFFFFF; }
		bool regenHP(UInt32);
        UInt8 allHpP();

        bool isCopyPassed(UInt8 copyid);

    private:
        GData::AttrExtra _hiattr;
    public:
        void addHIAttr(const GData::AttrExtra&);
        void clearHIAttr();
        inline const GData::AttrExtra* getHIAttr() const { return &_hiattr; }

		void pendTael(UInt32);
		void pendCoupon(UInt32);
		void pendCoin(UInt32);
		void pendAchievement(UInt32);
		inline void pendDungeon(UInt16 d) { _lastDungeon = d; }
        inline void delayNotifyAthleticsAward(LastAthAward* aa) { _lastAthAward.push_back(*aa); }

		void sendMsgCode(UInt8, UInt32, UInt32 = 0);

		void sendModification(UInt8, UInt32, bool = true);
		void updateDB(UInt8, UInt32);

		UInt32 getGold(UInt32 c = 0, IncommingInfo* ii = NULL);
		UInt32 useGold(UInt32 c, ConsumeInfo * ci=NULL);
        void deleteGold(UInt32 c);
        UInt32 useGold4LuckDraw(UInt32 c);
        UInt32 getGold4LuckDraw();
		bool holdGold(UInt32 c, UInt8, ConsumeInfo * ci = NULL);

		UInt32 getGoldOrCoupon();
		UInt32 useGoldOrCoupon(UInt32,ConsumeInfo * ci=NULL);//return gold

		UInt32 getCoupon(UInt32 c = 0);
		UInt32 useCoupon(UInt32 c,ConsumeInfo * ci=NULL);

		UInt32 getTael(UInt32 c = 0);
		UInt32 useTael(UInt32 c,ConsumeInfo * ci=NULL);
		void useTael2(UInt32 c, Player *attacker, ConsumeInfo * ci = NULL);
        UInt32 useMoneyArena( UInt32 a,ConsumeInfo * ci = NULL);
        UInt32 getMoneyArenaLua(UInt32 c);
        UInt32 getMoneyArena( UInt32 c, IncommingInfo *ii = NULL);

        UInt32 useClanProffer(UInt32 c, ConsumeInfo *ci = NULL);
        UInt32 getClanProffer(UInt32 c = 0, IncommingInfo *ii = NULL);

		UInt32 getCoin(UInt32 c = 0);
		UInt32 useCoin(UInt32 c, ConsumeInfo * ci=NULL, bool notify = true);
		bool holdCoin(UInt32 c, UInt8);

        bool useDemonLog(UInt32 id, UInt32 num, ConsumeInfo* ci);

		void addTrainFighterFromDB(UInt32, UInt8, UInt32, UInt32, UInt32, UInt32);
		bool delTrainFighter(UInt32, bool = false);
		bool hasTrainFighter(UInt32);
		bool addTrainFighter(UInt32, UInt8, UInt32);
		bool accTrainFighter(UInt32, UInt32);
		bool cancelTrainFighter(UInt32);
		TrainFighterData* getTrainFighterData(UInt32);
        void makeTrainFighterInfo(Stream& st);

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

        void loadTitleAll(UInt8 t, UInt32 timeEnd);
        void fixOldVertionTitle(UInt8 t);
		void setTitle(UInt8 s, UInt32 timeLen = 0);
		UInt8 getTitle();
        UInt8 getTitle_noCheck();
        bool checkTitleTimeEnd(UInt8 title, UInt32& timeLeft);
        bool makeTitleAllInfo(Stream& st);
        bool hasTitle(UInt8);
        void changeTitle(UInt8 t);
        bool notifyTitleAll();
        void writeTitleAll();
        void ArenaExtraAct(UInt8 type, UInt8 opt);

        void FirstRechargeAct(UInt8 step, UInt8 type, UInt8 career);
        void sendFirstRecharge(bool isLogin = false);

		UInt32 getAchievement(UInt32 a = 0);
		UInt32 useAchievement(UInt32 a,ConsumeInfo * ci=NULL);
		void useAchievement2( UInt32 a, Player *attacker, ConsumeInfo * ci = NULL);

        UInt32 getAttainment(UInt32 a = 0);
        UInt32 useAttainment(UInt32 a, ConsumeInfo * ci=NULL);

		UInt32 getPrestige(UInt32 a = 0, bool notify = true);
		UInt32 usePrestige(UInt32 a,ConsumeInfo * ci=NULL);

		void incIcCount();
		void resetIcCount();
		UInt8 getIcCount();

        // ??Ϊ?һ?????
		void incPIcCount();
        UInt8 getPIcCount();
        void checkPIcCount();

		inline UInt16 getPacksize() { return _playerData.packSize; }
        inline UInt8 getMounts() { return _playerData.mounts; }
        bool setMounts(UInt8 mounts);

        void setLineupDirty(bool = true);
        void setFightersDirty(bool bDirty=true);
        bool IsFighterEquipEnchantLev(UInt8 en, UInt8 num);
		inline size_t getFighterCount() { return _fighters.size(); }
        std::map<UInt32, Fighter *>& getFighterMap() {return _fighters;}
		bool isFighterFull() const;
		inline bool isMainFighter(UInt32 id) { return Fighter::isMainFighter(id); }
        void upInitCitta(Fighter* fgt,bool = false);
		void addFighter(Fighter * fgt, bool = true, bool = false);
		Fighter * takeFighter(UInt32, bool = true);
		void notifyAddFighter(Fighter * fgt);
		Fighter * findFighter(UInt32);
		Fighter * getMainFighter();
        Int32 getMainHP() { return getMainFighter()?getMainFighter()->getCurrentHP():0; }
        UInt32 getMainPExp() { return getMainFighter()?getMainFighter()->getPExp():0; }
		bool hasFighter(UInt32);
        bool addFighterFromItem(UInt32, UInt32);
        bool fighterFromItem(UInt32 fgtid);
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
		void foreachLineup(_FN& fn)
		{
			for(int i = 0; i < 5; ++ i)
			{
				if(_playerData.lineup[i].fighter != NULL)
					fn(_playerData.lineup[i].fighter);
			}
		}

		UInt32 addFightCurrentHp(UInt32 id, UInt32 hp, bool battle = false);
		void   addFightCurrentHpAll(UInt32 hp);

		void updateBattleFighters(bool = true);
		void storeFighters();
		bool setFormation(UInt16);
		void makePlayerInfo(Stream&);
		void makeFormationInfo(Stream&);
        void makeFighterSSList(Stream& st);
		void makeFighterList(Stream&);
		void makeFighterInfo(Stream&, Fighter *, bool = true);
		bool makeFighterInfo(Stream&, UInt32);
        void sendRechargeInfo(bool rank = false);
        void sendConsumeInfo(bool rank = false);
        void getMDItem();
        void sendMDSoul(UInt8 type, UInt32 id = 0);
        void sendJuneRechargeMails(UInt32 value);
        UInt32 _mditem;

		void autoRegenAll();
		void regenAll(bool = false);
        void setHPPercent(UInt8 p);

        UInt32 getClientAddress();
		bool setNewGuildTaskStep(UInt32);
        void sendNewGuild();

        bool OperationTaskAction(int type);

        // QQ????
        void sendYellowDiamondInfo();
        UInt8 rcvYellowDiamondAward(UInt8 type, UInt8 d3d6);
        void checkQQAward();
        void RollYDGem();
        void openLevelBox(UInt8 lvl, UInt8 cls);

        void consumeGold(UInt32 c);

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

		//ս??????
		bool challenge(Player *, UInt32 * = NULL, int * = NULL, bool = true, UInt32 = 0, bool = false, UInt32 = Battle::BS_ATHLETICS1, UInt8 = 0x03);
		bool attackNpc(UInt32, UInt32 = 0xFFFFFFFF, bool = false, bool = true);
        bool attackTianjieNpc(UInt32 npcId, UInt32 expMulti = 1, bool isEvent = false, bool isBoss = false);
        bool attackRareAnimal(UInt32 id);
        bool attackCopyNpc(UInt32, UInt8, UInt8, UInt8, bool&, UInt8 = 0, bool = false, std::vector<UInt16>* loot = NULL, bool = true);
        bool attackWorldBoss(UInt32, UInt8, UInt8, UInt8, bool = false);
        void autoFrontMapFailed();
        void autoCopyFailed(UInt8);
        inline bool isAutoCopyFailed() { return m_autoCopyFailed; }
        inline void resetAutoCopyFailed() { m_autoCopyFailed = false; }
        inline void setCopyFailed() { m_autoCopyFailed = true; }
		bool autoBattle(UInt32);
		void pushAutoBattle(UInt32, UInt16, UInt16);
        //void advancedHookExp();
		void pushAutoDungeon(UInt32, UInt32, UInt8);
		void cancelAutoBattle();
		void cancelAutoBattleNotify();
		void instantAutoBattle();
		void cancelAutoDungeon();
		void sendEvents();
		void sendAutoBattleEvent(EventBase *);
		void sendOnlineReward();
		void sendDailyInfo();

        void GetFuben(UInt8& copy, UInt8& copyMax, UInt8& dung, UInt8& dungMax, UInt8& format, UInt8& formatMax );
        void GetDailyTask(UInt8& shimenF, UInt8& shimenMax, UInt8& yamenF, UInt8& yamenMax, UInt8& clanF, UInt8& clanMax);

        void startAutoCopy(UInt8 id, UInt8 mtype);
        void cancelAutoCopy(UInt8 id);
        void instantAutoCopy(UInt8 id);
        void sendAutoCopy();

        void startAutoFrontMap(UInt8 id, UInt8 mtype);
        void cancelAutoFrontMap(UInt8 id);
        void instantAutoFrontMap(UInt8 id);
        void sendAutoFrontMap();

		inline UInt32 getNextExtraReward()
		{ return _playerData.nextExtraReward; }
		void setNextExtraReward(UInt32 ner);
		void exchangeForExtraReward();

		//????ϵͳ
		const std::string& getClanName() const;
		void setClan(Clan * c);
		inline Clan * getClan() { return _clan; }
		inline void setClanBattle(ClanBattle * c)  { _clanBattle = c; }
		inline ClanBattle * getClanBattle() { return _clanBattle; }

        void OnDoAttainment(UInt32 attId,   UInt32  param);
        void OnHeroMemo(UInt8,UInt8,UInt8,UInt8);
        void OnShuoShuo(UInt8);
		//////////////////////////////////////////////////////////////////////////
		//????ϵͳ
		inline bool IsInTeam() const { return false; }	//TODO

        //天劫每日任务
        void OnDoTianjieTask(UInt8 eventId, UInt8 cmd, UInt8 id);
        UInt8 attackTjEvent1(UInt8 id, UInt8 cmd);
        UInt8 attackTjEvent3(UInt8 id);
        void getTjTask1Data(Stream& st, bool isRefresh = false);
        void getTjTask2Data(Stream& st);
        void getTjTask3Data(Stream& st);
        void addExpOrTjScore(int exp, int score, bool isEventScore = true, bool isEndScore = false);

        void clearTjTaskData();

        void processAutoTlz();
        void cancleAutoTlz();
        void completeAutoTlz();
        
        //周年庆回流用户
        UInt8 getRPLoginDay();
        void sendYearRPInfo();
        void getYearRPPackage();
        void getYearRPReward();
        /////
	public:
		UInt16   GetFreePackageSize();
		bool     ExtendPackageSize();

		Package* GetPackage() { return m_Package; }
		TaskMgr* GetTaskMgr() { return m_TaskMgr; }
		MailBox* GetMailBox() { return m_MailBox; }
		AttainMgr* GetAttainMgr() { return m_AttainMgr; }
        ActivityMgr* GetActivityMgr(){return m_ActivityMgr;}
        StrengthenMgr* GetStrengthenMgr(){return m_StrengthenMgr;}
        HeroMemo* GetHeroMemo(){return m_HeroMemo;}
        ShuoShuo* GetShuoShuo(){return m_ShuoShuo;}
        CFriend* GetCFriend(){return m_CFriend;}
        NewRelation* GetNewRelation() { return m_relation; }
		Trade* GetTrade()			{ return m_Trade; }
		Sale* GetSale()				{ return m_Sale; }
		Athletics* GetAthletics()	{ return m_Athletics; }

	// ????ϵͳ
	public:

        void SetClanBattleStatus(UInt8 status) { m_ClanBattleStatus = status;}
        UInt8 GetClanBattleStatus() const { return m_ClanBattleStatus; }

        void SetClanBattleScore(UInt32 score) { m_ClanBattleScore = score; }
        void AddClanBattleScore(UInt32 score) { m_ClanBattleScore += score; }
        UInt32 GetClanBattleScore() const { return m_ClanBattleScore; }

        void SetClanBattleWinTimes(UInt32 times) { m_ClanBattleWinTimes = times; }
        void IncClanBattleWinTimes() { ++m_ClanBattleWinTimes; }
        UInt32 GetClanBattleWinTimes() const { return m_ClanBattleWinTimes; }

        void SetClanBattleSkillFlag(UInt8 skillId) { m_ClanBattleSkillFlag |= (1 << skillId); }
        bool CheckClanBattleSkillFlag(UInt8 skillId) { return (m_ClanBattleSkillFlag & (1 << skillId)) != 0; }
        void ClearClanBattleSkillFlag() { m_ClanBattleSkillFlag = 0; }
        UInt32 GetClanBattleSkillFlag() const { return m_ClanBattleSkillFlag; }

		void addFriendFromDB(Player *);
		void addBlockFromDB(Player *);
		void addFoeFromDB(Player *);
		void addCFriendFromDB(Player *);
		bool addFriend(Player *);
        void AddFriendAttainment( Player* other);
		void delFriend(Player *);
		bool addBlock(Player *);
		bool delBlock(Player *);
		inline bool hasBlock(Player *pl) { Mutex::ScopedLock lk(_mutex); return _hasBlock(pl); }
		bool addFoe(Player *);
		bool delFoe(Player *);
        bool addCFriend(Player*);
        void delCFriend(Player*);
		inline bool isFriendFull() { return _friends[0].size() >= MAX_FRIENDS; }
        inline bool isCFriendFull() { return _friends[3].size() >= MAX_CFRIENDS; }
        inline UInt32 getFrendsNum() const { return _friends[0].size(); }
        inline UInt32 getCFrendsNum() const { return _friends[3].size(); }
		bool testCanAddFriend(Player *);
		bool testCanAddCFriend(Player *);
        void tellCFriendLvlUp(UInt8);
        void OnCFriendLvlUp(Player*, UInt8);

		void sendFriendList(UInt8, UInt8, UInt8);

		void PutFighters(Battle::BattleSimulator&, int side, bool fullhp = false);

		inline void setNextTavernUpdate(UInt32 n) { _nextTavernUpdate = n; }
        void resetShiMen();
        void resetYaMen();
		void writeTavernIds();
		void writeShiMen();
		void writeYaMen();
        void writeShiYaMen();
        bool addAwardByTaskColor(UInt32, bool = false);
        void delColorTask(UInt32);

        UInt32 getClanTask();
        bool finishClanTask(UInt32);
        void delClanTask();
        void buildClanTask(bool fReset = false);
        void resetClanTask();
        UInt32 getClanTaskId();
        bool isClanTask(UInt32);
        bool isClanTaskFull();
        void writeClanTask();
        void AddClanBuilding(UInt32);
        void AddClanContrib(UInt32);

        ////////////////////////////////////////////
        // 帮派副本

        // GM命令设置帮派副本等级
        void setClanCopyLevel(UInt16 level);
        void setClanCopyTime(UInt32 time);


        // 帮派副本
        ////////////////////////////////////////////

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

        UInt8 GetFullPotFighterNum();
        UInt8 GetFullCapFighterNum();
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
        void patchDeleteDotS(std::string& name);
		inline void patchMergedName() { patchMergedName(_id, _playerData.name); }
		static void patchMergedName(UInt64 id, std::string& name);
#if 0
        const char *patchShowName(const char* name, const UInt64 playerId = 0);
#endif
        const char* getNameNoSuffix(std::string name);
		void autoCB(bool = true);

	public:
		static void setTavernInterval(UInt32);
		static void setTavernRate(UInt32);

        static void setRecruitCost(UInt32 recruit_cost);
        static void setTavernBlueCount(UInt32 tavernBlueCount);
        static void setTavernPurpleCount(UInt32 tavernPurpleCount);
        static void setTavernOrangeCount(UInt32 tavernPurpleCount);
        static void setShiMenActiveCount(UInt8);
        static void setYaMenActiveCount(UInt8);

		inline Mutex& getMutex() { return _mutex; }

        void setVipL(UInt8 lvl); // XXX:

	private:
		UInt32 calcVipLevel();
		UInt32 calcYDVipLevel(UInt32);
        UInt8 calcRechargeLevel(UInt32, UInt8&);
		Player * _findFriend( UInt8 type, std::string& name );
		bool _hasFriend( UInt8 type, Player * pl ) const;
		inline Player * _findFriend(std::string& name) { return _findFriend(0, name); }
		inline bool _hasFriend(Player *pl) const { return _hasFriend(0, pl); }
		inline Player * _findCFriend(std::string& name) { return _findFriend(3, name); }
		inline bool _hasCFriend(Player *pl) const { return _hasFriend(3, pl); }
		inline Player * _findBlock(std::string& name) { return _findFriend(1, name); }
		inline bool _hasBlock(Player *pl) const { return _hasFriend(1, pl); }
		inline Player * _findFoe(std::string& name) { return _findFriend(2, name); }
		inline bool _hasFoe(Player *pl) const { return _hasFriend(2, pl); }

		void appendFriendAct(Stream&, FriendActStruct*);
		void pushFriendAct(FriendActStruct * fas);
		void addFriendInternal(Player *, bool, bool = true);
		void delFriendInternal(Player *, bool = true);
		void addCFriendInternal(Player *, bool, bool = true);
		void delCFriendInternal(Player *, bool = true);

		void sendVIPMails(UInt8, UInt8);
		void sendYDVIPMails(UInt8, UInt8);
        void sendRechargeMails(UInt8, UInt8, UInt8);
		void checkIcExpire(bool = true);
		void sendBlockBossMail(UInt8, UInt8);

    private:
        bool _isJumpingMap;

        QixiInfo m_qixi;
        bool _qixiBinding;
    public:
        inline bool isJumpingMap() { return _isJumpingMap; }
        inline void setJumpingMap(bool v) { _isJumpingMap = v; }

        void loadQixiInfoFromDB(Player* pl, UInt8 bind, UInt8 pos, UInt8 event, UInt32 score)
        {
            m_qixi.lover = pl;
            m_qixi.bind = bind;
            m_qixi.pos = pos;
            m_qixi.event = event;
            m_qixi.score = score;
        }
        void sendQixiInfo();
        void divorceQixi();
        void postQixiEyes(Player* pl);
        void roamingQueqiao(UInt8 pos);
        void qixiStepAdvance(UInt8 pos, UInt8 event, UInt8 score);
        void resetQixi();
        void killMonsterStepAdvance(UInt32 pos, UInt8 curType, UInt8 curCount, UInt8 tips);

        void beDivorceQixi(Player* pl);
        UInt8 beQixiEyes(Player* pl);
        void onQixiEyesResp(UInt8 bind);
        void postRoamResult(UInt8 pos, UInt8 event, UInt8 score);
        void postKillMonsterRoamResult(UInt32 pos, UInt8 curType, UInt8 curCount, UInt8 tips);

        inline bool queQiaoCheck() { return m_qixi.bind; }
        inline UInt8 getQueqiaoPos() { return m_qixi.pos; }
        inline Player* getLover() { return m_qixi.lover; }
        inline UInt32 getScore() { return m_qixi.score; }
        std::set<Player *>& getInviters() {return _friends[3];};

        void setForbidSale(bool b) {_isForbidSale = b;}
        bool getForbidSale() {return _isForbidSale;}
	private:
		Mutex _mutex;

#ifdef _ARENA_SERVER
        int _channelId;
        int _serverId;
#endif

		std::map<UInt32, Fighter *> _fighters;
		std::map<UInt32, TrainFighterData *> _trainFighters;
		std::set<UInt32> _greatFighterFull;

#ifdef _FB
		std::set<UInt32> _trainFightersAct;
#endif

		Package* m_Package;

		std::set<Player *> _friends[4];
		std::vector<FriendActStruct *> _friendActs;

		TaskMgr* m_TaskMgr;
		Trade* m_Trade;
		Sale* m_Sale;
		Athletics* m_Athletics;

		AttainMgr* m_AttainMgr;
        ActivityMgr*  m_ActivityMgr;
        StrengthenMgr*  m_StrengthenMgr;
        HeroMemo* m_HeroMemo;
        ShuoShuo* m_ShuoShuo;
        CFriend* m_CFriend;
        NewRelation* m_relation;
		MailBox* m_MailBox;

		bool _isOnline;
        bool _isHoding;
        UInt32 _holdGold;

		PlayerData _playerData;
		UInt32 _buffData[PLAYER_BUFF_COUNT];

        VarSystem* m_pVars;

		AtomicVal<UInt8> _threadId;
		AtomicVal<int> _session;
		bool _availInit;
        AtomicVal<bool> _isForbidSale;

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

        UInt8 m_ClanBattleStatus;     //帮会战状态
        UInt32 m_ClanBattleScore;     //帮会战个人积分
        UInt32 m_ClanBattleWinTimes;  //帮会战连胜次数
        UInt32 m_ClanBattleSkillFlag; //帮派战已使用技能位

#ifdef _ARENA_SERVER
        inline const std::string& getDisplayName() { if(_displayName.empty()) rebuildBattleName(); return _displayName; }
    private:
        std::string _displayName;
#endif

        UInt64 _invitedBy;
    public:
        void setInvitedBy(UInt64 id, bool writedb = true);
        inline UInt64 getInvitedBy() { return _invitedBy; }
        void setCFriends();
        void useToken(UInt8 type);
        void sendTokenInfo();
        void sendDiscountLimit();

        void useMDSoul();
        void svrSt(UInt8);

    public:
		// Last battled monster
		GData::NpcGroup * _lastNg;
		std::vector<GData::LootResult> _lastLoot;
        std::vector<LastAthAward> _lastAthAward;
        std::vector<GData::LootResult> _equipAward;
		std::vector<GData::LootResult> _RegisterAward;
		std::vector<GData::LootResult> _BirthdayAward;
		std::vector<GData::LootResult> _lastQueqiaoAward;
        std::vector<GData::LootResult> _lastKillMonsterAward;
        std::vector<GData::LootResult> _lastNew7DayTargetAward;
        std::vector<GData::LootResult> _lastExJobAward;

    private:
		UInt16 _lastDungeon;
        static UInt32 _recruit_cost;
        static UInt32 _tavernBlueCount;
        static UInt32 _tavernPurpleCount;
        static UInt32 _tavernOrangeCount;
		static UInt32 _tavernInterval, _tavernRate;
		static UInt32 _bookStoreInterval, _bookStoreRate;
		UInt32 _exchangeTicketCount;//use for exchange plane ticket (new year activity)

        UInt32 _praplace;
        bool m_autoCopyFailed;

        // ͨ????????֮??
        UInt8 _justice_roar;
        float _spirit_factor;
        bool _diamond_privilege;
        bool _qqvip_privilege;
        UInt32 _athlRivalBuf;
        bool _new_rank;
    public:
        static UInt8 _yaMenActiveCount;
        static UInt8 _shiMenActiveCount;
	public:
		void setTicketCount(UInt32 cnt, bool writedb = true);
		inline UInt32 getTicketCount(){return _exchangeTicketCount;}

        inline bool isPracticing() { return _praplace; }
        inline void setPracticingPlaceSlot(UInt32 place) { _praplace = place; }
        inline UInt32 getPracticePlaceSlot() { return _praplace; }
        inline UInt32 getPracticePlace() { return _praplace>>16&0xffff; }
        inline UInt32 getPracticeSlot() { return _praplace&0xffff; }
        float getPracticeBufFactor();
        float getPracticeIncByDiamond();
        float getPracticeIncByQQVip();
        bool accPractice();
        void AddPracticeExp(const PracticeFighterExp* pfexp);

        inline void setJusticeRoar(UInt8 v) { _justice_roar = v; }
        inline UInt8 getJusticeRoar() { return _justice_roar; }

        inline void setNewRank(bool v) { if(cfg.merged && _new_rank != v) _new_rank = v; }
        inline bool isNewRank() { return cfg.merged && _new_rank; }
        inline bool isSameServer(Player* player) { return player && (getId() >> 48) == (player->getId() >> 48); }
        inline UInt16 getServerNo() { return static_cast<UInt16>(getId() >> 48); }

        inline void setSpiritFactor(float v) { _spirit_factor = v; }
        inline float getSpiritFactor() { return _spirit_factor; }

        inline void setDiamondPrivilege(UInt8 v) { _diamond_privilege = v; }
        inline UInt8 getDiamondPrivilege() { return _diamond_privilege; }

        inline void setQQVipPrivilege(UInt8 v) { _qqvip_privilege = v; }
        inline UInt8 getQQVipPrivilege() { return _qqvip_privilege; }

        inline void setAthlRivalBuff(UInt32 v) { _athlRivalBuf = v; }
        inline UInt32 getAthlRivalBuff() { return _athlRivalBuf; }

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
		UInt8 deactiveSecondPWD(std::string answer, bool = false);
		UInt8 changeSecondPWD(std::string oldPWD, std::string newPWD);
		std::string& getQuestionForPWD(){ return _pwdInfo.questionForPWD; }
		void makeSenconPWDInfo(Stream& st);
		bool hasChecked();
        inline const SecondPWDInfo& getPWDInfo() const { return _pwdInfo; }

    private:
        UInt32 _worldBossHp;
    public:
        inline void pendWorldBossHp(UInt32 hp) { _worldBossHp += hp; }
        inline void resetWorldBossHp() { _worldBossHp = 0; }
        inline UInt32 getWorldBossHp() const { return _worldBossHp; }

    public:

    public:
        void payPractice(UInt8 place, UInt16 slot, UInt8 type, UInt8 priceType, UInt8 time, UInt8 prot);
        void addPracticeFighter(UInt32* fighters, size_t size);

    private:
        UInt16 m_autoCopyComplete;
    public:
        inline void addCopyCompleteGold(UInt16 gold) { m_autoCopyComplete += gold; }
        inline UInt16 getCopyCompleteGold() { return m_autoCopyComplete; }
        inline void resetCopyCompleteGold() { m_autoCopyComplete = 0; }

    private:
        UInt8 hispot;
        UInt8 hitype;
    public:
        inline void setHISpot(UInt8 spot) { hispot = spot; }
        inline UInt8 getHISpot() { return hispot;}
        inline void setHIType(UInt8 type) { hitype = type; }
        inline UInt8 getHIType() { return hitype; }

    private:
        std::vector<UInt32> _enchantEqus;
    public:
        bool enchanted8(UInt32);
        void sendEnchanted8Box();
        void enchantGt11();

    public:
        void udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count = 1);
        void udpLog(UInt8 platform, const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count = 1);
        void udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op);
        void luaUdpLog(const char* str1, const char* str2, const char* type);
        void discountLog(UInt8 discountType);
        void tradeUdpLog(UInt32 id, UInt32 val = 0, UInt32 num = 1, bool priceLog = false);
        void skillStrengthenLog(UInt8 type, UInt32 val);
        void townDeamonUdpLog(UInt16 level);
        void dungeonUdpLog(UInt8 levelReq, UInt8 type);
        void frontMapUdpLog(UInt8 id, UInt8 type);
        void copyUdpLog(UInt8 levelReq, UInt8 type);
        void athleticsUdpLog(UInt32 id, UInt8 type = 0);
        void activityUdpLog(UInt32 id, UInt8 type = 0);
        void practiceUdpLog();
        void arenaUdpLog(UInt32 id, UInt8 type = 0);
        void luckyDrawUdpLog(UInt32 id, UInt8 type, UInt32 num = 1);
        void qixiUdpLog(UInt32 id);
        void clanUdpLog(UInt32 id);
        void countryBattleUdpLog(UInt32 id, UInt8 country);
        void secondSoulUdpLog(UInt32 id, UInt32 val = 0, UInt32 num = 1);
        void wBossUdpLog(UInt32 id);
        void clanCopyUdpLog(UInt32 id, UInt32 val = 0, UInt32 num = 1);
        void tripodUdpLog(UInt32 id, UInt32 val = 0, UInt32 num = 1);
        void storeUdpLog(UInt32 id, UInt32 type, UInt32 itemId, UInt32 num = 1);
        void newRC7DayUdpLog(UInt32 id, UInt32 type = 0, UInt32 num  = 1);
        void transformUdpLog(UInt32 id, UInt32 type, UInt32 money1, UInt32 money2, UInt32 money3, UInt32 money4, UInt8 val1);
        void guideUdp(UInt8 type, std::string& p1, std::string& p2);
        void moneyLog(int type, int gold, int coupon = 0, int tael = 0, int achievement = 0, int prestige = 0);
        void actUdp(UInt8 type, std::string& p1, std::string& p2);
    private:
#ifndef _WIN32
        CUserLogger* m_ulog;
#endif
    public:
        void sendMailPack(UInt16 title, UInt16 content, lua_tinker::table items);
        void sendMailItem(UInt16 title, UInt16 content, MailPackage::MailItem* mitem, UInt16 size, bool bind = true);
        void setVipAwardFlag(UInt8 type, UInt32 value);

        //传功
        UInt8 fightTransform(UInt16 fFighterId, UInt16 tFighterId, UInt8 type);
        UInt8 canTransform(Fighter * fFgt, Fighter * tFgt, UInt8 type);
        UInt8 transformUseMoney(Fighter * fFgt, Fighter * tFgt, UInt8 type);
        UInt8 transformExp(Fighter * fFgt, Fighter * tFgt);
        UInt8 transformPotential(Fighter * fFgt, Fighter * tFgt);
        UInt8 transformCapacity(Fighter * fFgt, Fighter * tFgt);
        UInt8 transformSoul(Fighter * fFgt, Fighter * tFgt);
        void transformElixir(Fighter * fFgt, Fighter * tFgt);
            
    private:
        char m_domain[256];
        char m_openid[256];
        char m_openkey[256];
        char m_clientIp[256];

        std::string m_source;
        std::string m_via;
        std::string m_invited;
        bool m_isOffical;
    public:
        inline void setDomain(const std::string& domain)
        {
            strncpy (m_domain, domain.c_str(), 256);
            m_isOffical = false;
            if (atoi(domain.c_str()) == 12)
                m_isOffical = true;
        }
        inline void setClientIp(const std::string& clientIp) 
        { 
            if (inet_addr(clientIp.c_str()) == INADDR_NONE)
            {
                strncpy(m_clientIp, "0.0.0.0", 16);
            }
            else
            {
                strncpy(m_clientIp, clientIp.c_str(), 256);
            }
        }
        void setOpenId(const std::string& openid, bool load = false);
        inline void setOpenKey(const std::string& openkey) { strncpy(m_openkey, openkey.c_str(), 256); }
        inline void setSource(const std::string& source) { m_source = source; }
        inline void setVia(const std::string& via) { m_via = via; }
        inline void setInvited(const std::string& inv) { m_invited = inv; }
        inline const char* getDomain() const { return m_domain; }
        inline const char* getOpenId() const { return m_openid; }
        inline const char* getOpenKey() const { return m_openkey; }
        inline const std::string& getSource() const { return m_source; }
        inline const std::string& getVia() const { return m_via; }
        inline const std::string& getInvited() const { return m_invited; }
        inline bool isOffical() const { return m_isOffical; }
        inline const char* getClientIp() const { return m_clientIp; }

        inline UInt8 getPlatform() const { return atoi(m_domain); }

    public:
        inline void setSysDailog(bool v) { m_sysDailog = v; }
        inline bool getSysDailog() { return m_sysDailog; }
    private:
        bool m_sysDailog;

    public:
        void sendTripodInfo();
        void addItem(UInt32 itemid, UInt16 num, UInt8 bind);
        void makeFire(UInt32 id1, UInt32 id2);

        void getAward();
        void genAward(Stream& st);
        bool genAward();

        void getAward(UInt8 type, UInt8 opt);
        void getSSDTAward(UInt8 opt);
        void sendSSDTInfo();
        void getHappyAward(UInt8 opt);
        void sendHappyInfo(UInt16 itemId = 0);
        void getYearActAward(UInt8 type);
        void getQgameGiftAward();
        void sendYearActInfo();
        void getTargetAward(UInt8 opt);
        void getTargetAwardRF(UInt8 opt);
        void getNewRegisterAward(UInt8 opt);
        void getAwardFromAD();
        void getAwardFromRF();
        void getAwardGiftCard();

        inline TripodData& getTripodData() { return m_td; }
        TripodData& newTripodData();
        TripodData& runTripodData(TripodData& data, bool = false);

    public:
        void sendSingleEnchant(UInt8 enchant);
        void sendOldRC7DayAward();

        TeamData* getTeamData();
        void setTeamData(TeamData* td);
        CopyTeamPage& getCopyTeamPage();
        void clearCopyTeamPage();
        TeamCopyPlayerInfo* getTeamCopyPlayerInfo();
        HoneyFall* getHoneyFall();

        // 帮派技能
        void addClanSkillFromDB(UInt8 skillId, UInt8 level);
        void addClanSkill(UInt8 skillId);
        UInt8 getClanSkillLevel(UInt8 skillId);
        UInt8 clanSkillLevelUp(UInt8 skillId);
        void makeClanSkillInfo(Stream&);
        void makeClanSkillInfo(Stream&, UInt8 skillId);
        void listClanSkills();
        void showClanSkill(UInt8 skillId);
        float getClanSkillHPEffect();
        float getClanSkillAtkEffect();
        float getClanSkillDefendEffect();
        float getClanSkillMagAtkEffect();
        float getClanSkillMagDefentEffect();
        float getClanSkillActionEffect();
        float getClanSkillHitrLvlEffect();
        float getClanSkillMaxSoulEffect();

        void buildClanTechSkill();
        UInt8 getClanSkillFlag() { return m_csFlag; }
        void setClanSkillFlag(UInt8 csFlag) { m_csFlag = csFlag; }
        DeamonPlayerData* getDeamonPlayerData() { return m_dpData; }
        void sendDeamonAwardsInfo();
        void getDeamonAwards();
        void lastLootPush(UInt16 itemId, UInt16 num);
        void RegisterAward(UInt16 itemId, UInt16 num);
        void sendNewRegisterAward(UInt8 idx);
        void BirthdayAward(UInt16 itemId, UInt16 num);
        void getAwardBirthday(UInt8 opt);
        void CheckCanAwardBirthday();
        void getAwardLogin(UInt8 opt);
        void getAwardBlueDiamond(UInt8 opt);
        void getThanksGivingDay(UInt8 opt);
        void IDIPAddItem(UInt16 itemId, UInt16 num, bool bind = true);
        int IDIPBuy(UInt32 itemId, UInt32 num, UInt32 price, std::string& err, bool bind = true);
        void lastExJobAwardPush(UInt16 itemId, UInt16 num);
        void checkLastExJobAward();
        void lastQueqiaoAwardPush(UInt16 itemId, UInt16 num);
        void checkLastQueqiaoAward();
        void lastKillMonsterAwardPush(UInt16 itemId, UInt16 num);
        void lastNew7DayTargetAwardPush(UInt16 itemId, UInt16 num);
        void checkLastKillMonsterAward();
        void checkLastNew7DayTargetAward();
        void sendNewRC7DayInfo(UInt8 type = 0);
        void sendNewRC7DayLogin();
        void sendNewRC7DayRecharge();
        void sendNewRC7DayTarget(UInt8 idx = 0);
        void getNewRC7DayLoginAward(UInt8 val, UInt8 off);
        void getNewRC7DayRechargeAward(UInt8 val);
        void getNewRC7DayTargetAward(UInt8 val);
        void get11DailyAward(UInt8 opt);
        void send11DailyInfo();
        void getSSToolbarAward();
        void sendSSToolbarInfo();

        // 帮派神像
        float getClanStatueHPEffect();
        float getClanStatueAtkEffect();
        float getClanStatueDefendEffect();
        float getClanStatueMagAtkEffect();
        float getClanStatueMagDefentEffect();
        float getClanStatueActionEffect();
        float getClanStatueHitrLvlEffect();
        void  AddStatueExp(UInt32 exp);
        void  SubStatueExp(UInt32 exp);

        // 所有将互斥法宝
        bool checkTrumpMutually(UInt32 trumpid);

    private:
        bool m_hasTripod;
        TripodData m_td;
        TeamData* m_teamData;
        CopyTeamPage m_ctp;
        TeamCopyPlayerInfo* m_tcpInfo;
        HoneyFall* m_hf;
        DeamonPlayerData* m_dpData;
        std::map<UInt8, ClanSkill> m_clanSkill;
        UInt8 m_csFlag;

    public:
        inline void setAtoHICfg(const std::string& cfg) { m_hicfg = cfg; }
        inline const std::string& getAtoHICfg() const { return m_hicfg; }
    public:
        static  UInt8 getMaxIcCount(UInt8 vipLevel);
    private:
        std::string m_hicfg;

    public:
        void onBlueactiveday();
        void sendSecondInfo();
        void recvYBBuf(UInt8 type);
        void sendYBBufInfo(UInt32 ybbuf, UInt32 qqvipbuf, UInt8 joy = 0);
        void adjustAthlBuffData(UInt32 type);
        void sendAthlBufInfo();

        bool hasRealItemAward(UInt32 id);
        void getRealItemAward(UInt32 id);

    public:
        struct RNR
        {
            RNR() : date(0), recharge(0) {}

            UInt32 date;
            UInt32 recharge;
        };

        void addRechargeNextRet(UInt32);
        void updateRNR2DB();
        void loadRNRFromDB(const std::string& str);
        void sendRNR(UInt32 now);
        void sendRechargeNextRetInfo(UInt32 now);
        bool inArenaCommitCD();
        void appendLineup2( Stream& st);
    private:
        std::vector<RNR> rechargs;
        UInt32 m_arenaCommitCD;

    public:
        void getSoSoMapAward();
        void sendSoSoMapInfo();
        void getKillMonsterAward();

        UInt32 getBattlePoint();
        void verifyFighter();
#ifdef _FB
    public:
        void sendLevelAward();
#endif

#ifdef _FB
    public:
        void equipForge(UInt32 fighterId, UInt32 itemId, UInt32 num);
    private:
        std::map<UInt32, UInt32> _forges;
#endif

    public:
        JobHunter * getJobHunter();
        void setJobHunter(std::string& fighterList, std::string& mapInfo, UInt8 progress, UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount);
        void sendAutoJobHunter();
    private:
        JobHunter * _jobHunter;

    public:
        bool hasFighterWithClass(UInt8 cls);

    public:
        void copyFrontWinAward(UInt8 index);
        void loadCopyFrontWinFromDB(UInt8 posOrig, UInt8 posPut, UInt32 itemId, UInt16 ratio);
        void getCopyFrontCurrentAward(UInt8 index);
        void getCopyFrontAwardByIndex(UInt8 copy_or_front, UInt8 index, UInt8 indexPut);
        void resetCopyFrontWinAward(bool fresh = false);
        void freshCopyFrontAwardByIndex(UInt8 copy_or_front, UInt8 index);
        void closeCopyFrontAwardByIndex(UInt8 copy_or_front, UInt8 index);
        void sendCopyFrontAllAward();
        UInt8 getCopyId();
        UInt8 getFrontmapId();
    private:
        UInt8 cf_posPut[5];//范围1-5
        UInt32 cf_itemId[5];
        UInt16 cf_ratio[5];

    public:
        void getGoodVoiceAward(UInt8 type);
        void sendGoodVoiceInfo();
        void get3366GiftAward(UInt8 type);
        void send3366GiftInfo();
        void sendQQGameGift1218();
	};

#define PLAYER_DATA(p, n) p->getPlayerData().n

	typedef GGlobalObjectManagerT<Player, UInt64> GlobalPlayers;
	extern GlobalPlayers globalPlayers;
	extern GlobalPlayers newPlayers;
	typedef GGlobalObjectManagerIStringT<Player> GlobalNamedPlayers;
	extern GlobalNamedPlayers globalNamedPlayers;
    typedef std::vector<UInt64> LevelPlayers;
    typedef std::map<UInt8, LevelPlayers*> GlobalLevelsPlayers;
    typedef GlobalLevelsPlayers::iterator GlobalLevelsPlayersIterator;
    extern GlobalLevelsPlayers globalLevelsPlayers;

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


    typedef Visitor<Player> PlayerVisitor;

}

#endif // _PLAYER_H_
