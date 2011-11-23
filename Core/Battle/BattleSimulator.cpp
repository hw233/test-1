#include "Config.h"
#include "BattleSimulator.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "Script/BattleFormula.h"
#include "BattleReport.h"
#include "Server/OidGenerator.h"

#include "Common/DirectoryIterator.h"
#include "Server/Cfg.h"
#include "MsgID.h"

namespace Battle
{

static UInt8 getPosition(UInt16 loc)
{
#if 0
	if(loc == 0x6FFF)
		return 6;
	if(loc == 0x7FFF)
		return 7;
	if(loc == 0x8FFF)
		return 8;
	loc >>= 8;
	if(loc > 6)
		return 3;
	if(loc > 5)
		return 1;
#endif
    UInt16 tmp = loc;
    if(loc < static_cast<UInt16>(0xF000))
        tmp = loc >> 8;

    UInt16 pos = GObject::GObjectManager::getBattleScene(tmp);
	return static_cast<UInt8>(pos);
}

BattleSimulator::BattleSimulator(UInt32 location, GObject::Player * player, const std::string& name2, UInt8 level2, bool rpt, UInt32 fr): BattleField(), _id(rpt ? IDGenerator::gBattleOidGenerator.ID() : 0), _winner(0), _turns(0), _report(rpt), _fake_turns(fr), _formula(Script::BattleFormula::getCurrent())
{
	_position = getPosition(location);
	player->testBattlePunish();
	_player[0] = player;
	_player[1] = NULL;
	_other_name = name2;
	_other_level = level2;
	_portrait[0] = 0;
	_portrait[1] = 0;
    _cur_fgtlist_idx = 0;
}

BattleSimulator::BattleSimulator(UInt32 location, GObject::Player * player, GObject::Player * player2, bool rpt, UInt32 fr): BattleField(), _id(rpt ? IDGenerator::gBattleOidGenerator.ID() : 0), _winner(0), _turns(0), _report(rpt), _fake_turns(fr), _formula(Script::BattleFormula::getCurrent())
{
	_position = getPosition(location);
	player->testBattlePunish();
	_player[0] = player;
	_player[1] = player2;
	_other_name = player2->getName();
	_other_level = player2->GetLev();
	_portrait[0] = 0;
	_portrait[1] = 0;
    _cur_fgtlist_idx = 0;
}

// #############################
// change the battle queue but not pass client check
void BattleSimulator::start()
{
	_packet.clear();
	_fgtlist[0].clear();
	_fgtlist[1].clear();

	// [[ Make packet header data
	_packet.init(REP::FIGHT_START);
	_packet << _id << _position;
	for(int i = 0; i < 2; ++ i)
		_packet << (_formation[i] ? _formation[i]->getId() : static_cast<UInt16>(0));
	_packet << _player[0]->GetLev() << _other_level;
	_packet << _portrait[0] << _portrait[1];

	_packet << _player[0]->getName() << _other_name;

	size_t offset = _packet.size();

	_packet << static_cast<UInt16>(0);

    // for the insertFighterStatus can only insert to next fgtlist, so to make sure to insert fighter status to _figtlist[0] and set the _cur_fgtlist to 1
    _cur_fgtlist_idx = 1;
	UInt8 cnt[2] = {0, 0};
	bool loaded[2] = {false, false};
	bool checkEnh = _player[1] == NULL;
	UInt32 now = TimeUtil::Now();
	for(int i = 0; i < 2; ++ i)
	{
		UInt32 flag = 0;
		if(_player[i] != NULL)
		{
			if(checkEnh)
			{
				if(_player[i]->getBuffData(PLAYER_BUFF_ATTR3, now) > 0)
					flag = 0x0C;
				else if(_player[i]->getBuffData(PLAYER_BUFF_ATTR2, now) > 0)
					flag = 0x08;
				else if(_player[i]->getBuffData(PLAYER_BUFF_ATTR1, now) > 0)
					flag = 0x04;
			}
			if(_player[i]->getBuffData(PLAYER_BUFF_CLANRCENHANCE, now) > 0)
			{
				flag |= BattleFighter::Enh3;
			}
		}
		for(int j = 0; j < 25; ++ j)
		{
			BattleObject * bo = _objs[i][j];
			if(bo != NULL)
			{
				if(bo->isChar())
				{
					BattleFighter * bf = static_cast<BattleFighter *>(bo);
					bool ismain = !_isBody[i][j];
                    UInt8 aura = 0;
                    UInt8 maxAura = 0;
					if(ismain)
					{
						if(flag > 0)
						{
							bf->addFlag(flag);
							if(!loaded[i])
							{
								cnt[i] |= (flag >> 2) << 5;
								loaded[i] = true;
							}
						}
						bf->initStats(checkEnh);
                        UInt8 justice_roar = (_player[i] != NULL ? _player[i]->getJusticeRoar() : 0);
                        if(justice_roar)
                            bf->AddAura(justice_roar);

                        aura = bf->getAura();
                        maxAura = bf->getAuraMax();
					}
					UInt32 maxhp = bf->getMaxHP();
					_packet << static_cast<UInt8>(j + 1) << bf->getFighter()->getBattleName();
					if(bf->getFighter()->isNpc())
						_packet << static_cast<UInt8>(bf->getFighter()->reqFriendliness);
					else
						_packet << bf->getFighter()->getClassAndSex();
					_packet << static_cast<UInt8>(_isBody[i][j] > 0 ? (_isBody[i][j] - 1) : bf->getFighter()->getLevel()) << bf->getPortrait() << static_cast<UInt8>(bf->getFlag() & 0x03);
					_packet << static_cast<UInt8>(bf->getFighter()->getColor())
						<< static_cast<UInt32>(_isBody[i][j] ? 0 : bf->getHP()) << static_cast<UInt32>(maxhp);

					_packet << aura << maxAura <<static_cast<UInt16>(bf->getAttack()) << static_cast<UInt16>(bf->getDefend()) << static_cast<UInt16>(bf->getAction());
                    // TODO: up skills
					_packet << static_cast<UInt16>(bf->getHitrate(NULL) * 100.0f) << static_cast<UInt16>(bf->getEvade(NULL) * 100.0f) << static_cast<UInt16>(bf->getCritical(NULL) * 100.0f) << static_cast<UInt16>(bf->getPierce(NULL) * 100.0f) << static_cast<UInt16>(bf->getCounter(NULL) * 100.0f) << static_cast<UInt16>(bf->getTough(NULL) * 100.0f) << static_cast<UInt16>(bf->getMagRes(NULL) * 100.0f);
                    _packet << bf->getFighter()->getPeerlessAndLevel();
                    bf->getFighter()->getAllUpSkillAndLevel(_packet);
                    bf->getFighter()->getAllPSkillAndLevel(_packet);
                    
					if(ismain)
					{
						bf->postInit();
						// FighterStatus fs(bf);
						// Insert into action queue
						insertFighterStatus(bf);
					}
				}
				else
				{
					_packet << static_cast<UInt8>(0) << static_cast<UInt8>(bo->getClass()) << static_cast<UInt8>(j + 1) << static_cast<UInt16>(bo->getHP());
				}
				++ cnt[i];
			}
		}
	}

    // after insert fighter status to _fgtlist[0] set the _cur_fgtlist_idx to 0
    _cur_fgtlist_idx = 0;
	if(_fgtlist[_cur_fgtlist_idx].empty())
	{
		_packet.clear();
		return;
	}
	_packet.data<UInt8>(offset) = cnt[0];
	_packet.data<UInt8>(offset + 1) = cnt[1];
	size_t cnt_pos = _packet.size();
	_packet << static_cast<UInt32>(0);
	// ]]

	UInt32 act_count = 0;
	_winner = testWinner();
    act_count += FightersEnter();
	while(_winner == 0 && act_count < _fake_turns)
	{
		int pos = findFirstAttacker();

		act_count += doAttack(pos);
#ifdef _DEBUG
        char path[1024], path2[1024];
        sprintf(path, "%s0/0", cfg.reportPath.c_str());
        sprintf(path2, "%s/%u.log", path, 0);
        File rfile(path);
        rfile.createDirectories();
        FILE * f = fopen(path2, "a+");

        if(f != NULL)
        {
            char szBuf[256] = {0};
            sprintf(szBuf, "rcnt=%d\r\n", act_count);
            fwrite(szBuf, 1, strlen(szBuf), f);
            fclose(f);
        }
#endif
	}
	_packet << static_cast<UInt8>(_winner);
	_packet.data<UInt32>(cnt_pos) = act_count;
	_packet << Stream::eos;
//	printf("Winner is: %d,  actions: %d\n", _winner, act_count);
#ifdef _DEBUG
    char path[1024], path2[1024];
    sprintf(path, "%s0/0", cfg.reportPath.c_str());
    sprintf(path2, "%s/%u.log", path, 0);
    File rfile(path);
    rfile.createDirectories();
    FILE * f = fopen(path2, "a+");

	if(f != NULL)
    {
        char szBuf[256] = {0};
        sprintf(szBuf, "act_count=%d\r\n\r\n\r\n", act_count);
        fwrite(szBuf, 1, strlen(szBuf), f);
        fclose(f);
    }
#endif

	_turns = act_count;

	if(act_count == 0)
		_winner = 1;

	if(_report || !cfg.GMCheck)
		battleReport.addReport(_id, _packet);
}

void BattleSimulator::insertFighterStatus( BattleFighter* bf )
{
    Int8 next_fgtlist_idx = _cur_fgtlist_idx == 0 ? 1 : 0;
    std::vector<BattleFighter*>& next_fgtlist = _fgtlist[next_fgtlist_idx];
	int cnt = static_cast<int>(next_fgtlist.size());
	for(int i = 0; i < cnt ; ++ i)
	{
        UInt32 bf_act = bf->getAction();
        BattleFighter* next_bf = next_fgtlist[i];
        UInt32 next_act = next_bf->getAction();
		if(next_act < bf_act)
		{
			next_fgtlist.insert(next_fgtlist.begin() + i, bf);
			return;
		}
	}
	next_fgtlist.insert(next_fgtlist.end(), bf);
}

void BattleSimulator::removeFighterStatus( BattleFighter* bf )
{
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[fgtlist_idx];
        size_t c = cur_fgtlist.size();
        size_t i = 0;
        while(i < c)
        {
            if(cur_fgtlist[i] == bf)
            {
                cur_fgtlist.erase(cur_fgtlist.begin() + i);
                -- c;
            }
            else
                ++ i;
        }
    }
}

void BattleSimulator::reQueueFighterStatus(BattleFighter* bf)
{
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[fgtlist_idx];
        size_t c = cur_fgtlist.size();
        size_t i = 0;
        while(i < c)
        {
            if(cur_fgtlist[i] == bf)
            {
                cur_fgtlist.erase(cur_fgtlist.begin() + i);
                -- c;

                std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[fgtlist_idx];
                int cnt = static_cast<int>(cur_fgtlist.size());
                int idx = 0;
                for(; idx < cnt ; ++ idx)
                {
                    UInt32 bf_act = bf->getAction();
                    BattleFighter* cur_list_bf = cur_fgtlist[idx];
                    UInt32 cur_list_act = cur_list_bf->getAction();
                    if(cur_list_act < bf_act)
                    {
                        cur_fgtlist.insert(cur_fgtlist.begin() + idx, bf);
                        break;
                    }
                }

                if( idx == cnt )
                    cur_fgtlist.insert(cur_fgtlist.end(), bf);

            }
            else
                ++ i;
        }
    }
}

// Find first attacker and update action points
int BattleSimulator::findFirstAttacker()
{
	// Randomly select an attacker in that some fighters have the same action points
    if(_fgtlist[_cur_fgtlist_idx].size() == 0)
    {
        _cur_fgtlist_idx = _cur_fgtlist_idx == 0 ? 1 : 0;
    }

    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
	size_t c = 1, cnt = cur_fgtlist.size();
    BattleFighter* bf = cur_fgtlist[0];
    UInt32 act = bf->getAction();
	while(c < cnt)
    {
        BattleFighter* bf = cur_fgtlist[c];
        UInt32 act_tmp = bf->getAction();
        if(act != act_tmp)
            break;

		++ c;
    }

	if(c == 1)
		return 0;
	return _rnd(c);
}

// XXX: 取消救援
#if 0
float BattleSimulator::testRescue(BattleFighter *& bf, int counter_deny, AttackPoint * counter_deny_list)
{
	const GData::Formation::GridEffect * effect = bf->getFormationEffect();
	if(effect != NULL && effect->rescue_ratio > 0)
	{
		UInt8 side = bf->getSide();
		BattleObject * rescue_obj = _objs[side][effect->rescue];
		if(rescue_obj != NULL && rescue_obj->isChar() && rescue_obj->getHP() > 0)
		{
			UInt8 ratio = effect->rescue_ratio;
			BattleFighter * target_bf = static_cast<BattleFighter *>(rescue_obj);
			if(target_bf->getFighter()->isMale() != bf->getFighter()->isMale())
				ratio += 3;
			if(_rnd(100) < ratio)
			{
				if(counter_deny > 0)
				{
					for(int i = 0; i < counter_deny; ++ i)
					{
						if(counter_deny_list[i].pos == effect->rescue)
							return 0.0f;
					}
				}
				bf = static_cast<BattleFighter *>(rescue_obj);
				return 0.1f;
			}
		}
	}
	return 0.0f;
}
#endif

