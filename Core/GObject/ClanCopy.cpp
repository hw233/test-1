#include "ClanCopy.h"
#include "Player.h"
#include "Common/Stream.h"

namespace GObject
{

const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目


ClanCopy::ClanCopy(Clan *c) : _clan(c), _copyLevel(0)
{
}


ClanCopy::~ClanCopy()
{
}

void ClanCopy::createEnemy(UInt32 round)
{
    // TODO: 敌人老巢产生敌人
}

void ClanCopy::enemyMove(UInt32 round)
{
    // TODO: 敌人移动
}

void ClanCopy::spotCombat(UInt8 spot, UInt32 round)
{
    // TODO: 该据点战斗
}

}
