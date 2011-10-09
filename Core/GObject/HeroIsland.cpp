
#include "HeroIsland.h"
#include "Server/SysMsg.h"
#include "Common/Random.h"
#include "MsgID.h"

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

bool HeroIsland::isActiveTime(UInt32 now)
{
    return false;
}

void HeroIsland::process(UInt32 now)
{
    if (isActiveTime(now))
    {
    }
    else
    {
    }
}

UInt8 HeroIsland::getIdentity(Player* player)
{
    if (!player)
        return 0;

    if (player->getThreadId() != COUNTRY_NEUTRAL)
        return 0;

    UInt8 type = 0;
    if (_types[0] < _types[1])
    {
        if (_types[2] < _types[0])
        {
            ++_types[2];
            type = 3;
        }
        else
        {
            ++_types[0];
            type = 1;
        }
    }
    else if (_types[1] < _types[2])
    {
        ++_types[1];
        type = 2;
    }
    else
    {
        ++_types[2];
        type = 3;
    }

    if (!enter(player, type, 0, false))
        type = 0;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(2) << type << Stream::eos;
    player->send(st);

    SYSMSG_SENDV(2116, player, type);
    return type;
}

HIPlayerData* HeroIsland::findPlayer(Player* player, UInt8& spot, UInt8& pos)
{
    if (!player || (spot > HERO_ISLAND_SPOTS && spot != 0xFF))
        return NULL;

    UInt8 start = 0;
    UInt8 end = 0;
    if (spot == 0xFF)
    {
        start = 0;
        end = HERO_ISLAND_SPOTS;
    }
    else
    {
        start = spot;
        end = spot+1;
    }

    for (UInt8 j = start; j < end; ++j)
    {
        size_t sz = _players[j].size();
        for (size_t i = 0; i < sz; ++i)
        {
            if (_players[j][i]->player == player)
            {
                spot = j;
                pos = i;
                return _players[j][i];
            }
        }
    }
    return NULL;
}

HIPlayerData* HeroIsland::findPlayer(UInt64 id, UInt8& spot, UInt8& pos)
{
    if (!id || (spot > HERO_ISLAND_SPOTS && spot != 0xFF))
        return NULL;

    UInt8 start = 0;
    UInt8 end = 0;
    if (spot == 0xFF)
    {
        start = 0;
        end = HERO_ISLAND_SPOTS;
    }
    else
    {
        start = spot;
        end = spot+1;
    }

    for (UInt8 j = start; j < end; ++j)
    {
        size_t sz = _players[j].size();
        for (size_t i = 0; i < sz; ++i)
        {
            if (_players[j][i]->player && _players[j][i]->player->getId() == id)
            {
                spot = j;
                pos = i;
                return _players[j][i];
            }
        }
    }
    return NULL;
}

RareAnimals& HeroIsland::findRareAnimal(UInt32 id, UInt8 spot)
{
    static RareAnimals null;
    if (!id || spot > HERO_ISLAND_SPOTS)
        return null;

    size_t sz = _animals[spot].size();
    for (size_t j = 0; j < sz; ++j)
    {
        if (_animals[spot][j].id == id)
        {
            return _animals[spot][j];
        }
    }
    return null;
}

bool HeroIsland::enter(Player* player, UInt8 type, UInt8 spot, bool movecd)
{
    if (!player || type > 3)
        return false;

    if (player->getThreadId() != COUNTRY_NEUTRAL)
        return false;

    UInt8 pos = 0;
    UInt8 rspot = spot;
    HIPlayerData* pd = findPlayer(player, rspot, pos);
    if (pd)
        return true;

    pd = new(std::nothrow) HIPlayerData;
    if (!pd) return false;
    pd->player = player;
    player->setHISpot(spot);
    return enter(pd, type, spot, movecd);
}

bool HeroIsland::enter(HIPlayerData* pd, UInt8 type, UInt8 spot, bool movecd)
{
    if (!pd || type > 3 || spot > HERO_ISLAND_SPOTS)
        return false;

    pd->type = type;
    pd->spot = spot;
    _players[spot].push_back(pd);

    if (movecd)
        pd->movecd = TimeUtil::Now() + 40;

    sendPlayers(pd, spot, 0, HERO_ISLANG_PAGESZ);
    sendSkills(pd);
    broadcast(pd, spot);

    return true;
}

