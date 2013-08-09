#include "MoFang.h"
#include "Player.h"
#include "Item.h"
#include "GData/AttrExtra.h"

namespace GObject
{

MoFang::MoFang(Player* pl)
{
    m_owner = pl;

    memset(m_grids, 0, sizeof(m_grids));
}

void MoFang::Init()
{
}

void makejiguan(UInt32 itemId, UInt8 type, UInt8 mark)
{

}

void upgradeJGS()
{

}

void equipJG(UInt32 jgId, UInt8 pos, UInt8 mark)
{

}

void dismantleJG(UInt8 pos, UInt8 mark)
{

}

void addJGYAttr(GData::AttrExtra& ae)
{

}

UInt16 useJGQskill()
{
    return 0;
}

void sendMoFangInfo(UInt8 mark)
{

}

bool checkEquip(UInt32 jgId)
{
    return true;
}

}
