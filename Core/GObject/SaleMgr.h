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
	SaleRowType _saleRow[63];	//普通1 强化2 (0 : reversed)
                                //装备3 武器4 头盔5 胸甲6 肩甲7 腰带8 腿甲9 项链10 戒指11
                                //心法12 增益心法13 技能心法14, 技能碎片15
                                //法宝16 被动法宝17 无双法宝18, 法宝碎片19，光环法宝20
                                //阵法21 七绝锁云阵22 四象元灵阵23 奇门遁甲阵24 天罡地煞阵25 都天烈火阵26 颠倒八卦阵27 北斗七星阵28 五行灭绝阵29 紫微太极阵30 金刚伏魔阵31 须弥九宫阵32 两仪微尘阵33
                                //宝石34 力量35 敏捷36 智力37 耐力38 意志39 生命40 攻击41 防御42 命中43 反击44 闪避45 暴击46 破击47 身法48 坚韧49 法抗50
                                //魂51 52攻击 53防御 54暴击 55破击 56身法 57坚韧 58毁灭 59生命
                                //元神60 61元神技能 62元神
	UInt32 _itemStat[5][63][7];	//分类(全部0 儒1释2道3墨4) (全部0 普通1 强化2 装备3.. 心法12.. 法宝16.. 阵法21.. 宝石34.. 魂51. 元神60) 0全部 1白色2绿色3蓝色4紫色5橙色6暗金
};

extern SaleMgr gSaleMgr;

}

#endif
