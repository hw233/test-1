
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

struct HIPlayerData
{
    HIPlayerData()
        : player(NULL), type(0), spot(0), movecd(0),
        fightcd(0), injuredcd(static_cast<UInt32>(-1)), expcd(0), straight(0),
        score(0), lasttype(0xff), attrcd(static_cast<UInt32>(-1)), attr(NULL), awardgot(0), inrank(0)
    {
    }

    Player* player;
    UInt8 type; // 0-无,1-天,2-地,3-人
    UInt8 spot;
    UInt32 movecd;
    UInt32 fightcd;
    UInt32 injuredcd;
    UInt32 expcd;
    UInt8 straight;
    UInt16 score;
    UInt8 lasttype;
    UInt32 attrcd; // 奇珍异兽效果持续时间
    GData::AttrExtra* attr;
    UInt8 awardgot; // 0-没有奖励,1-绿 2-蓝 3-紫 4-橙,0xFF-已领取
    UInt8 inrank; // 0-不在,>=1-在
    std::vector<Task> compass; // 击杀任务
};

struct RareAnimals
{
    RareAnimals() : id(0), last(0), cdlong(0), cd(0) {}

    UInt16 id; // NPC ID
    GData::AttrExtra attr; // 攻击成功后效果加成
    UInt32 last; // 效果持续时间
    UInt32 cdlong; // 攻击后冷却时间，全局
    UInt32 cd; // 冷却结束时间
};

struct lt_score
{
    bool operator()(const HIPlayerData* const & pd1, const HIPlayerData* const & pd2) const
    {
        return pd1->score - pd2->score < 0;
    }
};

typedef std::multiset<HIPlayerData*, lt_score> SortType;

class HeroIsland
{
public:
    HeroIsland() : _running(false), _prepareStep(0), _prepareTime(0), _startTime(0), _endTime(0)
    {
        _types[0] = _types[1] = _types[2] = 0;
    }

    ~HeroIsland() {}

public:
    static void setRareAnimals(UInt8 spot, UInt32 npcid, Table attr, UInt32 last, UInt32 cd);
    static void addHIAwardsCfg(UInt8 type, UInt32 id, UInt32 num, UInt32 prob);
    static void addRankAwards(UInt32 prestige);
    static bool isRareAnimal(UInt32 npcid);
private:
    static std::vector<RareAnimals> _animals[HERO_ISLAND_SPOTS];
    static std::vector<Awards> _awards[4];
    static std::vector<UInt32> _prestige;

public:
    void process(UInt32 now);
    void applayPlayers();
    void applayRareAnimals();
    void rankReward();
    void restart(UInt32 now);
    void broadcastTV(UInt32 now);
    void end();

    UInt8 getIdentity(Player* player, bool = false);
    bool enter(Player* player, UInt8 type, UInt8 spot, bool movecd = true);
    bool enter(HIPlayerData* pd, UInt8 type, UInt8 spot, bool movecd = true);
    HIPlayerData* leave(Player* player, UInt8 spot);
    HIPlayerData* leave(HIPlayerData* pd, UInt8 spot, UInt8 pos);
    void listPlayers(Player* player, UInt8 spot, UInt16 start, UInt8 pagesize);
    bool moveTo(Player* player, UInt8 to, bool = true);
    bool attack(Player* player, UInt8 type, UInt64 id);
    bool useSkill(Player* player, UInt16 skillid);
    bool getAward(Player* player, UInt8 id, UInt8 type);

    void playerInfo(Player* player);
    void playerEnter(Player* player);
    void playerLeave(Player* player);

    HIPlayerData* findPlayer(Player* player, UInt8& spot, UInt8& pos);
    HIPlayerData* findPlayer(UInt64 id, UInt8& spot, UInt8& pos);
    RareAnimals& findRareAnimal(UInt32 id, UInt8 spot);

    void startCompass(Player* player);
    void stopCompass(Player* player);
    void commitCompass(Player* player);

    void sendSpot(HIPlayerData* pd, UInt8 spot);
    void sendPlayers(HIPlayerData* pd, UInt8 spot, UInt16 start, UInt8 pagesize);
    void sendRareAnimals(HIPlayerData* pd, UInt8 spot);
    void sendSkills(HIPlayerData* pd);
    void broadcast(HIPlayerData* pd, UInt8 spot, UInt8 type);
    void broadcast(Stream& st, UInt8 spot, Player* = NULL);
    void broadcast(Stream& st);

    void listRank(Player* player, UInt16 start, UInt8 pagesize);

    inline void setRunning(bool r) { _running = r; }
    inline bool isRunning() { return _running; }

public:
    SortType _sorts;
    std::vector<HIPlayerData*> _players[HERO_ISLAND_SPOTS];
    UInt32 _types[3];

private:
    bool _running;
    UInt8 _prepareStep;
    UInt32 _prepareTime;
    UInt32 _startTime;
    UInt32 _endTime;
};

extern HeroIsland heroIsland;

}

#endif // HEROISLAND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

