
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
    time_t now2 = static_cast<time_t>(now);
    struct tm * t = localtime(&now2);
    if (t->tm_hour == 19)
        return true;
    return false;
}

void HeroIsland::process(UInt32 now)
{
    if (isActiveTime(now))
    {
        _running = true;
    }
    else
    {
        _running = false;
    }

    if (_running)
    {
    }
}

UInt8 HeroIsland::getIdentity(Player* player)
{
    if (!player)
        return 0;

    if (player->GetLev() < 40)
    {
        return 0;
    }

    if (player->getThreadId() != COUNTRY_NEUTRAL)
        return 0;

    UInt8 type = 0;
    if (player->hasFlag(Player::InHeroIsland))
    {
        type = player->getHIType();
    }
    else if (_types[0] < _types[1])
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

    player->setHIType(type);

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
    {
        sendSpot(pd, rspot);
        return true;
    }

    pd = new(std::nothrow) HIPlayerData;
    if (!pd) return false;
    pd->player = player;
    return enter(pd, type, spot, movecd);
}

bool HeroIsland::enter(HIPlayerData* pd, UInt8 type, UInt8 spot, bool movecd)
{
    if (!pd || type > 3 || (spot != 0xFF && spot > HERO_ISLAND_SPOTS))
        return false;

    if (spot == 0xFF)
        spot = 0;

    pd->type = type;
    pd->spot = spot;
    _players[spot].push_back(pd);

    if (movecd)
    {
        pd->movecd = TimeUtil::Now() + 40;
        pd->player->setBuffData(PLAYER_BUFF_HIMOVE, pd->movecd);
    }

    pd->player->setHISpot(spot);
    sendSpot(pd, spot);
    return true;
}

void HeroIsland::sendSpot(HIPlayerData* pd, UInt8 spot)
{
    if (!pd)
        return;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(4) << spot << Stream::eos;
    pd->player->send(st);

    sendPlayers(pd, spot, 0, HERO_ISLANG_PAGESZ);
    sendSkills(pd);
    broadcast(pd, spot);
}

