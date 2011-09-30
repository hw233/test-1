
#include "HeroIsland.h"
#include "Common/Stream.h"

namespace GObject
{

std::vector<RareAnimals> HeroIsland::_animals[HERO_ISLAND_SPOTS];

void HeroIsland::setRareAnimals(UInt8 spot, UInt32 npcid, Table attr, UInt32 last, UInt32 cd)
{
    if (spot > HERO_ISLAND_SPOTS)
        return;

    RareAnimals ra;
    ra.attr.strengthP = attr.get<UInt8>("strength")/(float)100;
    ra.attr.physiqueP = attr.get<UInt8>("physique")/(float)100;
    ra.attr.agilityP = attr.get<UInt8>("agility")/(float)100;
    ra.attr.intelligenceP = attr.get<UInt8>("intelligence")/(float)100;
    ra.attr.willP = attr.get<UInt8>("will")/(float)100;
    ra.attr.soulP = attr.get<UInt8>("soul")/(float)100;
    ra.attr.auraP = attr.get<UInt8>("aura");
    ra.attr.auraMaxP = attr.get<UInt8>("auraMax");
    ra.attr.attackP = attr.get<UInt8>("attack")/(float)100;
    ra.attr.magatkP = attr.get<UInt8>("magatk")/(float)100;
    ra.attr.defendP = attr.get<UInt8>("defend")/(float)100;
    ra.attr.magdefP = attr.get<UInt8>("magdef")/(float)100;
    ra.attr.hpP = attr.get<UInt8>("hp")/(float)100;
    ra.attr.toughP = attr.get<UInt8>("tough")/(float)100;
    ra.attr.actionP = attr.get<UInt8>("action")/(float)100;
    ra.attr.hitrateP = attr.get<UInt8>("hitrate")/(float)100;
    ra.attr.evadeP = attr.get<UInt8>("evade")/(float)100;
    ra.attr.criticalP = attr.get<UInt8>("critical")/(float)100;
    ra.attr.criticaldmgP = attr.get<UInt8>("criticaldmg")/(float)100;
    ra.attr.pierceP = attr.get<UInt8>("pierce")/(float)100;
    ra.attr.counterP = attr.get<UInt8>("counter")/(float)100;
    ra.attr.magresP = attr.get<UInt8>("magres")/(float)100;

    ra.id = npcid;
    ra.last = last;
    ra.cd = cd;
    ra.cdend = 0;
    _animals[spot-1].push_back(ra);
}

bool HeroIsland::isRareAnimal(UInt32 npcid)
{
    for (size_t i = 0; i < HERO_ISLAND_SPOTS; ++i)
    {
        size_t sz = _animals[i].size();
        for (size_t j = 0; j < sz; ++j)
        {
            if (_animals[i][j].id == npcid)
                return true;
        }
    }
    return false;
}

void HeroIsland::process(UInt32 now)
{
}

void HeroIsland::getIdentity(Player* player)
{
    if (!player)
        return;

    UInt8 type = 0;
    if (_types[0] < _types[1])
    {
        if (_types[2] < _types[0])
        {
            ++_types[2];
            type = 2;
        }
        else
        {
            ++_types[0];
            type = 0;
        }
    }
    else if (_types[1] < _types[2])
    {
        ++_types[1];
        type = 1;
    }
    else
    {
        ++_types[2];
        type = 2;
    }
}

HIPlayerData* HeroIsland::findPlayer(Player* player, UInt8 spot)
{
    if (!player || spot > HERO_ISLAND_SPOTS)
        return NULL;
    size_t sz = _players[spot].size();
    for (size_t i = 0; i < sz; ++i)
    {
        if (_players[spot][i]->player == player)
            return _players[spot][i];
    }
    return NULL;
}

void HeroIsland::enter(Player* player)
{
    if (!player)
        return;

    if (player->getThreadId() != COUNTRY_NEUTRAL)
    {
        return;
    }

    HIPlayerData* pd = findPlayer(player);
    if (!pd)
    {
        pd = new(std::nothrow) HIPlayerData;
        if (!pd)
            return;

        _players[0].push_back(pd);
    }
    else
    {
    }

    pd->straight = 0;

    sendPlayers(player);
}

void HeroIsland::sendPlayers(Player* player, UInt8 spot)
{
    if (spot > HERO_ISLAND_SPOTS)
        return;
    
    Stream st(0);
    size_t sz = _players[spot].size();
    for (size_t i = 0; i < sz; ++i)
    {
    }
    st << Stream::eos;
    player->send(st);
}

void HeroIsland::leave(Player* player)
{
    if (!player)
        return;
}

void HeroIsland::listPlayers(Player* player)
{
    if (!player)
        return;
}

void HeroIsland::moveTo(Player* player, UInt8 spot)
{
    if (!player || !spot)
        return;
}

void HeroIsland::attack(Player* player, UInt8 spot, UInt8 type, UInt64 id)
{
    if (!player || !spot || !id)
        return;
}

HeroIsland heroIsland;

}

