#ifndef _SINGLEHEROSTAGE_H_
#define _SINGLEHEROSTAGE_H_

#include "SingleHeroFighter.h"


class Stream;

namespace GObject
{

#define STAGE_FINAL_PLAYER_CNT 32
#define SINGLE_HERO_OPEN_LEVEL 70
#define MIN_VOTE_LEVEL         60
#define SH_OPEN_COND           99

    class Player;
    class StarTeam;

    class SHStarStage;
    class SHBattleStage;
    class SHTowerStage;

    class SHFinalStarStage;
    class SHFinalBattleStage;

    struct StageBattleReport
    {
        UInt8 _won;
        UInt8 _type;     // 0-胜1-负2-负
        UInt32 _battleId;
        UInt32 _battleTime;
        Player* _other;
    };

    struct TeamStageBattleReport
    {
        UInt8 _won;
        UInt8 _type;
        UInt32 _battleId;
        UInt32 _battleTime;
        StarTeam* _other;
    };

    struct QualifyBattle
    {
        UInt8 _id;
        std::vector<UInt8> _won;
        std::vector<UInt32> _battleId;
        QualifyBattle() { reset(); }
        void reset();
        UInt8 push(UInt8, UInt32);
        UInt8 winner();
    };

    struct TowerBattleReport
    {
        UInt8 _won;     // 0-胜1-负2-负
        UInt16 _level;
        UInt32 _turns;
        UInt32 _battleId;
        UInt32 _npcId;
    };

    struct SingleHeroPlayer
    {
        UInt8 _type;
        UInt8 _won;
        UInt8 _lose;
        UInt32 _teamId;
        Player* _player;
        SingleHeroFighter* _fgt;
        std::vector<StageBattleReport> _battles;

        SingleHeroPlayer(): _type(0), _won(0), _lose(0), _teamId(0), _player(NULL), _fgt(NULL) {}

        void PutFighters( Battle::BattleSimulator& bsim, int side );
        bool challenge( SingleHeroPlayer* other, UInt32 * rid );
        void setEnter(Player* pl, SingleHeroFighter* fgt, UInt8 type, bool writeDB = true);
        void setTeam(UInt32 teamId);
    };
    struct SHTowerPlayer
    {
        UInt8 _type;
        UInt16 _tLevel;
        UInt32 _tTurns;
        Player* _player;
        SingleHeroFighter* _fgt;
        std::map<UInt16, UInt32> _battles; // <lvl, rptId>

        SHTowerPlayer(): _type(0), _tLevel(0), _tTurns(0), _player(NULL), _fgt(NULL) {}

        void PutFighters(Battle::BattleSimulator& bsim);
        bool challenge(std::vector<std::vector<UInt32>>& monsters);
        void setEnter(Player* pl, SingleHeroFighter* fgt, UInt8 type, bool writeDB = true);
    };

    struct SHSupportPlayer
    {
        UInt8 _flag;
        Player* _player;
        std::vector<Player*> _mySupport;
        SHSupportPlayer(Player * player)
            : _flag(0), _player(player)
        {
            _mySupport.clear();
            _mySupport.resize(4);
            for (UInt8 i = 0; i < 4; ++ i)
            {
                _mySupport[i] = NULL;
            }
        }
        UInt8 getSupportCount()
        {
            UInt8 count = 0;
            UInt8 flag = _flag;
            for (UInt8 i = 0; i < 8; ++ i)
            {
                if (flag & 0x01)
                    ++ count;
                flag >>= 1;
            }
            return count;
        }
        Player * getPlayerByClass(UInt8 cls)
        {
            return _mySupport[cls];
        }
    };
    struct SHCandidate
    {
        Player *_player;
        UInt32 _supportedCount;
        UInt8  _pos;
        SHCandidate(Player* player)
            : _player(player), _supportedCount(0), _pos(0)
        {
        }
        SHCandidate(Player* player, UInt32 supportedCount, UInt8 pos)
            :_player(player), _supportedCount(supportedCount), _pos(pos)
        {
        }
    };

