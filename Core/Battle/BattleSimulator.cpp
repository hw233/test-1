#include "Config.h"
#include "BattleSimulator.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "Script/BattleFormula.h"
#include "BattleReport.h"
#include "Server/OidGenerator.h"

namespace Battle
{

static UInt8 getPosition(UInt16 loc)
{
	if(loc == 0x6FFF)
		return 6;
	if(loc == 0x7FFF)
		return 7;
	if(loc == 0x8FFF)
		return 8;
	loc >>= 12;
	if(loc > 6)
		return 3;
	if(loc > 5)
		return 1;
	return static_cast<UInt8>(loc);
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
	_packet.init(0x6C);
	_packet << _id << _position;
	for(int i = 0; i < 2; ++ i)
		_packet << (_formation[i] ? _formation[i]->getId() : static_cast<UInt8>(0));
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
					}
					UInt32 maxhp = bf->getMaxHP();
					_packet << static_cast<UInt8>(j + 1) << bf->getFighter()->getBattleName();
					if(bf->getFighter()->isNpc())
						_packet << static_cast<UInt8>(bf->getFighter()->reqFriendliness);
					else
						_packet << bf->getFighter()->getClassAndSex();
					_packet << static_cast<UInt8>(_isBody[i][j] > 0 ? (_isBody[i][j] - 1) : bf->getFighter()->getLevel()) << bf->getPortrait() << static_cast<UInt8>(bf->getFlag() & 0x03);
					_packet << static_cast<UInt8>((bf->getFighter()->getColor() << 4) | (bf->getFighter()->getWeapon() ? /*TODO: no weapon_def static_cast<UInt8>(bf->getFighter()->getWeapon()->getWeaponDef().getId() & 0x0F)*/ 0 : static_cast<UInt8>(0)))
						<< static_cast<UInt32>(_isBody[i][j] ? 0 : bf->getHP()) << static_cast<UInt32>(maxhp);

					_packet << static_cast<UInt16>(bf->getAttack()) << static_cast<UInt16>(bf->getDefend()) << static_cast<UInt16>(bf->getAction());
                    // TODO: up skills
					_packet << static_cast<UInt16>(bf->getHitrate() * 100.0f) << static_cast<UInt16>(bf->getEvade() * 100.0f) << static_cast<UInt16>(bf->getCritical() * 100.0f) << static_cast<UInt16>(bf->getPierce() * 100.0f) << static_cast<UInt16>(bf->getCounter() * 100.0f);
                    bf->getFighter()->getAllUpSkillAndLevel(_packet);
                    
