#include "Config.h"
#include "Store.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "MsgID.h"
#include "Server/Cfg.h"
#include "GObject/Package.h"

namespace GData
{

Store store;

void Store::process (UInt32 now)
{
    // 每隔五分钟的定时器检测，检查限购状态
    clearSpecialDiscount();
}

void Store::add( UInt8 type, UInt32 itemId, UInt32 price )
{
    if(cfg.arenaPort == 0 && (itemId == ARENA_BET_ITEM1 || itemId == ARENA_BET_ITEM2))
        return;

    if(type >= PURCHASE1 && type <= PURCHASE2)
    {
        _items[type - PURCHASE1].push_back(itemId + (price << 16));
        _itemPrices[type - PURCHASE1][itemId] = price;
    }
    else if(type >= PURCHASE3 && type <= PURCHASE4)
    {
        _items2[type - PURCHASE3].push_back(itemId + (price << 16));
        _itemPrices2[type - PURCHASE3][itemId] = price;
    }

    return;
}

void Store::addDiscountFromDB( UInt16 itemID, UInt8 discountType, UInt32 limitCount,
        UInt32 beginTime, UInt32 endTime, UInt16 priceOriginal, UInt16 priceDiscount)
{
    // 从数据库读取折扣信息
    Discount discount;
    discount.itemID = itemID;
    discount.discountType = discountType;
    discount.limitCount = limitCount;
    discount.beginTime = beginTime;
    discount.endTime = endTime;
    discount.priceDiscount = priceDiscount;
    discount.priceOriginal = priceOriginal;
    _itemsDiscount[DISCOUNTEND - DISCOUNT].push_back(discount);
    ++ (_itemTypeCountDiscount[DISCOUNTEND - DISCOUNT][discount.discountType]);
}

void Store::addExchange(UInt8 type, UInt32 itemId, UInt32 priceID, UInt32 priceNum)
{
    if(type >= EXCHANGE && type <= EXCHANGEEND)
    {
        Exchange exchange;
        exchange.itemID = static_cast<UInt16>(itemId);
        exchange.priceID = static_cast<UInt16>(priceID);
        exchange.priceNum = static_cast<UInt16>(priceNum);
        _itemsExchange[type - EXCHANGE].push_back(exchange);
        _itemPricesExchange[type - EXCHANGE][itemId] = priceID + (priceNum << 16);
    }
    return;
}

void Store::addNormalDiscount(UInt32 itemId, UInt32 discountNum)
{
    // lua脚本自动生成三五八折的限购商品表 
    UInt8 type = 0;
    switch (discountNum)
    {
        case 3:
            type = 7;
            break;
        case 5:
            type = 8;
            break;
        case 8:
            type = 9;
            break;
        default:
            break;
    }
    UInt32 now = TimeUtil::Now() + 30;
    Discount discount;
    discount.itemID = itemId;
    discount.discountType = type;
    discount.limitCount = discountNum + 4 / (_itemTypeCountDiscount[DISCOUNTEND - DISCOUNT][type] + 1);
    discount.beginTime = now;
    discount.endTime = TimeUtil::SharpWeek(1, now);
    discount.priceOriginal = getPrice(itemId);
    discount.priceDiscount = getPrice(itemId) * discountNum / 10;
    _itemsDiscount[DISCOUNTEND - DISCOUNT].push_back(discount);
    ++ (_itemTypeCountDiscount[DISCOUNTEND - DISCOUNT][discount.discountType]);
}

void Store::addSpecialDiscount()
{
    // FIXME: 优化lua动态载入特殊限购活动
    UInt32 now = TimeUtil::Now();
    lua_State* L = lua_open();
    luaL_openlibs(L); 
    {
        std::string path = cfg.scriptPath + "World/Store.lua";
        lua_tinker::dofile(L, path.c_str());

        lua_tinker::table t = lua_tinker::call<lua_tinker::table>(L, "GetSpecialDiscount");
        UInt32 size = t.size();
        for (UInt32 i= 1; i <= size; ++i)
        {
            lua_tinker::table specialDiscount = t.get<lua_tinker::table>(i);
            Discount discount;
            discount.itemID = specialDiscount.get<UInt16>(1);
            discount.discountType = specialDiscount.get<UInt8>(2);
            discount.limitCount = specialDiscount.get<UInt32>(3);
            discount.beginTime = specialDiscount.get<UInt32>(4);
            discount.endTime = specialDiscount.get<UInt32>(5);
            discount.priceDiscount = specialDiscount.get<UInt16>(6);
            discount.priceOriginal = specialDiscount.get<UInt16>(7);
            if (discount.endTime <= (now + 30))
            {
                continue;
            }
            _itemsDiscount[DISCOUNTEND - DISCOUNT].push_back(discount);
            ++ (_itemTypeCountDiscount[DISCOUNTEND - DISCOUNT][discount.discountType]);
        }
    }

    lua_close(L);
    return;
}

UInt8 Store::addSpecialDiscountFromBS(Discount discount)
{
    // GM通过后台发送新增限购活动
    UInt32 now = TimeUtil::Now();
    if (discount.itemID == 0 ||
            discount.beginTime < now)
    {
        return 1;
    }
    if (discount.discountType > 6 || discount.discountType < 4)
    {
        return 1;
    }
    _itemsDiscount[DISCOUNTEND - DISCOUNT].push_back(discount);
    ++ (_itemTypeCountDiscount[DISCOUNTEND - DISCOUNT][discount.discountType]);
    storeDiscount();
    return 0;
}

void Store::querySpecialDiscountFromBS()
{
    // TODO: GM通过后台查询现有的限购活动
}

void Store::clearSpecialDiscountFromBS()
{
    // GM通过后台发送清空限购活动
    std::vector<Discount>& items = _itemsDiscount[DISCOUNTEND - DISCOUNT];
    for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); )
    {
#if 1
        if ( ((*it).discountType) <= 6 && ((*it).discountType >= 4))
        {
            it = items.erase(it);
        }
        else
        {
            ++ it;
        }
#else
        ++ it;
#endif
    }
    storeDiscount();
}

