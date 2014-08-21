#ifndef DARKDARGON_H_
#define DARKDARGON_H_

#include <vector>
#include <map>
#include <set>
#include "Config.h"
#include "GData/NpcGroup.h"
#include "GObject/WBossMgr.h"
#include "GObject/Fighter.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"

using namespace std;

namespace GObject
{
    
    /*
     玩家状态--EPlayerStatus
        0:未在活动中
        1:驻防四轮之塔
        2:活动自由状态
     */
    enum EPlayerStatus
    {
        PLAYER_LEAVE = 0,
        PLAYER_DEFENCE = 1,
        PLAYER_FREE = 2,
    };

    /*
     活动状态--EBBStatus
        0:未初始化
        1:等待开启
        2:第一阶段
        3:第二阶段
        4:结束
     */
    enum EBBStatus
    {
        DARKDARGON_NOINIT = 0,
        DARKDARGON_AWAIT = 1,
        DARKDARGON_ONESTEP = 2,
        DARKDARGON_TWOSTEP = 3,
        DARKDARGON_OVER = 4,
    };
    
    #define MAXTIMESTAMP 0xFFFFFFFF

    class Player;

    struct DDPlayer
    {
        DDPlayer() : player(NULL),ePlStatus(PLAYER_LEAVE),toweridx(0),towerpos(0),score(0),driveCD(0),singleBuffer(0),thunderCD(0),recoverCD(0) 
        {
        }
        
        Player* player;
        EPlayerStatus ePlStatus;//玩家状态
        UInt8 toweridx;//塔编号
        UInt8 towerpos;//塔位置
        UInt32 score;//功德 
        UInt32 driveCD;//主动行动CD
        UInt8 singleBuffer;//单人BUFFER 0~3bit 圣佛尊帝 4 ~ 7bit 仙力加持
        UInt32 thunderCD;//天雷CD
        UInt32 recoverCD;//恢复活力CD

        void clearCD() { driveCD = 0; }

    };

    struct DDPlayerScore
    {
        Player * player;
        UInt32 score;
        UInt32 time;
        DDPlayerScore() : player(NULL), score(0), time(0) {}
    };

    struct ddlt_sort
    {
        bool operator() (const DDPlayerScore& a, const DDPlayerScore& b) const { return (a.score > b.score) || (a.score == b.score && a.time < b.time); }
    };

    struct RoundTower
    {
        UInt8 idx;//四轮之塔编号 1~4
        vector<Player*> defPlayer[6]; //6个塔驻防位置(塔顶，塔枢，中枢，塔底1，塔底2，塔底3)
        float durability;//耐久度
        UInt8 status;//状态 0 - 未初始化 1 - 开启 2 - 已攻破
        vector<UInt8> monsterNum;//怪物数 6组
        UInt8 defenceSum;//驻防总人数
        RoundTower(UInt8 i) : durability(100),status(0),defenceSum(0) 
        {
            idx = i; 
            vector<UInt8> tmpvector(6,30);
            monsterNum.swap(tmpvector);
        }
        void resetMonster(UInt8 num = 30) 
        {
            vector<UInt8> tmpvector(6,num);
            monsterNum.swap(tmpvector);
        }
        float calcDmg()
        {
            float dmg = 0.0f;
            for(vector<UInt8>::iterator it = monsterNum.begin(); it != monsterNum.end();it++)
            {
                if(*it)
                    dmg += static_cast<float>(0.05f * (*it));
            }
            return dmg;
        }
        void resetAll()
        {
            for (size_t m = 0; m < 6; m++)//遍历每一层
                defPlayer[m].clear();
            durability = 100;
            status = 0;
            vector<UInt8> tmpvector(6,30);
            monsterNum.swap(tmpvector);
            defenceSum = 0;
        }
    };

    struct StarMap
    {
        UInt8 idx;//三星阵编号 1~3
        UInt32 mapHp;//耐久度
        UInt32 arriveTS;//来临时间戳
        UInt8 arriveNum;//来临次数
        UInt8 status;//状态 0 - 未初始化 1 - 开启 2 - 已攻破
        StarMap(UInt8 i) : mapHp(100),arriveTS(0xFF),arriveNum(0),status(0) { idx = i; }     
        void overCD() { arriveTS = MAXTIMESTAMP; }
        void resetCD(UInt32 time) { arriveTS = time; }
        void resetAll()
        {
            mapHp = 100;
            arriveTS = 0xFF;
            arriveNum = 0;
            status = 0;
        }
    };