#if 0
float BattleSimulator::testLink( BattleFighter *& bf, UInt16& skillId )
{
	const GData::Formation::GridEffect * effect = bf->getFormationEffect();
	if(effect != NULL && effect->link_ratio > 0)
	{
		BattleObject * link_obj = _objs[bf->getSide()][effect->link];
		if(link_obj != NULL && link_obj->isChar() && link_obj->getHP() > 0)
		{
			UInt8 ratio = effect->link_ratio;
			BattleFighter * target_bf = static_cast<BattleFighter *>(link_obj);
			if(target_bf->getFighter()->isMale() != bf->getFighter()->isMale())
				ratio += 3;
			if(_rnd(100) < ratio)
			{
				bf = target_bf;
				return 1.0f;
			}
		}
	}
    // TODO: 
#if 0
	UInt32 sid = bf->getFighter()->getSkill();
	switch(sid)
	{
	case 301:
	case 302:
	case 310:
        {
			Script::BattleFormula::SkillData& sd = _formula->skillData(2, sid - 301, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				return 0;
            UInt8 myPos = bf->getPos();
            bf = getRandomFighter(bf->getSide(), &myPos, 1);
			if(bf)
			{
				skillId = sid;
				return sd.value1;
			}
			return 0.0f;
        }
	}
#endif
	return 0.0f;
}
#endif

UInt32 BattleSimulator::attackOnce(BattleFighter * bf, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * area_target_obj, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, int counter_deny, AttackPoint * counter_deny_list, std::vector<AttackAct>* atkAct)
{
	if(area_target_obj == NULL || area_target_obj->getHP() == 0 || bf == NULL)
		return 0;
	// if a fighter was attacked
	UInt32 dmg = 0;
    UInt32 magdmg = 0;
	if(area_target_obj->isChar())
	{
        bool counter100 = false;
        bool poison = false;
		BattleFighter * area_target = static_cast<BattleFighter *>(area_target_obj);
		UInt8 side = area_target->getSide();
		UInt8 pos = area_target->getPos();
        UInt8 target_stun = area_target->getStunRound();
        bool enterEvade = area_target->getEvad100();
        if(enterEvade)
            area_target->setEvad100(false);

        float aura_factor = 1;
        if(skill && skill->cond == GData::SKILL_PEERLESS)
        {
            aura_factor = 1 + static_cast<float>(bf->getAura()-100) * 0.0025;
        }

		if(!enterEvade && (target_stun > 0 || bf->calcHit(area_target)))
		{
            pr = bf->calcPierce(area_target);
            float atk = 0;
            float magatk = 0;
            if(NULL != skill && skill->effect != NULL)
            {
                bf->calcSkillAttack(cs, area_target, atk, magatk);
                //atk = bf->calcAttack(cs, area_target);
                //magatk = bf->calcMagAttack(cs, area_target);
                float crratk = 0;
                float crrmagatk = 0;
                if(bf->getClass() == 3)
                    crratk = aura_factor * (atk * skill->effect->crrdamP + skill->effect->addcrr * (cs ? bf->getCriticalDmg() : 1));
                else
                    crrmagatk = aura_factor * (magatk * skill->effect->crrdamP + skill->effect->addcrr * (cs ? bf->getCriticalDmg() : 1));

                atk = aura_factor * (atk * skill->effect->damageP + skill->effect->adddam * (cs ? bf->getCriticalDmg() : 1)) + crratk;
                magatk = aura_factor * (magatk * skill->effect->magdamP + skill->effect->addmag * (cs ? bf->getCriticalDmg() : 1)) + crrmagatk;
            }
            else
            {
                atk = bf->calcAttack(cs, area_target);
            }

#if 0
			float rescueRate = 0.0f;
			bool rescue = counter_deny >= 0 && (rescueRate = testRescue(area_target, counter_deny, counter_deny_list)) > 0.0f;

			// if rescure, take 1/10 of the damage
			if(rescue)
			{
				pos = area_target->getPos();
				atk *= rescueRate;
			}
#endif
			if(area_target->hasFlag(BattleFighter::IsMirror))
			{
				dmg = area_target->getHP();
			}
			else
			{
				float def;
                float magdef;
                if(magatk)
                {
                    magdef = area_target->getMagDefend();
                    magdmg = (pr ? static_cast<UInt32>(factor * magatk) : _formula->calcDamage(factor * magatk, magdef, bf->getLevel())) * (950 + _rnd(100)) / 1000;
                    magdmg = magdmg > 0 ? magdmg : 1;
                }

                if(atk)
                {
                    def = area_target->getDefend();
                    dmg = (pr ? static_cast<UInt32>(factor * atk) : _formula->calcDamage(factor * atk, def, bf->getLevel())) * (950 + _rnd(100)) / 1000;
                    dmg = dmg > 0 ? dmg : 1;
                }

			}

            bool defend100 = area_target->getDefend100();
            if(defend100)
            {
                dmg = dmg > 0 ? 1 : 0;
                magdmg = magdmg > 0 ? 1 : 0;
                defList[defCount].damType = e_damOut;
                area_target->setDefend100(false);
            }
            else
            {
                defList[defCount].damType = e_damNormal;
            }
            area_target->makeDamage(dmg + magdmg);
            defList[defCount].damage = dmg + magdmg;

			defList[defCount].leftHP = area_target->getHP();
//			printf("%u:%u %s %u:%u, made %u damage, hp left: %u\n", 1-side, from_pos, cs ? "CRITICALs" : "hits", side, pos, dmg, area_target->getHP());
			// killed the target fighter

            if(counter_deny >= 0 && (!skill || skill->cond == GData::SKILL_ACTIVE))
            {
                setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stAura, 25, 0, scList, scCount, false);
                setStatusChange( area_target->getSide(), area_target->getPos(), 1, 0, e_stAura, 25, 0, scList, scCount, true);
            }

			if(area_target->getHP() == 0)
            {
				onDead(area_target, atkAct);
            }
			else if(_winner == 0)
            {
				onDamage(area_target, scList, scCount, true, atkAct);

                if(NULL != skill && skill->effect != NULL && skill->effect->state == 1 && !defend100)
                {
                    float rate = skill->prob * 100;
                    if((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000))
                    {
                        // poison
                        poison = true;
                    }
                }
            }
		}
		else
		{
            if(atkAct)
            {
                const GData::SkillBase* target_skill = NULL;
                size_t idx = 0;
                while(NULL != (target_skill = area_target->getPassiveSkillAftEvd100(idx)))
                {
                    AttackAct aa = {0};
                    aa.bf = area_target;
                    aa.skill = target_skill;
                    aa.target_side = bf->getSide();
                    aa.target_pos = bf->getPos();
                    atkAct->push_back(aa);
                    counter100 = true;
                } 
                target_skill = area_target->getPassiveSkillAftEvd();
                if(NULL != target_skill)
                {
                    AttackAct aa = {0};
                    aa.bf = area_target;
                    aa.skill = target_skill;
                    aa.target_side = bf->getSide();
                    aa.target_pos = bf->getPos();
                    atkAct->push_back(aa);
                    counter100 = true;
                }
            }

			defList[defCount].damType = e_damEvade;
			defList[defCount].damage = 0;
			defList[defCount].leftHP = area_target->getHP();
//			printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
		}
        defList[defCount].pos = pos + (bf->getSide() == side ? 25 : 0);
        ++ defCount;

        UInt32 rhp = 0;
        if(NULL != skill && skill->effect != NULL && (skill->effect->absorb || skill->effect->absorbP))
        {
            rhp = (dmg + magdmg) * skill->effect->absorbP + skill->effect->absorb;
        }
 
        if(rhp > 0)
        {
            bf->regenHP(rhp);
            defList[defCount].damType = e_damAbsorb;
            defList[defCount].rhp= rhp;
            defList[defCount].rLeftHP = bf->getHP();
            defList[defCount].pos = bf->getPos() + 25;
            ++ defCount;
        }

        // 中毒
        if(poison)
        {
            doPoisonAttack(bf, cs, skill, area_target, factor, defList, defCount, scList, scCount, atkAct);
        }

        doPassiveSkillBeAtk(bf, area_target, atkAct, dmg + magdmg);

        // target fighter will do not counter while fighter is the same side
        bool can_counter = true;
        if(target_stun > 0 || bf->getSide() == area_target->getSide())
        {
            can_counter = false;
        }

		// if this fighter can counter
		if(can_counter && counter_deny >= 0 && _winner == 0 && !_isBody[side][pos] && area_target_obj->getHP() > 0 && bf->getHP() > 0)
		{
			BattleFighter * target_fighter = static_cast<BattleFighter *>(area_target_obj);
			// test counter by rolling dice
			if(counter100 || target_fighter->calcCounter(bf, !bf->canBeCounter()))
			{
				if(target_fighter->calcHit(bf))
				{
                    defList[0].damType2 |= 0x80;
					bool cs = false;
					float atk = target_fighter->getAttack();
					float def = bf->getDefend();
					bool pr = target_fighter->calcPierce(bf);
					UInt32 dmg2 = (pr ? static_cast<UInt32>(atk) : _formula->calcDamage(atk, def, target_fighter->getLevel())) * (950 + _rnd(100)) / 1000;

					bf->makeDamage(dmg2);

					if(cs)
						defList[0].damType2 |= 0x40;
					if(pr)
						defList[0].damType2 |= 0x20;
					defList[0].counterDmg = dmg2;
					defList[0].counterLeft = bf->getHP();

//					if(cs)
//						printf("  [Counter] %u:%u CRITICAL-STRIKEs %u:%u, made %u damage, hp left: %u\n", side, pos, 1-side, from_pos, dmg2, bf->getHP());
//					else
//						printf("  [Counter] %u:%u attacks %u:%u, made %u damage, hp left: %u\n", side, pos, 1-side, from_pos, dmg2, bf->getHP());

					// killed the fighter
					if(bf->getHP() == 0)
						onDead(bf, atkAct);
					else if(_winner == 0)
                    {
						onDamage(bf, scList, scCount, false);
                    }
				}
#if 0
				else
				{
//					printf("  [Counter] %u:%u attacks %u:%u, but missed!\n", side, pos, 1-side, from_pos);
					defList[0].damType |= 0x10;
					defList[0].counterDmg = 0;
					defList[0].counterLeft = bf->getHP();
				}
#endif
			}
		}
	}
	else // if attacked a barrier
	{
        float atk = bf->calcAttack(cs, 0);
		dmg = static_cast<int>(factor * atk) * (950 + _rnd(100)) / 1000;
		area_target_obj->makeDamage(dmg);
		defList[defCount].pos = area_target_obj->getPos() + (bf->getSide() == area_target_obj->getSide() ? 25 : 0);
		defList[defCount].damType = e_damNormal;
		defList[defCount].damage = dmg;
		defList[defCount].leftHP = area_target_obj->getHP();
//		printf("%u:%u %s ground object, made %u damage, hp left: %u\n", 1-side, from_pos, cs ? "CRITICALs" : "hits", dmg, area_target_obj->getHP());
		++ defCount;
	}

	return dmg + magdmg;
}

void BattleSimulator::doPassiveSkillBeAtk(BattleFighter* bf, BattleFighter* bo, std::vector<AttackAct>* atkAct, UInt32 dmg)
{
    if(bf == NULL || bo == NULL || atkAct == NULL)
        return;

    if(bo->getStunRound() || bo->getConfuseRound() || bo->getForgetRound())
    {
        return;
    }

    if(bo->getHP() > 0)
    {
        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        const GData::SkillBase* passiveSkillThorn = NULL;
        const GData::SkillBase* passiveSkillInj = NULL;
        while(NULL != (passiveSkill = bo->getPassiveSkillBeAtk100(idx)))
        {
            if(passiveSkill->effect == NULL)
                continue;
            if(passiveSkillThorn == NULL && (passiveSkill->effect->thorn || passiveSkill->effect->thornP))
            {
                passiveSkillThorn = passiveSkill;
            }
            else if(passiveSkillInj == NULL &&(passiveSkill->effect->inj2hpP || passiveSkill->effect->inj2hp))
            {
                passiveSkillInj = passiveSkill;
            }
            else
            {
                AttackAct aa = {0};
                aa.bf = bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();

                atkAct->push_back(aa);
            }
        }

        passiveSkill = bo->getPassiveSkillBeAtk();
        if(NULL != passiveSkill && passiveSkill->effect != NULL)
        {
            if( passiveSkillThorn == NULL && (passiveSkill->effect->thorn || passiveSkill->effect->thornP))
            {
                passiveSkillThorn = passiveSkill;
            }
            else if(passiveSkillInj == NULL && (passiveSkill->effect->inj2hpP || passiveSkill->effect->inj2hp))
            {
                passiveSkillInj = passiveSkill;
            }
            else
            {
                AttackAct aa = {0};
                aa.bf = bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();

                atkAct->push_back(aa);
            }
        }

        if(passiveSkillInj && dmg)
        {
            AttackAct aa = {0};
            aa.bf = bo;
            aa.skill = passiveSkillInj;
            aa.target_side = bo->getSide();
            aa.target_pos = bo->getPos();
            aa.param = dmg;

            atkAct->push_back(aa);
        }

        if(passiveSkillThorn && bf->getSide() != bo->getSide() && dmg)
        {
            AttackAct aa = {0};
            aa.bf = bo;
            aa.skill = passiveSkillThorn;
            aa.target_side = bf->getSide();
            aa.target_pos = bf->getPos();
            aa.param = dmg;

            atkAct->push_back(aa);
        }
    }
}