					if(ismain)
					{
						bf->postInit();
						FighterStatus fs(bf);
						// Insert into action queue
						insertFighterStatus(fs);
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
	while(_winner == 0 && act_count < _fake_turns)
	{
		int pos = findFirstAttacker();

		act_count += doAttack(pos);
	}
	_packet << static_cast<UInt8>(_winner);
	_packet.data<UInt32>(cnt_pos) = act_count;
	_packet << Stream::eos;
//	printf("Winner is: %d,  actions: %d\n", _winner, act_count);

	_turns = act_count;

	if(act_count == 0)
		_winner = 1;

	if(_report)
		battleReport.addReport(_id, _packet);
}

void BattleSimulator::insertFighterStatus( FighterStatus& fs )
{
    Int8 next_fgtlist_idx = _cur_fgtlist_idx == 0 ? 1 : 0;
    std::vector<FighterStatus>& next_fgtlist = _fgtlist[next_fgtlist_idx];
	int cnt = static_cast<int>(next_fgtlist.size());
	for(int i = 0; i < cnt ; ++ i)
	{
		if(next_fgtlist[i].action < fs.action)
		{
			next_fgtlist.insert(next_fgtlist.begin() + i, fs);
			return;
		}
	}
	next_fgtlist.insert(next_fgtlist.end(), fs);
}

void BattleSimulator::removeFighterStatus( FighterStatus& fs )
{
    for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<FighterStatus>& cur_fgtlist = _fgtlist[fgtlist_idx];
        size_t c = cur_fgtlist.size();
        size_t i = 0;
        while(i < c)
        {
            if(cur_fgtlist[i] == fs)
            {
                cur_fgtlist.erase(cur_fgtlist.begin() + i);
                -- c;
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
        std::vector<FighterStatus>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
        size_t cnt = cur_fgtlist.size();

        for(size_t idx = 0; idx < cnt; idx++)
        {
            BattleFighter* bf = cur_fgtlist[idx].bfgt;
            bf->releaseSkillCD(1);
        }
    }

    std::vector<FighterStatus>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
	size_t c = 1, cnt = cur_fgtlist.size();
    UInt32 act = cur_fgtlist[0].action;
	while(c < cnt && cur_fgtlist[c].action == act)
		++ c;
	if(c == 1)
		return 0;
	return _rnd(c);
}

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

UInt32 BattleSimulator::attackOnce(BattleFighter * bf, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * area_target_obj, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, int counter_deny, AttackPoint * counter_deny_list)
{
	if(area_target_obj == NULL || area_target_obj->getHP() == 0)
		return 0;
	// if a fighter was attacked
	UInt32 dmg = 0;
	if(area_target_obj->isChar())
	{
        bool counter100 = false;
		BattleFighter * area_target = static_cast<BattleFighter *>(area_target_obj);
		UInt8 side = area_target->getSide();
		UInt8 pos = area_target->getPos();
        UInt8 target_stun = area_target->getStunRound();
		if(target_stun > 0 || bf->calcHit(area_target))
		{
            pr = bf->calcPierce();
            float atk;
            if(NULL != skill)
            {
                atk = bf->calcMagAttack(cs) * skill->effect->magdamP + skill->effect->addmag + bf->calcAttack(cs) * skill->effect->damageP + skill->effect->adddam;
            }
            else
            {
                atk = bf->calcAttack(cs);
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
                if(NULL != skill)
                {
                    def = area_target->getMagDefend();
                }
                else
                {
                    def = area_target->getDefend();
                }
				dmg = (pr ? static_cast<UInt32>(factor * atk) : _formula->calcDamage(factor * atk, def)) * (950 + _rnd(100)) / 1000;
			}
			area_target->makeDamage(dmg);

			defList[defCount].damType = 0;
			defList[defCount].damage = dmg;
			defList[defCount].leftHP = area_target->getHP();
//			printf("%u:%u %s %u:%u, made %u damage, hp left: %u\n", 1-side, from_pos, cs ? "CRITICALs" : "hits", side, pos, dmg, area_target->getHP());
			// killed the target fighter
            bool rState = false;
			if(area_target->getHP() == 0)
				onDead(area_target);
			else if(_winner == 0)
            {
				onDamage(area_target, scList, scCount, true);

                if(skill->effect->state > 0)
                {
                    float rate = skill->prob * 100;
                    if(rate < _rnd(10000))
                    {
                        doSkillState(bf, skill, area_target, defList, defCount, rState);
                    }
                }

                if(counter_deny >= 0)
                {
                    setStatusChange( bf->getSide(), bf->getPos(), 1, 9, e_stAura, 25, 0, scList, scCount, true);
                    setStatusChange( area_target->getSide(), area_target->getPos(), 1, 9, e_stAura, 25, 0, scList, scCount, false);
                }
            }
            defList[defCount].pos = pos;
            ++ defCount;

            if(rState)
            {
                switch(skill->effect->state)
                {
                case 1:
                    defList[defCount].damType = e_ImPoison;
                case 2:
                    defList[defCount].damType = e_ImConfuse;
                case 4:
                    defList[defCount].damType = e_ImStun;
                case 8:
                    defList[defCount].damType = e_ImForget;
                }
                defList[defCount].damage = 0;
                defList[defCount].leftHP = area_target->getHP();
                defList[defCount].pos = bf->getPos() + 25;
                ++ defCount;
            }
		}
		else
		{
            const GData::SkillBase* target_skill = NULL;
            size_t idx = 0;
            while(NULL != (target_skill = area_target->getPassiveSkillAftEvd100(idx)))
            {
                if(target_skill->effect->state == 16)
                    counter100 = true;
            } 
            target_skill = area_target->getPassiveSkillAftEvd();
            if(NULL != target_skill)
            {
                if(target_skill->effect->state == 16)
                    counter100 = true;
            }

			defList[defCount].damType = 2;
			defList[defCount].damage = 0;
			defList[defCount].leftHP = area_target->getHP();
//			printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
            defList[defCount].pos = pos;
            ++ defCount;
		}

        UInt32 rhp = 0;
        if((skill->effect->absorb || skill->effect->absorbP))
        {
            rhp = dmg * skill->effect->absorbP + skill->effect->absorb;
        }
 
        if(rhp > 0)
        {
            bf->regenHP(rhp);
            defList[defCount].damType = 8;
            defList[defCount].damage = rhp;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            ++ defCount;
        }

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
			if(counter100 || target_fighter->calcCounter(!bf->canBeCounter()))
			{
				if(target_fighter->calcHit(bf))
				{
                    defList[0].damType |= 0x80;
					bool cs = false;
					float atk = target_fighter->getAttack();
					float def = bf->getDefend();
					bool pr = target_fighter->calcPierce();
					UInt32 dmg2 = (pr ? static_cast<UInt32>(atk) : _formula->calcDamage(atk, def)) * (950 + _rnd(100)) / 1000;

					bf->makeDamage(dmg2);

					if(cs)
						defList[0].damType |= 0x40;
					if(pr)
						defList[0].damType |= 0x20;
					defList[0].counterDmg = dmg2;
					defList[0].counterLeft = bf->getHP();

//					if(cs)
//						printf("  [Counter] %u:%u CRITICAL-STRIKEs %u:%u, made %u damage, hp left: %u\n", side, pos, 1-side, from_pos, dmg2, bf->getHP());
//					else
//						printf("  [Counter] %u:%u attacks %u:%u, made %u damage, hp left: %u\n", side, pos, 1-side, from_pos, dmg2, bf->getHP());

					// killed the fighter
					if(bf->getHP() == 0)
						onDead(bf);
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
        float atk = bf->calcAttack(cs);
		dmg = static_cast<int>(factor * atk) * (950 + _rnd(100)) / 1000;
		area_target_obj->makeDamage(dmg);
		defList[defCount].pos = area_target_obj->getPos();
		defList[defCount].damType = 0;
		defList[defCount].damage = dmg;
		defList[defCount].leftHP = area_target_obj->getHP();
//		printf("%u:%u %s ground object, made %u damage, hp left: %u\n", 1-side, from_pos, cs ? "CRITICALs" : "hits", dmg, area_target_obj->getHP());
		++ defCount;
	}
	return dmg;
}

void BattleSimulator::doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, DefStatus* defList, size_t& defCount, bool& rState)
{
    if(skill->effect->state == 0)
    {
        return;
    }

    BattleFighter* target_bo = static_cast<BattleFighter*>(bo);
    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    UInt8 immune = 0;
    while(NULL != (passiveSkill = target_bo->getPassiveSkillBeAtk100(idx)))
    {
        if(passiveSkill->effect->immune)
        {
            immune |= passiveSkill->effect->immune;
        }
    }

    passiveSkill = target_bo->getPassiveSkillBeAtk();
    if(NULL != passiveSkill && passiveSkill->effect->immune)
    {
        immune |= passiveSkill->effect->immune;
    }

    if(skill->effect->state & immune && SKILL_LEVEL(skill->getId()) <= SKILL_LEVEL(passiveSkill->getId()))
    {
        switch(skill->effect->state)
        {
        case 1:
            defList[defCount].damType = e_ImPoison;
            return;
        case 2:
            defList[defCount].damType = e_ImConfuse;
            return;
        case 4:
            defList[defCount].damType = e_ImStun;
            return;
        case 8:
            defList[defCount].damType = e_ImForget;
            return;
        }
    }

    rState = false;
    bool magRes = false;
    float rate = target_bo->getMagRes() * 100;
    if(rate > _rnd(10000))
    {
        defList[defCount].damType = e_Res;
        magRes = true;
        idx = 0;
        while(NULL != (passiveSkill = target_bo->getPassiveSkillAftRes100(idx)))
        {
            defList[defCount].damType = e_ResR;
            rState = true;
        }

        passiveSkill = target_bo->getPassiveSkillAftRes();
        if(NULL != passiveSkill)
        {
            defList[defCount].damType = e_ResR;
            rState = true;
        }

        return;
    }

    switch(skill->effect->state)
    {
    case 1:
        if(target_bo->getPoisonRound() < 1)
        {
            defList[defCount].damType = e_Poison;
            target_bo->setPoisonLevel(SKILL_LEVEL(skill->getId()));
            target_bo->setPoisonRound(skill->last);
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

UInt32 BattleSimulator::doNormalAttack(BattleFighter* bf, int otherside, int target_pos)
{
    BattleObject* target_object = _objs[otherside][target_pos];
    if(target_object!= NULL)
	{
		// find all targets that are hit
		GData::Area * area = NULL;
		GObject::ItemWeapon * weapon = bf->getFighter()->getWeapon();
		if(weapon != NULL)
		{
            // TODO: no weapon_def
			// area = &weapon->getWeaponDef().getArea();
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
		bool pr = false;

		float factor = (*area)[0].factor;

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
			dmg += attackOnce(bf, cs, pr, NULL, target_object, factor, defList, defCount, scList, scCount, apcnt, ap);
			for(int i = 0; i < apcnt; ++ i)
			{
				switch(ap[i].type)
				{
				case 0:
					factor = ap[i].factor;
					dmg += attackOnce(bf, cs, pr, NULL, _objs[otherside][ap[i].pos], factor, defList, defCount, scList, scCount);
					break;
				case 1:
					factor = (*area)[0].factor;
				case 2:
					{
						float newfactor = factor * ap[i].factor;
						if(dmg += attackOnce(bf, cs, pr, NULL,  _objs[otherside][ap[i].pos], newfactor, defList, defCount, scList, scCount))
							factor = newfactor;
					}
					break;
				}
			}
		}
		else
		{
			// attack only one target
			dmg += attackOnce(bf, cs, pr, NULL, target_object, factor, defList, defCount, scList, scCount, 0);
		}

        appendToPacket(bf->getSide(), bf->getPos(), target_pos, static_cast<UInt8>(0), static_cast<UInt16>(0), cs, pr, defList, defCount, scList, scCount);
        return dmg;
    }

    return 0;
}


UInt32 BattleSimulator::doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, BattleFighter* therapy_bf)
{
    UInt8 skill_target = skill->target;
    UInt8 target_side = skill_target ? bf->getSide() : 1 - bf->getSide();
    UInt32 dmg = 0;

    // therapy skill
    if(skill->effect->hp || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
    {
        UInt32 rhp = bf->calcTherapy(skill);
        DefStatus defList[25];
        size_t defCount = 0;
        if(1 == skill->area)
        {
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                if(bo->getLostHP() == 0)
                    continue;

                UInt32 hpr = bo->regenHP(rhp);
                if(hpr == 0)
                    continue;

                defList[defCount].pos = pos + 25;
                defList[defCount].damType = 3;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
            }
        }
        else if(NULL != therapy_bf)
        {
            UInt32 hpr = therapy_bf->regenHP(rhp);
            if(hpr != 0)
            {
                defList[defCount].pos = therapy_bf->getPos() + 25;
                defList[defCount].damType = 3;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = therapy_bf->getHP();
                ++ defCount;
            }
        }
        else
        {
            UInt8 pos= getPossibleTarget(bf->getSide(), bf->getPos());
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
            UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp);
            if(hpr != 0)
            {
                defList[defCount].pos = bf->getPos() + 25;
                defList[defCount].damType = 3;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
            }
        }

        appendToPacket( bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false, defList, defCount, NULL, 0);

        return 0;
    }

    if(skill->effect->damage || skill->effect->damageP || skill->effect->adddam)
    {
        UInt8 target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

        if(target_pos < 0)
            return 0;

        DefStatus defList[25];
        size_t defCount = 0;
        StatusChange scList[50];
        size_t scCount = 0;
		bool cs = false;
		bool pr = false;

        if(0 == skill->area)
        {
            dmg = attackOnce(bf, cs, pr, skill, _objs[target_side][target_pos], 1, defList, defCount, scList, scCount, 0);
        }
        else if(1 == skill->area)
        {
            for(UInt8 pos = 0; pos < 25; pos++)
            {
                if(_objs[target_side][pos] == NULL || _objs[target_side][pos]->getHP() == 0)
                    continue;
                dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][pos], 1, defList, defCount, scList, scCount);
            }
        }
        else
        {
            GData::Area* area = NULL;
            area = &(GData::areaList[skill->area]);

            int cnt = area->getCount();
            if(cnt < 1)
                return 0;

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
                ap[apcnt ++].factor = skill->factor[std::max(abs(ad.y), abs(ad.x))]; 
            }
            // attack the target on center
            dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][target_pos], skill->factor[0], defList, defCount, scList, scCount, apcnt, ap);
            for(int i = 0; i < apcnt; ++ i)
            {
                dmg += attackOnce(bf, cs, pr, skill, _objs[target_side][ap[i].pos], ap[i].factor, defList, defCount, scList, scCount);
            }
        }