void HeroIsland::sendPlayers(HIPlayerData* pd, UInt8 spot, UInt16 start, UInt8 pagesize)
{
    if (!pd || !pd->player || spot > HERO_ISLAND_SPOTS)
        return;
    
    Stream st(REP::HERO_ISLAND);
    size_t sz = _players[spot].size();

    if (start > sz)
        start = 0;

    st << static_cast<UInt8>(sz);
    for (size_t i = start; i < sz; ++i)
    {
        HIPlayerData* pd1 = _players[spot][i];
        st << pd1->player->getId();
        st << pd1->type;
        st << pd1->player->allHpP();
        st << pd1->player->GetLev();
        st << pd1->player->getName();
    }
    st << Stream::eos;
    pd->player->send(st);
}

void HeroIsland::sendRareAnimals(HIPlayerData* pd, UInt8 spot)
{
    if (spot > HERO_ISLAND_SPOTS)
        return;

    Stream st;
    size_t sz = _animals[spot].size();
    for (size_t i = 0; i < sz; ++i)
    {
        // TODO:
    }
    st << Stream::eos;
    pd->player->send(st);
}

void HeroIsland::sendSkills(HIPlayerData* pd)
{
    if (!pd || !pd->player)
        return;
    Stream st;
    // TODO:
    st << Stream::eos;
    pd->player->send(st);
}

void HeroIsland::broadcast(HIPlayerData* pd, UInt8 spot)
{
    if (!pd || spot > HERO_ISLAND_SPOTS)
        return;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(7);
    st << static_cast<UInt8>(0);
    st << pd->player->getId();
    st << pd->type;
    st << pd->player->allHpP();
    st << pd->player->GetLev();
    st << pd->player->getName();
    st << Stream::eos;
    broadcast(st, spot);
}

void HeroIsland::broadcast(Stream& st, UInt8 spot)
{
    if (spot > HERO_ISLAND_SPOTS)
        return;

    size_t sz = _players[spot].size();
    for (size_t i = 0; i < sz; ++i)
        _players[spot][i]->player->send(st);
}

void HeroIsland::broadcast(Stream& st)
{
    for (UInt8 i = 0; i < HERO_ISLAND_SPOTS; ++i)
        broadcast(st, i);
}

HIPlayerData* HeroIsland::leave(Player* player, UInt8 spot)
{
    if (!player || spot > HERO_ISLAND_SPOTS)
        return NULL;

    UInt8 rspot = spot;
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, rspot, pos);
    if (!pd) return NULL;
    return leave(pd, rspot, pos);
}

HIPlayerData* HeroIsland::leave(HIPlayerData* pd, UInt8 spot, UInt8 pos)
{
    if (!pd)
        return NULL;

    Stream st(REP::HERO_ISLAND);
    st << pd->player->getId() << Stream::eos;
    broadcast(st, pd->spot);
    _players[spot].erase(_players[spot].begin()+pos);
    return pd;
}

void HeroIsland::listPlayers(Player* player, UInt8 spot, UInt16 start, UInt8 pagesize)
{
    if (!player)
        return;

    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (pd)
        sendPlayers(pd, spot, start, pagesize);
}

bool HeroIsland::moveTo(Player* player, UInt8 from, UInt8 to)
{
    if (!player || from > HERO_ISLAND_SPOTS || to > HERO_ISLAND_SPOTS)
        return false;

    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, from, pos);
    if (!pd)
        return false;

    if (pd->movecd > TimeUtil::Now())
    {
        // TODO:
        return false;
    }

    if (leave(pd, from, pos))
    {
        if (enter(pd, pd->type, to))
        {
            Stream st(REP::HERO_ISLAND);
            st << static_cast<UInt8>(4) << to << Stream::eos;
            player->send(st);
        }
    }
    return false;
}

bool HeroIsland::moveTo(Player* player, UInt8 to)
{
    if (!player || to > HERO_ISLAND_SPOTS)
        return false;

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return false;

    if (spot == to)
        return false;

    if (pd->movecd > TimeUtil::Now())
    {
        // TODO:
        return false;
    }

    if (leave(pd, spot, pos))
    {
        if (enter(pd, pd->type, to))
        {
            Stream st(REP::HERO_ISLAND);
            st << static_cast<UInt8>(4) << to << Stream::eos;
            player->send(st);
        }
    }
    return false;
}

