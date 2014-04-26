#include "Config.h"
#include "CardSystem.h"
#include "GObject/CollectCard.h"

namespace GData
{

CardSystem csys;

static float lvlmark = 0.78;//40级系数

static float greenMark = 0.1;//绿色品质系数
static float blueMark = 0.2;//蓝色品质系数
static float purpleMark = 0.5;//紫色品质系数
static float orangeMark = 1;//橙色品质系数

static float equipMark = 1;//装备系数
static float humanMark = 1.2;//人物系数
static float speMark = 1;//特殊系数

void CalInitExp(UInt8& initExp ,UInt8 level, UInt8 color ,UInt8 type)
{
    float levelMark = 0.0f;
    float colorMark= 0.0f;
    float typeMark= 0.0f;
    
    levelMark = static_cast<float>(level - 40) * 0.2 + 0.78;
    
    switch(color)
    {
        case 1:
            colorMark = greenMark;
            break;
        case 2:
            colorMark = blueMark;
            break;
        case 3:
            colorMark = purpleMark;
            break;
        case 4:
            colorMark = orangeMark;
            break;
        default:
            colorMark = 0;
            break;
    }

    switch(type)
    {
        case 1:
            typeMark = equipMark;
            break;
        case 2:
            typeMark = humanMark;
            break;
        case 3:
            typeMark = speMark;
            break;
        default:
            break;

    }

    initExp = 100 * levelMark * colorMark * typeMark;

    return;
}

void CardSystem::loadCardUpgradeTable(DBCardUpgrade& dbcu)
{
    if(dbcu.level<= 0)
        return;
    CardUpgradeTable cut;
    cut.gexp = dbcu.gexp;
    cut.bexp = dbcu.bexp;
    cut.pexp = dbcu.pexp;
    cut.yexp = dbcu.yexp;
    cut.hgexp = dbcu.hgexp;
    cut.hbexp = dbcu.hbexp;
    cut.hpexp = dbcu.hpexp;
    cut.hyexp = dbcu.hyexp;
    cut.skillLevel = dbcu.skillLevel;
    cut.attrIndex = dbcu.attrIndex;

    _cardUpgrade.insert(std::make_pair(dbcu.level,cut));          

}

void CardSystem::loadInitCardInfo(DBCardInfo& dbci)
{
    if(dbci.id<= 0)
        return;
    CardInitInfo ci;
    ci.type = dbci.type;
    ci.color = dbci.color;
    ci.lvLimit = dbci.lvLimit;
    ci.skillId = dbci.skillId;
    CalInitExp(ci.initExp,static_cast<UInt8>(dbci.id/10),ci.color,ci.type);
    
    _cardInitInfo.insert(std::make_pair(dbci.id,ci));          

}

CardInitInfo* CardSystem::getCardInitInfo(UInt16 id)
{
    std::map<UInt16,CardInitInfo>::iterator it = _cardInitInfo.find(id);
    if(it != _cardInitInfo.end())
        return &(it->second);
    return NULL; 
}

bool CardSystem::checkUpgrade(GObject::CardInfo* ci)
{
    if(_cardUpgrade.find(ci->level) == _cardUpgrade.end())
        return false;
    CardUpgradeTable tmp = (_cardUpgrade.find(ci->level))->second; 
                
    if(ci->type == 1 || ci->type == 3)
    {
        switch(ci->color)
        {
            case 1:
                if(ci->exp < tmp.gexp)
                    return false;
                break;
            case 2:
                if(ci->exp < tmp.bexp)
                    return false;
                break;
            case 3:
                if(ci->exp < tmp.pexp)
                    return false;
                break;
            case 4:
                if(ci->exp < tmp.yexp)
                    return false;
                break;
            default:
                return false;
        }
    }
    else // ci->type == 2 人物卡牌
    {
        switch(ci->color)
        {
            case 1:
                if(ci->exp < tmp.hgexp)
                    return false;
                break;
            case 2:
                if(ci->exp < tmp.hbexp)
                    return false;
                break;
            case 3:
                if(ci->exp < tmp.hpexp)
                    return false;
                break;
            case 4:
                if(ci->exp < tmp.hyexp)
                    return false;
                break;
            default:
                return false;
        }
    }

    {
        ci->level += 1;                   
        ci->skill_id = (ci->skill_id / 10) * 10 + tmp.skillLevel; 
        ci->attr_id = tmp.attrIndex; 
    }

    return true;
}

void CardSystem::AddCardAttr(GData::AttrExtra& ae, UInt16 attr_id,UInt8 level ,UInt8 color,UInt8 type)
{
    const AttrExtraItem * attr = attrExtraManager[attr_id];
    if(attr == NULL)
        return;
    GData::AttrExtra tmp; 
    tmp += *(*attr);

    float colorMark= 0.0f;
    float typeMark= 0.0f;
    
    switch(color)
    {
        case 1:
            colorMark = greenMark;
            break;
        case 2:
            colorMark = blueMark;
            break;
        case 3:
            colorMark = purpleMark;
            break;
        case 4:
            colorMark = orangeMark;
            break;
        default:
            colorMark = 0;
            break;
    }

    switch(type)
    {
        case 1:
            typeMark = equipMark;
            break;
        case 2:
            typeMark = humanMark;
            break;
        case 3:
            typeMark = speMark;
            break;
        default:
            break;
    }
    tmp = tmp * (colorMark * typeMark);

    ae += tmp;
    return;
}

void CardSystem::AddSuitCardAttr(GData::AttrExtra& ae,UInt16 attr_id,UInt8 active_set)
{
    const AttrExtraItem * attr = attrExtraManager[attr_id];
    GData::AttrExtra tmp; 
    tmp += *(*attr);
    float activeMark = 0.0f;
    switch(active_set)
    {
        case 1:
            activeMark = 0.25; 
            break;
        case 2:
            activeMark = 0.5; 
            break;
        case 3:
            activeMark = 1.0; 
            break;
        default:
            break;
    }
    tmp = tmp * activeMark;
    ae += tmp;
    return;
}


}

