
#include "HeroIsland.h"
#include "Server/SysMsg.h"
#include "Common/Random.h"
#include "GObject/Package.h"
#include "Server/Cfg.h"
#include "MsgID.h"

namespace GObject
{

std::vector<RareAnimals> HeroIsland::_animals[HERO_ISLAND_SPOTS];
std::vector<Awards> HeroIsland::_awards;

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
    ra.cdlong = cd;
    ra.cd = 0;
    _animals[spot].push_back(ra);
}

class Sort 
{    
    public:
        bool operator()(Awards a, Awards b)
        {    
            return a.prob < b.prob;
        }    
};   

void HeroIsland::addHIAwardsCfg(UInt32 id, UInt32 num, UInt32 prob)
{
    if (!id)
    {
        std::sort(_awards.begin(), _awards.end(), Sort());
        size_t sz = _awards.size();
        for (size_t i = 1; i < sz; ++i)
            _awards[i].prob += _awards[i-1].prob;
        return;
    }
    _awards.push_back(Awards(id, num, prob));
}

bool HeroIsland::isRareAnimal(UInt32 npcid)
{
    for (size_t i = 1; i < HERO_ISLAND_SPOTS; ++i)
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

    applayHP();
    applayRareAnimals();
}

void HeroIsland::applayHP()
{
    UInt32 now = TimeUtil::Now();
    size_t sz = _players[0].size();
    for (size_t i = 0; i < sz; ++i)
    {
        HIPlayerData* pd = _players[0][i];
        if (pd && pd->player && pd->injuredcd <= now)
        {
            pd->player->regenAll(true);
            pd->injuredcd = static_cast<UInt32>(-1);
            broadcast(pd, 0, 2);
        }
    }
}

void HeroIsland::applayRareAnimals()
{
    return; // XXX: 不需要更新，只在攻击变化时更新
    static UInt32 count = 0;
    if (!(count++ % 2))
        return;

    UInt32 now = TimeUtil::Now();
    for (size_t i = 1; i < HERO_ISLAND_SPOTS; ++i)
    {
        size_t sz = _animals[i].size();
        for (size_t j = 0; j < sz; ++j)
        {
            RareAnimals& ra = _animals[i][j];
            Stream st(REP::HERO_ISLAND);
            st << static_cast<UInt8>(10);
            st << ra.id;
            st << static_cast<UInt16>(ra.cd > now ? ra.cd - now : 0);
            st << Stream::eos;
            broadcast(st, i);
        }
    }
}

