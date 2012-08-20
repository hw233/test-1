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
#include "MsgHandler/CountryMsgStruct.h"
#include "Script/GameActionLua.h"
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

    for(int i = 0; i < 2; ++i)
    {
        _teams[i] = 0;
    }
    InitAttainRecord();

    {
        statusFuncTable[e_stAura] = &BattleSimulator::setStatusChange_Aura;
        statusFuncTable2[e_stAura] = &BattleSimulator::setStatusChange2_Aura;
        statusFuncTable[e_stAtk] = &BattleSimulator::setStatusChange_Atk;
        statusFuncTable2[e_stAtk] = &BattleSimulator::setStatusChange2_Atk;
        statusFuncTable[e_stDef] = &BattleSimulator::setStatusChange_Def;
        statusFuncTable2[e_stDef] = &BattleSimulator::setStatusChange2_Def;
        statusFuncTable[e_stMagAtk] = &BattleSimulator::setStatusChange_MagAtk;
        statusFuncTable2[e_stMagAtk] = &BattleSimulator::setStatusChange2_MagAtk;
        statusFuncTable[e_stMagDef] = &BattleSimulator::setStatusChange_MagDef;
        statusFuncTable2[e_stMagDef] = &BattleSimulator::setStatusChange2_MagDef;
        statusFuncTable[e_stTough] = &BattleSimulator::setStatusChange_Tough;
        statusFuncTable2[e_stTough] = &BattleSimulator::setStatusChange2_Tough;
        statusFuncTable[e_stAction] = &BattleSimulator::setStatusChange_Action;
        statusFuncTable2[e_stAction] = &BattleSimulator::setStatusChange2_Action;
        statusFuncTable[e_stEvade] = &BattleSimulator::setStatusChange_Evade;
        statusFuncTable2[e_stEvade] = &BattleSimulator::setStatusChange2_Evade;
        statusFuncTable[e_stCritical] = &BattleSimulator::setStatusChange_Critical;
        statusFuncTable2[e_stCritical] = &BattleSimulator::setStatusChange2_Critical;
        statusFuncTable[e_stPierce] = &BattleSimulator::setStatusChange_Pierce;
        statusFuncTable2[e_stPierce] = &BattleSimulator::setStatusChange2_Pierce;
        statusFuncTable[e_stCounter] = &BattleSimulator::setStatusChange_Counter;
        statusFuncTable2[e_stCounter] = &BattleSimulator::setStatusChange2_Counter;
        statusFuncTable[e_stMagRes] = &BattleSimulator::setStatusChange_MagRes;
        statusFuncTable2[e_stMagRes] = &BattleSimulator::setStatusChange2_MagRes;
        statusFuncTable[e_stCriticalDmg] = &BattleSimulator::setStatusChange_CriDmg;
        statusFuncTable2[e_stCriticalDmg] = &BattleSimulator::setStatusChange2_CriDmg;
        statusFuncTable[e_stHitRate] = &BattleSimulator::setStatusChange_HitR;
        statusFuncTable2[e_stHitRate] = &BattleSimulator::setStatusChange2_HitR;
        statusFuncTable[e_stAtkReduce] = &BattleSimulator::setStatusChange_AtkReduce;
        statusFuncTable2[e_stAtkReduce] = &BattleSimulator::setStatusChange2_AtkReduce;
        statusFuncTable[e_stMagAtkReduce] = &BattleSimulator::setStatusChange_MagAtkReduce;
        statusFuncTable2[e_stMagAtkReduce] = &BattleSimulator::setStatusChange2_MagAtkReduce;
    }

    {
        for(int i = 0; i < GData::TYPE_MAX; ++ i)
        {
            skillStrengthenTable[i] = NULL;
        }
        skillStrengthenTable[GData::TYPE_DISPERSE] = &BattleSimulator::doSkillStrengthen_disperse;
        skillStrengthenTable[GData::TYPE_DAMAG_REDUCE] = &BattleSimulator::doSkillStrengthen_reduce;
        skillStrengthenTable[GData::TYPE_DEBUF_DEFEND] = &BattleSimulator::doSkillStrengthen_debuf_defend;
        skillStrengthenTable[GData::TYPE_ATKADD] = &BattleSimulator::doSkillStrengthen_atkadd;
    }
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

    for(int i = 0; i < 2; ++i)
    {
        _teams[i] = 0;
    }
    InitAttainRecord();

    {
        statusFuncTable[e_stAura] = &BattleSimulator::setStatusChange_Aura;
        statusFuncTable2[e_stAura] = &BattleSimulator::setStatusChange2_Aura;
        statusFuncTable[e_stAtk] = &BattleSimulator::setStatusChange_Atk;
        statusFuncTable2[e_stAtk] = &BattleSimulator::setStatusChange2_Atk;
        statusFuncTable[e_stDef] = &BattleSimulator::setStatusChange_Def;
        statusFuncTable2[e_stDef] = &BattleSimulator::setStatusChange2_Def;
        statusFuncTable[e_stMagAtk] = &BattleSimulator::setStatusChange_MagAtk;
        statusFuncTable2[e_stMagAtk] = &BattleSimulator::setStatusChange2_MagAtk;
        statusFuncTable[e_stMagDef] = &BattleSimulator::setStatusChange_MagDef;
        statusFuncTable2[e_stMagDef] = &BattleSimulator::setStatusChange2_MagDef;
        statusFuncTable[e_stTough] = &BattleSimulator::setStatusChange_Tough;
        statusFuncTable2[e_stTough] = &BattleSimulator::setStatusChange2_Tough;
        statusFuncTable[e_stAction] = &BattleSimulator::setStatusChange_Action;
        statusFuncTable2[e_stAction] = &BattleSimulator::setStatusChange2_Action;
        statusFuncTable[e_stEvade] = &BattleSimulator::setStatusChange_Evade;
        statusFuncTable2[e_stEvade] = &BattleSimulator::setStatusChange2_Evade;
        statusFuncTable[e_stCritical] = &BattleSimulator::setStatusChange_Critical;
        statusFuncTable2[e_stCritical] = &BattleSimulator::setStatusChange2_Critical;
        statusFuncTable[e_stPierce] = &BattleSimulator::setStatusChange_Pierce;
        statusFuncTable2[e_stPierce] = &BattleSimulator::setStatusChange2_Pierce;
        statusFuncTable[e_stCounter] = &BattleSimulator::setStatusChange_Counter;
        statusFuncTable2[e_stCounter] = &BattleSimulator::setStatusChange2_Counter;
        statusFuncTable[e_stMagRes] = &BattleSimulator::setStatusChange_MagRes;
        statusFuncTable2[e_stMagRes] = &BattleSimulator::setStatusChange2_MagRes;
        statusFuncTable[e_stCriticalDmg] = &BattleSimulator::setStatusChange_CriDmg;
        statusFuncTable2[e_stCriticalDmg] = &BattleSimulator::setStatusChange2_CriDmg;
        statusFuncTable[e_stHitRate] = &BattleSimulator::setStatusChange_HitR;
        statusFuncTable2[e_stHitRate] = &BattleSimulator::setStatusChange2_HitR;
        statusFuncTable[e_stAtkReduce] = &BattleSimulator::setStatusChange_AtkReduce;
        statusFuncTable2[e_stAtkReduce] = &BattleSimulator::setStatusChange2_AtkReduce;
        statusFuncTable[e_stMagAtkReduce] = &BattleSimulator::setStatusChange_MagAtkReduce;
        statusFuncTable2[e_stMagAtkReduce] = &BattleSimulator::setStatusChange2_MagAtkReduce;
    }

    {
        for(int i = 0; i < GData::TYPE_MAX; ++ i)
        {
            skillStrengthenTable[i] = NULL;
        }
        skillStrengthenTable[GData::TYPE_DISPERSE] = &BattleSimulator::doSkillStrengthen_disperse;
        skillStrengthenTable[GData::TYPE_DAMAG_REDUCE] = &BattleSimulator::doSkillStrengthen_reduce;
        skillStrengthenTable[GData::TYPE_DEBUF_DEFEND] = &BattleSimulator::doSkillStrengthen_debuf_defend;
        skillStrengthenTable[GData::TYPE_ATKADD] = &BattleSimulator::doSkillStrengthen_atkadd;
    }
}

void BattleSimulator::switchPlayer(GObject::Player* player, UInt8 side)
{
    if(side > 1)
        return;

	_player[side] = player;
}

void BattleSimulator::switchPlayer(const std::string& name, UInt8 level)
{
	_other_name = name;
	_other_level = level;
}

void BattleSimulator::putTeams(const std::string& name, UInt8 level, UInt16 portrait, UInt8 side)
{
    if(side > 1)
        return;
    UInt8 idx = _teams[side];
    if(idx > 1)
        return;
    ++_teams[side];
    _team_name[side].push_back(name);
    _team_level[side].push_back(level);
    _team_portrait[side].push_back(portrait);
}

UInt32 BattleSimulator::clearLastBattle(UInt8 side, bool isLast)
{
    if(side > 1)
        return 0;

    if(_id == 0)
    {
        _id = IDGenerator::gBattleOidGenerator.ID();
        _packet.data<UInt32>(4) = _id;
    }

    UInt32 oldID = _id; 
    if(!isLast)
    {
        _id = IDGenerator::gBattleOidGenerator.ID();
        _packet.data<UInt32>(8) = _id;
    }

    battleReport.addReport(oldID, _packet);

    if(!isLast)
    {
        for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
        {
            std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
            size_t c = fgtlist.size();
            for(size_t i = 0; i < c;)
            {
                if(fgtlist[i]->getSide() == side)
                {
                    fgtlist.erase(fgtlist.begin() + i);
                    -- c;
                }
                else
                    ++ i;
            }
        }

        for(int i = 0; i < 25; ++ i)
        {
            if(_objs[side][i] && !_isBody[side][i])
            {
                delete _objs[side][i];
                _objs[side][i] = NULL;
            }
        }
    }

    _teams[side] = 0;
    _team_name[side].clear();
    _team_level[side].clear();
    _team_portrait[side].clear();

    return oldID;
}

void  BattleSimulator::InitAttainRecord()
{
    for (UInt8 i = 0 ; i< 2; i++)
    {
        _evadeNum[i] = 0;
        _csNum[i] = 0;
        _prNum[i] = 0;
        _fjNum[i] = 0;

        _maxEvade [i] = 0;

         _maxCS[i] = 0;

         _maxPR[i] = 0;
         _maxFJ[i] = 0;
         _maxSkillDmg[i] = 0;
         _maxPeerLessDmg[i] = 0;
         _maxAura[i] = 0;
         _maxCSFactor[i] = 0.0f;

         _attackRound = 0;
         _firstPLDmg[0] = false;
       /* _skillDmg300[i] = false;
        _skillDmg1k[i] = false;
        _skillDmg5k[i] = false;
        _peerlessDmg1k[i] = false;
        _peerlessDmg5k[i] = false;
        _peerlessDmg1w[i] = false;*/
    }
}
// #############################
// change the battle queue but not pass client check
// prevWin for 组队
void BattleSimulator::start(UInt8 prevWin)
{
    if(prevWin != 0xFF && (prevWin == 0 || prevWin > 2))
        return;
	_packet.clear();
    if(prevWin == 0xFF)
    {
        _fgtlist[0].clear();
        _fgtlist[1].clear();
    }

	// [[ Make packet header data
	_packet.init(REP::FIGHT_START);
	_packet << _id << static_cast<UInt32>(0) << _position;
	for(int i = 0; i < 2; ++ i)
		_packet << (_formation[i] ? _formation[i]->getId() : static_cast<UInt16>(0));
	_packet << _player[0]->GetLev() << _other_level;
	_packet << _portrait[0] << _portrait[1];

	_packet << _player[0]->getName() << _other_name;

    //组队
    for(int tidx = 0; tidx < 2; ++tidx)
    {
        _packet << _teams[tidx];
        for(int idx = 0; idx < _teams[tidx]; ++idx)
        {
            _packet << _team_name[tidx][idx] << _team_level[tidx][idx] << _team_portrait[tidx][idx];
        }
    }

	size_t offset = _packet.size();

	_packet << static_cast<UInt16>(0);

    // for the insertFighterStatus can only insert to next fgtlist, so to make sure to insert fighter status to _figtlist[0] and set the _cur_fgtlist to 1
    UInt8 cur_idx = _cur_fgtlist_idx;
    _cur_fgtlist_idx = (cur_idx + 1)%2;
	UInt8 cnt[2] = {0, 0};
	bool loaded[2] = {false, false};
	//bool checkEnh = _player[1] == NULL;
	bool checkEnh = true; // XXX: 对怪对人都有用
	UInt32 now = TimeUtil::Now();
	for(int i = 0; i < 2; ++ i)
	{
		UInt32 flag = 0;
        UInt32 flag2 = 0;
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
            if (_player[i]->getBuffData(PLAYER_BUFF_YBUF, now) > 0)
                flag |= BattleFighter::Enh4;
            if (_player[i]->getBuffData(PLAYER_BUFF_BBUF, now) > 0)
                flag |= BattleFighter::Enh5;
            if (_player[i]->getBuffData(PLAYER_BUFF_QQVIPBUF, now) > 0)
                flag |= BattleFighter::Enh6;
            /*if(_player[i]->hasFlag(GObject::Player::Copy)
             || _player[i]->hasFlag(GObject::Player::AutoCopy)
             || _player[i]->hasFlag(GObject::Player::InCopyTeam)
             || _player[i]->hasFlag(GObject::Player::AutoDungeon)
             || _player[i]->hasFlag(GObject::Player::AutoFrontMap))*/
            if(_player[i]->hasFlag(GObject::Player::AthleticsBuff))
            {
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL1, now) > 0)
                    flag2 |= BattleFighter::AthlEnh1;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL2, now) > 0)
                    flag2 |= BattleFighter::AthlEnh2;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL3, now) > 0)
                    flag2 |= BattleFighter::AthlEnh3;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL4, now) > 0)
                    flag2 |= BattleFighter::AthlEnh4;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL5, now) > 0)
                    flag2 |= BattleFighter::AthlEnh5;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL6, now) > 0)
                    flag2 |= BattleFighter::AthlEnh6;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL7, now) > 0)
                    flag2 |= BattleFighter::AthlEnh7;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL8, now) > 0)
                    flag2 |= BattleFighter::AthlEnh8;
                if (_player[i]->getBuffData(PLAYER_BUFF_ATHL9, now) > 0)
                    flag2 |= BattleFighter::AthlEnh9;
            }
            flag2 |= _player[i]->getAthlRivalBuff();
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
                        if(flag2 > 0)
                            bf->addFlag2(flag2);
                        if((prevWin-1) != i)
                            bf->initStats(checkEnh);
                        UInt8 justice_roar = (_player[i] != NULL ? _player[i]->getJusticeRoar() : 0);
                        if(justice_roar)
                            bf->AddAura(justice_roar);

                        aura = bf->getAura();
                        maxAura = bf->getAuraMax();
					}
					UInt32 maxhp = bf->getMaxHP();
					_packet << static_cast<UInt8>(j + 1) << bf->getFighter()->getBattleName();

                    UInt8 clsnsex = bf->getFighter()->getClassAndSex();
                    UInt16 portrait = 0;

                    if (bf->getFighter()->getFashionTypeId() == 1700)
                        portrait = 1072;
                    else if (bf->getFighter()->getFashionTypeId() == 1701)
                        portrait = 1074;
                    else if (bf->getFighter()->getFashionTypeId() == 1702)
                        portrait = 1063;
                    else if (bf->getFighter()->getFashionTypeId() == 1703)
                        portrait = 1064;
                    else if (bf->getFighter()->getFashionTypeId() == 1704)
                        portrait = 1076;
                    else if (bf->getFighter()->getFashionTypeId() == 1705)
                        portrait = 1077;
                    else if (bf->getFighter()->getFashionTypeId() == 1706)
                        portrait = 1088;

                    if(bf->getBuffData(FIGHTER_BUFF_CRMASGIRL, now))
                        portrait = 1058;
                    else if(bf->getBuffData(FIGHTER_BUFF_DRESS, now))
                        portrait = 1063;
                    else if(bf->getBuffData(FIGHTER_BUFF_WEDDING, now))
                        portrait = 1064;
                    else if(bf->getBuffData(FIGHTER_BUFF_RMAN, now))
                    {
                        clsnsex = 1<<4;
                        portrait = 1;
                    }
                    else if(bf->getBuffData(FIGHTER_BUFF_RWMAN, now))
                    {
                        clsnsex = 1<<4|1;
                        portrait = 2;
                    }
                    else if(bf->getBuffData(FIGHTER_BUFF_SMAN, now))
                    {
                        clsnsex = 2<<4;
                        portrait = 3;
                    }
                    else if(bf->getBuffData(FIGHTER_BUFF_SWMAN, now))
                    {
                        clsnsex = 2<<4|1;
                        portrait = 4;
                    }
                    else if(bf->getBuffData(FIGHTER_BUFF_DMAN, now))
                    {
                        clsnsex = 3<<4;
                        portrait = 5;
                    }
                    else if(bf->getBuffData(FIGHTER_BUFF_DWMAN, now))
                    {
                        clsnsex = 3<<4|1;
                        portrait = 6;
                    }
                    else
                    {
                        if (!portrait)
                            portrait = bf->getPortrait();
                    }

					if(bf->getFighter()->isNpc())
						_packet << static_cast<UInt8>(bf->getFighter()->reqFriendliness);
                    else
						_packet << clsnsex;

					_packet << static_cast<UInt8>(_isBody[i][j] > 0 ? (_isBody[i][j] - 1) : bf->getFighter()->getLevel()) << portrait << static_cast<UInt8>(bf->getFlag() & 0x03);
					_packet << static_cast<UInt8>(bf->getFighter()->getColor())
						<< static_cast<UInt32>(_isBody[i][j] ? 0 : bf->getHP()) << static_cast<UInt32>(maxhp);

					_packet << aura << maxAura << static_cast<UInt32>(bf->getAttack()) << static_cast<UInt32>(bf->getMagAttack())
                        << static_cast<UInt32>(bf->getDefend()) << static_cast<UInt32>(bf->getMagDefend()) << static_cast<UInt16>(bf->getAction());

                    //add maxAura
                    if(maxAura > _maxAura[i])
                        _maxAura[i] = maxAura;
                    // TODO: up skills
					_packet << static_cast<UInt16>(bf->getHitrate(NULL) * 100.0f) << static_cast<UInt16>(bf->getEvade(NULL) * 100.0f) << static_cast<UInt16>(bf->getCritical(NULL) * 100.0f) << static_cast<UInt16>(bf->getPierce(NULL) * 100.0f) << static_cast<UInt16>(bf->getCounter(NULL) * 100.0f) << static_cast<UInt16>(bf->getTough(NULL) * 100.0f) << static_cast<UInt16>(bf->getCriticalDmg() * 100.0f) << static_cast<UInt16>(bf->getMagRes(NULL) * 100.0f);
                    _packet << bf->getFighter()->getPeerlessAndLevel();
                    bf->getFighter()->getAllUpSkillAndLevel(_packet);
                    bf->getFighter()->getAllPSkillAndLevel(_packet);
                    bf->getFighter()->getAllSSAndLevel(_packet);
                    
					if(ismain && (prevWin-1) != i)
					{
                        bf->postInit();
						// FighterStatus fs(bf);
						// Insert into action queue
						insertFighterStatus(bf);
					}
				}
				else
				{
					_packet << static_cast<UInt8>(0) << static_cast<UInt8>(bo->getClass()) << static_cast<UInt8>(j + 1) << static_cast<UInt32>(bo->getHP());
				}
				++ cnt[i];
			}
		}
	}

    // after insert fighter status to _fgtlist[0] set the _cur_fgtlist_idx to 0
    _cur_fgtlist_idx = cur_idx;
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
    act_count += FightersEnter(prevWin);
	while(_winner == 0 && act_count < _fake_turns)
	{
		int pos = findFirstAttacker();

		act_count += doAttack(pos);
       //  _attackRound ++ ;
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
    if(_winner == 0)
        _winner = testWinner2();

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

	if(_report || !cfg.GMCheck)
		battleReport.addReport(_id, _packet);

    CheckAttain();
}
void  BattleSimulator::SendAttainMsgToPlayer( GObject::Player* player, UInt32 id, UInt32 param)
{
#ifdef NO_ATTAINMENT
     return;
#endif
                 stAttainMsg  msg; 
                 msg.attainID = id;
                 msg.param = param;
                 GameMsgHdr h(0x244,  player->getThreadId(), player, sizeof(msg));
                 GLOBAL().PushMsg(h, & msg);

}
void BattleSimulator::CheckAttain()
{

  // for(UInt8 i = 0; i < 2; i++)
  //only  attacker
        if(_player[0])
        {
            UInt32 max = _maxEvade[0];
            if( max >= 3  &&  max <= 9 &&  max  > _player[0] -> GetVar(VAR_BATTLE_MISS))
            {
                SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_MISS, max);
            }
            max = _maxCS[0];
            if( max >= 3  &&  max<= 9 && max  > _player[0] -> GetVar(VAR_BATTLE_CS))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_CS, max);
            }
            max = _maxPR[0];
            if( max >= 3  &&  max <= 9 && max  > _player[0] -> GetVar(VAR_BATTLE_PR))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_PR, max);
            }

            max =  _maxFJ[0];
            if( max >=3  &&  max <= 9 && max  > _player[0] -> GetVar(VAR_BATTLE_FJ))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_FJ, max);
            }
            max  = _maxSkillDmg[0];
            if(max >= 300  &&  max >  _player[0] -> GetVar(VAR_BATTLE_SKILL_DMG))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_SKILLDMG, max);
            }
            max  =  _maxPeerLessDmg[0];
            if(max >= 1000  &&  max >  _player[0] -> GetVar(VAR_BATTLE_PEERLESS_DMG))
            {
                 SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_PLDMG, max);
            }
            max = _maxAura[0];
            if(max >= 200)
            {
                SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_MAX_AURA, max);
            }
            
            float fMax = _maxCSFactor[0];
            if(fMax >= 3.0)
            {
                SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_CSFactor, 0);
            }

            if(_firstPLDmg[0])
            {
                   SendAttainMsgToPlayer(_player[0] ,  Script::BATTLE_FIRST_PEERLESS_ATTACK, 0);
            }
        }
