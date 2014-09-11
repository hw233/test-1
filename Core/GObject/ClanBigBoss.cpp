#include "ClanBigBoss.h"
#include "Player.h"
#include "Common/TimeUtil.h"
#include "GObject/Package.h"
#include "MsgID.h"
#include "Country.h"
#include "GObject/WBossMgr.h"
#include "Server/SysMsg.h"
#include "Script/BattleFormula.h"
#include "Common/TimeUtil.h"
#include "Clan.h"
#include "MapCollection.h"

namespace GObject
{

extern URandom GRND;

const UInt16 BIGBOSS = 5509;//五鬼天王尚和阳
const UInt16 BIGBOSS1 = 5469;//五鬼天王尚和阳
const UInt32 BOSS_MIN_HP = 12000000;
const UInt32 BOSS_MAX_HP = 210000000;
const Int32 BOSS_MAX_ATK= 100000;
const float BOSS_BASE_TIME = 600.f;
const float BOSS_HP_FACTOR = 1.f;
const float BOSS_ATK_FACTOR = .5f;
const float BOSS_MAX_ASC_HP_FACTOR = 1.40f;
const float BOSS_MAX_ASC_ATK_FACTOR = 1.40f;
const UInt32 EXP = 21187500;


ClanBigBoss::ClanBigBoss(Clan* clan)
{
    _clan = clan;
    m_BossHP = 0; 
    m_spirit = 10;
    s_pds.clear();
    appointment_time = 0;
    lastAttack_time = 0;
    _status = CLAN_BIGBOSS_NOINIT;
    _lastTime = 0;
    _lastHp = 0;
    _lastAtk = 0;
    _lastMAtk = 0;
    m_final = false;
    _percent = 100;
    _flag = false;
    _buffer = 0;
    _ng = NULL;    
    
}

ClanBigBoss::~ClanBigBoss()
{
}

void ClanBigBoss::setNpcBoss()
{
    GData::NpcGroup * s_ng = new GData::NpcGroup(static_cast<UInt32>(5509));
    if(!s_ng || !GData::npcGroup_5509)
        return;
    /*GData::NpcGroups::iterator it = GData::npcGroups.find(BIGBOSS);
    if(it == GData::npcGroups.end())
        return;*/

    //*s_ng = *(it->second);
    *s_ng = *(GData::npcGroup_5509);
    _ng = s_ng;
    if(!fighter_5509)
        return;
    Fighter* fgt = fighter_5509->clone(NULL);
    std::vector<GData::NpcFData>& npclist = _ng->getList();
    npclist[0].fighter = fgt;
}

void ClanBigBoss::ReqBossAppointment(UInt8 app_time/* 时间点数 */,std::string str)
{
    if(getStatus() != CLAN_BIGBOSS_NOINIT && TimeUtil::Day(appointment_time) >= TimeUtil::Day())
        return;
    appointment_time = TimeUtil::SharpDay() + app_time * 60 * 60; 
    setStatus(CLAN_BIGBOSS_AWAIT); 
    addLevel();
    m_BossHP = getLastHP();
    _percent = 100;
    m_spirit = 10;
    m_final = false;
    s_pds.clear();
    updateInfo();

    //TODO DB
    DB4().PushUpdateData("REPLACE INTO `clanbigboss` VALUES(%u, %u, %u, %u, %u, %u, %u)",_clan->getId(),static_cast<UInt8>(_status), appointment_time,_lastTime,_lastHp,_lastAtk,_lastMAtk);
        
    Stream st;
    SYSMSGVP(st, 983,_clan->getName().c_str(),str.c_str(),app_time);
    _clan->broadcast(st);
    
    return;
}

void ClanBigBoss::setStatus(EStatus eTmp)
{
    if(_status == eTmp)
        return;
    _status = eTmp;
    //TODO DB
    DB4().PushUpdateData("UPDATE `clanbigboss` SET `status` = %u WHERE `clanid` = %u ",_status,_clan->getId());

}

void ClanBigBoss::resetBossInfo()
{
    m_BossHP = MAXHP;
    setStatus(CLAN_BIGBOSS_NOINIT);
    updateInfo();
}

void ClanBigBoss::closeBossInfo(UInt8 flag)
{
    m_BossHP = 0;
    //TODO 结算Boss
    
    class StreamVisitor : public Visitor<ClanMember>
    {
        public:
            StreamVisitor(UInt8 flag,ClanBigBoss* cbb)
            {
                _flag = flag;
                _cbb = cbb;
            }

            bool operator()(ClanMember* member)
            {
                if(_cbb == NULL)
                    return false;
                _cbb->reward(member->player,_flag);
                return true;
            }
        private:
            UInt8 _flag;
            ClanBigBoss* _cbb;
    };
    
    StreamVisitor visitor(flag+2,this);//Boss击退奖励
    _clan->VisitMembers(visitor);

    if(flag)
    {
        _lastTime = 60 * 30;
        //TODO DB
        DB4().PushUpdateData("UPDATE `clanbigboss` SET `last` = %u WHERE `clanid` = %u ",_lastTime,_clan->getId());
    }

    if(!flag)
    {
        UInt8 idx = 1;
        PlayerDamageSort::iterator it = s_pds.begin(); 
        while(it != s_pds.end()) 
        {
            if(idx > 3)
                break;
            if(it->player == NULL)
                continue;
            reward(it->player,5,idx);
            it ++;
            idx ++;
        }    
    }
    
    //
    _buffer = 0;
    setStatus(CLAN_BIGBOSS_OVER);
    updateInfo();
}

void ClanBigBoss::attack(Player* pl)
{
    if (pl->getLocation() != 4612)
        return ;
        
    if (pl->getThreadId() !=  mapCollection.getCountryFromSpot(4612))
        return ;

    if (_ng == NULL || _ng1 == NULL)
        return ;
    
    if (pl->GetPackage()->GetRestPackageSize() <= 0)
    {
        pl->sendMsgCode(0, 1011);
        return ;
    }

    if(_status != CLAN_BIGBOSS_BEGIN)
        return;

    bool res = false;
    if(!m_final)
        res = attackBoss(pl);
    else
        res = attackBossFinal(pl);

    if (res && !m_final)//打掉元神
    {
        reward(pl,4);
        m_spirit --; 
        if(m_spirit == 0)
            m_final = true;
    }
    else if (res && m_final)
    {
        reward(pl,0);
        closeBossInfo();
    }
    updateInfo();

    return;
}


bool ClanBigBoss::attackBoss(Player* player)
{
    if(!getHP() || getHP() == MAXHP)
        return false;
    if (!player) return false;

    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        player->sendMsgCode(0, 1407, buffLeft - now);
        return false;
    }
    player->checkLastBattled();

