#include "Config.h"
#include "LuckyDraw.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "Item.h"
#include "Script/GameActionLua.h"
#include "Common/URandom.h"
#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "MsgID.h"

namespace GObject
{

void LuckyDraw::sendInfo(Player* player)
{
    FastMutex::ScopedLock lock(_lock);
    
    Stream st(REP::LUCKYDRAW);
    st << static_cast<UInt8>(1);
    st << Stream::eos;
    player->send(st);
}

void LuckyDraw::draw(Player* player)
{
    FastMutex::ScopedLock lock(_lock);

    Stream st(REP::LUCKYDRAW);
    st << static_cast<UInt8>(0);
    st << Stream::eos;
    player->send(st);
}

LuckyDraw luckyDraw;

}