UInt32 BattleSimulator::doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, std::vector<AttackAct>* atkAct)
{
    if(bf->getStunRound() || bf->getConfuseRound() || bf->getForgetRound())
    {
        return 0;
    }

    UInt32 dmg = 0;
    UInt32 dmg2 = abs(bf->calcPoison(skill, area_target, cs)) * factor;
    // 第一波毒
    doSkillState(bf, skill, area_target, defList, defCount, atkAct);
    defList[defCount].pos = area_target->getPos();
    if(defList[defCount].damType == e_Poison)
    {
        area_target->makeDamage(dmg2*0.5);
        defList[defCount].damage = dmg2*0.5;
        defList[defCount].leftHP = area_target->getHP();
        ++defCount;
        dmg += dmg2;

        if(area_target->getHP() == 0)
        {
            onDead(area_target, atkAct);
            return dmg;
        }
        else if(_winner == 0)
        {
            onDamage(area_target, scList, scCount, true);
        }

        // 第二波毒
        doSkillState(bf, skill, area_target, defList, defCount, atkAct);
        defList[defCount].pos = area_target->getPos();
        if(defList[defCount].damType == e_Poison)
        {
            area_target->makeDamage(dmg2);
            defList[defCount].damage = dmg2;
            defList[defCount].leftHP = area_target->getHP();
            ++defCount;
            dmg += dmg2;

            if(area_target->getHP() == 0)
            {
                onDead(area_target, atkAct);
                return dmg;
            }
            else if(_winner == 0)
            {
                onDamage(area_target, scList, scCount, true);
            }

            // 第三波毒
            doSkillState(bf, skill, area_target, defList, defCount, atkAct);
            defList[defCount].pos = area_target->getPos();
            if(defList[defCount].damType == e_Poison)
            {
                defList[defCount].damType = e_UnPoison;
                area_target->makeDamage(dmg2 * 1.5);
                defList[defCount].damage = dmg2 * 1.5;
                defList[defCount].leftHP = area_target->getHP();
                ++defCount;
                dmg += dmg2;

                if(area_target->getHP() == 0)
                    onDead(area_target, atkAct);
                else if(_winner == 0)
                {
                    onDamage(area_target, scList, scCount, true);
                }
            } // 第三波毒
            else
            {
                defList[defCount].leftHP = area_target->getHP();
                ++defCount;
            }
        } // 第二波毒
        else
        {
            defList[defCount].leftHP = area_target->getHP();
            ++defCount;
        }
    } // 第一波毒
    else
    {
        defList[defCount].leftHP = area_target->getHP();
        ++defCount;
    }

    return dmg;
}

void BattleSimulator::doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, DefStatus* defList, size_t& defCount, std::vector<AttackAct>* atkAct)
{
    if(NULL == skill || bf == NULL)
    {
        return;
    }

    if(skill->effect == NULL)
        return;
    if(skill->effect->state == 0)
        return;

    if(bf->getStunRound() || bf->getConfuseRound() || bf->getForgetRound())
    {
        return;
    }
    UInt8 state[3] = {0};
    UInt8 cnt = 0;
    UInt8 state_idx = 0;
    UInt8 effect_state = skill->effect->state;
    if(SKILL_ID(skill->getId()) == 136) // 光棍
    {
        switch(bf->getClass())
        {
        case 1:
            effect_state = 0x02;
            break;
        case 2:
            return;
        case 3:
            effect_state = 0x04;
            break;
        }
    }

    state[0] = effect_state;
    if(effect_state & 0xe)
    {
        if(effect_state & 0x2)
        {
            state[cnt] = 0x2;
            ++cnt;
        }
        if(effect_state & 0x4)
        {
            state[cnt] = 0x4;
            ++cnt;
        }
        if(effect_state & 0x8)
        {
            state[cnt] = 0x8;
            ++cnt;
        }
        if(cnt > 1)
            state_idx = _rnd(cnt);
    }

    BattleFighter* target_bo = static_cast<BattleFighter*>(bo);
    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    UInt8 immune = target_bo->getImmune();
    if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= target_bo->getImmuneLevel())
    {
        switch(state[state_idx])
        {
        case 1:
        case 2:
        case 4:
        case 8:
            defList[defCount].damType = e_Immune;
            return;
        }
    }

    if(atkAct)
    {
        float rate = target_bo->getMagRes(bf) * 100;
        if(rate > _rnd(10000))
        {
            defList[defCount].damType = e_Res;
            idx = 0;
            while(NULL != (passiveSkill = target_bo->getPassiveSkillAftRes100(idx)));

            if( passiveSkill == NULL)
                passiveSkill = target_bo->getPassiveSkillAftRes();

            if(NULL != passiveSkill)
            {
                AttackAct aa = {0};
                aa.bf = target_bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();
                aa.param = skill->getId();

                atkAct->push_back(aa);
                defList[defCount].damType = e_ResR;
            }

            return;
        }
    }

    switch(state[state_idx])
    {
    case 1:
        if(target_bo->getPoisonRound() < 1)
        {
            defList[defCount].damType = e_Poison;
            //target_bo->setPoisonLevel(SKILL_LEVEL(skill->getId()));
            //target_bo->setPoisonRound(skill->last);
        }
        break;
    case 2:
        if(target_bo->getConfuseRound() < 1)
        {
            defList[defCount].damType = e_Confuse;
            target_bo->setConfuseLevel(SKILL_LEVEL(skill->getId()));
            target_bo->setConfuseRound(skill->last);
        }
        break;
    case 4:
        if(target_bo->getStunRound() < 1)
        {
            defList[defCount].damType = e_Stun;
            target_bo->setStunLevel(SKILL_LEVEL(skill->getId()));
            target_bo->setStunRound(skill->last);
        }
        break;
    case 8:
        if(target_bo->getForgetRound() < 1)
        {
            defList[defCount].damType = e_Forget;
            target_bo->setForgetLevel(SKILL_LEVEL(skill->getId()));
            target_bo->setForgetRound(skill->last);
        }
        break;
    }

}

UInt32 BattleSimulator::doNormalAttack(BattleFighter* bf, int otherside, int target_pos, std::vector<AttackAct>* atkAct)
{
    if(bf == NULL || bf->getHP() == 0)
        return 0;

    BattleObject* target_object = _objs[otherside][target_pos];
    if(target_object!= NULL)
	{
		// find all targets that are hit
		DefStatus defList[25];
		size_t defCount = 0;
		StatusChange scList[50];
		size_t scCount = 0;

        memset(defList, 0, sizeof(defList));
		// calculate damage
		bool cs = false;
		bool pr = false;

		UInt32 dmg = 0;
        // attack only one target
        dmg += attackOnce(bf, cs, pr, NULL, target_object, 1, defList, defCount, scList, scCount, 0, NULL, atkAct);

        int self_side = bf->getSide() == otherside ? 25 : 0;
        appendToPacket(bf->getSide(), bf->getPos(), target_pos + self_side, static_cast<UInt8>(0), static_cast<UInt16>(0), cs, pr, defList, defCount, scList, scCount);
        return dmg;
    }

    return 0;
}

void BattleSimulator::getSkillTarget(BattleFighter* bf, const GData::SkillBase* skill, int& target_side, int& target_pos, int& cnt)
{
    if(skill == NULL)
        return;
    if(skill->effect == NULL)
        return;

    target_side = skill->target != 1 ? bf->getSide() : 1 - bf->getSide();

    if(1 == skill->area)
    {
        target_pos = 0;
        cnt = 25;
    }
    else if( 0 == skill->target )
    {
        UInt8 excepts[25] = {0};
        size_t exceptCnt = 1;
        excepts[0] = bf->getPos();

        if(skill->effect->aura > 0 || skill->effect->auraP > 0.001)
        {
            UInt8 side = bf->getSide();
            for(int i = 0; i < 25; ++ i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[side][i]);
                if(!bo)
                    continue;

                if(bo->getAura() > 99)
                {
                    excepts[exceptCnt] = i;
                    ++ exceptCnt;
                }
            }
        }

        BattleFighter* bo = getRandomFighter(bf->getSide(), excepts, exceptCnt);
        if(NULL == bo)
        {
            target_pos = bf->getPos();
            cnt = 1;
        }
        else
        {
            target_pos = bo->getPos();
            cnt = 1;
        }
    }
    else if( 1 == skill->target )
    {
        target_pos = getPossibleTarget(bf->getSide(), bf->getPos());
        cnt = 1;
    }
    else
    {
        target_pos = bf->getPos();
        cnt = 1;
    }
}

