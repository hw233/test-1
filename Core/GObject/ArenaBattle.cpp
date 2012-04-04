
#include "ArenaBattle.h"
#include "Player.h"
#include "Common/Random.h"

namespace GObject
{

ArenaBattle::ArenaBattle()
    : m_levelMan(0), m_progress(0)
{
}

ArenaBattle::~ArenaBattle()
{
}

void ArenaBattle::addLevelMan(bool init)
{
    ++m_levelMan;
    if (m_levelMan >= 32 && !init)
    {
        // notify
    }
}

} // namespace GObject

