#ifndef _CARDSYSTEM_H_
#define _CARDSYSTEM_H_

#include "GDataDBExecHelper.h"
#include "GData/AttrExtra.h"
#include "GObject/CollectCard.h"

namespace GData
{
    
    struct CardUpgradeTable
    {
        //UInt8 level;
        UInt32 gexp;
        UInt32 bexp;
        UInt32 pexp;
        UInt32 yexp;
        UInt32 hgexp;
        UInt32 hbexp;
        UInt32 hpexp;
        UInt32 hyexp;
        UInt8 skillLevel;
        UInt16 attrIndex;

        CardUpgradeTable(): gexp(0),bexp(0),pexp(0),yexp(0),hgexp(0),hbexp(0),hpexp(0),hyexp(0),skillLevel(0),attrIndex(0){}
    };

    struct CardInitInfo
    {
        UInt8 type;//1 - 装备 2 - 人物 3 - 特殊卡牌
        UInt8 color;//0 - 白色 1 - 绿色 2 - 蓝色 3 - 紫色 4 - 橙色 
        UInt8 lvLimit;
        UInt16 skillId;//初始技能id
        UInt32 initExp;

        CardInitInfo(): type(0),color(0),lvLimit(0),skillId(0){}
    };


class CardSystem 
{

public:
    void loadCardUpgradeTable(DBCardUpgrade&);
    void loadInitCardInfo(DBCardInfo&);
    void AddSuitCardAttr(GData::AttrExtra&,UInt16 attr_id,UInt8 active_set);
    void AddCardAttr(GData::AttrExtra&, UInt16 attr_id,UInt8 level ,UInt8 color,UInt8 type);
    bool checkUpgrade(GObject::CardInfo* ci);
    CardInitInfo* getCardInitInfo(UInt16 id);
    CardUpgradeTable* getCardUpgradeTable(UInt8 level);
    
private:
    std::map<UInt8/*level*/, CardUpgradeTable> _cardUpgrade;
    std::map<UInt16/*id*/, CardInitInfo> _cardInitInfo;
};

extern CardSystem csys;
 

}

#endif
