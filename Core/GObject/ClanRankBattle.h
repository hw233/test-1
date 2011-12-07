#ifndef _CLAN_RANK_BATTLE_H_
#define _CLAN_RANK_BATTLE_H_

#include <map>
#include <vector>
#include "Config.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/Singleton.h"
#include "GData/AttrExtra.h"


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
     *@brief 帮会排名战役
     */
    class ClanRankBattleField
    {
        typedef std::vector<Player*> PlayerVec;

        enum PlayerBattleStatus
        {
            PLAYER_WAIT = 1,     //等待
            PLAYER_BATTLE = 2,   //战斗中
            PLAYER_DEAD = 3,     //死亡
            PLAYER_WIN = 4,      //连胜10场
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
         *@brief 获取帮会贡献
         */
        void End(UInt32 extScore1, UInt32 extScore2);


        void FillPlayers(Stream& stream, UInt32 clan);
        void FillPlayer(Stream& stream, Player* player);
        void NotifyPlayerLeave(Player* player);

    private:
        void ResetPlayerStatus(UInt32 clan);
        void ClearPlayerData(Player* player);

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
        void BroadcastStatus();
        void BroadcastScores();

        /**
         *@brief 广播
         */
        void Broadcast(Stream& stream, bool bAll = false)
        {
            m_Clan1->Broadcast(stream, bAll);
            m_Clan2->Broadcast(stream, bAll);
        }
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
        //战斗双方帮会
        ClanRankBattleInfo* m_Clan1;
        ClanRankBattleInfo* m_Clan2;
        //双方积分
        UInt32 m_ClanScore1;
        UInt32 m_ClanScore2;
        //战役
        ClanRankBattleField m_Fields[RANK_BATTLE_FIELD_NUM];
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
         *@brief 领取奖励
         */
        void GetRewards(Player* player);

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
        void SendRewards(Player* player);

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
        void GetCanBattleClans(bool bNotify = true);

        /**
         *@brief 给帮会排名
         */
        void SortClans();

        /**
         *@brief 没得到机会
         */
        void SetNoTurns(ClanRankBattleInfo* clan);

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
        
        //播报报名公告倒计时
        UInt32 m_SignupCountDown;
        //战斗场次
        UInt32 m_BattleNo;
        //战斗列表
        BattleVec m_Battles;

        //帮派技能
        ClanBattleSkill m_Skills[RANK_BATTLE_SKILL_NUM + 1];
    };
}


#endif