    struct StarTeam
    {
        UInt8 _type;
        UInt8 _won;
        UInt8 _lose;
        UInt32 _teamId;
        std::vector<SingleHeroPlayer*> _playerList;
        std::vector<TeamStageBattleReport> _battles;

        StarTeam() : _type(0), _won(0), _lose(0), _teamId(0) {}
        void PutFighters( Battle::BattleSimulator& bsim, int side );
        bool challenge( StarTeam * other, UInt32 * rid );
    };

    struct SHPlayerScore
    {
        UInt32 _score;
        Player* _player;

        SHPlayerScore() : _score(0), _player(NULL) {}
    };

    class SHPlayerVisitor
    {
    public:
        virtual ~SHPlayerVisitor(){}
        virtual bool operator()(SingleHeroPlayer*, UInt8 cls, UInt8 idx) = 0;
    };

    class SHTeamVisitor
    {
    public:
        virtual ~SHTeamVisitor(){}
        virtual bool operator()(StarTeam*, UInt8 idx) = 0;
    };


    enum GroupType {
        e_sh_gp_sl = 0x01, // 单职业
        e_sh_gp_sy = 0x02, // 策略
        e_sh_gp_sr = 0x03, // 明星
        e_sh_gp_tw = 0x04, // 塔
        e_sh_gp_sp = 0x05, // 人气
    };
    enum StageType {
        e_sh_slsy     = 0x01,  // 单职业,策略,塔
        e_sh_sl_earth = 0x02,  // 单职业地榜
        e_sh_sl_final = 0x04,  // 单职业决赛
        e_sh_sy_earth = 0x08,  // 策略地榜
        e_sh_sy_final = 0x10,  // 策略决赛
        e_sh_sr       = 0x20,  // 明星,人气
        e_sh_sr_earth = 0x40,  // 明星地榜
        e_sh_sr_final = 0x80,  // 明星决赛
    };

#define SH_GP_OFFSET_B_E    11
    enum GlobalProgress {
        e_sh_signIn         = 1,  // 报名
        e_sh_signIn_end     = 2,  // 报名
        e_sh_single_pre     = 3,  // 单职业预选赛
        e_sh_single_eli     = 4,  // 单职业淘汰赛
        e_sh_strategy_pre   = 5,  // 策略预选赛
        e_sh_single_fin     = 6,  // 单职业决赛
        e_sh_star_pre       = 7,  // 明星预选赛
        e_sh_strategy_eli_s = 8,  // 策略淘汰赛,人气
        e_sh_star_eli       = 9,  // 明星淘汰赛
        e_sh_strategy_fin   = 10,  // 策略决赛
        e_sh_star_fin       = 11,  // 明星决赛
        e_sh_tower          = 12,  // 逆转乾坤
        e_sh_result         = 13,  // 结果

        e_sh_single_pre_end     = 14,  // 单职业预选赛结束
        e_sh_single_eli_end     = 15,  // 单职业淘汰赛结束
        e_sh_strategy_pre_end   = 16,  // 策略预选赛结束
        e_sh_single_fin_end     = 17,  // 单职业决赛结束
        e_sh_star_pre_end       = 18,  // 明星预选赛结束
        e_sh_strategy_eli_s_end = 19,  // 策略淘汰赛结束
        e_sh_star_eli_end       = 20,  // 明星淘汰赛结束
        e_sh_strategy_fin_end   = 21,  // 策略决赛结束
        e_sh_star_fin_end       = 22,  // 明星决赛结束
        e_sh_tower_end          = 23,  // 逆转乾坤结束
        e_sh_result_end         = 24,  // 结束
    };
    enum StageProgress {
        e_sh_pre  = 1, // 预赛
        e_sh_16  = 2,  // 16强赛
        e_sh_8   = 3,  // 8强赛
        e_sh_4   = 4,  // 4强赛
        e_sh_2   = 5,  // 2强赛
        e_sh_1   = 6,  // 1,2,3排名赛
        e_sh_0   = 7,  // 决赛
        e_sh_end  = 8, // 结束
    };

    class SHBattleStage
    {
    public:
        SHBattleStage(StageType type, GroupType gpId, UInt8 cls);
        ~SHBattleStage();
        bool enter(Player* player, SingleHeroFighter* fgt);