#if 0
        for (UInt8 i = 0 ; i < 2; i ++)
        {
            printf("miss %u\n",   _maxEvade[i]   );
            printf("%u\n" , _maxCS[i]);
            printf("%u\n" , _maxPR[i]);
            printf("%u\n" , _maxFJ[i]);
            printf("%u\n" , _maxSkillDmg[i]);
            printf("%u\n" , _maxPeerLessDmg[i]);
            printf("%u\n" , _maxAura[i]);
            printf("%f\n" , _maxCSFactor[i]);
        }
#endif
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
                {
                    cur_fgtlist.insert(cur_fgtlist.end(), bf);
                }
                return;

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
        _attackRound ++ ;
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

UInt32 BattleSimulator::doXinmoAttack(BattleFighter * bf, BattleObject* bo, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount)
{
    if(!bf || !bo || bf->getHP() == 0 || bo->getHP() == 0)
        return 0;
	UInt32 dmg = 0;

    BattleFighter * area_target = static_cast<BattleFighter *>(bo);
    UInt8 target_stun = area_target->getStunRound();
    bool enterEvade = area_target->getEvad100();
    bool defend100 = area_target->getDefend100();

    {
        defList[defCount].damType = e_xinmo;
        defList[defCount].damage = 0;
        defList[defCount].leftHP = bf->getHP();
        defList[defCount].pos = bf->getPos();
        ++ defCount;
    }

    bool pr = false;
    bool cs = false;
    if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(area_target))))
    {
        pr = bf->calcPierce(area_target);
        float atk = 0;
        float cf = 0.0f;
        atk = bf->calcAttack(cs, area_target, &cf);
        if(cs )
        {
            UInt8 s = bf->getSide();
            if(s < 2)
                _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

        }

        float def;
        float toughFactor = pr ? area_target->getTough(bf) : 1.0f;
        def = area_target->getDefend();
        float atkreduce = area_target->getAtkReduce();
        dmg = _formula->calcDamage(atk, def, bf->getLevel(), toughFactor, atkreduce);

        dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

        dmg = dmg > 0 ? dmg : 1;

        defList[defCount].damType = e_damNormal;
        area_target->makeDamage(dmg);
        defList[defCount].damage = dmg;

        defList[defCount].leftHP = area_target->getHP();
        defList[defCount].pos = area_target->getPos();
        ++ defCount;
        // killed the target fighter
        if(area_target->getHP() == 0)
        {
            onDead(false, area_target, defList, defCount);
        }
        else if(_winner == 0)
        {
            onDamage(area_target, scList, scCount, true, NULL);
        }
    }
    else
    {
        if(enterEvade)
        {
            defList[defCount].damType = e_damEvade;
            area_target->setEvad100(false);
        }

        if(defend100)
        {
            defList[defCount].damType = e_damOut;
            area_target->setDefend100(false);
        }
        
        defList[defCount].damage = 0;
        defList[defCount].leftHP = area_target->getHP();
//			printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
        defList[defCount].pos = area_target->getPos();
        ++ defCount;
    }

    return dmg;
}

