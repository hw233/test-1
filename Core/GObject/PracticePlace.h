
#ifndef PRACTICEPLACE_H_
#define PRACTICEPLACE_H_

#include "Common/Singleton.h"
#include "Common/Mutex.h"
#include <map>
#include <set>

namespace GObject
{

class Fighter;

struct PPlace
{
    UInt8 id;
    UInt32 ownerid;
    UInt16 maxslot;
    UInt32 protid;
    UInt8 open;
};

struct PracticeData
{
    UInt8  priceType;
    UInt32 price;
    UInt32 traintime;
    UInt32 checktime;
    UInt32 trainend;
    UInt8 proted; // 是否申请保护 0-没有任何保护, 1-护法弟子保护, 2-仙府禁法
};

struct PlaceData
{
    PPlace place;
    std::set<Fighter*> fighters;
};

class PracticePlace : public Singleton<PracticePlace>
{
public:
    PracticePlace(){}
    ~PracticePlace(){}

    // 修炼
    bool sitdown(UInt8 place, Fighter* fgt, PracticeData& data);
    // 退出修炼
    bool standup(UInt8 place, Fighter* fgt);

    // 取得某一修为挂机点位置
    void getList(UInt8 place, UInt16 pageno);

    // 更换护法弟子
    bool replaceProtecter(UInt8 place, UInt32 protid);
    // 挑战
    bool doChallenge(UInt8 place, UInt32 id);

    // 增加一个修炼点
    bool addPlace(PPlace& place);

private:
    Mutex m_lock;
    std::map<UInt8, PlaceData> m_places;
};

#define practicePlace PracticePlace::Instance()

} // namespace GObject

#endif // PRACTICEPLACE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

