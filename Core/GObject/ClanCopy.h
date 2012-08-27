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

struct ClanCopyCheckData
{
    ClanCopyCheckData(UInt8 t = 0, UInt32 id = 0, UInt16 minF = 0, UInt16 maxF = 0)
        : type(t), objectId(id), minFloor(minF), maxFloor(maxF)
    {
    }

    UInt8 type;    // 格子类型：普通怪，精英怪，机关，Boss
    UInt32 objectId; // 格子的id（机关种类或者怪物id）
    UInt16 minFloor; // 最低出现层数
    UInt16 maxFloor; // 最高出现层数
};

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

struct ClanCopyMonster
{
    UInt32 npcId;
    UInt32 currentSpot;
    UInt32 nextSpot;
};

class ClanCopy
{
    typedef std::map<UInt8, ClanCopyMonster> spotMonster;
    public:
        ClanCopy(Clan *c); 
        ~ClanCopy();

        void createEnemy(UInt32 round);

    private:
        Clan    * _clan;
        UInt16 _copyLevel;
};

}



#endif