UInt32 BattleSimulator::attackOnce(BattleFighter * bf, bool& first, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * area_target_obj, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, int counter_deny, AttackPoint * counter_deny_list, std::vector<AttackAct>* atkAct)
{
	if(area_target_obj == NULL || area_target_obj->getHP() == 0 || bf == NULL)
		return 0;
	// if a fighter was attacked
	UInt32 dmg = 0;
    UInt32 magdmg = 0;
    bool cs2 = false;
    bool pr2 = false;
	if(area_target_obj->isChar())
	{
        bool counter100 = false;
        bool poison = false;
		BattleFighter * area_target = static_cast<BattleFighter *>(area_target_obj);
		UInt8 side = area_target->getSide();
		UInt8 pos = area_target->getPos();
        UInt8 target_stun = area_target->getStunRound();
        bool enterEvade = area_target->getEvad100();
        bool defend100 = area_target->getDefend100();
        // 雪人
        if(area_target->getId() == 5679)
            target_stun = 1;

        GData::SkillStrengthenBase* ss = NULL;
        if(skill)
            ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
        // target fighter will do not counter while fighter is the same side
        bool can_counter = true;
        if(target_stun > 0 || bf->getSide() == area_target->getSide())
        {
            can_counter = false;
        }

        float aura_factor = 1;
        if(skill && skill->cond == GData::SKILL_PEERLESS)
        {
            aura_factor = 1 + static_cast<float>(bf->getAura()-100) * 0.0025;
        }

		if(!defend100 && (target_stun > 0 || (!enterEvade && bf->calcHit(area_target))))
		{
            UInt8& deepforgetlast = area_target->getDeepForgetLast();
            if(deepforgetlast > 0 && bf->getSide() != area_target->getSide())
            {
                -- deepforgetlast;
                factor += area_target->getDeepForgetDmgExtra();
                if(deepforgetlast == 0)
                {
                    area_target->setDeepForgetDmgExtra(0, 0);
                    defList[defCount].damType = e_unDeepForget;
                    defList[defCount].damage = 0;
                    defList[defCount].pos = area_target->getPos();
                    defList[defCount].leftHP = area_target->getHP();
                    defCount++;

                    defList[defCount].damType = e_Forget;
                    defList[defCount].damage = 0;
                    defList[defCount].pos = area_target->getPos();
                    defList[defCount].leftHP = area_target->getHP();
                    defCount++;
                }
            }
            UInt8& deepstunlast = area_target->getDeepStunLast();
            if(deepstunlast > 0 && bf->getSide() != area_target->getSide())
            {
                -- deepstunlast;
                factor += area_target->getDeepStunDmgExtra();
                if(deepstunlast == 0)
                {
                    area_target->setDeepStunDmgExtra(0, 0);
                    defList[defCount].damType = e_unDeepStun;
                    defList[defCount].damage = 0;
                    defList[defCount].pos = area_target->getPos();
                    defList[defCount].leftHP = area_target->getHP();
                    defCount++;

                    defList[defCount].damType = e_Stun;
                    defList[defCount].damage = 0;
                    defList[defCount].pos = area_target->getPos();
                    defList[defCount].leftHP = area_target->getHP();
                    defCount++;
                }
            }

            UInt8& defdeclast = area_target->getDefDecLast();
            if(defdeclast > 0 && bf->getSide() != area_target->getSide())
            {
                UInt8 deflast = area_target->getDefendAddLast();
                UInt8 magdeflast = area_target->getMagDefendAddLast();
                float defAdd = area_target->getDefendAdd();
                float magdefAdd = area_target->getMagDefendAdd();
                if(deflast > 0)
                {
                    defAdd += area_target->_defend * area_target->getDefDec()/100;
                    magdefAdd += area_target->_magdef * area_target->getDefDec()/100;
                    area_target->setDefendAdd(defAdd, deflast);
                    area_target->setMagDefendAdd(magdefAdd, magdeflast);
                    -- defdeclast;

                    UInt8 defdecTimes = area_target->getDefDecTimes();
                    ++ defdecTimes;
                    defList[defCount].pos = area_target->getPos();
                    defList[defCount].damType = e_defDec;
                    defList[defCount].damage = defdecTimes;
                    defList[defCount].leftHP = area_target->getHP();
                    ++ defCount;
                    area_target->setDefDecTimes(defdecTimes);
                }
                else
                    defdeclast = 0;
            }

            pr2 = bf->calcPierce(area_target);
            float atk = 0;
            float magatk = 0;
            if(NULL != skill && skill->effect != NULL)
            {
                float  cf;
                bf->calcSkillAttack(cs2, area_target, atk, magatk , & cf);
                if(cs2)
                {
                     UInt8 s = bf->getSide();
                    if(s < 2)
                        _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

 
                }
                //atk = bf->calcAttack(cs2, area_target);
                //magatk = bf->calcMagAttack(cs2, area_target);
                float crratk = 0;
                float crrmagatk = 0;
                if(bf->getClass() == 3)
                    crratk = aura_factor * (atk * skill->effect->crrdamP + skill->effect->addcrr * (cs2 ? bf->calcCriticalDmg(area_target) : 1));
                else
                    crrmagatk = aura_factor * (magatk * skill->effect->crrdamP + skill->effect->addcrr * (cs2 ? bf->calcCriticalDmg(area_target) : 1));

                atk = aura_factor * (atk * skill->effect->damageP + skill->effect->adddam * (cs2 ? bf->calcCriticalDmg(area_target) : 1)) + crratk;
                magatk = aura_factor * (magatk * skill->effect->magdamP + skill->effect->addmag * (cs2 ? bf->calcCriticalDmg(area_target) : 1)) + crrmagatk;
            }
            else
            {
                float cf = 0.0f;
                atk = bf->calcAttack(cs2, area_target, &cf);
                if(cs2)
                {
                    UInt8 s = bf->getSide();
                    if(s < 2)
                        _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

                }
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
                float toughFactor = pr2 ? area_target->getTough(bf) : 1.0f;
                if(magatk)
                {
                    magdef = area_target->getMagDefend();
                    float magatkreduce = area_target->getMagAtkReduce();
                    magdmg = _formula->calcDamage(factor * magatk, magdef, bf->getLevel(), toughFactor, magatkreduce);

                    magdmg *= static_cast<float>(950 + _rnd(100)) / 1000;

                    magdmg = magdmg > 0 ? magdmg : 1;
                }

                if(atk)
                {
                    def = area_target->getDefend();
                    float atkreduce = area_target->getAtkReduce();
                    dmg = _formula->calcDamage(factor * atk, def, bf->getLevel(), toughFactor, atkreduce);

                    dmg *= static_cast<float>(950 + _rnd(100)) / 1000;

                    dmg = dmg > 0 ? dmg : 1;
                }

			}
            if(area_target->getMagAtkReduce3Last() > 0)
            {
                StatusChange& sc = scList[scCount];
                sc.pos = pos + (bf->getSide() == side ? 25 : 0);
                sc.statusId = 0;
                area_target->setMagAtkReduce3(0, 0);
                sc.type = e_stMagAtkReduce;
                sc.data = static_cast<UInt32>(area_target->getMagAtkReduce());
                ++scCount;
            }
            if(area_target->getAtkReduce3Last() > 0)
            {
                StatusChange& sc = scList[scCount];
                sc.pos = pos + (bf->getSide() == side ? 25 : 0);
                sc.statusId = 0;
                area_target->setAtkReduce3(0, 0);
                sc.type = e_stAtkReduce;
                sc.data = static_cast<UInt32>(area_target->getAtkReduce());
                ++scCount;
            }

            defList[defCount].damType = e_damNormal;
            area_target->makeDamage(dmg + magdmg);
            defList[defCount].damage = dmg + magdmg;

			defList[defCount].leftHP = area_target->getHP();
            defList[defCount].pos = pos + (bf->getSide() == side ? 25 : 0);
            ++ defCount;
//			printf("%u:%u %s %u:%u, made %u damage, hp left: %u\n", 1-side, from_pos, cs2 ? "CRITICALs" : "hits", side, pos, dmg, area_target->getHP());
			// killed the target fighter

            if(bf->getSide() != area_target->getSide() && counter_deny >= 0 && (!skill || skill->cond == GData::SKILL_ACTIVE))
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, 25 + bf->getSoulExtraAura(), 0, scList, scCount, false);
                setStatusChange(bf, area_target->getSide(), area_target->getPos(), 1, 0, e_stAura, 25 + area_target->getSoulExtraAura(), 0, scList, scCount, true);
            }

			if(area_target->getHP() == 0)
            {
				onDead((side == bf->getSide()), area_target, defList, defCount);
            }
			else if(_winner == 0)
            {
                const GData::SkillStrengthenEffect* ef = NULL;
                if(cs2 && ss)
                    ef = ss->getEffect(GData::ON_CRITICAL, GData::TYPE_BLEED);
                if(!ef && pr2 && ss)
                    ef = ss->getEffect(GData::ON_PIERCE, GData::TYPE_BLEED);
                if (ef)
                {
                    switch(bf->getClass())
                    {
                    case 1:
                        defList[defCount].damType = e_Bleed1;
                        area_target->setBleed1(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    case 2:
                        defList[defCount].damType = e_Bleed2;
                        area_target->setBleed2(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    case 3:
                        defList[defCount].damType = e_Bleed3;
                        area_target->setBleed3(ef->value/100*(dmg + magdmg), ef->last);
                        break;
                    }

                    defList[defCount].damage = 0;
                    defList[defCount].pos = pos;
                    defList[defCount].leftHP = area_target->getHP();
                    ++ defCount;
                }

				onDamage(area_target, scList, scCount, true, atkAct);

                if(NULL != skill && skill->effect != NULL && skill->effect->state == 1)
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
            if(atkAct && !defend100 && can_counter)
            {
                const GData::SkillBase* target_skill = NULL;
                size_t idx = 0;
                while(NULL != (target_skill = area_target->getPassiveSkillAftEvd100(idx)))
                {
                    defList[defCount].pos = pos;
                    defList[defCount].damType = e_skill;
                    defList[defCount].damage = target_skill->getId();
                    defList[defCount].leftHP = area_target->getHP();
                    ++ defCount;
                    /***************************
                    AttackAct aa = {0};
                    aa.bf = area_target;
                    aa.skill = target_skill;
                    aa.target_side = bf->getSide();
                    aa.target_pos = bf->getPos();
                    atkAct->push_back(aa);
                    *****************************/
                    counter100 = true;
                    break;
                } 
                if(counter100 != true)
                {
                    target_skill = area_target->getPassiveSkillAftEvd();
                    if(NULL != target_skill)
                    {
                        defList[defCount].pos = pos;
                        defList[defCount].damType = e_skill;
                        defList[defCount].damage = target_skill->getId();
                        defList[defCount].leftHP = area_target->getHP();
                        ++ defCount;
                        /***************************
                        AttackAct aa = {0};
                        aa.bf = area_target;
                        aa.skill = target_skill;
                        aa.target_side = bf->getSide();
                        aa.target_pos = bf->getPos();
                        atkAct->push_back(aa);
                        *****************************/
                        counter100 = true;
                    }
                }
            }

			defList[defCount].damType = e_damEvade;
            if(enterEvade)
                area_target->setEvad100(false);

            if(defend100)
            {
                defList[defCount].damType = e_damOut;
                area_target->setDefend100(false);
                if(bf->getSide() != area_target->getSide() && counter_deny >= 0 && (!skill || skill->cond == GData::SKILL_ACTIVE))
                {
                    setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, 25 + bf->getSoulExtraAura(), 0, scList, scCount, false);
                    setStatusChange(bf, area_target->getSide(), area_target->getPos(), 1, 0, e_stAura, 25 + area_target->getSoulExtraAura(), 0, scList, scCount, true);
                }
            }
            
			defList[defCount].damage = 0;
			defList[defCount].leftHP = area_target->getHP();
//			printf("%u:%u hits %u:%u, but missed!\n", 1-side, from_pos, side, pos);
            defList[defCount].pos = pos + (bf->getSide() == side ? 25 : 0);
            ++ defCount;
		}

        UInt32 rhp = 0;
        if(NULL != skill && skill->effect != NULL && (skill->effect->absorb || skill->effect->absorbP))
        {
            rhp = (dmg + magdmg) * skill->effect->absorbP + skill->effect->absorb;
        }
        const GData::SkillStrengthenEffect* ef = NULL;
        if(ss)
            ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_ABSORB);
        if(ef)
        {
            rhp += (dmg + magdmg) * ef->value/100;
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
            doPoisonAttack(bf, cs2, skill, area_target, factor, defList, defCount, scList, scCount, atkAct);
        }

        std::vector<AttackAct> atkAct2;
        atkAct2.clear();
        doPassiveSkillBeAtk(bf, area_target, &atkAct2, atkAct, dmg + magdmg);
        doSkillAtk2((side == bf->getSide()), &atkAct2, defList, defCount, scList, scCount);

		// if this fighter can counter
		if(can_counter && counter_deny >= 0 && _winner == 0 && !_isBody[side][pos] && area_target_obj->getHP() > 0 && bf->getHP() > 0)
		{
			BattleFighter * target_fighter = static_cast<BattleFighter *>(area_target_obj);
			// test counter by rolling dice
			if(counter100 || target_fighter->calcCounter(bf, !bf->canBeCounter()))
			{
				if(counter100 || target_fighter->calcHit(bf))
				{
                    defList[0].damType2 |= 0x80;
					bool cs2 = false;
                    float cf = 0.0f;
                    float atk = target_fighter->calcAttack(cs2, bf, &cf);
                    if(cs2)
                    {
                        UInt8 s = target_fighter->getSide();
                        if(s < 2)
                            _maxCSFactor[s] = std::max( cf, _maxCSFactor[s] ) ;

                    }
					//float atk = target_fighter->getAttack();
					float def = bf->getDefend();
					bool pr2 = target_fighter->calcPierce(bf);
                    float toughFactor = pr2 ? bf->getTough(target_fighter) : 1.0f;
                    float atkreduce = bf->getAtkReduce();
					UInt32 dmg2 = _formula->calcDamage(atk, def, target_fighter->getLevel(), toughFactor, atkreduce);

                    dmg2 *= static_cast<float>(950 + _rnd(100)) / 1000;

                    dmg2 = dmg2 > 0 ? dmg2 : 1;

					bf->makeDamage(dmg2);
                    if(bf->getMagAtkReduce3Last() > 0)
                    {
                        StatusChange& sc = scList[scCount];
                        sc.pos = bf->getPos() + 25;
                        sc.statusId = 0;
                        bf->setMagAtkReduce3(0, 0);
                        sc.type = e_stMagAtkReduce;
                        sc.data = static_cast<UInt32>(bf->getMagAtkReduce());
                        ++scCount;
                    }

                    if(bf->getAtkReduce3Last() > 0)
                    {
                        StatusChange& sc = scList[scCount];
                        sc.pos = bf->getPos() + 25;
                        sc.statusId = 0;
                        bf->setAtkReduce3(0, 0);
                        sc.type = e_stAtkReduce;
                        sc.data = static_cast<UInt32>(bf->getAtkReduce());
                        ++scCount;
                    }

					if(cs2)
						defList[0].damType2 |= 0x40;
					if(pr2)
						defList[0].damType2 |= 0x20;
					defList[0].counterDmg = dmg2;
					defList[0].counterLeft = bf->getHP();

//					if(cs2)
//						printf("  [Counter] %u:%u CRITICAL-STRIKEs %u:%u, made %u damage, hp left: %u\n", side, pos, 1-side, from_pos, dmg2, bf->getHP());
//					else
//						printf("  [Counter] %u:%u attacks %u:%u, made %u damage, hp left: %u\n", side, pos, 1-side, from_pos, dmg2, bf->getHP());

					// killed the fighter
					if(bf->getHP() == 0)
                    {
						if(!onDead(true, bf, defList, defCount))
                            defList[0].counterLeft = bf->getHP();
                    }
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
        float atk = bf->calcAttack(cs2, 0, NULL);
		dmg = static_cast<int>(factor * atk) * (950 + _rnd(100)) / 1000;
		area_target_obj->makeDamage(dmg);
		defList[defCount].pos = area_target_obj->getPos() + (bf->getSide() == area_target_obj->getSide() ? 25 : 0);
		defList[defCount].damType = e_damNormal;
		defList[defCount].damage = dmg;
		defList[defCount].leftHP = area_target_obj->getHP();
//		printf("%u:%u %s ground object, made %u damage, hp left: %u\n", 1-side, from_pos, cs2 ? "CRITICALs" : "hits", dmg, area_target_obj->getHP());
		++ defCount;
	}

    if(first)
    {
        cs = cs2;
        pr = pr2;
        first = false;
    }
//to get attainment
    UInt8 s = bf->getSide();
    if(skill && s < 2 &&  _player[s] != NULL)
    {
        UInt32 d = magdmg + dmg;
        if(skill->cond == GData::SKILL_PEERLESS)
        {
            if( 0 == _attackRound )
                _firstPLDmg[s] = true;
           // printf("第一回合释放无双技能%u",  _attackRound);
            _maxPeerLessDmg[s] = std::max( _maxPeerLessDmg[s], d);
        }
        else
        {
            _maxSkillDmg[s] = std::max(_maxSkillDmg[s], d);
        }
    }
	return dmg + magdmg;
}

void BattleSimulator::doPassiveSkillBeAtk(BattleFighter* bf, BattleFighter* bo, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct, UInt32 dmg)
{
    if(bf == NULL || bo == NULL || atkAct == NULL)
        return;

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
            else if(bf->getHP() > 0 && bf->getSide() != bo->getSide())
            {
                AttackAct aa = {0};
                aa.bf = bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();

                atkAct2->push_back(aa);
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
            else if(bf->getHP() > 0 && bf->getSide() != bo->getSide())
            {
                AttackAct aa = {0};
                aa.bf = bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();

                atkAct2->push_back(aa);
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

            atkAct2->push_back(aa);
        }

        if(passiveSkillThorn && bf->getSide() != bo->getSide() && dmg && bf->getHP() > 0)
        {
            AttackAct aa = {0};
            aa.bf = bo;
            aa.skill = passiveSkillThorn;
            aa.target_side = bf->getSide();
            aa.target_pos = bf->getPos();
            aa.param = dmg;

            atkAct2->push_back(aa);
        }
    }
}

UInt32 BattleSimulator::doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, std::vector<AttackAct>* atkAct)
{
    UInt32 dmg = 0;
    UInt32 dmg2 = abs(bf->calcPoison(skill, area_target, cs)) * factor;
    // 第一波毒
    std::vector<AttackAct> atkAct2;
    atkAct2.clear();
    doSkillState(bf, skill, area_target, defList, defCount, &atkAct2, atkAct);
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
            if(onDead(false, area_target, defList, defCount))
                return dmg;
        }
        else if(_winner == 0)
        {
            onDamage(area_target, scList, scCount, true);
        }

        // 第二波毒
        doSkillState(bf, skill, area_target, defList, defCount, &atkAct2, atkAct);
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
                if(onDead(false, area_target, defList, defCount))
                    return dmg;
            }
            else if(_winner == 0)
            {
                onDamage(area_target, scList, scCount, true);
            }

            // 第三波毒
            doSkillState(bf, skill, area_target, defList, defCount, &atkAct2, atkAct);
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
                    onDead(false, area_target, defList, defCount);
                else if(_winner == 0)
                {
                    onDamage(area_target, scList, scCount, true);
                }
            } // 第三波毒
            else
            {
                defList[defCount].leftHP = area_target->getHP();
                ++defCount;
                doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
            }
        } // 第二波毒
        else
        {
            defList[defCount].leftHP = area_target->getHP();
            ++defCount;
            doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
        }
    } // 第一波毒
    else
    {
        defList[defCount].leftHP = area_target->getHP();
        ++defCount;
        doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
    }

    return dmg;
}