        bool processPreliminary();
        bool processFinals();
        bool process3Finals(); // 1,2,3名

        void initQualifyStage();
        void loadFromDB();
        void clear();

        void write2DB();
        void pushPlayer(Player* pl, SingleHeroFighter* fgt);

        bool sendEliminary(Stream& st, UInt8 type);
        bool isInFinal(Player* pl);

        inline UInt32 getPlayerCount() { return m_players.size(); }
        inline SingleHeroPlayer* getStagePlayer(Player* pl)
        {
            std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(pl);
            if(it == m_players.end())
                return NULL;

            return it->second;
        }

        void sendStageMail(SingleHeroPlayer* spl, bool win);

    private:
        UInt8 m_currIdx;
        UInt8 m_swapIdx;
        UInt8 m_gpId;
        UInt8 m_cls;
        UInt8 m_type;     // 擂台类型: StageType
        UInt8 m_round;
        UInt8 m_progress;

        std::map<Player*, SingleHeroPlayer*> m_players; // 玩家

        std::vector<SingleHeroPlayer*> m_preliminary[2][5]; // 预选赛[idx][round]
        SingleHeroPlayer* m_finals[2][5][16]; // 晋级赛[stage:上下场][progress][pos]
        QualifyBattle m_finalBattles[2][15];  // 晋级赛战报:[stage:上下场][idx]
        SingleHeroPlayer* m_3finals[2][4];       // 1,2,3名:[pos]
        QualifyBattle m_3finalBattles[2];     // 1,2,3名赛战报
    };
    class SHFinalBattleStage
    {
    public:
        SHFinalBattleStage(StageType type, GroupType gpId, UInt8 cls);
        ~SHFinalBattleStage();
        bool enter(Player* player, SingleHeroFighter* fgt);
        void clear();
        void loadFromDB();
        void pushPlayer(Player* pl, SingleHeroFighter* fgt);

        bool process();
        void write2DB();

        bool sendEliminary(Stream& st);
        void sendLeaderBoard(Stream& st);

        inline SingleHeroPlayer* getStagePlayer(Player* pl)
        {
            std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(pl);
            if(it == m_players.end())
                return NULL;

            return it->second;
        }

        void visitFinals(SHPlayerVisitor& visitor);

        void sendStageMail(SingleHeroPlayer* spl, UInt8 idx);

    private:
        std::map<Player*, SingleHeroPlayer*> m_players; // 玩家
        std::vector<SingleHeroPlayer*> m_finals;
        UInt8 m_gpId;
        UInt8 m_cls;
        UInt8 m_progress;
        UInt8 m_round;
        UInt8 m_type;
    };


    class SHTowerStage
    {
    public:
        SHTowerStage(StageType type, GroupType gpId, UInt8 cls);
        ~SHTowerStage();
        void loadFromDB();
        bool process(UInt8 process);
        bool enter(Player* player, SingleHeroFighter* fgt);
        void clear();
        void pushPlayer(Player* player, SingleHeroFighter* fgt, UInt16 lvl, UInt32 turns, UInt16 lastTurns);
        bool challenge(SingleHeroFighter* fighter);
        void sendTowerInfo(Player* player, UInt16 tLevel, UInt32 tTurns);
        void setLevel(UInt16 level);
        Player *getTopPlayer(UInt8 pos);
        UInt16 getTopPlayerLevel(UInt8 pos);
        UInt32 getTopPlayerTurns(UInt8 pos);
       
    private:
        std::vector<UInt32> m_Monsters;

        struct TowerGreater
        {
            bool operator() (const SingleHeroFighter* shfFirst, const SingleHeroFighter* shfSecond)
            {
                if (shfFirst->getTLevel() != shfSecond->getTLevel())
                    return shfFirst->getTLevel() > shfSecond->getTLevel(); 
                if (shfFirst->getTTurns() != shfSecond->getTTurns())
                    return shfFirst->getTTurns() < shfSecond->getTTurns();
                if (shfFirst->getTLastTurns() != shfSecond->getTLastTurns())
                    return shfFirst->getTLastTurns() < shfSecond->getTLastTurns();
                return shfFirst < shfSecond;
            }
        };

