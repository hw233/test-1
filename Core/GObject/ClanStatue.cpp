#include "Config.h"

#include "Common/Stream.h"
#include "Player.h"
#include "Clan.h"

#include "ClanStatue.h"

namespace GObject
{

ClanStatue::ClanStatue(Clan *c) : _level(0), _clan(c)
{
}

ClanStatue::~ClanStatue()
{
}

}