UInt32 BattleSimulator::doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct, UInt32 skillParam)
{
    if(NULL == skill || target_pos < 0 || bf == NULL)
        return 0;
    if(skill->effect == NULL)
        return 0;

    if(bf->getStunRound() || bf->getConfuseRound() || bf->getForgetRound() || bf->getHP() == 0)
    {
        return 0;
    }

    // 法术抵抗后的技能
    if(skill->cond == GData::SKILL_AFTRES && bf->getSide() != target_side)
    {
        size_t defCount = 0;
        DefStatus defList[25];

        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
        if(NULL == bo)
            return 0;

        const GData::SkillBase* boSkill = GData::skillManager[skillParam];
        if(boSkill == NULL || boSkill->effect == NULL)
            return 0;

        UInt8 state[3] = {0};
        UInt8 cnt = 0;
        UInt8 idx = 0;
        state[0] = boSkill->effect->state;
        if(boSkill->effect->state & 0xe)
        {
            if(boSkill->effect->state & 0x2)
            {
                state[cnt] = 0x2;
                ++cnt;
            }
            if(boSkill->effect->state & 0x4)
            {
                state[cnt] = 0x4;
                ++cnt;
            }
            if(boSkill->effect->state & 0x8)
            {
                state[cnt] = 0x8;
                ++cnt;
            }
            idx = _rnd(cnt);
        }

        switch(state[idx])
        {
        case 1:
            {
                UInt32 dmg = abs(bo->calcPoison(boSkill, bo, false));
                bo->makeDamage(dmg*0.5);
                defList[defCount].damage = dmg*0.5;
                defList[defCount].leftHP = bo->getHP();
                defList[defCount].pos = target_pos;
                ++defCount;
                if(bo->getHP() == 0)
                    break;

                bo->makeDamage(dmg);
                defList[defCount].damage = dmg;
                defList[defCount].leftHP = bo->getHP();
                defList[defCount].pos = target_pos;
                ++defCount;
                if(bo->getHP() == 0)
                    break;

                bo->makeDamage(dmg*1.5);
                defList[defCount].damage = dmg*1.5;
            }
            break;
        case 2:
            if(bo->getConfuseRound() < 1)
            {
                defList[defCount].damage = 0;
                defList[defCount].damType = e_Confuse;
                bo->setConfuseLevel(SKILL_LEVEL(boSkill->getId()));
                bo->setConfuseRound(boSkill->last);
            }
            break;
        case 4:
            if(bo->getStunRound() < 1)
            {
                defList[defCount].damage = 0;
                defList[defCount].damType = e_Stun;
                bo->setStunLevel(SKILL_LEVEL(boSkill->getId()));
                bo->setStunRound(boSkill->last);
            }
            break;
        case 8:
            if(bo->getForgetRound() < 1)
            {
                defList[defCount].damage = 0;
                defList[defCount].damType = e_Forget;
                bo->setForgetLevel(SKILL_LEVEL(boSkill->getId()));
                bo->setForgetRound(boSkill->last);
            }
            break;
        }

        defList[defCount].pos = target_pos;
        defList[defCount].leftHP = bo->getHP();
        ++defCount;

        appendToPacket( bf->getSide(), bf->getPos(), target_pos, 2, skill->getId(), false, false, defList, defCount, NULL, 0);
        return 0;
    }

    // 被攻击后的技能
    if(skill->cond == GData::SKILL_BEATKED)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);

        if(NULL == bo)
            return 0;

        if(bf->getHP() > 0 && bo->getHP() > 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;

            memset(defList, 0, sizeof(defList));
            UInt32 fdmg = 0;
            UInt32 inj2hp = 0;
            if(fdmg == 0 && (skill->effect->thorn || skill->effect->thornP))
            {
                fdmg = skillParam * skill->effect->thornP + skill->effect->thorn;
            }
            else if(inj2hp == 0 &&(skill->effect->inj2hpP || skill->effect->inj2hp))
            {
                inj2hp = skillParam * skill->effect->inj2hpP + skill->effect->inj2hp;
            }

            if(inj2hp > 0)
            {
                bf->regenHP(inj2hp);
                defList[defCount].damType = e_damInj;
                defList[defCount].damage = inj2hp;
                defList[defCount].pos = bf->getPos() + 25;
                defList[defCount].leftHP = bf->getHP();
                ++ defCount;

                appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false, defList, defCount, NULL, 0);

                return 0;
            }
            else if(fdmg > 0 && bf->getSide() != target_side)
            {
                UInt8 immune = bo->getImmune();
                if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel())
                {
                   defList[defCount].damType = e_Immune;
                }
                else
                {
                    defList[defCount].damType = e_damBack;
                    defList[defCount].damage = fdmg;
                    bo->makeDamage(fdmg);
                }

                defList[defCount].pos = bo->getPos();
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;

                appendToPacket(bf->getSide(), bf->getPos(), target_pos, 2, skill->getId(), false, false, defList, defCount, NULL, 0);

                return 0;
            }

        }
    }

    if(skill->cond == GData::SKILL_AFTEVD
        || skill->cond == GData::SKILL_DEAD
      )
    {
        int self_side = bf->getSide() == target_side ? 25 : 0;
        appendToPacket( bf->getSide(), bf->getPos(), target_pos + self_side, 2, skill->getId(), false, false, NULL, 0, NULL, 0);
        return 0;
    }

    if(skill->cond == GData::SKILL_ENTER)
    {
        DefStatus defList[25];
        size_t defCount = 0;

        StatusChange scList[250];
        size_t scCount = 0;

        memset(defList, 0, sizeof(defList));
        if(cnt == 1)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(NULL == bo)
                return 0;

            if(skill->effect->immune)
            {
                bo->setImmune(skill->effect->immune);
                bo->setImmuneLevel(SKILL_LEVEL(skill->getId()));
            }
            else if(skill->effect->evade == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setEvad100(true);
                bf->setEvad100CD(skill->cd + 1);
                bf->setEvad100BaseCD(skill->cd + 1);
            }
            else if(skill->effect->def == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setDefend100(true);
                bf->setDefend100CD(skill->cd + 1);
                bf->setDefend100BaseCD(skill->cd + 1);
            }
            else
            {
                doSkillStatus2(bf, skill, target_side, target_pos, cnt, scList, scCount);
            }

            int self_side = bf->getSide() == bo->getSide() ? 25 : 0;
            defList[defCount].pos = target_pos + self_side;
            defList[defCount].damType = e_damNormal;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;

            appendToPacket(bf->getSide(), bf->getPos(), bo->getPos() + self_side, 2, skill->getId(), false, false, defList, defCount, scList, scCount);
            return 0;
        }

        for(int pos = 0; pos < cnt; pos++)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
            if(NULL == bo)
                continue;

            if(skill->effect->immune)
            {
                bo->setImmuneLevel(SKILL_LEVEL(skill->getId()));
            }
            else if(skill->effect->evade == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setEvad100(true);
                bf->setEvad100CD(skill->cd + 1);
                bf->setEvad100BaseCD(skill->cd + 1);
            }
            else if(skill->effect->def == GData::SKILL_EFFECT_FALG_VALUE)
            {
                bo->setDefend100(true);
                bf->setDefend100CD(skill->cd + 1);
                bf->setDefend100BaseCD(skill->cd + 1);
            }
            else
            {
                doSkillStatus2(bf, skill, target_side, pos, 1, scList, scCount);
            }

            int self_side = bf->getSide() == bo->getSide() ? 25 : 0;
            defList[defCount].pos = pos + self_side;
            defList[defCount].damType = e_damNormal;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
        }

        int self_side = bf->getSide() == target_side ? 25 : 0;
        appendToPacket(bf->getSide(), bf->getPos(), target_pos + self_side, 2, skill->getId(), false, false, defList, defCount, scList, scCount);
        return 0;
    }

    UInt32 dmg = 0;

    GData::Area* area = NULL;
    area = &(GData::areaList[skill->area]);

    AttackPoint ap[25];
    int apcnt = 0;

    if(skill->area != 0 && skill->area != 1 && area->getCount() > 0)
    {
        int x_ = target_pos % 5;
        int y_ = target_pos / 5;
        int cnt = area->getCount();
        for(int i = 1; i < cnt; ++ i)
        {
            GData::Area::Data& ad = (*area)[i];
            int x = x_ + ad.x;
            int y = y_ + ad.y;
            if(x < 0 || x > 4 || y < 0 || y > 4)
            {
                continue;
            }
            ap[apcnt].pos = x + y * 5;
            ap[apcnt ++].factor = skill->factor[std::max(abs(ad.y), abs(ad.x))]; 
        }
    }

    size_t defCount = 0;
    DefStatus defList[250];
    size_t scCount = 0;
    StatusChange scList[250];
    bool cs = false;
    bool pr = false;

    memset(defList, 0, sizeof(defList));
    // 免疫降灵气
    bool dostatus = true;
    if(skill->effect->state == 64)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
        if(NULL == bo)
            return 0;

        UInt8 immune = bo->getImmune();
        if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel())
        {
            dostatus = false;
            defList[defCount].damType = e_Immune;
            defList[defCount].pos = target_pos;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
        }
    }
    if(dostatus)
    {
        if(1 == skill->area)
        {
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                doSkillStatus(bf, skill, target_side, pos, 1, scList, scCount);
            }
        }
        else if(0 == skill->area)
        {
            doSkillStatus(bf, skill, target_side, target_pos, 1, scList, scCount);
        }
        else
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                doSkillStatus(bf, skill, target_side, target_pos, 1, scList, scCount);
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                doSkillStatus(bf, skill, target_side, ap[i].pos, 1, scList, scCount);
            }

        }
    }

    // therapy skill
    if(skill->effect->hp > 0 || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
    {
        UInt32 rhp = bf->calcTherapy(skill);

        if(bf->getSide() != target_side)
        {
            UInt32 hpr = bf->regenHP(rhp);
            if(hpr != 0)
            {
                defList[defCount].pos = bf->getPos() + 25;
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bf->getHP();
                ++ defCount;
            }
        }
        else if(1 == skill->area)
        {
            int i = 0;
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                if(bo->getLostHP() == 0)
                    continue;

                UInt32 hpr = bo->regenHP(rhp) * skill->factor[i];
                ++i;
                if(hpr == 0)
                    continue;

                defList[defCount].pos = pos + 25;
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
            }
        }
        else if(0 == skill->area)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                return dmg;

            UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp);
            if(hpr != 0)
            {
                defList[defCount].pos = bo->getPos() + 25;
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
            }
        }
        else
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp) * skill->factor[0];
                if(hpr != 0)
                {
                    defList[defCount].pos = bo->getPos() + 25;
                    defList[defCount].damType = e_damHpAdd;
                    defList[defCount].damage = hpr;
                    defList[defCount].leftHP = bo->getHP();
                    ++ defCount;
                }
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                if(NULL == bo)
                    continue;

                UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp) * ap[i].factor;
                if(hpr != 0)
                {
                    defList[defCount].pos = bo->getPos() + 25;
                    defList[defCount].damType = e_damHpAdd;
                    defList[defCount].damage = hpr;
                    defList[defCount].leftHP = bo->getHP();
                    ++ defCount;
                }
            }
        }
    }
    else if( (skill->effect->state != 1) && (skill->effect->hp < 0 || skill->effect->addhp < 0 || skill->effect->hpP < -0.001) )
    {
        if(bf->getSide() != target_side)
        {
            if(1 == skill->area)
            {
                int i = 0;
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false)) * skill->factor[i];
                    ++i;
                    bo->makeDamage(dmg);
                    defList[defCount].pos = pos;
                    defList[defCount].damType = e_damNormal;
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;

                    if(bo->getHP() == 0)
                    {
                        onDead(bo, atkAct);
                    }
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                    bo->makeDamage(dmg);
                    defList[defCount].pos = target_pos;
                    defList[defCount].damType = e_damNormal;
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;

                    if(bo->getHP() == 0)
                    {
                        onDead(bo, atkAct);
                    }
                }
            }
            else
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false)) * skill->factor[0];
                    bo->makeDamage(dmg);
                    defList[defCount].pos = target_pos;
                    defList[defCount].damType = e_damNormal;
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;

                    if(bo->getHP() == 0)
                    {
                        onDead(bo, atkAct);
                    }
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false)) * ap[i].factor;
                    bo->makeDamage(dmg);
                    defList[defCount].pos = ap[i].pos;
                    defList[defCount].damType = e_damNormal;
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;

                    if(bo->getHP() == 0)
                    {
                        onDead(bo, atkAct);
                    }
                }
            }
        }
    }

    // 混乱晕眩封印状态
    if(skill->effect->state & 0x0e)
    {
        float rate = skill->prob * 100;
	    if(bf->getSide() != target_side)
        {
            if(1 == skill->area)
            {
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                    {
                        defList[defCount].damage = 0;
                        defList[defCount].pos = pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, atkAct);
                        defCount ++;
                    }
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                    {
                        defList[defCount].damage = 0;
                        defList[defCount].pos = target_pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, atkAct);
                        defCount ++;
                    }
                }
            }
            else
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                    {
                        defList[defCount].damage = 0;
                        defList[defCount].pos = target_pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, atkAct);
                        defCount ++;
                    }
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                    {
                        defList[defCount].damage = 0;
                        defList[defCount].pos = ap[i].pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, atkAct);
                        defCount ++;
                    }
                }
            }
        }
    }

    // 驱散
    if(skill->effect->disperse > 0)
    {
        float rate = skill->prob * 100;
	    if(bf->getSide() == target_side && ((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                defList[defCount].damType = e_Disperse;
                if(skill->effect->disperse & 2)
                {
                    bf->setConfuseLevel(0);
                    bf->setConfuseRound(0);
                }

                if(skill->effect->disperse & 4)
                {
                    bf->setStunLevel(0);
                    bf->setStunRound(0);
                }

                if(skill->effect->disperse & 8)
                {
                    bf->setForgetLevel(0);
                    bf->setForgetRound(0);
                }

                defList[defCount].damage = 0;
                defList[defCount].pos = target_pos + 25;
                defList[defCount].leftHP = bf->getHP();
                defCount ++;
            }
        }
    }

    if(skill->effect->state == 1 || skill->effect->damage || skill->effect->damageP || skill->effect->adddam
            || skill->effect->magdam || skill->effect->magdamP || skill->effect->addmag
            || skill->effect->crrdam || skill->effect->crrdamP || skill->effect->addcrr)
    {
        if(0 == skill->area)
        {
            dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][target_pos], 1, defList, defCount, scList, scCount, 0, NULL, atkAct);
        }
        else if(1 == skill->area)
        {
            int i = 0;
            for(UInt8 pos = 0; pos < 25; pos++)
            {
                if(_objs[target_side][pos] == NULL || _objs[target_side][pos]->getHP() == 0)
                    continue;
                dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][pos], skill->factor[i], defList, defCount, scList, scCount);
                ++i;
            }
        }
        else
        {
            // attack the target on center
            dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][target_pos], skill->factor[0], defList, defCount, scList, scCount, apcnt, ap, atkAct);
            for(int i = 0; i < apcnt; ++ i)
            {
                dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][ap[i].pos], ap[i].factor, defList, defCount, scList, scCount);
            }
        }
    }

    if (skill && skill->cond == GData::SKILL_PEERLESS)
    {
        setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stAura, -1 * bf->getAura(), 0, scList, scCount, false);
    }

    int self_side = bf->getSide() == target_side ? 25 : 0;
    appendToPacket( bf->getSide(), bf->getPos(), target_pos + self_side, 2, skill->getId(), cs, pr, defList, defCount, scList, scCount);
    return dmg;
}

void BattleSimulator::doSkillStatus2(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt ,StatusChange* scList, size_t& scCount)
{
    if(NULL == skill || bf == NULL)
        return;

    if(skill->effect == NULL)
        return;

    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
    if(NULL == bo)
        return;

    if(skill->effect->auraP || skill->effect->aura)
    {
        float rate = skill->prob * 100;
        if(rate > _rnd(10000))
        {
            float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
            if(value > 0 && bf->getSide() != target_side)
            {
                float value = bf->_aura * skill->effect->auraP + skill->effect->aura;
                setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAura, value, skill->last, scList, scCount, false);
            }
            else
            {
                setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAura, value, skill->last, scList, scCount, bf->getSide() != target_side);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAtk, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtk, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_defend * skill->effect->defP + skill->effect->def;
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stDef, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stDef, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stTough, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stTough, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->action || skill->effect->actionP)
    {
        float value = bo->_maxAction * skill->effect->actionP + skill->effect->action;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_maxAction * skill->effect->actionP + skill->effect->action;
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAction, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAction, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stEvade, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stEvade, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCritical, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCritical, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stPierce, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stPierce, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCounter, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCounter, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->hitrate)
    {
        float value = skill->effect->hitrate;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stHitRate, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange2( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stHitRate, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }
}


void BattleSimulator::doSkillStatus(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt ,StatusChange* scList, size_t& scCount)
{
    if(NULL == skill || bf == NULL)
        return;
    if(skill->effect == NULL)
        return;

    if(bf->getStunRound() || bf->getConfuseRound() || bf->getForgetRound())
        return;

    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
    if(NULL == bo)
        return;

    if(skill->effect->auraP || skill->effect->aura)
    {
        if(SKILL_ID(skill->getId()) == 136 && bf->getClass() != 2) // 光棍
            return;

        float rate = skill->prob * 100;
        if(rate > _rnd(10000))
        {
            float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
            if(value > 0 && bf->getSide() != target_side)
            {
                float value = bf->_aura * skill->effect->auraP + skill->effect->aura;
                setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAura, value, skill->last, scList, scCount, false);
            }
            else
            {
                setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAura, value, skill->last, scList, scCount, bf->getSide() != target_side);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAtk, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtk, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_defend * skill->effect->defP + skill->effect->def;
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stDef, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stDef, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stTough, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stTough, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->action || skill->effect->actionP)
    {
        float value = bo->_maxAction * skill->effect->actionP + skill->effect->action;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_maxAction * skill->effect->actionP + skill->effect->action;
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAction, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAction, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stEvade, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stEvade, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCritical, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCritical, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stPierce, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stPierce, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCounter, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCounter, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }

    if(skill->effect->hitrate)
    {
        float value = skill->effect->hitrate;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange( bf->getSide(), bf->getPos(), 1, skill->getId(), e_stHitRate, value, skill->last, scList, scCount, false);
        }
        else
        {
            setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stHitRate, value, skill->last, scList, scCount, bf->getSide() != target_side);
        }
    }
}

