#ifndef _GAMEACTIONLUA_INC_
#define _GAMEACTIONLUA_INC_

#include "Script.h"

typedef lua_tinker::table Table;

#include "GData/GDataManager.h"
#include "GObject/TaskData.h"
#include "GObject/ActivityMgr.h"
namespace GObject
{
	class Player;
	class Fighter;
}
using namespace GObject;

namespace Script
{
	extern 	void SysBroadcast(UInt8 type, const char * msg);

    enum AttainType
    {
        AddAcupoint = 10051,
        AddSoulMax = 10087,
        AddPExp = 10089,
        FIGHTER_POT_FULL = 10109,
        FIGHTER_CAP_FULL = 10110,
        AddMainFighterCapacity = 10111,
        MainFighterCapFull = 10112,
        TEN_FIGHTER_PC_FULL = 10115,
        MainFighterColChange = 10117,
        MainFighterFullPot = 10119,
        AddPEquip = 10151,
        AddYEquip = 10155,
        ONE_FIGHTER_ENCHANT_8 = 10167,
        FIVE_FIGHTER_ENCHANT_9 = 10168,
        FAIL_ENCH = 10169,
        SPLIT_EQUIP = 10176,
        SPLIT_EQUIP_COLOR = 10178,
        
        ADD_TRUMP  = 10200,
        ADD_NTRUMP = 10201,
        SPLIT_THRUMP = 10208,
        SPLIT_THRUMP_COLOR = 10210,

        LEARNED_FORMATION = 10251,
        LEARNED_SFORMATION =10265,
        ON_ADD_ITEM = 10253,
        SUBMIT_TASKS = 10301,
        SUBMIT_SPECIAL_TASK =10305,
        SUBMIT_YAMEN_TASKS = 10313,
        SUBMIT_SHIMEN_TASKS = 10315,
        SUBMIT_CLAN_TASKS = 10317,

        ADD_FRIEND = 10451,
        ONE_FRIEND_LEV_UP = 10453,
        FIVE_FRIEND_LEV_50 = 10454,
        FIVE_FRIEND_LEV_80 = 10456,
        JOIN_CLAN = 10501,
        CLAN_ADD_MEMBER = 10502,
        CLAN_LEVUP = 10504,
        SELECT_COUNTRY = 10551,
        COUNTRY_BATTLE_WIN = 10552,
        COUNTRY_BATTLE_KILL_STREAK = 10555,
        ATHLETICS_WIN = 10558,
    
        BATTLE_MISS = 10601,
        BATTLE_CS = 10602,
        BATTLE_PR = 10603,
        BATTLE_FJ = 10604,
        BATTLE_SKILLDMG = 10609,
        BATTLE_PLDMG= 10612,
        BATTLE_FIRST_PEERLESS_ATTACK = 10615,
        BATTLE_MAX_AURA = 10616,
        BATTLE_CSFactor = 10617,