void BattleSimulator::doSkillAtk2(bool activeFlag, std::vector<AttackAct>* atkAct, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount)
{
    if(atkAct == NULL)
        return;

    size_t actCnt = atkAct->size();
    for(size_t idx = 0; idx < actCnt; idx++)
    {
        BattleFighter * bf = (*atkAct)[idx].bf;
        if(!bf || bf->getHP() == 0)
            continue;

        int target_side = (*atkAct)[idx].target_side;
        int target_pos = (*atkAct)[idx].target_pos;
        const GData::SkillBase* skill = (*atkAct)[idx].skill;

        defList[defCount].pos = bf->getPos() + (activeFlag ? 25 : 0);
        defList[defCount].damType = e_skill;
        defList[defCount].damage = skill->getId();
        defList[defCount].leftHP = bf->getHP();
        ++ defCount;

        UInt32 skillParam = (*atkAct)[idx].param;
        // 法术抵抗后的技能
        if(skill->cond == GData::SKILL_AFTRES && bf->getSide() != target_side)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(NULL == bo)
                continue;

            const GData::SkillBase* boSkill = GData::skillManager[skillParam];
            if(boSkill == NULL || boSkill->effect == NULL)
                continue;

            UInt8 state[3] = {0};
            UInt8 cnt = 0;
            UInt8 idx = 0;

            UInt8 effect_state = boSkill->effect->state;
            if(SKILL_ID(boSkill->getId()) == 136 || SKILL_ID(boSkill->getId()) == 146) // 光棍, 冰火双剪
            {
                switch(bo->getClass())
                {
                case 1:
                    effect_state = 0x02;
                    break;
                case 2:
                    effect_state = 0;
                    break;
                case 3:
                    effect_state = 0x04;
                    break;
                }
            }

            state[0] = effect_state;
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
                if(boSkill->effect->state & 0x20)
                {
                    state[cnt] = 0x20;
                    ++cnt;
                }
                idx = _rnd(cnt);
            }

            UInt8 immune = bo->getImmune();
            if((state[idx] & immune) && SKILL_LEVEL(boSkill->getId()) <= bo->getImmuneLevel(state[idx]))
            {
                defList[defCount].damType = e_Immune;
                defList[defCount].pos = bo->getPos() + (activeFlag ? 0 : 25);
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
                continue;
            }

            switch(state[idx])
            {
            case 1:
                {
                    UInt32 dmg = abs(bo->calcPoison(boSkill, bo, false));
                    bo->makeDamage(dmg*0.5);
                    defList[defCount].damage = dmg*0.5;
                    defList[defCount].leftHP = bo->getHP();
                    defList[defCount].pos = bo->getPos() + (activeFlag ? 0 : 25);
                    ++defCount;
                    if(bo->getHP() == 0)
                    {
                        if(onDead(!activeFlag, bo, defList, defCount))
                            break;
                    }

                    bo->makeDamage(dmg);
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defList[defCount].pos = bo->getPos() + (activeFlag ? 0 : 25);
                    ++defCount;
                    if(bo->getHP() == 0)
                    {
                        if(onDead(!activeFlag, bo, defList, defCount))
                            break;
                    }

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
                    bo->setConfuseRound(boSkill->last + 1);
                }
                break;
            case 4:
                if(bo->getStunRound() < 1)
                {
                    defList[defCount].damage = 0;
                    defList[defCount].damType = e_Stun;
                    bo->setStunLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setStunRound(boSkill->last + 1);
                }
                break;
            case 8:
                if(bo->getForgetRound() < 1)
                {
                    defList[defCount].damage = 0;
                    defList[defCount].damType = e_Forget;
                    bo->setForgetLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setForgetRound(boSkill->last + 1);
                }
                break;
            case 0x20:
                if(bo->getWeakRound() < 1)
                {
                    defList[defCount].damage = 0;
                    defList[defCount].damType = e_Weak;
                    bo->setWeakLevel(SKILL_LEVEL(boSkill->getId()));
                    bo->setWeakRound(boSkill->last);
                }
                break;
            }

            defList[defCount].pos = bo->getPos() + (activeFlag ? 0 : 25);
            defList[defCount].leftHP = bo->getHP();
            ++defCount;
            if(bo->getHP() == 0)
            {
                onDead(!activeFlag, bo, defList, defCount);
            }
            continue;
        }

        // 被攻击后的技能
        if(skill->cond == GData::SKILL_BEATKED)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);

            if(NULL == bo)
                continue;

            if(bf->getHP() > 0 && bo->getHP() > 0)
            {
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
                    defList[defCount].pos = bf->getPos() + (activeFlag ? 25 : 0);
                    defList[defCount].leftHP = bf->getHP();
                    ++ defCount;

                    continue;
                }
                else if(fdmg > 0 && bf->getSide() != target_side)
                {
                    UInt8 immune = bo->getImmune();
                    if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel(0x10))
                    {
                       defList[defCount].damType = e_Immune;
                    }
                    else
                    {
                        defList[defCount].damType = e_damBack;
                        defList[defCount].damage = fdmg;
                        bo->makeDamage(fdmg);
                    }

                    defList[defCount].pos = bo->getPos() + (activeFlag ? 0 : 25);
                    defList[defCount].leftHP = bo->getHP();
                    ++ defCount;

                    if(bo->getHP() == 0)
                    {
                        onDead(!activeFlag, bo, defList, defCount);
                    }
                    continue;
                }

            }
        }

        GData::Area* area = NULL;
        area = &(GData::areaList[skill->area]);

        AttackPoint ap[25];
        int apcnt = 0;

        if(skill->area != 0 && skill->area != 1 && area->getCount() > 0)
        {
            int x_ = target_pos % 5;
            int y_ = target_pos / 5;
            int cnt = area->getCount();
            int fsize = skill->factor.size();
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
                int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
                if(fsize != 0)
                    ap[apcnt ++].factor = skill->factor[idx];
                else
                    ap[apcnt ++].factor = 1;
            }
        }

        // 免疫降灵气
        //bool dostatus = true;
        //if(dostatus)
        {
            bool self = false;
            bool flag = ((target_side == bf->getSide()) ? activeFlag : !activeFlag);
            if(1 == skill->area)
            {
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    bool ifDecAura = isImmuneDecAura(skill, target_side, pos, defList, defCount);
                    doSkillStatus(flag, bf, skill, target_side, pos, 1, scList, scCount, self, ifDecAura);
                }
            }
            else if(0 == skill->area)
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos, defList, defCount);
                doSkillStatus(flag, bf, skill, target_side, target_pos, 1, scList, scCount, self, ifDecAura);
            }
            else
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos, defList, defCount);
                    doSkillStatus(flag, bf, skill, target_side, target_pos, 1, scList, scCount, self, ifDecAura);
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    bool ifDecAura = isImmuneDecAura(skill, target_side, ap[i].pos, defList, defCount);
                    doSkillStatus(flag, bf, skill, target_side, ap[i].pos, 1, scList, scCount, self, ifDecAura);
                }

            }
        }

        // therapy skill
        if(skill->effect->hp > 0 || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
        {
            bool cs = false;
            bool first = true;
            UInt32 rhp = bf->calcTherapy(cs, first, skill);
            if(bf->getSide() != target_side)
            {
                UInt32 hpr = bf->regenHP(rhp);
                if(hpr != 0)
                {
                    defList[defCount].pos = bf->getPos() + (activeFlag ? 25 : 0);
                    defList[defCount].damType = e_damHpAdd;
                    defList[defCount].damage = hpr;
                    defList[defCount].leftHP = bf->getHP();
                    ++ defCount;
                }
            }
            else if(1 == skill->area)
            {
                int i = 0;
                int fsize = skill->factor.size();
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    if(bo->getLostHP() == 0)
                        continue;

                    float factor = 1;
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        factor = skill->factor[idx];

                    UInt32 hpr = bo->regenHP(rhp * factor);
                    ++i;
                    if(hpr == 0)
                        continue;

                    defList[defCount].pos = pos + (activeFlag ? 25 : 0);
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
                    continue;

                UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp);
                if(hpr != 0)
                {
                    defList[defCount].pos = bo->getPos() + (activeFlag ? 25 : 0);
                    defList[defCount].damType = e_damHpAdd;
                    defList[defCount].damage = hpr;
                    defList[defCount].leftHP = bo->getHP();
                    ++ defCount;
                }
            }
            else
            {
                int fsize = skill->factor.size();
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    float factor = 1;
                    if(fsize > 0)
                        factor = skill->factor[0];
                    UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp * factor);
                    if(hpr != 0)
                    {
                        defList[defCount].pos = bo->getPos() + (activeFlag ? 25 : 0);
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

                    UInt32 hpr = static_cast<BattleFighter*>(bo)->regenHP(rhp * ap[i].factor, skill->cond == GData::SKILL_ACTIVE);
                    if(hpr != 0)
                    {
                        defList[defCount].pos = bo->getPos() + (activeFlag ? 25 : 0);
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
                    int fsize = skill->factor.size();
                    for(UInt8 pos = 0; pos < 25; ++ pos)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                        int idx = std::min(abs(fsize-1), i);
                        if(fsize > 0)
                            dmg *= skill->factor[idx];
                        ++i;
                        bo->makeDamage(dmg);
                        defList[defCount].pos = pos + (activeFlag ? 0 : 25);
                        defList[defCount].damType = e_damNormal;
                        defList[defCount].damage = dmg;
                        defList[defCount].leftHP = bo->getHP();
                        defCount ++;

                        if(bo->getHP() == 0)
                        {
                            onDead(!activeFlag, bo, defList, defCount);
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
                        defList[defCount].pos = target_pos + (activeFlag ? 0 : 25);
                        defList[defCount].damType = e_damNormal;
                        defList[defCount].damage = dmg;
                        defList[defCount].leftHP = bo->getHP();
                        defCount ++;

                        if(bo->getHP() == 0)
                        {
                            onDead(activeFlag, bo, defList, defCount);
                        }
                    }
                }
                else
                {
                    int fsize = skill->factor.size();
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                        if(fsize > 0)
                            dmg *= skill->factor[0];
                        bo->makeDamage(dmg);
                        defList[defCount].pos = target_pos + (activeFlag ? 0 : 25);
                        defList[defCount].damType = e_damNormal;
                        defList[defCount].damage = dmg;
                        defList[defCount].leftHP = bo->getHP();
                        defCount ++;

                        if(bo->getHP() == 0)
                        {
                            onDead(activeFlag, bo, defList, defCount);
                        }
                    }

                    for(int i = 0; i < apcnt; ++ i)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        UInt32 dmg = abs(bf->calcPoison(skill, bo, false)) * ap[i].factor;
                        bo->makeDamage(dmg);
                        defList[defCount].pos = ap[i].pos + (activeFlag ? 0 : 25);
                        defList[defCount].damType = e_damNormal;
                        defList[defCount].damage = dmg;
                        defList[defCount].leftHP = bo->getHP();
                        defCount ++;

                        if(bo->getHP() == 0)
                        {
                            onDead(activeFlag, bo, defList, defCount);
                        }
                    }
                }
            }
        }

        // 混乱晕眩封印状态
        if(skill->effect->state & 0x2e)
        {
            float rate = skill->prob * 100;
            if(bf->getSide() != target_side)
            {
                if(1 == skill->area)
                {
                    int i = 0;
                    int fsize = skill->factor.size();
                    for(UInt8 pos = 0; pos < 25; ++ pos)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        float factor = 1;
                        int idx = std::min(abs(fsize-1), i);
                        if(fsize > 0)
                            factor = skill->factor[idx];
                        if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || (rate * factor) > _rnd(10000)))
                        {
                            defList[defCount].damage = 0;
                            defList[defCount].pos = pos + (activeFlag ? 0 : 25);
                            defList[defCount].leftHP = bo->getHP();
                            doSkillState(bf, skill, bo, defList, defCount, NULL, NULL);
                            defCount ++;
                        }
                        ++ i;
                    }
                }
                else if(0 == skill->area)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                        {
                            std::vector<AttackAct> atkAct2;
                            atkAct2.clear();
                            defList[defCount].damage = 0;
                            defList[defCount].pos = target_pos + (activeFlag ? 0 : 25);
                            defList[defCount].leftHP = bo->getHP();
                            doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                            defCount ++;
                            doSkillAtk2(!activeFlag, &atkAct2, defList, defCount, scList, scCount);
                        }
                    }
                }
                else
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                    if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                    {
                        int fsize = skill->factor.size();
                        float factor = 1;
                        if(fsize > 0)
                            factor = skill->factor[0];
                        if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || (rate * factor) > _rnd(10000)))
                        {
                            std::vector<AttackAct> atkAct2;
                            atkAct2.clear();
                            defList[defCount].damage = 0;
                            defList[defCount].pos = target_pos + (activeFlag ? 0 : 25);
                            defList[defCount].leftHP = bo->getHP();
                            doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                            defCount ++;
                            doSkillAtk2(!activeFlag, &atkAct2, defList, defCount, scList, scCount);
                        }
                    }

                    for(int i = 0; i < apcnt; ++ i)
                    {
                        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                        if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                            continue;

                        if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || (rate * ap[i].factor) > _rnd(10000)))
                        {
                            std::vector<AttackAct> atkAct2;
                            atkAct2.clear();
                            defList[defCount].damage = 0;
                            defList[defCount].pos = ap[i].pos + (activeFlag ? 0 : 25);
                            defList[defCount].leftHP = bo->getHP();
                            doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                            defCount ++;
                            doSkillAtk2(!activeFlag, &atkAct2, defList, defCount, scList, scCount);
                        }
                    }
                }
            }
        }
    }
}

void BattleSimulator::doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, DefStatus* defList, size_t& defCount, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct)
{
    if(NULL == skill || bf == NULL)
    {
        return;
    }

    if(skill->effect == NULL)
        return;
    if(skill->effect->state == 0)
        return;

    UInt8 state[3] = {0};
    UInt8 cnt = 0;
    UInt8 state_idx = 0;
    UInt8 effect_state = skill->effect->state;
    if(SKILL_ID(skill->getId()) == 136 || SKILL_ID(skill->getId()) == 146) // 光棍, 冰火双剪
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

    GData::SkillStrengthenBase* ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    state[0] = effect_state;
    if(effect_state & 0x2e)
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
        if(effect_state & 0x20)
        {
            state[cnt] = 0x20;
            ++cnt;
        }
        if(cnt > 1)
            state_idx = _rnd(cnt);
    }

    BattleFighter* target_bo = static_cast<BattleFighter*>(bo);
    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    UInt8 immune = target_bo->getImmune();
    UInt8 immune2 = target_bo->getImmune2();
    if((state[state_idx] & immune2))
    {
        target_bo->setImmune2(0);
        defList[defCount].damType = e_unImmune2;
        return;
    }
    if((state[state_idx] & immune) && SKILL_LEVEL(skill->getId()) <= target_bo->getImmuneLevel(state[state_idx]))
    {
        defList[defCount].damType = e_Immune;
        return;
    }

    float rate = target_bo->getMagRes(bf) * 100;
    if(rate > _rnd(10000))
    {
        defList[defCount].damType = e_Res;
        idx = 0;

        if(atkAct)
        {
            if(passiveSkill == NULL)
                passiveSkill = target_bo->getPassiveSkillAftRes();

            if(passiveSkill == NULL)
            {
                idx = 0;
                while(NULL == passiveSkill)
                {
                    size_t oidx = idx;
                    passiveSkill = target_bo->getPassiveSkillAftRes100(idx);
                    if (oidx == idx)
                        break;
                }
            }

            if(NULL != passiveSkill)
            {
                AttackAct aa = {0};
                aa.bf = target_bo;
                aa.skill = passiveSkill;
                aa.target_side = bf->getSide();
                aa.target_pos = bf->getPos();
                aa.param = skill->getId();

                atkAct2->push_back(aa);
                defList[defCount].damType = e_ResR;
            }
        }
        return;
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
            if(skill->cond == GData::SKILL_BEATKED)
                target_bo->setConfuseRound(skill->last + 1);
            else
                target_bo->setConfuseRound(skill->last);
        }
        break;
    case 4:
        {
            if(target_bo->getDeepStunDmgExtra() > 0.001f)
                break;
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_STUN, GData::TYPE_DAMAG_A);
            if(ef)
            {
                defList[defCount].damType = e_deepStun;
                target_bo->setStunLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setStunRound(skill->last + 1);
                else
                    target_bo->setStunRound(skill->last);

                target_bo->setDeepStunDmgExtra(ef->value/100, ef->last);
            }
            else if(target_bo->getStunRound() < 1)
            {
                defList[defCount].damType = e_Stun;
                target_bo->setStunLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setStunRound(skill->last + 1);
                else
                    target_bo->setStunRound(skill->last);
            }
        }
        break;
    case 8:
        {
            if(target_bo->getDeepForgetDmgExtra() > 0.001f)
                break;
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_FORGET, GData::TYPE_DAMAG_A);
            if(ef)
            {
                defList[defCount].damType = e_deepForget;
                target_bo->setForgetLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setForgetRound(skill->last + 1);
                else
                    target_bo->setForgetRound(skill->last);

                target_bo->setDeepForgetDmgExtra(ef->value/100, ef->last);
            }
            else if(target_bo->getForgetRound() < 1)
            {
                defList[defCount].damType = e_Forget;
                target_bo->setForgetLevel(SKILL_LEVEL(skill->getId()));
                if(skill->cond == GData::SKILL_BEATKED)
                    target_bo->setForgetRound(skill->last + 1);
                else
                    target_bo->setForgetRound(skill->last);
            }
        }
        break;
    case 0x20:
        if(target_bo->getWeakRound() < 1)
        {
            defList[defCount].damType = e_Weak;
            target_bo->setWeakLevel(SKILL_LEVEL(skill->getId()));
            target_bo->setWeakRound(skill->last);
        }
        break;
    }

    return;
}

bool BattleSimulator::doNormalAttack(BattleFighter* bf, int otherside, int target_pos, std::vector<AttackAct>* atkAct)
{
    if(bf == NULL || bf->getHP() == 0)
        return false;

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
        bool first = true;

		UInt32 dmg = 0;
        // attack only one target
        dmg += attackOnce(bf, first, cs, pr, NULL, target_object, 1, defList, defCount, scList, scCount, 0, NULL, atkAct);

        int self_side = bf->getSide() == otherside ? 25 : 0;
        appendToPacket(bf->getSide(), bf->getPos(), target_pos + self_side, static_cast<UInt8>(0), static_cast<UInt16>(0), cs, pr, defList, defCount, scList, scCount);
        return true;
    }

    return false;
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
        target_pos = getPossibleTarget(bf->getSide(), bf->getPos());
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
                if(!bo)  // 雪人
                    continue;

                if(bo->getAura() > 99 || bo->getId() == 5679)
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