bool HeroIsland::attack(Player* player, UInt8 type, UInt64 id)
{
    if (!player || !id)
        return false;

    if (player->getBuffData(PLAYER_BUFF_ATTACKING))
    {
        player->sendMsgCode(0, 1412);
        return false;
    }

    UInt8 spot = 0xFF;
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;

    if (!pd->spot)
    {
        return false;
    }

    if (pd->fightcd > TimeUtil::Now())
    {
        // TODO:
        return false;
    }

    if (type == 0) // NPC
    {
        RareAnimals& ra = findRareAnimal(id, pd->spot);
        if (!ra.id)
            return false;

        pd->fightcd = TimeUtil::Now() + 30;
        if (pd->player->attackRareAnimal(ra.id))
        {
            // TODO:
        }

        return true;
    }
    else if (type == 1) // Player
    {
        UInt8 spot = pd->spot;
        UInt8 pos = 0;
        HIPlayerData* pd1 = findPlayer(id, spot, pos);
        if (!pd1)
        {
            return false;
        }

        if (!pd1->spot)
        {
            return false;
        }

        int turns = 0;
        bool res = player->challenge(pd1->player, NULL, &turns, false, 50);
        player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + 2 * turns);
        pd1->player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + 2 * turns);
        pd->fightcd = TimeUtil::Now() + 30;

        // TODO: 奖励
        if (res)
        {

            pd->lasttype = pd1->type;
            moveTo(pd1->player, pd1->spot, 0);
            pd1->fightcd = TimeUtil::Now() + 30;

            size_t sz = pd->compass.size();
            if (sz && pd->compass[sz-1].type == pd1->type)
                pd->compass[sz-1].status = 1;
        }
        else
        {
        }

        return true;
    }
    return false;
}

bool HeroIsland::useSkill(Player* player, UInt8 spot)
{
    if (!player)
        return false;

    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;
    if (pd->spot)
        return false;

    // TODO:

    return false;
}

void HeroIsland::playerInfo(Player* player)
{
    if (!player)
        return;
    UInt8 spot = 0xFF;
    UInt8 pos = 0;
    UInt8 in = 0;
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(0);
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (pd)
        in = 1;
    st << in;

    if (in)
    {
        st << pd->type;
        st << pd->spot;
        st << pd->straight;

        size_t sz = pd->compass.size();
        if (sz)
        {
            size_t i = ((sz-1)/3)*3;
            for (; i < sz; ++i)
                st << pd->compass[i].type;
            st << pd->compass[i].status;
        }
    }

    st << Stream::eos;
    player->send(st);
}

void HeroIsland::playerEnter(Player* player)
{
    if (!player)
        return;

    UInt8 spot = 0xFF;
    UInt8 pos = 0;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(1);
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
    {
        st << static_cast<UInt8>(0);
    }
    else
    {
        st << static_cast<UInt8>(1);
    }
    st << Stream::eos;
    player->send(st);
}

void HeroIsland::playerLeave(Player* player)
{
    UInt8 spot = 0xFF;
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;
    leave(pd, spot, pos);
    delete pd;
}

void HeroIsland::listRank(Player* player, UInt16 start, UInt8 pagesize)
{
}

void HeroIsland::startCompass(Player* player)
{
    if (!player)
        return;

    UInt8 spot = 0xFF;
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(3);
    size_t sz = pd->compass.size();
    if (sz)
    {
        if (pd->compass[sz-1].status != 2)
        {
            st << static_cast<UInt8>(0) << Stream::eos;
            player->send(st);
            return;
        }
    }

    UInt8 type = uRand(4);
    if (!type)
        type = 1;
    pd->compass.push_back(Task(type, 0));

    st << type << Stream::eos;
    player->send(st);
}

void HeroIsland::stopCompass(Player* player)
{
    if (!player)
        return;
}

void HeroIsland::commitCompass(Player* player)
{
    if (!player)
        return;
    UInt8 spot = 0;
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;

    Stream st(REP::HERO_ISLAND);
    size_t sz = pd->compass.size();

    if (pd->compass[sz-1].status == 0 || pd->compass[sz-1].status == 2)
    {
        st << static_cast<UInt8>(6) << static_cast<UInt8>(0) << Stream::eos;
        player->send(st);
        return;
    }

    if (sz && !(sz % 3))
    {
        bool same = true;
        for (size_t i = 0; i < sz - 1; ++i)
        {
            if (pd->compass[i].type != pd->compass[i+1].type)
            {
                same = false;
                break;
            }
            ++pd->straight;
        }

        pd->compass[sz-1].status = 2;
        if (same)
        {
            // TODO:
        }
        else
        {
            pd->compass.clear();
        }
    }
    else if (sz)
    {
        pd->compass[sz-1].status = 2;
    }

    st << static_cast<UInt8>(6) << static_cast<UInt8>(0) << Stream::eos;
    player->send(st);
}

HeroIsland heroIsland;

}

