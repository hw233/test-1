#ifndef _LEADERBOARD_H_
#define _LEADERBOARD_H_

#include "Common/Stream.h"
#include "Common/AtomicVal.h"
#include "Common/Mutex.h"
#include "Common/BinaryReader.h"

namespace GObject
{

class Player;
class FairyPet;

struct LeaderboardTowndown
{
    UInt64 id;
	std::string name;
    UInt16 level;
    UInt32 time;
};

struct LeaderboardClanCopy
{
    UInt64 id;
	std::string name;
    UInt32 level;
    UInt64 time;
};

struct LeaderboardLingbao
{
    UInt64 id;
    std::string name;
    UInt32 itemId;
    UInt8 tongling;
    UInt8 lbColor;
    std::string types;
    std::string values;
    std::string skills;
    std::string factors;
    UInt32 battlePoint;
};

struct RankingInfoList
{
    UInt64 id;
	std::string name;
    UInt32 country;
    std::string clanName;
    UInt32 roleLevel;
    UInt64 value;
};
struct TownRankingInfoList
{
    UInt64 id;
	std::string name;
    UInt32 country;
    std::string clanName;
    UInt32 roleLevel;
    UInt32 value; //锁妖塔层数
    UInt64 reachTime;
};
struct ClanCopyRankingInfoList
{
	std::string name;
    std::string leaderName;
    UInt32 level;
    UInt32 memberCount;
    UInt32 country;
    UInt32 value; //帮派副本层数 
    UInt64 reachTime;
};
struct ClanBattleRankingInfoList
{
	std::string name;
    std::string leaderName;
    UInt32 level;
    UInt32 memberCount;
    UInt32 country;
    UInt32 value; //帮派积分 
};

struct LingbaoInfoList
{
    UInt64 id;
    UInt32 equipId;
    std::string  name;
    UInt8  pf;
    UInt8  country;
    UInt32 battlePoint;
    UInt32 itemId;
    UInt8  tongling;
    UInt8  lbColor;
    UInt8  type[4];
    UInt16  value[4];
    UInt16  skill[2];
    UInt16  factor[2];
    LingbaoInfoList()
        :id(0), equipId(0), pf(0), country(0), battlePoint(0), itemId(0), tongling(0), lbColor(0)
    {
        memset(type, 0, sizeof(type));
        memset(value, 0, sizeof(value));
        memset(skill, 0, sizeof(skill));
        memset(factor, 0, sizeof(factor));
    }
};

struct PetNeidanAttr
{
    UInt16 itemId;
    UInt8  lv;
    UInt16 skill;
    UInt16 gems[4]; //精魄附加属性
};

struct PetInfoList
{
    UInt64 id;
    std::string  name;
    UInt8 pf;
    UInt8 country;
    UInt32 battlePoint;
    UInt16 petId;
    UInt16 petLev;
    UInt16 gengu;
    PetNeidanAttr neidan[3];
    std::map<UInt8, UInt8> sanhun;

    PetInfoList() : id(0), pf(0), country(0), battlePoint(0), petId(0), petLev(0), gengu(0)
    {
        memset(neidan, 0, sizeof(neidan));
    }
};

//全服充值排行
struct AllServersRecharge
{
    Player * player;
    std::string name;
    UInt32 total;
    UInt32 rank;

    AllServersRecharge() : player(NULL), total(0), rank(0) {}
};

class Leaderboard
{
public:
	Leaderboard(): _id(0), _maxLevel(100), m_sorting(false) {}
	void update();
	bool hasUpdate(UInt32);
	bool getPacket(UInt8, Stream&, Player* pl=NULL);
	void sendOwnRank(Player *, UInt32);
    void newDrawingGame(UInt32 nextday); //新人冲级赛
	inline UInt8 getMaxLevel() { return _maxLevel; }

    struct bpGreater
    {
        bool operator() (const LingbaoInfoList& first, const LingbaoInfoList& second)
        {
            if (first.battlePoint != second.battlePoint)
                return first.battlePoint > second.battlePoint;
            return first.equipId < second.equipId;
        }
    };
    typedef std::set<LingbaoInfoList, bpGreater> LingbaoInfoSet;

    struct petGreater
    {
        bool operator() (const PetInfoList& first, const PetInfoList& second)
        {
            if (first.battlePoint != second.battlePoint)
                return first.battlePoint > second.battlePoint;
            return first.id < second.id;
        }
    };
    typedef std::set<PetInfoList, petGreater> PetInfoSet;
    typedef PetInfoSet::iterator PetInfoSetIt;
    typedef std::multimap<Player*, PetInfoSetIt> PetInfoSetMap;



