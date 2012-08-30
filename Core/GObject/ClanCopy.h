#ifndef _CLAN_COPY_H_
#define _CLAN_COPY_H_

#include "Config.h"


namespace Battle
{
    class BattleSimulator;
}

namespace GData
{
    class NpcGroup;
}

namespace GObject
{

#define  CLAN_COPY_LOCATION  0x3202

class Player;
class Clan;

enum Copy_Spot
{
    Home = 0x01,
    Route1_Spot1 = 0x11,
    Route2_Spot1 = 0x21,
    Route3_Spot1 = 0x31,
    Enemy_Base = 0xff,
};

enum CLAN_COPY_STATUS
{
    CLAN_COPY_READY   = 0x01,
    CLAN_COPY_PROCESS = 0x02,
    CLAN_COPY_WIN     = 0x03,
    CLAN_COPY_LOSE    = 0x04,
};

struct ClanCopyPlayer
{
	ClanCopyPlayer(Player * player, UInt8 spSkillType = 0)
		: player(player), spSkillType(spSkillType)
	{
        formalBattleRound = 0;
	}
	Player * player;
    UInt8 spSkillType;
    UInt8 formalBattleRound;
};

struct ClanCopyMonster
{
    // 帮派副本怪物信息结构
    UInt32 npcId;
    UInt16 waveCount;       // 第几波产生的怪物
    UInt16 npcValue;
    bool   hasMoved;        // 是否已经移动
};

struct ClanCopySpot
{
    // 帮派副本的据点信息结构
    UInt8 spotId;
    UInt8 nextSpotId;       // 该据点之后的据点

};

class ClanCopy
{
    typedef std::map<UInt8, ClanCopySpot> SpotList;
    typedef std::multimap<UInt8, ClanCopyMonster> CopyMonster; // 该据点的怪物
    typedef std::multimap<UInt8, ClanCopyPlayer> SpotPlayer;   // 该据点的玩家
    typedef std::multimap<UInt8, ClanCopyPlayer> DeadPlayer;   // 该据点死亡的玩家（逃跑也算）

    public:
        ClanCopy(Clan *c); 
        ~ClanCopy();

        void process();

        void createEnemy(UInt32 round);
        void roundMove(UInt32 round);
        void roundCombat(UInt8 round);
        void attackHome(const ClanCopyMonster& clanCopyMonster);
        void lose();
        void win();
        void setInterval(UInt32 interval);
        void setStartTick(UInt32 tickCount);

    private:
        Clan    * _clan;
        UInt16 _copyLevel;
        CopyMonster _copyMonster;
        SpotPlayer _spotPlayer;
        SpotList _spotList;
        UInt32 _copyProcessTime;
        UInt32 _homeHp;
        DeadPlayer _deadPlayer;
        
};

}



#endif