    typedef map<Player*,DDPlayer *> DDMap;
    typedef multiset<DDPlayerScore, ddlt_sort> DDPlayerScoreSort;
    typedef map<UInt64,Player *> PlayerMap;

    class DarkDargon : public Singleton <DarkDargon>
    {
    public:
        DarkDargon();
        ~DarkDargon();
        void process(UInt32 now);
        void RetAwaitInfo(Player* pl = NULL);
        void RetFirstStepInfo(Player* pl);
        void RetSecStepInfo(Player* pl);
        void RefreshBossInfo(Player* pl);
        void RetDargonArrive(Player* pl);
        void EnterDarkDargon(Player* pl);
        void QuitDarkDargon(Player* pl);

        void ReturnRoundTowerInfo(Player* pl,UInt8 idx, UInt8 pos = 1, UInt8 opt = 1, bool res = true,Player* objpl = NULL);
        void DefRoundTower(Player* pl,UInt8 idx, UInt8 pos);//驻防
        void QuitRoundTower(Player* pl);
        void PKRoundTower(Player* pl,UInt8 idx, UInt8 pos,UInt64 playerId);
        void RefreshTowerMonster(UInt8 num = 10);
        void AccountTowerMonster();
        void SendDefAccountInfo(UInt8 idx,Stream& st);
        void DestroyTower(UInt8 i);
        void AttackTowerMonster();
        void ReturnStarMapInfo(Player* pl,UInt8 opt,UInt8 idx = 1,UInt32 ext1 = 0,UInt8 ext2 = 0);
        UInt8 GetBufferFlag(DDPlayer* ddpl,bool isStarMap);
        void SetBufferFlag(Player* pl, UInt8 bit_num);
        void AttackStarMap(Player* pl,UInt8 idx,UInt8 opt/* 0 - 破坏阵眼 1 - 延滞降临 */);
        void CheckDargonArrive(UInt8 idx);
        void InitStarMap(UInt8 idx = 0);

        void OptBoss(Player* pl,UInt8 opt);
        void AttackBoss(Player* pl);
        void DamageBoss(DDPlayer* ddpl,UInt32 dmg);
        void AutoDamageBoss();
        void ReturnBossInfo(Player* pl,UInt8 type,UInt32 ext1 = 0);

        bool attackPlayer(DDPlayer* atker,DDPlayer* defer);
        bool attackNpc(DDPlayer* atker,GData::NpcGroup* n_ng , bool b_extend = false,UInt8 idx = 0xFF);
        DDPlayer* getDDPlayer(Player* pl);
        Player* getPlayer(UInt64 playerId);

        typedef void (DarkDargon::*Func1)(Player*); 

        void broadcast(Stream& st,bool isIn = true);
        void broadcast(Func1 func,bool isIn = true);
        void AddDDScore(DDPlayer* ddpl,Int32 score);
        void AddAllDDScore(Int32 score);
        void AddDargonArriveTime(DDPlayer* ddpl,UInt8 idx,UInt32 add_tm);
        void rebuildNpc();
        bool CheckStartAct();
            
        void rebuildFgtAttr(Player* player,bool isddbuf);
        inline EBBStatus getStatus() { return _status; }
        void GMDestroyStarMap();
        void AccountFirstStepTask();
        void AccountFinalScore();
        void DestroyStarMap();
        void ReserAllFunc();

    private:
		GData::NpcGroup* _ng;//塔怪
		GData::NpcGroup* _ng1;//阵怪
		GData::NpcGroup* _ng2;//boss
        UInt32 beginTime;
        EBBStatus _status; 
        RoundTower* roundTowers[4];
        StarMap* starMaps[3];
        UInt8 globalBuffer;//7个全服BUFFER 0~3bit 四轮之塔 4~6bit 三星阵 初始值 - 0x0f 
        DDMap m_ddMap;
        DDPlayerScoreSort s_pss;
        PlayerMap m_pMap;
        
        UInt32 _lastHP;
        Int32 _lastAtk;
        Int32 _lastMAtk;

        UInt32 _hp; // boss 

    };






}

#endif