    UInt16 ret = 0x0100;

    Battle::BattleSimulator bsim(Battle::BS_WBOSS, player, _ng1->getName(), 255, true);
    player->PutFighters(bsim, 0);
    _ng1->putFighters(bsim);
    
    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;
    bool res = bsim.getWinner() == 1;

    UInt64 exp = 0 ; 
    if(res)
    {
        ret = 0x0101;
        player->_lastNg = _ng1;
        exp = _ng1->getExp(); 
        player->pendExp(exp);
        //_ng1->getLoots(player, player->_lastLoot, 1, NULL);
    }
        
    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(player, lastExp) << static_cast<UInt8>(0);
    UInt8 size = player->_lastLoot.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
    {
        st << player->_lastLoot[i].id << player->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << static_cast<UInt64>(exp);
    st << Stream::eos;
    player->send(st);
    bsim.applyFighterHP(0, player);

    player->setBuffData(PLAYER_BUFF_ATTACKING, now + 30);

    return res;
}

bool ClanBigBoss::attackBossFinal(Player* player)
{
    if(!getHP() || getHP() == MAXHP)
        return false;
    if (!player) return false;

    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(buffLeft > now)
    {
        player->sendMsgCode(0, 1407, buffLeft - now);
        return false;
    }
    player->checkLastBattled();

    bool res = false;
    UInt16 ret = 0x0100;

    SetDirty(player,true);
    Battle::BattleSimulator bsim(Battle::BS_WBOSS, player, _ng->getName(), 255, false);
    player->PutFighters(bsim, 0);
    //_ng->putFighters(bsim);

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    if(nflist.size() == 0)
       return false;
    
    GData::NpcFData& nfdata = nflist[0];
    Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
    bf->setHP(m_BossHP);
    TRACE_LOG("Clan_BOSS INSERT ret: %u (pid: %" I64_FMT "u, m_BossHP: %u, _lastHp: %u)", ret, player->getId(), m_BossHP, _lastHp);
    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;
    res = bsim.getWinner() == 1;
    Battle::BattleObject * obj = bsim(1, nfdata.pos);
    
    reward(player,1);//幸运奖励
    UInt32 oldHP = m_BossHP;
    Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
    m_BossHP = bfgt->getHP();
    UInt32 damage = oldHP - m_BossHP;
    insertDamage(player,damage);
    UInt64 exp = 0;
    //UInt64 exp = ((float)damage / _lastHp) * EXP * 1;
    //if (exp < 1000)
    //    exp = 1000;
    //player->pendExp(exp);

    TRACE_LOG("Clan_BOSS INSERT ret: %u (pid: %" I64_FMT "u, dmg: %u)", ret, player->getId(), damage);
    if(damage > 2000000)
        TRACE_LOG("Clan_BOSS reportid: %u (pid: %" I64_FMT "u, dmg: %u)", bsim.getId(), player->getId(), damage);

    UInt8 newPercent = ((float)m_BossHP / nflist[0].fighter->getMaxHP()) * 100;

    if (newPercent > 100)
        newPercent = 100;
    if (_percent < newPercent)
        _percent = newPercent;

    if(!newPercent && m_BossHP == 0)
    {
        Stream st;
        SYSMSGVP(st, 982, nflist[0].fighter->getId());
        _clan->broadcast(st);
        nflist[0].fighter->setExtraAttack(0);
        nflist[0].fighter->setExtraMagAttack(0);
        nflist[0].fighter->setWBoss(false);
        _lastTime = now - appointment_time; 
        //TODO DB
        DB4().PushUpdateData("UPDATE `clanbigboss` SET `last` = %u WHERE `clanid` = %u ",_lastTime,_clan->getId());
        res = true;

    }else if(_percent > 75 && newPercent <= 75)
    {
        Stream st;
        SYSMSGVP(st,966, player->getCountry(), player->getName().c_str(), nflist[0].fighter->getId(), 75);
        _clan->broadcast(st);
    }else if(_percent > 50  && newPercent <= 50)
    {
        Stream st;
        SYSMSGVP(st,966, player->getCountry(), player->getName().c_str(), nflist[0].fighter->getId(), 50);
        _clan->broadcast(st);
    }else if(_percent > 25  && newPercent <= 25)
    {
        Stream st;
        SYSMSGVP(st,966, player->getCountry(), player->getName().c_str(), nflist[0].fighter->getId(), 25);
        _clan->broadcast(st);
    }else if(_percent > 5 && newPercent <= 5)
    {
        Stream st;
        SYSMSGVP(st,966, player->getCountry(), player->getName().c_str(), nflist[0].fighter->getId(), 5);
        _clan->broadcast(st);
    }
    
    _percent = newPercent;

    if(res)
        ret = 0x0101;
        
    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(player, lastExp) << static_cast<UInt8>(0);
    UInt8 size = player->_lastLoot.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
    {
        st << player->_lastLoot[i].id << player->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << static_cast<UInt64>(exp);
    st << Stream::eos;
    player->send(st);
    bsim.applyFighterHP(0, player);
    SetDirty(player,false);

    player->setBuffData(PLAYER_BUFF_ATTACKING, now + 30);

    return res;


}

void ClanBigBoss::reward(Player* player,UInt8 type ,UInt8 idx)
{
    if(player->GetVar(VAR_CLANBOSS_CLANBIGBOSS_LIMIT))
        return;
    
    switch(type)
    {
        case 0 ://击杀奖励
            {
                MailPackage::MailItem item1[] = {{9478,1}};
                player->sendMailItem(971, 972, item1, 1, false);
                Stream st;
                SYSMSGVP(st, 979, player->getName().c_str());
                _clan->broadcast(st);
            }
            break;
        case 1 ://幸运奖励
            {
                if(player->GetVar(VAR_CLANBOSS_LUCKY_LIMIT) >= 3)
                    return;
                UInt8 rand = uRand(100)+1;
                if(rand > 10)
                    return;
                MailPackage::MailItem item1[] = {{15,2},{56,1}};
                player->sendMailItem(967, 968, item1, 2, false);
                player->AddVar(VAR_CLANBOSS_LUCKY_LIMIT,1);
                Stream st;
                SYSMSGVP(st, 977, player->getName().c_str());
                _clan->broadcast(st);
            }
            break;
        case 2 ://终结奖励1
            {
                MailPackage::MailItem item1[] = {{9478,2}};
                player->sendMailItem(973, 974, item1, 1, false);
            }
            break;
        case 3 ://终结奖励2
            {
                MailPackage::MailItem item1[] = {{9478,1}};
                player->sendMailItem(973, 974, item1, 1, false);
            }
            break;
        case 4 ://秘密宝盒
            {
                if(player->GetVar(VAR_CLANBOSS_SECURE_LIMIT) >= 3)
                    return;
                MailPackage::MailItem item1[] = {{15,1},{133,1}};
                player->sendMailItem(975, 976, item1, 2, false);
                player->AddVar(VAR_CLANBOSS_SECURE_LIMIT,1);
            }
            break;
        case 5 ://伤害奖励
            {
                if(idx != 1 && idx != 2 && idx != 3)
                    return;
                MailPackage::MailItem item1[] = {{9478,static_cast<UInt32>(6 - idx)}};
                player->sendMailItem(969, 970, item1, 1, false);
                Stream st;
                SYSMSGVP(st, 978, player->getName().c_str(),idx);
                _clan->broadcast(st);
            }
            break;
        case 6 ://宕机补偿
            {
                MailPackage::MailItem item1[] = {{9478,3}};
                player->sendMailItem(980, 981, item1, 1, false);
            }
            break;
        default:
            break;


    }

    return;
}


void ClanBigBoss::insertDamage(Player* pl,UInt32 damage)
{
    PlayerDamage pd;    
        
    for(PlayerDamageSort::iterator i = s_pds.begin(); i != s_pds.end(); ++i)
    {
        if(i->player == NULL)
            continue;

        if(i->player == pl)    
        {
            pd.player = i->player;  
            pd.damage = i->damage;  
            s_pds.erase(i);
            break;
        }
    }

    //if(pd.player == 0 || pd.damage == 0 || pd.time == 0)
    pd.player = pl;  
    pd.damage = pd.damage + damage;  
    pd.time = TimeUtil::Now();  
    
    s_pds.insert(pd);

    return;
}


void ClanBigBoss::addLevel()
{
    GData::NpcGroups::iterator it1 = GData::npcGroups.find(BIGBOSS1);
    if(it1 == GData::npcGroups.end())
        return;

    _ng1 = it1->second;
    if (!_ng1) return;

    /*GData::NpcGroups::iterator it = GData::npcGroups.find(BIGBOSS);
    if(it == GData::npcGroups.end())
        return;

    _ng = it->second;*/
    if (!_ng) return;

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    
    Int32 baseatk = Script::BattleFormula::getCurrent()->calcAttack(nflist[0].fighter);
    Int32 basematk = Script::BattleFormula::getCurrent()->calcMagAttack(nflist[0].fighter);
    
    if(getLastHP() == 0)//第一次刷BOSS
    {
        worldBoss.calInitClanBigBoss(_lastHp,_lastAtk,_lastMAtk);  
        if(_lastHp == 0 || _lastAtk == 0 || _lastMAtk == 0)
        {
            nflist[0].fighter->setWBoss(true);
            _lastHp = nflist[0].fighter->getMaxHP() * 0.6; 
            _lastAtk = baseatk * 0.6;
            _lastMAtk = basematk * 0.6;
            nflist[0].fighter->setBaseHP(_lastHp);
            nflist[0].fighter->setExtraAttack(_lastAtk - baseatk);
            nflist[0].fighter->setExtraMagAttack(_lastMAtk - basematk);

            return;
        }

        nflist[0].fighter->setWBoss(true);
        m_BossHP = _lastHp;
        nflist[0].fighter->setBaseHP(_lastHp);
        //if(_lastAtk > baseatk)
            nflist[0].fighter->setExtraAttack(_lastAtk - baseatk);
        //if(_lastMAtk > basematk)
            nflist[0].fighter->setExtraMagAttack(_lastMAtk - basematk);
    }
    else
    {
        UInt32 ohp = 0;
        Int32 extatk = 0;
        Int32 extmagatk = 0;
        Int32 atk = 0;
        Int32 matk = 0;
                      
        ohp = getLastHP();
        atk = getLastAtk();
        matk = getLastMAtk();

        if(_lastTime == 0)
            _lastTime = 60 * 30;
        float hp_factor = (float)BOSS_BASE_TIME / (float)_lastTime;
        if(hp_factor > BOSS_MAX_ASC_HP_FACTOR)
            hp_factor = BOSS_MAX_ASC_HP_FACTOR;
        ohp = ohp * hp_factor;
        if (ohp < BOSS_MIN_HP)
            ohp = BOSS_MIN_HP;
        if (ohp > BOSS_MAX_HP)
            ohp = BOSS_MAX_HP;

        m_BossHP = ohp;
        nflist[0].fighter->setBaseHP(ohp);
        nflist[0].fighter->setWBoss(true);

        float atk_factor = (BOSS_BASE_TIME/(float)_lastTime - 1.f) * BOSS_ATK_FACTOR;
        if(atk_factor > BOSS_MAX_ASC_ATK_FACTOR)
            atk_factor = BOSS_MAX_ASC_ATK_FACTOR;
        extatk = atk_factor * (atk + baseatk) ;
        if(extatk > BOSS_MAX_ATK)
            extatk = BOSS_MAX_ATK;
        else if(extatk < 0)
            extatk = 0;
        nflist[0].fighter->setExtraAttack(extatk + atk);

        extmagatk = atk_factor * (matk + basematk) ;
        if(extmagatk > BOSS_MAX_ATK)
            extmagatk = BOSS_MAX_ATK;
        else if(extmagatk < 0)
            extmagatk = 0;
        nflist[0].fighter->setExtraMagAttack(extmagatk + matk);

        _lastHp = ohp;
        _lastAtk = extatk + atk;
        _lastMAtk = extmagatk + matk;

    }

    //TODO DB
    DB4().PushUpdateData("UPDATE `clanbigboss` SET `hp` = %u, `atk` = %u, `matk` = %u WHERE `clanid` = %u ",_lastHp, _lastAtk,_lastMAtk ,_clan->getId());

}

void ClanBigBoss::broadcastStream(UInt8 status)
{
    if (NULL == _clan || (status != 0 && status != 1 && status != 2 && status != 3))
        return;
    class StreamVisitor : public Visitor<ClanMember>
    {
        public:
            StreamVisitor(UInt8 flag,ClanBigBoss* cbb)
            {
                _flag = flag;
                _cbb = cbb;
            }

            bool operator()(ClanMember* member)
            {
                if(_cbb == NULL)
                    return false;
                _cbb->ReturnBossInfo(member->player,_flag);
                return true;
            }
        private:
            UInt8 _flag;
            ClanBigBoss* _cbb;
    };
    StreamVisitor visitor(status,this);
    _clan->VisitMembers(visitor);

}

void ClanBigBoss::process(UInt32 now)
{
    if (NULL == _clan || 0 == now)
        return;
    
    //if (now >= (appointment_time - 10 * 60) && status == CLAN_BIGBOSS_AWAIT)
       //_prepareStep = 1;

    class StreamVisitor : public Visitor<ClanMember>
    {
        public:
            StreamVisitor(UInt8 buffer_)
            {
                buffer = buffer_;
            }

            bool operator()(ClanMember* member)
            {
                //SYSMSG_SENDV(MsgID, member->player);
                if(member->player == NULL)
                    return false;
                Stream st(REP::CLAN_COPY);  
                st << static_cast<UInt8>(0x10) << static_cast<UInt8>(5) << buffer;
                st << Stream::eos;
                member->player->send(st);
                return true;
            }
        private:
            UInt8 buffer;
    };

    switch (_status)
    {
        case CLAN_BIGBOSS_AWAIT:
        {
            if (now < appointment_time - 10 * 60)
                return;

            Stream st;
            if(now == appointment_time - 10 * 60)
            {
                //StreamVisitor visitor(963);//10分钟
                SYSMSGVP(st, 963, _clan->getName().c_str());
                //_clan->VisitMembers(visitor);
                _clan->broadcast(st);
            }
            
            if(now == appointment_time - 5 * 60)
            {
                //StreamVisitor visitor(963);
                //_clan->VisitMembers(visitor);
                SYSMSGVP(st, 964, _clan->getName().c_str());
                _clan->broadcast(st);
            }
            
            if(now == appointment_time )
            {
                //StreamVisitor visitor(963);//开始
                //_clan->VisitMembers(visitor);
                //SYSMSGVP(st, 965, _clan->getName().c_str());
                
                Stream st(REP::CLAN_COPY);  
                st << static_cast<UInt8>(0x10) << static_cast<UInt8>(4);
                st << Stream::eos;
                _clan->broadcast(st);


                setStatus(CLAN_BIGBOSS_BEGIN);
                updateInfo();
            }
            
            if(now > appointment_time)
                closeBossInfo(1);

            break;
        }

        case CLAN_BIGBOSS_BEGIN:
        {
            if(now == appointment_time + 5 * 60)
            {
                _buffer = 1; 
                StreamVisitor visitor(_buffer);
                _clan->VisitMembers(visitor);
            }
            if(now == appointment_time + 10 * 60)
            {
                _buffer = 2; 
                StreamVisitor visitor(_buffer);
                _clan->VisitMembers(visitor);
            }
            if(now == appointment_time + 15 * 60)
            {
                _buffer = 3; 
                StreamVisitor visitor(_buffer);
                _clan->VisitMembers(visitor);
            }
            
            if (now < appointment_time + 30 * 60)
                return;
            //StreamVisitor visitor(1111);//Boss打完了
            //_clan->VisitMembers(visitor);
            closeBossInfo(1);
            //if(now > appointment_time + 30 * 60)
               // offsetException();
            break;
        }

        default:
            break;
    }
    
    return;
}

void ClanBigBoss::updateInfo()
{
    EStatus e_status = getStatus();
    broadcastStream(static_cast<UInt8>(e_status));

    return;
}

void ClanBigBoss::ReturnBossInfoPl(Player* pl)
{
    EStatus e_status = getStatus();
    ReturnBossInfo(pl,static_cast<UInt8>(e_status)); 
}

void ClanBigBoss::ReturnBossInfo(Player* pl,UInt8 status)
{
    if(getFlag())
    {
        if (!_ng) return;
        std::vector<GData::NpcFData>& nflist = _ng->getList();
        Int32 baseatk = Script::BattleFormula::getCurrent()->calcAttack(nflist[0].fighter);
        Int32 basematk = Script::BattleFormula::getCurrent()->calcMagAttack(nflist[0].fighter);
        nflist[0].fighter->setWBoss(true);
        nflist[0].fighter->setBaseHP(_lastHp);
        nflist[0].fighter->setExtraAttack(_lastAtk - baseatk);
        nflist[0].fighter->setExtraMagAttack(_lastMAtk - basematk);
                 
        setFlag(false);
    }
    
    UInt32 now = TimeUtil::Now();
    //if(_status == CLAN_BIGBOSS_OVER && TimeUtil::Day() != TimeUtil::Day(appointment_time))
    if(_status == CLAN_BIGBOSS_OVER && !TimeUtil::SameDay(now,appointment_time))
    {
        UInt32 nowday = TimeUtil::SharpDay(0,now);
        UInt32 appday = TimeUtil::SharpDay(0,appointment_time);
        if(nowday > appday)
            resetBossInfo();
    }

    Stream st(REP::CLAN_COPY);  
    st << static_cast<UInt8>(0x10) << static_cast<UInt8>(1);
    st << status;
    UInt32 leaveTime = 0;
    switch(status)
    {
        case 0 :
            break;
        case 1:
            leaveTime = appointment_time - TimeUtil::Now();
            st << leaveTime;
            break;
        case 2 :
            {
                UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING,now);
                if(buffLeft)
                    buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING,now) - now;

                st << buffLeft << _lastHp << m_BossHP << m_spirit ;
                
                size_t off = st.size();
                st << static_cast<UInt8>(0);
                UInt8 size = 0;
                for(PlayerDamageSort::iterator i = s_pds.begin(); i != s_pds.end(); ++i)
                {
                    if(i->player == NULL)
                        continue;
                    st << i->player->getName() << i->damage ;
                    ++size;
                    if(size >= 10)
                        break;
                }
                
                if (size)
                    st.data<UInt8>(off) = size;
            }
            break;
        case 3:
            break;
        default:
            break;
    }
    st << Stream::eos;
   