bool BattleSimulator::doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct, UInt32 skillParam)
{
    if(NULL == skill || target_pos < 0 || bf == NULL)
        return false;
    if(skill->effect == NULL)
        return false;

    if(bf->getHP() == 0)
    {
        return false;
    }

    GData::SkillStrengthenBase*  ss = bf->getSkillStrengthen(SKILL_ID(skill->getId()));
    // 雪球-137 小蜘蛛-434
    if(SKILL_ID(skill->getId()) == 137 || SKILL_ID(skill->getId()) == 434 || SKILL_ID(skill->getId()) == 479)
    {
        static UInt8 skill_prob_137[10][3] = {
            {0, 0, 0},
            {97, 99, 100},
            {85, 96, 100},
            {73, 93, 100},
            {61, 90, 100},
            {49, 87, 100},
            {37, 84, 100},
            {25, 81, 100},
            {13, 78, 100},
            {0, 75, 100}
        };
        size_t defCount = 0;
        DefStatus defList[25];
        size_t scCount = 0;
        StatusChange scList[25];

        UInt8 cnt = 0;
        if(SKILL_ID(skill->getId()) == 137)
        {
            UInt8 level = SKILL_LEVEL(skill->getId());
            if(level > 9)
                level = 9;
            UInt8 roll = _rnd(100);
            if(skill_prob_137[level][0] > roll)
                cnt = 1;
            else if(skill_prob_137[level][1] > roll)
                cnt = 2;
            else if(skill_prob_137[level][2] > roll)
                cnt = 3;
        }
        else
            cnt = 2;

        for(UInt8 i = 0; i < cnt; ++i)
        {
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
				++ y;
			}
			if(posCount == 0)
				break;
			bf->addFlag(BattleFighter::Mirrored);
			UInt8 pos = posList[_rnd(posCount)];
            GObject::Fighter * fgt = NULL;
            if(SKILL_ID(skill->getId()) == 137)
                fgt = globalFighters[5679];
            else if(SKILL_ID(skill->getId()) == 434)
                fgt = globalFighters[6011];
            else
                fgt = globalFighters[7006];
            if(fgt == NULL)
                break;
			BattleFighter * newf = new(std::nothrow) Battle::BattleFighter(_formula, fgt, side, pos);
			if(newf == NULL)
				break;
            newf->initStats(false);
			newf->setSideAndPos(side, pos);
			newf->addFlag(BattleFighter::IsMirror);
			setObject(side, pos, newf);
            
            defList[i].pos = pos + 25;
            defList[i].damType = e_Summon; 
            defList[i].damage = fgt->getId();
            defList[i].leftHP = newf->getHP();
            ++defCount;

			//FighterStatus fs(newf);
			// insert fighter into queue by order
			insertFighterStatus(newf);
        }

        setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, -1 * bf->getAura(), 0, scList, scCount, false);

        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 2, skill->getId(), false, false, defList, defCount, scList, scCount);

        return true;
    }

    UInt32 dmg = 0;

    GData::Area* area = NULL;
    area = &(GData::areaList[skill->area]);

    AttackPoint ap[25];
    int apcnt = 0;
    int apcnt2 = 0;

    if(skill->area != 0 && skill->area != 1 && area->getCount() > 0)
    {
        int x_ = target_pos % 5;
        int y_ = target_pos / 5;
        int cnt = area->getCount();
        int fsize = skill->factor.size();
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
            int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
            if(fsize != 0)
                ap[apcnt ++].factor = skill->factor[idx];
            else
                ap[apcnt ++].factor = 1;
        }
        if(ss != NULL)
        {
            apcnt2 = apcnt;
            const std::vector<const GData::SkillStrengthenEffect*>& efs = ss->effect;
            for(int j = 0; j < efs.size(); ++ j)
            {
                if(efs[j]->type != GData::TYPE_AREA)
                    continue;
                GData::Area* area = NULL;
                area = &(GData::areaList[efs[j]->area]);
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
                    ap[apcnt2].pos = x + y * 5;
                    ap[apcnt2 ++].factor = efs[j]->value/100;
                }
                break;
            }
        }
    }

    bool specialEf = true;  // 鹊桥技能需要特殊处理
    bool isQueqiao = false;
    bool bQueqiaoState = false;  // 鹊桥技能，是否主目标上buf
    if(SKILL_ID(skill->getId()) == 147 || SKILL_ID(skill->getId()) == 148 || SKILL_ID(skill->getId()) == 149)
    {
        isQueqiao = true;
        specialEf = false;
        int bfCnt = 1;
        assert(apcnt < 6 && skill->factor.size() == 7); // 必须有足够多的技能伤害因子，否则会出错
        for(int i = 0; i < apcnt; ++ i)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
            if(!bo || bo->getHP() == 0)
                continue;
            // 鹊桥技能按照主次目标的顺序确定技能伤害因子
            ap[i].factor = skill->factor[i+1];
            ++ bfCnt;
        }
        if(bfCnt == 2)
            specialEf = true;

        if(specialEf)  // 如果目标两人，伤害因子统一用factor[6]
        {
            for(int i = 0; i < apcnt; ++i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                if(!bo || bo->getHP() == 0)
                    continue;
                ap[i].factor = skill->factor[6];
            }
            bQueqiaoState = (uRand(10000) > 5000) ? true : false;
        }
//#ifdef _DEBUG
//        fprintf(stderr, "specialEf = %d, isQueqiao = %d, bmainstate = %d\n", specialEf, isQueqiao, bQueqiaoState);
//#endif
    }

    size_t defCount = 0;
    DefStatus defList[250];
    size_t scCount = 0;
    StatusChange scList[250];
    bool cs = false;
    bool pr = false;
    bool first = true;

    memset(defList, 0, sizeof(defList));

    if(ss)
    {
        const GData::SkillStrengthenEffect* ef = ss->getEffect(GData::ON_WEAK, GData::TYPE_IMMUNE);
        if(ef)
            doSkillStrengthen_week(bf, skill, ef, bf->getSide(), bf->getPos(), defList, defCount, scList, scCount, true);
    }
    // 免疫降灵气
    //bool dostatus = true;
    //if(dostatus)
    if(specialEf)
    {
        bool self = false;
        bool flag = ((target_side == bf->getSide()) ? false : true);
        if(1 == skill->area)
        {
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, pos, defList, defCount);
                doSkillStatus(flag, bf, skill, target_side, pos, 1, scList, scCount, self, ifDecAura);
            }
        }
        else if( 0 == skill->area )
        {
            bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos, defList, defCount);
            doSkillStatus(flag, bf, skill, target_side, target_pos, 1, scList, scCount, self, ifDecAura);
        }
        else if(isQueqiao)  // 伤害两个人的鹊桥技能，随机一个人中状态
        {
            bool bDecAura = false;
            if(bQueqiaoState)  // 主目标
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                     bDecAura = isImmuneDecAura(skill, target_side, target_pos, defList, defCount);
                     doSkillStatus(flag, bf, skill, target_side, target_pos, 1, scList, scCount, self, bDecAura);
                }
            }
            else // 分目标
            {
                // 这里虽然写了for循环，但是肯定只有一个活人。。。
                for(int i = 0; i < apcnt; ++i)
                {
                    bDecAura = isImmuneDecAura(skill, target_side, ap[i].pos, defList, defCount);
                    doSkillStatus(flag, bf, skill, target_side, ap[i].pos, 1, scList, scCount, self, bDecAura);
                }
            }
        }
        else
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, target_pos, defList, defCount);
                doSkillStatus(flag, bf, skill, target_side, target_pos, 1, scList, scCount, self, ifDecAura);
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                bool ifDecAura = isImmuneDecAura(skill, target_side, ap[i].pos, defList, defCount);
                doSkillStatus(flag, bf, skill, target_side, ap[i].pos, 1, scList, scCount, self, ifDecAura);
            }

        }
    }

    // therapy skill
    if(skill->effect->hp > 0 || skill->effect->addhp > 0 || skill->effect->hpP > 0.001)
    {
        UInt32 rhp = bf->calcTherapy(cs, first, skill);
        // 释、普渡慈航
        if(SKILL_ID(skill->getId()) == 18)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                return false;

            int cnt = 3;
            UInt8 excepts[25] = {0};
            size_t exceptCnt = 0;
            do
            {
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 rhp2 = rhp * skill->factor[3-cnt];
                    float deFactor = calcTherapyDebuf(bo, defList, defCount);
                    UInt32 hpr = bo->regenHP(rhp2 * deFactor, skill->cond == GData::SKILL_ACTIVE);
                    if(hpr != 0)
                    {
                        excepts[exceptCnt] = bo->getPos();
                        ++ exceptCnt;
                        defList[defCount].pos = bo->getPos() + 25;
                        defList[defCount].damType = e_damHpAdd;
                        defList[defCount].damage = hpr;
                        defList[defCount].leftHP = bo->getHP();
                        ++ defCount;
                        if(skill->cond == GData::SKILL_ACTIVE)
                            releaseWeak(bo, defList, defCount);
                    }
                }

                bo = getTherapyTarget2(bf, excepts, exceptCnt);

            }while(--cnt);
        }
        else if(bf->getSide() != target_side)
        {
            float deFactor = calcTherapyDebuf(bf, defList, defCount);
            UInt32 hpr = bf->regenHP(rhp * deFactor, skill->cond == GData::SKILL_ACTIVE);

            if(hpr != 0)
            {
                defList[defCount].pos = bf->getPos() + 25;
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bf->getHP();
                ++ defCount;
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bf, defList, defCount);
            }
        }
        else if(1 == skill->area)
        {
            int i = 0;
            int fsize = skill->factor.size();
            for(UInt8 pos = 0; pos < 25; ++ pos)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                    continue;

                if(bo->getLostHP() == 0)
                    continue;

                float factor = 1;
                int idx = std::min(abs(fsize-1), i);
                if(fsize > 0)
                    factor = skill->factor[idx];

                float deFactor = calcTherapyDebuf(bo, defList, defCount);
                UInt32 hpr = bo->regenHP(rhp * factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                ++i;
                if(hpr == 0)
                    continue;

                defList[defCount].pos = pos + 25;
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bo, defList, defCount);
            }
        }
        else if(0 == skill->area)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                return false;

            float deFactor = calcTherapyDebuf(bo, defList, defCount);
            UInt32 hpr = bo->regenHP(rhp * deFactor, skill->cond == GData::SKILL_ACTIVE);
            if(hpr != 0)
            {
                defList[defCount].pos = bo->getPos() + 25;
                defList[defCount].damType = e_damHpAdd;
                defList[defCount].damage = hpr;
                defList[defCount].leftHP = bo->getHP();
                ++ defCount;
                if(skill->cond == GData::SKILL_ACTIVE)
                    releaseWeak(bo, defList, defCount);
            }
        }
        else
        {
            int fsize = skill->factor.size();
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(bo != NULL && bo->getHP() != 0 && bo->isChar())
            {
                float factor = 1;
                if(fsize > 0)
                    factor = skill->factor[0];
                float deFactor = calcTherapyDebuf(bo, defList, defCount);
                UInt32 hpr = bo->regenHP(rhp * factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                if(hpr != 0)
                {
                    defList[defCount].pos = bo->getPos() + 25;
                    defList[defCount].damType = e_damHpAdd;
                    defList[defCount].damage = hpr;
                    defList[defCount].leftHP = bo->getHP();
                    ++ defCount;
                    if(skill->cond == GData::SKILL_ACTIVE)
                        releaseWeak(bo, defList, defCount);
                }
            }

            for(int i = 0; i < apcnt; ++ i)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                if(NULL == bo)
                    continue;

                float deFactor = calcTherapyDebuf(bo, defList, defCount);
                UInt32 hpr = bo->regenHP(rhp * ap[i].factor * deFactor, skill->cond == GData::SKILL_ACTIVE);
                if(hpr != 0)
                {
                    defList[defCount].pos = bo->getPos() + 25;
                    defList[defCount].damType = e_damHpAdd;
                    defList[defCount].damage = hpr;
                    defList[defCount].leftHP = bo->getHP();
                    ++ defCount;
                    if(skill->cond == GData::SKILL_ACTIVE)
                        releaseWeak(bo, defList, defCount);
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
                int fsize = skill->factor.size();
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        dmg *= skill->factor[idx];
                    ++i;
                    bo->makeDamage(dmg);
                    defList[defCount].pos = pos;
                    defList[defCount].damType = e_damNormal;
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;

                    if(bo->getHP() == 0)
                    {
                        onDead(false, bo, defList, defCount);
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
                        onDead(false, bo, defList, defCount);
                    }
                }
            }
            else
            {
                int fsize = skill->factor.size();
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    UInt32 dmg = abs(bf->calcPoison(skill, bo, false));
                    if(fsize > 0)
                        dmg *= skill->factor[0];
                    bo->makeDamage(dmg);
                    defList[defCount].pos = target_pos;
                    defList[defCount].damType = e_damNormal;
                    defList[defCount].damage = dmg;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;

                    if(bo->getHP() == 0)
                    {
                        onDead(false, bo, defList, defCount);
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
                        onDead(false, bo, defList, defCount);
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
                if(skill->effect->disperse & 2 && SKILL_LEVEL(skill->getId()) >= bo->getConfuseLevel())
                {
                    bo->setConfuseLevel(0);
                    bo->setConfuseRound(0);
                }

                if(skill->effect->disperse & 4 && SKILL_LEVEL(skill->getId()) >= bo->getStunLevel())
                {
                    bo->setStunLevel(0);
                    bo->setStunRound(0);
                }

                if(skill->effect->disperse & 8 && SKILL_LEVEL(skill->getId()) >= bo->getForgetLevel())
                {
                    bo->setForgetLevel(0);
                    bo->setForgetRound(0);
                }

                if(skill->effect->disperse & 0x20 && SKILL_LEVEL(skill->getId()) >= bo->getWeakLevel())
                {
                    bo->setWeakLevel(0);
                    bo->setWeakRound(0);
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
        //儒、元磁神雷
        if(SKILL_ID(skill->getId()) == 122 || SKILL_ID(skill->getId()) == 477)
        {
            static UInt8 skill_prob_122[10][4] = {
                {0, 0, 0},
                {70, 100, 0, 0},
                {60, 100, 0, 0},
                {50, 100, 0, 0},
                {40, 100, 0, 0},
                {30, 90,  100, 0},
                {20, 80,  100, 0},
                {20, 60,  90,  100},
                {10, 40,  80,  100},
                {0,  20,  70,  100}
            };
            UInt8 cnt = 0;
            UInt8 level = SKILL_LEVEL(skill->getId());
            if(level > 9)
                level = 9;
            UInt8 roll = _rnd(100);
            if(skill_prob_122[level][0] > roll)
                cnt = 0;
            else if(skill_prob_122[level][1] > roll)
                cnt = 1;
            else if(skill_prob_122[level][2] > roll)
                cnt = 2;
            else if(skill_prob_122[level][3] > roll)
                cnt = 3;
            for(int i = 0; i < cnt; ++ i)
            {
                dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], 1, defList, defCount, scList, scCount);
            }
            dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], 1, defList, defCount, scList, scCount, 0, NULL, atkAct);
 
        }
        //道、万剑诀
        else if(SKILL_ID(skill->getId()) == 27)
        {
            UInt8 dmgCount[25] = {0};
            int cnt = 8;
            for(int i = 0; i < cnt; ++ i)
            {
                BattleFighter* rnd_bf = getRandomFighter(target_side, NULL, 0);
                dmg += attackOnce(bf, first, cs, pr, skill, rnd_bf, 1, defList, defCount, scList, scCount);
                if(rnd_bf)
                    ++ dmgCount[rnd_bf->getPos()];
            }
            dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], 1, defList, defCount, scList, scCount, 0, NULL, atkAct);
            ++ dmgCount[target_pos];
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_DAMAGE, GData::TYPE_DEBUF_THERAPY);
            if(ef)
            {
                for(int i = 0; i < 25; ++ i)
                {
                    if(dmgCount[i] < 3)
                        continue;

                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][i]);
                    if(bo->getHP() == 0 && bo->getTherapyDecLast() != 0)
                        continue;

                    defList[defCount].damType = e_neishan;
                    defList[defCount].damage = 0;
                    defList[defCount].pos = i;
                    defList[defCount].leftHP = bo->getHP();
                    defCount ++;
                    bo->setTherapyDec(ef->value/100, ef->last);
                }
            }
        }
        //儒、青莲剑歌
        else if(SKILL_ID(skill->getId()) == 9)
        {
            int idx = 0;
            for(int i = 0; i < apcnt2; ++ i)
            {
                BattleObject * area_target_obj = _objs[target_side][ap[i].pos];
                if(area_target_obj == NULL || area_target_obj->getHP() == 0 || bf == NULL)
                    continue;
                ++ idx;
            }

            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > idx)
                factor = skill->factor[idx];
            else
                factor = skill->factor[fsize-1];

            dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], factor, defList, defCount, scList, scCount, apcnt, ap, atkAct);
            for(int j = 0; j < apcnt; ++ j)
                dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][ap[j].pos], factor, defList, defCount, scList, scCount);
            for(int k = apcnt; k < apcnt2; ++ k)
                dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][ap[k].pos], factor*ap[k].factor, defList, defCount, scList, scCount);

        }
        else if(0 == skill->area)
        {
            dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], 1, defList, defCount, scList, scCount, 0, NULL, atkAct);

            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
            {
                if(cs)
                    ef = ss->getEffect(GData::ON_CRITICAL, GData::TYPE_DAMAG_B);
                if(!ef && pr)
                    ef = ss->getEffect(GData::ON_PIERCE, GData::TYPE_DAMAG_B);
            }
            if(ef)
            {
                int pos2 = -1;
                for(int pos = target_pos + 5; pos < 25; pos += 5)
                {
                    if(_objs[target_side][pos] != NULL && _objs[target_side][pos]->getHP() != 0)
                    {
                        pos2 = pos;
                        break;
                    }
                }

                if(pos2 != -1)
                    dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][pos2], ef->value/100, defList, defCount, scList, scCount);
            }
        }
        else if(1 == skill->area)
        {
            int i = 0;
            int fsize = skill->factor.size();
            for(UInt8 pos = 0; pos < 25; pos++)
            {
                if(_objs[target_side][pos] == NULL || _objs[target_side][pos]->getHP() == 0)
                    continue;
                float factor = 1;
                int idx = std::min(abs(fsize-1), i);
                if(fsize > 0)
                    factor = skill->factor[idx];
                dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][pos], factor, defList, defCount, scList, scCount);
                ++i;
            }
        }
        else if(specialEf && isQueqiao)
        {
            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > 0)
                factor = skill->factor[fsize-1];
            dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], factor, defList, defCount, scList, scCount, apcnt, ap, atkAct);
            for(int i = 0; i < apcnt; ++ i)
            {
                dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][ap[i].pos], factor, defList, defCount, scList, scCount);
            }
        }
        else
        {
            const GData::SkillStrengthenEffect* ef = NULL;
            if(ss)
                ef = ss->getEffect(GData::ON_SPREAD, GData::TYPE_XINMO);
            // attack the target on center
            int fsize = skill->factor.size();
            float factor = 1;
            if(fsize > 0)
                factor = skill->factor[0];
            dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][target_pos], skill->factor[0], defList, defCount, scList, scCount, apcnt, ap, atkAct);
            for(int i = 0; i < apcnt; ++ i)
            {
                dmg += attackOnce(bf, first, cs, pr, skill, _objs[target_side][ap[i].pos], ap[i].factor, defList, defCount, scList, scCount);
            }
            if(ef)
            {
                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bf = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                    if(!bf || bf->getHP() == 0)
                        continue;
                    if(_rnd(10000) < ef->value * 100)
                    {
                        doXinmoAttack(bf, _objs[target_side][target_pos], defList, defCount, scList, scCount);
                        break;
                    }
                }
            }
        }
    }

    // 混乱晕眩封印状态
    if((skill->effect->state & 0x2e) && specialEf)
    {
        float rate = skill->prob * 100;
	    if(bf->getSide() != target_side)
        {
            if(1 == skill->area)
            {
                int i = 0;
                int fsize = skill->factor.size();
                for(UInt8 pos = 0; pos < 25; ++ pos)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    float factor = 1;
                    int idx = std::min(abs(fsize-1), i);
                    if(fsize > 0)
                        factor = skill->factor[idx];
                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || (rate * factor) > _rnd(10000)))
                    {
                        defList[defCount].damage = 0;
                        defList[defCount].pos = pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, NULL, NULL);
                        defCount ++;
                    }
                    ++ i;
                }
            }
            else if(0 == skill->area)
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                    {
                        std::vector<AttackAct> atkAct2;
                        atkAct2.clear();
                        defList[defCount].damage = 0;
                        defList[defCount].pos = target_pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                        defCount ++;
                        doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
                    }
                }
            }
            else if(isQueqiao) // 两个目标的鹊桥，随机一个上状态
            {
                int ntarpos = target_pos;
                BattleFighter* bo = NULL;
                if(bQueqiaoState) // 主目标上buf
                {
                    bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar()) // 无效的目标
                        bo = NULL;
                }
                else
                {
                    for(int i = 0; i < apcnt; ++ i)
                    {
                        bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                        if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                        {
                            ntarpos = ap[i].pos;
                            break; // 找到目标了
                        }
                    }
                }
               if(bo != NULL)
               {
                   if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || rate > _rnd(10000)))
                   {
//#ifdef _DEBUG
//                       fprintf(stderr, "i beat you to dizz!! rate = %f\n", rate);
//#endif
                       std::vector<AttackAct> atkAct2;
                       atkAct2.clear();
                       defList[defCount].damage = 0;
                       defList[defCount].pos = ntarpos;
                       defList[defCount].leftHP = bo->getHP();
                       doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                       defCount ++;
                       doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
                   }
               }
            }
            else
            {
                BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
                if(bo != NULL && bo->getHP() != 0 && bo->isChar())
                {
                    int fsize = skill->factor.size();
                    float factor = 1;
                    if(fsize > 0)
                        factor = skill->factor[0];
                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || (rate * factor) > _rnd(10000)))
                    {
                        std::vector<AttackAct> atkAct2;
                        atkAct2.clear();
                        defList[defCount].damage = 0;
                        defList[defCount].pos = target_pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                        defCount ++;
                        doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
                    }
                }

                for(int i = 0; i < apcnt; ++ i)
                {
                    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][ap[i].pos]);
                    if(bo == NULL || bo->getHP() == 0 || !bo->isChar())
                        continue;

                    if(((skill->cond != GData::SKILL_ACTIVE && skill->cond != GData::SKILL_PEERLESS) || (rate * ap[i].factor) > _rnd(10000)))
                    {
                        std::vector<AttackAct> atkAct2;
                        atkAct2.clear();
                        defList[defCount].damage = 0;
                        defList[defCount].pos = ap[i].pos;
                        defList[defCount].leftHP = bo->getHP();
                        doSkillState(bf, skill, bo, defList, defCount, &atkAct2, atkAct);
                        defCount ++;
                        doSkillAtk2(false, &atkAct2, defList, defCount, scList, scCount);
                    }
                }
            }
        }
    }

    if(ss && bf->getHP() != 0)
    {
        const std::vector<const GData::SkillStrengthenEffect*>& efs = ss->effect;
        for(int i = 0; i < efs.size(); ++ i)
        {
            const GData::SkillStrengthenEffect* ef = efs[i];
            if(ef->target == 3)
                doSkillStrengthenAttack(bf, skill, ef, bf->getSide(), bf->getPos(), defList, defCount, scList, scCount, true);
            else
                doSkillStrengthenAttack(bf, skill, ef, target_side, target_pos, defList, defCount, scList, scCount, true);
        }
    }


    if (skill && skill->cond == GData::SKILL_PEERLESS)
    {
        setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAura, -1 * bf->getAura(), 0, scList, scCount, false);
    }

    int self_side = bf->getSide() == target_side ? 25 : 0;
    appendToPacket( bf->getSide(), bf->getPos(), target_pos + self_side, 2, skill->getId(), cs, pr, defList, defCount, scList, scCount);
    return true;
}

