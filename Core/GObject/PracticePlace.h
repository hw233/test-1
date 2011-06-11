
#ifndef PRACTICEPLACE_H_
#define PRACTICEPLACE_H_

#include "Common/Singleton.h"
#include "Common/Mutex.h"
#include <map>
#include <vector>

#define PPLACE_MAX 7

namespace GObject
{

class Player;

struct PPlace
{
    UInt32 ownerid;
    UInt32 protid;
    UInt16 maxslot;
    UInt16 protmoney;
    UInt16 slotmoney;
    UInt8 open;
};

struct PracticeData
{
    UInt8 type;         // 修炼类型 1-8小时, 2-24小时
    UInt32 price;       // 银币数量
    UInt32 traintime;   // 修炼分钟数
    UInt32 checktime;   // 修炼分钟数
    UInt32 trainend;    // 修炼结束时间
    UInt8 prot;         // 是否申请保护 0-没有任何保护, 1-护法弟子保护, 2-仙府禁法
};

struct PlayerPracticeData
{
    PlayerPracticeData(){}
    ~PlayerPracticeData(){ fighters.clear(); }

    PracticeData data;
    std::vector<UInt32> fighters;
};

struct PlaceCD
{
    size_t idx;
    UInt32 cdend;
};

struct PlaceData
{
    PlaceData()
    {
        bzero(&place, sizeof(place));
    }
    ~PlaceData()
    {
        players.clear();
    }

    PPlace place;
    std::map<Player*, PlaceCD> players;
    std::vector<PlayerPracticeData>  playerdata;
};

class PracticePlace : public Singleton<PracticePlace>
{
public:
    PracticePlace() { }
    ~PracticePlace() { }

    // 付费
    bool pay(Player* pl, UInt8 place, UInt16 slot, UInt8 type, UInt8 time, UInt8 prot);

    // 修炼
    bool sitdown(Player* pl, UInt8 place, UInt16 slot, UInt32* fgtid, size_t size);
    // 退出修炼
    bool standup(Player* pl, UInt8 place, UInt16 slot, UInt32* fgtid, size_t size);

    // 取得修炼点信息
    void getPlaceInfo(Player* pl, UInt8 place);
    // 取得某一修为挂机点位置
    void getList(Player* pl, UInt8 place, UInt16 pageno);

    // 更换护法弟子
    bool replaceProtecter(Player* pl, UInt8 place, UInt64 protid);
    // 设置挂机收费
    bool setCharges(Player* pl, UInt8 place, UInt16 money);
    // 设置保护收费
    bool setProtCharges(Player* pl, UInt8 place, UInt16 money);

    // 挑战
    bool doChallenge(Player* pl, UInt8 place, UInt16 idx);

    // 增加修练空间
    bool addSlot(Player* pl, UInt8 place, UInt8 num);

    // 增加一个修炼点
    bool addPlace(PPlace& place, UInt8 idx);

private:
    Mutex m_lock[PPLACE_MAX];
    PlaceData m_places[PPLACE_MAX];
};

#define practicePlace PracticePlace::Instance()

} // namespace GObject

#endif // PRACTICEPLACE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