        SHIMEN_5_TODAY = 11000,
        YAMEN_5_TODAY = 11001,
    };
	class GameActionLua:
		public Script
	{
	public:
		GameActionLua(UInt8, const char *);
		~GameActionLua();

	public:
		void init();
		void postInit();

	public:
		const char* GetItemName(UInt32 itemId);
		const char* GetGreatFighterName(UInt32 fgtId);

	public:
		Table  GetTaskData(Player * player, UInt32 id);
		UInt32 GetRandLoopTask(Player * player, UInt32 dayTaskId);
		UInt8  GetRandLoopTaskQuality();
		UInt16 GetLoopTaskMaxCount(UInt32 dayTaskId);
		UInt16 GetLoopTaskMaxQualityCount(UInt32 dayTaskId);
		Table  GetLoopTaskTasks(Player * player, UInt32 dayTaskId);
		UInt32 GetLoopTaskIdByNpc(Player * player, UInt32 npcId);
		UInt32 GetLoopTaskFlushGold(UInt32 dayTaskId);
		UInt8  GetRandLoopTaskManualQuality(UInt8 color, bool useGold, UInt32 count);
		Table  FlushLoopTask(UInt8 preColor, UInt8 color, UInt32 count, UInt32 preCount);
		UInt32 GetAutoCompletedTask(Player * player, UInt32 dayTaskId);
		UInt32 GetAutoCompletedConsume();
		bool   DayTaskAward(Player * player, UInt32 dayTaskId, UInt16 count, UInt8 color);
		bool   IsFlushQualityDayTask(UInt32 dayTaskId);

		bool IsAutoCompletedTask(UInt32 taskId);
		Table GetDayTaskCompletedCount(Player * player, UInt32 dayTaskId);
		Table GetDayTaskFlushColor(Player * player, UInt32 dayTaskId);

		Table RunDayCopyTask(Player *, UInt32);
		Table RunDayCopyTaskStep(Player *, UInt32, UInt32);
        Table RunActiveTask(Player * player, UInt32 npcId);
        Table RunActiveTaskStep(Player *, UInt32, UInt32);

		Table RunNpcDefaultAction(Player * player, UInt32 npcId);
		void  RunItemTaskAction(Player* player, UInt32 taskId, UInt32 dummyNpcId);

		UInt32 GetSharpDay(UInt32);

		bool RunOperationTaskAction0(Player *, UInt8);
		bool RunOperationTaskAction1(Player *, UInt8, UInt32);
		bool RunOperationTaskAction2(Player *, UInt8, UInt32, UInt32);
		bool RunOperationTaskAction3(Player *, UInt8, UInt32, UInt32, UInt32);

	public:
		inline Table GetNpcRelationTask(UInt32 npcId);
		inline const char* GetPlayerName(Player* player);
		inline const char* GetPlayerStateName(Player* player);

	public:
		void Print() { std::cout << "Lua Test" << std::endl; }
		void DoLuaTest(Player * player, const std::string& buffer);
 
	public:
		template <typename R> 
		inline R RunMO(Player* player, UInt32 npcId);

		Table RunCollectTask(Player* player, UInt32 npcId);
		bool RunCollectTaskItem(Player* player, UInt32 npcId);
		//?????ڽű??а?̽????????Ϣ????client
		bool RunExploreTask(Player* player, UInt32 npcId);	//true:??̽???????? false:û??̽??????

		bool  RunAutoBattleAction(Player* player, UInt32 npcId, UInt32 turns);
		
		bool  CheckTaskAcceptCondition(Player* player, UInt32 taskId);	//????????????????
		Table RunTask(Player* player, UInt32 taskId, UInt32 npcId);
		Table RunTaskStep(Player* player, UInt32 taskId, UInt8 step);
		Table RunDayTask(Player* player, UInt32 npcId);
		Table RunDayTaskAccept(Player* player, UInt32 taskId, UInt32 npcId);

        Table GetClanBattleBuffs();
        Table GetClanBattleDailyRewards();
        Table GetClanBattleWeekSortRewards();
        Table GetClanBattleWeekScoreRewards();

		bool  MonsterKilled(Player* player, UInt32 monsterId, UInt16 monsterNum = 1);

		bool AcceptTask(Player* player, UInt32 taskId);
		bool SubmitTask(Player* player, UInt32 taskId, UInt32 itemId, UInt16 itemNum);
		bool AbandonTask(Player* player, UInt32 taskId);

		bool SetPlayerData(Player* player, UInt8 field, UInt32 value);
		UInt32 GetPlayerData(Player* player, UInt8 field);

		bool RunConveyAction(Player* player, UInt32 taskId);
		UInt32 GetConveyMonster(Player* player, UInt32 path, UInt16 spot);
		bool UpdateConveyStep(Player* player, UInt32 taskId, UInt16 spot, bool succ);

		bool IsItemTaskItem(UInt32 itemId);
		bool IsTaskItemItem(UInt32 itemId);

		bool RunTaskItemUse(Player* player, UInt32 itemId);	
		bool RunItemTaskUse(Player* player, UInt32 itemId);	
		UInt16 RunItemNormalUse(Player* player, UInt32 itemId, UInt32 param, UInt16 num, bool bind);
		UInt16 RunItemNormalUseOther(Player* player, UInt32 itemId, Player* other, UInt16 num, bool bind);
        UInt16 getRandTrump(UInt8 lvl);

		void RunAutoRegen(Player* player, Fighter* fighter);

		void onLogin(Player* player);
		void onDungeonWin(Player* player, UInt8 id, UInt8 count);
		void onClanBattleAttend(Player* player);
		void onCountryBattleAttend(Player* player);
		void onCountryBattleWinStreak(Player* player, UInt32 count);
		void onAthleticWin(Player* player);
		void onDayTaskComplete(Player* player, UInt32 count);
		void onExchange(Player* player);
		bool onOnlineAward(Player* player, UInt32 itemId, UInt8 count);
        bool onThanksgivingDay(Player* player);
        bool onBlueactiveday(Player* player);
        bool onChristmas(Player* player);
        bool onNewYear(Player* player);
        bool onValentineDay(Player* player);
        bool onFoolsDay(Player* player);
        bool onMayDay(Player* player);
        bool onMayDay1(Player* player);
		void onAttackBoss(Player* player);
		bool getHeroMemoAward(Player* player, UInt8 idx, UInt32 soul);
		bool getShuoShuoAward(Player* player, UInt8 idx);
		bool onPurchase(Player* player, UInt32 id, UInt8 count);
		void exchangeExtraReward(Player* player, UInt32 id);
		void onLevelup(Player* player, UInt8 olev, UInt8 nlev);
		void onInvitedBy(Player* player);
		bool onGetCFriendAward(Player* player, UInt8 idx);
		void onGetVipPack(Player* player, UInt8 type);
		bool testTakePack(Player* player, UInt8 type, UInt8 freq);
		bool testTakePackSize(Player* player, UInt8 type);
        void onCopyFloorWin( Player* player, UInt8 id, UInt8 floor, UInt8 spot, UInt8 lootlvl);
        void onCopyWin( Player* player, UInt8 id, UInt8 floor, UInt8 spot, UInt8 lootlvl);
        void onFrontMapFloorWin( Player* player, UInt8 id, UInt8 spot, UInt8 lootlvl);
        void onFrontMapWin( Player* player, UInt8 id, UInt8 spot, UInt8 lootlvl);
		void onEnchant(Player* player, UInt8 level);
		void onEnchantAct(Player* player, UInt8 level, UInt8 type = 0);
		void onEnchantGt11(Player* player, UInt16 id, UInt8 level, UInt8 type);
        void onSoulEnchantMaxSoul(Player* player, UInt32 oms, UInt32 yams);
		void onTrainFighterAct(Player* player, Fighter* fgt);
        void onRechargeAct(Player* player, UInt32 total);
		void onTopup(Player* player, UInt32 ogold, UInt32 ngold);
		UInt32 onAttakerAddexp(Player* attacker, Player *defender, UInt32 award);
		UInt32 onTavernFlush(UInt8 color);
		bool onTakeMailPackage(Player* player, UInt32 pkgId);
		lua_tinker::table onGetMailItems(UInt32 pkgId);
        lua_tinker::table GetOnlineReward(UInt8 cnt);
        void   GetAtyReward(Player* p, UInt32 flag);
		UInt32 onLuckyDrawItemRoll(UInt8 type);
		UInt32 onDungeonLootItemRoll(Player * player, UInt8 id, UInt8 level, bool isBoss);
		void onMergeGem(Player * player, UInt8 lev, UInt8 num);
		bool getActivateAttrResult(UInt8 lastActivateCount, UInt8 quality);
        void onRecruitAward(Player* player);
        void onCLLoginReward(Player* player, UInt8 cts);
        UInt16 onCLLoginRewardRF(Player* player, UInt8 cts, UInt8 type);
        void onCL3DayReward(Player* player);
        bool onRC7DayWill(Player* player, UInt8 idx);
        UInt32 onUseMDSoul(Player* player, UInt8 type);
        bool onTurnOnRC7Day(Player* player, UInt32 total, UInt32 offset);
        bool onTurnOnRF7Day(Player* player, UInt32 total, UInt32 offset);
        lua_tinker::table luckyDraw(Player* player, UInt8 id, UInt8 num);
        bool RunSSDTAward(Player* player, UInt8 opt);
        lua_tinker::table luckyDraw(Player* player, UInt8 id, UInt8 num, bool bind);
        UInt16 RunHappyAward(Player* player, UInt8 opt);
        UInt16 RunTargetAward(Player* player);
        UInt16 RunTargetAwardRF(Player* player);
        void sendRNR(Player* player, UInt32 now, UInt32 date, UInt32 total);
        void sendRechargeMails(Player* player, UInt32 ototal, UInt32 ntotal);
        void onEquipForge( Player* player, UInt32 itemId, UInt32 onums);
	public:

		template <typename R>
		inline R Run(Player* player, const std::string& script);
		
		template <typename R, typename T1>
		inline R Run(Player* player, const std::string& script, const T1& t1);
		template <typename R, typename T1, typename T2>
		inline R Run(Player* player, const std::string& script, const T1& t1,  const T2& t2);
		template <typename R, typename T1, typename T2, typename T3>
		inline R Run(Player* player, const std::string& script, const T1& t1,  const T2& t2, const T3& t3);
		template <typename R, typename T1, typename T2, typename T3, typename T4>
		inline R Run(Player* player, const std::string& script, const T1& t1,  const T2& t2, const T3& t3, const T4& t4);

	public:
		template<typename R>
		inline R Call(const std::string& name);
		template<typename R, typename T1>
		inline R Call(const std::string& name, const T1& t1);
		template<typename R, typename T1, typename T2>
		inline R Call(const std::string& name, const T1& t1, const T2& t2);
		template<typename R, typename T1, typename T2, typename T3>
		inline R Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3);
		template<typename R, typename T1, typename T2, typename T3, typename T4>
		inline R Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3, const T4& t4);
		template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
		inline R Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5);


	protected:
		void RegisterActionInterface();						//ע??C++?????ӿ?

	public:
		Player* GetPlayer1() { return _player1; }
		Player* GetPlayer2() { return _player2; }

    // ?ɾʹ???
    public:
        void doAttainment(Player* pl, UInt16 attainId, UInt32 param);
        void finishAttainment(Player* pl, UInt16 attainId);

    public:
        void doAty( Player* pl, UInt32 id,  UInt32 param1, UInt32 param2);
        UInt32  GetAtyCheckFlag(UInt32 idx);
        UInt32  GetMaxActivity(UInt32 vip);
	private:
		Player* _player1;	//?ű???????Ϊ????1
		Player* _player2;	//?ű???????Ϊ????2

	private:
		UInt8 _tid;	// ?????߳?ID
	};

#include "GameActionLuaImpl.inl"

}

#endif