bool BattleSimulator::doSkillStatus2(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt ,StatusChange* scList, size_t& scCount)
{
    if(NULL == skill || bf == NULL)
        return false;

    if(skill->effect == NULL)
        return false;

    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
    if(NULL == bo)
        return false;

    if(skill->effect->auraP || skill->effect->aura)
    {
        float rate = skill->prob * 100;
        if(rate > _rnd(10000))
        {
            float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
            if(value > 0 && bf->getSide() != target_side)
            {
                float value = bf->_aura * skill->effect->auraP + skill->effect->aura;
                setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAura, value, skill->last, scList, scCount, target_side != 0);
            }
            else
            {
                setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAura, value, skill->last, scList, scCount, bf->getSide() != 0);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAtk, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtk, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_defend * skill->effect->defP + skill->effect->def;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stDef, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stDef, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtk, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagDef, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stTough, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stTough, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->action || skill->effect->actionP)
    {
        float value = bo->_maxAction * skill->effect->actionP + skill->effect->action;
        if(value > 0 && bf->getSide() != target_side)
        {
            float value = bf->_maxAction * skill->effect->actionP + skill->effect->action;
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAction, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAction, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stEvade, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stEvade, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCritical, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCritical, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stPierce, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stPierce, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stCounter, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stCounter, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagRes, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->hitrate)
    {
        float value = skill->effect->hitrate;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stHitRate, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stHitRate, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->atkreduce)
    {
        float value = skill->effect->atkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stAtkReduce, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stAtkReduce, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    if(skill->effect->magatkreduce)
    {
        float value = skill->effect->magatkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            setStatusChange2(bf, bf->getSide(), bf->getPos(), 1, skill->getId(), e_stMagAtkReduce, value, skill->last, scList, scCount, target_side != 0);
        }
        else
        {
            setStatusChange2(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill->getId(), e_stMagAtkReduce, value, skill->last, scList, scCount, bf->getSide() != 0);
        }
    }

    return true;
}


bool BattleSimulator::doSkillStatus(bool activeFlag, BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt ,StatusChange* scList, size_t& scCount, bool& self, bool ifDecAura)
{
    if(NULL == skill || bf == NULL)
        return false;
    if(skill->effect == NULL)
        return false;

    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
    if(NULL == bo)
        return false;

    bool tmpself = self;
    if(skill->effect->auraP || skill->effect->aura)
    {
        if((SKILL_ID(skill->getId()) == 136 || SKILL_ID(skill->getId()) == 146) && bf->getClass() != 2) // 光棍, 冰火双剪
            return false;

        float rate = skill->prob * 100;
        if(rate > _rnd(10000))
        {
            float value = bo->_aura * skill->effect->auraP + skill->effect->aura;
            if(value > 0 && bf->getSide() != target_side)
            {
                if(!self)
                {
                    float value = bf->_aura * skill->effect->auraP + skill->effect->aura;
                    setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAura, value, skill->last, scList, scCount, !activeFlag);
                    tmpself = true;
                }
            }
            else if(!ifDecAura)
            {
                setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAura, value, skill->last, scList, scCount, activeFlag);
            }
        }
    }

    if(skill->effect->atkP || skill->effect->atk)
    {
        float value = bo->_attack * skill->effect->atkP + skill->effect->atk;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_attack * skill->effect->atkP + skill->effect->atk;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAtk, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAtk, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->defP || skill->effect->def)
    {
        float value = bo->_defend * skill->effect->defP + skill->effect->def;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_defend * skill->effect->defP + skill->effect->def;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stDef, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stDef, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->magatkP || skill->effect->magatk)
    {
        float value = bo->_magatk * skill->effect->magatkP + skill->effect->magatk;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_magatk * skill->effect->magatkP + skill->effect->magatk;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagAtk, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagAtk, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->magdefP || skill->effect->magdef)
    {
        float value = bo->_magdef * skill->effect->magdefP + skill->effect->magdef;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_magdef * skill->effect->magdefP + skill->effect->magdef;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagDef, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagDef, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->tough)
    {
        float value = skill->effect->tough;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stTough, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stTough, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->action || skill->effect->actionP)
    {
        float value = bo->_maxAction * skill->effect->actionP + skill->effect->action;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                float value = bf->_maxAction * skill->effect->actionP + skill->effect->action;
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAction, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAction, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->evade)
    {
        float value = skill->effect->evade;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stEvade, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stEvade, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->critical)
    {
        float value = skill->effect->critical;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stCritical, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stCritical, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->pierce)
    {
        float value = skill->effect->pierce;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stPierce, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stPierce, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->counter)
    {
        float value = skill->effect->counter;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stCounter, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stCounter, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->magres)
    {
        float value = skill->effect->magres;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagRes, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagRes, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->hitrate)
    {
        float value = skill->effect->hitrate;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stHitRate, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stHitRate, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->atkreduce)
    {
        float value = skill->effect->atkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stAtkReduce, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stAtkReduce, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(skill->effect->magatkreduce)
    {
        float value = skill->effect->magatkreduce;
        if(value > 0 && bf->getSide() != target_side)
        {
            if(!self)
            {
                setStatusChange(bf, bf->getSide(), bf->getPos(), 1, skill, e_stMagAtkReduce, value, skill->last, scList, scCount, !activeFlag);
                tmpself = true;
            }
        }
        else
        {
            setStatusChange(bf, target_side, bo == NULL ? 0 : bo->getPos(), cnt, skill, e_stMagAtkReduce, value, skill->last, scList, scCount, activeFlag);
        }
    }

    if(self != tmpself)
        self = tmpself;

    return true;
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

BattleFighter* BattleSimulator::getTherapyTarget2(BattleFighter* bf, UInt8 * excepts, size_t exceptCount)
{
    UInt8 side = bf->getSide();
    BattleFighter* bo = NULL;
    UInt32 maxHpLost = 0;
    UInt8 pos = 0;
	for(UInt8 i = 0; i < 25; ++ i)
	{
		bo = static_cast<BattleFighter*>(_objs[side][i]);
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

        UInt32 hp = bo->getHP();
        UInt32 maxHp = bo->getMaxHP();
        if(hp < (maxHp >> 1))
        {
            return bo;
        }

        if(maxHp - hp > maxHpLost)
        {
            maxHpLost = maxHp - hp;
            pos = i;
        }
	}

    if(maxHpLost != 0)
		bo = static_cast<BattleFighter*>(_objs[side][pos]);

    return bo;
}

UInt32 BattleSimulator::FightersEnter(UInt8 prevWin)
{
    UInt32 rcnt = 0;

    std::vector<BattleFighter*>  cur_fgtlist = _fgtlist[_cur_fgtlist_idx];

    size_t cnt = cur_fgtlist.size();
    size_t defCount = 0;
    DefStatus defList[300];
    size_t scCount = 0;
    StatusChange scList[300];

    for(size_t idx = 0; idx < cnt; idx++)
    {
        BattleFighter* bf = cur_fgtlist[idx];

        if((prevWin-1) != bf->getSide())
        {
            const GData::SkillBase* passiveSkill = NULL;
            size_t skillIdx = 0;
            while(NULL != (passiveSkill = bf->getPassiveSkillEnter100(skillIdx)))
            {
                int target_side, target_pos, cnt;
                getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
                UInt32 res = doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt, defList, defCount, scList, scCount);
                if(rcnt == 0 && res != 0)
                    rcnt = 1;
            }

            if(NULL != (passiveSkill = bf->getPassiveSkillEnter()))
            {
                int target_side, target_pos, cnt;
                getSkillTarget(bf, passiveSkill, target_side, target_pos, cnt);
                UInt32 res = doSkillAttackAftEnter(bf, passiveSkill, target_side, target_pos, cnt, defList, defCount, scList, scCount);
                if(rcnt == 0 && res != 0)
                    rcnt = 1;
            }
        }
    }

    if(rcnt != 0)
        appendToPacket(0, -1, -1, 0, 0, false, false, defList, defCount, scList, scCount);

    return rcnt;
}

