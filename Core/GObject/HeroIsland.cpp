
#include "HeroIsland.h"
#include "Server/SysMsg.h"
#include "Common/Random.h"
#include "GObject/Package.h"
#include "Battle/BattleSimulator.h"
#include "GData/Money.h"
#include "MsgID.h"
#include "Country.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "HeroMemo.h"

namespace GObject
{

#define ISINJZ(pd) (pd->skills[4].lastcd && pd->skills[4].lastcd != static_cast<UInt32>(-1))

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

HeroIsland::~HeroIsland()
{
    for (UInt8 j = 0; j < HERO_ISLAND_SPOTS; ++j)
    {
        for (std::vector<HIPlayerData*>::iterator
                i = _players[j].begin(), e = _players[j].end(); i != e; ++i)
        {
            delete *i;
        }
        _players[j].clear();
    }
}

bool HeroIsland::initSkillAttr()
{
    _skillattr[1].magatkP = 0.1;
    _skillattr[1].attackP = 0.1;
    _skillattr[2].defendP = 0.1;
    _skillattr[2].magdefP = 0.1;
    _skillattr[3].aura = 25;
    return false;
}

void HeroIsland::clearAllHICfg()
{
    for (UInt8 i = 0; i < HERO_ISLAND_SPOTS; ++i)
        _animals[i].clear();
    for (UInt8 i = 0; i < 4; ++i)
        _awards[i].clear();
    _prestige.clear();
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
        case 5493:
            ra.bufid = PLAYER_BUFF_HIRA5;
            break;
        case 5494:
            ra.bufid = PLAYER_BUFF_HIRA6;
            break;
        case 5495:
            ra.bufid = PLAYER_BUFF_HIRA7;
            break;
        case 5496:
            ra.bufid = PLAYER_BUFF_HIRA8;
            break;
        case 5497:
            ra.bufid = PLAYER_BUFF_HIRA9;
            break;
        case 5498:
            ra.bufid = PLAYER_BUFF_HIRA10;
            break;
        case 5499:
            ra.bufid = PLAYER_BUFF_HIRA11;
            break;
        case 5500:
            ra.bufid = PLAYER_BUFF_HIRA12;
            break;
        case 5501:
            ra.bufid = PLAYER_BUFF_HIRA13;
            break;
        case 5502:
            ra.bufid = PLAYER_BUFF_HIRA14;
            break;
        case 5503:
            ra.bufid = PLAYER_BUFF_HIRA15;
            break;
        case 5504:
            ra.bufid = PLAYER_BUFF_HIRA16;
            break;
        case 5505:
            ra.bufid = PLAYER_BUFF_HIRA17;
            break;
        case 5506:
            ra.bufid = PLAYER_BUFF_HIRA18;
            break;
        case 5507:
            ra.bufid = PLAYER_BUFF_HIRA19;
            break;
        case 5508:
            ra.bufid = PLAYER_BUFF_HIRA20;
            break;
        default:
            ra.bufid = DEFAULT_BUFID;
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
    _prepareTime = now + 60;
    _prepareStep = 0;
    broadcastTV(now);
}

void HeroIsland::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime && !_prepareStep)
        _prepareStep = 1;

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
            if (now < _startTime)
                return;
            SYSMSG_BROADCASTV(2118);
            _prepareStep = 5;
            break;

        default:
            break;
    }
}

