
#ifndef HEROISLAND_H_
#define HEROISLAND_H_

#include "Config.h"
#include "Player.h"
#include "Common/Stream.h"
#include "Server/Cfg.h"
#include "Script/GameActionLua.h"

namespace GObject
{

#define HERO_ISLAND_SPOTS 5
#define HERO_ISLANG_PAGESZ 12
#define DEFAULT_BUFID (static_cast<UInt8>(-1))
#define DEFAULT_CD (static_cast<UInt32>(-1))

struct Task
{
    Task(UInt8 type, UInt8 status) : type(type), status(status) {}
    UInt8 type;
    UInt8 status;
};

struct Awards
{
    Awards(UInt32 id, UInt32 num, UInt32 prob) : id(id), num(num), prob(prob) {}
    UInt32 id;
    UInt32 num;
    UInt32 prob;
};

struct Skill
{
    Skill() : last(2*60), lastcd(0), bufid(DEFAULT_BUFID), cd(0), incd(false), attr(NULL) {}

    void reset()
    {
        last = 2*60;
        lastcd = 0;
        bufid = DEFAULT_BUFID;
        cd = 0;
        incd = false;
        attr = NULL;
    }

    UInt32 last;
    UInt32 lastcd;
    UInt8 bufid;
    UInt32 cd;
    bool incd;
    GData::AttrExtra* attr;
};

struct HIPlayerData
{
    HIPlayerData()
        : player(NULL), type(0), spot(0), movecd(0),
        fightcd(0), injuredcd(static_cast<UInt32>(-1)), expcd(0), straight(0), round(0),
        score(0), lasttype(0xff), attrcd(static_cast<UInt32>(-1)), bufid(DEFAULT_BUFID),
        attr(NULL), awardgot(0), inrank(0), tasks(0)
    {
    }

    void reset(bool running)
    {
        if (player)
        {
            if (player->getBuffData(PLAYER_BUFF_HIFIGHT))
                player->setBuffData(PLAYER_BUFF_HIFIGHT, 0, false);
            if (player->getBuffData(PLAYER_BUFF_HIMOVE))
                player->setBuffData(PLAYER_BUFF_HIMOVE, 0, false);
            if (player->getBuffData(PLAYER_BUFF_HIWEAK))
                player->setBuffData(PLAYER_BUFF_HIWEAK, 0, false);
        }

        if (!running)
        {
            straight = 0;
            round = 0;
            score = 0;
            inrank = 0;
            tasks = 0;
            awardgot = 0;
            lasttype = 0;
            compass.clear();
        }

        movecd = 0;
        fightcd = 0;
        injuredcd = 0;
        expcd = 0;
        //attrcd = static_cast<UInt32>(-1);
        //bufid = DEFAULT_BUFID;
        attr = NULL;
        for (UInt8 i = 0; i < 5; ++i)
            skills[i].reset();
    }

    Player* player;
    UInt8 type; // 0-无,1-天,2-地,3-人
    UInt8 spot;
    UInt32 movecd;
    UInt32 fightcd;
    UInt32 injuredcd;
    UInt32 expcd;
    UInt8 straight;
    UInt8 round;
    UInt16 score;
    UInt8 lasttype;
    UInt32 attrcd; // 奇珍异兽效果持续时间
    UInt8 bufid;
    GData::AttrExtra* attr;
    UInt8 awardgot; // 0-没有奖励,1-绿 2-蓝 3-紫 4-橙,0xFF-已领取
    UInt8 inrank; // 0-不在,>=1-在
    std::vector<Task> compass; // 击杀任务
    UInt16 tasks; // 总完成任务数
    Skill skills[5];
};

struct RareAnimals
{
    RareAnimals() : id(0), last(0), cdlong(0), cd(0), bufid(DEFAULT_BUFID) {}