bool Store::needResetDiscount()
{ 
    // 判断是否需要更新限购商品
    if(!_itemsDiscount[0].size())
        return true;
    else
    {
        // 检查三五八折是否过期
        std::vector<Discount>& items = _itemsDiscount[DISCOUNTEND - DISCOUNT];
		for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            if ((*it).discountType >= 7 && (*it).discountType <= 9)
            {
                // 限购商品的过期时间检查
                if ((*it).endTime > TimeUtil::Now() + 30)
                {
                    // 本周限购未过期
                    return false;
                }
                else
                {
                    // 本周限购已经过期
                    return true;
                }
            }
        }
        // 没有三五八折限购商品信息
        return true;
    }
}

UInt32 Store::getPrice( UInt8 type, UInt16 itemId )
{
    if(type >= PURCHASE1 && type <= PURCHASE2)
    {
        std::map<UInt32, UInt32>::iterator it = _itemPrices[type - PURCHASE1].find(itemId);
        if(it == _itemPrices[type - PURCHASE1].end())
            return 0xFFFFFFFF;
        return it->second;
    }
    else if(type >= PURCHASE3 && type <= PURCHASE4)
    {
        std::map<UInt32, UInt32>::iterator it = _itemPrices2[type - PURCHASE3].find(itemId);
        if(it == _itemPrices2[type - PURCHASE3].end())
            return 0xFFFFFFFF;
        return it->second;
    }
    else if(type >= EXCHANGE && type <= EXCHANGEEND)
    {
        std::map<UInt32, UInt32>::iterator it = _itemPricesExchange[type - EXCHANGE].find(itemId);
        if(it == _itemPricesExchange[type - EXCHANGE].end())
            return 0xFFFFFFFF;
        return it->second;
    }
    return 0xFFFFFFFF;
}

UInt32 Store::getPrice( UInt8 type, UInt16 itemId, UInt16 flag)
{
    // 获取限购商品的价格
    if (type >=DISCOUNT && type <= DISCOUNTEND)
    {
        std::vector<Discount>& items = _itemsDiscount[type - DISCOUNT];
		for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            if ((*it).itemID == itemId && (*it).discountType == flag)
                return (*it).priceDiscount;
        }
       
    }
    return 0xFFFFFFFF;
}

UInt32 Store::getPrice( UInt16 itemId )
{
    for (UInt8 type = 1; type <= 3; ++type)
    {
        std::map<UInt32, UInt32>::iterator it = _itemPrices[type - 1].find(itemId);
        if(it == _itemPrices[type - 1].end())
            continue;
        return it->second;
    }
    return 0;
}