BattleFighter* BattleSimulator::getTherapyTarget(BattleFighter* bf)
{
    UInt8 side = bf->getSide();
	for(UInt8 i = 0; i < 25; ++ i)
	{
		BattleFighter* bo = static_cast<BattleFighter*>(_objs[side][i]);
		if(bo == NULL || bo->getHP() == 0)
			continue;
        if(bo->getHP() < (bo->getMaxHP() >> 1))
        {
            return bo;
        }
	}

    return NULL;
}

UInt32 BattleSimulator::FightersEnter()
{
    UInt32 rcnt = 0;

    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
    size_t cnt = cur_fgtlist.size();

    for(size_t idx = 0; idx < cnt; idx++)
    {
        BattleFighter* bf = cur_fgtlist[idx];

        rcnt += doSkillAttackAftEnter(bf);
    }

    return rcnt;
}

UInt32 BattleSimulator::doSkillAttackAftEnter(BattleFighter* bf)
{
    UInt32 rcnt = 0;

    const GData::SkillBase* passiveSkill = NULL;
    size_t skillIdx = 0;
    while(NULL != (passiveSkill = bf->getPassiveSkillEnter100(skillIdx)))
    {
        int target_side, target_pos, cnt;
        getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
        doSkillAttack(bf, passiveSkill, target_side, target_pos, cnt, NULL);
        ++ rcnt;
    }

    if(NULL != (passiveSkill = bf->getPassiveSkillEnter()))
    {
        int target_side, target_pos, cnt;
        getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
        doSkillAttack(bf, passiveSkill, target_side, target_pos, 1, NULL);
        ++ rcnt;
    }

    return rcnt;
}

// single attack round
UInt32 BattleSimulator::doAttack( int pos )
{
	UInt32 rcnt = 0;
    std::vector<BattleFighter*>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
	BattleFighter* bf = cur_fgtlist[pos];

	cur_fgtlist.erase(cur_fgtlist.begin() + pos);

	// update all action points
#if 0
	UInt32 minact = fs.action;
	for(size_t i = 0; i < _fgtlist.size(); ++ i)
	{
		if(_fgtlist[i].bfgt->getHP() == 0)
			continue;
		_fgtlist[i].addAction(minact);
	}
#endif

	// insert the fighter to next queue by order
#if 0
    UInt32 bPoisonLevel = bf->getPoisonLevel();
    if(bPoisonLevel > 0)
    {
        UInt32 round = bf->getPoisonRound();
        if(round > 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;
            StatusChange scList[50];
            size_t scCount = 0;

            memset(defList, 0, sizeof(defList));
            UInt32 fdmg = bf->getMaxHP() * bPoisonLevel/100;
            -- round;
            defList[defCount].damType = (round > 0) ? e_Poison : e_UnPoison;
            defList[defCount].damage = fdmg;
            defList[defCount].pos = bf->getPos() + 25;
            bf->makeDamage(fdmg);
            defList[defCount].leftHP = bf->getHP();
            ++ defCount;
            bf->setPoisonRound(round);
            if(round == 0)
                bf->setPoisonLevel(0);
            //else
            //{
            //	fs.resetAction();
            //	insertFighterStatus(fs);
            //}
            // killed
 
            std::vector<AttackAct> atkAct;
            atkAct.clear();
            if(bf->getHP() == 0)
            {
                onDead(bf, &atkAct);
                return 1;
            }
            else if(_winner == 0)
                onDamage(bf, scList, scCount, false);
            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
            rcnt++;
 
            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL);
                rcnt++;
            }

            atkAct.clear();
       }
    }
#endif
    //fs.resetAction();
	UInt32 stun = bf->getStunRound();
    UInt32 confuse = bf->getConfuseRound();
    UInt32 forget = bf->getForgetRound();

    rcnt += releaseCD(bf);
    insertFighterStatus(bf);

    if(stun > 0)
    {
        return rcnt;
    }

    int target_pos;
    int otherside = 1 - bf->getSide();
    UInt32 dmg = 0;
    if(confuse > 0)
    {
        BattleFighter* rnd_bf = NULL; 
        if(_rnd(2) == bf->getSide())
        {
            UInt8 myPos = bf->getPos();
            rnd_bf = getRandomFighter(bf->getSide(), &myPos, 1);
        }

        if(NULL == rnd_bf)
        {
	        target_pos = getPossibleTarget(bf->getSide(), bf->getPos());
        }
        else
        {
            target_pos = rnd_bf->getPos();
            otherside = bf->getSide();
        }

        if(target_pos < 0)
            return rcnt;

        std::vector<AttackAct> atkAct;
        atkAct.clear();
        dmg += doNormalAttack(bf, otherside, target_pos, rnd_bf != NULL ? &atkAct : NULL);
        ++ rcnt;

        if(rnd_bf && rnd_bf->getHP() == 0)
        {
            onDead(rnd_bf, &atkAct);
        }

        size_t actCnt = atkAct.size();
        for(size_t idx = 0; idx < actCnt; idx++)
        {
            doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
            BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
            if(tmpbo && tmpbo->getHP() == 0)
            {
                onDead(tmpbo, &atkAct);
                actCnt = atkAct.size();
            }
            ++ rcnt;
        }

        atkAct.clear();

        return rcnt;
    }
    else if(forget > 0)
    {
        target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

        if(target_pos < 0)
            return rcnt;

        std::vector<AttackAct> atkAct;
        atkAct.clear();
        dmg += doNormalAttack(bf, otherside, target_pos, &atkAct);
        ++ rcnt;

        size_t actCnt = atkAct.size();
        for(size_t idx = 0; idx < actCnt; idx++)
        {
            doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
            BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
            if(tmpbo && tmpbo->getHP() == 0)
            {
                onDead(tmpbo, &atkAct);
                actCnt = atkAct.size();
            }
            ++ rcnt;
        }
        atkAct.clear();

        return rcnt;
    }
    else
    {
        const GData::SkillBase* skill = NULL;
        // do preve attack passive skill that must act
        size_t skillIdx = 0;
        int cnt = 0;
        while(NULL != (skill = bf->getPassiveSkillPrvAtk100(skillIdx)))
        {
            getSkillTarget(bf, skill, otherside, target_pos, cnt);
            if(cnt < 1)
                return rcnt;

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            dmg += doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct);
            ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                if(tmpbo && tmpbo->getHP() == 0)
                {
                    onDead(tmpbo, &atkAct);
                    actCnt = atkAct.size();
                }
                ++ rcnt;
            }
            atkAct.clear();
        }

        skill = bf->getPassiveSkillPreAtk();
        if(NULL != skill)
        {
            getSkillTarget(bf, skill, otherside, target_pos, cnt);
            if(cnt < 1)
                return rcnt;

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            dmg += doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct);
            ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                if(tmpbo && tmpbo->getHP() == 0)
                {
                    onDead(tmpbo, &atkAct);
                    actCnt = atkAct.size();
                }
                ++ rcnt;
            }
            atkAct.clear();
        }

        // do active skill
        BattleFighter* therapy_bf = getTherapyTarget(bf);
        skill = bf->getActiveSkill(therapy_bf!= NULL);
        if(NULL != skill)
        {
            if(NULL != therapy_bf && (skill->effect->hpP > 0 || skill->effect->hp > 0 || skill->effect->addhp > 0) && skill->target == 0)
            {
                otherside = therapy_bf->getSide();
                target_pos = therapy_bf->getPos();
                cnt = 1;
            }
            else
            {
                getSkillTarget(bf, skill, otherside, target_pos, cnt);
            }
            if(cnt < 1)
                return rcnt;

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            dmg += doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct);
            ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                if(tmpbo && tmpbo->getHP() == 0)
                {
                    onDead(tmpbo, &atkAct);
                    actCnt = atkAct.size();
                }
                ++ rcnt;
            }
            atkAct.clear();
        }
        else
        {
            target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

            if(target_pos < 0)
                return rcnt;

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            dmg += doNormalAttack(bf, otherside, target_pos, &atkAct);
            ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                if(tmpbo && tmpbo->getHP() == 0)
                {
                    onDead(tmpbo, &atkAct);
                    actCnt = atkAct.size();
                }
                ++ rcnt;
            }
            atkAct.clear();

            // 普通攻击后的被动技能
            if(bf->getHP() > 0)
            {
                size_t idx = 0;
                const GData::SkillBase* passiveSkill = NULL;
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[otherside][target_pos]);
                while(NULL != (passiveSkill = bf->getPassiveSkillAftNAtk100(idx)))
                {
                    // 敌方
                    if(passiveSkill->target == 1 && (!bo || bo->getHP() == 0))
                    {
                        continue;
                    }

                    std::vector<AttackAct> atkAct;
                    atkAct.clear();
                    doSkillAttack(bf, passiveSkill, otherside, target_pos, 1, &atkAct);
                    ++ rcnt;

                    size_t actCnt = atkAct.size();
                    for(size_t idx = 0; idx < actCnt; idx++)
                    {
                        doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                        BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                        if(tmpbo && tmpbo->getHP() == 0)
                        {
                            onDead(tmpbo, &atkAct);
                            actCnt = atkAct.size();
                        }
                        ++ rcnt;
                    }
                    atkAct.clear();
                }

                passiveSkill = bf->getPassiveSkillAftNAtk();
                if(NULL != passiveSkill && (passiveSkill->target != 1 || (passiveSkill->target == 1 && bo && bo->getHP() > 0)) )
                {
                    std::vector<AttackAct> atkAct;
                    atkAct.clear();
                    doSkillAttack(bf, passiveSkill, otherside, target_pos, 1, &atkAct);
                    ++ rcnt;

                    size_t actCnt = atkAct.size();
                    for(size_t idx = 0; idx < actCnt; idx++)
                    {
                        doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                        BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                        if(tmpbo && tmpbo->getHP() == 0)
                        {
                            onDead(tmpbo, &atkAct);
                            actCnt = atkAct.size();
                        }
                        ++ rcnt;
                    }
                    atkAct.clear();
                }
            }
        }
    }

    // 攻击后的被动技能
    if(bf->getHP() > 0)
    {
        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        BattleFighter* bo = static_cast<BattleFighter*>(_objs[otherside][target_pos]);
        while(NULL != (passiveSkill = bf->getPassiveSkillAftAtk100(idx)))
        {
            if(passiveSkill->target == 1 && (!bo || bo->getHP() == 0))
            {
                continue;
            }

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            doSkillAttack(bf, passiveSkill, otherside, target_pos, 1, &atkAct);
            ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                if(tmpbo && tmpbo->getHP() == 0)
                {
                    onDead(tmpbo, &atkAct);
                    actCnt = atkAct.size();
                }
                ++ rcnt;
            }
            atkAct.clear();
        }

        passiveSkill = bf->getPassiveSkillAftAtk();
        if(NULL != passiveSkill && (passiveSkill->target != 1 || (passiveSkill->target == 1 && bo && bo->getHP() > 0)) )
        {
            std::vector<AttackAct> atkAct;
            atkAct.clear();
            doSkillAttack(bf, passiveSkill, otherside, target_pos, 1, &atkAct);
            ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param);
                BattleFighter* tmpbo = static_cast<BattleFighter*>(_objs[atkAct[idx].target_side][atkAct[idx].target_pos]);
                if(tmpbo && tmpbo->getHP() == 0)
                {
                    onDead(tmpbo, &atkAct);
                    actCnt = atkAct.size();
                }
                ++ rcnt;
            }
            atkAct.clear();
        }
    }

