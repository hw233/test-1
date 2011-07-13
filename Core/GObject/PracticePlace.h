
#ifndef PRACTICEPLACE_H_
#define PRACTICEPLACE_H_

#include "Common/Singleton.h"
#include "Common/Mutex.h"
#include "GData/ObjectManager.h"
#include <map>
#include <list>
#include <vector>

#define PPLACE_MAX 7

namespace GObject
{

class Player;

// 山头信息
struct PPlace
{
    UInt64 ownerid;
    UInt64 protid;
    UInt16 maxslot;
    UInt16 protmoney;
    UInt16 slotmoney; // 8小时所收费用
    UInt8 openslot;   // 金币开启的洞府
    UInt8 techslot;   // 科技开启的洞府
    UInt8 open;
    UInt16 enemyCount;
    UInt16 winCount;
};

struct PracticeData : public GData::ObjectBaseNT<UInt64>
{
    PracticeData(UInt64 id) :GData::ObjectBaseNT<UInt64>(id) {}
    ~PracticeData() { fighters.clear(); }

    UInt8 type;         // 修炼类型 0-8小时, 1-24小时
    UInt8 pricetype;    // 付费方式 0-金币， 1-银币
    UInt16 price;       // 花了多少钱
    UInt32 traintime;   // 修炼分钟数
    UInt32 checktime;   // 修炼分钟数
    UInt32 trainend;    // 修炼结束时间
    UInt8 prot;         // 是否申请保护 0-没有任何保护, 1-护法弟子保护, 2-仙府禁法
    UInt32 cdend;       // 下次可修炼时间
    UInt64 winnerid;    // 挑战胜利者ID

    //Mutex lock;
    std::list<UInt32> fighters;
};

struct PlaceData
{
    PlaceData() : used(0)
    {
        bzero(&place, sizeof(place));
    }
    ~PlaceData() { }

    PPlace place;
    std::vector<PracticeData*> data;
    size_t used;
};

// XXX: 修炼指令必须是世界线程执行
class PracticePlace : public Singleton<PracticePlace>
{
public:
    PracticePlace() { }
    ~PracticePlace() { }

    // 付费
    bool pay(Player* pl, UInt8 place, UInt16 slot, UInt8 type, UInt8 priceType, UInt8 time, UInt8 prot);
    // 停止修炼
    bool stop(Player* pl);

    // 将修炼
    bool sitdown(Player* pl, UInt32* fgtid, size_t size);
    // 将退出修炼
    bool standup(Player* pl, UInt32* fgtid, size_t size);
    // 更新修炼将
    void updateFighters(std::list<UInt32>& fgts, UInt64 id);

    // 取得所有修练点信息 
    void getAllPlaceInfo(Player* pl);
    // 取得修炼点信息
    void getPlaceInfo(Player* pl, UInt8 place);
    // 取得某一修为挂机点位置
    void getList(Player* pl, UInt8 place, UInt16 pageno, UInt16 pagenum);

    // 更换护法弟子
    bool replaceProtecter(Player* pl, UInt8 place, UInt64 protid);
    // 设置挂机收费
    bool setCharges(Player* pl, UInt8 place, UInt16 money);
    // 设置保护收费
    bool setProtCharges(Player* pl, UInt8 place, UInt16 money);
    // 将所有修炼人放入无主之地
    void moveAllToMax(UInt8 place);

    // 挑战
    bool doChallenge(Player* pl, UInt8 place, UInt16 idx, const std::string& name);

    // 使用金币增加修练空间
    bool addSlot(Player* pl, UInt8 place);

    // 通过帮派科技增加修炼空间
    bool addSlotFromTech(Player* pl, UInt8 place = 0);

    // 山头易主
    bool replaceOwner(Player* oldpl, Player* newpl);

    // 占领山头
    bool ocupyPlace(Player* pl, UInt8 place);

    // 增加一个修炼点
    bool addPlace(PPlace& place, UInt8 idx);
    // 增加一个人
    bool addPractice(Player* pl, PracticeData* data, UInt8 place);
    // 取得修炼数据
    PracticeData* getPracticeData(UInt64 playerid);
    PracticeData* getPracticeData(Player* pl);
    bool delPracticeData(Player* pl);
    UInt64 getPlaceOwnerId(UInt8 place);

    bool isSitdownYet(PracticeData* pd, UInt32 id);

private:
    Mutex m_lock[PPLACE_MAX];
    PlaceData m_places[PPLACE_MAX];
    std::map<UInt64, PracticeData*> m_pradata;
};

#define practicePlace PracticePlace::Instance()

} // namespace GObject

#endif // PRACTICEPLACE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