void Store::sendList( UInt8 type, GObject::Player * player )
{
    if(type >= PURCHASE1 && type <= PURCHASE2)
        player->send(_storePacket[type - PURCHASE1]);
    else if(type >= PURCHASE3 && type <= PURCHASE4)
        player->send(_storePacket2[type - PURCHASE3]);
    else if(type >= EXCHANGE && type <= EXCHANGEEND)
        player->send(_storePacketExchange[type - EXCHANGE]);

    // FIXME: 通知客户端限购购买的次数
    if (type == 1)
    {
        player->send(_storePacketDiscount[type - DISCOUNT]);
        player->sendDiscountLimit();
    }
}

void Store::makePacket()
{
	for(int i = 0; i <= PURCHASE2 - PURCHASE1; ++ i)
	{
		std::vector<UInt32>& items = _items[i];
		_storePacket[i].init(REP::STORE_LIST);
		_storePacket[i] << static_cast<UInt8>(PURCHASE1 + i) << static_cast<UInt8>(items.size());
		for(std::vector<UInt32>::iterator it = items.begin(); it != items.end(); ++ it)
		{
			_storePacket[i] << *it;
		}
		_storePacket[i] << Stream::eos;
	}

    for(int i = 0; i <= PURCHASE4 - PURCHASE3; ++ i)
	{
		std::vector<UInt32>& items = _items2[i];
		_storePacket2[i].init(REP::STORE_LIST);
		_storePacket2[i] << static_cast<UInt8>(PURCHASE3 + i) << static_cast<UInt8>(items.size());
		for(std::vector<UInt32>::iterator it = items.begin(); it != items.end(); ++ it)
		{
			_storePacket2[i] << *it;
        }
		_storePacket2[i] << Stream::eos;
	}

    for (int i = 0; i <= EXCHANGEEND - EXCHANGE; ++i)
    {
		std::vector<Exchange>& items = _itemsExchange[i];
		_storePacketExchange[i].init(REP::STORE_LIST_EXCHANGE);
		_storePacketExchange[i] << static_cast<UInt8>(i + EXCHANGE) << static_cast<UInt8>(items.size());
		for(std::vector<Exchange>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            _storePacketExchange[i] << (*it).itemID << (*it).priceID << (*it).priceNum;
        }
		_storePacketExchange[i] << Stream::eos;
    }

    for (int i = 0; i<= DISCOUNTEND - DISCOUNTEND; ++ i)
    {
        std::vector<Discount>& items = _itemsDiscount[i];
        _storePacketDiscount[i].init(REP::STORE_LIST);
		_storePacketDiscount[i] << static_cast<UInt8>(DISCOUNT + i) << static_cast<UInt8>(items.size());
		for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
		{
			_storePacketDiscount[i] << (*it).itemID << (*it).discountType << (*it).limitCount << \
                (*it).beginTime << (*it).endTime << (*it).priceDiscount << (*it).priceOriginal;
        }
		_storePacketDiscount[i] << Stream::eos;
    }
}

void Store::clear()
{
	for(int i = 0; i <= PURCHASE2 - PURCHASE1; ++i)
	{
		_storePacket[i].clear();
        //if (i) // XXX: do not clear _items[0]
        _items[i].clear();
		_itemPrices[i].clear();
	}
	for(int i = 0; i <= PURCHASE4 - PURCHASE3; ++i)
	{
		_storePacket2[i].clear();
		_items2[i].clear();
		_itemPrices2[i].clear();
	}
    for (int i = 0; i <= EXCHANGEEND - EXCHANGE; ++i)
    {
		_storePacketExchange[i].clear();
		_itemsExchange[i].clear();
		_itemPricesExchange[i].clear();
    }
}

void Store::clearNormalDiscount()
{
    // 清除普通的三五八折存储信息
    std::vector<Discount>& items = _itemsDiscount[DISCOUNTEND - DISCOUNT];
    for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); )
    {
        if ((*it).discountType >= 7 && (*it).discountType <= 9)
        {
            it = items.erase(it);
        }
        else
        {
            ++ it;
        }
    }

    return;
}

