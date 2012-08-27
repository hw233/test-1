#include "ClanCopy.h"
#include "Player.h"
#include "Common/Stream.h"

namespace GObject
{

const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目

enum Copy_Spot
{
    Home = 0x01,
    Route1_Spot1 = 0x11,
    Route2_Spot1 = 0x21,
    Route3_Spot1 = 0x31,
    Enemy_Base = 0xff,
};

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

}
