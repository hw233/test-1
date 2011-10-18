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
	SaleRowType _saleRow[48];	//��ͨ1 ǿ��2 (0 : reversed)
                                //װ��3 ����4 ͷ��5 �ؼ�6 ���7 ����8 �ȼ�9 ����10 ��ָ11
                                //�ķ�12 �����ķ�13 �����ķ�14
                                //����15 ��������16 ��˫����17
                                //��18 �߾�������19 ����Ԫ����20 ���Ŷݼ���21 ���ɷ��22 �����һ���23 �ߵ�������24
                                //����������25 ���������26 ��΢̫����27 ��շ�ħ��28 ���־Ź���29 ����΢����30
                                //��ʯ31 ����32 ����33 ����34 ����35 ��־36 ����37 ����38 ����39 ����40 ����41 ����42 ����43 �ƻ�44 ��45 ����46 ����47
	UInt32 _itemStat[4][48][7];	//����(ȫ��0 ��1��2��3) (ȫ��0 ��ͨ1 ǿ��2 װ��3.. �ķ�12.. ����15.. ��18.. ��ʯ31..) 0ȫ�� 1��ɫ2��ɫ3��ɫ4��ɫ5��ɫ6����
};

extern SaleMgr gSaleMgr;

}


#endif