void HeroIsland::calcNext(UInt32 now)
{
    if (cfg.GMCheck)
    {
        _prepareTime = TimeUtil::SharpDayT(0,now) + 11 * 60 * 60 + 45 * 60;

#if 0
        if(World::_wday == 6 || World::_wday == 7)
        {
            if (now >= TimeUtil::SharpDayT(0,now) + 18 * 60 * 60 + 45 * 60)
                _prepareTime = TimeUtil::SharpDayT(1,now) + 11 * 60 * 60 + 45 * 60;
            else if (now >= TimeUtil::SharpDayT(0,now) + 12 * 60 * 60 + 45 * 60)
                _prepareTime = TimeUtil::SharpDayT(0,now) + 17 * 60 * 60 + 45 * 60;
        }
        else
        {
#endif
            if (now >= TimeUtil::SharpDayT(0,now) + 12 * 60 * 60 + 45 * 60)
                _prepareTime += 24 * 60 * 60;
#if 0
        }
#endif

        _startTime = _prepareTime + 15 * 60;
        _endTime = _startTime + 55 * 60;
    }
    else
    {
        _prepareTime = now;
        _startTime = _prepareTime + 30;
        _endTime = _startTime + 15 * 60;
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(15);
    st << now;
    st << _startTime;
    st << _endTime;
    st << Stream::eos;
    broadcast(st);
}

void HeroIsland::rankReward()
{
    if (!_running || !_endTime)
        return;

    size_t size = _sorts.size();
    float factor = size / (float)100;
    if (!cfg.GMCheck)
        factor = 1.0;

    SYSMSG_BROADCASTV(2312);

    UInt8 n = 0;
    UInt8 nsz = _prestige.size();
    for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
    {
        if (!(*i) || !(*i)->player)
            continue;

        UInt16 prestige = 0;
        if (n < nsz)
        {
            prestige = _prestige[n] * factor + (*i)->tasks * 10;
            if (World::_wday == 6 || World::_wday == 7)
                prestige *= 2;
            if (n < 3)
            {
                UInt16 count = (World::_wday == 6 || World::_wday == 7)?2:1;
                MailPackage::MailItem item[] = {{10,count},};
                if (count == 1)
                    (*i)->player->sendMailItem(2310, 2311, item, 1);
                else
                    (*i)->player->sendMailItem(2310, 2315, item, 1);
                SYSMSG_BROADCASTV(2313, n+1, (*i)->player->getCountry(), (*i)->player->getName().c_str(), prestige, count);
            }

            ++n;
        }
        else
        {
            if (nsz)
            {
                prestige = _prestige[nsz-1] * factor + (*i)->tasks * 10;
                if (World::_wday == 6 || World::_wday == 7)
                    prestige *= 2;
            }
        }

        if (prestige)
        {
            if ((*i)->player->getThreadId() != WORKER_THREAD_NEUTRAL)
            {
                GameMsgHdr hdr(0x236, (*i)->player->getThreadId(), (*i)->player, sizeof(prestige));
                GLOBAL().PushMsg(hdr, &prestige);
            }
            else
                (*i)->player->getPrestige(prestige);
        }

        //
        if((*i)->player->getThreadId() != WORKER_THREAD_NEUTRAL)
        {
            stActivityMsg msg;
            msg.id = AtyHeroIsland;
            GameMsgHdr hdr2(0x245, (*i)->player->getThreadId(), (*i)->player, sizeof(stActivityMsg));
            GLOBAL().PushMsg(hdr2, &msg);
        }
        else
        {
            GameAction()->doAty((*i)->player, AtyHeroIsland, 0 ,0);
        }
    }
}

void HeroIsland::end(UInt32 now)
{
    rankReward();
    calcNext(now);
    _running = false;
    reset();
    _prepareStep = 0;
    SYSMSG_BROADCASTV(2116);
    setStatus(2);
}

void HeroIsland::reset()
{
    for (UInt8 i = 0; i < HERO_ISLAND_SPOTS; ++i)
    {
        size_t sz = _players[i].size();
        for (size_t j = 0; j < sz; ++j)
        {
            HIPlayerData* pd = _players[i][j];
            if (pd && pd->player)
                pd->reset(false);
            //clearBuff(1, pd, 0);
            //clearBuff(2, pd, 0);
        }
    }
    _sorts.clear();
}

void HeroIsland::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);

    broadcastTV(now);

    if (!_running && _startTime && now >= _startTime)
    {
        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(15);
        st << now; 
        st << _startTime;
        st << _endTime;
        st << Stream::eos;
        broadcast(st);
        if (cfg.GMCheck)
            _disperseTime = now + 5 * 60;
        else
            _disperseTime = now + 30;
        if (cfg.GMCheck)
            _expTime = now + 10 * 60;
        else
            _expTime = now + 30;
        _running = true;
        setStatus(1);
    }

    if (_running && now >= _endTime)
        end(now);

    if (now >= _notifyTime)
        notifyCount(now);
    if (_running && now >= _expTime)
        expFactor(now);
    if (_running && now >= _disperseTime)
        disperse(now);

    applayPlayers();
    applayRareAnimals();
}

void HeroIsland::disperse(UInt32 now)
{
    std::vector<HIPlayerData*> l = _players[0];
    size_t sz = l.size();
    for (size_t i = 0; i < sz; ++i)
    {
        HIPlayerData* pd = l[i];
        if (pd && pd->player && !pd->player->hasFlag(Player::InHeroIsland))
            continue;
        if (pd && pd->player)
        {
            if (!pd->injuredcd || pd->injuredcd == static_cast<UInt32>(-1))
            {
                UInt8 to = uRand(100);
                to %= 5;
                if (!to) to = uRand(5);
                if (!to) to = 1;
                moveTo(pd->player, to, false, true);
            }
        }
    }

    if (cfg.GMCheck)
        _disperseTime = now + 5 * 60;
    else
        _disperseTime = now + 30;

    SYSMSG_BROADCAST(2314);
}

