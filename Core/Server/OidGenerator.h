#ifndef _OIDGENERATOR_H_
#define _OIDGENERATOR_H_

#include "Common/AtomicVal.h"

namespace IDGenerator
{
	class IDGen
	{
	public:
		IDGen(UInt32 maxid = 0): _maxID(maxid) {}
	protected:
		virtual UInt32 genID()
		{
			return ++ _maxID;
		}
		inline void setMaxID(UInt32 id)
		{
			if(_maxID < id)
				_maxID = id;
		}
		AtomicVal<UInt32> _maxID;
	};

	class ItemIDGen:
		public IDGen
	{
		//物品ID生成规则
	public:
		ItemIDGen(UInt32 maxid = 30000): IDGen(maxid) {}
	};

	class ChatFgtIDGen:
		public IDGen
	{
		//物品ID生成规则
	public:
		ChatFgtIDGen(UInt32 maxid = 0x1000): IDGen(maxid) {}
	};

	class MailIDGen:
		public IDGen
	{
		//物品ID生成规则
	public:
		MailIDGen(UInt32 maxid = 0): IDGen(maxid) {}
	};
	class SalesIDGen:
		public IDGen
	{
		//物品ID生成规则
	public:
		SalesIDGen(UInt32 maxid = 0): IDGen(maxid) {}
	};

	class BattleIDGen:
		public IDGen
	{
		//战报ID生成规则
	public:
		BattleIDGen(UInt32 maxid = 0): IDGen(maxid) {}
	};

	class ClanIDGen:
		public IDGen
	{
		//宗族ID生成规则
	public:
		ClanIDGen(UInt32 maxid = 0): IDGen(maxid) {}
	};

	class TradeIDGen:
		public IDGen
	{
	public:
		TradeIDGen(UInt32 maxid = 0) : IDGen(maxid) {}
	};

	class SaleIDGen:
		public IDGen
	{
	public:
		SaleIDGen(UInt32 maxid = 0) : IDGen(maxid) {}
	};

	class AthleticsRecordIDGen:
		public IDGen
	{
	public:
		AthleticsRecordIDGen(UInt32 maxid = 0) : IDGen(maxid) {}
	};

    class AthleticsEventIDGen:
        public IDGen
    {
    public:
        AthleticsEventIDGen(UInt32 maxid = 0) : IDGen(maxid) {}
    };

	class ClanBatterRecordIDGen:
		public IDGen
	{
	public:
		ClanBatterRecordIDGen(UInt32 maxid = 0) : IDGen(maxid) {}
	};

	class SeedIDGen:
		public IDGen
	{
		//随机种子ID生成规则
	public:
		SeedIDGen(UInt32 maxid = static_cast<UInt32>(time(NULL))): IDGen(maxid) {}
		virtual UInt32 genID() { _maxID += static_cast<UInt32>(time(NULL)); return _maxID; }
	};

	template <typename GenType>
	class OidGenerator : public GenType
	{
	public:
		UInt32 ID()
		{
			return GenType::genID();
		}
		void Init(UInt32 maxid)
		{
			GenType::setMaxID(maxid);
		}
	};

	typedef OidGenerator<ItemIDGen> ItemOidGenerator;
	typedef OidGenerator<ChatFgtIDGen> ChatFgtOidGenerator;
	typedef OidGenerator<MailIDGen> MailOidGenerator;
	typedef OidGenerator<SalesIDGen> SalesOidGenerator;
	typedef OidGenerator<BattleIDGen> BattleOidGenerator;
	typedef OidGenerator<ClanIDGen> ClanOidGenerator;
	typedef OidGenerator<SeedIDGen> SeedOidGenerator;
	typedef OidGenerator<TradeIDGen> TradeOidGenerator;
	typedef OidGenerator<SaleIDGen> SaleOidGenerator;
	typedef OidGenerator<AthleticsRecordIDGen> AthleticsRecordOidGenerator;
	typedef OidGenerator<ClanBatterRecordIDGen> ClanBatterRecordIDGenerator;
	typedef OidGenerator<AthleticsEventIDGen> AthleticsEventOidGenerator;

	extern ItemOidGenerator gItemOidGenerator;
	extern ChatFgtOidGenerator gChatFgtOidGenerator;
	extern MailOidGenerator gMailOidGenerator;
	extern SalesOidGenerator gSalesOidGenerator;
	extern BattleOidGenerator gBattleOidGenerator;
	extern ClanOidGenerator gClanOidGenerator;
	extern SeedOidGenerator gSeedOidGenerator;
	extern TradeOidGenerator gTradeOidGenerator;
	extern SaleOidGenerator gSaleOidGenerator;
	extern AthleticsRecordOidGenerator gAthleticsRecordOidGenerator;
	extern ClanBatterRecordIDGenerator gClanBatterRecordIDGenerator;
	extern AthleticsEventOidGenerator gAthleticsEventOidGenerator;
}


#endif