        bool TowerPlayerBetter(const SingleHeroFighter* shfFirst, const SingleHeroFighter* shfSecond)
        {
            if (shfFirst->getTLevel() != shfSecond->getTLevel())
                return shfFirst->getTLevel() > shfSecond->getTLevel(); 
            if (shfFirst->getTTurns() != shfSecond->getTTurns())
                return shfFirst->getTTurns() < shfSecond->getTTurns();
            if (shfFirst->getTLastTurns() != shfSecond->getTLastTurns())
                return shfFirst->getTLastTurns() < shfSecond->getTLastTurns();
            return false;
        }

        std::set<SingleHeroFighter*, TowerGreater> m_towerFighters;
        typedef std::set<SingleHeroFighter*, TowerGreater>::iterator TowerFighterIt;
        std::map<Player*, TowerFighterIt> m_towerFightersIt;
        typedef std::vector<TowerBattleReport> TowerBattles;
        std::map<Player*, TowerBattles> _battleMap;
        SingleHeroFighter* m_topTowerFighter[3];

        URandom _rnd;
        UInt8 m_gpId;
        UInt8 m_round;
        UInt8 m_type;
        UInt8 m_cls;
    };

    class SHStarStage
    {
    public:
        SHStarStage(StageType type, GroupType gpId);
        ~SHStarStage();
        bool enter(Player* player, SingleHeroFighter* fgt);
        bool enter(StarTeam* starTeam);

        bool processPreliminary();
        bool processFinals();
        bool process3Finals(); // 1,2,3名

        void createTeams();
        void initQualifyStage();
        void loadFromDB();
        void clear();

        void write2DB();
        void pushPlayer(Player* pl, UInt32 teamId, SingleHeroFighter* fgt);

        bool sendEliminary(Stream& st, UInt8 type);
        bool isInFinal(Player* pl);
        void sendStageMail(StarTeam* spl, bool win);

        inline UInt32 getTeamCount() { return m_starTeam.size(); }
        inline SingleHeroPlayer* getStagePlayer(Player* pl)
        {
            std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(pl);
            if(it == m_players.end())
                return NULL;

            return it->second;
        }
        inline StarTeam* getStageTeam(Player* pl)
        {
            std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(pl);
            if(it == m_players.end())
                return NULL;

            UInt32 teamId = it->second->_teamId;
            std::map<UInt32, StarTeam*>::iterator itTeam = m_starTeam.find(teamId);
            if(itTeam == m_starTeam.end())
                return NULL;

            return itTeam->second;
        }

    private:
        UInt8 m_currIdx;
        UInt8 m_swapIdx;
        UInt8 m_gpId;
        UInt8 m_type;     // 擂台类型: StageType
        UInt8 m_round;    // 比赛场次
        UInt8 m_progress; // 擂台赛阶段

        std::map<Player*, SingleHeroPlayer*> m_players; // 玩家
        std::map<UInt32, StarTeam*> m_starTeam; // 明星队伍

        std::vector<StarTeam*> m_preliminary[2][5]; // 预选赛[idx][round]
        StarTeam* m_finals[2][5][16]; // 晋级赛[stage:上下场][progress][pos]
        QualifyBattle m_finalBattles[2][15];  // 晋级赛战报:[stage:上下场][idx]
        StarTeam* m_3finals[2][4];       // 1,2,3名:[pos]
        QualifyBattle m_3finalBattles[2];     // 1,2,3名赛战报
    };
    class SHFinalStarStage
    {
    public:
        SHFinalStarStage(StageType type, GroupType gpId);
        ~SHFinalStarStage();
        bool enter(StarTeam* starTeam);
        void loadFromDB();
        void clear();

        void write2DB();
        bool process();
        void pushPlayer(Player* pl, UInt32 teamId, SingleHeroFighter* fgt);