UInt32 BattleSimulator::doSkillAttackAftEnter(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount)
{
    UInt32 rcnt = 0;
    if(skill->cond != GData::SKILL_ENTER)
    {
        return rcnt;
    }

    do {
        int self_side = bf->getSide() == 0 ? 25 : 0;
        defList[defCount].pos = bf->getPos() + self_side;
        defList[defCount].damType = e_skill;
        defList[defCount].damage = skill->getId();
        defList[defCount].leftHP = bf->getHP();
        ++ defCount;

        if(cnt == 1)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
            if(NULL == bo)
                break;

            if(skill->effect->immune)
            {
                bo->setImmune(skill->effect->immune);
                bo->setImmuneLevel(skill->effect->immune, SKILL_LEVEL(skill->getId()));
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

            int self_side = bo->getSide() == 0 ? 25 : 0;
            defList[defCount].pos = bo->getPos() + self_side;
            defList[defCount].damType = e_damNormal;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++defCount;
 
            rcnt = 1;
            break;
        }

        for(int pos = 0; pos < cnt; pos++)
        {
            BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][pos]);
            if(NULL == bo)
                continue;

            if(skill->effect->immune)
            {
                bo->setImmune(skill->effect->immune);
                bo->setImmuneLevel(skill->effect->immune, SKILL_LEVEL(skill->getId()));
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

            int self_side = bo->getSide() == 0 ? 25 : 0;
            defList[defCount].pos = bo->getPos() + self_side;
            defList[defCount].damType = e_damNormal;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++defCount;
 
        }

        rcnt = 1;
    } while(false);

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

    insertFighterStatus(bf);

    do {
        rcnt += doDeBufAttack(bf);
        if(stun > 0)
        {
            break;
        }

        // 雪人
        if(bf->getId() == 5679)
            break;

        int target_pos;
        int otherside = 1 - bf->getSide();
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
                break;

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            if(doNormalAttack(bf, otherside, target_pos, &atkAct))
                ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                if(atkAct[idx].bf->getHP() == 0)
                    continue;
                if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                    ++ rcnt;
            }

            atkAct.clear();

            break;
        }
        else if(forget > 0)
        {
            target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

            if(target_pos < 0)
                break;

            std::vector<AttackAct> atkAct;
            atkAct.clear();
            if(doNormalAttack(bf, otherside, target_pos, &atkAct))
                ++ rcnt;

            size_t actCnt = atkAct.size();
            for(size_t idx = 0; idx < actCnt; idx++)
            {
                if(atkAct[idx].bf->getHP() == 0)
                    continue;
                if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                    ++ rcnt;
            }
            atkAct.clear();

            break;
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
                    break;

                std::vector<AttackAct> atkAct;
                atkAct.clear();
                if(doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct))
                    ++ rcnt;

                size_t actCnt = atkAct.size();
                for(size_t idx = 0; idx < actCnt; idx++)
                {
                    if(atkAct[idx].bf->getHP() == 0)
                        continue;
                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                        ++ rcnt;
                }
                atkAct.clear();
            }

            skill = bf->getPassiveSkillPreAtk();
            if(NULL != skill)
            {
                getSkillTarget(bf, skill, otherside, target_pos, cnt);
                if(cnt < 1)
                    break;

                std::vector<AttackAct> atkAct;
                atkAct.clear();
                if(doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct))
                    ++ rcnt;

                size_t actCnt = atkAct.size();
                for(size_t idx = 0; idx < actCnt; idx++)
                {
                    if(atkAct[idx].bf->getHP() == 0)
                        continue;
                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                        ++ rcnt;
                }
                atkAct.clear();
            }

            // do active skill
            BattleFighter* therapy_bf = getTherapyTarget(bf);
            skill = bf->getActiveSkill(therapy_bf!= NULL);
            bool canNormal = true;
            if(NULL != skill)
            {
                canNormal = false;
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
                    break;

                std::vector<AttackAct> atkAct;
                atkAct.clear();
                if(doSkillAttack(bf, skill, otherside, target_pos, cnt, &atkAct))
                    ++ rcnt;

                size_t actCnt = atkAct.size();
                for(size_t idx = 0; idx < actCnt; idx++)
                {
                    if(atkAct[idx].bf->getHP() == 0)
                        continue;
                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                        ++ rcnt;
                }
                atkAct.clear();

                // 昊天镜
                if(SKILL_ID(skill->getId()) == 132)
                {
                    float rate = skill->prob * 100;
                    if(rate > _rnd(10000))
                    {
                        canNormal = true;
                    }
                }
            }

            if(canNormal)
            {
                target_pos = getPossibleTarget(bf->getSide(), bf->getPos());

                if(target_pos < 0)
                    break;

                std::vector<AttackAct> atkAct;
                atkAct.clear();
                if(doNormalAttack(bf, otherside, target_pos, &atkAct))
                    ++ rcnt;

                size_t actCnt = atkAct.size();
                for(size_t idx = 0; idx < actCnt; idx++)
                {
                    if(atkAct[idx].bf->getHP() == 0)
                        continue;
                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
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

                        int cnt = 0;
                        getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                            ++ rcnt;

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                                ++ rcnt;
                        }
                        atkAct.clear();
                    }

                    passiveSkill = bf->getPassiveSkillAftNAtk();
                    if(NULL != passiveSkill && (passiveSkill->target != 1 || (passiveSkill->target == 1 && bo && bo->getHP() > 0)) )
                    {
                        int cnt = 0;
                        getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                        std::vector<AttackAct> atkAct;
                        atkAct.clear();
                        if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                            ++ rcnt;

                        size_t actCnt = atkAct.size();
                        for(size_t idx = 0; idx < actCnt; idx++)
                        {
                            if(atkAct[idx].bf->getHP() == 0)
                                continue;
                            if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
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

                int cnt = 0;
                getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                std::vector<AttackAct> atkAct;
                atkAct.clear();
                if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                    ++ rcnt;

                size_t actCnt = atkAct.size();
                for(size_t idx = 0; idx < actCnt; idx++)
                {
                    if(atkAct[idx].bf->getHP() == 0)
                        continue;
                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
                        ++ rcnt;
                }
                atkAct.clear();
            }

            passiveSkill = bf->getPassiveSkillAftAtk();
            if(NULL != passiveSkill && (passiveSkill->target != 1 || (passiveSkill->target == 1 && bo && bo->getHP() > 0)) )
            {
                int cnt = 0;
                getSkillTarget(bf, passiveSkill, otherside, target_pos, cnt);
                std::vector<AttackAct> atkAct;
                atkAct.clear();
                if(doSkillAttack(bf, passiveSkill, otherside, target_pos, cnt, &atkAct))
                    ++ rcnt;

                size_t actCnt = atkAct.size();
                for(size_t idx = 0; idx < actCnt; idx++)
                {
                    if(atkAct[idx].bf->getHP() == 0)
                        continue;
                    if(doSkillAttack(atkAct[idx].bf, atkAct[idx].skill, atkAct[idx].target_side, atkAct[idx].target_pos, 1, NULL, atkAct[idx].param))
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
    }
    while(false);

    rcnt += releaseCD(bf);
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

int BattleSimulator::testWinner2()
{
    if(_player[1] == NULL)
        return 2;

    UInt32 leftHPAll[2] = {0, 0};
	for(Int8 fgtlist_idx = 0; fgtlist_idx < 2; fgtlist_idx++)
    {
        std::vector<BattleFighter*>& fgtlist = _fgtlist[fgtlist_idx];
        size_t c = fgtlist.size();
        for(size_t i = 0; i < c; ++ i)
        {
            leftHPAll[fgtlist[i]->getSide()] += fgtlist[i]->getHP();
        }
    }

    return leftHPAll[0] > leftHPAll[1] ? 1 : 2;
}

void BattleSimulator::appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr, DefStatus* defList, size_t defCount, StatusChange * scList, size_t scCount)
{
	_packet << static_cast<UInt8>(from_side);
	// attack mode
	_packet << static_cast<UInt8>(atk_type | (cs ? 0x80 : 0) | (pr ? 0x40 : 0)) << add_id;

    //攻击成就判断
    if(from_side < 2 && _player[from_side])
    {
        if(cs)
        {
            _csNum[from_side] ++ ;
            /*
            if(_csNum[from_side] ==3)
                 _cs3ok[from_side] = true;

            if(_csNum[from_side] == 9)
                _cs9ok[from_side] = true;
                */
            _maxCS[from_side] = std::max(  _csNum[from_side],  _maxCS[from_side]);
        }
        if(pr)
        {
            _prNum[from_side] ++ ;
            /*
            if(_prNum[from_side] == 3)
                  _pr3ok[from_side] = true;
            if(_prNum[from_side]== 9)
                _pr9ok[from_side] = true;*/
             _maxPR[from_side] =std:: max(  _prNum[from_side],  _maxPR[from_side]);
        }
    }
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

        //成就判断
        if(from_side<2)
        {
            bool bDamage = defList[i].damage != 0;

            if( bDamage)
            {
                if(!cs)
                    _csNum[from_side] = 0; //打断连续情况
                if(!pr)
                    _prNum[from_side] = 0;
            }
            UInt8 o_side = (from_side + 1) %2;
            if(defList[i].damType == e_damEvade)
            {
                 _evadeNum[o_side] ++ ;
                 /*
                 if(_evadeNum[o_side] == 3)
                    _evade3OK[o_side] = true;
                 if(_evadeNum[o_side] == 9)
                    _evade9ok[o_side] = true;*/
                 _maxEvade[o_side] = std::max(_maxEvade[o_side], _evadeNum[o_side]);
            }
            else if( bDamage)
            {
                _evadeNum[o_side] = 0;
            }

            if((defList[i].damType2 & 0x80) == 0x80)
            {
                _fjNum[o_side] ++ ;
                /*
                if(_fjNum [o_side] == 3)
                    _fj3ok[o_side] = true;
                if(_fjNum[o_side] == 9)
                    _fj9ok[o_side] = true;*/
                _maxFJ[o_side] = std::max( _maxFJ[o_side] ,  _fjNum[o_side]);
            }
            else if(bDamage)
            {
                _fjNum[o_side] = 0;
            }
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
	UInt32 autohp = 0; // player->getBuffData(PLAYER_BUFF_AUTOHEAL, 0);
	for(int j = 0; j < 5; ++ j)
	{
		GObject::Lineup& pd = player->getLineup(j);
		if(pd.fighter == NULL)
			continue;
		BattleObject * obj = _objs[side][pd.pos];
		if(obj != NULL && obj->isChar())
		{
			BattleFighter * bf = static_cast<BattleFighter *>(obj);
			UInt32 hp = bf->getHP();

			UInt32 oldmaxhp = pd.fighter->getMaxHP();
			UInt32 currhp = pd.fighter->getCurrentHP();
			if(currhp == 0)
				currhp = oldmaxhp;
			UInt32 newhp = (float)hp / bf->getMaxHP() * oldmaxhp;
			if(newhp > currhp)
				newhp = currhp;
			if(useRegen)
			{
				if(newhp == 0)
                    newhp = 1;
                if (!World::getAutoHeal())
                {
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
                    newhp = 0;
                    res = true;
                }
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
	// player->setBuffData(0, autohp);
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
			UInt32 hp_ = bf->getHP();
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
					sc.data = static_cast<UInt32>(bfgt->getAction());
					break;
				default:
					bfgt->setHP(value);
					sc.data = static_cast<UInt32>(bfgt->getHP());
					break;
				}
				++ scCount;
			}
		}
	}
}

void BattleSimulator::setStatusChange2(BattleFighter* bf, UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
	for(UInt8 i = pos; i < pos + cnt; ++ i)
	{
		if(_objs[side][i] != NULL)
		{
			BattleObject * bo = _objs[side][i];
			if(bo->isChar() && _isBody[side][i] == 0 && bo->getHP() > 0)
			{
                (this->*statusFuncTable2[type])(bf, side, i, skillId, value, last, scList, scCount, active);
#if 0
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
                case e_stCriticalDmg:
                    bfgt->setCriticalDmgAdd2(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCriticalDmg() * 100);
                    break;
                case e_stHitRate:
                    bfgt->setHitrateAdd2(value);
                    sc.data = static_cast<UInt16>(bfgt->getHitrate(NULL));
                    break;
                case e_stAtkReduce:
                    bfgt->setAtkReduce2(value);
                    sc.data = static_cast<UInt16>(bfgt->getAtkReduce());
                    break;
                case e_stMagAtkReduce:
                    bfgt->setMagAtkReduce2(value);
                    sc.data = static_cast<UInt16>(bfgt->getMagAtkReduce());
                    break;
				}
				++ scCount;
#endif
			}
		}
	}
}


void BattleSimulator::setStatusChange(BattleFighter * bf, UInt8 side, UInt8 pos, int cnt, const GData::SkillBase* skill, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
	for(UInt8 i = pos; i < pos+cnt; ++ i)
	{
		if(_objs[side][i] != NULL)
		{
			BattleObject * bo = _objs[side][i];
			if(bo->isChar() && _isBody[side][pos] == 0 && bo->getHP() > 0)
			{
                (this->*statusFuncTable[type])(bf, side, i, skill, value, last, scList, scCount, active);
#if 0
				BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
				StatusChange& sc = scList[scCount];
				sc.pos = static_cast<UInt8>(i);
				if(!active)
					sc.pos += 25;
                if(skill)
                    sc.statusId = skill->getId();
                else
                    sc.statusId = 0;
				sc.type = type;
                if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
                    ++last;
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
                case e_stCriticalDmg:
                    bfgt->setCriticalDmgAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getCriticalDmg() * 100);
                    break;
                case e_stHitRate:
                    bfgt->setHitrateAdd(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getHitrate(NULL));
                    break;
                case e_stAtkReduce:
                    bfgt->setAtkReduce(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getAtkReduce());
                    break;
                case e_stMagAtkReduce:
                    bfgt->setMagAtkReduce(value, last);
                    sc.data = static_cast<UInt16>(bfgt->getMagAtkReduce());
                    break;
				}
				++ scCount;
#endif
			}
		}
	}
}

bool BattleSimulator::onDead(bool activeFlag, BattleObject * bo, DefStatus* defList, size_t& defCount)
{
	if(!bo->isChar())
		return true;

    bool fRevival = false;
    bool fFakeDead = false;
    size_t idx = 0;
    const GData::SkillBase* passiveSkill = NULL;
    passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead100(idx);
    if(passiveSkill == NULL)
        passiveSkill = (static_cast<BattleFighter*>(bo))->getPassiveSkillDead();

    if(passiveSkill != NULL)
    {
        switch(SKILL_ID(passiveSkill->getId()))
        {
        case 226:
            {
                size_t cnt = passiveSkill->factor.size();
                UInt16 i = (static_cast<BattleFighter*>(bo))->getFakeDeadTimes();
                if(i >= cnt)
                    i = cnt - 1;

                if(uRand(10000) < passiveSkill->prob * passiveSkill->factor[i] * 100)
                {
                    fFakeDead = true;
                    (static_cast<BattleFighter*>(bo))->fakeDead();
                }
            }
            break;
        default:
            {
                if(!(static_cast<BattleFighter*>(bo))->isRevival())
                {
                    fRevival = true;
                    (static_cast<BattleFighter*>(bo))->setRevival();
                    //doSkillAttackAftEnter(static_cast<BattleFighter*>(bo));
                }
            }
            break;
        }

        if(fFakeDead || fRevival)
        {
            defList[defCount].pos = bo->getPos() + (activeFlag ? 25 : 0);
            defList[defCount].damType = e_skill;
            defList[defCount].damage = passiveSkill->getId();
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
            return false;
        }
    }

    if(!fFakeDead && !fRevival)
    {
        // remove from action queue
        BattleFighter* toremove = static_cast<BattleFighter *>(bo);
        removeFighterStatus(toremove);

        // re-test winner
        _winner = testWinner();
    }

    return true;
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
            if(0 != bf->getDeepForgetLast())
            {
                bf->setDeepForgetDmgExtra(0, 0);
                defList[defCount].damType = e_unDeepForget;
            }
            else
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
            if(0 != bf->getDeepStunLast())
            {
                bf->setDeepStunDmgExtra(0, 0);
                defList[defCount].damType = e_unDeepStun;
            }
            else
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
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAtk, 0, 0, scList, scCount, false);
    }

    UInt8& magAtkAdd_last = bf->getMagAttackAddLast();
    if(magAtkAdd_last > 0)
    {
        -- magAtkAdd_last;
       if(0 == magAtkAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagAtk, 0, 0, scList, scCount, false);
    }

    bool clearDefDec = false;
    UInt8& defAdd_last = bf->getDefendAddLast();
    if(defAdd_last > 0)
    {
        -- defAdd_last;
       if(0 == defAdd_last)
       {
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stDef, 0, 0, scList, scCount, false);
            float defdec = abs(bf->getDefDec());
            if(defdec > 0.001f)
            {
                bf->setDefDec(0, 0);
                bf->setDefDecTimes(0);
                clearDefDec = true;
            }
       }
    }

    UInt8& magDefAdd_last = bf->getMagDefendAddLast();
    if(magDefAdd_last > 0)
    {
        -- magDefAdd_last;
       if(0 == magDefAdd_last)
       {
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagDef, 0, 0, scList, scCount, false);
            float defdec = abs(bf->getDefDec());
            if(defdec > 0.001f)
            {
                bf->setDefDec(0, 0);
                bf->setDefDecTimes(0);
                clearDefDec = true;
            }
       }
    }

    if(clearDefDec)
    {
        defList[defCount].pos = bf->getPos() + 25;
        defList[defCount].damType = e_undefDec;
        defList[defCount].damage = bf->getDefDecTimes();
        defList[defCount].leftHP = bf->getHP();
        ++ defCount;
        bf->setDefDecTimes(0);
    }

    UInt8& hitrateAdd_last = bf->getHitrateAddLast();
    if(hitrateAdd_last > 0)
    {
        -- hitrateAdd_last;
       if(0 == hitrateAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stHitRate, 0, 0, scList, scCount, false);
    }

    UInt8& evadeAdd_last = bf->getEvadeAddLast();
    if(evadeAdd_last > 0)
    {
        -- evadeAdd_last;
       if(0 == evadeAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stEvade, 0, 0, scList, scCount, false);
    }

    UInt8& criticalAdd_last = bf->getCriticalAddLast();
    if(criticalAdd_last > 0)
    {
        -- criticalAdd_last;
       if(0 == criticalAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stCritical, 0, 0, scList, scCount, false);
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
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stPierce, 0, 0, scList, scCount, false);
    }

    UInt8& counterAdd_last = bf->getCounterAddLast();
    if(counterAdd_last > 0)
    {
        -- counterAdd_last;
       if(0 == counterAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stCounter, 0, 0, scList, scCount, false);
    }

    UInt8& magResAdd_last = bf->getMagResAddLast();
    if(magResAdd_last > 0)
    {
        -- magResAdd_last;
       if(0 == magResAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagRes, 0, 0, scList, scCount, false);
    }

    UInt8& toughAdd_last = bf->getToughAddLast();
    if(toughAdd_last > 0)
    {
        -- toughAdd_last;
       if(0 == toughAdd_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stTough, 0, 0, scList, scCount, false);
    }

    UInt8& atkreduce_last = bf->getAtkReduceLast();
    if(atkreduce_last > 0)
    {
        -- atkreduce_last;
       if(0 == atkreduce_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAtkReduce, 0, 0, scList, scCount, false);
    }

    UInt8& magatkreduce_last = bf->getMagAtkReduceLast();
    if(magatkreduce_last > 0)
    {
        -- magatkreduce_last;
       if(0 == magatkreduce_last)
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stMagAtkReduce, 0, 0, scList, scCount, false);
    }

    UInt8& evad100CD = bf->getEvad100CD();
    bool evad100 = bf->getEvad100();
    if(evad100CD > 0 && !evad100)
    {
        --evad100CD;
        if(0 == evad100CD)
        {
            bf->setEvad100(true);
            bf->setEvad100CD(bf->getEvad100BaseCD());
        }
    }

    UInt8& defend100CD = bf->getDefend100CD();
    bool defend100 = bf->getDefend100();
    if(defend100CD > 0 && !defend100)
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
            setStatusChange(bf, bf->getSide(), bf->getPos(), 1, 0, e_stAction, 0, 0, scList, scCount, false);
    }

    if(defCount > 0 || scCount > 0)
    {
        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 5, 0, false, false, defList, defCount, scList, scCount);
        ++ rcnt;
    }

    return rcnt;
}