#if 0
	if(target_object != NULL)
	{
		// find all targets that are hit
		GData::Area * area = NULL;
		GObject::ItemWeapon * weapon = bf->getFighter()->getWeapon();
		if(weapon != NULL)
		{
            // TODO: no weapon_def
			// area = &weapon->getWeaponDef().getArea();
            area = &GData::Area::getDefault();
		}
		else
		{
			area = &GData::Area::getDefault();
		}
		int cnt = area->getCount();
		if(cnt <= 0)
			return 0;

		DefStatus defList[25];
		size_t defCount = 0;
		StatusChange scList[50];
		size_t scCount = 0;

		// calculate damage
		bool cs = false;
		bool pr = bf->calcPierce();
		float atk = bf->calcAttack(cs);

		float factor = (*area)[0].factor;

		rcnt += tryPreUseSkill(bf, target_object);
		UInt32 dmg = 0;
		if(cnt > 1)
		{
			AttackPoint ap[25];
			int apcnt = 0;
			int x_ = target_pos % 5;
			int y_ = target_pos / 5;
			for(int i = 1; i < cnt; ++ i)
			{
				GData::Area::Data& ad = (*area)[i];
				int x = x_ + ad.x;
				int y = y_ + ad.y;
				if(x < 0 || x > 4 || y < 0 || y > 4)
				{
					continue;
				}
				ap[apcnt].pos = x + y * 5;
				ap[apcnt].type = ad.type;
				ap[apcnt ++].factor = ad.factor;
			}
			// attack the target on center
			dmg = attackOnce(bf, atk, cs, pr, target_object, factor, defList, defCount, scList, scCount, apcnt, ap);
			for(int i = 0; i < apcnt; ++ i)
			{
				switch(ap[i].type)
				{
				case 0:
					factor = ap[i].factor;
					attackOnce(bf, atk, cs, pr, _objs[otherside][ap[i].pos], factor, defList, defCount, scList, scCount);
					break;
				case 1:
					factor = (*area)[0].factor;
				case 2:
					{
						float newfactor = factor * ap[i].factor;
						if(attackOnce(bf, atk, cs, pr, _objs[otherside][ap[i].pos], newfactor, defList, defCount, scList, scCount))
							factor = newfactor;
					}
					break;
				}
			}
		}
		else
		{
			// attack only one target
			dmg = attackOnce(bf, atk, cs, pr, target_object, factor, defList, defCount, scList, scCount, 0);
		}
		// damage by skill or buff
		UInt16 skillid = 0;
		if(_winner == 0)
		{
			if(dmg > 0 && target_object->isChar())
			{
                // TODO: 
				BattleFighter * target_fighter = static_cast<BattleFighter *>(target_object);
				UInt16 myskillid = bf->getFighter()->getSkill();
				if(target_fighter->getHP() > 0)
				{
					switch(myskillid)
					{
					case 202:
					case 203:
					case 204:
					case 209:
						{
							UInt8 poisonLevel = target_fighter->getPoisonLevel();
							if(myskillid - 201 <= poisonLevel)
							{
								poisonLevel = 0;
								break;
							}
							Script::BattleFormula::SkillData& sd = _formula->skillData(1, myskillid - 201, bf->getFighter()->getSkillLevel());
							if(_rnd(100) >= sd.rate)
							{
								poisonLevel = 0;
								break;
							}
							poisonLevel = static_cast<UInt8>(myskillid - 201);
							if(poisonLevel > 3)
								poisonLevel = 4;
							target_fighter->setPoisonLevel(poisonLevel);
							skillid = myskillid;
							UInt32 round = sd.value3;
							if(round > 0)
							{
								UInt32 fdmg;
								if(target_fighter->hasFlag(BattleFighter::IsMirror))
								{
									fdmg = target_fighter->getHP();
								}
								else
								{
									const UInt16 poisonSid[] = {0, 1, 2, 3, 8};
									Script::BattleFormula::SkillData& sd = _formula->skillData(1, poisonSid[poisonLevel], bf->getFighter()->getSkillLevel());
									float pdmg = static_cast<float>(sd.value1) * bf->getAttack();
									fdmg = _formula->calcDamage(pdmg, target_fighter->getDefend());
								}
								-- round;
								defList[defCount].damType = (round > 0) ? 6 : 7;
								defList[defCount].damage = fdmg;
								defList[defCount].pos = target_pos;
								target_object->makeDamage(fdmg);
								defList[defCount].leftHP = target_fighter->getHP();
								++ defCount;
								target_fighter->setPoisonRound(round);
								if(round == 0)
									target_fighter->setPoisonLevel(0);
								// killed
								if(target_object->getHP() == 0)
									onDead(target_fighter);
								else if(_winner == 0)
								{
									onDamage(target_fighter, scList, scCount, true);
									FighterStatus fs2(target_fighter, bf->getAction() * 3 / 4, fdmg);
									insertFighterStatus(fs2);
								}
							}
						}
						break;
					case 305:
					case 306:
					case 307:
					case 309:
						{
							UInt8 confuseLevel = target_fighter->getConfuseLevel();
							if(myskillid - 304 <= confuseLevel)
								break;
							Script::BattleFormula::SkillData& sd = _formula->skillData(2, myskillid - 301, bf->getFighter()->getSkillLevel());
							if(_rnd(100) >= sd.rate)
								break;
							if(confuseLevel > 0)
							{
								target_fighter->_hitrateAdd = 0;
							}
							confuseLevel = myskillid - 304;
							if(confuseLevel > 3)
								confuseLevel = 4;
							target_fighter->setConfuseLevel(confuseLevel);
							skillid = myskillid;
							switch(confuseLevel)
							{
							case 1:
							case 2:
								setStatusChange(target_fighter->getSide(), target_pos, 1, myskillid, 4, -sd.value1, scList, scCount, true);
								break;
							case 3:
								setStatusChange(target_fighter->getSide(), (target_pos / 5 * 5), 5, myskillid, 4, -sd.value1, scList, scCount, true);
								break;
							case 4:
								setStatusChange(target_fighter->getSide(), 0, 25, myskillid, 4, -sd.value1, scList, scCount, true);
								break;
							}
						}
						break;
					}
				}
				if(bf->getHP() > 0)
				{
					switch(myskillid)
					{
					case 107:
					case 108:
						{
							Script::BattleFormula::SkillData& sd = _formula->skillData(0, myskillid - 101, bf->getFighter()->getSkillLevel());
							if(_rnd(100) >= sd.rate)
								break;
							UInt32 rhp = static_cast<UInt32>(sd.value1 * dmg);
							if(rhp == 0)
								rhp = 1;
							defList[defCount].damType = 3;
							defList[defCount].damage = rhp;
							defList[defCount].pos = 25 + bf->getPos();
							bf->regenHP(rhp);
							defList[defCount].leftHP = bf->getHP();
							++ defCount;
							skillid = myskillid;
						}
						break;
					}
					if(_winner == 0 && defCount > 0 && (defList[0].damType & 0x17) == 0 && target_fighter->getHP() > 0)
					{
						UInt8 thornType = target_fighter->getThornLevel();
						UInt16 tskillid = target_fighter->getFighter()->getSkill();
						switch(tskillid)
						{
						case 101:
						case 102:
						case 103:
						case 109:
							{
								if(tskillid - 100 <= thornType)
									break;
								Script::BattleFormula::SkillData& sd = _formula->skillData(0, tskillid - 101, target_fighter->getFighter()->getSkillLevel());
								if(_rnd(100) >= sd.rate)
									break;
								thornType = static_cast<UInt8>(tskillid - 100);
								if(thornType > 3)
									thornType = 4;
								target_fighter->setThornLevel(thornType);
							}
							break;
						}
						if(thornType > 0)
						{
							const UInt16 thronSid[] = {0, 0, 1, 2, 8};
							Script::BattleFormula::SkillData& sd = _formula->skillData(0, thronSid[thornType], target_fighter->getFighter()->getSkillLevel());
							float cdmg;
							switch(thornType)
							{
							case 1:
							case 2:
								cdmg = static_cast<float>(sd.value1);
								break;
							case 3:
							case 4:
							default:
								cdmg = sd.value1 * target_fighter->getMaxHP();
								break;
							}
							UInt32 fdmg = _formula->calcDamage(cdmg, bf->getDefend());
							defList[0].damType = 5;
							defList[0].counterDmg = fdmg;
							bf->makeDamage(fdmg);
							defList[0].counterLeft = bf->getHP();
							// killed
							if(bf->getHP() == 0)
								onDead(bf);
							else if(_winner == 0)
								onDamage(bf, scList, scCount, false);
						}
					}
				}
			}
			UInt8 confuseLevel = bf->getConfuseLevel();
			if(confuseLevel > 0)
			{
				bf->setConfuseLevel(0);
				setStatusChange(bf->getSide(), pos, 1, 0, 4, static_cast<UInt32>(0), scList, scCount, false);
			}
		}

		appendToPacket(bf->getSide(), bf->getPos(), target_pos, static_cast<UInt8>(skillid > 0 ? 2 : 0), skillid, cs, pr, defList, defCount, scList, scCount);
		++ rcnt;
		if(_winner == 0 && bf->getHP() > 0)
			rcnt += tryDelayUseSkill(bf, target_object);

		if(target_object->getHP() > 0)
		{
			BattleFighter * tmp_bf = bf;
			float atkRate;
			UInt16 skillId = 0;
			if((atkRate = testLink(tmp_bf, skillId)) > 0.0f && tmp_bf != NULL)
			{
				bool tmp_cs = false;
				bool tmp_pr = tmp_bf->calcPierce();
				float tmp_atk = tmp_bf->calcAttack(tmp_cs) * atkRate;
				defCount = 0;
				scCount = 0;
				GData::Area * area2 = NULL;
				if(tmp_bf->getFighter()->getWeapon() != NULL)
				{
                    // TODO: no weapon_def
					// area2 = &tmp_bf->getFighter()->getWeapon()->getWeaponDef().getArea();
				}
				else
				{
					area2 = &GData::Area::getDefault();
				}
				if(area2->getCount() > 0)
					atkRate *= (*area)[0].factor;
				if(skillId > 0)
				{
					appendToPacket(bf->getSide(), bf->getPos(), tmp_bf->getPos() + 25, 2, skillId, false, false, NULL, 0, NULL, 0);
					++ rcnt;
				}
				attackOnce(tmp_bf, tmp_atk, tmp_cs, tmp_pr, NULL, target_object, atkRate, defList, defCount, scList, scCount);
				if(defCount > 0)
				{
					appendToPacket(bf->getSide(), tmp_bf->getPos(), target_pos, 0, 0, tmp_cs, tmp_pr, defList, defCount, scList, scCount);
					++ rcnt;
				}
			}
		}
	}
#endif
	return rcnt;
}

// try to pre-use skill before attack
UInt32 BattleSimulator::tryPreUseSkill( BattleFighter * bf, BattleObject * target_object )
{
	return 0;
}

struct _PosAndHP
{
	UInt32 hp;
	UInt8 pos;
	bool operator <(const _PosAndHP& other) const
	{
		return hp > other.hp;
	}
};

