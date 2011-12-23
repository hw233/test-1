#include "HoneyFall.h"
#include "Country.h"


namespace GObject
{

HoneyFall::HoneyFall(Player* pl)
{
    m_Owner = pl;
    memset(m_hft_value, 0, sizeof(m_hft_value));
}

HoneyFall::~HoneyFall()
{
}

UInt32 HoneyFall::getHftValue(HoneyFallType hft)
{
    if(!checkHft(hft))
        return 0;

    return m_hft_value[hft];
}

void HoneyFall::setHftValue(UInt8 hft, UInt32 value)
{
    if(!checkHft(hft))
        return;

    m_hft_value[hft] = value;
}

UInt32 HoneyFall::incHftValue(HoneyFallType hft, UInt32 inc)
{
    if(!checkHft(hft))
        return 0;

    m_hft_value[hft] += inc;

    return m_hft_value[hft];
}

void HoneyFall::updateHftValueToDB(HoneyFallType hft)
{
    DB3().PushUpdateData("REPLACE INTO `player_honeyfall` (`playerId`, `type`, `value`) VALUES (%"I64_FMT"u, %u, %u)", m_Owner->getId(), hft, m_hft_value[hft]);
}

UInt32 HoneyFall::getChanceFromHft(UInt8 q, UInt8 lv, HoneyFallType hft)
{
    UInt32 v = getHftValue(hft);

    return GObjectManager::getChanceFromHft(q, lv, v);
}

bool HoneyFall::checkHft(UInt8 hft)
{
    if(hft == e_HFT_None || e_HFT_Max <= hft)
        return false;

    return true;
}

}

