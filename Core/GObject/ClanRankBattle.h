#ifndef _CLAN_RANK_BATTLE_H_
#define _CLAN_RANK_BATTLE_H_

#include <set>
#include <map>
#include <vector>
#include "Config.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/Singleton.h"
#include "GData/AttrExtra.h"
#include "Mail.h"

namespace GObject
{

    class Clan;
    class Player;
    class ClanRankBattle;

    //一场战斗战场数
    const static UInt32 RANK_BATTLE_FIELD_NUM = 6;
    //战斗据点
    const static UInt32 RANK_BATTLE_LOCATION = 0x0713;
    //帮会战技能列表
    const static UInt32 RANK_BATTLE_SKILL_NUM = 12;

    /**
     *@brief 排名战帮会信息
     */
    struct ClanRankBattleInfo
    {
        typedef std::vector<Player*> PlayerVec;
        typedef std::map<UInt32, PlayerVec> PlayerMap;

    public:
        ClanRankBattleInfo(Clan* _clan):clan(_clan), battle(NULL), m_bLowerHead(true){}

    public:

        bool HasPlayer(Player* player);
        UInt32 GetBattleField(Player* player);
        void RemovePlayer(Player* player, UInt32 field = UInt32(-1));
        void AddPlayer(Player* player, UInt32 field);
        UInt32 GetMinPlayerField();
        UInt32 GetPlayerNum();
        /**
         *@brief 广播给本帮参加帮会战的人
         *@param bAll true包含不再帮战地点的人   false不包括
         */
        void Broadcast(Stream& stream, bool bAll = false);

    public:
        //帮会
        Clan* clan;
        //当前战争
        ClanRankBattle* battle;
        //当前排列顺序是否为才从小到大
        bool m_bLowerHead;
        //队伍
        PlayerMap  players;
    };

    /**
     *@brief 帮会战奖励
     */
    typedef MailPackage::MailItem ClanBattleReward;

    
    /**
     *@brief 帮会战技能
     */
    struct ClanBattleSkill
    {
        ClanBattleSkill()
            :id(0),price(0){}

        //id
        UInt32 id;
        //价格
        UInt32 price;
        //技能加成属性
        GData::AttrExtra  attrs;
    };


    /**
     *@brief 帮会战战报信息
     */
    struct ClanRankBattleReport
    {
        ClanRankBattleReport():result(0), reportid(0){}

        //战斗结果 1为玩家1胜  2为玩家2胜
        UInt8 result;
        //玩家1
        std::string player1;
        //玩家2
        std::string player2;
        //战报id
        UInt32 reportid;
    };


    /**
     *@brief 帮会排名战役
     */
    class ClanRankBattleField
    {
        typedef std::vector<Player*> PlayerVec;
        typedef std::vector<ClanRankBattleReport> ReportVec;

        enum PlayerBattleStatus
        {
            PLAYER_WAIT = 1,     //等待
            PLAYER_BATTLE = 2,   //战斗中
            PLAYER_DEAD = 3,     //死亡
            PLAYER_WIN = 4,      //连胜8场
        };

    public:
        ClanRankBattleField();
        ~ClanRankBattleField();

    public:
        void Init(UInt32 id, ClanRankBattle* battle);

        /**
         *@brief 开始战役
         */
        void Start(UInt32 clan1, const PlayerVec& team1, UInt32 clan2, const PlayerVec& team2, UInt32 now);
        /**
         *@brief 处理
         */
        void Process(UInt32 now);
        /**
         *@brief 判断是否结束
         */
        bool IsEnd() const { return m_bEnd; }
        /**
         *@brief 结算 
         */
        void End(UInt32 extScore1, UInt32 extScore2);
        /**
         *@brief 战役结束后获取胜利方
         */
        UInt32 GetWinner() const { return m_Winner; }
        /**
         *@brief 玩家退出
         */
        bool OnPlayerLeave(Player* player);
        /**
         *@brief 获取一个帮会玩家数
         */
        UInt32 GetPlayerNum(UInt32 clan);
        /**
         *@brief 重置
         */
        void Reset();
        /**
         *@brief 获取连胜玩家数
         */
        UInt32 GetWinStateNum() const;
        