void HeroIsland::notifyCount(UInt32 now)
{
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(16);

    size_t off = st.size();
    st << static_cast<UInt8>(0);

    UInt8 count = 0;
    for (UInt8 j = 0; j < HERO_ISLAND_SPOTS; ++j)
    {
        st << j;
        st << _nplayers[j];
        ++count;
    }
    st.data<UInt8>(off) = count;
    st << Stream::eos;
    broadcast(st);

    _notifyTime = now + 5;
}

#define EXPFACTOR() \
    Stream st(REP::HERO_ISLAND); \
    st << static_cast<UInt8>(17); \
    st << static_cast<UInt8>(5); \
    st << static_cast<UInt8>(0); \
    st << static_cast<UInt8>(0); \
    for (UInt8 i = 0; i < 4; ++i) \
    { \
        _expfactor[i] = factor[i]; \
        st << static_cast<UInt8>(i+1); \
        st << static_cast<UInt8>(2*_expfactor[i]); \
    } \
    st << Stream::eos;

static float factor[4] = {1.5, 2.0, 2.5, 3.0};

void HeroIsland::expFactor(UInt32 now)
{
    for (UInt8 n = 0; n < 4; ++n)
    {
        UInt8 i = uRand(4);
        if (n == i)
        {
            UInt8 j = uRand(4);
            float tmp = factor[i];
            factor[i] = factor[j];
            factor[j] = tmp;
        }
        else
        {
            float tmp = factor[n];
            factor[n] = factor[i];
            factor[i] = tmp;
        }
    }

    EXPFACTOR();
    broadcast(st);

    if (cfg.GMCheck)
        _expTime = now + 10 * 60;
    else
        _expTime = now + 30;
}

void HeroIsland::sendExpFactor(Player* player)
{
    if (!player)
        return;

    EXPFACTOR();
    player->send(st);
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

            if (pd && pd->player && !pd->player->hasFlag(Player::InHeroIsland))
                continue;

            if (!j && pd && pd->player && pd->injuredcd <= now)
            {
                pd->player->regenAll(true);
                pd->injuredcd = static_cast<UInt32>(-1);
                pd->player->setBuffData(PLAYER_BUFF_HIWEAK, 0, false);
                broadcast(pd, 0, 2);
            }

            if (_running)
            {
                if (j && pd && pd->player && pd->expcd <= now)
                {
                    UInt8 factor = (World::_wday == 6 || World::_wday == 7)?2:1;
                    pd->player->AddExp(calcExp(pd->player->GetLev())*_expfactor[j-1]*factor);
                    pd->expcd = now + 60;
                }
            }

            if (pd && pd->player && now >= pd->attrcd && pd->attr)
                clearBuff(1, pd, now);

            for (UInt8 i = 0; i < 5; ++i)
            {
                if (pd->skills[i].cd && now >= pd->skills[i].cd)
                {
                    pd->skills[i].incd = false;
                    pd->skills[i].cd = 0;
                }

                if (pd->skills[i].lastcd && now >= pd->skills[i].lastcd)
                {
                    if (i && pd->skills[i].bufid != DEFAULT_BUFID)
                    {
                        clearBuff(2, pd, now, i);
                        if (i == 4)
                        {
                            broadcast(pd, pd->spot, 0);
                            ++_nplayers[pd->spot];
                        }
                    }
                }
            }
        }
    }
}

void HeroIsland::clearBuff(UInt8 type, HIPlayerData* pd, UInt32 now, UInt8 skillid)
{
    if (!pd) return;

    pd->player->clearHIAttr();

    if (now)
    {
        if (type == 1 && pd->bufid != DEFAULT_BUFID)
        {
            pd->attrcd = static_cast<UInt32>(-1);
            pd->player->setBuffData(pd->bufid, 0, false);
            pd->bufid = DEFAULT_BUFID;

            for (UInt8 i = 1; i < 5; ++i)
            {
                if (now < pd->skills[i].lastcd && pd->skills[i].attr)
                    pd->player->addHIAttr(*pd->skills[i].attr);
            }
        }
        else if (type == 2 && skillid)
        {
            if (pd->skills[skillid].bufid != DEFAULT_BUFID)
                pd->player->setBuffData(pd->skills[skillid].bufid, 0, false);
            pd->skills[skillid].lastcd = static_cast<UInt32>(-1);
            pd->skills[skillid].attr = NULL;

            for (UInt8 i = 1; i < 5; ++i)
            {
                if (now < pd->skills[i].lastcd && pd->skills[i].attr && i != skillid)
                    pd->player->addHIAttr(*pd->skills[i].attr);
            }
        }
    }
    else
    {
        if (type == 1)
        {
            if (pd->bufid != DEFAULT_BUFID)
                pd->player->setBuffData(pd->bufid, 0, false);
            pd->attrcd = static_cast<UInt32>(-1);
            pd->bufid = DEFAULT_BUFID;
        }
        else if (type == 2)
        {
            for (UInt8 i = 1; i < 5; ++i)
            {
                if (pd->skills[i].bufid != DEFAULT_BUFID)
                    pd->player->setBuffData(pd->skills[i].bufid, 0, false);
                pd->skills[i].lastcd = static_cast<UInt32>(-1);
                pd->skills[i].bufid = DEFAULT_BUFID;
                pd->skills[i].attr = NULL;
            }
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
#if 0
        if (player->hasFlag(Player::InHeroIsland))
        {
            type = player->getHIType();
        }
        else if (_types[0] < _types[1])
#else
        if (_types[0] < _types[1])
#endif
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
    return type;
}

HIPlayerData* HeroIsland::findPlayer(Player* player, UInt8& spot, UInt16& pos)
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
                player->setHISpot(spot);
                _players[j][i]->spot = spot;
                return _players[j][i];
            }
        }
    }
    return NULL;
}