void BattleSimulator::releaseWeak(BattleFighter* bo, DefStatus* defList, size_t& defCount)
{
	UInt32 weak = bo->getWeakRound();
	if(weak > 0)
	{
		-- weak;
	    if(weak == 0)
        {
            defList[defCount].damType = e_UnWeak;
            defList[defCount].damage = 0;
            defList[defCount].pos = bo->getPos() + 25;
            defList[defCount].leftHP = bo->getHP();
            defCount++;
        }

	    bo->setWeakRound(weak);
	}
}

bool BattleSimulator::isImmuneDecAura(const GData::SkillBase* skill, int target_side, int target_pos, DefStatus* defList, size_t& defCount)
{
    if(NULL == skill)
        return false;

    if(skill->effect == NULL)
        return false;

    BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
    if(NULL == bo)
        return false;

    bool fimm = false;
    if(skill->effect->state & 0x40)
    {
        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
        if(NULL == bo)
            return false;

        UInt8 immune = bo->getImmune();
        if((skill->effect->state & immune) && SKILL_LEVEL(skill->getId()) <= bo->getImmuneLevel(0x40))
        {
            fimm = true;
            defList[defCount].damType = e_Immune;
            defList[defCount].pos = target_pos;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
        }
    }

    return fimm;
}

void BattleSimulator::setStatusChange2_Atk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stAtk;
    bfgt->setAttackAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getAttack());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Def(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stDef;
    bfgt->setDefendAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getDefend());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Evade(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stEvade;
    bfgt->setEvadeAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getEvade(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Critical(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stCritical;
    bfgt->setCriticalAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getCritical(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Pierce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stPierce;
    bfgt->setPierceAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getPierce(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Counter(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stCounter;
    bfgt->setCounterAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getCounter(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Aura(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stAura;
    bfgt->AddAura(value);
    sc.data = static_cast<UInt32>(bfgt->getAura());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Tough(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stTough;
    bfgt->setToughAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getTough(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_MagAtk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stMagAtk;
    bfgt->setMagAttackAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getMagAttack());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_MagDef(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stMagDef;
    bfgt->setMagDefendAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getMagDefend());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_MagRes(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stMagRes;
    bfgt->setMagResAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getMagRes(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_Action(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stAction;
    bfgt->setActionAdd2(value);
    reQueueFighterStatus(bfgt);
    sc.data = static_cast<UInt32>(bfgt->getAction());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_CriDmg(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stCriticalDmg;
    bfgt->setCriticalDmgAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getCriticalDmg() * 100);

    ++ scCount;
}

void BattleSimulator::setStatusChange2_HitR(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stHitRate;
    bfgt->setHitrateAdd2(value);
    sc.data = static_cast<UInt32>(bfgt->getHitrate(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange2_AtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stAtkReduce;
    bfgt->setAtkReduce2(value);
    sc.data = static_cast<UInt32>(bfgt->getAtkReduce());

    ++ scCount;
}

void BattleSimulator::setStatusChange2_MagAtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    sc.statusId = skillId;
    sc.type = e_stMagAtkReduce;
    bfgt->setMagAtkReduce2(value);
    sc.data = static_cast<UInt32>(bfgt->getMagAtkReduce());

    ++ scCount;
}

void BattleSimulator::setStatusChange_Atk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stAtk;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setAttackAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getAttack());

    ++ scCount;
}

void BattleSimulator::setStatusChange_Def(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stDef;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setDefendAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getDefend());

    ++ scCount;
}

void BattleSimulator::setStatusChange_Evade(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stEvade;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setEvadeAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getEvade(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_Critical(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stCritical;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setCriticalAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getCritical(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_Pierce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stPierce;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setPierceAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getPierce(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_Counter(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stCounter;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setCounterAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getCounter(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_Aura(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stAura;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->AddAura(value);
    sc.data = static_cast<UInt32>(bfgt->getAura());

    ++ scCount;
}


void BattleSimulator::setStatusChange_Tough(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stTough;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setToughAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getTough(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_MagAtk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stMagAtk;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setMagAttackAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getMagAttack());

    ++ scCount;
}

void BattleSimulator::setStatusChange_MagDef(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stMagDef;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setMagDefendAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getMagDefend());

    ++ scCount;
}

void BattleSimulator::setStatusChange_MagRes(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stMagRes;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setMagResAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getMagRes(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_Action(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stAction;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setActionAdd(value, last);
    reQueueFighterStatus(bfgt);
    sc.data = static_cast<UInt32>(bfgt->getAction());

    ++ scCount;
}

void BattleSimulator::setStatusChange_CriDmg(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stCriticalDmg;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setCriticalDmgAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getCriticalDmg() * 100);

    ++ scCount;
}

void BattleSimulator::setStatusChange_HitR(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stHitRate;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setHitrateAdd(value, last);
    sc.data = static_cast<UInt32>(bfgt->getHitrate(NULL));

    ++ scCount;
}

void BattleSimulator::setStatusChange_AtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stAtkReduce;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setAtkReduce(value, last);
    sc.data = static_cast<UInt32>(bfgt->getAtkReduce());

    ++ scCount;
}

void BattleSimulator::setStatusChange_MagAtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active )
{
    BattleObject * bo = _objs[side][pos];
    BattleFighter * bfgt = static_cast<BattleFighter *>(bo);
    StatusChange& sc = scList[scCount];
    sc.pos = pos;
    if(!active)
        sc.pos += 25;
    if(skill)
        sc.statusId = skill->getId();
    else
        sc.statusId = 0;
    sc.type = e_stMagAtkReduce;
    if(skill && skill->cond == GData::SKILL_BEATKED && bf->getPos() + 25 != sc.pos)
        ++last;
    bfgt->setMagAtkReduce(value, last);
    sc.data = static_cast<UInt32>(bfgt->getMagAtkReduce());

    ++ scCount;
}


bool BattleSimulator::doSkillStrengthen_disperse(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(_objs[target_side][target_pos]);
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    if(_rnd(10000) < ef->value*100)
    {
        if(bo->getConfuseRound() != 0)
        {
            bo->setConfuseLevel(0);
            bo->setConfuseRound(0);

            defList[defCount].pos = bo->getPos() + pos0;
            defList[defCount].damType = e_UnConfuse;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
        }

        if(bo->getStunRound() != 0)
        {
            bo->setStunLevel(0);
            bo->setStunRound(0);

            if(bo->getDeepForgetLast() != 0)
            {
                bo->setDeepStunDmgExtra(0, 0);
                defList[defCount].damType = e_unDeepForget;
            }
            else
                defList[defCount].damType = e_UnForget;

            defList[defCount].pos = bo->getPos() + pos0;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
        }

        if(bo->getForgetRound() != 0)
        {
            bo->setForgetLevel(0);
            bo->setForgetRound(0);

            if(bo->getDeepForgetLast() != 0)
            {
                bo->setDeepForgetDmgExtra(0, 0);
                defList[defCount].damType = e_unDeepStun;
            }
            else
                defList[defCount].damType = e_UnStun;

            defList[defCount].pos = bo->getPos() + pos0;
            defList[defCount].damage = 0;
            defList[defCount].leftHP = bo->getHP();
            ++ defCount;
        }
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_reduce(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(_objs[target_side][target_pos]);
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    {
        StatusChange& sc = scList[scCount];
        sc.pos = target_pos + pos0;
        if(skill)
            sc.statusId = skill->getId();
        else
            sc.statusId = 0;

        sc.type = e_stAtkReduce;
        bo->setAtkReduce3(ef->value, ef->last);
        sc.data = static_cast<UInt32>(bo->getAtkReduce());
        ++scCount;
    }
    {
        StatusChange& sc = scList[scCount];
        sc.pos = target_pos + pos0;
        if(skill)
            sc.statusId = skill->getId();
        else
            sc.statusId = 0;

        sc.type = e_stMagAtkReduce;
        bo->setMagAtkReduce3(ef->value, ef->last);
        sc.data = static_cast<UInt32>(bo->getMagAtkReduce());
        ++scCount;
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_week(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(_objs[target_side][target_pos]);
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    if( ((float)bo->getHP()/bo->getMaxHP()) < ef->value/100)
    {
        bo->setImmune2(0xE);
        defList[defCount].pos = bo->getPos() + pos0;
        defList[defCount].damType = e_Immune2;
        defList[defCount].damage = 0;
        defList[defCount].leftHP = bo->getHP();
        ++ defCount;
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_debuf_defend(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(_objs[target_side][target_pos]);
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    int pos0 = 0;
    if(active && bf->getSide() == target_side)
        pos0 = 25;
    else if(!active && bf->getSide() != target_side)
        pos0 = 25;
    {
        bo->setDefDec(ef->value, ef->last);
        bf->setDefDecTimes(0);
        defList[defCount].pos = bo->getPos() + pos0;
        defList[defCount].damType = e_defDec;
        defList[defCount].damage = 0;
        defList[defCount].leftHP = bo->getHP();
        ++ defCount;
    }

    return true;
}

bool BattleSimulator::doSkillStrengthen_atkadd(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active)
{
    BattleFighter* bo = static_cast<BattleFighter *>(_objs[target_side][target_pos]);
    if(!bo || !bf || !ef || !skill || bo->getHP() == 0)
        return false;

    bool flag = bf->getSide() == target_side ? !active : active;
    UInt16 last = ef->last + (bf == bo ? 1 : 0);
    float atkadd = bo->_attack * ef->value/100;
    setStatusChange_Atk(bf, target_side, target_pos, skill, atkadd, last, scList, scCount, flag );
    float magatkadd = bo->_magatk * ef->value/100;
    setStatusChange_MagAtk(bf, target_side, target_pos, skill, magatkadd, last, scList, scCount, flag );

    return true;
}



bool BattleSimulator::doSkillStrengthenAttack(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active)
{
    if(!bf || !ef || !skill)
        return false;
    if((this->skillStrengthenTable[ef->type]) == NULL)
        return false;

    GData::Area* area = NULL;
    area = &(GData::areaList[ef->area]);

    AttackPoint ap[25];
    int apcnt = 0;

    if(ef->area != 0 && ef->area != 1 && area->getCount() > 0)
    {
        int x_ = target_pos % 5;
        int y_ = target_pos / 5;
        int cnt = area->getCount();
        int fsize = ef->factor.size();
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
            int idx = std::min(abs(fsize-1), std::max(abs(ad.y), abs(ad.x)));
            if(fsize != 0)
                ap[apcnt ++].factor = ef->factor[idx];
            else
                ap[apcnt ++].factor = 1;
        }
    }

    if(1 == skill->area)
    {
        for(UInt8 pos = 0; pos < 25; ++ pos)
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, pos, defList, defCount, scList, scCount, active);
        }
    }
    else if( 0 == skill->area )
    {
        (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, target_pos, defList, defCount, scList, scCount, active);
    }
    else
    {
        BattleFighter* bo = static_cast<BattleFighter*>(_objs[target_side][target_pos]);
        if(bo != NULL && bo->getHP() != 0 && bo->isChar())
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, target_pos, defList, defCount, scList, scCount, active);
        }

        for(int i = 0; i < apcnt; ++ i)
        {
            (this->*skillStrengthenTable[ef->type])(bf, skill, ef, target_side, ap[i].pos, defList, defCount, scList, scCount, active);
        }

    }

    return true;
}

bool BattleSimulator::doDeBufAttack(BattleFighter* bf)
{
    UInt32 rcnt = 0;
    if(NULL == bf)
        return 0;

    DefStatus defList[25];
    size_t defCount = 0;
    StatusChange scList[25];
    size_t scCount = 0;

    memset(defList, 0, sizeof(defList));

    UInt8& last1 = bf->getBleed1Last();
    if(last1 != 0)
    {
        UInt32 dmg = static_cast<UInt32>(bf->getBleed1());
        bf->makeDamage(dmg);
        defList[defCount].damage = dmg;
        defList[defCount].pos = bf->getPos() + 25;
        defList[defCount].leftHP = bf->getHP();
        -- last1;
        if(last1 == 0)
        {
            bf->setBleed1(0, 0);
            defList[defCount].damType = e_unBleed1;
        }
        else
            defList[defCount].damType = e_Bleed1;

        ++ defCount;

        if(bf->getHP() == 0)
        {
            onDead(true, bf, defList, defCount);
        }

    }

    UInt8& last2 = bf->getBleed2Last();
    if(last2 != 0)
    {
        UInt32 dmg = static_cast<UInt32>(bf->getBleed2());
        bf->makeDamage(dmg);
        defList[defCount].damage = dmg;
        defList[defCount].pos = bf->getPos() + 25;
        defList[defCount].leftHP = bf->getHP();
        -- last2;
        if(last2 == 0)
        {
            bf->setBleed2(0, 0);
            defList[defCount].damType = e_unBleed2;
        }
        else
            defList[defCount].damType = e_Bleed2;

        ++ defCount;

        if(bf->getHP() == 0)
        {
            onDead(true, bf, defList, defCount);
        }
    }

    UInt8& last3 = bf->getBleed3Last();
    if(last3 != 0)
    {
        UInt32 dmg = static_cast<UInt32>(bf->getBleed3());
        bf->makeDamage(dmg);
        defList[defCount].damage = dmg;
        defList[defCount].pos = bf->getPos() + 25;
        defList[defCount].leftHP = bf->getHP();
        -- last3;
        if(last3 == 0)
        {
            bf->setBleed3(0, 0);
            defList[defCount].damType = e_unBleed3;
        }
        else
            defList[defCount].damType = e_Bleed3;

        ++ defCount;

        if(bf->getHP() == 0)
        {
            onDead(true, bf, defList, defCount);
        }
    }


    if(defCount > 0 || scCount > 0)
    {
        appendToPacket(bf->getSide(), bf->getPos(), bf->getPos() + 25, 5, 0, false, false, defList, defCount, scList, scCount);
        ++ rcnt;
    }

    return rcnt;
}

float BattleSimulator::calcTherapyDebuf(BattleFighter* bo, DefStatus* defList, size_t& defCount)
{
    float factor = 1.0f;
    UInt8& last = bo->getTherapyDecLast();
    if(last > 0)
    {
        factor *= (1.0f - bo->getTherapyDec());
        -- last;
        if(last == 0)
            bo->setTherapyDec(0, 0);
        defList[defCount].damType = e_unneishan;
        defList[defCount].damage = 0;
        defList[defCount].pos = bo->getPos() + 25;
        defList[defCount].leftHP = bo->getHP();
        defCount ++;
    }

    return factor;
}


}