    pl->send(st);

    if(_buffer == 1 || _buffer == 2 || _buffer == 3) 
    {
        Stream st1(REP::CLAN_COPY);  
        st1 << static_cast<UInt8>(0x10) << static_cast<UInt8>(5) << _buffer;
        st1 << Stream::eos;
        pl->send(st1);
    }


    return;
}

void ClanBigBoss::offsetException()
{
    class StreamVisitor : public Visitor<ClanMember>
    {
        public:
            StreamVisitor(UInt8 flag,ClanBigBoss* cbb)
            {
                _flag = flag;
                _cbb = cbb;
            }

            bool operator()(ClanMember* member)
            {
                if(_cbb == NULL)
                    return false;
                _cbb->reward(member->player,_flag);
                return true;
            }
        private:
            UInt8 _flag;
            ClanBigBoss* _cbb;
    };

    StreamVisitor visitor(6,this);//Boss宕机奖励
    _clan->VisitMembers(visitor);

    return;
}

void ClanBigBoss::GMAttackBoss(UInt32 attack_boss)
{
    m_BossHP -= attack_boss;
    updateInfo();
}

void ClanBigBoss::LoadFromDB(DBClanBigBoss* dcbb)
{
    _lastTime = dcbb->last; 
    _lastHp = dcbb->hp; 
    _lastAtk = dcbb->atk; 
    _lastMAtk = dcbb->matk; 
    _status = static_cast<EStatus>(dcbb->status);
    appointment_time = dcbb->app_time;
    m_BossHP = _lastHp;
    
    if(_status != CLAN_BIGBOSS_NOINIT && _status != CLAN_BIGBOSS_OVER)
        ClanBigBossMgr::Instance().insertMap(_clan->getId(),this); 

    GData::NpcGroups::iterator it1 = GData::npcGroups.find(BIGBOSS1);
    if(it1 == GData::npcGroups.end())
        return;

    _ng1 = it1->second;
    if (!_ng1) return;

    GData::NpcGroups::iterator it = GData::npcGroups.find(BIGBOSS);
    if(it == GData::npcGroups.end())
        return;
  
    if(dcbb->status == CLAN_BIGBOSS_AWAIT)
        setFlag(true);
    
}