HIPlayerData* HeroIsland::findPlayer(UInt64 id, UInt8& spot, UInt16& pos)
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
                _players[j][i]->player->setHISpot(spot);
                _players[j][i]->spot = spot;
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

    UInt16 pos = 0;
    UInt8 rspot = 0xFF; //spot;
    HIPlayerData* pd = findPlayer(player, rspot, pos);
    if (pd)
    {
        UInt32 now = TimeUtil::Now();
        pd->expcd = now + 60;
        pd->player->setBuffData(PLAYER_BUFF_HIMOVE, 0, false);
        pd->type = type;

        sendSpot(pd, rspot);
        return true;
    }

    pd = new(std::nothrow) HIPlayerData;
    if (!pd) return false;
    pd->player = player;
    pd->expcd = TimeUtil::Now() + 60;

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
            pd->movecd = TimeUtil::Now() + 10;
        else
            pd->movecd = TimeUtil::Now() + 10;
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
    sendSkills(pd);
    if (spot)
        sendRareAnimals(pd, spot);

    if (!ISINJZ(pd))
    {
        broadcast(pd, spot, 0);
        ++_nplayers[spot];
    }
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

    size_t off = st.size();
    st << static_cast<UInt16>(0);

    UInt16 count = 0;
    UInt8 l1 = pd->player->GetLev();
    for (size_t i = start; i < sz; ++i)
    {
        HIPlayerData* pd1 = _players[spot][i];
        if (pd1->player && pd1->player != pd->player &&
                pd1->player->hasFlag(Player::InHeroIsland) &&
                (!ISINJZ(pd1)))
        {
            UInt8 l2 = pd1->player->GetLev();
            if (l1 < l2 && l2 - l1 > 20)
                continue;
            if (l1 > l2 && l1 - l2 > 10)
                continue;

            st << pd1->player->getId();
            st << pd1->type;
            st << pd1->player->allHpP();
            st << pd1->player->GetLev();
            st << pd1->player->getName();
            ++count;
        }
    }
    st.data<UInt16>(off) = count;
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

#if 0
    if (spot && !pd->player->allHpP())
    {
        fprintf(stderr, "error spot: %u, type: %u, hp: %u\n", spot, type, pd->player->allHpP());
        return;
    }
#endif

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
        if (!player1)
            continue;
        if (!player1->hasFlag(Player::InHeroIsland))
            continue;
        if (!player || (player && player != player1))
        {
            if (player)
            {
                UInt8 l1 = player1->GetLev();
                UInt8 l2 = player->GetLev();
                if (l1 < l2 && l2 - l1 > 20)
                    continue;
                if (l1 > l2 && l1 - l2 > 10)
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
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, rspot, pos);
    if (!pd) return NULL;
    return leave(pd, rspot, pos);
}

HIPlayerData* HeroIsland::leave(HIPlayerData* pd, UInt8 spot, UInt16 pos)
{
    if (!pd || spot > HERO_ISLAND_SPOTS || pos > _players[spot].size())
        return NULL;

    std::vector<HIPlayerData*>::iterator it = _players[spot].begin();
    std::advance(it, pos);
    if (it != _players[spot].end())
        _players[spot].erase(it);
    else
    {
        return NULL;
    }

    if (!ISINJZ(pd))
    {
        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(7) << static_cast<UInt8>(1) << pd->player->getId() << Stream::eos;
        broadcast(st, pd->spot);
        if (_nplayers[spot])
            --_nplayers[spot];
    }
    if (pd->player && !spot && pd->ato && pd->player->allHpP() < 100)
        pd->player->regenAll(true);
    return pd;
}

void HeroIsland::listPlayers(Player* player, UInt8 spot, UInt16 start, UInt8 pagesize)
{
    if (!player)
        return;

    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (pd)
        sendPlayers(pd, spot, start, pagesize);
}

