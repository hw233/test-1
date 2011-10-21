
#include "HeroIsland.h"
#include "Server/SysMsg.h"
#include "Common/Random.h"
#include "GObject/Package.h"
#include "Battle/BattleSimulator.h"
#include "MsgID.h"

namespace GObject
{

inline UInt32 calcExp(UInt8 lvl)
{
    if (lvl < 10)
        lvl = 0;
    return ((lvl-10)*(lvl/10)*5+25);
}

std::vector<RareAnimals> HeroIsland::_animals[HERO_ISLAND_SPOTS];
std::vector<Awards> HeroIsland::_awards[4];
std::vector<UInt32> HeroIsland::_prestige;
GData::AttrExtra HeroIsland::_skillattr[5];

bool HeroIsland::initSkillAttr()
{
    _skillattr[1].magatkP = 0.1;
    _skillattr[1].attackP = 0.1;
    _skillattr[2].defendP = 0.1;
    _skillattr[2].magdefP = 0.1;
    _skillattr[3].aura = 25;
    return false;
}

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
    ra.attr.aura = attr.get<UInt8>("aura");
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

    switch (ra.id)
    {
        case 5489:
            ra.bufid = PLAYER_BUFF_HIRA1;
            break;
        case 5490:
            ra.bufid = PLAYER_BUFF_HIRA2;
            break;
        case 5491:
            ra.bufid = PLAYER_BUFF_HIRA3;
            break;
        case 5492:
            ra.bufid = PLAYER_BUFF_HIRA4;
            break;
        case 5500:
            ra.bufid = PLAYER_BUFF_HIRA5;
            break;
        case 5493:
            ra.bufid = PLAYER_BUFF_HIRA6;
            break;
        case 5494:
            ra.bufid = PLAYER_BUFF_HIRA7;
            break;
        case 5495:
            ra.bufid = PLAYER_BUFF_HIRA8;
            break;
        case 5496:
            ra.bufid = PLAYER_BUFF_HIRA9;
            break;
        case 5501:
            ra.bufid = PLAYER_BUFF_HIRA10;
            break;
        case 5497:
            ra.bufid = PLAYER_BUFF_HIRA11;
            break;
        case 5498:
            ra.bufid = PLAYER_BUFF_HIRA12;
            break;
        case 5499:
            ra.bufid = PLAYER_BUFF_HIRA13;
            break;
        case 5505:
            ra.bufid = PLAYER_BUFF_HIRA14;
            break;
        case 5507:
            ra.bufid = PLAYER_BUFF_HIRA15;
            break;
        case 5502:
            ra.bufid = PLAYER_BUFF_HIRA16;
            break;
        case 5503:
            ra.bufid = PLAYER_BUFF_HIRA17;
            break;
        case 5504:
            ra.bufid = PLAYER_BUFF_HIRA18;
            break;
        case 5506:
            ra.bufid = PLAYER_BUFF_HIRA19;
            break;
        case 5508:
            ra.bufid = PLAYER_BUFF_HIRA20;
            break;
    }

    std::vector<RareAnimals>::iterator
        i = _animals[spot].begin(), e = _animals[spot].end();
    for ( ; i != e; ++i)
    {
        if ((*i).id > ra.id)
        {
            _animals[spot].insert(i, ra);
            break;
        }
    }
    if (i == e)
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

void HeroIsland::addHIAwardsCfg(UInt8 quality, UInt32 id, UInt32 num, UInt32 prob)
{
    if (!quality|| quality > 4)
        return;

    if (!id)
    {
        std::sort(_awards[quality-1].begin(), _awards[quality-1].end(), Sort());
        size_t sz = _awards[quality-1].size();
        for (size_t i = 1; i < sz; ++i)
            _awards[quality-1][i].prob += _awards[quality-1][i-1].prob;
        return;
    }
    _awards[quality-1].push_back(Awards(id, num, prob));
}

void HeroIsland::addRankAwards(UInt32 prestige)
{
    _prestige.push_back(prestige);
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

void HeroIsland::restart(UInt32 now)
{
    _running = false;
    _prepareTime = now + 10;
    broadcastTV(now);
}

void HeroIsland::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime)
    {
        _prepareStep = 1;
        if (cfg.GMCheck)
        {
            _startTime = _prepareTime + 15 * 60;
            _endTime = _startTime + 60 * 60;
            _prepareTime = TimeUtil::SharpDay(1) + 18 * 60 * 60 + 45 * 60;
        }
        else
        {
            _startTime = _prepareTime + 2 * 60;
            _endTime = _startTime + 30 * 60;
            _prepareTime = now + 20 * 60;
        }
    }