void ClanBigBoss::SetDirty(Player* player,bool _iscbbbuf)
{
    std::map<UInt32, Fighter *>& fighters = player->getFighterMap();
    float factor = 0.0f; 
    switch(_buffer)
    {
        case 1:
            factor = 0.5; 
            break;
        case 2:
            factor = 1.0; 
            break;
        case 3:
            factor = 2.0; 
            break;
        default:
            break;
    }

    for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
    {
        Int32 baseatk = Script::BattleFormula::getCurrent()->calcAttack(it->second);
        Int32 basematk = Script::BattleFormula::getCurrent()->calcMagAttack(it->second);
        
        it->second->setPlCBBExtraAttack(static_cast<Int32>(baseatk * factor));
        it->second->setPlCBBExtraMagAttack(static_cast<Int32>(basematk * factor));

        it->second->setClanBigBossBuf(_iscbbbuf); 
        it->second->setDirty();
    }

}


///////////////////////////


ClanBigBossMgr::ClanBigBossMgr()
{
    _clanBB.clear();  

}

ClanBigBossMgr::~ClanBigBossMgr()
{
    _clanBB.clear();  

}

void ClanBigBossMgr::insertMap(UInt32 clanid,ClanBigBoss* cbb)
{
    _clanBB.insert(std::make_pair(clanid,cbb));
}

