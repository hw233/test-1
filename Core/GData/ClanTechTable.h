#ifndef CLANTECHTABLE_INC
#define CLANTECHTABLE_INC

namespace GData
{

struct ClanTechTableData
{
	ClanTechTableData(UInt16 i = 0, UInt8 l = 0,  UInt32 n = 0, UInt32 an = 0, UInt8 c = 0, UInt32 e1 = 0, UInt32 e2 = 0)
		: id(i), level(l), needs(n), accNeeds(an), clanLev(c), effect1(e1), effect2(e2) {}

	UInt16 id;
	UInt8  level;
	UInt32 needs;
	UInt32 accNeeds;
	UInt8  clanLev;
	UInt32 effect1;
	UInt32 effect2;
};

typedef std::vector<ClanTechTableData>		SingleClanTechTable;
typedef std::vector<SingleClanTechTable>	ClanTechTable;

extern ClanTechTable clanTechTable;

}

#endif
