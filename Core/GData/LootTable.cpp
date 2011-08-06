#include "Config.h"
#include "LootTable.h"
#include "ItemType.h"
#include "GObject/Player.h"
#include "GObject/Package.h"

namespace GData
{

LootTable lootTable;

void LootTable::add( UInt32 id, LootItem& li )
{
	if(id >= _table.size())
		_table.resize((id + 0x40) & ~0x3F);
	_table[id] = li;
}

const LootItem * LootTable::operator[]( UInt32 id )
{
	if(id >= _table.size() || _table[id].items.empty())
		return NULL;
	return &_table[id];
}

void LootItem::roll( GObject::Player * player ) const
{
	LootResult lr = roll();
	if(lr.id == 0)
		return;
	player->GetPackage()->Add(lr.id, lr.count, false, false, FromNpc);
}

LootResult LootItem::roll(URandom * ur) const
{
	if(ur == NULL)
	{
		ur = &static_cast<BaseThread *>(Thread::current())->uRandom;
	}
	LootResult r = {0, 0};
	if(isPack)
	{
		size_t cnt = items.size();
		UInt32 rollNum = (*ur)(10000);
		for(size_t i = 0; i < cnt; ++ i)
		{
			const LootData& ld = items[i];
			if(rollNum < ld.chance)
			{
				const LootItem * li = lootTable[ld.id];
				if(li != NULL)
				{
					r = li->roll(ur);
					if(!ld.counts.empty())
					{
						UInt32 countR = (*ur)(10000);
						size_t ccnt = ld.counts.size();
						for(size_t j = 0; j < ccnt; ++ j)
						{
							if(ld.counts[j].chance > countR)
							{
								r.count = ld.counts[j].count;
								break;
							}
							countR -= ld.counts[j].chance;
						}
					}
				}
				return r;
			}
			rollNum -= items[i].chance;
		}
		return r;
	}
	r.id = items[(*ur)(items.size())].id;
	r.count = 1;
	return r;
}

}