        appendToPacket( bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false, defList, defCount, scList, scCount);

        return dmg;
    }

    StatusChange scList[50];
    size_t scCount = 0;
	
    doSkillStatus(bf, skill, scList, scCount);
    if(scCount < 1)
        return 0;

    appendToPacket( bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false, NULL, 0, scList, scCount);
    return 0;
}

void BattleSimulator::doSkillStatus(BattleFighter* bf, const GData::SkillBase* skill, StatusChange* scList, size_t& scCount)
{
    BattleFighter* bo = NULL;
    int cnt = 0;
    UInt8 target_side = skill->target ? bf->getSide() : 1 - bf->getSide();

    if(0 == skill->target && 0 == skill->area)
    {
        UInt8 myPos = bf->getPos();
        bo = getRandomFighter(bf->getSide(), &myPos, 1);
        cnt = 1;
    }
    else if(2 == skill->target)
    {
        bo = bf;
        cnt = 1;
    }
    else if(0 == skill->area)
    {
        UInt8 target_pos = getPossibleTarget(bf->getSide(), bf->getPos());
        bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
        cnt = 25;
    }

    if(skill->effect->auraP || skill->effect->aura)
    {
        float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAura, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtk, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stDef, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stTough, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->action)
    {
        float value = skill->effect->action;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAction, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stEvade, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCritical, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stPierce, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCounter, value, skill->last, scList, scCount, bf->getSide() == target_side);
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        setStatusChange( target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, bf->getSide() == target_side);
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

// single attack round
UInt32 BattleSimulator::doAttack( int pos )
{
	UInt32 rcnt = 0;
    std::vector<FighterStatus>& cur_fgtlist = _fgtlist[_cur_fgtlist_idx];
	FighterStatus fs = cur_fgtlist[pos];
	BattleFighter * bf = fs.bfgt;

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

            UInt32 fdmg = fs.poisonDamage;
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
            if(bf->getHP() == 0)
            {
                onDead(bf);
                return 1;
            }
            else if(_winner == 0)
                onDamage(bf, scList, scCount, false);
            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
            rcnt++;
        }
    }
    //fs.resetAction();
    insertFighterStatus(fs);

	UInt32 stun = bf->getStunRound();
    UInt32 confuse = bf->getConfuseRound();
    UInt32 forget = bf->getForgetRound();
    if(confuse > 0)
    {
        -- confuse;

        if(confuse == 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;

            defList[defCount].damType = e_UnConfuse;
            defList[defCount].damage = 0;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            defCount++;

            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
        }

        bf->setConfuseRound(confuse);
    }
    if(forget > 0)
    {
        -- forget;
        if(forget == 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;

            defList[defCount].damType = e_UnForget;
            defList[defCount].damage = 0;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            defCount++;

            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
        }

        bf->setForgetRound(forget);
    }
	if(stun > 0)
	{
		-- stun;
	    if(stun == 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;

            defList[defCount].damType = e_UnStun;
            defList[defCount].damage = 0;
            defList[defCount].pos = bf->getPos() + 25;
            defList[defCount].leftHP = bf->getHP();
            defCount++;

            appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
        }

	    bf->setStunRound(stun);
	}

    if(stun > 0)
    {
        return 0;
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
            return 0;

        dmg += doNormalAttack(bf, otherside, target_pos);
        rcnt++;
    }
    else if(forget > 0)
    {
        target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

        if(target_pos < 0)
            return 0;

       dmg += doNormalAttack(bf, otherside, target_pos);
       rcnt++;
    }
    else
    {
        const GData::SkillBase* skill = NULL;
        // do preve attack passive skill that must act
        size_t skillIdx = 0;
        while(NULL != (skill = bf->getPassiveSkillPrvAtk100(skillIdx)))
        {
            dmg += doSkillAttack(bf, skill);
            rcnt++;
        }

        skill = bf->getPassiveSkillPreAtk();
        if(NULL != skill)
        {
            dmg += doSkillAttack(bf, skill);
            rcnt ++;
        }

        // do active skill
        BattleFighter* therapy_bf = getTherapyTarget(bf);
        skill = bf->getActiveSkill(therapy_bf!= NULL);
        if(NULL != skill)
        {
            dmg += doSkillAttack(bf, skill, therapy_bf);
            rcnt ++;
        }
        else
        {
            target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

            if(target_pos < 0)
                return 0;

           dmg += doNormalAttack(bf, otherside, target_pos);
           rcnt ++;
        }
    }

    if(dmg > 0 && otherside != bf->getSide())
    {
        BattleFighter* bo = static_cast<BattleFighter*>(_objs[otherside][target_pos]);
        if(bo->getHP() > 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;
            StatusChange scList[50];
            size_t scCount = 0;

            size_t idx = 0;
            const GData::SkillBase* passiveSkill = NULL;
            UInt32 fdmg = 0;
            while(NULL != (passiveSkill = bo->getPassiveSkillBeAtk100(idx)))
            {
                if(passiveSkill->effect->thorn || passiveSkill->effect->thornP)
                {
                    fdmg = dmg * passiveSkill->effect->thornP + passiveSkill->effect->thorn;
                    break;
                }
            }

            passiveSkill = bo->getPassiveSkillBeAtk();
            if(NULL != passiveSkill)
            {
                if(passiveSkill->effect->thorn || passiveSkill->effect->thornP)
                {
                    fdmg = dmg * passiveSkill->effect->thornP + passiveSkill->effect->thorn;
                }
            }

            if(fdmg > 0)
            {
                defList[defCount].damType = 5;
                defList[defCount].damage = fdmg;
                defList[defCount].pos = bf->getPos() + 25;
                bf->makeDamage(fdmg);
                defList[defCount].leftHP = bf->getHP();
                ++ defCount;
         
                if(bf->getHP() == 0)
                {
                    onDead(bf);
                }
                else if(_winner == 0)
                    onDamage(bf, scList, scCount, false);
                appendToPacket(bo->getSide(), bo->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
                rcnt++;
            }

        }

        if(bf->getHP() > 0)
        {
            DefStatus defList[25];
            size_t defCount = 0;

            size_t idx = 0;
            const GData::SkillBase* passiveSkill = NULL;
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[otherside][target_pos]);
            while(NULL != (passiveSkill = bf->getPassiveSkillAftAtk100(idx)))
            {
                defList[defCount].damage = 0;
                defList[defCount].pos = bf->getPos() + 25;
                defList[defCount].leftHP = bf->getHP();
                bool rState;
                doSkillState(bf, passiveSkill, bo, defList, defCount, rState);
                defCount ++;
                appendToPacket(bo->getSide(), bo->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
                rcnt++;
            }

            passiveSkill = bf->getPassiveSkillAftAtk();
            if(NULL != passiveSkill)
            {
                defList[defCount].damage = 0;
                defList[defCount].pos = bf->getPos() + 25;
                defList[defCount].leftHP = bf->getHP();
                bool rState;
                doSkillState(bf, passiveSkill, bo, defList, defCount, rState);
                defCount ++;
                appendToPacket(bo->getSide(), bo->getPos(), bf->getPos() + 25, 0, 0, false, false, defList, defCount, NULL, 0);
                rcnt++;
            }
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
				defList[defCount].damType = 3;
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
			        defList[i].damType = 0;
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
				defList[i].damType = 0;
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
        std::vector<FighterStatus>& fgtlist = _fgtlist[fgtlist_idx];
        size_t c = fgtlist.size();
        for(size_t i = 0; i < c; ++ i)
        {
            alive[fgtlist[i].bfgt->getSide()] ++;
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
		_packet << defList[i].pos << defList[i].damType << defList[i].damage << defList[i].leftHP;
		if((defList[i].damType & 0x07) == 5 || (defList[i].damType & 0x80) == 0x80)
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
	for(UInt8 i = pos; i < cnt; ++ i)
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
					sc.data = static_cast<UInt16>(bfgt->getEvade());
					break;
				case e_stCritical:
					bfgt->setCriticalAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getCritical());
					break;
				case e_stPierce:
					bfgt->setPierceAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getPierce());
					break;
				case e_stCounter:
					bfgt->setCounterAdd(value, last);
					sc.data = static_cast<UInt16>(bfgt->getCounter());
					break;
                case e_stAura:
                    bfgt->AddAura(value);
                    sc.data = static_cast<UInt16>(bfgt->getAura());
                    break;
                case e_stTough:
                    bfgt->setToughAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getTough());
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
                    sc.data = static_cast<UInt16>(bfgt->getMagRes());
                    break;
                case e_stAction:
                    bfgt->setActionAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getAction());
                    break;
				}
				++ scCount;
			}
		}
	}
}

void BattleSimulator::onDead(BattleObject * bo)
{
	if(!bo->isChar())
		return;

	// remove from action queue
	FighterStatus toremove;
	toremove.bfgt = static_cast<BattleFighter *>(bo);
	removeFighterStatus(toremove);

	// re-test winner
	_winner = testWinner();
}

void BattleSimulator::onDamage( BattleObject * bo, StatusChange * scList, size_t& scCount, bool active )
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

}
