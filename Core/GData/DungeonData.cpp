#include "Config.h"
#include "DungeonData.h"
#include "LootTable.h"
#include "GObject/Player.h"
#include "GObject/Package.h"

namespace GData
{

ObjectListT<DungeonMonster> dungeonMonsters;
ObjectListT<DungeonData> dungeons;

void DungeonLevel::getLoot( GObject::Player * player, UInt32 specialItem, UInt8 gemDecrease ) const
{
	std::vector<const LootItem *>::const_iterator it;
	bool replace = false;
	if(specialItem != 0)
	{
		gemDecrease = 100;//for weekday Saturday activity
		if(loots.size() >= 4)
			replace = true;
		else
		{
			LootResult lr;
			lr.id = specialItem;
			lr.count = 1;
			player->GetPackage()->Add(specialItem, 1, true, true, FromDungeon);
			player->_lastLoot.push_back(lr);
		}
	}
	for(it = loots.begin(); it != loots.end(); ++ it)
	{
        if (!(*it))
            continue;

#if 0
        LootResult lr = (*it)->roll();
        const ItemBaseType * ibt = itemBaseTypeManager[lr.id];
        if(replace)
        {
            if(ibt != NULL && ibt->quality < 5)
            {
                lr.id = specialItem;
                lr.count = 1;
                replace = false;
                specialItem = 0;
                player->GetPackage()->Add(lr.id, lr.count, true, true, FromDungeon);//only use for weekday Saturday activity
                player->_lastLoot.push_back(lr);
                lr.id = 0;
            }
        }
        if(lr.id == 0)
            continue;
        else if(gemDecrease > 0 && IsGemId(lr.id))
        {
            if(uRand(100) < gemDecrease)
            {
                lr.id = 0;
                continue;
            }
        }
        player->GetPackage()->Add(lr.id, lr.count, lr.id > 5000 && ibt->bindType > 0, true, FromDungeon);
        player->_lastLoot.push_back(lr);
#else
        std::vector<LootResult> lr;
        (*it)->roll(lr);
        for (size_t j = 0; j < lr.size(); ++j)
        {
            const ItemBaseType * ibt = itemBaseTypeManager[lr[j].id];
            if(replace)
            {
                if(ibt != NULL && ibt->quality < 5)
                {
                    lr[j].id = specialItem;
                    lr[j].count = 1;
                    replace = false;
                    specialItem = 0;
                    player->GetPackage()->Add(lr[j].id, lr[j].count, lr[j].bind, true, FromDungeon);//only use for weekday Saturday activity
                    player->_lastLoot.push_back(lr[j]);
                    lr[j].id = 0;
                }
            }
            if(lr[j].id == 0)
                continue;
            else if(gemDecrease > 0 && IsGemId(lr[j].id))
            {
                if(uRand(100) < gemDecrease)
                {
                    lr[j].id = 0;
                    continue;
                }
            }
            player->GetPackage()->Add(lr[j].id, lr[j].count, lr[j].bind, true, FromDungeon);
            player->_lastLoot.push_back(lr[j]);
        }
#endif
	}
}

DungeonData::~DungeonData()
{
    for(size_t i = 0; i < monsters.size(); ++ i)
    {
        delete monsters[i];
    }
}

}