        void sendStageMail(SingleHeroPlayer* spl, UInt8 idx);
        bool sendEliminary(Stream& st);
        void sendLeaderBoard(Stream& st);
        void visitFinals(SHTeamVisitor& visitor);
        inline StarTeam* getStageTeam(Player* pl)
        {
            std::map<Player*, SingleHeroPlayer*>::iterator it = m_players.find(pl);
            if(it == m_players.end())
                return NULL;

            UInt32 teamId = it->second->_teamId;
            std::map<UInt32, StarTeam*>::iterator itTeam = m_starTeam.find(teamId);
            if(itTeam == m_starTeam.end())
                return NULL;

            return itTeam->second;
        }

    private:
        std::map<Player*, SingleHeroPlayer*> m_players; // 玩家
        std::map<UInt32, StarTeam*> m_starTeam;
        std::vector<StarTeam*> m_finals;
        UInt8 m_gpId;
        UInt8 m_round;
        UInt8 m_type;
        UInt8 m_progress;
    };

    class SHBattleStageMgr
    {
    public:
        SHBattleStageMgr();
        ~SHBattleStageMgr();
        bool loadFromDB();
        void process();
        void clear();

        void init(UInt32 now, UInt8 weekday);
        bool enter(Player* player, Fighter* fgt);

        bool processSingleStage();
        bool processStrategyStage();
        bool processStarStage();
        bool processTowerStage();
        void createTeams();
        bool processStageOver();

        SHStarStage* starStage()
        {
            if(!m_starStage)
                m_starStage= new SHStarStage(e_sh_sr, e_sh_gp_sr);
            return m_starStage;
        }
        SHStarStage* starStage2()
        {
            if(!m_starStage2)
                m_starStage2 = new SHStarStage(e_sh_sr_earth, e_sh_gp_sr);
            return m_starStage2;
        }
        SHFinalStarStage* starFinalStage()
        {
            if(!m_starFinalStage)
                m_starFinalStage = new SHFinalStarStage(e_sh_sr_final, e_sh_gp_sr);
            return m_starFinalStage;
        }

        SHBattleStage* singleStage(UInt8 cls)
        {
            if(!m_singleStage[cls])
                m_singleStage[cls] = new SHBattleStage(e_sh_slsy, e_sh_gp_sl, cls);
            return m_singleStage[cls];
        }
        SHBattleStage* singleStage2(UInt8 cls)
        {
            if(!m_singleStage2[cls])
                m_singleStage2[cls] = new SHBattleStage(e_sh_sl_earth, e_sh_gp_sl, cls);
            return m_singleStage2[cls];
        }
        SHFinalBattleStage* singleFinalStage(UInt8 cls)
        {
            if(!m_singleFinalStage[cls])
                m_singleFinalStage[cls] = new SHFinalBattleStage(e_sh_sl_final, e_sh_gp_sl, cls);
            return m_singleFinalStage[cls];
        }

        SHBattleStage* strategyStage(UInt8 cls)
        {
            if(!m_strategyStage[cls])
                m_strategyStage[cls] = new SHBattleStage(e_sh_slsy, e_sh_gp_sy, cls);
            return m_strategyStage[cls];
        }
        SHBattleStage* strategyStage2(UInt8 cls)
        {
            if(!m_strategyStage2[cls])
                m_strategyStage2[cls] = new SHBattleStage(e_sh_sy_earth, e_sh_gp_sy, cls);
            return m_strategyStage2[cls];
        }
        SHFinalBattleStage* strategyFinalStage(UInt8 cls)
        {
            if(!m_strategyFinalStage[cls])
                m_strategyFinalStage[cls] = new SHFinalBattleStage(e_sh_sy_final, e_sh_gp_sy, cls);
            return m_strategyFinalStage[cls];
        }

        SHTowerStage* towerStage(UInt8 cls)
        {
            if(!m_towerStage[cls])
                m_towerStage[cls] = new SHTowerStage(e_sh_slsy, e_sh_gp_tw, cls);
            return m_towerStage[cls];
        }

        bool getActive() { return m_lvlCnt70 > SH_OPEN_COND; }
        void incActive(UInt8 v) { m_lvlCnt70 += v; }
        void sendActive(Player* pl);

