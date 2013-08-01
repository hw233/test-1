#include "MoFang.h"
#include "Player.h"
#include "Item.h"
#include "GData/AttrExtra.h"

namespace GObject
{

MoFang::MoFang(Player* pl)
{
    m_owner = pl;
    memset(m_jg, 0, sizeof(m_jg));
}

void MoFang::Init()
{
}

void MoFang::addLearnedPaper(UInt8 paperId)
{
}

void MoFang::addLearningPaper(UInt8 paperId, UInt8 value)
{
}

void MoFang::EquipJg(ItemJiguan* jg, UInt8 idx)
{
}

void MoFang::analysisItem(std::vector<UInt32>& items)
{
}

void MoFang::makeJiguan(UInt8 paperId)
{
}

void MoFang::learnPaper(UInt8 paperId, UInt8 value)
{
}

void MoFang::addAttr(GData::AttrExtra& ae)
{
}

}