    switch (_prepareStep)
    {
        case 1:
            SYSMSG_BROADCASTV(2117, 15);
            _prepareStep = 2;
            break;

        case 2:
            if (now < _startTime - 10 * 60)
                return;
            SYSMSG_BROADCASTV(2117, 10);
            _prepareStep = 3;
            break;

        case 3:
            if (now < _startTime - 5 * 60)
                return;
            SYSMSG_BROADCASTV(2117, 5);
            _prepareStep = 4;
            break;

        case 4:
            SYSMSG_BROADCASTV(2118);
            _prepareStep = 0;
            break;

        default:
            break;
    }

}

void HeroIsland::rankReward()
{
    if (!_running || !_endTime)
        return;

    size_t size = _sorts.size();
    float factor = size / (float)100;
    if (!cfg.GMCheck)
        factor = 1.0;

    UInt8 n = 0;
    UInt8 nsz = _prestige.size();
    for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
    {
        if (!(*i) || !(*i)->player)
            continue;
        if (n < nsz)
        {
            (*i)->player->getPrestige(_prestige[n] * factor + (*i)->compass.size());
            ++n;
        }
        else
            (*i)->player->getPrestige(100);
    }
}

void HeroIsland::end()
{
    rankReward();
    _endTime = 0;
    _startTime = 0;
    _running = false;
    SYSMSG_BROADCASTV(2116);
}

void HeroIsland::process(UInt32 now)
{
    if (!_prepareTime)
    {
        if (cfg.GMCheck)
            _prepareTime = TimeUtil::SharpDay(0) + 18 * 60 * 60 + 45 * 60;
        else
            _prepareTime = TimeUtil::Now() + 30;
    }

    broadcastTV(now);

    if (!_running && _startTime && now >= _startTime)
        _running = true;

    if (_running && now >= _endTime)
        end();

    applayPlayers();
    applayRareAnimals();
}

void HeroIsland::applayPlayers()
{
    UInt32 now = TimeUtil::Now();
    for (UInt8 j = 0; j < HERO_ISLAND_SPOTS; ++j)
    {
        size_t sz = _players[j].size();
        for (size_t i = 0; i < sz; ++i)
        {
            HIPlayerData* pd = _players[j][i];
            if (!j && pd && pd->player && pd->injuredcd <= now)
            {
                pd->player->regenAll(true);
                pd->injuredcd = static_cast<UInt32>(-1);
                broadcast(pd, 0, 2);
            }

            if (_running)
            {
                if (j && pd && pd->player && pd->expcd <= now)
                {
                    pd->player->AddExp(calcExp(pd->player->GetLev())*2);
                    pd->expcd = now + 60;
                }
            }

            if (pd && pd->player && now >= pd->attrcd && pd->attr)
                clearBuff(1, pd, now);

            for (UInt8 i = 0; i < 5; ++i)
            {
                if (now >= pd->skills[i].cd)
                {
                    pd->skills[i].incd = false;
                    pd->skills[i].cd = static_cast<UInt32>(-1);
                }

                if (now >= pd->skills[i].lastcd)
                {
                    if (i && pd->skills[i].bufid)
                        clearBuff(2, pd, now, i);
                }
            }
        }
    }
}

