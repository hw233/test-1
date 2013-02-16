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
    std::vector<LootResult> lr;
	roll(lr);
    for (size_t n = 0; n < lr.size(); ++n)
    {
        if (!lr[n].id)
            continue;
        player->GetPackage()->Add(lr[n].id, lr[n].count, lr[n].bind, false, FromNpc);
    }
}

void LootItem::roll(std::vector<LootResult>& lr, URandom * ur, bool bind) const
{
	if(ur == NULL)
	{
		ur = &static_cast<BaseThread *>(Thread::current())->uRandom;
	}
	LootResult r;
	if(isPack == 1)
	{
		size_t cnt = items.size();
		UInt32 rollNum = (*ur)(10000);
		for(size_t i = 0; i < cnt; ++i)
		{
			const LootData& ld = items[i];
			if(rollNum < ld.chance)
			{
				const LootItem * li = lootTable[ld.id];
				if(li != NULL)
				{
                    std::vector<LootResult> lr1;
					li->roll(lr1, ur, bind || m_bind);
					if(!ld.counts.empty() && lr1.size())
					{
						UInt32 countR = (*ur)(10000);
						size_t ccnt = ld.counts.size();
						for(size_t j = 0; j < ccnt; ++ j)
						{
							if(ld.counts[j].chance > countR)
							{
                                for (size_t k = 0; k < lr1.size(); ++k)
                                {
                                    lr1[k].count = ld.counts[j].count;
                                }
								break;
							}
							countR -= ld.counts[j].chance;
						}
					}
                    if (lr1.size())
                        lr.insert(lr.end(), lr1.begin(), lr1.end());
				}
				return;
			}
			rollNum -= items[i].chance;
		}
		return;
	}
    else if (isPack == 2)
    {
		size_t cnt = items.size();
		UInt32 rollNum = (*ur)(10000);
		for(size_t i = 0; i < cnt; ++ i)
		{
			const LootData& ld = items[i];
			if(rollNum < ld.chance)
            {
                size_t lcnt = ld.counts.size();
                for (size_t l = 0; l < lcnt; ++l)
                {
                    r.id = ld.counts[l].count;
                    r.bind = bind || m_bind;
                    r.count = 1;
                    lr.push_back(r);
                }
                return;
            }
			rollNum -= items[i].chance;
        }
        return;
    }
	r.id = items[(*ur)(items.size())].id;
    r.bind = bind || m_bind;
	r.count = 1;
    lr.push_back(r);
	return;
}

}