bool HeroIsland::moveTo(Player* player, UInt8 to, bool movecd, bool force)
{
    if (!player || to > HERO_ISLAND_SPOTS)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    UInt8 spot = 0xFF; // player->getHISpot();
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;

    if (spot == to)
        return false;

    UInt32 now = TimeUtil::Now();
    if (!force)
    {
        if (movecd && pd->movecd > now)
        {
            pd->player->sendMsgCode(0, 2001);
            return false;
        }
    }

    if (leave(pd, spot, pos))
    {
        bool cd = to?true:false;
        if (!movecd)
            cd = false;
        if (enter(pd, pd->type, to, cd))
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
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;

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
            player->addHIAttr(ra.attr);
            pd->player->setBuffData(ra.bufid, pd->attrcd, false);
            SYSMSG_SEND(2119, pd->player);
        }
        else
        {
            if (cfg.GMCheck)
                pd->injuredcd = now + 6;
            else
                pd->injuredcd = now + 6;
            pd->player->setBuffData(PLAYER_BUFF_HIWEAK, pd->injuredcd+4, false);
            moveTo(pd->player, 0, false, true);
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
        UInt16 pos = 0;
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

        if (!pd1->player->hasFlag(Player::InHeroIsland))
        {
            return false;
        }

        int turns = 0;
        bool res = player->challenge(pd1->player, NULL, &turns, false, 0, true, Battle::BS_COPY5, pd->ato==1?0x00:0x01);
        if (cfg.GMCheck)
            pd->fightcd = now + 40;
        else
            pd->fightcd = now + 40;
        pd->player->setBuffData(PLAYER_BUFF_HIFIGHT, pd->fightcd, false);

        if (res)
        {
            pd->lasttype = pd1->type;
            if (cfg.GMCheck)
                pd1->injuredcd = now + 6;
            else
                pd1->injuredcd = now + 6;
            pd1->player->setBuffData(PLAYER_BUFF_HIWEAK, pd1->injuredcd+4, false);

            if (_running)
            {
                size_t sz = pd->compass.size();
                if (sz && pd->compass[sz-1].type == pd1->type)
                {
                    UInt8 status = 1;
                    if (pd->compass[sz-1].status == 2 || pd->compass[sz-1].status == 3)
                        return false;
                    status = pd->compass[sz-1].status = 2;

                    if (sz == 1 || sz == 4 || sz == 7)
                        pd->straight = 1;
                    else if (sz > 1)
                    {
                        if (pd->compass[sz-1].type == pd->compass[sz-2].type)
                            ++pd->straight;
                    }
                    ++pd->tasks;
                    commitCompass(pd->player);

                    Stream st(REP::HERO_ISLAND);
                    st << static_cast<UInt8>(5) << static_cast<UInt8>(status) << pd->straight << Stream::eos;
                    pd->player->send(st);

                    pd->player->OnHeroMemo(MC_ATHLETICS, MD_MASTER, 0, 1);
                }
            }

            player->pendExp(calcExp(!pd1->player?0:pd1->player->GetLev())/2);

            moveTo(pd1->player, 0, false, true);
            broadcast(pd, pd->spot, 2);
        }
        else
        {
            if (cfg.GMCheck)
                pd->injuredcd = now + 6;
            else
                pd->injuredcd = now + 6;
            pd->player->setBuffData(PLAYER_BUFF_HIWEAK, pd->injuredcd+4, false);

            moveTo(pd->player, 0, false, true);
            broadcast(pd1, pd1->spot, 2);
        }

        return true;
    }
    return false;
}

#define CHECK_MONEY() \
{\
    UInt32 goldUse = GData::moneyNeed[GData::HISKILL].gold; \
    if (pd->player->getGold() < goldUse) \
    { \
        pd->player->sendMsgCode(0, 1104); \
        return false; \
    } \
    ConsumeInfo ci(HeroIslandSkill,0,0); \
    pd->player->useGold(goldUse, &ci); \
}