void HeroIsland::clearBuff(UInt8 type, HIPlayerData* pd, UInt32 now, UInt8 skillid)
{
    if (!pd || !pd->bufid) return;
    GData::AttrExtra attr;
    pd->player->addAttr(attr);
    if (type == 1 && pd->bufid)
    {
        pd->attrcd = static_cast<UInt32>(-1);
        pd->player->setBuffData(pd->bufid, 0);
        pd->bufid = 0;

        for (UInt8 i = 1; i < 5; ++i)
        {
            if (now < pd->skills[i].lastcd && pd->skills[i].attr)
                pd->player->addAttr(*pd->skills[i].attr);
        }
    }
    else if (type == 2 && skillid)
    {
        pd->player->setBuffData(pd->skills[skillid].bufid, 0, false);
        pd->skills[skillid].lastcd = static_cast<UInt32>(-1);
        pd->skills[skillid].attr = NULL;
        if (pd && pd->player && now < pd->attrcd && pd->attr)
            pd->player->addAttr(*pd->attr);
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
        player->sendMsgCode(0, 1010);
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

    //SYSMSG_SENDV(2116, player, type);
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
    pd->expcd = TimeUtil::Now() + 60;

    pd->skills[1].last = 2*60;
    pd->skills[2].last = 2*60;
    pd->skills[3].last = 1*60;
    pd->skills[4].last = 2*60;

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
            pd->movecd = TimeUtil::Now() + 30;
        else
            pd->movecd = TimeUtil::Now() + 30;
        pd->player->setBuffData(PLAYER_BUFF_HIMOVE, pd->movecd, false);
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

    UInt8 l1 = pd->player->GetLev();
    for (size_t i = start; i < sz; ++i)
    {
        HIPlayerData* pd1 = _players[spot][i];
        if (pd1->player && pd1->player != pd->player)
        {
            UInt8 l2 = pd1->player->GetLev();
            if (l1 < l2 && l2 - l1 > 20)
                continue;
            if (l1 > l2 && l1 - l2 > 5)
                continue;

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

    UInt32 now = TimeUtil::Now();
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(11);
    st << static_cast<UInt8>(5);
    for (UInt8 i = 0; i < 5; ++i)
    {
        st << static_cast<UInt8>(i+1);
        if (now >= pd->skills[i].cd)
            st << static_cast<UInt16>(0);
        else
            st << static_cast<UInt16>(pd->skills[i].cd - now);
    }
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
        {
            if (player && player1)
            {
                UInt8 l1 = player1->GetLev();
                UInt8 l2 = player->GetLev();
                if (l1 < l2 && l2 - l1 > 20)
                    continue;
                if (l1 > l2 && l1 - l2 > 5)
                    continue;
            }
            player1->send(st);
        }
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
        pd->player->sendMsgCode(0, 2001);
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
            pd->player->sendMsgCode(0, 2005);
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
            clearBuff(1, pd, now);
            pd->attrcd = now + ra.last;
            pd->attr = &ra.attr;
            pd->bufid = ra.bufid;
            player->addAttr(ra.attr);
            pd->player->setBuffData(ra.bufid, pd->attrcd, false);
            pd->player->sendMsgCode(0, 2119);
        }
        else
        {
            if (cfg.GMCheck)
                pd->injuredcd = now + 40;
            else
                pd->injuredcd = now + 40;
            pd->player->setBuffData(PLAYER_BUFF_HIWEAK, pd->injuredcd, false);
            moveTo(pd->player, 0, false);
        }

        return true;
    }
    else if (type == 1) // Player
    {
        if (pd->fightcd > now)
        {
            pd->player->sendMsgCode(0, 2004);
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
            pd->player->sendMsgCode(0, 2002);
            return false;
        }

        if (pd1->player->getBuffData(PLAYER_BUFF_HIJZ))
        {
            pd->player->sendMsgCode(0, 2003);
            return false;
        }

        int turns = 0;
        bool res = player->challenge(pd1->player, NULL, &turns, false, 0, true, Battle::BS_COPY5);
        if (cfg.GMCheck)
            pd->fightcd = now + 40;
        else
            pd->fightcd = now + 40;
        pd->player->setBuffData(PLAYER_BUFF_HIFIGHT, pd->fightcd, false);

        if (res)
        {

            pd->lasttype = pd1->type;
            moveTo(pd1->player, 0, false);
            if (cfg.GMCheck)
                pd1->injuredcd = now + 40;
            else
                pd1->injuredcd = now + 40;
            pd1->player->setBuffData(PLAYER_BUFF_HIWEAK, pd1->injuredcd, false);

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
                commitCompass(pd->player);
            }
            else
            {
                pd->straight = 1;
            }

            Stream st(REP::HERO_ISLAND);
            st << static_cast<UInt8>(5) << static_cast<UInt8>(2) << pd->straight << Stream::eos;
            pd->player->send(st);

            player->pendExp(calcExp(!pd1->player?0:pd1->player->GetLev()));
            broadcast(pd, pd->spot, 2);
        }
        else
        {
            if (cfg.GMCheck)
                pd->injuredcd = now + 40;
            else
                pd->injuredcd = now + 40;
            pd->player->setBuffData(PLAYER_BUFF_HIWEAK, pd->injuredcd, false);
            moveTo(pd->player, 0, false);

            broadcast(pd1, pd1->spot, 2);
        }

        return true;
    }
    return false;
}