void HeroIsland::sendPlayers(HIPlayerData* pd, UInt8 spot, UInt16 start, UInt8 pagesize)
{
    if (!pd || !pd->player || spot > HERO_ISLAND_SPOTS)
        return;
    
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(6);
    size_t sz = _players[spot].size();

    if (start > sz)
        start = 0;

    if (sz)
        st << static_cast<UInt8>(sz-1);
    else
        st << static_cast<UInt8>(sz);

    for (size_t i = start; i < sz; ++i)
    {
        HIPlayerData* pd1 = _players[spot][i];
        if (pd1->player != pd->player)
        {
            st << pd1->player->getId();
            st << pd1->type;
            st << pd1->player->allHpP();
            st << pd1->player->GetLev();
            st << pd1->player->getName();
        }
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
    broadcast(st, spot, pd->player);
}

void HeroIsland::broadcast(Stream& st, UInt8 spot, Player* player)
{
    if (spot > HERO_ISLAND_SPOTS)
        return;

    size_t sz = _players[spot].size();
    for (size_t i = 0; i < sz; ++i)
    {
        Player* player1 = _players[spot][i]->player;
        if (player && player != player1 && player1->hasFlag(Player::InHeroIsland))
            player1->send(st);
    }
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
    st << static_cast<UInt8>(7) << static_cast<UInt8>(1) << pd->player->getId() << Stream::eos;
    broadcast(st, pd->spot, pd->player);

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

bool HeroIsland::moveTo(Player* player, UInt8 to, bool movecd)
{
    if (!player || to > HERO_ISLAND_SPOTS)
        return false;

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return false;
    player->setHISpot(spot);

    if (spot == to)
        return false;

    if (movecd && pd->movecd > TimeUtil::Now())
    {
        // TODO:
        return false;
    }

    if (leave(pd, spot, pos))
    {
        if (enter(pd, pd->type, to, to?true:false))
        {
            Stream st(REP::HERO_ISLAND);
            st << static_cast<UInt8>(4) << to << Stream::eos;
            player->send(st);
            return true;
        }
    }
    return false;
}

bool HeroIsland::attack(Player* player, UInt8 type, UInt64 id)
{
    if (!player || !id)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    if (player->getBuffData(PLAYER_BUFF_ATTACKING))
    {
        player->sendMsgCode(0, 1407);
        return false;
    }

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;
    player->setHISpot(spot);

    if (!pd->spot)
    {
        return false;
    }

    UInt32 now = TimeUtil::Now();
    if (type == 0) // NPC
    {
        RareAnimals& ra = findRareAnimal(id, pd->spot);
        if (!ra.id)
            return false;

        if (ra.cd > now)
        {
            // TODO:
            return false;
        }

        ra.cd = now + ra.cdend;
        if (pd->player->attackRareAnimal(ra.id))
        {
            // TODO:
        }

        return true;
    }
    else if (type == 1) // Player
    {
        if (pd->fightcd > now)
        {
            // TODO:
            return false;
        }

        UInt8 spot = pd->spot;
        UInt8 pos = 0;
        HIPlayerData* pd1 = findPlayer(id, spot, pos);
        if (!pd1)
        {
            return false;
        }

        if (pd == pd1 || pd->player == pd1->player)
        {
            return false;
        }

        if (!pd1->spot)
        {
            return false;
        }

        int turns = 0;
        bool res = player->challenge(pd1->player, NULL, &turns, false, 0, true);
        pd->fightcd = now + 30;
        pd->player->setBuffData(PLAYER_BUFF_HIFIGHT, pd->fightcd);

        if (res)
        {

            pd->lasttype = pd1->type;
            moveTo(pd1->player, 0, false);
            pd1->injuredcd = now + 30;
            pd1->player->setBuffData(PLAYER_BUFF_HIWEAK, pd1->injuredcd);

            size_t sz = pd->compass.size();
            if (sz && pd->compass[sz-1].type == pd1->type)
            {
                pd->compass[sz-1].status = 2;

                Stream st(REP::HERO_ISLAND);
                st << static_cast<UInt8>(5) << static_cast<UInt8>(2) << Stream::eos;
                pd->player->send(st);
            }
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

    if (!player->hasFlag(Player::InHeroIsland))
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

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    UInt8 in = 0;
    UInt8 type = 0;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(0);

    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (pd)
    {
        in = 1;
        type = pd->type;
        player->setHISpot(spot);
    }
    else
    {
        type = player->getHIType();
    }

    st << in;
    st << type;

    if (in)
    {
        st << pd->spot;
        st << pd->straight;

        size_t sz = pd->compass.size();
        if (sz)
        {
            size_t i = ((sz-1)/3)*3;
            for (; i < sz; ++i)
                st << pd->compass[i].type;

            if (i%3)
            {
                for (size_t j = 0; j < (3-(i%3)); ++j)
                {
                    st << static_cast<UInt8>(0);
                }
            }

            st << pd->compass[i].status;
        }
        else
        {
            st << static_cast<UInt32>(0);
        }
    }

    st << Stream::eos;
    player->send(st);
}

void HeroIsland::playerEnter(Player* player)
{
    if (!player)
        return;

    if (player->GetLev() < 40)
    {
        return;
    }

    if (!player->getHIType())
        return;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(1);
    if (enter(player, player->getHIType(), player->getHISpot(), false))
    {
        st << static_cast<UInt8>(0);
        player->addFlag(Player::InHeroIsland);
    }
    else
        st << static_cast<UInt8>(1);
    st << Stream::eos;
    player->send(st);
}

void HeroIsland::playerLeave(Player* player)
{
    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;
    if (leave(pd, spot, pos))
        _players[0].push_back(pd);
    player->delFlag(Player::InHeroIsland);
    player->setHISpot(0xFF);
}

void HeroIsland::listRank(Player* player, UInt16 start, UInt8 pagesize)
{
}

void HeroIsland::startCompass(Player* player)
{
    if (!player)
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;
    player->setHISpot(spot);

    size_t sz = pd->compass.size();
    if (sz)
    {
        if (pd->compass[sz-1].status != 3)
            return;
    }

    UInt8 type = uRand(4);
    if (!type)
        type = 1;
    pd->compass.push_back(Task(type, 1));

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(3);
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

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;
    player->setHISpot(spot);

    size_t sz = pd->compass.size();
    if (!sz)
        return;

    if (pd->compass[sz-1].status != 2)
        return;

    if (sz == 1)
        pd->straight = 1;
    else
    {
        if (pd->compass[sz-1].type == pd->compass[sz-2].type)
            ++pd->straight;
    }
    pd->compass[sz-1].status = 3;

    if (!(sz % 3))
    {
        if (pd->straight == sz)
        {
            // TODO:
        }
        else
        {
            pd->compass.clear();
        }
    }
    else
    {
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(5) << static_cast<UInt8>(3) << Stream::eos;
    player->send(st);
}

HeroIsland heroIsland;

}

