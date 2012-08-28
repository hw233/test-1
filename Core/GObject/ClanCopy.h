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

struct ClanCopyPlayer
{
	ClanCopyPlayer(Player * player_ = NULL, UInt16 hold_ = 0, UInt8 status_ = 1)
		: player(player_), hold(hold_), status(status_)
	{
		id = 0;
		reliveNum = 0;
		offTime = 0;
		hasEnter = 0;
		hasAttack = 0;
	}
	Player * player;
	UInt32 id;
	UInt16 hold;
	UInt8  status;	
	UInt16 reliveNum;
	UInt32 offTime;		
	UInt8  hasEnter;
	UInt8  hasAttack;
};

// 帮派副本怪物信息结构
struct ClanCopyMonster
{
    UInt32 npcId;
    UInt8 currentSpot;
    UInt8 nextSpot;
};

class ClanCopy
{
    typedef std::map<UInt8, ClanCopyMonster> SpotMonster; // 该据点的怪物
    typedef std::map<UInt8, ClanCopyPlayer> SpotPlayer;   // 该据点的玩家

    public:
        ClanCopy(Clan *c); 
        ~ClanCopy();

        void createEnemy(UInt32 round);
        void enemyMove(UInt32 round);
        void spotCombat(UInt8 spot, UInt32 round);

    private:
        Clan    * _clan;
        UInt16 _copyLevel;
        SpotMonster _spotMonster;
        SpotPlayer _spotPlayer;
        
};

}



#endif
