#ifndef _RACEBATTLE_H_
#define _RACEBATTLE_H_
#include "Common/Singleton.h"

namespace GObject
{
#define RACEBATTLE_LOCATION 0xFFFF

struct TSort
{
    Player* player;
    UInt16 total;
    UInt32 time;
    TSort() : player(NULL), total(0), time(0) {}
};

struct t_sort
{
    bool operator()(const TSort& a, const TSort& b) const { return a.total > b.total || (a.total == b.total && a.time < b.time); }
};

typedef std::set<TSort, t_sort> RBSortType;


class RaceBattle : public Singleton<RaceBattle>
{
    public:
        RaceBattle() {}
        ~RaceBattle() {}
        void raceBattleCheck(UInt32 time);
        UInt16 getLocation() { return RACEBATTLE_LOCATION; }
        void enterPos(Player* pl, UInt8 pos);
        void autoBattle(Player* pl);
        void cancelBattle(Player* pl);
        void freshContinueWinRank(Player* pl);
        void getAward(Player* pl);
        //void readBattleReport(Player* pl, UInt32 reportId);
        bool requestMatch(Player* pl);
        bool isStart();
        void makeStarInfo(Stream& st, Player* pl, UInt8 level);
        void sendContinueWinSort(Player* player, UInt8 page);
        void sendBattleInfo(Player* pl);
        void sendMatchPlayer(Player* pl, Player* matchPlayer);
        UInt8 attackPlayer(Player* pl, Player* matchPlayer);
        void awardLevelRank();
        void awardContinueWinRank();
        void awardLevelRankOne(Player* pl, UInt8 rank);
        void awardContinueWinRankOne(Player* pl, UInt8 num);
        void eraseLevelStarSort(Player* pl, UInt8 level);
        void insertLevelStarSort(Player* pl, UInt8 level);
        void exitRB(Player* pl);
        void attackLevelPlayer(Player* pl, UInt64 defenderId);
        void attackContinueWinPlayer(Player* pl, UInt64 defenderId);
        void insertContinueWinSort(Player* pl);
        void eraseContinueWinSort(Player* pl);
        void sendOwnerInfo(Player* pl);
        void braodCancelContinueWin(Player* pl, Player* p2);

    private:
        static UInt8 _status; //0（未开启），1-3（17：50~20：00），4（20：00~20：30）
        static RBSortType _levelStarSort[5]; //每层星级数排名
        static RBSortType _contineWinSort;
};

#define raceBattle RaceBattle::Instance()

} //namespace GObject

#endif