        void FillPlayers(Stream& stream, UInt32 clan);
        void FillPlayer(Stream& stream, Player* player);
        void NotifyPlayerLeave(Player* player);
        void Broadcast(UInt32 clan, Stream& stream);
        void Broadcast(Stream& stream);
        void SendBattleReport(Player* player);

    private:
        void ResetPlayerStatus(UInt32 clan);
        void ResetPlayerData(Player* player);
        void GetReportsData(Stream& stream);

    private:
        //id
        UInt32 m_Id;
        //所属战斗
        ClanRankBattle* m_pBattle;

        //帮会
        UInt32 m_Clan1;
        UInt32 m_Clan2;
        //等待队伍
        std::map<UInt32, PlayerVec>   m_WaitPlayers;
        //死亡队伍
        std::map<UInt32, PlayerVec>   m_DeadPlayers;
        //状态改变
        std::map<UInt64, UInt32>  m_StatusChanged;

        //战报时间
        UInt32 m_ReportTime;
        //本次战报
        ReportVec m_Reports;

        //最后连胜方
        UInt32 m_LastWinClan;

        //当前时间
        UInt32 m_Now;
        //开始时间
        UInt32 m_StartTime;
        //轮次
        UInt32 m_Round;
        //是否结束
        bool m_bEnd;
        //胜利者(结束后返回0表示平局)
        UInt32 m_Winner;
    };



    /**
     *@brief 帮会排名站一场战斗
     */
    class ClanRankBattle
    {
        typedef std::vector<Player*> PlayerVec;

        //状态
        enum BattleState
        {
            STATE_PREPARE,  //装备阶段
            STATE_BATTLE,   //战斗阶段
        };

    public:
        ClanRankBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2);
        ~ClanRankBattle();

    public:
        /**
         *@brief 定时处理
         */
        void Process(UInt32 now);
        
        /**
         *@brief 开始结束
         */
        void Start(UInt32 now);
        void End();
        void Reset();
        bool IsEnd() const { return m_bEnd; }

        /**
         *@brief 玩家离开
         */
        void OnPlayerLeave(Player* player);

        /**
         *@brief 获取对战方帮会
         */
        ClanRankBattleInfo* GetClan(UInt32 id);
        ClanRankBattleInfo* GetOtherClan(UInt32 id);

        /**
         *@brief 使用技能
         */
        void UseSkill(Player* player, ClanBattleSkill* skill);

        /**
         *@brief 发送相关信息
         */
        void SendBattleStatus(Player* player);
        void SendBattleInfo(Player* player);
        void SendBattleReport(Player* player, UInt8 fightId);
        void BroadcastStatus();
        void BroadcastScores(UInt8 fightId, UInt32 winner, UInt32 extScore = 0);

        /**
         *@brief 广播
         */
        void Broadcast(Stream& stream, bool bAll = false);

        /**
         *@brief 广播给战斗中玩家
         */
        void BroadcastBattle(UInt32 clan, Stream& stream);
        void BroadcastBattle(Stream& stream);

    private:
        /**
         *@brief 获取战役
         */
        ClanRankBattleField* GetField(UInt32 id){
            if(id >= RANK_BATTLE_FIELD_NUM) return NULL;
            return &m_Fields[id];
        }

