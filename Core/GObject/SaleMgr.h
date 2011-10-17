#ifndef _SALEMGR_H_
#define _SALEMGR_H_

#include "Sale.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"

namespace GObject
{

class Player;
class ItemBase;

struct SaleData
{
	UInt32 _pos;
	UInt32 _id;
	UInt8  _status;
	UInt32 _time;
	UInt8  _priceType;
	UInt32 _price;
	ItemBase * _item;
	Player * _owner;
};

class SaleMgr
{
static const UInt32 SALE_TIME_OUT = 24 * 60 * 60;
//static const UInt32 SALE_TIME_OUT = 5 * 60;

public:
	SaleMgr();
	~SaleMgr();
	
public:
	bool addSaleFromDB(SaleData *);

public:
	void sellSale(Player *, SalePut*, UInt8);
	void buySale(Player *, UInt32);
	void addSaleItem(Player *, UInt32, UInt32);
	void cancelSale(Player *, UInt32);

	void requestSaleList(Player *, UInt16, UInt16, std::string&, UInt8, UInt8, UInt8, UInt8);
	
	void searchPlayerSale(Player *, Player *, UInt16, UInt16);
	void searchPlayerSaleResp(Player *, Player *, UInt16, UInt16, UInt32 *, UInt16);
	void searchSaleByItemName(Player *, std::string&, UInt16, UInt16);

protected:
	bool shiftSingleSaleList(UInt8, UInt8, UInt8, UInt16, UInt16&, UInt16&);
	bool shiftSingleSaleList2(UInt8, UInt8, UInt8, UInt16&, UInt16&, UInt16&);
	bool shiftTotalSaleList(UInt8, UInt8, UInt16, UInt8&, UInt16&, UInt16&);
	UInt16 appendSingleSaleList(Player *, Stream&, UInt8, UInt8, UInt8,  UInt16, UInt16, UInt16);
	UInt16 appendTotalSaleList(Player *, Stream&, UInt8, UInt8, UInt8, UInt16, UInt16, UInt16);

public:
	void update(UInt32);
	void delSaleCheck(SaleData *);

	static inline bool isTimeoutSale(UInt32 curr, UInt32 now = TimeUtil::Now())
	{
		if (curr < now && now - curr >= SALE_TIME_OUT)
			return true;
		return false;
	}

    void setOnOff(UInt8 on_off)
    {
        _on_off = on_off;
    }

    UInt8 getOnOff() { return _on_off; }

protected:
	inline UInt32 getNextIndex()
	{
		UInt32 index = static_cast<UInt32>(-1);
		if (!_saleIndex.empty())
		{
			std::set<UInt32>::iterator it = _saleIndex.begin();
			index = *it;
			_saleIndex.erase(it);
		}	
		return index;
	}

	inline void setNextIndex(UInt32 index)
	{
		_saleIndex.insert(index);
	}

	UInt8 Index(UInt8 type, UInt32 typeId);

	UInt8 StatIndex(UInt8 type, UInt32 typeId, UInt8& parent);

	inline UInt32 saleRowStat(UInt8 type, UInt8 color, UInt8 career)
	{
		return _itemStat[career][type][color];
	}

	void addRowSale(SaleData *);
	void delRowSale(SaleData *);


private:
	typedef std::map<UInt32, std::vector<SaleData *>, std::greater<UInt32> > SaleRowType;		//itemId -> saleData
	typedef std::multimap<UInt32, UInt32> SaleCheckType;				//time -> itemPos
	typedef std::map<UInt32, UInt32> SalePosType;						//id -> itemPos

    UInt8 _on_off;
	std::vector<SaleData *> _sales;
	std::set<UInt32> _saleIndex;
	
	SalePosType _salePos;
	SaleCheckType _saleCheck[3];
	SaleRowType _saleRow[48];	//普通1 强化2 (0 : reversed)
                                //装备3 武器4 头盔5 胸甲6 肩甲7 腰带8 腿甲9 项链10 戒指11
                                //心法12 增益心法13 技能心法14
                                //法宝15 被动法宝16 无双法宝17
                                //阵法18 七绝锁云阵19 四象元灵阵20 奇门遁甲阵21 天罡地煞阵22 都天烈火阵23 颠倒八卦阵24
                                //北斗七星阵25 五行灭绝阵26 紫微太极阵27 金刚伏魔阵28 须弥九宫阵29 两仪微尘阵30
                                //宝石31 力量32 敏捷33 智力34 耐力35 意志36 生命37 攻击38 防御39 命中40 反击41 闪避42 暴击43 破击44 身法45 坚韧46 法抗47
	UInt32 _itemStat[4][48][7];	//分类(全部0 儒1释2道3) (全部0 普通1 强化2 装备3.. 心法12.. 法宝15.. 阵法18.. 宝石31..) 0全部 1白色2绿色3蓝色4紫色5橙色6暗金
};

extern SaleMgr gSaleMgr;

}


#endif
