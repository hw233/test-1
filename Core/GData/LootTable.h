#ifndef _LOOTTABLE_H_
#define _LOOTTABLE_H_

class URandom;

namespace GObject
{
	class Player;
}

namespace GData
{

struct LootResult
{
	UInt16 id;
	UInt16 count;
};

struct LootItem
{
	struct LootCount
	{
		UInt32 count;
		UInt32 chance;
	};
	struct LootData
	{
		LootData(): id(0), chance(0) {}
		UInt32 id;
		UInt32 chance;
		std::vector<LootCount> counts;
	};
	std::vector<LootData> items;
	UInt8 isPack;

	void roll(std::vector<LootResult>& lt, URandom * = NULL) const;
	void roll(GObject::Player *) const;
};

class LootTable
{
public:
	void add(UInt32, LootItem&);
	const LootItem * operator[](UInt32);
private:
	std::vector<LootItem> _table;
};

extern LootTable lootTable;

}

#endif // _LOOTTABLE_H_
