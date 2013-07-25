
#ifndef NEWHEROISLAND_H_
#define NEWHEROISLAND_H_

#include "Config.h"
#include "Player.h"
#include "Common/Stream.h"
#include "Server/Cfg.h"
#include "Script/GameActionLua.h"

namespace GObject
{

#define NEWHERO_ISLAND_SPOTS 5
#define DEFAULT_BUFID (static_cast<UInt8>(-1))
#define NEWHERO_ISLAND_JOIN   0
#define NEWHERO_ISLAND_PAIR   1
#define NEWHERO_ISLAND_FIGHT  2
#define NEWHERO_ISLAND_MOVE   3
#define NEWHERO_ISLAND_ESCAPE 4

struct joinByePlayer
{
    joinByePlayer(): player(NULL), status(0xFF) {}

    Player * player;
    UInt8 status;
};

struct NewHeroIsland_Skill
{
    NewHeroIsland_Skill() : id(0), cd(0), last(0), lastcd(0), bufid(DEFAULT_BUFID), attr(NULL) {}

    UInt8 id;
    UInt32 cd;
    UInt16 last;
    UInt32 lastcd;
    UInt8 bufid;
    //技能加成属性
    GData::AttrExtra attrs;
    GData::AttrExtra* attr;

    void reset()
    {
        bufid = DEFAULT_BUFID;
        cd = 0;
        attr = NULL;
    }
};

struct NHIPlayerData
{
    NHIPlayerData()
        : player(NULL), stage(1), type(0xFF), spot(0), movecd(0), status(0xFF), rndEvent(0)
    { memset(score, 0, sizeof(score)); }

    Player * player;
    UInt8 stage;
    UInt8 type;     // 0-无,1-天,2-地,3-人
    UInt8 spot;
    UInt32 movecd;
    UInt16 score[3];
    UInt8 status;   //0-进入 1-战斗 2-移动 3-逃离
    UInt8 rndEvent;
    NewHeroIsland_Skill skills[5];

    void reset(bool running)
    {
        if (player)
        {
            if (player->getBuffData(PLAYER_BUFF_HIMOVE))
                player->setBuffData(PLAYER_BUFF_HIMOVE, 0, false);
        }

        if (!running)
        {
            memset(score, 0, sizeof(score));
        }

        movecd = 0;
        for (UInt8 i = 0; i < 5; ++i)
        {
            skills[i].reset();
        }
    }

};

struct hI_score
{
    bool operator()(const NHIPlayerData* const & pd1, const NHIPlayerData* const & pd2) const
    {
        return pd1->score[pd1->stage==0 ? 0 : pd1->stage-1] > pd2->score[pd2->stage==0 ? 0 : pd2->stage-1];
    }
};
struct hI_scoreAll
{
    bool operator()(const NHIPlayerData* const & pd1, const NHIPlayerData* const & pd2) const
    {
        UInt32 score1 = 0, score2 = 0;
        for(UInt8 i = 0; i < 3; ++ i)
        {
            score1 += pd1->score[i];
            score2 += pd2->score[i];
        }
        return score1 > score2;
    }
};

struct pairsNHIPlayerData
{
    pairsNHIPlayerData() : nhipd1(NULL), nhipd2(NULL) {}

    NHIPlayerData * nhipd1;
    NHIPlayerData * nhipd2;
};

typedef std::multiset<NHIPlayerData*, hI_score> NHISort;
typedef std::multiset<NHIPlayerData*, hI_scoreAll> NHISortAll;

class NewHeroIsland
{
public:
    NewHeroIsland() : _tickTime(0), _expTime(0), _prepareStep(0),
    _prepareTime(0), _startTime(0), _endTime(0), _status(0), _stage(1)
    {
        //initSkillAttr();
        memset(_types, 0, sizeof(_types));
        memset(_nplayers, 0, sizeof(_nplayers));
        memset(_isAward, false, sizeof(_isAward));
        memset(_scores, 0, sizeof(_scores));
        for (UInt8 i = 0; i < NEWHERO_ISLAND_SPOTS-1; ++ i)
            _noOwned.push_back(i);
    }

    ~NewHeroIsland() { reset(); }

private:
    NHISortAll _sorts;
    //std::vector<NHIPlayerData *> _players[NEWHERO_ISLAND_SPOTS];
    NHISort _players[NEWHERO_ISLAND_SPOTS];
    std::map<Player *, NHIPlayerData *> _allPlayer;
    std::vector<pairsNHIPlayerData> _pairPlayer[NEWHERO_ISLAND_SPOTS];

    std::map<UInt8, UInt8> _wasOwned;  //被占据的据点
    std::vector<UInt8> _noOwned;    //未被占据的据点

    UInt16 _nplayers[NEWHERO_ISLAND_SPOTS][3]; //每个据点的人数
    UInt32 _types[3];   //天地人的人数
    bool _isAward[3];   //每阶段是否结算奖励
    UInt32 _scores[NEWHERO_ISLAND_SPOTS][3];    //每个据点上每阶段天地人的积分
    static NewHeroIsland_Skill _skills[5];

    UInt32 _tickTime;
    UInt32 _expTime;
    UInt8 _prepareStep;
    UInt32 _prepareTime;
    UInt32 _startTime;
    UInt32 _endTime;
    UInt8 _status;
    UInt8 _stage;   //分阶段0/1/2
public:
    inline void setStatus(UInt8 status)
    {
        _status = status;
        sendDaily(NULL);
    }
    static void Init();
    void useSkill(Player * player, UInt8 skillid, UInt8 useGold);
    void clearBuff(NHIPlayerData* pd, bool = true);
    void playerEnter(Player* player);
    void enterPairPlayer(NHIPlayerData * pd);
    bool enter(NHIPlayerData* pd, UInt8 spot, bool movecd = true);
    void playerLeave(Player* player);
    bool moveTo(Player* player, UInt8 to, bool movecd = true, bool force = false);
    UInt8 getIdentity(Player* player, bool = false);
    NHIPlayerData* findPlayer(Player* player);
    void process(UInt32 now);
    void makePairPlayers();
    void handleBattle();
    void addSpotScore(UInt8 spot, UInt8 type, UInt16 score, Player* player);
    void insertNHIPlayerData(NHIPlayerData*);
    void checkOccupySpot();
    bool checkSpecialWin(NHIPlayerData *, NHIPlayerData *);
    void checkAddExp(UInt32 now);
    void checkSkillTime();
    bool checkSettleAccounts(UInt32 now);
    void sendOccupySpotAward(NHIPlayerData * pd, UInt8 spot);
    void randomEvent();
    void end(UInt32 now);
    void reset();
    void calcNext(UInt32 now);
    void rankReward();
    void rankReward1();
    void broadcastTV(UInt32 now);
    void sendDaily(Player* player);

    void broadcastRank(Player* player = NULL);
    void broadcast(Stream& st);
    void sendAllPlayerInfo();
    void sendPairPlayerInfo(NHIPlayerData *, NHIPlayerData *);
    void updateSpotPlayers(UInt8 spot);
    void playerInfo(Player *);
    void updatePlayerInfo(UInt32 now);
    void sendSkillInfo(NHIPlayerData *);
    void sendWinerInfo(Player * player1, Player * player2, UInt16 score, UInt8 spot);
};

extern NewHeroIsland newHeroIsland;

}

#endif // NEWHEROISLAND_H_

