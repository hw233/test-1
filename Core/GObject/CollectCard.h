
#ifndef COLLECTCARD_H_
#define COLLECTCARD_H_

#include "Config.h"
#include <vector>
#include <map>
#include "Common/Stream.h"

namespace GData
{
    class AttrExtra;
}

namespace GObject
{

    enum EAttrType
    {
        ATTR_NULL = 0,
        ATTR_ATK,//物理攻击
        ATTR_MAGATK,//法术攻击
        ATTR_DEF,//物防
        ATTR_MAGDEF,//法防
        ATTR_PIERCE,//破击
        ATTR_CRITICAL,//暴击率
        ATTR_EVADE,//闪避
        ATTR_HITRATE,//命中
        ATTR_COUNTER,//反击
        ATTR_TOUGH,//坚韧
        ATTR_HP,//生命
        ATTR_MAX
    };

    enum CardSetLevel
    {
        CARDLEVEL_NULL = 0,
        CARDLEVEL_40,
        CARDLEVEL_50,
        CARDLEVEL_60,
        CARDLEVEL_70,
        CARDLEVEL_80,
        CARDLEVEL_90,
        CARDLEVEL_100,
        CARDLEVEL_110,
        CARDLEVEL_120,
        CARDLEVEL_MAX
    };

    struct CardInfo
    {
        UInt32 id;//流水号
        UInt16 cid;//卡牌id
        UInt8 type;//卡牌类型 1 - 人物卡牌 2 - 装备卡牌 3 - 特殊卡  
        UInt8 level;//等级
        UInt32 exp;//经验
        UInt16 skill_id;//技能id
        UInt8 pos;//位置 0 - 未装备 X - 装备在x位置(1-4是装备卡牌位置,5是人物卡牌位置)
        UInt8 color;//颜色
        UInt16 attr_id;//属性表id
        
        CardInfo(UInt32 num1 = 0,UInt16 num2 = 0,UInt8 num3 = 0,UInt8 num4 = 1,UInt32 num5 = 0,UInt16 num6 = 0,UInt8 num7 = 0,UInt8 num8 = 0,UInt16 num9 = 0)
        {
            id = num1;
            cid = num2;
            type = num3;
            level = num4;
            exp = num5;
            skill_id = num6;
            pos = num7;
            color = num8;
            attr_id = num9;
        }
        
        bool checkInfo();

    };
    
    class CardInfo_eq
    {
        public: 
            CardInfo_eq(const UInt32& num):id(num){}
            bool operator() (const CardInfo* c) const
            {
                if(!c)
                    return false;
                return c->id == id; 
            }
        private:
            UInt32 id;

    };

    struct SuitCardInfo
    {
        UInt8 id;//套牌组名id(20 - 特殊卡牌 40 - 40级套牌 50 - 50级套牌...)
        UInt8 suit_mark;//八张套牌标识位
        UInt8 active;//激活度(0 - 没激活 1 - 25% 2 - 50% 3 - 100%)
        UInt32 spe_mark;//特殊卡牌标识位
        UInt8 collect_degree;//收集度

        SuitCardInfo(UInt8 num1 = 0,UInt8 num2 = 0,UInt8 num3 = 0,UInt32 num4 = 0,UInt8 num5 = 0)
        {
            id = num1;
            suit_mark = num2;
            active = num3;
            spe_mark = num4;
            collect_degree = num5;
        }
        
        void checkExistSetBit(UInt16 cid);
        bool checkActive(UInt8 active_set);

    };

    class Player;
	class Fighter;
	class DBCard;
	class DBCardSuit;

    class CollectCard
    {
    public:
        CollectCard(Player* player);
        
        ~CollectCard();

        void ReturnCardInfo(UInt8 flag);
        
        bool ReturnSuitInfo(Stream&,UInt8 suit_lvl);

        void EquipCard(UInt32 id,UInt8 pos);//装备卡牌
        
        void ExchangeCard(UInt8 flag/* 0 - 手动 1- 自动*/,UInt8 level);//兑换卡牌
        
        void ActiveCardSet(UInt8 suit_lvl,UInt8 active_set);//激活卡组
        
        void UpGradeCard(UInt32 id,std::vector<UInt32>& vecid);//卡片升级

        void AddCardAttr(GData::AttrExtra& ae);//计算增加的属性

        void RebuildCardAttr();//重算卡片属性

        CardInfo* AddCard(UInt16 cid );//增加卡牌
    
        bool DelAddCard(UInt32 id);
    
        void ExchangeSpeCard(UInt16 itemid);
    
        void InsertCard(DBCard dbc);
        
        void InsertCardSuit(DBCardSuit dbcs);

    private:
        std::vector<CardInfo*> VecEquipSlot;//装备卡牌槽
        
        std::map<UInt32,CardInfo*> MapFreeCardSlot;//空闲卡牌槽
        
        std::map<UInt8,SuitCardInfo*> MapCardStamp;//套牌集邮册
        
        Player* m_owner;//卡牌拥有者
            
        typedef std::vector<CardInfo*> VecSlot;
        
        typedef std::map<UInt32,CardInfo*> MSlot;//空闲卡牌槽

        typedef std::map<UInt8,SuitCardInfo*> MStamp;//空闲卡牌槽
    };

}
#endif // COLLECTCARD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