// try to post-use skill after attack
UInt32 BattleSimulator::tryDelayUseSkill( BattleFighter * bf, BattleObject * target_object )
{
#if 0
	UInt16 skillId = bf->getFighter()->getSkill();
	switch(skillId)
	{
	case 104:
	case 105:
	case 110:
		{
			Script::BattleFormula::SkillData& sd = _formula->skillData(0, skillId - 101, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				break;

			float atk = sd.value1 * bf->getAttack();
			UInt8 side = target_object->getSide();
			UInt8 pos = target_object->getPos(), minpos = pos / 5 * 5, maxpos = minpos + 5;
			size_t defCount = 0, scCount = 0;
			DefStatus defList[5];
			StatusChange scList[50];
			for(UInt8 i = minpos; i < maxpos; ++ i)
			{
				BattleObject * bo = _objs[side][i];
				if(bo == NULL || bo->getHP() == 0)
					continue;
				UInt32 dmg;
				if(bo->isChar())
					dmg = _formula->calcDamage(atk, static_cast<BattleFighter *>(bo)->getDefend());
				else
					dmg = static_cast<UInt32>(atk);
				bo->makeDamage(dmg);
				defList[defCount].pos = i;
				defList[defCount].damType = 0;
				defList[defCount].damage = dmg;
				defList[defCount].leftHP = bo->getHP();
				++ defCount;
				if(bo->getHP() == 0)
					onDead(bo);
				else if(_winner == 0)
					onDamage(bo, scList, scCount, true);
			}
			if(defCount > 0)
			{
				appendToPacket(bf->getSide(), bf->getPos(), pos, 2, skillId, false, false, defList, defCount, scList, scCount);
				return 1;
			}
		}
		return 0;
	case 201:
		{
			if(target_object->getHP() == 0)
				return 0;
			Script::BattleFormula::SkillData& sd = _formula->skillData(1, skillId - 201, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				return 0;
			UInt8 pos = target_object->getPos();
			float atk = bf->getAttack();
			UInt32 dmg;
			if(target_object->isChar())
			{
				float def = static_cast<BattleFighter *>(target_object)->getDefend();
				dmg = _formula->calcDamage(atk, def);
			}
			else
			{
				dmg = static_cast<UInt32>(atk);
			}
			size_t defCount = 0;
			DefStatus defList[5];
			for(UInt32 i = 0; i < sd.value3; ++ i)
			{
				UInt32 fdmg = static_cast<UInt32>((sd.value1 + _rnd(static_cast<UInt32>(sd.value2 - sd.value1) + 1)) / 100.0f * dmg);
				target_object->makeDamage(fdmg);
				defList[defCount].pos = pos;
				defList[defCount].damType = 0;
				defList[defCount].damage = fdmg;
				defList[defCount].leftHP = target_object->getHP();
				++ defCount;
				if(target_object->getHP() == 0)
					break;
			}
			StatusChange scList[50];
			size_t scCount = 0;
			if(target_object->isChar())
			{
				if(target_object->getHP() == 0)
					onDead(target_object);
				else if(_winner == 0)
					onDamage(target_object, scList, scCount, true);
			}
			appendToPacket(bf->getSide(), bf->getPos(), pos, 2, skillId, false, false, defList, defCount, scList, scCount);
			return 1;
		}
		return 0;
	case 205:
	case 208:
		{
			if(bf->hasFlag(BattleFighter::Mirrored))
				return 0;
			Script::BattleFormula::SkillData& sd = _formula->skillData(1, skillId - 201, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				return 0;
			UInt8 posList[25];
			size_t posCount = 0;
			UInt8 side = bf->getSide();
			UInt8 y = 0;
			while(y < 5)
			{
				UInt8 p = y * 5;
				for(UInt8 x = 0; x < 5; ++ x, ++ p)
				{
					if(_objs[side][p] == NULL || _objs[side][p]->getHP() == 0)
						posList[posCount ++] = p;
				}
				if(posCount > 0)
					break;
				++ y;
			}
			if(posCount == 0)
				return 0;
			bf->addFlag(BattleFighter::Mirrored);
			UInt8 pos = posList[_rnd(posCount)];
			BattleFighter * newf = new(std::nothrow) Battle::BattleFighter(*bf);
			if(newf == NULL)
				return 0;
			newf->setSideAndPos(side, pos);
			newf->addFlag(BattleFighter::IsMirror);
			setObject(side, pos, newf);
			newf->_attack = newf->_attack * sd.value1;
			newf->_attackAdd = newf->_attackAdd * sd.value1;
			newf->_defendAdd = 0;
			FighterStatus fs(newf);
			fs.action = 0;
			// insert fighter into queue by order
			insertFighterStatus(fs);
			appendToPacket(bf->getSide(), bf->getPos(), pos + 25, 2, skillId, false, false, NULL, 0, NULL, 0);
			return 1;
		}
		return 0;
	case 206:
	case 207:
	case 210:
		{
			Script::BattleFormula::SkillData& sd = _formula->skillData(1, skillId - 201, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				return 0;
			float atk = sd.value1 * bf->getAttack();
			UInt8 side = target_object->getSide();
			size_t defCount = 0, scCount = 0;
			DefStatus defList[25];
			StatusChange scList[50];
			for(UInt8 i = 0; i < 25; ++ i)
			{
				BattleObject * bo = _objs[side][i];
				if(bo == NULL || bo->getHP() == 0)
					continue;
				UInt32 dmg;
				if(bo->isChar())
					dmg = _formula->calcDamage(atk, static_cast<BattleFighter *>(bo)->getDefend());
				else
					dmg = static_cast<UInt32>(atk);
				bo->makeDamage(dmg);
				defList[defCount].pos = i;
				defList[defCount].damType = e_damNormal;
				defList[defCount].damage = dmg;
				defList[defCount].leftHP = bo->getHP();
				++ defCount;
				if(bo->getHP() == 0)
					onDead(bo);
				else if(_winner == 0)
					onDamage(bo, scList, scCount, true);
			}
			if(defCount > 0)
			{
				appendToPacket(bf->getSide(), bf->getPos(), target_object->getPos(), 2, skillId, false, false, defList, defCount, scList, scCount);
				return 1;
			}
		}
		return 0;
	case 303:
		{
			Script::BattleFormula::SkillData& sd = _formula->skillData(2, 2, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				return 0;
			UInt8 side = bf->getSide();
			UInt32 rhp = sd.value3 + static_cast<UInt32>(sd.value1 * bf->getIntelligence());
			UInt32 maxrcount = sd.value4;
			_PosAndHP rlist[25];
			UInt32 rcount = 0;
			for(UInt8 pos = 0; pos < 25; ++ pos)
			{
				BattleObject * bo = _objs[side][pos];
				if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
					continue;
				rlist[rcount].hp = static_cast<BattleFighter *>(bo)->getLostHP();
				if(rlist[rcount].hp == 0)
					continue;
				rlist[rcount].pos = pos;
				if(rcount >= maxrcount)
				{
					std::push_heap(rlist, rlist + maxrcount + 1);
					std::pop_heap(rlist, rlist + maxrcount + 1);
				}
				else
				{
					++ rcount;
					std::push_heap(rlist, rlist + rcount);
				}
			}
			DefStatus defList[25];
			size_t defCount = 0;
			for(UInt32 i = 0; i < rcount; ++ i)
			{
				BattleObject * bo = _objs[side][rlist[i].pos];
				UInt32 hpr = static_cast<BattleFighter *>(bo)->regenHP(rhp);
				if(hpr == 0)
					continue;
				defList[defCount].pos = rlist[i].pos + 25;
				defList[defCount].damType = e_damHpAdd;
				defList[defCount].damage = hpr;
				defList[defCount].leftHP = bo->getHP();
				++ defCount;
			}
			appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, 303, false, false, defList, defCount, NULL, 0);
		}
		return 1;
	case 304:
	case 308:
		{
			Script::BattleFormula::SkillData& sd = _formula->skillData(2, skillId - 301, bf->getFighter()->getSkillLevel());
			if(_rnd(100) >= sd.rate)
				return 0;
			StatusChange scList[50];
			size_t scCount = 0;
			UInt8 side = target_object->getSide();
			DefStatus defList[25];
			float atk = sd.value1;
			for(UInt32 i = 0; i < sd.value3; ++ i)
			{
				BattleFighter * tf = getRandomFighter(side, NULL, 0);
				if(tf != NULL)
					defList[i].pos = tf->getPos();
				else
					defList[i].pos = 12;
			    BattleObject * bo = _objs[side][defList[i].pos];
			    if(bo == NULL || bo->getHP() == 0)
			    {
			        defList[i].damType = e_damNormal;
			        defList[i].damage = 0;
			        defList[i].leftHP = 0;
			        continue;
			    }
				UInt32 dmg;
				if(bo->isChar())
				{
					dmg = _formula->calcDamage(atk, static_cast<BattleFighter *>(bo)->getDefend());
					static_cast<BattleFighter *>(bo)->setStunRound(sd.value4 + 1);
				}
				else
					dmg = static_cast<UInt32>(atk);
				bo->makeDamage(dmg);
				defList[i].damType = e_damNormal;
				defList[i].damage = dmg;
				defList[i].leftHP = bo->getHP();
				if(bo->getHP() == 0)
					onDead(bo);
				else if(_winner == 0)
					onDamage(bo, scList, scCount, true);
			}
			appendToPacket(bf->getSide(), bf->getPos(), target_object->getPos(), 2, skillId, false, false, defList, sd.value3, scList, scCount);
		}
		return 1;
	default:
		return 0;
	}
#endif
	return 0;
}

int BattleSimulator::testWinner()
{
	int alive[2] = { 0, 0 };
	for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
        size_t c = fgtlist.size();
        for(size_t i = 0; i < c; ++ i)
        {
            alive[fgtlist[i]->getSide()] ++;
        }
    }
	if(alive[0] == 0)
		return 2;
	else if(alive[1] == 0)
		return 1;
	return 0;
}

void BattleSimulator::appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr, DefStatus* defList, size_t defCount, StatusChange * scList, size_t scCount)
{
	_packet << static_cast<UInt8>(from_side);
	// attack mode
	_packet << static_cast<UInt8>(atk_type | (cs ? 0x80 : 0) | (pr ? 0x40 : 0)) << add_id;
	// reserved
	_packet << static_cast<UInt8>(0);
	// attack point
	_packet << static_cast<UInt8>(target_pos);
	// action players count
	_packet << static_cast<UInt8>(1);
	// action players list
	_packet << static_cast<UInt8>(from_pos) << static_cast<UInt8>(0);
	// attacked players count
	_packet << static_cast<UInt8>(defCount);
	// attacked players list
	for(size_t i = 0; i < defCount; ++ i)
	{
		_packet << defList[i].pos << defList[i].damType << defList[i].damType2 << defList[i].damage << defList[i].leftHP;
        if(defList[i].damType == e_damAbsorb)
        {
            _packet << defList[i].rhp << defList[i].rLeftHP;
        }

		if((defList[i].damType2 & 0x80) == 0x80)
		{
			_packet << defList[i].counterDmg << defList[i].counterLeft;
		}
	}
	// status change
	_packet << static_cast<UInt8>(scCount);
	for(size_t i = 0; i < scCount; ++ i)
	{
		_packet << scList[i].pos << scList[i].statusId << scList[i].type << scList[i].data;
	}

#ifdef _DEBUG
	char path[1024], path2[1024];
	sprintf(path, "%s0/0", cfg.reportPath.c_str());
	sprintf(path2, "%s/%u.log", path, 0);
	File rfile(path);
	rfile.createDirectories();
	FILE * f = fopen(path2, "a+");
	if(f == NULL)
		return;

    char szBuf[256] = {0};
    sprintf(szBuf, "from_side=%d\r\n", from_side);
	fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "atk_type=%d\r\n", atk_type);
	fwrite(szBuf, 1, strlen(szBuf), f);
    if(cs)
        sprintf(szBuf, "critical\r\n");
    if(pr)
        sprintf(szBuf, "pierce\r\n");

    sprintf(szBuf, "skillId=%d\r\n", add_id);
	fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "target_pos=%d\r\n", target_pos);
	fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "from_pos=%d\r\n", from_pos);
	fwrite(szBuf, 1, strlen(szBuf), f);
    sprintf(szBuf, "defCount=%lu\r\n", defCount);
	fwrite(szBuf, 1, strlen(szBuf), f);
	for(size_t i = 0; i < defCount; ++ i)
	{
        sprintf(szBuf, "defList[%lu].pos=%d\r\n", i, defList[i].pos);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].damType=%d\r\n", i, defList[i].damType);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].damage=%d\r\n", i, defList[i].damage);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "defList[%lu].leftHP=%d\r\n", i, defList[i].leftHP);
        fwrite(szBuf, 1, strlen(szBuf), f);
        if(defList[i].damType == e_damAbsorb)
        {
            sprintf(szBuf, "defList[%lu].rhp=%d\r\n", i, defList[i].rhp);
            fwrite(szBuf, 1, strlen(szBuf), f);
            sprintf(szBuf, "defList[%lu].rLeftHP=%d\r\n", i, defList[i].rLeftHP);
            fwrite(szBuf, 1, strlen(szBuf), f);
        }

		if((defList[i].damType2 & 0x80) == 0x80)
		{
            sprintf(szBuf, "defList[%lu].counterDmg=%d\r\n", i, defList[i].counterDmg);
            fwrite(szBuf, 1, strlen(szBuf), f);
            sprintf(szBuf, "defList[%lu].counterLeft=%d\r\n", i, defList[i].counterLeft);
            fwrite(szBuf, 1, strlen(szBuf), f);
		}
	}
	// status change
    sprintf(szBuf, "scCount=%lu\r\n", scCount);
    fwrite(szBuf, 1, strlen(szBuf), f);
	for(size_t i = 0; i < scCount; ++ i)
	{
        sprintf(szBuf, "scList[%lu].pos=%d\r\n", i, scList[i].pos);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "scList[%lu].type=%d\r\n", i, scList[i].type);
        fwrite(szBuf, 1, strlen(szBuf), f);
        sprintf(szBuf, "scList[%lu].data=%d\r\n", i, scList[i].data);
        fwrite(szBuf, 1, strlen(szBuf), f);
	}

    sprintf(szBuf, "------------------------------------\r\n");
    fwrite(szBuf, 1, strlen(szBuf), f);
	fclose(f);
#endif
}

bool BattleSimulator::applyFighterHP( UInt8 side, GObject::Player * player, bool useRegen, UInt32 sysRegen )
{
	bool res = false;
	UInt32 autohp = player->getBuffData(PLAYER_BUFF_AUTOHEAL, 0);
	for(int j = 0; j < 5; ++ j)
	{
		GObject::Lineup& pd = player->getLineup(j);
		if(pd.fighter == NULL)
			continue;
		BattleObject * obj = _objs[side][pd.pos];
		if(obj != NULL && obj->isChar())
		{
			BattleFighter * bf = static_cast<BattleFighter *>(obj);
			UInt16 hp = bf->getHP();

			UInt32 oldmaxhp = pd.fighter->getMaxHP();
			UInt32 currhp = pd.fighter->getCurrentHP();
			if(currhp == 0)
				currhp = oldmaxhp;
			UInt32 newhp = hp * oldmaxhp / bf->getMaxHP();
			if(newhp > currhp)
				newhp = currhp;
			if(useRegen)
			{
				if(newhp == 0)
					newhp = 1;
				if(autohp > 0)
				{
					if(newhp < oldmaxhp)
					{
						if(newhp + autohp >= oldmaxhp)
						{
							autohp -= oldmaxhp - newhp;
							newhp = 0;
						}
						else
						{
							newhp += autohp;
							autohp = 0;
							res = true;
						}
					}
					else
						newhp = 0;
				}
				else
					res = true;
			}
			else
			{
				if(newhp < currhp)
				{
					newhp = (currhp - newhp) * sysRegen / 100 + newhp;
					if(newhp == 0)
						newhp = 1;
					res = true;
				}
			}
			pd.fighter->setCurrentHP(newhp);
		}
	}
	player->setBuffData(0, autohp);
	return res;
}

void BattleSimulator::getFighterHP( UInt8 side, GObject::Fighter ** fighters, UInt8 * pos, UInt32 * hp, UInt32 percent )
{
	for(int j = 0; j < 5; ++ j)
	{
		if(fighters[j] == NULL)
			continue;
		BattleObject * obj = _objs[side][pos[j]];
		if(obj != NULL && obj->isChar())
		{
			BattleFighter * bf = static_cast<BattleFighter *>(obj);
			UInt16 hp_ = bf->getHP();
			UInt32 maxhp = bf->getMaxHP();
			if(percent > 0)
			{
				hp_ = (maxhp - hp_) * percent / 100 + hp_;
			}
			if(hp_ >= maxhp)
				hp_ = 0;
			else if(hp_ == 0)
				hp_ = 1;
			hp[j] = hp_;
		}
	}
}

BattleFighter * BattleSimulator::newFighter( UInt8 side, UInt8 pos, GObject::Fighter * fgt )
{
	BattleFighter * bf = new(std::nothrow) Battle::BattleFighter(_formula, fgt, side, pos);
	setObject(side, pos, bf);
	return bf;
}

void BattleSimulator::setStatusChange( UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, UInt32 value, StatusChange * scList, size_t& scCount, bool active )
{
	for(UInt8 i = pos; i < pos + cnt; ++ i)
	{
		if(_objs[side][i] != NULL)
		{
			BattleObject * bo = _objs[side][i];
			if(bo->isChar() && _isBody[side][pos] == 0)
			{
				BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
				StatusChange& sc = scList[scCount];
				sc.pos = static_cast<UInt8>(i);
				if(!active)
					sc.pos += 25;
				sc.statusId = skillId;
				sc.type = 1;
				switch(type)
				{
				case 3:
					bfgt->setActionAdd(value);
					sc.data = static_cast<UInt16>(bfgt->getAction());
					break;
				default:
					bfgt->setHP(value);
					sc.data = static_cast<UInt16>(bfgt->getHP());
					break;
				}
				++ scCount;
			}
		}
	}
}

