#ifndef _GAMEACTIONLUA_INC_
#define _GAMEACTIONLUA_INC_

#include "Script.h"

typedef lua_tinker::table Table;

#include "GData/GDataManager.h"
#include "GObject/TaskData.h"

namespace GObject
{
	class Player;
	class Fighter;
}
using namespace GObject;

namespace Script
{
	extern 	void SysBroadcast(UInt8 type, const char * msg);

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

		Table RunGreatFighterAction(Player * player, UInt32 fightId);
		Table RunGreatCollectActionStep(Player * player, UInt32 fightId);
		Table RunGreatCtrlActionStep(Player * player, UInt32 fightId);
		bool  RunGreatTaskAction(Player * player, UInt32 fgtId);
		UInt32 GetGreatFighterFavor(UInt32 fightId);
		UInt32 GetGreatFighterReqFriendliness(UInt32 fightId);
		Table RunDayCopyTask(Player *, UInt32);
		Table RunDayCopyTaskStep(Player *, UInt32, UInt32);
        Table RunActiveTask(Player * player, UInt32 npcId);
        Table RunActiveTaskStep(Player *, UInt32, UInt32);

		Table RunNpcDefaultAction(Player * player, UInt32 npcId);

		bool RunTakeFighterAction(Player *);

		void  RunItemTaskAction(Player* player, UInt32 taskId, UInt32 dummyNpcId);

		UInt8 GetGreatFighterLev(UInt32);
		UInt8 GetGreatFighterCorlor(UInt32 fgtId);

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
		//这里在脚本中把探索类的消息发给client
		bool RunExploreTask(Player* player, UInt32 npcId);	//true:有探索任务， false:没有探索任务

		bool  RunAutoBattleAction(Player* player, UInt32 npcId, UInt32 turns);
		
		bool  CheckTaskAcceptCondition(Player* player, UInt32 taskId);	//任务接受条件检查
		Table RunTask(Player* player, UInt32 taskId, UInt32 npcId);
		Table RunTaskStep(Player* player, UInt32 taskId, UInt8 step);
		Table RunDayTask(Player* player, UInt32 npcId);
		Table RunDayTaskAccept(Player* player, UInt32 taskId, UInt32 npcId);

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
		bool RunItemNormalUse(Player* player, UInt32 itemId, UInt32 param, UInt16 num, bool bind);

		void RunAutoRegen(Player* player, Fighter* fighter);

		void onLogin(Player* player);
		void onDungeonWin(Player* player, UInt8 id, UInt8 level);
		void onClanBattleAttend(Player* player);
		void onCountryBattleAttend(Player* player);
		void onCountryBattleWinStreak(Player* player, UInt32 count);
		void onAthleticWin(Player* player);
		void onDayTaskComplete(Player* player, UInt32 count);
		void onExchange(Player* player);
		bool onOnlineAward(Player* player, UInt32 itemId, UInt8 count);
		void onAttackBoss(Player* player);
		bool onPurchase(Player* player, UInt32 id, UInt8 count);
		void exchangeExtraReward(Player* player, UInt32 id);
		void onLevelup(Player* player, UInt8 olev, UInt8 nlev);
		void onEnchant(Player* player, UInt8 level);
		void onTopup(Player* player, UInt32 ogold, UInt32 ngold);
		UInt32 onAttakerAddexp(Player* attacker, Player *defender, UInt32 award);
		UInt32 onTavernFlush(UInt8 color);
		bool onTakeMailPackage(Player* player, UInt32 pkgId);
		lua_tinker::table onGetMailItems(UInt32 pkgId);
		UInt32 onLuckyDrawItemRoll(UInt8 type);
		UInt32 onDungeonLootItemRoll(Player * player, UInt8 id, UInt8 level, bool isBoss);
		void onMergeGem(Player * player, UInt8 lev, UInt8 num);
		bool getActivateAttrResult(UInt8 lastActivateCount, UInt8 quality);

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
		void RegisterActionInterface();						//注册C++函数接口

	public:
		Player* GetPlayer1() { return _player1; }
		Player* GetPlayer2() { return _player2; }

    // 成就达成
    public:
        void doAttainment(Player* pl, UInt16 attainId, UInt32 param);
        void finishAttainment(Player* pl, UInt16 attainId);

	private:
		Player* _player1;	//脚本动作行为主体1
		Player* _player2;	//脚本动作行为主体2

	private:
		UInt8 _tid;	// 所属线程ID
	};

#include "GameActionLuaImpl.inl"

}

#endif