        void sendProgress(Player* player);
        void sendStatus(Player* player);
        void sendSHStageInfo(Player* player, UInt8 cls, UInt8 type);  // 单职业
        void sendSYStageInfo(Player* player, UInt8 cls, UInt8 type);  // 策略
        void sendSRStageInfo(Player* player, UInt8 cls, UInt8 type);  // 明星
        void sendTowerInfo(Player* player, UInt8 cls);    // 塔
        void sendLeaderInfo(Player* player, UInt8 cls);   // 排行榜

        void sendTeamId(Player* player);

        void newCandidate(Player * player, UInt8 cls);
        void initSupportData();
        void sendSupportInfo(Player* player);
        void voteForHero(Player *player, UInt8 cls, Player *votePlayer); // 给人气投票
        void clearVoteData();
        void setTopCandidate(Player * player, UInt8 cls, UInt8 pos);
        void voteEnd();

        void towerEnd();

        void setDstProgress(UInt8 v) { if(v > e_sh_result) return; m_dstprogress = v; }
        void setNextTime();
        void reset();

        bool loadAttrExtraEquip();
        bool loadAttr2();

        void pushPlayer(SingleHeroPlayer* sp, UInt16 lvl, UInt32 turns, UInt16 lastTurns);
        void addScore(Player* pl, UInt8 cls, UInt32 score, bool writeDB = false);

        void sendLeaderBoard(Player* player);           // 总榜排名
        void sendSHLeaderBoard(Player* player);         // 单职业排名
        void sendSYLeaderBoard(Player* player);         // 策略排名
        void sendSRLeaderBoard(Player* player);         // 明星排名
        void sendSupportLeaderBoard(Player* player);    // 人气排名
        void sendTowerLeaderBoard(Player* player);      // 逆转乾坤排名

        void saveStrategyFighter(Player* pl, std::vector<UInt16>& trumps, std::vector<UInt16>& cittas);
        void showStrategyFighter(Player* pl);
 
        void onFighterClone(Player* pl, Fighter* fgt);

        inline UInt32 getPlayerCount() { return m_players.size(); }
        inline UInt8 getProgress()
        {
            if(m_progress > e_sh_result) return m_progress - SH_GP_OFFSET_B_E;
            else return m_progress;
        }
    private:
        std::map<Player*, SingleHeroPlayer*> m_players;       // 玩家
        std::map<Player*, SingleHeroFighter*> m_singlFgt;
        std::map<Player*, SingleHeroFighter*> m_strategyFgt;

        /////////////////////////////////////////////
        // 人气达人
        typedef std::set<SHCandidate*>::iterator SHCandidateIt;

        std::set<SHCandidate*> m_stars[4]; // 人气:明星
        std::map<Player*, SHCandidateIt> m_candidateMap;      // 人气:候选人
        std::map<Player*, SHSupportPlayer*> m_support;        // 人气:投票
        Player* m_topCandidate[4][3];
        Player* m_topPopular[4][3];
        // 人气达人
        /////////////////////////////////////////////

        struct ScoreGreater
        {
             bool operator()(const SHPlayerScore* sp1, const SHPlayerScore* sp2) const
             {
                 return (sp1->_score > sp2->_score);
             }
        };

        typedef std::multiset<SHPlayerScore*, ScoreGreater> SHScoreSet;
        typedef SHScoreSet::iterator SHScoreSetIt;
        typedef std::map<Player*, SHScoreSetIt> SHScoreMap;
        SHScoreSet m_scoreSet[4];
        SHScoreMap m_scoreMap[4];

        SHStarStage* m_starStage;
        SHStarStage* m_starStage2;
        SHFinalStarStage* m_starFinalStage;

        SHBattleStage* m_singleStage[4];
        SHBattleStage* m_singleStage2[4];
        SHFinalBattleStage* m_singleFinalStage[4];

        SHBattleStage* m_strategyStage[4];
        SHBattleStage* m_strategyStage2[4];
        SHFinalBattleStage* m_strategyFinalStage[4];

        SHTowerStage* m_towerStage[4];

        UInt8 m_dstprogress;

        UInt8 m_progress;  // 阶段：
        UInt32 m_nextTime; // 下次擂台开始时间
        UInt32 m_session;  // 届数
        UInt32 m_lvlCnt70;
    };


    extern SHBattleStageMgr shStageMgr;
}

#endif