    private:
        //状态
        BattleState m_State;
        //开始时间
        UInt32 m_StartTime;
        //当前时间
        UInt32 m_Now;
        //是否结束
        bool m_bEnd;
        //战斗双方帮会
        ClanRankBattleInfo* m_Clan1;
        ClanRankBattleInfo* m_Clan2;
        //双方积分
        UInt32 m_ClanScore1;
        UInt32 m_ClanScore2;
        //战役
        ClanRankBattleField m_Fields[RANK_BATTLE_FIELD_NUM];
        //最终胜利方
        UInt32 m_Winner;
    };

     
    /**
     *@brief 帮会排名战
     */
    class ClanRankBattleMgr : public Singleton<ClanRankBattleMgr>
    {
        typedef std::map<UInt32,ClanRankBattleInfo> ClanMap;
        typedef std::vector<ClanRankBattle*> BattleVec;
        typedef std::map<UInt32, ClanRankBattle*> BattleMap;
        typedef std::vector<Clan*> ClanVec;
        typedef std::vector<Player*> PlayerVec;
        typedef std::set<Player*> PlayerSet;

        typedef std::vector<ClanBattleReward> RewardVec;
        typedef std::map<UInt32, RewardVec> RewardsMap;

        /**
         *@brief 战斗状态
         */
        enum BattleState
        {
            STATE_INIT,    //初始状态
            STATE_SIGNUP,  //报名状态
            STATE_BATTLE,  //战斗状态
        };


    public:
        ClanRankBattleMgr();
        ~ClanRankBattleMgr();

        /**
         *@brief 初始化
         */
        void Init();
        void UnInit();

        /**
         *@brief 是否在帮会战报名战斗期间
         */
        bool IsInBattle(Clan* clan);

        /**
         *@brief 定时处理
         */
        void Process(UInt32 now);

        /**
         *@brief 报名,退出(注意这两个接口会多个线程访问)
         */
        void Signup(Player* player, UInt32 fieldId);
        void Signout(Player* player);

        /**
         *@brief 玩家进入离开帮会战地点
         */
        void PlayerEnter(Player* player);
        void PlayerLeave(Player* player);

        /**
         *@brief 重排列帮会的出战队伍
         */
        void SortClanPlayers(Player* player);
        void AdjustPlayerPos(Player* player, Player* member, UInt32 field, UInt32 pos);

        /**
         *@brief 使用技能
         */
        void UseSkill(Player* player, UInt32 id);

        /**
         *@brief 发送相关信息
         */
        void SendInitStatus(Player* player);
        void SendBattleStatus(Player* player);
        void SendSortList(Player* player, UInt16 start, UInt8 count);
        void SyncState();
        void SendPlayerList(Player* player);
        void SendBattleInfo(Player* player);
        void SendSkillList(Player* player);
        void SendBattleReport(Player* player, UInt8 fightId);

        /**
         *@brief 奖励相关
         */
        void GiveDailyRewards();
        void GiveWeeklyRewards();
        void GiveClanRewards(Clan* clan, RewardVec& rewards);

        /**
         *@brief 创建邮件消息内容
         */
        void MakeDailyMailInfo(UInt32 score, Stream& st);
        void MakeWeeklyMailInfo(UInt32 score, Stream& st);
        void MakeMailInfo(Stream& st, RewardVec& rewards);

        /**
         *@brief 添加道具
         */
        bool AddDailyMailItems(Player* player, UInt32 score);
        bool AddWeeklyMailItems(Player* player, UInt32 score);
        bool AddMailItems(Player* player, RewardVec& rewards);

    private:
        void ProcessInit(bool bWeekChange);
        void ProcessSignup();
        void ProcessBattle();

    private:
        /**
         *@brief 开始一场战斗
         */
        void StartOneBattle();
        void EndOneBattle();

        /**
         *@brief 创建战斗
         */
        bool CreateBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2);

        /**
         *@brief 获取一个帮会的信息
         */
        ClanRankBattleInfo* GetClanInfo(UInt32 id);

        /**
         *@brief 获取可以参加帮会战的帮派
         */
        void GetCanBattleClans(bool bBegin = true);

        /**
         *@brief 给帮会排名
         */
        void SortClans(bool bNotify);

        /**
         *@brief 给经验奖励，一分钟一次
         */
        void CheckAddExp();

    private:
        //当前状态
        BattleState m_State;
        //当前时间
        UInt32 m_Now;
        //帮会战开始时间(包括报名准备阶段)
        UInt32 m_StartTime;
        //加经验次数
        UInt32 m_expTime;
        
        //参加战斗的帮会列表
        ClanMap m_Clans;
        //当前帮会积分排名
        ClanVec m_ClanRanking;
        //在据点上的玩家列表
        PlayerSet m_InplacePlayers;

        
        //播报报名公告倒计时
        UInt32 m_SignupCountDown;
        //战斗场次
        UInt32 m_BattleNo;
        //战斗列表
        BattleVec m_Battles;

        //帮派技能
        ClanBattleSkill m_Skills[RANK_BATTLE_SKILL_NUM + 1];

        //每日个人奖励
        RewardsMap  m_DailyRewards;
        //每周个人奖励
        RewardsMap  m_WeeklyRewards;
        //每周帮会积分奖励
        RewardsMap  m_WeeklyClanRewards;
        //每周帮会排名奖励
        RewardsMap  m_WeeklyClanSortRewards;
    };
}


#endif