bool HeroIsland::useSkill(Player* player, UInt8 skillid)
{
    if (!player || !skillid || skillid > 5)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    UInt8 spot = player->getHISpot();
    UInt8 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;
    if (pd->spot)
        return false;

    if (pd->skills[skillid-1].incd)
    {
        return false;
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(12);
    st << skillid;

    UInt32 now = TimeUtil::Now();
    switch (skillid)
    {
        case 1:
            {
                if (!pd->player->getBuffData(PLAYER_BUFF_HIWEAK))
                {
                    pd->player->sendMsgCode(0, 2006);
                    return false;
                }

                UInt16 cd = 20*60;
                pd->skills[0].cd = now + cd; // TODO:
                pd->skills[0].bufid = 0;
                pd->skills[0].attr = NULL;
                pd->player->setBuffData(PLAYER_BUFF_HIWEAK, 0, false);
                st << cd;
            }
            break;

        case 2:
            {
                UInt16 cd = 20*60;
                pd->skills[1].lastcd = now + pd->skills[1].last;
                pd->skills[1].cd = now + cd;
                pd->skills[1].bufid = PLAYER_BUFF_HIPG;
                pd->skills[1].attr = &_skillattr[1];
                pd->player->setBuffData(pd->skills[1].bufid, pd->skills[1].lastcd);
                st << cd;
            }
            break;

        case 3:
            {
                UInt16 cd = 20*60;
                pd->skills[2].lastcd = now + pd->skills[2].last;
                pd->skills[2].cd = now + cd;
                pd->skills[2].bufid = PLAYER_BUFF_HIBT;
                pd->skills[2].attr = &_skillattr[2];
                pd->player->setBuffData(pd->skills[2].bufid, pd->skills[2].lastcd);
                st << cd;
            }
            break;

        case 4:
            {
                UInt16 cd = 20*60;
                pd->skills[3].lastcd = now + pd->skills[3].last;
                pd->skills[3].cd = now + cd;
                pd->skills[3].bufid = PLAYER_BUFF_HILN;
                pd->skills[3].attr = &_skillattr[3];
                pd->player->setBuffData(pd->skills[3].bufid, pd->skills[3].lastcd);
                st << cd;
            }
            break;

        case 5:
            {
                UInt16 cd = 20*60;
                pd->skills[4].lastcd = now + pd->skills[4].last;
                pd->skills[4].cd = now + cd;
                pd->skills[4].bufid = PLAYER_BUFF_HIJZ;
                pd->skills[4].attr = NULL;
                pd->player->setBuffData(pd->skills[4].bufid, pd->skills[4].lastcd);
                st << cd;
            }
            break;
            
        default:
            break;
    }

    pd->skills[skillid-1].incd = true;
    st << Stream::eos;
    pd->player->send(st);
    return true;
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

        st << static_cast<UInt8>(pd->awardgot==0xFF?0:pd->awardgot);
        st << pd->inrank;
        st << static_cast<UInt16>(pd->score);
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
        player->sendMsgCode(0, 1010);
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
    UInt8 count = 0;
    for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
    {
        if (jump)
        {
            --jump;
            continue;
        }

        st << (*i)->player->getName();
        st << (*i)->player->GetLev();
        st << static_cast<UInt16>((*i)->score);

        ++count;
        if (count >= pagesize)
            break;
    }

    st.data<UInt8>(off) = count;
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

    if (pd->awardgot == 0xFF)
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
    UInt16 score = pd->score;

    if (!(sz % 3))
    {
        if (pd->straight == 3)
        {
            ++pd->round;
            pd->awardgot = pd->round+1;
        }
        else
            pd->awardgot = 1;

        if (sz >= 12)
        {
            pd->round = 0;
            pd->compass.clear();
        }
        pd->straight = 0; // XXX: 每三次为一轮
    }
    else
    {
    }

    pd->score += 10;

    if (score != pd->score)
    {
        SortType::iterator i = _sorts.find(pd);
        if (i != _sorts.end())
            _sorts.erase(i);
        _sorts.insert(pd);

        pd->inrank = 0;
        for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
        {
            ++pd->inrank;
            if (*i == pd)
                break;
        }

        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(14) << pd->inrank << pd->score << Stream::eos;
        player->send(st);
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(5) << static_cast<UInt8>(3) << pd->straight
        << static_cast<UInt8>(pd->awardgot==0xFF?0:pd->awardgot) << Stream::eos;
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

    if (player->GetPackage()->IsFull())
    {
        player->sendMsgCode(0, 1011);
        return false;
    }

    if (type == 1)
    {
        if (!pd->awardgot || pd->awardgot == 0xFF)
            return false;

        struct Award
        {
            UInt16 id;
            UInt32 num;
        };

        UInt8 quality = pd->straight/3;
        UInt8 sz = _awards[quality].size();
        UInt32 total = _awards[quality][sz-1].prob;
        Award awards[5] = {{0,},};
        for (UInt8 j = 0; j < 5; ++j)
        {
            UInt32 v = uRand(total);
            Awards* awds = &_awards[quality][0];
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
            if (awards[id].id > 5)
            {
                return player->GetPackage()->Add(
                        awards[id].id, awards[id].num, true, false, FromHeroIsland);
            }

            switch (awards[id].id)
            {
                case 1:
                    player->getTael(awards[id].num);
                    break;

                case 2:
                    player->AddExp(awards[id].num * calcExp(player->GetLev()));
                    break;

                case 3:
                    player->getCoupon(awards[id].num);
                    break;

                case 4:
                    player->getPrestige(awards[id].num);
                    break;

                default:
                    break;
            }
            pd->awardgot = 0xFF;
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