    UInt16 id; // NPC ID
    GData::AttrExtra attr; // 攻击成功后效果加成
    UInt32 last; // 效果持续时间
    UInt32 cdlong; // 攻击后冷却时间，全局
    UInt32 cd; // 冷却结束时间
    UInt8 bufid; // 
};

struct lt_score
{
    bool operator()(const HIPlayerData* const & pd1, const HIPlayerData* const & pd2) const
    {
        return pd1->score < pd2->score;
    }
};

typedef std::multiset<HIPlayerData*, lt_score> SortType;

class HeroIsland
{
public:
    HeroIsland() : _running(false), _notifyTime(0), _prepareStep(0),
    _prepareTime(0), _startTime(0), _endTime(0), _count(0), _expTime(0)
    {
        _types[0] = _types[1] = _types[2] = 0;
        _expfactor[0] = _expfactor[1] = _expfactor[2] = _expfactor[3] = 2.0;
        _nplayers[0] = _nplayers[1] = _nplayers[2] = _nplayers[3] = _nplayers[4] = 0;
        initSkillAttr();
    }

    ~HeroIsland();

public:
    static void clearAllHICfg();
    static void setRareAnimals(UInt8 spot, UInt32 npcid, Table attr, UInt32 last, UInt32 cd);
    static void addHIAwardsCfg(UInt8 type, UInt32 id, UInt32 num, UInt32 prob);
    static void addRankAwards(UInt32 prestige);
    static bool isRareAnimal(UInt32 npcid);
    static bool initSkillAttr();
private:
    static std::vector<RareAnimals> _animals[HERO_ISLAND_SPOTS];
    static std::vector<Awards> _awards[4];
    static std::vector<UInt32> _prestige;
    static GData::AttrExtra _skillattr[5];

public:
    void process(UInt32 now);
    void applayPlayers();
    void applayRareAnimals();
    void rankReward();
    void restart(UInt32 now);
    void broadcastTV(UInt32 now);
    void calcNext(UInt32 now);
    void end(UInt32 now);
    void reset();

    UInt8 getIdentity(Player* player, bool = false);
    bool enter(Player* player, UInt8 type, UInt8 spot, bool movecd = true);
    bool enter(HIPlayerData* pd, UInt8 type, UInt8 spot, bool movecd = true);
    HIPlayerData* leave(Player* player, UInt8 spot);
    HIPlayerData* leave(HIPlayerData* pd, UInt8 spot, UInt16 pos);
    void listPlayers(Player* player, UInt8 spot, UInt16 start, UInt8 pagesize);
    bool moveTo(Player* player, UInt8 to, bool = true);
    bool attack(Player* player, UInt8 type, UInt64 id);
    bool useSkill(Player* player, UInt8 skillid);
    bool getAward(Player* player, UInt8 id, UInt8 type);
    void clearBuff(UInt8 type, HIPlayerData* pd, UInt32 now, UInt8 skillid = 0);

    void playerInfo(Player* player);
    void playerEnter(Player* player);
    void playerLeave(Player* player);

    HIPlayerData* findPlayer(Player* player, UInt8& spot, UInt16& pos);
    HIPlayerData* findPlayer(UInt64 id, UInt8& spot, UInt16& pos);
    RareAnimals& findRareAnimal(UInt32 id, UInt8 spot);

    void startCompass(Player* player);
    void commitCompass(Player* player);

    void sendSpot(HIPlayerData* pd, UInt8 spot);
    void sendPlayers(HIPlayerData* pd, UInt8 spot, UInt16 start, UInt8 pagesize);
    void sendRareAnimals(HIPlayerData* pd, UInt8 spot);
    void sendSkills(HIPlayerData* pd);
    void broadcast(HIPlayerData* pd, UInt8 spot, UInt8 type);
    void broadcast(Stream& st, UInt8 spot, Player* = NULL);
    void broadcast(Stream& st);
    void notifyCount(UInt32 now);
    void expFactor(UInt32 now);

    void listRank(Player* player, UInt16 start, UInt8 pagesize);
    void saveAtoCfg(Player* player, const std::string& cfg);
    void sendAtoCfg(Player* player);

    inline void setRunning(bool r) { _running = r; }
    inline bool isRunning() { return _running; }

private:
    SortType _sorts;
    std::vector<HIPlayerData*> _players[HERO_ISLAND_SPOTS];
    UInt16 _nplayers[HERO_ISLAND_SPOTS];
    UInt32 _types[3];

private:
    bool _running;
    UInt32 _notifyTime;
    UInt8 _prepareStep;
    UInt32 _prepareTime;
    UInt32 _startTime;
    UInt32 _endTime;
    UInt8 _count;
    float _expfactor[4];
    UInt32 _expTime;
};

extern HeroIsland heroIsland;

}

#endif // HEROISLAND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

