#ifndef RECHARGE_TEMPLATE
#define RECHARGE_TEMPLATE

#include <set>
#include <map>
#include "Script/Script.h"
#include "Mail.h"
#include "Player.h"

using namespace std;
namespace GObject
{
    struct RechargeItem
    {
        UInt32 id;
        UInt32 score;
        int    num;
        RechargeItem()
        {
            memset(this, 0, sizeof(RechargeItem));
        }
        RechargeItem(UInt32 i, UInt32 s, int n)
        {
            id = i;
            score=s;
            num=n;
        }
        RechargeItem& operator = (RechargeItem& other)
        {
            id = other.id;
            score = other.score;
            num = other.num;
            return *this;
        }
        bool operator == (RechargeItem& other)
        {
            return id == other.id;
        }
        bool operator == (UInt32 otherId)
        {
            return id == otherId;
        }
        bool operator < (const RechargeItem& other) const
        {
            return id < other.id;
        }
        bool operator () (const UInt32 a, const UInt32 b) 
        {
            return a < b;
        }
    };
    class RechargeTmpl
    {
    public:
        RechargeTmpl();
        static RechargeTmpl& instance()
        {
            static RechargeTmpl s;
            return s;
        }
        inline void setFilename(const char * fn) { _filename = fn; }
        void Init();
        void loadPlayersItem();
        void load(const char * = NULL);
        void clear();
        void addItem(UInt32 id, UInt32 score, int num);
        void addScore(Player* pl, UInt32 oldRecharge, UInt32 newRecharge);
        void sendStreamInfo(Player* pl);
        void sendScoreInfo(Player* pl);

        UInt8 getItem(Player* pl, UInt32 itemId, int& n);
    private:
        int findNum(Player* pl, UInt32 itemId, int maxNum);
        const RechargeItem* findItem(UInt32 id);
 
        std::string _filename;
        std::set<RechargeItem> _items; //所有奖励物品
        std::map<UInt64, std::map<UInt32, int> > _playerItems; //map<playerid,map<itemid,num> >

        bool m_isCleared;
    };

    extern RechargeTmpl rechargeTmpl;
}


namespace Script
{
    class RechargeTmplScript:
        public Script
    {
    public:
        RechargeTmplScript(GObject::RechargeTmpl * rechargetmpl);
    };

}

#endif  //RECHARGE_TEMPLATE