void BattleSimulator::setStatusChange2( UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
	for(UInt8 i = pos; i < pos + cnt; ++ i)
	{
		if(_objs[side][i] != NULL)
		{
			BattleObject * bo = _objs[side][i];
			if(bo->isChar() && _isBody[side][pos] == 0 && bo->getHP() > 0)
			{
				BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
				StatusChange& sc = scList[scCount];
				sc.pos = static_cast<UInt8>(i);
				if(!active)
					sc.pos += 25;
				sc.statusId = skillId;
				sc.type = type;
				switch(type)
				{
                case e_stAtk:
					bfgt->setAttackAdd2(value);
					sc.data = static_cast<UInt16>(bfgt->getAttack());
					break;
				case e_stDef:
					bfgt->setDefendAdd2(value);
					sc.data = static_cast<UInt16>(bfgt->getDefend());
					break;
				case e_stEvade:
					bfgt->setEvadeAdd2(value);
					sc.data = static_cast<UInt16>(bfgt->getEvade(NULL));
					break;
				case e_stCritical:
					bfgt->setCriticalAdd2(value);
					sc.data = static_cast<UInt16>(bfgt->getCritical(NULL));
					break;
				case e_stPierce:
					bfgt->setPierceAdd2(value);
					sc.data = static_cast<UInt16>(bfgt->getPierce(NULL));
					break;
				case e_stCounter:
					bfgt->setCounterAdd2(value);
					sc.data = static_cast<UInt16>(bfgt->getCounter(NULL));
					break;
                case e_stAura:
                    bfgt->AddAura(value);
                    sc.data = static_cast<UInt16>(bfgt->getAura());
                    break;
                case e_stTough:
                    bfgt->setToughAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getTough(NULL));
                    break;
                case e_stMagAtk:
                    bfgt->setMagAttackAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagAttack());
                    break;
                case e_stMagDef:
                    bfgt->setMagDefendAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagDefend());
                    break;
                case e_stMagRes:
                    bfgt->setMagResAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagRes(NULL));
                    break;
                case e_stAction:
                    bfgt->setActionAdd2(value);
                    reQueueFighterStatus(bfgt);
                    sc.data = static_cast<UInt16>(bfgt->getAction());
                    break;
                    // TODO
#if 0
                case e_stCriticalDmg:
                    bfgt->setCriticalDmgAdd2(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCriticalDmg() * 100);
                    break;
#endif
                case e_stHitRate:
                    bfgt->setHitrateAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getHitrate(NULL));
                    break;
				}
				++ scCount;
			}
		}
	}
}


void BattleSimulator::setStatusChange( UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
	for(UInt8 i = pos; i < pos+cnt; ++ i)
	{
		if(_objs[side][i] != NULL)
		{
			BattleObject * bo = _objs[side][i];
			if(bo->isChar() && _isBody[side][pos] == 0 && bo->getHP() > 0)
			{
				BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
				StatusChange& sc = scList[scCount];
				sc.pos = static_cast<UInt8>(i);
				if(!active)
					sc.pos += 25;
				sc.statusId = skillId;
				sc.type = type;
				switch(type)
				{
                case e_stAtk:
					bfgt->setAttackAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getAttack());
					break;
				case e_stDef:
					bfgt->setDefendAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getDefend());
					break;
				case e_stEvade:
					bfgt->setEvadeAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getEvade(NULL));
					break;
				case e_stCritical:
					bfgt->setCriticalAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getCritical(NULL));
					break;
				case e_stPierce:
					bfgt->setPierceAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getPierce(NULL));
					break;
				case e_stCounter:
					bfgt->setCounterAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getCounter(NULL));
					break;
                case e_stAura:
                    bfgt->AddAura(value);
                    sc.data = static_cast<UInt16>(bfgt->getAura());
                    break;
                case e_stTough:
                    bfgt->setToughAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getTough(NULL));
                    break;
                case e_stMagAtk:
                    bfgt->setMagAttackAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagAttack());
                    break;
                case e_stMagDef:
                    bfgt->setMagDefendAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagDefend());
                    break;
                case e_stMagRes:
                    bfgt->setMagResAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagRes(NULL));
                    break;
                case e_stAction:
                    bfgt->setActionAdd(value, last);
                    reQueueFighterStatus(bfgt);
                    sc.data = static_cast<UInt16>(bfgt->getAction());
                    break;
                    // TODO
#if 0
                case e_stCriticalDmg:
                    bfgt->setCriticalDmgAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCriticalDmg() * 100);
                    break;
#endif
                case e_stHitRate:
                    bfgt->setHitrateAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getHitrate(NULL));
                    break;
				}
				++ scCount;
			}
		}
	}
}

void BattleSimulator::onDead(BattleObject * bo, std::vector<AttackAct>* atkAct)
{
	if(!bo->isChar())
		return;

    bool fRevival = false;
    if(false == (static_cast<BattleFighter*>(bo))->isRevival() && atkAct != NULL)
    {
        size_t idx = 0;
        const GData::SkillBase* passiveSkill = NULL;
        while(NULL != (passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead100(idx)))
        {
            AttackAct aa = {0};
            aa.bf = static_cast<BattleFighter*>(bo);
            aa.skill = passiveSkill;
            aa.target_side = bo->getSide();
            aa.target_pos = bo->getPos();
            atkAct->push_back(aa);

            fRevival = true;
        }

        passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead();
        if( NULL != passiveSkill )
        {
            AttackAct aa = {0};
            aa.bf = static_cast<BattleFighter*>(bo);
            aa.skill = passiveSkill;
            aa.target_side = bo->getSide();
            aa.target_pos = bo->getPos();
            atkAct->push_back(aa);

            fRevival = true;
        }
    }

    if(fRevival)
    {
        (static_cast<BattleFighter*>(bo))->setRevival();
        //doSkillAttackAftEnter(static_cast<BattleFighter*>(bo));
    }
    else
    {
        // remove from action queue
        BattleFighter* toremove = static_cast<BattleFighter *>(bo);
        removeFighterStatus(toremove);

        // re-test winner
        _winner = testWinner();
    }
}

void BattleSimulator::onDamage( BattleObject * bo, StatusChange * scList, size_t& scCount, bool active, std::vector<AttackAct>* atkAct)
{
#if 0
	if(bo->isChar() && bo->getHP() > 0)
	{
		BattleFighter * bf = static_cast<BattleFighter *>(bo);
		bool ref = bf->hasFlag(BattleFighter::MaxHPRef);
		if(!ref)
		{
			if(bf->getFighter()->getSkill() == 106)
			{
				Script::BattleFormula::SkillData& sd = _formula->skillData(0, 5, bf->getFighter()->getSkillLevel());
				if(_rnd(100) < sd.rate)
				{
					bf->addFlag(BattleFighter::MaxHPRef);
					ref = true;
				}
			}
		}
		if(ref)
		{
			Script::BattleFormula::SkillData& sd = _formula->skillData(0, 5, bf->getFighter()->getSkillLevel());
			setStatusChange(bf->getSide(), 0, 25, 106, 1, (sd.value1 + bf->getStrength() * sd.value2) * (bf->getMaxHP() - bf->getHP()) / bf->getMaxHP(), scList, scCount, active);
		}
	}
#endif
}

BattleFighter * BattleSimulator::getRandomFighter( UInt8 side, UInt8 * excepts, size_t exceptCount )
{
	UInt8 posList[25];
	UInt32 posSize = 0;
	for(UInt8 i = 0; i < 25; ++ i)
	{
		BattleObject * bo = _objs[side][i];
		if(bo == NULL || bo->getHP() == 0)
			continue;
		bool except = false;
		for(size_t j = 0; j < exceptCount; ++ j)
		{
			if(excepts[j] == i)
			{
				except = true;
				break;
			}
		}
		if(except)
			continue;
		posList[posSize ++] = i;
	}
	if(posSize == 0)
		return NULL;
	return static_cast<BattleFighter *>(_objs[side][posList[_rnd(posSize)]]);
}

UInt32 BattleSimulator::releaseCD(BattleFighter* bf)
{
    UInt32 rcnt = 0;
    if(NULL == bf)
    {
        return 0;
    }

    bf->releaseSkillCD(1);

    DefStatus defList[25];
    size_t defCount = 0;
    StatusChange scList[25];
    size_t scCount = 0;

    memset(defList, 0, sizeof(defList));

    UInt32 confuse = bf->getConfuseRound();
    if(confuse > 0)
    {
        -- confuse;

        if(confuse == 0)
        {
            defList[defCount].damType = e_UnConfuse;
            defList[defCount].damage = 0;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            defCount++;
        }

        bf->setConfuseRound(confuse);
    }

    UInt32 forget = bf->getForgetRound();
    if(forget > 0)
    {
        -- forget;
        if(forget == 0)
        {
            defList[defCount].damType = e_UnForget;
            defList[defCount].damage = 0;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            defCount++;
        }

        bf->setForgetRound(forget);
    }

	UInt32 stun = bf->getStunRound();
	if(stun > 0)
	{
		-- stun;
	    if(stun == 0)
        {
            defList[defCount].damType = e_UnStun;
            defList[defCount].damage = 0;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            defCount++;
        }

	    bf->setStunRound(stun);
	}


    UInt8& atkAdd_last = bf->getAttackAddLast();
    if(atkAdd_last > 0)
    {
        -- atkAdd_last;
       if(0 == atkAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stAtk, 0, 0, scList, scCount, false);
    }

    UInt8& magAtkAdd_last = bf->getMagAttackAddLast();
    if(magAtkAdd_last > 0)
    {
        -- magAtkAdd_last;
       if(0 == magAtkAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stMagAtk, 0, 0, scList, scCount, false);
    }

    UInt8& defAdd_last = bf->getDefendAddLast();
    if(defAdd_last > 0)
    {
        -- defAdd_last;
       if(0 == defAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stDef, 0, 0, scList, scCount, false);
    }

    UInt8& magDefAdd_last = bf->getMagDefendAddLast();
    if(magDefAdd_last > 0)
    {
        -- magDefAdd_last;
       if(0 == magDefAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stMagDef, 0, 0, scList, scCount, false);
    }

    UInt8& hitrateAdd_last = bf->getHitrateAddLast();
    if(hitrateAdd_last > 0)
    {
        -- hitrateAdd_last;
       if(0 == hitrateAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stHitRate, 0, 0, scList, scCount, false);
    }

    UInt8& evadeAdd_last = bf->getEvadeAddLast();
    if(evadeAdd_last > 0)
    {
        -- evadeAdd_last;
       if(0 == evadeAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stEvade, 0, 0, scList, scCount, false);
    }

    UInt8& criticalAdd_last = bf->getCriticalAddLast();
    if(criticalAdd_last > 0)
    {
        -- criticalAdd_last;
       if(0 == criticalAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stCritical, 0, 0, scList, scCount, false);
    }

    // TODO
#if 0
    UInt32 criticalDmgAdd_last = bf->getCriticalDmgAddLast();
    if(criticalDmgAdd_last > 0)
    {
        -- criticalDmgAdd_last;
       if(0 == criticalDmgAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stCriticalDmg, 0, 0, scList, scCount, false);
    }
#endif

    UInt8& pierceAdd_last = bf->getPierceAddLast();
    if(pierceAdd_last > 0)
    {
        -- pierceAdd_last;
       if(0 == pierceAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stPierce, 0, 0, scList, scCount, false);
    }

    UInt8& counterAdd_last = bf->getCounterAddLast();
    if(counterAdd_last > 0)
    {
        -- counterAdd_last;
       if(0 == counterAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stCounter, 0, 0, scList, scCount, false);
    }

    UInt8& magResAdd_last = bf->getMagResAddLast();
    if(magResAdd_last > 0)
    {
        -- magResAdd_last;
       if(0 == magResAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stMagRes, 0, 0, scList, scCount, false);
    }

    UInt8& toughAdd_last = bf->getToughAddLast();
    if(toughAdd_last > 0)
    {
        -- toughAdd_last;
       if(0 == toughAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stTough, 0, 0, scList, scCount, false);
    }

    UInt8& evad100CD = bf->getEvad100CD();
    if(evad100CD > 0)
    {
        --evad100CD;
        if(0 == evad100CD)
        {
            bf->setEvad100(true);
            bf->setEvad100CD(bf->getEvad100BaseCD());
        }
    }

    UInt8& defend100CD = bf->getDefend100CD();
    if(defend100CD > 0)
    {
        --defend100CD;
        if(0 == defend100CD)
        {
            bf->setDefend100(true);
            bf->setDefend100CD(bf->getDefend100BaseCD());
        }
    }

    // TODO
#if 0
    UInt32 maxhpAdd_last = bf->getMaxHPAddLast();
    if(maxhpAdd_last > 0)
    {
        -- maxhpAdd_last;
       if(0 == maxhpAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stMaxHP, 0, 0, scList, scCount, false);
    }
#endif

    UInt8& maxActionAdd_last = bf->getActionAddLast();
    if(maxActionAdd_last > 0)
    {
        -- maxActionAdd_last;
       if(0 == maxActionAdd_last)
            setStatusChange( bf->getSide(), bf->getPos(), 1, 0, e_stAction, 0, 0, scList, scCount, false);
    }

    if(defCount > 0 || scCount > 0)
    {
        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 5, 0, false, false, defList, defCount, scList, scCount);
        ++ rcnt;
    }

    return rcnt;
}

}