    std::vector<RankingInfoList>* getLevelList() {return &_level;};
    std::vector<RankingInfoList>* getAthleticsList() {return &_athletics;};
    std::vector<RankingInfoList>* getAchievementList() {return &_achievement;};
    std::vector<TownRankingInfoList>& getTownList() {return _town;};
    std::vector<ClanCopyRankingInfoList>& getClanCopyList() {return _clanCopyInfo;};
    std::vector<ClanBattleRankingInfoList>& getClanBattleList() {return _clanBattleInfo;};
    LingbaoInfoSet& getLingbaoSet() { return _lingbaoInfoSet; }
    std::vector<RankingInfoList>* getPopularityList() { return &_popularityList; }
    const std::vector<LeaderboardTowndown>& getTowndown()
    {
        FastMutex::ScopedLock lk(_tmutex);
        return _towndown;
    };

    const std::vector<LeaderboardClanCopy>& getClanCopy()
    {
        FastMutex::ScopedLock lk(_cmutex);
        return _clancopy;
    };

    const std::vector<RankingInfoList>& getLevel()
    {
        FastMutex::ScopedLock lk(_lmutex);
        return _level;
    };
    void begin() { m_sorting = true; }
    void end() { m_sorting = false; }
    bool isSorting() const { return m_sorting; }
    void buildBattlePacket();
    void buildPacketForLingbao(Stream& st, UInt8 t, bool merge = true);
    int getMyRank(Player* pl, UInt8 type , bool setLock = true);

    void pushLingbaoInfo(LingbaoInfoList lingbaoInfo);
    void eraseLingbaoInfo(LingbaoInfoList lingbaoInfo);

    PetInfoSet& getPetSet() { return _petInfoSet; }
    void pushPetInfo(FairyPet* pet);
    void erasePetInfo(FairyPet* pet);
    void buildPacketForPet();

    //全服充值排名活动
    void readRechargeRank100(BinaryReader&);
    void readRechargeSelf(BinaryReader&);
    void sendMyRechargeRank(Player *);
    void sendRechargeRank100(Player *, UInt8, UInt8);
    void giveRechargeRankAward();
    void sendGoldLvlAward(BinaryReader&);
private:
	void doUpdate();
    void makeRankStream(Stream* st, UInt8 type, Player* pl);
    void makeRankAndValueStream(Stream* st, UInt8 type, Player* pl, UInt32 value);

	Stream _levelStream;
	Stream _moneyStream;
	Stream _achievementStream;
	Stream _clanStream;
	Stream _clanCopyStream;
    Stream _lingbaoStream;
    Stream _battleStream;
    Stream _petStream;
	UInt32 _id;
	UInt8 _maxLevel;

	std::map<UInt64, UInt16> _levelRankWorld;
	std::map<UInt64, UInt16> _levelRankCountry[2];
	std::map<UInt64, UInt16> _moneyRankWorld;
	std::map<UInt64, UInt16> _moneyRankCountry[2];
	std::map<UInt64, UInt16> _achievementRankWorld;
	std::map<UInt64, UInt16> _achievementRankCountry[2];
	std::map<UInt32, UInt16> _clanRankWorld;
	std::map<UInt32, UInt16> _clanRankCountry[2];
    std::vector<UInt64> _levelRankWorld10;  //世界等级前十名
    std::multimap<int, Player*, std::greater<int> > _battleRankWorld;
    std::multimap<UInt64, Player*, std::greater<UInt64> > _expRankWorld;
    FastMutex _tmutex;
    std::vector<LeaderboardTowndown> _towndown;
    FastMutex _cmutex;
    std::vector<LeaderboardClanCopy> _clancopy;
    FastMutex _lmutex;
    std::vector<RankingInfoList> _level;
    std::vector<RankingInfoList> _athletics;
    std::vector<RankingInfoList> _achievement;
    std::vector<TownRankingInfoList> _town;
    std::vector<ClanCopyRankingInfoList> _clanCopyInfo;
    std::vector<ClanBattleRankingInfoList> _clanBattleInfo;
    LingbaoInfoSet _lingbaoInfoSet;
    FastMutex _lbMutex;
    std::vector<RankingInfoList> _popularityList;
    PetInfoSet _petInfoSet;
    PetInfoSetMap _petInfoSetMap;

    AtomicVal<bool> m_sorting;

    std::map<UInt64, int> _playerLevelRank;
    std::map<UInt64, int> _playerAthleticsRank;
    std::map<UInt64, int> _playerClanRank;
    std::map<UInt64, int> _playerBattleRank;
    std::map<UInt64, int> _playerClanCopyRank;
    std::map<UInt64, int> _lingbaoRank;
    std::map<UInt64, int> _playerPopularityRank;
    std::map<UInt64, int> _petRank;
    FastMutex _opMutex;
    FastMutex _petMutex;

    std::map<UInt64, AllServersRecharge> _rechargeSelf;
    std::vector<AllServersRecharge> _rechargeRank100;
};

extern Leaderboard leaderboard;

}

#endif // _LEADERBOARD_H_