UInt8 HeroIsland::getIdentity(Player* player, bool rand)
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
    if (rand)
    {
        type = player->getHIType();
        if (type && _types[type-1])
            --_types[type-1];

        type = (uRand(100) % 3) + 1;
        ++_types[type];
    }
    else
    {
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
    if (!id || !spot || spot > HERO_ISLAND_SPOTS-1)
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
        if (cfg.GMCheck)
            pd->movecd = TimeUtil::Now() + 40;
        else
            pd->movecd = TimeUtil::Now() + 3;
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
    if (!spot)
        sendSkills(pd);
    else
        sendRareAnimals(pd, spot);
    broadcast(pd, spot, 0);
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
        st << static_cast<UInt16>(sz-1);
    else
        st << static_cast<UInt16>(sz);

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
    if (!pd || spot > HERO_ISLAND_SPOTS)
        return;

    UInt32 now = TimeUtil::Now();
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(9);
    size_t sz = _animals[spot].size();
    st << static_cast<UInt8>(sz);
    for (size_t i = 0; i < sz; ++i)
    {
        RareAnimals& ra = _animals[spot][i];
        st << ra.id;
        st << static_cast<UInt16>(ra.cd > now ? ra.cd - now : 0);
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

void HeroIsland::broadcast(HIPlayerData* pd, UInt8 spot, UInt8 type)
{
    if (!pd || spot > HERO_ISLAND_SPOTS)
        return;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(7);
    st << type;
    if (type == 0)
    {
        st << pd->player->getId();
        st << pd->type;
        st << pd->player->allHpP();
        st << pd->player->GetLev();
        st << pd->player->getName();
    }
    else if (type == 2)
    {
        st << pd->player->getId();
        st << pd->player->allHpP();
    }
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
        if ((!player || (player && player != player1)) && player1 && player1->hasFlag(Player::InHeroIsland))
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

    UInt32 now = TimeUtil::Now();
    if (movecd && pd->movecd > now)
    {
        // TODO:
        return false;
    }

    if (pd->injuredcd <= now)
        player->regenAll(true);

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

    if (pd->injuredcd <= now)
        player->regenAll(true);

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

        ra.cd = now + ra.cdlong;

        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(10);
        st << ra.id;
        st << static_cast<UInt16>(ra.cdlong);
        st << Stream::eos;
        broadcast(st, spot);

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
        if (cfg.GMCheck)
            pd->fightcd = now + 30;
        else
            pd->fightcd = now + 3;
        pd->player->setBuffData(PLAYER_BUFF_HIFIGHT, pd->fightcd);

        if (res)
        {

            pd->lasttype = pd1->type;
            moveTo(pd1->player, 0, false);
            if (cfg.GMCheck)
                pd1->injuredcd = now + 90;
            else
                pd1->injuredcd = now + 90;
            pd1->player->setBuffData(PLAYER_BUFF_HIWEAK, pd1->injuredcd);

            size_t sz = pd->compass.size();
            if (sz && pd->compass[sz-1].type == pd1->type)
            {
                pd->compass[sz-1].status = 2;

                if (sz == 1)
                    pd->straight = 1;
                else if (sz > 1)
                {
                    if (pd->compass[sz-1].type == pd->compass[sz-2].type)
                        ++pd->straight;
                }

                Stream st(REP::HERO_ISLAND);
                st << static_cast<UInt8>(5) << static_cast<UInt8>(2) << pd->straight << Stream::eos;
                pd->player->send(st);
            }

            broadcast(pd, pd->spot, 2);
        }
        else
        {
            moveTo(pd->player, 0, false);
            if (cfg.GMCheck)
                pd->injuredcd = now + 90;
            else
                pd->injuredcd = now + 90;
            pd->player->setBuffData(PLAYER_BUFF_HIWEAK, pd->injuredcd);

            broadcast(pd1, pd1->spot, 2);
        }

        return true;
    }
    return false;
}

bool HeroIsland::useSkill(Player* player, UInt16 skillid)
{
    if (!player)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    UInt8 spot = player->getHISpot();
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

            st << pd->compass[i-1].status;
        }
        else
        {
            st << static_cast<UInt32>(0);
        }

        st << static_cast<UInt8>(pd->awardgot==1?1:0);
        st << pd->inrank;
        st << pd->score;
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
    if (!player)
        return;

    size_t sz = _sorts.size();
    if (start > sz)
        return;

    if (pagesize > sz)
        pagesize = sz;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(13);
    st << start;
    st << static_cast<UInt16>(sz);

    size_t off = st.size();
    st << static_cast<UInt8>(0);

    UInt32 jump = start;
    UInt32 count = 0;
    for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
    {
        if (jump)
        {
            --jump;
            continue;
        }

        st << (*i)->player->getName();
        st << (*i)->player->GetLev();
        st << (*i)->score;

        ++count;
        if (count >= pagesize)
            break;
    }

    st.data<UInt16>(off) = count;
    st << Stream::eos;
    player->send(st);
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

    if (pd->awardgot == 2)
        pd->awardgot = 0;

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

    pd->compass[sz-1].status = 3;

    UInt32 score = pd->score;


    if (!(sz % 3))
    {
        pd->awardgot = 1;

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

    // TODO:
    pd->score += 1;

    if (score != pd->score)
    {
        _sorts.insert(pd);

        pd->inrank = 0;
        for (SortType::iterator i = _sorts.begin(), e = _sorts.end(); i != e; ++i)
        {
            if (*i == pd)
            {
                ++pd->inrank;
                break;
            }
        }

        if (_sorts.size() > 100)
        {
            (*_sorts.begin())->inrank = 0;
            _sorts.erase(_sorts.begin());
        }

        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(14) << pd->inrank << pd->score << Stream::eos;
        player->send(st);
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(5) << static_cast<UInt8>(3) << pd->straight
        << static_cast<UInt8>(pd->awardgot==1?1:0) << Stream::eos;
    player->send(st);
}

bool HeroIsland::getAward(Player* player, UInt8 id, UInt8 type)
{
    if (!player || id > 4)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return false;
    player->setHISpot(spot);

    if (type == 1)
    {
        if (!pd->awardgot || pd->awardgot == 2)
            return false;

        UInt8 sz = _awards.size();
        if (sz < 5)
            return false;

        struct Award
        {
            UInt16 id;
            UInt8 num;
        };

        if (player->GetPackage()->IsFull())
        {
            player->sendMsgCode(0, 1011);
            return false;
        }

        UInt32 total = _awards[sz-1].prob;
        Award awards[5] = {{0,},};
        Awards* awds = &_awards[0];
        for (UInt8 j = 0; j < 5; ++j)
        {
            UInt32 v = uRand(total);
            for (UInt8 i = 0; i < sz; ++i, ++awds)
            {
                if (v < awds->prob)
                {
                    awards[j].id = awds->id;
                    awards[j].num = awds->num;
                    break;
                }
            }
        }

        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(8);
        st << static_cast<UInt8>(5);
        for (UInt8 i = 0; i < 5; ++i)
        {
            st << awards[i].id;
            st << awards[i].num;
        }
        st << Stream::eos;
        player->send(st);

        if (awards[id].id && awards[id].num)
        {
            if (awards[id].id == 0)
            {
                player->getTael(awards[id].num);
            }
            else if (awards[id].id == 1)
            {
                player->getCoupon(awards[id].num);
            }
            else
            {
                player->GetPackage()->Add(awards[id].id, awards[id].num, true, true);
            }
            pd->awardgot = 2;
        }
        else
            return false;
    }
    else
    {
    }

    return true;
}

HeroIsland heroIsland;

}