void ClanBigBossMgr::closeAllBossInfo()
{
    for (MClanBB::iterator it = _clanBB.begin(); it != _clanBB.end();)
    {
        if(it->second->getStatus() != CLAN_BIGBOSS_OVER)//帮派BOSS没打完
        {
            //it->second->offsetException();
            it->second->closeBossInfo(1);
        }
        _clanBB.erase(it++);
    }

    return;
}

void ClanBigBossMgr::process(UInt32 nowTime)
{
    if(GVAR.GetVar(GVAR_CLAN_BIGBOSS_ENABLE))// 0 - 默认开启  
        return;
    UInt32 nowday = TimeUtil::SharpDayT();

    if(nowTime > TimeUtil::SharpHour(22,nowday) + 30 * 60)//22:30分以后
    {
        closeAllBossInfo();//结束当天所有的Boss
        GVAR.SetVar(GVAR_CLAN_BIGBOSS_ENABLE,1);
        return;
    }

    if(nowTime >= TimeUtil::SharpHour(9,nowday) - 10 * 60)//8点50以后
    {
        for (MClanBB::iterator it = _clanBB.begin(); it != _clanBB.end();)
        {
            if(it->second->getStatus() == CLAN_BIGBOSS_OVER)
                _clanBB.erase(it++);
            else
            {
                it->second->process(nowTime);
                it++;
            }
            
        }
    }

    return;
}

void ClanBigBossMgr::playerReqStart(Player * pl,Clan* clan,UInt8 hour)//预约Boss时间 
{
    MClanBB::iterator it = _clanBB.begin(); 
    while(it != _clanBB.end()) 
    {
        if(it->first == clan->getId())
            break;
        it ++;
    }
    if(it != _clanBB.end())
        return;
    else
    {
        clan->getClanBigBoss()->ReqBossAppointment(hour,pl->getName());
        _clanBB.insert(std::make_pair(clan->getId(),clan->getClanBigBoss()));
    }

    return;
}


}