bool HeroIsland::useSkill(Player* player, UInt8 skillid, UInt8 type)
{
    if (!player || !skillid || skillid > 5)
        return false;

    if (!player->hasFlag(Player::InHeroIsland))
        return false;

    UInt8 spot = player->getHISpot();
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return false;
    //if (pd->spot)
    //    return false;

    if (!type && pd->skills[skillid-1].incd)
    {
        return false;
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(12);
    st << skillid;

    UInt16 cd = 0;
    if (cfg.GMCheck)
        cd = 20*60;
    else
        cd = 60;
    UInt32 now = TimeUtil::Now();
    switch (skillid)
    {
        case 1:
            {
                if (pd->player->allHpP() >= 100)
                {
                    pd->player->sendMsgCode(0, 2006);
                    return false;
                }

                if (pd->skills[0].cd && now < pd->skills[0].cd)
                {
                    if (type)
                    {
                        CHECK_MONEY();
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    pd->skills[0].cd = now + cd; // TODO:
                }
                pd->skills[0].bufid = DEFAULT_BUFID;
                pd->skills[0].attr = NULL;
                pd->player->setHPPercent(10);
                pd->player->setBuffData(PLAYER_BUFF_HIWEAK, 0, false);
                pd->player->setBuffData(PLAYER_BUFF_HIMOVE, 0, false);
                SYSMSG_SEND(2134, pd->player);
                st << cd;
            }
            break;

        case 2:
            {
                if (pd->skills[1].cd)
                    cd = static_cast<UInt16>(cd - (now + cd - pd->skills[1].cd));
                if (pd->skills[1].cd && now < pd->skills[1].cd)
                {
                    if (type)
                    {
                        CHECK_MONEY();
                        if (!pd->skills[1].lastcd || pd->skills[1].lastcd == static_cast<UInt32>(-1))
                            pd->skills[1].lastcd = now + pd->skills[1].last;
                        else
                            pd->skills[1].lastcd += pd->skills[1].last;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    pd->skills[1].cd = now + cd;
                    if (!pd->skills[1].lastcd || pd->skills[1].lastcd == static_cast<UInt32>(-1))
                        pd->skills[1].lastcd = now + pd->skills[1].last;
                    else
                        pd->skills[1].lastcd += pd->skills[1].last;

                    pd->skills[1].attr = &_skillattr[1];
                    pd->player->addHIAttr(*pd->skills[1].attr);
                }
                pd->skills[1].bufid = PLAYER_BUFF_HIPG;
                pd->player->setBuffData(pd->skills[1].bufid, pd->skills[1].lastcd, false);
                SYSMSG_SEND(2130, pd->player);
                st << cd;
            }
            break;

        case 3:
            {
                if (pd->skills[2].cd)
                    cd = static_cast<UInt16>(cd - (now + cd - pd->skills[2].cd));
                if (pd->skills[2].cd && now < pd->skills[2].cd)
                {
                    if (type)
                    {
                        CHECK_MONEY();
                        if (!pd->skills[2].lastcd || pd->skills[2].lastcd == static_cast<UInt32>(-1))
                            pd->skills[2].lastcd = now + pd->skills[2].last;
                        else
                            pd->skills[2].lastcd += pd->skills[2].last;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    pd->skills[2].cd = now + cd;
                    if (!pd->skills[2].lastcd || pd->skills[2].lastcd == static_cast<UInt32>(-1))
                        pd->skills[2].lastcd = now + pd->skills[2].last;
                    else
                        pd->skills[2].lastcd += pd->skills[2].last;

                    pd->skills[2].attr = &_skillattr[2];
                    pd->player->addHIAttr(*pd->skills[2].attr);
                }

                pd->skills[2].bufid = PLAYER_BUFF_HIBT;
                pd->player->setBuffData(pd->skills[2].bufid, pd->skills[2].lastcd, false);
                SYSMSG_SEND(2131, pd->player);
                st << cd;
            }
            break;

        case 4:
            {
                if (pd->skills[3].cd)
                    cd = static_cast<UInt16>(cd - (now + cd - pd->skills[3].cd));
                if (pd->skills[3].cd && now < pd->skills[3].cd)
                {
                    if (type)
                    {
                        CHECK_MONEY();
                        if (!pd->skills[3].lastcd || pd->skills[3].lastcd == static_cast<UInt32>(-1))
                            pd->skills[3].lastcd = now + pd->skills[3].last;
                        else
                            pd->skills[3].lastcd += pd->skills[3].last;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    pd->skills[3].cd = now + cd;
                    if (!pd->skills[3].lastcd || pd->skills[3].lastcd == static_cast<UInt32>(-1))
                        pd->skills[3].lastcd = now + pd->skills[3].last;
                    else
                        pd->skills[3].lastcd += pd->skills[3].last;

                    pd->skills[3].attr = &_skillattr[3];
                    pd->player->addHIAttr(*pd->skills[3].attr);
                }

                pd->skills[3].bufid = PLAYER_BUFF_HILN;
                pd->player->setBuffData(pd->skills[3].bufid, pd->skills[3].lastcd, false);
                SYSMSG_SEND(2132, pd->player);
                st << cd;
            }
            break;

        case 5:
            {
                if (pd->skills[4].cd)
                    cd = static_cast<UInt16>(cd - (now + cd - pd->skills[4].cd));
                if (pd->skills[4].cd && now < pd->skills[4].cd)
                {
                    if (type)
                    {
                        CHECK_MONEY();
                        if (!pd->skills[4].lastcd || pd->skills[4].lastcd == static_cast<UInt32>(-1))
                            pd->skills[4].lastcd = now + pd->skills[4].last;
                        else
                            pd->skills[4].lastcd += pd->skills[4].last;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    pd->skills[4].cd = now + cd;
                    if (!pd->skills[4].lastcd || pd->skills[4].lastcd == static_cast<UInt32>(-1))
                        pd->skills[4].lastcd = now + pd->skills[4].last;
                    else
                        pd->skills[4].lastcd += pd->skills[4].last;
                }

                pd->skills[4].bufid = PLAYER_BUFF_HIJZ;
                pd->skills[4].attr = NULL;
                pd->player->setBuffData(pd->skills[4].bufid, pd->skills[4].lastcd, false);
                SYSMSG_SEND(2133, pd->player);
                st << cd;

                Stream st(REP::HERO_ISLAND);
                st << static_cast<UInt8>(7) << static_cast<UInt8>(1) << pd->player->getId() << Stream::eos;
                broadcast(st, pd->spot);
                if (_nplayers[pd->spot])
                    --_nplayers[pd->spot];
            }
            break;
            
        default:
            return false;
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

    UInt8 spot = 0xFF; // player->getHISpot();
    UInt16 pos = 0;
    UInt8 in = 0;
    UInt8 type = 0;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(0);

    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (pd && pd->player && pd->player->hasFlag(Player::InHeroIsland))
    {
        in = 1;
        type = pd->type;
    }
    else
    {
        type = player->getHIType();
    }

    st << in;
    st << TimeUtil::Now();
    st << _startTime;
    st << _endTime;
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
        st << static_cast<UInt8>(pd->ato);
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

    UInt32 now = TimeUtil::Now();
    UInt32 left = player->getBuffData(PLAYER_BUFF_HIESCAPE);
    if (left > now)
        left -= now;
    else
        left = 0;
    if (left)
    {
        player->sendMsgCode(0, 2007, left);
        return;
    }

    getIdentity(player);
    if (!player->getHIType())
        return;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(1);
    if (enter(player, player->getHIType(), player->getHISpot(), false))
    {
        st << static_cast<UInt8>(0);
        st << now; 
        st << _startTime;
        st << _endTime;
        player->addFlag(Player::InHeroIsland);

        sendExpFactor(player);

        if (player->getAtoHICfg().length())
            sendAtoCfg(player);
    }
    else
        st << static_cast<UInt8>(1);
    st << Stream::eos;
    player->send(st);

    player->OnHeroMemo(MC_ATHLETICS, MD_MASTER, 0, 0);
}

void HeroIsland::playerLeave(Player* player)
{
    if (!player)
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    UInt8 spot = 0xFF;
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return;

    if (!ISINJZ(pd))
    {
        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(7) << static_cast<UInt8>(1) << pd->player->getId() << Stream::eos;
        broadcast(st, pd->spot);
        if (_nplayers[spot])
            --_nplayers[spot];
    }

    clearBuff(1, pd, 0);
    clearBuff(2, pd, 0); // XXX: must before reset
    pd->reset(_running);

    player->delFlag(Player::InHeroIsland);
    player->regenAll(true);

    if (cfg.GMCheck)
        player->setBuffData(PLAYER_BUFF_HIESCAPE, TimeUtil::Now()+5*60, false);
    else
        player->setBuffData(PLAYER_BUFF_HIESCAPE, TimeUtil::Now()+60, false);
}

void HeroIsland::playerOffline(Player* player)
{
    if (!player)
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    UInt8 spot = 0xFF;
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd) return;

    if (!ISINJZ(pd))
    {
        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(7) << static_cast<UInt8>(1) << pd->player->getId() << Stream::eos;
        broadcast(st, pd->spot);
        if (_nplayers[spot])
            --_nplayers[spot];
    }
}

void HeroIsland::listRank(Player* player, UInt16 start, UInt8 pagesize)
{
    if (!player)
        return;

    size_t sz = _sorts.size();
    if (start > sz)
        return;

    if (sz > 100)
        sz = 100;

    if (pagesize > sz)
        pagesize = sz;

    if (pagesize > 100)
        pagesize = 100;

    if (start > 100)
        start = 0;

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(13);
    st << start;
    st << static_cast<UInt16>(sz);

    size_t off = st.size();
    st << static_cast<UInt8>(0);

    UInt32 jump = start;
    UInt32 num = start+1;
    UInt8 count = 0;
    for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
    {
        if (jump)
        {
            --jump;
            continue;
        }

        if (num > 100)
            break;
        ++num;

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

    if (!_running)
    {
        player->sendMsgCode(0, 2008);
        return;
    }

    UInt8 spot = player->getHISpot();
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;

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

void HeroIsland::commitCompass(Player* player)
{
    if (!player)
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    UInt8 spot = player->getHISpot();
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;

    size_t sz = pd->compass.size();
    if (!sz)
        return;

    if (pd->compass[sz-1].status != 2)
        return;
    pd->compass[sz-1].status = 3;

    UInt8 straight = pd->straight;
    if (sz && !(sz % 3))
    {
        if (straight >= 3)
        {
            ++pd->round;
            pd->awardgot = pd->round+1;
        }
        else
            pd->awardgot = 1;

        if (sz >= 9)
        {
            pd->round = 0;
            pd->compass.clear();
        }

        pd->straight = 0; // XXX: 每三次为一轮
    }
    else
    {
    }

    {
        for (SortType::iterator i = _sorts.begin(), e = _sorts.end(); i != e; ++i)
        {
            if (*i && (*i)->player == pd->player)
            {
                _sorts.erase(i);
                break;
            }
        }
        // XXX: 这行代码只允许放在这里
        pd->score += 10;
        _sorts.insert(pd);

        pd->inrank = 0;
        for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e; ++i)
        {
            ++pd->inrank;
            if (*i == pd)
                break;
        }

        Stream st(REP::HERO_ISLAND);
        st << static_cast<UInt8>(14) << pd->inrank << pd->score;
        st << Stream::eos;
        player->send(st);


        if (pd->inrank <= 3)
        {
            Stream st1(REP::HERO_ISLAND);
            st1 << static_cast<UInt8>(19);
            size_t off = st1.size();
            st1 << static_cast<UInt8>(0);

            UInt8 j = 0;
            UInt8 count = 0;
            for (SortType::reverse_iterator i = _sorts.rbegin(), e = _sorts.rend(); i != e && j < 3; ++i, ++j)
            {
                if (*i)
                {
                    st1 << (*i)->player->getName();
                    st1 << (*i)->score;
                    ++count;
                }
            }
            st1.data<UInt8>(off) = count;
            st1 << Stream::eos;
            broadcast(st1);
        }
    }

    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(5) << static_cast<UInt8>(3) << straight
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
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return false;

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

        UInt8 factor = (World::_wday == 6 || World::_wday == 7)?2:1;
        UInt8 quality = pd->awardgot;
        if (quality)
            quality -= 1;
        if (quality > 3)
            quality = 3;
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
                    if (awds->id == 2)
                        awards[j].num = awds->num * calcExp(player->GetLev()) * factor;
                    else
                        awards[j].num = awds->num * factor;
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
            player->OnHeroMemo(MC_ATHLETICS, MD_MASTER, 0, 2);

            pd->awardgot = 0xFF;
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
                    player->AddExp(awards[id].num);
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
        }
        else
            return false;
    }
    else
    {
    }

    return true;
}

void HeroIsland::saveAtoCfg(Player* player, const std::string& cfg)
{
    if (!player || !cfg.length())
        return;

    if (player->getVipLevel() < 3)
        return;

    player->setAtoHICfg(cfg);
    DB3().PushUpdateData("UPDATE `player` set `atohicfg` = '%s' WHERE `id` = %"I64_FMT"u", cfg.c_str(), player->getId());
}

void HeroIsland::sendAtoCfg(Player* player)
{
    if (!player)
        return;
    const std::string& cfg = player->getAtoHICfg();
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(18);
    st << cfg;
    st << Stream::eos;
    player->send(st);
}

void HeroIsland::setAto(Player* player, UInt8 onoff)
{
    if (!player || !onoff)
        return;

    if (!isRunning())
        return;

    if (!player->hasFlag(Player::InHeroIsland))
        return;

    if (player->getVipLevel() < 3)
        return;

    UInt8 spot = player->getHISpot();
    UInt16 pos = 0;
    HIPlayerData* pd = findPlayer(player, spot, pos);
    if (!pd)
        return;

    if (onoff == 1 && !pd->ato)
    {
        UInt32 goldUse = GData::moneyNeed[GData::AUTOHI].gold;
        if (player->getGold() < goldUse)
        {
            player->sendMsgCode(0, 1104);
            return;
        }
        ConsumeInfo ci(HeroIslandAuto,0,0);
        player->useGold(goldUse, &ci);
    }

    if (onoff > 2)
        onoff = 2;
    pd->ato = onoff;
    Stream st(REP::HERO_ISLAND);
    st << static_cast<UInt8>(20) << static_cast<UInt8>(onoff) << Stream::eos;
    player->send(st);
}

void HeroIsland::sendDaily(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(8);
    st << static_cast<UInt8>(_status);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

HeroIsland heroIsland;

}

