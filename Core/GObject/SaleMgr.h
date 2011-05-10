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
static const UInt32 SALE_TIME_OUT = 48 * 60 * 60;

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

	void requestSaleList(Player *, UInt16, UInt16, UInt8, std::string&, UInt8, UInt8, UInt8, UInt8);
	
	void searchPlayerSale(Player *, Player *, UInt16, UInt16);
	void searchPlayerSaleResp(Player *, Player *, UInt16, UInt16, UInt32 *, UInt16);
	void searchSaleByItemName(Player *, std::string&, UInt16, UInt16, UInt8);

protected:
	bool shiftSingleSaleList(UInt8, UInt8, UInt16, UInt16&, UInt16&);
	bool shiftSingleSaleList2(UInt8, UInt8, UInt16&, UInt16&, UInt16&);
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

	inline UInt8 Index(UInt8 type, UInt32 typeId)
	{
		static UInt8 cvt[] = { 1, 1, 2, 1, 6, 7, 8, 9, 10, 11, 12, 13, 4, 1 };
		if (cvt[type] != 1)
			return cvt[type];
		else
		{
			if ((typeId >= 8914 && typeId <= 8929) || typeId == 9215)
				return 3;
			return 1;
		}
	}
	inline UInt8 StatIndex(UInt8 type, UInt32 typeId)
	{
		static UInt8 cvt[] = { 1, 1, 2, 1, 6, 7, 8, 9, 10, 11, 12, 13, 4, 1 };
		if (cvt[type] != 1)
			return cvt[type];
		else
		{
			if ((typeId >= 8914 && typeId <= 8929) || typeId == 9215)
				return 3;
			return 1;				
		}
	}

	inline UInt32 saleRowStat(UInt8 type, UInt8 color)
	{
		return _itemStat[type][color];		
	}

	void addRowSale(SaleData *);
	void delRowSale(SaleData *);


private:
	typedef std::map<UInt32, std::vector<SaleData *>, std::greater<UInt32> > SaleRowType;		//itemId -> saleData
	typedef std::multimap<UInt32, UInt32> SaleCheckType;				//time -> itemPos
	typedef std::map<UInt32, UInt32> SalePosType;						//id -> itemPos

	std::vector<SaleData *> _sales;
	std::set<UInt32> _saleIndex;
	
	SalePosType _salePos;
	SaleCheckType _saleCheck;
	SaleRowType _saleRow[14];	//普通1 宝石2 强化3 喜好品4 装备5 武器6 头盔7 胸甲8 肩甲9 腰带10 腿甲11 项链12 戒指13 (0 : reversed)
	UInt32 _itemStat[14][7];	//分类(全部0 普通1 宝石2 强化3 喜好品4 装备5 武器6 头盔7 胸甲8 肩甲9 腰带10 腿甲11 项链12 戒指13) 0全部 1白色2绿色3蓝色4紫色5橙色6暗金
};

extern SaleMgr gSaleMgr;

}


#endif
