﻿#ifndef TIANJIE_H_
#define TIANJIE_H_

#include <map>
#include "Config.h"
#include "GData/NpcGroup.h"
#include "GObject/WBossMgr.h"
#include "GObject/Fighter.h"
#include "Common/Stream.h"
#include "MsgHandler/MsgTypes.h"
#include "MsgHandler/MsgFunc.h"
#include "Common/BinaryReader.h"
#include "Battle/BattleSimulator.h"

using namespace std;

#define MIN(a,b)  ((a)<(b)? (a):(b))
#define TIANJIE_EXP(lv) ((lv-10)*(MIN(lv,99)/10)*5+25) 
typedef Battle::BattleSimulator BattleSimulator;
namespace GObject
{
    struct TlzNpcAttra
    {
        float attack;                                                                                                                                                 
        float magatk;                                                                                                                                                 
        float defend;                                                                                                                                                 
        float magdef;                                                                                                                                                 
        float hp;                                                                                                                                                     
        float action;                                                                                                                                                 
        float hitrate;                                                                                                                                                
        float evade; 
        float counter;
        float magres;
    };
    class Player;

    struct Tianleizhen
    {
        int npcid;
        int score;
    };
    class Tianjie
    {
    public:
		static Tianjie& instance()
		{
		    static Tianjie s;
			return s;
		}
    public:
        void setTj1Count(int count) {m_tj1Count = count;};
        void setTj2Count(int count) {m_tj2Count = count;};
        void setTj3Count(int count) {m_tj3Count = count;};
        void setTj4BossHp(int maxHp) {m_tj4BossHp = maxHp;};
    private:
        int m_tj1Count;
        int m_tj2Count;
        int m_tj3Count;
        int m_tj4BossHp;
    public:
		Tianjie();
		bool Init();
		bool LoadFromDB();
        UInt8 getTjTypeId() {return m_tjTypeId;};
        void setNetOk() {m_isNetOk = true;};
       
        void onTianjieReq(GameMsgHdr&, const void*);
        void getTianjieData(Player* pl, bool isLogin=false);
        void getRate1DailyData(Player* pl, Stream& st);
		void getRate2DailyData(Player* pl, Stream& st);
        void getRate3DailyData(Player* pl, Stream& st);

		void process(UInt32 now);
		void startTianjie(bool isRestart=false);
		void closeTianjie();
        bool isFinish();
		void goNext();

		void attack(Player* pl, UInt16 loc, UInt32 npcid);

        void getEvent1Data(Stream& st, Player* pl=NULL);
        void getEvent2Data(Stream& st, Player* pl=NULL);
        void getEvent3Data(Stream& st, Player* pl=NULL);
        void broadEvent1(Player* pl = NULL);
        void broadEvent2(Player* pl = NULL);
        void broadEvent3(Player* pl = NULL);
        void updateEventData(Player* pl);
        void updateRankData(Player* pl);
        void setRatePercent();
        void broadEventTop1(Player* pl);
	
        //雷魔闹世 
		void start1();                          //显示事件1的怪物
		void attack1(Player* pl, UInt16 loc, UInt32 npcid);
		void record1(Player* pl, int npcIndex); //计算玩家积分
		void close1();
        void randomTask1Data(int roleLev,short& npcId, UInt8& color, int& exp);

        //御雷神针
        void start2();
        void donate2(Player* pl, UInt8 id); //捐献资源
        void close2();

        //天雷阵
        void start3();
        void putTlzFighters(BattleSimulator& bsim, int tlzLevel);
        void attack3(Player* pl);
        void close3();
        int  makeTlzKey(UInt8 type, UInt16 level);

        bool attackTlz(Player* pl, UInt16 level);

        //末日天雷
        void broadBossAppear(UInt32 bossId, UInt16 loc, Player* pl=NULL);
        void broadBossCount(int count, Player* pl=NULL);
        void start4(bool isRestart=false);
        void attack4(Player* pl, UInt16 loc, UInt32 npcid);
        void close4();

        //最终BOSS
        void startBoss();
        bool attackBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final);
        void closeBoss();
        void sendHp(Player* player = NULL);
        void sendDmg(UInt32 damage);

        void broadTianjiePassed();
    public:
		bool isPlayerInTj(int playerLevel);
		bool isOpenTj(Player* pl);
        bool isTjExecute() {return m_isTjExecute;};
        bool isTjRateNpc(int npcid);
        bool isTjOpened() {return m_isTjOpened;};
		bool isTjNpc(int npcid, UInt16 loc);
        void OpenTj();

        void initSortMap();
        void insertToScoreSortMap(Player* pl, int newScore, int oldScore);
        void insertToEventSortMap(Player* pl, int newScore, int oldScore);
        short getEventRank(Player* pl);
        short getScoreRank(Player* pl);

	private:
	    bool addNpc(int npcid);
		void deleteNpc(int npcid, UInt16 loc);
        void rewardEventBox(Player*pl, int score);
        void rewardTotalBox(Player*pl, int score);
        void reward(multimap<int, Player*>& m, UInt8 varId, UInt8 EventOrTotal);
        void rewardBoss();
        void rewardTask();  //关闭天劫前,根据玩家的天劫声望发奖品

        std::string GetNextSection(std::string& strString , char cSeperator);
		
    private:
		GData::NpcGroup* _ng;
		multimap<int, int> m_locNpcMap; //据点上的怪物
	    UInt8 m_tjTypeId;
        bool m_isNetOk;

		bool m_isTjOpened;         //天劫开启状态
		bool m_isTjExecute;        //天劫事件是否正在运行
		bool m_isFinish;           //事件是否已达成
        int  m_isOk;               //BOSS是否已击杀
		int m_currOpenedTjLevel;   //当前的天劫
		int m_currTjRate;          //天劫的进度 1:雷魔闹世2:御雷神针3:天雷阵4:末日天雷
        UInt32 m_openTime;         //开启的时间
        int m_notifyRate;          // 1:15 min 2 :10min 3:5min 4:0min  5:opened
        int m_nextTjTime;

        int m_Top1Record;          //第一名玩家的积分
        int m_Top1PlayerId;        //第一名玩家的ID号

        int m_eventMaxNumber;      //该事件达成条件
        int m_eventCurrNumber;     //该事件已达成数
        int m_eventOldPercent;     //上一次广播的事件达成百分比
        UInt8 m_oldBroadPercent; 
        multimap<int, Player*> m_eventSortMap;
        multimap<int, Player*> m_scoreSortMap;
        //事件1
        short m_rate1KilledCount[4]; //怪物已击杀数

        //事件2
        short m_rate2DonateCount[4];                  //捐款总积分
 
        //事件3
        static const int s_maxTlzLevel = 500;    //天雷阵最高层
        string m_tlzNpcName[2];
        TlzNpcAttra m_tlzNpcBaseAttra[2];
        GObject::Fighter* m_fighter[2];
        //事件4
        int m_bossIndex;
        string m_bossName;
        UInt16 m_loc;                           //boss地点
        UInt8 m_bossDay;                        //打BOSS的天数
        UInt32  _hp;
        UInt32 m_bossMaxHp;
        AtkInfoType m_atkinfo;
        UInt8 _percent;

        pthread_mutex_t m_eventMutex;
        pthread_mutex_t m_totalMutex;
        pthread_mutex_t m_locMutex;
    };
}
#endif