void Store::clearSpecialDiscount()
{
    // 清除过期的限购打折信息
    std::vector<Discount>& items = _itemsDiscount[DISCOUNTEND - DISCOUNT];
    UInt32 now = TimeUtil::Now();
    for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); )
    {
        if ((*it).endTime <= (now + 30))
        {
            if ((*it).discountType < 7)
            {
                DB3().PushUpdateData("DELETE FROM `discount` \
                        where `itemid` = %u and `type` = %u", \
                        (*it).itemID, (*it).discountType);
                it = items.erase(it);
            }
            else 
                ++ it;
        }
        else
        {
            ++ it;
        }
    }
}

void Store::storeDiscount()
{
    // 数据库中存入折扣商品信息
    DB3().PushUpdateData("DELETE FROM `discount`;");
    for (UInt8 i = 0; i <= DISCOUNTEND - DISCOUNT; ++i)
    {
        std::vector<Discount>& items = _itemsDiscount[i];
		for(std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
		{
            DB3().PushUpdateData("INSERT INTO `discount` \
                (`itemid`, `timeBegin`, `timeEnd`, `priceOriginal`, `priceDiscount`, `type`, `count`) \
                VALUES (%u, %u, %u, %u, %u, %u, %u)",
                (*it).itemID, (*it).beginTime, (*it).endTime, (*it).priceOriginal, (*it).priceDiscount, (*it).discountType, (*it).limitCount);
        }
    }
}


UInt8 Store::getDiscountType(UInt8 columnIndex)
{
    // 根据竖栏数目获取相应的限购类型
    ++columnIndex;
    for (UInt8 i = 0; i < 3; ++i)
    {
        std::map<UInt8, UInt8>::iterator it = _itemTypeCountDiscount[DISCOUNTEND - DISCOUNT].find(columnIndex);
        if(it == _itemTypeCountDiscount[DISCOUNTEND - DISCOUNT].end())
            columnIndex += 3;
        else
        {
            return columnIndex;
        }
    }

    return 0;
}

UInt16 Store::getDiscountLimit(UInt8 type)
{
    // 根据不同的限购类型获取限购的数量
    for (UInt8 i = 0; i < DISCOUNTEND - DISCOUNT + 1; ++ i)
    {
        std::vector<Discount>& items = _itemsDiscount[i];
        for (std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            if ((*it).discountType == type)
                    return (*it).limitCount;
        }
    }
    return 0;
}


UInt8 Store::getDisTypeVarOffset(UInt8 type)
{
    // 获取限购种类的var索引偏移量
    // type：1,2,3为全服限购，不需要获得玩家变量
    //       4,5,6为活动限时限购
    //       7,8,9为普通三五八折限购
    switch (type)
    {
        case 1:
            return 0xfe;
        case 2:
            return 0xfe;
        case 3:
            return 0xfe;
        case 4:
            return 144-80;
        case 5:
            return 145-80;
        case 6:
            return 146-80;
        case 7:
            return 82-80;
        case 8:
            return 81-80;
        case 9:
            return 80-80;
        default:
            return 0xff;
    }
    return 0xff;
}


UInt8 Store::getItemsByDiscount(UInt8 type, UInt16 items[4])
{
    // 获取限购商品
    UInt8 c = 0;

    std::vector<Discount>& itemsDiscount = _itemsDiscount[DISCOUNTEND - DISCOUNT];
    for(std::vector<Discount>::iterator it = itemsDiscount.begin(); it != itemsDiscount.end(); ++ it)
    {
        if ((*it).discountType == type)
        {
            items[c] = (*it).itemID;
            ++c;
        }
    }
    return c;
}

UInt32 Store::getEndTimeByDiscountType(UInt8 type)
{
    // 根据限购类型返回结束时间(时间戳)
    for (UInt8 i = 0; i < DISCOUNTEND - DISCOUNT + 1; ++ i)
    {
        std::vector<Discount>& items = _itemsDiscount[i];
        for (std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            if ((*it).discountType == type)
                    return (*it).endTime;
        }
    }
    return 0;
}

UInt32 Store::getBeginTimeByDiscountType(UInt8 type)
{
    // 根据限购类型返回开始时间(时间戳)
    for (UInt8 i = 0; i < DISCOUNTEND - DISCOUNT + 1; ++ i)
    {
        std::vector<Discount>& items = _itemsDiscount[i];
        for (std::vector<Discount>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            if ((*it).discountType == type)
                    return (*it).beginTime;
        }
    }
    return 0;
}

}

